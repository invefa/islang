#ifndef ISC_LEXER_H
#define ISC_LEXER_H

#include "isl_codepage.h"
#include "isl_token.h"

// TODO: add an error signal.
typedef struct ist_lexer {

    ist_module*   module;          /*  the module which this lexer belongs to               */
    ist_codepage* codepage;        /*  the codepage currently being read                    */
    ist_token                      /*  the lexer will provide a table of token              */
        pre_token,                 /*  previous token, the token before the current token   */
        cur_token,                 /*  current token, the current token                     */
        nex_token,                 /*  next token, the token after the current token        */
        sec_token;                 /*  second token, the token after the next token         */

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

void ist_lexer_lex(ist_lexer* this);
void ist_lexer_advance(ist_lexer* this);

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