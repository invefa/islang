#include "isl_codepage.h"


inline ist_string isl_read_file(ist_cstring _filepath) {
    FILE* file = fopen(_filepath, "rb");
    isl_ifnreport(file, rid_open_file_failed, _filepath, isp_catch_coreloc);
    fseek(file, 0, SEEK_END);
    ist_usize length = ftell(file);
    fseek(file, 0, SEEK_SET);
    ist_string source = isl_list_malloc(ist_byte, length + 1);
    fread(source, 1, length, file);
    source[length] = '\0';
    fclose(file);
    return source;
}

ist_codepage* ist_codepage_createby_full(
    ist_string    _name,
    ist_module*   _module,
    ist_string    _source,
    ist_codepage* _prev_page
) {

    isl_assert(_module, "codepage must belong to a module.");

    ist_codepage* this = isl_malloc(ist_codepage);

    /* initialize the basic information */
    // this->name = _name;
    // this->module = _module;
    this->source    = _source;
    this->prev_page = _prev_page;

    /* register the string buffer to the module */
    ist_module_register_strbuf(_module, _name, ISL_STRBUFT_NAME);
    ist_module_register_strbuf(_module, _source, ISL_STRBUFT_SOURCE);

    /* initialize the UTF-8 decode information */
    this->next_sequence_index     = 0;
    this->decode_codepoint_length = 0;

    /* decode the first utf8 sequence */
    this->current_codepoint =
        isl_utf8_decode(&this->source, this->next_sequence_index, &this->decode_codepoint_length);
    this->next_sequence_index += this->decode_codepoint_length;

    /* initialize the location */
    this->location = ist_location_consby_full(_module, _name);

    return this;
}
ist_codepage* ist_codepage_createby_filepath(ist_module* _module, ist_string _filepath) {
    ist_string source = isl_read_file(_filepath);
    return ist_codepage_createby_full(NULL, _module, source, NULL);
}
ist_codepage* ist_codepage_createby_source(
    ist_module* _module,
    ist_string  _name,
    ist_string  _source
) {
    return ist_codepage_createby_full(_name, _module, _source, NULL);
}
ist_codepage* ist_codepage_createby_string(
    ist_module* _module,
    ist_string  _name,
    ist_string  _string,
    ist_usize   _length
) {
    return ist_codepage_createby_full(
        _name, _module, ist_string_consby_ref(_string, _length), NULL
    );
}

inline void ist_codepage_delete(ist_codepage* this) {
    isl_ifnreport(this, rid_catch_nullptr, isp_catch_coreloc);
    isl_free(this);
}
inline void ist_codepage_delete_chain(ist_codepage* this) {
    isl_ifnreport(this, rid_catch_nullptr, isp_catch_coreloc);
    if (this->prev_page) ist_codepage_delete_chain(this->prev_page);
    isl_free(this);
}
