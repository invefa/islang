#include "inttypes.h"
#include "isg_list.h"
#include "isl_dump.h"
#include "isl_report.h"


#define val(_valp, _type)      (*(_type*)(_valp))
#define tab(_count)            isl_dump_tabs(dctx.buffer, dctx.idxptr, _count)
#define dumpf(_fmt, _vargs...) ist_strbuf_sprintf(dctx.buffer, dctx.idxptr, _fmt, ##_vargs)
#define dumpr(_raw)            ist_strbuf_append_raw(dctx.buffer, dctx.idxptr, _raw)
#define asdumper(_adr)         ((ist_dumper)(_adr))
#define appdumper(_dumper, _valp, _depth, _style) \
    asdumper(_dumper)(_valp, ist_dumpctx_{dctx.buffer, dctx.idxptr, _depth, _style})


ist_string ist_dumpimage_dump(ist_dumpimage* this, ist_dumpctx dctx) {
    dctx.idxptr = dctx.idxptr ?: (ist_usize[1]){};

    const ist_dumpstyle dkind  = dctx.style & DKIND_MASK;
    const ist_dumpstyle dflag  = dctx.style & DFLAG_MASK;
    const ist_bool      muline = dctx.indent != -1;
    const ist_bool      dowrap = this->name && dflag & DFLAG_HEAD_DOWRAP;
    const ist_bool      noname = !this->name;

    if (!(dflag & DFLAG_SPREAD)) dctx.style &= DKIND_MASK;
    if (dflag & DFLAG_HEAD_NL) dumpr("\n");
    if (dflag & DFLAG_HEAD_TAB) tab(dctx.indent);

    static const struct _fmtcons {
        ist_cstring open;
        ist_cstring close;
        ist_cstring mapto;
        ist_cstring divide;
        ist_cstring line;
    } silinefmts[] =
        {
            [DKIND_JSON] =
                {
                    .open   = "{",
                    .close  = "}",
                    .mapto  = ":",
                    .divide = ",",
                    .line   = NULL,
                },
            [DKIND_INDENT] =
                {
                    .open   = NULL,
                    .close  = NULL,
                    .mapto  = ": ",
                    .divide = "\n",
                    .line   = "\n",
                },
            [DKIND_STRUCT] =
                {
                    .open   = "{",
                    .close  = "}",
                    .mapto  = "=",
                    .divide = ",",
                    .line   = NULL,
                },
        },
      mulinefmts[] = {
          [DKIND_JSON] =
              {
                  .open   = "{\n",
                  .close  = "}",
                  .mapto  = ":",
                  .divide = ",\n",
                  .line   = "\n",
              },
          [DKIND_INDENT] =
              {
                  .open   = NULL,
                  .close  = NULL,
                  .mapto  = ": ",
                  .divide = "\n",
                  .line   = "\n",
              },
          [DKIND_STRUCT] =
              {
                  .open   = "{\n",
                  .close  = "}",
                  .mapto  = " = ",
                  .divide = ", ",
                  .line   = "\n",
              },
      };

    struct _fmtcons fmt = muline ? mulinefmts[dkind] : silinefmts[dkind];
    // TODO: use a table and one drive code to dump all format.

    switch (dkind) {
        case DKIND_JSON: {
            dumpr(fmt.open);
            for (ist_usize i = 0; i < this->count; ++i) {
                if (dowrap && !i) {
                    if (muline) tab(dctx.indent + 1);
                    dumpf("\"%s\": \"%s\"%s", this->wrapkey, this->name, fmt.divide);
                }
                ist_dumpitem item = this->items[i];

                if (i) dumpr(fmt.divide);
                if (muline) tab(dctx.indent + 1);

                dumpf("\"%s\": ", item.key);
                appdumper(item.dumper, item.valp, muline ? dctx.indent + 1 : -1, dctx.style);
            }

            if (muline) dumpr(fmt.line), tab(dctx.indent);
            dumpr(fmt.close);
            break;
        }


        case DKIND_INDENT: {
            if (!muline) dctx.indent = 0;

            if (dflag & DFLAG_THIS_ONVALSIDE) dumpr("\n"), tab(dctx.indent);
            if (!noname && !dowrap) dumpf("%s:\n", this->name), ++dctx.indent;
            for (ist_usize i = 0; i < this->count; ++i) {
                if (dowrap && !i) dumpf("%s: %s\n", this->wrapkey, this->name);

                ist_dumpitem item = this->items[i];

                if (i) dumpr("\n");
                if (i || !noname) tab(dctx.indent);
                if (i && !noname && dflag & DFLAG_BODY_AFT2SPACE) dumpr("  ");

                dumpf("%s: ", item.key);
                appdumper(
                    item.dumper, item.valp, dctx.indent + 1, dctx.style | DFLAG_THIS_ONVALSIDE
                );
            }
            break;
        }


        case DKIND_STRUCT: {
            // if (muline && dflag & DFLAG_THIS_ONVALSIDE && !noname) dumpr("\n"), tab(++dctx.indent);
            if (!noname) dumpf("(%s)", this->name);
            dumpr(muline ? "{\n" : "{");
            for (ist_usize i = 0; i < this->count; ++i) {
                ist_dumpitem item = this->items[i];

                if (i) dumpr(muline ? ",\n" : ", ");
                if (muline) tab(dctx.indent + 1);

                dumpf(".%s = ", item.key);
                appdumper(
                    item.dumper,
                    item.valp,
                    muline ? dctx.indent + 1 : -1,
                    dctx.style | DFLAG_THIS_ONVALSIDE
                );
            }

            if (muline) dumpr("\n"), tab(dctx.indent);
            dumpr("}");
            break;
        }
        default:
            isp_unreachable();
    }

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



ist_string isl_dump_tabs(ist_strbuf buffer, ist_usize* idxptr, ist_usize count) {
    if (count == -1) isp_unreachable();
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
