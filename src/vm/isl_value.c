#include "isl_value.h"

#include <inttypes.h>

#include "isl_string.h"
#include "isl_token.h"

ist_string ist_value_dump_old(
    ist_value* this,
    ist_u32     type,
    ist_string* buffer,
    ist_usize*  idxptr
) {
    isl_ifnreport(this, rid_catch_nullptr, isp_catch_coreloc);

    switch (type) {
        case ISL_TOKENT_VL_INT:
            return ist_strbuf_sprintf(buffer, idxptr, "%" PRId64, this->as_i64);
        case ISL_TOKENT_VL_REAL:
            return ist_strbuf_sprintf(buffer, idxptr, "%g", this->as_f64);
        case ISL_TOKENT_VL_STRING:
            return ist_strbuf_sprintf(buffer, idxptr, "\"%s\"", this->as_str);
        default:
            return ist_strbuf_sprintf(buffer, idxptr, "null");
    }
}

ist_string ist_tvalue_dump(
    ist_tvalue* this,
    ist_string*   buffer,
    ist_usize*    idxptr,
    ist_usize     depth,
    ist_dumpstyle style
) {
    isl_ifnreport(this, rid_catch_nullptr, isp_catch_coreloc);

    switch (this->type) {
        case isl_valtype_i64:
            return ist_strbuf_sprintf(buffer, idxptr, "%" PRId64, this->data.as_i64);
        case isl_valtype_f64:
            return ist_strbuf_sprintf(buffer, idxptr, "%g", this->data.as_f64);
        case isl_valtype_str:
            return ist_strbuf_sprintf(buffer, idxptr, "\"%s\"", this->data.as_str);
        default:
            return ist_strbuf_sprintf(buffer, idxptr, "(unknown)");
    }
}

#define ISG_VALUE_DONT_CLEAN
#define ISG_VALUE_TYPE ist_value
#include "isg_list_code.h"
#define ISG_VALUE_DONT_CLEAN
#define ISG_VALUE_TYPE ist_value
#include "isg_stack_code.h"
