#include "isl_astnode.h"

// #define ISG_STRUCT_NAME             ist_astnodeList
// #define ISG_VALUE_TYPE              ist_astnode
// #define ISG_VALUE_FN_CLEAN(_node_p) ist_astnode_clean((_pent_p))
// #include "isg_list_code.h"


#define ISG_STRUCT_NAME             ist_parsentList
#define ISG_VALUE_TYPE              ist_parsent
#define ISG_VALUE_FN_CLEAN(_pent_p) ist_parsent_delete(*(_pent_p))
#include "isg_list_code.h"


ist_astnode ist_astnode_consby_full(ist_astnodeKind kind, ist_location loc, ist_astnodeAs as) {
    return ist_astnode_{.kind = kind, .location = loc, .as = as};
}
_isl_define_initby_createby_with_consby(
    (ist_astnodeKind kind, ist_location loc, ist_astnodeAs as),
    (kind, loc, as),
    ist_astnode,
    full
);

ist_astnode ist_astnode_consby_location(ist_location loc) {
    return ist_astnode_{.location = loc};
}
_isl_define_initby_createby_with_consby((ist_location loc), (loc), ist_astnode, location);


void ist_astnode_clean(ist_astnode* this) {}
void ist_astnode_delete(ist_astnode* this) {
    if (!this) return;
    ist_astnode_clean(this);
    isl_free(this);
}


void ist_parsent_delete(ist_parsent this) {}

ist_string ist_parsent_dump(ist_parsent this, ist_dumpctx dctx) {
    return *dctx.buffer;
}



// #include "inttypes.h"
// #include "isl_astnode.h"
// #include "isl_dump.h"



// #define ISG_STRUCT_NAME            ist_astnodeptr_list
// #define ISG_VALUE_TYPE             ist_astnode*
// #define ISG_VALUE_FN_CLEAN(_astpp) ist_ast_delete(*(_astpp))
// #include "isg_list_code.h"


// ist_string ist_astnode_type_names[] = {
// #define manifest(_name, _struct) [isl_astnt_##_name] = #_name,
// #include "isl_astnodes.h"
// #undef manifest
// };

// #define manifest(_name, _struct)                            \
//     inline ist_astnode_##_name* isl_as_##_name(void* adr) { \
//         isl_assert(adr);                                    \
//         return adr;                                         \
//     }
// #include "isl_astnodes.h"
// #undef manifest


// void ist_astnode_delete(void* this) {
//     if (this == NULL) return;

//     ist_astnode_typenum type = 0 [(ist_astnode_typenum*)this];

//     /**
//      * Delete the astnode by the type. It serves for the counting of the memory.
//      * Because the astnode have no constant length, we can not use the free function directly.
//      */
//     switch (type) {

// #define manifest(_name, _struct)              \
//     case isl_astnt_##_name:                   \
//         isl_free((ist_astnode_##_name*)this); \
//         break;
// #include "isl_astnodes.h"
// #undef manifest

//         default:
//             isl_report(rid_unknown_enum_value, isp_catch_coreloc, type);
//             break;
//     }
// }

// void ist_ast_delete(void* this) {
//     if (this == NULL) return;

//     /* delete the sub nodes of the astnode by the type */
//     switch (0 [(ist_astnode_typenum*)this]) {
//         case isl_astnt_scope:
//         case isl_astnt_module:
//         case isl_astnt_arg_list_patt:
//         case isl_astnt_params_list_patt:
//         case isl_astnt_node_list: {
//             ist_astnode_node_list* node_list = this;
//             ist_astnodeptr_list_clean(&node_list->list);
//             break;
//         }
//         case isl_astnt_unexpr: {
//             ist_ast_delete(isl_as_unexpr(this)->sub);
//             break;
//         }
//         case isl_astnt_binexpr: {
//             ist_astnode_binexpr* node = this;
//             ist_ast_delete(node->lhs);
//             ist_ast_delete(node->rhs);
//             break;
//         }
//         case isl_astnt_ternexpr: {
//             ist_astnode_ternexpr* node = this;
//             ist_ast_delete(node->first);
//             ist_ast_delete(node->second);
//             ist_ast_delete(node->third);
//             break;
//         }
//         case isl_astnt_fncall_expr: {
//             ist_astnode_fncall_expr* node = this;
//             ist_ast_delete(node->fn);
//             ist_astnodeptr_list_clean(&node->arglist);
//             break;
//         }
//     }
//     ist_astnode_delete(this);
// }



// ist_string ist_astptr_dump(ist_vptr* this, ist_dumpctx dctx) {
//     if (!this) return ist_strbuf_append_raw(dctx.buffer, dctx.idxptr, "null");
//     return ist_ast_dump(*this, dctx);
// }


// ist_string ist_ast_dump(ist_vptr this, ist_dumpctx dctx) {
//     isl_dreport(rid_inform_dumping, "astnode", this);
//     dctx.idxptr = dctx.idxptr ?: (ist_usize[1]){};
//     if (!this) return ist_strbuf_append_raw(dctx.buffer, dctx.idxptr, "null");

//     ist_astnode_typenum type = *(ist_astnode_typenum*)this;

//     // dctx.style |= DFLAG_HEAD_DOWRAP;

//     switch (type) {
//         case isl_astnt_unknown:
//             break;

//         case isl_astnt_scope:
//         case isl_astnt_module:
//         case isl_astnt_arg_list_patt:
//         case isl_astnt_params_list_patt:
//         case isl_astnt_node_list: {
//             ist_astnode_node_list* node_list = this;
//             return ist_dumpimage_dump(
//                 &ist_dumpimage_{
//                     .name    = ist_astnode_type_names[type],
//                     .wrapkey = "type",
//                     .count   = 2,
//                     ist_dumpitemar_{
//                         {"location", ist_location_dump, &node_list->base.location},
//                         {
//                             "list",
//                             isg_list_dumpack_dump,
//                             &isg_list_dumpack_{
//                                 .name   = NULL,
//                                 .idxtag = "[%" PRIuPTR "]",
//                                 .dowrap = true,
//                                 ist_astptr_dump,
//                                 &node_list->list,
//                                 ist_astnodeptr_list_capacity(&node_list->list),
//                             },
//                         },
//                     },
//                 },
//                 dctx
//             );

//             break;
//         }

//         case isl_astnt_literal: {
//             ist_astnode_literal* literal = this;
//             return ist_dumpimage_dump(
//                 &ist_dumpimage_{
//                     .name    = ist_astnode_type_names[type],
//                     .wrapkey = "type",
//                     .count   = 3,
//                     ist_dumpitemar_{
//                         {"location", ist_location_dump, &literal->base.location},
//                         {"litype", ist_cstring_dump_ident, &ist_token_names[literal->litype]},
//                         {
//                             "value",
//                             ist_tvalue_dump,
//                             &ist_tvalue_{
//                                 isl_toklitype_to_valtype[literal->litype],
//                                 literal->value,
//                             },
//                         },
//                     },
//                 },
//                 dctx
//             );
//             break;
//         }

//         case isl_astnt_binexpr: {
//             ist_astnode_binexpr* expr = this;
//             return ist_dumpimage_dump(
//                 &ist_dumpimage_{
//                     .name    = ist_astnode_type_names[type],
//                     .wrapkey = "type",
//                     .count   = 4,
//                     ist_dumpitemar_{
//                         {"location", ist_location_dump, &expr->base.location},
//                         {"optype", ist_cstring_dump_ident, &ist_token_names[expr->optype]},
//                         {"lhs", ist_ast_dump, expr->lhs},
//                         {"rhs", ist_ast_dump, expr->rhs},
//                     },
//                 },
//                 ist_dumpctx_{dctx.buffer, dctx.idxptr, dctx.indent, dctx.style}
//             );
//             break;
//         }

//         case isl_astnt_unexpr: {
//             ist_astnode_unexpr* expr = this;
//             return ist_dumpimage_dump(
//                 &ist_dumpimage_{
//                     .name    = ist_astnode_type_names[type],
//                     .wrapkey = "type",
//                     .count   = 4,
//                     ist_dumpitemar_{
//                         {"location", ist_location_dump, &expr->base.location},
//                         {"optype", ist_cstring_dump_ident, &ist_token_names[expr->optype]},
//                         {"onlhs", ist_bool_dump, &expr->onlhs},
//                         {"sub", ist_ast_dump, expr->sub},
//                     },
//                 },
//                 dctx

//             );
//             break;
//         }

//         case isl_astnt_name: {
//             ist_astnode_name* name = this;
//             return ist_dumpimage_dump(
//                 &ist_dumpimage_{
//                     .name    = ist_astnode_type_names[type],
//                     .wrapkey = "type",
//                     .count   = 2,
//                     ist_dumpitemar_{
//                         {"location", ist_location_dump, &name->base.location},
//                         {"name", ist_cstring_dump, &name->name},
//                     },
//                 },
//                 dctx

//             );
//             break;
//         }

//         case isl_astnt_use_stmt: {
//             ist_astnode_use_stmt* stmt = this;
//             return ist_dumpimage_dump(
//                 &ist_dumpimage_{
//                     .name    = ist_astnode_type_names[type],
//                     .wrapkey = "type",
//                     .count   = 3,
//                     ist_dumpitemar_{
//                         {"location", ist_location_dump, &stmt->base.location},
//                         {"lhs", ist_ast_dump, stmt->lhs},
//                         {"rhs", ist_ast_dump, stmt->rhs},
//                     },
//                 },
//                 dctx

//             );
//             break;
//         }


//         case isl_astnt_fncall_expr: {
//             ist_astnode_fncall_expr* fncall = this;
//             return ist_dumpimage_dump(
//                 &ist_dumpimage_{
//                     .name    = ist_astnode_type_names[type],
//                     .wrapkey = "type",
//                     .count   = 3,
//                     ist_dumpitemar_{
//                         {"location", ist_location_dump, &fncall->base.location},
//                         {"fn", ist_ast_dump, fncall->fn},
//                         {
//                             "arg_list",
//                             isg_list_dumpack_dump,
//                             &isg_list_dumpack_{
//                                 .name   = NULL,
//                                 .idxtag = "[%" PRIuPTR "]",
//                                 .dowrap = true,
//                                 ist_astptr_dump,
//                                 &fncall->arglist,
//                                 ist_astnodeptr_list_capacity(&fncall->arglist),
//                             },
//                         },
//                     },
//                 },
//                 dctx

//             );
//             break;
//         }

//         default:
//             break;
//     }

//     return *dctx.buffer;
// }


// inline void ist_astnode_node_list_add(ist_astnode_node_list* this, void* node) {
//     ist_astnodeptr_list_addm(&this->list, node);
// }
