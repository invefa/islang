#include "isl_token.h"

#include <inttypes.h>
#include <stdio.h>

#include "isl_list.h"
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


inline ist_string ist_token_dump(ist_token* this, ist_string* buffer, ist_usize* idxptr) {
    isl_ifnreport(this, rid_catch_nullptr, isp_catch_coreloc);
    idxptr = idxptr ?: (ist_usize[1]){};
    ist_strbuf_sprintf(
        buffer,
        idxptr,
        "token<0x%zX> {location=<%s:%s:%zu:%zu>,type=%s,extract[%zu]=\"%.*s\"",
        (ist_usize)this,
        this->location.module->name,
        this->location.pagename ?: (ist_string) "\b",
        this->location.line,
        this->location.column,
        ist_token_names[this->type],
        this->length,
        this->length,
        this->extract
    );
    switch (this->type) {
        case ISL_TOKENT_VL_INT:
            ist_strbuf_sprintf(buffer, idxptr, ",value=%" PRId64, this->value.int_value);
            break;
        case ISL_TOKENT_VL_REAL:
            ist_strbuf_sprintf(buffer, idxptr, ",value=%lf", this->value.real_value);
            break;
        case ISL_TOKENT_VL_STRING:
            ist_strbuf_sprintf(buffer, idxptr, ",value=\"%s\"", this->value.string_value);
            break;
    }
    return ist_strbuf_append_raw(buffer, idxptr, "}");
}


inline ist_string ist_token_dump_json(ist_token* this, ist_string* buffer, ist_usize* idxptr) {
    isl_ifnreport(this, rid_catch_nullptr, isp_catch_coreloc);
    idxptr = idxptr ?: (ist_usize[1]){};
    ist_strbuf_sprintf(
        buffer,
        idxptr,
        "{\"type\":\"%s\",\"location\":\"<%s:%s:%zu:%zu>\",\"extract\"=\"%.*s\"",
        ist_token_names[this->type],
        this->location.module->name,
        this->location.pagename ?: (ist_string) "\b",
        this->location.line,
        this->location.column,
        this->length,
        this->extract
    );
    switch (this->type) {
        case ISL_TOKENT_VL_INT:
            ist_strbuf_sprintf(buffer, idxptr, ",\"value\":%" PRId64, this->value.int_value);
            break;
        case ISL_TOKENT_VL_REAL:
            ist_strbuf_sprintf(buffer, idxptr, ",\"value\":%lf", this->value.real_value);
            break;
        case ISL_TOKENT_VL_STRING:
            ist_strbuf_sprintf(buffer, idxptr, ",\"value\":\"%s\"", this->value.string_value);
            break;
    }
    return ist_strbuf_append_raw(buffer, idxptr, "}");
}

ist_token_type ist_string_is_keyword(ist_cstring this, ist_usize length) {
    for (ist_usize i = ISL_TOKENT_START_KEYWORDS + 1; i < ISL_TOKENT_END_KEYWORDS; ++i)
        if (!ist_token_reflects[i] || strlen(ist_token_reflects[i]) != length) continue;
        else if (!strncmp(this, ist_token_reflects[i], length)) return i;
    return ISL_TOKENT_ID;
}
