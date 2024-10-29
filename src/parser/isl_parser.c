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

typedef struct ist_presult {
    enum ist_presult_state {
        PRS_SUCCESS     = 0,
        PRS_FUNREPROTED = 1,
        PRS_FREPROTED   = 2,
        PRS_FAHEAD      = 3,
    } state: 2;
    ist_astnode* node;
} ist_presult;


#define parse_result_setstate(_result, _state) \
    ({                                         \
        (_result).state = (_state);            \
        (_result);                             \
    })

#define ist_presult_consby_null() ((ist_presult){.state = PRS_SUCCESS, .node = NULL})
#define ist_presult_consby_node(_node) \
    ((ist_presult){.state = PRS_SUCCESS, .node = (ist_astnode*)(_node)})


typedef ist_presult (*ist_pletnud_fn)(ist_parser*);
typedef ist_presult (*ist_pletled_fn)(ist_parser*, ist_astnode*);

ist_presult nud_literal_ent(ist_parser* this);
ist_presult nud_prefix(ist_parser* this);

ist_presult led_infix(ist_parser* this, ist_astnode* lhs);


#define pre_token(this) ((this)->lexer.pre_token)
#define cur_token(this) ((this)->lexer.cur_token)
#define nex_token(this) ((this)->lexer.nex_token)
#define sec_token(this) ((this)->lexer.sec_token)


#define inert_parse(_resultv, _fncall)                           \
    ({                                                           \
        ist_presult inert_parse_result = _fncall;                \
        if (inert_parse_result.state) return inert_parse_result; \
        (_resultv) = inert_parse_result;                         \
    })

#define force_parse(_resultv, _fncall, _rid, _rptvargs...) \
    ({                                                     \
        ist_presult force_parse_result = _fncall;          \
        switch (force_parse_result.state) {                \
            case PRS_FUNREPROTED:                          \
                isl_report(_rid, _rptvargs);               \
            case PRS_FREPROTED:                            \
                force_parse_result.state = PRS_FREPROTED;  \
            case PRS_FAHEAD:                               \
                return force_parse_result;                 \
            case PRS_SUCCESS:                              \
                break;                                     \
        }                                                  \
        (_resultv) = force_parse_result;                   \
    })

#define ahead_match(_fncall)                                                                  \
    ({                                                                                        \
        ist_lexer_lookahead_start(&this->lexer);                                              \
        ist_presult ahead_match_result = fncall;                                              \
        ist_lexer_lookahead_end(&this->lexer);                                                \
        isl_assert(                                                                           \
            ahead_match_result.state == PRS_SUCCESS || ahead_match_result.state == PRS_FAHEAD \
        );                                                                                    \
        !ahead_match_result.state;                                                            \
    })


#define parse_fail(_result, _rid, _rptvargs...)               \
    do {                                                      \
        isl_report(_rid, _rptvargs);                          \
        return parse_result_setstate(_result, PRS_FREPROTED); \
    } while (0)

#define assert_token(_result, _type)                  \
    do {                                              \
        if (!match_token(this, _type))                \
            parse_fail(                               \
                _result,                              \
                rid_assert_tokentype_failed,          \
                cur_token(this).location,             \
                ist_token_names[_type],               \
                ist_token_names[cur_token(this).type] \
            );                                        \
    } while (0)



ist_token* advance(ist_parser* this) {
    ist_lexer_advance(&this->lexer);
    return &pre_token(this);
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


ist_presult parse_expr(ist_parser* this, ist_optbindpower minbp) {
    ist_presult result   = ist_presult_consby_null();
    ist_token   curtoken = cur_token(this);

    if (opttattrs[curtoken.type].nud) inert_parse(result, opttattrs[curtoken.type].nud(this));
    else parse_fail(result, rid_expect_expression, curtoken.location);

    while (minbp <= opttattrs[cur_token(this).type].lbp) {
        curtoken = cur_token(this);
        if (!opttattrs[curtoken.type].led) break;

        force_parse(
            result,
            opttattrs[curtoken.type].led(this, result.node),
            rid_expect_expression,
            curtoken.location
        );
    }

    return result;
}

void ist_parser_parse(ist_parser* this) {
    this->root = parse_expr(this, OBP_LOWEST).node;
}

ist_presult nud_literal_ent(ist_parser* this) {
    ist_presult result =
        ist_presult_consby_node(ist_astnode_createby_full(LITERAL_ENT, cur_token(this).location));

    ISL_AS_LITERAL_ENT(result.node)->value        = cur_token(this).value;
    ISL_AS_LITERAL_ENT(result.node)->litype = cur_token(this).type;

    advance(this);

    return result;
}

ist_presult nud_prefix(ist_parser* this) {
    ist_token   curtoken = cur_token(this);
    ist_presult result =
        ist_presult_consby_node(ist_astnode_createby_full(UNARY_OPT, curtoken.location));


    ISL_AS_UNARY_OPT(result.node)->onlhs       = true;
    ISL_AS_UNARY_OPT(result.node)->optype = curtoken.type;
    ISL_AS_UNARY_OPT(result.node)->sub_node =
        force_parse(
            ist_presult_consby_null(),
            parse_expr(this, opttattrs[advance(this)->type].rbp),
            rid_expect_expression,
            curtoken.location
        )
            .node;

    return result;
}

ist_presult led_infix(ist_parser* this, ist_astnode* lhs) {
    ist_token   curtoken = cur_token(this);
    ist_presult result =
        ist_presult_consby_node(ist_astnode_createby_full(BINARY_OPT, curtoken.location));


    ISL_AS_BINARY_OPT(result.node)->lhs_node     = lhs;
    ISL_AS_BINARY_OPT(result.node)->optype = curtoken.type;
    ISL_AS_BINARY_OPT(result.node)->rhs_node =
        force_parse(
            ist_presult_consby_null(),
            parse_expr(this, opttattrs[advance(this)->type].rbp),
            rid_expect_expression,
            cur_token(this).location
        )
            .node;

    return result;
}