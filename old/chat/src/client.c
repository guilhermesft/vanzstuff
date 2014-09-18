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

#include	"client.h"
#include 	"log.h"

int create_socket(const char* server_ip, unsigned int port);
int connect_server(const char* nickname, int socket_server);

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:
 * =====================================================================================
 */
int main ( int argc, char *argv[] )
{

	int socket_server = create_socket("127.0.0.1",4040); //create the socket with the server
	if(socket_server > 0){
		connect_server("vanz", socket_server);
	}
	return 0;
}

int connect_server(const char* nickname, int socket_server)
{
	command * cmd = (command*) malloc(sizeof(command));
	cmd->data = strdup(nickname);
	cmd->data_size = strlen(cmd->data);
	cmd->type = CONNECT;
	char* buffer = (char*) malloc(command_size(cmd));
	size_t buf_size = command_to_buffer(cmd, buffer );
	int send_ret = send(socket_server, buffer, buf_size, 0 );
	if(send_ret < 0){ // send success
		return send_ret;
	}
	//TODO recive response from server
	free(cmd);
	free(buffer);
	return 1;
}

int create_socket(const char* server_ip, unsigned int port)
{
	int socket_server;
	int connection;
	struct sockaddr_in server_config;
	socklen_t server_size;

	socket_server = socket(PF_INET, SOCK_STREAM, 0); // TCP/IP socket

	memset(&server_config, 0, sizeof(server_config));
	server_config.sin_family = PF_INET;
	server_config.sin_port = htons(port);
	server_config.sin_addr.s_addr = inet_addr(server_ip);

	server_size = sizeof(server_config);
	connection = connect(socket_server, (struct sockaddr*) &server_config, server_size);

	if(connection < 0){
		logInfo("Socket creation failed");
		return connection;
	}
	logDebug("Socket created");
	return socket_server;
}


size_t get_message_byte_size(const message * msg)
{
	return  ( sizeof(msg->length) + strlen(msg->msg) );
}

message* create_message(const char* msg_text)
{
	message* msg = (message*) malloc(sizeof(message));
	msg->msg = (char*) malloc(strlen(msg_text));
	memset(msg->msg,0,strlen(msg_text));
	msg->length = strlen(msg_text);
	strcpy(msg->msg, msg_text);
	return msg;
}


message* getMsgFromBuffer ( const char* buffer)
{
	size_t lenSize = sizeof(size_t);
	message * msg = (message*) malloc(sizeof(message));
	memcpy( &msg->length, buffer, lenSize);
	msg->msg = malloc(msg->length);
	memcpy(msg->msg, buffer + lenSize, msg->length);
	return msg;
}

void getBufferToSend ( const message * msg, void *buffer )
{
	logDebug("Preparing buffer to send...");
	memcpy(buffer, &msg->length, sizeof(msg->length) );
	memcpy(buffer+sizeof(msg->length), msg->msg, strlen(msg->msg) );
	logDebug("BUFFER ( %d length ) = %s", get_message_byte_size(msg), (char*)buffer );
	logDebug("Buffer ready");
}
