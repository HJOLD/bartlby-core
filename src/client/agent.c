#include <stdio.h>
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

#define CONN_TIMEOUT 10

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
	
	FILE * fplg;
	
	struct sigaction act1, oact1;
	
        if(argc < 3) {
        	_log("Usage: bartlby_agent <IPLIST> <LOAD-LIMIT> <PORT>");
        	printf("IPLIST is a comma seperated list of IP addresses\n");
        		
        }
        
        token=strtok(argv[0],",");
        
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
        
        plugin_dir=getConfigValue("agent_plugin_dir", argv[2]);
        if(plugin_dir == NULL) {
        	_log("plugin dir failed");	
        	exit(1);
        }
        
        
        
        load=fopen("/proc/loadavg", "r");
        fscanf(load, "%f %f %f", &loadavg[0], &loadavg[1], &loadavg[2]);
        fclose(load);
        
        if(loadavg[0] < atof(argv[1])) {
		
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
			plugin_path=malloc(sizeof(char) * (strlen(plugin_dir)+strlen(plg)));
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
				exec_str=malloc(sizeof(char) * (strlen(plugin_path)+strlen(plg_args)));
				sprintf(exec_str, "%s %s", plugin_path, plg_args);
				fplg=popen(exec_str, "r");
				if(fplg != NULL) {
					if(fgets(plugin_output, 1024, fplg) != NULL) {
						plugin_output[strlen(plugin_output)-1]='\0';
						sprintf(svc_back, "%d|%s\n", WEXITSTATUS(plugin_rtc), plugin_output);		
					} else {
						sprintf(svc_back, "1|No Output");	
					}
					plugin_rtc=pclose(fplg);
					
				} else {
					sprintf(svc_back, "1|Plugin open failed");	
				}

				free(exec_str);
				
			}
			
			
			free(plugin_path);
			
			
		}
		
		
		
		
		        	
        } else { 
        	sprintf(svc_back, "1|LoadLimit reached %.02f skipping Check!|\n", loadavg[0]);
        }
	//printf("SVC_BACK: %s\n", svc_back);
	bartlby_encode(svc_back, strlen(svc_back));
	printf("%s", svc_back);
	bartlby_decode(svc_back, strlen(svc_back));
	//printf("\n %s \n", svc_back);
	return 1;
}
