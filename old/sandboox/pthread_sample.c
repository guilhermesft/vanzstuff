/*
 * =====================================================================================
 *
 *       Filename:  pthread_sample.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  04/02/2014 07:53:01 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  José Guilherme Vanz (vanz), guilherme.sft@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define THREAD_NUMBER 5

void print_function(int *value);

int main( int argc, char ** argv)
{
	pthread_t ** threads = calloc(THREAD_NUMBER, sizeof(pthread_t));
	int i,value;
	value = 9;
	for(i = 0; i < THREAD_NUMBER; i++){
		threads[i] = malloc(sizeof(pthread_t));
		pthread_create(threads[i], NULL,  (void*) print_function, (void*) &value );

	}
	for(i = 0; i < THREAD_NUMBER; i++){
		pthread_join(*threads[i], NULL);
	}

	free(threads);
	return 0;
}


void print_function(int *value)
{
	static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
	if(pthread_mutex_lock(&lock) == 0){
		static int x = 0;
		int count = 0;
		while( count < 5 ){
			printf("Value = %d\n", x);
			x += 1;
			count += 1;
		}
		pthread_mutex_unlock(&lock);
	}
}
