#ifndef ISL_OVERLOAD_H
#define ISL_OVERLOAD_H
/*
    This header suplies some overloading macros.
    It will be used in the implementation of isl_assert and isl_wssert...
    Written by invefa.
*/

// This macro will extpand to the concatenation of _x and _y with underline.
#define __ISL_MACRO_CONCAT_WITH_UNDERLINE_IMPL(_x, _y) _x##_##_y

// Why do a wrapper for this macro? Because we need to wait the expanding of __ISL_MACRO_GET_ARGS_COUNT(...)
#define __ISL_MACRO_CONCAT_WITH_UNDERLINE(_x, _y) __ISL_MACRO_CONCAT_WITH_UNDERLINE_IMPL(_x, _y)

// This macro will extpand to the twelfth argument.
#define __ISL_MACRO_GET_TWELFTH_ARG(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, ...) _12

// This macro is used to get the number of arguments, and supports max to 10 arguments.
#define __ISL_MACRO_GET_ARGS_COUNT(_vargs...) \
    __ISL_MACRO_GET_TWELFTH_ARG(,##_vargs, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

// This macro is used to forward call the overload function.
// It is a implementation for _isl_overload, just to keep the _isl_overload be simplified.
#define __ISL_OVERLOAD_FORWARD_CALL(_fn, _vargs...) \
    __ISL_MACRO_CONCAT_WITH_UNDERLINE(_fn,__ISL_MACRO_GET_ARGS_COUNT(_vargs))(_vargs)

// This macro is the implementation of c-style overload on islang core codes.
#define _isl_overload(_fn, _vargs...) __ISL_OVERLOAD_FORWARD_CALL(_fn,##_vargs)

/*
    If you don't understand this macro, please read the comments below.
*/

/*
    Macro Expanding Order:
        1. Prescan the args of the macro, if it will be stringized or concatenated, just not expand it even it was a macro.
        1. If it will not, and it was a macro, do expanding, begin form step 1.
        2. Call the macro, do expanding, replace the params with args that all expanded.
        4. Cyclic do step 1.
*/

/*
    This is a example of macro expansion.

    test_overload()
    prescan ->  test_overload()
    call    ->  _ISL_OVERLOAD(test_overload)
    prescan ->  _ISL_OVERLOAD(test_overload)
    call    ->  __ISL_OVERLOAD_FORWARD_CALL(test_overload)
    prescan ->  __ISL_OVERLOAD_FORWARD_CALL(test_overload)
    call    ->  __ISL_MACRO_CONCAT_WITH_UNDERLINE(test_overload, _ISL_MACRO_GET_ARGS_COUNT())()
    prescan ->  __ISL_MACRO_CONCAT_WITH_UNDERLINE(test_overload, 0)()
    call    ->  __ISL_MACRO_CONCAT_WITH_UNDERLINE_IMPL(test_overload, 0)()
    prescan ->  __ISL_MACRO_CONCAT_WITH_UNDERLINE_IMPL(test_overload, 0)()
    call    ->  test_overload_0()
    test_overload_0()
*/

/*
    This is another example of macro expansion.

    test_overload(123456)
    prescan ->  test_overload(123456)
    call    ->  _ISL_OVERLOAD(test_overload, 123456)
    prescan ->  _ISL_OVERLOAD(test_overload, 123456)
    call    ->  __ISL_OVERLOAD_FORWARD_CALL(test_overload, 123456)
    prescan ->  __ISL_OVERLOAD_FORWARD_CALL(test_overload, 123456)
    call    ->  __ISL_MACRO_CONCAT_WITH_UNDERLINE(test_overload, _ISL_MACRO_GET_ARGS_COUNT(123456))(123456)
    prescan ->  __ISL_MACRO_CONCAT_WITH_UNDERLINE(test_overload, 1)(123456)
    call    ->  __ISL_MACRO_CONCAT_WITH_UNDERLINE_IMPL(test_overload, 1)(123456)
    prescan ->  __ISL_MACRO_CONCAT_WITH_UNDERLINE_IMPL(test_overload, 1)(123456)
    call    ->  test_overload_1(123456)
    test_overload_1(123456)
*/


/*
    If we lost __ISL_MACRO_CONCAT_WITH_UNDERLINE_IMPL(...), it will be:

    test_overload(123456)
    prescan ->  test_overload(123456)
    call    ->  _ISL_OVERLOAD(test_overload, 123456)
    prescan ->  _ISL_OVERLOAD(test_overload, 123456)
    call    ->  __ISL_OVERLOAD_FORWARD_CALL(test_overload, 123456)
    prescan ->  __ISL_OVERLOAD_FORWARD_CALL(test_overload, 123456)
    call    ->  __ISL_MACRO_CONCAT_WITH_UNDERLINE(test_overload, _ISL_MACRO_GET_ARGS_COUNT(123456))(123456)
    prescan ->  __ISL_MACRO_CONCAT_WITH_UNDERLINE(test_overload, _ISL_MACRO_GET_ARGS_COUNT(123456))(123456)
    call    ->  test_overload__ISL_MACRO_GET_ARGS_COUNT(123456)(123456)
    test_overload__ISL_MACRO_GET_ARGS_COUNT(123456)(123456)

    On this example, when the expanding come to __ISL_MACRO_CONCAT_WITH_UNDERLINE(...),
    the args will be concatenated, so they should not be expanded anyway according to the prescan rule.
    So the function of __ISL_MACRO_CONCAT_WITH_UNDERLINE_IMPL(...)was wait the expanding
    of _ISL_MACRO_GET_ARGS_COUNT(...), then to concat test_overload and the result with underline.
*/


#endif