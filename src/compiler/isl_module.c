#include "isl_dump.h"
#include "isl_module.h"


#define ISG_VALUE_TYPE            ist_mostring
#define ISG_VALUE_FN_CLEAN(_entp) ist_string_clean(&(_entp)->data)
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
        .name          = _name,
        .filepath      = _filepath,
        .mostring_list = ist_mostring_list_consm(4),
    };
    ist_module_register_string(&module, _filepath, ISL_MOSKIND_FILEPATH);
    ist_module_register_string(&module, _name, ISL_MOSKIND_NAME);
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
    ist_mostring_list_clean(&this->mostring_list);
    this->name     = NULL;
    this->filepath = NULL;
}
inline void ist_module_delete(ist_module* this) {
    ist_module_clean(this);
    isl_free(this);
}


inline ist_usize ist_module_register_string(
    ist_module* this,
    ist_string  _strbuf,
    ist_moskind _kind
) {
    if (!_strbuf) return this->mostring_list.size;

    isg_list_foreach (itp, this->mostring_list, idx)
        if (itp->data == _strbuf) return itp->kind = _kind, idx;

    return ist_mostring_list_addm(
        &this->mostring_list, (ist_mostring){.kind = _kind, .data = _strbuf}
    );
}


ist_string __mostring_list_dump_json(ist_strbuf tbuffer, void* mostring_listp) {
    static ist_cstring isl_moskind_names[] = {
        [ISL_MOSKIND_UNKNOWN]   = "unknown",
        [ISL_MOSKIND_IDENTIFER] = "symbol",
        [ISL_MOSKIND_SOURCE]    = "source",
        [ISL_MOSKIND_NAME]      = "name",
        [ISL_MOSKIND_FILEPATH]  = "filepath",
        [ISL_MOSKIND_LITERAL]   = "literal",
    };
    ist_mostring_list* list = mostring_listp;
    ist_usize          idx  = 0;

    isg_list_foreach (mostrp, *list, i) {
        if (i) ist_strbuf_append_raw(tbuffer, &idx, ",");
        dump_json(
            tbuffer,
            &idx,
            (ist_dump_item[]){
                {"kind", &isl_moskind_names[mostrp->kind], JKIND_STRING},
                {"data", &mostrp->data, JKIND_STRING},
            },
            2
        );
    }
    return *tbuffer;
}

ist_string ist_module_dump_json(ist_module* this, ist_strbuf buffer, ist_usize* idxptr) {
    isl_ifnreport(this, rid_catch_nullptr, isp_catch_coreloc);
    isl_dreport(rid_inform_dumping, "module", this);
    idxptr = idxptr ?: (ist_usize[1]){};

    return dump_json(
        buffer,
        idxptr,
        (ist_dump_item[]){
            {"name", &this->name, JKIND_STRING},
            {"filepath", &this->filepath, JKIND_STRING},
            {"mostrings", &this->mostring_list, JKIND_ARRAY, __mostring_list_dump_json},
        },
        3
    );
}
