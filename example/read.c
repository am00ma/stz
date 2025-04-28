#include <stz/arena.h>
#include <stz/str.h>

#include <stdio.h> // FILE, fopen, ...

typedef enum {
    FILE_SUCCESS = 0,
    FILE_FAIL_OPEN,
    FILE_FAIL_SEEK,
    FILE_FAIL_READ,
    FILE_FAIL_CLOSE,
} FileError;

typedef struct {
    Str       data;
    FileError err;
} StrOK;

StrOK file_read(Str path, Arena* a);

StrOK file_read(Str path, Arena* a)
{
    const char* path_cstr = str_c(path, a);

    FILE* f = fopen(path_cstr, "rb");
    if (f == NULL)
    {
        perror("fopen");
        return (StrOK){.err = FILE_FAIL_OPEN};
    }

    if (fseek(f, 0, SEEK_END) != 0)
    {
        perror("fseek");
        return (StrOK){.err = FILE_FAIL_SEEK};
    }

    isize size = ftell(f);
    Str   data = {
          .len = size,
          .buf = new (a, char, size),
    };

    if (fseek(f, 0, SEEK_SET) != 0)
    {
        perror("fseek");
        return (StrOK){.err = FILE_FAIL_SEEK};
    }

    if (fread(data.buf, sizeof(char), data.len, f) != (usize)data.len)
    {
        perror("fseek");
        return (StrOK){.err = FILE_FAIL_SEEK};
    }

    if (fclose(f) != 0)
    {
        perror("fclose");
        return (StrOK){.err = FILE_FAIL_CLOSE};
    }

    return (StrOK){.data = data, .err = FILE_SUCCESS};
}

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
