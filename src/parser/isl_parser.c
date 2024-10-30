#include "isl_parser.h"

inline ist_parser ist_parser_consby_full(ist_lexer _lexer) {
    return (ist_parser){.lexer = _lexer, .root = NULL, .pstate = 0};
}
ist_parser ist_parser_consby_module(ist_module* _module) {
    return ist_parser_consby_full(ist_lexer_consby_module(_module));
}


ist_parser* ist_parser_initby_full(ist_parser* this, ist_lexer _lexer) {
    *this = ist_parser_consby_full(_lexer);
    return this;
}
inline ist_parser* ist_parser_initby_module(ist_parser* this, ist_module* _module) {
    *this = ist_parser_consby_module(_module);
    return this;
}

inline ist_parser* ist_parser_createby_full(ist_lexer _lexer) {
    return ist_parser_initby_full(isl_malloc(ist_parser), _lexer);
}
inline ist_parser* ist_parser_createby_module(ist_module* _module) {
    return ist_parser_initby_module(isl_malloc(ist_parser), _module);
}


void ist_parser_clean(ist_parser* this) {
    isl_ifnreport(this, rid_catch_nullptr, isp_catch_coreloc);
    if (this->root) ist_ast_delete(this->root);
    ist_lexer_clean(&this->lexer);
}

void ist_parser_delete(ist_parser* this) {
    ist_parser_clean(this);
    isl_free(this);
}



typedef enum ist_optbindpower {
    OBP_NONE    = 0x0,
    OBP_LOWEST  = 0x1,
    OBP_ASSIGN  = 0x20,
    OBP_ARITH   = 0x30,
    OBP_TERM    = 0x40,
    OBP_FACTOR  = 0x50,
    OBP_PREFIX  = 0x60,
    OBP_SUFFIX  = 0x70,
    OBP_PAREN   = 0xFF,
    OBP_HIGHEST = INT16_MAX,
} ist_optbindpower;


typedef void* (*ist_pletnud_fn)(ist_parser*);
typedef void* (*ist_pletled_fn)(ist_parser*, ist_astnode*);


void* parse_expr(ist_parser* this, ist_optbindpower minbp);

void* nud_literal_ent(ist_parser* this);
void* nud_prefix(ist_parser* this);
void* led_infix(ist_parser* this, ist_astnode* lhs);



#define pre_token(this) ((this)->lexer.pre_token)
#define cur_token(this) ((this)->lexer.cur_token)
#define nex_token(this) ((this)->lexer.nex_token)
#define sec_token(this) ((this)->lexer.sec_token)

#define handle_aheading(this, _node)                 \
    do {                                             \
        if (ist_lexer_islookahead(&(this)->lexer)) { \
            (this)->pstate = PRS_FAHEAD;             \
            return _node;                            \
        }                                            \
    } while (0)

#define handle_pstate_inert(this, _node)  \
    do {                                  \
        if ((this)->pstate) return _node; \
    } while (0)

/* handle presult state in force */
#define handle_pstate_force(this, _node, _rid, _rptvargs...) \
    do {                                                     \
        switch ((this)->pstate) {                            \
            case PRS_FUNREPROTED:                            \
                handle_aheading(this, _node);                \
                isl_report(_rid, _rptvargs);                 \
            case PRS_FREPROTED:                              \
            case PRS_FAHEAD:                                 \
                return _node;                                \
            case PRS_SUCCESS:                                \
                break;                                       \
        }                                                    \
    } while (0)

#define parse_failed(this, _node, _rid, _rptvargs...) \
    do {                                              \
        handle_aheading(this, _node);                 \
        isl_report(_rid, _rptvargs);                  \
        (this)->pstate = PRS_FREPROTED;               \
        return _node;                                 \
    } while (0)

#define assert_token(this, _node, _type)              \
    do {                                              \
        if (!match_token(this, _type)) {              \
            parse_failed(                             \
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

ist_bool match_token(ist_parser* this, ist_token_type _type) {
    if (cur_token(this).type != _type) return false;
    advance(this);
    return true;
}


struct ist_opttattr {

    ist_pletnud_fn   nud;
    ist_pletled_fn   led;
    ist_optbindpower lbp: 16;
    ist_optbindpower rbp: 16;

} opttattrs[] = {
    [ISL_TOKENT_LPARE] = {NULL, NULL, OBP_NONE, OBP_NONE},
    [ISL_TOKENT_RPARE] = {NULL, NULL, OBP_NONE, OBP_NONE},

    [ISL_TOKENT_ASSIGN]     = {NULL, NULL, OBP_ASSIGN + 1, OBP_ASSIGN},
    [ISL_TOKENT_ADD_ASSIGN] = {NULL, NULL, OBP_ASSIGN + 1, OBP_ASSIGN},
    [ISL_TOKENT_SUB_ASSIGN] = {NULL, NULL, OBP_ASSIGN + 1, OBP_ASSIGN},
    [ISL_TOKENT_MUL_ASSIGN] = {NULL, NULL, OBP_ASSIGN + 1, OBP_ASSIGN},
    [ISL_TOKENT_DIV_ASSIGN] = {NULL, NULL, OBP_ASSIGN + 1, OBP_ASSIGN},
    [ISL_TOKENT_MOD_ASSIGN] = {NULL, NULL, OBP_ASSIGN + 1, OBP_ASSIGN},

    [ISL_TOKENT_ADD] = {nud_prefix, led_infix, OBP_ARITH, OBP_ARITH + 1},
    [ISL_TOKENT_SUB] = {nud_prefix, led_infix, OBP_ARITH, OBP_ARITH + 1},

    [ISL_TOKENT_MUL] = {NULL, led_infix, OBP_TERM, OBP_TERM + 1},
    [ISL_TOKENT_DIV] = {NULL, led_infix, OBP_TERM, OBP_TERM + 1},
    [ISL_TOKENT_MOD] = {NULL, led_infix, OBP_TERM, OBP_TERM + 1},

    [ISL_TOKENT_VL_INT]    = {nud_literal_ent, NULL, OBP_NONE, OBP_NONE},
    [ISL_TOKENT_VL_REAL]   = {nud_literal_ent, NULL, OBP_NONE, OBP_NONE},
    [ISL_TOKENT_VL_STRING] = {nud_literal_ent, NULL, OBP_NONE, OBP_NONE},
    [ISL_TOKENT_BV_FALSE]  = {nud_literal_ent, NULL, OBP_NONE, OBP_NONE},
    [ISL_TOKENT_BV_TRUE]   = {nud_literal_ent, NULL, OBP_NONE, OBP_NONE},

    [ISL_TOKENT_LATEST] = {NULL, NULL, OBP_NONE, OBP_NONE},
};

void ist_parser_parse(ist_parser* this) {
    this->root = parse_expr(this, OBP_LOWEST);
}

void* parse_expr(ist_parser* this, ist_optbindpower minbp) {
    ist_token curtoken = cur_token(this);
    void*     lhs;

    if (opttattrs[curtoken.type].nud) {

        lhs = opttattrs[curtoken.type].nud(this);
        handle_pstate_inert(this, lhs);

    } else if (curtoken.type == ISL_TOKENT_LPARE) {

        advance(this);
        lhs = parse_expr(this, OBP_LOWEST);
        assert_token(this, lhs, ISL_TOKENT_RPARE);
    } else
        parse_failed(
            this,
            NULL,
            rid_expect_expression_before,
            curtoken.location,
            ist_token_names[curtoken.type]
        );

    while (minbp < opttattrs[cur_token(this).type].lbp) {
        curtoken = cur_token(this);
        if (!opttattrs[curtoken.type].led) break;

        lhs = opttattrs[curtoken.type].led(this, lhs);
        handle_pstate_force(
            this,
            lhs,
            rid_expect_expression_after,
            curtoken.location,
            ist_token_names[curtoken.type]
        );
    }
    return lhs;
}


void* nud_literal_ent(ist_parser* this) {
    ist_token curtoken = advance(this);

    return ist_astnode_createby_full(LITERAL_ENT, curtoken.location, {
        __result__->value  = curtoken.value;
        __result__->litype = curtoken.type;
    });
}

void* nud_prefix(ist_parser* this) {
    ist_token curtoken = advance(this);

    IST_ASTNODE_UNARY_OPT* node = ist_astnode_createby_full(UNARY_OPT, curtoken.location, {
        __result__->onlhs    = true;
        __result__->optype   = curtoken.type;
        __result__->sub_node = parse_expr(this, OBP_PREFIX);
    });

    handle_pstate_force(
        this, node, rid_expect_expression_after, curtoken.location, ist_token_names[curtoken.type]
    );
    return node;
}

void* led_infix(ist_parser* this, ist_astnode* lhs) {
    ist_token curtoken = advance(this);

    IST_ASTNODE_BINARY_OPT* node = ist_astnode_createby_full(BINARY_OPT, curtoken.location, {
        __result__->lhs_node = lhs;
        __result__->optype   = curtoken.type;
        __result__->rhs_node = parse_expr(this, opttattrs[curtoken.type].rbp);
    });

    handle_pstate_force(
        this, node, rid_expect_expression_after, curtoken.location, ist_token_names[curtoken.type]
    );
    return node;
}
