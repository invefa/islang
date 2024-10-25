#ifndef ISC_LEXER_H
#define ISC_LEXER_H

#include "isl_module.h"
#include "isl_token.h"
#include "isl_utf8.h"


ist_string isl_read_file(ist_cstring _filepath);

/*
    Codepage is a tool for maintaining the source code file currently being read.
    It also provides methods for future macro support. It maintains location and source,
    current is used for token generation, and the module in location can provide relevant
    context information of the macro when expanding the macro.
*/
typedef struct ist_codepage {

    /*
        The name of this codepage, it will be used in reporting message.
        And if it was nullptr, indicate that it is the default codepage.
    */
    // ist_string name;

    /* the module which this codepage belongs to */
    // ist_module* module;

    /* the source code read from the source code file */
    ist_string source;

    /*
        when codepage is created, it will decode the first utf8 sequence,
        and the index of next utf8 sequence will be stored here.
    */
    ist_usize next_sequence_index;


    /* when codepage is created, decode the first utf8 sequence, and store the codepoint there */
    ist_codepoint current_codepoint;
    ist_u8        decode_codepoint_length;

    /* the current location in the source code file */
    ist_location location;

    /*
        if we need to expand macro, we will create a new codepage for macro expansion,
        then store the current codepage to this member. and when the macro expansion is done,
        the new codepage will be set to the lexer for token generation. And when the new codepage is
       read, it will be released after the current codepage restored.
    */
    struct ist_codepage* prev_page;


} ist_codepage;

ist_codepage* ist_codepage_createby_full(
    ist_string    _name,
    ist_module*   _module,
    ist_string    _source,
    ist_codepage* _prev_page
);

ist_codepage* ist_codepage_createby_filepath(ist_module* _module, ist_string _filepath);

ist_codepage* ist_codepage_createby_source(
    ist_module* _module,
    ist_string  _name,
    ist_string  _source
);

ist_codepage* ist_codepage_createby_string(
    ist_module* _module,
    ist_string  _name,
    ist_string  _string,
    ist_usize   _length
);

void ist_codepage_delete(ist_codepage* this);
void ist_codepage_delete_chain(ist_codepage* this);


// TODO: add an error signal.
typedef struct ist_lexer {

    ist_module*   module;   /*  the module which this lexer belongs to               */
    ist_codepage* codepage; /*  the codepage currently being read                    */
    ist_token               /*  the lexer will provide a table of token              */
        pre_token,          /*  previous token, the token before the current token   */
        cur_token,          /*  current token, the current token                     */
        nex_token,          /*  next token, the token after the current token        */
        sec_token;          /*  second token, the token after the next token         */
    // ana_token;                 /*  analysis token, a temporary token                    */

    ist_token* ahead_token_list;   /*  the list of ahead tokens            */
    ist_usize  ahead_token_count;  /*  the count of ahead tokens           */
    ist_usize  ahead_token_index;  /*  the index of current ahead token    */

    ist_usize* ahead_backup_stack; /*  the stack of backup index           */
    ist_usize  ahead_backup_count; /*  the count of backup index           */

} ist_lexer;

ist_lexer ist_lexer_consby_full(ist_module* _module, ist_codepage* _codepage);

ist_lexer ist_lexer_consby_module(ist_module* _module);

void ist_lexer_clean(ist_lexer* this);
void ist_lexer_delete(ist_lexer* this);

void ist_lexer_lookahead_start(ist_lexer* this);
void ist_lexer_lookahead_end(ist_lexer* this);

void       ist_lexer_lex(ist_lexer* this);
ist_token* ist_lexer_advance(ist_lexer* this);

void ist_lexer_switch_codepage(ist_lexer* this, ist_codepage* _codepage);

void          ist_lexer_advance_codepoint(ist_lexer* this);
ist_codepoint ist_lexer_get_next_codepoint(ist_lexer* this);
ist_codepoint ist_lexer_get_current_codepoint(ist_lexer* this);
ist_bool      ist_lexer_match_current_codepoint(ist_lexer* this, ist_codepoint _codepoint);

void          ist_lexer_skip_comment(ist_lexer* this, ist_bool _is_block);
ist_codepoint ist_lexer_skip_blanks(ist_lexer* this);

void ist_lexer_parse_identifier(ist_lexer* this);
void ist_lexer_parse_number(ist_lexer* this);
void ist_lexer_parse_string(ist_lexer* this);

#endif // ISC_LEXER_H