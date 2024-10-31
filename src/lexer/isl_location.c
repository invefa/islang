#include "isl_location.h"


ist_string ist_location_dump_json(ist_location* this, ist_string* buffer, ist_usize* idxptr) {
    isl_ifnreport(this, rid_catch_nullptr, isp_catch_coreloc);
    ist_strbuf_sprintf(
        ((buffer)),
        idxptr,
        "\"location\":\"%s",
        this->module ? this->module->name : (ist_string) "(null)"
    );
    if (this->pagename) ist_strbuf_sprintf(buffer, idxptr, ":%s", this->pagename);
    return ist_strbuf_sprintf(buffer, idxptr, ":%zu:%zu\"", this->line, this->column);
}
