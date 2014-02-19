/*
 * =====================================================================================
 *
 *       Filename:  fifo_productor.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  02/19/2014 09:58:28 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  José Guilherme Vanz (vanz), guilherme.sft@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

#include 	<stdio.h>
#include	<stdlib.h>
#include	<errno.h>
#include 	<string.h>
#include 	<fcntl.h>
#include 	<sys/types.h>
#include 	<sys/stat.h>
#include 	<unistd.h>

#define FIFO_FILE "/temp/fifo" // name of fifo file

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:
 * =====================================================================================
 */
int main ( int argc, char *argv[] )
{
	int fd;
	char * msg = "Text to send for the pipe FIFO";

	// create FIFO
	mknod(FIFO_FILE, S_IFIFO | 0666, 0);

	printf("Waiting for the consumer...\n");
	fd = open(FIFO_FILE, O_WRONLY);
	printf("Consumer open the pipe\n");
	write(fd, msg, strlen(msg));
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
