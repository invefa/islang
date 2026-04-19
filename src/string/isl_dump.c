#include "inttypes.h"
#include "isg_list.h"
#include "isl_dump.h"
#include "isl_report.h"


#define val(_valp, _type) (*(_type*)(_valp))
#define asdumper(_adr)    ((ist_dumper)(_adr))

#define _ist_x_dump_impl(_x)                                                                      \
    ist_string ist_##_x##_dump(                                                                   \
        ist_vptr valp, ist_strbuf buffer, ist_usize* idxptr, ist_usize depth, ist_dumpstyle style \
    ) {                                                                                           \
        idxptr = idxptr ?: (ist_usize[1]){};                                                      \
        return ist_strbuf_sprintf(buffer, idxptr, PRI##_x, val(valp, ist_##_x));                  \
    }

_ist_x_dump_impl(u8);
_ist_x_dump_impl(u16);
_ist_x_dump_impl(u32);
_ist_x_dump_impl(u64);
_ist_x_dump_impl(i8);
_ist_x_dump_impl(i16);
_ist_x_dump_impl(i32);
_ist_x_dump_impl(i64);

ist_string ist_f32_dump(
    ist_vptr      valp,
    ist_strbuf    buffer,
    ist_usize*    idxptr,
    ist_usize     depth,
    ist_dumpstyle style
) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, "%f", val(valp, ist_f32));
}
ist_string ist_f64_dump(
    ist_vptr      valp,
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

ist_string ist_cstring_dump_ident(
    ist_cstring* this,
    ist_strbuf    buffer,
    ist_usize*    idxptr,
    ist_usize     depth,
    ist_dumpstyle style
) {
    idxptr = idxptr ?: (ist_usize[1]){};

    style &= DKIND_MASK;

    switch (style) {
        case DKIND_JSON:
            return ist_strbuf_sprintf(buffer, idxptr, "\"%s\"", *this);
            break;
        case DKIND_INDENT:
            return ist_strbuf_sprintf(buffer, idxptr, "%s", *this);
            break;
        case DKIND_STRUCT:
            return ist_strbuf_sprintf(buffer, idxptr, "%s", *this);
            break;
        default:
            isp_unreachable();
    }
    return *buffer;
}

#define dumps(_vargs...) ist_strbuf_sprintf(buffer, idxptr, ##_vargs)
ist_string ist_dumpimage_dump(
    ist_dumpimage* this,
    ist_strbuf    buffer,
    ist_usize*    idxptr,
    ist_usize     depth,
    ist_dumpstyle style
) {
    idxptr = idxptr ?: (ist_usize[1]){};

    ist_dumpstyle dkind   = style & DKIND_MASK;
    ist_dumpstyle dflag   = style & DFLAG_MASK;
    ist_bool      dobreak = depth != -1;

    if (!(dflag & DFLAG_SPREAD)) style &= DKIND_MASK;
    if (dflag & DFLAG_HEAD_BREAK) ist_strbuf_append_raw(buffer, idxptr, "\n");
    if (dflag & DFLAG_HEAD_INDENT) isl_dump_tabs(buffer, idxptr, depth);

    switch (dkind) {
        case DKIND_JSON: {

            ist_strbuf_append_raw(buffer, idxptr, dobreak ? "{\n" : "{");

            for (ist_usize i = 0; i < this->count; ++i) {
                ist_dumpitem item = this->items[i];

                if (i) ist_strbuf_append_raw(buffer, idxptr, dobreak ? ",\n" : ", ");
                if (dobreak) isl_dump_tabs(buffer, idxptr, depth + 1);

                dumps("\"%s\": ", item.name);
                asdumper(item.dumper)(item.valp, buffer, idxptr, dobreak ? depth + 1 : -1, style);
            }
            if (dobreak) {
                ist_strbuf_append_raw(buffer, idxptr, "\n");
                isl_dump_tabs(buffer, idxptr, depth);
            }
            ist_strbuf_append_raw(buffer, idxptr, "}");
            break;
        }


        case DKIND_INDENT: {
            if (!dobreak) depth = 0;
            ist_bool dump_name = this->name && !(dflag & DFLAG_HEAD_NONAME);
            if (dump_name) {
                dumps("%s:\n", this->name);
                ++depth;
            }
            for (ist_usize i = 0; i < this->count; ++i) {
                ist_dumpitem item = this->items[i];

                if (i) ist_strbuf_append_raw(buffer, idxptr, "\n");
                if (i || dump_name) isl_dump_tabs(buffer, idxptr, depth);
                if (i && dflag & DFLAG_BODY_AFT2SPACE) ist_strbuf_append_raw(buffer, idxptr, "  ");

                dumps("%s: ", item.name);
                asdumper(item.dumper)(item.valp, buffer, idxptr, depth + 1, style);
            }
            break;
        }


        case DKIND_STRUCT: {
            if (this->name && !(dflag & DFLAG_HEAD_NONAME)) dumps("%s ", this->name);
            ist_strbuf_append_raw(buffer, idxptr, dobreak ? "{\n" : "{");
            for (ist_usize i = 0; i < this->count; ++i) {
                ist_dumpitem item = this->items[i];

                if (i) ist_strbuf_append_raw(buffer, idxptr, dobreak ? ",\n" : ", ");
                if (dobreak) isl_dump_tabs(buffer, idxptr, depth + 1);

                dumps("%s = ", item.name);
                asdumper(item.dumper)(item.valp, buffer, idxptr, dobreak ? depth + 1 : -1, style);
            }

            if (dobreak) {
                ist_strbuf_append_raw(buffer, idxptr, "\n");
                isl_dump_tabs(buffer, idxptr, depth);
            }
            ist_strbuf_append_raw(buffer, idxptr, "}");

            break;
        }
        default:
            isp_unreachable();
    }

    return *buffer;
}
#undef dumps


ist_string isg_list_dumpack_dump(
    isg_list_dumpack* this,
    ist_strbuf    buffer,
    ist_usize*    idxptr,
    ist_usize     depth,
    ist_dumpstyle style
) {
    idxptr = idxptr ?: (ist_usize[1]){};

    ist_dumpstyle dkind   = style & DKIND_MASK;
    ist_dumpstyle dflag   = style & DFLAG_MASK;
    ist_bool      dobreak = depth != -1;

    if (!(dflag & DFLAG_SPREAD)) style &= DKIND_MASK;
    if (dflag & DFLAG_HEAD_BREAK) ist_strbuf_append_raw(buffer, idxptr, "\n");
    if (dflag & DFLAG_HEAD_INDENT) isl_dump_tabs(buffer, idxptr, depth);
    if (this->header || this->withidx) style |= DFLAG_HEAD_NONAME;

    isg_list*  list   = this->listp;
    ist_usize  llen   = isl_list_catch_length(list->data);
    ist_usize  elen   = llen / this->capacity;
    ist_dumper dumper = this->dumper;

    if (llen % this->capacity) isp_dunreachable();

    switch (dkind) {
        case DKIND_STRUCT:
            ist_strbuf_append_raw(buffer, idxptr, dobreak ? "[\n" : "[");
            for (ist_usize i = 0; i < list->size; ++i) {
                if (i) ist_strbuf_append_raw(buffer, idxptr, dobreak ? ",\n" : ", ");
                if (dobreak) isl_dump_tabs(buffer, idxptr, depth + 1);
                if (this->header) {
                    if (this->withidx) ist_strbuf_sprintf(buffer, idxptr, this->header, i);
                    else ist_strbuf_append_raw(buffer, idxptr, this->header);
                    ist_strbuf_append_raw(buffer, idxptr, " = ");
                } else if (this->withidx)
                    ist_strbuf_sprintf(buffer, idxptr, "[%" PRIuPTR "] = ", i);
                dumper(list->data + elen * i, buffer, idxptr, dobreak ? depth + 1 : -1, style);
            }
            if (dobreak) {
                ist_strbuf_append_raw(buffer, idxptr, "\n");
                isl_dump_tabs(buffer, idxptr, depth);
            }
            ist_strbuf_append_raw(buffer, idxptr, "]");
            break;


        case DKIND_JSON:
            ist_strbuf_append_raw(buffer, idxptr, dobreak ? "[\n" : "[");
            for (ist_usize i = 0; i < list->size; ++i) {
                if (i) ist_strbuf_append_raw(buffer, idxptr, dobreak ? ",\n" : ", ");
                if (dobreak) isl_dump_tabs(buffer, idxptr, depth + 1);
                dumper(list->data + elen * i, buffer, idxptr, dobreak ? depth + 1 : -1, style);
            }
            if (dobreak) {
                ist_strbuf_append_raw(buffer, idxptr, "\n");
                isl_dump_tabs(buffer, idxptr, depth);
            }
            ist_strbuf_append_raw(buffer, idxptr, "]");
            break;


        case DKIND_INDENT:
            if (!dobreak) depth = 0;
            for (ist_usize i = 0; i < list->size; ++i) {
                ist_strbuf_append_raw(buffer, idxptr, "\n");
                isl_dump_tabs(buffer, idxptr, depth);
                ist_strbuf_append_raw(buffer, idxptr, "- ");
                if (this->header) {
                    if (this->withidx) ist_strbuf_sprintf(buffer, idxptr, this->header, i);
                    else ist_strbuf_append_raw(buffer, idxptr, this->header);
                    ist_strbuf_append_raw(buffer, idxptr, ":\n");
                    isl_dump_tabs(buffer, idxptr, depth + 1);
                    dumper(list->data + elen * i, buffer, idxptr, depth + 1, style);
                } else if (this->withidx) {
                    ist_strbuf_sprintf(buffer, idxptr, "[%" PRIuPTR "]:\n", i);
                    isl_dump_tabs(buffer, idxptr, depth + 1);
                    dumper(list->data + elen * i, buffer, idxptr, depth + 1, style);
                } else
                    dumper(
                        list->data + elen * i, buffer, idxptr, depth, style | DFLAG_BODY_AFT2SPACE
                    );
            }
            break;
        default:
            isp_unreachable();
    }

    return *buffer;
}
