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

int main(int argc, char ** argv) {
	
	char * shmtok;
	int shm_id;
	//int * shm_elements;
	void * bartlby_address;
	
	int x;
	
	
	struct shm_header * shm_hdr;
	struct worker * wrkmap;
	struct service * svcmap;
	
	shmtok = getConfigValue("shm_key", argv[1]);
	if(shmtok == NULL) {
		_log("Unset variable `shm_key'");
		exit(1);
	}
	shm_id = shmget(ftok(shmtok, 32), 0, 0777);
	if(shm_id != -1) {
		bartlby_address=shmat(shm_id,NULL,0);
		shm_hdr=bartlby_SHM_GetHDR(bartlby_address);
		wrkmap=bartlby_SHM_WorkerMap(bartlby_address);
		svcmap=bartlby_SHM_ServiceMap(bartlby_address);
		printf("Services:\n");
		for(x=0; x<shm_hdr->svccount; x++) 
			printf("\t%s:%d/%s %d\n", svcmap[x].server_name, svcmap[x].client_port, svcmap[x].service_name, svcmap[x].current_state);
		printf("Workers:\n");
		for(x=0; x<shm_hdr->wrkcount; x++) 
			printf("\t%s\n", wrkmap[x].mail);
		
		printf("Current running checks: %d\n", shm_hdr->current_running);
		shmdt(bartlby_address);
		
		free(shmtok);
		
	} else {
		_log("SHM doesnt exist is %s running", PROGNAME);
	}	
		
		
	
	return 1;
}
