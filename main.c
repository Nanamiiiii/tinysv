/* main.c */

#include "logger.h"
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

#define QUEUE_SIZE 1024
#define BUFFER_SIZE 1024
#define MAX_THREAD 10

volatile int interrupted_flag = 0;
int debug_flg = 0;

static _Atomic unsigned int client_cnt = 0;
pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct _cli_thread {

} CLI_THREAD;

typedef struct _thread_args {
    int cli_sock;
} THREAD_ARGS;

typedef struct _mime_type {
    const char* ext;
    const char* mime;
} MIME_TYPE;

MIME_TYPE mime[] = {
    { ".html", "text/html" },
    { ".htm", "text/htm" },
    { ".js", "application/javascript" },
    { ".css", "text/css" },
    { ".jpg", "image/jpeg" },
    { ".jpeg", "image/jpeg" },
    { ".png", "image/png" },
    { ".ico", "image/x-icon" },
    { NULL, NULL },
};

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
} HTTP_METHOD;

typedef struct _http_request {
    HTTP_METHOD request_method;
    char* path;
    HashMap header;
    char* body;
};

/* output functions */
void print_AA() {
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "   __  _\n");
    fprintf(stdout, "  / /_(_)__  __ ______  __\n");
    fprintf(stdout, " / __/ / _ \\/ // (_-< |/ /\n");
    fprintf(stdout, " \\__/_/_//_/\\_, /___/___/\n");
    fprintf(stdout, "           /___/\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "Welcome to tiny webserver created by Nanamiiiii!\n\n");
}

void print_version(FILE *fp) {
    fprintf(fp, "tinysv %s\n", VERSION);
    fprintf(fp, "\tA tiny server application created by Nanamiiiii\n\n");
}

void print_usage(FILE *fp) {
    fprintf(fp, "USAGE:\n");
    fprintf(fp, "\ttinysv\n");
    fprintf(fp, "\ttinysv [OPTIONS]\n\n");
    fprintf(fp, "OPTIONS:\n");
    fprintf(fp, "\t-s, --server <addr>\n");
    fprintf(fp, "\t\tOverride default server address (0.0.0.0).\n\n");
    fprintf(fp, "\t-p, --port <PORT>\n");
    fprintf(fp, "\t\tOverride default PORT number (5000).\n\n");
    fprintf(fp, "\t-t, --thread <number of process>\n");
    fprintf(fp, "\t\tOverride the number of thread runs. (5)\n\n");
    fprintf(fp, "\t-h, --help\n");
    fprintf(fp, "\t\tPrint help message.\n\n");
    fprintf(fp, "\t-v, --version\n");
    fprintf(fp, "\t\tPrint version info.\n\n");
}

char* fmt_client_addr(struct sockaddr_in sock_addr){
    char buf[16];
    sprintf(buf, "%d.%d.%d.%d", 
        sock_addr.sin_addr.s_addr & 0xff,
        (sock_addr.sin_addr.s_addr & 0xff00) >> 8,
        (sock_addr.sin_addr.s_addr & 0xff0000) >> 16,
        (sock_addr.sin_addr.s_addr & 0xff000000) >> 24);
    return buf;
}

/* keyboard interruption */
void interruption_handler(int sig, siginfo_t *info, void *ctx) {
    interrupted_flag = 1;
}

/* Create and Listen socket */
int open_svsock(char* sv_addr, int sv_port) {
    print_AA();
    int sv_sock;
    int optval = 1;
    struct sockaddr_in sv_sock_addr;
    if ((sv_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        
        return -1;
    }

    if (setsockopt(sv_sock, SOL_SOCKET, SO_REUSEADDR, (const void*) &optval, sizeof(int)) < 0) {
        close(sv_sock);
        return -1;
    }

    if (setsockopt(sv_sock, 6, TCP_CORK, (const void*) &optval, sizeof(int)) < 0) {
        close(sv_sock);
        return -1;
    }

    memset(&sv_sock_addr, 0, sizeof(struct sockaddr_in));
    sv_sock_addr.sin_family = AF_INET;
    sv_sock_addr.sin_port = htons((unsigned short) sv_port);
    sv_sock_addr.sin_addr.s_addr = inet_addr(sv_addr);

    if (bind(sv_sock, (const struct sockaddr*) &sv_sock_addr, sizeof(sv_sock_addr)) < 0) {
        close(sv_sock);
        return -1;
    }

    if (listen(sv_sock, QUEUE_SIZE) < 0) {
        close(sv_sock);
        return -1;
    }

    return sv_sock;
}

/* recieving data */
int recieve_data (int cli_sock, char* buf, uint32_t buf_size) {
    return recv(cli_sock, buf, buf_size, 0);
}

/* processing request */
void* process_request(void* args) {
    if (debug_flg) logger(stdout, "connection accepted.");

    int recieved;
    char* recieved_buf = (char*) malloc((size_t) 8 * BUFFER_SIZE * sizeof(char));
    char* response_buf = (char*) malloc((size_t) 8 * BUFFER_SIZE * sizeof(char));
    char request_method[BUFFER_SIZE];
    char path[BUFFER_SIZE];
    char header[BUFFER_SIZE];
    char body[BUFFER_SIZE];
    uint16_t status;
    uint32_t filesize;


}

int main(int argc, char** argv) {
    char sv_addr[16];
    int sv_port = DEFAULT_PORT;
    strcpy(sv_addr, DEFAULT_ADDR);

    struct sockaddr_in cli_sock_addr;
    uint16_t cli_addr_len = sizeof cli_sock_addr;
    int cli_sock;
    pthread_t thread_id;

    /* Analyzing option arguments */
    int opt;
    int proc_num = MAX_THREAD;
    const char* optstr = "s:p:t:hdv";
    const struct option longopts[] = {
        {"server", required_argument, 0, 's'},
        {"port", required_argument, 0, 'p'},
        {"thread", required_argument, 0, 't'},
        {"help", no_argument, 0, 'h'},
        {"debug", no_argument, &debug_flg, 1},
        {"version", no_argument, 0, 'v'},
        {0, 0, 0, 0},
    };
    int longindex = 0;
    opterr = 0;
    while ((opt = getopt_long(argc, argv, optstr, longopts, &longindex)) != -1) {
        switch(opt) {
            case 0:
                break;
            case 's':
                strcpy(sv_addr, optarg);
                break;
            case 'p':
                sv_port = atoi(optarg);
                break;
            case 't':
                proc_num = atoi(optarg);
                break;
            case 'h':
                print_version(stdout);
                print_usage(stdout);
                return 0;
            case 'd':
                debug_flg = 1;
                break;
            case 'v':
                print_version(stdout);
                return 0;
            default:
                logger(stderr, "[Error] Invalid Argument.");
                print_usage(stderr);
                return -1;
        }
    }

    if(debug_flg)
        logger(stdout, "[Debug] Runnning in debug mode.");

    /* Create socket */
    int sv_sock;
    if((sv_sock = open_svsock(sv_addr, sv_port)) < 0) {
        logger(stderr, "[Error] cannot open socket.");
        return -1;
    }

    logger(stdout, "[Info] listening %s:%d", sv_addr, sv_port);

    /* Handle signals */
    signal(SIGPIPE, SIG_IGN);
    struct sigaction sigact_int;
    memset(&sigact_int, 0, sizeof(sigact_int));
    sigact_int.sa_sigaction = interruption_handler;
    sigact_int.sa_flags = SA_SIGINFO;
    if(sigaction(SIGINT, &sigact_int, NULL) < 0) {
        logger(stderr, "[Error] something went wrong.");
        exit(-1);
    }

    logger(stdout, "[Info] max thread: %d", proc_num);

    // TODO: accepting and thread creation
    while (!interrupted_flag) {
        cli_sock = accept(sv_sock, (struct sockaddr *) &cli_sock_addr, &cli_addr_len);
        if (client_cnt == proc_num) { // Threads limitation
            logger(stdout, "Client limit. Access rejected. [from] %s:%d", fmt_client_addr(cli_sock_addr), cli_sock_addr.sin_port);
            close(cli_sock);
            continue;
        }
    }

    // TODO: exiting process

    close(sv_sock);
}
