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
Revision 1.3  2005/09/13 19:43:31  hjanuschka
human readable release code name REL_NAME
fixed printf() in shutdown daemon *fg*

Revision 1.2  2005/09/13 19:29:18  hjanuschka
daemon: pidfile, remove pidfile at end
mysql.c: fixed 2 segfaults under _MALLOC_CHECK=2

Revision 1.1  2005/09/05 19:53:57  hjanuschka
daemon maybe safe shutdown by SIGINT SIGUSR1

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
#include <sys/types.h>
#include <sys/stat.h>



#include <bartlby.h>

void bartlby_end_daemon(char *cfgfile) {
	char * base_dir;
	char pidfname[1024];
	
	char * pid_def_name;
	base_dir = getConfigValue("basedir", cfgfile);
	pid_def_name = getConfigValue("pidfile_dir", cfgfile);
	
	
	if(base_dir == NULL) {
		
		base_dir=strdup("/");
	}
	if(pid_def_name == NULL) {
		pid_def_name=strdup(base_dir);
	}
	sprintf(pidfname, "%s/bartlby.pid", pid_def_name);
	unlink(pidfname);
	_log("%s Pid file removed", pidfname);
	free(base_dir);
	free(pid_def_name);
	
	
}

void bartlby_get_daemon(char * cfgfile) {
	pid_t pid;
	FILE * pidfile;
	char * base_dir;
	char pidfname[1024];
	char pidstr[1024];
	char * pid_def_name;
	
	
	base_dir = getConfigValue("basedir", cfgfile);
	pid_def_name = getConfigValue("pidfile_dir", cfgfile);
	
	
	if(base_dir == NULL) {
		
		base_dir=strdup("/");
	}
	if(pid_def_name == NULL) {
		pid_def_name=strdup(base_dir);
	}

	
	if ((pid = fork ()) != 0) {
		//_log("Fork failed");
      		exit(1);
      	}
      	
	if(setsid() < 0 ) {
		_log("Cannot setsid()");	
	}
	signal(SIGHUP, SIG_IGN);
	chdir(base_dir);
	_log("basedir set to:%s", base_dir);
	umask(0);
	sprintf(pidfname, "%s/bartlby.pid", pid_def_name);
	pidfile=fopen(pidfname, "w");
	if(pidfile == NULL) {
		_log("Pid file  failed '%s'", pidfname);
		
	} else {
		sprintf(pidstr, "%d", getpid());
		fwrite(pidstr, sizeof(char), strlen(pidstr), pidfile);
		fclose(pidfile);
		_log("pidfile is at: '%s'", pidfname);
	}
	free(base_dir);
	free(pid_def_name);
	
	
}
