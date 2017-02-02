/* CS 2200 - Project 7 - Networking
 * Client file.
 * Name:
 * gtid:
**/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "rtp.h"

void print_use_and_exit(){
	fprintf(stderr,"Usage:  prj7-client host port\n\n");
	fprintf(stderr,"   example ./prj7-client 127.0.0.1 4000\n\n");
	exit (EXIT_FAILURE);
}

int main(int argc, char**argv){
	char *host,*port;
	char* messages[5];
	messages[0] = "C is quirky flawed and an enormous success - Dennis M. Ritchie";
	messages[1] = "The question of whether computers can think is like the question of whether submarines can swim - Edsger W. Dijkstra";
	messages[2] = "Typing is no substitute for thinking. - Dartmouth Basic Manual";
	messages[3] = "Any C programmer here ? Yeah i got a C in programming class. That counts ? - Unknown";
	messages[4] = "The Internet?  Is that thing still around? - Homer Simpson";
	int num_msgs = 5;

	if(argc < 3 || argc > 3){
		printf("Incorrect number of args.\n");
		print_use_and_exit();
	}else{
		host = argv[1];
		port = argv[2];
	}

	CONN_INFO* connection = setup_socket(host, port);
	if(connection == NULL) print_use_and_exit();
	for(int i = 0; i < num_msgs; i++){
		MESSAGE*msg = malloc(sizeof(MESSAGE));
		msg->buffer = messages[i];
		msg->length = strlen(messages[i])+1;

		rtp_send_message(connection, msg);
		printf("Sent message %i to server...\n", i);

		MESSAGE*response = rtp_receive_message(connection);
		if(response) printf("Received reponse:\n\t%s\n\n", response->buffer);
		else{
			perror("Error must have occurred while receiving response. No message returned\n");
			free(connection);
			exit(EXIT_FAILURE);
		}
		free(msg);
	}
	shutdown_socket(connection);
	free(connection);

	return 0;
}
