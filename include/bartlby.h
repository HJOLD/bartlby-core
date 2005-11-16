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
#define REL_NAME "Exusiai"
#define VERSION  "0.9.9a"
#define EXPECTCORE 1009091 //Module V Check's


#define STATE_OK 0
#define STATE_WARNING 1
#define STATE_CRITICAL 2
#define STATE_UNKOWN 3

#define SVC_TYPE_ACTIVE 1
#define SVC_TYPE_PASSIVE 2
#define SVC_TYPE_GROUP 3


#define LOAD_SYMBOL(x,y,z) 	x=dlsym(y, z); \
    	if((dlmsg=dlerror()) != NULL) { \
        	_log("-Error: %s", dlmsg); \
        	exit(1); \
    	}
    	
    	

struct shm_header {
	        int svccount;
	        int wrkcount;
	        int current_running;
		char  version[50];
		int do_reload;
		int last_replication;
		int startup_time;

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




char * getConfigValue(char *, char *);
int clear_serviceMap(struct service **);
int clear_workerMap(struct worker ** m);

int schedule_loop(char *, void *, void *);
void sched_reschedule(struct service * svc);

void bartlby_check_service(struct service * svc, void *, void *, char *);


//Replication
int replication_go(char *, void *, void *);

//SHM


struct service * bartlby_SHM_ServiceMap(void *);
struct shm_header * bartlby_SHM_GetHDR(void *);
struct worker * bartlby_SHM_WorkerMap(void * shm_addr);



void bartlby_trigger(struct service * svc, char * cfgfile, void * shm_addr);
//Global :-)
int _log(char * str,  ...);
void bartlby_decode(char * msg, int length);
void bartlby_encode(char * msg, int length);
char * bartlby_beauty_state(int status);

void bartlby_end_daemon(char *cfgfile);
void bartlby_get_daemon(char * cfgfile);
void set_cfg(char * cfg);

extern char config_file[255];
