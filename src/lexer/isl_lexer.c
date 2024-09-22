#include "isl_lexer.h"

#include "isl_memgr.h"
#include "isl_utf8.h"

ist_codepage* ist_codepage_createby_source(ist_string _source) {

    ist_codepage* codepage = isl_malloc(ist_codepage);

    codepage->source = _source;
    codepage->next_sequence_index = 0;

    /* decode the first utf8 sequence */
    codepage->current_codepoint = isl_utf8_decode(&codepage->source, &codepage->next_sequence_index);

    /* initialize the location, module name is <buildin-source> */
    ist_location_init(&codepage->location, "<buildin-source>");

    codepage->prev_page = codepage->next_page = NULL;

    return codepage;

}

void ist_lexer_init(ist_lexer* lexer, ist_codepage* codepage) {

    /* initialize the codepage */
    lexer->codepage = codepage;
}
