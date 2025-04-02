# nullprogram

src: [c related](https://nullprogram.com/tags/c/)

## Arenas

| date       | title                                               | link                                     |
| ---------- | --------------------------------------------------- | ---------------------------------------- |
| 2023-09-27 | Arena allocator tips and tricks                     | https://nullprogram.com/blog/2023/09/27/ |
| 2023-09-30 | An easy-to-implement, arena-friendly hash map       | https://nullprogram.com/blog/2023/09/30/ |
| 2023-10-05 | A simple, arena-backed, generic dynamic array for C | https://nullprogram.com/blog/2023/10/05/ |
| 2024-04-14 | Speculations on arenas and custom strings in C++    | https://nullprogram.com/blog/2024/04/14/ |
| 2024-05-25 | Arenas and the almighty concatenation operator      | https://nullprogram.com/blog/2024/05/25/ |

### Arena allocator tips and tricks

- Example of `get_lines` with perm and scratch arenas
- TODO: Implementation explaining modulus operator
- How to handle oom
- Flags
- `new` macro
- attributes:
  - `malloc` : good
  - `alloc_size` : broken
  - `alloc_align` : no benefit observed
- size and growth
- TODO: pointer laundering (`.bss` file?)

Defined in `arena.h` and implemented in `arena.c`.

### An easy-to-implement, arena-friendly hash map

- Prerequisite: [The quick and practical "MSI" hash table](https://nullprogram.com/blog/2022/08/08/)

- hash trie: `child[4], key, value`, `upsert()`
- 4-ary tree
- single function to insert and retrieve
- top two bits select branch
- NOTE: Relative pointers
- TODO: Concurrent hash map

### A simple, arena-backed, generic dynamic array for C

- vec: `data, len, cap`, `push()`
- TODO: why order? : `push()` operates on any object which is slice-shaped. That is it has a pointer field named data, a ptrdiff_t length field named len, a ptrdiff_t capacity field named cap, and **all in that order**.
- NOTE: note that the grow branch uses a comma operator to sequence growth before pointer derivation, as grow will change the value of data as a side effect.

Rules of growing:

1. If the data pointer is null, make an initial allocation.
2. If the array ends at the next arena allocation, extend it.
3. Otherwise allocate a fresh array and copy.

### Speculations on arenas and custom strings in C++

- LATER:

### Arenas and the almighty concatenation operator

- `concat(Arena* a, T head, T tail)`
  - in place if possible
  - else copy
