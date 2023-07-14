#pragma once

#define ASSERT(_e, ...) if (!(_e)) { fprintf(stderr, __VA_ARGS__); exit(1); }

#define PI 3.1415926535f
#define TAU PI * 2
#define EULER 2.7182818284590f

template <typename T>
T min(T a, T b) {
    return a < b ? a : b;
}
template <typename T, typename Q>
T min(T a, Q b) {
    return a < b ? a : (T)b;
}

template <typename T>
T max(T a, T b) {
    return a > b ? a : b;
}

template <typename T, typename Q>
T max(T a, Q b) {
    return a > b ? a : b;
}

#define sign(a) ({                                       \
        __typeof__(a) _a = (a);                          \
        (__typeof__(a))(_a < 0 ? -1 : (_a > 0 ? 1 : 0)); \
    })
