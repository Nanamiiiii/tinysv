/* default_module.c */

/* This is the sample module of tinysv */
/* This module handles GET Request.    */

#include "module.h"

/* Entry point: tiny_main */
/* @ctx: request context  */
void tiny_main(CTX* ctx) {
    printf("Test Mod.\n");
}