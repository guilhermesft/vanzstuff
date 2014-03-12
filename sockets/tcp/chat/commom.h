/*
 * =====================================================================================
 *
 *       Filename:  commom.h
 *
 *    Description: Common struct and functions in both server and client side.
 *
 *        Version:  1.0
 *        Created:  02/27/2014 08:37:00 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  José Guilherme Vanz (vanz), guilherme.sft@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */


#ifndef COMMOM_HEADER
#define COMMOM_HEADER

#define MSG_TEXT_SIZE 100
#define READ_BLOCK_SIZE 512


#include <stdlib.h>

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

void prepareMsgBufferToSend (char *buffer, const message * msg);

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
