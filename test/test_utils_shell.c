#include "arena.h"
#include "str.h"
#include "types.h"
#include "utils/shell.h"

#include <stdio.h> // printf

int main()
{
    constexpr i32 MB = 1024 * 1024;

    Arena       perm    = arena_new(16 * MB);
    Str         command = {0};
    ShellResult result  = {0};
    Arena       temp    = {0};

    temp    = perm;
    command = Str("ls -lah ./");
    result  = shell_exec(command, 14 * MB, 2 * MB, &temp);
    printf("Status: %d\n", result.status);
    printf("stdout:\n%.*s\n", pstr(result.stdout));
    printf("stderr:\n%.*s\n", pstr(result.stderr));
    arena_print("After `ls -lah ./` > temp:\n", &temp);
    arena_print("After `ls -lah ./` > perm:\n", &perm);

    temp    = perm;
    command = Str("ls -lah ./abcdef");
    result  = shell_exec(command, 14 * MB, 2 * MB, &temp);
    printf("Status: %d\n", result.status);
    printf("stdout:\n%.*s\n", pstr(result.stdout));
    printf("stderr:\n%.*s\n", pstr(result.stderr));
    arena_print("After `ls -lah ./abcdef` > temp:\n", &temp);
    arena_print("After `ls -lah ./abcdef` > perm:\n", &perm);

    return 0;
}
