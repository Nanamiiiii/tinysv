/* main.h */

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
#define LF "\n"

#define QUEUE_SIZE 1024
#define BUFFER_SIZE 1024
#define MAX_THREAD 10

typedef struct _cli_thread {

} CLI_THREAD;

typedef struct _thread_args {
    int cli_sock;
} THREAD_ARGS;

typedef struct _mime_type {
    const char* ext;
    const char* mime;
} MIME_TYPE;

#define METHOD_N 9

typedef enum _http_method {
    GET,
    POST,
    PUT,
    DELETE,
    PATCH,
    CONNECT,
    HEAD,
    OPTIONS,
    TRACE,
    UNDEFINED,
} HTTP_METHOD;

typedef struct _method_map {
    const char* str;
    HTTP_METHOD method;
} METHOD_MAP;

typedef struct _http_request {
    HTTP_METHOD request_method;
    char* path;
    HashMap header;
    char* body;
} HTTP_REQUEST;

typedef struct _http_status {
    u_int16_t code;
    char* str;
} HTTP_STATUS;

typedef struct _http_response {
    HTTP_STATUS status;
    HashMap header;
    char* body;
} HTTP_RESPONSE;
