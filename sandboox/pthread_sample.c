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
	int x = 0;
	while( x++ < 5 ){
		printf("Value = %d\n", *value);
	}
}
