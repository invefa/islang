#ifndef ISL_MEMGR_H
#define ISL_MEMGR_H

#include <string.h>

#include "isl_types.h"
#include "isl_xssert.h"
#include "isl_overload.h"

// all we allocated memory will be recorded at this variable.
extern ist_usize isl_allocated_length;

// allocate a memory and record length of it.
void* isl_allocate(ist_usize _size, ist_bool _clean);

// free a memory and record length of it.
void isl_release(void* _ptr, ist_usize _size);

void* _isl_set_adr_usize_value(void* _adr, ist_usize _value);


// after there, we define some useful macros to allocate and release single type memory.

#define isl_malloc(_type) ((_type*)isl_allocate(sizeof(_type),0))
#define isl_calloc(_type) ((_type*)isl_allocate(sizeof(_type),1))


// freev means free the memory and set the ptr variable to NULL.
#define isl_freev(_ptrv)            \
do{                                 \
    isl_assert(_ptrv);              \
    isl_release(_ptrv,              \
        sizeof(typeof(*_ptrv)));    \
    _ptrv = NULL;                   \
}while(0)

// just free the memory.
#define isl_free(_ptr)              \
do{                                 \
    void* _ptr_ = _ptr;             \
    isl_assert(_ptr_);              \
    isl_release(_ptr_,              \
        sizeof(typeof(*_ptr)));     \
}while(0)


#endif