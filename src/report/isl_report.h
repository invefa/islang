#ifndef ISL_REPORT_H
#define ISL_REPORT_H

/*
    report system was the important part of islang.
    It is used to report some information to the user.
    And the report was divided into 5 levels:
        1. info:    it was used to notify user some thing is going on, and supplied with some information.
        2. warning: used to notify the user that something should not happen and needs to be fixed.
        3. error:   notify user some thing can't be done.
        4. panic:   something is about to crash the program.
        5. fatal:   hell word, crash program.
    Written by invefa.
*/

#include <stdio.h>
#include <stdarg.h>

#include "isl_string.h"

enum {
    INFO, WARNING, ERROR, PANIC, FATAL,
    CORE, LEXER, PARSER, COMPILER, VM,
    NONE, CUSTOM, LOCATION,
};

typedef enum isp_level {
    ISP_LEVEL_INFO = 0,
    ISP_LEVEL_WARNING = 2,
    ISP_LEVEL_ERROR = 4,
    ISP_LEVEL_PANIC = 5,
    ISP_LEVEL_FATAL = 6
} isp_level;

typedef enum isp_domain {
    ISP_DOMAIN_CORE,
    ISP_DOMAIN_LEXER,
    ISP_DOMAIN_PARSER,
    ISP_DOMAIN_COMPILER,
    ISP_DOMAIN_VM
} isp_domain;

typedef enum isp_attribute {
    ISP_ATTR_NONE,
    ISP_ATTR_CUSTOM,
    ISP_ATTR_LOCATION,
} isp_attribute;

typedef struct isp_replocation {
    isp_level       level : 3;
    isp_domain      domain : 3;
    isp_attribute   attribute : 2;
} isp_replocation;

#define isp_reploc(_level, _domain, _attr)  \
    ((isp_replocation)                      \
        {                                   \
        .level=ISP_LEVEL_##_level ,         \
        .domain=ISP_DOMAIN_##_domain ,      \
        .attribute=ISP_ATTR_##_attr         \
        })


/* typedef struct isp_repcode {
    ist_u32    repid : 24;
    ISP_LEVEL  level : 3;
    isp_domain domain : 3;
    ist_bool   do_abort : 1;
} isp_repcode;

typedef ist_u32 isp_repcode_raw;

#define isp_repcode_cast_raw(_repcode) \
    ((isp_repcode_raw)((_repcode).level << 29 | (_repcode).domain << 26 | (_repcode).do_abort << 24 | (_repcode).repid))

#define isp_raw_repcode_cast(_rawcode)          \
    ((isp_repcode)                              \
        {                                       \
            .repid = (_rawcode) & 0xFFFFFF,     \
            .level = (_rawcode) >> 29 & 0x7,    \
            .domain = (_rawcode) >> 26 & 0x7,   \
            .do_abort = (_rawcode) >> 24 & 0x1  \
        }                                       \
    ) */

extern const ist_string         isp_fmts[];
extern const ist_string         isp_repid_names[];
extern const isp_replocation    isp_replocs[];

typedef enum isp_repid {
#   define manifest(_name, _reploc, _fmt) rid_##_name,
#   include "isl_repids.h"
#   undef manifest
} isp_repid;

void isl_report(isp_repid _rid, ...);

#endif