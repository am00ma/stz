#pragma once

#include "str.h"

/* ---------------------------------------------------------------------------
 * Single token
 * ------------------------------------------------------------------------- */

typedef struct {

    int type;
    Str lexeme;
    Str literal;
    i32 line;

} Token;

Str token_string(Token* t, Arena* perm);

/* ---------------------------------------------------------------------------
 * List of tokens
 * ------------------------------------------------------------------------- */

typedef struct {

    isize  len;
    isize  cap;
    Token* data;

} Tokens;

Tokens tokens_new(isize cap, Arena* perm);
Tokens tokens_scan(Str text, Arena* perm);

/* ---------------------------------------------------------------------------
 * Scanner
 * ------------------------------------------------------------------------- */

typedef struct {

    isize start;
    isize current;
    isize line;

    Str text;

} Scanner;

Scanner scanner_new(Str text);
Str     scanner_string(Scanner* s, Arena* perm);

/* ---------------------------------------------------------------------------
 * Helper macros and functions (with short names)
 * ------------------------------------------------------------------------- */

#define SCurrent (s->text.buf[s->current])
#define SAdvance (s->text.buf[s->current++])
#define SIsAtEnd (s->current >= s->text.len)
bool SMatch(Scanner* s, char expected);
char SPeek(Scanner* s);

/* ---------------------------------------------------------------------------
 * Adding new tokens
 * ------------------------------------------------------------------------- */

void tokens_append(Tokens* tokens, Scanner* s, int type, Arena* perm);
void tokens_append_literal(Tokens* tokens, Scanner* s, int type, Str literal, Arena* perm);
