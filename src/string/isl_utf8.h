#ifndef ISL_UTF8_H
#define ISL_UTF8_H

#include "isl_types.h"
#include "isl_string.h"

ist_u8 isl_utf8_codepoint_encode_length(ist_i32 _codepoint);

ist_u8 isl_utf8_encode(ist_i32 _codepoint, ist_string* _buffer, ist_usize _index);

#endif