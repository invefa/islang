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

enum isr_level {
    ISR_LEVEL_INFO = 0,
    ISR_LEVEL_WARNING = 2,
    ISR_LEVEL_ERROR = 4,
    ISR_LEVEL_PANIC = 5,
    ISR_LEVEL_FATAL = 6
};

void isl_report(enum isr_level _level, char* _format, ...) {
    va_list args;
    va_start(args, _format);
    vprintf(_format, args);
    va_end(args);
}

#endif