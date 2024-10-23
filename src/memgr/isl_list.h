#ifndef ISC_LIST_H
#define ISC_LIST_H

#include "isl_memgr.h"
#include "isl_overload.h"
#include "isl_report.h"
#include "isl_types.h"
#include "isl_xssert.h"


/**
 * Before read the comments below, you must to digest some basic concepts about the list:
 *  1. length:      the count of bytes that the list has.
 *  2. capacity:    the number of elements that the list can hold.
 *  3. size:        the number of elements that the list used.
 *
 * isl_list is a special structure:
 * it has a base address that maintains a usize memory to store the length of its elements space.
 * it has a head address that points to the first element of the list, and the head just located
 * after the usize memory.
 */


/**
 * Why 'catch' instead of 'get': because you will get control of the length value of the list,
 * and the 'get' cannot reflect the right to operate, so I decided to use 'catch'.
 */
#define isl_list_catch_length(_list_adr) (((ist_usize*)(_list_adr))[-1])
#define isl_list_adr_get_capacity(_list_adr, _element_type) \
    (isl_list_catch_length(_list_adr) / sizeof(_element_type))
#define isl_list_ptr_get_capacity(_list_ptr) \
    isl_list_adr_get_capacity(_list_ptr, typeof(*_list_ptr))

/**
 * head regress base: convert the first element address of the list to the base address.
 * base regress head: convert the base address of the list to the first element address.
 */

#define isl_list_head_regress_base(_list_adr) (((ist_usize*)(_list_adr)) - 1)


/**
 * The difference between 'adr' and 'ptr':
 * 'adr' is the address of some piece of memory. Normally, it does not come with type information
 * (same as void*). 'ptr' same as 'adr', but with type information, so you can read and write the
 * memory by the type (same as int*..).
 *
 * Notice: list adr is the address of the first element of the list in default, ptr as same.
 * Notice: 'ptrv' was indicated a pointer variable.
 *
 * Operability: ptrv > ptr > adr.
 */

#define isl_list_base_regress_head_ptr(_list_base_adr, _element_type) \
    ((_element_type*)(((ist_usize*)(_list_base_adr)) + 1))
#define isl_list_base_regress_head_adr(_list_base_adr) \
    isl_list_base_regress_head_ptr(_list_base_adr, void)



/* components for __ISL_LIST_XALLOC */
#define __ISL_LIST_XALLOC_FLAG_m 0
#define __ISL_LIST_XALLOC_FLAG_c 1

#define __ISL_LIST_XALLOC(_x, _type, _count)                                              \
    isl_list_base_regress_head_ptr(                                                       \
        _isl_set_adr_usize_value(                                                         \
            isl_allocate(                                                                 \
                sizeof(ist_usize) + sizeof(_type) * (_count), __ISL_LIST_XALLOC_FLAG_##_x \
            ),                                                                            \
            sizeof(_type) * (_count)                                                      \
        ),                                                                                \
        _type                                                                             \
    )

#define isl_list_malloc(_type, _count) __ISL_LIST_XALLOC(m, _type, _count)
#define isl_list_calloc(_type, _count) __ISL_LIST_XALLOC(c, _type, _count)



/* freev list means free the list and set the ptr variable to NULL */
#define isl_list_freev(_list_ptrv)                                \
    do {                                                          \
        isl_assert(_list_ptrv);                                   \
        isl_release(                                              \
            isl_list_head_regress_base(_list_ptrv),               \
            sizeof(ist_usize) + isl_list_catch_length(_list_ptrv) \
        );                                                        \
        _list_ptrv = NULL;                                        \
    } while (0)

// just free the list.
#define isl_list_free(_list_adr)                                                \
    do {                                                                        \
        void* isl_list_free_list_adr = _list_adr; /*to eliminate side effects*/ \
        isl_list_freev(isl_list_free_list_adr);                                 \
    } while (0)



#define __ISL_LIST_RESIZXV(_x, _ptrv, _nse_newcap)                                               \
    do {                                                                                         \
        typedef typeof(*(_ptrv)) isl_list_resizev_element_type;                                  \
        isl_assert((_ptrv) && (_nse_newcap));                                                    \
        isl_list_resizev_element_type* isl_list_resizev_new_list =                               \
            __ISL_LIST_XALLOC(_x, isl_list_resizev_element_type, _nse_newcap);                   \
        memcpy(                                                                                  \
            isl_list_resizev_new_list,                                                           \
            _ptrv,                                                                               \
            isl_minimum(_nse_newcap, isl_list_ptr_get_capacity(_ptrv))                           \
                * sizeof(isl_list_resizev_element_type)                                          \
        );                                                                                       \
        isl_list_catch_length(isl_list_resizev_new_list) = sizeof(isl_list_resizev_element_type) \
                                                         * (_nse_newcap);                        \
        isl_list_freev(_ptrv);                                                                   \
        (_ptrv) = isl_list_resizev_new_list;                                                     \
    } while (0)

#define isl_list_resizcv(_ptrv, _newcapv) __ISL_LIST_RESIZXV(c, _ptrv, _newcapv)
#define isl_list_resizmv(_ptrv, _newcapv) __ISL_LIST_RESIZXV(m, _ptrv, _newcapv)


/**
 * components for __ISL_LIST_RESIZX, this provide an alernative
 * option to storage the result of resize.
 */
#define __ISL_LIST_RESIZE_STORAGE_2(_ptrv, _list)            _ptrv = _list
#define __ISL_LIST_RESIZE_STORAGE_3(_ptr, _list, _stv)       _stv = _list
#define __ISL_LIST_RESIZE_STORAGE_4(_ptr, _list, _stv, _wtf) isl_assert(0)

/**
 * stv: storager variable, if there are some reason cause
 * that you can't provide ptrv but ptr, it was an alternative option.
 */
#define __ISL_LIST_RESIZX(_x, _ptr, _newcap, _stv...)                                      \
    do {                                                                                   \
        typeof(_ptr) isl_list_resize_ptr          = _ptr;    /*to eliminate side effects*/ \
        ist_usize    isl_list_resize_new_capacity = _newcap; /*to eliminate side effects*/ \
        isl_assert(isl_list_resize_ptr &&isl_list_resize_new_capacity);                    \
        __ISL_LIST_RESIZXV(_x, isl_list_resize_ptr, isl_list_resize_new_capacity);         \
        _isl_overload(__ISL_LIST_RESIZE_STORAGE, _ptr, isl_list_resize_ptr, ##_stv);       \
    } while (0)

#define isl_list_resizc(_ptr, _newcap, _stv...) __ISL_LIST_RESIZX(c, _ptr, _newcap, ##_stv)
#define isl_list_resizm(_ptr, _newcap, _stv...) __ISL_LIST_RESIZX(m, _ptr, _newcap, ##_stv)


#define __ISL_LIST_ENSUREXV(_x, _ptrv, _nse_size, _nse_require)                                 \
    do {                                                                                        \
        ist_usize isl_list_ensurev_capacity = isl_list_ptr_get_capacity(_ptrv);                 \
        isl_ifreport(                                                                           \
            isl_list_ensurev_capacity < (_nse_size), rid_catch_size_overflow, isp_catch_coreloc \
        );                                                                                      \
        if (isl_list_ensurev_capacity - (_nse_size) < (_nse_require)) {                         \
            isl_report(                                                                         \
                rid_inform_list_reisze,                                                         \
                _ptrv,                                                                          \
                isl_list_ensurev_capacity,                                                      \
                isl_list_ensurev_capacity - (_nse_size),                                        \
                (_nse_require),                                                                 \
                ceil_upon_powertwo(isl_list_ensurev_capacity + (_nse_require))                  \
            );                                                                                  \
            __ISL_LIST_RESIZXV(                                                                 \
                _x, _ptrv, ceil_upon_powertwo(isl_list_ensurev_capacity + (_nse_require))       \
            );                                                                                  \
        }                                                                                       \
    } while (0)

#define isl_list_ensurecv(_ptrv, _nse_size, _nse_require) \
    __ISL_LIST_ENSUREXV(c, _ptrv, _nse_size, _nse_require)
#define isl_list_ensuremv(_ptrv, _nse_size, _nse_require) \
    __ISL_LIST_ENSUREXV(m, _ptrv, _nse_size, _nse_require)


/**
 * components for __ISL_LIST_ENSUREX, used to fix the problem when the ptr is a side effect
 * expression. when there exist the stv, indicate the ptr might not be a variable, and it might has
 * side effects, in this case, we can't provide it to __ISL_LIST_ENSUREXV directly, so we use
 * __ISL_LIST_ENSURE_STORAGE to forward it.
 */
#define __ISL_LIST_ENSURE_STORAGE_1(_ptr)             _ptr
#define __ISL_LIST_ENSURE_STORAGE_2(_ptr, _stv)       _stv
#define __ISL_LIST_ENSURE_STORAGE_3(_ptr, _stv, _wtf) isl_assert(0)

/**
 * why there has double underline suffix variable here?
 * because there will occur _ptr_=_ptr_ in macro:resize when it has sigle underline suffix.
 */
#define __ISL_LIST_ENSUREX(_x, _ptr, _size, _require, _stv...)                         \
    do {                                                                               \
        typeof(_ptr) isl_list_ensure_ptr     = _ptr;     /*to eliminate side effects*/ \
        ist_usize    isl_list_ensure_size    = _size;    /*to eliminate side effects*/ \
        ist_usize    isl_list_ensure_require = _require; /*to eliminate side effects*/ \
        __ISL_LIST_ENSUREXV(                                                           \
            _x, isl_list_ensure_ptr, isl_list_ensure_size, isl_list_ensure_require     \
        );                                                                             \
        _isl_overload(__ISL_LIST_ENSURE_STORAGE, _ptr, ##_stv) = isl_list_ensure_ptr;  \
    } while (0)

#define isl_list_ensurec(_ptr, _size, _require, _stv...) \
    __ISL_LIST_ENSUREX(c, _ptr, _size, _require, ##_stv)
#define isl_list_ensurem(_ptr, _size, _require, _stv...) \
    __ISL_LIST_ENSUREX(m, _ptr, _size, _require, ##_stv)



#define __ISL_LIST_ADDXV(_x, _ptrv, _sizev, _value) \
    do {                                            \
        __ISL_LIST_ENSUREXV(_x, _ptrv, _sizev, 1);  \
        (_ptrv)[(_sizev)++] = _value;               \
    } while (0)

#define isl_list_addcv(_ptrv, _sizev, _value) __ISL_LIST_ADDXV(c, _ptrv, _sizev, _value)
#define isl_list_addmv(_ptrv, _sizev, _value) __ISL_LIST_ADDXV(m, _ptrv, _sizev, _value)


/* components for __ISL_LIST_ADDX */
#define __ISL_LIST_ADD_STORAGE_3(_ptr, _sizev, _value)       (_ptr)[(_sizev)++] = _value
#define __ISL_LIST_ADD_STORAGE_4(_ptr, _sizev, _value, _stv) (_stv)[(_sizev)++] = _value

/* ensure slot and add element value to the last unused slot of the list then advance sizev */
#define __ISL_LIST_ADDX(_x, _ptr, _sizev, _value, _stv...)                   \
    do {                                                                     \
        __ISL_LIST_ENSUREX(_x, _ptr, _sizev, 1, ##_stv);                     \
        _isl_overload(__ISL_LIST_ADD_STORAGE, _ptr, _sizev, _value, ##_stv); \
    } while (0)

#define isl_list_addc(_ptr, _sizev, _value, _stv...) \
    __ISL_LIST_ADDX(c, _ptr, _sizev, _value, ##_stv)
#define isl_list_addm(_ptr, _sizev, _value, _stv...) \
    __ISL_LIST_ADDX(m, _ptr, _sizev, _value, ##_stv)



#define in ,

#define __ISL_LIST_FOREACH_IDXVNAME_0()                       isl_list_foreach_index
#define __ISL_LIST_FOREACH_IDXVNAME_1(_idxv_name)             _idxv_name
#define __ISL_LIST_FOREACH_IDXVNAME_2(_idxv_name, _capv_name) _idxv_name

#define __ISL_LIST_FOREACH_CAPVNAME_0()                       isl_list_foreach_capacity
#define __ISL_LIST_FOREACH_CAPVNAME_1(_idxv_name)             __ISL_LIST_FOREACH_CAPVNAME_0()
#define __ISL_LIST_FOREACH_CAPVNAME_2(_idxv_name, _capv_name) _capv_name

#define _ISL_LIST_FOREACH_IDXVNAME(_names...) _isl_overload(__ISL_LIST_FOREACH_IDXVNAME, ##_names)
#define _ISL_LIST_FOREACH_CAPVNAME(_names...) _isl_overload(__ISL_LIST_FOREACH_CAPVNAME, ##_names)

/**
 * from index that given to capacity that given.
 *
 * iterpid stand for iterator ptr identifier, indicate you must provide a legal identifier there.
 * vargs reserved for the optional index and capacity variable name,
 * and the first for index, second for capacity.
 */
#define isl_list_foreach_from_to(_iterpid, _listv, _from, _to, _names...)                         \
    for (ist_usize _ISL_LIST_FOREACH_IDXVNAME(_names) = _from,                                    \
                   _ISL_LIST_FOREACH_CAPVNAME(_names) = _to;                                      \
         _ISL_LIST_FOREACH_IDXVNAME(_names) < _ISL_LIST_FOREACH_CAPVNAME(_names);                 \
         ++_ISL_LIST_FOREACH_IDXVNAME(_names))                                                    \
        for (ist_bool isl_list_foreach_flag = 1; isl_list_foreach_flag;)                          \
            for (typeof((_listv)[0])* _iterpid = ((_listv) + _ISL_LIST_FOREACH_IDXVNAME(_names)); \
                 isl_list_foreach_flag;                                                           \
                 isl_list_foreach_flag = 0)

/**
 * form index that given to the end of the list.
 *
 * iterpid stand for iterator ptr identifier, indicate you must provide a legal identifier there.
 * vargs reserved for the optional index and capacity variable name,
 * and the first for index, second for capacity.
 */
#define isl_list_foreach_from(_iterpid, _listv, _from, _names...) \
    isl_list_foreach_from_to (_iterpid, _listv, _from, isl_list_ptr_get_capacity(_listv), ##_names)

/**
 * form 0 to capacity that given.
 *
 * iterpid stand for iterator ptr identifier, indicate you must provide a legal identifier there.
 * vargs reserved for the optional index and capacity variable name,
 * and the first for index, second for capacity.
 */
#define isl_list_foreach_to(_iterpid, _listv, _to, _names...) \
    isl_list_foreach_from_to (_iterpid, _listv, 0, _to, ##_names)

/**
 * form 0 to the end of the list.
 *
 * iterpid stand for iterator ptr identifier, indicate you must provide a legal identifier there.
 * vargs reserved for the optional index and capacity variable name,
 * and the first for index, second for capacity.
 */
#define isl_list_foreach(_iterpid, _listv, _names...) \
    isl_list_foreach_from_to (_iterpid, _listv, 0, isl_list_ptr_get_capacity(_listv), ##_names)

/**
 * this function will obtain a value, and return the nearest power of two,
 * and if you submit a number that was power of two, the function will return itself.
 */
ist_usize ceil_upon_powertwo(ist_usize _value);

#endif // ISC_LIST_H