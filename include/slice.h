#pragma once

#include "arena.h"

typedef struct {
    isize len;
    isize cap;
    i32*  data;
} Si32;

Si32 si32_new(isize cap, Arena* perm);
void si32_push(Si32* s, i32 x);
i32 si32_pop(Si32* s);
