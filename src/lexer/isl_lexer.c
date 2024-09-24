#include "isl_lexer.h"

#include <ctype.h>
#include <stdlib.h>

#include "isl_memgr.h"
#include "isl_utf8.h"
#include "isl_report.h"

#define analysis_token (this->sec_token)

#define CHECK_SINCHAR_TOKENT(_char, _tokent) \
    case _char:analysis_token.type=_tokent;break

#define CHECK_BINCHAR_TOKENT(_char, _sufchar, _unary_tokent, _binary_tokent) \
case _char:                                                                  \
    if(ist_lexer_get_next_codepoint(this)==_sufchar){                        \
        ist_lexer_advance_codepoint(this);                                   \
        analysis_token.type=_binary_tokent;break;                            \
    }   analysis_token.type=_unary_tokent;break;

#define CHECK_BINCHAR_TOKENT_HEAD(_char, _sufchar, _binary_tokent) \
case _char:                                                        \
    if(ist_lexer_get_next_codepoint(this)==_sufchar){              \
        ist_lexer_advance_codepoint(this);                         \
        analysis_token.type=_binary_tokent;break;                  \
    }

#define CHECK_BINCHAR_TOKENT_BODY(_sufchar, _binary_tokent) \
    else if(ist_lexer_get_next_codepoint(this)==_sufchar){  \
        ist_lexer_advance_codepoint(this);                  \
        analysis_token.type=_binary_tokent;break;           \
    }

#define CHECK_BINCHAR_TOKENT_TAIL(_unary_tokent) \
        analysis_token.type=_unary_tokent;break


ist_codepage* ist_codepage_createby_source(ist_string _source) {

    ist_codepage* codepage = isl_malloc(ist_codepage);

    codepage->source = _source;
    codepage->next_sequence_index = 0;
    codepage->decode_codepoint_length = 0;

    /* decode the first utf8 sequence */
    codepage->current_codepoint =
        isl_utf8_decode(&codepage->source,
                        codepage->next_sequence_index,
                        &codepage->decode_codepoint_length);
    codepage->next_sequence_index += codepage->decode_codepoint_length;

    /* initialize the location, module name is <buildin-source> */
    ist_location_init(&codepage->location, "<buildin-source>");

    codepage->prev_page = codepage->next_page = NULL;

    return codepage;

}

ist_codepage* ist_codepage_createby_file(ist_string _file_path) {
    FILE* file = fopen(_file_path, "r");
    isl_ifnreport(file, rid_open_file_failed, _file_path, isp_catch_coreloc);

    ist_codepage* codepage = ist_codepage_createby_source("ist_string_create(file)");
    fclose(file);
    return codepage;
}

void ist_lexer_init(ist_lexer* this, ist_codepage* _codepage) {
    this->codepage = _codepage;
    ist_token_init_with_location(&this->pre_token, _codepage->location);
    ist_token_init_with_location(&this->cur_token, _codepage->location);
    ist_token_init_with_location(&this->nex_token, _codepage->location);
    ist_token_init_with_location(&this->sec_token, _codepage->location);
    ist_lexer_advance(this);
    ist_lexer_advance(this);
    ist_lexer_advance(this);
}

ist_lexer* ist_lexer_create(ist_codepage* _codepage) {
    ist_lexer* lexer = isl_malloc(ist_lexer);
    ist_lexer_init(lexer, _codepage);
    return lexer;
}

ist_lexer* ist_lexer_createby_source(ist_string* _source) {
    return ist_lexer_create(ist_codepage_createby_source(*_source));
}

ist_lexer* ist_lexer_createby_file(ist_string* _file_path) {
    return ist_lexer_create(ist_codepage_createby_file(*_file_path));
}


ist_token* ist_lexer_advance(ist_lexer* this) {

    this->pre_token = this->cur_token;
    this->cur_token = this->nex_token;
    this->nex_token = this->sec_token;

    if (!this->codepage->current_codepoint)
        ist_token_init_full(
            &analysis_token,
            ISL_TOKENT_EOF,
            this->codepage->location,
            this->codepage->source
                + this->codepage->next_sequence_index
                - this->codepage->decode_codepoint_length,
            0, (ist_value) { 0 });

    while (this->codepage->current_codepoint != 0) {
        ist_lexer_skip_blanks(this);

        ist_token_init_full(
            &analysis_token,
            ISL_TOKENT_EOF,
            this->codepage->location,
            this->codepage->source
                + this->codepage->next_sequence_index
                - this->codepage->decode_codepoint_length,
            0, (ist_value) { 0 });

        switch (this->codepage->current_codepoint) {

            CHECK_SINCHAR_TOKENT('(', ISL_TOKENT_LPARE);
            CHECK_SINCHAR_TOKENT(')', ISL_TOKENT_RPARE);

            CHECK_SINCHAR_TOKENT('{', ISL_TOKENT_LBRACE);
            CHECK_SINCHAR_TOKENT('}', ISL_TOKENT_RBRACE);

            CHECK_SINCHAR_TOKENT('[', ISL_TOKENT_LBRACKET);
            CHECK_SINCHAR_TOKENT(']', ISL_TOKENT_RBRACKET);

            CHECK_SINCHAR_TOKENT(',', ISL_TOKENT_COMMA);
            CHECK_SINCHAR_TOKENT(';', ISL_TOKENT_EOS);
            CHECK_SINCHAR_TOKENT(':', ISL_TOKENT_COLON);
            CHECK_SINCHAR_TOKENT('.', ISL_TOKENT_DOT);
            CHECK_SINCHAR_TOKENT('?', ISL_TOKENT_QUESTION);

            CHECK_BINCHAR_TOKENT('=', '=', ISL_TOKENT_ASSIGN, ISL_TOKENT_EQUAL);
            CHECK_BINCHAR_TOKENT('!', '=', ISL_TOKENT_LNOT, ISL_TOKENT_NOTEQUAL);

            CHECK_BINCHAR_TOKENT('&', '&', ISL_TOKENT_AND, ISL_TOKENT_LAND);
            CHECK_BINCHAR_TOKENT('|', '|', ISL_TOKENT_OR, ISL_TOKENT_LOR);

            CHECK_BINCHAR_TOKENT('*', '=', ISL_TOKENT_MUL, ISL_TOKENT_MUL_ASSIGN);
            CHECK_BINCHAR_TOKENT('%', '=', ISL_TOKENT_MOD, ISL_TOKENT_MOD_ASSIGN);

            CHECK_BINCHAR_TOKENT_HEAD('<', '=', ISL_TOKENT_LESSEQUAL)
                CHECK_BINCHAR_TOKENT_BODY('<', ISL_TOKENT_LSHIFT)
                CHECK_BINCHAR_TOKENT_TAIL(ISL_TOKENT_LESSTHAN);

            CHECK_BINCHAR_TOKENT_HEAD('>', '=', ISL_TOKENT_GREATEQUAL)
                CHECK_BINCHAR_TOKENT_BODY('>', ISL_TOKENT_RSHIFT)
                CHECK_BINCHAR_TOKENT_TAIL(ISL_TOKENT_GREATERTHAN);

            CHECK_BINCHAR_TOKENT_HEAD('+', '=', ISL_TOKENT_ADD_ASSIGN)
                CHECK_BINCHAR_TOKENT_BODY('+', ISL_TOKENT_SELFADD)
                CHECK_BINCHAR_TOKENT_TAIL(ISL_TOKENT_ADD);

            CHECK_BINCHAR_TOKENT_HEAD('-', '=', ISL_TOKENT_SUB_ASSIGN)
                CHECK_BINCHAR_TOKENT_BODY('-', ISL_TOKENT_SELFSUB)
                CHECK_BINCHAR_TOKENT_BODY('>', ISL_TOKENT_WRAPPER)
                CHECK_BINCHAR_TOKENT_TAIL(ISL_TOKENT_SUB);

        case '/': {
            ist_codepoint next_codepoint = ist_lexer_get_next_codepoint(this);
            if (ist_lexer_get_next_codepoint(this) == '=') {
                ist_lexer_advance_codepoint(this);
                analysis_token.type = ISL_TOKENT_DIV_ASSIGN; break;
            } else if (next_codepoint == '/' || next_codepoint == '*') {
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
            } else if (isl_utf8_legal_identifier_codepoint(this->codepage->current_codepoint, true)) {
                ist_lexer_parse_identifier(this);
            } else {
                isl_report(
                    rid_unrecongnized_codepoint,
                    this->codepage->location,
                    this->codepage->current_codepoint
                );
                ist_lexer_advance_codepoint(this);
                continue;
            }
            goto ist_lexer_advance_label_ending;
        }

        analysis_token.length =
            (this->codepage->source
                + this->codepage->next_sequence_index)
            - analysis_token.extract;

        ist_lexer_advance_codepoint(this);
        break;
    }
ist_lexer_advance_label_ending:
    return &this->pre_token;
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
             this->codepage->current_codepoint, false)
     ) ist_lexer_advance_codepoint(this);

    /* because the next codepoint is not belong this identifier, we should -1 */
    analysis_token.length =
        (this->codepage->source
            + this->codepage->next_sequence_index)
        - analysis_token.extract - 1;

}

void ist_lexer_parse_number(ist_lexer* this) {
    ist_usize dot_count = 0;
    while (isdigit(this->codepage->current_codepoint) || this->codepage->current_codepoint == '.') {
        if (this->codepage->current_codepoint == '.')
            ++dot_count;
        ist_lexer_advance_codepoint(this);
    }
    isl_ifreport(dot_count > 1, rid_is_it_the_version_code, this->codepage->location);

    /* is it a integer number or a real number? */
    analysis_token.type = dot_count ? ISL_TOKENT_REAL : ISL_TOKENT_INT;

    /* because the next codepoint is not belong this number, we should -1 */
    analysis_token.length =
        (this->codepage->source
            + this->codepage->next_sequence_index)
        - analysis_token.extract - 1;

    /* convert the extract to number */
    if (dot_count)  analysis_token.value.real_value = atof(analysis_token.extract);
    else            analysis_token.value.int_value = atol(analysis_token.extract);

}

void ist_lexer_parse_string(ist_lexer* this) {

}

void ist_lexer_skip_comment(ist_lexer* this, ist_bool _is_block) {
    while (true) {
        if (_is_block) {
            if (this->codepage->current_codepoint == '*'
                && ist_lexer_get_next_codepoint(this) == '/') {
                /* skip comment block ending symbols */
                ist_lexer_advance_codepoint(this);
                ist_lexer_advance_codepoint(this);
                return;
            } else if (this->codepage->current_codepoint == '\0') {
                isl_report(rid_unterminated_comment_block, this->codepage->location);
                return;
            }
        } else if (ist_lexer_match_current_codepoint(this, '\n')) {
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
                &this->codepage->decode_codepoint_length);
}

/* return the current codepoint, and advance to the next codepoint */
ist_codepoint ist_lexer_advance_codepoint(ist_lexer* this) {

    if (this->codepage->current_codepoint == 0) {
        isl_report(rid_advance_codepoint_when_eof, isp_catch_coreloc);
        return -1;
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
    this->codepage->next_sequence_index += this->codepage->decode_codepoint_length;

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
