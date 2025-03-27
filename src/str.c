#include "str.h"    // str
#include <stdarg.h> // va_list, va_start, va_end
#include <stdio.h>  // vsnprintf
#include <string.h> // strlen, memcpy, memcmp

// Allocate space for a new string
str str_new(isize len, Arena* a) { return (str){.buf = new (a, char, len), .len = len}; }

// Store null-terminated string in arena
str str_store(char* s, Arena* a)
{
    str c = {0};
    c.len = strlen(s); // Keep str semantics (dont count '\0')
    // Null terminated on arena (memset 0 sets '\0' as flags is 0)
    c.buf = new (a, char, c.len + 1);
    if (c.len) memcpy(c.buf, s, c.len);
    return c;
}

str str_copy(str src, Arena* a)
{
    str dst = {.buf = new (a, char, src.len), .len = src.len};
    if (src.len) memcpy(dst.buf, src.buf, src.len);
    return dst;
}

// Get c style null terminated string of maybe unterminated str
char* str_c(str s, Arena* a)
{
    // Null terminated on arena (memset 0 sets '\0' as flags is 0)
    char* c = new (a, char, s.len + 1);
    if (s.len) memcpy(c, s.buf, s.len);
    return c;
}

// Store formatted string in the arena (modifies arena to reclaim unused space)
str str_fmt(Arena* a, char const* fmt, ...)
{
    char* beg = a->beg;
    str   s   = {.buf = new (a, char, STR_MAXLEN)};

    va_list arg;
    va_start(arg, fmt);
    s.len = vsnprintf(s.buf, STR_MAXLEN, fmt, arg);
    va_end(arg);

    a->beg = beg + s.len + 1; // Discard extra and advance
    return s;
}

// Check if equal to string literal
bool str_equal(str* s, char* c)
{
    isize len = strlen(c);
    if (len != s->len) return false;
    return !memcmp(s->buf, c, len);
}
