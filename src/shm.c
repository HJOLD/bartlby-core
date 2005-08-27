#include <string.h>
#include <stdlib.h>



#include <bartlby.h>



struct shm_header * bartlby_SHM_GetHDR(void * shm_addr) {
	return (struct shm_header *)(void *)shm_addr;
}



struct worker * bartlby_SHM_WorkerMap(void * shm_addr) {
	//Is beyond the 3 integers :-)
	struct shm_header * hdr;
	struct service * svcmap;
	hdr=bartlby_SHM_GetHDR(shm_addr);
	
	svcmap=bartlby_SHM_ServiceMap(shm_addr);
	
	return (struct worker *)(void*)&svcmap[hdr->svccount]+20;
}

struct service * bartlby_SHM_ServiceMap(void * shm_addr) {
	//Is beyond the 3 integers :-)
	return (struct service *)(void *)shm_addr+sizeof(struct shm_header);
}
