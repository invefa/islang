#include "inttypes.h"
#include "isl_dump.h"
#include "isl_report.h"

#define val(_valp, _type) (*(_type*)(_valp))

ist_string dump_u8(ist_strbuf tbuffer, void* valp) {
    return ist_strbuf_sprintf(tbuffer, NULL, PRIu8, val(valp, ist_u8));
}
ist_string dump_u16(ist_strbuf tbuffer, void* valp) {
    return ist_strbuf_sprintf(tbuffer, NULL, PRIu16, val(valp, ist_u16));
}
ist_string dump_u32(ist_strbuf tbuffer, void* valp) {
    return ist_strbuf_sprintf(tbuffer, NULL, PRIu32, val(valp, ist_u32));
}
ist_string dump_u64(ist_strbuf tbuffer, void* valp) {
    return ist_strbuf_sprintf(tbuffer, NULL, PRIu64, val(valp, ist_u64));
}

ist_string dump_i8(ist_strbuf tbuffer, void* valp) {
    return ist_strbuf_sprintf(tbuffer, NULL, PRIi8, val(valp, ist_i8));
}
ist_string dump_i16(ist_strbuf tbuffer, void* valp) {
    return ist_strbuf_sprintf(tbuffer, NULL, PRIi16, val(valp, ist_i16));
}
ist_string dump_i32(ist_strbuf tbuffer, void* valp) {
    return ist_strbuf_sprintf(tbuffer, NULL, PRIi32, val(valp, ist_i32));
}
ist_string dump_i64(ist_strbuf tbuffer, void* valp) {
    return ist_strbuf_sprintf(tbuffer, NULL, PRIi64, val(valp, ist_i64));
}

ist_string dump_f32(ist_strbuf tbuffer, void* valp) {
    return ist_strbuf_sprintf(tbuffer, NULL, "%f", val(valp, ist_f32));
}
ist_string dump_f64(ist_strbuf tbuffer, void* valp) {
    return ist_strbuf_sprintf(tbuffer, NULL, "%llf", val(valp, ist_f64));
}


ist_string dump_json(
    ist_strbuf     buffer,
    ist_usize*     idxptr,
    ist_dump_item* item_list,
    ist_usize      count
) {
    idxptr = idxptr ?: (ist_usize[1]){};

    ist_strbuf tbuffer = ist_strbuf_cons(8);
    ist_strbuf_append_raw(buffer, idxptr, "{");

    for (ist_usize i = 0; i < count; ++i) {
        if (i) ist_strbuf_append_raw(buffer, idxptr, ",");

        ist_cstring   name = item_list[i].name;
        void*         valp = item_list[i].valp;
        ist_json_kind kind = item_list[i].kind;
        ist_dump_fn   fn   = item_list[i].fn;

#define dumps(_vargs...) ist_strbuf_sprintf(buffer, idxptr, ##_vargs)

        switch (kind) {
            case JKIND_OBJECT:
                if (fn) dumps("\"%s\":{%s}", name, fn(tbuffer, valp));
                else {
                    ist_dump_item_clist clist = val(valp, ist_dump_item_clist);
                    dumps("\"%s\":%s", name, dump_json(tbuffer, NULL, clist.data, clist.count));
                }
                break;
            case JKIND_ARRAY:
                if (fn) dumps("\"%s\":[%s]", name, fn(tbuffer, valp));
                else isp_unreachable();
                break;
            case JKIND_STRING:
                if (fn) dumps("\"%s\":\"%s\"", name, fn(tbuffer, valp));
                else dumps("\"%s\":\"%s\"", name, val(valp, ist_string));
                break;
            case JKIND_INT:
                if (fn) dumps("\"%s\":%s", name, fn(tbuffer, valp));
                else dumps("\"%s\":%" PRIi64, name, val(valp, ist_i64));
                break;
            case JKIND_FLOAT:
                if (fn) dumps("\"%s\":%s", name, fn(tbuffer, valp));
                else dumps("\"%s\":%llf", name, val(valp, ist_f64));
                break;
            case JKIND_BOOL:
                dumps("\"%s\":%s", name, val(valp, ist_bool) ? "true" : "false");
                break;
            case JKIND_TRUE:
                dumps("\"%s\":true", name);
                break;
            case JKIND_FALSE:
                dumps("\"%s\":false", name);
                break;
            default:
                isp_unreachable();
                break;
        }
    }

#undef dumps

    ist_strbuf_append_raw(buffer, idxptr, "}");
    ist_strbuf_clean(&tbuffer);
    return *buffer;
}
