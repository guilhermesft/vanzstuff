/*
 * =====================================================================================
 *
 *       Filename:  client.c
 *
 *    Description: This source contains the implementation of client side of the chat application
 *
 *        Version:  1.0
 *        Created:  02/27/2014 08:36:41 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  José Guilherme Vanz (vanz), guilherme.sft@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */


#include	<stdlib.h>
#include	<sys/socket.h>
#include	<arpa/inet.h>
#include	<string.h>
#include	<netinet/in.h>
#include	<stdio.h>

#include	"commom.h"
#include	"log.h"

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:
 * =====================================================================================
 */
int main ( int argc, char *argv[] )
{
	int socket_server;
	int connection;
	struct sockaddr_in server_config;
	socklen_t server_size;

	socket_server = socket(PF_INET, SOCK_STREAM, 0); // TCP/IP socket

	memset(&server_config, 0, sizeof(server_config));
	server_config.sin_family = PF_INET;
	server_config.sin_port = htons(4040);
	server_config.sin_addr.s_addr = inet_addr("127.0.0.1");

	server_size = sizeof(server_config);

	connection = connect(socket_server, (struct sockaddr*) &server_config, server_size);

	if(connection == -1){
		logError("Connection not established");
		perror("Connection error: ");
		return EXIT_SUCCESS;
	}
	logInfo("Connection established");

	//sending a message to the server

	message * msg = create_message("Oi server! Funcionou porrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrraaaaaaaaaaaaaaaaaaaaa!!!!!!!!!!!!!!!!!!!");
	void *buffer = malloc( get_message_byte_size(msg) );
	getBufferToSend(msg, buffer);
	if(buffer){
		ssize_t return_send = send(socket_server, buffer, get_message_byte_size(msg) , 0);

		if(return_send == -1){
			perror("Error sending the message: ");
		}
		logDebug("Message sent");
	}
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
