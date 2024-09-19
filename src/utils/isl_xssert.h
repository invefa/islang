#ifndef ISL_XSSERT_H
#define ISL_XSSERT_H
/*
    This header suplies macro isl_assert(...) and isl_wssert(...),
    which are used in debug mode to detect the error of the core code of islang.
*/

#include "isl_options.h"
#include "isl_ansictrl.h"

#ifdef ISL_ENABLE_XSSERT

/*
    There are two macros that indicate the difference between isl_assert and isl_wssert:
    __ISL_XSSERT_EXIT_a: when isl_assert failed, exit with code ISL_ASSERT_FAILURE_CODE.
    __ISL_XSSERT_EXIT_w: when isl_wssert failed, do nothing.
*/

#define __ISL_XSSERT_EXIT_a() exit(ISL_ASSERT_FAILURE_CODE);
#define __ISL_XSSERT_EXIT_w()

/*
    ISL_XSSERT_EXT was the extension part of isl_assert and isl_wssert.
    It is optional, can make the output more readable.
*/

#define __ISL_XSSERT_EXT_TEXT_0()
#define __ISL_XSSERT_EXT_ARGS_0()


#define __ISL_XSSERT_EXT_TEXT_1(_expl) \
        "\tit can be explained as: " ANSI_HIL _expl ANSI_RST "\n"
#define __ISL_XSSERT_EXT_ARGS_1(_expl)


#define __ISL_XSSERT_EXT_TEXT_2(_expl, _details) \
    __ISL_XSSERT_EXT_TEXT_1(_expl) \
    "\tthere are some details: " _details "\n"
#define __ISL_XSSERT_EXT_ARGS_2(_expl, _details)



#define __ISL_XSSERT_EXT_TEXT_3(_expl, _details_fmt, _fmt_args_package) \
    __ISL_XSSERT_EXT_TEXT_2(_expl, _details_fmt)
#define __ISL_XSSERT_EXT_ARGS_3(_expl, _details_fmt, _fmt_args_package) \
    ,__ISL_MACRO_UNPACKAGER _fmt_args_package


#define __ISL_XSSERT(_x, _expr, _exts...)                                   \
do{                                                                         \
    if(!(_expr)) {                                                          \
        fprintf(stderr, ANSI_HIR #_x "ssert failure:" ANSI_RST              \
        "\n\tin file '"__FILE__"':\n\tat fn %s(...): expression: "          \
        ANSI_HIL #_expr ANSI_RST " <line:%d>.\n"                            \
        _isl_overload(__ISL_XSSERT_EXT_TEXT,##_exts)                        \
        ,__func__,__LINE__ _isl_overload(__ISL_XSSERT_EXT_ARGS,##_exts));   \
        __ISL_XSSERT_EXIT_##_x()                                            \
    }                                                                       \
} while(0)

/*
    This macro was used in debug mode to detect the error of the core code of islang.
    And it has some extensions that can make the output more readable.
    expr: what we asserting, when it go false, will output the error to stderr and exit with code ISL_ASSERT_FAILURE_CODE.
    exts: it can receive 3 arguments maximum, and they are optional, can make the output more readable.
*/
#define isl_assert(_expr, _exts...) __ISL_XSSERT(a, _expr, ##_exts)

/*
    This macro was used in debug mode to detect the error of the core code of islang.
    And it has some extensions that can make the output more readable.
    expr: what we asserting, when it go false, will output the error to stderr.
    exts: it can receive 3 arguments maximum, and they are optional, can make the output more readable.
*/
#define isl_wssert(_expr, _exts...) __ISL_XSSERT(w, _expr, ##_exts)

#else

#define isl_assert(_expr, _vargs...)
#define isl_wssert(_expr, _vargs...)

#endif
#endif