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
#include <dirent.h>
#include <sys/stat.h>



#include <bartlby.h>

#define NOTIFY_MSG "     State Change %s to %s (%s)\\n*********** %s %s ********************\\n[  Server: %s, Service: %s, State: %s]\\n%s"
#define FL 0
#define TR 1
#define ESCALATION_MINUTES 2
#define ESCALATION_LIMIT 5

int bartlby_trigger_worker_level(struct worker * w, int level) {
	char * find_level;
	char * blevel;
	int rt;
	
	blevel=bartlby_beauty_state(level);
	find_level=malloc(10+2);
	sprintf(find_level, "|%d|",level);
	if(strstr(w->notify_levels, find_level) != NULL || strlen(w->notify_levels) == 0) {
		rt=TR;
	} else {
		rt=FL;	
		_log("Worker %s doesnt have level: %s '%s'-->'%s'(%d)", w->mail, blevel, find_level, w->notify_levels, strlen(w->notify_levels));
		
	}
	
	free(blevel);
	free(find_level);	
	return TR;
}

int bartlby_trigger_escalation(struct worker *w) {
	if((time(NULL) - w->escalation_time) >= (ESCALATION_MINUTES*60)) {
		w->escalation_count=0;
		return TR;	
	} else {
		if(w->escalation_count > ESCALATION_LIMIT) {
			_log("escalation!!! %s->%d/%d",w->mail, w->escalation_count, ESCALATION_LIMIT);	
			return FL;
		} else {
			w->escalation_count++;
			return TR;	
		}	
	}
}

int bartlby_trigger_chk(struct service *svc) {
	
	
	if(svc->notify_enabled == 0) {
		_log("Suppressed notify: Notifications disabled %s:%d/%s",svc->client_ip, svc->client_port, svc->service_name);
		return FL;
	} else {
		if((time(NULL)- svc->last_notify_send) >= (2*60)) {
			svc->flap_count=0;
			return TR;				
		} else {
			
			if(svc->flap_count > 2) {
				_log("Suppressed notify: Service %s:%d/%s currently flapping: %d", svc->client_ip, svc->client_port, svc->service_name, svc->flap_count);
				return FL;
			} else {
				//Log("trigger", "Service %s:%d/%s Sent", svc->client_ip, svc->client_port, svc->service_name);	
				svc->flap_count++;
				return TR;
			}
			
		}
	}
					
  

	
}

void bartlby_trigger(struct service * svc, char * cfgfile, void * shm_addr) {
	char * trigger_dir;
	struct dirent *entry;
	DIR * dtrigger;
	char * full_path;
	struct stat finfo;	
	int x;
	char * human_state, * human_state_last;
	char * notify_msg;
	char * find_str;
	struct worker * wrkmap;
	struct shm_header * hdr;
	char * exec_str;
	FILE * ptrigger;
	char trigger_return[128];
	
	hdr=bartlby_SHM_GetHDR(shm_addr);
	wrkmap=bartlby_SHM_WorkerMap(shm_addr);
	
	
	if((bartlby_trigger_chk(svc)) == FL) {
		return;	
	}
	
	human_state=bartlby_beauty_state(svc->current_state);
	human_state_last=bartlby_beauty_state(svc->last_state);
	
	/*
	LAST CUR SERVICE
	PROGNAME VERSION
	SERVERN SERVICE CUR MSG
	*/
	find_str=malloc(10+2);
	sprintf(find_str, "|%d|", svc->service_id);
	
	notify_msg=malloc(sizeof(char)*(strlen(NOTIFY_MSG)+strlen(human_state_last)+strlen(human_state)+strlen(svc->service_name)+strlen(PROGNAME)+strlen(VERSION)+strlen(svc->server_name)+strlen(svc->service_name)+strlen(human_state)+strlen(svc->new_server_text)));
	sprintf(notify_msg, NOTIFY_MSG, human_state_last, human_state, svc->service_name, PROGNAME, VERSION, svc->server_name, svc->service_name, human_state, svc->new_server_text);
	
	
	
	trigger_dir=getConfigValue("trigger_dir", cfgfile);
	if(trigger_dir == NULL) {
		_log("bartlby_trigger() failed");
		return;	
	}
	dtrigger = opendir(trigger_dir);
	if(!dtrigger) {
		_log("opendir %s failed", trigger_dir);
		return;	
	}
	while((entry = readdir(dtrigger)) != NULL) {
		full_path=malloc(sizeof(char) * (strlen(entry->d_name)+strlen(trigger_dir)+2));
		sprintf(full_path, "%s/%s", trigger_dir, entry->d_name);
		if(lstat(full_path, &finfo) < 0) {
			_log("lstat() %s failed", full_path);
			return;	
		}
		if(S_ISREG(finfo.st_mode)) {
			for(x=0; x<hdr->wrkcount; x++) {
				if(strstr(wrkmap[x].services, find_str) != NULL || strlen(wrkmap[x].services) == 0) {
					
					if((bartlby_trigger_escalation(&wrkmap[x])) == FL) continue;
					if((bartlby_trigger_worker_level(&wrkmap[x], svc->current_state)) == FL) continue;
					//_log("EXEC trigger: %s", full_path);
					_log("@NOT@%d|%d|%d|%s|%s", svc->service_id, svc->last_state ,svc->current_state,entry->d_name,wrkmap[x].mail);
					
					svc->last_notify_send=time(NULL);
					wrkmap[x].escalation_time=time(NULL);
					exec_str=malloc(sizeof(char)*(strlen(full_path)+strlen("\"\"\"\"   ")+strlen(notify_msg)+strlen(wrkmap[x].mail)));
					sprintf(exec_str, "%s \"%s\" \"%s\"", full_path, wrkmap[x].mail, notify_msg);
					ptrigger=popen(exec_str, "r");
					if(ptrigger != NULL) {
						if(fgets(trigger_return, 1024, ptrigger) != NULL) {
							trigger_return[strlen(trigger_return)-1]='\0';
							_log("Trigger returned: `%s'", trigger_return);
      						} else {
      							_log("Trigger empty output");
      						}
      						pclose(ptrigger);
      					} else {
      						_log("trigger failed `%s'", full_path);	
      					}
					free(exec_str);
				}
			}	
		}
				
		
		
		free(full_path);
	}
	free(find_str);
	free(human_state);
	free(human_state_last);
	free(notify_msg);
	closedir(dtrigger);
}

