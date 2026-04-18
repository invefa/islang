#include "isl_dump.h"
#include "isl_report.h"

typedef ist_string (*ist_dump_sub_fn)(ist_strbuf, void*);

ist_string dump_json(
    ist_strbuf   buffer,
    ist_usize*   idxptr,
    ist_cstring* name_list,
    void**       valp_list,
    json_kind*   kind_list,
    void**       fn_list,
    ist_usize    count
) {
    ist_strbuf tbuffer = ist_strbuf_cons(8);
    ist_strbuf_append_raw(buffer, idxptr, "{");
    for (ist_usize i = 0; i < count; ++i) {
        if (i) ist_strbuf_append_raw(buffer, idxptr, ",");
        switch (kind_list[i]) {
            case JKIND_STRING:
                if (((ist_dump_sub_fn*)fn_list)[i])
                    ist_strbuf_sprintf(
                        buffer,
                        idxptr,
                        "\"%s\":\"%s\"",
                        name_list[i],
                        i[(ist_dump_sub_fn*)fn_list](tbuffer, valp_list[i])
                    );
                else
                    ist_strbuf_sprintf(
                        buffer, idxptr, "\"%s\":\"%s\"", name_list[i], *(ist_string*)valp_list[i]
                    );
                break;
            case JKIND_ARRAY: {
                ist_dump_sub_fn subfn = ((ist_dump_sub_fn*)fn_list)[i];
                if (subfn) {
                    // printf("test dumping in main: %p\n", valp_list + i);
                    ist_strbuf_sprintf(
                        buffer, idxptr, "\"%s\":[%s]", name_list[i], subfn(tbuffer, valp_list[i])
                    );
                }

                else
                    isp_unreachable();
                break;
            }
            case JKIND_INT:
                ist_strbuf_sprintf(
                    buffer, idxptr, "\"%s\":%d", name_list[i], i[(ist_i64*)valp_list]
                );
                break;
            default:
                isp_unreachable();
                break;
        }
    }
    ist_strbuf_append_raw(buffer, idxptr, "}");
    ist_strbuf_clean(&tbuffer);
    return *buffer;
}

ist_string dump_json_(
    ist_strbuf     buffer,
    ist_usize*     idxptr,
    ist_dump_item* item_list,
    ist_usize      count
) {
    ist_strbuf tbuffer = ist_strbuf_cons(8);
    ist_strbuf_append_raw(buffer, idxptr, "{");
    for (ist_usize i = 0; i < count; ++i) {
        if (i) ist_strbuf_append_raw(buffer, idxptr, ",");

        ist_cstring     name = item_list[i].name;
        void*           valp = item_list[i].valp;
        json_kind       kind = item_list[i].kind;
        ist_dump_sub_fn fn   = item_list[i].fn;

        switch (kind) {
            case JKIND_STRING:
                if (fn)
                    ist_strbuf_sprintf(buffer, idxptr, "\"%s\":\"%s\"", name, fn(tbuffer, valp));
                else ist_strbuf_sprintf(buffer, idxptr, "\"%s\":\"%s\"", name, *(ist_string*)valp);
                break;
            case JKIND_ARRAY:
                if (fn) ist_strbuf_sprintf(buffer, idxptr, "\"%s\":[%s]", name, fn(tbuffer, valp));
                else isp_unreachable();
                break;
            case JKIND_INT:
                ist_strbuf_sprintf(buffer, idxptr, "\"%s\":%d", name, *(ist_i64*)valp);
                break;
            default:
                isp_unreachable();
                break;
        }
    }
    ist_strbuf_append_raw(buffer, idxptr, "}");
    ist_strbuf_clean(&tbuffer);
    return *buffer;
}