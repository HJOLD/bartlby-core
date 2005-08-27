#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>




#include <bartlby.h>



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
