#include "isl_value.h"

#include <inttypes.h>

#include "isl_string.h"
#include "isl_token.h"

ist_cstring ist_valueTypeNames[] = {
    [isn_valueType_unk]    = "unk",
    [isn_valueType_i8]     = "i8",
    [isn_valueType_u8]     = "u8",
    [isn_valueType_i16]    = "i16",
    [isn_valueType_u16]    = "u16",
    [isn_valueType_i32]    = "i32",
    [isn_valueType_u32]    = "u32",
    [isn_valueType_i64]    = "i64",
    [isn_valueType_u64]    = "u64",
    [isn_valueType_f32]    = "f32",
    [isn_valueType_f64]    = "f64",
    [isn_valueType_bool]   = "bool",
    [isn_valueType_usize]  = "usize",
    [isn_valueType_strbuf] = "strbuf",
    [isn_valueType_str]    = "str",
};

const isn_valueType isl_toklitype_to_valueType[] = {
    [ISL_TOKENT_VL_INT]    = isn_valueType_i64,
    [ISL_TOKENT_VL_REAL]   = isn_valueType_f64,
    [ISL_TOKENT_VL_STRING] = isn_valueType_str,
};

ist_u8 isn_valueType_sizeof(isn_valueType v) {
    _isn_valueTypeCons kind = v & isn_valueTypeMask_kind;
    _isn_valueTypeCons size = v & isn_valueTypeMask_size;
    if (kind == isn_valueTypeKind_p) size = isn_valueTypeSize_arch;
    if (size == isn_valueTypeSize_arch) size = isn_valueTypeSize_64;
    return size;
}



ist_string ist_value_dump_old(
    ist_value*    this,
    ist_tokenType type,
    ist_string*   buffer,
    ist_usize*    idxptr
) {
    isl_ifnreport(this, rid_catch_nullptr, isp_catch_coreloc);

    switch (type) {
        case ISL_TOKENT_VL_INT:
            return ist_strbuf_sprintf(buffer, idxptr, "%" PRId64, this->as.i64);
        case ISL_TOKENT_VL_REAL:
            return ist_strbuf_sprintf(buffer, idxptr, "%g", this->as.f64);
        case ISL_TOKENT_VL_STRING:
            return ist_strbuf_sprintf(buffer, idxptr, "\"%s\"", this->as.str);
        default:
            return ist_strbuf_sprintf(buffer, idxptr, "null");
    }
}

ist_string ist_tvalue_dump(ist_tvalue* this, ist_dumpctx dctx) {
    isl_ifnreport(this, rid_catch_nullptr, isp_catch_coreloc);

    switch (this->type) {
        case isn_valueType_unk:
            return ist_strbuf_append_raw(dctx.buffer, dctx.idxptr, "unk");
        case isn_valueType_i8:
            return ist_strbuf_sprintf(dctx.buffer, dctx.idxptr, "%" PRId8, this->as.i8);
        case isn_valueType_i16:
            return ist_strbuf_sprintf(dctx.buffer, dctx.idxptr, "%" PRId16, this->as.i16);
        case isn_valueType_i32:
            return ist_strbuf_sprintf(dctx.buffer, dctx.idxptr, "%" PRId32, this->as.i32);
        case isn_valueType_i64:
            return ist_strbuf_sprintf(dctx.buffer, dctx.idxptr, "%" PRId64, this->as.i64);
        case isn_valueType_u8:
            return ist_strbuf_sprintf(dctx.buffer, dctx.idxptr, "%" PRIu8, this->as.u8);
        case isn_valueType_u16:
            return ist_strbuf_sprintf(dctx.buffer, dctx.idxptr, "%" PRIu16, this->as.u16);
        case isn_valueType_u32:
            return ist_strbuf_sprintf(dctx.buffer, dctx.idxptr, "%" PRIu32, this->as.u32);
        case isn_valueType_u64:
            return ist_strbuf_sprintf(dctx.buffer, dctx.idxptr, "%" PRIu64, this->as.u64);
        case isn_valueType_f32:
            return ist_strbuf_sprintf(dctx.buffer, dctx.idxptr, "%f", this->as.f32);
        case isn_valueType_f64:
            return ist_strbuf_sprintf(dctx.buffer, dctx.idxptr, "%g", this->as.f64);
        case isn_valueType_str:
            return ist_strbuf_sprintf(dctx.buffer, dctx.idxptr, "\"%s\"", this->as.str);
        default:
            return ist_strbuf_append_raw(dctx.buffer, dctx.idxptr, "(unknown)");
    }
}

#define ISG_VALUE_DONT_CLEAN
#define ISG_VALUE_TYPE ist_value
#include "isg_list_code.h"
#define ISG_VALUE_DONT_CLEAN
#define ISG_VALUE_TYPE ist_value
#include "isg_stack_code.h"
