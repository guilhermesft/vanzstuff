/* *
 * C programming language (2nd edition)
 * Exercise 1-20
 * */

#include <stdio.h>

#define MAX_LINE 1000
#define TAB_SPACES 8

/*  functions */
int readline(char line[]);
void convertSpaces(char line[], int len);

int main( int argc, char** argv ){
	char line[MAX_LINE];
	int len = 0;
	while( (len = readline(line)) > 0){
		convertSpaces(line, len);
		printf("%s", line);
	}
	return 0;
}

/* *
 * Read a line from stdin
 * */
int readline(char line[]){
	int i, len = 0;
	char c;
	for( i = 0; i < MAX_LINE && (c=getchar()) != EOF && c != '\n'; ++i){
		line[i] = c;
		len++;
	}
	return len;
}

/* *
 * Convert white spaces to tabs
 * */
void convertSpaces(char line[], int len){
	int spaceCount, current = 0;
	char newLine[MAX_LINE];
	for( current = 0; current < len; ++current){
		if( spaceCount > 0 ) {
			//is countin white spaces
			if( line[current] == ' ' ){
				spaceCount++;
			}
			if( spaceCount == TAB_SPACES ){

			}
		} else {

		}
	}

}
