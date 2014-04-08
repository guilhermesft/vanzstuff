/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  04/07/2014 06:02:05 PM
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
#include	<string.h>
#include	<sys/mman.h>
#include	<sys/stat.h>
#include	<sys/fcntl.h>

#define SIZE sysconf(_SC_PAGESIZE)

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:
 * =====================================================================================
 */
int main ( int argc, char *argv[] )
{
	int pid = fork();
	int shared_memory;
	if(pid){

		shared_memory = shm_open("/myshm", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
		void * addr = mmap( NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory, 0);
		if( addr == MAP_FAILED){
			perror("Map failed");
		}
		char * hello = "Hello";
		ftruncate(shared_memory,strlen(hello));
		memcpy(addr, hello, strlen(hello));
	}else{
	}
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
