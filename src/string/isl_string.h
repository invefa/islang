#ifndef ISL_STRING_H
#define ISL_STRING_H

#include "isl_types.h"

/* ist_string was the byte isl_list actually. */
typedef ist_byte* ist_string;

void        ist_string_init(ist_string* _string, char* _cstring, ist_usize _length);

ist_string* ist_string_create(char* _cstring, ist_usize _length);
ist_string* ist_string_create_buffer(ist_usize _capacity);

void        ist_string_clean(ist_string* _string);
void        ist_string_delete(ist_string* _string);

void        ist_string_buffer_ensure(ist_string* _buffer, ist_usize _buffer_size, ist_usize _required_length);

#endif