#include "isl_string.h"

#include "isl_list.h"


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


inline void ist_strbuf_ensure(ist_string* this, ist_usize size, ist_usize _reqlen) {

    /* check nullptr */
    isl_assert(this);

    /* check buffer remaining length */
    if (isl_list_catch_length(*this) - size >= _reqlen) return;

    /* inform buffer will be reiszed */
    isl_report(
        rid_inform_buffer_reisze,
        this,
        isl_list_catch_length(*this),
        isl_list_catch_length(*this) - size,
        _reqlen,
        ceil_upon_powertwo(size + _reqlen)
    );

    /* resize buffer */
    isl_list_resizc(*this, ceil_upon_powertwo(size + _reqlen));
}


inline ist_string ist_strbuf_append_ref(
    ist_string* this,
    ist_usize*  idxptr,
    ist_cstring _string,
    ist_usize   _length
) {
    /* set the default value for idxptr if it was NULL */
    idxptr = idxptr ?: (ist_usize[1]){};

    /* check buffer remaining length */
    ist_strbuf_ensure(this, *idxptr, _length + 1);

    /* append string */
    memcpy((*this) + *idxptr, _string, _length);

    /* update the index and set the end of strbuf */
    *idxptr          += _length;
    (*this)[*idxptr]  = '\0';

    return *this;
}

inline ist_string ist_strbuf_append_raw(ist_string* this, ist_usize* idxptr, ist_cstring _string) {
    return ist_strbuf_append_ref(this, idxptr, (ist_string)_string, strlen(_string));
}

ist_string ist_strbuf_append_raws(ist_string* this, ist_usize* idxptr, ist_cstring _string, ...) {
    va_list args;
    va_start(args, _string);
    for (ist_string catstr = _string; catstr; catstr = va_arg(args, ist_string))
        ist_strbuf_append_raw(this, idxptr, catstr);
    va_end(args);
    return *this;
}

ist_string ist_strbuf_sprintf(ist_string* this, ist_usize* idxptr, ist_cstring _format, ...) {
    va_list args;

    /* set the default value for idxptr if it was NULL */
    idxptr = idxptr ?: (ist_usize[1]){};

    /**
     * check the length and ersure that the string buffer has enough space to
     * store the formatted string.
     */
    va_start(args, _format);
    ist_strbuf_ensure(this, *idxptr, vsnprintf(NULL, 0, _format, args) + 1);
    va_end(args);

    /* format the string */
    va_start(args, _format);
    *idxptr += vsprintf(*this + *idxptr, _format, args);
    va_end(args);

    return *this;
}