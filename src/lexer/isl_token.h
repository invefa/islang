#ifndef ISC_TOKEN_H
#define ISC_TOKEN_H

#include "isl_location.h"
#include "isl_value.h"


typedef enum ist_token_type {
#define manifest(_name, _reflect) ISL_TOKENT_##_name,
#include "isl_tokens.h"
#undef manifest
} ist_token_type;

extern const ist_cstring ist_token_reflects[];
extern const ist_cstring ist_token_names[];


/*
    token is the basic unit for parser to analyze.
    the member:length is the length of extracted string
*/
typedef struct ist_token {
    ist_token_type type: 8;
    ist_location   location;
    ist_cstring    extract;
    ist_usize      length;
    ist_value      value;
} ist_token;

#define ist_token_consby_null()              \
    ((ist_token                              \
    ){.value.int_value = 0,                  \
      .type            = ISL_TOKENT_UNKNOWN, \
      .extract         = NULL,               \
      .length          = 0,                  \
      .location        = {.pagename = NULL, .line = 1, .column = 1}})
#define ist_token_consby_location(_location) \
    ((ist_token                              \
    ){.value.int_value = 0,                  \
      .type            = ISL_TOKENT_UNKNOWN, \
      .extract         = NULL,               \
      .length          = 0,                  \
      .location        = (_location)})
#define ist_token_consby_full(_type, _location, _extract, _length, _value) \
    ((ist_token                                                            \
    ){.type     = (_type),                                                 \
      .extract  = (_extract),                                              \
      .length   = (_length),                                               \
      .location = (_location),                                             \
      .value    = (_value)})


ist_string ist_token_dump(ist_token* this, ist_string* buffer, ist_usize* idxptr);
ist_string ist_token_dump_json(ist_token* this, ist_string* buffer, ist_usize* idxptr);

/*
    if the string is a keyword, return the keyword type.
    otherwise, return ISL_TOKENT_ID.
*/
ist_token_type ist_string_is_keyword(ist_cstring this, ist_usize length);

#endif // ISC_TOKEN_H