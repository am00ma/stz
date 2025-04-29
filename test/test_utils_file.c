#include "arena.h"
#include "str.h"
#include "types.h"
#include "utils/file.h"

#include <stdio.h> // printf

int main(int argc, char* argv[])
{
    constexpr i32 MB = 1024 * 1024;

    Arena perm = arena_new(2 * MB);

    StrOK dataOK = {0};

    dataOK = file_read(Str("./hello"), &perm);
    if (dataOK.err) { printf("Error: %d\n", dataOK.err); }
    else { printf("%.*s", pstr(dataOK.data)); }

    dataOK = file_read(Str("./example/read.c"), &perm);
    if (dataOK.err) { printf("Error: %d\n", dataOK.err); }
    else { printf("%.*s", pstr(dataOK.data)); }

    arena_print("Final:", &perm);

    return 0;
}
