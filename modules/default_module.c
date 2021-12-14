/* default_module.c */

/* This is the sample module of tinysv */
/* This module handles GET Request.    */

#include "module.h"

/* Entry point: __handler */
/* @ctx: request context  */
void __handler(CTX* ctx) {
    printf("Test Mod.\n");
}