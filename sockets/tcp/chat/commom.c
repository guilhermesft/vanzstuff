/*
 * =====================================================================================
 *
 *       Filename:  commom.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  03/17/2014 05:47:37 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  José Guilherme Vanz (vanz), guilherme.sft@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <string.h>

#include "commom.h"

size_t command_to_buffer (command * cmd, char* buffer)
{
	size_t buf_size = command_size(cmd);
	memset(buffer, 0, buf_size);
	memcpy(buffer, &cmd->type, sizeof(cmd->type));
	memcpy(buffer, &cmd->data_size, sizeof(cmd->data_size));
	memcpy(buffer, cmd->data, cmd->data_size);
	return buf_size;
}

size_t command_size(command * cmd)
{
	return sizeof(command) + cmd->data_size;
}
