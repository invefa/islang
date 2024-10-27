#include "isl_module.h"


#define ISG_VALUE_TYPE            ist_strbuf_entry
#define ISG_VALUE_FN_CLEAN(_entp) ist_string_clean(&(_entp)->buffer)
#include "isg_list_code.h"

#define ISG_VALUE_TYPE ist_module
#include "isg_list_code.h"

inline ist_string isl_filename_catchby_filepath(ist_cstring _filepath) {

    /* find the start of the filename, capable of both unix and windows */
    ist_string start_unix = strrchr(_filepath, '/');
    ist_string start_wins = strrchr(_filepath, '\\');

    /* judge where is the start of the filename */
    ist_string start = (start_unix && start_wins)
                         ? (start_unix > start_wins ? start_unix : start_wins)
                         : (start_unix ? start_unix + 1 : (start_wins ? start_wins + 1 : _filepath)
                           );

    /* find and judge the end of the filename */
    ist_string end = strrchr(start, '.');
    if (!end) end = _filepath + strlen(_filepath);

    return ist_string_consby_ref(start, end - start);
}


inline ist_module ist_module_consby_full(ist_string _name, ist_string _filepath) {
    ist_module module = (ist_module){
        .name              = _name,
        .filepath          = _filepath,
        .strbuf_entry_list = ist_strbuf_entry_list_consm(4),
    };
    ist_module_register_strbuf(&module, _filepath, ISL_STRBUFT_FILEPATH);
    ist_module_register_strbuf(&module, _name, ISL_STRBUFT_NAME);
    return module;
}
inline ist_module* ist_module_initby_full(
    ist_module* this,
    ist_string _name,
    ist_string _filepath
) {
    *this = ist_module_consby_full(_name, _filepath);
    return this;
}
inline ist_module* ist_module_createby_full(ist_string _name, ist_string _filepath) {
    return ist_module_initby_full(isl_malloc(ist_module), _name, _filepath);
}


inline ist_module ist_module_consby_filepath(ist_string _filepath) {
    return ist_module_consby_full(isl_filename_catchby_filepath(_filepath), _filepath);
}
inline ist_module* ist_module_initby_filepath(ist_module* this, ist_string _filepath) {
    return ist_module_initby_full(this, isl_filename_catchby_filepath(_filepath), _filepath);
}
inline ist_module* ist_module_createby_filepath(ist_string _filepath) {
    return ist_module_createby_full(isl_filename_catchby_filepath(_filepath), _filepath);
}


inline void ist_module_clean(ist_module* this) {
    isl_assert(this);
    ist_strbuf_entry_list_clean(&this->strbuf_entry_list);
    this->name     = NULL;
    this->filepath = NULL;
}
inline void ist_module_delete(ist_module* this) {
    ist_module_clean(this);
    isl_free(this);
}


inline ist_usize ist_module_register_strbuf(
    ist_module* this,
    ist_string _strbuf,
    ist_sbtype _type
) {
    if (!_strbuf) return this->strbuf_entry_list.size;

    isg_list_foreach (itp, this->strbuf_entry_list, idx)
        if (itp->buffer == _strbuf) return itp->type = _type, idx;

    return ist_strbuf_entry_list_addm(
        &this->strbuf_entry_list, (ist_strbuf_entry){.type = _type, .buffer = _strbuf}
    );
}

ist_string ist_module_dump_json(ist_module* this, ist_string* buffer, ist_usize* idxptr) {
    isl_ifnreport(this, rid_catch_nullptr, isp_catch_coreloc);
    isl_report(rid_inform_dumping, "module", this);
    idxptr = idxptr ?: (ist_usize[1]){};

    ist_strbuf_sprintf(
        buffer,
        idxptr,
        "{\"name\":\"%s\",\"filepath\":\"%s\",\"strbufs\":[",
        this->name,
        this->filepath
    );

    static ist_cstring isl_strbuf_type_names[] =
        {[ISL_STRBUFT_UNKNOWN]  = "unknown",
         [ISL_STRBUFT_SYMBOL]   = "symbol",
         [ISL_STRBUFT_SOURCE]   = "source",
         [ISL_STRBUFT_NAME]     = "name",
         [ISL_STRBUFT_FILEPATH] = "filepath",
         [ISL_STRBUFT_LITERAL]  = "literal"};

    isg_list_foreach (entp, this->strbuf_entry_list) {
        ist_strbuf_sprintf(
            buffer,
            idxptr,
            "{\"type\":\"%s\",\"buffer\":\"%s\"},",
            isl_strbuf_type_names[entp->type],
            entp->buffer
        );
    }

    return ist_strbuf_append_raw(buffer, idxptr, "\b]}");
}
