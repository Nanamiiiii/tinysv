/* logger.c */

#include "logger.h"
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

const char* wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

void put_ts(FILE *__stream) {
    time_t t = time(NULL);
    struct tm _tm;
    localtime_r(&t, &_tm);
    fprintf(__stream, "%04d-%02d-%02d %s %02d:%02d:%02d ", _tm.tm_year + 1900, _tm.tm_mon + 1, _tm.tm_mday, wday[_tm.tm_wday], _tm.tm_hour, _tm.tm_min, _tm.tm_sec);
}

void logger(FILE *__stream, const char *__format, ...) {
    put_ts(__stream);
    va_list args;
    va_start(args, __format);
    vfprintf(__stream, __format, args);
    va_end(args);
    fprintf(__stream, "\n");
}
