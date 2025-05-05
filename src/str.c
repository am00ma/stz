#include "str.h"    // str
#include <assert.h> // assert for delimiter
#include <stdarg.h> // va_list, va_start, va_end
#include <stdio.h>  // vsnprintf
#include <string.h> // strlen, memcpy, memcmp

/* ---------------------------------------------------------------------------
 * Lifetimes
 * ------------------------------------------------------------------------- */

// Allocate space for a new string
Str str_new(isize len, Arena* a) { return (Str){.buf = new (a, char, len), .len = len}; }

// Store null-terminated string in arena
Str str_store(char* s, Arena* a)
{
    Str c = {0};
    c.len = strlen(s); // Keep str semantics (dont count '\0')
    // Null terminated on arena (memset 0 sets '\0' as flags is 0)
    c.buf = new (a, char, c.len + 1);
    if (c.len) memcpy(c.buf, s, c.len);
    return c;
}

Str str_copy(Str src, Arena* a)
{
    Str dst = {.buf = new (a, char, src.len), .len = src.len};
    if (src.len) memcpy(dst.buf, src.buf, src.len);
    return dst;
}

/* ---------------------------------------------------------------------------
 * Formatting
 * ------------------------------------------------------------------------- */

// Get c style null terminated string of maybe unterminated str
char* str_c(Str s, Arena* a)
{
    // Null terminated on arena (memset 0 sets '\0' as flags is 0)
    char* c = new (a, char, s.len + 1);
    if (s.len) memcpy(c, s.buf, s.len);
    return c;
}

// Store formatted string in the arena (modifies arena to reclaim unused space)
Str str_fmt(Arena* a, char const* fmt, ...)
{
    char* beg = a->beg;
    Str   s   = {.buf = new (a, char, STR_MAXLEN)};

    va_list arg;
    va_start(arg, fmt);
    s.len = vsnprintf(s.buf, STR_MAXLEN, fmt, arg);
    va_end(arg);

    a->beg = beg + s.len + 1; // Discard extra and advance
    return s;
}

// Specify custrom STR_MAXLEN
Str str_fmtn(Arena* a, isize len, char const* fmt, ...)
{
    char* beg = a->beg;
    Str   s   = {.buf = new (a, char, len)};

    va_list arg;
    va_start(arg, fmt);
    s.len = vsnprintf(s.buf, len, fmt, arg);
    va_end(arg);

    a->beg = beg + s.len + 1; // Discard extra and advance BUG: May go one byte over capacity
    return s;
}

/* ---------------------------------------------------------------------------
 * Comparizon
 * ------------------------------------------------------------------------- */

bool str_equal(Str s, Str c)
{
    if (c.len != s.len) return false;
    if (!c.len) return true; // Empty strings are equal
    return !memcmp(s.buf, c.buf, c.len);
}

/* ---------------------------------------------------------------------------
 * Hash (FNV from wikipedia)
 * ------------------------------------------------------------------------- */

#define FNV_32_OFFSET_BASIS 2166136261
#define FNV_32_PRIME        16777619

u32 str_hash32(Str s)
{
    u32 h = FNV_32_OFFSET_BASIS;
    for (isize i = 0; i < s.len; i++)
    {
        h ^= s.buf[i] & 255;
        h *= FNV_32_PRIME;
    }
    return h;
}

#define FNV_64_OFFSET_BASIS 0xcbf29ce484222325
#define FNV_64_PRIME        1099511628211

u64 str_hash64(Str s)
{
    u64 h = FNV_64_OFFSET_BASIS;
    for (isize i = 0; i < s.len; i++)
    {
        h ^= s.buf[i] & 255;
        h *= FNV_64_PRIME;
    }
    return h;
}

/* ---------------------------------------------------------------------------
 * Array of strings, useful for operations like extracting lines with no copy
 * ------------------------------------------------------------------------- */

Strs strs_new(isize len, Arena* a) { return (Strs){.len = len, .data = new (a, Str, len)}; }

/* ---------------------------------------------------------------------------
 * String functions that return array of strings
 * ------------------------------------------------------------------------- */

/* Split text into parts on delimiter
 *
 *   Strs str_split(Str text, Str delimiter, bool ignore_empty, bool substitute_null, Arena* a);
 *
 *   Currently supports only single char delimiter
 *   When using `substitute_null = true`, crashes if text.buf is string literal
 * */
Strs str_split(Str text, Str delimiter, bool ignore_empty, bool substitute_null, Arena* a)
{
    // TODO: implement for len > 1
    assert(delimiter.len == 1);

    // Start position
    char* start = &text.buf[0];

    // Alloc dynamically (no other user/variable on arena)
    Strs parts = {.data = new (a, Str, 0)};
    for (int i = 0; i < text.len; i++)
    {
        if (text.buf[i] == delimiter.buf[0])
        {
            isize len = &text.buf[i] - start;
            if (len || !ignore_empty)
            {
                new (a, Str); // Extending arena and throwing away ref (exit on oom)
                parts.data[parts.len] = (Str){.buf = start, .len = len};
                parts.len++;
            }

            // Trick like strtok to get char**
            if (substitute_null) text.buf[i] = '\0';

            // Skip newline
            start = &text.buf[i] + 1;
        }
    }

    // Last line
    if ((isize)(start - text.buf) <= text.len)
    {
        isize len = text.len - (start - text.buf);
        if (len || !ignore_empty)
        {
            new (a, Str);
            parts.data[parts.len] = (Str){.buf = start, .len = text.len - (start - text.buf)};
            parts.len++;
        }
    }

    return parts;
}
