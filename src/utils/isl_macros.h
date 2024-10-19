#ifndef ISC_MACROS_H
#define ISC_MACROS_H
/*
    This header suplies some macros that can make the implementation of overload and xssert
    simplify. Written by invefa.
*/

#define isl_minimum(_x, _y) ((_x) < (_y) ? (_x) : (_y))
#define isl_maximum(_x, _y) ((_x) > (_y) ? (_x) : (_y))

// This macro will extpand to the concatenation of _x and _y with underline.
#define __ISL_MACRO_CONCAT_WITH_UNDERLINE_IMPL(_x, _y) _x##_##_y

// Why do a wrapper for this macro? Because we need to wait the expanding of
// __ISL_MACRO_GET_ARGS_COUNT(...)
#define __ISL_MACRO_CONCAT_WITH_UNDERLINE(_x, _y) __ISL_MACRO_CONCAT_WITH_UNDERLINE_IMPL(_x, _y)

// This macro will extpand to the twelfth argument.
#define __ISL_MACRO_GET_TWELFTH_ARG(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, ...) _12

// This macro is used to get the number of arguments, and supports max to 10 arguments.
#define __ISL_MACRO_GET_ARGS_COUNT(_vargs...) \
    __ISL_MACRO_GET_TWELFTH_ARG(, ##_vargs, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

/*
    Unpackager macro, it can make args_package: '(arg1,arg2,arg3)' convert to
    args_series:'arg1,arg2,arg3'. Usage: __ISL_MACRO_UNPACKAGER args_package.
*/
#define __ISL_MACRO_UNPACKAGER(_args_series...) _args_series

#endif // ISC_MACROS_H