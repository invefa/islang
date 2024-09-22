#include "isl_string.h"

#include <string.h>

#include "isl_memgr.h"
#include "isl_list.h"
#include "isl_report.h"

ist_string* ist_string_create(char* _cstring, ist_usize _length) {
    ist_string* string = isl_malloc(ist_string);
    ist_string_init(string, _cstring, _length);
    return string;
}

ist_string* ist_string_create_buffer(ist_usize _capacity) {
    ist_string* string = isl_malloc(ist_string);
    isl_ifnreport(_capacity, rid_catch_zero_capacity_buffer, isp_catch_coreloc, string);
    *string = isl_calloc_list(ist_byte, _capacity);
    return string;
}

void ist_string_init(ist_string* _string, char* _cstring, ist_usize _length) {
    *_string = isl_calloc_list(ist_byte, _length + 1);
    isl_ifnreport(_length, rid_catch_zero_string_length, isp_catch_coreloc, _string);
    memcpy(*_string, _cstring, _length);
    (*_string)[_length] = '\0';
}

void ist_string_clean(ist_string* _string) {
    isl_ifnreport(_string, rid_catch_nullptr, isp_catch_coreloc);
    if (*_string)
        isl_freev_list(*_string);
    *_string = NULL;
}

void ist_string_delete(ist_string* _string) {
    isl_ifnreport(_string, rid_catch_nullptr, isp_catch_coreloc);
    ist_string_clean(_string);
    isl_free(_string);
}

void ist_string_buffer_ensure(ist_string* _buffer, ist_usize _buffer_size, ist_usize _required_length) {

    if (isl_list_catch_length(*_buffer) - _buffer_size >= _required_length) {
        return;
    }

    isl_report(
                rid_inform_buffer_reiszed,
                _buffer,
                isl_list_catch_length(*_buffer),
                isl_list_catch_length(*_buffer) - _buffer_size,
                _required_length,
                ceil_upon_powertwo(_buffer_size + _required_length)
    );

    isl_list_resizec(*_buffer, ceil_upon_powertwo(_buffer_size + _required_length));
}

