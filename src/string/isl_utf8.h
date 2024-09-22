#ifndef ISL_UTF8_H
#define ISL_UTF8_H

#include "isl_types.h"
#include "isl_string.h"

ist_u8  isl_utf8_encode_length(ist_codepoint _codepoint);
ist_u8  isl_utf8_decode_length(ist_string* _buffer, ist_usize _index);

ist_u8          isl_utf8_encode(ist_codepoint _codepoint, ist_string* _buffer, ist_usize _index);
ist_codepoint   isl_utf8_decode(ist_string* _buffer, ist_usize _index, ist_u8* _decode_length_stv);
ist_codepoint   isl_utf8_decode_and_advance(ist_string* _buffer, ist_usize* _index);

ist_bool isl_utf8_legal_varname_codepoint(ist_codepoint _codepoint, ist_bool _as_first);

#endif