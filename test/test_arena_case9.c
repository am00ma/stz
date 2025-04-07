#include "range.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char* Arena;

// Global capacity
constexpr isize capacity = 1024;
char*           end      = 0;

typedef enum {
    NOZERO   = 0x1,
    SOFTFAIL = 0x2,
} ArenaFlags;

Arena arena_new(isize cap);
char* arena_alloc(Arena a, isize objsize, isize align, isize count, int flags);
void  arena_print(char* title, Arena a); // Only function that uses `cap`

#define new(...)                 newx(__VA_ARGS__, new4, new3, new2)(__VA_ARGS__)
#define newx(a, b, c, d, e, ...) e
#define new2(a, t)               (t*)arena_alloc(a, sizeof(t), alignof(t), 1, 0)
#define new3(a, t, n)            (t*)arena_alloc(a, sizeof(t), alignof(t), n, 0)
#define new4(a, t, n, f)         (t*)arena_alloc(a, sizeof(t), alignof(t), n, f)

Arena arena_new(isize cap)
{
    char* p = (char*)malloc(cap);
    end     = p + capacity;
    return p;
}

char* arena_alloc(Arena a, isize objsize, isize align, isize count, int flags)
{
    isize pad = -(uptr)a & (align - 1); // TODO: How is this calculated?
    if (count > (end - a - pad) / objsize)
    {
        if (flags & SOFTFAIL) return 0;
        exit(EXIT_FAILURE);
    }

    isize total  = count * objsize;
    char* p      = a + pad;
    a           += pad + total;
    return flags & NOZERO ? p : (char*)memset(p, 0, total);
}

void arena_print(char* title, Arena a)
{
    fprintf(stderr, "%s: %p (beg: %p, end: %p)\n  left: %ld, used: %ld, cap: %ld\n", title, (void*)&a, (void*)a,
            (void*)end, end - a, capacity - (end - a), capacity);
}

int main()
{
    Arena perm = arena_new(capacity);
    arena_print("Init", perm);

    f32* x = new (perm, f32, 10);
    RANGE(i, 10) printf("%d: %.2f\n", i, x[i]);

    perm = new (perm, char, 0);
    arena_print("Final", perm);

    return 0;
}
