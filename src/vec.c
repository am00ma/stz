#include "vec.h"

#define VEC_IMPLEMENTATION 1

#define T i32
#include "vec-ctl.h"

#define T f32
#include "vec-ctl.h"

#define T f64
#include "vec-ctl.h"

#undef VEC_IMPLEMENTATION
