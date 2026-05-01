#ifndef ISC_PARSER_H
#define ISC_PARSER_H

#include "isl_lexer.h"


typedef struct ist_parseYield {
    ist_parsent ok;
    enum ist_parseYieldStatus {
        isn_pasreYieldStatus_success    = 0, /* success */
        isn_pasreYieldStatus_reported   = 1, /* failure and unreported */
        isn_pasreYieldStatus_unreported = 2, /* failure and reported */
        isn_pasreYieldStatus_aheading   = 3, /* failure while aheading */
    } status;
} ist_parseYield;
#define ist_parseYield_ (ist_parseYield)

typedef struct ist_parser {
    ist_module*  module;
    ist_lexer    lexer;
    ist_astnode* root;
} ist_parser;

ist_parser ist_parser_consby_lexer(ist_lexer _lexer);
_isl_declare_initby_createby_with_consby((ist_lexer _lexer), ist_parser, lexer);

ist_parser ist_parser_consby_module(ist_module* _module);
_isl_declare_initby_createby_with_consby((ist_module * _module), ist_parser, module);


void ist_parser_clean(ist_parser* this);
void ist_parser_delete(ist_parser* this);

ist_parseYield ist_parser_parse(ist_parser* this);

#endif // ISC_PARSER_H