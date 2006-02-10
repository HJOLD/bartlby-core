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
Revision 1.33  2006/02/10 23:54:46  hjanuschka
SIRENE mode added

Revision 1.32  2006/02/09 00:14:50  hjanuschka
datalib: mysql/ catch failed logins
core: fixed some setuid problems with datalib
core: zero worker detected and logged
core: network code re-worked, much faster and cleaner now
core: encode/decode removed
php: encode/decode removed
ui: topology map manager added
ui: nicer menu (flap)
ui: server_detail (added)
startup sh: pre-start check if logfile is writeable

Revision 1.31  2006/01/16 20:51:41  hjanuschka
performance stuff moved to perf.c
timeing information on perf handler

Revision 1.30  2006/01/09 23:53:10  hjanuschka
minor changes

Revision 1.29  2006/01/08 16:17:24  hjanuschka
mysql shema^

Revision 1.28  2005/12/31 00:29:44  hjanuschka
some more perf fixes during high load test

Revision 1.27  2005/12/27 22:00:14  hjanuschka
*** empty log message ***

Revision 1.26  2005/12/25 23:01:16  hjanuschka
stress testing with RRD
perf fixes

Revision 1.25  2005/12/25 12:55:45  hjanuschka
service_check_timeout is dynamic now

Revision 1.24  2005/12/25 00:38:04  hjanuschka
perf_trigger: BARTLBY_CONFIG env set
              sample (bartlby_load) RRD+graph sample performance trigger

Revision 1.23  2005/12/25 00:30:08  hjanuschka
perf trigger: envs, BARTLBY_CURR_HOST, BARTLBY_CURR_SERVICE, BARTLBY_CURR_PLUGIN set right before trigger is executed

Revision 1.22  2005/12/24 17:53:41  hjanuschka
performance interface i.e: for adding RRD tools or something like that

Revision 1.21  2005/11/27 02:04:42  hjanuschka
setuid/setgid for security and web ui

Revision 1.20  2005/11/16 23:51:29  hjanuschka
version bump 0.9.9a (Exusiai)
replication tests minor fixes

Revision 1.19  2005/10/25 20:36:32  hjanuschka
startup time is'nt reset on cfg reload now

Revision 1.18  2005/10/13 22:13:14  hjanuschka
logging improved, check fixup

Revision 1.17  2005/10/09 14:44:09  hjanuschka
agent announces OS and version

Revision 1.16  2005/09/28 21:46:30  hjanuschka
converted files to unix
jabber.sh -> disabled core dumps -> jabblibs segfaults
                                    will try to patch it later

Revision 1.15  2005/09/25 13:30:18  hjanuschka
cfg: jabber variables
daemon: setenv BARTLBY_HOME (for triggers)
sched: wait_open timeout
mail.sh: sendmail trigger
trigger: $1 == email
$2 == icq
$3 == name
$4 == msg

Revision 1.14  2005/09/24 10:34:11  hjanuschka
deadlock sched_wait_open fixed

Revision 1.13  2005/09/23 18:21:18  hjanuschka
if check times out curren_running gets in negative integer fixed
default check timeout 15 seconds

Revision 1.12  2005/09/22 02:55:03  hjanuschka
agent: def timeout 15
check: strreplace ' "

Revision 1.11  2005/09/14 22:01:41  hjanuschka
debug in data_lib added and removed
agent: off by two :-) *fG* malloc error producing magic char's  (fixed)

Revision 1.10  2005/09/11 21:42:24  hjanuschka
log files are now archived by Y.M.d

Revision 1.9  2005/09/11 09:20:58  hjanuschka
logging issue ;-)
ui now can display log in a nice layout ;-)

Revision 1.8  2005/09/07 22:36:56  hjanuschka
portier: added err code -4 svc not found
check: group check fixed , runnaway strtok :-)

Revision 1.7  2005/09/07 21:51:40  hjanuschka
fixed passive check_fin bug
added bartlby_portier passive results may now be deliverd from remote

Revision 1.6  2005/09/05 19:53:12  hjanuschka
2 day uptime without a single sigsegv ;-)
added daemon function ;-)
	new cfg vars daemon=[true|false], basedir, logfile

Revision 1.5  2005/09/03 20:11:22  hjanuschka
fixups

added addworker, deleteworker, modifyworker, getworkerbyid

Revision 1.4  2005/08/28 16:02:59  hjanuschka
CVS Header


*/
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

#include <bartlby.h>




#define PASSIVE_TIMEOUT "Passive Service has been timed out"
#define DNS_ERROR "DNS lookup error"
#define SOCKET_CREATE_ERROR "Socket create error"
#define ALARM_ERROR "Alarm setup error"
#define CONN_ERROR "Connection error"
#define RECV_ERROR "Recieve Error"
#define PROTOCOL_ERROR "Protocol Error"
#define TIMEOUT_ERROR "Recv() timedout"

#define CONN_TIMEOUT 15



#define GROUP_CRITICAL "Group check critical"
#define GROUP_WITHOUT_PARMS "Group check without parameters"
#define GROUP_OK "Group check OK"

static int connection_timed_out=0;




static void bartlby_conn_timeout(int signo) {
 	connection_timed_out = 1;
}



void bartlby_check_active(struct service * svc, char * cfgfile) {
	int client_socket;
	int client_connect_retval=-1;
	int return_bytes;
	
	
	char return_buffer[1024];
	char * client_request;
	
	
	
	
	
	
	struct sockaddr_in remote_side;
	struct hostent * remote_host;
	struct sigaction act1, oact1;
	
	/*
	NEW
	*/
	char * rmessage;
	int sum_rmessage;
	int char_idx=0, cur_char_idx=0;
	
	char * curr_line;
	
	
	
	
	
	connection_timed_out=0;
	
	if((remote_host = gethostbyname(svc->client_ip)) == 0) {
		
		sprintf(svc->new_server_text, "%s", DNS_ERROR);
		svc->current_state=STATE_CRITICAL;
		
		return;
	}
	memset(&remote_side, '\0', sizeof(remote_side));
	remote_side.sin_family=AF_INET;
	remote_side.sin_addr.s_addr = htonl(INADDR_ANY);
	remote_side.sin_addr.s_addr = ((struct in_addr *) (remote_host->h_addr))->s_addr;
	remote_side.sin_port=htons(svc->client_port);
	
	if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			
		
		sprintf(svc->new_server_text, "%s", SOCKET_CREATE_ERROR);
		svc->current_state=STATE_CRITICAL;
				
		return;
			
			
	}
	act1.sa_handler = bartlby_conn_timeout;
	sigemptyset(&act1.sa_mask);
	act1.sa_flags=0;
	#ifdef SA_INTERRUPT
	act1.sa_flags |= SA_INTERRUPT;
	#endif
	
	if(sigaction(SIGALRM, &act1, &oact1) < 0) {
		
		sprintf(svc->new_server_text, "%s", ALARM_ERROR);
		svc->current_state=STATE_CRITICAL;
				
		return;
	
		
	}
	alarm(svc->service_check_timeout);
	client_connect_retval = connect(client_socket, (void *) &remote_side, sizeof(remote_side));
	alarm(0);
	
	if(connection_timed_out == 1 || client_connect_retval == -1) {
		sprintf(svc->new_server_text, "%s", CONN_ERROR);
		svc->current_state=STATE_CRITICAL;
		close(client_socket);
		return;
	} 
	
	
	/*
		SEND Request
	*/
	connection_timed_out=0;
	client_request=malloc(sizeof(char)*(strlen(svc->plugin)+strlen(svc->plugin_arguments)+30));
	sprintf(client_request, "%s| %s|", svc->plugin, svc->plugin_arguments);
	
	//Encode it
	bartlby_encode(client_request, strlen(client_request));
	
	//Send it
	alarm(svc->service_check_timeout);
	send(client_socket, client_request, (strlen(svc->plugin)+strlen(svc->plugin_arguments)+3),0);
	//Check it
	if(connection_timed_out == 1) {
		
		
		sprintf(svc->new_server_text, "%s", CONN_ERROR);
		svc->current_state=STATE_CRITICAL;
		free(client_request);
		return;
	}
	
	free(client_request);
	
	//Recieve Reply
	rmessage=malloc(sizeof(char)*(1024*4));
	memset(rmessage, '\0', sizeof(char)*(1024*4));
	alarm(svc->service_check_timeout);
	connection_timed_out=0;
	while((return_bytes=recv(client_socket, return_buffer, 1024, 0)) > 0 &&  sum_rmessage < 1024*4 && connection_timed_out == 0) {
		return_buffer[return_bytes] = '\0';
		strcat(rmessage, return_buffer);		
		sum_rmessage += return_bytes;
	}
	
	//_log("ALL: '%s'", rmessage);
	
	//Check if timed out or Receive error
	if(connection_timed_out == 1 || return_bytes < 0) {
		
		if(connection_timed_out == 1) {
			sprintf(svc->new_server_text, "%s", TIMEOUT_ERROR);
		} else {
			sprintf(svc->new_server_text, "%s", RECV_ERROR);
		}
		svc->current_state=STATE_CRITICAL;
		free(rmessage);
		return;
	}
	alarm(0);
	close(client_socket);
	
	
	
	
	cur_char_idx=0;
	curr_line=malloc(sizeof(char) * (strlen(rmessage)+20));
   	while(char_idx < strlen(rmessage)) {
   		//_log("%c", rmessage[cur_char_idx]);
   		
   		curr_line[cur_char_idx]=rmessage[char_idx];
   		if(rmessage[char_idx] == '\n' || char_idx == strlen(rmessage)) {
   			curr_line[cur_char_idx]='\0';
   			bartlby_action_handle_reply_line(svc, curr_line, cfgfile);
   			cur_char_idx=0;	
   			char_idx++;
   			continue;	
   		}
   		cur_char_idx++;
   		char_idx++;
   	}
	free(curr_line);
	free(rmessage);
	
        return;
		
}
void bartlby_action_handle_reply_line(struct service * svc, char * line, char * cfgfile) {
	char * return_token;
	if(strlen(line) == 0) {
		return;	
	}
	if(strncmp(line, "OS:", 3) == 0) {
		//_log("OS: '%s'", line);
		return;	
	}	
	if(strncmp(line, "PERF: ", 6) == 0) {
		//_log("PERF: '%s'", line);
		bartlby_perf_track(svc,line, strlen(line), cfgfile);	
		return;
	}
	//_log("DATA: '%s'", line);
	return_token = strtok(line, "|");
        if(return_token != NULL) {
        	//Verfiy result code to be 0-2 :-) 
        	if(return_token[0] != '0' && return_token[0] != '1' && return_token[0] != '2') {
        		svc->current_state=STATE_UNKOWN;	
        	} else {
        		svc->current_state=atoi(return_token);
        	}
        	
        	return_token = strtok(NULL, "|");
        	if(return_token != NULL) {
        		sprintf(svc->new_server_text, "%s", return_token);
        	} else {
        		
        		sprintf(svc->new_server_text, "(empty output)");
        		
        	}	
        	return;
        } else {
        	
        	sprintf(svc->new_server_text, PROTOCOL_ERROR);
        	svc->current_state=STATE_CRITICAL;
        	return;
        }
	
	
	
}

/* 
void bartlby_check_active_old(struct service * svc, char * cfgfile) {
	int client_socket;
	int client_connect_retval=-1;
	int return_bytes;
	
	
	char return_buffer[1024];
	char * client_request;
	char * return_token;
	
	
	
	char return_delimeter[]="|";
	
	struct sockaddr_in remote_side;
	struct hostent * remote_host;
	struct sigaction act1, oact1;
	
	
	
	connection_timed_out=0;
	
	if((remote_host = gethostbyname(svc->client_ip)) == 0) {
		
		sprintf(svc->new_server_text, "%s", DNS_ERROR);
		svc->current_state=STATE_CRITICAL;
		
		return;
	}
	memset(&remote_side, '\0', sizeof(remote_side));
	remote_side.sin_family=AF_INET;
	remote_side.sin_addr.s_addr = htonl(INADDR_ANY);
	remote_side.sin_addr.s_addr = ((struct in_addr *) (remote_host->h_addr))->s_addr;
	remote_side.sin_port=htons(svc->client_port);
	
	if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			
		
		sprintf(svc->new_server_text, "%s", SOCKET_CREATE_ERROR);
		svc->current_state=STATE_CRITICAL;
				
		return;
			
			
	}
	act1.sa_handler = bartlby_conn_timeout;
	sigemptyset(&act1.sa_mask);
	act1.sa_flags=0;
	#ifdef SA_INTERRUPT
	act1.sa_flags |= SA_INTERRUPT;
	#endif
	
	if(sigaction(SIGALRM, &act1, &oact1) < 0) {
		
		sprintf(svc->new_server_text, "%s", ALARM_ERROR);
		svc->current_state=STATE_CRITICAL;
				
		return;
	
		
	}
	alarm(svc->service_check_timeout);
	client_connect_retval = connect(client_socket, (void *) &remote_side, sizeof(remote_side));
	alarm(0);
	
	if(connection_timed_out == 1 || client_connect_retval == -1) {
		sprintf(svc->new_server_text, "%s", CONN_ERROR);
		svc->current_state=STATE_CRITICAL;
		close(client_socket);
		return;
	} 
	connection_timed_out=0;
	alarm(svc->service_check_timeout);
	return_bytes=recv(client_socket, return_buffer, 1024, 0);
	alarm(0);
	
	if (return_bytes == -1 || connection_timed_out == 1) {
            	_log("%s:%d/%s - TIMEOUT", svc->server_name, svc->client_port,svc->service_name );
		sprintf(svc->new_server_text, "%s", RECV_ERROR);
		svc->current_state=STATE_CRITICAL;
		
		close(client_socket);
		return;
        } 
      
	
	return_buffer[return_bytes-1]='\0';
	//_log("Client: %s", return_buffer);
	connection_timed_out=0;
	
	client_request=malloc(sizeof(char)*(strlen(svc->plugin)+strlen(svc->plugin_arguments)+30));
	sprintf(client_request, "%s| %s|", svc->plugin, svc->plugin_arguments);
	
	//_log("Crequest(%s): %s %d",svc->server_name, client_request, strlen(client_request));
	bartlby_encode(client_request, strlen(client_request));
	
	
	alarm(svc->service_check_timeout);
	send(client_socket, client_request, (strlen(svc->plugin)+strlen(svc->plugin_arguments)+3),0);
	//_log("sending `%s`", client_request);
	if(connection_timed_out == 1) {
		
		
		sprintf(svc->new_server_text, "%s", CONN_ERROR);
		svc->current_state=STATE_CRITICAL;
		free(client_request);
		return;
	}
	free(client_request);
	
	
	alarm(0);
	connection_timed_out=0;
	
	alarm(svc->service_check_timeout);
	return_bytes=recv(client_socket, return_buffer, 256, 0);
	alarm(0);
	
	if (return_bytes == -1 || connection_timed_out == 1) {
            	_log("%s:%d/%s - TIMEOUT", svc->server_name, svc->client_port,svc->service_name );
		sprintf(svc->new_server_text, "%s", RECV_ERROR);
		svc->current_state=STATE_CRITICAL;
		
		close(client_socket);
		return;
        } 
        
        bartlby_decode(return_buffer, return_bytes);
        return_buffer[return_bytes-1]='\0';
        if(return_bytes >= 5) {
        	
        	if(strncmp(return_buffer, "PERF: ", 6) == 0) {
        		//Perfway ;-)
			
			bartlby_perf_track(svc,return_buffer, return_bytes, cfgfile);       		
        		
        		//Read again for result
        		alarm(0);
			connection_timed_out=0;
			
			alarm(svc->service_check_timeout);
			return_bytes=recv(client_socket, return_buffer, 1024, 0);
			alarm(0);
			
			if (return_bytes == -1 || connection_timed_out == 1) {
        		    	_log("%s:%d/%s - TIMEOUT AFETER performance", svc->server_name, svc->client_port,svc->service_name );
				sprintf(svc->new_server_text, "%s", RECV_ERROR);
				svc->current_state=STATE_CRITICAL;
				
				close(client_socket);
				return;
        		} 
        		
        		bartlby_decode(return_buffer, return_bytes);
        		return_buffer[return_bytes-1]='\0';
        		//_log("RB: '%s'", return_buffer);
        		
	        }
	}
        
        close(client_socket);
        
	//bartlby_decode(return_buffer, return_bytes);
	
	
	
	return_token = strtok(return_buffer, return_delimeter);
        if(return_token != NULL) {
        	//Verfiy result code to be 0-2 :-) 
        	if(return_token[0] != '0' && return_token[0] != '1' && return_token[0] != '2') {
        		svc->current_state=STATE_UNKOWN;	
        	} else {
        		svc->current_state=atoi(return_token);
        	}
        	
        	return_token = strtok(NULL, return_delimeter);
        	if(return_token != NULL) {
        		sprintf(svc->new_server_text, "%s", return_token);
        	} else {
        		
        		sprintf(svc->new_server_text, "(empty output)");
        		
        	}	
        } else {
        	
        	sprintf(svc->new_server_text, PROTOCOL_ERROR);
        	svc->current_state=STATE_CRITICAL;
        }	
        return;
		
}
*/

void bartlby_check_group(struct service * svc, void * shm_addr) {
	struct shm_header * hdr;
	struct service * svcmap;


	char del[]="|";
	char tmp_svcvar[2048];	
	char * tok;
	int svc_id;
	int state;
	struct service * svg;
	int x;
	
	hdr=bartlby_SHM_GetHDR(shm_addr);
	svcmap=bartlby_SHM_ServiceMap(shm_addr);	
	
	if(svc->service_var[0] == '\0')  {
		
		
		sprintf(svc->new_server_text, "%s", GROUP_WITHOUT_PARMS);
		svc->current_state=STATE_CRITICAL;	
	} else {
		strcpy(tmp_svcvar, svc->service_var);
		
		tok=strtok(tmp_svcvar, del);
		while(tok != NULL) {
			
			sscanf(tok, "%d=%d", &svc_id, &state);
			
			
			
			svg=NULL;
			for(x=0; x<hdr->svccount; x++) {
				
				if(svcmap[x].service_id == svc_id) {
					svg=&svcmap[x];
				}
			}	
			if(svg == NULL) { 
				
				sprintf(svc->new_server_text, "%s", GROUP_CRITICAL);
				svc->current_state=STATE_CRITICAL;
				return;
			}
			
			if(svg->current_state != state) {
				//_log("Service: is not %d\n", svg->current_state );
				sprintf(svc->new_server_text, "%s", GROUP_CRITICAL);
				svc->current_state=STATE_CRITICAL;
				
				return;	
			}
			
			tok=strtok(NULL, del);
			
		}
		
		sprintf(svc->new_server_text, "%s", GROUP_OK);
		svc->current_state=STATE_OK;
	}
		
		
}
void bartlby_fin_service(struct service * svc, void * SOHandle, void * shm_addr,char * cfgfile) {
	char * dlmsg;
	struct worker * wrkmap;
	int x;
	
	struct shm_header * hdr;
	
	int (*doUpdate)(struct service *,char *);
	
	
	hdr=bartlby_SHM_GetHDR(shm_addr);
	wrkmap=bartlby_SHM_WorkerMap(shm_addr);
	
		
	if(svc->current_state != svc->last_state) {
		//udate tstamp text and call trigger *g*
		//_log("<%d/%d--DOLOG>%d;%d;);		
		_log("@LOG@%d|%d|%s:%d/%s|%s", svc->service_id, svc->current_state, svc->server_name, svc->client_port, svc->service_name, svc->new_server_text);
		//pos2_pull_trigger(svc);	
		bartlby_trigger(svc, cfgfile, shm_addr, 1);
		
		svc->last_check=time(NULL);
		svc->last_state=svc->current_state;
		
		
					
	} else {
		
		
		if(svc->service_type != SVC_TYPE_PASSIVE) {
			
			svc->last_check=time(NULL);
		}
		
	}
	
	for(x=0; x<=strlen(svc->new_server_text); x++) {
		if(svc->new_server_text[x] == '\'')
			svc->new_server_text[x]='"';
	}
	LOAD_SYMBOL(doUpdate,SOHandle, "doUpdate");
	doUpdate(svc,cfgfile);
	
	
	
}

void bartlby_check_service(struct service * svc, void * shm_addr, void * SOHandle, char * cfgfile) {
	//_log("check service");
	int ctime, pdiff;
	//_log("<%d/%d -- CHECK >: %s",svc->current_state,svc->last_state, svc->service_name);
	
	setenv("BARTLBY_CONFIG", cfgfile,1);
	setenv("BARTLBY_CURR_PLUGIN", svc->plugin,1);
	setenv("BARTLBY_CURR_HOST", svc->server_name,1);
	setenv("BARTLBY_CURR_SERVICE", svc->service_name,1);
	
	if(svc->service_type == SVC_TYPE_GROUP) {
		bartlby_check_group(svc, shm_addr);
		bartlby_fin_service(svc,SOHandle,shm_addr,cfgfile);
		return;	
	}
	if(svc->service_type == SVC_TYPE_PASSIVE) {
		ctime=time(NULL);
		pdiff=ctime-svc->last_check;
		
		if(pdiff >= svc->service_passive_timeout) {
			
			sprintf(svc->new_server_text, "%s", PASSIVE_TIMEOUT);
			svc->current_state=STATE_WARNING;
			
			
		}
		//_log("PASSIVE_CHECK %d->%d", svc->service_passive_timeout, svc->service_id);
		bartlby_fin_service(svc, SOHandle,shm_addr,cfgfile);
		return;	
	}
	if(svc->service_type == SVC_TYPE_ACTIVE) {
		bartlby_check_active(svc,cfgfile);
		bartlby_fin_service(svc,SOHandle,shm_addr,cfgfile);
		return;		
	}
	
	
	
	
	return;
}
