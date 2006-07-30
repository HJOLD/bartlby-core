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
Revision 1.41  2006/07/30 22:47:06  hjanuschka
auto commit

Revision 1.40  2006/07/18 21:38:23  hjanuschka
core: a major BUG has been discoverd in the first production envorioments
	 when a worker has only selected OK and CRITICAL notifications
	 he always got notified about a change from (unselected) WARNING back to OK
	 this had produce ALOT of unserious OK notifications
	 -- 18-07-06 fixed :-)

core: perfhandlers have been re-worked to only collect data

Revision 1.39  2006/07/15 14:02:08  hjanuschka
data lib active_service etc..

Revision 1.38  2006/06/25 21:24:25  hjanuschka
strip "'" out of plugin arguments during saving

Revision 1.37  2006/06/04 23:55:28  hjanuschka
core: SSL_connect (timeout issue's solved , at least i hope :))
core: when perfhandlers_enabled == false, you now can enable single services
core: plugin_arguments supports $MACROS
core: config variables try now to cache themselfe to minimize I/O activity
core: .so extensions support added

Revision 1.36  2006/05/20 18:29:16  hjanuschka
snmp implented

Revision 1.35  2006/05/12 23:38:02  hjanuschka
*** empty log message ***

Revision 1.34  2006/04/23 18:07:43  hjanuschka
core/ui/php: checks can now be forced
ui: remote xml special_addon support
core: svc perf MS
core: round perf MS
php: svcmap, get_service perf MS
ui: perf MS

Revision 1.33  2006/04/09 22:12:03  hjanuschka
R E L E A S E (1.1.8a -> Naproxen):

perf: distribute RRDs correspodening to the perf handler
core: sched_timeout refined
core: service_retain
core: lib/mysql service_retain
php: service_retain
ui: overview supports remote bartlby's
ui: server/service detail supports remote bartlby's
ui: services list supports remote bartlby
ui: service_retain
ui: add perf defaults to package
ui: catch un-existing objects, server|service|worker
ui: exit if either built in nor shared bartlby extension was found (discovered during php upgrade )
ui: addons got own config file (ui-extra.conf)
php: E_WARNING on unexisting config file

Revision 1.32  2006/03/18 01:54:46  hjanuschka
perf: distribute RRDs correspodening to the perf handler
core: sched_timeout refined
core: service_retain
core: lib/mysql service_retain
php: service_retain
ui: service_retain
ui: add perf defaults to package
ui: catch un-existing objects, server|service|worker
ui: exit if either built in nor shared bartlby extension was found (discovered during php upgrade )
ui: addons got own config file (ui-extra.conf)
php: E_WARNING on unexisting config file

Revision 1.31  2006/02/25 02:02:46  hjanuschka
core: configure/ --with-user=
core: configure/ install all files and directories with chown $BARTLBY_USER
core: lib/mysql [worker|service|server]_by_id returns negative value if not found

Revision 1.30  2006/02/22 22:33:25  hjanuschka
core: lib/mysql "no worker found" wrong logging msg

Revision 1.29  2006/02/17 22:13:18  hjanuschka
configure script added

Revision 1.28  2006/02/17 20:06:19  hjanuschka
	acknowledgeable services

Revision 1.27  2006/02/12 18:37:51  hjanuschka
trigger fixes trigger logging refined
datalib: mysql/ returns now the server version

Revision 1.26  2006/02/05 21:49:47  hjanuschka
*** empty log message ***

Revision 1.25  2006/01/29 15:53:05  hjanuschka
server icon

Revision 1.24  2006/01/19 23:30:22  hjanuschka
introducing downtime's

Revision 1.23  2006/01/08 16:17:24  hjanuschka
mysql shema^

Revision 1.22  2005/12/25 12:55:45  hjanuschka
service_check_timeout is dynamic now

Revision 1.21  2005/12/13 23:17:53  hjanuschka
setuid before creating shm segment

Revision 1.20  2005/11/27 02:04:43  hjanuschka
setuid/setgid for security and web ui

Revision 1.19  2005/09/30 21:01:45  hjanuschka
delete server deletes now also the services ;-)

Revision 1.18  2005/09/28 21:46:30  hjanuschka
converted files to unix
jabber.sh -> disabled core dumps -> jabblibs segfaults
                                    will try to patch it later

Revision 1.17  2005/09/27 18:21:57  hjanuschka
*** empty log message ***

Revision 1.16  2005/09/25 16:31:05  hjanuschka
trigger: can now be enabled/disabled per trigger in web ui
ui: add/modify worker can now set and display workers selected
datalib: api modifications for trigger enable/disable feature

Revision 1.15  2005/09/18 11:28:12  hjanuschka
replication now works :-)
core: can run as slave and load data from a file instead of data_lib
ui: displays a warning if in slave mode to not add/modify servers/services
portier: recieves and writes shm dump to disk
so hot stand by should be possible ;-)
slave also does service checking

Revision 1.14  2005/09/13 19:29:18  hjanuschka
daemon: pidfile, remove pidfile at end
mysql.c: fixed 2 segfaults under _MALLOC_CHECK=2

Revision 1.13  2005/09/05 19:53:13  hjanuschka
2 day uptime without a single sigsegv ;-)
added daemon function ;-)
	new cfg vars daemon=[true|false], basedir, logfile

Revision 1.12  2005/09/03 23:01:13  hjanuschka
datalib api refined
moved to version 0.9.7
reload via SHM

Revision 1.11  2005/09/03 20:11:22  hjanuschka
fixups

added addworker, deleteworker, modifyworker, getworkerbyid

Revision 1.10  2005/09/02 02:16:58  hjanuschka
some trap downs ;-)

Revision 1.9  2005/08/31 20:05:18  hjanuschka
removed some debug infos

Revision 1.8  2005/08/28 22:57:14  hjanuschka
config.c: fixed fclose BUG (too many open files ) missing fclose
service_active is now set by data_lib and acutally used by scheduler

Revision 1.7  2005/08/28 22:25:58  hjanuschka
*** empty log message ***

Revision 1.6  2005/08/28 18:00:22  hjanuschka
data_lib api extended, service/add/delete/update

Revision 1.5  2005/08/28 16:02:59  hjanuschka
CVS Header


*/

#include <mysql/mysql.h>
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
      			_log("Mysql Error %s:%d", __FILE__, __LINE__); \
      			return -1; \
      		}

#define CHK_ERR_NULL(x) \
		if (x != NULL) {\
			if(mysql_errno(x) != 0) {\
		 		_log("mysql error: %s", mysql_error(x)); \
      		 		return NULL; \
      			}\
      		} else {\
      			_log("Mysql Error %s:%d", __FILE__, __LINE__); \
      			return NULL; \
      		}      		

#define AUTOR "Helmut Januschka \"helmut@januschka.com\" http://bartlby.org"
#define NAME "MYSQL Connector"
#define DLVERSION  "0.9.1"



#define SELECTOR "select svc.service_id, svc.service_name, svc.service_state, srv.server_name, srv.server_id, srv.server_port, srv.server_ip, svc.service_plugin, svc.service_args, UNIX_TIMESTAMP(svc.service_last_check), svc.service_interval, svc.service_text, HOUR(svc.service_time_from), MINUTE(svc.service_time_from), HOUR(svc.service_time_to), MINUTE(svc.service_time_to), svc.service_notify, svc.service_type, svc.service_var, svc.service_passive_timeout,service_active, svc.service_check_timeout, srv.server_ico, svc.service_ack, svc.service_retain, svc.service_snmp_community, svc.service_snmp_objid, svc.service_snmp_version, svc.service_snmp_warning, svc.service_snmp_critical, svc.service_snmp_type  from services svc, servers srv where svc.server_id=srv.server_id ORDER BY svc.service_type asc, svc.server_id"
#define WORKER_SELECTOR "select worker_mail, worker_icq, enabled_services,notify_levels, worker_active, worker_name, worker_id, password, enabled_triggers from workers"
#define SERVICE_UPDATE_TEXT "update services set service_last_check=FROM_UNIXTIME(%d), service_text='%s', service_state=%d, service_active=%d, service_notify=%d, service_check_timeout=%d, service_ack=%d where service_id=%d"


#define ADD_SERVER "insert into servers (server_name,server_ip,server_port, server_ico) VALUES('%s','%s', '%d', '%s')"
#define DELETE_SERVER "delete from servers where server_id=%d"
#define UPDATE_SERVER "update servers set server_name='%s',server_ip='%s',server_port=%d, server_ico='%s' where server_id=%d"
#define SERVER_SELECTOR "select server_name, server_ip, server_port, server_ico from servers where server_id=%d"


#define DELETE_SERVICE_BY_SERVER "delete from services where server_id=%d"

#define ADD_SERVICE "insert into services(server_id, service_plugin, service_name, service_state,service_text, service_args,service_notify, service_active,service_time_from,service_time_to, service_interval, service_type,service_var,service_passive_timeout,service_check_timeout, service_ack, service_retain, service_snmp_community, service_snmp_objid, service_snmp_version, service_snmp_warning, service_snmp_critical, service_snmp_type) values(%d,'%s','%s',4, 'Newly created', '%s',%d,%d,'%s','%s',%d,%d,'%s',%d, %d, %d, %d, '%s', '%s', %d, %d, %d, %d)"
#define DELETE_SERVICE "delete from services where service_id=%d"


#define UPDATE_SERVICE "update services set service_type=%d,service_name='%s',server_id=%d,service_time_from='%s',service_time_to='%s',service_interval = %d, service_plugin='%s',service_args='%s',service_passive_timeout=%d, service_var='%s',service_check_timeout=%d, service_ack='%d', service_retain='%d', service_snmp_community='%s', service_snmp_objid='%s', service_snmp_version='%d', service_snmp_warning='%d', service_snmp_critical='%d', service_snmp_type='%d', service_notify='%d', service_active='%d'  where service_id=%d"
#define SERVICE_SELECTOR "select svc.service_id, svc.service_name, svc.service_state, srv.server_name, srv.server_id, srv.server_port, srv.server_ip, svc.service_plugin, svc.service_args, UNIX_TIMESTAMP(svc.service_last_check), svc.service_interval, svc.service_text, HOUR(svc.service_time_from), MINUTE(svc.service_time_from), HOUR(svc.service_time_to), MINUTE(svc.service_time_to), svc.service_notify, svc.service_type, svc.service_var, svc.service_passive_timeout, svc.service_active,svc.service_check_timeout, svc.service_ack, svc.service_retain, svc.service_snmp_community, svc.service_snmp_objid, svc.service_snmp_version, svc.service_snmp_warning, svc.service_snmp_critical, svc.service_snmp_type, srv.server_ico from services svc, servers srv where svc.server_id=srv.server_id and svc.service_id=%d"




#define ADD_WORKER    "INSERT INTO workers(worker_mail, worker_icq, enabled_services, notify_levels, worker_active, worker_name, password,enabled_triggers) VALUES('%s', '%s', '%s','%s', %d, '%s', '%s', '%s')"
#define DELETE_WORKER "delete from workers where worker_id=%d"
#define UPDATE_WORKER "update workers set worker_mail='%s', worker_icq='%s', enabled_services='%s', notify_levels='%s', worker_active=%d, worker_name='%s', password='%s', enabled_triggers='%s' WHERE worker_id=%d"
#define WORKER_SEL "select worker_mail, worker_icq, enabled_services,notify_levels, worker_active, worker_name, worker_id, password, enabled_triggers from workers where worker_id=%d"

#define UPDATE_DOWNTIME "update downtime set downtime_notice='%s', downtime_from=%d,downtime_to=%d, service_id=%d, downtime_type=%d where downtime_id=%d"
#define DEL_DOWNTIME "delete from downtime where downtime_id=%d"
#define ADD_DOWNTIME "INSERT INTO downtime(downtime_type, downtime_from,downtime_to,service_id, downtime_notice) VALUES(%d,%d,%d,%d, '%s')"
#define DOWNTIME_SEL "select downtime_id, downtime_type, downtime_from, downtime_to, downtime_notice, service_id from downtime"

//Counters
#define COUNT_SERVICES "select count(1) from services"
#define COUNT_WORKERS "select count(1) from workers"
#define COUNT_DOWNTIMES "select count(1) from downtime"


/*

struct shm_counter {
	int worker;
	int services;
	int downtimes;	
}

*/

struct shm_counter * GetCounter(char * config) {
	MYSQL *mysql;
	MYSQL_ROW  row;
	MYSQL_RES  *res;
	struct shm_counter * shmc;
	
	
	shmc = malloc(sizeof(struct shm_counter) + 10);
	
	
	char * mysql_host = getConfigValue("mysql_host", config);
	char * mysql_user = getConfigValue("mysql_user", config);
	char * mysql_pw = getConfigValue("mysql_pw", config);
	char * mysql_db = getConfigValue("mysql_db", config);
	
	
	


	mysql=mysql_init(NULL);
	CHK_ERR_NULL(mysql);
	mysql=mysql_real_connect(mysql, mysql_host, mysql_user, mysql_pw, NULL, 0, NULL, 0);
	CHK_ERR_NULL(mysql);
     mysql_select_db(mysql, mysql_db);
     CHK_ERR_NULL(mysql);
	
	
	mysql_query(mysql, COUNT_SERVICES);
	CHK_ERR_NULL(mysql);
     res = mysql_store_result(mysql);
     CHK_ERR_NULL(mysql);
     
     
     if(mysql_num_rows(res) > 0) {
     	row=mysql_fetch_row(res);

  		if(row[0] != NULL) {
     	 	shmc->services = atoi(row[0]);
     	}
     	
     	
     } else {
     	shmc->services = 0;	
     }
      		
     mysql_free_result(res);
     
     mysql_query(mysql, COUNT_WORKERS);
	CHK_ERR_NULL(mysql);
     res = mysql_store_result(mysql);
     CHK_ERR_NULL(mysql);
     
     
     if(mysql_num_rows(res) > 0) {
     	row=mysql_fetch_row(res);

  		if(row[0] != NULL) {
     	 	shmc->worker = atoi(row[0]);
     	}
     	
     	
     } else {
     	shmc->worker = 0;	
     }
      		
     mysql_free_result(res);
     
     mysql_query(mysql, COUNT_DOWNTIMES);
	CHK_ERR_NULL(mysql);
     res = mysql_store_result(mysql);
     CHK_ERR_NULL(mysql);
     
     
     if(mysql_num_rows(res) > 0) {
     	row=mysql_fetch_row(res);

  		if(row[0] != NULL) {
     	 	shmc->downtimes = atoi(row[0]);
     	}
     	
     	
     } else {
     	shmc->downtimes = 0;	
     }
      		
     mysql_free_result(res);
     
     
	mysql_close(mysql);
	free(mysql_host);
	free(mysql_user);
	free(mysql_pw);
	free(mysql_db);
	
	return shmc;
	
	
}


int UpdateDowntime(struct downtime * svc, char *config) {
	/*
		modify worker
	*/
	MYSQL *mysql;
	int rtc;
	
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
	
	
	sqlupd=malloc(sizeof(char)*(strlen(UPDATE_DOWNTIME)+sizeof(struct downtime)+200));
	sprintf(sqlupd, UPDATE_DOWNTIME, svc->downtime_notice, svc->downtime_from, svc->downtime_to, svc->service_id, svc->downtime_type, svc->downtime_id);
	
	
	
	mysql_query(mysql, sqlupd);
		CHK_ERR(mysql);
	
	
	free(sqlupd);
	rtc=1;
	mysql_close(mysql);
		
	free(mysql_host);
	free(mysql_user);
	free(mysql_pw);
	free(mysql_db);
	
	return rtc;	
}

int DeleteDowntime(int downtime_id, char * config) {
	
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
	
	
	sqlupd=malloc(sizeof(char)*(strlen(DEL_DOWNTIME)+20));
	sprintf(sqlupd, DEL_DOWNTIME, downtime_id);
	
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

int AddDowntime(struct downtime * svc, char *config) {
	
	MYSQL *mysql;
	int rtc;
	
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
	
	
	sqlupd=malloc(sizeof(char)*(strlen(ADD_DOWNTIME)+sizeof(struct downtime)+40));
	sprintf(sqlupd, ADD_DOWNTIME, svc->downtime_type, svc->downtime_from, svc->downtime_to, svc->service_id, svc->downtime_notice);
	
	
	
	mysql_query(mysql, sqlupd);
		CHK_ERR(mysql);
	
	
	free(sqlupd);
	rtc=mysql_insert_id(mysql);
	mysql_close(mysql);
		
	free(mysql_host);
	free(mysql_user);
	free(mysql_pw);
	free(mysql_db);
	
	return rtc;	
}	

int GetDowntimeMap(struct downtime * svcs, char * config) {
	
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
	
	
	mysql_query(mysql, DOWNTIME_SEL);
		CHK_ERR(mysql);
      	res = mysql_store_result(mysql);
      		CHK_ERR(mysql);
      	
      	
      	if(mysql_num_rows(res) > 0) {
      		
      		while ( (row=mysql_fetch_row(res)) != NULL) {

  			if(row[0] != NULL) {
      				
      				svcs[i].downtime_id = atoi(row[0]);
      			} else {
      				svcs[i].downtime_id = -1;    				
      			}
      			
      			if(row[1] != NULL) {
      				
      				svcs[i].downtime_type = atoi(row[1]);
      			} else {
      				svcs[i].downtime_type = -1;    				
      			}
      			if(row[2] != NULL) {
      				
      				svcs[i].downtime_from = atoi(row[2]);
      			} else {
      				svcs[i].downtime_from = -1;    				
      			}
      			if(row[3] != NULL) {
      				
      				svcs[i].downtime_to = atoi(row[3]);
      			} else {
      				svcs[i].downtime_to = -1;    				
      			}
      			
      			if(row[4] != NULL) {
      				//svcs[i].icq=malloc(strlen(row[1])*sizeof(char)+2);
      				sprintf(svcs[i].downtime_notice, "%s", row[4]);
      			} else {
      				sprintf(svcs[i].downtime_notice, "(null)");     				
      			}
      			if(row[5] != NULL) {
      				
      				svcs[i].service_id = atoi(row[5]);
      			} else {
      				svcs[i].service_id = -1;    				
      			}
      			
      			
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
      		_log( "no Downtimes found!");	
      	}
	
	
	
	
	free(mysql_host);
	free(mysql_user);
	free(mysql_pw);
	free(mysql_db);
	
	return -1;
	
	
}






int GetWorkerById(int worker_id, struct worker * svc, char * config) {
	
	int tmprc;
	MYSQL *mysql;
	MYSQL_ROW  row;
	MYSQL_RES  *res;
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
	
	sqlupd=malloc(sizeof(char)*(strlen(WORKER_SEL)+sizeof(struct worker)+200));
	sprintf(sqlupd, WORKER_SEL, worker_id);
	
	
	mysql_query(mysql, sqlupd);
		CHK_ERR(mysql);
      	res = mysql_store_result(mysql);
      		CHK_ERR(mysql);
      	
      	
      	if(mysql_num_rows(res) == 1 ) {
      		row=mysql_fetch_row(res);
      		
      		if(row[0] != NULL ) {
      			sprintf(svc->mail, "%s", row[0]);	
      		} else {
      			sprintf(svc->mail, "(null)");
      		}
      		if(row[1] != NULL) {
      			sprintf(svc->icq, "%s", row[1]);	
      		} else {
      			sprintf(svc->icq, "(null)");	
      		}
      		if(row[2] != NULL) {
      			sprintf(svc->services, "%s", row[2]);	
      		} else {
      			sprintf(svc->services, "(null)");	
      		}
      		if(row[3] != NULL) {
      			sprintf(svc->notify_levels, "%s", row[3]);	
      		} else {
      			sprintf(svc->notify_levels, "(null)");	
      		}
      		if(row[4] != NULL) {
      			svc->active=atoi(row[4]);	
      		} else {
      			svc->active=-1;	
      		}
      		if(row[5] != NULL) {
      			sprintf(svc->name, "%s", row[5]);	
      		} else {
      			sprintf(svc->name, "(null)");	
      		}
      		if(row[6] != NULL) {
      			svc->worker_id=atoi(row[6]);	
      		} else {
      			svc->worker_id=-1;	
      		}
      		if(row[7] != NULL) {
      			sprintf(svc->password, "%s", row[7]);
      					
      		} else {
      			sprintf(svc->password, "(null)");	
      		}
      		if(row[8] != NULL) {
      			sprintf(svc->enabled_triggers, "%s", row[8]);
      				
      		} else {
      			sprintf(svc->enabled_triggers, "(null)");	
      		}
      		tmprc=0;
      	} else {
			tmprc=-1;
	}
	
	
	mysql_free_result(res);
      	mysql_close(mysql);
      	free(mysql_host);
	free(mysql_user);
	free(mysql_pw);
	free(mysql_db);
	free(sqlupd);
	return tmprc;
		
}

int UpdateWorker(struct worker * svc, char *config) {
	/*
		modify worker
	*/
	MYSQL *mysql;
	int rtc;
	
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
	
	
	sqlupd=malloc(sizeof(char)*(strlen(UPDATE_WORKER)+sizeof(struct worker)+200));
	sprintf(sqlupd, UPDATE_WORKER, svc->mail, svc->icq, svc->services, svc->notify_levels, svc->active, svc->name,svc->password,svc->enabled_triggers, svc->worker_id);
	
	
	
	mysql_query(mysql, sqlupd);
		CHK_ERR(mysql);
	
	
	free(sqlupd);
	rtc=1;
	mysql_close(mysql);
		
	free(mysql_host);
	free(mysql_user);
	free(mysql_pw);
	free(mysql_db);
	
	return rtc;	
}


int DeleteWorker(int worker_id, char * config) {
	/*
		we get a svc->server_id
		KICK it (not like beckham)
		
	*/
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
	
	
	sqlupd=malloc(sizeof(char)*(strlen(DELETE_WORKER)+20));
	sprintf(sqlupd, DELETE_WORKER, worker_id);
	
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

int AddWorker(struct worker * svc, char *config) {
	/*
		We get a struct worker
		filled with worker_mail, worker_ic, enabled_services, notify_levels, active
		store it ;-)
		and return wrk->worker_id
	*/
	MYSQL *mysql;
	int rtc;
	
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
	
	
	sqlupd=malloc(sizeof(char)*(strlen(ADD_WORKER)+sizeof(struct worker)+40));
	sprintf(sqlupd, ADD_WORKER, svc->mail, svc->icq, svc->services, svc->notify_levels, svc->active, svc->name, svc->password, svc->enabled_triggers);
	
	
	
	mysql_query(mysql, sqlupd);
		CHK_ERR(mysql);
	
	
	free(sqlupd);
	rtc=mysql_insert_id(mysql);
	mysql_close(mysql);
		
	free(mysql_host);
	free(mysql_user);
	free(mysql_pw);
	free(mysql_db);
	
	return rtc;	
}	


int GetServiceById(int service_id, struct service * svc, char * config) {
	
	
	MYSQL *mysql;
	MYSQL_ROW  row;
	MYSQL_RES  *res;
	char * sqlupd;
	
	int tmprc;
	
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
	
	sqlupd=malloc(sizeof(char)*(strlen(SERVICE_SELECTOR)+20));
	sprintf(sqlupd, SERVICE_SELECTOR, service_id);
	
	
	mysql_query(mysql, sqlupd);
		CHK_ERR(mysql);
      	res = mysql_store_result(mysql);
      		CHK_ERR(mysql);
      	
      	
      	if(mysql_num_rows(res) == 1 ) {
      		row=mysql_fetch_row(res);
      		svc->service_id=atoi(row[0]);
      		svc->server_id=atoi(row[4]);
      		svc->last_state=atoi(row[2]);
      		svc->current_state=atoi(row[2]);
      		
      		if(row[1] != NULL) {
      			//svc->service_name=malloc(strlen(row[1])*sizeof(char)+2);
      			sprintf(svc->service_name, "%s", row[1]);
      			
      		} else {
      			//svc->service_name=NULL;     				
      			sprintf(svc->service_name, "(null)");
      		}
      		
      		
      		if(row[3] != NULL) {
      			//svc->server_name=malloc(strlen(row[3])*sizeof(char)+2);
      			sprintf(svc->server_name, "%s", row[3]);
      			
      		} else {
//    			svc->server_name=NULL;   
      			sprintf(svc->server_name, "(null)");  				
      		}
      		
      		
      		if(row[6] != NULL) {
      			//svc->client_ip=malloc(strlen(row[6])*sizeof(char)+2);
      			sprintf(svc->client_ip, "%s", row[6]);
      			
      		} else {
      			  
      			sprintf(svc->client_ip, "(null)");     				
      		}
      		
      		
      		svc->client_port=atoi(row[5]);
      		
//     		svc->new_server_text=malloc(strlen(row[11])*sizeof(char)+2);
      		
      		sprintf(svc->new_server_text, "%s", row[11]);
      		
      		///svc->new_server_text=row[11];
      		
      		
      		
      		
      		if(row[7] != NULL) {
      			//svc->plugin=malloc(strlen(row[7])*sizeof(char)+2);
      			sprintf(svc->plugin, "%s", row[7]);
      			
      		} else {
      			//svc->plugin=NULL; 
      			sprintf(svc->plugin, "(null)");       				
      		}
      		
      		if(row[8] != NULL) {
      			//svc->plugin_arguments=malloc(strlen(row[8])*sizeof(char)+2);
      			sprintf(svc->plugin_arguments, "%s", row[8]);
      			
      		} else {
      			//svc->plugin_arguments=NULL; 
      			sprintf(svc->plugin_arguments, "(null)");      				
      		}
      		
      		
      		svc->last_check=atoi(row[9]);
      		
      		svc->check_interval=atoi(row[10]);
      		      		
      		
      		svc->hour_from=atoi(row[12]);
      		svc->min_from=atoi(row[13]);
      		
      		svc->hour_to=atoi(row[14]);
      		svc->min_to=atoi(row[15]);
      		
      		svc->notify_enabled=atoi(row[16]);
      		//svc->last_notify_send=time(NULL);
      		
      		//svc.service_type, svc.service_var, svc.service_passive_timeout
      		svc->service_type = atoi(row[17]);
      		
      		if(row[18] != NULL) {
      			//svc->service_var=malloc(strlen(row[18])*sizeof(char)+2);
      			sprintf(svc->service_var, "%s", row[18]);
      			
      		} else {
      			//svc->service_var=NULL;
      			sprintf(svc->service_var, "(null)");
      		}
      		
      		svc->service_passive_timeout=atoi(row[19]);
      		
      		svc->service_active=atoi(row[20]);
      		svc->service_check_timeout=atoi(row[21]);
      		if(row[22] != NULL) {
      			svc->service_ack = atoi(row[22]);	
      		} else {
      			svc->service_ack = ACK_NOT_NEEDED;
      		}
      		svc->service_retain=atoi(row[23]);
      		svc->flap_count=0;
      		
      		if(row[24] != NULL) {
      			snprintf(svc->snmp_info.community, 500, "%s", row[24]);
      		} else {
      			sprintf(svc->snmp_info.community, "(null)");
      		}
      		
      		if(row[25] != NULL) {
      			snprintf(svc->snmp_info.objid, 1000, "%s", row[25]);
      		} else {
      			sprintf(svc->snmp_info.objid, "(null)");	
      		}
      		
      		svc->snmp_info.version = atoi(row[26]);
      		svc->snmp_info.warn = atoi(row[27]);
      		svc->snmp_info.crit = atoi(row[28]);
      		svc->snmp_info.type = atoi(row[29]);
      		
      		if(row[30] != NULL) {
      			snprintf(svc->server_icon, 1000, "%s", row[30]);
      		} else {
      			sprintf(svc->server_icon, "(null)");	
      		}		
      		
      		
      		tmprc=0;
      	} else {
		tmprc=-1;
	}
	
	
	mysql_free_result(res);
      	mysql_close(mysql);
      	free(mysql_host);
	free(mysql_user);
	free(mysql_pw);
	free(mysql_db);
	free(sqlupd);
	
	
	return tmprc;	
}

int UpdateService(struct service * svc, char *config) {
	/*
		We get a struct service
		fully filled :-)
		update it it ;-)
		and return svc->service_id
	*/
	MYSQL *mysql;
	int rtc;
	
	char * sqlupd;
	
	char * SVC_TIME_FROM, * SVC_TIME_TO;
	
	
	SVC_TIME_FROM=malloc(sizeof(char)*strlen("00:00:00            "));
	SVC_TIME_TO=malloc(sizeof(char)*strlen("00:00:00            "));
	sprintf(SVC_TIME_FROM,"%02d:%02d:00", svc->hour_from, svc->min_from);
	sprintf(SVC_TIME_TO,"%02d:%02d:00", svc->hour_to, svc->min_to);
	
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
	
	
	/*
	server_id %d
	service_plugin %s
	service_name %s
	service_state 4
	service_text 'Newly"
	service_args %s
	service_notify %d
	service_active %d
	service_time_from %s
	service_time_to  %s
	service_intervall %d
	service_type %d
	service_var %s
	service_passive_timeout %d
	
	 */
	 
	sqlupd=malloc(sizeof(char)*(strlen(UPDATE_SERVICE)+sizeof(struct service)+20));
	sprintf(sqlupd, UPDATE_SERVICE, 
	svc->service_type, 
	svc->service_name, 
	svc->server_id,
	SVC_TIME_FROM,
	SVC_TIME_TO,
	svc->check_interval,
	svc->plugin,
	svc->plugin_arguments,
	svc->service_passive_timeout,
	svc->service_var,
	svc->service_check_timeout,
	svc->service_ack,
	svc->service_retain,
	svc->snmp_info.community,
	svc->snmp_info.objid,
	svc->snmp_info.version,
	svc->snmp_info.warn,
	svc->snmp_info.crit,
	svc->snmp_info.type,
	svc->notify_enabled,
	svc->service_active,	
	svc->service_id
	
	);
	
	//Log("dbg", sqlupd);

	mysql_query(mysql, sqlupd);
		CHK_ERR(mysql);
	
	
	free(sqlupd);
	rtc=mysql_insert_id(mysql);
	mysql_close(mysql);
		
	free(mysql_host);
	free(mysql_user);
	free(mysql_pw);
	free(mysql_db);
	free(SVC_TIME_FROM);
	free(SVC_TIME_TO);
	
	return rtc;	
}

int DeleteService(int service_id, char * config) {
	/*
		we get a svc->server_id
		KICK it (not like beckham)
		
	*/
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
	
	
	sqlupd=malloc(sizeof(char)*(strlen(DELETE_SERVICE)+20));
	sprintf(sqlupd, DELETE_SERVICE, service_id);
	
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


int AddService(struct service * svc, char *config) {
	/*
		We get a struct service
		fully filled :-)
		store it ;-)
		and return svc->service_id
	*/
	MYSQL *mysql;
	int rtc;
	
	char * sqlupd;
	int x;
	
	char * SVC_TIME_FROM, * SVC_TIME_TO;
	
	for(x=0; x<=strlen(svc->plugin_arguments); x++) {
		if(svc->plugin_arguments[x] == '\'')
			svc->plugin_arguments[x]=' '; 
	}
	
	SVC_TIME_FROM=malloc(sizeof(char)*strlen("00:00:00                      "));
	SVC_TIME_TO=malloc(sizeof(char)*strlen("00:00:00                      "));
	sprintf(SVC_TIME_FROM,"%d:%d:00", svc->hour_from, svc->min_from);
	sprintf(SVC_TIME_TO,"%d:%d:00", svc->hour_to, svc->min_to);
	
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
	
	
	/*
	server_id %d
	service_plugin %s
	service_name %s
	service_state 4
	service_text 'Newly"
	service_args %s
	service_notify %d
	service_active %d
	service_time_from %s
	service_time_to  %s
	service_intervall %d
	service_type %d
	service_var %s
	service_passive_timeout %d
	snmp_info 
		char community[512];
		int version;
		char objid[1024];
		int warn;
		int crit;
		int type;
		
	 */
	 
	sqlupd=malloc(sizeof(char)*(strlen(ADD_SERVICE)+sizeof(struct service)+20));
	
	
	
	sprintf(sqlupd, ADD_SERVICE, 
	svc->server_id, 
	svc->plugin, 
	svc->service_name,
	svc->plugin_arguments,
	svc->notify_enabled,
	svc->service_active,
	SVC_TIME_FROM,
	SVC_TIME_TO,
	svc->check_interval,
	svc->service_type,
	svc->service_var,
	svc->service_passive_timeout,
	svc->service_check_timeout,
	svc->service_ack,
	svc->service_retain,
	svc->snmp_info.community,
	svc->snmp_info.objid,
	svc->snmp_info.version,
	svc->snmp_info.warn,
	svc->snmp_info.crit,
	svc->snmp_info.type
	);
	
	//Log("dbg", sqlupd);
	
	
	
	mysql_query(mysql, sqlupd);
		CHK_ERR(mysql);
	
	
	free(sqlupd);
	rtc=mysql_insert_id(mysql);
	
	mysql_close(mysql);
		
	free(mysql_host);
	free(mysql_user);
	free(mysql_pw);
	free(mysql_db);
	free(SVC_TIME_FROM);
	free(SVC_TIME_TO);
	
	return rtc;	
}

int GetServerById(int server_id, struct service * svc, char * config) {
	
	int tmprc;
	MYSQL *mysql;
	MYSQL_ROW  row;
	MYSQL_RES  *res;
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
	
	sqlupd=malloc(sizeof(char)*(strlen(SERVER_SELECTOR)+20));
	sprintf(sqlupd, SERVER_SELECTOR, server_id);
	
	
	mysql_query(mysql, sqlupd);
		CHK_ERR(mysql);
      	res = mysql_store_result(mysql);
      		CHK_ERR(mysql);
      	
      	
      	if(mysql_num_rows(res) == 1 ) {
      		row=mysql_fetch_row(res);
      		
      		if(row[0] != NULL ) {
      			sprintf(svc->server_name, "%s", row[0]);	
      		} else {
      			sprintf(svc->server_name, "(null)");
      		}
      		if(row[1] != NULL) {
      			sprintf(svc->client_ip, "%s", row[1]);	
      		} else {
      			sprintf(svc->client_ip, "(null)");	
      		}
      		if(row[2] != NULL) {
      			svc->client_port=atoi(row[2]);	
      		} else {
      			svc->client_port=-1;	
      		}
      		if(row[3] != NULL) {
      			sprintf(svc->server_icon, "%s", row[3]);
      		} else {
      			sprintf(svc->server_icon, "(null)");
      		}
      		tmprc=0;
      	} else {
			tmprc=-1;
	}
	
	
	mysql_free_result(res);
      	mysql_close(mysql);
      	free(mysql_host);
	free(mysql_user);
	free(mysql_pw);
	free(mysql_db);
	free(sqlupd);
	return tmprc;
		
}
		
int ModifyServer(struct service * svc, char *config) {
	/*
		We get a struct service
		filled with server_name, client_port, client_ip, server_id
		Modify_it it ;-)
		and return svc->server_id
	*/
	MYSQL *mysql;
	int rtc;
	
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
	
	
	sqlupd=malloc(sizeof(char)*(strlen(UPDATE_SERVER)+strlen(svc->server_name)+strlen(svc->client_ip)+20+strlen(svc->server_icon)));
	sprintf(sqlupd, UPDATE_SERVER, svc->server_name, svc->client_ip, svc->client_port,svc->server_icon, svc->server_id);
	
	//Log("dbg", sqlupd);
	
	mysql_query(mysql, sqlupd);
		CHK_ERR(mysql);
	
	
	free(sqlupd);
	rtc=1;
	mysql_close(mysql);
		
	free(mysql_host);
	free(mysql_user);
	free(mysql_pw);
	free(mysql_db);
	
	return rtc;	
}		
	
int DeleteServer(int server_id, char * config) {
	/*
		we get a svc->server_id
		KICK it (not like beckham)
		
	*/
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
	
	
	sqlupd=malloc(sizeof(char)*(strlen(DELETE_SERVER)+20));
	sprintf(sqlupd, DELETE_SERVER, server_id);
	
	//Log("dbg", sqlupd);
	
	mysql_query(mysql, sqlupd);
		CHK_ERR(mysql);
	
	
	free(sqlupd);
	
	
	//DELETE_SERVICE_BY_SERVER
	sqlupd=malloc(sizeof(char)*(strlen(DELETE_SERVICE_BY_SERVER)+20));
	sprintf(sqlupd, DELETE_SERVICE_BY_SERVER, server_id);
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
int AddServer(struct service * svc, char *config) {
	/*
		We get a struct service
		filled with server_name, client_port, client_ip
		store it ;-)
		and return svc->server_id
	*/
	MYSQL *mysql;
	int rtc;
	
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
	
	
	sqlupd=malloc(sizeof(char)*(strlen(ADD_SERVER)+strlen(svc->server_name)+strlen(svc->client_ip)+20+strlen(svc->server_icon)));
	sprintf(sqlupd, ADD_SERVER, svc->server_name, svc->client_ip, svc->client_port, svc->server_icon);
	
	//Log("dbg", sqlupd);
	
	mysql_query(mysql, sqlupd);
		CHK_ERR(mysql);
	
	
	free(sqlupd);
	rtc=mysql_insert_id(mysql);
	mysql_close(mysql);
		
	free(mysql_host);
	free(mysql_user);
	free(mysql_pw);
	free(mysql_db);
	
	return rtc;	
}	
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
	char * vers;
	vers = malloc(sizeof(char) * (strlen(DLVERSION)+strlen(MYSQL_SERVER_VERSION)+30));
	sprintf(vers, "%s on %s", DLVERSION, MYSQL_SERVER_VERSION);
	return vers;
}

int doUpdate(struct service * svc, char * config) {
	
	
	
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
	
	
	
	
	sqlupd=malloc(sizeof(char) *(strlen(SERVICE_UPDATE_TEXT)+sizeof(struct service)+255));
	
	sprintf(sqlupd, SERVICE_UPDATE_TEXT, svc->last_check, svc->new_server_text, svc->current_state, svc->service_active, svc->notify_enabled, svc->service_check_timeout,svc->service_ack, svc->service_id);
	
	
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
      			if(row[4] != NULL) {
      				svcs[i].active = atoi(row[4]);	
      			} else {
      				svcs[i].active = -1;
      			}
      			if(row[5] != NULL) {
      				sprintf(svcs[i].name, "%s", row[5]);
      					
      			} else {
      				sprintf(svcs[i].name, "(null)");	
      			}
      			if(row[6] != NULL) {
      				svcs[i].worker_id = atoi(row[6]);	
      			} else {
      				svcs[i].worker_id = -1;
      			}
      			if(row[7] != NULL) {
      				sprintf(svcs[i].password, "%s", row[7]);
      					
      			} else {
      				sprintf(svcs[i].password, "(null)");	
      			}
      			if(row[8] != NULL) {
      				sprintf(svcs[i].enabled_triggers, "%s", row[8]);
      					
      			} else {
      				sprintf(svcs[i].enabled_triggers, "(null)");	
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
	set_cfg(config);
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
      			svcs[i].service_active=atoi(row[20]);
      			svcs[i].service_check_timeout=atoi(row[21]);
      			if(row[22] != NULL) {
      				sprintf(svcs[i].server_icon, "%s", row[22]);	
      			} else {
      				sprintf(svcs[i].server_icon, "(null)");
      			}
      			
      			if(row[23] != NULL) {
      				svcs[i].service_ack = atoi(row[23]);
      			} else {
      				svcs[i].service_ack = ACK_NOT_NEEDED;
      			}
      			
      			svcs[i].service_retain=atoi(row[24]);
      			svcs[i].service_retain_current=atoi(row[24])+1;
      			
      			svcs[i].flap_count=0;
      			svcs[i].check_is_running = 0;
      			
      			svcs[i].notify_last_state=svcs[i].current_state;
      			svcs[i].notify_last_time=time(NULL);
      			svcs[i].pstat.sum=0;
      			svcs[i].pstat.counter=0;
      			svcs[i].do_force=0;
      			svcs[i].recovery_outstanding=RECOVERY_DONE;
      			if(svcs[i].current_state == STATE_CRITICAL) {
      				svcs[i].recovery_outstanding=RECOVERY_OUTSTANDING;
      			}
      			
      			
      			//Log("load", "%s -> %s", svcs[i].plugin, svcs[i].plugin_arguments);
      			//int notify_enabled;
			//int last_notify_send;
			//int flap_count;
      			//, HOUR(svc.service_time_from), MINUTE(svc.service_time_from), HOUR(svc.service_time_to), MINUTE(svc.service_time_to)
      				
      				
      			if(row[25] != NULL) {
      				snprintf(svcs[i].snmp_info.community, 500, "%s", row[25]);
      			} else {
      				sprintf(svcs[i].snmp_info.community, "(null)");
      			}
      			
      			if(row[26] != NULL) {
      				snprintf(svcs[i].snmp_info.objid, 1000, "%s", row[26]);
      			} else {
      				sprintf(svcs[i].snmp_info.objid, "(null)");	
      			}
      			
      			svcs[i].snmp_info.version = atoi(row[27]);
      			svcs[i].snmp_info.warn = atoi(row[28]);
      			svcs[i].snmp_info.crit = atoi(row[29]);
      			svcs[i].snmp_info.type = atoi(row[30]);
      			
      			
      			bartlby_replace_svc_in_str(svcs[i].plugin_arguments, &svcs[i], 2048);
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
	return 0;	
}


