#ifndef ISC_COMPENT_BASETYPE_H
#define ISC_COMPENT_BASETYPE_H

#include "isl_value.h"


/**
 * `iscn_basetype` is a mapping for `isn_valueType`,
 * ISlang_CompEnt_sTruct::basetype  => isct_basetype
 * ISlang_CompEnt_eNum::basetype::X => iscn_basetype_X
 */


typedef void isct_basetype_void;

typedef ist_valueAs_bool isct_basetype_bool;

typedef ist_valueAs_i8  isct_basetype_i8;
typedef ist_valueAs_i16 isct_basetype_i16;
typedef ist_valueAs_i32 isct_basetype_i32;
typedef ist_valueAs_i64 isct_basetype_i64;

typedef ist_valueAs_u8    isct_basetype_u8;
typedef ist_valueAs_u16   isct_basetype_u16;
typedef ist_valueAs_u32   isct_basetype_u32;
typedef ist_valueAs_u64   isct_basetype_u64;
typedef ist_valueAs_usize isct_basetype_usize;

typedef ist_valueAs_f32 isct_basetype_f32;
typedef ist_valueAs_f64 isct_basetype_f64;

typedef ist_valueAs_str isct_basetype_str;
typedef ist_valueAs_adr isct_basetype_adr;


typedef enum _iscn_basetypeCons {

    iscn_basetypeMask_kind = isn_valueTypeMask_kind,
    iscn_basetypeMask_size = isn_valueTypeMask_size,

    iscn_basetypeSize_0    = isn_valueTypeSize_0,
    iscn_basetypeSize_1    = isn_valueTypeSize_8,
    iscn_basetypeSize_2    = isn_valueTypeSize_16,
    iscn_basetypeSize_4    = isn_valueTypeSize_32,
    iscn_basetypeSize_8    = isn_valueTypeSize_64,
    iscn_basetypeSize_arch = isn_valueTypeSize_arch,

    iscn_basetypeKind_v = isn_valueTypeKind_v,
    iscn_basetypeKind_i = isn_valueTypeKind_i,
    iscn_basetypeKind_u = isn_valueTypeKind_u,
    iscn_basetypeKind_f = isn_valueTypeKind_f,
    iscn_basetypeKind_p = isn_valueTypeKind_p,

} _iscn_basetypeCons;
typedef enum iscn_basetype {
    iscn_basetype_unk = isn_valueType_unk,

    iscn_basetype_void = iscn_basetypeKind_v | iscn_basetypeSize_0,

    iscn_basetype_bool = isn_valueType_bool,

    iscn_basetype_i8    = isn_valueType_i8,
    iscn_basetype_i16   = isn_valueType_i16,
    iscn_basetype_i32   = isn_valueType_i32,
    iscn_basetype_i64   = isn_valueType_i64,
    iscn_basetype_isize = isn_valueType_isize,

    iscn_basetype_u8    = isn_valueType_u8,
    iscn_basetype_u16   = isn_valueType_u16,
    iscn_basetype_u32   = isn_valueType_u32,
    iscn_basetype_u64   = isn_valueType_u64,
    iscn_basetype_usize = isn_valueType_usize,

    iscn_basetype_f32 = isn_valueType_f32,
    iscn_basetype_f64 = isn_valueType_f64,

    iscn_basetype_str = isn_valueType_str,
    iscn_basetype_adr = isn_valueType_adr,

} iscn_basetype;

isn_valueType iscn_basetype_to_isn_valueType(iscn_basetype v);
ist_u8        iscn_basetype_sizeof(iscn_basetype v);



#endif // ISC_COMPENT_BASETYPE_H