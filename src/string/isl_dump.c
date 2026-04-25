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


ist_string isl_tab_dump(ist_string* reserved, ist_dumpctx dctx) {
    if (dctx.indent == -1) isp_unreachable();
    for (ist_usize i = 0; i < dctx.indent; ++i)
        ist_strbuf_append_raw(dctx.buffer, dctx.idxptr, "    ");
    if (reserved) ist_strbuf_append_raw(dctx.buffer, dctx.idxptr, *reserved);
    return *dctx.buffer;
}

ist_string _ist_dumpimage_dump_json(ist_dumpimage* this, ist_dumpctx dctx) {
    dctx.idxptr = dctx.idxptr ?: (ist_usize[1]){};

    /**
     * expect inline dumping, in some dumping format(like indent), it will be ignore.
     * But at here, we need to handle it.
     */
    ist_bool dowrap = this->wrapkey && this->name;
    ist_bool muline = dctx.indent != -1;
    // if (dctx.style & DFLAG_THIS_ONVALSIDE && muline) dumpr(" ");

    dumpr("{");
    if (muline) dumpr("\n"), tab(++dctx.indent);
    for (ist_usize i = 0; i < this->count; ++i) {
        if (!i && dowrap) {
            if (muline) dumpf("\"%s\": \"%s\",\n", this->wrapkey, this->name), tab(dctx.indent);
            else dumpf("\"%s\":\"%s\",", this->wrapkey, this->name);
        }
        ist_dumpitem item = this->items[i];

        if (i) {
            dumpr(",");
            if (muline) dumpr("\n"), tab(dctx.indent);
        }

        dumpf("\"%s\": ", item.key);
        appdumper(item.dumper, item.valp, dctx.indent, dctx.style | DFLAG_THIS_ONVALSIDE);
    }
    if (muline) dumpr("\n"), tab(--dctx.indent);
    dumpr("}");

    return *dctx.buffer;
}

ist_string _ist_dumpimage_dump_indent(ist_dumpimage* this, ist_dumpctx dctx) {
    dctx.idxptr = dctx.idxptr ?: (ist_usize[1]){};
    if (dctx.indent == -1) dctx.indent = 0;

    ist_bool dowrap      = dctx.style & DFLAG_HEAD_DOWRAP && this->wrapkey && this->name;
    ist_bool doaft2space = dctx.style & DFLAG_BODY_AFT2SPACE && (dowrap || !this->name);
    if (dctx.style & DFLAG_THIS_ONVALSIDE) dumpr("\n"), tab(dctx.indent);

    if (!dowrap && this->name) dumpf("%s:\n", this->name);
    tab(++dctx.indent);
    for (ist_usize i = 0; i < this->count; ++i) {
        if (!i && dowrap) dumpf("%s: %s\n", this->wrapkey, this->name), tab(dctx.indent);
        ist_dumpitem item = this->items[i];

        if (i) {
            dumpr("\n"), tab(dctx.indent);
            if (doaft2space) dumpr("  ");
        }

        dumpf("%s: ", item.key);
        appdumper(item.dumper, item.valp, dctx.indent + 1, dctx.style | DFLAG_THIS_ONVALSIDE);
    }

    return *dctx.buffer;
}

ist_string _ist_dumpimage_dump_struct(ist_dumpimage* this, ist_dumpctx dctx) {
    dctx.idxptr = dctx.idxptr ?: (ist_usize[1]){};

    /**
     * expect inline dumping, in some dumping format(like indent), it will be ignore.
     * But at here, we need to handle it.
     */
    ist_bool dowrap = dctx.style & DFLAG_HEAD_DOWRAP && this->wrapkey && this->name;
    ist_bool muline = dctx.indent != -1;
    if (dctx.style & DFLAG_THIS_ONVALSIDE && muline) dumpr("\n"), tab(dctx.indent);

    if (!dowrap && this->name) dumpf("(%s)", this->name);
    dumpr("{");
    if (muline) dumpr("\n"), tab(++dctx.indent);
    for (ist_usize i = 0; i < this->count; ++i) {
        if (!i && dowrap) {
            if (muline) dumpf(".%s = %s,\n", this->wrapkey, this->name), tab(dctx.indent);
            else dumpf(".%s=%s,", this->wrapkey, this->name);
        }
        ist_dumpitem item = this->items[i];

        if (i) {
            dumpr(",");
            if (muline) dumpr("\n"), tab(dctx.indent);
        }

        if (muline) dumpf(".%s = ", item.key);
        else dumpf(".%s=", item.key);
        appdumper(
            item.dumper, item.valp, muline ? dctx.indent + 1 : -1, dctx.style | DFLAG_THIS_ONVALSIDE
        );
    }
    if (muline) dumpr("\n"), tab(--dctx.indent);
    dumpr("}");

    return *dctx.buffer;
}

ist_string ist_dumpimage_dump(ist_dumpimage* this, ist_dumpctx dctx) {
    dctx.idxptr = dctx.idxptr ?: (ist_usize[1]){};

    static const ist_dumper forward[] = {
        [DKIND_JSON]   = (ist_dumper)_ist_dumpimage_dump_json,
        [DKIND_INDENT] = (ist_dumper)_ist_dumpimage_dump_indent,
        [DKIND_STRUCT] = (ist_dumper)_ist_dumpimage_dump_struct,
    };

    ist_dumpstyle dkind = dctx.style & DKIND_MASK;
    if (dctx.style & DFLAG_HEAD_NL) dumpr("\n");
    if (dctx.style & DFLAG_HEAD_TAB) tab(dctx.indent);

    if (forward[dkind]) return forward[dkind](this, dctx);
    else isp_unreachable();
    return *dctx.buffer;
    // static const struct dumpimage_fmtcons {
    //     ist_cstring space;
    //     ist_cstring namef;
    //     ist_cstring open;
    //     ist_cstring wrapf;
    //     ist_cstring keyf;
    //     ist_cstring divide;
    //     ist_cstring close;
    //     ist_cstring line;
    //     ist_bool    allow_inline;
    //     ist_bool    allow_muline;
    //     ist_bool    allow_headspace;
    //     ist_bool    allow_open;
    //     ist_bool    allow_wrap;
    //     ist_bool    allow_keyspace;
    //     ist_bool    allow_divide;
    //     ist_bool    allow_close;
    //     ist_bool    allow_newline;
    // } mulinefmts[] =
    //     {
    //         [DKIND_INDENT] =
    //             {.space  = " ",
    //              .namef  = "%s:\n",
    //              .open   = "",
    //              .wrapf  = "%s: %s",
    //              .keyf   = "%s: ",
    //              .divide = "\n",
    //              .close  = "",
    //              .line   = "\n"},
    //         [DKIND_STRUCT] =
    //             {.space  = " ",
    //              .namef  = "(%s)",
    //              .open   = "{\n",
    //              .wrapf  = "",
    //              .keyf   = ".%s = ",
    //              .divide = ",\n",
    //              .close  = "}",
    //              .line   = "\n"},
    //         [DKIND_JSON] =
    //             {.space  = " ",
    //              .namef  = "",
    //              .open   = "{\n",
    //              .wrapf  = "\"%s\": \"%s\"",
    //              .keyf   = "\"%s\": ",
    //              .divide = ",\n",
    //              .close  = "}",
    //              .line   = "\n"},

    //     },
    //   inlinefmts[] = {
    //       [DKIND_INDENT] = mulinefmts[DKIND_INDENT],
    //       [DKIND_STRUCT] =
    //           {.space  = "",
    //            .namef  = "(%s)",
    //            .open   = "{",
    //            .wrapf  = "",
    //            .keyf   = ".%s=",
    //            .divide = ",",
    //            .close  = "}",
    //            .line   = ""},
    //       [DKIND_JSON] =
    //           {.space  = "",
    //            .namef  = "",
    //            .open   = "{",
    //            .wrapf  = "\"%s\":%s",
    //            .keyf   = "\"%s\":",
    //            .divide = ",",
    //            .close  = "}",
    //            .line   = ""},
    //   };


    // ist_bool      muline = dctx.indent != -1;
    // ist_bool      noname = !this->name;
    // ist_bool      dowrap = this->name && this->wrapkey && dflag & DFLAG_HEAD_DOWRAP;



    // struct dumpimage_fmtcons fmt = dctx.indent != -1 ? mulinefmts[dkind] : inlinefmts[dkind];
    // // TODO: use a table and one drive code to dump all format.

    // // ist_bool expact_muline = dctx.indent != -1;
    // ist_bool dump_expinline = !!(dctx.indent == -1); // expect inline
    // ist_bool dump_headnl    = !!(dflag & DFLAG_HEAD_NL);
    // ist_bool dump_headtab   = !!(dflag & DFLAG_HEAD_TAB);
    // ist_bool dump_muline    = !!(*fmt.line); // force muline as expect, ensure by fmt selection
    // ist_bool dump_wrap = !!(*fmt.wrapf && this->name && this->wrapkey && dflag |
    // DFLAG_HEAD_DOWRAP); ist_bool dump_headspace      = !!(*fmt.space && !dump_wrap && dflag |
    // DFLAG_THIS_ONVALSIDE); ist_bool dump_name           = !!(this->name && !dump_wrap); ist_bool
    // dump_open           = !!(*fmt.open); ist_bool dump_divide         = !!(*fmt.divide); ist_bool
    // dump_loopindent     = !!(dump_muline); ist_bool dump_loopstepindent = !!(dump_muline);
    // ist_bool dump_valindent      = !!(dump_expinline && *fmt.line);
    // ist_bool dump_valstepindent  = !!(dump_expinline && *fmt.line);
    // ist_bool dump_closenltab     = !!(dump_muline);
    // ist_bool dump_close          = !!(*fmt.close);

    // if (dump_muline && dctx.indent == -1) dctx.indent = 0;
    // if (dump_headspace) dumpr(fmt.space);
    // if (dump_headnl) dumpr("\n");
    // if (dump_headtab) tab(dctx.indent);
    // if (dump_name) dumpf(fmt.namef, this->name);
    // if (dump_open) dumpr(fmt.open);
    // for (ist_usize i = 0; i < this->count; ++i) {
    //     if (!i && dump_wrap) {
    //         if (dump_muline) dumpr(fmt.line), tab(dctx.indent + (dump_loopstepindent ? 1 : 0));
    //         dumpf(fmt.wrapf, this->wrapkey, this->name);
    //         if (dump_divide) dumpr(fmt.divide);
    //     }

    //     ist_dumpitem item = this->items[i];

    //     if (dump_divide && i) dumpr(fmt.divide);
    //     if (dump_loopindent) {
    //         tab(dctx.indent + (dump_loopstepindent ? 1 : 0));
    //         if (dump_muline) dumpr(fmt.line);
    //     }
    //     dumpf(fmt.keyf, item.key);
    //     appdumper(
    //         item.dumper,
    //         item.valp,
    //         dump_valindent ? dctx.indent + (dump_valstepindent ? 1 : 0) : -1,
    //         dctx.style | DFLAG_THIS_ONVALSIDE
    //     );
    //     // if (dump_muline) dumpr(fmt.line);
    // }

    // if (dump_closenltab) dumpr(fmt.line), tab(dctx.indent);
    // if (dump_close) dumpr(fmt.close);

    // return *dctx.buffer;

    // switch (dkind) {
    //     case DKIND_JSON: {
    //         dumpr(fmt.open);
    //         for (ist_usize i = 0; i < this->count; ++i) {
    //             if (this->name && this->wrapkey && !i) {
    //                 if (muline) tab(dctx.indent + 1);
    //                 dumpf("\"%s\": \"%s\"%s", this->wrapkey, this->name, fmt.divide);
    //             }
    //             ist_dumpitem item = this->items[i];

    //             if (i) dumpr(fmt.divide);
    //             if (muline) tab(dctx.indent + 1);

    //             dumpf("\"%s\": ", item.key);
    //             appdumper(item.dumper, item.valp, muline ? dctx.indent + 1 : -1, dctx.style);
    //         }

    //         if (muline) dumpr(fmt.line), tab(dctx.indent);
    //         dumpr(fmt.close);
    //         break;
    //     }


    //     case DKIND_INDENT: {
    //         if (!muline) dctx.indent = 0;

    //         if (dflag & DFLAG_THIS_ONVALSIDE) dumpr("\n"), tab(dctx.indent);
    //         if (!noname && !dowrap) dumpf("%s:\n", this->name), ++dctx.indent;
    //         for (ist_usize i = 0; i < this->count; ++i) {
    //             if (dowrap && !i) dumpf("%s: %s\n", this->wrapkey, this->name);

    //             ist_dumpitem item = this->items[i];

    //             if (i) dumpr("\n");
    //             if (i || !noname) tab(dctx.indent);
    //             if (i && !noname && dflag & DFLAG_BODY_AFT2SPACE) dumpr("  ");

    //             dumpf("%s: ", item.key);
    //             appdumper(
    //                 item.dumper, item.valp, dctx.indent + 1, dctx.style | DFLAG_THIS_ONVALSIDE
    //             );
    //         }
    //         break;
    //     }


    //     case DKIND_STRUCT: {
    //         // if (muline && dflag & DFLAG_THIS_ONVALSIDE && !noname) dumpr("\n"),
    //         // tab(++dctx.indent);
    //         if (!noname) dumpf("(%s)", this->name);
    //         dumpr(muline ? "{\n" : "{");
    //         for (ist_usize i = 0; i < this->count; ++i) {
    //             ist_dumpitem item = this->items[i];

    //             if (i) dumpr(muline ? ",\n" : ", ");
    //             if (muline) tab(dctx.indent + 1);

    //             dumpf(".%s = ", item.key);
    //             appdumper(
    //                 item.dumper,
    //                 item.valp,
    //                 muline ? dctx.indent + 1 : -1,
    //                 dctx.style | DFLAG_THIS_ONVALSIDE
    //             );
    //         }

    //         if (muline) dumpr("\n"), tab(dctx.indent);
    //         dumpr("}");
    //         break;
    //     }
    //     default:
    //         isp_unreachable();
    // }

    // return *dctx.buffer;
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
