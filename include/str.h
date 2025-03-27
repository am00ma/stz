#pragma once

#include "arena.h"
#include "types.h"

typedef struct {
    char* buf;
    isize len;
} Str;

// Create new null-terminated string from literal
#define Str(s)                                                                                                         \
    (Str) { s, lengthof(s) }

// For printing %.*s
#define pstr(s) (int)s.len, s.buf

// Allocate space for a new string
Str str_new(isize len, Arena* a);

// Store null-terminated string in arena
Str str_store(char* s, Arena* a);

// Copy string
Str str_copy(Str src, Arena* a);

// Get c style null terminated string of maybe unterminated str
char* str_c(Str s, Arena* a);

// Store formatted string in the arena (modifies arena to reclaim unused space)
Str str_fmt(Arena* a, char const* fmt, ...);

// Used by str_fmt as max allowed string length
constexpr int STR_MAXLEN = 1024;

// Alternative to str_fmt for smaller/bigger strings
Str str_fmtn(Arena* a, isize len, char const* fmt, ...);

// Check if equal to string literal
bool str_equal(Str* s, char* c);
