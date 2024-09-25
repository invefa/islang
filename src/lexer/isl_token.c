#include "isl_token.h"

#include <stdio.h>

#include "isl_memgr.h"
#include "isl_report.h"

const ist_string ist_token_reflects[] = {
#   define manifest(_name, _reflect) _reflect,
#   include "isl_tokens.h"
#   undef manifest
};

const ist_string ist_token_names[] = {
#   define manifest(_name, _reflect) #_name,
#   include "isl_tokens.h"
#   undef manifest
};


inline void ist_location_init(ist_location* this, ist_string _module) {
    this->module = _module;
    this->line = 1;
    this->column = 1;
}

/* nothing to clean now */
void ist_location_clean(ist_location* this) {
    isl_ifnreport(this, rid_catch_nullptr, isp_catch_coreloc);
    // if (this->module) ist_string_clean(&this->module);
}

inline void ist_token_init_null(ist_token* this) {
    this->type = ISL_TOKENT_EOF;

    ist_location_init(&this->location, NULL);

    this->extract = NULL;
    this->length = 0;
    this->value.int_value = 0;

}

inline void ist_token_initby_location(ist_token* this, ist_location _location) {
    this->type = ISL_TOKENT_EOF;
    this->location = _location;
    this->extract = NULL;
    this->length = 0;
    this->value.int_value = 0;
}

inline void ist_token_init_full(
    ist_token* this,
    ist_token_type  _type,
    ist_location    _location,
    ist_string      _extract,
    ist_usize       _length,
    ist_value       _value)
{
    this->type = _type;
    this->location = _location;
    this->extract = _extract;
    this->length = _length;
    this->value = _value;
}

inline void ist_token_print(ist_token* this) {
    ist_string extract;
    ist_string_initby_ref(&extract, this->extract, this->length);
    printf("token<0x%zX>:\n", (ist_usize)this);
    printf("module:   <%s>\n", this->location.module);
    printf("location: <%zu:%zu>\n", this->location.line, this->location.column);
    printf("type:     %s\n", ist_token_names[this->type]);
    printf("extract:  \"%s\"\n", extract);
    printf("length:   %zu\n", this->length);
    if (this->type == ISL_TOKENT_INT)
        printf("value:    %lld\n", this->value.int_value);
    if (this->type == ISL_TOKENT_REAL)
        printf("value:    %lf\n", this->value.real_value);
    ist_string_clean(&extract);
}

inline ist_string* ist_token_dump(ist_token* this, ist_string* _buffer) {
    ist_byte storager;
    if (this->extract) {
        storager = this->extract[this->length];
        this->extract[this->length] = '\0';
    }
    snprintf(*_buffer, ISL_DEFAULT_BUFFER_LENGTH,
        "token<0x%zX> {module=<%s>,location=<%zu:%zu>,type=%s,"
        "extract=\"%s\",length=%zu,value={int=%lld,real=%g}}",
        (ist_usize)this,
        this->location.module,
        this->location.line,
        this->location.column,
        ist_token_names[this->type],
        this->extract,
        this->length,
        this->value.int_value,
        this->value.real_value);

    if (this->extract)
        this->extract[this->length] = storager;

    return _buffer;
}
