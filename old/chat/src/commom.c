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
	int buf_index = 0;
	memset(buffer, 0, buf_size);
	memcpy(buffer, &cmd->type, sizeof(cmd->type));
	buf_index += sizeof(cmd->type);
	memcpy(buffer + buf_index, &cmd->data_size, sizeof(cmd->data_size));
	buf_index += sizeof(cmd->data_size);
	memcpy(buffer + buf_index, cmd->data, cmd->data_size);
	return buf_size;
}

size_t command_size(command * cmd)
{
	return sizeof(cmd->type) + sizeof(cmd->data_size) + cmd->data_size;
}

void buffer_to_command ( const char* buffer, command * cmd )
{
	if(cmd){
		int buf_index = 0;
		memcpy(&cmd->type, buffer, sizeof(cmd->type));
		buf_index += sizeof(cmd->type);
		memcpy(&cmd->data_size, buffer + buf_index, sizeof(cmd->data_size));
		buf_index += sizeof(cmd->data_size);
		cmd->data = (char*) malloc(cmd->data_size + 1);
		cmd->data[cmd->data_size] = '\0';
		memcpy(cmd->data, buffer + buf_index, cmd->data_size);
	}
}
