#ifndef ISL_TOKEN_H
#define ISL_TOKEN_H

#include "isl_types.h"
#include "isl_string.h"

typedef enum ist_token_type {
#   define manifest(_name, _reflect) ISL_TOKEN_##_name,
#   include "isl_tokens.h"
#   undef manifest
} ist_token_type;

extern const ist_string ist_token_reflects[];

typedef struct ist_location {
    ist_string module;

    ist_usize line;
    ist_usize column;
} ist_location;


typedef struct ist_token {
    ist_token_type type;

    ist_location location;

    ist_string extract;
    ist_usize extract_length;

} ist_token;


#endif