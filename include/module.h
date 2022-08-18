/* module.h 
 */

#ifndef __MODULE_H__
#define __MODULE_H__

#include "hashmap.h"
#include "config.h"
#include "types.h"

#define MODULE_ENTRY "__handler"
#define MOD_PATH_PREFIX "./modules/" 
#define MODULE_N 32
#define FILE_BUF 1024

void load_modules(MODULE_C *module_conf, int mod_n);
void module_exec(MODULE_C *module_conf, int mod_idx, CTX *ctx);

#endif
