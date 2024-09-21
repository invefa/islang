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


/*
    This enumeration type has no use except to provide code highlighting for isl_repids.
    So you can ignore it.
*/
enum {
    INFO, WARNING, ERROR, PANIC, FATAL,
    CORE, LEXER, PARSER, COMPILER, VM,
    NONE, CUSTOM, LOCATION,
};


/*
    This enumeration type provides the level of the repid, and match a number for it.
    It indicates the severity of the report.
*/
typedef enum isp_level {
    ISP_LEVEL_INFO = 0,
    ISP_LEVEL_WARNING = 2,
    ISP_LEVEL_ERROR = 4,
    ISP_LEVEL_PANIC = 5,
    ISP_LEVEL_FATAL = 6
} isp_level;

/*
    This enumeration type provides the domain of the repid.
    It indicates the domain of the report.
*/
typedef enum isp_domain {
    ISP_DOMAIN_CORE,
    ISP_DOMAIN_LEXER,
    ISP_DOMAIN_PARSER,
    ISP_DOMAIN_COMPILER,
    ISP_DOMAIN_VM
} isp_domain;


/*
    Attribute for repid.
    NONE:       no attribute.
    CUSTOM:     indicate the fmt of repid will be provided by user.
    LOCATION:   signed the user code file location will be provided
                by user at the second arg of isl_report.
*/
typedef enum isp_attribute {
    ISP_ATTR_NONE,
    ISP_ATTR_CUSTOM,
    ISP_ATTR_LOCATION,
} isp_attribute;

/*
    This structure contains the information about the repid.
    It describes who the specified repid is written for and what features it has.
    So I call it replocation, meaning the location of the repid.
*/
typedef struct isp_replocation {
    isp_level       level : 3;
    isp_domain      domain : 3;
    isp_attribute   attribute : 2;
} isp_replocation;

/* generate a replocation for repid */
#define isp_gen_reploc(_domain, _level, _attr)  \
    ((isp_replocation)                          \
        {                                       \
        .level=ISP_LEVEL_##_level,              \
        .domain=ISP_DOMAIN_##_domain,           \
        .attribute=ISP_ATTR_##_attr             \
        })


/* there are some check table for repid */
extern const ist_string         isp_fmts[];
extern const ist_string         isp_repid_names[];
extern const isp_replocation    isp_replocs[];

/* the one who is served */
typedef enum isp_repid {
#   define manifest(_name, _reploc, _fmt) rid_##_name,
#   include "isl_repids.h"
#   undef manifest
} isp_repid;

/* the only function of the reporting system */
void isl_report(isp_repid _rid, ...);

#endif