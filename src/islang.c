/*
    This c source file is the main source file for islang and it is the entrance of islang.
    This project is compiled with -std=gnu11, because it will use some extension functions
    which are suplied by GNU standard extensions.

    There are a list of extensions are used:
        1. typeof keyword.
        2. u8 prefix for string literal.
    And some features are used:
        1. special macro expansion.

    Anyway, you had better compile this project with gcc -std=gnu11.
    Written by invefa.
*/

#include <stdio.h>
#include <stdlib.h>

#include "isl_xssert.h"
#include "isl_overload.h"
#include "isl_ansictrl.h"
#include "isl_memgr.h"
#include "isl_list.h"
#include "isl_string.h"
#include "isl_utf8.h"
#include "isl_dbgutils.h"
#include "isl_report.h"
#include "isl_lexer.h"

void isl_test_overload(void);
void isl_test_xssert(void);
void isl_test_list(void);
void isl_test_memgr(void);
void isl_test_string(void);
void isl_test_report(void);
void isl_test_lexer(void);

int main(void) {

    isl_test_overload();
    isl_test_xssert();
    isl_test_list();
    isl_test_memgr();
    isl_test_string();
    isl_test_report();
    isl_test_lexer();

    return 0;
}


void isl_test_lexer(void) {

    ist_string file = isl_read_file("./scripts/test.is");
    printf("file context:\n--->%s<---\n", file);
    ist_string_clean(&file);

    // ist_string* source = ist_string_createby_raw(u8"abc@..qwen2啊啊啊,qwq. 啊 .//123123\naabc 123/*qqweq\n123*/12.31.3 2h哈哈h");
    // ist_codepage* codepage = ist_codepage_createby_source(*source);
    // ist_lexer* lexer = ist_lexer_create(codepage);

    ist_lexer* lexer = ist_lexer_createby_file("./scripts/test.is");

    ist_string* token_dump_buffer = ist_string_create_buffer(ISL_DEFAULT_BUFFER_LENGTH);
    while (lexer->cur_token.type != ISL_TOKENT_EOF) {
        // ist_token_print(&lexer->cur_token);
        ist_token_dump(&lexer->cur_token, token_dump_buffer);
        printf("%s\n", *token_dump_buffer);
        ist_lexer_advance(lexer);
    }
    token_dump_buffer = ist_token_dump(&lexer->cur_token, token_dump_buffer);
    printf("%s\n", *token_dump_buffer);
    ist_string_delete(token_dump_buffer);

    ist_lexer_delete(lexer);
    // ist_string_delete(source);

}


// #define pal() printf("allocated length = %u\n", isl_allocated_length)

void isl_test_report(void) {

    /*  fprintf(stderr, ANSI_GRE_SET("this is a info    message: %d\n"), 123456);
        fprintf(stderr, ANSI_YEL_SET("this is a warning message: %d\n"), 123456);
        fprintf(stderr, ANSI_HIR_SET("this is a error   message: %d\n"), 123456);
        fprintf(stderr, ANSI_HIM_SET("this is a panic   message: %d\n"), 123456);
        fprintf(stderr, ANSI_RED_SET("this is a fatal   message: %d\n"), 123456); */

    isl_report(rid_unknown);
    isl_report(rid_custom_core_info, "the second arg was %d.", 123);
    isl_report(rid_custom_core_warn, "the second arg was %d.", 345);
    isl_report(rid_custom_core_error, "the second arg was %d.", 789);
    isl_report(rid_custom_core_panic, "the second arg was %d.", 666);
    // isl_report(rid_custom_core_fatal, "the second arg was %d.", 123456);

    isl_report(rid_utf8_negative_codepoint, isp_catch_coreloc);
    isl_utf8_encode_length(-22);
    isl_report(rid_unknown);
    // isl_ifnreport(NULL, rid_catch_nullptr, isp_catch_coreloc);


    isl_wssert(0);
}

void isl_test_string(void) {
    ist_string* str1 = ist_string_createby_ref(u8"abcdef", 6);
    ist_string* str2 = ist_string_createby_ref(u8"world", 5);

    ist_string str3;
    ist_string_init(&str3, u8"hello", 5);

    printf("str1 = %s\n", *str1);
    printf("str2 = %s\n", *str2);
    printf("str3 = %s\n", str3);

    ist_string* buffer = ist_string_create_buffer(0);
    ist_usize index = 0;
    for (ist_usize i = 0; i < 128; ++i) {
        index += isl_utf8_encode(0x6C49, buffer, index);
    }

    index += isl_utf8_encode('a', buffer, index);
    index += isl_utf8_encode('b', buffer, index);
    index += isl_utf8_encode('c', buffer, index);
    index += isl_utf8_encode('d', buffer, index);

    ist_string* tmp_buffer = ist_string_create_buffer(1);

    for (ist_usize i = 0;i < isl_list_catch_length(*buffer);++i) {
        u8_to_string((*buffer)[i], tmp_buffer, 16);
        if (i == 0) {
            printf("encoded_utf8_sequence = {");
            continue;
        }
        if (i == isl_list_catch_length(*buffer) - 1) {
            printf("0x%s", *tmp_buffer);
            break;
        }
        printf("0x%s,", *tmp_buffer);
    }
    printf("}\n\n");

    ist_u8 decode_length;
    index = 0;
    for (ist_usize i = 0;i < 132;++i) {
        u32_to_string(isl_utf8_decode(buffer, index, &decode_length), tmp_buffer, 16);
        index += decode_length;
        if (i == 0) {
            printf("decoded_utf8_codepoints = {");
            continue;
        }
        if (i == 131) {
            printf("0x%s", *tmp_buffer);
            break;
        }
        printf("0x%s,", *tmp_buffer);
    }
    printf("}\n\n");

    ist_string* str4 = ist_string_createby_ref("汉", 5);
    printf("str4 context = %s\n", *str4);
    index = 0;
    u32_to_string(isl_utf8_decode(str4, index, &decode_length), tmp_buffer, 16);
    index += decode_length;
    printf("str4 codepoint = 0x%s\n", *tmp_buffer);

    ist_string* str5 = ist_string_createby_ref("", 0);

    ist_string_delete(str1);
    ist_string_delete(str2);
    ist_string_clean(&str3);
    ist_string_delete(str4);
    ist_string_delete(str5);
    ist_string_delete(buffer);
    ist_string_delete(tmp_buffer);
}



typedef struct ist_i32_list {
    ist_i32* data;
    ist_usize size;
} ist_i32_list;



ist_i32* isl_emit_i32_list() {
    ist_i32* list = isl_malloc_list(ist_i32, 10);
    list[0] = 1, list[1] = 2, list[2] = 3, list[3] = 4, list[4] = 5;
    list[5] = 6, list[6] = 7, list[7] = 8, list[8] = 9, list[9] = 10;
    return list;
}

void isl_test_memgr(void) {
    printf("test for memgr\n");
    ist_i32* list = isl_calloc_list(ist_i32, 10);
    list[0] = 1, list[1] = 2, list[2] = 3, list[3] = 4, list[4] = 5;
    list[5] = 6, list[6] = 7, list[7] = 8, list[8] = 9, list[9] = 10;
    ist_i32_list* list2 = isl_calloc(ist_i32_list);
    list2->data = isl_calloc_list(ist_i32, 10);
    list2->data[0] = 1, list2->data[1] = 2, list2->data[2] = 3, list2->data[3] = 4, list2->data[4] = 5;
    list2->data[5] = 6, list2->data[6] = 7, list2->data[7] = 8, list2->data[8] = 9, list2->data[9] = 10;
    list2->size = 10;
    isl_freev_list(list);
    isl_freev_list(list2->data);
    isl_free(list2);
    list = isl_emit_i32_list();
    list2 = isl_calloc(ist_i32_list);
    isl_list_resizec(isl_emit_i32_list(), 20, list2->data);
    isl_list_resizec(list, 20);
    isl_free_list(list);
    isl_free_list(list2->data);
    isl_free(list2);
    isl_list_resizec(isl_emit_i32_list(), 100, list);
    isl_free_list(list);
}


void isl_test_list(void) {

    ist_i32* list = isl_calloc_list(ist_i32, 10);
    // ist_i32* new_list = NULL;
    printf("list = 0x%zX\n", (ist_usize)list);
    printf("length of list = %zu\n", isl_list_catch_length(list));
    printf("capacity of list = %zu\n", isl_list_ptr_get_capacity(list));
    list[0] = 1, list[1] = 2, list[2] = 3, list[3] = 4, list[4] = 5;
    list[5] = 6, list[6] = 7, list[7] = 8, list[8] = 9, list[9] = 10;

    isl_list_resizec(list, 20);
    isl_list_resizec(list, 30);
    isl_list_resizem(list, 6);
    isl_freev_list(list);
    isl_list_resizem(isl_emit_i32_list(), 5, list);
    isl_freev_list(list);

}


void isl_test_xssert(void) {
    /*
        __ISL_XSSERT(w, num != 1);
        __ISL_XSSERT(w, num != 1, "num should not be 1.");
        __ISL_XSSERT(w, num != 1, "num should not be 1.", "this is just a test.");
        __ISL_XSSERT(a, num != 1, "num should not be 1.", "num = %d, num + 1 = %d.", (num, num + 1));
    */

    int num = 1;
    isl_wssert(num != 1);
    isl_wssert(num != 1, "num should not be 1.");
    isl_wssert(num != 1, "num should not be 1.", "this is just a test.");
    isl_wssert(num != 1, "num should not be 1.", "num = %d, num + 1 = %d.", (num, num + 1));

}


void isl_test_overload(void) {

#define test_overload(_vargs...) _isl_overload(test_overload,##_vargs)

#define test_overload_0()       printf("test overload: 0 args\n")
#define test_overload_1(_1)     printf("test overload: 1 args\nfirst arg = %d\n", _1)
#define test_overload_2(_1, _2) printf("test overload: 2 args\nfirst arg = %d, and second arg = %d\n", _1, _2)

    test_overload();
    test_overload(123456);
    test_overload(123, 456);

}

