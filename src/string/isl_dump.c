#include "inttypes.h"
#include "isg_list.h"
#include "isl_dump.h"
#include "isl_report.h"


#define val(_valp, _type) (*(_type*)(_valp))

ist_string ist_u8_dump(
    void*         valp,
    ist_strbuf    buffer,
    ist_usize*    idxptr,
    ist_usize     depth,
    ist_dumpstyle style
) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, PRIu8, val(valp, ist_u8));
}
ist_string ist_u16_dump(
    void*         valp,
    ist_strbuf    buffer,
    ist_usize*    idxptr,
    ist_usize     depth,
    ist_dumpstyle style
) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, PRIu16, val(valp, ist_u16));
}
ist_string ist_u32_dump(
    void*         valp,
    ist_strbuf    buffer,
    ist_usize*    idxptr,
    ist_usize     depth,
    ist_dumpstyle style
) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, PRIu32, val(valp, ist_u32));
}
ist_string ist_u64_dump(
    void*         valp,
    ist_strbuf    buffer,
    ist_usize*    idxptr,
    ist_usize     depth,
    ist_dumpstyle style
) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, PRIu64, val(valp, ist_u64));
}

ist_string ist_i8_dump(
    void*         valp,
    ist_strbuf    buffer,
    ist_usize*    idxptr,
    ist_usize     depth,
    ist_dumpstyle style
) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, PRIi8, val(valp, ist_i8));
}
ist_string ist_i16_dump(
    void*         valp,
    ist_strbuf    buffer,
    ist_usize*    idxptr,
    ist_usize     depth,
    ist_dumpstyle style
) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, PRIi16, val(valp, ist_i16));
}
ist_string ist_i32_dump(
    void*         valp,
    ist_strbuf    buffer,
    ist_usize*    idxptr,
    ist_usize     depth,
    ist_dumpstyle style
) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, PRIi32, val(valp, ist_i32));
}
ist_string ist_i64_dump(
    void*         valp,
    ist_strbuf    buffer,
    ist_usize*    idxptr,
    ist_usize     depth,
    ist_dumpstyle style
) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, PRIi64, val(valp, ist_i64));
}

ist_string ist_f32_dump(
    void*         valp,
    ist_strbuf    buffer,
    ist_usize*    idxptr,
    ist_usize     depth,
    ist_dumpstyle style
) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, "%f", val(valp, ist_f32));
}
ist_string ist_f64_dump(
    void*         valp,
    ist_strbuf    buffer,
    ist_usize*    idxptr,
    ist_usize     depth,
    ist_dumpstyle style
) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, "%llf", val(valp, ist_f64));
}

ist_string isl_dump_tabs(ist_strbuf buffer, ist_usize* idxptr, ist_usize count) {
    for (ist_usize i = 0; i < count; ++i) ist_strbuf_append_raw(buffer, idxptr, "    ");
    return *buffer;
}

ist_string ist_cstring_dump(
    ist_cstring* this,
    ist_strbuf    buffer,
    ist_usize*    idxptr,
    ist_usize     depth,
    ist_dumpstyle style
) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, "\"%s\"", *this);
}

ist_string ist_dumpimage_dump(
    ist_dumpimage* this,
    ist_strbuf    buffer,
    ist_usize*    idxptr,
    ist_usize     depth,
    ist_dumpstyle style
) {
    idxptr = idxptr ?: (ist_usize[1]){};

    ist_bool dofmt = depth != -1;

    switch (style) {
        case DKIND_JSON:
            ist_strbuf_append_raw(buffer, idxptr, dofmt ? "{\n" : "{");
            break;
        case DKIND_YAML:
            if (this->name) ist_strbuf_sprintf(buffer, idxptr, "%s:\n", this->name);
            break;
        default:
            isp_unreachable();
    }


    for (ist_usize i = 0; i < this->count; ++i) {
        if (i) switch (style)
            {
                case DKIND_JSON:
                    ist_strbuf_append_raw(buffer, idxptr, dofmt ? ",\n" : ", ");
                    break;
                case DKIND_YAML:
                    ist_strbuf_append_raw(buffer, idxptr, "\n");
                    break;
                default:
                    isp_unreachable();
            }


        ist_cstring name   = this->items[i].name;
        ist_dumper  dumper = this->items[i].dumper;
        ist_vptr    valp   = this->items[i].valp;

#define dumps(_vargs...) ist_strbuf_sprintf(buffer, idxptr, ##_vargs)


        switch (style) {
            case DKIND_JSON:
                if (dofmt) isl_dump_tabs(buffer, idxptr, depth + 1);
                dumps("\"%s\": ", name);
                break;
            case DKIND_YAML:
                if (i || this->name) isl_dump_tabs(buffer, idxptr, depth + 1);
                dumps("%s: ", name);
                break;
            default:
                isp_unreachable();
        }

        dumper(valp, buffer, idxptr, depth + 1, style);
    }

#undef dumps

    switch (style) {
        case DKIND_JSON:
            if (dofmt) {
                ist_strbuf_append_raw(buffer, idxptr, "\n");
                isl_dump_tabs(buffer, idxptr, depth);
            }
            ist_strbuf_append_raw(buffer, idxptr, "}");
            break;
        case DKIND_YAML:
            break;
        default:
            isp_unreachable();
    }

    return *buffer;
}


ist_string isg_list_dumpack_dump(
    isg_list_dumpack* this,
    ist_strbuf    buffer,
    ist_usize*    idxptr,
    ist_usize     depth,
    ist_dumpstyle style
) {
    idxptr = idxptr ?: (ist_usize[1]){};

    isg_list*  list   = this->listp;
    ist_usize  llen   = isl_list_catch_length(list->data);
    ist_usize  elen   = llen / this->capacity;
    ist_bool   dofmt  = depth != -1;
    ist_dumper dumper = this->dumper;

    if (llen % this->capacity) isp_dunreachable();
    switch (style) {
        case DKIND_JSON:
            ist_strbuf_append_raw(buffer, idxptr, dofmt ? "[\n" : "[");
            break;
        case DKIND_YAML:
            break;
        default:
            isp_unreachable();
    }

    for (ist_usize i = 0; i < list->size; ++i) {
        switch (style) {
            case DKIND_JSON:
                if (i) ist_strbuf_append_raw(buffer, idxptr, dofmt ? ",\n" : ", ");
                if (dofmt) isl_dump_tabs(buffer, idxptr, depth + 1);
                dumper(list->data + elen * i, buffer, idxptr, depth + 1, style);
                break;
            case DKIND_YAML:
                ist_strbuf_append_raw(buffer, idxptr, "\n");
                isl_dump_tabs(buffer, idxptr, depth + 1);
                ist_strbuf_append_raw(buffer, idxptr, "- ");
                dumper(list->data + elen * i, buffer, idxptr, depth + 1, style);
                break;
            default:
                isp_unreachable();
        }
    }

    switch (style) {
        case DKIND_JSON:
            if (dofmt) {
                ist_strbuf_append_raw(buffer, idxptr, "\n");
                isl_dump_tabs(buffer, idxptr, depth);
            }
            ist_strbuf_append_raw(buffer, idxptr, "]");
            break;
        case DKIND_YAML:
            break;
        default:
            isp_unreachable();
    }
    return *buffer;
}
