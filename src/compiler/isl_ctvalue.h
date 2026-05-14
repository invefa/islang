#ifndef ISC_CTVALUE_H
#define ISC_CTVALUE_H

#include "isl_dump.h"
#include "isl_string.h"
#include "isl_types.h"

/**
 * islang compile-time value
 * Why separate the value into comptime and runtime?
 * Because
 */

typedef ist_i8     ist_ctvalueAs_i8;
typedef ist_u8     ist_ctvalueAs_u8;
typedef ist_i16    ist_ctvalueAs_i16;
typedef ist_u16    ist_ctvalueAs_u16;
typedef ist_i32    ist_ctvalueAs_i32;
typedef ist_u32    ist_ctvalueAs_u32;
typedef ist_i64    ist_ctvalueAs_i64;
typedef ist_u64    ist_ctvalueAs_u64;
typedef ist_f32    ist_ctvalueAs_f32;
typedef ist_f64    ist_ctvalueAs_f64;
typedef ist_bool   ist_ctvalueAs_bool;
typedef ist_usize  ist_ctvalueAs_usize;
typedef ist_strbuf ist_ctvalueAs_strbuf;
typedef ist_string ist_ctvalueAs_str;

typedef union ist_ctvalueAs {
    ist_ctvalueAs_i8  i8;
    ist_ctvalueAs_u8  u8;
    ist_ctvalueAs_i16 i16;
    ist_ctvalueAs_u16 u16;
    ist_ctvalueAs_i32 i32;
    ist_ctvalueAs_u32 u32;
    ist_ctvalueAs_i64 i64;
    ist_ctvalueAs_u64 u64;
    ist_ctvalueAs_f32 f32;
    ist_ctvalueAs_f64 f64;
    ist_ctvalueAs_bool bool;
    ist_ctvalueAs_usize  usize;
    ist_ctvalueAs_strbuf strbuf;
    ist_ctvalueAs_str    str;
} ist_ctvalueAs;

typedef enum ist_ctvalueType {
    isn_ctvalueType_void,
    isn_ctvalueType_i8,
    isn_ctvalueType_u8,
    isn_ctvalueType_i16,
    isn_ctvalueType_u16,
    isn_ctvalueType_i32,
    isn_ctvalueType_u32,
    isn_ctvalueType_i64,
    isn_ctvalueType_u64,
    isn_ctvalueType_f32,
    isn_ctvalueType_f64,
    isn_ctvalueType_bool,
    isn_ctvalueType_usize,
    isn_ctvalueType_strbuf,
    isn_ctvalueType_str,
} ist_ctvalueType;

extern ist_cstring ist_ctvalueTypeNames[];


typedef struct ist_ctvalue {
    ist_ctvalueType type;
    ist_ctvalueAs   as;
} ist_ctvalue;
#define ist_ctvalue_ (ist_ctvalue)

ist_string ist_ctvalue_dump(ist_ctvalue* this, ist_dumpctx dctx);


#define ISG_VALUE_TYPE ist_ctvalue
#include "isg_list_head.h"


#endif // ISC_CTVALUE_H