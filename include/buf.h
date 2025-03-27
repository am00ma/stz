#pragma once

#include "arena.h" // Arena
#include "str.h"   // Str
#include "types.h" // isize

typedef struct {
    char* buf;
    isize len;
    isize cap;
} Buf;

Buf buf_new(isize cap, Arena* a);
Str buf_join(Buf* b, Str c);
Str buf_final(Buf* b, Arena* a); // Shrinks arena
