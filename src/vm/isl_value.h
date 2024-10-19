#ifndef ISC_VALUE_H
#define ISC_VALUE_H

#include "isl_string.h"
#include "isl_types.h"


typedef union ist_value {
    ist_i64     int_value;
    ist_u64     uint_value;
    ist_f64     real_value;
    ist_byte    byte_value;
    ist_bool    bool_value;
    ist_string* strbuf;
} ist_value;

#define ist_value_consby_null()          ((ist_value){.int_value = 0})
#define ist_value_consby_i64(_i64)       ((ist_value){.int_value = (_i64)})
#define ist_value_consby_u64(_u64)       ((ist_value){.uint_value = (_u64)})
#define ist_value_consby_double(_double) ((ist_value){.real_value = (_double)})
#define ist_value_consby_bool(_bool)     ((ist_value){.bool_value = (_bool)})
#define ist_value_consby_byte(_byte)     ((ist_value){.byte_value = (_byte)})
#define ist_value_consby_strbuf(_strbuf) ((ist_value){.strbuf = (_strbuf)})

#endif // ISC_VALUE_H