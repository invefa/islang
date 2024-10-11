#ifndef ISL_STRING_H
#define ISL_STRING_H

#include "isl_types.h"

/* ist_string was the byte isl_list actually. */
typedef ist_byte* ist_string;

ist_string  ist_string_consby_ref(char* _cstring, ist_usize _length);
ist_string* ist_string_initby_ref(ist_string* this, char* _cstring, ist_usize _length);
ist_string* ist_string_createby_ref(char* _cstring, ist_usize _length);

ist_string  ist_string_consby_raw(char* _cstring);
ist_string* ist_string_initby_raw(ist_string* this, char* _cstring);
ist_string* ist_string_createby_raw(const char* _cstring);

ist_string  ist_string_cons_buffer(ist_usize _capacity);
ist_string* ist_string_init_buffer(ist_string* this, ist_usize _capacity);
ist_string* ist_string_create_buffer(ist_usize _capacity);

void        ist_string_clean(ist_string* this);
void        ist_string_delete(ist_string* this);

void        ist_string_buffer_ensure(ist_string* this, ist_usize _buffer_size, ist_usize _required_length);
// void        ist_string_buffer_append_autostv(ist_string* this, ist_usize* _index, ist_string _string, ist_usize _length);

#endif