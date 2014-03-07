/*
 * =====================================================================================
 *
 *       Filename:  commom.c
 *
 *    Description:  Implementation of commom function user on server and client side
 *
 *        Version:  1.0
 *        Created:  06/03/14 00:57:35
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  José Guilherme Vanz (vanz), guilherme.sft@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

#include "commom.h"
#include <string.h>

size_t message_byte_size(const message * msg)
{
	return sizeof(message) + strlen(msg->msg) +1;
}

message* create_message(const char* msg_text)
{
	message* msg = (message*) malloc(sizeof(message));
	msg->length = strlen(msg_text);
	strcpy(msg->msg, msg_text);
	return msg;
}
