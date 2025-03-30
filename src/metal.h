#ifndef METAL_H
#define METAL_H
// Do something, if it goes wrong break the program
#ifdef __EMSCRIPTEN__
#define ASSERT(_e, ...) if (!(_e)) { fprintf(stderr, __VA_ARGS__); emscripten_force_exit(1); }
#else
#define ASSERT(_e, ...) if (!(_e)) { fprintf(stderr, __VA_ARGS__); exit(1); }
#endif
// Define more readable definitions
typedef float    f32;
typedef double   f64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef size_t   usize;
typedef ssize_t  isize;

// Define window dimensions
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 400
#define WINDOW_SCALE 1

// Define vector 2 with float and int
typedef struct v2_s { f32 x, y; } v2;
typedef struct v2i_s { i32 x, y; } v2i;
typedef struct { int val, side; v2 pos; } hit_point;

// Get the dot product of two vectors
#define dot(v0, v1)                  \
    ({ const v2 _v0 = (v0), _v1 = (v1); (_v0.x * _v1.x) + (_v0.y * _v1.y); })

// Get the length of a vector
#define length(v) ({ const v2 _v = (v); sqrtf(dot(_v, _v)); })

// normalises two vectors
#define normalize(u) ({              \
        const v2 _u = (u);           \
        const f32 l = length(_u);    \
        (v2) { _u.x / l, _u.y / l }; \
    })

// returns the smallest and biggest of the supplied values
#define min(a, b) ({ __typeof__(a) _a = (a), _b = (b); _a < _b ? _a : _b; })
#define max(a, b) ({ __typeof__(a) _a = (a), _b = (b); _a > _b ? _a : _b; })

// returns a -1 if a is negative, 0 if = 0 and 1 if positive
#define sign(a) ({                                       \
        __typeof__(a) _a = (a);                          \
        (__typeof__(a))(_a < 0 ? -1 : (_a > 0 ? 1 : 0)); \
    })

#endif