#include "isl_lexer.h"

#include <ctype.h>

#include "isl_memgr.h"
#include "isl_utf8.h"
#include "isl_report.h"

#define analysis_token (this->sec_token)

ist_codepage* ist_codepage_createby_source(ist_string _source) {

    ist_codepage* codepage = isl_malloc(ist_codepage);

    codepage->source = _source;
    codepage->next_sequence_index = 0;
    codepage->current_codepoint_decode_length = 0;
    codepage->decoded_codepoint_count = 0;

    /* decode the first utf8 sequence */
    codepage->current_codepoint =
        isl_utf8_decode(&codepage->source,
                        codepage->next_sequence_index,
                        &codepage->current_codepoint_decode_length);
    codepage->next_sequence_index += codepage->current_codepoint_decode_length;

    /* initialize the location, module name is <buildin-source> */
    ist_location_init(&codepage->location, "<buildin-source>");

    codepage->prev_page = codepage->next_page = NULL;

    return codepage;

}

void ist_lexer_init(ist_lexer* this, ist_codepage* codepage) {

    ist_token_init_null(&this->pre_token);
    ist_token_init_null(&this->cur_token);
    ist_token_init_null(&this->nex_token);
    ist_token_init_null(&this->sec_token);

    this->codepage = codepage;

    ist_lexer_advance(this);
    ist_lexer_advance(this);
    ist_lexer_advance(this);
}

ist_lexer* ist_lexer_create(ist_codepage* _codepage) {
    ist_lexer* lexer = isl_malloc(ist_lexer);
    ist_lexer_init(lexer, _codepage);
    return lexer;
}

ist_token* ist_lexer_advance(ist_lexer* this) {


    this->pre_token = this->cur_token;
    this->cur_token = this->nex_token;
    this->nex_token = this->sec_token;


    while (this->codepage->current_codepoint != 0) {

        ist_token_init_full(
            &analysis_token,
            ISL_TOKENT_EOF,
            this->codepage->location,
            this->codepage->source
                + this->codepage->next_sequence_index
                - isl_utf8_encode_length(this->codepage->current_codepoint),
            0, (ist_value) { 0 });

        ist_lexer_skip_blanks(this);

        switch (this->codepage->current_codepoint) {

#define _ISL_LEXER_GEN_SYMBOL_CASE(_symbol, _token_type) \
    case _symbol:analysis_token.type=_token_type;break

#define _ISL_LEXER_GEN_SYMBOL_CASE_MAYBE_BIN(_symbol, _symbol_suffix, _token_type_unary, _token_type_binary) \
case _symbol:                                                   \
    if (ist_lexer_get_next_codepoint(this)==_symbol_suffix){    \
        ist_lexer_advance_codepoint(this);                      \
        analysis_token.type=_token_type_binary;break;           \
    }   analysis_token.type=_token_type_unary;break;

#define _ISL_LEXER_GEN_SYMBOL_CASE_MAYBE_BIN_HEAD(_symbol, _symbol_suffix, _token_type_binary) \
case _symbol:                                                   \
    if (ist_lexer_get_next_codepoint(this)==_symbol_suffix){    \
        ist_lexer_advance_codepoint(this);                      \
        analysis_token.type=_token_type_binary;break;           \
    }

#define _ISL_LEXER_GEN_SYMBOL_CASE_MAYBE_BIN_BODY(_symbol_suffix, _token_type_binary) \
    else if (ist_lexer_get_next_codepoint(this)==_symbol_suffix){   \
        ist_lexer_advance_codepoint(this);                          \
        analysis_token.type=_token_type_binary;break;               \
    }

#define _ISL_LEXER_GEN_SYMBOL_CASE_MAYBE_BIN_TAIL(_token_type_unary) \
        analysis_token.type=_token_type_unary;break

            _ISL_LEXER_GEN_SYMBOL_CASE('(', ISL_TOKENT_LPARE);
            _ISL_LEXER_GEN_SYMBOL_CASE(')', ISL_TOKENT_RPARE);

            _ISL_LEXER_GEN_SYMBOL_CASE('{', ISL_TOKENT_LBRACE);
            _ISL_LEXER_GEN_SYMBOL_CASE('}', ISL_TOKENT_RBRACE);

            _ISL_LEXER_GEN_SYMBOL_CASE('[', ISL_TOKENT_LBRACKET);
            _ISL_LEXER_GEN_SYMBOL_CASE(']', ISL_TOKENT_RBRACKET);

            _ISL_LEXER_GEN_SYMBOL_CASE(',', ISL_TOKENT_COMMA);
            _ISL_LEXER_GEN_SYMBOL_CASE(';', ISL_TOKENT_EOS);
            _ISL_LEXER_GEN_SYMBOL_CASE(':', ISL_TOKENT_COLON);
            _ISL_LEXER_GEN_SYMBOL_CASE('.', ISL_TOKENT_DOT);
            _ISL_LEXER_GEN_SYMBOL_CASE('?', ISL_TOKENT_QUESTION);

            _ISL_LEXER_GEN_SYMBOL_CASE_MAYBE_BIN('=', '=', ISL_TOKENT_ASSIGN, ISL_TOKENT_EQUAL);
            _ISL_LEXER_GEN_SYMBOL_CASE_MAYBE_BIN('!', '=', ISL_TOKENT_LNOT, ISL_TOKENT_NOTEQUAL);

            _ISL_LEXER_GEN_SYMBOL_CASE_MAYBE_BIN('&', '&', ISL_TOKENT_AND, ISL_TOKENT_LAND);
            _ISL_LEXER_GEN_SYMBOL_CASE_MAYBE_BIN('|', '|', ISL_TOKENT_OR, ISL_TOKENT_LOR);

            _ISL_LEXER_GEN_SYMBOL_CASE_MAYBE_BIN('*', '=', ISL_TOKENT_MUL, ISL_TOKENT_MUL_ASSIGN);
            _ISL_LEXER_GEN_SYMBOL_CASE_MAYBE_BIN('%', '=', ISL_TOKENT_MOD, ISL_TOKENT_MOD_ASSIGN);

            _ISL_LEXER_GEN_SYMBOL_CASE_MAYBE_BIN_HEAD('<', '=', ISL_TOKENT_LESSEQUAL)
                _ISL_LEXER_GEN_SYMBOL_CASE_MAYBE_BIN_BODY('<', ISL_TOKENT_LSHIFT)
                _ISL_LEXER_GEN_SYMBOL_CASE_MAYBE_BIN_TAIL(ISL_TOKENT_LESSTHAN);

            _ISL_LEXER_GEN_SYMBOL_CASE_MAYBE_BIN_HEAD('>', '=', ISL_TOKENT_GREATEQUAL)
                _ISL_LEXER_GEN_SYMBOL_CASE_MAYBE_BIN_BODY('>', ISL_TOKENT_RSHIFT)
                _ISL_LEXER_GEN_SYMBOL_CASE_MAYBE_BIN_TAIL(ISL_TOKENT_GREATERTHAN);

            _ISL_LEXER_GEN_SYMBOL_CASE_MAYBE_BIN_HEAD('+', '=', ISL_TOKENT_ADD_ASSIGN)
                _ISL_LEXER_GEN_SYMBOL_CASE_MAYBE_BIN_BODY('+', ISL_TOKENT_SELFADD)
                _ISL_LEXER_GEN_SYMBOL_CASE_MAYBE_BIN_TAIL(ISL_TOKENT_ADD);

            _ISL_LEXER_GEN_SYMBOL_CASE_MAYBE_BIN_HEAD('-', '=', ISL_TOKENT_SUB_ASSIGN)
                _ISL_LEXER_GEN_SYMBOL_CASE_MAYBE_BIN_BODY('-', ISL_TOKENT_SELFSUB)
                _ISL_LEXER_GEN_SYMBOL_CASE_MAYBE_BIN_BODY('>', ISL_TOKENT_WRAPPER)
                _ISL_LEXER_GEN_SYMBOL_CASE_MAYBE_BIN_TAIL(ISL_TOKENT_SUB);

        case '/': {
            ist_codepoint next_codepoint = ist_lexer_get_next_codepoint(this);
            if (ist_lexer_get_next_codepoint(this) == '=') {
                ist_lexer_advance_codepoint(this);
                analysis_token.type = ISL_TOKENT_DIV_ASSIGN; break;
            }
            else if (next_codepoint == '/' || next_codepoint == '*') {
                ist_lexer_advance_codepoint(this);
                ist_lexer_advance_codepoint(this);
                ist_lexer_skip_comment(this, next_codepoint == '*');
                continue;
            }
            analysis_token.type = ISL_TOKENT_DIV; break;
        }

        case '"':
            ist_lexer_parse_string(this);
            return &this->pre_token;

        default:
            if (isdigit(this->codepage->current_codepoint)) {
                ist_lexer_parse_number(this);
                break;
            }
            else if (isl_utf8_legal_identifier_codepoint(this->codepage->current_codepoint, true)) {
                ist_lexer_parse_identifier(this);
                break;
            }
            isl_report(rid_unrecongnized_codepoint, this->codepage->current_codepoint);
            ist_lexer_advance_codepoint(this);
            continue;
        }

        analysis_token.extract_length =
            (this->codepage->source
                + this->codepage->next_sequence_index)
            - analysis_token.extract;

        ist_lexer_advance_codepoint(this);

        return &this->pre_token;
    }
}


void ist_lexer_skip_blanks(ist_lexer* this) {
    while (isspace(this->codepage->current_codepoint)) {
        ist_lexer_advance_codepoint(this);
    }
}

void ist_lexer_parse_identifier(ist_lexer* this) {
    analysis_token.type = ISL_TOKENT_ID;

    /* skip identifier context */
    while (
         isl_utf8_legal_identifier_codepoint(
             ist_lexer_advance_codepoint(this), false)
     ) NULL;

    analysis_token.extract_length =
        (this->codepage->source
            + this->codepage->next_sequence_index)
        - analysis_token.extract - 1;

}

void ist_lexer_parse_number(ist_lexer* this) {

}

void ist_lexer_parse_string(ist_lexer* this) {

}

void ist_lexer_skip_comment(ist_lexer* this, ist_bool _is_block) {
    while (this->codepage->current_codepoint != 0) {
        if (_is_block) {
            if (this->codepage->current_codepoint == '*'
                && ist_lexer_get_next_codepoint(this) == '/') {
                /* skip comment block ending symbols */
                ist_lexer_advance_codepoint(this);
                ist_lexer_advance_codepoint(this);
                return;
            }
            else if (this->codepage->current_codepoint == 0) {
                isl_report(rid_unterminated_comment_block);
            }
        }
        else if (ist_lexer_match_current_codepoint(this, '\n')) {
            return;
        }
        /* skip comment context */
        ist_lexer_advance_codepoint(this);
    }
}


//TODO: codepage switching will also only happen here when we read the file further.
//      the another switching place was in the end of lookahead.
ist_codepoint ist_lexer_get_next_codepoint(ist_lexer* this) {
    return isl_utf8_decode(
                &this->codepage->source,
                this->codepage->next_sequence_index,
                &this->codepage->current_codepoint_decode_length);
}

/* return the current codepoint, and advance to the next codepoint */
ist_codepoint ist_lexer_advance_codepoint(ist_lexer* this) {

    if (this->codepage->current_codepoint == 0) {
        isl_report(rid_advance_codepoint_when_eof, isp_catch_coreloc);
        return 0;
    }

    /* update the location */
    ++this->codepage->location.column;
    if (this->codepage->current_codepoint == '\n') {
        ++this->codepage->location.line;
        this->codepage->location.column = 1;
    }

    /* store the current codepoint */
    ist_codepoint codepoint = this->codepage->current_codepoint;

    /* update the current codepoint */
    this->codepage->current_codepoint = ist_lexer_get_next_codepoint(this);
    //NOTICE: when we get the next codepoint, the page maybe changed.
    //        at the time, the next sequence index can't be changed.
    this->codepage->next_sequence_index += this->codepage->current_codepoint_decode_length;

    /* return the stored codepoint */
    return codepoint;

}

/*
    if next codepoint matches arg codepoint, then advance to the next codepoint,
    and return true, otherwise return false.
*/
ist_bool ist_lexer_match_current_codepoint(ist_lexer* this, ist_codepoint _codepoint) {
    if (this->codepage->current_codepoint != _codepoint) return false;
    ist_lexer_advance_codepoint(this);
    return true;
}

void ist_lexer_correct_next_codepoint_index(ist_lexer* this) {
    this->codepage->next_sequence_index += this->codepage->current_codepoint_decode_length;
}
