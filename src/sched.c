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
struct shm_header * gshm_hdr;

void catch_signal(int signum) {
	if(signum == SIGINT) {
		signal(SIGINT, catch_signal);
		do_shutdown=1;
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
	
	
	if(my_diff >= svc->check_interval && tmnow->tm_hour >= svc->hour_from && tmnow->tm_hour <= svc->hour_to && tmnow->tm_min >= svc->min_from && tmnow->tm_min <= svc->min_to) {
		//_log("Mydiff %d >= %d, %d>=%d, %d<=%d %d>=%d %d<=%d", my_diff, svc->check_interval,tmnow->tm_hour,svc->hour_from,tmnow->tm_hour , svc->hour_to,tmnow->tm_min , svc->min_from ,tmnow->tm_min , svc->min_to);	
		
		return 1;
	}	
	return -1;
}

void sched_wait_open() {
	while(current_running != 0) {
			
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
	
	char * cfg_mps;
		

	struct service * services;
	struct shm_header * hdr;
	
	
	
	gshm_hdr=bartlby_SHM_GetHDR(shm_addr);
	
	_log("Scheduler working on %d Services", hdr->svccount);
	
	cfg_mps=getConfigValue("max_concurent_checks", cfgfile);
	if(cfg_mps == NULL) {
		_log("<Warn>Defaulting max_concurent_checks to '20'");
		cfg_max_parallel=20;
	} else {
		cfg_max_parallel=atoi(cfg_mps);
		free(cfg_mps);	
	}
	
	
	signal(SIGINT, catch_signal);
	signal(SIGCHLD, sched_reaper);
	
	services=bartlby_SHM_ServiceMap(shm_addr);
		
	while(1) {
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
														
								
								exit(1);
								
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
		_log("Done %d Services in %d Seconds", round_visitors, time(NULL)-round_start);				
		round_start=time(NULL);
		round_visitors=0;
		sleep(SCHED_PAUSE);
	}
	free(cfg_mps);
	return 1;
	
	
	
}


