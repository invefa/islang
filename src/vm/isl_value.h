#ifndef ISC_VALUE_H
#define ISC_VALUE_H

#include "isl_dump.h"
#include "isl_string.h"
#include "isl_types.h"

/**
 * If you want to reduce the size of the ist_value, you can change below typedefs to the smaller
 * ones, make sure the size be same as another any one. why we set below typedefs? Because we want
 * to make a protocol for ist_value.
 */
typedef ist_i8     ist_value_i8;
typedef ist_u8     ist_value_u8;
typedef ist_i16    ist_value_i16;
typedef ist_u16    ist_value_u16;
typedef ist_i32    ist_value_i32;
typedef ist_u32    ist_value_u32;
typedef ist_i64    ist_value_i64;
typedef ist_u64    ist_value_u64;
typedef ist_f32    ist_value_f32;
typedef ist_f64    ist_value_f64;
typedef ist_u64    ist_value_bool;
typedef ist_usize  ist_value_usize;
typedef ist_strbuf ist_value_strbuf;
typedef ist_string ist_value_str;

typedef union ist_value {
    ist_value_i8     as_i8;
    ist_value_u8     as_u8;
    ist_value_i16    as_i16;
    ist_value_u16    as_u16;
    ist_value_i32    as_i32;
    ist_value_u32    as_u32;
    ist_value_i64    as_i64;
    ist_value_u64    as_u64;
    ist_value_f32    as_f32;
    ist_value_f64    as_f64;
    ist_value_bool   as_bool;
    ist_value_usize  as_usize;
    ist_value_strbuf as_strbuf;
    ist_value_str    as_str;
} ist_value;

typedef enum ist_value_type {
    isl_valtype_void,
    isl_valtype_i8,
    isl_valtype_u8,
    isl_valtype_i16,
    isl_valtype_u16,
    isl_valtype_i32,
    isl_valtype_u32,
    isl_valtype_i64,
    isl_valtype_u64,
    isl_valtype_f32,
    isl_valtype_f64,
    isl_valtype_bool,
    isl_valtype_usize,
    isl_valtype_strbuf,
    isl_valtype_str,
} ist_value_type;

#define ist_value_cons                   (ist_value)
#define ist_value_consby_null()          ((ist_value){.as_i64 = 0})
#define ist_value_consby_i64(_i64)       ((ist_value){.as_i64 = (_i64)})
#define ist_value_consby_u64(_u64)       ((ist_value){.as_u64 = (_u64)})
#define ist_value_consby_f64(_f64)       ((ist_value){.as_f64 = (_f64)})
#define ist_value_consby_bool(_bool)     ((ist_value){.as_bool = (_bool)})
#define ist_value_consby_byte(_byte)     ((ist_value){.as_u8 = (_byte)})
#define ist_value_consby_usize(_usize)   ((ist_value){.as_usize = (_usize)})
#define ist_value_consby_strbuf(_strbuf) ((ist_value){.as_strbuf = (_strbuf)})
#define ist_value_consby_str(_string)    ((ist_value){.as_str = (_string)})

ist_string ist_value_dump_old(ist_value* this, ist_u32 type, ist_string* buffer, ist_usize* idxptr);

typedef struct ist_tvalue {
    ist_value_type type;
    ist_value      data;
} ist_tvalue;
#define ist_tvalue_ (ist_tvalue)

ist_string ist_tvalue_dump(ist_tvalue* this, ist_dumpctx dctx);

extern const ist_value_type isl_toklitype_to_valtype[];

#define ISG_VALUE_TYPE ist_value
#include "isg_list_head.h"
#define ISG_VALUE_TYPE ist_value
#include "isg_stack_head.h"


#endif // ISC_VALUE_H