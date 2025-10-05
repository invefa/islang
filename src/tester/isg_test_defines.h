
/* concat with underline */
#define ISG_UCAT_IMPL(_a, _b) _a##_##_b
#define ISG_UCAT(_a, _b)      ISG_UCAT_IMPL(_a, _b)
#define ISG_STRING_IMPL(_sth) #_sth
#define ISG_STRING(_sth)      ISG_STRING_IMPL(_sth)


#define ISL_TEST_HEAD                                                    \
    void ISG_UCAT(isl_test, ISL_TEST_ITEM)(void) {                       \
        isl_report(rid_inform_start_testing, ISG_STRING(ISL_TEST_ITEM)); \
        isl_max_allocated_length = 0;

#define ISL_TEST_TAIL                                                                          \
    isl_report(rid_custom_core_info, "max allocated-length = %zu.", isl_max_allocated_length); \
    isl_report(rid_inform_end_testing, ISG_STRING(ISL_TEST_ITEM));                             \
    }
