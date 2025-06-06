#pragma once

#include "arena.h"
#include "str.h"

typedef struct {
    isize len;
    i32*  data;
} Ai32;

Ai32 ai32_new(isize len, Arena* perm);

Str ai32_print(Ai32* a, Arena* perm);
