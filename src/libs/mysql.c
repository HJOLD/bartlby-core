#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <time.h>

#include <bartlby.h>

#define CHK_ERR(x) \
		if (x != NULL) {\
			if(mysql_errno(x) != 0) {\
		 		_log("mysql error: %s", mysql_error(x)); \
      		 		return -1; \
      			}\
      		} else {\
      			_log("Null Exception %s:%d", __FILE__, __LINE__); \
      			return -1; \
      		}

#define AUTOR "Helmut Januschka \"helmut@januschka.com\" http://www.januschka.com/BARTLBY"
#define NAME "MYSQL Connector"
#define DLVERSION  "0.1"



#define SELECTOR "select svc.service_id, svc.service_name, svc.service_state, srv.server_name, srv.server_id, srv.server_port, srv.server_ip, svc.service_plugin, svc.service_args, UNIX_TIMESTAMP(svc.service_last_check), svc.service_interval, svc.service_text, HOUR(svc.service_time_from), MINUTE(svc.service_time_from), HOUR(svc.service_time_to), MINUTE(svc.service_time_to), svc.service_notify, svc.service_type, svc.service_var, svc.service_passive_timeout  from services svc, servers srv where svc.server_id=srv.server_id and svc.service_active=1 and srv.server_enabled=1 ORDER BY svc.service_type asc, svc.server_id"
#define WORKER_SELECTOR "select worker_mail, worker_icq, enabled_services,notify_levels from workers where worker_active=1"
#define SERVICE_UPDATE_TEXT "update services set service_last_check=FROM_UNIXTIME(%d), service_text='%s', service_state=%d where service_id=%d"

		//Check functions
		// Autor, Name, Version
		// GetServiceMap
		// SetService
		// GetWorkers
		// SetWorker
		// GetServiceThrewID
		
char * GetName() {
	
	return strdup(NAME);
}
long ExpectVersion() {
	return EXPECTCORE;	
}	
char * GetAutor() {
	
	return strdup(AUTOR);
}
char * GetVersion() {
	
	return strdup(DLVERSION);
}

int doUpdate(struct service * svc, char * config) {
	//_log("Update: %s:%d/%s to: %d", svc->server_name, svc->client_port, svc->service_name, svc->current_state);
	
	
	MYSQL *mysql;
	
	char * sqlupd;
	


	char * mysql_host = getConfigValue("mysql_host", config);
	char * mysql_user = getConfigValue("mysql_user", config);
	char * mysql_pw = getConfigValue("mysql_pw", config);
	char * mysql_db = getConfigValue("mysql_db", config);

	mysql=mysql_init(NULL);
		CHK_ERR(mysql);
	mysql=mysql_real_connect(mysql, mysql_host, mysql_user, mysql_pw, NULL, 0, NULL, 0);
		CHK_ERR(mysql);
	mysql_select_db(mysql, mysql_db);
      		CHK_ERR(mysql);
	
	
	
	
	sqlupd=malloc(strlen(SERVICE_UPDATE_TEXT)+strlen(svc->new_server_text)+255);
	memset(sqlupd,'\0', strlen(SERVICE_UPDATE_TEXT)+strlen(svc->new_server_text)+10);
	sprintf(sqlupd, SERVICE_UPDATE_TEXT, svc->last_check, svc->new_server_text, svc->current_state, svc->service_id);
	
	//Log("dbg", sqlupd);
	mysql_query(mysql, sqlupd);
		CHK_ERR(mysql);
	
	
	free(sqlupd);
		
	mysql_close(mysql);
		
	free(mysql_host);
	free(mysql_user);
	free(mysql_pw);
	free(mysql_db);
	return 1;
}

int GetWorkerMap(struct worker * svcs, char * config) {
	
	MYSQL *mysql;
	MYSQL_ROW  row;
	MYSQL_RES  *res;
	
	
	char * mysql_host = getConfigValue("mysql_host", config);
	char * mysql_user = getConfigValue("mysql_user", config);
	char * mysql_pw = getConfigValue("mysql_pw", config);
	char * mysql_db = getConfigValue("mysql_db", config);
	int i=0;
	
	


	mysql=mysql_init(NULL);
		CHK_ERR(mysql);
	mysql=mysql_real_connect(mysql, mysql_host, mysql_user, mysql_pw, NULL, 0, NULL, 0);
		CHK_ERR(mysql);
      	mysql_select_db(mysql, mysql_db);
      		CHK_ERR(mysql);
	
	
	mysql_query(mysql, WORKER_SELECTOR);
		CHK_ERR(mysql);
      	res = mysql_store_result(mysql);
      		CHK_ERR(mysql);
      	
      	
      	if(mysql_num_rows(res) > 0) {
      		
      		while ( (row=mysql_fetch_row(res)) != NULL) {
      			
      			
      			
      			
      			if(row[0] != NULL) {
      				//svcs[i].mail=malloc(strlen(row[0])*sizeof(char)+2);
      				sprintf(svcs[i].mail, "%s", row[0]);
      			} else {
      				sprintf(svcs[i].mail, "(null)");     				
      			}
      			
      			if(row[1] != NULL) {
      				//svcs[i].icq=malloc(strlen(row[1])*sizeof(char)+2);
      				sprintf(svcs[i].icq, "%s", row[1]);
      			} else {
      				sprintf(svcs[i].icq, "(null)");     				
      			}
      			
      			if(row[2] != NULL) {
      				//svcs[i].services=malloc(strlen(row[2])*sizeof(char)+2);
      				sprintf(svcs[i].services, "%s", row[2]);
      			} else {
      				//svcs[i].services=NULL;   
      				sprintf(svcs[i].services, "(null)");     				  				
      			}
      			if(row[3] != NULL) {
      				sprintf(svcs[i].notify_levels, "%s", row[3]);
      					
      			} else {
      				sprintf(svcs[i].notify_levels, "(null)");	
      			}
      			
      			svcs[i].escalation_count=0;
      			svcs[i].escalation_time=time(NULL);
      			i++;
      		}
      		
      		mysql_free_result(res);
      		mysql_close(mysql);
      		free(mysql_host);
		free(mysql_user);
		free(mysql_pw);
		free(mysql_db);
      		return i;
      	} else { 
      		_log( "no worker found!");	
      	}
	
	
	
	
	free(mysql_host);
	free(mysql_user);
	free(mysql_pw);
	free(mysql_db);
	
	return -1;
	
	
}
int GetServiceMap(struct service * svcs, char * config) {
	
	MYSQL *mysql;
	MYSQL_ROW  row;
	MYSQL_RES  *res;
	
	char * mysql_host = getConfigValue("mysql_host", config);
	char * mysql_user = getConfigValue("mysql_user", config);
	char * mysql_pw = getConfigValue("mysql_pw", config);
	char * mysql_db = getConfigValue("mysql_db", config);
	int i=0;
	
	mysql=mysql_init(NULL);
		CHK_ERR(mysql);
	mysql=mysql_real_connect(mysql, mysql_host, mysql_user, mysql_pw, NULL, 0, NULL, 0);
		CHK_ERR(mysql);
      	mysql_select_db(mysql, mysql_db);
      		CHK_ERR(mysql);
      		
      	mysql_query(mysql, SELECTOR);
		CHK_ERR(mysql);
      	res = mysql_store_result(mysql);
      		CHK_ERR(mysql);
      		
      		
	if(mysql_num_rows(res) > 0) {
      		
      		
      		while ( (row=mysql_fetch_row(res)) != NULL) {
      			
      			svcs[i].service_id=atoi(row[0]);
      			svcs[i].server_id=atoi(row[4]);
      			svcs[i].last_state=atoi(row[2]);
      			svcs[i].current_state=atoi(row[2]);
      			
      			if(row[1] != NULL) {
      				//svcs[i].service_name=malloc(strlen(row[1])*sizeof(char)+2);
      				sprintf(svcs[i].service_name, "%s", row[1]);
      				
      			} else {
      				//svcs[i].service_name=NULL;     				
      				sprintf(svcs[i].service_name, "(null)");
      			}
      			
      			
      			if(row[3] != NULL) {
      				//svcs[i].server_name=malloc(strlen(row[3])*sizeof(char)+2);
      				sprintf(svcs[i].server_name, "%s", row[3]);
      				
      			} else {
//    				svcs[i].server_name=NULL;   
      				sprintf(svcs[i].server_name, "(null)");  				
      			}
      			
      			
      			if(row[6] != NULL) {
      				//svcs[i].client_ip=malloc(strlen(row[6])*sizeof(char)+2);
      				sprintf(svcs[i].client_ip, "%s", row[6]);
      				
      			} else {
      				  
      				sprintf(svcs[i].client_ip, "(null)");     				
      			}
      			
      			
      			svcs[i].client_port=atoi(row[5]);
      			
//     			svcs[i].new_server_text=malloc(strlen(row[11])*sizeof(char)+2);
      			
      			sprintf(svcs[i].new_server_text, "%s", row[11]);
      			
      			///svcs[i].new_server_text=row[11];
      			
      			
      			
      			
      			if(row[7] != NULL) {
      				//svcs[i].plugin=malloc(strlen(row[7])*sizeof(char)+2);
      				sprintf(svcs[i].plugin, "%s", row[7]);
      				
      			} else {
      				//svcs[i].plugin=NULL; 
      				sprintf(svcs[i].plugin, "(null)");       				
      			}
      			
      			if(row[8] != NULL) {
      				//svcs[i].plugin_arguments=malloc(strlen(row[8])*sizeof(char)+2);
      				sprintf(svcs[i].plugin_arguments, "%s", row[8]);
      				
      			} else {
      				//svcs[i].plugin_arguments=NULL; 
      				sprintf(svcs[i].plugin_arguments, "(null)");      				
      			}
      			
      			
      			svcs[i].last_check=atoi(row[9]);
      			
      			svcs[i].check_interval=atoi(row[10]);
      			
      			/*if(i > POS2_QUEUER_THREADS*POS2_QUEUER_THREADS*POS2_QUEUER_THREADS) {
      				ht += 10;
      				svcs[i].check_interval=ht;
      				_log("New distance : %d", svcs[i].check_interval);
      			}*/
      			
      			
      			svcs[i].hour_from=atoi(row[12]);
      			svcs[i].min_from=atoi(row[13]);
      			
      			svcs[i].hour_to=atoi(row[14]);
      			svcs[i].min_to=atoi(row[15]);
      			
      			svcs[i].notify_enabled=atoi(row[16]);
      			svcs[i].last_notify_send=time(NULL);
      			
      			//svc.service_type, svc.service_var, svc.service_passive_timeout
      			svcs[i].service_type = atoi(row[17]);
      			
      			if(row[18] != NULL) {
      				//svcs[i].service_var=malloc(strlen(row[18])*sizeof(char)+2);
      				sprintf(svcs[i].service_var, "%s", row[18]);
      				
      			} else {
      				//svcs[i].service_var=NULL;
      				sprintf(svcs[i].service_var, "(null)");
      			}
      			
      			svcs[i].service_passive_timeout=atoi(row[19]);
      			
      			
      			svcs[i].flap_count=0;
      			
      			
      			//Log("load", "%s -> %s", svcs[i].plugin, svcs[i].plugin_arguments);
      			//int notify_enabled;
			//int last_notify_send;
			//int flap_count;
      			//, HOUR(svc.service_time_from), MINUTE(svc.service_time_from), HOUR(svc.service_time_to), MINUTE(svc.service_time_to)
      				
      			i++;
      		}
      		
      		
      		mysql_free_result(res);
      		mysql_close(mysql);
      		free(mysql_host);
		free(mysql_user);
		free(mysql_pw);
		free(mysql_db);
      		return i;
      	} else { 
      		_log("no services found!");	
      	}
    	
    	free(mysql_host);
	free(mysql_user);
	free(mysql_pw);
	free(mysql_db);
	return -1;	
}

void clone_me() {
	printf("Go and create a	database:\n");
	
	printf("	CREATE TABLE `workers` ( \n");
	printf("`worker_id` int(12) NOT NULL auto_increment,\n");
  	printf("`worker_name` varchar(255) NOT NULL default '',\n");
  	printf("`worker_mail` varchar(255) NOT NULL default '',\n");
  	printf("`worker_icq` int(20) NOT NULL default '0',\n");
  	printf("`worker_active` int(2) NOT NULL default '0',\n");
  	printf("`password` varchar(255) NOT NULL default '',\n");
  	printf("`status` varchar(255) NOT NULL default '',\n");
  	printf("`enabled_services` varchar(255) NOT NULL default '',\n");
  	printf("`notify_levels` varchar(50) NOT NULL default '',\n");
  	printf("PRIMARY KEY  (`worker_id`)\n");
	printf(");");

	printf("CREATE TABLE `services` (\n");
  	printf("`service_id` int(12) NOT NULL auto_increment,\n");
  	printf("`server_id` int(12) NOT NULL default '0',\n");
  	printf("`service_plugin` varchar(255) NOT NULL default '',\n");
  	printf("`service_name` varchar(255) NOT NULL default '',\n");
  	printf("`service_state` int(12) NOT NULL default '0',\n");
  	printf("`service_text` varchar(255) NOT NULL default '',\n");
  	printf("`service_args` varchar(255) NOT NULL default '',\n");
  	printf("`service_last_check` datetime NOT NULL default '0000-00-00 00:00:00',\n");
  	printf("`service_notify` int(2) NOT NULL default '0',\n");
  	printf("`service_active` int(2) NOT NULL default '1',\n");
  	printf("`service_current` int(2) NOT NULL default '0',\n");
  	printf("`service_flapping` datetime NOT NULL default '0000-00-00 00:00:00',\n");
  	printf("`service_time_from` time NOT NULL default '00:00:00',\n");
  	printf("`service_time_to` time NOT NULL default '00:00:00',\n");
  	printf("`service_interval` int(255) NOT NULL default '1',\n");
  	printf("`service_type` int(11) NOT NULL default '1',\n");
  	printf("`service_var` varchar(255) default NULL,\n");
  	printf("`service_passive_timeout` int(11) NOT NULL default '100',\n");
  	printf("PRIMARY KEY  (`service_id`),\n");
  	printf("KEY `service_id` (`service_id`),\n");
  	printf("KEY `service_id_2` (`service_id`,`server_id`)\n");
	printf(") ;\n");
	
  	printf("CREATE TABLE `servers` (\n");
  	printf("`server_id` int(12) NOT NULL auto_increment,\n");
  	printf("`server_ip` varchar(255) NOT NULL default '',\n");
  	printf("`server_name` varchar(255) NOT NULL default '',\n");
  	printf("`server_ico` varchar(255) NOT NULL default '',\n");
  	printf("`server_enabled` int(2) NOT NULL default '1',\n");
  	printf("`server_port` int(255) NOT NULL default '9030',\n");
	printf("PRIMARY KEY  (`server_id`),\n");
  	printf("UNIQUE KEY `server_id` (`server_id`))\n");


	
	
	
}
