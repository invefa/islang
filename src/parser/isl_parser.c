#include "isl_parser.h"

inline ist_parser ist_parser_consby_full(ist_lexer _lexer) {
    return (ist_parser){.lexer = _lexer, .root = NULL};
}
inline ist_parser ist_parser_consby_module(ist_module* _module) {
    return ist_parser_consby_full(ist_lexer_consby_module(_module));
}


inline ist_parser* ist_parser_initby_full(ist_parser* this, ist_lexer _lexer) {
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
        ist_parse_result inert_parse_result = fncall;                   \
        if (!inert_parse_result.state) return inert_parse_result.state; \
        inert_parse_result;                                             \
    })

#define force_parse(fncall, _rid, _rptvargs...)           \
    ({                                                    \
        ist_parse_result force_parse_result = fncall;     \
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
        ist_parse_result ahead_match_result = fncall;                                         \
        ist_lexer_lookahead_end(&this->lexer);                                                \
        isl_assert(                                                                           \
            ahead_match_result.state == PRS_SUCCESS || ahead_match_result.state == PRS_FAHEAD \
        );                                                                                    \
        !ahead_match_result.state;                                                            \
    })

#define pre_token(this) ((this)->lexer.pre_token)
#define cur_token(this) ((this)->lexer.cur_token)
#define nex_token(this) ((this)->lexer.nex_token)
#define sec_token(this) ((this)->lexer.sec_token)


#define advance(this) ist_lexer_advance(&this->lexer)

ist_bool match_token_type(ist_parser* this, ist_token_type _type) {
    if (cur_token(this).type != _type) return false;
    advance(this);
    return true;
}

#define parse_fail(_result, _rid, _rptvargs...) \
    do {                                        \
        isl_report(_rid, _rptvargs);            \
        (_result).state = PRS_FREPROTED;        \
        return (_result);                       \
    } while (0)

#define assert_token_type(this, _type, _result)       \
    do {                                              \
        if (cur_token(this).type != _type) {          \
            parse_fail(                               \
                _result,                              \
                rid_assert_tokentype_failed,          \
                cur_token(this).location,             \
                ist_token_names[_type],               \
                ist_token_names[cur_token(this).type] \
            );                                        \
        }                                             \
        advance(this);                                \
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

struct {
    ist_bool          nud: 1;
    ist_bool          led: 1;
    enum optbindpower lbp: 15, rbp: 15;
} opttattrs[] = {
    [ISL_TOKENT_ASSIGN]     = {false, true, PCD_ASSIGN + 1, PCD_ASSIGN},
    [ISL_TOKENT_ADD_ASSIGN] = {false, true, PCD_ASSIGN + 1, PCD_ASSIGN},
    [ISL_TOKENT_SUB_ASSIGN] = {false, true, PCD_ASSIGN + 1, PCD_ASSIGN},
    [ISL_TOKENT_MUL_ASSIGN] = {false, true, PCD_ASSIGN + 1, PCD_ASSIGN},
    [ISL_TOKENT_DIV_ASSIGN] = {false, true, PCD_ASSIGN + 1, PCD_ASSIGN},
    [ISL_TOKENT_MOD_ASSIGN] = {false, true, PCD_ASSIGN + 1, PCD_ASSIGN},
    [ISL_TOKENT_ADD]        = {true, true, PCD_ARITH, PCD_ARITH + 1},
    [ISL_TOKENT_SUB]        = {true, true, PCD_ARITH, PCD_ARITH + 1},
    [ISL_TOKENT_MUL]        = {false, true, PCD_TERM, PCD_TERM + 1},
    [ISL_TOKENT_DIV]        = {false, true, PCD_TERM, PCD_TERM + 1},
    [ISL_TOKENT_MOD]        = {false, true, PCD_TERM, PCD_TERM + 1},
    [ISL_TOKENT_VL_INT]     = {true, false, PCD_HIGHEST, PCD_HIGHEST},
    [ISL_TOKENT_VL_REAL]    = {true, false, PCD_HIGHEST, PCD_HIGHEST},
    [ISL_TOKENT_VL_STRING]  = {true, false, PCD_HIGHEST, PCD_HIGHEST},
    [ISL_TOKENT_BV_FALSE]   = {true, false, PCD_HIGHEST, PCD_HIGHEST},
    [ISL_TOKENT_BV_TRUE]    = {true, false, PCD_HIGHEST, PCD_HIGHEST},
};

typedef struct ist_parse_result {
    enum parse_result_state {
        PRS_SUCCESS     = 0,
        PRS_FUNREPROTED = 1,
        PRS_FREPROTED   = 2,
        PRS_FAHEAD      = 3,
    } state: 2;
    ist_astnode* node;
} ist_parse_result;


ist_parse_result parse_expr(ist_parser* this, optbindpower minbp) {
    ist_parse_result result = {.state = PRS_SUCCESS, .node = NULL};
    switch (cur_token(this).type) {
        case ISL_TOKENT_VL_INT:
        case ISL_TOKENT_VL_REAL:
        case ISL_TOKENT_VL_STRING:
        case ISL_TOKENT_BV_FALSE:
        case ISL_TOKENT_BV_TRUE:
            result.node = ist_astnode_createby_full(LITERAL_ENT, cur_token(this).location);
            ISL_AS_LITERAL_ENT(result.node)->value        = cur_token(this).value;
            ISL_AS_LITERAL_ENT(result.node)->literal_type = cur_token(this).type;
            break;

        case ISL_TOKENT_LPARE:
            assert_token_type(this, ISL_TOKENT_LPARE, result);
            result = force_parse(
                parse_expr(this, minbp), rid_expect_expression, cur_token(this).location
            );
            assert_token_type(this, ISL_TOKENT_RPARE, result);
            break;
        default:
            isl_report(rid_expect_expression, cur_token(this).location);
            return (ist_parse_result){.state = PRS_FUNREPROTED};
    }
    return result;
}

void ist_parser_parse(ist_parser* this) {
    this->root = parse_expr(this, PCD_LOWEST).node;
}