# stz

C core library using arenas.

## Use cases

| no  | file                      | desc                                                                                                   |
| --- | ------------------------- | ------------------------------------------------------------------------------------------------------ |
| 1   | `test/test_arena_case1.c` | Return valid reference from function                                                                   |
| 2   | `test/test_arena_case2.c` | Nested structs that need initialization                                                                |
| 3   | `test/test_arena_case3.c` | Read files and reclaim space on error                                                                  |
| 4   | `test/test_arena_case4.c` | Parsing a file, and growing an array dynamically                                                       |
| 5   | `test/test_arena_case5.c` | Growing two arrays dynamically                                                                         |
| 6   | `test/test_arena_case6.c` | Using a temporary arena per frame (allocate on realtime threads)                                       |
| 7   | `test/test_str_case1.c`   | Return string from function, get null terminated string                                                |
| 8   | `test/test_str_case2.c`   | Accept string and return parsed, formatted and contatenated string                                     |
| 9   | `test/test_buf_case1.c`   | Accept string and return parsed, formatted and contatenated string, but using buffers to reclaim space |

## Units

| no  | file    | size (K) | desc                                       |
| --- | ------- | -------- | ------------------------------------------ |
| 1   | abort.o | 5.3      | oom                                        |
| 2   | arena.o | 6.1      | Arena - new, alloc                         |
| 3   | str.o   | 7.7      | Str - new, store, equal, fmt               |
| 4   | buf.o   | 4.8      | Buf - new, join, final                     |
| 5   | vec.o   | 15       | i32, f32, f64 - new, range, sum, prod, div |
