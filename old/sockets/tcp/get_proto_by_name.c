/*
 * =====================================================================================
 *
 *       Filename:  get_proto_by_name.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  02/24/2014 07:53:37 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  José Guilherme Vanz (vanz), guilherme.sft@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

#include 	<stdlib.h>
#include	<stdio.h>
#include	<netdb.h>

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:
 * =====================================================================================
 */
int main ( int argc, char *argv[] )
{
	struct protoent * protocolo;
	protocolo = getprotobyname("tcp");
	printf("Protocolo: TCP - Número: %d\n", protocolo->p_proto);
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
