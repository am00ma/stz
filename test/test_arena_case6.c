/* Using a temporary arena per frame */

#include "arena.h"
#include "range.h"
#include "types.h"
#include <stdio.h>

typedef struct {
    char* buf;
    isize len;
} str;

void process_frame(i32 i, Arena temp)
{
    str a = {.buf = new (&temp, char, 10), .len = 10};

    FOR(a, j) a.buf[j] = 65 + j + (i % 10);

    // Print at intervals
    if (!((i + 1) % 128)) printf("%d: %.*s\n", i, (int)a.len, a.buf);
}

int main()
{
    // Create arena
    Arena temp = arena_new(1024);

    // Process frame (passing by value, so same arena each time)
    RANGE(i, 1024) process_frame(i, temp);

    // Print used space
    arena_print("temp: ", &temp);
    return 0;
}
