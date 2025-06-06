#pragma once

#include "arena.h"
#include "str.h"

typedef struct {
    isize len;
    i32*  data;
} Ai32;

// Create from const array
#define Ai32(N, ...)                                                                                                         \
    (Ai32) { N, (i32*){(i32[N]){__VA_ARGS__}} }

Ai32 ai32_new(isize len, Arena* perm);

Str ai32_print(Ai32* a, Arena* perm);
