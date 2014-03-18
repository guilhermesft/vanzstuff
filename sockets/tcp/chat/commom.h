/*
 * =====================================================================================
 *
 *       Filename:  commom.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  03/12/2014 09:28:02 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  José Guilherme Vanz (vanz), guilherme.sft@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

/**
 * TODO - Documentation
 */
typedef enum{
	CONNECT = 1,
	DISCONNECT,

} command_type;

/**
 * TODO - Documentation
 */
typedef struct{
	command_type type;
	char * data;
	size_t data_size;
} command;

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  command_to_buffer
 *  Description: Function fill a buffer with data of a command
 * =====================================================================================
 */
size_t command_to_buffer (command * cmd, char* buffer);

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  command_size
 *  Description: Function return the size in bytes of a command
 * =====================================================================================
 */
size_t command_size(command * cmd);
