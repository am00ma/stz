/* Parsing a file, and growing an array dynamically */

#include "arena.h"
#include "range.h"
#include "types.h"
#include <stdio.h>

typedef struct {
    char* buf;
    isize len;
} str;

typedef struct {
    char c;
    i32  i;
} match;

typedef struct {
    match* m;
    isize  n;
} matches;

matches parse_file(Arena temp, Arena* perm)
{
    char file[500] = {0};
    file[10]       = 'A';
    file[20]       = 'B';
    file[30]       = 'C';
    file[40]       = 'D';
    file[50]       = 'E';

    // Create return array on perm, empty list
    matches m = {.m = new (perm, match, 0), .n = 0};

    // Read size of file
    isize size = sizeof(file);

    // Allocate on temporary arena
    str out = {.buf = new (&temp, char, size), .len = size};

    // Read and parse
    RANGE(i, size)
    {
        out.buf[i] = file[i]; // 'Reading' from disk
        if (out.buf[i])
        {
            /* NOTE: Allocate for new element, throw away ref as we are not
             * using arena for anything else.
             * In other words, the allocated memory is for `m.m[m.n]`
             * This gives us the advantage of automatic oom management */
            new (perm, match);

            m.m[m.n] = (match){.c = out.buf[i], .i = i}; // Register match
            m.n++;                                       // Update length
        }
    }

    return m;
}

void print_matches(matches m) { RANGE(i, m.n) printf("%d: %c\n", m.m[i].i, m.m[i].c); }

int main()
{
    // Create arena
    Arena perm = arena_new(1024);
    Arena temp = arena_new(1024);

    matches out = {0};

    // In case there was no error
    out = parse_file(temp, &perm);
    print_matches(out);

    arena_print("No error: perm", &perm);
    arena_print("No error: temp ", &temp);

    // In case of error, we want to reclaim the space
    out = parse_file(temp, &perm);
    print_matches(out);

    arena_print("With error: perm", &perm);
    arena_print("With error: temp ", &temp);

    return 0;
}
