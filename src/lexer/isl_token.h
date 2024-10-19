#ifndef ISC_TOKEN_H
#define ISC_TOKEN_H

#include "isl_types.h"
#include "isl_string.h"
#include "isl_value.h"
#include "isl_module.h"

typedef ist_u8 ist_token_type;
enum ist_token_type {
#   define manifest(_name, _reflect) ISL_TOKENT_##_name,
#   include "isl_tokens.h"
#   undef manifest
};

extern const ist_string ist_token_reflects[];
extern const ist_string ist_token_names[];


typedef struct ist_location {
    ist_module* module;
    ist_string  pagename;
    ist_usize   line;
    ist_usize   column;
} ist_location;


#define ist_location_consby_full(_module, _pagename) \
((ist_location){                \
    .module   = (_module),      \
    .pagename = (_pagename),    \
    .line     = 1,              \
    .column   = 1               \
})

#define ist_location_consby_null() \
ist_location_consby_full(NULL, NULL)

#define ist_location_consby_pagename(_pagename) \
ist_location_consby_full(NULL, _pagename)


/*
    token is the basic unit for parser to analyze.
    the member:length is the length of extracted string
*/
typedef struct ist_token {
    ist_token_type  type;
    ist_location    location;
    ist_string      extract;
    ist_usize       length;
    ist_value       value;
} ist_token;

#define ist_token_consby_null()     \
((ist_token){                       \
    .value.int_value = 0,           \
    .type     = ISL_TOKENT_UNKNOWN, \
    .extract  = NULL,               \
    .length   = 0,                  \
    .location = {                   \
        .pagename = NULL,           \
        .line     = 1,              \
        .column   = 1               \
    }                               \
})
#define ist_token_consby_location(_location) \
((ist_token){                       \
    .value.int_value = 0,           \
    .type     = ISL_TOKENT_UNKNOWN, \
    .extract  = NULL,               \
    .length   = 0,                  \
    .location = (_location)         \
})
#define ist_token_consby_full(_type, _location, _extract, _length, _value) \
((ist_token){                 \
    .type     = (_type),      \
    .extract  = (_extract),   \
    .length   = (_length),    \
    .location = (_location),  \
    .value    = (_value)      \
})


ist_string* ist_token_dump(ist_token* this, ist_string* _buffer);

/*
    if the string is a keyword, return the keyword type.
    otherwise, return ISL_TOKENT_ID.
*/
ist_token_type ist_string_is_keyword(ist_string this, ist_usize _length);

#endif // ISC_TOKEN_H