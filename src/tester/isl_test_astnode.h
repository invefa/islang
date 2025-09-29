#include "isl_parser.h"

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
