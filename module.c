/* module.c */

#include "module.h"
#include "logger.h"
#include <dlfcn.h>

extern int debug_flg;

void load_modules(MODULE_C *module_conf, int mod_n) {
    int i;
    for (i = 0; i < mod_n; i++) {
        if (debug_flg) {
            logger(stdout, "[Debug] Loading module: %s", module_conf[mod_n].name);
        }

        size_t buf_size = strlen(MOD_PATH_PREFIX) + strlen(module_conf->filename) + 1;
        char *filepath = (char *) malloc((size_t) buf_size * sizeof(char));
        strcpy(filepath, MOD_PATH_PREFIX);
        strcat(filepath, module_conf->filename);

        if ((module_conf->handle = dlopen(filepath, RTLD_LAZY)) == NULL) {
            logger(stderr, "[Error] cannot load module: %s", module_conf[mod_n].name);
            exit(1);
        }

        dlerror();
        char *errmsg;

        if ((module_conf->entry_handle = dlsym(module_conf->handle, MODULE_ENTRY)) == NULL && (errmsg = dlerror()) != NULL) {
            logger(stderr, "[Error] cannot find module entrypoint: %s", module_conf[mod_n].name);
            if (debug_flg) {
                logger(stderr, "Errmsg: %s", errmsg);
            }
            exit(1);
        }
    }
    logger(stdout, "[Info] %d modules loaded", mod_n);
}