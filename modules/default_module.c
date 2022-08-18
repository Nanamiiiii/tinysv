/* default_module.c 
 */

/* 
 *  This is the sample module of tinysv 
 *  This module handles GET Request.    
 */

#include "module.h"
#include "logger.h"
#include "types.h"
#include <fcntl.h>
#include <unistd.h>

/* Entry point: __handler 
   @ctx: request context  */
void __handler(CTX* ctx) {
    int fd;
    ssize_t len;
    ssize_t total = 0;
    char tmp[10];

    char *buf = (char *) malloc((size_t) FILE_BUF * sizeof(char));

    HTTP_METHOD method = ctx->request.request_method;
    init_map(&ctx->response.header, 32);
    store(&ctx->response.header, "Content-Type", "text/html; charset=utf-8");
    store(&ctx->response.header, "Server", "tinysv");
    ctx->response.body = (char *) malloc((size_t) 8 * FILE_BUF * sizeof(char));
    ctx->response.body[0] = '\0';
    switch (method) {
        case GET:
            if ((fd = open(ctx->request.path, O_RDONLY)) < 0) {
                if (ctx->debug) 
                    logger(stdout, "[Info] file: %s is not found", ctx->request.path);
                ctx->response.status = 404;
                if ((fd = open("./template/404.html", O_RDONLY)) < 0) {
                    logger(stderr, "[Error] cannot retrive template");
                    ctx->error = -1;
                    return;
                }
            } else {
                ctx->response.status = 200;
                if (ctx->debug) 
                    logger(stdout, "[Debug] retrived file: %s", ctx->request.path);
            }
            
            do {
                len = read(fd, buf, FILE_BUF);
                strcat(ctx->response.body, buf);
                total += len;
            } while (len == FILE_BUF);
            close(fd);
            free(buf);

            sprintf(tmp, "%ld", total);
            store(&ctx->response.header, "Content-Length", tmp);
            break;

        default:
            ctx->response.status = 404;
            if ((fd = open("./template/404.html", O_RDONLY)) < 0) {
                logger(stderr, "[Error] cannot retrive template");
                ctx->error = -1;
                return;
            }

            do {
                len = read(fd, buf, FILE_BUF);
                strcat(ctx->response.body, buf);
                total += len;
            } while (len == FILE_BUF);
            close(fd);
            free(buf);

            sprintf(tmp, "%ld", total);
            store(&ctx->response.header, "Content-Length", tmp);
            break;
    }
}