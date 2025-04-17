

#define ISL_CONCAT_IMPL(_a, _b) _a##_b
#define ISL_CONCAT(_a, _b)      ISL_CONCAT_IMPL(_a, _b)
#define ISL_TEST_FN_NAME        ISL_CONCAT(isl_test_, ISL_TEST_UNIT_NAME)