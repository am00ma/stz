#include "token.h"
#include "macros.h" // die

/* ---------------------------------------------------------------------------
 * Single token
 * ------------------------------------------------------------------------- */

Str token_string(Token* t, Arena* perm)
{
    constexpr isize maxlen = 1024;
    return str_fmtn(perm, maxlen, "(%2d, %.*s, %.*s, %d)", t->type, pstr(t->lexeme), pstr(t->literal), t->line);
}

/* ---------------------------------------------------------------------------
 * List of tokens
 * ------------------------------------------------------------------------- */

// Constructor
Tokens tokens_new(isize cap, Arena* perm)
{
    return (Tokens){
        .len  = 0,
        .cap  = cap,
        .data = new (perm, Token, cap),
    };
}

/* ---------------------------------------------------------------------------
 * Scanner
 * ------------------------------------------------------------------------- */

Scanner scanner_new(Str text)
{
    return (Scanner){
        .start   = 0,
        .current = 0,
        .line    = 1,
        .text    = text,
    };
}

bool SMatch(Scanner* s, char expected)
{
    if (SIsAtEnd) return false;
    if (SCurrent != expected) return false;
    s->current++;
    return true;
}

char SPeek(Scanner* s)
{
    if (SIsAtEnd) return '\0';
    return SCurrent;
}

Str scanner_string(Scanner* s, Arena* perm)
{
    constexpr isize maxlen = 1024;
    return str_fmtn(perm, maxlen,
                    "start  : %ld\n"
                    "current: %ld\n"
                    "line   : %ld\n"
                    "len    : %ld\n",
                    s->start, s->current, s->line, s->text.len);
}

/* ---------------------------------------------------------------------------
 * Adding new tokens
 * ------------------------------------------------------------------------- */

void tokens_append_token(Tokens* tokens, Token t, Arena* perm)
{
    if (tokens->len >= tokens->cap) { die(1, "Out of memory"); }
    tokens->data[tokens->len] = t;
    tokens->len++;
}

// Without literal
void tokens_append(Tokens* tokens, Scanner* s, int type, Arena* perm)
{
    if (tokens->len >= tokens->cap) { die(1, "Out of memory"); }
    tokens->data[tokens->len] = (Token){
        .type    = type,
        .line    = s->line,
        .lexeme  = {0},
        .literal = {0},
    };
    tokens->len++;
}

// With literal
void tokens_append_literal(Tokens* tokens, Scanner* s, int type, Str literal, Arena* perm)
{
    if (tokens->len >= tokens->cap) { die(1, "Out of memory"); }
    tokens->data[tokens->len] = (Token){
        .type = type,
        .line = s->line,
        .lexeme =
            {
                .buf = &s->text.buf[s->start],
                .len = s->current - s->start,
            },
        .literal = literal,
    };
    tokens->len++;
}

/* ---------------------------------------------------------------------------
 * Common cases
 * ------------------------------------------------------------------------- */

Token case_comment_or_slash(Scanner* s, int comment, int slash, Arena* perm)
{
    Token t = {};

    if (SMatch(s, '/'))
    {
        // A comment goes until the end of the line.
        while ((SPeek(s) != '\n') && (!SIsAtEnd)) { SAdvance; };
        t.type    = comment;
        t.line    = s->line;
        t.literal = (Str){.buf = &s->text.buf[s->start], .len = s->current - s->start};
    }
    else
    {
        t.type = slash;
        t.line = s->line;
    }
    return t;
}
