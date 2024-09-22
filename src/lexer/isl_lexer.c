#include "isl_lexer.h"

#include <ctype.h>

#include "isl_memgr.h"
#include "isl_utf8.h"

ist_codepage* ist_codepage_createby_source(ist_string _source) {

    ist_codepage* codepage = isl_malloc(ist_codepage);

    codepage->source = _source;
    codepage->next_sequence_index = 0;

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

#   define analysis_token (this->sec_token)

    this->pre_token = this->cur_token;
    this->cur_token = this->nex_token;
    this->nex_token = this->sec_token;

    ist_token_init_full(
        &analysis_token,
        ISL_TOKENT_EOF,
        this->codepage->location,
        this->codepage->source
            + this->codepage->next_sequence_index
            - isl_utf8_encode_length(this->codepage->current_codepoint),
        0, (ist_value) { 0 });

    ist_lexer_skip_blanks(this);

    while (this->codepage->current_codepoint != 0) {
        break;
        switch (this->codepage->current_codepoint) {
        case '+': analysis_token.type = ISL_TOKENT_ADD; break;
        case '-': analysis_token.type = ISL_TOKENT_SUB; break;
        case '*': analysis_token.type = ISL_TOKENT_MUL; break;
        case '/': analysis_token.type = ISL_TOKENT_DIV; break;
        case '%': analysis_token.type = ISL_TOKENT_MOD; break;
        case '=': analysis_token.type = ISL_TOKENT_ASSIGN; break;
        case '!': analysis_token.type = ISL_TOKENT_LNOT; break;
        case '(': analysis_token.type = ISL_TOKENT_LPARE; break;
        case ')': analysis_token.type = ISL_TOKENT_RPARE; break;
        case '{': analysis_token.type = ISL_TOKENT_LBRACE; break;
        case '}': analysis_token.type = ISL_TOKENT_RBRACE; break;
        }
    }

#   undef analysis_token
}

void ist_lexer_skip_blanks(ist_lexer* this) {
    while (isspace(this->codepage->current_codepoint)) {
        ist_lexer_advance_codepoint(this);
    }
}

ist_codepoint ist_lexer_get_next_codepoint(ist_lexer* this) {
    return isl_utf8_decode(
                &this->codepage->source,
                this->codepage->next_sequence_index,
                &this->codepage->current_codepoint_decode_length);
}

void ist_lexer_advance_codepoint(ist_lexer* this) {
    this->codepage->current_codepoint = ist_lexer_get_next_codepoint(this);
    this->codepage->next_sequence_index += this->codepage->current_codepoint_decode_length;
}

ist_bool ist_lexer_match_next_codepoint(ist_lexer* this, ist_codepoint _codepoint) {
    ist_usize index = this->codepage->next_sequence_index;
    if (ist_lexer_get_next_codepoint(this) == _codepoint) {
        this->codepage->next_sequence_index += this->codepage->current_codepoint_decode_length;
        return 1;
    }
    return 0;
}
