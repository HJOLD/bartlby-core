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
Revision 1.4  2006/04/23 18:07:43  hjanuschka
core/ui/php: checks can now be forced
ui: remote xml special_addon support
core: svc perf MS
core: round perf MS
php: svcmap, get_service perf MS
ui: perf MS

Revision 1.3  2006/02/09 00:14:50  hjanuschka
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

Revision 1.2  2006/01/16 20:51:41  hjanuschka
performance stuff moved to perf.c
timeing information on perf handler

Revision 1.1  2005/12/29 20:05:55  hjanuschka
core statistic (should be used in debug mode only produces a biiiig file)

*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>



#include <bartlby.h>

int bartlby_milli_timediff(struct timeval end, struct timeval start) {
	return ((end.tv_sec - start.tv_sec) * 1000) +  (((1000000 + end.tv_usec - start.tv_usec) / 1000) - 1000);	
}

int bartlby_core_perf_track(struct shm_header * hdr, struct service * svc, int type, int time) {
	
	// be nice to CFG access use env variable :-)
	switch(type) {
		case PERF_TYPE_ROUND_TIME:
			
			hdr->pstat.counter++;
			hdr->pstat.sum += time;
		break;
		case PERF_TYPE_SVC_TIME:
			svc->pstat.counter++;
			svc->pstat.sum += time;
			
		break;
		
		
		default: _log("unknown perf type: %d", type);	
	}
	return -1;	
}

void bartlby_perf_track(struct service * svc,char * return_buffer, int return_bytes, char * cfgfile) {
	struct stat perf_s;
	char perf_out[2048];
	char * cfg_perf_dir;
	char * perf_trigger;
	int perf_child;
	struct timeval stat_end, stat_start;
	
	cfg_perf_dir=getConfigValue("performance_dir", cfgfile);
	if(cfg_perf_dir != NULL) {
		perf_trigger = malloc(sizeof(char) * (strlen(cfg_perf_dir)+50+strlen(svc->plugin)+return_bytes+20));
		sprintf(perf_trigger, "%s/%s", cfg_perf_dir, svc->plugin);
		if(stat(perf_trigger, &perf_s) < 0) {
			_log("Performance Trigger: %s not found", perf_trigger);	
		} else {
			
			sprintf(perf_trigger, "%s/%s %d %s 2>&1 > /dev/null", cfg_perf_dir, svc->plugin, svc->service_id, return_buffer);
			switch(perf_child=fork()) {
				case -1:
					_log("fork error");
				break;
				
				case 0:
					gettimeofday(&stat_start,NULL);
					system(perf_trigger);
					gettimeofday(&stat_end,NULL);
					//_log("@PERF@%d|%s:%d/%s", bartlby_milli_timediff(stat_end,stat_start),svc->server_name,svc->client_port, svc->service_name);
					exit(1);
				break;	
				default:
					//_log("Forked perf trigger %s", perf_trigger);
					
				break;
			}
			
			
		}
		free(perf_trigger);
		
		free(cfg_perf_dir);	
	}	
	
}

