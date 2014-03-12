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
#include "log.h"
#include <string.h>

size_t get_message_byte_size(const message * msg)
{
	return  ( sizeof(msg->length) + strlen(msg->msg) );
}

message* create_message(const char* msg_text)
{
	message* msg = (message*) malloc(sizeof(message));
	msg->msg = (char*) malloc(strlen(msg_text));
	memset(msg->msg,0,strlen(msg_text));
	msg->length = strlen(msg_text);
	strcpy(msg->msg, msg_text);
	return msg;
}

message* getMsgFromBuffer ( const char* buffer)
{
	size_t lenSize = sizeof(size_t);
	message * msg = (message*) malloc(sizeof(message));
	memcpy( &msg->length, buffer, lenSize);
	msg->msg = malloc(msg->length);
	memcpy(msg->msg, buffer + lenSize, msg->length);
	return msg;
}

void getBufferToSend ( const message * msg, void *buffer )
{
	logDebug("Preparing buffer to send...");
	memcpy(buffer, &msg->length, sizeof(msg->length) );
	memcpy(buffer+sizeof(msg->length), msg->msg, strlen(msg->msg) );
	logDebug("BUFFER ( %d length ) = %s", get_message_byte_size(msg), (char*)buffer );
	logDebug("Buffer ready");
}		/* -----  end of function getBufferToSend  ----- */
