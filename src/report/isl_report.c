#include "isl_report.h"

#include <stdlib.h>

#include "isl_ansictrl.h"
#include "isl_location.h"
#include "isl_module.h"
#include "isl_types.h"


ist_u8 isp_report_option = ISP_ROPTM_NONE;


const ist_cstring isp_fmts[] = {
#define manifest(_name, _reploc, _fmt) [rid_##_name] = _fmt,
#include "isl_repids.h"
#undef manifest
};

const ist_cstring isp_repid_names[] = {
#define manifest(_name, _reploc, _fmt) [rid_##_name] = #_name,
#include "isl_repids.h"
#undef manifest
};

const isp_replocation isp_replocs[] = {
#define manifest(_name, _reploc, _fmt) [rid_##_name] = isp_gen_reploc _reploc,
#include "isl_repids.h"
#undef manifest
};

ist_cstring level_fmts[] = {
    [ISP_LEVEL_INFO]    = "info",
    [ISP_LEVEL_NOTE]    = "note",
    [ISP_LEVEL_WARNING] = "warn",
    [ISP_LEVEL_ERROR]   = "error",
    [ISP_LEVEL_PANIC]   = "panic",
    [ISP_LEVEL_FATAL]   = "fatal",
};

ist_cstring level_colors[] = {
    [ISP_LEVEL_INFO]    = ANSI_GRE,
    [ISP_LEVEL_NOTE]    = ANSI_CLY,
    [ISP_LEVEL_WARNING] = ANSI_YEL,
    [ISP_LEVEL_ERROR]   = ANSI_HIR,
    [ISP_LEVEL_PANIC]   = ANSI_MAG,
    [ISP_LEVEL_FATAL]   = ANSI_RED,
};

ist_cstring domain_fmts[] = {
    [ISP_DOMAIN_CORE]     = "core",
    [ISP_DOMAIN_LEXER]    = "lexing",
    [ISP_DOMAIN_PARSER]   = "syntax",
    [ISP_DOMAIN_COMPILER] = "semantic",
    [ISP_DOMAIN_VM]       = "runtime",
};


#define ISP_BUFFER_SIZE 4096

void isl_report(isp_repid rid, ...) {

    isp_replocation reploc = isp_replocs[rid];

    if (isp_report_option & ISP_ROPTM_NO_CORE_INFO)
        if (reploc.domain == ISP_DOMAIN_CORE)
            if (reploc.level == ISP_LEVEL_INFO) return;
    if (isp_report_option & ISP_ROPTM_NO_NOTE)
        if (reploc.level == ISP_LEVEL_NOTE) return;
    if (isp_report_option & ISP_ROPTM_NO_WARN)
        if (reploc.level == ISP_LEVEL_WARNING) return;

    va_list vargs;
    va_start(vargs, rid);

    /**
     * using the c-array buffer, instead strbuf which
     * come from islang to avoid recursive reoport.
     */
    char buffer[ISP_BUFFER_SIZE] = {0};

    /* if reploc.attribute == ISP_ATTR_CUSTOM, then the fmt will be provided by user */
    if (reploc.attribute == ISP_ATTR_CUSTOM) {
        ist_cstring custom_fmt = va_arg(vargs, ist_cstring);

        snprintf(
            buffer,
            ISP_BUFFER_SIZE,
            "%s%s %s: %s" ANSI_RST "\n",
            level_colors[reploc.level],
            domain_fmts[reploc.domain],
            level_fmts[reploc.level],
            custom_fmt
        );

    }

    /**
     * if reploc.attribute == ISP_ATTR_CORELOC, then we will obtain the core location to report
     * when the compile mode was release, the core location will not show at reporting message.
     */
    else if (reploc.attribute == ISP_ATTR_CORELOC)
    {

#ifdef ISL_DEBUG
        ist_cstring file_name = va_arg(vargs, ist_cstring);
        ist_cstring func_name = va_arg(vargs, ist_cstring);
        ist_usize   line      = va_arg(vargs, ist_usize);
#else
        va_arg(vargs, void*);
#endif
        snprintf(
            buffer,
            ISP_BUFFER_SIZE,
            "%s%s %s:"
#ifdef ISL_DEBUG
            "\n"
            "\tin file '%s':\n"
            "\tat fn %s(...) <line:%zu>:\n"
#else
            " "
#endif
            "%s" ANSI_RST "\n",
            level_colors[reploc.level],
            domain_fmts[reploc.domain],
            level_fmts[reploc.level],
#ifdef ISL_DEBUG
            file_name,
            func_name,
            line,
#endif
            isp_fmts[rid]
        );

    }

    /* if reploc.attribute == ISP_ATTR_USERLOC, then we will obtain the userfile location to report
     */
    else if (reploc.attribute == ISP_ATTR_USERLOC)
    {
        ist_location location = va_arg(vargs, ist_location);

        snprintf(
            buffer,
            ISP_BUFFER_SIZE,
            "%s%s %s:" ANSI_RST " in module %s:%s<%zu:%zu>: %s\n",
            level_colors[reploc.level],
            domain_fmts[reploc.domain],
            level_fmts[reploc.level],
            location.module->name,
            location.pagename ?: (ist_cstring) "\b",
            location.line,
            location.column,
            isp_fmts[rid]
        );

    }

    /* default method for fmts concatenation */
    else
    {
        snprintf(
            buffer,
            ISP_BUFFER_SIZE,
            "%s%s %s: %s" ANSI_RST "\n",
            level_colors[reploc.level],
            domain_fmts[reploc.domain],
            level_fmts[reploc.level],
            isp_fmts[rid]
        );
    }

    /* write to output stream, this is the core of the report */
    vfprintf(reploc.level >= ISP_LEVEL_ERROR ? stderr : stdout, buffer, vargs);

    va_end(vargs);
    if (reploc.level >= ISP_LEVEL_FATAL) exit(rid);
}
