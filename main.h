/* main.h */

#ifndef __MAIN_H__
#define __MAIN_H__

#include "hashmap.h"
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
#define MAX_THREAD 10

typedef struct _cli_thread {

} CLI_THREAD;

typedef struct _thread_args {
    int cli_sock;
} THREAD_ARGS;

#endif
