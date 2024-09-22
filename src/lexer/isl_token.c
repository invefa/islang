#include "isl_token.h"

const ist_string ist_token_reflects[] = {
#   define manifest(_name, _reflect) _reflect,
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
    this->extract_length = 0;
    this->value.int_value = 0;

}

void ist_token_init_with_location(ist_token* this, ist_location _location) {
    this->type = ISL_TOKENT_EOF;
    this->location = _location;
    this->extract = NULL;
    this->extract_length = 0;
    this->value.int_value = 0;
}

inline void ist_token_init_full(
    ist_token* this,
    ist_token_type  _type,
    ist_location    _location,
    ist_string      _extract,
    ist_usize       _extract_length,
    ist_value       _value)
{
    this->type = _type;
    this->location = _location;
    this->extract = _extract;
    this->extract_length = _extract_length;
    this->value = _value;
}
