#include "isl_parser.h"

inline ist_parser ist_parser_consby_lexer(ist_lexer _lexer) {
    return (ist_parser){
        .lexer  = _lexer,
        .module = _lexer.module,
        .root   = NULL,
    };
}
_isl_define_initby_createby_with_consby((ist_lexer _lexer), (_lexer), ist_parser, lexer);

ist_parser ist_parser_consby_module(ist_module* _module) {
    return ist_parser_consby_lexer(ist_lexer_consby_module(_module));
}
_isl_define_initby_createby_with_consby((ist_module * _module), (_module), ist_parser, module);


void ist_parser_clean(ist_parser* this) {
    isl_ifnreport(this, rid_catch_nullptr, isp_catch_coreloc);
    if (this->root)
        isg_list_foreach (itp, this->root->as.scope.stmts) ist_parsent_delete(*itp);
    ist_lexer_clean(&this->lexer);
}

void ist_parser_delete(ist_parser* this) {
    if (!this) return;
    ist_parser_clean(this);
    isl_free(this);
}

enum ist_optbindpower;
typedef enum ist_optbindpower ist_optbindpower;


ist_token advance(ist_parser* this);
ist_bool  match_token(ist_parser* this, ist_tokenType _type);


ist_parseYield parse(ist_parser* this);

/* parse statment */
ist_parseYield parse_stmts(ist_parser* this);

ist_parseYield parse_stmt(ist_parser* this);
ist_parseYield parse_import_stmt(ist_parser* this);
ist_parseYield parse_use_stmt(ist_parser* this);
ist_parseYield parse_do_stmt(ist_parser* this);

/* parse common pattern */
ist_parseYield parse_codetail_patt(ist_parser* this);
ist_parseYield parse_codeblock_patt(ist_parser* this);
ist_parseYield parse_paramlist_patt(ist_parser* this);
ist_parseYield parse_arglist_patt(ist_parser* this);

/* parse expression */
ist_parseYield parse_expr(ist_parser* this, ist_optbindpower lhsrbp);

ist_parseYield parse_nud_literal(ist_parser* this);
ist_parseYield parse_nud_name(ist_parser* this);
ist_parseYield parse_nud_prefix_expr(ist_parser* this);

ist_parseYield parse_led_suffix_expr(ist_parser* this, ist_astnode* lhs);
ist_parseYield parse_led_infix_expr(ist_parser* this, ist_astnode* lhs);
ist_parseYield parse_led_fncall_expr(ist_parser* this, ist_astnode* lhs);
ist_parseYield parse_led_wrap_expr(ist_parser* this, ist_astnode* lhs);



#define pre_token(this) ((this)->lexer.pre_token)
#define cur_token(this) ((this)->lexer.cur_token)
#define nex_token(this) ((this)->lexer.nex_token)
#define sec_token(this) ((this)->lexer.sec_token)


/**
 * handle ahead state, when aheading, do not report
 * anything about failure, just return to caller to notify it.
 */
#define _handle_aheading(this, _node)                                       \
    do {                                                                    \
        if (ist_lexer_islookahead(&(this)->lexer))                          \
            return ist_parseYield_{(_node), isn_pasreYieldStatus_aheading}; \
    } while (0)

/* raise the parsing fail, and report the error message */
#define raise_parsing_fail(this, _node, _rid, _rptvargs...)             \
    do {                                                                \
        _handle_aheading(this, _node);                                  \
        isl_report(_rid, ##_rptvargs);                                  \
        return ist_parseYield_{(_node), isn_pasreYieldStatus_reported}; \
    } while (0)

/**
 * handle parsing state in force
 * this macro is designed for reporting the error message when the parsing failed.
 */
#define parse_force(this, _fncall, _rid, _rptvargs...)           \
    ({                                                           \
        ist_parseYield ___yield = _fncall;                       \
        _handle_aheading((this), ___yield.ok);                   \
        switch (___yield.status) {                               \
            case isn_pasreYieldStatus_unreported:                \
                isl_report(_rid, ##_rptvargs);                   \
                ___yield.status = isn_pasreYieldStatus_reported; \
            case isn_pasreYieldStatus_reported:                  \
            case isn_pasreYieldStatus_aheading:                  \
                return ___yield;                                 \
            case isn_pasreYieldStatus_success:                   \
                break;                                           \
            default:                                             \
                isp_unreachable();                               \
        }                                                        \
        ___yield.ok;                                             \
    })

/* handle parsing state in inert */
#define parse_inert(this, _fncall)            \
    ({                                        \
        ist_parseYield ___yield = _fncall;    \
        if (___yield.status) return ___yield; \
        ___yield.ok;                          \
    })

#define parse_ahead(this, _fncall)                 \
    ({                                             \
        ist_lexer_lookahead_start(&(this)->lexer); \
        ist_parseYield ___yield = _fncall;         \
        ist_lexer_lookahead_end(&(this)->lexer);   \
        ___yield;                                  \
    })


#define assert_token(this, _node, _type)              \
    do {                                              \
        if (!match_token(this, _type)) {              \
            raise_parsing_fail(                       \
                this,                                 \
                _node,                                \
                rid_assert_tokentype_failed,          \
                cur_token(this).location,             \
                ist_token_names[_type],               \
                ist_token_names[cur_token(this).type] \
            );                                        \
        }                                             \
    } while (0)



ist_token advance(ist_parser* this) {
    ist_lexer_advance(&this->lexer);
    return pre_token(this);
}

ist_bool match_token(ist_parser* this, ist_tokenType _type) {
    if (cur_token(this).type != _type) return false;
    advance(this);
    return true;
}


/**
 * Operator binding power, the higher the value, the higher the priority.
 * The values ​​of two adjacent enumerations must differ by at least two, in order to ensure
 * that there is room for the associative adjustment.
 */
enum ist_optbindpower {
    OBP_NONE      = 0x0,       // reserved for certain suffix operator.
    OBP_LOWEST    = 0x1,       // reserved for parse enterance.
    OBP_RPARE     = 0x2,       // reserved for rpare.
    OBP_ASSIGN    = 0x10,      // = += -= *= /= %= ^= |= &= ...
    OBP_CONDITION = 0x20,      // ?:
    OBP_LOGIC_OR  = 0x30,      // ||
    OBP_LOGIC_AND = 0x40,      // &&
    OBP_EQUALITY  = 0x50,      // == !=
    OBP_COMPARE   = 0x60,      // < > <= >= <=>
    OBP_BIT_OR    = 0x70,      // |
    OBP_BIT_AND   = 0x80,      // &
    BOP_BIT_SHIFT = 0x90,      // << >>
    OBP_ARITH     = 0xA0,      // + -
    OBP_TERM      = 0xB0,      // * / %
    OBP_FACTOR    = 0xC0,      // ^
    OBP_PREFIX    = 0xD0,      // ++ -- * & ! ~
    OBP_SUFFIX    = 0xE0,      // ++ -- * & ^ !
    OBP_CALL      = 0xF0,      // (...) [...] . ->
    OBP_ATOM      = 0XFFF,     // reserved for identifier or unit.
    OBP_HIGHEST   = INT16_MAX, // highest of i16.
};


/* entrance method for parser */
ist_parseYield ist_parser_parse(ist_parser* this) {
    if (!this) isp_unreachable();
    if (!this->root)
        this->root = ist_astnode_createby_full(
            ist_astnodeKind_scope,
            cur_token(this).location,
            ist_astnodeAs_{
                .scope.stmts = ist_parsentList_consm(4),
            }
        );

    while (cur_token(this).type != ISL_TOKENT_EOF) {
        ist_astnode* node = parse_force(this, parse(this), rid_parse_error);
        if (node) ist_parsentList_addm(&this->root->as.scope.stmts, node);
    }

    return ist_parseYield_{this->root};
}

ist_parseYield parse(ist_parser* this) {
    ist_astnode* node = NULL;

    while (match_token(this, ISL_TOKENT_EOS));
    switch (cur_token(this).type) {
        case ISL_TOKENT_KW_USE:
        case ISL_TOKENT_KW_DO:
            node = parse_force(this, parse_stmt(this), rid_expect_parsent_stmt);
            break;
        default:
            node = parse_force(this, parse_expr(this, OBP_LOWEST), rid_expect_parsent_expr);
            break;
    }

    return ist_parseYield_{node};
}

ist_parseYield parse_stmts(ist_parser* this) {
    ist_astnode* stmts = ist_astnode_createby_full(
        ist_astnodeKind_list,
        cur_token(this).location,
        ist_astnodeAs_{
            .list.this = ist_parsentList_consm(4),
        }
    );

    while (cur_token(this).type != ISL_TOKENT_EOF || cur_token(this).type != ISL_TOKENT_RBRACE) {
        ist_parseYield yield = parse_stmt(this);
        if (yield.ok) ist_parsentList_addm(&stmts->as.list.this, yield.ok);
        switch (yield.status) {
            case isn_pasreYieldStatus_aheading:
                return ist_parseYield_{stmts, isn_pasreYieldStatus_aheading};
            case isn_pasreYieldStatus_unreported:
                isl_report(rid_expect_stmt_parsent);
            case isn_pasreYieldStatus_reported:
                while (pre_token(this).type != ISL_TOKENT_EOS) advance(this);
            case isn_pasreYieldStatus_success:
                break;
            default:
                isp_unreachable();
        }
    }
    return ist_parseYield_{stmts};
}

ist_parseYield parse_stmt(ist_parser* this) {
    ist_astnode* node = NULL;

    while (match_token(this, ISL_TOKENT_EOS));
    switch (cur_token(this).type) {
        case ISL_TOKENT_KW_USE:
            node = parse_force(this, parse_use_stmt(this), rid_expect_parsent_use_stmt);
            break;
        default:
            isp_unreachable();
    }
    match_token(this, ISL_TOKENT_EOS);
    return ist_parseYield_{node};
}

ist_parseYield parse_use_stmt(ist_parser* this) {
    // BUGFIX: there implies a terrible case: when parse_force do return, the `node` will be leak!
    assert_token(this, NULL, ISL_TOKENT_KW_USE);
    ist_astnode* node = ist_astnode_createby_full(
        ist_astnodeKind_use_stmt,
        cur_token(this).location,
        ist_astnodeAs_{.use_stmt.is_assign = true}
    );
    node->as.use_stmt.lhs = parse_force(this, parse_nud_name(this), rid_expect_parsent_name);
    assert_token(this, node, ISL_TOKENT_ASSIGN);
    node->as.use_stmt.rhs = parse_force(this, parse(this), rid_expect_parsent_expr);
    return ist_parseYield_{node};
}


ist_parseYield parse_do_stmt(ist_parser* this) {
    assert_token(this, NULL, ISL_TOKENT_KW_DO);
    return ist_parseYield_{ist_astnode_createby_full(
        ist_astnodeKind_do_stmt,
        cur_token(this).location,
        ist_astnodeAs_{
            .do_stmt.expr =
                parse_force(this, parse_expr(this, OBP_LOWEST), rid_expect_parsent_expr),
        }
    )};
}


/**
 * This concept come form TDOP(aka Top Down Operator Precedence / Pratt Parser)
 * pletnud: parselet of nud (null denotation)
 * pletled: parselet of led (left denotation)
 */
typedef ist_parseYield (*ist_pletnud_fn)(ist_parser*);
typedef ist_parseYield (*ist_pletled_fn)(ist_parser*, ist_astnode*);

/* nud operator attributes */
struct ist_nudoptattr {

    ist_pletnud_fn   nud;
    ist_optbindpower rbp: 16;

} nudoptattrs[] = {

    [ISL_TOKENT_ADD]     = {parse_nud_prefix_expr, OBP_PREFIX},
    [ISL_TOKENT_SUB]     = {parse_nud_prefix_expr, OBP_PREFIX},
    [ISL_TOKENT_SELFADD] = {parse_nud_prefix_expr, OBP_PREFIX},
    [ISL_TOKENT_SELFSUB] = {parse_nud_prefix_expr, OBP_PREFIX},

    [ISL_TOKENT_VL_INT]    = {parse_nud_literal, OBP_NONE},
    [ISL_TOKENT_VL_REAL]   = {parse_nud_literal, OBP_NONE},
    [ISL_TOKENT_VL_STRING] = {parse_nud_literal, OBP_NONE},
    [ISL_TOKENT_BV_FALSE]  = {parse_nud_literal, OBP_NONE},
    [ISL_TOKENT_BV_TRUE]   = {parse_nud_literal, OBP_NONE},
    [ISL_TOKENT_ID]        = {parse_nud_name, OBP_NONE},

    [ISL_TOKENT_LATEST] = {NULL, OBP_NONE},

};

/* led operator attributes */
struct ist_ledoptattr {

    ist_pletled_fn   led;
    ist_optbindpower lbp: 16;
    ist_optbindpower rbp: 16;

} ledoptattrs[] = {

    [ISL_TOKENT_RPARE] = {NULL, OBP_RPARE, OBP_NONE},

    [ISL_TOKENT_ASSIGN]     = {parse_led_infix_expr, OBP_ASSIGN + 1, OBP_ASSIGN},
    [ISL_TOKENT_ADD_ASSIGN] = {parse_led_infix_expr, OBP_ASSIGN + 1, OBP_ASSIGN},
    [ISL_TOKENT_SUB_ASSIGN] = {parse_led_infix_expr, OBP_ASSIGN + 1, OBP_ASSIGN},
    [ISL_TOKENT_MUL_ASSIGN] = {parse_led_infix_expr, OBP_ASSIGN + 1, OBP_ASSIGN},
    [ISL_TOKENT_DIV_ASSIGN] = {parse_led_infix_expr, OBP_ASSIGN + 1, OBP_ASSIGN},
    [ISL_TOKENT_MOD_ASSIGN] = {parse_led_infix_expr, OBP_ASSIGN + 1, OBP_ASSIGN},

    [ISL_TOKENT_ADD] = {parse_led_infix_expr, OBP_ARITH, OBP_ARITH},
    [ISL_TOKENT_SUB] = {parse_led_infix_expr, OBP_ARITH, OBP_ARITH},
    [ISL_TOKENT_MUL] = {parse_led_infix_expr, OBP_TERM, OBP_TERM},
    [ISL_TOKENT_DIV] = {parse_led_infix_expr, OBP_TERM, OBP_TERM},
    [ISL_TOKENT_MOD] = {parse_led_infix_expr, OBP_TERM, OBP_TERM},

    [ISL_TOKENT_SELFADD] = {parse_led_suffix_expr, OBP_PREFIX, OBP_NONE},
    [ISL_TOKENT_SELFSUB] = {parse_led_suffix_expr, OBP_PREFIX, OBP_NONE},

    [ISL_TOKENT_LPARE]   = {parse_led_fncall_expr, OBP_SUFFIX, OBP_NONE},
    [ISL_TOKENT_WRAPPER] = {parse_led_wrap_expr, OBP_SUFFIX, OBP_ATOM},

    [ISL_TOKENT_LATEST] = {NULL, OBP_NONE, OBP_NONE},

};

ist_parseYield parse_expr(ist_parser* this, ist_optbindpower lhsrbp) {
    ist_parseYield yield = {};

    ist_token curtoken = cur_token(this);

    if (curtoken.type == ISL_TOKENT_LPARE) {

        /* handle paren */
        advance(this);
        yield.ok = parse_force(
            this,
            parse_expr(this, OBP_LOWEST),
            rid_expect_parsent_expr_after,
            curtoken.location,
            ist_token_names[curtoken.type]
        );

        assert_token(this, yield.ok, ISL_TOKENT_RPARE);

    } else if (nudoptattrs[curtoken.type].nud)
        yield.ok = parse_inert(this, nudoptattrs[curtoken.type].nud(this));

    else if (match_token(this, ISL_TOKENT_EOS)) return yield;
    else if (match_token(this, ISL_TOKENT_EOF)) return yield;
    else
        raise_parsing_fail(
            (this),
            yield.ok,
            rid_expect_nud_failed,
            curtoken.location,
            ist_token_names[curtoken.type]
        );

    /**
     * if left-hand-side's right-binding-power is less than the current token's
     * left-binding-power, then we should make sure the node that parsing by prefix
     * will belong to the node of the current token.
     */
    while (lhsrbp < ledoptattrs[cur_token(this).type].lbp) {
        curtoken = cur_token(this);

        /* this for infix or suffix recognizing and parsing */
        if (!ledoptattrs[curtoken.type].led) break;

        yield.ok = parse_force(
            this,
            ledoptattrs[curtoken.type].led(this, yield.ok),
            rid_expect_parsent_expr_after,
            curtoken.location,
            ist_token_names[curtoken.type]
        );
    }

    return yield;
}


ist_parseYield parse_nud_literal(ist_parser* this) {
    switch (cur_token(this).type) {
        case ISL_TOKENT_VL_INT:
        case ISL_TOKENT_VL_REAL:
        case ISL_TOKENT_VL_STRING:
            advance(this);
            break;
        default:
            raise_parsing_fail(
                (this),
                (NULL),
                rid_expect_parsent_literal_but,
                ist_token_names[cur_token(this).type]
            );
    }
    return ist_parseYield_{
        ist_astnode_createby_full(
            ist_astnodeKind_literal,
            pre_token(this).location,
            ist_astnodeAs_{
                .literal.this =
                    ist_tvalue_{
                        .type = isl_toklitype_to_valtype[pre_token(this).type],
                        .data = pre_token(this).value,
                    },
            }
        ),
    };
}

ist_parseYield parse_nud_name(ist_parser* this) {
    assert_token(this, NULL, ISL_TOKENT_ID);
    ist_string id = ist_string_consby_ref(pre_token(this).extract, pre_token(this).length);
    ist_module_register_string(this->module, id, ISL_MOSKIND_IDENTIFER);
    return ist_parseYield_{
        ist_astnode_createby_full(
            ist_astnodeKind_name,
            pre_token(this).location,
            ist_astnodeAs_{
                .name.name = id,
            }
        ),
    };
}

ist_parseYield parse_nud_prefix_expr(ist_parser* this) {
    ist_token optok = advance(this);
    return ist_parseYield_{
        ist_astnode_createby_full(
            ist_astnodeKind_expr,
            optok.location,
            ist_astnodeAs_{
                .expr.kind = isn_psentExprKind_unary,

                .expr.as.unary.op  = optok.type,
                .expr.as.unary.rhs = parse_force(
                    this,
                    parse_expr(this, nudoptattrs[optok.type].rbp),
                    rid_expect_parsent_expr_after,
                    optok.location,
                    ist_token_names[optok.type]
                ),
            }
        ),
    };
}

ist_parseYield parse_led_suffix_expr(ist_parser* this, ist_astnode* lhs) {
    ist_token optok = advance(this);
    return ist_parseYield_{
        ist_astnode_createby_full(
            ist_astnodeKind_expr,
            optok.location,
            ist_astnodeAs_{
                .expr.kind = isn_psentExprKind_unary,

                .expr.as.unary.op  = optok.type,
                .expr.as.unary.lhs = lhs,
            }
        ),
    };
}

ist_parseYield parse_led_infix_expr(ist_parser* this, ist_astnode* lhs) {
    ist_token optok = advance(this);
    return ist_parseYield_{
        ist_astnode_createby_full(
            ist_astnodeKind_expr,
            optok.location,
            ist_astnodeAs_{
                .expr.kind = isn_psentExprKind_binary,

                .expr.as.binary.op  = optok.type,
                .expr.as.binary.lhs = lhs,
                .expr.as.binary.rhs = parse_force(
                    this,
                    parse_expr(this, ledoptattrs[optok.type].rbp),
                    rid_expect_parsent_expr_after,
                    optok.location,
                    ist_token_names[optok.type]
                ),
            }
        ),
    };
}

ist_parseYield parse_led_fncall_expr(ist_parser* this, ist_astnode* lhs) {
    ist_token optok = advance(this);
    return ist_parseYield_{
        ist_astnode_createby_full(
            ist_astnodeKind_expr,
            optok.location,
            ist_astnodeAs_{
                .expr.kind = isn_psentExprKind_fncall,

                .expr.as.fncall.fn   = lhs,
                .expr.as.fncall.args = ({
                    ist_parsentList arglist = ist_parsentList_consc(2);
                    do {
                        ist_token curtoken = cur_token(this);
                        ist_parsentList_addc(
                            &arglist,
                            parse_force(
                                this,
                                parse_expr(this, OBP_LOWEST),
                                rid_expect_parsent_expr_after,
                                curtoken.location,
                                ist_token_names[curtoken.type]
                            )
                        );
                    } while (match_token(this, ISL_TOKENT_COMMA));
                    assert_token(this, NULL, ISL_TOKENT_RPARE);
                    arglist;
                })
            }
        ),
    };
}

ist_parseYield parse_led_wrap_expr(ist_parser* this, ist_astnode* lhs) {
    ist_token curtoken = advance(this);
    return ist_parseYield_{
        ist_astnode_createby_full(
            ist_astnodeKind_expr,
            curtoken.location,
            ist_astnodeAs_{
                .expr.kind = isn_psentExprKind_fncall,

                .expr.as.fncall.fn = parse_force(
                    this,
                    parse_expr(this, ledoptattrs[curtoken.type].rbp),
                    rid_expect_fn_entity_after,
                    curtoken.location,
                    ist_token_names[curtoken.type]
                ),
                .expr.as.fncall.args = ({
                    ist_parsentList arglist = ist_parsentList_consm(2);
                    ist_parsentList_addm(&arglist, lhs);
                    do {
                        if (!match_token(this, ISL_TOKENT_LPARE)) break;
                        if (match_token(this, ISL_TOKENT_RPARE)) break;
                        do {
                            curtoken = cur_token(this);
                            ist_parsentList_addc(
                                &arglist,
                                parse_force(
                                    this,
                                    parse_expr(this, OBP_LOWEST),
                                    rid_expect_parsent_expr_after,
                                    curtoken.location,
                                    ist_token_names[curtoken.type]
                                )
                            );
                        } while (match_token(this, ISL_TOKENT_COMMA));
                        assert_token(this, NULL, ISL_TOKENT_RPARE);
                    } while (0);
                    arglist;
                })
            }
        ),
    };
}
