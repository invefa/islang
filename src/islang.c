/*
    This c source file is the main source file for islang and it is the entrance of islang.
    This project is compiled with -std=gnu11, because it will use some extension functions
    which are suplied by GNU standard extensions.

    There are a list of extensions are used:
        1. typeof keyword.
    And some features are used:
        1. special macro expansion.

    Anyway, you had better compile this project with gcc -std=gnu11.
    Written by invefa.
*/

#include <stdio.h>

#include "isl_xssert.h"
#include "isl_overload.h"


void test_overload_0(void) {
    printf("test_overload_0\n");
}

void test_overload_1(int _a) {
    printf("test_overload_1\narg_a = %d\n", _a);
}

void test_overload_2(int _a, int _b) {
    printf("test_overload_2\narg_a = %d\narg_b = %d\n", _a, _b);
}

int main(void) {

    typeof(0) a = __ISL_MACRO_GET_ARGS_COUNT(1, 2, 3, 4, 5);
    printf("Hello, World!\na = %d\n", a);

    __ISL_OVERLOAD_FORWARD_CALL(test_overload, a);

#   define test_overload(_vargs...) _isl_overload(test_overload,##_vargs)


    test_overload();
    test_overload(123456);
    test_overload(123, 456);

    return 0;

}

