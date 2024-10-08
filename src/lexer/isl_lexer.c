#include "isl_lexer.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "isl_memgr.h"
#include "isl_list.h"
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


inline ist_string isl_read_file(ist_string _filepath) {
    FILE* file = fopen(_filepath, "rb");
    isl_ifnreport(file, rid_open_file_failed, _filepath, isp_catch_coreloc);
    fseek(file, 0, SEEK_END);
    ist_usize length = ftell(file);
    fseek(file, 0, SEEK_SET);
    ist_string source = isl_malloc_list(ist_byte, length + 1);
    fread(source, 1, length, file);
    source[length] = '\0';
    fclose(file);
    return source;
}


/* the source and module must be a string that allocated in the heap */
inline ist_codepage* ist_codepage_createby_source(ist_string _source, ist_string _module) {

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

    /* initialize the location */
    ist_location_init(&codepage->location, _module);

    codepage->prev_page = NULL;

    return codepage;

}

ist_string isl_module_name_catchby_filepath(ist_string _filepath) {

    ist_string start = strrchr(_filepath, '/') + 1;
    ist_string end = strrchr(start, '.');

    if (!start) start = _filepath;
    if (!end) end = _filepath + strlen(_filepath);

    return ist_string_consby_ref(start, end - start);
}


inline ist_codepage* ist_codepage_createby_file(ist_string _filepath) {
    return ist_codepage_createby_source(
            isl_read_file(_filepath),
            isl_module_name_catchby_filepath(_filepath));
}

inline void ist_codepage_delete(ist_codepage* this) {
    isl_ifnreport(this, rid_catch_nullptr, isp_catch_coreloc);
    isl_free(this);
}


inline ist_lexer ist_lexer_consby_file(ist_string _filepath) {
    return ist_lexer_consby_codepage(ist_codepage_createby_file(_filepath));
}

inline ist_lexer ist_lexer_consby_source(ist_string _source, ist_string _module) {
    return ist_lexer_consby_codepage(ist_codepage_createby_source(_source, _module));
}

inline ist_lexer ist_lexer_consby_codepage(ist_codepage* _codepage) {
    ist_lexer lexer;
    ist_lexer_initby_codepage(&lexer, _codepage);
    return lexer;
}

inline void ist_lexer_initby_codepage(ist_lexer* this, ist_codepage* _codepage) {
    this->codepage = _codepage;
    this->source_list = isl_malloc_list(ist_string, 1);
    this->module_list = isl_malloc_list(ist_string, 1);
    this->source_list[0] = _codepage->source;
    this->module_list[0] = _codepage->location.module;
    this->source_count = 1;
    this->module_count = 1;
    ist_token_initby_location(&this->pre_token, _codepage->location);
    ist_token_initby_location(&this->cur_token, _codepage->location);
    ist_token_initby_location(&this->nex_token, _codepage->location);
    ist_token_initby_location(&this->sec_token, _codepage->location);
    ist_lexer_advance(this);
    ist_lexer_advance(this);
    ist_lexer_advance(this);
}

inline ist_lexer* ist_lexer_createby_codepage(ist_codepage* _codepage) {
    ist_lexer* lexer = isl_malloc(ist_lexer);
    ist_lexer_initby_codepage(lexer, _codepage);
    return lexer;
}

inline ist_lexer* ist_lexer_createby_source(ist_string _source, ist_string _module) {
    return ist_lexer_createby_codepage(ist_codepage_createby_source(_source, _module));
}

inline ist_lexer* ist_lexer_createby_file(ist_string _filepath) {
    return ist_lexer_createby_codepage(ist_codepage_createby_file(_filepath));
}

inline void ist_lexer_delete(ist_lexer* this) {
    ist_lexer_clean(this);
    isl_free(this);
}

inline void ist_lexer_clean(ist_lexer* this) {
    isl_ifnreport(this, rid_catch_nullptr, isp_catch_coreloc);

    for (ist_usize i = 0; i < this->source_count; i++) {
        if (this->source_list[i]) isl_free_list(this->source_list[i]);
        else isl_report(rid_catch_nullptr, isp_catch_coreloc);
    }
    isl_free_list(this->source_list);

    for (ist_usize i = 0; i < this->module_count; i++) {
        if (this->module_list[i]) ist_string_clean(this->module_list + i);
        else isl_report(rid_catch_nullptr, isp_catch_coreloc);
    }
    isl_free_list(this->module_list);

    //TODO: delete all of the codepage on the chain.
    ist_codepage_delete(this->codepage);
}


inline ist_token* ist_lexer_advance(ist_lexer* this) {

    this->pre_token = this->cur_token;
    this->cur_token = this->nex_token;
    this->nex_token = this->sec_token;

    while (ist_lexer_skip_blanks(this)) {

        ist_token_initby_full(
            &analysis_token,
            ISL_TOKENT_EOF,
            this->codepage->location,
            this->codepage->source
                + this->codepage->next_sequence_index
                - this->codepage->decode_codepoint_length,
            0, (ist_value) { 0 });

        switch (ist_lexer_get_current_codepoint(this)) {
        case ISL_CODEPOINT_EOCCP:
            ist_lexer_advance_codepoint(this);
            continue;
        case ISL_CODEPOINT_ERROR:
            goto ist_lexer_advance_label_ending;

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
            goto ist_lexer_advance_label_ending;

        default:
            if (isdigit(ist_lexer_get_current_codepoint(this))) {
                ist_lexer_parse_number(this);
            } else if (
                    isl_utf8_legal_identifier_codepoint(
                        ist_lexer_get_current_codepoint(this), true)) {
                ist_lexer_parse_identifier(this);
            } else {
                isl_report(
                    rid_unrecongnized_codepoint,
                    this->codepage->location,
                    ist_lexer_get_current_codepoint(this));

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

    if (!ist_lexer_get_current_codepoint(this))
        ist_token_initby_full(
            &analysis_token,
            ISL_TOKENT_EOF,
            this->codepage->location,
            this->codepage->source
                + this->codepage->next_sequence_index
                - this->codepage->decode_codepoint_length,
            0, (ist_value) { 0 });

ist_lexer_advance_label_ending:
    return &this->pre_token;
}

inline void ist_lexer_switch_codepage(ist_lexer* this, ist_codepage* _codepage) {
    _codepage->prev_page = this->codepage;
    this->codepage = _codepage;

    isl_list_ensurec(this->source_list, this->source_count, 1);
    this->source_list[this->source_count++] = _codepage->source;

    isl_list_ensurec(this->module_list, this->module_count, 1);
    this->module_list[this->module_count++] = _codepage->location.module;
}


inline ist_codepoint ist_lexer_skip_blanks(ist_lexer* this) {
    while (isspace(ist_lexer_get_current_codepoint(this))) {
        ist_lexer_advance_codepoint(this);
    }
    return ist_lexer_get_current_codepoint(this);
}

inline void ist_lexer_parse_identifier(ist_lexer* this) {
    analysis_token.type = ISL_TOKENT_ID;

    /* skip identifier context */
    while (
        isl_utf8_legal_identifier_codepoint(
            ist_lexer_get_current_codepoint(this), false)
     ) ist_lexer_advance_codepoint(this);

    /*
        because the current codepoint is not belong this identifier,
        so we should sub with the length of the current codepoint.
    */
    analysis_token.length =
        (this->codepage->source
            + this->codepage->next_sequence_index)
        - analysis_token.extract - this->codepage->decode_codepoint_length;

}

inline void ist_lexer_parse_number(ist_lexer* this) {
    ist_usize dot_count = 0;
    while (isdigit(ist_lexer_get_current_codepoint(this))
            || ist_lexer_get_current_codepoint(this) == '.') {
        if (ist_lexer_get_current_codepoint(this) == '.')
            ++dot_count;
        ist_lexer_advance_codepoint(this);
    }
    isl_ifreport(dot_count > 1, rid_is_it_the_version_code, analysis_token.location);

    /* is it a integer number or a real number? */
    analysis_token.type = dot_count ? ISL_TOKENT_REAL_LITERAL : ISL_TOKENT_INTEGER_LITERAL;

    /*
        because the current codepoint is not belong this number,
        so we should sub with the length of the current codepoint.
    */
    analysis_token.length =
        (this->codepage->source
            + this->codepage->next_sequence_index)
        - analysis_token.extract - this->codepage->decode_codepoint_length;

    /* convert the extract to number */
    if (dot_count) analysis_token.value.real_value = atof(analysis_token.extract);
    else analysis_token.value.int_value = atol(analysis_token.extract);

}

inline void ist_lexer_parse_string(ist_lexer* this) {
    analysis_token.type = ISL_TOKENT_STRING_LITERAL;

    /* skip the starting double quote. */
    ist_lexer_advance_codepoint(this);

    while (ist_lexer_get_current_codepoint(this) != '"') {
        if (ist_lexer_get_current_codepoint(this) == '\0') {
            isl_report(rid_unterminated_string, analysis_token.location);
            return;
        }
        ist_lexer_advance_codepoint(this);
    }

    /*
        skip the starting double quote, trim it from the extract,
        because it is a ASCII character, so we should +1.
    */
    ++analysis_token.extract;

    /*
        because the current codepoint is not belong this identifier,
        so we should sub with the length of the current codepoint.
    */
    analysis_token.length =
        (this->codepage->source
            + this->codepage->next_sequence_index)
        - analysis_token.extract - this->codepage->decode_codepoint_length;

    ist_lexer_advance_codepoint(this);
}

inline void ist_lexer_skip_comment(ist_lexer* this, ist_bool _is_block) {
    while (true) {
        if (_is_block) {
            if (ist_lexer_get_current_codepoint(this) == '*'
                && ist_lexer_get_next_codepoint(this) == '/') {

                /* skip comment block ending symbols */
                ist_lexer_advance_codepoint(this);
                ist_lexer_advance_codepoint(this);
                return;

            } else if (ist_lexer_get_current_codepoint(this) == '\0') {
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


inline ist_codepoint ist_lexer_get_next_codepoint(ist_lexer* this) {
    return isl_utf8_decode(
            &this->codepage->source,
            this->codepage->next_sequence_index,
            &this->codepage->decode_codepoint_length);
}

/* return the current codepoint, and advance to the next codepoint */
inline void ist_lexer_advance_codepoint(ist_lexer* this) {

    if (!ist_lexer_get_current_codepoint(this)) {
        isl_report(rid_advance_codepoint_when_eof, isp_catch_coreloc);
        return;
    }

    /* switch to the previous codepage, and delete the current codepage */
    else if (ist_lexer_get_current_codepoint(this) == ISL_CODEPOINT_EOCCP) {
        ist_codepage* codepage = this->codepage;
        this->codepage = this->codepage->prev_page;
        ist_codepage_delete(codepage);
        return;
    }

    if (!ist_lexer_get_next_codepoint(this) && this->codepage->prev_page) {
        this->codepage->current_codepoint = ISL_CODEPOINT_EOCCP;
        /*
            because we must advance next_sequence_index,
            and the next codepoint is ASCII::EOF, so we should +1.
        */
        ++this->codepage->next_sequence_index;
        return;
    }

    /* update the location */
    ++this->codepage->location.column;
    if (ist_lexer_get_current_codepoint(this) == '\n') {
        ++this->codepage->location.line;
        this->codepage->location.column = 1;
    }

    /* update the current codepoint and the next codepoint index */
    this->codepage->current_codepoint = ist_lexer_get_next_codepoint(this);
    this->codepage->next_sequence_index += this->codepage->decode_codepoint_length;

}

inline ist_codepoint ist_lexer_get_current_codepoint(ist_lexer* this) {
    return this->codepage->current_codepoint;
}


/*
    if next codepoint matches arg codepoint, then advance to the next codepoint,
    and return true, otherwise return false.
*/
inline ist_bool ist_lexer_match_current_codepoint(ist_lexer* this, ist_codepoint _codepoint) {
    if (ist_lexer_get_current_codepoint(this) != _codepoint) return false;
    ist_lexer_advance_codepoint(this);
    return true;
}
