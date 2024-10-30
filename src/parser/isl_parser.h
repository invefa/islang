#ifndef ISC_PARSER_H
#define ISC_PARSER_H

#include "isl_astnode.h"
#include "isl_lexer.h"

typedef enum ist_pstate {
    PRS_SUCCESS     = 0,
    PRS_FUNREPROTED = 1,
    PRS_FREPROTED   = 2,
    PRS_FAHEAD      = 3,
} ist_pstate;

typedef struct isl_parser {
    ist_lexer    lexer;
    ist_astnode* root;

    /* parse time results for parsing-fns */
    void*      pnode;
    ist_pstate pstate: 2;

} ist_parser;

ist_parser  ist_parser_consby_full(ist_lexer _lexer);
ist_parser  ist_parser_consby_module(ist_module* _module);
ist_parser* ist_parser_initby_full(ist_parser* this, ist_lexer _lexer);
ist_parser* ist_parser_initby_module(ist_parser* this, ist_module* _module);
ist_parser* ist_parser_createby_full(ist_lexer _lexer);
ist_parser* ist_parser_createby_module(ist_module* _module);

void ist_parser_clean(ist_parser* this);
void ist_parser_delete(ist_parser* this);

void ist_parser_parse(ist_parser* this);

#endif // ISC_PARSER_H