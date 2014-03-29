/*
 * =====================================================================================
 *
 *       Filename:  TCPEchoClient4.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  18/02/14 01:20:44
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  José Guilherme Vanz (vanz), guilherme.sft@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Practical.h"

int main(int argc, char** argv)
{
	if ( arvc < 3 || argc > 4 )
		DieWithUserMessage("Parameter(s)", "<Server Address> <Echo World> [<Server Port>]");

	char *servIP = argv[1];  // Primeiro argumento: IP do servidor
	char *echoString = argv[2]; //Segundo argumento: string para o echo

	//Terceiro argumento  ( opcional ) : porta do servidor ( númerica )
	in_port_t servPort = (argc ==4) ? atoi(argv[3]) : 7;

	//Cria stream socket usando TCP
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if( sock < 0 )
		DieWithUserMessage("socket failed")

	//Construção da struct do  endereço para servidor
	struct sockaddr_in servAddr;	// endereço do servidor
	memset(&servAddr, 0, sizeof(servAddr));	//limpa struct
	servAddr.sin_family = AF_NET; //familia do endereço IPv4

	//converte endereço
	int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);
	if ( rtnVal == 0 )
		DieWithUserMessage("inet_pton failed", "endereço inválido");
	else if ( rtnVal < 0 )
		DieWithUserMessage("inet_pton failed");
	servAddr.sin_port = htons(servPort);

	//Estabelecendo conexão com o servidor do echo
	if(connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
		DieWithUserMessage("connection failed");

	size_t echoStringLen = strlen(echoString); // temanho do string de input

	//envia string para o servidor
	ssize_t numBytes = send(sock, echoString, echoStringLen);
	if ( numBytes < 0 )
		DieWithUserMessage("send failed");
	else if ( numBytes != echoStringLen)
		DieWithUserMessage("send", "set unexpected number of bytes");

	//recebe a mesma string de volta do servidor
	unsigned int totalBytesRcvd = 0; //total de bytes recebidos
	fputs("Received: ", stdout); // prepara o print da string ecoada
	while( totalBytesRcvd < echoStringLen)
	{
		char buffer[BUFSIZE]; //buffer de I/O
		numBytes = recv(sock, buffer, BUFSIZE -1, 0); // recebe a o tamanho do buffer( -1 para o char de terminador ) de bytes do servidor
		if ( numBytes < 0 )
		{
			DieWithUserMessage("recv failed");
		}else if ( numBytes == 0 )
		{
			DieWithUserMessage("recv", "connection closed prematurly");
		}
		totalBytesRcvd += numBytes; // mantendo o total de bytes
		buffer[numBytes] = '\0' // colocar char de terminação da string
		fputs(buffer,stdout); // print o buffer do echo


	}
	foutc('\n', stdout); // print final de linha

	close(sock);
	exit(0);



}


