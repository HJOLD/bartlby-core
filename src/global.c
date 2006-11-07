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
Revision 1.19  2006/11/07 14:46:58  hjanuschka
*** empty log message ***

Revision 1.18  2006/08/06 02:01:01  hjanuschka
*** empty log message ***

Revision 1.17  2006/08/03 20:52:57  hjanuschka
*** empty log message ***

Revision 1.16  2006/07/25 21:42:03  hjanuschka
auto commit

Revision 1.15  2006/07/23 20:32:54  hjanuschka
*** empty log message ***

Revision 1.14  2006/06/14 22:44:50  hjanuschka
fixing stdout bug on early mysql errors
fixing miss behavior of the extension interface in various code pieces

Revision 1.13  2006/06/04 23:55:28  hjanuschka
core: SSL_connect (timeout issue's solved , at least i hope :))
core: when perfhandlers_enabled == false, you now can enable single services
core: plugin_arguments supports $MACROS
core: config variables try now to cache themselfe to minimize I/O activity
core: .so extensions support added

Revision 1.12  2006/02/10 23:54:46  hjanuschka
SIRENE mode added

Revision 1.11  2006/02/09 00:14:50  hjanuschka
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

Revision 1.10  2006/01/10 22:37:25  hjanuschka
some changes
	trigger msg comes out of cfgfile with some $VAR macros

Revision 1.9  2005/09/28 21:46:30  hjanuschka
converted files to unix
jabber.sh -> disabled core dumps -> jabblibs segfaults
                                    will try to patch it later

Revision 1.8  2005/09/27 18:21:57  hjanuschka
*** empty log message ***

Revision 1.7  2005/09/11 21:42:24  hjanuschka
log files are now archived by Y.M.d

Revision 1.6  2005/09/07 21:51:40  hjanuschka
fixed passive check_fin bug
added bartlby_portier passive results may now be deliverd from remote

Revision 1.5  2005/09/05 20:00:54  hjanuschka
stupid configfile issue fixed

Revision 1.4  2005/09/05 19:53:12  hjanuschka
2 day uptime without a single sigsegv ;-)
added daemon function ;-)
	new cfg vars daemon=[true|false], basedir, logfile

Revision 1.3  2005/08/28 16:02:59  hjanuschka
CVS Header


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

char config_file[255];



void set_cfg(char * cfg) {
	sprintf(config_file, "%s", cfg);	
}





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
		case STATE_SIRENE:
			ret=strdup("SIRENE");
		break;
		
		default:
			ret=strdup("UNKOWN");	
	}
	return ret;			
}

void bartlby_decode(char * msg, int length) {
	int x;
		
	for(x=0; x<length; x++) {
		//msg[x]=2^msg[x];	
		
	}
	
}
void str_mysql_safe(char * str) {
	int x;
	for(x=0; x<=strlen(str); x++) {
		if(str[x] == '\'')
			str[x]=' '; 
	}	
}
void service_mysql_safe(struct service * svc) {
	str_mysql_safe(svc->new_server_text);	
	str_mysql_safe(svc->service_name);
	str_mysql_safe(svc->server_name);
	str_mysql_safe(svc->client_ip);
	str_mysql_safe(svc->plugin);
	str_mysql_safe(svc->plugin_arguments);
	str_mysql_safe(svc->service_var);
	str_mysql_safe(svc->server_icon);
	str_mysql_safe(svc->snmp_info.community);
	str_mysql_safe(svc->snmp_info.objid);
	
}

void bartlby_encode(char * msg, int length) {
	int x;
	for(x=0; x<length; x++) {
		//msg[x]=msg[x]^2;	
	}
	
}

int _log(char * str,  ...) {
//	printf("LOG: %s\n", str);
	
	
	va_list argzeiger;
	time_t tnow;
	struct tm *tmnow;
	
	char * logfile;
	char * logfile_dd;
	FILE * fp;
	
	time(&tnow);
	tmnow = localtime(&tnow);
	
	if(strcmp(config_file, "") != 0) {
		logfile_dd=getConfigValue("logfile", config_file);
		if(logfile_dd == NULL) {
			printf("Logfile not set");
			exit(1);	
		}
		if(strcmp(logfile_dd, "/dev/stdout") != 0) {
			logfile=malloc(sizeof(char) * (strlen(logfile_dd)+50));
			sprintf(logfile, "%s.%02d.%02d.%02d", logfile_dd, tmnow->tm_year + 1900,tmnow->tm_mon + 1,tmnow->tm_mday); 	
		} else {
			logfile=strdup("/dev/stdout");	
		}
		
		
		
	} else {
		logfile_dd=strdup("-");	
		logfile=strdup("/dev/stdout");	
	}
	
	
	
 	va_start(argzeiger,str);
	if(strcmp(logfile, "/dev/stdout") == 0) {
		printf("%02d.%02d.%02d %02d:%02d:%02d;[%d];", tmnow->tm_mday,tmnow->tm_mon + 1,tmnow->tm_year + 1900, tmnow->tm_hour, tmnow->tm_min, tmnow->tm_sec, getpid());
		vprintf(str, argzeiger);
		printf(";\n");
	} else {
		fp=fopen(logfile, "a");   	
		if(fp == NULL) {
			perror(logfile);
			exit(1);	
		}
		fprintf(fp, "%02d.%02d.%02d %02d:%02d:%02d;[%d];", tmnow->tm_mday,tmnow->tm_mon + 1,tmnow->tm_year + 1900, tmnow->tm_hour, tmnow->tm_min, tmnow->tm_sec, getpid());
		vfprintf(fp, str, argzeiger);
		fprintf(fp, ";\n");
		//vprintf(string,argzeiger);
		//fflush(pos2_log_file);
		fclose(fp);
	}
   	
   	va_end(argzeiger);
   	free(logfile);
   	free(logfile_dd);
	return 1;   
}


void str_replace(char *str, const char *from, const char *to, int maxlen)
{
  char  *pstr   = str;
  int   fromlen = strlen(from);
  int   tolen   = strlen(to);
  
  while (*pstr != '\0' && pstr - str < maxlen) {
    if (strncmp(pstr, from, fromlen) != 0) {
      *pstr++;
      continue;
    }
    memmove(pstr + tolen, pstr + fromlen, maxlen - ((pstr + tolen) - str) - 1);
    memcpy(pstr, to, tolen);
    pstr += tolen;
  }
}

void bartlby_replace_svc_in_str(char * str, struct service * svc, int max) {
	char * human_state, * human_state_last, * clport;
	
	clport=malloc(sizeof(char) * 30);
	human_state=bartlby_beauty_state(svc->current_state);
	human_state_last=bartlby_beauty_state(svc->last_state);
	sprintf(clport, "%d", svc->client_port);
	
	str_replace(str,"$READABLE_STATE", human_state, max); 
	setenv("READABLE_STATE", human_state, 1);
	setenv("READABLE_LAST", human_state_last, 1);
	str_replace(str,"$READABLE_LAST", human_state_last, max); 
	setenv("PROGNAME", PROGNAME, 1);
	str_replace(str,"$PROGNAME", PROGNAME, max); 
	setenv("VERSION", VERSION, 1);
	str_replace(str,"$VERSION", VERSION, max); 
	setenv("SERVER", svc->server_name, 1);
	str_replace(str,"$SERVER", svc->server_name, max); 
	setenv("SERVICE", svc->service_name, 1);
	str_replace(str,"$SERVICE", svc->service_name, max); 
	setenv("MESSAGE",  svc->new_server_text, 1);
	str_replace(str,"$MESSAGE", svc->new_server_text, max); 
	setenv("CLIENT_IP", svc->client_ip, 1);
	str_replace(str,"$CLIENT_IP", svc->client_ip, max); 
	setenv("CLIENT_PORT", clport, 1);
	str_replace(str,"$CLIENT_PORT", clport, max); 
	
	
	
	free(clport);
	free(human_state_last);
	free(human_state);
}
