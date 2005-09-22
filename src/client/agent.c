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
Revision 1.9  2005/09/22 02:55:03  hjanuschka
agent: def timeout 15
check: strreplace ' "

Revision 1.8  2005/09/18 04:04:52  hjanuschka
replication interface (currently just a try out)
one instance can now replicate itself to another using portier as a transport way
FIXME: need to sort out a binary write() problem

Revision 1.7  2005/09/14 22:01:41  hjanuschka
debug in data_lib added and removed
agent: off by two :-) *fG* malloc error producing magic char's  (fixed)

Revision 1.6  2005/09/13 22:11:52  hjanuschka
ip_list moved to .cfg
	allowed_ips
load limit moved to cfg
	agent_load_limit

portier now also uses ip list to verify ip of connector

portier: passive check without plg args fixed

Revision 1.5  2005/09/02 02:16:57  hjanuschka
some trap downs ;-)

Revision 1.4  2005/08/30 20:13:17  hjanuschka
fixed pclose() wrong exit code in agent

Revision 1.3  2005/08/28 16:02:59  hjanuschka
CVS Header


*/
#include <malloc.h>
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

#include <bartlby.h>
static int connection_timed_out=0;

#define CONN_TIMEOUT 15

static void agent_conn_timeout(int signo) {
 	connection_timed_out = 1;
}
int main(int argc, char ** argv) {
	float loadavg[3];
	FILE * load;
	char svc_back[1024];
	char svc_in[1024];
	char * plugin_dir;
	char  * plugin_path;
	char plg[1024];
	char plg_args[1024];
	char * token;
	char * exec_str;
	int ip_ok=-1;
	int plugin_rtc;
	struct stat plg_stat;
	char plugin_output[1024];
	struct sockaddr_in name;
   	int namelen = sizeof(name);
	char * agent_load_limit;
	char * allowed_ip_list;
	
	
	FILE * fplg;
	
	struct sigaction act1, oact1;
	
        if(argc < 1) {
        	_log("Usage: bartlby_agent <CFGFILE>");
        	
        		
        }
        
        agent_load_limit=getConfigValue("agent_load_limit", argv[0]);
        allowed_ip_list=getConfigValue("allowed_ips", argv[0]);
        
        if(agent_load_limit == NULL) {
        	agent_load_limit=strdup("10");	
        }
        if(allowed_ip_list == NULL) {
        	printf("No Ip Allowed");
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
        	sprintf(svc_back, "2|IP Blocked ");
        	bartlby_encode(svc_back, strlen(svc_back));
		printf("%s", svc_back);	
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
	
    	sprintf(svc_back, "1|ouuutsch");
        
        plugin_dir=getConfigValue("agent_plugin_dir", argv[0]);
        if(plugin_dir == NULL) {
        	_log("plugin dir failed");	
        	exit(1);
        }
        
        
        
        load=fopen("/proc/loadavg", "r");
        fscanf(load, "%f %f %f", &loadavg[0], &loadavg[1], &loadavg[2]);
        fclose(load);
        
        if(loadavg[0] < atof(agent_load_limit)) {
		free(agent_load_limit);
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
		
		svc_in[strlen(svc_in)-1]='\0';
		bartlby_decode(svc_in, strlen(svc_in));
		token=strtok(svc_in, "|");
		if(token == NULL) {
			sprintf(svc_back,"1|Protocol Error (No plugin specified");	
		} else {
			sprintf(plg, "%s", token);
			syslog(LOG_ERR, "bartlby_agent: %s",plg);
			plugin_path=malloc(sizeof(char) * (strlen(plugin_dir)+strlen(plg)+255));
			sprintf(plugin_path, "%s/%s", plugin_dir, plg);
			if(stat(plugin_path,&plg_stat) < 0) {
				sprintf(svc_back, "1|Plugin does not exist (%s)", plugin_path);	
			} else {
				token=strtok(NULL, "|");
				if(token == NULL) {
					sprintf(plg_args, " ");	
				} else {
					sprintf(plg_args, "%s", token);
				}
				exec_str=malloc(sizeof(char) * (strlen(plugin_path)+strlen(plg_args)+255));
				sprintf(exec_str, "%s %s", plugin_path, plg_args);
				//printf("E_STR: P: '%s' A: '%s' F: '%s'\n", plugin_path, plg_args, exec_str);
				
				fplg=popen(exec_str, "r");
				if(fplg != NULL) {
					if(fgets(plugin_output, 1024, fplg) != NULL) {
						plugin_rtc=pclose(fplg);
						plugin_output[strlen(plugin_output)-1]='\0';
						sprintf(svc_back, "%d|%s\n", WEXITSTATUS(plugin_rtc), plugin_output);		
					} else {
						plugin_rtc=pclose(fplg);
						sprintf(svc_back, "%d|No Output - %s", WEXITSTATUS(plugin_rtc), exec_str);	
						
						
					}
					
					
				} else {
					sprintf(svc_back, "1|Plugin open failed");	
				}

				free(exec_str);
				
			}
			
			
			free(plugin_path);
			
			
		}
		
		
		
		
		        	
        } else { 
        	sprintf(svc_back, "1|LoadLimit reached %.02f skipping Check!|\n", loadavg[0]);
        	free(agent_load_limit);
        }
	//printf("SVC_BACK: %s\n", svc_back);
	syslog(LOG_ERR, "bartlby_agent: %s",svc_back);
	bartlby_encode(svc_back, strlen(svc_back));
	printf("%s", svc_back);
	bartlby_decode(svc_back, strlen(svc_back));
	//printf("\n %s \n", svc_back);
	return 1;
}
