#ifndef ISL_TOKEN_H
#define ISL_TOKEN_H

#include "isl_types.h"
#include "isl_string.h"
#include "isl_value.h"

typedef enum ist_token_type {
#   define manifest(_name, _reflect) ISL_TOKENT_##_name,
#   include "isl_tokens.h"
#   undef manifest
} ist_token_type;

extern const ist_string ist_token_reflects[];
extern const ist_string ist_token_names[];

//pre-declare for location
struct ist_codepage;

typedef struct ist_location {
    struct ist_codepage* codepage;
    ist_usize            line;
    ist_usize            column;
} ist_location;

#define ist_location_consby_codepage(_codepage) \
((ist_location){                \
    .codepage = (_codepage),    \
    .line     = 1,              \
    .column   = 1               \
})

// void ist_location_initby_full(ist_location* this, struct ist_codepage* _codepage);
// void ist_location_clean(ist_location* this);

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

#define ist_token_cons_empty()      \
((ist_token){                       \
    .value.int_value = 0,           \
    .type     = ISL_TOKENT_UNKNOWN, \
    .extract  = NULL,               \
    .length   = 0,                  \
    .location = {                   \
        .codepage = NULL,           \
        .line     = 1,              \
        .column   = 1               \
    }                               \
})

#define ist_token_consby_full(_type, _location, _extract, _length, _value) \
((ist_token){                 \
    .type     = (_type),      \
    .extract  = (_extract),   \
    .length   = (_length),    \
    .location = (_location),  \
    .value    = (_value)      \
})

// void ist_token_init_empty(ist_token* this);
// void ist_token_initby_location(ist_token* this, ist_location _location);

// void ist_token_initby_full(
//     ist_token* this,
//     ist_token_type  _type,
//     ist_location    _location,
//     ist_string      _extract,
//     ist_usize       _length,
//     ist_value       _value);

ist_string* ist_token_dump(ist_token* this, ist_string* _buffer);

/*
    if the string is a keyword, return the keyword type.
    otherwise, return ISL_TOKENT_ID.
*/
ist_token_type ist_string_is_keyword(ist_string this, ist_usize _length);

#endif