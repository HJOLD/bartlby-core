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
Revision 1.1  2006/04/24 22:20:00  hjanuschka
core: event queue


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
#include <stdarg.h>

#include <bartlby.h>



struct btl_event * evs;
struct shm_header * hdr;

void bartlby_event_init(void * bartlby_address) {
	int x;
	
	hdr=bartlby_SHM_GetHDR(bartlby_address);
	evs=bartlby_SHM_EventMap(bartlby_address);
	hdr->cur_event_index=0;
	
	for(x=0; x<EVENT_QUEUE_MAX; x++) {
		evs[x].evnt_id=0;
		sprintf(evs[x].evnt_message, "(null)");
	}
	_log("Init event queue done %d messages available", x);
}

int bartlby_push_event(int event_id, char * str,  ...) {
//	printf("LOG: %s\n", str);
	
	
	if((hdr->cur_event_index+1) >= EVENT_QUEUE_MAX) {
		_log("Event: %d will reach maximum", (hdr->cur_event_index+1));
		hdr->cur_event_index=0;	
	} else {
		hdr->cur_event_index++;	
		_log("NEW CID: %d", hdr->cur_event_index);
	}

	va_list argzeiger;
	
		
 	va_start(argzeiger,str);
 	
 	evs[hdr->cur_event_index].evnt_id=event_id;
 	
 	
   	
   	vsnprintf(evs[hdr->cur_event_index].evnt_message, 900, str, argzeiger);
   	
   	_log("Event pushed: index=>%d id=>%d, Message=>'%s'", hdr->cur_event_index, evs[hdr->cur_event_index].evnt_id, evs[hdr->cur_event_index].evnt_message);
   	
   	va_end(argzeiger);
   	
   	
   	return 1;   
}

