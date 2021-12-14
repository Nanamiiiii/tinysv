/* main.c */

#include "logger.h"
#include "module.h"
#include "config.h"
#include "main.h"

volatile int interrupted_flag = 0;
int debug_flg = 0;

static _Atomic unsigned int client_cnt = 0;
pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;

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

METHOD_MAP method_map[] = {
    { "GET", GET },
    { "POST", POST },
    { "PUT", PUT },
    { "DELETE", DELETE },
    { "PATCH", PATCH },
    { "CONNECT", CONNECT },
    { "HEAD", HEAD },
    { "OPTIONS", OPTIONS },
    { "TRACE", TRACE },
    { NULL, UNDEFINED },
};

HTTP_STATUS status_map[] = {
    { 100, "Continue" },
    { 101, "Switching Protocols" },
    { 103, "Early Hints" },
    { 200, "OK" },
    { 201, "Created" },
    { 202, "Accepted" },
    { 203, "Non-Authoritative Information" },
    { 204, "No Content" },
    { 205, "Reset Content" },
    { 206, "Partial Content" },
    { 300, "Multiple Choices" },
    { 301, "Moved Permanently" },
    { 302, "Found" },
    { 303, "See Other" },
    { 304, "Not Modified" },
    { 307, "Temporary Redirect" },
    { 308, "Permanent Redirect" },
    { 400, "Bad Request" },
    { 401, "Unauthorized" },
    { 402, "Payment Required" },
    { 403, "Forbidden" },
    { 404, "Not Found" },
    { 405, "Method Not Allowed" },
    { 406, "Not Acceptable" },
    { 407, "Proxy Authentication Required" },
    { 408, "Request Timeout" },
    { 409, "Conflict" },
    { 410, "Gone" },
    { 411, "Length Required" },
    { 412, "Precondition Failed" },
    { 413, "Payload Too Large" },
    { 414, "URI Too Long" },
    { 415, "Unsupported Media Type" },
    { 416, "Range Not Satisfiable" },
    { 417, "Expectation Failed" },
    { 418, "I'm a teapot" },
    { 422, "Unprocessable Entity" },
    { 425, "Too Early" },
    { 426, "Upgrade Required" },
    { 428, "Precondition Required" },
    { 429, "Too Many Request" },
    { 431, "Request Header Fields Too Large" },
    { 451, "Unavailable For Legal Reasons" },
    { 500, "Internal Server Error" },
    { 501, "Not Implemented" },
    { 502, "Bad Gateway" },
    { 503, "Service Unavailable" },
    { 504, "Gateway Timeout" },
    { 505, "HTTP Version Not Supported" },
    { 506, "Variant Also Negotiates" },
    { 507, "Insufficient Storage" },
    { 508, "Loop Detected" },
    { 510, "Not Extended" },
    { 511, "Network Authentication Required" },
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
    char* buf = (char*) malloc((size_t) 16 * sizeof(char));
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

/* fetch method */
HTTP_METHOD fetch_method(const char* str) {
    int i;
    for (i = 0; method_map[i].str != NULL; i++) {
        if (!strcmp(str, method_map[i].str)) {
            return method_map[i].method;
        }
    }
    return method_map[i].method;
}

/* parse request */
void parse_http_request(HTTP_REQUEST* request, char* buf) {
    char *req_line, *header, *body, *tmp;

    req_line = strtok(buf, LF); // Read firstline
    tmp = strtok(NULL, LF);
    header = (char *) malloc((size_t) 4 * BUFFER_SIZE * sizeof(char));
    while (tmp[0] != '\r') { // roop for request header
        sprintf(header, "%s%s", header, tmp);
        tmp = strtok(NULL, LF);
    }
    body = strtok(NULL, "\0");

    request->request_method = fetch_method(strtok(req_line, " "));
    request->path = strtok(NULL, " ");

    init_map(&request->header, 32);
    char *key, *val;
    key = strtok(header, ":");
    do {
        val = strtok(NULL, CR);
        store(&request->header, key, val);
    } while ((key = strtok(NULL, ":")) != NULL);

    strcpy(request->body, body);
}

/* recieving data */
HTTP_REQUEST recieve_data (int cli_sock, char* buf, u_int32_t buf_size) {
    HTTP_REQUEST request;
    int recieved = recv(cli_sock, buf, buf_size, 0);
    if (recieved < 0) {
        logger(stderr, "[Error] failed data recieving.");
        return request;
    }
    if (debug_flg)
        logger(stdout, "[Debug] recieved request\n%s", buf);
    
    parse_http_request(&request, buf);
    return request;
}

/* processing request */
void* process_request(void* args) {
    if (debug_flg) logger(stdout, "[Info] connection accepted.");

    char* recieved_buf = (char*) malloc((size_t) 8 * BUFFER_SIZE * sizeof(char));
    char* response_buf = (char*) malloc((size_t) 8 * BUFFER_SIZE * sizeof(char));
    HTTP_REQUEST request;
    request = recieve_data(((THREAD_ARGS *) args)->cli_sock, recieved_buf, 8 * BUFFER_SIZE);

    
}

/* main function */
int main(int argc, char** argv) {
    char sv_addr[16];
    int sv_port = DEFAULT_PORT;

    struct sockaddr_in cli_sock_addr;
    u_int16_t cli_addr_len = sizeof cli_sock_addr;
    int cli_sock;
    pthread_t thread_id;

    print_AA();

    /* load configuration */
    CONFIG config;
    config.module_conf = (MODULE_C *) malloc((size_t) MODULE_N * sizeof(MODULE_C));
    config.file_handler = (FILE_HANDLER *) malloc((size_t) MODULE_N * sizeof(FILE_HANDLER));
    config.route_handler = (ROUTE_HANDLER *) malloc((size_t) MODULE_N * sizeof(ROUTE_HANDLER));
    config.mod_n = 0;
    config.fh_n = 0;
    config.rh_n = 0;
    
    parse_config(&config);

    strcpy(sv_addr, config.server_conf.addr);
    sv_port = config.server_conf.port;

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

    if(debug_flg) {
        logger(stdout, "[Debug] Runnning in debug mode.");
        logger(stdout, "[Debug] show configs");
        logger(stdout, "Server Addr: %s", sv_addr);
        logger(stdout, "Server Port: %d", sv_port);
        logger(stdout, "Module: %d", config.mod_n);
        logger(stdout, "File Handler: %d", config.fh_n);
        logger(stdout, "Route Handler: %d", config.rh_n);
    }

    /* TODO: load modules */

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
        cli_sock = accept(sv_sock, (struct sockaddr *) &cli_sock_addr, (socklen_t*) &cli_addr_len);
        if (client_cnt == proc_num) { // Threads limitation
            logger(stdout, "Client limit. Access rejected. [from] %s:%d", fmt_client_addr(cli_sock_addr), cli_sock_addr.sin_port);
            close(cli_sock);
            continue;
        }
    }

    // TODO: exiting process

    close(sv_sock);
}
