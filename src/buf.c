#include "buf.h"
#include "abort.h"
#include <string.h> // memcpy

Buf buf_new(isize cap, Arena* a) { return (Buf){.buf = new (a, char, cap), .len = 0, .cap = cap}; }

Str buf_final(Buf* b, Arena* a)
{
    a->beg -= b->cap - b->len;
    b->cap  = b->len;
    return (Str){.buf = b->buf, .len = b->len};
}

Str buf_join(Buf* b, Str c)
{
    if (b->len + c.len >= b->cap) oom(); // Straight out oom, can add flags later

    memcpy(&b->buf[b->len], c.buf, c.len);
    b->len += c.len;
    return (Str){.buf = b->buf, .len = b->len};
}
