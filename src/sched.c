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
Revision 1.12  2005/09/18 22:30:28  hjanuschka
replication works now

Revision 1.11  2005/09/18 11:28:12  hjanuschka
replication now works :-)
core: can run as slave and load data from a file instead of data_lib
ui: displays a warning if in slave mode to not add/modify servers/services
portier: recieves and writes shm dump to disk
so hot stand by should be possible ;-)
slave also does service checking

Revision 1.10  2005/09/18 01:33:54  hjanuschka
*** empty log message ***

Revision 1.9  2005/09/11 21:42:24  hjanuschka
log files are now archived by Y.M.d

Revision 1.8  2005/09/05 19:53:12  hjanuschka
2 day uptime without a single sigsegv ;-)
added daemon function ;-)
	new cfg vars daemon=[true|false], basedir, logfile

Revision 1.7  2005/09/03 23:01:13  hjanuschka
datalib api refined
moved to version 0.9.7
reload via SHM

Revision 1.6  2005/09/03 20:11:22  hjanuschka
fixups

added addworker, deleteworker, modifyworker, getworkerbyid

Revision 1.5  2005/08/30 21:00:55  hjanuschka
Signal handling, specialy SIGINT shutdown thingy rethought

Revision 1.4  2005/08/28 22:57:14  hjanuschka
config.c: fixed fclose BUG (too many open files ) missing fclose
service_active is now set by data_lib and acutally used by scheduler

Revision 1.3  2005/08/28 16:02:59  hjanuschka
CVS Header


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

#include <bartlby.h>


#define SCHED_PAUSE 10


void catch_signal(int signum);
int do_shutdown=0;
int current_running=0;
pid_t sched_pid;

struct shm_header * gshm_hdr;

void catch_signal(int signum) {
	pid_t sig_pid;
	if(signum == SIGINT || signum == SIGUSR1) {
		do_shutdown=1;
		sig_pid=getpid();
		if(sig_pid != sched_pid) {
			kill(sched_pid, SIGINT); //Notify scheduler that someone is trying to kill us
			exit(1); // Exid child
				
		}
		
		
		//signal(SIGINT, catch_signal);
		
	}
}

void sched_reschedule(struct service * svc) {
	svc->last_check=time(NULL);
	//_log("Set last_check to %d on %s:%d/%s(%s)", svc->last_check, svc->server_name, svc->client_port, svc->service_name, svc->new_server_text);
}

int sched_check_waiting(struct service * svc) {
	int cur_time;
	int my_diff;
	time_t tnow;
	struct tm *tmnow;
		
	cur_time=time(NULL);
				
	time(&tnow);
	tmnow = localtime(&tnow);
	my_diff=cur_time - svc->last_check;
	
	
	if(svc->service_active == 1 && my_diff >= svc->check_interval && tmnow->tm_hour >= svc->hour_from && tmnow->tm_hour <= svc->hour_to && tmnow->tm_min >= svc->min_from && tmnow->tm_min <= svc->min_to) {
		//_log("Mydiff %d >= %d, %d>=%d, %d<=%d %d>=%d %d<=%d", my_diff, svc->check_interval,tmnow->tm_hour,svc->hour_from,tmnow->tm_hour , svc->hour_to,tmnow->tm_min , svc->min_from ,tmnow->tm_min , svc->min_to);	
		
		return 1;
	}	
	return -1;
}

void sched_wait_open() {
	while(current_running != 0 && do_shutdown == 0) {
			
			sleep(1);
						
	}	
}

void sched_reaper(int signum) {

	 while (waitpid (-1, NULL, WNOHANG) > 0) {
	 	current_running--;
	 	gshm_hdr->current_running--;
	 }
	
	//_log("Child exited cnt @: %d",current_running); 
	
		
}


int schedule_loop(char * cfgfile, void * shm_addr, void * SOHandle) {

	
	
	int x;
	int child_pid;
	int cfg_max_parallel=0;
	int shutdown_waits=0;
	int round_start, round_visitors;
	
	char * i_am_a_slave;
	char * cfg_mps;
		

	struct service * services;
	
	sched_pid=getpid();
	
	
	gshm_hdr=bartlby_SHM_GetHDR(shm_addr);
	
	_log("Scheduler working on %d Services", gshm_hdr->svccount);
	
	cfg_mps=getConfigValue("max_concurent_checks", cfgfile);
	if(cfg_mps == NULL) {
		_log("<Warn>Defaulting max_concurent_checks to '20'");
		cfg_max_parallel=20;
	} else {
		cfg_max_parallel=atoi(cfg_mps);
		free(cfg_mps);	
	}
	
	
	signal(SIGINT, catch_signal);
	signal(SIGUSR1, catch_signal);
	signal(SIGCHLD, sched_reaper);
	
	services=bartlby_SHM_ServiceMap(shm_addr);
	gshm_hdr->do_reload=0;
	
	while(1) {
		if(gshm_hdr->do_reload == 1) {
			_log("queuing Reload");	
			while(current_running != 0) {
				_log("Shutdown wait");
				sleep(1); //Wait for finish
				shutdown_waits++;
				if(shutdown_waits > 20) {
					//Well this is the case where someone wants to keep us waiting
					// women ? :-)	
					return -2;
				}	
			}	
			return -2;
		}
		if(do_shutdown == 1) {
			_log("Exit recieved");	
			while(current_running != 0) {
				_log("Shutdown wait");
				sleep(1); //Wait for finish
				shutdown_waits++;
				if(shutdown_waits > 20) {
					//Well this is the case where someone wants to keep us waiting
					// women ? :-)	
					break;
				}	
			}	
			break;
		 }
		
		
		
		//_log("Exsisting shm with %d elements",  *shm_wrk_cnt);
		round_start=time(NULL);
		round_visitors=0;	
		
		for(x=0; x<gshm_hdr->svccount; x++) {
			
			
			if(current_running < cfg_max_parallel) { 
				if(sched_check_waiting(&services[x]) == 1) {
						round_visitors++;
				 		//services[x].last_check=time(NULL);
						child_pid=fork();
						switch(child_pid) {
							case -1:
								_log("FORK Error");
								return -1;
							break;
							case 0:
								
								//signal(SIGCHLD, sched_reaper);
								
								bartlby_check_service(&services[x], shm_addr, SOHandle, cfgfile);	
								shmdt(shm_addr);
								exit(0);
								
							break;	
							default:
								gshm_hdr->current_running++;
								current_running++;
								
							break;
						}
				
				
				}				
			} else {
				sched_wait_open();	
			}
			
		}
		sched_wait_open();
		//_log("Done %d Services in %d Seconds", round_visitors, time(NULL)-round_start);				
		round_start=time(NULL);
		round_visitors=0;
		sleep(SCHED_PAUSE);
		i_am_a_slave = getConfigValue("i_am_a_slave", cfgfile);
		if(i_am_a_slave == NULL) {
			replication_go(cfgfile, shm_addr, SOHandle);
		} else {
			_log("Skipped repl because me is a slave");	
			free(i_am_a_slave);
			return -2;
		}
		
	}
	return 1;
	
	
	
}


