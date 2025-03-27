#pragma once

#include <stddef.h>  // ptrdiff_t
#include <stdint.h>  // uint8_t, int32_t, int32_t, uint32_t, uint64_t, uintptr_t
#include <uchar.h>   // char16_t

typedef uint8_t   u8;
typedef int32_t   b32;
typedef int32_t   i32;
typedef int64_t   i64;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;
typedef char16_t  c16;
typedef float     f32;
typedef double    f64;
typedef uintptr_t uptr;
typedef ptrdiff_t isize;
typedef size_t    usize;

#define countof(a)  (isize)(sizeof(a) / sizeof(*(a)))
#define lengthof(s) (countof(s) - 1)
