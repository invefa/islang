#ifndef ISL_LIST_H
#define ISL_LIST_H

#include "isl_overload.h"
#include "isl_xssert.h"
#include "isl_memgr.h"
#include "isl_types.h"
#include "isl_report.h"

/*
    Before read the comments below, you must to digest some basic concepts about the list:
        1. length:      the count of bytes that the list has.
        2. capacity:    the number of elements that the list can hold.
        3. size:        the number of elements that the list used.

    isl_list is a special structure:
        it has a base address that maintains a usize memory to store the length of its elements space.
        it has a head address that points to the first element of the list,
        and the head just located after the usize memory.
*/

/*
    Why 'catch' instead of 'get': because you will get control of the length value of the list,
    and the 'get' cannot reflect the right to operate, so I decided to use 'catch'.
*/
#define isl_list_catch_length(_list_adr)                    (((ist_usize*)(_list_adr))[-1])
#define isl_list_adr_get_capacity(_list_adr, _element_type) (isl_list_catch_length(_list_adr)/sizeof(_element_type))
#define isl_list_ptr_get_capacity(_list_ptr)                isl_list_adr_get_capacity(_list_ptr,typeof(*_list_ptr))

/*
    head regress base: convert the first element address of the list to the base address.
    base regress head: convert the base address of the list to the first element address.
*/

#define isl_list_head_regress_base(_list_adr) (((ist_usize*)(_list_adr))-1)

/*
    The difference between 'adr' and 'ptr':
    'adr' is the address of some piece of memory. Normally, it does not come with type information (same as void*).
    'ptr' same as 'adr', but with type information, so you can read and write the memory by the type (same as int*..).
    Notice: list adr is the address of the first element of the list in default, ptr as same.
    Notice: 'ptrv' was indicated a pointer variable.
    Operability: ptrv > ptr > adr.
*/

#define isl_list_base_regress_head_ptr(_list_base_adr, _element_type)   ((_element_type*)(((ist_usize*)(_list_base_adr))+1))
#define isl_list_base_regress_head_adr(_list_base_adr)                  isl_list_base_regress_head_ptr(_list_base_adr,void)


// components for __ISL_XALLOC_LIST
#define __ISL_XALLOC_FLAG_m 0
#define __ISL_XALLOC_FLAG_c 1

#define __ISL_XALLOC_LIST(_x, _type, _count)                \
    isl_list_base_regress_head_ptr(                         \
        _isl_set_adr_usize_value(                           \
            isl_allocate(                                   \
                sizeof(ist_usize)+sizeof(_type)*(_count),   \
                __ISL_XALLOC_FLAG_##_x),                    \
            sizeof(_type)*(_count)),                        \
        _type)

#define isl_malloc_list(_type, _count) __ISL_XALLOC_LIST(m,_type,_count)
#define isl_calloc_list(_type, _count) __ISL_XALLOC_LIST(c,_type,_count)


// components for __ISL_LIST_RESIZEX, this provide an alernative option to storage the result of resize.
#define __ISL_LIST_RESIZE_STORAGE_2(_ptrv, _list)               _ptrv=_list
#define __ISL_LIST_RESIZE_STORAGE_3(_ptr, _list, _stv)          _stv =_list
#define __ISL_LIST_RESIZE_STORAGE_4(_ptr, _list, _stv, _wtf)    isl_assert(0)

/*
    stv: storager variable, if there are some reason cause
    that you can't provide ptrv but ptr, it was an alternative option.
*/
#define __ISL_LIST_RESIZEX(_x, _ptr, _new_capcaity, _stv...)                                                \
do{                                                                                                         \
    typedef typeof(*_ptr) _element_type_;                                                                   \
    _element_type_* _ptr_=_ptr;                /*to eliminate side effects*/                                \
    ist_usize _new_capcaity_=_new_capcaity;    /*to eliminate side effects*/                                \
    isl_assert(_ptr_&&_new_capcaity_);                                                                      \
    ist_usize _capacity_=isl_list_ptr_get_capacity(_ptr_);                                                  \
    _element_type_* _new_list_=__ISL_XALLOC_LIST(_x,_element_type_,_new_capcaity_);                         \
    memcpy(_new_list_,_ptr_,(_new_capcaity_<_capacity_?_new_capcaity_:_capacity_)*sizeof(_element_type_));  \
    isl_list_catch_length(_new_list_)=sizeof(_element_type_)*_new_capcaity_;                                \
    isl_free_list(_ptr_);                                                                                   \
    _isl_overload(__ISL_LIST_RESIZE_STORAGE,_ptr,_new_list_,##_stv);                                        \
}while(0)

#define isl_list_resizec(_ptr, _new_capcaity, _stv...) __ISL_LIST_RESIZEX(c,_ptr,_new_capcaity,##_stv)
#define isl_list_resizem(_ptr, _new_capcaity, _stv...) __ISL_LIST_RESIZEX(m,_ptr,_new_capcaity,##_stv)

#define __ISL_LIST_ENSUREX(_x, _ptr, _size, _require, _stv...)                  \
do {                                                                            \
    ist_usize _size_=_size;                                                     \
    ist_usize _require_=_require;                                               \
    ist_usize _cap_=isl_list_ptr_get_capacity(_ptr);                            \
    isl_ifreport(_cap_<_size_,rid_catch_size_overflow, isp_catch_coreloc);      \
    if(_cap_-_size_<_require_){                                                 \
        isl_report(rid_inform_list_reisze,_ptr,_cap_,_cap_-_size_,_require_,    \
                    ceil_upon_powertwo(_cap_+_require_));                       \
        __ISL_LIST_RESIZEX(_x,_ptr,ceil_upon_powertwo(_cap_+_require_),##_stv); \
    }                                                                           \
}while (0)

//TODO: test ensurex.
#define isl_list_ensurec(_ptr, _size, _require, _stv...) __ISL_LIST_ENSUREX(c,_ptr,_size,_require,##_stv)
#define isl_list_ensurem(_ptr, _size, _require, _stv...) __ISL_LIST_ENSUREX(m,_ptr,_size,_require,##_stv)

// freev list means free the list and set the ptr variable to NULL.
#define isl_freev_list(_list_ptrv)                              \
do{                                                             \
    isl_assert(_list_ptrv);                                     \
    isl_release(                                                \
        isl_list_head_regress_base(_list_ptrv),                 \
        sizeof(ist_usize)+isl_list_catch_length(_list_ptrv));   \
    _list_ptrv = NULL;                                          \
}while(0)

// just free the list.
#define isl_free_list(_list_adr)                                \
do{                                                             \
    void* _list_adr_ = _list_adr; /*to eliminate side effects*/ \
    isl_assert(_list_adr_);                                     \
    isl_release(                                                \
        isl_list_head_regress_base(_list_adr_),                 \
        sizeof(ist_usize)+isl_list_catch_length(_list_adr_));   \
}while(0)

/*
    this function will obtain a value, and return the nearest power of two,
    and if you submit a number that was power of two, the function will return itself.
*/
ist_usize ceil_upon_powertwo(ist_usize _value);

#endif