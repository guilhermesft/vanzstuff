/*
 * =====================================================================================
 *
 *       Filename:  fifo_consumer.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  02/19/2014 12:56:17 PM
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
#include 	<errno.h>
#include	<string.h>
#include	<fcntl.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include 	<unistd.h>

#define FIFO_FILE "/tmp/fifo"

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:
 * =====================================================================================
 */
int main ( int argc, char *argv[] )
{
	int n,fd;

	char msg[500];
	mknod(FIFO_FILE,S_IFIFO | 0666, 0);
	fd = open(FIFO_FILE, O_RDONLY);
	n = read(fd, msg, 500);
	msg[n] = '\0';
	printf("Dados enviados pelo produtori: %s\n", msg);
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
