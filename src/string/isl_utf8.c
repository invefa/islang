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

    isl_wssert(
        buffer_length - _index >= codepoint_encode_length,
        "there has not enough place to encode the codepoint, we will resize it.",
        "remaining space length: %u, but require length: %u.",
        (buffer_length - _index, codepoint_encode_length));

    if (buffer_length - _index < codepoint_encode_length) {
        isl_list_resizec(*_buffer, ceil_upon_powertwo(codepoint_encode_length + _index));
    }

    ist_byte* buffer = *_buffer + _index;
    switch (codepoint_encode_length) {
    case 4:
        buffer[0] = 0b11110000 | ((_codepoint >> 18) & 0b00000111);
        buffer[1] = 0b10000000 | ((_codepoint >> 12) & 0b00111111);
        buffer[2] = 0b10000000 | ((_codepoint >> 6) & 0b00111111);
        buffer[3] = 0b10000000 | (_codepoint & 0x3F);
        break;
    case 3:
        buffer[0] = 0b11100000 | ((_codepoint >> 12) & 0b00001111);
        buffer[1] = 0b10000000 | ((_codepoint >> 6) & 0b00111111);
        buffer[2] = 0b10000000 | (_codepoint & 0b00111111);
        break;
    case 2:
        buffer[0] = 0b11000000 | ((_codepoint >> 6) & 0b00011111);
        buffer[1] = 0b10000000 | (_codepoint & 0b00111111);
        break;
    case 1:
        buffer[0] = _codepoint & 0x7F;
        break;
    default:
        isl_assert(0, "WTF.");
    }

    return codepoint_encode_length;

}
