#include "isl_value.h"

#include <inttypes.h>

#include "isl_string.h"
#include "isl_token.h"

ist_string ist_value_dump_json(
    ist_value* this,
    ist_u32     type,
    ist_string* buffer,
    ist_usize*  idxptr
) {

    switch (type) {
        case ISL_TOKENT_VL_INT:
            return ist_strbuf_sprintf(buffer, NULL, "%" PRId64, this->int_value);
        case ISL_TOKENT_VL_REAL:
            return ist_strbuf_sprintf(buffer, NULL, "%g", this->real_value);
        case ISL_TOKENT_VL_STRING:
            return ist_strbuf_sprintf(buffer, NULL, "\"%s\"", this->string_value);
        default:
            return ist_strbuf_sprintf(buffer, NULL, "null");
    }
}

#define ISG_VALUE_DONT_CLEAN
#define ISG_VALUE_TYPE ist_value
#include "isg_list_code.h"
