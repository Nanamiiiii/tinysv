/* config.c */

#include "config.h"
#include "module.h"
#include "logger.h"
#include <expat.h>
#include <fcntl.h>
#include <unistd.h>

TAGS current_tag = INIT;

static void XMLCALL startElement(void *user_data, const XML_Char *name, const XML_Char **attrs) {
    CONFIG *config = (CONFIG *) user_data;
    switch (current_tag) {
        case INIT:
            if (!strcmp(name, "Server")) current_tag = SERVER;
            else if (!strcmp(name, "Modules")) current_tag = MODULES;
            else if (!strcmp(name, "FileHandler")) current_tag = FILEHANDLE;
            else if (!strcmp(name, "RouteHandle")) current_tag = ROUTEHANDLE;
            break;
        case SERVER:
            if (!strcmp(name, "Address")) current_tag = ADDRESS;
            else if (!strcmp(name, "Port")) current_tag = PORT;
            else if (!strcmp(name, "Root")) current_tag = ROOT;
            else if (!strcmp(name, "RootFile")) current_tag = ROOTFILE;
            break;
        case MODULES:
            if (!strcmp(name, "Module")) current_tag = MODULE;
            break;
        case FILEHANDLE:
            if (!strcmp(name, "Handler")) current_tag = F_HANDLER;
            break;
        case ROUTEHANDLE:
            if (!strcmp(name, "Handler")) current_tag = R_HANDLER;
            break;
        case MODULE:
            if (!strcmp(name, "Name")) current_tag = MOD_NAME;
            else if (!strcmp(name, "Object")) current_tag = MOD_OBJECT;
            break;
        case F_HANDLER:
            if (!strcmp(name, "Name")) current_tag = MOD_NAME_FH;
            else if (!strcmp(name, "Ext")) current_tag = FILE_EXT;
            break;
        case R_HANDLER:
            if (!strcmp(name, "Name")) current_tag = MOD_NAME_RH;
            else if (!strcmp(name, "Route")) current_tag = ROUTE;
            break;
        default:
            break;
    }
}

static void XMLCALL endElement(void *user_data, const XML_Char *name, const XML_Char **attrs) {
    CONFIG *config = (CONFIG *) user_data;
    switch (current_tag) {
        case SERVER:
            current_tag = INIT;
            break;
        case ADDRESS:
            current_tag = SERVER;
            break;
        case PORT:
            current_tag = SERVER;
            break;
        case ROOT:
            current_tag = SERVER;
            break;
        case ROOTFILE:
            current_tag = SERVER;
            break;
        case MODULES:
            current_tag = INIT;
            break;
        case MODULE:
            current_tag = MODULES;
            config->mod_n++;
            break;
        case MOD_NAME:
            current_tag = MODULE;
            break;
        case MOD_OBJECT:
            current_tag = MODULE;
            break;
        case FILEHANDLE:
            current_tag = INIT;
            break;
        case FILE_EXT:
            current_tag = F_HANDLER;
            break;
        case MOD_NAME_FH:
            current_tag = F_HANDLER;
            break;
        case F_HANDLER:
            current_tag = FILEHANDLE;
            config->fh_n++;
            break;
        case ROUTEHANDLE:
            current_tag = INIT;
            break;
        case MOD_NAME_RH:
            current_tag = R_HANDLER;
            break;
        case ROUTE:
            current_tag = R_HANDLER;
            break;
        case R_HANDLER:
            current_tag = ROUTEHANDLE;
            config->rh_n++;
            break;
        default:
            break;
    }
}

static void XMLCALL elementData(void *user_data, const XML_Char *data, int size) {
    CONFIG *config = (CONFIG *) user_data;
    switch (current_tag) {
        case ADDRESS:
            strcpy(config->server_conf.addr, data);
            break;
        case PORT:
            config->server_conf.port = atoi(data);
            break;
        case ROOT:
            strcpy(config->server_conf.root, data);
            break;
        case ROOTFILE:
            strcpy(config->server_conf.root_file, data);
            break;
        case MOD_NAME:
            strcpy(config->module_conf[config->mod_n].name, data);
            break;
        case MOD_OBJECT:
            strcpy(config->module_conf[config->mod_n].filename, data);
            break;
        case MOD_NAME_FH:
            strcpy(config->file_handler[config->fh_n].module_name, data);
            break;
        case FILE_EXT:
            strcpy(config->file_handler[config->fh_n].ext, data);
            break;
        case MOD_NAME_RH:
            strcpy(config->route_handler[config->rh_n].module_name, data);
            break;
        case ROUTE:
            strcpy(config->route_handler[config->rh_n].route, data);
            break;
        default:
            break;
    }
}

void parse_config (CONFIG *config) {
    XML_Parser parser = XML_ParserCreate(NULL);

    XML_SetUserData(parser, &config);
    XML_SetElementHandler(parser, startElement, endElement);
    XML_SetCharacterDataHandler(parser, elementData);

    int fd = open(CONF_FILE, O_RDONLY);
    if (fd < 0) {
        logger(stderr, "Cannot open config.");
        exit(1);
    }

    char *buf = (char *) malloc((size_t) CONF_BUF * sizeof(char));

    ssize_t len;
    do {
        len = read(fd, buf, CONF_BUF);
        if (len < 0) {
            logger(stderr, "Cannot read config.");
            exit(1);
        }

        if (XML_Parse(parser, buf, (int) len, len < CONF_BUF) == XML_STATUS_ERROR) {
            logger(stderr, "config parse error.");
            exit(1);
        }
    } while (!(len < CONF_BUF));

    XML_ParserFree(parser);
    return;
}