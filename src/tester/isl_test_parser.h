#include "isl_parser.h"

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
