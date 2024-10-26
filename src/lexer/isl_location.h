#ifndef ISC_LOCATION_H
#define ISC_LOCATION_H

#include "isl_module.h"
#include "isl_string.h"

typedef struct ist_location {
    ist_module* module;
    ist_string  pagename;
    ist_usize   line;
    ist_usize   column;
} ist_location;

ist_string ist_location_dump_json(ist_location* this, ist_string* buffer, ist_usize* idxptr);

#define ist_location_consby_full(_module, _pagename) \
    ((ist_location){.module = (_module), .pagename = (_pagename), .line = 1, .column = 1})

#define ist_location_consby_null() ist_location_consby_full(NULL, NULL)

#define ist_location_consby_pagename(_pagename) ist_location_consby_full(NULL, _pagename)



#endif // ISC_LOCATION_H