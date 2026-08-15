#ifndef ISC_COMPENT_BASETYPE_H
#define ISC_COMPENT_BASETYPE_H

#include "isl_value.h"


/**
 * `iscen_basetype` is a mapping for `isn_valueType`,
 * ISlang_CompEnt_sTruct::basetype  => iscet_basetype
 * ISlang_CompEnt_eNum::basetype::X => iscen_basetype_X
 */


typedef void iscet_basetype_void;

typedef ist_valueAs_bool iscet_basetype_bool;

typedef ist_valueAs_i8  iscet_basetype_i8;
typedef ist_valueAs_i16 iscet_basetype_i16;
typedef ist_valueAs_i32 iscet_basetype_i32;
typedef ist_valueAs_i64 iscet_basetype_i64;

typedef ist_valueAs_u8    iscet_basetype_u8;
typedef ist_valueAs_u16   iscet_basetype_u16;
typedef ist_valueAs_u32   iscet_basetype_u32;
typedef ist_valueAs_u64   iscet_basetype_u64;
typedef ist_valueAs_usize iscet_basetype_usize;

typedef ist_valueAs_f32 iscet_basetype_f32;
typedef ist_valueAs_f64 iscet_basetype_f64;

typedef ist_valueAs_str iscet_basetype_str;
typedef ist_valueAs_adr iscet_basetype_adr;


typedef enum _iscen_basetypeCons {

    iscen_basetypeMask_kind = isn_valueTypeMask_kind,
    iscen_basetypeMask_size = isn_valueTypeMask_size,

    iscen_basetypeSize_0    = isn_valueTypeSize_0,
    iscen_basetypeSize_1    = isn_valueTypeSize_8,
    iscen_basetypeSize_2    = isn_valueTypeSize_16,
    iscen_basetypeSize_4    = isn_valueTypeSize_32,
    iscen_basetypeSize_8    = isn_valueTypeSize_64,
    iscen_basetypeSize_arch = isn_valueTypeSize_arch,

    iscen_basetypeKind_v = isn_valueTypeKind_v,
    iscen_basetypeKind_i = isn_valueTypeKind_i,
    iscen_basetypeKind_u = isn_valueTypeKind_u,
    iscen_basetypeKind_f = isn_valueTypeKind_f,
    iscen_basetypeKind_p = isn_valueTypeKind_p,

} _iscen_basetypeCons;
typedef enum iscen_basetype {
    iscen_basetype_unk = isn_valueType_unk,

    iscen_basetype_void = iscen_basetypeKind_v | iscen_basetypeSize_0,

    iscen_basetype_bool = isn_valueType_bool,

    iscen_basetype_i8    = isn_valueType_i8,
    iscen_basetype_i16   = isn_valueType_i16,
    iscen_basetype_i32   = isn_valueType_i32,
    iscen_basetype_i64   = isn_valueType_i64,
    iscen_basetype_isize = isn_valueType_isize,

    iscen_basetype_u8    = isn_valueType_u8,
    iscen_basetype_u16   = isn_valueType_u16,
    iscen_basetype_u32   = isn_valueType_u32,
    iscen_basetype_u64   = isn_valueType_u64,
    iscen_basetype_usize = isn_valueType_usize,

    iscen_basetype_f32 = isn_valueType_f32,
    iscen_basetype_f64 = isn_valueType_f64,

    iscen_basetype_str = isn_valueType_str,
    iscen_basetype_adr = isn_valueType_adr,

} iscen_basetype;

isn_valueType iscen_basetype_to_isn_valueType(iscen_basetype v);
ist_u8        iscen_basetype_sizeof(iscen_basetype v);



#endif // ISC_COMPENT_BASETYPE_H