#pragma once

#include "arena.h"
#include "str.h"

typedef enum {
    CMD_SUCCESS = 0,
    CMD_FAIL_OOM,
    CMD_FAIL_PIPE,
    CMD_FAIL_FORK,
    CMD_FAIL_UNKNOWN,
} ShellError;

typedef struct {
    Str stdout;
    Str stderr;
    int status;

    ShellError err;
} ShellResult;

ShellResult shell_exec(Str command, isize outlen, isize errlen, Arena* perm);
