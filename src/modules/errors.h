#pragma once
#include "def.h"

#define NON_SIGNAL_PREFIX 128

enum struct Errors : byte{ // May add others just in case
    SIGABRT             =   6,
    SIGSEGV             =   11,
    // Non-signal errors (1-128)
    OS_UNSUPPORTED      =   NON_SIGNAL_PREFIX + 1,
    NO_FILE_ARG         =   NON_SIGNAL_PREFIX + 2,
    FILE_NOT_FOUND      =   NON_SIGNAL_PREFIX + 3,
    FILE_TOO_BIG        =   NON_SIGNAL_PREFIX + 4,
    ERROR_OPENING_FILE  =   NON_SIGNAL_PREFIX + 5,
};

void raise(Errors code);