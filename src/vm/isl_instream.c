#include "isl_instream.h"

#define ISG_VALUE_TYPE  ist_instruction
#define ISG_STRUCT_NAME ist_instream
#define ISG_VALUE_DONT_CLEAN
#include "isg_list_code.h"

inline void ist_instream_append_ins(ist_instream* this, ist_instruction ins) {
    ist_instream_addc(this, ins);
}

inline void ist_instream_append_i8(ist_instream* this, ist_i8 val) {
    ist_instream_ensurc(this, sizeof(ist_i8));
    0 [(ist_i8*)(this->data + this->size)] = val, this->size += sizeof(ist_i8);
}
inline void ist_instream_append_i16(ist_instream* this, ist_i16 val) {
    ist_instream_ensurc(this, sizeof(ist_i16));
    0 [(ist_i16*)(this->data + this->size)] = val, this->size += sizeof(ist_i16);
}
inline void ist_instream_append_i32(ist_instream* this, ist_i32 val) {
    ist_instream_ensurc(this, sizeof(ist_i32));
    0 [(ist_i32*)(this->data + this->size)] = val, this->size += sizeof(ist_i32);
}
inline void ist_instream_append_i64(ist_instream* this, ist_i64 val) {
    ist_instream_ensurc(this, sizeof(ist_i64));
    0 [(ist_i64*)(this->data + this->size)] = val, this->size += sizeof(ist_i64);
}

inline void ist_instream_append_u8(ist_instream* this, ist_u8 val) {
    ist_instream_ensurc(this, sizeof(ist_u8));
    0 [(ist_u8*)(this->data + this->size)] = val, this->size += sizeof(ist_u8);
}
inline void ist_instream_append_u16(ist_instream* this, ist_u16 val) {
    ist_instream_ensurc(this, sizeof(ist_u16));
    0 [(ist_u16*)(this->data + this->size)] = val, this->size += sizeof(ist_u16);
}
inline void ist_instream_append_u32(ist_instream* this, ist_u32 val) {
    ist_instream_ensurc(this, sizeof(ist_u32));
    0 [(ist_u32*)(this->data + this->size)] = val, this->size += sizeof(ist_u32);
}
inline void ist_instream_append_u64(ist_instream* this, ist_u64 val) {
    ist_instream_ensurc(this, sizeof(ist_u64));
    0 [(ist_u64*)(this->data + this->size)] = val, this->size += sizeof(ist_u64);
}

inline void ist_instream_append_f32(ist_instream* this, ist_f32 val) {
    ist_instream_ensurc(this, sizeof(ist_f32));
    0 [(ist_f32*)(this->data + this->size)] = val, this->size += sizeof(ist_f32);
}
inline void ist_instream_append_f64(ist_instream* this, ist_f64 val) {
    ist_instream_ensurc(this, sizeof(ist_f64));
    0 [(ist_f64*)(this->data + this->size)] = val, this->size += sizeof(ist_f64);
}


ist_string ist_instream_dumpack_dump(ist_instream_dumpack* this, ist_dumpctx dctx) {
    dctx.idxptr = dctx.idxptr ?: (ist_usize[1]){};

    ist_instream* instream = this->instream;
    while (this->index < instream->size) {
        ist_instruction inst     = instream->data[this->index++];
        ist_instruction argcount = ist_instruction_argcounts[inst];
        ist_strbuf_append_raw(dctx.buffer, dctx.idxptr, ist_instruction_names[inst]);
        if (argcount) {
            ist_strbuf_append_raw(dctx.buffer, dctx.idxptr, " ");
            for (ist_u8 i = 0; i < argcount; ++i) {
                if (i) ist_strbuf_append_raw(dctx.buffer, dctx.idxptr, ", ");
                ist_instargtype argtype = ist_instruction_argtypes[inst][i];
                ist_instargtype_dumpack_dump(
                    &ist_instargtype_dumpack_{
                        argtype,
                        instream->data + this->index,
                    },
                    dctx
                );
                this->index += ist_instargtype_sizeof[argtype];
            }
        }
        ist_strbuf_append_raw(dctx.buffer, dctx.idxptr, "\n");
    }

    return *dctx.buffer;
}
