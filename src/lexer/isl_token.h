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

typedef struct ist_location {
    ist_string  module;
    ist_usize   line;
    ist_usize   column;
} ist_location;

void ist_location_init(ist_location* this, ist_string _module);
void ist_location_clean(ist_location* this);

/* length is the length of extracted string */
typedef struct ist_token {
    ist_token_type  type;
    ist_location    location;
    ist_string      extract;
    ist_usize       length;
    ist_value       value;
} ist_token;

void ist_token_inito_null(ist_token* this);
void ist_token_initby_location(ist_token* this, ist_location _location);

void ist_token_initby_full(
    ist_token* this,
    ist_token_type  _type,
    ist_location    _location,
    ist_string      _extract,
    ist_usize       _length,
    ist_value       _value);

// void ist_token_print(ist_token* this);
ist_string* ist_token_dump(ist_token* this, ist_string* _buffer);

/*
    if the string is a keyword, return the keyword type.
    otherwise, return ISL_TOKENT_ID.
*/
ist_token_type ist_string_is_keyword(ist_string this, ist_usize _length);

#endif