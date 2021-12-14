/* config.h */

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdio.h>
#include <stdlib.h>

#define CONF_FILE "config.xml"
#define CONF_BUF 1024

#define END '\0'

typedef struct _server_conf {
    char *addr;
    int port;
    char *root;
    char *root_file;
} SERVER_CONF;

typedef struct _module_conf {
    char *name;
    char *filename;
    void *handle;
    void *entry_handle;
} MODULE_C;

typedef struct _file_handler {
    char *ext;
    char *module_name;
} FILE_HANDLER;

typedef struct _route_handler {
    char *route;
    char *module_name;
} ROUTE_HANDLER;

typedef struct _config {
    SERVER_CONF server_conf;
    MODULE_C *module_conf;
    u_int16_t mod_n;
    FILE_HANDLER *file_handler;
    u_int16_t fh_n;
    ROUTE_HANDLER *route_handler;
    u_int16_t rh_n;
} CONFIG;

typedef enum {
    INIT,
    SERVER,
    ADDRESS,
    PORT,
    ROOT,
    ROOTFILE,
    MODULES,
    MODULE,
    MOD_NAME,
    MOD_OBJECT,
    FILEHANDLE,
    FILE_EXT,
    MOD_NAME_FH,
    F_HANDLER,
    ROUTEHANDLE,
    MOD_NAME_RH,
    ROUTE,
    R_HANDLER
} TAGS;

void parse_config(CONFIG *config);

#endif