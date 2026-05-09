#include "isl_compiler.h"
#include "isl_parser.h"
#include "isl_vm.h"

#define ISL_TEST_ITEM compiler
#include "isg_test_defines.h"
ISL_TEST_HEAD {

    ist_string   filepath = ist_string_consby_raw("./scripts/test.is");
    ist_module   module   = ist_module_consby_filepath(filepath);
    ist_parser   parser   = ist_parser_consby_module(&module);
    ist_compiler compiler = ist_compiler_{
        .module   = &module,
        .parser   = &parser,
        .instream = ist_instream_createc(16),
    };


    printf("file context:\n%s\n", parser.lexer.codepage->source);

    ist_parser_parse(&parser);

    ist_strbuf dump_buffer = ist_strbuf_cons(1024);

    printf("%s\n", ist_module_dump(&module, ist_dumpctx_{dump_buffer, NULL, DKIND_INDENT, -1}));
    printf("%s\n", ist_parsent_dump(parser.root, ist_dumpctx_{dump_buffer, NULL, DKIND_INDENT}));

    compiler.node = parser.root;
    ist_compiler_compile(&compiler);

    ist_instream_dumpack_dump(&ist_instream_dumpack_{compiler.instream}, ist_dumpctx_{dump_buffer});
    printf("instream:\n%s\n", *dump_buffer);
    printf("%p :", compiler.instream->data);
    isg_list_foreach (instp, *compiler.instream) {
        printf("%02X ", *instp);
    }
    printf("\n");

    ist_vm vm = ist_vm_consby_instream(*compiler.instream);

    ist_vm_run(&vm);
    ist_i64_dump(&vm.sp[0].as_i64, ist_dumpctx_{dump_buffer});
    printf("vm result: %s\n", *dump_buffer);
    ist_i64_dump(isl_span(ist_i64){1 + (2 * 3 + (4 / 5 * (2 / 3)))}, ist_dumpctx_{dump_buffer});
    printf("c  result: %s\n", *dump_buffer);

    printf("sizeof(astnodeKind): %zu\n", sizeof(ist_astnodeKind));
    printf("sizeof(location): %zu\n", sizeof(ist_location));
    printf("sizeof(astnodeAs): %zu\n", sizeof(ist_astnodeAs));
    printf("sizeof(astnodeAs_expr): %zu\n", sizeof(ist_astnodeAs_expr));
    printf("sizeof(astnode): %zu\n", sizeof(ist_astnode));

    ist_module_clean(&module);
    ist_parser_clean(&parser);
    ist_string_delete(dump_buffer);
    ist_instream_delete(compiler.instream);
    ist_vm_clean(&vm);
}
ISL_TEST_TAIL
#include "isg_test_undefs.h"
