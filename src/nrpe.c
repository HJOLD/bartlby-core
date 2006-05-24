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
Revision 1.3  2006/05/24 13:22:45  hjanuschka
*** empty log message ***

Revision 1.2  2006/05/24 13:12:51  hjanuschka
nrpe: ARG1 fix

Revision 1.1  2006/05/24 13:07:39  hjanuschka
NRPE support (--enable-nrpe)

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

#include <bartlby.h>





#ifndef WITH_NRPE
void bartlby_check_nrpe(struct service * svc, char * cfgfile, int use_ssl) {
	sprintf(svc->new_server_text, "%s", "NRPE  is expiremental Support ist not compiled in (--enable-nrpe=yes) bartlby-core");
	svc->current_state = STATE_CRITICAL;
}

#else

static int conn_timedout = 0;
static unsigned long crc32_table[256];


#ifdef HAVE_SSL
#include <openssl/dh.h>
#include <openssl/ssl.h>

DH *get_dh512()
	{
	static unsigned char dh512_p[]={
		0xA5,0x6F,0x2C,0x53,0x06,0x55,0x5C,0xD7,0xAB,0x08,0x2E,0x56,
		0x39,0x8F,0x10,0x3B,0x2E,0xB1,0x6E,0xBA,0x79,0x34,0x1E,0x06,
		0x2A,0xEF,0xF6,0xE3,0xEB,0x7D,0xB4,0x57,0x01,0x4B,0x9E,0x1F,
		0x4C,0xCB,0x7F,0xA3,0x6F,0x67,0xFE,0x7D,0x5E,0x19,0x7D,0x88,
		0x3E,0xB2,0x6D,0x41,0xD1,0x4A,0x10,0x25,0x13,0x36,0x8D,0x79,
		0x57,0x40,0x4B,0x4B,
		};
	static unsigned char dh512_g[]={
		0x02,
		};
	DH *dh;

	if ((dh=DH_new()) == NULL) return(NULL);
	dh->p=BN_bin2bn(dh512_p,sizeof(dh512_p),NULL);
	dh->g=BN_bin2bn(dh512_g,sizeof(dh512_g),NULL);
	if ((dh->p == NULL) || (dh->g == NULL))
		{ DH_free(dh); return(NULL); }
	return(dh);
	}

#endif



#define PROGRAM_VERSION "2.3-bartlby"
#define MODIFICATION_DATE "05-24-2006"
#define STATE_UNKNOWN   3
#define MAX_PACKETBUFFER_LENGTH 1024
#define OK		0
#define DEFAULT_SOCKET_TIMEOUT  10
#define TRUE            1
#define FALSE           0
#define MAX_INPUT_BUFFER        2048
#define DEFAULT_SERVER_PORT 5666
#define DEFAULT_NRPE_COMMAND    "_NRPE_CHECK"  /* check version of NRPE daemon */
#define QUERY_PACKET		1
#define NRPE_PACKET_VERSION_2   2
#define	RESPONSE_PACKET		2

#define OK		0
#define ERROR		-1

#define TRUE		1
#define FALSE		0


typedef struct packet_struct{
        int16_t   packet_version;
        int16_t   packet_type;
        u_int32_t crc32_value;
        int16_t   result_code;
        char      buffer[MAX_PACKETBUFFER_LENGTH];
        }packet;
        

void nrpe_generate_crc32_table(void);
int my_connect(char *host_name,int port,int *sd,char *proto, struct service * svc);
void alarm_handler(int sig);
int my_tcp_connect(char *host_name,int port,int *sd, struct service * svc);
int my_inet_aton(register const char *cp, struct in_addr *addr);
unsigned long calculate_crc32(char *buffer, int buffer_size);
void randomize_buffer(char *buffer,int buffer_size);
int nrperecvall(int s, char *buf, int *len, int timeout);        
int nrpesendall(int s, char *buf, int *len);


void bartlby_check_nrpe(struct service * svc, char * cfgfile, int use_ssl) {
	
	int sd;
	
	char query[MAX_INPUT_BUFFER]="";
	
	#ifdef HAVE_SSL
	SSL_METHOD *meth;
	SSL_CTX *ctx;
	SSL *ssl;
	#endif
     
     u_int32_t packet_crc32;
     u_int32_t calculated_crc32;
     int16_t result;
     int rc;
     packet send_packet;
     packet receive_packet;
     int bytes_to_send;
     int bytes_to_recv;
     
     
      /* generate the CRC 32 table */
      nrpe_generate_crc32_table();
        
#ifdef HAVE_SSL
        /* initialize SSL */
        if(use_ssl==TRUE){
                SSL_library_init();
                SSLeay_add_ssl_algorithms();
                meth=SSLv23_client_method();
                SSL_load_error_strings();
                if((ctx=SSL_CTX_new(meth))==NULL){
                        sprintf(svc->new_server_text, "%s", "CHECK_NRPE: Error - could not create SSL context.\n");
                        svc->current_state=STATE_CRITICAL;
                        }

                
                /* use only TLSv1 protocol */
                SSL_CTX_set_options(ctx,SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3);
       }
#endif	
	
	signal(SIGALRM,alarm_handler);

	conn_timedout=0;
	alarm(svc->service_check_timeout);
	result=my_tcp_connect(svc->client_ip,svc->client_port,&sd, svc);
	
	if(conn_timedout == 1) {
		sprintf(svc->new_server_text, "%s", "timed out");
		svc->current_state=STATE_CRITICAL;	
		return;
	}
	if(result != STATE_OK) {
		svc->current_state=STATE_CRITICAL;
		return;
	}
	if(result == STATE_OK) {
#ifdef HAVE_SSL
        /* do SSL handshake */
		if(use_ssl==TRUE){
     		if((ssl=SSL_new(ctx))!=NULL){
     	     	SSL_CTX_set_cipher_list(ctx,"ADH");
				SSL_set_fd(ssl,sd);
     	          if((rc=SSL_connect(ssl))!=1){
     	          	sprintf(svc->new_server_text, "%s", "CHECK_NRPE: Error - Could not complete SSL handshake.\n");
     	          	svc->current_state=STATE_CRITICAL;
     	          	SSL_CTX_free(ctx);
                        	
     	          	return;
     	          }
			} else {
				sprintf(svc->new_server_text,"CHECK_NRPE: Error - Could not create SSL connection structure.\n"); 
				svc->current_state=STATE_CRITICAL;
				SSL_CTX_free(ctx);
                    close(sd);
                    return;
			}
		}
#endif		
		bzero(&send_packet,sizeof(send_packet));
     	
     	/* fill the packet with semi-random data */
     	randomize_buffer((char *)&send_packet,sizeof(send_packet));
     	
     	
     	snprintf(query,sizeof(query),"%s",svc->plugin);
        	query[sizeof(query)-1]='\x0';
        	strcat(query,"!!");
        	strncat(query,svc->plugin_arguments,strlen(svc->plugin_arguments));
          query[sizeof(query)-1]='\x0';
     	
     	
     	/* initialize packet data */
     	send_packet.packet_version=(int16_t)htons(NRPE_PACKET_VERSION_2);
     	send_packet.packet_type=(int16_t)htons(QUERY_PACKET);
     	strncpy(&send_packet.buffer[0],query,MAX_PACKETBUFFER_LENGTH);
     	send_packet.buffer[MAX_PACKETBUFFER_LENGTH-1]='\x0';	
     	
     	send_packet.crc32_value=(u_int32_t)0L;
          calculated_crc32=calculate_crc32((char *)&send_packet,sizeof(send_packet));
          send_packet.crc32_value=(u_int32_t)htonl(calculated_crc32);
     	
     	 bytes_to_send=sizeof(send_packet);
           if(use_ssl==FALSE)
           	rc=nrpesendall(sd,(char *)&send_packet,&bytes_to_send);
#ifdef HAVE_SSL
		else{
          	rc=SSL_write(ssl,&send_packet,bytes_to_send);
               if(rc<0)
               	rc=-1;
			}
			
#endif
		if(rc==-1){
			sprintf(svc->new_server_text, "%s", "CHECK_NRPE: Error sending query to host.\n");
			close(sd);
			svc->current_state=STATE_CRITICAL;
			return;
		}
     	bytes_to_recv=sizeof(receive_packet);
     	if(use_ssl==FALSE)
			rc=nrperecvall(sd,(char *)&receive_packet,&bytes_to_recv,svc->service_check_timeout);
#ifdef HAVE_SSL
		else
          	rc=SSL_read(ssl,&receive_packet,bytes_to_recv);
#endif

		/* reset timeout */
		alarm(0);		
#ifdef HAVE_SSL
		if(use_ssl==TRUE){
		        SSL_shutdown(ssl);
		        SSL_free(ssl);
		        SSL_CTX_free(ctx);
		}
#endif    
	if(rc<0){
		sprintf(svc->new_server_text, "%s", "CHECK_NRPE: Error receiving data from daemon.\n");
		svc->current_state=STATE_CRITICAL;
		return;
	}else if(rc==0){
		sprintf(svc->new_server_text,"%s", "CHECK_NRPE: Received 0 bytes from daemon.  Check the remote server logs for error messages.\n");
		svc->current_state=STATE_CRITICAL;
		return;
	}else if(bytes_to_recv<sizeof(receive_packet)){
		sprintf(svc->new_server_text, "CHECK_NRPE: Receive underflow - only %d bytes received (%d expected).\n",bytes_to_recv,sizeof(receive_packet));
		svc->current_state=STATE_CRITICAL;
		return;
	}
           
	packet_crc32=ntohl(receive_packet.crc32_value);
	receive_packet.crc32_value=0L;
	calculated_crc32=calculate_crc32((char *)&receive_packet,sizeof(receive_packet));
	if(packet_crc32!=calculated_crc32){
	        sprintf(svc->new_server_text,"%s", "CHECK_NRPE: Response packet had invalid CRC32.\n");
	        svc->current_state=STATE_CRITICAL;
	        close(sd);
	        return;
	        }
	
	/* check packet version */
	if(ntohs(receive_packet.packet_version)!=NRPE_PACKET_VERSION_2){
	        sprintf(svc->new_server_text,"%s","CHECK_NRPE: Invalid packet version received from server.\n");
	        close(sd);
	        svc->current_state=STATE_CRITICAL;
	        return;
	        }
	
	/* check packet type */
	if(ntohs(receive_packet.packet_type)!=RESPONSE_PACKET){
	        sprintf(svc->new_server_text,"%s","CHECK_NRPE: Invalid packet type received from server.\n");
	        close(sd);
	        svc->current_state=STATE_CRITICAL;
	        return;
	        }
	
	/* get the return code from the remote plugin */
	result=(int16_t)ntohs(receive_packet.result_code);
	
	/* print the output returned by the daemon */
	receive_packet.buffer[MAX_PACKETBUFFER_LENGTH-1]='\x0';
	if(!strcmp(receive_packet.buffer,""))
	        sprintf(svc->new_server_text,"%s","CHECK_NRPE: No output returned from daemon.\n");
	else
	        sprintf(svc->new_server_text,"%s",receive_packet.buffer);


	
	}
	switch ((int16_t)ntohs(receive_packet.result_code)) {
		case STATE_OK:
		case STATE_WARNING:
		case STATE_CRITICAL:
			svc->current_state=(int16_t)ntohs(receive_packet.result_code);
		break;
		
		default:
		
		svc->current_state=STATE_CRITICAL;
				
	}
	
                    
     
	return;
	
	
	
		
}

void nrpe_generate_crc32_table(void){
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
int my_connect(char *host_name,int port,int *sd,char *proto, struct service * svc){
	struct sockaddr_in servaddr;
	struct hostent *hp;
	struct protoent *ptrp;
	int result;

	bzero((char *)&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(port);

	/* try to bypass using a DNS lookup if this is just an IP address */
	if(!my_inet_aton(host_name,&servaddr.sin_addr)){

		/* else do a DNS lookup */
		hp=gethostbyname((const char *)host_name);
		if(hp==NULL){
			sprintf(svc->new_server_text, "Invalid host name '%s'\n",host_name);
			svc->current_state=STATE_CRITICAL;
			return STATE_UNKNOWN;
		        }

		memcpy(&servaddr.sin_addr,hp->h_addr,hp->h_length);
	        }

	/* map transport protocol name to protocol number */
	if(((ptrp=getprotobyname(proto)))==NULL){
		sprintf(svc->new_server_text, "Cannot map \"%s\" to protocol number\n",proto);
		svc->current_state=STATE_CRITICAL;
		return STATE_UNKNOWN;
	        }

	/* create a socket */
	*sd=socket(PF_INET,(!strcmp(proto,"udp"))?SOCK_DGRAM:SOCK_STREAM,ptrp->p_proto);
	if(*sd<0){
		sprintf(svc->new_server_text, "Socket creation failed\n");
		svc->current_state=STATE_CRITICAL;
		return STATE_UNKNOWN;
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

void alarm_handler(int sig){

        conn_timedout = 1;
       
}
int my_tcp_connect(char *host_name,int port,int *sd, struct service * svc){
	int result;

	result=my_connect(host_name,port,sd,"tcp", svc);

	return result;
}


/* This code was taken from Fyodor's nmap utility, which was originally taken from
   the GLIBC 2.0.6 libraries because Solaris doesn't contain the inet_aton() funtion. */
int my_inet_aton(register const char *cp, struct in_addr *addr){
	register unsigned int val;	/* changed from u_long --david */
	register int base, n;
	register char c;
	u_int parts[4];
	register u_int *pp = parts;

	c=*cp;

	for(;;){

		/*
		 * Collect number up to ``.''.
		 * Values are specified as for C:
		 * 0x=hex, 0=octal, isdigit=decimal.
		 */
		if (!isdigit((int)c))
			return (0);
		val=0;
		base=10;

		if(c=='0'){
			c=*++cp;
			if(c=='x'||c=='X')
				base=16,c=*++cp;
			else
				base=8;
		        }

		for(;;){
			if(isascii((int)c) && isdigit((int)c)){
				val=(val*base)+(c -'0');
				c=*++cp;
			        } 
			else if(base==16 && isascii((int)c) && isxdigit((int)c)){
				val=(val<<4) | (c+10-(islower((int)c)?'a':'A'));
				c = *++cp;
			        } 
			else
				break;
		        }

		if(c=='.'){

			/*
			 * Internet format:
			 *	a.b.c.d
			 *	a.b.c	(with c treated as 16 bits)
			 *	a.b	(with b treated as 24 bits)
			 */
			if(pp>=parts+3)
				return (0);
			*pp++=val;
			c=*++cp;
		        } 
		else
			break;
	        }

	/* Check for trailing characters */
	if(c!='\0' && (!isascii((int)c) || !isspace((int)c)))
		return (0);

	/* Concoct the address according to the number of parts specified */
	n=pp-parts+1;
	switch(n){

	case 0:
		return (0);		/* initial nondigit */

	case 1:				/* a -- 32 bits */
		break;

	case 2:				/* a.b -- 8.24 bits */
		if(val>0xffffff)
			return (0);
		val|=parts[0]<<24;
		break;

	case 3:				/* a.b.c -- 8.8.16 bits */
		if(val>0xffff)
			return (0);
		val|=(parts[0]<< 24) | (parts[1]<<16);
		break;

	case 4:				/* a.b.c.d -- 8.8.8.8 bits */
		if(val>0xff)
			return (0);
		val|=(parts[0]<<24) | (parts[1]<<16) | (parts[2]<<8);
		break;
	        }

	if(addr)
		addr->s_addr=htonl(val);

	return (1);
        }
        
void randomize_buffer(char *buffer,int buffer_size){
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
unsigned long calculate_crc32(char *buffer, int buffer_size){
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
        

/* sends all data - thanks to Beej's Guide to Network Programming */
int nrpesendall(int s, char *buf, int *len){
	int total=0;
	int bytesleft=*len;
	int n=0;

	/* send all the data */
	while(total<*len){

		/* send some data */
		n=send(s,buf+total,bytesleft,0);

		/* break on error */
		if(n==-1)
			break;

		/* apply bytes we sent */
		total+=n;
		bytesleft-=n;
	        }

	/* return number of bytes actually send here */
	*len=total;

	/* return -1 on failure, 0 on success */
	return n==-1?-1:0;
        }


/* receives all data - modelled after sendall() */
int nrperecvall(int s, char *buf, int *len, int timeout){
	int total=0;
	int bytesleft=*len;
	int n=0;
	time_t start_time;
	time_t current_time;
	
	/* clear the receive buffer */
	bzero(buf,*len);

	time(&start_time);

	/* receive all data */
	while(total<*len){

		/* receive some data */
		n=recv(s,buf+total,bytesleft,0);

		/* no data has arrived yet (non-blocking socket) */
		if(n==-1 && errno==EAGAIN){
			time(&current_time);
			if(current_time-start_time>timeout)
				break;
			sleep(1);
			continue;
		        }

		/* receive error or client disconnect */
		else if(n<=0)
			break;

		/* apply bytes we received */
		total+=n;
		bytesleft-=n;
	        }

	/* return number of bytes actually received here */
	*len=total;

	/* return <=0 on failure, bytes received on success */
	return (n<=0)?n:total;
        }
#endif











