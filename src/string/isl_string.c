#include "isl_string.h"

#include <string.h>

#include "isl_memgr.h"
#include "isl_list.h"


ist_string* ist_string_create(char* _cstring, ist_usize _length) {
    ist_string* string = isl_malloc(ist_string);
    ist_string_init(string, _cstring, _length);
    return string;
}

ist_string* ist_string_create_buffer(ist_usize _capacity) {
    isl_assert(_capacity);
    ist_string* string = isl_malloc(ist_string);
    *string = isl_calloc_list(ist_byte, _capacity);
    return string;
}

void ist_string_init(ist_string* _string, char* _cstring, ist_usize _length) {
    *_string = isl_calloc_list(ist_byte, _length + 1);
    memcpy(*_string, _cstring, _length);
    (*_string)[_length] = '\0';
}

void ist_string_clean(ist_string* _string) {
    isl_assert(_string);
    if (*_string)
        isl_freev_list(*_string);
    *_string = NULL;
}

void ist_string_delete(ist_string* _string) {
    ist_string_clean(_string);
    isl_free(_string);
}


