#ifndef ISL_LEXER_H
#define ISL_LEXER_H

#include "isl_token.h"

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
    ist_i32 current_codepoint;

    /* the current location in the source code file */
    ist_location location;

    /*
        if we need to expand macro, we will suspend the current codepage, and create a new codepage
        for macro expansion, then let the current codepage store to this variable. and when the macro
        expansion is done, the new codepage will be released after the current codepage restored.
    */
    struct ist_codepage* next_page;

    /* the previous codepage in the codepage linked-list, use this variable to restore the lookahead */
    struct ist_codepage* prev_page;

} ist_codepage;

ist_codepage* ist_codepage_createby_source(ist_string _source);
// ist_codepage* ist_codepage_createby_file(ist_string _file_path);
// ist_codepage* ist_codepage_createby_string(ist_string _string, ist_usize _length);

void ist_codepage_delete(ist_codepage* _codepage);

typedef struct ist_lexer {

    /*
        The lexer will provide a table of token.
        pre_token:  the token before the current token.
        cur_token:  the current token.
        nex_token:  the token after the current token.
        sec_token:  the token after the nex_token.
    */
    ist_token pre_token, cur_token, nex_token, sec_token;

    ist_codepage* codepage;

} ist_lexer;

ist_lexer* ist_lexer_create(ist_codepage* _codepage);
void ist_lexer_delete(ist_lexer* _lexer);

void ist_lexer_init(ist_lexer* _lexer, ist_codepage* _codepage);
void ist_lexer_clean(ist_lexer* _lexer);


#endif