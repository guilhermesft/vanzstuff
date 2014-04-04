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
#include	"tanaguara/thread/threadpool.h"
#include	"server.h"


#define THREAD_LISTEN_NUMBER 1

typedef struct{
	int socket;
	char * nickname;
} client;

static thread_pool * tp_listen;

void startListenThread(int * socket_servidor);
void handlerClientConnection();

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:
 * =====================================================================================
 */
int main (int argc, char *argv[] )
{
	struct sockaddr_in server_config;
	socklen_t size_server;
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
	startListenThread(&socket_servidor);
	wait_thread(tp_listen);
	free(tp_listen);
	return EXIT_SUCCESS;
}

void startListenThread(int * socket_servidor)
{
	tp_listen = create_thread_pool(THREAD_LISTEN_NUMBER);
	int i = 0;
	for( ; i < THREAD_LISTEN_NUMBER; i++){
		init_new_thread(tp_listen, handlerClientConnection, socket_servidor);
	}
}

void handlerClientConnection( int * socket_servidor )
{
	struct sockaddr_in client_config;
	socklen_t size_client = sizeof(client_config);
	while(1){
		int client = accept(*socket_servidor, (struct sockaddr*) &client_config , &size_client);

		if(client < 0){
			perror("Error accept: ");
			//TODO handler error
		}else{
			//Create a thread to handler client connection
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
			}
			free(buffer);
		}
	}
}
