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
Revision 1.5  2005/09/18 04:04:52  hjanuschka
replication interface (currently just a try out)
one instance can now replicate itself to another using portier as a transport way
FIXME: need to sort out a binary write() problem

Revision 1.4  2005/09/13 22:11:52  hjanuschka
ip_list moved to .cfg
	allowed_ips
load limit moved to cfg
	agent_load_limit

portier now also uses ip list to verify ip of connector

portier: passive check without plg args fixed

Revision 1.3  2005/09/09 19:23:37  hjanuschka
portier: added get_passive cmd
added a little dummy passive portier query tool (wich can set passive states and recieve passiv service info

Revision 1.2  2005/09/07 22:36:56  hjanuschka
portier: added err code -4 svc not found
check: group check fixed , runnaway strtok :-)

Revision 1.1  2005/09/07 21:52:25  hjanuschka
portier import




*/
#include <dlfcn.h>
#include <time.h>
#include <stdio.h>
#include <syslog.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/shm.h>


#include <bartlby.h>
static int connection_timed_out=0;


#define CMD_PASSIVE 1
#define CMD_GET_PLG 2
#define CMD_REPL 3

#define CONN_TIMEOUT 10

static void agent_conn_timeout(int signo) {
 	connection_timed_out = 1;
}
int main(int argc, char ** argv) {
	struct sigaction act1, oact1;
	char svc_in[2048];
	char svc_out[2048];
	
	char * allowed_ip_list;
	int ip_ok=-1;
	struct sockaddr_in name;
   	int namelen = sizeof(name);
	
	char * token;
	
	
	int command;
	int svc_found=0;
	
	char * SOName; //Shared library name
	void * SOHandle;
	const char * dlmsg;
	
	/////////// Replication ////////////////
	long repl_SHMSize;
	void * repl_shm_addr;
	struct service * repl_svcmap;
	struct worker * repl_wrkmap;
	struct shm_header * repl_hdr;
	int (*addService)(struct service *,char *);
	int (*addWorker)(struct worker *, char*);
	int read_rtc;
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int passive_svcid;
	int passive_state;
	char passive_text[2048];
	char * passive_beauty;
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//SHM
	char * shmtok;
	int shm_id;
	//int * shm_elements;
	void * bartlby_address;
	
	int x;
	
	
	struct shm_header * shm_hdr;
	struct service * svcmap;
	
	
	allowed_ip_list=getConfigValue("allowed_ips", argv[0]);
	if(allowed_ip_list == NULL) {
        	printf("-No Ip Allowed");
        	exit(1);
        	
        }
	
	token=strtok(allowed_ip_list,",");
        
        if (getpeername(0,(struct sockaddr *)&name, &namelen) < 0) {
   		//syslog(LOG_ERR, "getpeername: %m");
   		exit(1);
   	} else {
   		//syslog(LOG_INFO, "Connection from %s",	inet_ntoa(name.sin_addr));
   	}
        
        while(token != NULL) {
        	//printf("CHECKING: %s against %s\n", token, inet_ntoa(name.sin_addr));
        	if(strcmp(token, inet_ntoa(name.sin_addr)) == 0) {
        		ip_ok=0;	
        	}
        	token=strtok(NULL, ",");	
        }
        free(allowed_ip_list);
        if(ip_ok < 0) {
        	printf("-IP Blocked\n");
		exit(1);
        }	
	
	shmtok = getConfigValue("shm_key", argv[0]);
	
	if(shmtok == NULL) {
		_log("Unset variable `shm_key'");
		fflush(stdout);
		exit(1);
	}
	
	shm_id = shmget(ftok(shmtok, 32), 0, 0777);
	if(shm_id != -1) {
		
		bartlby_address=shmat(shm_id,NULL,0);
		shm_hdr=bartlby_SHM_GetHDR(bartlby_address);
		svcmap=bartlby_SHM_ServiceMap(bartlby_address);
		printf("+SVCC: %d WRKC: %d V: %s\n", shm_hdr->svccount, shm_hdr->wrkcount, shm_hdr->version);
		fflush(stdout);
		
	} else {
		printf("-1 Bartlby down!!!\n");	
		fflush(stdout);
		exit(1);
	}
	
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	if(argc == 0) {
		printf("CONFIG FILE MISSING\n");
		exit(1);	
	}
	act1.sa_handler = agent_conn_timeout;
	sigemptyset(&act1.sa_mask);
	act1.sa_flags=0;
	#ifdef SA_INTERRUPT
	act1.sa_flags |= SA_INTERRUPT;
	#endif
	if(sigaction(SIGALRM, &act1, &oact1) < 0) {
		
		printf("ALARM SETUP ERROR");
		exit(1);
				
		return -1;
	
		
	}
	connection_timed_out=0;
	alarm(CONN_TIMEOUT);
	//ipmlg]ajgai]Amoowlkecvg~"/j"nmacnjmqv~
	if(read(fileno(stdin), svc_in, 1024) < 0) {
		printf("BAD!");
		exit(1);
	}
	alarm(0);
	
	if(connection_timed_out == 1) {
		printf("Timed out!!!\n");
		exit(1);	
	}
	passive_svcid=-1;
	passive_state=-1;
	sprintf(passive_text, "failed");
	sprintf(svc_out, "-20");
	token=strtok(svc_in, "|");
	if(token != NULL) {
		command=atoi(token);
		switch(command) {
			case CMD_REPL:
				token=strtok(NULL, "|");
				if(token != NULL) {
					/*
					long repl_SHMSize;
					void * repl_shm_addr;
					struct service * repl_svcmap;
					struct worker * repl_wrkmap;
					struct shm_header * repl_hdr;
					*/
					SOName = getConfigValue("data_library", argv[0]);
					if(SOName == NULL) {
						printf("-No data_library specified in `%s' config file", argv[0]);
						exit(1);
					}
					SOHandle=dlopen(SOName, RTLD_LAZY);
		
    					if((dlmsg=dlerror()) != NULL) {
        					printf("-Error: %s", dlmsg);
        					exit(1);
    					}
					LOAD_SYMBOL(addWorker,SOHandle, "AddWorker");
					LOAD_SYMBOL(addService,SOHandle, "AddService");
					

					repl_SHMSize=atol(token);
					printf("+waiting for: %ld Bytes\n", repl_SHMSize);
					fflush(stdout);
					
					
					repl_shm_addr=malloc(sizeof(void *)*repl_SHMSize);
					
					connection_timed_out=0;
					alarm(CONN_TIMEOUT);
					
					if((read_rtc=read(fileno(stdin), repl_shm_addr, repl_SHMSize)) < 0) {
						printf("-BAD!");
						exit(1);
					}
					alarm(0);
					
					if(connection_timed_out == 1) {
						printf("-Timed out!!!\n");
						exit(1);	
					}
					repl_hdr=bartlby_SHM_GetHDR(repl_shm_addr);
					repl_svcmap=bartlby_SHM_ServiceMap(repl_shm_addr);
					repl_wrkmap=bartlby_SHM_WorkerMap(repl_shm_addr);
					
					
					/*FILE * fp;
					fp=fopen("/var/tmp/1.shm", "wb");
					fwrite(repl_shm_addr,sizeof(repl_shm_addr), repl_SHMSize, fp);
					fclose(fp);
					*/
					printf("+ FIXME !! Read: %d Got: %d Services %d Workers\n",read_rtc, repl_hdr->svccount, repl_hdr->wrkcount);
					fflush(stdout);
					free(repl_shm_addr);
					exit(1);
					
					
						
				} else {
					sprintf(svc_out, "-7 Bytes not supplied\n");	
				}
			break;
			case CMD_GET_PLG:
				token=strtok(NULL, "|");
				if(token != NULL) {
					passive_svcid=atoi(token);
					
					svc_found=0;
					for(x=0; x<shm_hdr->svccount; x++) {
						
						if(svcmap[x].service_id == passive_svcid) {
							svc_found = 1;
							break;
						}
					}
					if(svc_found == 1) {
						//1|413395|2|dasdsadsadsadas|
						if(svcmap[x].service_type == SVC_TYPE_PASSIVE) {
							
							sprintf(svc_out, "+PLG|%s %s|\n", svcmap[x].plugin,svcmap[x].plugin_arguments);
							
						} else {
							sprintf(svc_out, "-3 Service is not of type 'PASSIVE'");	
						}
					} else {
						sprintf(svc_out, "-4 Service not found\n");	
					}
					
					
					
				} else {
					sprintf(svc_out, "-5 SVCID missing\n");	
					
				}
			break;
			case CMD_PASSIVE:
				//Second is SVCID
				//Third is new status
				//Fourth is new service_text
				token=strtok(NULL, "|");
				if(token != NULL) {
					passive_svcid=atoi(token);
					
					token=strtok(NULL, "|");
					if(token != NULL) {
						
						passive_state=atoi(token);
						token=strtok(NULL, "|");
						if(token != NULL) {
							
							sprintf(passive_text, "%s", token);
							
						} else {
							sprintf(passive_text," ");
									
						}
						
					
						
					
						svc_found=0;
						for(x=0; x<shm_hdr->svccount; x++) {
							
							if(svcmap[x].service_id == passive_svcid) {
								svc_found = 1;
								break;
							}
						}
						if(svc_found == 1) {
							//2|413395
							if(svcmap[x].service_type == SVC_TYPE_PASSIVE) {
								svcmap[x].last_state=svcmap[x].current_state;
								svcmap[x].current_state=passive_state;
								sprintf(svcmap[x].new_server_text, "%s", passive_text);
								svcmap[x].last_check=time(NULL);
								
								passive_beauty=bartlby_beauty_state(svcmap[x].current_state);
								sprintf(svc_out, "+PASSIVOK (%d) %d : %s (%s)\n", x, svcmap[x].service_id, passive_beauty, svcmap[x].new_server_text);
								free(passive_beauty);
							} else {
								sprintf(svc_out, "-3 Service is not of type 'PASSIVE'");	
							}
						} else {
							sprintf(svc_out, "-4 Service not found\n");	
						}
						
					} else {
						sprintf(svc_out, "New state missing\n");		
					}
					
				} else {
					sprintf(svc_out, "SVCID missing\n");	
					
				}
			break;
			default:
				printf("-2 cmd not found\n");
				exit(1);				
		}
		
		printf(svc_out);
			
	}
	shmdt(bartlby_address);
	//bartlby_encode(svc_out, strlen(svc_out));
	//printf("%s", svc_out);
	//bartlby_decode(svc_out, strlen(svc_out));
	
	
	return 1;
}
