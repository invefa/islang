#ifndef ISC_OPTIONS_H
#define ISC_OPTIONS_H
/*
    This header is used to control some options of islang core.
    If you want to disable some options, just comment out the corresponding macro.
    Written by invefa.
*/

#ifdef __GNUC__
#if __GNUC__ < 5 || (__GNUC__ == 5 && __GNUC_MINOR__ < 1)
#error "islang requires GCC 5.1 or later!"
#endif // GNUC version check
#else  // __GNUC__
#error "islang requires GCC!"
#endif // __GNUC__


#ifdef _DEBUG

/*
    This macro is used to enable debug mode for islang core.
    If you want to disable debug mode, just comment out this macro
    and now it will be control by the definition of _DEBUG which is defined by makefile.
*/
#define ISL_DEBUG

#endif // _DEBUG


#ifdef ISL_DEBUG

/*
    This macro is used to enable xssert which is the total name of assert and wssert (weak assert),
    xssert is used in debug mode to detect the error of the core code of islang.
*/
#define ISL_ENABLE_XSSERT

/*
    When the assert go false, program will exit with code ISL_ASSERT_FAILURE_CODE.
    It will call exit(ISL_ASSERT_FAILURE_CODE).
*/
#define ISL_ASSERT_FAILURE_CODE 0x24AD6F05

#endif // ISL_DEBUG

#define ISL_ENABLE_INFORM_ALLOCATED
#define ISL_ENABLE_INFORM_RELEASED


#define ISL_DEFAULT_BUFFER_LENGTH 256

#endif // ISC_OPTIONS_H