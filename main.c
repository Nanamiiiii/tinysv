/* main.c */

#include "logger.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define VERSION "0.0.1"

#define DEFAULT_ADDR "0.0.0.0"
#define DEFAULT_PORT 5000

#define QUEUE_SIZE 1024

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
    fprintf(fp, "\t-h, --help\n");
    fprintf(fp, "\t\tPrint help message.\n\n");
    fprintf(fp, "\t-v, --version\n");
    fprintf(fp, "\t\tPrint version info.\n\n");
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

int main(int argc, char** argv) {
    char sv_addr[16];
    int sv_port = DEFAULT_PORT;
    strcpy(sv_addr, DEFAULT_ADDR);

    /* Analyzing option arguments */
    int opt;
    int debug_flg = 0;
    const char* optstr = "s:p:hdv";
    const struct option longopts[] = {
        {"server", required_argument, 0, 's'},
        {"port", required_argument, 0, 'p'},
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
        logger(stdout, "[Info] Runnning in debug mode.");

    /* Create socket */
    int sv_sock;
    if((sv_sock = open_svsock(sv_addr, sv_port)) < 0)
        logger(stderr, "[Error] cannot open socket.");

    logger(stdout, "[Info] listening %s:%d\n", sv_addr, sv_port);

    signal(SIGPIPE, SIG_IGN);
    


    close(sv_sock);
}
