#include "isl_instruction.h"
#include "isl_overload.h"
#include "isl_report.h"


const ist_cstring ist_instruction_names[] = {
#define manifest(_name, _argtypes, _effect) [ist_inst_##_name] = #_name,
#include "isl_instructions.h"
#undef manifest
};

const ist_u8 ist_instruction_argcounts[] = {
#define manifest(_name, _argtypes, _effect) \
    [ist_inst_##_name] = __ISL_MACRO_GET_ARGS_COUNT _argtypes,
#include "isl_instructions.h"
#undef manifest
};

const ist_i8 ist_instruction_effects[] = {
#define manifest(_name, _argtypes, _effect) [ist_inst_##_name] = _effect,
#include "isl_instructions.h"
#undef manifest
};

#define _IST_INSTARGTYPE(_x) ist_instargtype_##_x
#define _IST_INSTARGTYPE_CONVERT_0()
#define _IST_INSTARGTYPE_CONVERT_1(_1)     _IST_INSTARGTYPE(_1)
#define _IST_INSTARGTYPE_CONVERT_2(_1, _2) _IST_INSTARGTYPE(_1), _IST_INSTARGTYPE(_2)
#define _IST_INSTARGTYPE_CONVERT_3(_1, _2, _3) \
    _IST_INSTARGTYPE(_1), _IST_INSTARGTYPE(_2), _IST_INSTARGTYPE(_3)
#define _IST_INSTARGTYPE_CONVERT(_vargs...) _isl_overload(_IST_INSTARGTYPE_CONVERT, ##_vargs)

const ist_instargtype ist_instruction_argtypes[][3] = {
#define manifest(_name, _argtypes, _effect) \
    [ist_inst_##_name] = {_IST_INSTARGTYPE_CONVERT _argtypes},
#include "isl_instructions.h"
#undef manifest
};

const ist_u8 ist_instargtype_sizeof[] = {
    [ist_instargtype_void] = 0,
    [ist_instargtype_i8]   = 1,
    [ist_instargtype_i16]  = 2,
    [ist_instargtype_i32]  = 4,
    [ist_instargtype_i64]  = 8,
    [ist_instargtype_u8]   = 1,
    [ist_instargtype_u16]  = 2,
    [ist_instargtype_u32]  = 4,
    [ist_instargtype_u64]  = 8,
    [ist_instargtype_f32]  = 4,
    [ist_instargtype_f64]  = 8,
};



ist_string ist_instargtype_dumpack_dump(ist_instargtype_dumpack* this, ist_dumpctx dctx) {
    dctx.idxptr = dctx.idxptr ?: (ist_usize[1]){};
    switch (this->type) {
        case ist_instargtype_i8:
            return ist_i8_dump(this->value, dctx);
        case ist_instargtype_i16:
            return ist_i16_dump(this->value, dctx);
        case ist_instargtype_i32:
            return ist_i32_dump(this->value, dctx);
        case ist_instargtype_i64:
            return ist_i64_dump(this->value, dctx);
        case ist_instargtype_u8:
            return ist_u8_dump(this->value, dctx);
        case ist_instargtype_u16:
            return ist_u16_dump(this->value, dctx);
        case ist_instargtype_u32:
            return ist_u32_dump(this->value, dctx);
        case ist_instargtype_u64:
            return ist_u64_dump(this->value, dctx);
        case ist_instargtype_f32:
            return ist_f32_dump(this->value, dctx);
        case ist_instargtype_f64:
            return ist_f64_dump(this->value, dctx);
        default:
            isp_unreachable();
    }
    return *dctx.buffer;
}