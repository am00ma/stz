/*
 * Return string from function, get null terminated string
 */
#include "arena.h"
#include "str.h"
#include <stdio.h>

Str parse_and_format(Arena* perm)
{
    Str a = str_store("hello\n", perm);
    return a;
}

int main()
{
    Arena perm = arena_new(16);

    Str a = parse_and_format(&perm); // Allocated inside

    // Print using len
    printf("%.*s", pstr(a)); // Accessible outside

    arena_print("After store", &perm); // Used: 6+1 ("hello\n\0")

    // Normal c string with null terminator
    Arena temp = perm;             // Use a temporary arena for this
    printf("%s", str_c(a, &temp)); // Needs to be accessible outsize

    arena_print("Final", &perm); // Used: 6+1 * 2 ("hello\n\0")

    return 0;
}
