/*
 * =====================================================================================
 *
 *       Filename:  SyncHRDT_2.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  02/26/2014 06:46:16 PM
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
#include	<time.h>
#include 	<arpa/inet.h>
#include	<unistd.h>

struct HrDt
{
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
};

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  HoraData
 *  Description:
 * =====================================================================================
 */
struct HrDt HoraData ()
{
	struct HrDt Hora_Data;
	struct tm *ptr;
	time_t lt;

	lt = time(NULL);
	ptr = (struct tm*) localtime(&lt);

	Hora_Data.tm_sec = ptr->tm_sec;
	Hora_Data.tm_min = ptr->tm_min;
	Hora_Data.tm_hour = ptr->tm_hour;
	Hora_Data.tm_mday = ptr->tm_mday;
	Hora_Data.tm_mon = ptr->tm_mon;
	Hora_Data.tm_year = ptr->tm_year;
	return Hora_Data;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:
 * =====================================================================================
 */
int main ( int argc, char *argv[] )
{
	struct HrDt Est_Hora_Data;
	struct sockaddr_in EndLocal, EndRemoto;
	int sockfd, len, retorno;
	socklen_t lenRemoto;
	int cliente;

	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	len = sizeof(EndLocal);

	memset((struct sockaddr_in *) &(EndLocal), 0, len);
	EndLocal.sin_family = PF_INET;
	EndLocal.sin_port = htons(4040);
	EndLocal.sin_addr.s_addr = inet_addr("192.168.0.1");
	retorno = bind(sockfd, (struct sockaddr*) &EndLocal, len);

	if(retorno == -1){
		exit(0);
	}
	listen(sockfd, 5);
	while(1){
		cliente = accept(sockfd, (struct sockaddr *) &EndRemoto, &lenRemoto);
		Est_Hora_Data = HoraData();
		send(cliente, &Est_Hora_Data, sizeof(Est_Hora_Data), 0);
		close(cliente);
	}
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
