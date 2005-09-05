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
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>	
#include <unistd.h>	

#include <dlfcn.h>


#include <bartlby.h>



#define LOAD_SYMBOL(x,y,z) 	x=dlsym(y, z); \
    	if((dlmsg=dlerror()) != NULL) { \
        	_log("Error: %s", dlmsg); \
        	exit(1); \
    	}

int main(int argc, char ** argv) {
	
	
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
	
	int (*GetServiceMap)(struct service *, char *);
	int (*GetWorkerMap)(struct worker *,char *);
	
	
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
		
		struct shmid_ds shm_desc;
		long SHMSize;
		struct shm_header * shm_hdr;
		
	/*
		END SHM stuff
	*/

	
	struct service * svcmap;
	struct worker * wrkmap;

	int exi_code;
	
	
	
	// Parse Config
	if(argc >= 2) {
		set_cfg(argv[1]);
		SOName = getConfigValue("data_library", argv[1]);
		if(SOName == NULL) {
			_log("No data_library specified in `%s' config file", argv[1]);
			exit(1);
		}
	}		
	
	_log("%s Version %s (%s/%s) started", PROGNAME, VERSION, __DATE__,__TIME__);
	daemon_mode=getConfigValue("daemon", argv[1]);
	if(strcmp(daemon_mode,"true") == 0) {	
		bartlby_get_daemon(argv[1]);
	} 	
	
		
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
    	LOAD_SYMBOL(GetName,SOHandle, "GetName");
    	LOAD_SYMBOL(ExpectVersion,SOHandle, "ExpectVersion");
    	
    	 	
    	
    	
    	
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
		SHMSize=2048*2048*2;	
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
			
			_log("Workers: %d", shm_hdr->wrkcount);
			shm_hdr->current_running=0;
			sprintf(shm_hdr->version, "%s-%s (%s/%s)", PROGNAME, VERSION, __DATE__,__TIME__);
			shm_hdr->do_reload=0;
			
			
			
		} else {
			
			
			_log("SHM is already exsisting do a `ipcrm shm SHMID' or something like that");
			system("ipcs -m|grep \"0x\"|awk '{if($2 != 0) {print \"ipcrm shm \" $2; } }'|sh");
			exit(1);
		}
		
		
		exi_code=schedule_loop(argv[1], bartlby_address, SOHandle);
		_log("Scheduler ended with: %d", exi_code);
		
		
		
		//Destroy SHM
		shmdt(bartlby_address);
		shm_id = shmget(ftok(shmtok, 32), 0, 0600);
		shmctl(shm_id, IPC_RMID, &shm_desc);
	}
	
	free(shmtok);
		
	dlclose(SOHandle);	
		
	_log("%s Ended", PROGNAME);	
		
		
		
	return 1;
}
