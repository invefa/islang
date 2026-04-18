#include "inttypes.h"
#include "isl_dump.h"
#include "isl_report.h"

#define val(_valp, _type) (*(_type*)(_valp))

ist_string dump_u8(void* valp, ist_strbuf buffer, ist_usize* idxptr) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, PRIu8, val(valp, ist_u8));
}
ist_string dump_u16(void* valp, ist_strbuf buffer, ist_usize* idxptr) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, PRIu16, val(valp, ist_u16));
}
ist_string dump_u32(void* valp, ist_strbuf buffer, ist_usize* idxptr) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, PRIu32, val(valp, ist_u32));
}
ist_string dump_u64(void* valp, ist_strbuf buffer, ist_usize* idxptr) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, PRIu64, val(valp, ist_u64));
}

ist_string dump_i8(void* valp, ist_strbuf buffer, ist_usize* idxptr) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, PRIi8, val(valp, ist_i8));
}
ist_string dump_i16(void* valp, ist_strbuf buffer, ist_usize* idxptr) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, PRIi16, val(valp, ist_i16));
}
ist_string dump_i32(void* valp, ist_strbuf buffer, ist_usize* idxptr) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, PRIi32, val(valp, ist_i32));
}
ist_string dump_i64(void* valp, ist_strbuf buffer, ist_usize* idxptr) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, PRIi64, val(valp, ist_i64));
}

ist_string dump_f32(void* valp, ist_strbuf buffer, ist_usize* idxptr) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, "%f", val(valp, ist_f32));
}
ist_string dump_f64(void* valp, ist_strbuf buffer, ist_usize* idxptr) {
    idxptr = idxptr ?: (ist_usize[1]){};
    return ist_strbuf_sprintf(buffer, idxptr, "%llf", val(valp, ist_f64));
}


// ist_string isl_dump_json(
//     ist_strbuf     buffer,
//     ist_usize*     idxptr,
//     ist_dump_item* item_list,
//     ist_usize      count
// ) {
//     idxptr = idxptr ?: (ist_usize[1]){};

//     ist_strbuf tbuffer = ist_strbuf_cons(8);
//     ist_strbuf_append_raw(buffer, idxptr, "{");

//     for (ist_usize i = 0; i < count; ++i) {
//         if (i) ist_strbuf_append_raw(buffer, idxptr, ",");

//         ist_cstring   name = item_list[i].name;
//         void*         valp = item_list[i].valp;
//         ist_json_kind kind = item_list[i].kind;
//         ist_dump_fn   fn   = item_list[i].fn;

// #define dumps(_vargs...) ist_strbuf_sprintf(buffer, idxptr, ##_vargs)

//         static const ist_cstring fmt = "\"%s\":%s";

//         switch (kind) {
//             case JKIND_OBJECT:
//                 if (fn) dumps(fmt, name, fn(valp, tbuffer, NULL));
//                 else {
//                     ist_dump_item_clist clist = val(valp, ist_dump_item_clist);
//                     dumps(fmt, name, ist_dumpimage_dump_json(tbuffer, NULL, clist.data,
//                     clist.count));
//                 }
//                 break;
//             case JKIND_ARRAY:
//                 if (fn) dumps(fmt, name, fn(valp, tbuffer, NULL));
//                 else isp_unreachable();
//                 break;
//             case JKIND_STRING:
//                 if (fn) dumps("\"%s\":\"%s\"", name, fn(valp, tbuffer, NULL));
//                 else dumps("\"%s\":\"%s\"", name, val(valp, ist_string));
//                 break;
//             case JKIND_INT:
//                 if (fn) dumps(fmt, name, fn(valp, tbuffer, NULL));
//                 else dumps("\"%s\":%" PRIi64, name, val(valp, ist_i64));
//                 break;
//             case JKIND_FLOAT:
//                 if (fn) dumps(fmt, name, fn(valp, tbuffer, NULL));
//                 else dumps("\"%s\":%llf", name, val(valp, ist_f64));
//                 break;
//             case JKIND_BOOL:
//                 dumps(fmt, name, val(valp, ist_bool) ? "true" : "false");
//                 break;
//             case JKIND_TRUE:
//                 dumps("\"%s\":true", name);
//                 break;
//             case JKIND_FALSE:
//                 dumps("\"%s\":false", name);
//                 break;
//             default:
//                 isp_unreachable();
//                 break;
//         }
//     }

// #undef dumps

//     ist_strbuf_append_raw(buffer, idxptr, "}");
//     ist_strbuf_clean(&tbuffer);
//     return *buffer;
// }

ist_string ist_dumpimage_dump_json(ist_dumpimage* image, ist_strbuf buffer, ist_usize* idxptr) {
    idxptr = idxptr ?: (ist_usize[1]){};

    ist_strbuf tbuffer = ist_strbuf_cons(8);
    ist_strbuf_append_raw(buffer, idxptr, "{");

    for (ist_usize i = 0; i < image->count; ++i) {
        if (i) ist_strbuf_append_raw(buffer, idxptr, ",");

        ist_cstring   name = image->items[i].name;
        void*         valp = image->items[i].valp;
        ist_json_kind kind = image->items[i].kind;
        ist_dump_fn   fn   = image->items[i].fn;

#define dumps(_vargs...) ist_strbuf_sprintf(buffer, idxptr, ##_vargs)

        static const ist_cstring fmt = "\"%s\":%s";

        switch (kind) {
            case JKIND_OBJECT:
                if (fn) dumps(fmt, name, fn(valp, tbuffer, NULL));
                else
                    dumps(
                        fmt,
                        name,
                        ist_dumpimage_dump_json(&val(valp, ist_dumpimage), tbuffer, NULL)
                    );
                break;
            case JKIND_ARRAY:
                if (fn) dumps(fmt, name, fn(valp, tbuffer, NULL));
                else isp_unreachable();
                break;
            case JKIND_STRING:
                if (fn) dumps("\"%s\":\"%s\"", name, fn(valp, tbuffer, NULL));
                else dumps("\"%s\":\"%s\"", name, val(valp, ist_string));
                break;
            case JKIND_INT:
                if (fn) dumps(fmt, name, fn(valp, tbuffer, NULL));
                else dumps("\"%s\":%" PRIi64, name, val(valp, ist_i64));
                break;
            case JKIND_FLOAT:
                if (fn) dumps(fmt, name, fn(valp, tbuffer, NULL));
                else dumps("\"%s\":%llf", name, val(valp, ist_f64));
                break;
            case JKIND_BOOL:
                dumps(fmt, name, val(valp, ist_bool) ? "true" : "false");
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
