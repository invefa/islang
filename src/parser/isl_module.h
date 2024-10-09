#ifndef ISL_MODULE_H
#define ISL_MODULE_H

/*
    A module is an extract of source code.
    It will store various data structures expressed by the source code.
*/

#include "isl_string.h"

ist_string isl_filename_catchby_filepath(ist_string _filepath);

typedef struct ist_module {
    ist_string  name;
    ist_string* symbol_list;
    ist_usize   symbol_count;
} ist_module;

void        ist_module_initby_name(ist_module* this, ist_string _name);
ist_module  ist_module_consby_name(ist_string _name);
ist_module* ist_module_createby_name(ist_string _name);

void        ist_module_initby_filepath(ist_module* this, ist_string _filepath);
ist_module  ist_module_consby_filepath(ist_string _filepath);
ist_module* ist_module_createby_filepath(ist_string _filepath);

ist_usize   ist_module_register_symbol(ist_module* this, ist_string _symbol);

void        ist_module_clean(ist_module* this);
void        ist_module_delete(ist_module* this);

#endif