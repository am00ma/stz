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

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("USAGE: parse-so FILE_PATH\n");
        return EXIT_FAILURE;
    }

    // Only one arg, but use it as Str, not char*
    Str file_path = {.buf = argv[1], .len = strlen(argv[1])};

    // Currently hardcoded, but can be given with args
    char* name = "tree_sitter_c";
    char* path = TS_DIR "/c.so";

    // Alloc 10MB
    Arena perm = arena_new(10 * MB);

    // Read file (exit on error)
    auto contentOK = file_read(file_path, &perm);
    if (contentOK.err) { die(contentOK.err, "Could not read %.*s", pstr(file_path)); }
    Str content = contentOK.data;

    // Open treesitter .so file (exit on error)
    TsOK  tsOK = ts_open(name, path);
    if (tsOK.err) { die(tsOK.err, "Could not load %s from %s", name, path); }
    Ts ts = tsOK.data;

    // Parse given file
    ts_parse(&ts, content);

    // Recursively print the tree
    ts_print_named_node(ts.root, content.buf, 0, NULL);
    printf("\n");

    // Close parser
    ts_close(&ts);

    return EXIT_SUCCESS;
}
