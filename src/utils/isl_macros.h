#ifndef ISC_MACROS_H
#define ISC_MACROS_H
/*
    This header suplies some macros that can make the implementation of overload and xssert
    simplify. Written by invefa.
*/

#define isl_minimum(_x, _y) ((_x) < (_y) ? (_x) : (_y))
#define isl_maximum(_x, _y) ((_x) > (_y) ? (_x) : (_y))

// This macro will extpand to the concatenation of _x and _y with underline.
#define __ISL_MACRO_CONCAT_WITH_IMPL(_x, _with, _y) _x##_with##_y

// Why do a wrapper for this macro? Because we need to wait the expanding of
// __ISL_MACRO_GET_ARGS_COUNT(...)
#define __ISL_MACRO_CONCAT_WITH(_x, _with, _y) __ISL_MACRO_CONCAT_WITH_IMPL(_x, _with, _y)

// This macro will extpand to the twelfth argument.
#define __ISL_MACRO_GET_TWELFTH_ARG(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, ...) _12

// This macro is used to get the number of arguments, and supports max to 10 arguments.
#define __ISL_MACRO_GET_ARGS_COUNT(_vargs...) \
    __ISL_MACRO_GET_TWELFTH_ARG(, ##_vargs, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

// Supports max to 10 arguments.
#define __ISL_MACRO_HAS_ARG(_vargs...) \
    __ISL_MACRO_GET_TWELFTH_ARG(, ##_vargs, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)

/*
    Unpackager macro, it can make args_package: '(arg1,arg2,arg3)' convert to
    args_series:'arg1,arg2,arg3'. Usage: _ISL_MACRO_UNPACK args_package.
*/
#define _ISL_MACRO_UNPACK(_args_series...) _args_series

/* to avoid some trivial case, mute the noisy warning notice... */
#define _ISL_MAYBE_UNUSED (void)
#define _ISL_MACRO_EMPTY

// for easy...
#define null NULL


#define __ISL_UNPACK_CONCAT_WITH_IMPL(_x, _with, _y) _x##_with##_y
#define __ISL_UNPACK_CONCAT_WITH(_x, _with, _y)      __ISL_UNPACK_CONCAT_WITH_IMPL(_x, _with, _y)

#define __ISL_UNPACK_GET_TWELFTH_ARG(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, ...) _12
#define __ISL_UNPACK_GET_ARGS_COUNT(_vargs...) \
    __ISL_UNPACK_GET_TWELFTH_ARG(, ##_vargs, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define __ISL_UNPACK_FORWARD_0(_fn, _)              _ISL_MAYBE_UNUSED 0
#define __ISL_UNPACK_FORWARD_1(_fn, _1)             _fn(_1)
#define __ISL_UNPACK_FORWARD_2(_fn, _1, _2)         __ISL_UNPACK_FORWARD_1(_fn, _1), _fn(_2)
#define __ISL_UNPACK_FORWARD_3(_fn, _1, _2, _3)     __ISL_UNPACK_FORWARD_2(_fn, _1, _2), _fn(_3)
#define __ISL_UNPACK_FORWARD_4(_fn, _1, _2, _3, _4) __ISL_UNPACK_FORWARD_3(_fn, _1, _2, _3), _fn(_4)
#define __ISL_UNPACK_FORWARD_5(_fn, _1, _2, _3, _4, _5) \
    __ISL_UNPACK_FORWARD_4(_fn, _1, _2, _3, _4), _fn(_5)
#define __ISL_UNPACK_FORWARD_6(_fn, _1, _2, _3, _4, _5, _6) \
    __ISL_UNPACK_FORWARD_5(_fn, _1, _2, _3, _4, _5), _fn(_6)
#define __ISL_UNPACK_FORWARD_7(_fn, _1, _2, _3, _4, _5, _6, _7) \
    __ISL_UNPACK_FORWARD_6(_fn, _1, _2, _3, _4, _5, _6), _fn(_7)
#define __ISL_UNPACK_FORWARD_8(_fn, _1, _2, _3, _4, _5, _6, _7, _8) \
    __ISL_UNPACK_FORWARD_7(_fn, _1, _2, _3, _4, _5, _6, _7), _fn(_8)
#define __ISL_UNPACK_FORWARD_9(_fn, _1, _2, _3, _4, _5, _6, _7, _8, _9) \
    __ISL_UNPACK_FORWARD_8(_fn, _1, _2, _3, _4, _5, _6, _7, _8), _fn(_9)
#define __ISL_UNPACK_FORWARD_10(_fn, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) \
    __ISL_UNPACK_FORWARD_9(_fn, _1, _2, _3, _4, _5, _6, _7, _8, _9), _fn(_10)

#define _ISL_UNPACK_FORWARD(_fn, _vargs...)                                                \
    __ISL_UNPACK_CONCAT_WITH(__ISL_UNPACK_FORWARD, _, __ISL_UNPACK_GET_ARGS_COUNT(_vargs)) \
    (_fn, _vargs)


#endif // ISC_MACROS_H