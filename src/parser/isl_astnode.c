#include "isl_astnode.h"

#include "isl_list.h"
#include "isl_memgr.h"


const ist_string ist_astnode_type_names[] = {
#define manifest(_name, _struct) [ISL_ASTNT_##_name] = #_name,
#include "isl_astnodes.h"
#undef manifest
};

#define manifest(_name, _struct)                            \
    inline IST_ASTNODE_##_name* ISL_AS_##_name(void* adr) { \
        isl_assert(adr);                                    \
        return adr;                                         \
    }
#include "isl_astnodes.h"
#undef manifest


void ist_astnode_delete(void* this) {
    if (this == NULL) return;

    ist_astnode_type type = 0 [(ist_astnode_type*)this];

    /**
     * Delete the astnode by the type. It serves for the counting of the memory.
     * Because the astnode have no constant length, we can not use the free function directly.
     */
    switch (type) {

#define manifest(_name, _struct)              \
    case ISL_ASTNT_##_name:                   \
        isl_free((IST_ASTNODE_##_name*)this); \
        break;
#include "isl_astnodes.h"
#undef manifest

        default:
            isl_report(rid_unknown_enum_value, isp_catch_coreloc, type);
            break;
    }
}

void ist_ast_delete(void* this) {
    if (this == NULL) return;

    /* delete the sub nodes of the astnode by the type */
    switch (0 [(ist_astnode_type*)this]) {
        case ISL_ASTNT_SCOPE:
        case ISL_ASTNT_MODULE:
        case ISL_ASTNT_ARG_LIST:
        case ISL_ASTNT_PARAM_LIST:
        case ISL_ASTNT_NODE_LIST: {
            IST_ASTNODE_NODE_LIST* node_list = this;

            isl_list_foreach_to (nodepp, node_list->nodeptr_list, node_list->nodeptr_count)
                ist_ast_delete(*nodepp);

            isl_list_freev(node_list->nodeptr_list);
            break;
        }
        case ISL_ASTNT_UNARY_OPT: {
            ist_ast_delete(ISL_AS_UNARY_OPT(this)->sub_node);
            break;
        }
        case ISL_ASTNT_BINARY_OPT: {
            IST_ASTNODE_BINARY_OPT* binary_opt = this;
            ist_ast_delete(binary_opt->left_node);
            ist_ast_delete(binary_opt->right_node);
            break;
        }
        case ISL_ASTNT_TERNARY_OPT: {
            IST_ASTNODE_TERNARY_OPT* ternary_opt = this;
            ist_ast_delete(ternary_opt->first_node);
            ist_ast_delete(ternary_opt->second_node);
            ist_ast_delete(ternary_opt->third_node);
            break;
        }
    }
    ist_astnode_delete(this);
}

ist_string ist_ast_dump_json(void* this, ist_string* buffer) {

    isl_report(rid_custom_core_warning, "dumping node<0x%zX>.", (ist_usize)this);

    ist_string* tmp_buffer = ist_string_create_buffer(64);

    ist_astnode* node  = this;
    ist_usize    index = 0;

    ist_strbuf_sprintf(
        buffer,
        &index,
        "{\"type:\":\"%s\",%s,",
        ist_astnode_type_names[node->type],
        ist_location_dump_json(&node->location, tmp_buffer)
    );

    switch (node->type) {
        case ISL_ASTNT_UNKNOWN:
            ist_strbuf_append_raw(buffer, &index, "\b");
            break;

        case ISL_ASTNT_SCOPE:
        case ISL_ASTNT_MODULE:
        case ISL_ASTNT_ARG_LIST:
        case ISL_ASTNT_PARAM_LIST:
        case ISL_ASTNT_NODE_LIST: {
            IST_ASTNODE_NODE_LIST* node_list = this;

            ist_strbuf_append_raw(buffer, &index, "\"nodeptr_list\":[");
            isl_list_foreach_to (nodepp, node_list->nodeptr_list, node_list->nodeptr_count)
                ist_strbuf_append_raws(
                    buffer, &index, ist_ast_dump_json(*nodepp, tmp_buffer), ",", NULL
                );

            ist_strbuf_append_raw(buffer, &index, "\b]");
            break;
        }

        case ISL_ASTNT_LITERAL_ENT: {
            IST_ASTNODE_LITERAL_ENT* literal_ent = this;
            ist_strbuf_sprintf(
                buffer,
                &index,
                "\"literal_type\":\"%s\",\"value\":%s",
                ist_token_names[literal_ent->literal_type],
                ist_value_dump_json(&literal_ent->value, literal_ent->literal_type, tmp_buffer)
            );
            break;
        }

        case ISL_ASTNT_BINARY_OPT: {
            IST_ASTNODE_BINARY_OPT* binary_opt = this;
            ist_strbuf_sprintf(
                buffer,
                &index,
                "\"operator_type\":\"%s\",\"left_node\":%s,\"right_node\":",
                ist_token_names[binary_opt->operator_type],
                ist_ast_dump_json(binary_opt->left_node, tmp_buffer)
            );
            ist_strbuf_append_raw(
                buffer, &index, ist_ast_dump_json(binary_opt->right_node, tmp_buffer)
            );
            break;
        }

        case ISL_ASTNT_UNARY_OPT: {
            IST_ASTNODE_UNARY_OPT* unary_opt = this;
            ist_strbuf_sprintf(
                buffer,
                &index,
                "\"operator_type\":\"%s\",\"on_left\":%s,\"sub_node\":%s",
                ist_token_names[unary_opt->operator_type],
                unary_opt->on_left ? "true" : "false",
                ist_ast_dump_json(unary_opt->sub_node, tmp_buffer)
            );
            break;
        }

        default:
            ist_strbuf_append_raw(buffer, &index, "\b");
            break;
    }

    ist_strbuf_append_raw(buffer, &index, "}");

    ist_string_delete(tmp_buffer);
    return *buffer;
}