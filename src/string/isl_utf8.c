#include "isl_utf8.h"

#include "isl_xssert.h"
#include "isl_list.h"

ist_u8 isl_utf8_codepoint_encode_length(ist_i32 _codepoint) {
    isl_assert(_codepoint > 0, "can't encode negative codepoint.");
    if (_codepoint <= 0x7F) return 1;
    else if (_codepoint <= 0x7FF)  return 2;
    else if (_codepoint <= 0xFFFF) return 3;
    else if (_codepoint <= 0x10FFFF) return 4;
    else return 0;
}

ist_u8 isl_utf8_encode(ist_i32 _codepoint, ist_string* _buffer, ist_usize _index) {
    isl_assert(_buffer && (*_buffer));

    ist_usize buffer_length = isl_list_catch_length(*_buffer);
    ist_u8 codepoint_encode_length = isl_utf8_codepoint_encode_length(_codepoint);

    ist_string_buffer_ensure(_buffer, _index, codepoint_encode_length);

    ist_byte* buffer = *_buffer + _index;
    switch (codepoint_encode_length) {
    case 4:
        buffer[0] = 0xF0 | ((_codepoint >> 18) & 0x07);
        buffer[1] = 0x80 | ((_codepoint >> 12) & 0x3F);
        buffer[2] = 0x80 | ((_codepoint >> 6) & 0x3F);
        buffer[3] = 0x80 | (_codepoint & 0x3F);
        break;
    case 3:
        buffer[0] = 0xE0 | ((_codepoint >> 12) & 0x0F);
        buffer[1] = 0x80 | ((_codepoint >> 6) & 0x3F);
        buffer[2] = 0x80 | (_codepoint & 0x3F);
        break;
    case 2:
        buffer[0] = 0b11000000 | ((_codepoint >> 6) & 0x1F);
        buffer[1] = 0x80 | (_codepoint & 0x3F);
        break;
    case 1:
        buffer[0] = _codepoint & 0x7F;
        break;
    default:
        isl_assert(0, "WTF.");
    }

    return codepoint_encode_length;

}

ist_u8 isl_utf8_decode_length(ist_string* _buffer, ist_usize _index) {
    if ((_buffer[0][_index] & 0xC0) == 0x80) return 0;
    else if ((_buffer[0][_index] & 0xF8) == 0xF0) return 4;
    else if ((_buffer[0][_index] & 0xF0) == 0xE0) return 3;
    else if ((_buffer[0][_index] & 0xE0) == 0xC0) return 2;
    else return 1;
}

ist_i32 isl_utf8_decode(ist_string* _buffer, ist_usize* _index) {
    ist_i32 result = 0;
    ist_byte* buffer = *_buffer + *_index;
    ist_u8 utf8_decode_length = isl_utf8_decode_length(_buffer, *_index);
    ist_string_buffer_ensure(_buffer, *_index, utf8_decode_length);
    *_index += utf8_decode_length;
    switch (utf8_decode_length) {
    case 4:
        result =
            ((buffer[0] & 0x07) << 18)
            | ((buffer[1] & 0x3F) << 12)
            | ((buffer[2] & 0x3F) << 6)
            | (buffer[3] & 0x3F);
        break;
    case 3:
        result =
            ((buffer[0] & 0x0F) << 12)
            | ((buffer[1] & 0x3F) << 6)
            | (buffer[2] & 0x3F);
        break;
    case 2:
        result =
            ((buffer[0] & 0x1F) << 6)
            | (buffer[1] & 0x3F);
        break;
    case 1:
        result =
            buffer[0] & 0x7F;
        break;
    default:
        isl_assert(0, "WTF.");
    }

    return result;

}
