#include "isl_token.h"

#include <stdio.h>

#include "isl_memgr.h"
#include "isl_report.h"
#include "isl_lexer.h"

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


inline ist_string* ist_token_dump(ist_token* this, ist_string* _buffer) {
    ist_byte storager = 0;
    if (this->extract) {
        storager = this->extract[this->length];
        this->extract[this->length] = '\0';
    }
    snprintf(*_buffer, ISL_DEFAULT_BUFFER_LENGTH,
        "token<0x%zX> {module=<%s:%s>,location=<%zu:%zu>,type=%s,"
        "extract=\"%s\",length=%zu,value={int=%lld,real=%g}}",
        (ist_usize)this,
        this->location.codepage->module->name,
        this->location.codepage->name ? this->location.codepage->name : (ist_string)"\b",
        this->location.line,
        this->location.column,
        ist_token_names[this->type],
        this->extract,
        this->length,
        this->value.int_value,
        this->value.real_value);

    if (this->extract)
        this->extract[this->length] = storager;

    return _buffer;
}


ist_token_type ist_string_is_keyword(ist_string this, ist_usize _length) {
    for (ist_usize i = ISL_TOKENT_START_KEYWORDS + 1; i < ISL_TOKENT_END_KEYWORDS; ++i)
        if (!ist_token_reflects[i]) continue;
        else if (!strncmp(this, ist_token_reflects[i], _length))
            return i;
    return ISL_TOKENT_ID;
}
