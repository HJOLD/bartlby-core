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




#include <bartlby.h>

int bartlby_milli_timediff(struct timeval end, struct timeval start) {
	return ((end.tv_sec - start.tv_sec) * 1000) +  (((1000000 + end.tv_usec - start.tv_usec) / 1000) - 1000);	
}

int bartlby_core_perf_track(struct service * svc, int value, int type, char * cfg) {
	// be nice to CFG access use env variable :-)
	char * perf_file;
	FILE * fp;
	int ctim;
	
	perf_file=getConfigValue("core_performance", cfg);	
	if(perf_file != NULL) {
		fp=fopen(perf_file, "a");
		if(fp != NULL) {	
			ctim=time(NULL);
			fprintf(fp, "%d		%d		%s		%s		%s		%d		%d\n", ctim , svc->service_id,svc->server_name,svc->service_name, svc->plugin, value, type);	
			fclose(fp);
			free(perf_file);
			return 0;	
		} else {
			return -2;	
		}
	}
	return -1;	
}
