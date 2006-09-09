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
Revision 1.36  2006/09/09 19:38:34  hjanuschka
auto commit

Revision 1.35  2006/09/03 22:19:47  hjanuschka
auto commit

Revision 1.34  2006/08/03 20:29:13  hjanuschka
auto commit

Revision 1.33  2006/07/25 21:42:03  hjanuschka
auto commit

Revision 1.32  2006/06/04 23:55:28  hjanuschka
core: SSL_connect (timeout issue's solved , at least i hope :))
core: when perfhandlers_enabled == false, you now can enable single services
core: plugin_arguments supports $MACROS
core: config variables try now to cache themselfe to minimize I/O activity
core: .so extensions support added

Revision 1.31  2006/05/28 16:18:27  hjanuschka
commit before release

Revision 1.30  2006/05/24 19:18:35  hjanuschka
version bump

Revision 1.29  2006/05/21 21:18:10  hjanuschka
commit before workweek

Revision 1.28  2006/05/20 20:52:18  hjanuschka
set core dump limit in deamon mode
snmp minimal fixes
announce if SNMP is compiled in on startup

Revision 1.27  2006/04/24 22:20:00  hjanuschka
core: event queue

Revision 1.26  2006/04/23 18:07:43  hjanuschka
core/ui/php: checks can now be forced
ui: remote xml special_addon support
core: svc perf MS
core: round perf MS
php: svcmap, get_service perf MS
ui: perf MS

Revision 1.25  2006/02/25 18:53:22  hjanuschka
setuid too late, pid file was created by root
makefile: copy mysql.shema to $BASEDIR

Revision 1.24  2006/02/25 02:02:46  hjanuschka
core: configure/ --with-user=
core: configure/ install all files and directories with chown $BARTLBY_USER
core: lib/mysql [worker|service|server]_by_id returns negative value if not found

Revision 1.23  2006/02/10 23:54:46  hjanuschka
SIRENE mode added

Revision 1.22  2006/02/05 21:49:47  hjanuschka
*** empty log message ***

Revision 1.21  2006/01/19 23:30:22  hjanuschka
introducing downtime's

Revision 1.20  2006/01/09 23:53:10  hjanuschka
minor changes

Revision 1.19  2006/01/08 16:17:24  hjanuschka
mysql shema^

Revision 1.18  2005/12/13 23:17:53  hjanuschka
setuid before creating shm segment

Revision 1.17  2005/11/16 23:51:29  hjanuschka
version bump 0.9.9a (Exusiai)
replication tests minor fixes

Revision 1.16  2005/10/25 20:36:32  hjanuschka
startup time is'nt reset on cfg reload now

Revision 1.15  2005/09/30 23:58:02  hjanuschka
*** empty log message ***

Revision 1.14  2005/09/28 21:46:30  hjanuschka
converted files to unix
jabber.sh -> disabled core dumps -> jabblibs segfaults
                                    will try to patch it later

Revision 1.13  2005/09/18 11:28:12  hjanuschka
replication now works :-)
core: can run as slave and load data from a file instead of data_lib
ui: displays a warning if in slave mode to not add/modify servers/services
portier: recieves and writes shm dump to disk
so hot stand by should be possible ;-)
slave also does service checking

Revision 1.12  2005/09/18 05:05:43  hjanuschka
compile warnings

Revision 1.11  2005/09/18 04:04:52  hjanuschka
replication interface (currently just a try out)
one instance can now replicate itself to another using portier as a transport way
FIXME: need to sort out a binary write() problem

Revision 1.10  2005/09/13 19:43:31  hjanuschka
human readable release code name REL_NAME
fixed printf() in shutdown daemon *fg*

Revision 1.9  2005/09/13 19:29:18  hjanuschka
daemon: pidfile, remove pidfile at end
mysql.c: fixed 2 segfaults under _MALLOC_CHECK=2

Revision 1.8  2005/09/06 20:59:12  hjanuschka
performance tests fixes addition's

Revision 1.7  2005/09/05 19:53:12  hjanuschka
2 day uptime without a single sigsegv ;-)
added daemon function ;-)
	new cfg vars daemon=[true|false], basedir, logfile

Revision 1.6  2005/09/03 23:01:13  hjanuschka
datalib api refined
moved to version 0.9.7
reload via SHM

Revision 1.5  2005/09/03 20:11:22  hjanuschka
fixups

added addworker, deleteworker, modifyworker, getworkerbyid

Revision 1.4  2005/08/28 18:00:22  hjanuschka
data_lib api extended, service/add/delete/update

Revision 1.3  2005/08/28 15:59:47  hjanuschka
CVS header ;-)

*/


#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>	
#include <unistd.h>	
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>

#include <dlfcn.h>


#include <bartlby.h>




int main(int argc, char ** argv, char ** envp) {
	
	
	/*
		DLStuff
	*/
	char * GetAutorStr;
	char * GetVersionStr;
	char * GetNameStr;
	
	char * daemon_mode;
	
	char * SOName; //Shared library name
	void * SOHandle;
	const char * dlmsg;
	
	
	char * (*GetAutor)();
	char * (*GetVersion)();
	char * (*GetName)();
	long   (*ExpectVersion)();
	
	int global_startup_time;
	
	int (*GetServiceMap)(struct service *, char *);
	int (*GetWorkerMap)(struct worker *,char *);
	int (*GetDowntimeMap)(struct downtime *, char *);
	
	
	struct  shm_counter  * (*GetCounter)(char *);
	struct shm_counter * shmc;
	int suggested_minimum;
	
	long cfg_shm_size_bytes;
	char *  cfg_shm_size;
	/*
		End DL STUFF
	
	*/
	
	/* 
		SHM Stuff
	*/
	char * shmtok;
	int shm_id;
	//int * shm_elements;
	void * bartlby_address;
	int shm_svc_cnt;
	int shm_wrk_cnt;
	int shm_dt_cnt;
	
	struct shmid_ds shm_desc;
	long SHMSize;
	struct shm_header * shm_hdr;
	
	struct service * svcmap;
	struct worker * wrkmap;
	struct downtime * dtmap;
		
	/*
		END SHM stuff
	*/
	
	

	char * cfg_user;
	struct passwd * ui;
	
	int exi_code;
	
	if(argc >= 2) {
		set_cfg(argv[1]);
	} else {
		printf("config file missing\n");	
		exit(1);
	}
	
	cfg_user = getConfigValue("user", argv[1]);
	if(cfg_user == NULL) {
		_log("user not set in config file");
		exit(2);			
	}
	ui=getpwnam(cfg_user);
	if(ui == NULL) {
		_log("User: %s not found cannot setuid running as %d", cfg_user, getuid());	
	} else {
		setuid(ui->pw_uid);
		setgid(ui->pw_gid);
		_log("User: %s/%d", ui->pw_name, ui->pw_gid);	
	}
	
	
	global_startup_time=time(NULL);
	
	// Parse Config
	
	SOName = getConfigValue("data_library", argv[1]);
	if(SOName == NULL) {
		_log("No data_library specified in `%s' config file", argv[1]);
		exit(1);
	}
		
	
	_log("%s Version %s (%s) started. compiled %s/%s", PROGNAME, VERSION,REL_NAME, __DATE__, __TIME__);
	#ifdef SNMP_ADDON
	_log("SNMP support compiled in");
	#endif
	#ifdef HAVE_SSL
	_log("SSL support compiled in");
	#endif
	#ifdef WITH_NRPE
	_log("NRPE Support compiled in");
	#endif
	
	
	daemon_mode=getConfigValue("daemon", argv[1]);
	if(daemon_mode == NULL) {
		daemon_mode=strdup("false");	
	}
	if(strcmp(daemon_mode,"true") == 0) {	
		
		bartlby_get_daemon(argv[1]);
	} 	
	
	bartlby_pre_init(argv[1]);
	_log("using data lib: `%s'", SOName);
	SOHandle=dlopen(SOName, RTLD_LAZY);
		
    	if((dlmsg=dlerror()) != NULL) {
        	_log("Error: %s", dlmsg);
        	exit(1);
    	}
	LOAD_SYMBOL(GetAutor,SOHandle, "GetAutor");
    	LOAD_SYMBOL(GetVersion,SOHandle, "GetVersion");
    	LOAD_SYMBOL(GetServiceMap,SOHandle, "GetServiceMap");
    	LOAD_SYMBOL(GetWorkerMap,SOHandle, "GetWorkerMap");
    	LOAD_SYMBOL(GetDowntimeMap,SOHandle, "GetDowntimeMap");
    	LOAD_SYMBOL(GetName,SOHandle, "GetName");
    	LOAD_SYMBOL(ExpectVersion,SOHandle, "ExpectVersion");
    	LOAD_SYMBOL(GetCounter,SOHandle, "GetCounter");
    	
    	
    	 	
    	
    	
    	
    	GetAutorStr=GetAutor();
    	GetVersionStr=GetVersion();
    	GetNameStr=GetName();
    	
    	if(ExpectVersion() > EXPECTCORE || EXPECTCORE < ExpectVersion() || EXPECTCORE != ExpectVersion()) {
    		_log("*****Version check failed Module is compiled for version '%d' of %s", ExpectVersion(), PROGNAME);	
    		_log("*****The Module is compiled under '%d' Version of %s", EXPECTCORE, PROGNAME);
    		exit(1);
    	} 
    	
    	_log("Data Lib (%s) by: '%s' Version: %s", GetNameStr, GetAutorStr, GetVersionStr);
    		
	
	
	shmtok = getConfigValue("shm_key", argv[1]);
	if(shmtok == NULL) {
		_log("Unset variable `shm_key'");
		exit(1);
	}
	
	free(GetAutorStr);
	free(GetVersionStr);
	free(GetNameStr);
	free(SOName);
	
	exi_code=0;
		
	while(exi_code != 1) {
		
		
		cfg_shm_size = getConfigValue("shm_size", argv[1]);
		
		if(cfg_shm_size==NULL) {
			cfg_shm_size_bytes=10;		
		} else {
			cfg_shm_size_bytes=atol(cfg_shm_size);
		}
		
		free(cfg_shm_size);
		
		shmc = GetCounter(argv[1]);
		if(shmc == NULL) {
			exit(0);	
		}
		SHMSize=cfg_shm_size_bytes*1024*1024;	
		
		suggested_minimum = (sizeof(struct shm_header) + (sizeof(struct worker) * shmc->worker) + (sizeof(struct service) * shmc->services) + (sizeof(struct downtime) * shmc->downtimes) + 2000 + (sizeof(struct btl_event)*EVENT_QUEUE_MAX)) * 2;
		if(SHMSize <= suggested_minimum) {
			_log("SHM is to small minimum: %d KB ", suggested_minimum/1024);
			exit(1);	
		}
		_log("SHM requires: %d KB ", suggested_minimum/1024);
		_log("Size: S=%d, W=%d, D=%d, H=%d, E=%d", sizeof(struct service), sizeof(struct worker), sizeof(struct downtime), sizeof(struct shm_header), sizeof(struct btl_event));
		free(shmc);
		
		
		
		
		
		shm_id = shmget(ftok(shmtok, 32), SHMSize,IPC_CREAT | IPC_EXCL | 0777);
		
		if(shm_id != -1) {
			bartlby_address=shmat(shm_id,NULL,0);
			
			shm_hdr=(struct shm_header *)(void *)bartlby_address;
			svcmap=(struct service *)(void *)bartlby_address+sizeof(struct shm_header);
			
							
				
			shm_svc_cnt=GetServiceMap(svcmap, argv[1]);
			
			shm_hdr->svccount=shm_svc_cnt;
			
			svcmap=bartlby_SHM_ServiceMap(bartlby_address);
			
			wrkmap=(struct worker *)(void*)&svcmap[shm_svc_cnt]+20;
			shm_wrk_cnt=GetWorkerMap(wrkmap, argv[1]);
			shm_hdr->wrkcount=shm_wrk_cnt;
			
			dtmap=(struct downtime *)(void *)&wrkmap[shm_wrk_cnt]+20;
			shm_dt_cnt=GetDowntimeMap(dtmap, argv[1]);
			shm_hdr->dtcount=shm_dt_cnt;
				
				
			//06.04.24 Init EVENT QUEUE
			bartlby_event_init(bartlby_address);
			cfg_init_cache();
			bartlby_ext_init(bartlby_address, SOHandle, argv[1]);
				
			
			_log("Workers: %d", shm_hdr->wrkcount);
			_log("Downtimes: %d", shm_hdr->dtcount);
			shm_hdr->current_running=0;
			sprintf(shm_hdr->version, "%s-%s (%s)", PROGNAME, VERSION, REL_NAME);
						
			shm_hdr->do_reload=0;
			shm_hdr->last_replication=-1;
			//shm_hdr->startup_time=time(NULL);
			shm_hdr->startup_time=global_startup_time;
			
			shm_hdr->sirene_mode=0; //Default disable	
			shm_hdr->size_of_structs=sizeof(struct shm_header)+sizeof(struct worker)+sizeof(struct service)+sizeof(struct downtime);
			shm_hdr->pstat.sum=0;
			shm_hdr->pstat.counter=0;
			
			
			
			
			
			if(shm_hdr->wrkcount <= 0) {
				_log("Found workers are below zero (%d) maybe your datalib config isnt OK or you havent completed the setup", shm_hdr->wrkcount);
				shmdt(bartlby_address);
				shm_id = shmget(ftok(shmtok, 32), 0, 0600);
				shmctl(shm_id, IPC_RMID, &shm_desc);
				break;
				
			}
			
			
		} else {
			
			
			_log("SHM is already exsisting do a `ipcrm shm SHMID' or something like that");
			exit(1);
		}
		
		exi_code=schedule_loop(argv[1], bartlby_address, SOHandle);
		_log("Scheduler ended with: %d", exi_code);
		
		//Destroy SHM
		bartlby_ext_shutdown(exi_code);
		//write back all services 
		sched_write_back_all(argv[1], bartlby_address, SOHandle);
		
		shmdt(bartlby_address);
		shm_id = shmget(ftok(shmtok, 32), 0, 0600);
		shmctl(shm_id, IPC_RMID, &shm_desc);
		
		
	}
	
	free(shmtok);
		
	dlclose(SOHandle);	
		
	_log("%s Ended(Daemon: %s)", PROGNAME, daemon_mode);	
		
	//remove PidFile
	bartlby_end_clean(argv[1]);
	free(daemon_mode);
	free(cfg_user);
	return 1;
}
