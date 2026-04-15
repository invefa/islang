#ifndef ISC_INSTREAM_H
#define ISC_INSTREAM_H
#include "isl_instruction.h"

#define ISG_VALUE_TYPE  ist_instruction
#define ISG_STRUCT_NAME ist_instream
#include "isg_list_head.h"

void ist_instream_append_ins(ist_instream* this, ist_instruction ins);

void ist_instream_append_i8(ist_instream* this, ist_i8 val);
void ist_instream_append_i16(ist_instream* this, ist_i16 val);
void ist_instream_append_i32(ist_instream* this, ist_i32 val);
void ist_instream_append_i64(ist_instream* this, ist_i64 val);

void ist_instream_append_u8(ist_instream* this, ist_u8 val);
void ist_instream_append_u16(ist_instream* this, ist_u16 val);
void ist_instream_append_u32(ist_instream* this, ist_u32 val);
void ist_instream_append_u64(ist_instream* this, ist_u64 val);

void ist_instream_append_f32(ist_instream* this, ist_f32 val);
void ist_instream_append_f64(ist_instream* this, ist_f64 val);


#endif // ISC_INSTREAM_H