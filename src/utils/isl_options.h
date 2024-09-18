#ifndef ISL_OPTIONS_H
#define ISL_OPTIONS_H


#ifdef _DEBUG
/*
    This macro is used to enable debug mode.
    If you want to disable debug mode, just comment out this macro
    and now it will be control by the definition of _DEBUG which is defined by makefile.
*/
#define ISL_DEBUG_MODE
#endif


#ifdef ISL_DEBUG_MODE
/*
    This macro is used to enable xssert which is the total name of assert and wssert (weak assert),
    xssert is used in debug mode to detect the error of the core code of islang.
*/
#define ISL_ENABLE_XSSERT
#endif


#endif