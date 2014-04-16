/**
 * This source code implements a simple sample of System V shared memory segment usage.
 * It's not using semaphores, I'll write other program to practice that.
 * */

#include    <stdlib.h>
#include    <stdio.h>
#include    <unistd.h>
#include    <string.h>
#include    <sys/wait.h>
#include    <sys/shm.h>
#include    <sys/ipc.h>
#include    <fcntl.h>

#define PG_SIZE getpagesize()
#define PROJ_ID 1
#define SHM_FILE "myshm"

void executeChild( void );
void executeParent( void );

int main ( int argc, char *argv[] )
{
	int fd = creat(SHM_FILE, S_IRWXU );
	if( fd == -1){
		perror("File creation failed");
		exit(1);
	}
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
	close(fd);
	return EXIT_SUCCESS;
}

void executeChild( void )
{
	printf("Child running\n");
	sleep(5);
	key_t token = ftok(SHM_FILE, PROJ_ID);
	if( token == -1 ){
		perror("Child - Token creation failed");
		exit(1);
	}
	printf("Child - Token = %d\n", token);
	int segment = shmget( token, PG_SIZE, O_RDONLY);
	if ( segment == -1 ){
		perror("Child - Segment creation failed");
		exit(1);
	}
	char * shm_ptr = (char*) shmat(segment, NULL, 0);
	if( shm_ptr == (void *)(-1) ){
		perror("Child - Segment attachament failed");
		exit(1);
	}
	printf("Child - Shared memory ( %d ) attached\n", segment);
	struct shmid_ds shm_info;
	if( shmctl(segment, IPC_STAT, &shm_info) == -1 ){
		perror("Child - shmctl failed");
		exit(1);
	}
	printf("Child - Segment size = %zu\n", shm_info.shm_segsz);
	char * read_data = malloc(PG_SIZE);
	strncpy(read_data, shm_ptr, PG_SIZE);
	printf("Data read: %s\n", read_data);
	free(read_data);
	if( shmdt(shm_ptr) == -1 ){
		perror("Detach failed");
		exit(1);
	}
	close(segment);

}

void executeParent( void )
{
	printf("Parent running\n");
	key_t token = ftok(SHM_FILE, PROJ_ID);
	if( token == -1 ){
		perror("Token creation failed");
		exit(1);
	}
	printf("Token = %d\n", token);
	int segment = shmget( token, PG_SIZE, IPC_CREAT | O_WRONLY | S_IRWXU);
	if ( segment == -1 ){
		perror("Segment creation failed");
		exit(1);
	}
	char * shm_ptr = (char*) shmat(segment, NULL, 0);
	if( shm_ptr == (void *)(-1) ){
		perror("Segment attachament failed");
		exit(1);
	}
	printf("Shared memory ( %d ) attached\n", segment);
	struct shmid_ds shm_info;
	if( shmctl(segment, IPC_STAT, &shm_info) == -1 ){
		perror("shmctl failed");
		exit(1);
	}
	printf("Segment size = %zu\n", shm_info.shm_segsz);
	printf("Writing...\n");
	char * test = "Hello";
	strncpy(shm_ptr, test, PG_SIZE);
	*(shm_ptr + strlen(test)) = '\0';
	sleep(7);
	shmctl(segment, IPC_RMID, &shm_info);
	if( shmdt(shm_ptr) == -1 ){
		perror("Detach failed");
		exit(1);
	}
	close(segment);
}
