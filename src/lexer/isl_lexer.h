#ifndef ISL_LEXER_H
#define ISL_LEXER_H

#include "isl_token.h"
#include "isl_utf8.h"

ist_string isl_read_file(ist_string _file_path);

/*
    Codepage is a tool for maintaining the source code file currently being read.
    It also provides methods for future macro support. It maintains location and source,
    current is used for token generation, and the module in location can provide relevant
    context information of the macro when expanding the macro.
*/
typedef struct ist_codepage {

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
        the new codepage will be set to the lexer for token generation. And when the new codepage is read,
        it will be released after the current codepage restored.
    */
    struct ist_codepage* prev_page;


} ist_codepage;

ist_codepage* ist_codepage_createby_source(ist_string _source);
ist_codepage* ist_codepage_createby_file(ist_string _file_path);
// ist_codepage* ist_codepage_createby_string(ist_string _string, ist_usize _length);

void ist_codepage_delete(ist_codepage* this);


typedef struct ist_lexer {

    /*
        The lexer will provide a table of token.
        pre_token:  the token before the current token.
        cur_token:  the current token.
        nex_token:  the token after the current token.
        sec_token:  the token after the nex_token.
    */
    ist_token pre_token, cur_token, nex_token, sec_token;

    ist_string* source_list;
    ist_usize source_count;

    ist_codepage* codepage;

    //TODO: add an error signal.

} ist_lexer;

ist_lexer* ist_lexer_createby_codepage(ist_codepage* _codepage);
ist_lexer* ist_lexer_createby_source(ist_string* _source);
ist_lexer* ist_lexer_createby_file(ist_string _file_path);
void       ist_lexer_delete(ist_lexer* this);

void ist_lexer_init(ist_lexer* this, ist_codepage* _codepage);
void ist_lexer_clean(ist_lexer* this);

ist_token* ist_lexer_advance(ist_lexer* this);

ist_codepoint ist_lexer_advance_codepoint(ist_lexer* this);
ist_codepoint ist_lexer_get_current_codepoint(ist_lexer* this);
ist_codepoint ist_lexer_get_next_codepoint(ist_lexer* this);
ist_bool      ist_lexer_match_current_codepoint(ist_lexer* this, ist_codepoint _codepoint);

void          ist_lexer_skip_comment(ist_lexer* this, ist_bool _is_block);
ist_codepoint ist_lexer_skip_blanks(ist_lexer* this);

void ist_lexer_parse_identifier(ist_lexer* this);
void ist_lexer_parse_number(ist_lexer* this);
void ist_lexer_parse_string(ist_lexer* this);

#endif