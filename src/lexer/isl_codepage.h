#ifndef ISC_CODEPAGE_H
#define ISC_CODEPAGE_H

#include "isl_location.h"
#include "isl_module.h"
#include "isl_string.h"
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



#endif // ISC_CODEPAGE_H