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
#include <semaphore.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <sys/wait.h>

#define SIZE getpagesize()
#define SHM_NAME "/vanz"
#define SEM_NAME "/semvanz"
#define LOOP_COUNT 25

struct shared_data{
	int msg;
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
		if( fork() ){
			executeParent();
			printf("Parent waiting...\n");
			int status = 0;
			wait(&status); //wait for child process
			if( shm_unlink(SEM_NAME) == -1 ){
				perror( "sem unlink failed");
				exit(1);
			}
			if( shm_unlink(SHM_NAME) == -1 ){
				perror("shm unlink failed");
				exit(1);
			}
			printf("Parent done\n");
		} else {
			executeChild();
		}
	}else{
		executeChild();
	}
	return EXIT_SUCCESS;
}

void executeParent()
{
	int shmfd = shm_open(SHM_NAME, O_CREAT | O_RDWR, S_IRWXU);
	int semfd = shm_open(SEM_NAME, O_CREAT | O_RDWR, S_IRWXU); //semaphore
	if(shmfd < 0 || semfd < 0){
		perror("[Parent] shm_open failed");
		exit(1);
	}
	struct shared_data * data = (struct shared_data*) mmap(NULL,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0 );
	sem_t * sem = (sem_t*) mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, semfd, 0 );
	if( data == MAP_FAILED || sem == MAP_FAILED ){
		perror("[Parent] SHM map failed: ");
		exit(1);
	}
	if( ftruncate(shmfd, SIZE) == -1 ){
		perror("[Parent] ftruncate failed");
		exit(1);
	}
	if( ftruncate(semfd, sizeof(sem_t)) == -1 ){
		perror("[Parent] semaphore ftruncate failed");
		exit(1);
	}
	if( sem_init(sem, 1, 1) == -1){
		perror("[Parent] Semaphore inicialization failed");
		exit(1);
	}
	data->msg = 1;
	while( data->msg < LOOP_COUNT){
		if ( sem_wait(sem) == -1 ){
			perror("[Parent] Semaphore lock failed");
			continue;
		}
		if(data->msg % 2 == 0){
			data->msg += 1;
			printf("[Parent] Msg = %d\n", data->msg);
		}
		if( sem_post(sem) == -1){
			perror("[Parent] Semaphore unlock failed");
			continue;
		}

	}
	munmap(data, SIZE);
}

void executeChild()
{
	int shmfd = shm_open(SHM_NAME, O_RDWR, S_IRWXU);
	int semfd = shm_open(SEM_NAME, O_RDWR, S_IRWXU); //semaphore
	if(shmfd < 0 || semfd < 0){
		while(shmfd < 0 && errno == ENOENT){
			printf("[Child( %d )] Trying to open the shm\n", getpid());
			errno = 0;
			shmfd = shm_open(SHM_NAME, O_RDWR, S_IRWXU);
		}
		while(semfd < 0 && errno == ENOENT){
			printf("[Child( %d )] Trying to open the semaphore\n", getpid());
			errno = 0;
			semfd = shm_open(SEM_NAME, O_RDWR, S_IRWXU);
		}
		if( errno != 0 ){
			perror("[Child] shm_open failed");
			exit(1);
		}
	}
	struct shared_data * data = (struct shared_data*) mmap(NULL,SIZE, PROT_WRITE | PROT_READ , MAP_SHARED, shmfd, 0 );
	sem_t * sem = (sem_t*) mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, semfd, 0 );
	if( data == MAP_FAILED || sem == MAP_FAILED){
		perror("[Child] SHM map failed: ");
		exit(1);
	}
	if( sem_init(sem, 1, 1) == -1){
		perror("[Child] Semaphore inicialization failed");
		exit(1);
	}
	while( data->msg < LOOP_COUNT){
		if ( sem_wait(sem) == -1 ){
			perror("[Child] Semaphore lock failed");
			continue;
		}
		if(data->msg % 2 != 0){
			data->msg += 1;
			printf("[Child( %d )] Msg = %d\n", getpid(), data->msg);
		}
		if( sem_post(sem) == -1){
			perror("[Child] Semaphore unlock failed");
			continue;
		}

	}
	munmap(data, SIZE);
	printf("Child( %d ) done\n", getpid());
}
