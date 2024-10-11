#include "isl_module.h"

#include "isl_memgr.h"
#include "isl_list.h"

inline ist_string isl_filename_catchby_filepath(ist_string _filepath) {

    ist_string start_unix = strrchr(_filepath, '/');
    ist_string start_wins = strrchr(_filepath, '\\');

    ist_string start =
        (start_unix && start_wins) ?
        (start_unix > start_wins ? start_unix : start_wins) :
        (start_unix ? start_unix + 1 : (start_wins ? start_wins + 1 : _filepath));

    ist_string end = strrchr(start, '.');
    if (!end) end = _filepath + strlen(_filepath);

    return ist_string_consby_ref(start, end - start);
}


inline ist_module ist_module_consby_name(ist_string _name) {
    ist_module module = (ist_module){
        .name = _name,
        .strbuf_list = isl_malloc_list(ist_string, 2),
        .srcidx_list = isl_malloc_list(ist_usize, 2),
        .strbuf_count = 0,
        .srcidx_count = 0,
    };
    ist_module_register_strbuf(&module, _name);
    return module;
}
inline ist_module* ist_module_initby_name(ist_module* this, ist_string _name) {
    *this = ist_module_consby_name(_name);
    return this;
}
inline ist_module* ist_module_createby_name(ist_string _name) {
    return ist_module_initby_name(isl_malloc(ist_module), _name);
}


inline ist_module ist_module_consby_filepath(ist_string _filepath) {
    return ist_module_consby_name(isl_filename_catchby_filepath(_filepath));
}
inline ist_module* ist_module_initby_filepath(ist_module* this, ist_string _filepath) {
    return ist_module_initby_name(this, isl_filename_catchby_filepath(_filepath));
}
inline ist_module* ist_module_createby_filepath(ist_string _filepath) {
    return ist_module_createby_name(isl_filename_catchby_filepath(_filepath));
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
}
inline void ist_module_delete(ist_module* this) {
    ist_module_clean(this);
    isl_free(this);
}


inline ist_usize ist_module_register_strbuf(ist_module* this, ist_string _strbuf) {

    for (ist_usize i = 0; i < this->strbuf_count; ++i)
        if (this->strbuf_list[i] == _strbuf) return i;

    isl_list_ensurem(this->strbuf_list, this->strbuf_count, 1);
    this->strbuf_list[this->strbuf_count++] = _strbuf;
    return this->strbuf_count - 1;
}
ist_usize ist_module_register_source(ist_module* this, ist_string _source) {
    for (ist_usize i = 0; i < this->srcidx_count; ++i)
        if (this->strbuf_list[this->srcidx_list[i]] == _source)
            return this->srcidx_list[i];

    isl_list_ensurem(this->srcidx_list, this->srcidx_count, 1);
    this->srcidx_list[this->srcidx_count++] = this->strbuf_count;

    isl_list_ensurem(this->strbuf_list, this->strbuf_count, 1);
    this->strbuf_list[this->strbuf_count++] = _source;
    return this->strbuf_count - 1;
}

