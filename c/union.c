#include <stdio.h>

union myunion
{
	char a[2];
	struct
	{
		char c;
		char d;
	}b;
};

int main( int argc, char ** args)
{
	union myunion foo;
	foo.a[0] = 'a';
	foo.a[1] = 'a';
	foo.b.c = 'c';
	foo.b.d = 'd';

	printf("Valores = %c e %c", foo.a[0], foo.a[1]);
}
