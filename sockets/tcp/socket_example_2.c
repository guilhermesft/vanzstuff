/*
 * =====================================================================================
 *
 *       Filename:  socket_example_2.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  02/25/2014 06:07:53 PM
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
#include	<string.h>
#include	<sys/socket.h>
#include	<netinet/in.h>

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:
 * =====================================================================================
 */
int main ( int argc, char *argv[] )
{
	int socket_servidor, socket_cliente;
	int size_cliente;
	struct sockaddr_in servidor;
	struct sockaddr_in cliente;
	int retorno;

	socket_servidor = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servidor, 0, sizeof(servidor));
	servidor.sin_family = PF_INET;
	servidor.sin_port = htons(3000);
	servidor.sin_addr.s_addr = inet_addr("192.168.0.1");
	retorno = bind(socket_servidor, (struct sockaddr*) &servidor, sizeof(servidor));

	if(retorno == -1){
		perror("Erro no bind: ");
	}
	retorno = listen(socket_servidor, 5);
	if(retorno == -1){
		perror("Erro no listen: ");
	}

	socket_cliente = accept(socket_servidor, (struct sockaddr *) &cliente,&size_cliente);

	if(socket_cliente == -1){
		perror("Erro accept: ");
	}else{
		printf("Cliente conectou\n");
	}
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
