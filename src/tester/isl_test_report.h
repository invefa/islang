#include "isl_report.h"

void isl_test_report(void) {

    isl_report(rid_unknown);
    isl_report(rid_custom_core_info, "the second arg was %d.", 123);
    isl_report(rid_custom_core_warn, "the second arg was %d.", 345);
    isl_report(rid_custom_core_error, "the second arg was %d.", 789);
    isl_report(rid_custom_core_panic, "the second arg was %d.", 666);
    // isl_report(rid_custom_core_fatal, "the second arg was %d.", 123456);

    isl_report(rid_utf8_negative_codepoint, isp_catch_coreloc);
    isl_utf8_encode_length(-22);
    isl_report(rid_unknown);
    // isl_ifnreport(NULL, rid_catch_nullptr, isp_catch_coreloc);

    isl_wssert(0);
}
