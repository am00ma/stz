#pragma once

#include "types.h"

constexpr isize CAPACITY = 64 * 1024 * 1024; // 64 MB

typedef struct {
    char* beg;
    char* end;
    isize cap; // Useful for debugging
} Arena;

typedef enum {
    NOZERO   = 0x1,
    SOFTFAIL = 0x2,
} ArenaFlags;

Arena arena_new(isize cap);
char* arena_alloc(Arena* a, isize objsize, isize align, isize count, int flags);
void  arena_print(char* title, Arena* a); // Only function that uses `cap`

#define new(...)                 newx(__VA_ARGS__, new4, new3, new2)(__VA_ARGS__)
#define newx(a, b, c, d, e, ...) e
#define new2(a, t)               (t*)arena_alloc(a, sizeof(t), alignof(t), 1, 0)
#define new3(a, t, n)            (t*)arena_alloc(a, sizeof(t), alignof(t), n, 0)
#define new4(a, t, n, f)         (t*)arena_alloc(a, sizeof(t), alignof(t), n, f)
