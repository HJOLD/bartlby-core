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


void bartlby_get_daemon(char * cfgfile) {
	pid_t pid;
	
	char * base_dir;
	
	
	base_dir = getConfigValue("basedir", cfgfile);
	if(base_dir == NULL) {
		_log("basedir not set	");
	}
	
	
	if((pid = fork()) != 0) {
		_log("fork failed");
		exit(1);	
	}	
	if(setsid() < 0 ) {
		_log("Cannot setsid()");	
	}
	signal(SIGHUP, SIG_IGN);
	chdir(base_dir);
	umask(0);
	
	
}
