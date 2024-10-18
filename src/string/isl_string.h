#ifndef ISL_STRING_H
#define ISL_STRING_H

#include "isl_types.h"

/* ist_string was the byte isl_list actually, restrict type that must allocate at heap */
typedef ist_i8* ist_string;

/*c string , can be char* or ist_string */
typedef ist_string ist_cstring;

ist_string  ist_string_consby_ref(ist_cstring _cstring, ist_usize _length);
ist_string* ist_string_initby_ref(ist_string* this, ist_cstring _cstring, ist_usize _length);
ist_string* ist_string_createby_ref(ist_cstring _cstring, ist_usize _length);

ist_string  ist_string_consby_raw(ist_cstring _cstring);
ist_string* ist_string_initby_raw(ist_string* this, ist_cstring _cstring);
ist_string* ist_string_createby_raw(ist_cstring _cstring);

ist_string  ist_string_cons_buffer(ist_usize _capacity);
ist_string* ist_string_init_buffer(ist_string* this, ist_usize _capacity);
ist_string* ist_string_create_buffer(ist_usize _capacity);

void ist_string_clean(ist_string* this);
void ist_string_delete(ist_string* this);


void ist_string_buffer_ensure(ist_string* this, ist_usize _buffer_size, ist_usize _required_length);

ist_string* ist_string_buffer_append_raw(ist_string* this, ist_usize* _indexv, ist_cstring _string);
ist_string* ist_string_buffer_append_ref(
    ist_string* this,
    ist_usize*  _indexv,
    ist_cstring _string,
    ist_usize   _length
);

#endif