/* main.h */

#ifndef __MAIN_H__
#define __MAIN_H__

#include "hashmap.h"
#include "hashmap_int.h"
#include "logger.h"
#include "module.h"
#include "config.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define VERSION "0.0.1"

#define DEFAULT_ADDR "0.0.0.0"
#define DEFAULT_PORT 5000
#define CRLF "\r\n"
#define CR "\r"
#define LF "\n"

#define QUEUE_SIZE 1024
#define BUFFER_SIZE 1024
#define MAX_THREAD 5

typedef struct _thread_args {
    int sv_sock;
    HashMap_int fh_map;
    HashMap_int rh_map;
    CONFIG config;
} THREAD_ARGS;

#endif
