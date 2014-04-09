/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  04/09/2014 12:22:24 AM
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
#include	<unistd.h>
#include	<sys/wait.h>

void executeChild( void );
void executeParent( void );

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:
 * =====================================================================================
 */
	int
main ( int argc, char *argv[] )
{
	int pid = fork();
	if( pid < 0){
		perror("Fork failed\n");
		return EXIT_FAILURE;
	}
	if( pid ){
		executeParent();
		printf("Parent waiting...\n");
		int status = 0;
		wait(&status); //wait for child process
		printf("Parent done\n");

	}else{
		executeChild();
	}
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

void executeChild( void )
{
	printf("Child running\n");
	sleep(10);
}

void executeParent( void )
{
	printf("Parent running\n");
}
