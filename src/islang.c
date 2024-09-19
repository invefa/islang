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

#   define ISL_ASSERT_FAILURE_CODE 24605
#   define __ISL_XSSERT_EXIT_a() exit(ISL_ASSERT_FAILURE_CODE);
#   define __ISL_XSSERT_EXIT_w()

#   define __ISL_XSSERT_SELECTOR_0()
#   define __ISL_XSSERT_EXT_0()


#   define __ISL_XSSERT_SELECTOR_1(_expl) \
        "\tit can be explained as: " ANSI_HIL _expl ANSI_RST "\n"

#   define __ISL_XSSERT_EXT_1(_expl)


#   define __ISL_XSSERT_SELECTOR_2(_expl, _details) \
        __ISL_XSSERT_SELECTOR_1(_expl)\
        "\tthere are some details: " ANSI_HIL _details ANSI_RST "\n"

#   define __ISL_XSSERT_EXT_2(_expl, _details)


#   define __ISL_MACRO_UNPACKAGER(_package...) _package

#   define __ISL_XSSERT_SELECTOR_3(_expl, _details_fmt, _fmt_args) \
        __ISL_XSSERT_SELECTOR_2(_expl, _details_fmt)

#   define __ISL_XSSERT_EXT_3(_expl, _details_fmt, _fmt_args) \
        ,__ISL_MACRO_UNPACKAGER _fmt_args


#   define __ISL_XSSERT(_x, _expr, _vargs...)                                       \
        do{                                                                         \
            if(!(_expr)) {                                                          \
                fprintf(stderr, ANSI_HIR #_x "ssert failure:" ANSI_RST              \
                "\n\tin file '"__FILE__"':\n\tat fn %s(...): expression: "          \
                ANSI_HIL #_expr ANSI_RST " <line:%d>.\n"                            \
                _isl_overload(__ISL_XSSERT_SELECTOR,##_vargs)                       \
                , __func__, __LINE__ _isl_overload(__ISL_XSSERT_EXT,##_vargs));     \
                __ISL_XSSERT_EXIT_##_x()                                            \
            }\
        } while(0)

#   define isl_assert(_expr, _vargs...) __ISL_XSSERT(a, _expr, ##_vargs)
#   define isl_wssert(_expr, _vargs...) __ISL_XSSERT(w, _expr, ##_vargs)

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

