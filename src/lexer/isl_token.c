#include "isl_token.h"

#include <stdio.h>

const ist_string ist_token_reflects[] = {
#   define manifest(_name, _reflect) _reflect,
#   include "isl_tokens.h"
#   undef manifest
};

const ist_string ist_token_names[] = {
#   define manifest(_name, _reflect) #_name,
#   include "isl_tokens.h"
#   undef manifest
};


void ist_location_init(ist_location* this, ist_string _module) {
    this->module = _module;
    this->line = 1;
    this->column = 1;
}

void ist_token_init_null(ist_token* this) {
    this->type = ISL_TOKENT_EOF;

    ist_location_init(&this->location, NULL);

    this->extract = NULL;
    this->length = 0;
    this->value.int_value = 0;

}

void ist_token_init_with_location(ist_token* this, ist_location _location) {
    this->type = ISL_TOKENT_EOF;
    this->location = _location;
    this->extract = NULL;
    this->length = 0;
    this->value.int_value = 0;
}

inline void ist_token_init_full(
    ist_token* this,
    ist_token_type  _type,
    ist_location    _location,
    ist_string      _extract,
    ist_usize       _length,
    ist_value       _value)
{
    this->type = _type;
    this->location = _location;
    this->extract = _extract;
    this->length = _length;
    this->value = _value;
}

void ist_token_print(ist_token* this) {
    ist_string extract;
    ist_string_init(&extract, this->extract, this->length);
    printf("token<0x%p>:\n", this);
    printf("module:   <%s>\n", this->location.module);
    printf("location: <%llu:%llu>\n", this->location.line, this->location.column);
    printf("type:     %s\n", ist_token_names[this->type]);
    printf("extract:  \"%s\"\n", extract);
    printf("length:   %llu\n", this->length);
    if (this->type == ISL_TOKENT_INT)
        printf("value:    %lld\n", this->value.int_value);
    if (this->type == ISL_TOKENT_REAL)
        printf("value:    %llf\n", this->value.int_value);
    ist_string_clean(&extract);
}

