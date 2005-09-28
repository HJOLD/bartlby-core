#include <ibase.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>


#include <bartlby.h>

#define    MAXLEN    1024

#define AUTOR "Helmut Januschka"
#define NAME "Firebird Connector"
#define DLVERSION  "0.1"

#define SELECTOR "select svc.service_id, svc.service_name, svc.service_state, srv.server_name, srv.server_id, srv.server_port, srv.server_ip, svc.service_plugin, svc.service_args, svc.service_last_check, svc.service_interval, svc.service_text, svc.service_time_from, svc.service_time_from, svc.service_time_to, svc.service_time_to, svc.service_notify, svc.service_type, svc.service_var, svc.service_passive_timeout  from services svc, servers srv where svc.server_id=srv.server_id and svc.service_active=1 and srv.server_enabled=1 ORDER BY svc.service_type asc, svc.server_id"



		//Check functions
		// Autor, Name, Version
		// GetServiceMap
		// SetService
		// GetWorkers
		// SetWorker
		// GetServiceThrewID
		
		
char * GetAutor() {
	
	return strdup(AUTOR);
}
char * GetVersion() {
	
	return strdup(DLVERSION);
}


struct service ** GetServiceMap(char * config) {
	
    	
    	
	return NULL;	
}
