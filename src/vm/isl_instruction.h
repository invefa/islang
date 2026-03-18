#ifndef ISC_INSTRUCTION_H
#define ISC_INSTRUCTION_H
#include "isl_types.h"


/**
 * instruction take one byte
 */
typedef ist_u8 ist_instruction;

enum ist_instruction {
#define manifest(_name, _argcount, _effect) ist_inst_##_name,
#include "isl_instructions.h"
#undef manifest
};

#endif // ISC_INSTRUCTION_H