/* Read files and reclaim space on error
 *
 * Ideally, we will return error values as well, but assume return string of
 * length zero means error is a valid assumption in this case.
 * */

#include "arena.h"
#include "macros.h"
#include "range.h"
#include "types.h"
#include <stdio.h>
#include <string.h>

typedef struct {
    char* buf;
    isize len;
} str;

str read_file(bool should_err, Arena* perm)
{
    char file[500] = {0};
    memset(file, 'A', 500); // Create string of 500 A's

    // Read size of file
    isize size = sizeof(file);

    // Allocate on arena
    str out = {.buf = new (perm, char, size), .len = size};

    // Actually read
    if (should_err)
    {
        // NOTE: reclaim the space before returning
        perm->beg -= size;
        return (str){0};
    }
    else { RANGE(i, size) out.buf[i] = file[i]; }

    return out;
}

int main()
{
    // Create arena
    Arena perm = arena_new(1024);
    str   out  = {0};

    // In case there was no error
    out = read_file(false, &perm);
    if (!out.len) error("Could not read");

    RANGE(i, out.len) printf("%c ", out.buf[i]);
    printf("\n");

    arena_print("No error", &perm);

    // In case of error, we want to reclaim the space
    out = read_file(true, &perm);
    if (!out.len) error("Could not read");

    arena_print("With error (no extra space used)", &perm);

    return 0;
}
