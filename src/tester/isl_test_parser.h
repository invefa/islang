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

    printf("%s\n", ist_module_dump(&module, ist_dumpctx_{dump_buffer, DKIND_INDENT, 0, -1}));

    printf("%s\n", ist_parsent_dump(parser.root, ist_dumpctx_{dump_buffer, DKIND_JSON}));
    printf("%s\n", ist_parsent_dump(parser.root, ist_dumpctx_{dump_buffer, DKIND_INDENT}));
    printf("%s\n", ist_parsent_dump(parser.root, ist_dumpctx_{dump_buffer, DKIND_STRUCT}));
    printf("%s\n", ist_parsent_dump(parser.root, ist_dumpctx_{dump_buffer, DKIND_JSON, 0, -1}));
    printf("%s\n", ist_parsent_dump(parser.root, ist_dumpctx_{dump_buffer, DKIND_INDENT, 0, -1}));
    printf("%s\n", ist_parsent_dump(parser.root, ist_dumpctx_{dump_buffer, DKIND_STRUCT, 0, -1}));
    ist_module_clean(&module);
    ist_parser_clean(&parser);
    ist_string_delete(dump_buffer);
}
ISL_TEST_TAIL
#include "isg_test_undefs.h"
