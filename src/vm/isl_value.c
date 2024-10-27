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
    isl_ifnreport(this, rid_catch_nullptr, isp_catch_coreloc);

    switch (type) {
        case ISL_TOKENT_VL_INT:
            return ist_strbuf_sprintf(buffer, idxptr, "%" PRId64, this->int_value);
        case ISL_TOKENT_VL_REAL:
            return ist_strbuf_sprintf(buffer, idxptr, "%g", this->real_value);
        case ISL_TOKENT_VL_STRING:
            return ist_strbuf_sprintf(buffer, idxptr, "\"%s\"", this->string_value);
        default:
            return ist_strbuf_sprintf(buffer, idxptr, "null");
    }
}

#define ISG_VALUE_DONT_CLEAN
#define ISG_VALUE_TYPE ist_value
#include "isg_list_code.h"
