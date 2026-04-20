#include "isl_location.h"
#include "isl_module.h"


ist_string ist_location_dump_old(ist_location* this, ist_string* buffer, ist_usize* idxptr) {
    isl_ifnreport(this, rid_catch_nullptr, isp_catch_coreloc);
    ist_strbuf_sprintf(
        ((buffer)),
        idxptr,
        "\"location\":\"%s",
        this->module ? this->module->name : (ist_string) "(unknown)"
    );
    if (this->pagename) ist_strbuf_sprintf(buffer, idxptr, ":%s", this->pagename);
    return ist_strbuf_sprintf(buffer, idxptr, ":%zu:%zu\"", this->line, this->column);
}

ist_string ist_location_dump(
    ist_location* this,
    ist_strbuf    buffer,
    ist_usize*    idxptr,
    ist_usize     depth,
    ist_dumpstyle style
) {
    idxptr = idxptr ?: (ist_usize[1]){};
    ist_strbuf_sprintf(buffer, idxptr, "\"%s", this->module ? this->module->name : "(unknown)");
    if (this->pagename) ist_strbuf_sprintf(buffer, idxptr, ":%s", this->pagename);
    return ist_strbuf_sprintf(buffer, idxptr, ":%zu:%zu\"", this->line, this->column);
}
