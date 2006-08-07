
#define PROGNAME "bartlby"
#define REL_NAME "Druid"
#define VERSION  "1.2.2"
#define EXPECTCORE 1102021 //Module V Check's

#define MAX_CCACHE 1024


#define RECOVERY_OUTSTANDING 1
#define RECOVERY_DONE 0


#define EXTENSION_OK 0
#define EXTENSION_NOK 1


#define EXTENSION_CALLBACK_ALL -1
#define EXTENSION_CALLBACK_PRE_CHECK 1
#define EXTENSION_CALLBACK_POST_CHECK 2
#define EXTENSION_CALLBACK_STATE_CHANGED 3
#define EXTENSION_CALLBACK_TRIGGER_PRE 4
#define EXTENSION_CALLBACK_SCHED_WAIT 5
#define EXTENSION_CALLBACK_UNKOWN_CHECK_TYPE 6
#define EXTENSION_CALLBACK_ROUND_TIME 7
#define EXTENSION_CALLBACK_CHECK_TIME 8
#define EXTENSION_CALLBACK_EVENT_PUSHED 9
#define EXTENSION_CALLBACK_REPLICATION_GO 10
#define EXTENSION_CALLBACK_TRIGGER_FIRED 11




#define EVENT_QUEUE_MAX 128
#define EVENT_STATUS_CHANGED 2
#define EVENT_TRIGGER_PUSHED 3



#define STATE_OK 0
#define STATE_WARNING 1
#define STATE_CRITICAL 2
#define STATE_UNKOWN 3
#define STATE_SIRENE 7


//Service ACK

#define ACK_NOT_NEEDED 0
#define ACK_NEEDED 1
#define ACK_OUTSTANDING 2

#define DT_SERVICE 1
#define DT_SERVER 2

#define SVC_TYPE_ACTIVE 1
#define SVC_TYPE_PASSIVE 2
#define SVC_TYPE_GROUP 3
#define SVC_TYPE_LOCAL 4
#define SVC_TYPE_SNMP 5
#define SVC_TYPE_NRPE 6
#define SVC_TYPE_NRPE_SSL 7


#define SVC_THRESHOLD 10

#define PERF_TYPE_SVC_TIME 1
#define PERF_TYPE_ROUND_TIME 2 

#define LOAD_SYMBOL(x,y,z) 	x=dlsym(y, z); \
    	if((dlmsg=dlerror()) != NULL) { \
        	_log("-Error: %s", dlmsg); \
        	exit(1); \
    	}
    	

struct shm_counter {
	int worker;
	int services;
	int downtimes;	
};

struct perf_statistic {
	long sum;
	long counter;	
};


struct snmpi {
	char community[512];
	int version;
	char objid[1024];
	int warn;
	int crit;
	int type;
};

struct shm_header {
	int size_of_structs;
	int svccount;
	int wrkcount;
	int current_running;
	char  version[50];
	int do_reload;
	int last_replication;
	int startup_time;
	int dtcount;
	int sirene_mode;
	struct perf_statistic pstat;
	int cur_event_index;
	
};

struct service {
	int service_id;
	int server_id;
	int last_state;
	int current_state;
	int client_port;
	char  new_server_text[2048];
	char  service_name[2048];
	char  server_name[2048];
	char  client_ip[2048];
	char  plugin[2048];
	char  plugin_arguments[2048];
	int check_interval;
	int last_check;
	
	/*Time stuff*/
	
	int hour_from;
	int min_from;
	int hour_to;
	int min_to;
	
	/*Notify things*/
	int notify_enabled;
	int last_notify_send;
	int flap_count;
	
	int service_active;
	
	char  service_var[2048];
	int service_type;
	int service_passive_timeout;
	
	int notify_last_state;
	int notify_last_time;
	int service_check_timeout;
	
	char server_icon[1024];
	
	int service_ack;
	
	int service_retain;
	int service_retain_current;
	
	int check_is_running;
	
	struct perf_statistic pstat;
	
	int do_force;
	
	struct snmpi snmp_info;
	
	int recovery_outstanding; //Flag to see if recover is waiting
	
};

struct worker {
	char name[2048];
	char  mail[2048];
	char  icq[2048];
	char  services[2048];
	
	int worker_id;
	int active;
	
	char password[2048];
	
	int escalation_count;
	int escalation_time;
	char notify_levels[20];
	char enabled_triggers[2048];
	char t[500];

}sa;


struct downtime {
	int downtime_id;
	int downtime_type;
	int downtime_from;
	int downtime_to;
	char downtime_notice[2048];
	int service_id;
	
}sb;

struct btl_event {
	int evnt_id;
	char evnt_message[1024];
	int evnt_time;
		
}eb;
struct ext_notify {
	struct service * svc;	
	struct worker * wrk;
	char * trigger;
} ty;



char * getConfigValue(char *, char *);
int clear_serviceMap(struct service **);
int clear_workerMap(struct worker ** m);

int schedule_loop(char *, void *, void *);
void sched_write_back_all(char * cfgfile, void * shm_addr, void * SOHandle);

void sched_reschedule(struct service * svc);

void bartlby_check_service(struct service * svc, void *, void *, char *);


void bartlby_check_snmp(struct service * svc, char * cfgfile);
void bartlby_check_nrpe(struct service * svc, char * cfgfile, int use_ssl);

void str_mysql_safe(char * str);
void service_mysql_safe(struct service * svc);


//Replication
int replication_go(char *, void *, void *);

//SHM

int GetDowntimeMap(struct downtime * svcs, char * config);
struct service * bartlby_SHM_ServiceMap(void *);
struct downtime * bartlby_SHM_DowntimeMap(void * shm_addr);
struct shm_header * bartlby_SHM_GetHDR(void *);
struct worker * bartlby_SHM_WorkerMap(void * shm_addr);

void bartlby_perf_track(struct service * svc,char * return_buffer, int return_bytes, char * cfgfile);
int bartlby_core_perf_track(struct shm_header * hdr, struct service * svc, int type, int time);
int bartlby_milli_timediff(struct timeval end, struct timeval start);

void bartlby_trigger(struct service * svc, char * cfgfile, void * shm_addr, int do_check);
//Global :-)
int _log(char * str,  ...);
void bartlby_decode(char * msg, int length);
void bartlby_encode(char * msg, int length);
char * bartlby_beauty_state(int status);

void bartlby_end_clean(char *cfgfile);
void bartlby_get_daemon(char * cfgfile);
void set_cfg(char * cfg);

void str_replace(char *str, const char *from, const char *to, int maxlen);
void bartlby_replace_svc_in_str(char * str, struct service * svc, int max);

void bartlby_action_handle_reply(struct service * svc, char * rmessage, char * cfgfile);
int bartlby_action_handle_reply_line(struct service * svc, char * line, char *cfgfile);
void bartlby_check_sirene(char * configfile, void * bartlby_address);
int bartlby_is_in_downtime(void * bartlby_address, struct service * svc);
ssize_t recvall(int _socket, char* buffer, int max_len,int flags);


//EVNT's
void bartlby_event_init(void * bartlby_address);
struct btl_event * bartlby_SHM_EventMap(void * shm_addr);
int bartlby_push_event(int event_id, char * str,  ...);

void bartlby_pre_init(char * cfgfile);

void cfg_init_cache(void);

void bartlby_ext_init(void * shm_addr, void * data_loader, char * cfg);
void bartlby_ext_shutdown(int sched_exit_code);
int bartlby_ext_register_callback(int type, void * fcn);
int bartlby_callback(int type, void *data);

extern char config_file[255];



void nrpe_display_license(void);
