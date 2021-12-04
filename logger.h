/* logger.h */

#include <stdio.h>

void put_ts(FILE *__stream);
void logger(FILE *__stream, const char * __format, ...);
