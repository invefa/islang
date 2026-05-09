#ifndef ISC_OVERLOAD_H
#define ISC_OVERLOAD_H
/**
 * This header suplies some overloading macros.
 * It will be used in the implementation of isl_assert and isl_wssert...
 * Written by invefa.
 */

#include "isl_macros.h"

// This macro is used to forward call the overload function.
// It is a implementation for _isl_overload, just to keep the _isl_overload be simplified.
#define __ISL_OVERLOAD_FORWARD_CALL(_fn, _vargs...) \
    __ISL_MACRO_CONCAT_WITH(_fn, _, __ISL_MACRO_GET_ARGS_COUNT(_vargs))(_vargs)

// This macro is the implementation of c-style overload on islang core codes.
#define _isl_overload(_fn, _vargs...) __ISL_OVERLOAD_FORWARD_CALL(_fn, ##_vargs)

/**
 * If you don't understand this macro, please read the comments below.
 */

/**
 * Macro Expanding Order:
 * 1. Prescan the args of the macro, if it will be stringized or concatenated,
 *    just not expand it even it was a macro.
 * 2. If it will not, and it was a macro, do expanding, begin form step 1.
 * 3. Call the macro, do expanding, replace the params with args that all expanded.
 * 4. Cyclic do step 1.
 */

/**
 * This is a example of macro expansion.
 *
 * test_overload()
 * prescan ->  test_overload()
 * call    ->  _isl_overload(test_overload)
 * prescan ->  _isl_overload(test_overload)
 * call    ->  __ISL_OVERLOAD_FORWARD_CALL(test_overload)
 * prescan ->  __ISL_OVERLOAD_FORWARD_CALL(test_overload)
 * call    ->  __ISL_MACRO_CONCAT_WITH(test_overload, _ISL_MACRO_GET_ARGS_COUNT())()
 * prescan ->  __ISL_MACRO_CONCAT_WITH(test_overload, 0)()
 * call    ->  __ISL_MACRO_CONCAT_WITH_IMPL(test_overload, 0)()
 * prescan ->  __ISL_MACRO_CONCAT_WITH_IMPL(test_overload, 0)()
 * call    ->  test_overload_0()
 * test_overload_0()
 */

/**
 * This is another example of macro expansion.
 *
 * test_overload(123456)
 * prescan ->  test_overload(123456)
 * call    ->  _isl_overload(test_overload, 123456)
 * prescan ->  _isl_overload(test_overload, 123456)
 * call    ->  __ISL_OVERLOAD_FORWARD_CALL(test_overload, 123456)
 * prescan ->  __ISL_OVERLOAD_FORWARD_CALL(test_overload, 123456)
 * call    ->  __ISL_MACRO_CONCAT_WITH(test_overload, _, _ISL_MACRO_GET_ARGS_COUNT(123456))(123456)
 * prescan ->  __ISL_MACRO_CONCAT_WITH(test_overload, _, 1)(123456)
 * call    ->  __ISL_MACRO_CONCAT_WITH_IMPL(test_overload, _, 1)(123456)
 * prescan ->  __ISL_MACRO_CONCAT_WITH_IMPL(test_overload, _, 1)(123456)
 * call    ->  test_overload_1(123456)
 * test_overload_1(123456)
 */


/**
 * If we lost __ISL_MACRO_CONCAT_WITH_UNDERLINE_IMPL(...), it will be:
 *
 * test_overload(123456)
 * prescan ->  test_overload(123456)
 * call    ->  _isl_overload(test_overload, 123456)
 * prescan ->  _isl_overload(test_overload, 123456)
 * call    ->  __ISL_OVERLOAD_FORWARD_CALL(test_overload, 123456)
 * prescan ->  __ISL_OVERLOAD_FORWARD_CALL(test_overload, 123456)
 * call    ->  __ISL_MACRO_CONCAT_WITH(test_overload, _, _ISL_MACRO_GET_ARGS_COUNT(123456))(123456)
 * prescan ->  __ISL_MACRO_CONCAT_WITH(test_overload, _, _ISL_MACRO_GET_ARGS_COUNT(123456))(123456)
 * call    ->  test_overload__ISL_MACRO_GET_ARGS_COUNT(123456)(123456)
 * test_overload__ISL_MACRO_GET_ARGS_COUNT(123456)(123456)
 *
 * On this example, when the expanding come to __ISL_MACRO_CONCAT_WITH_UNDERLINE(...),
 * the args will be concatenated, so they should not be expanded anyway according to the prescan
 * rule. So the function of __ISL_MACRO_CONCAT_WITH_UNDERLINE_IMPL(...) was wait the expanding of
 * _ISL_MACRO_GET_ARGS_COUNT(...), then to concat test_overload and the result with underline.
 */

 
#define __ISL_BINARY_OVERLOAD_FORWARD_CALL(_fn, _vargs...) \
    __ISL_MACRO_CONCAT_WITH(_fn, _, __ISL_MACRO_HAS_ARG(_vargs))(_vargs)

/**
 * Binary overload forward only two case:
 *  1. _vargs has no content, it will forward to _fn##_0
 *  2. _vargs has    content, it will forward to _fn##_1 */
#define _isl_binary_overload(_fn, _vargs...) __ISL_BINARY_OVERLOAD_FORWARD_CALL(_fn, ##_vargs)


#endif // ISC_OVERLOAD_H