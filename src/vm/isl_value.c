#include "isl_value.h"

#include "isl_string.h"
#include "isl_token.h"

ist_string ist_value_dump_json(ist_value* this, ist_u8 type, ist_string* buffer) {

    switch (type) {
        case ISL_TOKENT_VL_INT:
            return ist_strbuf_sprintf(buffer, NULL, "%lld", this->int_value);
        case ISL_TOKENT_VL_REAL:
            return ist_strbuf_sprintf(buffer, NULL, "%g", this->real_value);
        case ISL_TOKENT_VL_STRING:
            return ist_strbuf_sprintf(buffer, NULL, "\"%s\"", this->string_value);
        default:
            return ist_strbuf_sprintf(buffer, NULL, "null");
    }
}