#include "inttypes.h"
#include "isg_list.h"
#include "isl_dump.h"
#include "isl_report.h"


#define val(_valp, _type)      (*(_type*)(_valp))
#define tab(_count)            isl_dump_tabs(dctx.buffer, dctx.idxptr, _count)
#define dumpf(_fmt, _vargs...) ist_strbuf_sprintf(dctx.buffer, dctx.idxptr, _fmt, ##_vargs)
#define dumpr(_raw)            ist_strbuf_append_raw(dctx.buffer, dctx.idxptr, _raw)
#define dumprs(_raws...)       ist_strbuf_append_raws(dctx.buffer, dctx.idxptr, ##_raws)
#define asdumper(_adr)         ((ist_dumper)(_adr))
#define appdumper(_dumper, _valp, _indent, _style) \
    asdumper(_dumper)(_valp, ist_dumpctx_{dctx.buffer, dctx.idxptr, _indent, _style})

#define nl()                                dumpr("\n")
#define nltab(_count)                       dumpr("\n"), tab(_count)
#define tabdumpr(_count, _raw)              tab(_count), dumpr(_raw)
#define tabdumpf(_count, _fmt, _vargs...)   tab(_count), dumpf(_fmt, ##_vargs)
#define dumprtab(_count, _raw)              dumpr(_raw), tab(_count)
#define dumpftab(_count, _fmt, _vargs...)   dumpf(_fmt, ##_vargs), tab(_count)
#define nltabdumpr(_count, _raw)            dumpr("\n"), tab(_count), dumpr(_raw)
#define nltabdumpf(_count, _fmt, _vargs...) dumpr("\n"), tab(_count), dumpf(_fmt, ##_vargs)


ist_string _ist_dumpimage_dump_json(ist_dumpimage* this, ist_dumpctx dctx, ist_dumpstyle dflag) {
    dctx.idxptr = dctx.idxptr ?: (ist_usize[1]){};

    ist_bool dowrap = this->wrapkey && this->name;
    ist_bool muline = dctx.indent != -1;

    dumpr("{");
    if (muline) nltab(++dctx.indent);
    for (ist_usize i = 0; i < this->count; ++i) {
        if (!i && dowrap) {
            if (muline) dumpftab(dctx.indent, "\"%s\": \"%s\",\n", this->wrapkey, this->name);
            else dumpf("\"%s\":\"%s\",", this->wrapkey, this->name);
        }
        ist_dumpitem item = this->items[i];

        if (i) {
            dumpr(",");
            if (muline) nltab(dctx.indent);
        }

        dumpf("\"%s\": ", item.key);
        appdumper(item.dumper, item.valp, dctx.indent, dctx.style | DFLAG_THIS_ONVALSIDE);
    }
    if (muline) nltab(--dctx.indent);
    dumpr("}");

    return *dctx.buffer;
}

ist_string _ist_dumpimage_dump_indent(ist_dumpimage* this, ist_dumpctx dctx, ist_dumpstyle dflag) {
    dctx.idxptr = dctx.idxptr ?: (ist_usize[1]){};
    if (dctx.indent == -1) dctx.indent = 0;

    ist_bool dowrap      = dflag & DFLAG_HEAD_DOWRAP && this->wrapkey && this->name;
    ist_bool doaft2space = dflag & DFLAG_BODY_AFT2SPACE && (dowrap || !this->name);

    if (dflag & DFLAG_THIS_ONVALSIDE) nltab(++dctx.indent);

    if (!dowrap && this->name) {
        dumpf("%s:\n", this->name);
        tab(++dctx.indent);
    }
    for (ist_usize i = 0; i < this->count; ++i) {
        if (!i && dowrap) dumpftab(dctx.indent, "%s: %s\n", this->wrapkey, this->name);
        ist_dumpitem item = this->items[i];

        if (i) {
            nltab(dctx.indent);
            if (doaft2space) dumpr("  ");
        }

        dumpf("%s: ", item.key);
        appdumper(item.dumper, item.valp, dctx.indent, dctx.style | DFLAG_THIS_ONVALSIDE);
    }

    return *dctx.buffer;
}

ist_string _ist_dumpimage_dump_struct(ist_dumpimage* this, ist_dumpctx dctx, ist_dumpstyle dflag) {
    dctx.idxptr = dctx.idxptr ?: (ist_usize[1]){};

    ist_bool dowrap   = dflag & DFLAG_HEAD_DOWRAP && this->wrapkey && this->name;
    ist_bool muline   = dctx.indent != -1;
    ist_bool dumpname = !dowrap && this->name;

    if (dumpname) dumpf("(%s)", this->name);
    dumpr("{");
    if (muline) nltab(++dctx.indent);
    for (ist_usize i = 0; i < this->count; ++i) {
        if (!i && dowrap) {
            if (muline) dumpftab(dctx.indent, ".%s = %s,\n", this->wrapkey, this->name);
            else dumpf(".%s=%s,", this->wrapkey, this->name);
        }
        ist_dumpitem item = this->items[i];

        if (i) {
            dumpr(",");
            if (muline) nltab(dctx.indent);
        }

        if (muline) dumpf(".%s = ", item.key);
        else dumpf(".%s=", item.key);
        appdumper(item.dumper, item.valp, dctx.indent, dctx.style | DFLAG_THIS_ONVALSIDE);
    }
    if (muline) nltab(--dctx.indent);
    dumpr("}");

    return *dctx.buffer;
}

ist_string ist_dumpimage_dump(ist_dumpimage* this, ist_dumpctx dctx) {
    dctx.idxptr = dctx.idxptr ?: (ist_usize[1]){};

    static const ist_string (*forward[])(ist_dumpimage*, ist_dumpctx, ist_dumpstyle) = {
        [DKIND_JSON]   = _ist_dumpimage_dump_json,
        [DKIND_INDENT] = _ist_dumpimage_dump_indent,
        [DKIND_STRUCT] = _ist_dumpimage_dump_struct,
    };

    ist_dumpstyle dkind = dctx.style & DKIND_MASK;
    ist_dumpstyle dflag = dctx.style & DFLAG_MASK;
    if (dflag & DFLAG_HEAD_NL) dumpr("\n");
    if (dflag & DFLAG_HEAD_TAB) tab(dctx.indent);
    dctx.style &= ~DFLAG_MASK_ONCE;

    if (forward[dkind]) return forward[dkind](this, dctx, dflag);
    else isp_unreachable();
    return *dctx.buffer;
}

ist_string isg_list_dumpack_dump(isg_list_dumpack* this, ist_dumpctx dctx) {
    dctx.idxptr = dctx.idxptr ?: (ist_usize[1]){};

    const ist_dumpstyle dkind  = dctx.style & DKIND_MASK;
    const ist_dumpstyle dflag  = dctx.style & DFLAG_MASK;
    const ist_bool      muline = dctx.indent != -1;

    if (!(dflag & DFLAG_SPREAD)) dctx.style &= DKIND_MASK;
    if (dflag & DFLAG_HEAD_NL) dumpr("\n");
    if (dflag & DFLAG_HEAD_TAB) tab(dctx.indent);
    if (this->dowrap) dctx.style |= DFLAG_HEAD_DOWRAP;

    isg_list*  list   = this->listp;
    ist_usize  llen   = isl_list_catch_length(list->data);
    ist_usize  elen   = llen / this->capacity;
    ist_dumper dumper = this->dumper;

    if (llen % this->capacity) isp_dunreachable();

    switch (dkind) {
        case DKIND_STRUCT:
            dumpr(muline ? "{\n" : "{");
            for (ist_usize i = 0; i < list->size; ++i) {
                if (i) dumpr(muline ? ",\n" : ", ");
                if (muline) tab(dctx.indent + 1);
                if (this->idxtag) dumpf(this->idxtag, i), dumpr(" = ");
                appdumper(dumper, list->data + elen * i, muline ? dctx.indent + 1 : -1, dctx.style);
            }

            if (muline) dumpr("\n"), tab(dctx.indent);
            dumpr("}");
            break;


        case DKIND_JSON:
            dumpr(muline ? "[\n" : "[");
            for (ist_usize i = 0; i < list->size; ++i) {
                if (i) dumpr(muline ? ",\n" : ", ");
                if (muline) tab(dctx.indent + 1);
                appdumper(dumper, list->data + elen * i, muline ? dctx.indent + 1 : -1, dctx.style);
            }

            if (muline) dumpr("\n"), tab(dctx.indent);
            dumpr("]");
            break;


        case DKIND_INDENT:
            if (!muline) dctx.indent = 0;
            for (ist_usize i = 0; i < list->size; ++i) {
                dumpr("\n");
                tab(dctx.indent), dumpr("- ");
                if (this->idxtag) {
                    dumpf(this->idxtag, i), dumpr(":\n");
                    tab(dctx.indent + 1),
                        appdumper(dumper, list->data + elen * i, dctx.indent + 1, dctx.style);
                } else
                    appdumper(
                        dumper,
                        list->data + elen * i,
                        dctx.indent,
                        dctx.style | DFLAG_BODY_AFT2SPACE
                    );
            }
            break;
        default:
            isp_unreachable();
    }

    return *dctx.buffer;
}


// TODO: use dumpctx also, and it can dump tabs intelligently.
// we can divide all of the component of dumping string into the function, make it smart!!
ist_string isl_dump_tabs(ist_strbuf buffer, ist_usize* idxptr, ist_usize count) {
    if (count == -1) {
        isp_unreachable();
    }
    for (ist_usize i = 0; i < count; ++i) ist_strbuf_append_raw(buffer, idxptr, "    ");
    return *buffer;
}

#define _ist_x_dump_impl(_x)                                      \
    ist_string ist_##_x##_dump(ist_vptr this, ist_dumpctx dctx) { \
        dctx.idxptr = dctx.idxptr ?: (ist_usize[1]){};            \
        return dumpf(PRI##_x, val(this, ist_##_x));               \
    }

_ist_x_dump_impl(u8);
_ist_x_dump_impl(u16);
_ist_x_dump_impl(u32);
_ist_x_dump_impl(u64);
_ist_x_dump_impl(i8);
_ist_x_dump_impl(i16);
_ist_x_dump_impl(i32);
_ist_x_dump_impl(i64);
ist_string ist_usize_dump(ist_vptr this, ist_dumpctx dctx) {
    dctx.idxptr = dctx.idxptr ?: (ist_usize[1]){};
    return dumpf(PRIuPTR, val(this, ist_usize));
}

ist_string ist_f32_dump(ist_vptr this, ist_dumpctx dctx) {
    dctx.idxptr = dctx.idxptr ?: (ist_usize[1]){};
    return dumpf("%f", val(this, ist_f32));
}
ist_string ist_f64_dump(ist_vptr this, ist_dumpctx dctx) {
    dctx.idxptr = dctx.idxptr ?: (ist_usize[1]){};
    return dumpf("%f", val(this, ist_f64));
}

ist_string ist_bool_dump(ist_vptr this, ist_dumpctx dctx) {
    dctx.idxptr = dctx.idxptr ?: (ist_usize[1]){};
    return dumpf("%s", val(this, ist_bool) ? "true" : "false");
}

ist_string ist_cstring_dump(ist_vptr this, ist_dumpctx dctx) {
    dctx.idxptr = dctx.idxptr ?: (ist_usize[1]){};
    return dumpf("\"%s\"", val(this, ist_cstring));
}
ist_string ist_cstring_dump_ident(ist_vptr this, ist_dumpctx dctx) {
    dctx.idxptr = dctx.idxptr ?: (ist_usize[1]){};

    switch (dctx.style &= DKIND_MASK) {
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
    return *dctx.buffer;
}

#undef val
#undef tab
#undef dumpf
#undef dumpr
#undef asdumper
#undef appdumper
