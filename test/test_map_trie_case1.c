#include "map_trie.h"
#include <stdio.h> // printf

// clang-format off
const char *bit_rep[16] = {
    [ 0] = "0000", [ 1] = "0001", [ 2] = "0010", [ 3] = "0011",
    [ 4] = "0100", [ 5] = "0101", [ 6] = "0110", [ 7] = "0111",
    [ 8] = "1000", [ 9] = "1001", [10] = "1010", [11] = "1011",
    [12] = "1100", [13] = "1101", [14] = "1110", [15] = "1111",
};
const char *hex_rep[16] = {
    [ 0] = "0", [ 1] = "1", [ 2] = "2", [ 3] = "3",
    [ 4] = "4", [ 5] = "5", [ 6] = "6", [ 7] = "7",
    [ 8] = "8", [ 9] = "9", [10] = "a", [11] = "b",
    [12] = "c", [13] = "d", [14] = "e", [15] = "f",
};
// clang-format on

void print_byte(u64 byte)
{
    printf("%20ld | ", byte);
    for (int i = 0; i < 8; i++)
    {
        u8 part = (byte >> (i * 4)) & 0x0F;
        printf("%s", hex_rep[part]);
    }
    printf(" \n");
}

// Print hash
#define PH(text)                                                                                                       \
    h = str_hash64(Str(text));                                                                                         \
    printf("%10.*s | %20lu | %ld \n", (int)s.len, s.buf, h, h >> 62);                                                  \
    print_byte(h);

int main(int argc, char* argv[])
{
    // 1. Check a few hash functions
    Str s;
    u64 h;

    PH("hello");
    PH("hi");
    PH("how");
    PH("are");
    PH("you");
    //    hello |  9056308289470953916 | 1
    //       hi |  4059371871905309591 | 0
    //      how |   916393671311057402 | 0
    //      are |   807366383986214394 | 0
    //      you |  4579279992790449811 | 0

    PH("this");
    PH("is");
    PH("some");
    PH("supreme");
    PH("bullshit");
    //     this |  7365605355570464576 | 1
    //       is | -5526878167481075084 | 2
    //     some |  6072071276228292792 | 1
    //  supreme | -5026769000608138253 | 2
    // bullshit |  8278192414267644971 | 1

    PH("this is some supreme bullshit");
    return 0;
}
