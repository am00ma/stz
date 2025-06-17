#include "arena.h"
#include "macros.h"
#include "range.h"
#include "token.h"
#include <stdio.h>

typedef enum {

    EMPTY = 0,

    // Single-character tokens
    LEFT_BRACE,
    RIGHT_BRACE,
    DOT,
    COLON,
    SEMICOLON,

    // Literals
    IDENTIFIER,
    STRING,
    NUMBER,

    // End of file
    ENDOFFILE

} TokenType;

// Core function
Tokens scanner_scan(Scanner* s, isize maxtokens, Arena* perm)
{
    Tokens tokens = tokens_new(maxtokens, perm);

    while (!SIsAtEnd)
    {
        s->start = s->current;

        char c = SAdvance;
        switch (c)
        {
        case '{': tokens_append(&tokens, s, LEFT_BRACE, perm); break;
        case '}': tokens_append(&tokens, s, RIGHT_BRACE, perm); break;
        case '.': tokens_append(&tokens, s, DOT, perm); break;
        case ':': tokens_append(&tokens, s, COLON, perm); break;
        case ';': tokens_append(&tokens, s, SEMICOLON, perm); break;

        case '\n': s->line++; break;

        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace.
            break;

        default: error("Unexpected character: %c", c); break;
        }
    }

    return tokens;
}

int main()
{
    Arena perm = arena_new(16 * 1024 * 1024);
    Str   text = Str("body {\nwidth: 200px;\nheight:200px\n}");

    Scanner s = {
        .start   = 0,
        .current = 0,
        .line    = 1,
        .text    = text,
    };

    Tokens tokens = scanner_scan(&s, 1024, &perm);

    RANGE(i, tokens.len)
    {
        Arena temp = perm;
        printf("%d: %.*s\n", i, pstr(token_string(&tokens.data[i], &temp)));
    }

    return 0;
}
