/* Growing two arrays dynamically
 *
 * TODO: Can we avoid copy, yet keep dynamic nature?
 * */

#include "arena.h"
#include "range.h"
#include "types.h"
#include <stdio.h>
#include <string.h>

typedef struct {
    char* buf;
    isize len;
} str;

typedef struct {
    str a;
    str b;
} pair;

void str_copy(str* dst, str src)
{
    if (dst->len != src.len) return;
    memcpy(dst->buf, src.buf, dst->len);
}

constexpr isize MAXLEN = 256;

pair grow_arrays_with_temp(Arena temp, Arena* perm)
{
    // Parse on temp
    pair p = {0};
    str  a = {.buf = new (&temp, char, MAXLEN), .len = 0};
    str  b = {.buf = new (&temp, char, MAXLEN), .len = 0};
    RANGE(i, 10)
    {
        a.buf[i] = 65 + i;
        b.buf[i] = 75 + i;
        a.len++;
        b.len++;
    }

    // Copy to perm
    p.a = (str){.buf = new (perm, char, a.len), .len = a.len};
    p.b = (str){.buf = new (perm, char, b.len), .len = b.len};
    str_copy(&p.a, a);
    str_copy(&p.b, b);

    return p;
}

pair grow_arrays(Arena* perm)
{
    pair p = {0};

    p.a = (str){.buf = new (perm, char, MAXLEN), .len = 0};
    p.b = (str){.buf = new (perm, char, MAXLEN), .len = 0};

    RANGE(i, 10)
    {
        p.a.buf[i] = 65 + i;
        p.b.buf[i] = 75 + i;
        p.a.len++;
        p.b.len++;
    }

    return p;
}

/* Using ref as this is now twice as heavy as str for copy */
void pair_print(pair* p)
{
    printf("a: %.*s\n", (int)p->a.len, p->a.buf);
    printf("b: %.*s\n", (int)p->b.len, p->b.buf);
}

int main()
{
    // Create arena
    Arena perm1 = arena_new(1024);
    Arena perm2 = arena_new(1024);
    Arena temp  = arena_new(1024);

    pair p = {0};

    p = grow_arrays(&perm1);
    pair_print(&p);
    arena_print("Only perm: perm", &perm1);

    p = grow_arrays_with_temp(temp, &perm2);
    pair_print(&p);
    arena_print("With both: perm: ", &perm2);
    arena_print("With both: temp: ", &temp);

    return 0;
}
