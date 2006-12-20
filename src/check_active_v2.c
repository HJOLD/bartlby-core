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
based on Ethan gelstad's nrpe: http://nagios.cvs.sourceforge.net/nagios/nrpe/src/check_nrpe.c?view=log
$Revision$
$Source$


$Log$
Revision 1.7  2006/12/20 21:28:56  hjanuschka
performance on large

Revision 1.6  2006/12/05 03:47:12  hjanuschka
auto commit

Revision 1.5  2006/11/28 21:37:25  hjanuschka
auto commit

Revision 1.3  2006/11/28 03:30:42  hjanuschka
auto commit

Revision 1.2  2006/11/27 21:16:28  hjanuschka
auto commit

Revision 1.1  2006/11/25 22:04:40  hjanuschka
*** empty log message ***

Revision 1.1  2006/11/25 00:54:23  hjanuschka
auto commit

*/


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <netdb.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <time.h>
#include <fcntl.h>


#ifdef HAVE_SSL 
	#include <openssl/dh.h>
	#include <openssl/ssl.h>
	#include <openssl/err.h>
	#include <bartlby_v2_dh.h>
#endif

#include <bartlby.h>



static int conn_timedout = 0;
static unsigned long crc32_table[256];



void agent_v2_generate_crc32_table(void);
int agent_v2_my_connect(char *host_name,int port,int *sd,char *proto, struct service * svc);
void agent_v2_alarm_handler(int sig);
int agent_v2_my_tcp_connect(char *host_name,int port,int *sd, struct service * svc);
unsigned long agent_v2_calculate_crc32(char *buffer, int buffer_size);
void agent_v2_randomize_buffer(char *buffer,int buffer_size);



void bartlby_check_v2(struct service * svc, char * cfgfile, int use_ssl) {
	
	int sd;
	
#ifdef HAVE_SSL 
	SSL_METHOD *meth;
	SSL_CTX *ctx;
	SSL *ssl;
#endif     
	u_int32_t packet_crc32;
	u_int32_t calculated_crc32;
	int16_t result;
	int rc;
	agent_v2_packet send_packet;
	agent_v2_packet receive_packet;
	int bytes_to_send;
	int bytes_to_recv;
     
	signal(SIGALRM,agent_v2_alarm_handler);
      /* generate the CRC 32 table */
	agent_v2_generate_crc32_table();
	
#ifdef HAVE_SSL
	if(use_ssl == 1) {
		
		meth=SSLv23_client_method();
       	if((ctx=SSL_CTX_new(meth))==NULL){
			sprintf(svc->new_server_text, "%s", "AgentV2: Error - could not create SSL context.");
       		svc->current_state=STATE_CRITICAL;
       		_log("%s", ERR_error_string(ERR_get_error(), NULL));
		}
		/* use only TLSv1 protocol */
		SSL_CTX_set_options(ctx,SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3);
	}
#endif

	conn_timedout=0;
	alarm(svc->service_check_timeout);
	result=agent_v2_my_tcp_connect(svc->client_ip,svc->client_port,&sd, svc);
	
	if(conn_timedout == 1) {
		sprintf(svc->new_server_text, "%s", "timed out");
		svc->current_state=STATE_CRITICAL;	
		return;
	}
	if(result != STATE_OK) {
		svc->current_state=STATE_CRITICAL;
		return;
	}
	
#ifdef HAVE_SSL	
	if(use_ssl == 1) {
		/* do SSL handshake */
		if((ssl=SSL_new(ctx))!=NULL){
			SSL_CTX_set_cipher_list(ctx,"ADH");
			SSL_set_fd(ssl,sd);
			
			rc=SSL_connect(ssl);
			
			conn_timedout=0;
			alarm(svc->service_check_timeout);
				
			if(rc !=1){
				sprintf(svc->new_server_text, "%s", "AgentV2: Error - Could not complete SSL handshake.");
					_log("SSL_error: %s", ERR_error_string(ERR_get_error(), NULL));
     		         		svc->current_state=STATE_CRITICAL;
     		         		SSL_CTX_free(ctx);
     		         		return;
			}
		} else {
			sprintf(svc->new_server_text,"AgentV2: Error - Could not create SSL connection structure."); 
			svc->current_state=STATE_CRITICAL;
			_log("%s", ERR_error_string(ERR_get_error(), NULL));
			SSL_CTX_free(ctx);
			close(sd);
			return;
		}
	} 
#endif
	
	
	
	bzero(&send_packet,sizeof(send_packet));
     	
     	/* fill the packet with semi-random data */
     	agent_v2_randomize_buffer((char *)&send_packet,sizeof(send_packet));

     	/* initialize packet data */
	sprintf(send_packet.plugin, "%s", svc->plugin);
	sprintf(send_packet.cmdline, "%s", svc->plugin_arguments);
	sprintf(send_packet.perf_handler, " ");
	
	send_packet.packet_type=(int16_t)htons(AGENT_V2_SENT_PACKET);
	
     	send_packet.crc32_value=(u_int32_t)0L;
     	
     	
	calculated_crc32=agent_v2_calculate_crc32((char *)&send_packet,sizeof(send_packet));
	
	send_packet.crc32_value=(u_int32_t)htonl(calculated_crc32);
     	
	bytes_to_send=sizeof(send_packet);


	conn_timedout=0;
	alarm(svc->service_check_timeout);
#ifdef HAVE_SSL
	if(use_ssl == 1) {
		
		rc=SSL_write(ssl,&send_packet,bytes_to_send);
		
	} else {
#endif
		rc=bartlby_tcp_sendall(sd,(char *)&send_packet,&bytes_to_send);
		
			
#ifdef HAVE_SSL
	}
#endif
	if(conn_timedout == 1) {
		_log("V2: timeout ok");
		sprintf(svc->new_server_text, "%s", "V2 timed out2");
		svc->current_state=STATE_CRITICAL;	
		return;
	}
       if(rc<0)
       	rc=-1;

	if(rc==-1){
		sprintf(svc->new_server_text, "%s", "AgentV2: Error sending to host");
		close(sd);
		svc->current_state=STATE_CRITICAL;
		return;
	}
     	bytes_to_recv=sizeof(receive_packet);

	conn_timedout=0;
	alarm(svc->service_check_timeout);
	
#ifdef HAVE_SSL
	if(use_ssl == 1) {
		
		rc=SSL_read(ssl,(char *)&receive_packet,bytes_to_recv);
		
	} else {
#endif       
       
       rc=bartlby_tcp_recvall(sd,(char *)&receive_packet,&bytes_to_recv,svc->service_check_timeout);
       
#ifdef HAVE_SSL       
	}
#endif

       if(conn_timedout == 1) {
		_log("timeout ok");
		sprintf(svc->new_server_text, "%s", "timed out4");
		svc->current_state=STATE_CRITICAL;	
		return;
	}
	/* reset timeout */
	alarm(0);

#ifdef HAVE_SSL 		
	if(use_ssl == 1) {
		SSL_shutdown(ssl);
		SSL_free(ssl);
		SSL_CTX_free(ctx);
	}
#endif

	close(sd);

	if(rc<0){
		sprintf(svc->new_server_text, "%s", "AgentV2: Error receiving data from agent");
		svc->current_state=STATE_CRITICAL;
		return;
	}else if(rc==0){
		sprintf(svc->new_server_text,"%s", "AgentV2: Received 0 bytes from agent");
		svc->current_state=STATE_CRITICAL;
		return;
	}else if(bytes_to_recv<sizeof(receive_packet)){
		sprintf(svc->new_server_text, "AgentV2: Receive underflow - only %d bytes received (%ld expected).\n",bytes_to_recv,sizeof(receive_packet));
		svc->current_state=STATE_CRITICAL;
		return;
	}
           
	packet_crc32=ntohl(receive_packet.crc32_value);
	receive_packet.crc32_value=0L;
	calculated_crc32=agent_v2_calculate_crc32((char *)&receive_packet,sizeof(receive_packet));
	if(packet_crc32!=calculated_crc32){
		sprintf(svc->new_server_text,"%s", "AgentV2: Response packet had invalid CRC32.");
		svc->current_state=STATE_CRITICAL;
		close(sd);
		return;
	}
	
	

	
	/* check packet type */
	if(ntohs(receive_packet.packet_type)!=AGENT_V2_RETURN_PACKET){
		sprintf(svc->new_server_text,"%s","AgentV2: Invalid packet type received from server.");
		close(sd);
		svc->current_state=STATE_CRITICAL;
		return;
	}
	
	/* get the return code from the remote plugin */
	result=(int16_t)receive_packet.exit_code;
	
	/* print the output returned by the daemon */
	receive_packet.output[2048-1]='\x0';
	if(!strcmp(receive_packet.output,"")) {
		sprintf(svc->new_server_text,"%s","AgentV2: No output returned from agent");
	} else {
		sprintf(svc->new_server_text,"%s",receive_packet.output);
	}

	switch ((int16_t)receive_packet.exit_code) {
		case STATE_OK:
		case STATE_WARNING:
		case STATE_CRITICAL:
			svc->current_state=(int16_t)receive_packet.exit_code;
		break;
		
		default:
		
		svc->current_state=STATE_CRITICAL;
				
	}
	
	if(strlen(receive_packet.perf_handler) > 6) {
		bartlby_perf_track(svc,receive_packet.perf_handler, strlen(receive_packet.perf_handler), cfgfile);
		//_log("perf: %s", receive_packet.perf_handler);
	}
                    
     
	return;
	
	
	
		
}

void agent_v2_generate_crc32_table(void){
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


/* opens a tcp or udp connection to a remote host */
int agent_v2_my_connect(char *host_name,int port,int *sd,char *proto, struct service * svc){
	struct sockaddr_in servaddr;
	struct hostent *hp;
	struct protoent *ptrp;
	int result;

	bzero((char *)&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(port);

	hp=gethostbyname((const char *)host_name);
	if(hp==NULL){
		sprintf(svc->new_server_text, "Invalid host name '%s'\n",host_name);
		svc->current_state=STATE_CRITICAL;
		return STATE_CRITICAL;
	}

	memcpy(&servaddr.sin_addr,hp->h_addr,hp->h_length);
	

	/* map transport protocol name to protocol number */
	if(((ptrp=getprotobyname(proto)))==NULL){
		sprintf(svc->new_server_text, "Cannot map \"%s\" to protocol number\n",proto);
		svc->current_state=STATE_CRITICAL;
		return STATE_CRITICAL;
	        }

	/* create a socket */
	*sd=socket(PF_INET,(!strcmp(proto,"udp"))?SOCK_DGRAM:SOCK_STREAM,ptrp->p_proto);
	if(*sd<0){
		sprintf(svc->new_server_text, "Socket creation failed\n");
		svc->current_state=STATE_CRITICAL;
		return STATE_CRITICAL;
	        }

	/* open a connection */
	result=connect(*sd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	if(result<0){
		switch(errno){  
		case ECONNREFUSED:
			sprintf(svc->new_server_text, "Connection refused by host\n");
			svc->current_state=STATE_CRITICAL;
			break;
		case ETIMEDOUT:
			sprintf(svc->new_server_text, "Timeout while attempting connection\n");
			svc->current_state=STATE_CRITICAL;
			break;
		case ENETUNREACH:
			sprintf(svc->new_server_text, "Network is unreachable\n");
			svc->current_state=STATE_CRITICAL;
			break;
		default:
			sprintf(svc->new_server_text, "Connection refused or timed out\n");
			svc->current_state=STATE_CRITICAL;
		}

			return STATE_CRITICAL;
	 }

	return STATE_OK;
}

void agent_v2_alarm_handler(int sig){

        conn_timedout = 1;
        //_log("FIXME: nrpe timeout SSL_connect");
        
       
       
}
int agent_v2_my_tcp_connect(char *host_name,int port,int *sd, struct service * svc){
	int result;

	result=agent_v2_my_connect(host_name,port,sd,"tcp", svc);

	return result;
}


        
void agent_v2_randomize_buffer(char *buffer,int buffer_size){
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
      
        
/* calculates the CRC 32 value for a buffer */
unsigned long agent_v2_calculate_crc32(char *buffer, int buffer_size){
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
        



