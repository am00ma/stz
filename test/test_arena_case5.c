/* Growing two arrays dynamically
 *
 * TODO: Can we avoid copy, yet keep dynamic nature and not use max sizes?
 *
 * I think we have to use dynamic arrays, i.e. grow by twice, etc.
 *
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

str str_empty(isize len, Arena* a) { return (str){.buf = new (a, char, len)}; }

str str_copy(str src, Arena* a)
{
    if (!src.len) return src;
    str dst = {.buf = new (a, char, src.len), .len = src.len};
    memcpy(dst.buf, src.buf, dst.len);
    return dst;
}

str str_concat(str head, str tail, Arena* a)
{
    if ((char*)(head.buf + head.len) != a->beg) str_copy(head, a);
    str_copy(tail, a);
    return (str){.buf = head.buf, .len = head.len + tail.len};
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
    p.a = str_copy(a, perm);
    p.b = str_copy(a, perm);

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
    Arena perm = arena_new(1024);
    Arena temp = arena_new(1024);

    pair p = {0};

    Arena perm1 = perm;

    p = grow_arrays(&perm1);
    pair_print(&p);
    arena_print("Only perm: perm", &perm1);

    Arena perm2 = perm;

    p = grow_arrays_with_temp(temp, &perm2);
    pair_print(&p);
    arena_print("With both: perm: ", &perm2);
    arena_print("With both: temp: ", &temp);

    return 0;
}
