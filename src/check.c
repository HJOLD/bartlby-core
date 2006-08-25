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
Revision 1.56  2006/08/25 20:10:10  hjanuschka
auto commit

Revision 1.55  2006/08/19 00:07:29  hjanuschka
version bump 1.2.3 (August)
core: passive services get critical after timeout*2

Revision 1.54  2006/08/12 17:44:34  hjanuschka
auto commit

Revision 1.53  2006/08/03 20:52:57  hjanuschka
*** empty log message ***

Revision 1.52  2006/07/28 21:17:44  hjanuschka
auto commit

Revision 1.48  2006/07/18 21:38:23  hjanuschka
core: a major BUG has been discoverd in the first production envorioments
	 when a worker has only selected OK and CRITICAL notifications
	 he always got notified about a change from (unselected) WARNING back to OK
	 this had produce ALOT of unserious OK notifications
	 -- 18-07-06 fixed :-)

core: perfhandlers have been re-worked to only collect data

Revision 1.47  2006/06/21 11:34:30  hjanuschka
fixing trigger bug

Revision 1.46  2006/06/14 22:44:50  hjanuschka
fixing stdout bug on early mysql errors
fixing miss behavior of the extension interface in various code pieces

Revision 1.45  2006/06/04 23:55:28  hjanuschka
core: SSL_connect (timeout issue's solved , at least i hope :))
core: when perfhandlers_enabled == false, you now can enable single services
core: plugin_arguments supports $MACROS
core: config variables try now to cache themselfe to minimize I/O activity
core: .so extensions support added

Revision 1.44  2006/05/24 13:07:39  hjanuschka
NRPE support (--enable-nrpe)

Revision 1.43  2006/05/20 20:26:09  hjanuschka
ui: add/modify server page rebrush (should be user-friendlier)
core: snmp INTEGER greater, lower (warning,critical) (--enable-snmp=yes)
core/lib mysql: SNMP functionality
ui: add/modify_service SNMP
php: SNMP functions

Revision 1.42  2006/05/20 18:29:16  hjanuschka
snmp implented

Revision 1.41  2006/05/12 23:38:02  hjanuschka
*** empty log message ***

Revision 1.40  2006/05/06 23:32:02  hjanuschka
*** empty log message ***

Revision 1.39  2006/05/01 22:11:30  hjanuschka
some sched fixes
and event push immediatly when status change

Revision 1.38  2006/04/24 22:20:00  hjanuschka
core: event queue

Revision 1.37  2006/03/29 16:55:46  hjanuschka
mysql sheme fix -> forgott to add service_retain
check: notify + retain FIXed

Revision 1.36  2006/03/18 01:54:46  hjanuschka
perf: distribute RRDs correspodening to the perf handler
core: sched_timeout refined
core: service_retain
core: lib/mysql service_retain
php: service_retain
ui: service_retain
ui: add perf defaults to package
ui: catch un-existing objects, server|service|worker
ui: exit if either built in nor shared bartlby extension was found (discovered during php upgrade )
ui: addons got own config file (ui-extra.conf)
php: E_WARNING on unexisting config file

Revision 1.35  2006/02/17 20:06:19  hjanuschka
	acknowledgeable services

Revision 1.34  2006/02/12 00:15:34  hjanuschka
Makefile.conf added
Local checks implemented
minor active check fixes and clean ups for re-use with local checks

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
#include <sys/types.h>
#include <sys/wait.h>

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

void bartlby_check_local(struct service * svc, char * cfgfile) {
	struct sigaction act1, oact1;
	char * file_request;
	char c;
	char * plugin_dir;
	FILE * fp;
	int plugin_rtc;
	
	int round;
	char * rmessage, *rmessage_temp;
	struct stat plg_stat;
	
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
	plugin_dir=getConfigValue("agent_plugin_dir", cfgfile);
        if(plugin_dir == NULL) {
        	
        	sprintf(svc->new_server_text, "Plugin dir failed 'agent_plugin_dir' not set");
		svc->current_state=STATE_CRITICAL;		  	
		return;
        }
	connection_timed_out=0;
	file_request=malloc(sizeof(char)*(strlen(svc->plugin)+strlen(svc->plugin_arguments)+30+strlen(plugin_dir)));
	sprintf(file_request, "%s/%s",plugin_dir, svc->plugin);
	
	if(stat(file_request, &plg_stat) < 0) {
		//oops file is not here
		sprintf(svc->new_server_text, "Plugin does not exist");
		svc->current_state=STATE_CRITICAL;		  	
		free(plugin_dir);
		free(file_request);	
		return;
	}
	strcat(file_request, " ");
	strcat(file_request, svc->plugin_arguments);
	
	signal(SIGPIPE,SIG_DFL);
	signal(SIGCHLD,SIG_DFL);
		
	fp=popen(file_request, "r");
	if(fp != NULL) {
		connection_timed_out=0;
		alarm(svc->service_check_timeout);
		
		
		rmessage_temp=malloc(sizeof(char)*(1024*4));
		rmessage=malloc(sizeof(char)*(1024*4));
		memset(rmessage, '\0', sizeof(char)*(1024*4));
		memset(rmessage_temp, '\0', sizeof(char)*(1024*4));
		round=0;
		while((c=fgetc(fp)) != EOF && connection_timed_out != 1 && round < 1024*4){
			rmessage_temp[round]=c;	
			round++;
		}
		plugin_rtc=pclose(fp);
		
		rmessage_temp[round]='\0';
		
		
		alarm(0);
		
		
		
		if(connection_timed_out == 1) {
			//alarm has reached
			
			sprintf(svc->new_server_text, "Timed out");
			svc->current_state=STATE_CRITICAL;
			return;
				
		} 
		
		connection_timed_out = 0;
		sprintf(rmessage, "%d|%s", WEXITSTATUS(plugin_rtc), rmessage_temp); 
		
		bartlby_action_handle_reply(svc, rmessage, cfgfile);
		
		free(rmessage_temp);
		free(rmessage);
		
	} else {
		sprintf(svc->new_server_text, "popen failed on (%s)", file_request);
		svc->current_state=STATE_CRITICAL;
	}	
	free(file_request);
	free(plugin_dir);
	
	return;	
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
	sum_rmessage=0;
	while((return_bytes=recv(client_socket, return_buffer, 1024, 0)) > 0 &&  sum_rmessage < 1024*4 && connection_timed_out == 0) {
		return_buffer[return_bytes] = '\0';
		strcat(rmessage, return_buffer);		
		sum_rmessage += return_bytes;
	}
	
	//_log("ALL: '%s'", rmessage);
	
	//Check if timed out or Receive error
	if(connection_timed_out == 1 || sum_rmessage <= 0) {
		
		if(connection_timed_out == 1) {
			sprintf(svc->new_server_text, "%s", TIMEOUT_ERROR);
		} else if(sum_rmessage <= 0) {
			_log("%d recv error() '%s'", return_bytes, rmessage);
			sprintf(svc->new_server_text, "%s %d", RECV_ERROR, return_bytes);
		} 
		svc->current_state=STATE_CRITICAL;
		free(rmessage);
		return;
	}
	
	alarm(0);
	close(client_socket);
	
	
	bartlby_action_handle_reply(svc, rmessage, cfgfile);
	
	free(rmessage);
	
        return;
		
}
void bartlby_action_handle_reply(struct service * svc, char * rmessage, char * cfgfile) {
	int char_idx=0, cur_char_idx=0;
	
	char * curr_line;
	int data_is_ok;
	
	cur_char_idx=0;
	char_idx=0;
	curr_line=malloc(sizeof(char) * (strlen(rmessage)+20));
	
	
	
	data_is_ok=0;
   	while(char_idx < strlen(rmessage)) {
   		//_log("%c", rmessage[cur_char_idx]);
   		
   		curr_line[cur_char_idx]=rmessage[char_idx];
   		
   		if(rmessage[char_idx] == '\n' || char_idx == strlen(rmessage)) {
   			curr_line[cur_char_idx]='\0';
   			
   			if(strlen(curr_line) > 0) {
   				data_is_ok=bartlby_action_handle_reply_line(svc, curr_line, cfgfile);
   			}
   			
   			cur_char_idx=0;	
   			char_idx++;
   			continue;	
   		}
   		cur_char_idx++;
   		char_idx++;
   	}
   	
	free(curr_line);
	
	if(data_is_ok != 1) {
		//Maybe we did'nt receive any data like 0|Result\n
		sprintf(svc->new_server_text, "%s (1)", PROTOCOL_ERROR);
		svc->current_state=STATE_CRITICAL;	
	}
	
}
int bartlby_action_handle_reply_line(struct service * svc, char * line, char * cfgfile) {
	char * return_token;
	
	if(strlen(line) == 0) {
		return 2;	
	}
	if(strncmp(line, "OS:", 3) == 0) {
		//_log("OS: '%s'", line);
		return 0;	
	}	
	if(strncmp(line, "PERF: ", 6) == 0) {
		//_log("PERF: '%s'", line);
		bartlby_perf_track(svc,line, strlen(line), cfgfile);	
		return 0;
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
        	return 1;
        } else {
        	
        	sprintf(svc->new_server_text, PROTOCOL_ERROR);
        	svc->current_state=STATE_CRITICAL;
        	return 1;
        }
	
	
	
}

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
				
				sprintf(svc->new_server_text, "%s (Service: %d not found)", GROUP_CRITICAL, svc_id);
				svc->current_state=STATE_CRITICAL;
				return;
			}
			
			if(svg->current_state == state) {
				//_log("Service: is not %d\n", svg->current_state );
				sprintf(svc->new_server_text, "%s %s:%d/%s - %d", GROUP_CRITICAL, svg->server_name, svg->client_port, svg->service_name, state);
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

	
	struct shm_header * hdr;
	
	int (*doUpdate)(struct service *,char *);
	
	
	hdr=bartlby_SHM_GetHDR(shm_addr);
	wrkmap=bartlby_SHM_WorkerMap(shm_addr);
	
		
	if(svc->current_state != svc->last_state) {
		svc->service_retain_current=0;
		svc->last_state=svc->current_state;
		
		_log("@LOG@%d|%d|%s:%d/%s|%s", svc->service_id, svc->current_state, svc->server_name, svc->client_port, svc->service_name, svc->new_server_text);
		bartlby_push_event(EVENT_STATUS_CHANGED, "Service-Changed;%d;%s:%d/%s;%d;%s", svc->service_id, svc->server_name, svc->client_port, svc->service_name, svc->current_state, svc->new_server_text);
		bartlby_callback(EXTENSION_CALLBACK_STATE_CHANGED, svc);
	}	
	if(svc->service_retain_current == svc->service_retain && svc->current_state != svc->notify_last_state) {
	
		bartlby_push_event(EVENT_TRIGGER_PUSHED, "Service-Changed;%d;%s:%d/%s;%d;%s", svc->service_id, svc->server_name, svc->client_port, svc->service_name, svc->current_state, svc->new_server_text);
				
		if(svc->current_state == STATE_CRITICAL && svc->recovery_outstanding == RECOVERY_DONE) {
			svc->recovery_outstanding = RECOVERY_OUTSTANDING;	
		}
		
		bartlby_trigger(svc, cfgfile, shm_addr, 1);
		svc->notify_last_state=svc->current_state;
		
		if(svc->current_state == STATE_OK && svc->recovery_outstanding == RECOVERY_OUTSTANDING) {
			svc->recovery_outstanding=RECOVERY_DONE;
			
		}
		
		if(svc->service_ack == ACK_NEEDED && svc->current_state == STATE_CRITICAL) {
			svc->service_ack=ACK_OUTSTANDING;	
		}
		
		
	
	}
	
	//WTF?
	if(svc->service_type != SVC_TYPE_PASSIVE) {
			svc->last_check=time(NULL);
	}
	
	
	bartlby_callback(EXTENSION_CALLBACK_POST_CHECK, svc);
	
	svc->service_retain_current++;
	
	
	char * cfg_instant_wb;
	cfg_instant_wb = getConfigValue("instant_write_back", cfgfile);
	if(cfg_instant_wb == NULL) {
		cfg_instant_wb=strdup("true");	
	}
	
	if(strcmp(cfg_instant_wb, "true") == 0) {
		LOAD_SYMBOL(doUpdate,SOHandle, "doUpdate");
		doUpdate(svc,cfgfile);
	}
	
	free(cfg_instant_wb);
	
	
	
}

void bartlby_check_service(struct service * svc, void * shm_addr, void * SOHandle, char * cfgfile) {
	//_log("check service");
	int ctime, pdiff;
	//_log("<%d/%d -- CHECK >: %s",svc->current_state,svc->last_state, svc->service_name);
	
	setenv("BARTLBY_CONFIG", cfgfile,1);
	setenv("BARTLBY_CURR_PLUGIN", svc->plugin,1);
	setenv("BARTLBY_CURR_HOST", svc->server_name,1);
	setenv("BARTLBY_CURR_SERVICE", svc->service_name,1);
	
	if(bartlby_callback(EXTENSION_CALLBACK_PRE_CHECK, svc) != EXTENSION_OK) {
			/*
			a extension canceld the check
			*/
			return;	
	}	
	if(svc->service_type == SVC_TYPE_GROUP) {
		bartlby_check_group(svc, shm_addr);
		bartlby_fin_service(svc,SOHandle,shm_addr,cfgfile);
		return;	
	}
	if(svc->service_type == SVC_TYPE_PASSIVE) {
		ctime=time(NULL);
		pdiff=ctime-svc->last_check;
		
		if(svc->service_passive_timeout > 0 && pdiff >= svc->service_passive_timeout) {
			
			sprintf(svc->new_server_text, "%s", PASSIVE_TIMEOUT);
			if(pdiff >= svc->service_passive_timeout * 2) {
				svc->current_state=STATE_CRITICAL;
			} else {
				svc->current_state=STATE_WARNING;
			}
			
			
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
	if(svc->service_type == SVC_TYPE_LOCAL) {
		bartlby_check_local(svc,cfgfile);
		bartlby_fin_service(svc,SOHandle,shm_addr,cfgfile);
		return;		
	}
	if(svc->service_type == SVC_TYPE_SNMP) {
		
		bartlby_check_snmp(svc,cfgfile);
		bartlby_fin_service(svc,SOHandle,shm_addr,cfgfile);
		return;
	}
	if(svc->service_type == SVC_TYPE_NRPE) {
		bartlby_check_nrpe(svc, cfgfile, 0);	
		bartlby_fin_service(svc,SOHandle,shm_addr,cfgfile);
		return;
	}                                                       
	if(svc->service_type == SVC_TYPE_NRPE_SSL) {
		bartlby_check_nrpe(svc, cfgfile, 1);	           
		bartlby_fin_service(svc,SOHandle,shm_addr,cfgfile);
		return;
	
	}
	//
	if(bartlby_callback(EXTENSION_CALLBACK_UNKOWN_CHECK_TYPE, svc) != EXTENSION_OK) {
		bartlby_fin_service(svc,SOHandle,shm_addr,cfgfile);
		return;
	} else {
		_log("Undefined service check type: %d", svc->service_type);
		sprintf(svc->new_server_text, "undefined service type (%d)", svc->service_type);
		svc->current_state=STATE_CRITICAL;
	}
	
	return;
}
