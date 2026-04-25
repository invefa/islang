#include "isl_parser.h"

#define ISL_TEST_ITEM parser
#include "isg_test_defines.h"
ISL_TEST_HEAD {

    ist_string filepath = ist_string_consby_raw("./scripts/test.is");
    ist_module module   = ist_module_consby_filepath(filepath);
    ist_parser parser   = ist_parser_consby_module(&module);

    printf("file context:\n%s\n", parser.lexer.codepage->source);

    ist_parser_parse(&parser);

    ist_strbuf dump_buffer = ist_strbuf_cons(1024);

    printf("AST: %s\n", ist_ast_dump_old(parser.root, dump_buffer, NULL));
    // printf("%s\n", ist_module_dump_indent(&module, dump_buffer, NULL, 0));s
    printf("%s\n", ist_module_dump(&module, ist_dumpctx_{dump_buffer, NULL, 0, DKIND_JSON}));
    printf("%s\n", ist_module_dump(&module, ist_dumpctx_{dump_buffer, NULL, 0, DKIND_INDENT}));
    printf("%s\n", ist_module_dump(&module, ist_dumpctx_{dump_buffer, NULL, 0, DKIND_STRUCT}));
    printf("%s\n", ist_module_dump(&module, ist_dumpctx_{dump_buffer, NULL, -1, DKIND_JSON}));
    printf("%s\n", ist_module_dump(&module, ist_dumpctx_{dump_buffer, NULL, -1, DKIND_INDENT}));
    printf("%s\n", ist_module_dump(&module, ist_dumpctx_{dump_buffer, NULL, -1, DKIND_STRUCT}));
    printf("%s\n", ist_ast_dump(parser.root, ist_dumpctx_{dump_buffer, NULL, 0, DKIND_JSON}));
    printf("%s\n", ist_ast_dump(parser.root, ist_dumpctx_{dump_buffer, NULL, 0, DKIND_INDENT}));
    printf("%s\n", ist_ast_dump(parser.root, ist_dumpctx_{dump_buffer, NULL, 0, DKIND_STRUCT}));
    // printf("%s\n", ist_ast_dump(parser.root, ist_dumpctx_{dump_buffer, NULL, -1, DKIND_JSON}));
    // printf("%s\n", ist_ast_dump(parser.root, ist_dumpctx_{dump_buffer, NULL, -1, DKIND_INDENT}));
    // printf("%s\n", ist_ast_dump(parser.root, ist_dumpctx_{dump_buffer, NULL, -1, DKIND_STRUCT}));
    ist_module_clean(&module);
    ist_parser_clean(&parser);
    ist_string_delete(dump_buffer);
}
ISL_TEST_TAIL
#include "isg_test_undefs.h"
