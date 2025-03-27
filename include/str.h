#pragma once

#include "arena.h"
#include "types.h"

typedef struct {
    char* buf;
    isize len;
} str;

// Create new null-terminated string from literal
#define str(s)                                                                                                         \
    (str) { s, lengthof(s) }

// For printing %.*s
#define pstr(s) (int)s.len, s.buf

// Allocate space for a new string
str str_new(isize len, Arena* a);

// Store null-terminated string in arena
str str_store(char* s, Arena* a);

// Copy string
str str_copy(str src, Arena* a);

// Get c style null terminated string of maybe unterminated str
char* str_c(str s, Arena* a);

// Store formatted string in the arena (modifies arena to reclaim unused space)
str str_fmt(Arena* a, char const* fmt, ...);

constexpr int STR_MAXLEN = 1024;

// Check if equal to string literal
bool str_equal(str* s, char* c);
