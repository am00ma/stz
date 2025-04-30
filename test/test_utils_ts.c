#include "arena.h"
#include "macros.h"
#include "str.h"
#include "utils/file.h"
#include "utils/ts.h"

#include <stdio.h>  // printf
#include <stdlib.h> // EXIT_SUCCESS, EXIT_FAILURE
#include <string.h> // strlen

constexpr i32 MB = 1024 * 1024;

// For easier concatenation, defining as macro
#define TS_DIR "/home/x/hub/repos/study/c/libs/parsers/nvim-ts-lsp-dap/data/ts"

int main()
{
    // Currently hardcoded, but can be given with args
    Str   file = Str("include/str.h");
    char* name = "tree_sitter_c";
    char* path = TS_DIR "/c.so";

    // Alloc 10MB
    Arena perm = arena_new(10 * MB);

    // Read file (exit on error)
    auto textOK = file_read(file, &perm);
    if (textOK.err) { die(textOK.err, "Could not read %.*s", pstr(file)); }
    Str text = textOK.data;

    // Open treesitter .so file (exit on error)
    TsOK tsOK = ts_open(name, path);
    if (tsOK.err) { die(tsOK.err, "Could not load %s from %s", name, path); }
    Ts ts = tsOK.data;

    // Parse given file
    ts_parse(&ts, text);

    // Recursively print the tree
    ts_print_named_node(ts.root, text.buf, 0, NULL);
    printf("\n");

    // Close parser
    ts_close(&ts);

    return EXIT_SUCCESS;
}
