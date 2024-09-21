#include "isl_report.h"

#include <stdlib.h>

#include "isl_ansictrl.h"
#include "isl_string.h"

const ist_string isp_fmts[] = {
#   define manifest(_name, _reploc, _fmt) _fmt,
#   include "isl_repids.h"
#   undef manifest
};

const ist_string isp_repid_names[] = {
#   define manifest(_name, _reploc, _fmt) #_name,
#   include "isl_repids.h"
#   undef manifest
};

const isp_replocation isp_replocs[] = {
#   define manifest(_name, _reploc, _fmt) isp_gen_reploc _reploc,
#   include "isl_repids.h"
#   undef manifest
};

ist_string level_fmts[] = {
    ANSI_GRE_SET("info"),
    ANSI_HIL_SET("reserved"),
    ANSI_YEL_SET("warning"),
    ANSI_HIL_SET("reserved"),
    ANSI_HIR_SET("error"),
    ANSI_MAG_SET("panic"),
    ANSI_RED_SET("fatal"),
    ANSI_HIL_SET("reserved"),
};

ist_string domain_fmts[] = {
    "core",
    "lexer",
    "parser",
    "compiler",
    "vm",
};


void isl_report(isp_repid _rid, ...) {
    typedef FILE* ist_iostream;

    isp_replocation reploc = isp_replocs[_rid];
    ist_string      fmt = isp_fmts[_rid];

    ist_iostream output_stream = reploc.level >= ISP_LEVEL_ERROR ? stderr : stdout;

    va_list vargs;
    va_start(vargs, _rid);

    char buffer[4096] = {0};

    if (reploc.attribute == ISP_ATTR_CUSTOM) {
        ist_string custom_fmt = va_arg(vargs, ist_string);

        sprintf(buffer, "%s %s: %s\n",
                domain_fmts[reploc.domain],
                level_fmts[reploc.level],
                custom_fmt);

        goto isl_report_label_ending;
    }

    sprintf(buffer, "%s %s: %s\n",
            domain_fmts[reploc.domain],
            level_fmts[reploc.level],
            fmt);

isl_report_label_ending:

    vfprintf(output_stream, buffer, vargs);

    va_end(vargs);
    if (reploc.level >= ISP_LEVEL_FATAL) exit(_rid);
}
