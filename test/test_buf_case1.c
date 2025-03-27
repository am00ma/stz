#include "arena.h"
#include "buf.h"
#include "range.h"
#include <stdio.h>

constexpr isize PERM_CAPACITY = 128;

Str parse_and_format(Str src, Arena temp, Arena* perm)
{
    // Reserve PERM_CAPACITY so str_fmt can use rest
    Buf tmp = buf_new(PERM_CAPACITY, &temp);

    // Copy source string ( can only be done before using temp_loop )
    buf_join(&tmp, src);

    // Parse and append to tmp
    FOR(src)
    {
        if (src.buf[i] == 'l')
        {
            // Temp arena for this loop
            Arena temp_loop = temp;

            // Format and append ( uses STR_MAXLEN = 1024 )
            Str found = str_fmt(&temp_loop, "Found l: %d\n", i);

            /*
             * temp     :: 0x7fff192c7a70 (beg: 0x56aec34d93b0, end: 0x56aec34d97b0)
             *   left: 1024, used: 128, cap: 1152
             * temp_loop:: 0x7fff192c7a70 (beg: 0x56aec34d93bc, end: 0x56aec34d97b0)
             *   left: 1012, used: 140, cap: 1152
             *
             * 140 - 128 = 12 extra for `Found ... <10`
             * 141 - 128 = 13 extra for `Found ... >=10`
             */

            // Store
            buf_join(&tmp, found); // copies memory

            printf("\n---  %d :  ---\n", i);
            arena_print("temp     :", &temp);
            arena_print("temp_loop:", &temp_loop);
        }
    }

    printf("\n-------------\n");

    // Finalize temp arena ( Not neccessary, but frees STR_MAXLEN stuff )
    Str out = buf_final(&tmp, &temp);
    arena_print("temp     :", &temp);

    // Store in permanent arena
    Str dst = str_copy(out, perm);

    return dst;
}

int main()
{
    Arena perm = arena_new(PERM_CAPACITY);

    // Allocate just enough temp arena
    Arena temp = arena_new(STR_MAXLEN + PERM_CAPACITY);

    Str src = Str("hello hi, alles good?\n");

    Str dst = parse_and_format(src, temp, &perm);
    printf("%.*s", pstr(dst));

    arena_print("After parse_and_format: perm:", &perm);
    arena_print("After parse_and_format: temp:", &temp);

    return 0;
}
