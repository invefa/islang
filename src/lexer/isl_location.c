#include "isl_location.h"


ist_string ist_location_dump_json(ist_location* this, ist_string* buffer, ist_usize* idxptr) {
    isl_ifnreport(this, rid_catch_nullptr, isp_catch_coreloc);
    return ist_strbuf_sprintf(
        buffer,
        idxptr,
        "\"module\":\"%s%s,\"pagename\":\"%s%s,\"line\":%zu,\"column\":%zu",
        this->module ? this->module->name : (ist_string) "\bnull",
        this->module ? "\"" : "",
        this->pagename ?: (ist_string) "\bnull",
        this->pagename ? "\"" : "",
        this->line,
        this->column
    );
}
