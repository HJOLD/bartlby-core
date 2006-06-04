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
Revision 1.10  2006/06/04 23:55:28  hjanuschka
core: SSL_connect (timeout issue's solved , at least i hope :))
core: when perfhandlers_enabled == false, you now can enable single services
core: plugin_arguments supports $MACROS
core: config variables try now to cache themselfe to minimize I/O activity
core: .so extensions support added

Revision 1.9  2006/05/21 21:18:10  hjanuschka
commit before workweek

Revision 1.8  2006/05/20 20:52:18  hjanuschka
set core dump limit in deamon mode
snmp minimal fixes
announce if SNMP is compiled in on startup

Revision 1.7  2005/12/13 23:17:53  hjanuschka
setuid before creating shm segment

Revision 1.6  2005/11/27 02:04:42  hjanuschka
setuid/setgid for security and web ui

Revision 1.5  2005/09/28 21:46:30  hjanuschka
converted files to unix
jabber.sh -> disabled core dumps -> jabblibs segfaults
                                    will try to patch it later

Revision 1.4  2005/09/25 13:30:18  hjanuschka
cfg: jabber variables
daemon: setenv BARTLBY_HOME (for triggers)
sched: wait_open timeout
mail.sh: sendmail trigger
trigger: $1 == email
$2 == icq
$3 == name
$4 == msg

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
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
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
#include <pwd.h>



#include <bartlby.h>

void bartlby_pre_init(char * cfgfile) {
	FILE * pidfile;
	char * base_dir;
	char pidfname[1024];
	char pidstr[1024];
	char * pid_def_name;
	
	struct rlimit rlim; /* resource limits -kre */
	
	
	
	base_dir = getConfigValue("basedir", cfgfile);
	pid_def_name = getConfigValue("pidfile_dir", cfgfile);
	
	
	
	if(base_dir == NULL) {
		
		base_dir=strdup("/");
	}
	if(pid_def_name == NULL) {
		pid_def_name=strdup(base_dir);
	}
	getrlimit(RLIMIT_CORE, &rlim);
  	rlim.rlim_cur = rlim.rlim_max;
  	setrlimit(RLIMIT_CORE, &rlim);
  	
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
	
	if(setenv("BARTLBY_HOME", base_dir,1) == 0) {
		_log("$BARTLBY_HOME='%s'", base_dir);
	} else {
		_log("setenv $BARTLBY_HOME='%s' failed", base_dir);	
	}
	//freopen("/dev/null", "a", stderr);
	//freopen("/dev/null", "a", stdout);
	
	
	free(base_dir);
	free(pid_def_name);
	
	
}

void bartlby_end_clean(char *cfgfile) {
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
		
	if ((pid = fork ()) != 0) {
		//_log("Fork failed");
      		exit(1);
      	}
      	
	if(setsid() < 0 ) {
		_log("Cannot setsid()");	
	}
	
	
	
	signal(SIGHUP, SIG_IGN);
	
	
	
}
