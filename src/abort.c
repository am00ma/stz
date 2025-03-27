#include "abort.h"
#include <unistd.h>

void osfail(void) { _exit(1); }

i32 osread(i32 fd, char* buf, i32 cap) { return (i32)read(fd, buf, cap); }

b32 oswrite(i32 fd, char* buf, i32 len)
{
    for (i32 off = 0; off < len;)
    {
        i32 r = (i32)write(fd, buf + off, len - off);
        if (r < 1) { return 0; }
        off += r;
    }
    return 1;
}

void oom(void)
{
    static const char msg[] = "out of memory\n";
    oswrite(2, (char*)msg, lengthof(msg));
    osfail();
}
