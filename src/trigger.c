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
Revision 1.23  2006/10/05 23:19:37  hjanuschka
auto commit

Revision 1.22  2006/09/23 22:38:46  hjanuschka
auto commit

Revision 1.21  2006/08/07 18:40:10  hjanuschka
auto commit

Revision 1.20  2006/08/03 20:29:13  hjanuschka
auto commit

Revision 1.19  2006/07/18 21:38:23  hjanuschka
core: a major BUG has been discoverd in the first production envorioments
	 when a worker has only selected OK and CRITICAL notifications
	 he always got notified about a change from (unselected) WARNING back to OK
	 this had produce ALOT of unserious OK notifications
	 -- 18-07-06 fixed :-)

core: perfhandlers have been re-worked to only collect data

Revision 1.18  2006/06/29 18:14:40  hjanuschka
fixing trigger check typo

Revision 1.17  2006/02/12 18:37:51  hjanuschka
trigger fixes trigger logging refined
datalib: mysql/ returns now the server version

Revision 1.16  2006/02/10 23:54:46  hjanuschka
SIRENE mode added

Revision 1.15  2006/01/10 22:37:25  hjanuschka
some changes
	trigger msg comes out of cfgfile with some $VAR macros

Revision 1.14  2005/11/27 02:04:42  hjanuschka
setuid/setgid for security and web ui

Revision 1.13  2005/10/13 22:13:14  hjanuschka
logging improved, check fixup

Revision 1.12  2005/10/03 20:48:19  hjanuschka
*** empty log message ***

Revision 1.11  2005/09/28 21:46:30  hjanuschka
converted files to unix
jabber.sh -> disabled core dumps -> jabblibs segfaults
                                    will try to patch it later

Revision 1.10  2005/09/27 19:39:00  hjanuschka
trigger timeout
agent local timeout

Revision 1.9  2005/09/27 18:21:57  hjanuschka
*** empty log message ***

Revision 1.8  2005/09/25 16:31:05  hjanuschka
trigger: can now be enabled/disabled per trigger in web ui
ui: add/modify worker can now set and display workers selected
datalib: api modifications for trigger enable/disable feature

Revision 1.7  2005/09/25 15:24:22  hjanuschka
icq.sh trigger for icq in combination with a running licq

Revision 1.6  2005/09/25 13:30:18  hjanuschka
cfg: jabber variables
daemon: setenv BARTLBY_HOME (for triggers)
sched: wait_open timeout
mail.sh: sendmail trigger
trigger: $1 == email
$2 == icq
$3 == name
$4 == msg

Revision 1.5  2005/09/11 09:20:58  hjanuschka
logging issue ;-)
ui now can display log in a nice layout ;-)

Revision 1.4  2005/09/03 20:11:22  hjanuschka
fixups

added addworker, deleteworker, modifyworker, getworkerbyid

Revision 1.3  2005/08/28 16:02:59  hjanuschka
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
#include <dirent.h>
#include <sys/stat.h>



#include <bartlby.h>

#define DEFAULT_NOTIFY_MSG "State Change ($READABLE_STATE)\\n*********** $PROGNAME $VERSION ********************\\n[  Server: $SERVER, Service: $SERVICE, State: $READABLE_STATE]\\n%$MESSAGE"
#define FL 0
#define TR 1


static int connection_timed_out=0;
#define CONN_TIMEOUT 15


static void trigger_conn_timeout(int signo) {
 	connection_timed_out = 1;
}

int bartlby_trigger_worker_level(struct worker * w,  struct service * svc) {
	char * find_level, * last_level;
	char * blevel;
	int rt;
	int level;
	int last;
	
	level = svc->current_state;
	last  = svc->notify_last_state;
	
	blevel=bartlby_beauty_state(level);
	find_level=malloc(10+2);
	last_level=malloc(10+2);
	
	sprintf(last_level, "|%d|", last);
	sprintf(find_level, "|%d|",level);
	if(strstr(w->notify_levels, find_level) != NULL || strlen(w->notify_levels) == 0) {
		if(level < last) {
			//_log("@debug level:%d < last:%d", level, last);
			if(strstr(w->notify_levels, last_level) != NULL) {
				rt=TR;
				//_log("@debug has both levels: %d/%d", level, last);		
			} else {
				if(level == STATE_OK && svc->recovery_outstanding == RECOVERY_OUTSTANDING) {
					rt=TR;
					//_log("@debug services has recoverd so pull trigger %d/%d", level, last);
				} else { 
					//_log("@debug service doesn't have last_state %d/%d", level, last);	
					rt = FL;	
				}
			}
		} else {
			//_log("@debug current level is selected and higher than last so fire!! %d/%d", level, last);	
			rt=TR;
		}
	} else {
		rt=FL;	
		//_log("Worker %s doesnt have level: %s '%s'-->'%s'(%d)", w->mail, blevel, find_level, w->notify_levels, strlen(w->notify_levels));
		
	}
	
	free(blevel);
	free(last_level);
	free(find_level);	
	return rt;
}

int bartlby_trigger_escalation(struct worker *w, struct service * svc) {
	if(w->active != 1) {
		//_log("Worker: %s is inactive", w->mail);
		return FL;	
	}
	if((time(NULL) - w->escalation_time) >= (w->escalation_minutes*60)) {
		w->escalation_count=0;
		return TR;	
	} else {
		if(w->escalation_count > w->escalation_limit) {
			_log("@NOT-EXT@%d|%d|%d|%s||%s:%d/%s|'(escalation %d/%d)'", svc->service_id, svc->last_state ,svc->current_state,w->name, svc->server_name, svc->client_port, svc->service_name,w->escalation_count, w->escalation_limit);
			return FL;
		} else {
			w->escalation_count++;
			return TR;	
		}	
	}
}

int bartlby_trigger_chk(struct service *svc) {
	
	if(svc->srv->server_notify == 0) {
		_log("@NOT-EXT@%d|%d|%d|||%s:%d/%s|'(Notifications disabled on this server)'", svc->service_id, svc->last_state ,svc->current_state, svc->server_name, svc->client_port, svc->service_name);
	}
	if(svc->notify_enabled == 0) {
		//_log("Suppressed notify: Notifications disabled %s:%d/%s",svc->client_ip, svc->client_port, svc->service_name);
		_log("@NOT-EXT@%d|%d|%d|||%s:%d/%s|'(Notifications disabled)'", svc->service_id, svc->last_state ,svc->current_state, svc->server_name, svc->client_port, svc->service_name);
		return FL;
	} else {
		if((time(NULL) - svc->srv->last_notify_send) > svc->srv->server_flap_seconds) {
			svc->srv->flap_count=0;	
		} else {
			if(svc->srv->flap_count > 2) {
				_log("@NOT-EXT@%d|%d|%d|||%s:%d/%s|'(Server lazy %d)'", svc->service_id, svc->last_state ,svc->current_state, svc->server_name, svc->client_port, svc->service_name, svc->flap_count);
				return FL;
			} else {
				svc->srv->flap_count++;	
			}	
		}
		
		if((time(NULL)- svc->last_notify_send) >= svc->flap_seconds) {
			svc->flap_count=0;
			return TR;				
		} else {
			
			if(svc->flap_count > 2) {
				_log("@NOT-EXT@%d|%d|%d|||%s:%d/%s|'(Service flapping %d)'", svc->service_id, svc->last_state ,svc->current_state, svc->server_name, svc->client_port, svc->service_name, svc->flap_count);
				return FL;
			} else {
				//Log("trigger", "Service %s:%d/%s Sent", svc->client_ip, svc->client_port, svc->service_name);	
				svc->flap_count++;
				return TR;
			}
			
		}
	}
					
  

	
}


void bartlby_trigger(struct service * svc, char * cfgfile, void * shm_addr, int do_check) {
	char * trigger_dir;
	struct dirent *entry;
	DIR * dtrigger;
	char * full_path;
	struct stat finfo;	
	int x;
	
	struct ext_notify en;
	
	char * notify_msg;
	char * find_str;
	struct worker * wrkmap;
	struct shm_header * hdr;
	char * exec_str;
	FILE * ptrigger;
	char * find_trigger;
	char trigger_return[128];
	struct sigaction act1, oact1;
	char * cfg_trigger_msg;
	int notify_mem;
	
	hdr=bartlby_SHM_GetHDR(shm_addr);
	wrkmap=bartlby_SHM_WorkerMap(shm_addr);
	
	if(do_check == 1) {
		if(bartlby_trigger_chk(svc) == FL) {
			return;	
		}
	}
	
	signal(SIGPIPE,SIG_DFL);
	signal(SIGCHLD,SIG_DFL);
	
	act1.sa_handler = trigger_conn_timeout;
	sigemptyset(&act1.sa_mask);
	act1.sa_flags=0;
	#ifdef SA_INTERRUPT
	act1.sa_flags |= SA_INTERRUPT;
	#endif
	if(sigaction(SIGALRM, &act1, &oact1) < 0) {
		
		printf("ALARM SETUP ERROR");
		exit(1);
				
		
	
		
	}
	
	
	
	/*
	LAST CUR SERVICE
	PROGNAME VERSION
	SERVERN SERVICE CUR MSG
	*/
	find_str=malloc(10+2);
	find_trigger=malloc(100+200);
	
	sprintf(find_str, "|%d|", svc->service_id);
	cfg_trigger_msg=getConfigValue("trigger_msg", cfgfile);
	if(cfg_trigger_msg == NULL) {
		cfg_trigger_msg=strdup(DEFAULT_NOTIFY_MSG);	
	}
	notify_mem=(strlen(cfg_trigger_msg)+40+strlen(svc->service_name)+strlen(PROGNAME)+strlen(VERSION)+strlen(svc->server_name)+strlen(svc->service_name)+40+strlen(svc->new_server_text));
	notify_msg=malloc(sizeof(char)*notify_mem);
	sprintf(notify_msg, "%s", cfg_trigger_msg);
	//$VARS
	
	bartlby_replace_svc_in_str(notify_msg, svc, notify_mem);
	
	
	free(cfg_trigger_msg);
	
	
	
	
	
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
		sprintf(find_trigger, "|%s|" , entry->d_name);
		full_path=malloc(sizeof(char) * (strlen(entry->d_name)+strlen(trigger_dir)+2));
		sprintf(full_path, "%s/%s", trigger_dir, entry->d_name);
		if(lstat(full_path, &finfo) < 0) {
			_log("lstat() %s failed", full_path);
			return;	
		}
		if(S_ISREG(finfo.st_mode)) {
			
			for(x=0; x<hdr->wrkcount; x++) {
				if(strstr(wrkmap[x].services, find_str) != NULL || strlen(wrkmap[x].services) == 0 || do_check == 0) {
					if(strstr(wrkmap[x].enabled_triggers, find_trigger) != NULL || strlen(wrkmap[x].enabled_triggers) == 0) {
						
						
						if((bartlby_trigger_escalation(&wrkmap[x], svc)) == FL) continue;
						if((bartlby_trigger_worker_level(&wrkmap[x], svc)) == FL) continue;
						
						
						en.trigger = entry->d_name;
						en.svc = svc;
						en.wrk = &wrkmap[x];
						bartlby_callback(EXTENSION_CALLBACK_TRIGGER_FIRED, &en);
						
						//_log("EXEC trigger: %s", full_path);
						_log("@NOT@%d|%d|%d|%s|%s|%s:%d/%s", svc->service_id, svc->last_state ,svc->current_state,entry->d_name,wrkmap[x].name, svc->server_name, svc->client_port, svc->service_name);
						
						svc->last_notify_send=time(NULL);
						svc->srv->last_notify_send=time(NULL);
						wrkmap[x].escalation_time=time(NULL);
						exec_str=malloc(sizeof(char)*(strlen(full_path)+strlen("\"\"\"\"                         ")+strlen(wrkmap[x].icq)+strlen(wrkmap[x].name)+strlen(notify_msg)+strlen(wrkmap[x].mail)));
						sprintf(exec_str, "%s \"%s\" \"%s\" \"%s\" \"%s\"", full_path, wrkmap[x].mail,wrkmap[x].icq,wrkmap[x].name, notify_msg);
						ptrigger=popen(exec_str, "r");
						if(ptrigger != NULL) {
							connection_timed_out=0;
							alarm(CONN_TIMEOUT);
							if(fgets(trigger_return, 1024, ptrigger) != NULL) {
								trigger_return[strlen(trigger_return)-1]='\0';
								_log("@NOT-EXT@%d|%d|%d|%s|%s|%s:%d/%s|'%s'", svc->service_id, svc->last_state ,svc->current_state,entry->d_name,wrkmap[x].name, svc->server_name, svc->client_port, svc->service_name, trigger_return);
								
      							} else {
      								_log("@NOT-EXT@%d|%d|%d|%s|%s|%s:%d/%s|'(empty output)'", svc->service_id, svc->last_state ,svc->current_state,entry->d_name,wrkmap[x].name, svc->server_name, svc->client_port, svc->service_name);
      							}
      							
      							if(connection_timed_out == 1) {
      								_log("@NOT-EXT@%d|%d|%d|%s|%s|%s:%d/%s|'(timed out)'", svc->service_id, svc->last_state ,svc->current_state,entry->d_name,wrkmap[x].name, svc->server_name, svc->client_port, svc->service_name);
      							}
      							connection_timed_out=0;
							alarm(0);
							if(ptrigger != NULL) {
      								pclose(ptrigger);
      							}
      						} else {
      							_log("@NOT-EXT@%d|%d|%d|%s|%s|%s:%d/%s|'(failed %s)'", svc->service_id, svc->last_state ,svc->current_state,entry->d_name,wrkmap[x].name, svc->server_name, svc->client_port, svc->service_name, full_path);	
      						}
						free(exec_str);
					} else {
						//_log("Worker: %s does not have trigger: %s", wrkmap[x].name, entry->d_name);
					}
				}
			}	
		}
				
		
		
		free(full_path);
	}
	free(find_trigger);
	free(find_str);
	
	
	free(notify_msg);
	closedir(dtrigger);
}

