#ifndef ISC_REPORT_H
#define ISC_REPORT_H

/*
    report system was the important part of islang.
    It is used to report some information to the user.
    And the report was divided into 5 levels:
        1. info:    it was used to notify user some thing is going on,
                    and supplied with someinformation.
        2. warning: used to notify the user that something should not
                    happen and needs to be fixed.
        3. error:   notify user some thing can't be done.
        4. panic:   something is about to crash the program.
        5. fatal:   hell word, crash program.
    Written by invefa.
*/

#include <stdarg.h>
#include <stdio.h>


#include "isl_string.h"


/*
    This enumeration type has no use except to provide code highlighting for isl_repids.h
    So you can ignore it.
*/
enum {
    INFO,
    WARNING,
    ERROR,
    PANIC,
    FATAL,
    CORE,
    LEXER,
    PARSER,
    COMPILER,
    VM,
    NONE,
    CUSTOM,
    CORELOC,
    USERLOC,
};


/*
    This enumeration type provides the level of the repid, and match a number for it.
    It indicates the severity of the report.
*/
typedef enum isp_level {
    ISP_LEVEL_INFO    = 0,
    ISP_LEVEL_WARNING = 2,
    ISP_LEVEL_ERROR   = 4,
    ISP_LEVEL_PANIC   = 5,
    ISP_LEVEL_FATAL   = 6
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
    CORELOC:    signed the core code exact location will be provided, it consumes 3 args. (core code
   location) USERLOC:    signed the user code file location will be provided by user at the second
   arg of isl_report. (user file location)
*/
typedef enum isp_attribute {
    ISP_ATTR_NONE,
    ISP_ATTR_CUSTOM,
    ISP_ATTR_CORELOC,
    ISP_ATTR_USERLOC,
} isp_attribute;

/*
    This structure contains the information about the repid.
    It describes who the specified repid is written for and what features it has.
    So I call it replocation, meaning the location of the repid.
*/
typedef struct isp_replocation {
    isp_level     level    : 4;
    isp_domain    domain   : 4;
    isp_attribute attribute: 24;
} isp_replocation;

/* generate a replocation for repid */
#define isp_gen_reploc(_domain, _level, _attr) \
    ((isp_replocation                          \
    ){.level = ISP_LEVEL_##_level, .domain = ISP_DOMAIN_##_domain, .attribute = ISP_ATTR_##_attr})


/* there are some check table for repid */
extern const ist_string      isp_fmts[];
extern const ist_string      isp_repid_names[];
extern const isp_replocation isp_replocs[];

/* the one who is served */
typedef enum isp_repid {
#define manifest(_name, _reploc, _fmt) rid_##_name,
#include "isl_repids.h"
#undef manifest
} isp_repid;


/* the only function of the reporting system */
void isl_report(isp_repid _rid, ...);


/* if expr is false, then report */
#define isl_ifnreport(_expr, _vargs...)   \
    do {                                  \
        if (!(_expr)) isl_report(_vargs); \
    } while (0)


/* if expr is true, then report */
#define isl_ifreport(_expr, _vargs...) \
    do {                               \
        if (_expr) isl_report(_vargs); \
    } while (0)


/* catch the core location for attribute:CORELOC of report */
#define isp_catch_coreloc __FILE__, __func__, (ist_usize)__LINE__

#endif // ISC_REPORT_H