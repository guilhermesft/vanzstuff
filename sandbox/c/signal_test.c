/*
 * =====================================================================================
 *
 *       Filename:  signal_test.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  02/19/2014 09:49:16 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  José Guilherme Vanz (vanz), guilherme.sft@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

#include 	<stdio.h>
#include	<stdlib.h>
#include 	<signal.h>

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  RelerConfiguracao
 *  Description:
 * =====================================================================================
 */
void RelerConfiguracao ( int signal )
{
	printf("Recebeu sinal %d\n", signal);
	printf("Reler arquivos de configuração...\n");
}		/* -----  end of function RelerConfiguracao  ----- */



/*
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:
 * =====================================================================================
 */
int main ( int argc, char *argv[] )
{
	signal(SIGHUP, RelerConfiguracao);
	while(1);
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
