#include "inttypes.h"
#include "isl_astnode.h"


ist_string ist_astnodeKindNames[] = {
#define manifest(_name, _struct) [ist_astnodeKind_##_name] = #_name,
#include "isl_astnodes.h"
#undef manifest
};


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


void ist_parsent_delete(ist_parsent this) {
    if (!this) return;
    switch (this->kind) {
        case ist_astnodeKind_unk:
        case ist_astnodeKind_literal:
        case ist_astnodeKind_name:
            break;
        case ist_astnodeKind_list:
            ist_parsentList_clean(&this->as.list.this);
            break;
        case ist_astnodeKind_expr: {
            ist_astnodeAs_expr expr = this->as.expr;
            switch (this->as.expr.kind) {
                case isn_psentExprKind_unary:
                    ist_parsent_delete(expr.as.unary.lhs);
                    ist_parsent_delete(expr.as.unary.rhs);
                    break;
                case isn_psentExprKind_binary:
                    ist_parsent_delete(expr.as.binary.lhs);
                    ist_parsent_delete(expr.as.binary.rhs);
                    break;
                case isn_psentExprKind_ternary:
                    if (!expr.as.ternary) isp_unreachable();
                    ist_parsent_delete(expr.as.ternary->st);
                    ist_parsent_delete(expr.as.ternary->nd);
                    ist_parsent_delete(expr.as.ternary->rd);
                    break;
                case isn_psentExprKind_fncall:
                    ist_parsent_delete(expr.as.fncall.fn);
                    ist_parsentList_clean(&expr.as.fncall.args);
                    break;
            }
            break;
        }
        case ist_astnodeKind_scope:
            ist_parsentList_clean(&this->as.scope.stmts);
            break;
        case ist_astnodeKind_use_stmt:
            ist_parsent_delete(this->as.use_stmt.lhs);
            ist_parsent_delete(this->as.use_stmt.rhs);
            break;
        case ist_astnodeKind_do_stmt:
            ist_parsent_delete(this->as.do_stmt.expr);
            break;
        default:
            isp_unreachable();
    }
    ist_astnode_delete(this);
}

ist_string ist_parsentptr_dump(ist_parsent* this, ist_dumpctx dctx) {
    dctx.idxptr = dctx.idxptr ?: (ist_usize[1]){};
    if (!this) isp_unreachable();
    return ist_parsent_dump(*this, dctx);
}

ist_string ist_parsent_dump(ist_parsent this, ist_dumpctx dctx) {
    isl_dreport(rid_inform_dumping, "astnode", this);
    dctx.idxptr = dctx.idxptr ?: (ist_usize[1]){};
    if (!this) return ist_strbuf_append_raw(dctx.buffer, dctx.idxptr, "null");

    // dctx.style |= DFLAG_HEAD_DOWRAP;

    switch (this->kind) {
        case ist_astnodeKind_unk:
            break;

        case ist_astnodeKind_scope: {
            ist_parsentList list = this->as.scope.stmts;
            return ist_dumpimage_dump(
                &ist_dumpimage_{
                    .name    = ist_astnodeKindNames[this->kind],
                    .wrapkey = "type",
                    .count   = 2,
                    ist_dumpitemar_{
                        {"location", ist_location_dump, &this->location},
                        {
                            "list",
                            isg_list_dumpack_dump,
                            &isg_list_dumpack_{
                                .name   = NULL,
                                .idxtag = "[%zu]",
                                .dowrap = true,
                                ist_parsentptr_dump,
                                &list,
                                ist_parsentList_capacity(&list),
                            },
                        },
                    },
                },
                dctx
            );
            break;
        }

        case ist_astnodeKind_literal: {
            ist_astnodeAs_literal literal = this->as.literal;
            return ist_dumpimage_dump(
                &ist_dumpimage_{
                    .name    = ist_astnodeKindNames[this->kind],
                    .wrapkey = "type",
                    .count   = 3,
                    ist_dumpitemar_{
                        {"location", ist_location_dump, &this->location},
                        {"valtype", ist_cstring_dump_ident, &ist_valueTypeNames[literal.this.type]},
                        {"value", ist_tvalue_dump, &literal.this},
                    },
                },
                dctx
            );
            break;
        }

        case ist_astnodeKind_expr: {
            ist_astnodeAs_expr expr = this->as.expr;
            switch (expr.kind) {
                case isn_psentExprKind_binary:
                    return ist_dumpimage_dump(
                        &ist_dumpimage_{
                            .name    = "binexpr",
                            .wrapkey = "type",
                            .count   = 4,
                            ist_dumpitemar_{
                                {"location", ist_location_dump, &this->location},
                                {"optype",
                                 ist_cstring_dump_ident,
                                 &ist_token_names[expr.as.binary.op]},
                                {"lhs", ist_parsent_dump, expr.as.binary.lhs},
                                {"rhs", ist_parsent_dump, expr.as.binary.rhs},
                            },
                        },
                        dctx
                    );
                    break;

                case isn_psentExprKind_unary:
                    return ist_dumpimage_dump(
                        &ist_dumpimage_{
                            .name    = "unexpr",
                            .wrapkey = "type",
                            .count   = 4,
                            ist_dumpitemar_{
                                {"location", ist_location_dump, &this->location},
                                {"optype",
                                 ist_cstring_dump_ident,
                                 &ist_token_names[expr.as.unary.op]},
                                {"lhs", ist_parsent_dump, expr.as.unary.lhs},
                                {"rhs", ist_parsent_dump, expr.as.unary.lhs},
                            },
                        },
                        dctx
                    );
                    break;


                case isn_psentExprKind_fncall:
                    return ist_dumpimage_dump(
                        &ist_dumpimage_{
                            .name    = "fncallexpr",
                            .wrapkey = "type",
                            .count   = 3,
                            ist_dumpitemar_{
                                {"location", ist_location_dump, &this->location},
                                {"fn", ist_parsent_dump, expr.as.fncall.fn},
                                {
                                    "arg_list",
                                    isg_list_dumpack_dump,
                                    &isg_list_dumpack_{
                                        .name   = NULL,
                                        .idxtag = "[%zu]",
                                        .dowrap = true,
                                        ist_parsentptr_dump,
                                        &expr.as.fncall.args,
                                        ist_parsentList_capacity(&expr.as.fncall.args),
                                    },
                                },
                            },
                        },
                        dctx
                    );
                    break;
                default:
                    isp_unreachable();
                    break;
            }
        }
        case ist_astnodeKind_name: {
            ist_astnodeAs_name name = this->as.name;
            return ist_dumpimage_dump(
                &ist_dumpimage_{
                    .name    = ist_astnodeKindNames[this->kind],
                    .wrapkey = "type",
                    .count   = 2,
                    ist_dumpitemar_{
                        {"location", ist_location_dump, &this->location},
                        {"name", ist_cstring_dump, &name.name},
                    },
                },
                dctx
            );
            break;
        }

        case ist_astnodeKind_use_stmt: {
            ist_astnodeAs_use_stmt stmt = this->as.use_stmt;
            return ist_dumpimage_dump(
                &ist_dumpimage_{
                    .name    = ist_astnodeKindNames[this->kind],
                    .wrapkey = "type",
                    .count   = 3,
                    ist_dumpitemar_{
                        {"location", ist_location_dump, &this->location},
                        {"lhs", ist_parsent_dump, stmt.lhs},
                        {"rhs", ist_parsent_dump, stmt.rhs},
                    },
                },
                dctx
            );
            break;
        }

        default:
            isp_unreachable();
            break;
    }

    return *dctx.buffer;
}
