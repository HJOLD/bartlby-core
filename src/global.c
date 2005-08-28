#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>




#include <bartlby.h>


char * bartlby_beauty_state(int status) {
	char * ret;
	switch(status) {
		case STATE_OK:
			ret=strdup("OK");
		break;
		case STATE_WARNING:
			ret=strdup("WARNING");
			
		break;
		case STATE_CRITICAL:
			ret=strdup("CRITICAL");
		break;
		
		default:
			ret=strdup("UNKOWN-S");	
	}
	return ret;			
}

void bartlby_decode(char * msg, int length) {
	int x;
		
	for(x=0; x<length; x++) {
		msg[x]=2^msg[x];	
		
	}
	
}
void bartlby_encode(char * msg, int length) {
	int x;
	for(x=0; x<length; x++) {
		msg[x]=msg[x]^2;	
	}
	
}

int _log(char * str,  ...) {
//	printf("LOG: %s\n", str);

	va_list argzeiger;
	time_t tnow;
	struct tm *tmnow;
	
	
	
	time(&tnow);
	tmnow = localtime(&tnow);
 	va_start(argzeiger,str);
   
   	
	
   	
   	printf("%02d.%02d.%02d %02d:%02d:%02d;[%d];", tmnow->tm_mday,tmnow->tm_mon + 1,tmnow->tm_year + 1900, tmnow->tm_hour, tmnow->tm_min, tmnow->tm_sec, getpid());
   	vprintf(str, argzeiger);
   	printf(";\n");
   	//vprintf(string,argzeiger);
   	//fflush(pos2_log_file);
   	va_end(argzeiger);
   
	return 1;   
}
