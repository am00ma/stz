#include "utils/file.h"

#include <stdio.h> // FILE, fopen, ...

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
