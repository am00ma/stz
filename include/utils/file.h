#pragma once

#include "str.h"

typedef enum {
    FILE_SUCCESS = 0,
    FILE_FAIL_OPEN,
    FILE_FAIL_SEEK,
    FILE_FAIL_READ,
    FILE_FAIL_CLOSE,
} FileError;

typedef struct {
    Str       data;
    FileError err;
} StrOK;

StrOK file_read(Str path, Arena* a);
