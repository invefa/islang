#ifndef ISC_MODULE_H
#define ISC_MODULE_H

/*
    A module is an extract of source code.
    It will store various data structures expressed by the source code.
*/

#include "isl_string.h"

ist_string isl_filename_catchby_filepath(ist_cstring _filepath);

/* strbuf type */
typedef ist_u8 ist_sbtype;
enum ist_sbtype {
    ISL_STRBUFT_UNKNOWN,  /* unknown */
    ISL_STRBUFT_SYMBOL,   /* symbol */
    ISL_STRBUFT_SOURCE,   /* source code */
    ISL_STRBUFT_NAME,     /* struct name */
    ISL_STRBUFT_FILEPATH, /* file path */
    ISL_STRBUFT_LITERAL,  /* string literal analyzed from source */
};

typedef struct {
    ist_string buffer;
    ist_sbtype type;
} ist_strbuf_entry;


#define ISG_VALUE_TYPE ist_strbuf_entry
#include "isg_list_head.h"

typedef struct ist_module {

    /*
        The name of this module, used to report message or import this module.
        It will be generated by source code file name:
        module.name = isl_filename_catchby_filepath(filepath);
        We will extract the filename from the filepath and set it as the name of this module.
    */
    ist_string name;
    ist_string filepath;

    /*
        The list of string buffers for this module, it will store all string buffers.
        Including the name of all c-structures which serving this module (including the name of it
       self), as well as the source code read from the source code file or generated by macro, and
       the generated string from source code (such as string literal or identifier), etc. This list
       was designed for memory management, to ensure that the memory will be released when the
       compilation is finished.
    */
    ist_strbuf_entry_list strbuf_entry_list;

} ist_module;

ist_module  ist_module_consby_full(ist_string _name, ist_string _filepath);
ist_module* ist_module_initby_full(ist_module* this, ist_string _name, ist_string _filepath);
ist_module* ist_module_createby_full(ist_string _name, ist_string _filepath);

ist_module  ist_module_consby_filepath(ist_string _filepath);
ist_module* ist_module_initby_filepath(ist_module* this, ist_string _filepath);
ist_module* ist_module_createby_filepath(ist_string _filepath);

void ist_module_clean(ist_module* this);
void ist_module_delete(ist_module* this);

ist_usize ist_module_register_strbuf(ist_module* this, ist_string _strbuf, ist_sbtype _type);

ist_string ist_module_dump_json(ist_module* this, ist_string* buffer, ist_usize* idxptr);

/**
 * Define the list of modules.
 * It will store all modules in the compilation.
 */
#define ISG_VALUE_TYPE ist_module
#include "isg_list_head.h"

#endif // ISC_MODULE_H