#include "isl_astnode.h"


#define ISG_STRUCT_NAME           ist_astnodeptr_list
#define ISG_VALUE_TYPE            ist_astnode*
#define ISG_VALUE_FN_CLEAN(_astp) ist_ast_delete(*(_astp))
#include "isg_list_code.h"


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
        case ISL_ASTNT_SCOPE_ENT:
        case ISL_ASTNT_MODULE_ENT:
        case ISL_ASTNT_ARG_LIST_PATT:
        case ISL_ASTNT_PARAM_LIST_PATT:
        case ISL_ASTNT_NODE_LIST: {
            IST_ASTNODE_NODE_LIST* node_list = this;
            ist_astnodeptr_list_clean(&node_list->nodeptr_list);
            break;
        }
        case ISL_ASTNT_UNARY_EXPR: {
            ist_ast_delete(ISL_AS_UNARY_EXPR(this)->sub_node);
            break;
        }
        case ISL_ASTNT_BINARY_EXPR: {
            IST_ASTNODE_BINARY_EXPR* binary_opt = this;
            ist_ast_delete(binary_opt->lhs_node);
            ist_ast_delete(binary_opt->rhs_node);
            break;
        }
        case ISL_ASTNT_TERNARY_EXPR: {
            IST_ASTNODE_TERNARY_EXPR* ternary_opt = this;
            ist_ast_delete(ternary_opt->first_node);
            ist_ast_delete(ternary_opt->second_node);
            ist_ast_delete(ternary_opt->third_node);
            break;
        }
        case ISL_ASTNT_FNCALL_EXPR: {
            IST_ASTNODE_FNCALL_EXPR* fncall = this;
            ist_ast_delete(fncall->fn_entity);
            ist_astnodeptr_list_clean(&fncall->arglist);
            break;
        }
    }
    ist_astnode_delete(this);
}

ist_string ist_ast_dump_json(void* this, ist_string* buffer, ist_usize* idxptr) {
    // isl_ifnreport(this, rid_catch_nullptr, isp_catch_coreloc);
    isl_report(rid_inform_dumping, "node", this);
    idxptr = idxptr ?: (ist_usize[1]){};
    if (!this) return *buffer;

    ist_astnode_type type = 0 [(ist_astnode_type*)this];

    ist_strbuf_sprintf(buffer, idxptr, "{\"type:\":\"%s\",", ist_astnode_type_names[type]);
    ist_location_dump_json(&((ist_astnode*)this)->location, buffer, idxptr);
    ist_strbuf_append_raw(buffer, idxptr, ",");

    switch (type) {
        case ISL_ASTNT_UNKNOWN:
            ist_strbuf_append_raw(buffer, idxptr, "\b");
            break;

        case ISL_ASTNT_SCOPE_ENT:
        case ISL_ASTNT_MODULE_ENT:
        case ISL_ASTNT_ARG_LIST_PATT:
        case ISL_ASTNT_PARAM_LIST_PATT:
        case ISL_ASTNT_NODE_LIST: {
            IST_ASTNODE_NODE_LIST* node_list = this;

            ist_strbuf_append_raw(buffer, idxptr, "\"nodeptr_list\":[");
            isg_list_foreach (nodepp, node_list->nodeptr_list) {
                ist_ast_dump_json(*nodepp, buffer, idxptr);
                ist_strbuf_append_raw(buffer, idxptr, ",");
            }
            ist_strbuf_append_raw(buffer, idxptr, "\b]");
            break;
        }

        case ISL_ASTNT_LITERAL_ENT: {
            IST_ASTNODE_LITERAL_ENT* literal_ent = this;
            ist_strbuf_sprintf(
                buffer, idxptr, "\"litype\":\"%s\",\"value\":", ist_token_names[literal_ent->litype]
            );
            ist_value_dump_json(&literal_ent->value, literal_ent->litype, buffer, idxptr);
            break;
        }

        case ISL_ASTNT_BINARY_EXPR: {
            IST_ASTNODE_BINARY_EXPR* binary_opt = this;
            ist_strbuf_sprintf(
                buffer,
                idxptr,
                "\"optype\":\"%s\",\"lhs_node\":",
                ist_token_names[binary_opt->optype]
            );
            ist_ast_dump_json(binary_opt->lhs_node, buffer, idxptr);
            ist_strbuf_append_raw(buffer, idxptr, ",\"rhs_node\":");
            ist_ast_dump_json(binary_opt->rhs_node, buffer, idxptr);
            break;
        }

        case ISL_ASTNT_UNARY_EXPR: {
            IST_ASTNODE_UNARY_EXPR* unary_opt = this;
            ist_strbuf_sprintf(
                buffer,
                idxptr,
                "\"optype\":\"%s\",\"onlhs\":%s,\"sub_node\":",
                ist_token_names[unary_opt->optype],
                unary_opt->onlhs ? "true" : "false"
            );
            ist_ast_dump_json(unary_opt->sub_node, buffer, idxptr);
            break;
        }

        case ISL_ASTNT_NAME_ENT: {
            IST_ASTNODE_NAME_ENT* name_ent = this;
            ist_strbuf_sprintf(buffer, idxptr, "\"name\":\"%s\"", name_ent->name);
            break;
        }

        case ISL_ASTNT_FNCALL_EXPR: {
            IST_ASTNODE_FNCALL_EXPR* fncall = this;
            ist_strbuf_sprintf(buffer, idxptr, "\"fn_entity\":");
            ist_ast_dump_json(fncall->fn_entity, buffer, idxptr);
            ist_strbuf_append_raw(buffer, idxptr, ",\"arglist\":[");
            isg_list_foreach (nodepp, fncall->arglist) {
                ist_ast_dump_json(*nodepp, buffer, idxptr);
                ist_strbuf_append_raw(buffer, idxptr, ",");
            }
            ist_strbuf_append_raw(buffer, idxptr, "\b]");
            break;
        }

        default:
            ist_strbuf_append_raw(buffer, idxptr, "\b");
            break;
    }

    return ist_strbuf_append_raw(buffer, idxptr, "}");
}

inline void IST_ASTNODE_NODE_LIST_ADD(IST_ASTNODE_NODE_LIST* this, void* node) {
    ist_astnodeptr_list_addm(&this->nodeptr_list, node);
}
