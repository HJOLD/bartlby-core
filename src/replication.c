/* $Id$ */
/* ----------------------------------------------------------------------- *
 *
 *   Copyright 2005 Helmut Januschka - All Rights Reserved
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, Inc., 675 Mass Ave, Cambridge MA 02139,
 *   USA; either version 2 of the License, or (at your option) any later
 *   version; incorporated herein by reference.
 *
 * ----------------------------------------------------------------------- */
/*
$Revision$
$Source$


$Log$
Revision 1.3  2005/09/18 05:03:52  hjanuschka
replication is false by default now
need to fix the damn write()/read() -> while() sh**

Revision 1.2  2005/09/18 04:04:52  hjanuschka
replication interface (currently just a try out)
one instance can now replicate itself to another using portier as a transport way
FIXME: need to sort out a binary write() problem

Revision 1.1  2005/09/18 01:33:54  hjanuschka
*** empty log message ***


*/

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>  
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <bartlby.h>

static int connection_timed_out=0;


static void bartlby_conn_timeout(int signo) {
 	connection_timed_out = 1;
}


int connect_to(char * host, int port) {
	int client_socket;
	int client_connect_retval=-1;
	struct sockaddr_in remote_side;
	struct hostent * remote_host;
	struct sigaction act1, oact1;
	
	
	connection_timed_out=0;
	
	if((remote_host = gethostbyname(host)) == 0) {
		
		return -1; //timeout
	}
	memset(&remote_side, '\0', sizeof(remote_side));
	remote_side.sin_family=AF_INET;
	remote_side.sin_addr.s_addr = htonl(INADDR_ANY);
	remote_side.sin_addr.s_addr = ((struct in_addr *) (remote_host->h_addr))->s_addr;
	remote_side.sin_port=htons(port);
	
	if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			
		
		return -2; //Socket
			
			
	}
	act1.sa_handler = bartlby_conn_timeout;
	sigemptyset(&act1.sa_mask);
	act1.sa_flags=0;
	#ifdef SA_INTERRUPT
	act1.sa_flags |= SA_INTERRUPT;
	#endif
	
	if(sigaction(SIGALRM, &act1, &oact1) < 0) {
		
		return -3; //timeout handler
	
		
	}
	alarm(5);
	client_connect_retval = connect(client_socket, (void *) &remote_side, sizeof(remote_side));
	alarm(0);
	
	if(connection_timed_out == 1 || client_connect_retval == -1) {
		return -4; //connect
	} 
	connection_timed_out=0;
	
	return client_socket; 	
}


int replicate_single(char * hostname, void * shm_addr, char * cfgfile) {
	struct shm_header * hdr;
	long SHMSize;
	int res;
	char verstr[2048];
	int read_cnt;
	char cmdstr[2048];
	char * cfg_shm_size;
	long cfg_shm_size_bytes;
	
	cfg_shm_size = getConfigValue("shm_size", cfgfile);
	if(cfg_shm_size==NULL) {
		cfg_shm_size_bytes=10;		
	} else {
		cfg_shm_size_bytes=atol(cfg_shm_size);
	}
	
	free(cfg_shm_size);
	
	SHMSize=cfg_shm_size_bytes*1024*1024;	
	
	//SHMSize=20000;
	
	hdr=bartlby_SHM_GetHDR(shm_addr);
	
	_log("\tReplicate to: '%s'", hostname);	
	
	res=connect_to(hostname, 9031);
	if(res > 0) {
		
		connection_timed_out=0;
		alarm(5);
		if((read_cnt=read(res, verstr, 1024)) < 0) {
			_log("repl: Cant get version str");
			return -1;
		}
		verstr[read_cnt-1]='\0';
		if(verstr[0] != '+') {
			_log("\tServer said a bad result: '%s'\n", verstr);
			close(res);
			return -2;
		}
		alarm(0);
		
		_log("\t%s", verstr);
		
		sprintf(cmdstr, "3|%ld|", SHMSize);
		alarm(5);
		if(write(res, cmdstr, strlen(cmdstr)) < 0) {
			_log("\twrite timed out");
			close(res);
			return -3;
		}
		alarm(0);
		connection_timed_out=0;
		alarm(5);
		if((read_cnt=read(res, verstr, 1024)) < 0) {
			_log("repl: Cant get reply");
			return -1;
		}
		verstr[read_cnt-1]='\0';
		
		if(verstr[0] != '+') {
			_log("\tServer said a bad result: '%s'\n", verstr);
			close(res);
			return -4;
		}
		alarm(0);
		
		//_log("Result: %s", verstr);
		
		alarm(5);
		if((read_cnt=send(res, shm_addr, SHMSize,0)) < 0) {
			_log("\twrite timed out");
			close(res);
			return -3;
		}
		_log("\t wrote %d/%d bytes", SHMSize, read_cnt);
		
		alarm(0);
		connection_timed_out=0;
		alarm(5);
		if((read_cnt=read(res, verstr, 1024)) < 0) {
			_log("repl: Cant get reply");
			return -1;
		}
		verstr[read_cnt-1]='\0';
		
		if(verstr[0] != '+') {
			_log("\tServer said a bad result: '%s'\n", verstr);
			close(res);
			return -4;
		}
		alarm(0);
		_log("\tServer said: %s", verstr);
		
		close(res);
	}
	
	
		
	return 1;
}

int replication_go(char * cfgfile, void * shm_addr, void * SOHandle) {
	//_log("FIXME: replication");
	
	char * replicate_true;
	char * replicate_counts;
	int replicate_count_int;
	char * replication_find;
	char * replication_host;
	struct shm_header * hdr;
	
	char * replication_intervall;
	int replication_intervall_int;
	
	int curr_time;
	int time_diff;
	int repl_rtc;
	
	int x;
	
	hdr=bartlby_SHM_GetHDR(shm_addr);
	replicate_true=getConfigValue("replication", cfgfile);
	replicate_counts=getConfigValue("replicate_cnt", cfgfile);
	replication_intervall=getConfigValue("replication_intervall", cfgfile);
	
	if(replicate_true == NULL) {
		replicate_true=strdup("false");	
	}	
	
	
	if(replicate_counts == NULL) {
		replicate_counts=strdup("0");
	}	
	if(replication_intervall == NULL) {
		replication_intervall=strdup("10800");	
	}
	
	if(strcmp(replicate_true, "true") == 0) {
		curr_time=time(NULL);
		time_diff=curr_time-hdr->last_replication;
		replication_intervall_int=atoi(replication_intervall);
		if(time_diff >= replication_intervall_int) {
			//We do replicate	
			replicate_count_int=atoi(replicate_counts);
			_log("start replication on %d Hosts Last Replicate was on '%d'", replicate_count_int, hdr->last_replication);
			for(x=1; x<=replicate_count_int; x++) {
			
				replication_find=malloc((sizeof(char)*12)+255);
				sprintf(replication_find, "replicate[%d]", x);
				replication_host=getConfigValue(replication_find, cfgfile);
				if(replication_host == NULL) {
					_log("\tHost %d not set in cfg file", x);	
				} else {
					repl_rtc=replicate_single(replication_host, shm_addr,cfgfile);	
					if(repl_rtc >= 0) {
						_log("\tReplication OK");	
					} else {
						_log("\tReplication FAILED");
					}
				}
			
			
			
			
				free(replication_find);
				free(replication_host);
				hdr->last_replication=time(NULL);
			}
		} 
	}
	
	
	free(replication_intervall);
	free(replicate_true);
	free(replicate_counts);
	return 1;
}
