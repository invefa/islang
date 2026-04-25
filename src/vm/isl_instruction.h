#ifndef ISC_INSTRUCTION_H
#define ISC_INSTRUCTION_H
#include "isl_dump.h"
#include "isl_string.h"
#include "isl_types.h"


typedef ist_u8 ist_instargtype;
enum ist_instargtype {
    ist_instargtype_void,
    ist_instargtype_i8,
    ist_instargtype_i16,
    ist_instargtype_i32,
    ist_instargtype_i64,
    ist_instargtype_u8,
    ist_instargtype_u16,
    ist_instargtype_u32,
    ist_instargtype_u64,
    ist_instargtype_f32,
    ist_instargtype_f64,
};

extern const ist_u8 ist_instargtype_sizeof[];

typedef struct ist_instargtype_dumpack {
    ist_instargtype type;
    ist_vptr        value;
} ist_instargtype_dumpack;
#define ist_instargtype_dumpack_ (ist_instargtype_dumpack)

ist_string ist_instargtype_dumpack_dump(ist_instargtype_dumpack* this, ist_dumpctx dctx);

/**
 * instruction take one byte
 */
typedef ist_u8 ist_instruction;

enum ist_instruction {
#define manifest(_name, _argtypes, _effect) ist_inst_##_name,
#include "isl_instructions.h"
#undef manifest
};

extern const ist_cstring     ist_instruction_names[];
extern const ist_u8          ist_instruction_argcounts[];
extern const ist_i8          ist_instruction_effects[];
extern const ist_instargtype ist_instruction_argtypes[][3];

#endif // ISC_INSTRUCTION_H