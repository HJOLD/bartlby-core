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
Revision 1.25  2006/02/17 20:06:19  hjanuschka
	acknowledgeable services

Revision 1.24  2006/02/10 23:54:46  hjanuschka
SIRENE mode added

Revision 1.23  2006/01/19 23:30:22  hjanuschka
introducing downtime's

Revision 1.22  2006/01/09 23:53:10  hjanuschka
minor changes

Revision 1.21  2005/12/29 20:05:55  hjanuschka
core statistic (should be used in debug mode only produces a biiiig file)

Revision 1.20  2005/12/25 12:55:45  hjanuschka
service_check_timeout is dynamic now

Revision 1.19  2005/10/13 22:13:14  hjanuschka
logging improved, check fixup

Revision 1.18  2005/10/09 14:44:09  hjanuschka
agent announces OS and version

Revision 1.17  2005/09/28 21:46:30  hjanuschka
converted files to unix
jabber.sh -> disabled core dumps -> jabblibs segfaults
                                    will try to patch it later

Revision 1.16  2005/09/28 21:37:23  hjanuschka
*** empty log message ***

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
#include <sys/time.h>


#include <bartlby.h>



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

int sched_needs_ack(struct service * svc) {
	if(svc->service_ack == ACK_OUTSTANDING) {
		return 1; //Skip it something unAcked found
	} else {
		return 0; // No doIT
	}	
}

int sched_check_waiting(void * shm_addr, struct service * svc) {
	int cur_time;
	int my_diff;
	time_t tnow;
	struct tm *tmnow;
		
	cur_time=time(NULL);
				
	time(&tnow);
	tmnow = localtime(&tnow);
	my_diff=cur_time - svc->last_check;
	
	if(sched_needs_ack(svc) == 1) {
		//_log("Service: %s is in status outstanding", svc->service_name);
		return -1; //Dont sched this	
	}
	
	if(svc->service_active == 1 && my_diff >= svc->check_interval && tmnow->tm_hour >= svc->hour_from && tmnow->tm_hour <= svc->hour_to && tmnow->tm_min >= svc->min_from && tmnow->tm_min <= svc->min_to) {
		//_log("Mydiff %d >= %d, %d>=%d, %d<=%d %d>=%d %d<=%d", my_diff, svc->check_interval,tmnow->tm_hour,svc->hour_from,tmnow->tm_hour , svc->hour_to,tmnow->tm_min , svc->min_from ,tmnow->tm_min , svc->min_to);	
		if(bartlby_is_in_downtime(shm_addr, svc) > 0) {
			return 1;
		}
	}	
	return -1;
}

void sched_wait_open() {
	int x;
	x=0;
	int olim;
	if(current_running == 0) {
		olim=200;	
	} else {
		olim=current_running*20;
	}
	
	while(current_running != 0 && do_shutdown == 0 && x < olim) {
			
			sleep(1);
			x++;
						
	}	
	if(x >= olim) {
		current_running=0;
		gshm_hdr->current_running=0;
		_log("Sched_wait_open: timedout");	
	}
}

void sched_reaper(int signum) {

	 while (waitpid (-1, NULL, WNOHANG) > 0) {
	 	
	 	if(gshm_hdr->current_running > 0) {
	 		
	 		gshm_hdr->current_running--;
	 	}
	 	if( current_running > 0 ) {
	 		current_running--;	
	 	}
	 }
	
	//_log("Child exited cnt @: %d",current_running); 
	
		
}


int schedule_loop(char * cfgfile, void * shm_addr, void * SOHandle) {

	
	
	int x;
	int child_pid;
	int cfg_max_parallel=0;
	int shutdown_waits=0;
	int round_start, round_visitors;
	char * cfg_sched_pause;
	int sched_pause;
	
	struct timeval check_start, check_end, stat_round_start, stat_round_end;
	
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
	
	cfg_sched_pause = getConfigValue("sched_round_pause", cfgfile);
	if(cfg_sched_pause == NULL) {
		sched_pause=10;	
		_log("info: sched_pause defaulted to: %d Seconds (set sched_round_pause to modify)", sched_pause);
	} else {
		sched_pause=atoi(cfg_sched_pause);
		free(cfg_sched_pause);
	}
	
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
		
		if(gshm_hdr->sirene_mode == 1) {
			//We are in Sirene Mode dont check anything just notifie workers that something b ad is going on
			bartlby_check_sirene(cfgfile,shm_addr);
			continue;	
		}
		
		//_log("Exsisting shm with %d elements",  *shm_wrk_cnt);
		round_start=time(NULL);
		gettimeofday(&stat_round_start,NULL);
		round_visitors=0;	
		
		for(x=0; x<gshm_hdr->svccount; x++) {
			
			
			if(current_running < cfg_max_parallel) { 
				if(sched_check_waiting(shm_addr, &services[x]) == 1) {
						//_log("SVC timeout: %d", services[x].service_check_timeout);
						round_visitors++;
				 		//services[x].last_check=time(NULL);
						child_pid=fork();
						switch(child_pid) {
							case -1:
								_log("FORK Error");
								return -1;
							break;
							case 0:
								
								signal(SIGCHLD, sched_reaper);
								
								gettimeofday(&check_start, NULL);
																
								bartlby_check_service(&services[x], shm_addr, SOHandle, cfgfile);	
								
								gettimeofday(&check_end, NULL);
								
								
								bartlby_core_perf_track(&services[x], bartlby_milli_timediff(check_end,check_start), PERF_TYPE_SVC_TIME, cfgfile);
								
								
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
		if(time(NULL)-round_start > sched_pause) {
			_log("Done %d Services in %d Seconds", round_visitors, time(NULL)-round_start);				
		}
		round_start=time(NULL);
		round_visitors=0;
		
		//Log Round End
		gettimeofday(&stat_round_end,NULL);
		bartlby_core_perf_track(&services[0], bartlby_milli_timediff(stat_round_end,stat_round_start), PERF_TYPE_ROUND_TIME, cfgfile);
		
		
		sleep(sched_pause);
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


