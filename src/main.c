/* main.c */

#include "main.h"

volatile int interrupted_flag = 0;
int debug_flg = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

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
    { 0, NULL },
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

/* analyze path & fetch handler */
int execution(CTX *ctx, CONFIG *config, HashMap_int fh_map, HashMap_int rh_map){
    if (debug_flg) logger(stdout, "[Debug] execution");

    char *path = ctx->request.path;
    u_int32_t path_len = strlen(path);
    char *def_path;
    def_path = (char *) malloc((size_t) (path_len + strlen(config->server_conf.root) + strlen(config->server_conf.root_file) + 1) * sizeof(char));
    strcpy(def_path, config->server_conf.root);
    strcat(def_path, path);
    if (path[path_len - 1] == '/') {
        strcat(def_path, config->server_conf.root_file);
    }
    ctx->request.path = def_path;
    
    int i;
    for (i = 0; mime[i].ext != NULL; i++) {
        if (strstr(ctx->request.path, mime[i].ext) != NULL) break;
    }

    if (mime[i].ext == NULL) {
        /* fetch handler from route handler list */
        int *handle_idx = get_int(&rh_map, ctx->request.path);
        if (handle_idx == NULL) {
            logger(stderr, "[Error] cannot fetch handler");
            ctx->response.status = 404;
            return -1;
        }
        if (debug_flg) {
            logger(stdout, "[Debug] Handler fetched: %s", config->module_conf[*handle_idx].name);
        }
        module_exec(config->module_conf, *handle_idx, ctx);
    } else {
        /* fetch handler from file handler list */
        int *handle_idx = get_int(&fh_map, mime[i].ext);
        if (handle_idx == NULL) {
            logger(stderr, "[Error] cannot fetch handler.");
            ctx->response.status = 404;
            return -1;
        }
        if (debug_flg) {
            logger(stdout, "[Debug] Handler fetched: %s", config->module_conf[*handle_idx].name);
        }
        module_exec(config->module_conf, *handle_idx, ctx);
    }
    return ctx->error;
}

/* mapping file handler */
void map_file_handler(CONFIG* config, HashMap_int* fh_map) {
    int i, j;
    for (i = 0; i < config->fh_n; i++) {
        for (j = 0; j < config->mod_n; j++) {
            if (!strcmp(config->file_handler[i].module_name, config->module_conf[j].name)) break;
        }
        if (j == config->fh_n) {
            logger(stderr, "[Error] cannot find module");
            exit(1);
        }
        store_int(fh_map, config->file_handler[i].ext, j);
        if (debug_flg) {
            logger(stdout, "[Debug] mapping %s -> %s", config->file_handler[i].ext, config->file_handler[i].module_name);
        }
    }
}

/* mapping route handler */
void map_route_handler(CONFIG* config, HashMap_int* rh_map) {
    int i, j;
    for (i = 0; i < config->rh_n; i++) {
        for (j = 0; j < config->mod_n; j++) {
            if (!strcmp(config->route_handler[i].module_name, config->module_conf[j].name)) break;
        }
        if (j == config->rh_n) {
            logger(stderr, "[Error] cannot find module");
            exit(1);
        }
        store_int(rh_map, config->route_handler[i].route, j);
        if (debug_flg) {
            logger(stdout, "[Debug] mapping %s -> %s", config->route_handler[i].route, config->route_handler[i].module_name);
        }
    }
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
        strcat(header, tmp);
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
    request->body = (char *) malloc((size_t) 4 * BUFFER_SIZE * sizeof(char));
    if (body != NULL) strcpy(request->body, body);
    free(header);
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

    if (debug_flg)
        logger(stdout, "[Debug] request parsed");
    return request;
}

/* processing request */
void *process_request(void* args) {
    struct sockaddr_in cli_sock_addr;
    u_int16_t cli_addr_len = sizeof cli_sock_addr;

    int cli_sock;
    int sv_sock = ((THREAD_ARGS *) args)->sv_sock;
    HashMap_int fh_map = ((THREAD_ARGS *) args)->fh_map;
    HashMap_int rh_map = ((THREAD_ARGS *) args)->rh_map;
    CONFIG config = ((THREAD_ARGS *) args)->config;

    pthread_detach(pthread_self());
    
    while(1) {
        pthread_mutex_lock(&mutex);
        cli_sock = accept(sv_sock, (struct sockaddr *) &cli_sock_addr, (socklen_t*) &cli_addr_len);
        if (cli_sock < 0) {
            logger(stderr, "[Error] connot accept connection");
            pthread_mutex_unlock(&mutex);
            continue;
        }
        pthread_mutex_unlock(&mutex);

        if (debug_flg) logger(stdout, "[Info] connection accepted.");
        char* recieved_buf = (char *) malloc((size_t) 8 * BUFFER_SIZE * sizeof(char));
        char* response_buf = (char *) malloc((size_t) 8 * BUFFER_SIZE * sizeof(char));
        HTTP_REQUEST request;
        request = recieve_data(cli_sock, recieved_buf, 8 * BUFFER_SIZE);

        CTX ctx;
        ctx.request = request;
        ctx.init_map = 0;
        ctx.map_size = 0;
        ctx.error = 0;
        ctx.debug = debug_flg;

        execution(&ctx, &config, fh_map, rh_map);
        /* TODO: output information */

        /* format and send response */
        if (debug_flg)
            logger(stdout, "[Debug] formatting response");
        strncat(response_buf, "HTTP/1.1 ", 9);
        int i;
        for(i = 0; status_map[i].str != NULL; i++) {
            if (status_map[i].code == ctx.response.status) {
                break;
            }
        }

        if (status_map[i].str == NULL) {
            logger(stderr, "[Error] invalid status");
            logger(stderr, "exitting...");
            exit(1);
        }

        if (debug_flg)
            logger(stdout, "[Debug] Status: %d", status_map[i].code);
        char status_code[10];
        sprintf(status_code, "%d ", status_map[i].code);
        strcat(response_buf, status_code);
        strcat(response_buf, status_map[i].str);
        strcat(response_buf, CRLF);
        
        Data *node;
        for (i = 0; i < ctx.response.header.size; i++) {
            node = ctx.response.header.hash_table[i];
            while (node->key[0] != '\0') {
                strcat(response_buf, node->key);
                strcat(response_buf, ": ");
                strcat(response_buf, node->val);
                strcat(response_buf, CRLF);
                node = node->next;
            }
        }

        strcat(response_buf, CRLF);
        if (ctx.response.body != NULL) strcat(response_buf, ctx.response.body);
        send(cli_sock, response_buf, strlen(response_buf), 0);

        /* close connection */
        close(cli_sock);

        /* free memory */
        if (debug_flg)
            logger(stdout, "[Debug] clearing memory");
        _FREE(recieved_buf);
        _FREE(response_buf);
        _FREE(ctx.request.path);
        _FREE(ctx.request.body);
        free_hashmap(&ctx.request.header);
        _FREE(ctx.response.body);
        free_hashmap(&ctx.response.header);
        if (ctx.init_map) free_hashmap(&ctx.additional);
    }
    return (void *)0;
}

/* main function */
int main(int argc, char** argv) {
    char sv_addr[16];
    int sv_port = DEFAULT_PORT;

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

    /* load modules */
    load_modules(config.module_conf, config.mod_n);

    /* mapping handler */
    HashMap_int fh_map;
    HashMap_int rh_map;
    init_map_int(&fh_map, MODULE_N);
    init_map_int(&rh_map, MODULE_N);
    map_file_handler(&config, &fh_map);
    map_route_handler(&config, &rh_map);

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

    pthread_t *threads = (pthread_t *) malloc((size_t) proc_num * sizeof(pthread_t));

    /* setup thread arguments */
    THREAD_ARGS args;
    args.sv_sock = sv_sock;
    args.fh_map = fh_map;
    args.rh_map = rh_map;
    args.config = config;

    /* thread creation */
    int i;
    for (i = 0; i < proc_num; i++) {
        if (pthread_create(&threads[i], NULL, process_request, (void *) &args) < 0) {
            logger(stderr, "[Error] cannot create thread");
            exit(1);
        }
        if (debug_flg) {
            logger(stdout, "[Debug] created thread-%d id:%lu", i, threads[i]);
        }
    }
    logger(stdout, "[Info] running in %d threads", proc_num);

    while (!interrupted_flag);

    /* exiting process */
    fprintf(stderr, "keyboard interruption!\n");
    for (i = 0; i < proc_num; i++) {
        pthread_kill(threads[i], SIGINT);
    }
    close(sv_sock);
}
