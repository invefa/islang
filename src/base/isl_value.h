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
typedef ist_i8     ist_valueAs_i8;
typedef ist_u8     ist_valueAs_u8;
typedef ist_i16    ist_valueAs_i16;
typedef ist_u16    ist_valueAs_u16;
typedef ist_i32    ist_valueAs_i32;
typedef ist_u32    ist_valueAs_u32;
typedef ist_i64    ist_valueAs_i64;
typedef ist_u64    ist_valueAs_u64;
typedef ist_f32    ist_valueAs_f32;
typedef ist_f64    ist_valueAs_f64;
typedef ist_bool   ist_valueAs_bool;
typedef ist_usize  ist_valueAs_usize;
typedef ist_strbuf ist_valueAs_strbuf;
typedef ist_string ist_valueAs_str;

typedef union ist_valueAs {
    ist_valueAs_i8  i8;
    ist_valueAs_i16 i16;
    ist_valueAs_i32 i32;
    ist_valueAs_i64 i64;

    ist_valueAs_u8    u8;
    ist_valueAs_u16   u16;
    ist_valueAs_u32   u32;
    ist_valueAs_u64   u64;
    ist_valueAs_usize usize;

    ist_valueAs_f32 f32;
    ist_valueAs_f64 f64;

    ist_valueAs_bool bool;

    ist_valueAs_str    str;
    ist_valueAs_strbuf strbuf;

} ist_valueAs;
#define ist_valueAs_ (ist_valueAs)

typedef enum ist_valueType {
    isn_valueType_unk,
    isn_valueType_void,

    isn_valueType_i8,
    isn_valueType_i16,
    isn_valueType_i32,
    isn_valueType_i64,

    isn_valueType_u8,
    isn_valueType_u16,
    isn_valueType_u32,
    isn_valueType_u64,
    isn_valueType_usize,

    isn_valueType_f32,
    isn_valueType_f64,

    isn_valueType_bool,

    isn_valueType_str,
    isn_valueType_strbuf,

} ist_valueType;

extern ist_cstring         ist_valueTypeNames[];
extern const ist_valueType isl_toklitype_to_valtype[];

typedef struct ist_tvalue {
    ist_valueType type;
    ist_valueAs   as;
} ist_tvalue;
#define ist_tvalue_ (ist_tvalue)

ist_string ist_tvalue_dump(ist_tvalue* this, ist_dumpctx dctx);

typedef struct ist_value {
    ist_valueAs as;
} ist_value;
#define ist_value_ (ist_value)

ist_string ist_value_dump_old(ist_value* this, ist_u32 type, ist_string* buffer, ist_usize* idxptr);


#define ISG_VALUE_TYPE ist_value
#include "isg_list_head.h"
#define ISG_VALUE_TYPE ist_value
#include "isg_stack_head.h"


#endif // ISC_VALUE_H