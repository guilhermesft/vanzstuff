/*
 * =====================================================================================
 *
 *       Filename:  shmposix.c
 *
 *    Description: This source implements a simple sample of POSIX standart shared memory
 *    		   This semple does not use semaphores. Semaphores semple is will implemented
 *    		   This sample also has not a lot of good practices. This sample is just for
 *		   fun. ;)
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
#include <sys/wait.h>

#define SIZE getpagesize()
#define SHM_NAME "/vanz"

struct msg{
	char * msg;
};

void executeParent();
void executeChild();

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:
 * =====================================================================================
 */
int main ( int argc, char *argv[] )
{
	int pid = fork();
	if( pid ){
		executeParent();
		printf("Parent waiting...\n");
		int status = 0;
		wait(&status); //wait for child process
		printf("Parent done\n");
	}else{
		executeChild();
		printf("Child done!\n");
	}
	return EXIT_SUCCESS;
}

void executeParent()
{
	int shmfd = shm_open(SHM_NAME, O_CREAT | O_RDWR, S_IRWXU);
	if(shmfd < 0){
		perror("[Parent] shm_open failed");
		exit(1);
	}
	struct msg * data = (struct msg*) mmap(NULL,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0 );
	if( data == MAP_FAILED ){
		perror("[Parent] SHM map failed: ");
		exit(1);
	}
	if( ftruncate(shmfd, SIZE) == -1 ){
		perror("[Parent] ftruncate failed");
		exit(1);
	}
	data->msg = "Hi, Child!";
	printf("Parent wrote = %s\n", data->msg);
	sleep(3);
	printf("Parent read = %s\n", data->msg);
	munmap(data, SIZE);
	close(shmfd);
}

void executeChild()
{
	sleep(1);
	int shmfd = shm_open(SHM_NAME, O_RDWR, S_IRWXU);
	if(shmfd < 0){
		perror("[Child] shm_open failed");
		exit(1);
	}
	struct msg * data = (struct msg*) mmap(NULL,SIZE, PROT_WRITE | PROT_READ , MAP_SHARED, shmfd, 0 );
	if( data == MAP_FAILED ){
		perror("[Child] SHM map failed: ");
		exit(1);
	}
	printf("Child read = %s\n", data->msg);
	data->msg="Hello, father!";
	printf("Child wrote = %s\n", data->msg);
	munmap(data, SIZE);
	close(shmfd);
}
