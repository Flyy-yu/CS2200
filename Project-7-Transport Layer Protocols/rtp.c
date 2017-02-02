#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "rtp.h"

/* GIVEN Function:
 * Handles creating the client's socket and determining the correct
 * information to communicate to the remote server
 */
CONN_INFO* setup_socket(char* ip, char* port){
	struct addrinfo *connections, *conn = NULL;
	struct addrinfo info;
	memset(&info, 0, sizeof(struct addrinfo));
	int sock = 0;

	info.ai_family = AF_INET;
	info.ai_socktype = SOCK_DGRAM;
	info.ai_protocol = IPPROTO_UDP;
	getaddrinfo(ip, port, &info, &connections);

	/*for loop to determine corr addr info*/
	for(conn = connections; conn != NULL; conn = conn->ai_next){
		sock = socket(conn->ai_family, conn->ai_socktype, conn->ai_protocol);
		if(sock <0){
			if(DEBUG)
				perror("Failed to create socket\n");
			continue;
		}
		if(DEBUG)
			printf("Created a socket to use.\n");
		break;
	}
	if(conn == NULL){
		perror("Failed to find and bind a socket\n");
		return NULL;
	}
	CONN_INFO* conn_info = malloc(sizeof(CONN_INFO));
	conn_info->socket = sock;
	conn_info->remote_addr = conn->ai_addr;
	conn_info->addrlen = conn->ai_addrlen;
	return conn_info;
}

void shutdown_socket(CONN_INFO *connection){
	if(connection)
		close(connection->socket);
}

/* 
 * ===========================================================================
 *
 *			STUDENT CODE STARTS HERE. PLEASE COMPLETE ALL FIXMES
 *
 * ===========================================================================
 */


/*
 *  Returns a number computed based on the data in the buffer.
 */
static int checksum(char *buffer, int length){

int sum = 0;
for (int i = 0; i < length; i++)
{
	/* code */
	sum = sum+(int)buffer[i];

}
return sum;
	/*  ----  FIXME  ----
	 *
	 *  The goal is to return a number that is determined by the contents
	 *  of the buffer passed in as a parameter.  There a multitude of ways
	 *  to implement this function.  For simplicity, simply sum the ascii
	 *  values of all the characters in the buffer, and return the total.
	 */ 
}

/*
 *  Converts the given buffer into an array of PACKETs and returns
 *  the array.  The value of (*count) should be updated so that it 
 *  contains the length of the array created.
 */
static PACKET* packetize(char *buffer, int length, int *count){

int packagenum;
if (length% MAX_PAYLOAD_LENGTH == 0)
{
	packagenum = length/MAX_PAYLOAD_LENGTH;
}else{
	packagenum = length/MAX_PAYLOAD_LENGTH + 1;
}

PACKET *packarray = malloc(packagenum * sizeof(PACKET));

for (int i = 0; i < packagenum; i++)
{
	if (MAX_PAYLOAD_LENGTH * (i + 1) < length)
	{
		packarray[i].payload_length = MAX_PAYLOAD_LENGTH;
		packarray[i].type = DATA;
	}
	else{
		packarray[i].payload_length = length - MAX_PAYLOAD_LENGTH*i;
		packarray[i].type = LAST_DATA;
	}

	

for (int j = 0; j < packarray[i].payload_length; j++)
{
	packarray[i].payload[j]=buffer[MAX_PAYLOAD_LENGTH*i+j];
	/* code */
}

packarray[i].checksum = checksum(packarray[i].payload,packarray[i].payload_length);
	/* code */
}

*count = packagenum;
return packarray;

	
}

/*
 * Send a message via RTP using the connection information
 * given on UDP socket functions sendto() and recvfrom()
 */
int rtp_send_message(CONN_INFO *connection, MESSAGE*msg){

int *count = malloc(sizeof(int));
PACKET *pack = packetize(msg->buffer,msg->length,count);
PACKET *response=malloc(sizeof(PACKET));

for (int i = 0; i < *count; i++)
{
	sendto(connection->socket, &pack[i],sizeof(PACKET),0,connection->remote_addr,connection->addrlen);

	recvfrom(connection->socket,response,sizeof(PACKET),0,connection->remote_addr,&connection->addrlen );

if (response->type == NACK)
{
	i=i-1;
}
}


	/* ---- FIXME ----
	 * The goal of this function is to turn the message buffer
	 * into packets and then, using stop-n-wait RTP protocol,
	 * send the packets and re-send if the response is a NACK.
	 * If the response is an ACK, then you may send the next one
	 */
	 return 1;
}

/*
 * Receive a message via RTP using the connection information
 * given on UDP socket functions sendto() and recvfrom()
 */
MESSAGE* rtp_receive_message(CONN_INFO *connection){
	

	MESSAGE *message = malloc(sizeof(MESSAGE));
message->length = 0;

	PACKET *package = malloc(sizeof(PACKET));
PACKET *response = malloc(sizeof(PACKET));
do
{
recvfrom(connection->socket, package, sizeof(PACKET), 0,
				connection->remote_addr, &connection->addrlen);
if (checksum(package->payload,package->payload_length)==package->checksum)
{

	response->type=ACK;
	sendto(connection->socket, response, sizeof(PACKET),0,connection->remote_addr,connection->addrlen);
char* part = malloc((package->payload_length +message->length)*sizeof(char));
			for (int i = 0; i < message->length; i++)
			{
				part[i] = message->buffer[i];
			}
			for (int i = 0; i < package->payload_length; i++)
			{
				part[i + message->length] = package->payload[i];
			}
			
			message->buffer = part;
			message->length = message->length + package->payload_length;
}

else{
	response->type = NACK;
	sendto(connection->socket, response, sizeof(PACKET),0,connection->remote_addr,connection->addrlen);
	continue;
}

} while (response->type!=ACK || package->type!=LAST_DATA );

  return message;
}
