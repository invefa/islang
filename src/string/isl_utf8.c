#include "isl_utf8.h"

#include "isl_list.h"
#include "isl_report.h"
#include "isl_xssert.h"


inline ist_u8 isl_utf8_encode_length(ist_codepoint _codepoint) {
    isl_ifreport(_codepoint < 0, rid_utf8_negative_codepoint, isp_catch_coreloc);

    if (_codepoint <= 0x7F) return 1;
    else if (_codepoint <= 0x7FF) return 2;
    else if (_codepoint <= 0xFFFF) return 3;
    else if (_codepoint <= 0x10FFFF) return 4;
    else return 0;
}

inline ist_u8 isl_utf8_encode(ist_codepoint _codepoint, ist_string* _buffer, ist_usize _index) {
    isl_assert(_buffer);
    isl_assert(_buffer[0]);

    ist_u8 codepoint_encode_length = isl_utf8_encode_length(_codepoint);

    ist_strbuf_ensure(_buffer, _index, codepoint_encode_length);

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
            isl_report(rid_illegal_utf8_codepoint, isp_catch_coreloc);
    }

    return codepoint_encode_length;
}

inline ist_u8 isl_utf8_decode_length(ist_string* _buffer, ist_usize _index) {
    isl_assert(_buffer);
    isl_assert(_buffer[0]);

    if ((_buffer[0][_index] & 0xC0) == 0x80) return 0;
    else if ((_buffer[0][_index] & 0xF8) == 0xF0) return 4;
    else if ((_buffer[0][_index] & 0xF0) == 0xE0) return 3;
    else if ((_buffer[0][_index] & 0xE0) == 0xC0) return 2;
    else return 1;
}

inline ist_codepoint isl_utf8_decode(
    ist_string* _buffer,
    ist_usize   _index,
    ist_u8*     _decode_length_stv
) {
    isl_assert(_decode_length_stv, "decode length storage variable must be non-null.");

    ist_codepoint result = ISL_CODEPOINT_ERROR;
    *_decode_length_stv  = isl_utf8_decode_length(_buffer, _index);

    isl_ifreport(
        isl_list_catch_length(*_buffer) - _index < *_decode_length_stv,
        rid_incomplete_utf8_sequence,
        isp_catch_coreloc
    );

    // NOTICE: the following code is not safe, but it is not critical.
    ist_byte* buffer = *_buffer + _index;
    switch (*_decode_length_stv) {
        case 4:
            result = ((buffer[0] & 0x07) << 18) | ((buffer[1] & 0x3F) << 12)
                   | ((buffer[2] & 0x3F) << 6) | (buffer[3] & 0x3F);
            break;
        case 3:
            result = ((buffer[0] & 0x0F) << 12) | ((buffer[1] & 0x3F) << 6) | (buffer[2] & 0x3F);
            break;
        case 2:
            result = ((buffer[0] & 0x1F) << 6) | (buffer[1] & 0x3F);
            break;
        case 1:
            result = buffer[0] & 0x7F;
            break;
        default:
            isl_report(rid_illegal_utf8_sequence, isp_catch_coreloc);
    }

    return result;
}

inline ist_bool isl_utf8_legal_identifier_codepoint(ist_codepoint _codepoint, ist_bool _as_first) {
    if (_codepoint == ISL_CODEPOINT_ERROR) return false;
    if (_as_first && (0x0030 <= _codepoint && _codepoint <= 0x0039)) return false; // 0-9
    return _codepoint == 0x005F                                                    // UNDERLINE
        || (0x0041 <= _codepoint && _codepoint <= 0x005A)                          // A-Z
        || (0x0061 <= _codepoint && _codepoint <= 0x007A)                          // a-z
        || (0x4E00 <= _codepoint && _codepoint <= 0x9FFF)                          // CJK
        || (0x0391 <= _codepoint && _codepoint <= 0x03A9)                          // GREEK
        || (0x03B1 <= _codepoint && _codepoint <= 0x03C9)                          // GREEK
        || (0x0410 <= _codepoint && _codepoint <= 0x044F)                          // CYRILLIC
        || (0x2160 <= _codepoint && _codepoint <= 0x2188)                          // ARMENIAN
        || (0x0030 <= _codepoint && _codepoint <= 0x0039);                         // 0-9
}

inline ist_bool isl_utf8_is_space_codepoint(ist_codepoint _codepoint) {
    return _codepoint == 0x0020 || _codepoint == 0x0009 || _codepoint == 0x000A
        || _codepoint == 0x000B || _codepoint == 0x000C || _codepoint == 0x000D;
}

inline ist_bool isl_utf8_is_digit_codepoint(ist_codepoint _codepoint) {
    return 0x0030 <= _codepoint && _codepoint <= 0x0039;
}
