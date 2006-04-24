/*
	Release table
	
	angeloi
	Archangeloi 
	Archai 
	Exusiai 
	Dynameis 
	Kyriotetes 
	Throne
	Cherubim
	Seraphim
	
*/

#define PROGNAME "bartlby"
#define REL_NAME "Naproxen"
#define VERSION  "1.1.8a"
#define EXPECTCORE 1101080 //Module V Check's

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
		
}eb;



char * getConfigValue(char *, char *);
int clear_serviceMap(struct service **);
int clear_workerMap(struct worker ** m);

int schedule_loop(char *, void *, void *);
void sched_reschedule(struct service * svc);

void bartlby_check_service(struct service * svc, void *, void *, char *);


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

void bartlby_end_daemon(char *cfgfile);
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

extern char config_file[255];
