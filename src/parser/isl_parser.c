#include "isl_parser.h"

inline ist_parser ist_parser_consby_full(ist_lexer _lexer) {
    return (ist_parser){.lexer = _lexer, .root = NULL, .pstate = PRS_SUCCESS};
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
    OBP_ATOM    = 0xFF,
    OBP_HIGHEST = INT16_MAX,
} ist_optbindpower;


typedef void* (*ist_pletnud_fn)(ist_parser*);
typedef void* (*ist_pletled_fn)(ist_parser*, ist_astnode*);


void* parse_expr(ist_parser* this, ist_optbindpower lhsrbp);

void* nud_literal_entity(ist_parser* this);
void* nud_name_entity(ist_parser* this);
void* nud_prefix_opt(ist_parser* this);
void* led_suffix_opt(ist_parser* this, ist_astnode* lhs);
void* led_infix_opt(ist_parser* this, ist_astnode* lhs);
void* led_wrap_opt(ist_parser* this, ist_astnode* lhs);


#define pre_token(this) ((this)->lexer.pre_token)
#define cur_token(this) ((this)->lexer.cur_token)
#define nex_token(this) ((this)->lexer.nex_token)
#define sec_token(this) ((this)->lexer.sec_token)


/**
 * handle ahead state, when aheading, do not report
 * anything about failure, just return to caller to notify it.
 */
#define handle_aheading(this, _node)                 \
    do {                                             \
        if (ist_lexer_islookahead(&(this)->lexer)) { \
            (this)->pstate = PRS_FAHEAD;             \
            return _node;                            \
        }                                            \
    } while (0)

/* handle parsing state in inert */
#define handle_pstate_inert(this, _node)  \
    do {                                  \
        if ((this)->pstate) return _node; \
    } while (0)

/**
 * handle parsing state in force
 * this macro is designed for reporting the error message when the parsing failed.
 */
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

/**
 * raise the parsing failed, and report the error message.
 */
#define raise_parsing_failed(this, _node, _rid, _rptvargs...) \
    do {                                                      \
        handle_aheading(this, _node);                         \
        isl_report(_rid, _rptvargs);                          \
        (this)->pstate = PRS_FREPROTED;                       \
        return _node;                                         \
    } while (0)

#define assert_token(this, _node, _type)              \
    do {                                              \
        if (!match_token(this, _type)) {              \
            raise_parsing_failed(                     \
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



struct ist_prefix_optattr {

    ist_pletnud_fn   nud;
    ist_optbindpower rbp: 16;

} prefix_optattrs[] = {

    [ISL_TOKENT_ADD]     = {nud_prefix_opt, OBP_PREFIX},
    [ISL_TOKENT_SUB]     = {nud_prefix_opt, OBP_PREFIX},
    [ISL_TOKENT_SELFADD] = {nud_prefix_opt, OBP_PREFIX},
    [ISL_TOKENT_SELFSUB] = {nud_prefix_opt, OBP_PREFIX},

    [ISL_TOKENT_VL_INT]    = {nud_literal_entity, OBP_NONE},
    [ISL_TOKENT_VL_REAL]   = {nud_literal_entity, OBP_NONE},
    [ISL_TOKENT_VL_STRING] = {nud_literal_entity, OBP_NONE},
    [ISL_TOKENT_BV_FALSE]  = {nud_literal_entity, OBP_NONE},
    [ISL_TOKENT_BV_TRUE]   = {nud_literal_entity, OBP_NONE},
    [ISL_TOKENT_ID]        = {nud_name_entity, OBP_NONE},

    [ISL_TOKENT_LATEST] = {NULL, OBP_NONE},

};

struct ist_infix_optattr {

    ist_pletled_fn   led;
    ist_optbindpower lbp: 16;
    ist_optbindpower rbp: 16;

} infix_optattrs[] = {

    [ISL_TOKENT_ASSIGN]     = {led_infix_opt, OBP_ASSIGN + 1, OBP_ASSIGN},
    [ISL_TOKENT_ADD_ASSIGN] = {led_infix_opt, OBP_ASSIGN + 1, OBP_ASSIGN},
    [ISL_TOKENT_SUB_ASSIGN] = {led_infix_opt, OBP_ASSIGN + 1, OBP_ASSIGN},
    [ISL_TOKENT_MUL_ASSIGN] = {led_infix_opt, OBP_ASSIGN + 1, OBP_ASSIGN},
    [ISL_TOKENT_DIV_ASSIGN] = {led_infix_opt, OBP_ASSIGN + 1, OBP_ASSIGN},
    [ISL_TOKENT_MOD_ASSIGN] = {led_infix_opt, OBP_ASSIGN + 1, OBP_ASSIGN},

    [ISL_TOKENT_ADD] = {led_infix_opt, OBP_ARITH, OBP_ARITH + 1},
    [ISL_TOKENT_SUB] = {led_infix_opt, OBP_ARITH, OBP_ARITH + 1},
    [ISL_TOKENT_MUL] = {led_infix_opt, OBP_TERM, OBP_TERM + 1},
    [ISL_TOKENT_DIV] = {led_infix_opt, OBP_TERM, OBP_TERM + 1},
    [ISL_TOKENT_MOD] = {led_infix_opt, OBP_TERM, OBP_TERM + 1},

    [ISL_TOKENT_WRAPPER] = {led_wrap_opt, OBP_SUFFIX, OBP_SUFFIX},

    [ISL_TOKENT_LATEST] = {NULL, OBP_NONE, OBP_NONE},

};

struct ist_suffix_optattr {

    ist_pletled_fn   led;
    ist_optbindpower lbp: 16;

} suffix_optattrs[] = {

    [ISL_TOKENT_SELFADD] = {led_suffix_opt, OBP_PREFIX},
    [ISL_TOKENT_SELFSUB] = {led_suffix_opt, OBP_PREFIX},

    [ISL_TOKENT_LATEST] = {NULL, OBP_NONE},

};


void ist_parser_parse(ist_parser* this) {
    this->root = parse_expr(this, OBP_LOWEST);
}

void* parse_expr(ist_parser* this, ist_optbindpower lhsrbp) {
    ist_token curtoken = cur_token(this);

    void* node;

    if (prefix_optattrs[curtoken.type].nud) {

        node = prefix_optattrs[curtoken.type].nud(this);
        handle_pstate_inert(this, node);

    } else if (curtoken.type == ISL_TOKENT_LPARE) {

        advance(this);
        node = parse_expr(this, OBP_LOWEST);
        assert_token(this, node, ISL_TOKENT_RPARE);

    } else
        raise_parsing_failed(
            this,
            NULL,
            rid_expect_expression_before,
            curtoken.location,
            ist_token_names[curtoken.type]
        );

    while (true) {
        curtoken = cur_token(this);

        /**
         * if left-hand-side's right-binding-power is less than the current token's
         * left-binding-power, then we should make sure the node that parsing by prefix
         * will belong to the node of the current token.
         */

        if (lhsrbp < suffix_optattrs[curtoken.type].lbp) {
            /* this for suffix recognize and parsing */
            if (!suffix_optattrs[curtoken.type].led) break;
            node = suffix_optattrs[curtoken.type].led(this, node);

        } else if (lhsrbp < infix_optattrs[curtoken.type].lbp) {
            /* this for infix recognize and parsing */
            if (!infix_optattrs[curtoken.type].led) break;
            node = infix_optattrs[curtoken.type].led(this, node);

        } else break;

        handle_pstate_force(
            this,
            node,
            rid_expect_expression_after,
            curtoken.location,
            ist_token_names[curtoken.type]
        );
    }
    return node;
}


void* nud_literal_entity(ist_parser* this) {
    ist_token curtoken = advance(this);

    return ist_astnode_createby_full(LITERAL_ENT, curtoken.location, {
        __result__->value  = curtoken.value;
        __result__->litype = curtoken.type;
    });
}

void* nud_name_entity(ist_parser* this) {
    ist_token curtoken = advance(this);

    ist_string name = ist_string_consby_ref(curtoken.extract, curtoken.length);
    ist_module_register_strbuf(this->lexer.module, name, ISL_STRBUFT_SYMBOL);

    return ist_astnode_createby_full(NAME_ENT, curtoken.location, __result__->name = name);
}

void* nud_prefix_opt(ist_parser* this) {
    ist_token curtoken = advance(this);

    ist_astnode_defineby_full(node, UNARY_OPT, curtoken.location);

    node->onlhs    = true;
    node->optype   = curtoken.type;
    node->sub_node = parse_expr(this, prefix_optattrs[curtoken.type].rbp);

    handle_pstate_force(
        this,
        ((node)),
        rid_expect_expression_after,
        curtoken.location,
        ist_token_names[curtoken.type]
    );

    return node;
}

void* led_suffix_opt(ist_parser* this, ist_astnode* lhs) {
    ist_token curtoken = advance(this);

    ist_astnode_defineby_full(node, UNARY_OPT, curtoken.location);

    node->onlhs    = false;
    node->optype   = curtoken.type;
    node->sub_node = lhs;

    handle_pstate_force(
        this,
        ((node)),
        rid_expect_expression_after,
        curtoken.location,
        ist_token_names[curtoken.type]
    );

    return node;
}

void* led_infix_opt(ist_parser* this, ist_astnode* lhs) {
    ist_token curtoken = advance(this);

    ist_astnode_defineby_full(node, BINARY_OPT, curtoken.location);

    node->lhs_node = lhs;
    node->optype   = curtoken.type;
    node->rhs_node = parse_expr(this, infix_optattrs[curtoken.type].rbp);

    handle_pstate_force(
        this,
        ((node)),
        rid_expect_expression_after,
        curtoken.location,
        ist_token_names[curtoken.type]
    );

    return node;
}

void* led_wrap_opt(ist_parser* this, ist_astnode* lhs) {
    ist_token curtoken = advance(this);

    ist_astnode_defineby_full(node, BINARY_OPT, curtoken.location);

    node->lhs_node = lhs;
    node->optype   = ISL_TOKENT_LPARE;
    node->rhs_node = parse_expr(this, infix_optattrs[curtoken.type].rbp);

    return node;
}
