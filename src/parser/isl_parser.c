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

enum ist_optbindpower;
typedef enum ist_optbindpower ist_optbindpower;


/* parse statment */
void* parse_stmts(ist_parser* this);

void* parse_stmt(ist_parser* this);
void* parse_import_stmt(ist_parser* this);
void* parse_using_stmt(ist_parser* this);
void* parse_do_stmt(ist_parser* this);

/* parse common pattern */
void* parse_codetail_patt(ist_parser* this);
void* parse_codeblock_patt(ist_parser* this);
void* parse_paramlist_patt(ist_parser* this);
void* parse_arglist_patt(ist_parser* this);

/* parse expression */
void* parse_expr(ist_parser* this, ist_optbindpower lhsrbp);

void* nud_literal_entity(ist_parser* this);
void* nud_name_entity(ist_parser* this);
void* nud_prefix_expr(ist_parser* this);

void* led_suffix_expr(ist_parser* this, ist_astnode* lhs);
void* led_infix_expr(ist_parser* this, ist_astnode* lhs);
void* led_fncall_expr(ist_parser* this, ist_astnode* lhs);
void* led_wrap_expr(ist_parser* this, ist_astnode* lhs);


/* parse comptime-entity */
void* parse_name_entity(ist_parser* this);
void* parse_reference_entity(ist_parser* this);

void* parse_fn_entity(ist_parser* this);
void* parse_fn_prototype_entity(ist_parser* this);
void* parse_type_entity(ist_parser* this);

void* parse_regist_entity(ist_parser* this);
void* parse_slot_entity(ist_parser* this);
void* parse_literal_entity(ist_parser* this);



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


enum ist_optbindpower {
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
};


typedef void* (*ist_pletnud_fn)(ist_parser*);
typedef void* (*ist_pletled_fn)(ist_parser*, ist_astnode*);

struct ist_nudoptattr {

    ist_pletnud_fn   nud;
    ist_optbindpower rbp: 16;

} nudoptattrs[] = {

    [ISL_TOKENT_ADD]     = {nud_prefix_expr, OBP_PREFIX},
    [ISL_TOKENT_SUB]     = {nud_prefix_expr, OBP_PREFIX},
    [ISL_TOKENT_SELFADD] = {nud_prefix_expr, OBP_PREFIX},
    [ISL_TOKENT_SELFSUB] = {nud_prefix_expr, OBP_PREFIX},

    [ISL_TOKENT_VL_INT]    = {nud_literal_entity, OBP_NONE},
    [ISL_TOKENT_VL_REAL]   = {nud_literal_entity, OBP_NONE},
    [ISL_TOKENT_VL_STRING] = {nud_literal_entity, OBP_NONE},
    [ISL_TOKENT_BV_FALSE]  = {nud_literal_entity, OBP_NONE},
    [ISL_TOKENT_BV_TRUE]   = {nud_literal_entity, OBP_NONE},
    [ISL_TOKENT_ID]        = {nud_name_entity, OBP_NONE},

    [ISL_TOKENT_LATEST] = {NULL, OBP_NONE},

};

struct ist_ledoptattr {

    ist_pletled_fn   led;
    ist_optbindpower lbp: 16;
    ist_optbindpower rbp: 16;

} ledoptattrs[] = {

    [ISL_TOKENT_ASSIGN]     = {led_infix_expr, OBP_ASSIGN + 1, OBP_ASSIGN},
    [ISL_TOKENT_ADD_ASSIGN] = {led_infix_expr, OBP_ASSIGN + 1, OBP_ASSIGN},
    [ISL_TOKENT_SUB_ASSIGN] = {led_infix_expr, OBP_ASSIGN + 1, OBP_ASSIGN},
    [ISL_TOKENT_MUL_ASSIGN] = {led_infix_expr, OBP_ASSIGN + 1, OBP_ASSIGN},
    [ISL_TOKENT_DIV_ASSIGN] = {led_infix_expr, OBP_ASSIGN + 1, OBP_ASSIGN},
    [ISL_TOKENT_MOD_ASSIGN] = {led_infix_expr, OBP_ASSIGN + 1, OBP_ASSIGN},

    [ISL_TOKENT_ADD] = {led_infix_expr, OBP_ARITH, OBP_ARITH},
    [ISL_TOKENT_SUB] = {led_infix_expr, OBP_ARITH, OBP_ARITH},
    [ISL_TOKENT_MUL] = {led_infix_expr, OBP_TERM, OBP_TERM},
    [ISL_TOKENT_DIV] = {led_infix_expr, OBP_TERM, OBP_TERM},
    [ISL_TOKENT_MOD] = {led_infix_expr, OBP_TERM, OBP_TERM},

    [ISL_TOKENT_SELFADD] = {led_suffix_expr, OBP_PREFIX, OBP_NONE},
    [ISL_TOKENT_SELFSUB] = {led_suffix_expr, OBP_PREFIX, OBP_NONE},

    [ISL_TOKENT_LPARE]   = {led_fncall_expr, OBP_SUFFIX, OBP_NONE},
    [ISL_TOKENT_WRAPPER] = {led_wrap_expr, OBP_SUFFIX, OBP_ATOM},

    [ISL_TOKENT_LATEST] = {NULL, OBP_NONE, OBP_NONE},

};


/* entrance method for parser */
void ist_parser_parse(ist_parser* this) {
    this->root = parse_expr(this, OBP_LOWEST);
}


void* parse_expr(ist_parser* this, ist_optbindpower lhsrbp) {
    ist_token curtoken = cur_token(this);

    void* node;

    if (curtoken.type == ISL_TOKENT_LPARE) {

        /* handle paren */
        advance(this);
        node = parse_expr(this, OBP_LOWEST);
        assert_token(this, node, ISL_TOKENT_RPARE);

    } else if (nudoptattrs[curtoken.type].nud) {

        /* handle prefix or literal */
        node = nudoptattrs[curtoken.type].nud(this);
        handle_pstate_inert(this, node);

    } else
        raise_parsing_failed(
            ((this)),
            ((NULL)),
            rid_not_expression,
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
        node = ledoptattrs[curtoken.type].led(this, node);

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

void* nud_prefix_expr(ist_parser* this) {
    ist_token curtoken = advance(this);

    ist_astnode_defineby_full(node, UNARY_EXPR, curtoken.location);

    node->onlhs    = true;
    node->optype   = curtoken.type;
    node->sub_node = parse_expr(this, nudoptattrs[curtoken.type].rbp);

    handle_pstate_force(
        this,
        ((node)),
        rid_expect_expression_after,
        curtoken.location,
        ist_token_names[curtoken.type]
    );

    return node;
}

void* led_suffix_expr(ist_parser* this, ist_astnode* lhs) {
    ist_token curtoken = advance(this);

    ist_astnode_defineby_full(node, UNARY_EXPR, curtoken.location);

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

void* led_infix_expr(ist_parser* this, ist_astnode* lhs) {
    ist_token curtoken = advance(this);

    ist_astnode_defineby_full(node, BINARY_EXPR, curtoken.location);

    node->lhs_node = lhs;
    node->optype   = curtoken.type;
    node->rhs_node = parse_expr(this, ledoptattrs[curtoken.type].rbp);

    handle_pstate_force(
        this,
        ((node)),
        rid_expect_expression_after,
        curtoken.location,
        ist_token_names[curtoken.type]
    );

    return node;
}

void* led_fncall_expr(ist_parser* this, ist_astnode* lhs) {
    ist_token curtoken = advance(this);
    ist_astnode_defineby_full(node, FNCALL_EXPR, curtoken.location);

    node->fnentity = lhs;
    node->arglist  = ist_astnodeptr_list_consc(2);

    /* if the pair closed immediately, then the parsing over! */
    if (match_token(this, ISL_TOKENT_RPARE)) return node;

    do {
        curtoken = cur_token(this);

        ist_astnodeptr_list_addc(&node->arglist, parse_expr(this, OBP_LOWEST));
        handle_pstate_force(
            this,
            ((node)),
            rid_expect_expression_before,
            curtoken.location,
            ist_token_names[curtoken.type]
        );

    } while (match_token(this, ISL_TOKENT_COMMA));

    /**
     * if the args has be handled, there are must
     * a close paren to end the call of fn-entity.
     */
    assert_token(this, node, ISL_TOKENT_RPARE);

    return node;
}

void* led_wrap_expr(ist_parser* this, ist_astnode* lhs) {
    ist_token curtoken = advance(this);
    ist_astnode_defineby_full(node, FNCALL_EXPR, curtoken.location);

    node->arglist = ist_astnodeptr_list_consc(2);
    ist_astnodeptr_list_addc(&node->arglist, lhs);

    /* catch the fn-entity that will be called */
    node->fnentity = parse_expr(this, ledoptattrs[curtoken.type].rbp);
    handle_pstate_force(
        this,
        ((node)),
        rid_expect_fn_entity_after,
        curtoken.location,
        ist_token_names[curtoken.type]
    );

    /**
     * that means the paren pair can be omitted
     * if there are no antoher args in wrap_expr.
     */
    if (!match_token(this, ISL_TOKENT_LPARE)) return node;
    if (match_token(this, ISL_TOKENT_RPARE)) return node;

    /* that means there are some args need to handle */
    do {
        curtoken = cur_token(this);

        ist_astnodeptr_list_addc(&node->arglist, parse_expr(this, OBP_LOWEST));
        handle_pstate_force(
            this,
            ((node)),
            rid_expect_expression_after,
            curtoken.location,
            ist_token_names[curtoken.type]
        );

    } while (match_token(this, ISL_TOKENT_COMMA));

    /**
     * if the args has be handled, there are must
     * a close paren to end the call of fn-entity.
     */
    assert_token(this, node, ISL_TOKENT_RPARE);

    return node;
}
