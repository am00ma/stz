#include "map_msi.h"

int main(void)
{
    Arena     perm = arena_new(CAPACITY);
    StrMapMSI map  = strmap_msi_new(5, &perm); // 2^5 = 32 elements when full

    // Insert some values
    *strmap_msi_lookup(&map, Str("hello")) = Str("hi");
    *strmap_msi_lookup(&map, Str("how"))   = Str("are");
    *strmap_msi_lookup(&map, Str("you"))   = Str("doing");

    // Retrieve values
    strmap_msi_print(&map, Str("hello"));
    strmap_msi_print(&map, Str("how"));
    strmap_msi_print(&map, Str("you"));
    strmap_msi_print(&map, Str("doing"));

    return 0;
}
