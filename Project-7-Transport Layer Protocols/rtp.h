#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#ifndef _RTP_H
#define _RTP_H

#define MAX_PAYLOAD_LENGTH (10)
#define DEBUG (1)

enum {DATA=0, LAST_DATA, ACK, NACK, TERM};

typedef struct _PACKET{
	int type;
	int checksum;
	int payload_length;
	char payload[MAX_PAYLOAD_LENGTH];
} PACKET;

typedef struct _CONN_INFO{
	int socket;
	socklen_t addrlen;
	struct sockaddr *remote_addr;
} CONN_INFO;

typedef struct _MESSAGE{
	int length;
	char* buffer;
} MESSAGE;

CONN_INFO* setup_socket(char* ip, char* port);
int rtp_send_message(CONN_INFO *connection, MESSAGE*msg);
MESSAGE* rtp_receive_message(CONN_INFO *connection); 
void shutdown_socket(CONN_INFO *connection);

#endif
