/*
 * =====================================================================================
 *
 *       Filename:  client.h
 *
 *    Description:  :
 *
 *        Version:  1.0
 *        Created:  03/12/2014 08:30:52 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  José Guilherme Vanz (vanz), guilherme.sft@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

#ifndef __HEADER_CLIENT__
#define __HEADER_CLIENT__


#include "commom.h"

/* *
 * message type is used to send a message between server and client side.
 * Its fields:
 * 	length : size_t field that contains the size of the message
 * 	msg : char pointer that constains the message
 * */
typedef struct {
	size_t length;
	char *msg;
} message;


size_t get_message_byte_size(const message * msg);

message* create_message(const char* msg_text);

void prepare_msg_buffer_to_send (char *buffer, const message * msg);

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  getMsgFromBuffer
 *  Description: Convert a chat buffer with bytes recived from socket to a message pointer
 * =====================================================================================
 */
message* getMsgFromBuffer ( const char* buffer);

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  getBufferToSend
 *  Description:  Generate a buffer to send
 * =====================================================================================
 */
void getBufferToSend ( const message * msg, void* buffer );

#endif
