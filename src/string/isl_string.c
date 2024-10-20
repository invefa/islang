#include "isl_string.h"

#include <stdarg.h>
#include <string.h>


#include "isl_list.h"
#include "isl_memgr.h"
#include "isl_report.h"


inline ist_string ist_string_consby_ref(ist_cstring _cstring, ist_usize _length) {
    ist_string string = isl_list_calloc(ist_byte, _length + 1);
    memcpy(string, _cstring, _length);
    string[_length] = '\0';
    return string;
}
inline ist_string* ist_string_initby_ref(
    ist_string* this,
    ist_cstring _cstring,
    ist_usize   _length
) {
    *this = ist_string_consby_ref(_cstring, _length);
    return this;
}
inline ist_string* ist_string_createby_ref(ist_cstring _cstring, ist_usize _length) {
    return ist_string_initby_ref(isl_malloc(ist_string), _cstring, _length);
}


inline ist_string ist_string_consby_raw(ist_cstring _cstring) {
    return ist_string_consby_ref(_cstring, strlen(_cstring));
}
inline ist_string* ist_string_initby_raw(ist_string* this, ist_cstring _cstring) {
    return ist_string_initby_ref(this, _cstring, strlen(_cstring));
}
inline ist_string* ist_string_createby_raw(ist_cstring _cstring) {
    return ist_string_createby_ref(_cstring, strlen(_cstring));
}


ist_string ist_string_cons_buffer(ist_usize _capacity) {
    ist_string buffer = isl_list_calloc(ist_byte, _capacity);
    isl_ifnreport(_capacity, rid_catch_zero_capacity_buffer, isp_catch_coreloc, buffer);
    return buffer;
}
inline ist_string* ist_string_init_buffer(ist_string* this, ist_usize _capacity) {
    *this = ist_string_cons_buffer(_capacity);
    return this;
}
inline ist_string* ist_string_create_buffer(ist_usize _capacity) {
    return ist_string_init_buffer(isl_malloc(ist_string), _capacity);
}


inline void ist_string_clean(ist_string* this) {
    isl_ifnreport(this, rid_catch_nullptr, isp_catch_coreloc);
    if (*this) isl_list_freev(*this);
    *this = NULL;
}
inline void ist_string_delete(ist_string* this) {
    isl_ifnreport(this, rid_catch_nullptr, isp_catch_coreloc);
    ist_string_clean(this);
    isl_free(this);
}


inline void ist_string_buffer_ensure(
    ist_string* this,
    ist_usize _buffer_size,
    ist_usize _required_length
) {

    /* check nullptr */
    isl_assert(this);

    /* check buffer remaining length */
    if (isl_list_catch_length(*this) - _buffer_size >= _required_length) return;

    /* inform buffer will be reiszed */
    isl_report(
        rid_inform_buffer_reisze,
        this,
        isl_list_catch_length(*this),
        isl_list_catch_length(*this) - _buffer_size,
        _required_length,
        ceil_upon_powertwo(_buffer_size + _required_length)
    );

    /* resize buffer */
    isl_list_resizec(*this, ceil_upon_powertwo(_buffer_size + _required_length));
}


inline ist_string* ist_string_buffer_append_ref(
    ist_string* this,
    ist_usize*  _indexv,
    ist_cstring _string,
    ist_usize   _length
) {

    /* check nullptr */
    isl_assert(_indexv);

    /* check buffer remaining length */
    ist_string_buffer_ensure(this, *_indexv, _length + 1);

    /* append string */
    memcpy((*this) + (*_indexv), _string, _length);
    (*_indexv)        += _length;
    (*this)[*_indexv]  = '\0';

    return this;
}
inline ist_string* ist_string_buffer_append_raw(
    ist_string* this,
    ist_usize*  _indexv,
    ist_cstring _string
) {
    return ist_string_buffer_append_ref(this, _indexv, (ist_string)_string, strlen(_string));
}
inline ist_string* ist_string_buffer_append_raws(
    ist_string* this,
    ist_usize*  _indexv,
    ist_cstring _string,
    ...
) {
    va_list args;
    va_start(args, _string);
    for (ist_string cat_string = _string; cat_string; cat_string = va_arg(args, ist_string))
        ist_string_buffer_append_raw(this, _indexv, cat_string);
    va_end(args);
    return this;
}
