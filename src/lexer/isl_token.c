#include "isl_token.h"

#include <inttypes.h>
#include <stdio.h>


#include "isl_lexer.h"
#include "isl_list.h"
#include "isl_memgr.h"
#include "isl_report.h"


const ist_cstring ist_token_reflects[] = {
#define manifest(_name, _reflect) [ISL_TOKENT_##_name] = _reflect,
#include "isl_tokens.h"
#undef manifest
};

const ist_cstring ist_token_names[] = {
#define manifest(_name, _reflect) [ISL_TOKENT_##_name] = #_name,
#include "isl_tokens.h"
#undef manifest
};

ist_string ist_location_dump_json(ist_location* this, ist_string* buffer) {

    return ist_strbuf_sprintf(
        buffer,
        NULL,
        "\"module\":\"%s\",\"pagename\":\"%s\",\"line\":%zu,\"column\":%zu",
        this->module ? this->module->name : (ist_string) "",
        this->pagename ?: (ist_string) "",
        this->line,
        this->column
    );
}


inline ist_string ist_token_dump(ist_token* this, ist_string* buffer) {

    return ist_strbuf_sprintf(
        buffer,
        NULL,
        "token<0x%zX> {module=<%s:%s>,location=<%zu:%zu>,type=%s,"
        "extract=\"%.*s\",length=%zu,value={int=%" PRId64 ",real=%g}}",
        (ist_usize)this,
        this->location.module->name,
        this->location.pagename ?: (ist_string) "\b",
        this->location.line,
        this->location.column,
        ist_token_names[this->type],
        this->length,
        this->extract,
        this->length,
        this->value.int_value,
        this->value.real_value
    );
}

ist_token_type ist_string_is_keyword(ist_cstring this, ist_usize length) {
    for (ist_usize i = ISL_TOKENT_START_KEYWORDS + 1; i < ISL_TOKENT_END_KEYWORDS; ++i)
        if (!ist_token_reflects[i]) continue;
        else if (!strncmp(this, ist_token_reflects[i], length)) return i;
    return ISL_TOKENT_ID;
}
