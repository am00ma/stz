#include "arena.h"
#include "str.h"
#include <stdio.h>

str parse_and_format(Arena* perm)
{
    str a = str_store("hello\n", perm);
    return a;
}

int main()
{
    Arena perm = arena_new(16);

    str a = parse_and_format(&perm); // Allocated inside

    // Print using len
    printf("%.*s", pstr(a)); // Accessible outside

    arena_print("After store", &perm); // Used: 6+1 ("hello\n\0")

    // Normal c string with null terminator
    printf("%s", str_c(a, &perm)); // Needs to be accessible outsize

    arena_print("Final", &perm); // Used: 6+1 * 2 ("hello\n\0")

    return 0;
}
