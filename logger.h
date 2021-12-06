/* logger.h */
#ifndef __LOGGER_H__
#define __LOGGER_H__
#include <stdio.h>

void put_ts(FILE *__stream);
void logger(FILE *__stream, const char * __format, ...);

#endif
