/* module.c */

#include "module.h"
#include "logger.h"
#include <dlfcn.h>

extern int debug_flg;

void load_modules(MODULE_C *module_conf, int mod_n) {
    int i;
    for (i = 0; i < mod_n; i++) {
        if (debug_flg) {
            logger(stdout, "[Debug] Loading module: %s", module_conf[i].name);
        }

        size_t buf_size = strlen(MOD_PATH_PREFIX) + strlen(module_conf[i].filename) + 1;
        char *filepath = (char *) malloc((size_t) buf_size * sizeof(char));
        strcpy(filepath, MOD_PATH_PREFIX);
        strcat(filepath, module_conf[i].filename);
        if (debug_flg) {
            logger(stdout, "loading from: %s", filepath);
        }

        char *errmsg;

        if ((module_conf[i].handle = dlopen(filepath, RTLD_LAZY)) == NULL) {
            logger(stderr, "[Error] cannot load module: %s", module_conf[i].name);
            if (debug_flg) {
                logger(stderr, "Errmsg: %s", errmsg);
            }
            exit(1);
        }

        dlerror();

        if ((module_conf[i].entry_handle = dlsym(module_conf[i].handle, MODULE_ENTRY)) == NULL && (errmsg = dlerror()) != NULL) {
            logger(stderr, "[Error] cannot find module entrypoint: %s", module_conf[i].name);
            if (debug_flg) {
                logger(stderr, "Errmsg: %s", errmsg);
            }
            exit(1);
        }
    }
    logger(stdout, "[Info] %d modules loaded", mod_n);
}

void module_exec(MODULE_C *module_conf, int mod_idx, CTX *ctx) {
    void (*func)(CTX *);
    func = (void (*)(CTX *)) module_conf->entry_handle;
    (*func)(ctx);
}