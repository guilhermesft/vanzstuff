/*
 * =====================================================================================
 *
 *       Filename:  shmposix.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  04/15/2014 12:11:32 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  José Guilherme Vanz (vanz), guilherme.sft@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/fcntl.h>

#define SIZE getpagesize()

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:
 * =====================================================================================
 */
int main ( int argc, char *argv[] )
{
	int shmfd = shm_open("/vanz", O_CREAT | O_RDWR, S_IRWXU);
	if(shmfd < 0){
		perror("shm_open failed");
		exit(1);
	}
	char * data = (char*) mmap(NULL,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0 );
	if( data == MAP_FAILED ){
		perror("SHM map failed: ");
		exit(1);
	}
	if( ftruncate(shmfd, SIZE) == -1 ){
		perror("ftruncate failed");
		exit(1);
	}
	char * msg = "Hi, Child!";
	strncpy(data, msg, SIZE);
	munmap(data, SIZE);
	close(shmfd);
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
