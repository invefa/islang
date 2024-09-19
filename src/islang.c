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
#include <stdlib.h>

#include "isl_xssert.h"
#include "isl_overload.h"
#include "isl_ansictrl.h"

void isl_test_overload(void);
void isl_test_xssert(void);


int main(void) {

    // typeof(0) num = 0;
    // printf("Hello, World!\nnum = %d\n", num);

    isl_test_overload();
    isl_test_xssert();

    return 0;
}


void isl_test_xssert(void) {



    /*
        __ISL_XSSERT(w, num != 1);
        __ISL_XSSERT(w, num != 1, "num should not be 1.");
        __ISL_XSSERT(w, num != 1, "num should not be 1.", "this is just a test.");
        __ISL_XSSERT(a, num != 1, "num should not be 1.", "num = %d, num + 1 = %d.", (num, num + 1));
    */

    int num = 1;
    isl_wssert(num != 1);
    isl_wssert(num != 1, "num should not be 1.");
    isl_wssert(num != 1, "num should not be 1.", "this is just a test.");
    isl_assert(num != 1, "num should not be 1.", "num = %d, num + 1 = %d.", (num, num + 1));


}

void test_overload_0(void) {
    printf("test_overload_0\n");
}

void test_overload_1(int _a) {
    printf("test_overload_1\narg_a = %d\n", _a);
}

void test_overload_2(int _a, int _b) {
    printf("test_overload_2\narg_a = %d\narg_b = %d\n", _a, _b);
}

void isl_test_overload(void) {

    __ISL_OVERLOAD_FORWARD_CALL(test_overload, 1);

#   define test_overload(_vargs...) _isl_overload(test_overload,##_vargs)

    test_overload();
    test_overload(123456);
    test_overload(123, 456);

}

