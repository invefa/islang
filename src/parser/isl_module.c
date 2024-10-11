#include "isl_module.h"

#include "isl_memgr.h"
#include "isl_list.h"

inline ist_string isl_filename_catchby_filepath(ist_string _filepath) {

    /* find the start of the filename, capable of both unix and windows */
    ist_string start_unix = strrchr(_filepath, '/');
    ist_string start_wins = strrchr(_filepath, '\\');

    /* judge where is the start of the filename */
    ist_string start =
        (start_unix && start_wins) ?
        (start_unix > start_wins ? start_unix : start_wins) :
        (start_unix ? start_unix + 1 : (start_wins ? start_wins + 1 : _filepath));

    /* find and judge the end of the filename */
    ist_string end = strrchr(start, '.');
    if (!end) end = _filepath + strlen(_filepath);

    return ist_string_consby_ref(start, end - start);

}


inline ist_module ist_module_consby_full(ist_string _name, ist_string _filepath) {
    ist_module module = (ist_module){
        .name = _name,
        .filepath = _filepath,
        .strbuf_list = isl_malloc_list(ist_string, 4),
        .srcidx_list = isl_malloc_list(ist_usize, 2),
        .strbuf_count = 0,
        .srcidx_count = 0,
    };
    ist_module_register_strbuf(&module, _name);
    ist_module_register_strbuf(&module, _filepath);
    return module;
}
inline ist_module* ist_module_initby_full(ist_module* this, ist_string _name, ist_string _filepath) {
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
    if (this->strbuf_list) {
        for (ist_usize i = 0; i < this->strbuf_count; ++i)
            if (this->strbuf_list[i]) {
                ist_string_clean(this->strbuf_list + i);
            } else continue;
    }

    isl_freev_list(this->strbuf_list);
    isl_freev_list(this->srcidx_list);

    this->name = NULL;
    this->filepath = NULL;

    this->strbuf_count = 0;
    this->srcidx_count = 0;

}
inline void ist_module_delete(ist_module* this) {
    ist_module_clean(this);
    isl_free(this);
}


inline ist_usize ist_module_register_strbuf(ist_module* this, ist_string _strbuf) {

    for (ist_usize i = 0; i < this->strbuf_count; ++i)
        if (this->strbuf_list[i] == _strbuf) return i;

    isl_list_addm(this->strbuf_list, this->strbuf_count, _strbuf);

    return this->strbuf_count - 1;
}
ist_usize ist_module_register_source(ist_module* this, ist_string _source) {
    for (ist_usize i = 0; i < this->srcidx_count; ++i)
        if (this->strbuf_list[this->srcidx_list[i]] == _source)
            return this->srcidx_list[i];

    isl_list_addm(this->srcidx_list, this->srcidx_count, this->strbuf_count);
    isl_list_addm(this->strbuf_list, this->strbuf_count, _source);

    return this->strbuf_count - 1;
}

