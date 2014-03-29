/*
 * =====================================================================================
 *
 *       Filename:  bit_operations.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  03/13/2014 09:14:11 AM
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
#include	<string.h>

char symbol[2] = {'0','1'};

void show_int_bit(const char* varName, int value);
void show_byte_bit( unsigned char value);
void show_char_array_bits(const char* varName,char value[], unsigned int length);

void show_byte_bit( unsigned char value)
{
	int i =0;
	for(; i < 8; i++){
		printf("%c", symbol[(value>>i) & 0x01] );
	}

}

void show_int_bit(const char* varName, int value)
{
	unsigned int bit_count = sizeof(int) * 8;
	printf("%s (%d bits) = ", varName, bit_count);
	int i = 0, conter = 0;
	for(; i < bit_count; i++){
		if(conter == 8){
			printf(" ");
			conter = 0;
		}
		printf("%c", symbol[(value>>i) & 0x01] );
		conter++;
	}
	printf("\n");
}

void show_char_array_bits( const char* varName, char value[], unsigned int length)
{
	int i = 0;
	printf("%s = ", varName);
	for(; i < length; i++){
		show_byte_bit(value[i]);
		printf(" ");
	}
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:
 * =====================================================================================
 */
int main ( int argc, char *argv[] )
{
	int teste = 1234021;
	show_int_bit("TESTE", teste);
	teste <<= 8;
	show_int_bit("TESTE", teste);
	char buf[12] = { 255,0,0,0,'c', 'a', 'm', 'p', 'o', 'I', 'n', 't'};
	show_char_array_bits( "buf = ", buf, 12);
	return 1;
	unsigned int* num = (int*) malloc(sizeof(int));
	*num = 16;
	unsigned int x;
	unsigned char a[4];

	a[3] = (*num>>24) & 0xFF;
	a[2] = (*num>>16) & 0xFF;
	a[1] = (*num>>8) & 0xFF;
	a[0] = *num & 0xFF;

	int index = 0;
	for(index = 0; index < 4; index++ ){
		printf("Byte %d = %d\n", index, a[index]);
	}

	x = *(int *)a;
	printf("x = %d\n", x);

	printf("---------\n");
	int valor = 16;
	char * buffer = (char* )malloc(sizeof(int));
	memset(buffer, 0, sizeof(int));

	*buffer = ( valor >> 32 );

	int i = 0;
	for( i = 0; i < sizeof(int); i++){
		printf("buffer[%d] = %d\n", i, *(buffer+i));
	}

	valor = 0;
	valor = (*buffer >> 32);
	printf("valor = %d\n", valor);
	free(buffer);

	printf("---------\n");

	valor = 0;

	valor = (*buf >> 32); *buf >>= 32;
	char * nome = (char*) malloc(valor + 1);
	i = 0;
	for(; i < valor; i++){
		nome[i] = buf[i];

	}
	nome[valor] = '\0';
	printf("valor = %d\n", valor);
	printf("nome = %s\n", nome);

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
