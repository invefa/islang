/**
 * This c source file is the main source file for islang, and it is the entrance of islang.
 * This project is compiled with -std=gnu11, because it will use some extension functions
 * which are suplied by GNU standard extensions.
 *
 * There are a list of extensions are used:
 *  - Macros with a Variable Number of Arguments.
 *  - Statements and Declarations in Expressions.
 *  - Arithmetic on void- and Function-Pointers.
 *  - Conditionals with Omitted Operands.
 *  - Referring to a Type with typeof.
 *  - u8 prefix for string literal.
 *  - Function Names as Strings.
 *  - Designated Initializers.
 *  - Compound Literals.
 *
 * For more details about them, you can view:
 * https://gcc.gnu.org/onlinedocs/gcc/C-Extensions.html#C-Extensions
 *
 * Anyway, you had better compile this project with gcc -std=gnu11.
 * Written by invefa.
 */

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "isl_ansictrl.h"
#include "isl_astnode.h"
#include "isl_dbgutils.h"
#include "isl_lexer.h"
#include "isl_list.h"
#include "isl_memgr.h"
#include "isl_overload.h"
#include "isl_parser.h"
#include "isl_report.h"
#include "isl_string.h"
#include "isl_utf8.h"
#include "isl_xssert.h"


void isl_test_overload(void);
void isl_test_xssert(void);
void isl_test_list(void);
void isl_test_memgr(void);
void isl_test_string(void);
void isl_test_report(void);
void isl_test_lexer(void);
void isl_test_generic(void);
void isl_test_astnode(void);
void isl_test_parser(void);

int main(int argc, char* argv[]) {

    if (argc <= 1) system("chcp 65001");
    // isl_test_overload();
    // isl_test_xssert();
    // isl_test_list();
    // isl_test_memgr();
    // isl_test_string();
    // isl_test_report();
    // isl_test_lexer();
    isl_test_generic();
    // isl_test_astnode();
    // isl_test_parser();

    return 0;
}

void isl_test_parser(void) {
    isl_report(rid_inform_start_testing, "parser");
    isl_max_allocated_length = 0;

    ist_string filepath = ist_string_consby_raw("./scripts/test.is");
    ist_module module   = ist_module_consby_filepath(filepath);
    ist_parser parser   = ist_parser_consby_module(&module);

    ist_parser_parse(&parser);

    ist_string* dump_buffer = ist_string_create_buffer(1024);

    // printf("module: %s\n", ist_module_dump_json(&module, dump_buffer, NULL));
    printf("AST: %s\n", ist_ast_dump_json(parser.root, dump_buffer, NULL));

    ist_module_clean(&module);
    ist_parser_clean(&parser);
    ist_string_delete(dump_buffer);

    isl_report(rid_custom_core_info, "max allocated-length = %zu.", isl_max_allocated_length);
    isl_report(rid_inform_end_testing, "parser");
}

void isl_test_astnode(void) {
    isl_report(rid_inform_start_testing, "astnode");
    isl_max_allocated_length = 0;

    isl_report(
        rid_custom_core_warn,
        "sizeof(IST_ASTNODE_BINARY_OPT) = %zu",
        sizeof(IST_ASTNODE_BINARY_EXPR)
    );

    isl_report(
        rid_custom_core_warn, "sizeof(IST_ASTNODE_UNARY_OPT) = %zu", sizeof(IST_ASTNODE_UNARY_EXPR)
    );

    IST_ASTNODE_UNARY_EXPR* unopt_node =
        ist_astnode_createby_full(UNARY_EXPR, ist_location_consby_null());
    unopt_node->optype   = ISL_TOKENT_SUB;
    unopt_node->sub_node = ist_astnode_createby_full(LITERAL_ENT, ist_location_consby_null());
    unopt_node->onlhs    = true;
    ISL_AS_LITERAL_ENT(unopt_node->sub_node)->litype          = ISL_TOKENT_VL_INT;
    ISL_AS_LITERAL_ENT(unopt_node->sub_node)->value.int_value = 123;


    IST_ASTNODE_BINARY_EXPR* binopt_node =
        ist_astnode_createby_full(BINARY_EXPR, ist_location_consby_null());
    binopt_node->optype   = ISL_TOKENT_ADD;
    binopt_node->lhs_node = ist_astnode_createby_full(LITERAL_ENT, ist_location_consby_null());
    binopt_node->rhs_node = ist_astnode_createby_full(LITERAL_ENT, ist_location_consby_null());
    ISL_AS_LITERAL_ENT(binopt_node->lhs_node)->litype          = ISL_TOKENT_VL_INT;
    ISL_AS_LITERAL_ENT(binopt_node->rhs_node)->litype          = ISL_TOKENT_VL_INT;
    ISL_AS_LITERAL_ENT(binopt_node->lhs_node)->value.int_value = -123;
    ISL_AS_LITERAL_ENT(binopt_node->rhs_node)->value.int_value = -456;

    IST_ASTNODE_MODULE_ENT* module_node =
        ist_astnode_createby_full(MODULE_ENT, ist_location_consby_null());
    module_node->nodeptr_list = ist_astnodeptr_list_consc(2);
    IST_ASTNODE_NODE_LIST_ADD(module_node, binopt_node);
    IST_ASTNODE_NODE_LIST_ADD(module_node, unopt_node);

    ist_string* buffer = ist_string_create_buffer(684);

    printf("dump result: %s\n", ist_ast_dump_json(module_node, buffer, NULL));

    ist_string_delete(buffer);
    ist_ast_delete(module_node);

    isl_report(rid_custom_core_info, "max allocated-length = %zu.", isl_max_allocated_length);
    isl_report(rid_inform_end_testing, "astnode");
}

void isl_test_generic(void) {
    isl_report(rid_custom_core_warn, "start testing generic...");
    isl_max_allocated_length = 0;

    // isp_report_option_enable(ISP_ROPTM_NO_CORE_INFO);

    ist_value_list* value_list = ist_value_list_create(10, 1);

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    for (ist_usize i = 0; i < 123456; ++i)
        ist_value_list_addm(value_list, ist_value_consby_i64(i * 13));

    clock_gettime(CLOCK_REALTIME, &end);

    printf(
        "time cost: %lfs\n",
        (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.
    );

    ist_value_list_addc(value_list, ist_value_consby_i64(1234567890));

    // isg_list_foreach (iterp, *value_list, idx)
    //     printf("value[%zu] = %" PRId64 "\n", idx, iterp->int_value);


    ist_module_list* module_list = ist_module_list_calloc(10);
    ist_module_list_addc(
        module_list, ist_module_consby_filepath(ist_string_consby_raw("./scripts/test.is"))
    );
    ist_module_list_addc(
        module_list, ist_module_consby_filepath(ist_string_consby_raw("./scripts/test.is"))
    );
    ist_module_list_addc(
        module_list, ist_module_consby_filepath(ist_string_consby_raw("./scripts/test.is"))
    );

    ist_string buffer = ist_string_cons_buffer(142);

    isg_list_foreach (iterp, *module_list, idx) {
        printf("module[%zu] = %s\n", idx, ist_module_dump_json(iterp, &buffer, NULL));
    }

    ist_string_clean(&buffer);
    ist_module_list_delete(module_list);
    ist_value_list_delete(value_list);

    // isp_report_option_disable(ISP_ROPTM_NO_CORE_INFO);

    ist_value_stack* stack = ist_value_stack_calloc(16);
    for (ist_usize i = 0; i < 18; ++i) {
        ist_value_stack_pushc(stack, ist_value_consby_u64(i));
    }

    isg_list_foreach (vp, *stack) {
        printf("%" PRIu64 " ", vp->uint_value);
    }
    printf("\n");

    for (ist_usize i = 0; i < 18; ++i) {
        isl_assert(ist_value_stack_pop(stack).uint_value == 17 - i);
    }
    // ist_value_stack_pop(stack);

    ist_value_stack_delete(stack);

    isl_report(rid_custom_core_info, "max allocated-length = %zu.", isl_max_allocated_length);
    isl_report(rid_inform_end_testing, "generic");
}

void isl_test_lexer(void) {
    isl_report(rid_inform_start_testing, "lexer");
    isl_max_allocated_length = 0;

    /* init some basic information */
    ist_string filepath       = ist_string_consby_raw("./scripts/test.is");
    ist_string macro_source   = ist_string_consby_raw(u8"起始 1 is a "
                                                    u8"num;@&\n中间//awa\n/*1\n2*/\n886结束");
    ist_string macroin_source = ist_string_consby_raw(u8"cast!(1,string)");

    /* print file content */
    ist_string file_contents = isl_read_file(filepath);
    printf("file context:\n|\nv\n%s<--\n", file_contents);
    ist_string_clean(&file_contents);

    /* construct module and lexer, and create dump buffer */
    ist_module  module  = ist_module_consby_filepath(filepath);
    ist_lexer   lexer   = ist_lexer_consby_module(&module);
    ist_string* dumpbuf = ist_string_create_buffer(128);

    ist_module_register_strbuf(&module, macro_source, ISL_STRBUFT_SOURCE);
    ist_module_register_strbuf(&module, macroin_source, ISL_STRBUFT_SOURCE);

    /* make sure token dumping synchronized with fn:advance analysis */
    printf("%s\n", ist_token_dump(&lexer.cur_token, dumpbuf, NULL));
    printf("%s\n", ist_token_dump(&lexer.nex_token, dumpbuf, NULL));
    while (lexer.sec_token.type != ISL_TOKENT_EOF) {
        printf("%s\n", ist_token_dump(&lexer.sec_token, dumpbuf, NULL));

#define lexer_switch_codepage(_pagename, _pagesrc)                                                \
    ist_lexer_switch_codepage(                                                                    \
        &lexer, ist_codepage_createby_source(&module, ist_string_consby_raw(_pagename), _pagesrc) \
    )

        /* switch codepage if the current token is a wrapper */
        // if (lexer.sec_token.type == ISL_TOKENT_WRAPPER) lexer_switch_codepage("wrap",
        // macro_source); if (lexer.sec_token.type == ISL_TOKENT_AND)
        // lexer_switch_codepage("wrapin", macroin_source);

        // if (lexer.sec_token.type == ISL_TOKENT_VL_INT) {

        //     ist_lexer_lookahead_start(&lexer);
        //     isl_report(rid_custom_core_warn, "start lookahead.");

        //     while (lexer.sec_token.type != ISL_TOKENT_EOS && lexer.sec_token.type !=
        //     ISL_TOKENT_EOF)
        //     {
        //         ist_lexer_advance(&lexer);
        //         printf("%s\n", ist_token_dump(&lexer.sec_token, dumpbuf, NULL));
        //     }

        //     ist_lexer_lookahead_end(&lexer);
        //     isl_report(rid_custom_core_warn, "end lookahead.");
        // }

        ist_lexer_advance(&lexer);
    }
    /* print the last token:EOF */
    printf("%s\n", ist_token_dump(&lexer.sec_token, dumpbuf, NULL));

    /* clean up */
    ist_lexer_clean(&lexer);
    ist_module_clean(&module);
    ist_string_delete(dumpbuf);

    isl_report(rid_custom_core_info, "max allocated-length = %zu.", isl_max_allocated_length);
    isl_report(rid_inform_end_testing, "lexer");
}

void isl_test_report(void) {

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

typedef struct ist_i32_list {
    ist_i32*  data;
    ist_usize size;
} ist_i32_list;

ist_i32* isl_emit_i32_list() {
    ist_i32* list = isl_list_malloc(ist_i32, 10);
    list[0] = 1, list[1] = 2, list[2] = 3, list[3] = 4, list[4] = 5;
    list[5] = 6, list[6] = 7, list[7] = 8, list[8] = 9, list[9] = 10;
    return list;
}

void isl_test_memgr(void) {
    printf("test for memgr\n");
    ist_i32* list = isl_list_calloc(ist_i32, 10);
    list[0] = 1, list[1] = 2, list[2] = 3, list[3] = 4, list[4] = 5;
    list[5] = 6, list[6] = 7, list[7] = 8, list[8] = 9, list[9] = 10;
    ist_i32_list* list2 = isl_calloc(ist_i32_list);
    list2->data         = isl_list_calloc(ist_i32, 10);
    list2->data[0] = 1, list2->data[1] = 2, list2->data[2] = 3, list2->data[3] = 4,
    list2->data[4] = 5;
    list2->data[5] = 6, list2->data[6] = 7, list2->data[7] = 8, list2->data[8] = 9,
    list2->data[9] = 10;
    list2->size    = 10;
    isl_list_freev(list);
    isl_list_freev(list2->data);
    isl_free(list2);
    list  = isl_emit_i32_list();
    list2 = isl_calloc(ist_i32_list);
    isl_list_resizc(isl_emit_i32_list(), 20, list2->data);
    isl_list_resizc(list, 20);
    isl_list_free(list);
    isl_list_free(list2->data);
    isl_free(list2);
    isl_list_resizc(isl_emit_i32_list(), 100, list);
    isl_list_free(list);
}

void isl_test_list(void) {

    ist_i32* list = isl_list_calloc(ist_i32, 10);
    // ist_i32* new_list = NULL;
    printf("list = 0x%zX\n", (ist_usize)list);
    printf("length of list = %zu\n", isl_list_catch_length(list));
    printf("capacity of list = %zu\n", isl_list_ptr_get_capacity(list));

    list[0] = 1, list[1] = 2, list[2] = 3, list[3] = 4, list[4] = 5;
    list[5] = 6, list[6] = 7, list[7] = 8, list[8] = 9, list[9] = 10;
    ist_usize size = 10;

    isl_list_ensurec(list, size, 5);

    isl_list_resizc(list, 20);
    isl_list_resizc(list, 30);
    isl_list_resizc(list, 6);

    isl_list_freev(list);
    isl_wssert(0);
    isl_list_ensurec(isl_emit_i32_list(), 9, 1, list);
    size = 10;

    for (ist_usize i = 0; i < isl_list_ptr_get_capacity(list); ++i)
        printf("list[%zu] = %d\n", i, list[i]);

    isl_list_addc(list, size, 66, list);
    isl_list_addc(list, size, 66, list);
    isl_list_addc(list, size, 66, list);
    isl_list_addc(list, size, 66, list);
    isl_list_addc(list, size, 66, list);
    isl_list_addc(list, size, 66, list);
    isl_list_addc(list, size, 66, list);
    isl_list_addc(list, size, 66, list);
    isl_list_addc(list, size, 66, list);

    for (ist_usize i = 0; i < isl_list_ptr_get_capacity(list); ++i)
        printf("list[%zu] = %d\n", i, list[i]);

    isl_list_freev(list);
}

void isl_test_xssert(void) {

    int num = 1;
    isl_wssert(num != 1);
    isl_wssert(num != 1, "num should not be 1.");
    isl_wssert(num != 1, "num should not be 1.", "this is just a test.");
    isl_wssert(num != 1, "num should not be 1.", "num = %d, num + 1 = %d.", (num, num + 1));
    num = 2;
    (void)num;
}

void isl_test_overload(void) {

#define test_overload(_vargs...) _isl_overload(test_overload, ##_vargs)

#define test_overload_0()   printf("test overload: 0 args\n")
#define test_overload_1(_1) printf("test overload: 1 args\nfirst arg = %d\n", _1)
#define test_overload_2(_1, _2) \
    printf("test overload: 2 args\nfirst arg = %d, and second arg = %d\n", _1, _2)

    test_overload();
    test_overload(123456);
    test_overload(123, 456);
}
