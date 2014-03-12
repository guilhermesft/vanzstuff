/*
 * =====================================================================================
 *
 *       Filename:  log.h
 *
 *    Description:  This headers constains the prototype of functions used to log information
 *
 *        Version:  1.0
 *        Created:  02/03/14 21:07:49
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  José Guilherme Vanz (vanz), guilherme.sft@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */


#ifndef LOG_HEADER
#define LOG_HEADER

#include <stdio.h>
#include <stdarg.h>

typedef enum {
	INFO = 0,
	WARNING,
	ERROR,
	DEBUG
}LOG_TYPE;


void logInfo(const char * const msg, ...);
void logWarning(const char * const msg, ...);
void logError(const char * const msg, ...);
void logDebug(const char * const msg, ...);

#endif
