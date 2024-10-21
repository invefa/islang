#ifndef ISC_VALUE_H
#define ISC_VALUE_H

#include "isl_string.h"
#include "isl_types.h"

/**
 * If you want to reduce the size of the ist_value, you can change below typedefs to the smaller
 * ones, make sure the size be same as another any one. why we set below typedefs? Because we want
 * to make a protocol for ist_value.
 */
typedef ist_i64     ist_value_int;
typedef ist_u64     ist_value_uint;
typedef ist_f64     ist_value_real;
typedef ist_byte    ist_value_byte;
typedef ist_bool    ist_value_bool;
typedef ist_string* ist_value_string;

typedef union ist_value {
    ist_value_int    int_value;
    ist_value_uint   uint_value;
    ist_value_real   real_value;
    ist_value_byte   byte_value;
    ist_value_bool   bool_value;
    ist_value_string string_value;
} ist_value;

#define ist_value_consby_null()          ((ist_value){.int_value = 0})
#define ist_value_consby_i64(_i64)       ((ist_value){.int_value = (_i64)})
#define ist_value_consby_u64(_u64)       ((ist_value){.uint_value = (_u64)})
#define ist_value_consby_double(_double) ((ist_value){.real_value = (_double)})
#define ist_value_consby_bool(_bool)     ((ist_value){.bool_value = (_bool)})
#define ist_value_consby_byte(_byte)     ((ist_value){.byte_value = (_byte)})
#define ist_value_consby_strbuf(_string) ((ist_value){.string_value = (_string)})

ist_string ist_value_dump_json(ist_value* this, ist_u32 type, ist_string* buffer);


#define ISG_VALUE_TYPE ist_value
#include "isg_list_head.h"



#endif // ISC_VALUE_H