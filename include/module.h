/* module.h */

#ifndef __MODULE_H__
#define __MODULE_H__

#include "hashmap.h"
#include "config.h"

#define MODULE_ENTRY "__handler"
#define MOD_PATH_PREFIX "./modules/" 
#define MODULE_N 32
#define FILE_BUF 1024

typedef struct _mime_type {
    const char* ext;
    const char* mime;
} MIME_TYPE;

typedef enum {
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
    u_int16_t status;
    HashMap header;
    char* body;
} HTTP_RESPONSE;

typedef struct _context {
    HTTP_REQUEST request;
    HTTP_RESPONSE response;
    HashMap additional;
    int init_map;
    int map_size;
    int error;
    int debug;
} CTX;

void load_modules(MODULE_C *module_conf, int mod_n);
void module_exec(MODULE_C *module_conf, int mod_idx, CTX *ctx);

#endif
