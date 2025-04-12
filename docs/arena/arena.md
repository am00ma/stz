# arena

## Use cases

| no  | file                      | desc                                                                     |
| --- | ------------------------- | ------------------------------------------------------------------------ |
| 1   | `test/test_arena_case1.c` | Return valid reference from function                                     |
| 2   | `test/test_arena_case2.c` | Nested structs that need initialization                                  |
| 3   | `test/test_arena_case3.c` | Read files and reclaim space on error                                    |
| 4   | `test/test_arena_case4.c` | Parsing a file, and growing an array dynamically                         |
| 5   | `test/test_arena_case5.c` | Growing two arrays dynamically                                           |
| 6   | `test/test_arena_case6.c` | Using a temporary arena per frame (e.g. to allocate on realtime threads) |
| 7   | `test/test_arena_case7.c` | Using threads to load files async                                        |

### Return valid reference from function

### Nested structs that need initialization

### Read files and reclaim space on error

### Parsing a file, and growing an array dynamically

### Growing two arrays dynamically

### Using a temporary arena per frame (e.g. to allocate on realtime threads)

### Using threads to load files async

## Resources

1. nullprogram
2. Ryan Fleury
3. Nic Barker
4. Casey Muratori
