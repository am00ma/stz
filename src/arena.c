#include "arena.h"  // Arena
#include "abort.h"  // oom
#include <stdio.h>  // printf
#include <stdlib.h> // malloc
#include <string.h> // memset

Arena arena_new(isize cap)
{
    Arena a = {0};
    a.beg   = (char*)malloc(cap);
    a.end   = a.beg ? a.beg + cap : 0;
    a.cap   = a.beg ? cap : 0;
    return a;
}

char* arena_alloc(Arena* a, isize objsize, isize align, isize count, int flags)
{
    isize pad = -(uptr)a->beg & (align - 1); // TODO: How is this calculated?
    if (count > (a->end - a->beg - pad) / objsize)
    {
        if (flags & SOFTFAIL) return 0;
        oom();
    }

    isize total  = count * objsize;
    char* p      = a->beg + pad;
    a->beg      += pad + total;
    return flags & NOZERO ? p : (char*)memset(p, 0, total);
}

void arena_print(char* title, Arena* a)
{
    fprintf(stderr, "%s: %p (beg: %p, end: %p)\n  left: %ld, used: %ld, cap: %ld\n", title, (void*)&a, (void*)a->beg,
            (void*)a->end, a->end - a->beg, a->cap - (a->end - a->beg), a->cap);
}
