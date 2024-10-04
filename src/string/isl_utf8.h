#ifndef ISL_UTF8_H
#define ISL_UTF8_H

#include "isl_types.h"
#include "isl_string.h"

typedef ist_i32 ist_codepoint;

/* when decode failed, return this value */
#define ISL_CODEPOINT_ERROR ((ist_codepoint)(-1))

/*
    the special codepoint flag that means End-Of-Current-CodePage,
    it serves for the lexer to switch to the previous codepage
    when we reach end of current codepage.
*/
#define ISL_CODEPOINT_EOCCP ((ist_codepoint)(0x24AD6F05))

ist_u8          isl_utf8_encode_length(ist_codepoint _codepoint);
ist_u8          isl_utf8_decode_length(ist_string* _buffer, ist_usize _index);
ist_u8          isl_utf8_encode(ist_codepoint _codepoint, ist_string* _buffer, ist_usize _index);
ist_codepoint   isl_utf8_decode(ist_string* _buffer, ist_usize _index, ist_u8* _decode_length_stv);
ist_bool        isl_utf8_legal_identifier_codepoint(ist_codepoint _codepoint, ist_bool _as_first);

#endif