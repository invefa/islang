#include "isl_token.h"

const ist_string ist_token_reflects[] = {
#   define manifest(_name, _reflect) _reflect,
#   include "isl_tokens.h"
#   undef manifest
};

void ist_location_init(ist_location* _location, ist_string _module) {
    _location->module = _module;
    _location->line = 1;
    _location->column = 1;
}

void ist_token_init_null(ist_token* _token) {
    _token->type = ISL_TOKENT_EOF;

    ist_location_init(&_token->location, NULL);

    _token->extract = NULL;
    _token->extract_length = 0;

}
