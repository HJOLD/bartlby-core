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

	
	
	_log("%s Version %s started %d", PROGNAME, VERSION, argc);
	// Parse Config
	if(argc == 2) {
		SOName = getConfigValue("data_library", argv[1]);
		if(SOName == NULL) {
			_log("No data_library specified in `%s' config file", argv[1]);
			exit(1);
		}
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
    	
    	_log("Data Lib (%s) by: '%s' Version: %s %d/%d", GetNameStr, GetAutorStr, GetVersionStr, ExpectVersion(), EXPECTCORE);
    		
	
	
	shmtok = getConfigValue("shm_key", argv[1]);
	if(shmtok == NULL) {
		_log("Unset variable `shm_key'");
		exit(1);
	}
	
	
	
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
		sprintf(shm_hdr->version, "%s-%s", PROGNAME, VERSION);
		
		free(GetAutorStr);
		free(GetVersionStr);
		free(GetNameStr);
		free(SOName);
		
	} else {
		
		free(GetAutorStr);
		free(GetVersionStr);
		free(GetNameStr);
		free(SOName);
		_log("SHM is already exsisting do a `ipcrm shm SHMID' or something like that");
		system("ipcs -m|grep \"0x\"|awk '{if($2 != 0) {print \"ipcrm shm \" $2; } }'|sh");
		exit(1);
	}
	
	
	schedule_loop(argv[1], bartlby_address, SOHandle);
	
	
	
	//Destroy SHM
	shm_id = shmget(ftok(shmtok, 32), 0, 0600);
	shmctl(shm_id, IPC_RMID, &shm_desc);
		
	
	free(shmtok);
		
	dlclose(SOHandle);	
		
	_log("%s Ended", PROGNAME);	
		
		
		
	return 1;
}
