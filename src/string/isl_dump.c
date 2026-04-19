#include "inttypes.h"
#include "isl_dump.h"
#include "isl_report.h"

#define val(_valp, _type) (*(_type*)(_valp))

ist_string dump_u8(void* valp, ist_strbuf buffer, ist_usize* idxptr, ist_usize depth) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, PRIu8, val(valp, ist_u8));
}
ist_string dump_u16(void* valp, ist_strbuf buffer, ist_usize* idxptr, ist_usize depth) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, PRIu16, val(valp, ist_u16));
}
ist_string dump_u32(void* valp, ist_strbuf buffer, ist_usize* idxptr, ist_usize depth) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, PRIu32, val(valp, ist_u32));
}
ist_string dump_u64(void* valp, ist_strbuf buffer, ist_usize* idxptr, ist_usize depth) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, PRIu64, val(valp, ist_u64));
}

ist_string dump_i8(void* valp, ist_strbuf buffer, ist_usize* idxptr, ist_usize depth) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, PRIi8, val(valp, ist_i8));
}
ist_string dump_i16(void* valp, ist_strbuf buffer, ist_usize* idxptr, ist_usize depth) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, PRIi16, val(valp, ist_i16));
}
ist_string dump_i32(void* valp, ist_strbuf buffer, ist_usize* idxptr, ist_usize depth) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, PRIi32, val(valp, ist_i32));
}
ist_string dump_i64(void* valp, ist_strbuf buffer, ist_usize* idxptr, ist_usize depth) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, PRIi64, val(valp, ist_i64));
}

ist_string dump_f32(void* valp, ist_strbuf buffer, ist_usize* idxptr, ist_usize depth) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, "%f", val(valp, ist_f32));
}
ist_string dump_f64(void* valp, ist_strbuf buffer, ist_usize* idxptr, ist_usize depth) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, "%llf", val(valp, ist_f64));
}

ist_string isl_dump_tabs(ist_strbuf buffer, ist_usize* idxptr, ist_usize count) {
    for (ist_usize i = 0; i < count; ++i) ist_strbuf_append_raw(buffer, idxptr, "    ");
    return *buffer;
}

ist_string ist_dumpimage_dump_json(
    ist_dumpimage* this,
    ist_strbuf buffer,
    ist_usize* idxptr,
    ist_usize  depth
) {
    idxptr = idxptr ?: (ist_usize[1]){};

    ist_bool dofmt = depth != -1;
    if (dofmt) ++depth;

    ist_strbuf_append_raw(buffer, idxptr, dofmt ? "{\n" : "{");

    for (ist_usize i = 0; i < this->count; ++i) {
        if (i) ist_strbuf_append_raw(buffer, idxptr, dofmt ? ",\n" : ", ");

        ist_cstring  name = this->items[i].name;
        ist_dumpkind kind = this->items[i].kind;
        ist_vptr     valp = this->items[i].valp;
        ist_dump_fn  fn   = this->items[i].fn;

#define dumps(_vargs...) ist_strbuf_sprintf(buffer, idxptr, ##_vargs)

        if (dofmt) isl_dump_tabs(buffer, idxptr, depth);
        switch (kind) {
            case DKIND_OBJECT:
                ist_strbuf_sprintf(buffer, idxptr, "\"%s\": ", name);
                if (fn) fn(valp, buffer, idxptr, depth);
                else ist_dumpimage_dump_json(valp, buffer, idxptr, depth);
                break;
            case DKIND_ARRAY:
                ist_strbuf_sprintf(buffer, idxptr, "\"%s\": ", name);
                if (fn) fn(valp, buffer, idxptr, depth);
                else isp_unreachable();
                break;
            case DKIND_STRING:
                if (fn) {
                    ist_strbuf tbuffer = ist_strbuf_cons(8);
                    dumps("\"%s\": \"%s\"", name, fn(valp, tbuffer, NULL, depth));
                    ist_strbuf_clean(&tbuffer);
                } else dumps("\"%s\": \"%s\"", name, val(valp, ist_string));
                break;
            case DKIND_INT:
                ist_strbuf_sprintf(buffer, idxptr, "\"%s\": ", name);
                if (fn) fn(valp, buffer, idxptr, depth);
                else dumps("\"%s\": %" PRIi64, name, val(valp, ist_i64));
                break;
            case DKIND_FLOAT:
                ist_strbuf_sprintf(buffer, idxptr, "\"%s\": ", name);
                if (fn) fn(valp, buffer, idxptr, depth);
                else dumps("\"%s\": %llf", name, val(valp, ist_f64));
                break;
            case DKIND_BOOL:
                dumps("\"%s\": %s", name, val(valp, ist_bool) ? "true" : "false");
                break;
            case DKIND_TRUE:
                dumps("\"%s\": true", name);
                break;
            case DKIND_FALSE:
                dumps("\"%s\": false", name);
                break;
            default:
                isp_unreachable();
                break;
        }
    }

#undef dumps
    if (dofmt) {
        ist_strbuf_append_raw(buffer, idxptr, "\n");
        isl_dump_tabs(buffer, idxptr, depth - 1);
    }
    ist_strbuf_append_raw(buffer, idxptr, "}");
    return *buffer;
}
