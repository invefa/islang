#include "isl_dbgutils.h"
#include "isl_list.h"
#include "isl_report.h"
#include "isl_string.h"


#define ISL_TEST_ITEM string
#include "isg_test_defines.h"
ISL_TEST_HEAD {

    ist_string* str1 = ist_string_createby_ref(u8"abcdef", 6);
    ist_string* str2 = ist_string_createby_ref(u8"world", 5);

    ist_string str3;
    ist_string_initby_ref(&str3, u8"hello", 5);

    printf("str1 = %s\n", *str1);
    printf("str2 = %s\n", *str2);
    printf("str3 = %s\n", str3);

    ist_string* buffer = ist_string_create_buffer(0);
    ist_usize   index  = 0;
    for (ist_usize i = 0; i < 128; ++i) {
        index += isl_utf8_encode(0x6C49, buffer, index);
    }

    index += isl_utf8_encode('a', buffer, index);
    index += isl_utf8_encode('b', buffer, index);
    index += isl_utf8_encode('c', buffer, index);
    index += isl_utf8_encode('d', buffer, index);

    ist_string* tmp_buffer = ist_string_create_buffer(1);

    isl_report(rid_custom_core_error, "for loop.");
    for (ist_usize i = 0; i < isl_list_catch_length(*buffer); ++i) {
        u8_to_string((*buffer)[i], tmp_buffer, 16);
        if (i == 0) printf("encoded_utf8_sequence = {");
        printf("0x%s,", *tmp_buffer);
    }
    printf("\b}\n\n");

    isl_report(rid_custom_core_error, "isl_list_foreach loop.");
    isl_list_foreach (iterp, *buffer, idx, cap) {
        u8_to_string(*iterp, tmp_buffer, 16);
        if (!idx) printf("encoded_utf8_sequence[%zu] = {", cap);
        printf("0x%s,", *tmp_buffer);
    }
    printf("\b}\n\n");

    ist_u8 decode_length;
    for (ist_usize i = 0; i < isl_list_catch_length(*buffer); i += decode_length) {
        u32_to_string(isl_utf8_decode(buffer, i, &decode_length), tmp_buffer, 16);
        if (!i) printf("decoded_utf8_codepoints = {");
        printf("0x%s,", *tmp_buffer);
    }
    printf("\b}\n\n");

    ist_string* str4 = ist_string_createby_ref("汉", 5);
    printf("str4 context = %s\n", *str4);
    index = 0;
    u32_to_string(isl_utf8_decode(str4, index, &decode_length), tmp_buffer, 16);
    index += decode_length;
    printf("str4 codepoint = 0x%s\n", *tmp_buffer);

    ist_string* str5 = ist_string_createby_raw("just");
    index            = 4;

    ist_strbuf_append_raw(str5, &index, "udio");
    ist_strbuf_append_raw(str5, &index, " awa");

    printf("str5 = %s\n", *str5);
    printf("index = %zu\n", index);

    ist_string_delete(str1);
    ist_string_delete(str2);
    ist_string_clean(&str3);
    ist_string_delete(str4);
    ist_string_delete(str5);
    ist_string_delete(buffer);
    ist_string_delete(tmp_buffer);
}
ISL_TEST_TAIL
#include "isg_test_undefs.h"
