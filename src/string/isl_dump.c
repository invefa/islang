#include "inttypes.h"
#include "isg_list.h"
#include "isl_dump.h"
#include "isl_report.h"


#define val(_valp, _type)      (*(_type*)(_valp))
#define asdumper(_adr)         ((ist_dumper)(_adr))
#define dumpf(_fmt, _vargs...) ist_strbuf_sprintf(buffer, idxptr, _fmt, ##_vargs)
#define dumpr(_raw)            ist_strbuf_append_raw(buffer, idxptr, _raw)
#define tab(_count)            isl_dump_tabs(buffer, idxptr, _count)
#define appdumper(_dumper, _valp, _depth, _style) \
    asdumper(_dumper)(_valp, buffer, idxptr, _depth, _style)

#define _ist_dumper_params \
    ist_vptr this, ist_strbuf buffer, ist_usize *idxptr, ist_usize depth, ist_dumpstyle style

#define _ist_x_dump_impl(_x)                         \
    ist_string ist_##_x##_dump(_ist_dumper_params) { \
        idxptr = idxptr ?: (ist_usize[1]){};         \
        return dumpf(PRI##_x, val(this, ist_##_x));  \
    }

_ist_x_dump_impl(u8);
_ist_x_dump_impl(u16);
_ist_x_dump_impl(u32);
_ist_x_dump_impl(u64);
_ist_x_dump_impl(i8);
_ist_x_dump_impl(i16);
_ist_x_dump_impl(i32);
_ist_x_dump_impl(i64);

ist_string ist_f32_dump(_ist_dumper_params) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return dumpf("%f", val(this, ist_f32));
}
ist_string ist_f64_dump(_ist_dumper_params) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return dumpf("%llf", val(this, ist_f64));
}
ist_string ist_cstring_dump(_ist_dumper_params) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return dumpf("\"%s\"", val(this, ist_cstring));
}
ist_string ist_cstring_dump_ident(_ist_dumper_params) {
    idxptr = idxptr ?: (ist_usize[1]){};

    switch (style &= DKIND_MASK) {
        case DKIND_JSON:
            return dumpf("\"%s\"", val(this, ist_cstring));
            break;
        case DKIND_INDENT:
            return dumpf("%s", val(this, ist_cstring));
            break;
        case DKIND_STRUCT:
            return dumpf("%s", val(this, ist_cstring));
            break;
        default:
            isp_unreachable();
    }
    return *buffer;
}


ist_string isl_dump_tabs(ist_strbuf buffer, ist_usize* idxptr, ist_usize count) {
    for (ist_usize i = 0; i < count; ++i) dumpr("    ");
    return *buffer;
}

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
    if (dflag & DFLAG_HEAD_BREAK) dumpr("\n");
    if (dflag & DFLAG_HEAD_INDENT) tab(depth);

    switch (dkind) {
        case DKIND_JSON: {
            dumpr(dobreak ? "{\n" : "{");
            for (ist_usize i = 0; i < this->count; ++i) {
                ist_dumpitem item = this->items[i];

                if (i) dumpr(dobreak ? ",\n" : ", ");
                if (dobreak) tab(depth + 1);

                dumpf("\"%s\": ", item.name);
                appdumper(item.dumper, item.valp, dobreak ? depth + 1 : -1, style);
            }

            if (dobreak) dumpr("\n"), tab(depth);
            dumpr("}");
            break;
        }


        case DKIND_INDENT: {
            if (!dobreak) depth = 0;
            ist_bool dump_name = this->name && !(dflag & DFLAG_HEAD_NONAME);

            if (dump_name) dumpf("%s:\n", this->name), ++depth;
            for (ist_usize i = 0; i < this->count; ++i) {
                ist_dumpitem item = this->items[i];

                if (i) dumpr("\n");
                if (i || dump_name) tab(depth);
                if (i && !dump_name && dflag & DFLAG_BODY_AFT2SPACE) dumpr("  ");

                dumpf("%s: ", item.name);
                appdumper(item.dumper, item.valp, depth + 1, style);
            }
            break;
        }


        case DKIND_STRUCT: {
            if (this->name && !(dflag & DFLAG_HEAD_NONAME)) dumpf("%s ", this->name);
            dumpr(dobreak ? "{\n" : "{");
            for (ist_usize i = 0; i < this->count; ++i) {
                ist_dumpitem item = this->items[i];

                if (i) dumpr(dobreak ? ",\n" : ", ");
                if (dobreak) tab(depth + 1);

                dumpf("%s = ", item.name);
                appdumper(item.dumper, item.valp, dobreak ? depth + 1 : -1, style);
            }

            if (dobreak) dumpr("\n"), tab(depth);
            dumpr("}");
            break;
        }
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

    ist_dumpstyle dkind   = style & DKIND_MASK;
    ist_dumpstyle dflag   = style & DFLAG_MASK;
    ist_bool      dobreak = depth != -1;

    if (!(dflag & DFLAG_SPREAD)) style &= DKIND_MASK;
    if (dflag & DFLAG_HEAD_BREAK) dumpr("\n");
    if (dflag & DFLAG_HEAD_INDENT) tab(depth);
    if (this->noname) style |= DFLAG_HEAD_NONAME;

    isg_list*  list   = this->listp;
    ist_usize  llen   = isl_list_catch_length(list->data);
    ist_usize  elen   = llen / this->capacity;
    ist_dumper dumper = this->dumper;

    if (llen % this->capacity) isp_dunreachable();

    switch (dkind) {
        case DKIND_STRUCT:
            dumpr(dobreak ? "[\n" : "[");
            for (ist_usize i = 0; i < list->size; ++i) {
                if (i) dumpr(dobreak ? ",\n" : ", ");
                if (dobreak) tab(depth + 1);
                if (this->header) dumpf(this->header, i), dumpr(" = ");
                appdumper(dumper, list->data + elen * i, dobreak ? depth + 1 : -1, style);
            }

            if (dobreak) dumpr("\n"), tab(depth);
            dumpr("]");
            break;


        case DKIND_JSON:
            dumpr(dobreak ? "[\n" : "[");
            for (ist_usize i = 0; i < list->size; ++i) {
                if (i) dumpr(dobreak ? ",\n" : ", ");
                if (dobreak) tab(depth + 1);
                appdumper(dumper, list->data + elen * i, dobreak ? depth + 1 : -1, style);
            }

            if (dobreak) dumpr("\n"), tab(depth);
            dumpr("]");
            break;


        case DKIND_INDENT:
            if (!dobreak) depth = 0;
            for (ist_usize i = 0; i < list->size; ++i) {
                dumpr("\n");
                tab(depth), dumpr("- ");
                if (this->header) {
                    dumpf(this->header, i), dumpr(":\n");
                    tab(depth + 1), appdumper(dumper, list->data + elen * i, depth + 1, style);
                } else
                    appdumper(dumper, list->data + elen * i, depth, style | DFLAG_BODY_AFT2SPACE);
            }
            break;
        default:
            isp_unreachable();
    }

    return *buffer;
}


#undef dumpf
#undef dumpr
#undef tab