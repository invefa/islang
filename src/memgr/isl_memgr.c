#include "isl_memgr.h"
#include "isl_xssert.h"

#include <stdlib.h>

ist_usize isl_allocated_length = 0;

inline void* isl_allocate(ist_usize _size, ist_bool _clean) {
    isl_assert(_size);
    void* ptr = _clean ? calloc(_size, 1) : malloc(_size);
    isl_assert(ptr, "allocate failed.");
    isl_allocated_length += _size;
    return ptr;
}

inline void isl_release(void* _ptr, ist_usize _size) {
    isl_assert(_ptr);
    free(_ptr);
    isl_allocated_length -= _size;
}

inline void* _isl_set_adr_usize_value(void* _adr, ist_usize _value) {
    isl_assert(_adr);
    *(ist_usize*)_adr = _value;
    return _adr;
}
