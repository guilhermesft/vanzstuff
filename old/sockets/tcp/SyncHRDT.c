/*
 * =====================================================================================
 *
 *       Filename:  SyncHRDT.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  02/26/2014 05:38:29 PM
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
#include	<sys/types.h>
#include	<netinet/in.h>
#include	<time.h>
#include 	<arpa/inet.h>
#include	<unistd.h>

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  HoraData
 *  Description:
 * =====================================================================================
 */
char* HoraData ()
{
	struct tm *ptr;
	time_t lt;

	lt = time(NULL);
	ptr = (struct tm*)localtime(&lt);
	return (char*)asctime(ptr);
}


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:
 * =====================================================================================
 */
int main ( int argc, char *argv[] )
{
	struct sockaddr_in EndLocal, EndRemoto;
	int sockfd, len, retorno;
	socklen_t lenRemoto;
	int cliente;

	char *buffer;
	buffer = malloc(sizeof(char)*100);
	sockfd = socket(PF_INET, SOCK_STREAM, 0);

	len = sizeof(EndLocal);
	memset((struct sockaddr_in * ) &(EndLocal), 0, len);
	EndLocal.sin_family = PF_INET;
	EndLocal.sin_port = htons(4040);
	EndLocal.sin_addr.s_addr = inet_addr("127.0.0.1");

	retorno = bind(sockfd, (struct sockaddr*) &EndLocal, len);
	if( retorno == -1 ){
		exit(0);
	}
	listen(sockfd,5);
	while(1){
		cliente = accept(sockfd, (struct sockaddr*)&EndRemoto, &lenRemoto);
		buffer = HoraData();
		send(cliente, buffer, strlen(buffer),0);
		close(cliente);
	}
}
