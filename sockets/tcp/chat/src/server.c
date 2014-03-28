/*
 * =====================================================================================
 *
 *       Filename:  server.c
 *
 *    Description: This source contains the implementation of server side of the chat application
 *
 *        Version:  1.0
 *        Created:  02/27/2014 08:36:32 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  José Guilherme Vanz (vanz), guilherme.sft@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */


#include	<stdlib.h>
#include	<stdio.h>
#include	<errno.h>
#include	<string.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>

#include	"server.h"


typedef struct{
	int socket;
	char * nickname;
} client;

client clients[];

void create_client(const char * nickname, int client_socket);

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:
 * =====================================================================================
 */
int main (int argc, char *argv[] )
{
	struct sockaddr_in server_config, client_config;
	socklen_t size_client, size_server;
	int socket_servidor;

	socket_servidor = socket(PF_INET, SOCK_STREAM, 0); //Create the socket

	memset((struct sockaddr_in *) &(server_config), 0, sizeof(server_config)); //clean memory
	server_config.sin_family = PF_INET;
	server_config.sin_port = htons(4040);
	server_config.sin_addr.s_addr = inet_addr("127.0.0.1");

	size_server = sizeof(server_config);
	int retorno_bind = bind(socket_servidor, (struct sockaddr*)&server_config, size_server );
	if (retorno_bind == -1){
		perror("Error de bind: ");
		return -1;
	}

	int retorno = listen(socket_servidor, 5);

	if(retorno == -1){
		perror("Erro no listen: ");
		return -1;
	}
	while(1){
		size_client = sizeof(client_config);
		int client = accept(socket_servidor, (struct sockaddr*) &client_config , &size_client);

		if(client < 0){
			perror("Error accept: ");
			return -1;
		}else{
			logInfo("Client connected :)");
			char* buffer = malloc(READ_BLOCK_SIZE);
			ssize_t recv_bytes = recv(client, buffer, READ_BLOCK_SIZE, 0);

			if(recv_bytes < 0){
				logError("Error reading the message");
				perror("Error reading the message");
			}else{
				logDebug("%d BYTES RECIVED", recv_bytes);
				command * cmd = (command*) malloc(sizeof(command));
				buffer_to_command(buffer, cmd);
				logInfo("%s connected!", cmd->data);
				create_client(cmd->data, client);
			}
			free(buffer);
		}
	}
	return EXIT_SUCCESS;
}

int create_client(const char* nickname, int socket)
{
	client * client = (client*) malloc(sizeof(client));
	client->socket = socket;
	client->nickname = (char*) malloc(strlen(nickname));
	strcpy(&client->nickname, nickname);

}
