#include "isl_lexer.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "isl_list.h"
#include "isl_memgr.h"
#include "isl_module.h"
#include "isl_report.h"
#include "isl_utf8.h"


#define analysis_token (this->ana_token)

#define CHECK_SINCHAR_TOKENT(_char, _tokent) \
    case _char:                              \
        analysis_token.type = _tokent;       \
        break

#define CHECK_BINCHAR_TOKENT(_char, _sufchar, _unary_tokent, _binary_tokent) \
    case _char:                                                              \
        if (ist_lexer_get_next_codepoint(this) == _sufchar) {                \
            ist_lexer_advance_codepoint(this);                               \
            analysis_token.type = _binary_tokent;                            \
            break;                                                           \
        }                                                                    \
        analysis_token.type = _unary_tokent;                                 \
        break;

#define CHECK_BINCHAR_TOKENT_HEAD(_char, _sufchar, _binary_tokent) \
    case _char:                                                    \
        if (ist_lexer_get_next_codepoint(this) == _sufchar) {      \
            ist_lexer_advance_codepoint(this);                     \
            analysis_token.type = _binary_tokent;                  \
            break;                                                 \
        }

#define CHECK_BINCHAR_TOKENT_BODY(_sufchar, _binary_tokent)    \
    else if (ist_lexer_get_next_codepoint(this) == _sufchar) { \
        ist_lexer_advance_codepoint(this);                     \
        analysis_token.type = _binary_tokent;                  \
        break;                                                 \
    }

#define CHECK_BINCHAR_TOKENT_TAIL(_unary_tokent) \
    analysis_token.type = _unary_tokent;         \
    break

inline ist_string isl_read_file(ist_cstring _filepath) {
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

ist_codepage* ist_codepage_createby_full(
    ist_string    _name,
    ist_module*   _module,
    ist_string    _source,
    ist_codepage* _prev_page
) {

    isl_assert(_module, "codepage must belong to a module.");

    ist_codepage* this = isl_malloc(ist_codepage);

    /* initialize the basic information */
    // this->name = _name;
    // this->module = _module;
    this->source    = _source;
    this->prev_page = _prev_page;

    /* register the string buffer to the module */
    ist_module_register_strbuf(_module, _name, ISL_STRBUFT_NAME);
    ist_module_register_strbuf(_module, _source, ISL_STRBUFT_SOURCE);

    /* initialize the UTF-8 decode information */
    this->next_sequence_index     = 0;
    this->decode_codepoint_length = 0;

    /* decode the first utf8 sequence */
    this->current_codepoint =
        isl_utf8_decode(&this->source, this->next_sequence_index, &this->decode_codepoint_length);
    this->next_sequence_index += this->decode_codepoint_length;

    /* initialize the location */
    this->location = ist_location_consby_full(_module, _name);

    return this;
}
ist_codepage* ist_codepage_createby_filepath(ist_module* _module, ist_string _filepath) {
    ist_string source = isl_read_file(_filepath);
    return ist_codepage_createby_full(NULL, _module, source, NULL);
}
ist_codepage* ist_codepage_createby_source(
    ist_module* _module,
    ist_string  _name,
    ist_string  _source
) {
    return ist_codepage_createby_full(_name, _module, _source, NULL);
}
ist_codepage* ist_codepage_createby_string(
    ist_module* _module,
    ist_string  _name,
    ist_string  _string,
    ist_usize   _length
) {
    return ist_codepage_createby_full(
        _name,
        _module,
        ist_string_consby_ref(_string, _length),
        NULL
    );
}

inline void ist_codepage_delete(ist_codepage* this) {
    isl_ifnreport(this, rid_catch_nullptr, isp_catch_coreloc);
    isl_free(this);
}
inline void ist_codepage_delete_chain(ist_codepage* this) {
    isl_ifnreport(this, rid_catch_nullptr, isp_catch_coreloc);
    if (this->prev_page) ist_codepage_delete_chain(this->prev_page);
    isl_free(this);
}

ist_lexer ist_lexer_consby_full(ist_module* _module, ist_codepage* _codepage) {
    ist_lexer lexer = (ist_lexer){

        .module   = _module,
        .codepage = _codepage,

        .pre_token = ist_token_consby_location(_codepage->location),
        .cur_token = ist_token_consby_location(_codepage->location),
        .sec_token = ist_token_consby_location(_codepage->location),
        .nex_token = ist_token_consby_location(_codepage->location),
        .ana_token = ist_token_consby_location(_codepage->location),

        .ahead_token_list  = NULL,
        .ahead_token_count = 0,
        .ahead_token_index = 0,

        .ahead_backup_stack = NULL,
        .ahead_backup_count = 0

    };

    ist_lexer_advance(&lexer);
    ist_lexer_advance(&lexer);
    ist_lexer_advance(&lexer);
    return lexer;
}

ist_lexer ist_lexer_consby_module(ist_module* _module) {
    return ist_lexer_consby_full(
        _module,
        ist_codepage_createby_filepath(_module, _module->filepath)
    );
}

inline void ist_lexer_clean(ist_lexer* this) {
    isl_ifnreport(this, rid_catch_nullptr, isp_catch_coreloc);
    ist_codepage_delete_chain(this->codepage);

    if (this->ahead_token_list) isl_freev_list(this->ahead_token_list);
    if (this->ahead_backup_stack) isl_freev_list(this->ahead_backup_stack);
}
inline void ist_lexer_delete(ist_lexer* this) {
    ist_lexer_clean(this);
    isl_free(this);
}

#define ist_lexer_ahead_store_backup() \
    isl_list_addm(this->ahead_backup_stack, this->ahead_backup_count, this->ahead_token_index - 3)

#define ist_lexer_ahead_store_token(_token) \
    isl_list_addm(this->ahead_token_list, this->ahead_token_count, _token)

void ist_lexer_lookahead_start(ist_lexer* this) {

    if (!this->ahead_token_list) {
        this->ahead_token_list = isl_malloc_list(ist_token, 8);
        isl_assert(!this->ahead_token_count);
    }

    if (!this->ahead_backup_stack) {
        this->ahead_backup_stack = isl_malloc_list(ist_usize, 4);
        isl_assert(!this->ahead_backup_count);
    }

    if (!this->ahead_token_count) {
        ist_lexer_ahead_store_token(this->pre_token);
        ist_lexer_ahead_store_token(this->cur_token);
        ist_lexer_ahead_store_token(this->nex_token);
        ist_lexer_ahead_store_token(this->sec_token);
        this->ahead_token_index = this->ahead_token_count;
    }

    ist_lexer_ahead_store_backup();
}
void ist_lexer_lookahead_end(ist_lexer* this) {
    isl_assert(this->ahead_token_count && this->ahead_backup_count);

    this->ahead_token_index = this->ahead_backup_stack[--this->ahead_backup_count];

    this->pre_token = this->ahead_token_list[this->ahead_token_index - 1];
    this->cur_token = this->ahead_token_list[this->ahead_token_index++];
    this->nex_token = this->ahead_token_list[this->ahead_token_index++];
    this->sec_token = this->ahead_token_list[this->ahead_token_index++];
}

inline ist_token ist_lexer_lex(ist_lexer* this) {

    while (ist_lexer_skip_blanks(this)) {

        switch (ist_lexer_get_current_codepoint(this)) {
            case ISL_CODEPOINT_EOCCP:
                ist_lexer_advance_codepoint(this);
                continue;
            case ISL_CODEPOINT_ERROR:
                return analysis_token;

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
                    analysis_token.type = ISL_TOKENT_DIV_ASSIGN;
                    break;
                } else if (next_codepoint == '/' || next_codepoint == '*') {
                    ist_lexer_advance_codepoint(this);
                    ist_lexer_advance_codepoint(this);
                    ist_lexer_skip_comment(this, next_codepoint == '*');
                    continue;
                }
                analysis_token.type = ISL_TOKENT_DIV;
                break;
            }

            case '"':
                ist_lexer_parse_string(this);
                return analysis_token;

            default:
                if (isdigit(ist_lexer_get_current_codepoint(this))) {
                    ist_lexer_parse_number(this);
                } else if (isl_utf8_legal_identifier_codepoint(
                               ist_lexer_get_current_codepoint(this),
                               true
                           ))
                {
                    ist_lexer_parse_identifier(this);
                } else {
                    isl_report(
                        rid_unrecongnized_codepoint,
                        this->codepage->location,
                        ist_lexer_get_current_codepoint(this)
                    );

                    ist_lexer_advance_codepoint(this);
                    continue;
                }
                return analysis_token;
        }

        analysis_token.length =
            (this->codepage->source + this->codepage->next_sequence_index) - analysis_token.extract;

        ist_lexer_advance_codepoint(this);
        break;
    }
    return analysis_token;
}

inline ist_token* ist_lexer_advance(ist_lexer* this) {

    /*
        define some macro to make the logic of
        following code more easy to digest.
    */
#define lookaheading     (this->ahead_backup_count)
#define has_ahead_token  (this->ahead_token_count)
#define no_remain_token  (this->ahead_token_index == this->ahead_token_count)
#define read_from_list() this->sec_token = this->ahead_token_list[this->ahead_token_index++]

    isl_ifnreport(
        this->ahead_token_index <= this->ahead_token_count,
        rid_catch_size_overflow,
        isp_catch_coreloc
    );

    this->pre_token = this->cur_token;
    this->cur_token = this->nex_token;
    this->nex_token = this->sec_token;

    if (has_ahead_token) {
        if (no_remain_token) {

            /*
                when the remain token has been consumed,
                we can only read the next token from the source.
            */
            this->sec_token = ist_lexer_lex(this);

            if (lookaheading) {
                /*
                    at that case, the token will be stored to the ahead list,
                    and we advance the ahead_token_index to ensure it behind at sec_token.
                */
                ist_lexer_ahead_store_token(this->sec_token);
                ++this->ahead_token_index;

            } else this->ahead_token_count = this->ahead_token_index = 0;
        } else read_from_list();

    } else /* the common case if we have no any ahead token */
        this->sec_token = ist_lexer_lex(this);

    return &this->pre_token;

    /* undef for macros */
#undef lookaheading
#undef has_ahead_token
#undef has_remain_token
#undef read_from_list
}

inline void ist_lexer_switch_codepage(ist_lexer* this, ist_codepage* _codepage) {
    _codepage->prev_page = this->codepage;

    if (this->codepage->location.pagename) {

        /*
            interesting, when we catch the length of the pagename,
            the both of last char '\0' also was claculated within the length,
            so we don't need to add 2 to the length for the appending of '.' and '\0'.
        */
        ist_usize   index  = 0;
        ist_string* buffer = ist_string_create_buffer(
            isl_list_catch_length(this->codepage->location.pagename)
            + isl_list_catch_length(_codepage->location.pagename)
        );

        ist_string_buffer_append_raw(buffer, &index, this->codepage->location.pagename);
        ist_string_buffer_append_raw(buffer, &index, ".");
        ist_string_buffer_append_raw(buffer, &index, _codepage->location.pagename);

        ist_module_register_strbuf(
            this->codepage->location.module,
            _codepage->location.pagename = *buffer,
            ISL_STRBUFT_NAME
        );

        isl_freev(buffer);
    }

    this->codepage = _codepage;
}

inline ist_codepoint ist_lexer_skip_blanks(ist_lexer* this) {
    while (isl_utf8_is_space_codepoint(ist_lexer_get_current_codepoint(this)))
        ist_lexer_advance_codepoint(this);

    /* when skip blanks was done, we should reset analysis_token again */
    analysis_token = ist_token_consby_full(
        ISL_TOKENT_EOF,
        this->codepage->location,
        this->codepage->source + this->codepage->next_sequence_index
            - this->codepage->decode_codepoint_length,
        0,
        ist_value_consby_i64(0)
    );

    return ist_lexer_get_current_codepoint(this);
}

inline void ist_lexer_parse_identifier(ist_lexer* this) {

    /* skip identifier context */
    while (isl_utf8_legal_identifier_codepoint(ist_lexer_get_current_codepoint(this), false))
        ist_lexer_advance_codepoint(this);

    /*
        because the current codepoint is not belong this identifier,
        so we should sub with the length of the current codepoint.
    */
    analysis_token.length = (this->codepage->source + this->codepage->next_sequence_index)
                          - analysis_token.extract - this->codepage->decode_codepoint_length;

    analysis_token.type = ist_string_is_keyword(analysis_token.extract, analysis_token.length);
}

inline void ist_lexer_parse_number(ist_lexer* this) {
    ist_usize dot_count = 0;
    while (isdigit(ist_lexer_get_current_codepoint(this))
           || ist_lexer_get_current_codepoint(this) == '.')
    {
        if (ist_lexer_get_current_codepoint(this) == '.') ++dot_count;
        ist_lexer_advance_codepoint(this);
    }
    isl_ifreport(dot_count > 1, rid_is_it_the_version_code, analysis_token.location);

    /* is it a integer number or a real number? */
    analysis_token.type = dot_count ? ISL_TOKENT_VL_REAL : ISL_TOKENT_VL_INT;

    /*
        because the current codepoint is not belong this number,
        so we should sub with the length of the current codepoint.
    */
    analysis_token.length = (this->codepage->source + this->codepage->next_sequence_index)
                          - analysis_token.extract - this->codepage->decode_codepoint_length;

    /* convert the extract to number */
    if (dot_count) analysis_token.value.real_value = atof(analysis_token.extract);
    else analysis_token.value.int_value = atol(analysis_token.extract);
}

inline void ist_lexer_parse_string(ist_lexer* this) {
    analysis_token.type = ISL_TOKENT_VL_STRING;

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
    analysis_token.length = (this->codepage->source + this->codepage->next_sequence_index)
                          - analysis_token.extract - this->codepage->decode_codepoint_length;

    ist_lexer_advance_codepoint(this);
}

inline void ist_lexer_skip_comment(ist_lexer* this, ist_bool _is_block) {
    while (true) {
        if (_is_block) {
            if (ist_lexer_get_current_codepoint(this) == '*'
                && ist_lexer_get_next_codepoint(this) == '/')
            {

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
        &this->codepage->decode_codepoint_length
    );
}

/* return the current codepoint, and advance to the next codepoint */
inline void ist_lexer_advance_codepoint(ist_lexer* this) {

    if (!ist_lexer_get_current_codepoint(this)) {
        isl_report(rid_advance_codepoint_when_eof, isp_catch_coreloc);
        return;
    }

    /* switch to the previous codepage, and delete the current codepage */
    else if (ist_lexer_get_current_codepoint(this) == ISL_CODEPOINT_EOCCP)
    {
        ist_codepage* codepage = this->codepage;
        this->codepage         = this->codepage->prev_page;
        ist_codepage_delete(codepage);
        return;
    }

    if (!ist_lexer_get_next_codepoint(this) && this->codepage->prev_page) {
        this->codepage->current_codepoint = ISL_CODEPOINT_EOCCP;
        /*
            because we must advance next_sequence_index (to be compatible with parse_identifier),
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
    this->codepage->current_codepoint    = ist_lexer_get_next_codepoint(this);
    this->codepage->next_sequence_index += this->codepage->decode_codepoint_length;
}

/* return the current codepoint */
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
