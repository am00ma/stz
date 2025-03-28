/*
 * Accept string and return parsed, formatted and contatenated string
 */
#include "arena.h"
#include "range.h"
#include "str.h"
#include <stdio.h>
#include <string.h> // memcpy

constexpr isize PERM_CAPACITY = 128;

Str parse_and_format(Str src, Arena temp, Arena* perm)
{
    // Reserve PERM_CAPACITY so str_fmt can use rest
    Str tmp = str_new(PERM_CAPACITY, &temp);

    // Copy source string ( can only be done before using temp_loop )
    memcpy(tmp.buf, src.buf, src.len);

    // TODO: Set correct len, but now info about capacity is lost
    tmp.len = src.len;

    // Parse and append to tmp
    FOR(src)
    {
        if (src.buf[i] == 'l')
        {
            // Temp arena for this loop
            Arena temp_loop = temp;

            // Parse (note -> using new temp for each loop)
            Str found = str_fmt(&temp_loop, "Found l: %d\n", i); // uses STR_MAXLEN = 1024

            // Store (not checking capacity as it has been lost)
            memcpy(&tmp.buf[tmp.len], found.buf, found.len);
            tmp.len += found.len;
        }
    }

    // Copy to permanent arena
    Str dst = str_copy(tmp, perm);

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
