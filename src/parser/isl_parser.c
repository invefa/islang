#include "isl_parser.h"

inline ist_parser ist_parser_consby_full(ist_lexer _lexer) {
    return (ist_parser){.lexer = _lexer, .root = NULL};
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



#define inert_parse(fncall)                                             \
    ({                                                                  \
        parse_result inert_parse_result = fncall;                       \
        if (!inert_parse_result.state) return inert_parse_result.state; \
        inert_parse_result;                                             \
    })

#define force_parse(fncall, _rid, _rptvargs...)           \
    ({                                                    \
        parse_result force_parse_result = fncall;         \
        switch (force_parse_result.state) {               \
            case PRS_FUNREPROTED:                         \
                isl_report(_rid, _rptvargs);              \
            case PRS_FREPROTED:                           \
                force_parse_result.state = PRS_FREPROTED; \
            case PRS_FAHEAD:                              \
                return force_parse_result;                \
            case PRS_SUCCESS:                             \
                break;                                    \
        }                                                 \
        force_parse_result;                               \
    })

#define ahead_match(fncall)                                                                   \
    ({                                                                                        \
        ist_lexer_lookahead_start(&this->lexer);                                              \
        parse_result ahead_match_result = fncall;                                             \
        ist_lexer_lookahead_end(&this->lexer);                                                \
        isl_assert(                                                                           \
            ahead_match_result.state == PRS_SUCCESS || ahead_match_result.state == PRS_FAHEAD \
        );                                                                                    \
        !ahead_match_result.state;                                                            \
    })

#define pre_token() ((this)->lexer.pre_token)
#define cur_token() ((this)->lexer.cur_token)
#define nex_token() ((this)->lexer.nex_token)
#define sec_token() ((this)->lexer.sec_token)


#define advance() ist_lexer_advance(&this->lexer)

ist_bool match_token(ist_parser* this, ist_token_type _type) {
    if (cur_token().type != _type) return false;
    advance();
    return true;
}

#define parse_fail(_result, _rid, _rptvargs...) \
    do {                                        \
        isl_report(_rid, _rptvargs);            \
        (_result).state = PRS_FREPROTED;        \
        return (_result);                       \
    } while (0)



#define assert_token(_type, _result)              \
    do {                                          \
        if (!match_token(this, _type))            \
            parse_fail(                           \
                _result,                          \
                rid_assert_tokentype_failed,      \
                cur_token().location,             \
                ist_token_names[_type],           \
                ist_token_names[cur_token().type] \
            );                                    \
    } while (0)

typedef enum optbindpower {
    PCD_NONE    = 0x0,
    PCD_LOWEST  = 0x1,
    PCD_ASSIGN  = 0x20,
    PCD_ARITH   = 0x30,
    PCD_TERM    = 0x40,
    PCD_FACTOR  = 0x50,
    PCD_PREFIX  = 0x60,
    PCD_SUFFIX  = 0x70,
    PCD_PAREN   = 0xFF,
    PCD_HIGHEST = INT16_MAX,
} optbindpower;

typedef struct parse_result {
    enum parse_result_state {
        PRS_SUCCESS     = 0,
        PRS_FUNREPROTED = 1,
        PRS_FREPROTED   = 2,
        PRS_FAHEAD      = 3,
    } state: 2;
    ist_astnode* node;
} parse_result;

#define parse_result_setstate(_result, _state) \
    ({                                         \
        (_result).state = (_state);            \
        (_result);                             \
    })

#define parse_result_consby_null() ((parse_result){.state = PRS_SUCCESS, .node = NULL})
#define parse_result_consby_node(_node) \
    ((parse_result){.state = PRS_SUCCESS, .node = (ist_astnode*)(_node)})


typedef parse_result (*parselet_nud_fntype)(ist_parser*, ist_token);
typedef parse_result (*parselet_led_fntype)(ist_parser*, ist_token, optbindpower);

parse_result nud_literal_ent(ist_parser* this, ist_token _token) {
    IST_ASTNODE_LITERAL_ENT* node = ist_astnode_createby_full(LITERAL_ENT, _token.location);

    node->value        = _token.value;
    node->literal_type = _token.type;
    return parse_result_consby_node(node);
}

struct {
    parselet_nud_fntype nud;
    parselet_led_fntype led;
    optbindpower        lbp: 16;
    optbindpower        rbp: 16;
} opttattrs[] = {
    [ISL_TOKENT_ASSIGN]     = {NULL, NULL, PCD_ASSIGN + 1, PCD_ASSIGN},
    [ISL_TOKENT_ADD_ASSIGN] = {NULL, NULL, PCD_ASSIGN + 1, PCD_ASSIGN},
    [ISL_TOKENT_SUB_ASSIGN] = {NULL, NULL, PCD_ASSIGN + 1, PCD_ASSIGN},
    [ISL_TOKENT_MUL_ASSIGN] = {NULL, NULL, PCD_ASSIGN + 1, PCD_ASSIGN},
    [ISL_TOKENT_DIV_ASSIGN] = {NULL, NULL, PCD_ASSIGN + 1, PCD_ASSIGN},
    [ISL_TOKENT_MOD_ASSIGN] = {NULL, NULL, PCD_ASSIGN + 1, PCD_ASSIGN},
    [ISL_TOKENT_ADD]        = {NULL, NULL, PCD_ARITH, PCD_ARITH + 1},
    [ISL_TOKENT_SUB]        = {NULL, NULL, PCD_ARITH, PCD_ARITH + 1},
    [ISL_TOKENT_MUL]        = {NULL, NULL, PCD_TERM, PCD_TERM + 1},
    [ISL_TOKENT_DIV]        = {NULL, NULL, PCD_TERM, PCD_TERM + 1},
    [ISL_TOKENT_MOD]        = {NULL, NULL, PCD_TERM, PCD_TERM + 1},
    [ISL_TOKENT_VL_INT]     = {nud_literal_ent, NULL, PCD_NONE, PCD_NONE},
    [ISL_TOKENT_VL_REAL]    = {nud_literal_ent, NULL, PCD_NONE, PCD_NONE},
    [ISL_TOKENT_VL_STRING]  = {nud_literal_ent, NULL, PCD_NONE, PCD_NONE},
    [ISL_TOKENT_BV_FALSE]   = {nud_literal_ent, NULL, PCD_NONE, PCD_NONE},
    [ISL_TOKENT_BV_TRUE]    = {nud_literal_ent, NULL, PCD_NONE, PCD_NONE},
};


parse_result parse_expr(ist_parser* this, optbindpower minbp) {
    parse_result result = parse_result_consby_null();

    if (opttattrs[cur_token().type].nud)
        result = opttattrs[cur_token().type].nud(this, cur_token());

    else parse_fail(result, rid_expect_expression, cur_token().location);


    switch (cur_token().type) {
        case ISL_TOKENT_VL_INT:
        case ISL_TOKENT_VL_REAL:
        case ISL_TOKENT_VL_STRING:
        case ISL_TOKENT_BV_FALSE:
        case ISL_TOKENT_BV_TRUE:
            result.node = ist_astnode_createby_full(LITERAL_ENT, cur_token().location);
            ISL_AS_LITERAL_ENT(result.node)->value        = cur_token().value;
            ISL_AS_LITERAL_ENT(result.node)->literal_type = cur_token().type;
            break;

        case ISL_TOKENT_LPARE:
            assert_token(ISL_TOKENT_LPARE, result);
            result =
                force_parse(parse_expr(this, minbp), rid_expect_expression, cur_token().location);
            assert_token(ISL_TOKENT_RPARE, result);
            break;
        default:
            isl_report(rid_expect_expression, cur_token().location);
            return (parse_result){.state = PRS_FUNREPROTED};
    }
    return result;
}

void ist_parser_parse(ist_parser* this) {
    this->root = parse_expr(this, PCD_LOWEST).node;
}