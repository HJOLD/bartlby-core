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
Revision 1.6  2006/12/09 01:25:08  hjanuschka
auto commit

Revision 1.5  2006/12/08 22:39:34  hjanuschka
auto commit

Revision 1.6  2006/11/27 21:16:54  hjanuschka
auto commit

Revision 1.5  2006/11/25 12:31:56  hjanuschka
auto commit

Revision 1.4  2006/11/25 01:16:18  hjanuschka
auto commit



*/
#include <malloc.h>
#include <stdio.h>
#include <syslog.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>



#ifdef HAVE_SSL
	#include <openssl/dh.h>
	#include <openssl/ssl.h>
	#include <openssl/err.h>
	#include <openssl/rand.h>
	#include <bartlby_v2_dh.h>
#endif

#include <bartlby.h>
             
             
             
static int use_ssl=1;
char * cfg_use_ssl;
static unsigned long crc32_table[256];
static int connection_timed_out=0;


///SHM STUFF
struct shm_header * shm_hdr;
struct service * svcmap;
void * bartlby_address;
int shm_id;
char * shmtok;
int x;
///SHM STUFF

#ifdef HAVE_SSL
SSL_METHOD *meth;
SSL_CTX *ctx;
#endif



void portier_generate_crc32_table(void);
void portier_serve_request(int sock, char * cfgfile);
void portier_randomize_buffer(char *buffer,int buffer_size);
unsigned long portier_calculate_crc32(char *buffer, int buffer_size);


static void portier_conn_timeout(int signo) {
 	connection_timed_out = 1;
 	syslog(LOG_ERR, "TIMEOUT!!");
 	exit(1);
}


int main(int argc, char **argv){
		
#ifdef HAVE_SSL
	DH *dh;
#endif

	char seedfile[FILENAME_MAX];
	int i,c;
	
	
	
	/* open a connection to the syslog facility */
	openlog("bartlby-portier",LOG_PID,LOG_DAEMON);
	
	
	shmtok = getConfigValue("shm_key", argv[0]);
	
	if(shmtok == NULL) {
		syslog(LOG_ERR, "Unset variable `shm_key' '%s'", argv[0]);
		exit(1);
	}
	
	shm_id = shmget(ftok(shmtok, 32), 0, 0777);
	if(shm_id != -1) {
		bartlby_address=shmat(shm_id,NULL,0);
		shm_hdr=bartlby_SHM_GetHDR(bartlby_address);
		svcmap=bartlby_SHM_ServiceMap(bartlby_address);
		
		
	} else {
		syslog(LOG_ERR, "bartlby down!?? shm error");
		exit(1);
	}
	
	
	/* generate the CRC 32 table */
	portier_generate_crc32_table();
	
	cfg_use_ssl=getConfigValue("portier_use_ssl", argv[argc-1]);
	if(cfg_use_ssl == NULL) {
		use_ssl = 1;	
	} else {
		use_ssl=atoi(cfg_use_ssl);
		free(cfg_use_ssl);
	}
	
	
#ifdef HAVE_SSL
	if(use_ssl == 1) {
		SSL_library_init();
		SSLeay_add_ssl_algorithms();
		meth=SSLv23_server_method();
		SSL_load_error_strings();
		
		/* use week random seed if necessary */
		if((RAND_status()==0)){
			if(RAND_file_name(seedfile,sizeof(seedfile)-1))
				if(RAND_load_file(seedfile,-1))
					RAND_write_file(seedfile);
		
			if(RAND_status()==0){
				syslog(LOG_ERR,"Warning: SSL/TLS uses a weak random seed which is highly discouraged");
				srand(time(NULL));
				for(i=0;i<500 && RAND_status()==0;i++){
					for(c=0;c<sizeof(seedfile);c+=sizeof(int)){
						*((int *)(seedfile+c))=rand();
					        }
					RAND_seed(seedfile,sizeof(seedfile));
					}
				}
			}
		
		if((ctx=SSL_CTX_new(meth))==NULL){
			syslog(LOG_ERR,"Error: could not create SSL context.\n");
			exit(2);
		}
		
		
		SSL_CTX_set_options(ctx,SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3);
		
		/* use anonymous DH ciphers */
		SSL_CTX_set_cipher_list(ctx,"ADH");
		dh=get_dh512();
		SSL_CTX_set_tmp_dh(ctx,dh);
		DH_free(dh);
	}
#endif
	
	//as we are running under inetd!!
	close(2);
	open("/dev/null",O_WRONLY);
	
	portier_serve_request(0, argv[argc-1]);

#ifdef HAVE_SSL
	SSL_CTX_free(ctx);
#endif
	return 0;
	

}

void portier_serve_request(int sock, char * cfgfile) {
	u_int32_t calculated_crc32;
	portier_packet send_packet;
	portier_packet receive_packet;
	int bytes_to_send;
	int bytes_to_recv;
	int rc;	
	char * ts;
	struct sigaction act1, oact1;
	
	char * allowed_ip_list;
	char * token;
	struct sockaddr_in name;
	int namelen = sizeof(name);
	int ip_ok=-1;
	
	char str_id[4096];
	
	u_int32_t packet_crc32;

#ifdef HAVE_SSL
	SSL *ssl=NULL;
#endif

	allowed_ip_list=getConfigValue("allowed_ips", cfgfile);
	if(allowed_ip_list == NULL) {
        	syslog(LOG_ERR,"No allowed IP");
        	exit(1);
        	
        }
        
        
        token=strtok(allowed_ip_list,",");
        
        if (getpeername(0,(struct sockaddr *)&name, &namelen) < 0) {
   		syslog(LOG_ERR, "getpeername failed");
   		exit(1);
   	}
        
        while(token != NULL) {
        	if(strcmp(token, inet_ntoa(name.sin_addr)) == 0) {
        		ip_ok=0;	
        	}
        	token=strtok(NULL, ",");	
        }
        free(allowed_ip_list);
        
        if(ip_ok < 0) {
        	//sleep(1);
        	syslog(LOG_ERR, "ip blocked: %s", inet_ntoa(name.sin_addr));
		exit(1);
        }
        
	act1.sa_handler = portier_conn_timeout;
	sigemptyset(&act1.sa_mask);
	act1.sa_flags=0;
	#ifdef SA_INTERRUPT
	act1.sa_flags |= SA_INTERRUPT;
	#endif
	if(sigaction(SIGALRM, &act1, &oact1) < 0) {
		
		syslog(LOG_ERR,"alarm setup error");
		exit(1);
		
	}
        
       bytes_to_recv=sizeof(receive_packet);  	
#ifdef HAVE_SSL
	if(use_ssl == 1) {
		if((ssl=SSL_new(ctx))==NULL){
			syslog(LOG_ERR,"SSL init error");	
			return;
		}
		
		SSL_set_fd(ssl,sock);
		/* keep attempting the request if needed */
		while(((rc=SSL_accept(ssl))!=1) && (SSL_get_error(ssl,rc)==SSL_ERROR_WANT_READ));
		
		if(rc!=1){
			syslog(LOG_ERR,"Error: Could not complete SSL handshake. %d (%s)\n",SSL_get_error(ssl,rc), ERR_error_string(ERR_get_error(), NULL));
			return;
		}
		while(((rc=SSL_read(ssl,&receive_packet,bytes_to_recv))<=0) && (SSL_get_error(ssl,rc)==SSL_ERROR_WANT_READ));
	} else {
#endif
		
		rc=bartlby_tcp_recvall(sock,(char *)&receive_packet,&bytes_to_recv,PORTIER_CONN_TIMEOUT);
		
#ifdef HAVE_SSL
	}
#endif
	if(rc<=0){
		/* log error to syslog facility */
		syslog(LOG_ERR,"Could not read request from client bye bye ...");
		
#ifdef HAVE_SSL			
		if(use_ssl == 1) {
			if(ssl){
				SSL_shutdown(ssl);
				SSL_free(ssl);
			}
		}
#endif
		return;
			
	}
	
	if(bytes_to_recv!=sizeof(receive_packet)){

	
		/* log error to syslog facility */
		syslog(LOG_ERR,"Data packet from client was too short, bye bye ...");	
#ifdef HAVE_SSL			
		if(use_ssl == 1) {
			if(ssl){
				SSL_shutdown(ssl);
				SSL_free(ssl);
			}
		}
#endif
			
		return;		
			
	}

	packet_crc32=ntohl(receive_packet.crc32_value);
	receive_packet.crc32_value=0L;
	calculated_crc32=portier_calculate_crc32((char *)&receive_packet,sizeof(receive_packet));
	if(packet_crc32!=calculated_crc32){
		syslog(LOG_ERR,"Error: Request packet had invalid CRC32.");
		return;
	}
	
	
	receive_packet.cmdline[2048-1]='\0';
	receive_packet.plugin[2048-1]='\0';
	receive_packet.perf_handler[1024-1]='\0';
	receive_packet.output[2048-1]='\0';
	
	
	/* clear the response packet buffer */
	bzero(&send_packet,sizeof(send_packet));
	
	/* fill the packet with semi-random data */
	portier_randomize_buffer((char *)&send_packet,sizeof(send_packet));
				
	switch(ntohs(receive_packet.packet_type)) {
		
		case PORTIER_SVCLIST_PACKET:
			
			for(x=0; x<shm_hdr->svccount; x++) {
				if(svcmap[x].server_id == (int)receive_packet.service_id && svcmap[x].service_type == SVC_TYPE_PASSIVE) {
					ts=strdup(str_id);
					sprintf(str_id, "%d %s", svcmap[x].service_id, ts);
					free(ts);
					
					
				}
			}
			sprintf(send_packet.output, "%s",  str_id);
			
			
			
		break;
		
		case PORTIER_RESULT_PACKET:
			for(x=0; x<shm_hdr->svccount; x++) {
				
				if(svcmap[x].service_id == (int)receive_packet.service_id) {
					if(svcmap[x].service_type == SVC_TYPE_PASSIVE) {
						svcmap[x].last_state=svcmap[x].current_state;
						snprintf(svcmap[x].new_server_text, 2047, "%s", receive_packet.output);
						svcmap[x].current_state=(int16_t)receive_packet.exit_code;
						svcmap[x].last_check=time(NULL);
						
						if(strlen(receive_packet.perf_handler) > 5) {
							setenv("BARTLBY_CONFIG", cfgfile,1);
							setenv("BARTLBY_CURR_PLUGIN", svcmap[x].plugin,1);
							setenv("BARTLBY_CURR_HOST", svcmap[x].server_name,1);
							setenv("BARTLBY_CURR_SERVICE", svcmap[x].service_name,1);
							bartlby_perf_track(&svcmap[x],receive_packet.perf_handler, strlen(receive_packet.perf_handler), cfgfile);
							
						}
					}
				}
			}		
		break;
		
		case PORTIER_REQUEST_PACKET:
			
			for(x=0; x<shm_hdr->svccount; x++) {
				
				if(svcmap[x].service_id == (int)receive_packet.service_id) {
					if(svcmap[x].service_type == SVC_TYPE_PASSIVE) {
						sprintf(send_packet.plugin, "%s", svcmap[x].plugin);	
						sprintf(send_packet.cmdline, "%s", svcmap[x].plugin_arguments);
						
					}
				}
			}																																						

		break;
		
		default:
			syslog(LOG_ERR, "no packet type supplied or either wrong packet type");
			exit(1);	
		
	}
	
	

	/* initialize response packet data */
	send_packet.packet_type=receive_packet.packet_type;
	/* calculate the crc 32 value of the packet */
	send_packet.crc32_value=(u_int32_t)0L;
	calculated_crc32=portier_calculate_crc32((char *)&send_packet,sizeof(send_packet));
	send_packet.crc32_value=(u_int32_t)htonl(calculated_crc32);
	
	
	
	bytes_to_send=sizeof(send_packet);

#ifdef HAVE_SSL		
	if(use_ssl == 1) {
		SSL_write(ssl,&send_packet,bytes_to_send);
		if(ssl){
			SSL_shutdown(ssl);
			SSL_free(ssl);
		}
	} else {
#endif
		bartlby_tcp_sendall(sock,(char *)&send_packet,&bytes_to_send);
			
#ifdef HAVE_SSL
	}
#endif
	
	
		
}

void portier_randomize_buffer(char *buffer,int buffer_size){
	FILE *fp;
	int x;
	int seed;

	/**** FILL BUFFER WITH RANDOM ALPHA-NUMERIC CHARACTERS ****/

	/***************************************************************
	   Only use alpha-numeric characters becase plugins usually
	   only generate numbers and letters in their output.  We
	   want the buffer to contain the same set of characters as
	   plugins, so its harder to distinguish where the real output
	   ends and the rest of the buffer (padded randomly) starts.
	***************************************************************/

	/* try to get seed value from /dev/urandom, as its a better source of entropy */
	fp=fopen("/dev/urandom","r");
	if(fp!=NULL){
		seed=fgetc(fp);
		fclose(fp);
	        }

	/* else fallback to using the current time as the seed */
	else
		seed=(int)time(NULL);

	srand(seed);
	for(x=0;x<buffer_size;x++)
		buffer[x]=(int)'0'+(int)(72.0*rand()/(RAND_MAX+1.0));

	return;
        }
      

void portier_generate_crc32_table(void){
	unsigned long crc, poly;
	int i, j;

	poly=0xEDB88320L;
	for(i=0;i<256;i++){
		crc=i;
		for(j=8;j>0;j--){
			if(crc & 1)
				crc=(crc>>1)^poly;
			else
				crc>>=1;
		        }
		crc32_table[i]=crc;
                }

	return;
}


/* calculates the CRC 32 value for a buffer */
unsigned long portier_calculate_crc32(char *buffer, int buffer_size){
	register unsigned long crc;
	int this_char;
	int current_index;

	crc=0xFFFFFFFF;

	for(current_index=0;current_index<buffer_size;current_index++){
		this_char=(int)buffer[current_index];
		crc=((crc>>8) & 0x00FFFFFF) ^ crc32_table[(crc ^ this_char) & 0xFF];
	        }

	return (crc ^ 0xFFFFFFFF);
        }        
        
        
