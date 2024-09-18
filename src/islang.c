/*
    This is the main source file for islang and it is the entrance of islang.
    code written by invefa.

    This project is compiled with -std=gnu11, because it will use some extension functions
    which are suplied by GNU standard extensions.

    There are a list of extensions are used:
        1. typeof keyword
*/

#include <stdio.h>

#include "isl_xssert.h"

int main(void) {

    typeof(0) a = 0;
    printf("Hello, World!\na = %d\n", a);
    return 0;

}

