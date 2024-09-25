#include "isl_string.h"

#include <string.h>

#include "isl_memgr.h"
#include "isl_list.h"
#include "isl_report.h"

inline ist_string* ist_string_createby_ref(char* _cstring, ist_usize _length) {
    ist_string* string = isl_malloc(ist_string);
    ist_string_initby_ref(string, _cstring, _length);
    return string;
}

inline ist_string* ist_string_createby_raw(const char* _cstring) {
    ist_string* string = isl_malloc(ist_string);
    ist_string_initby_ref(string, (char*)_cstring, strlen(_cstring));
    return string;
}

inline ist_string* ist_string_create_buffer(ist_usize _capacity) {
    ist_string* string = isl_malloc(ist_string);
    isl_ifnreport(_capacity, rid_catch_zero_capacity_buffer, isp_catch_coreloc, string);
    ist_string_init_buffer(string, _capacity);
    return string;
}

inline ist_string ist_string_consby_ref(char* _cstring, ist_usize _length) {
    ist_string string;
    ist_string_initby_ref(&string, _cstring, _length);
    return string;
}

inline ist_string ist_string_consby_raw(char* _cstring) {
    ist_string string;
    ist_string_initby_raw(&string, _cstring);
    return string;
}

inline void ist_string_initby_ref(ist_string* this, char* _cstring, ist_usize _length) {
    *this = isl_calloc_list(ist_byte, _length + 1);
    isl_ifnreport(_length, rid_catch_zero_string_length, isp_catch_coreloc, this);
    memcpy(*this, _cstring, _length);
    (*this)[_length] = '\0';
}

inline void ist_string_initby_raw(ist_string* this, char* _cstring) {
    ist_string_initby_ref(this, (char*)_cstring, strlen(_cstring));
}

inline void ist_string_init_buffer(ist_string* this, ist_usize _capacity) {
    *this = isl_calloc_list(ist_byte, _capacity);
}

inline void ist_string_clean(ist_string* this) {
    isl_ifnreport(this, rid_catch_nullptr, isp_catch_coreloc);
    if (*this)
        isl_freev_list(*this);
    *this = NULL;
}

inline void ist_string_delete(ist_string* this) {
    isl_ifnreport(this, rid_catch_nullptr, isp_catch_coreloc);
    ist_string_clean(this);
    isl_free(this);
}

inline void ist_string_buffer_ensure(ist_string* this, ist_usize _buffer_size, ist_usize _required_length) {

    if (isl_list_catch_length(*this) - _buffer_size >= _required_length) return;

    isl_report(
                rid_inform_buffer_reisze, this,
                isl_list_catch_length(*this),
                isl_list_catch_length(*this) - _buffer_size,
                _required_length,
                ceil_upon_powertwo(_buffer_size + _required_length)
    );

    isl_list_resizec(*this, ceil_upon_powertwo(_buffer_size + _required_length));
}

inline void ist_string_buffer_append_autostv(ist_string* this, ist_usize* _index, ist_string _string, ist_usize _length) {
    ist_string_buffer_ensure(this, *_index, _length);
    memcpy((*this) + *_index, _string, _length);
    *_index += _length;
    (*this)[*_index] = '\0';
}
