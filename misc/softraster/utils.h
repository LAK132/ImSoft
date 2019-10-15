#ifndef SOFTRASTER_UTILS_H
#define SOFTRASTER_UTILS_H

#ifdef ARDUINO
// #   pragma GCC optimize("-03")
#endif

#include "defines.h"

template<typename T>
struct range_t
{
    T min, max;
};

template<typename T>
struct clip_t
{
    range_t<T> x, y;
};

template<typename POS, typename COL>
struct pixel_t
{
    POS x, y;
    float u, v;
    COL c;
};

template<typename POS, typename COL>
struct triangle_t
{
    pixel_t<POS, COL> p1, p2, p3;
};

template<typename POS, typename COL>
struct rectangle_t
{
    pixel_t<POS, COL> p1, p2;
};

template<typename T>
inline void swap(T* tri1, T* tri2)
{
    T temp;
    memcpy(&temp, tri1, sizeof(T));
    memcpy(tri1, tri2, sizeof(T));
    memcpy(tri2, &temp, sizeof(T));
}

// [0x00, 0xFF]
template<typename T> INLINE_DEC(T lerp(T a, T b, uint8_t f));
template<typename T> INLINE_DEF(T lerp(T a, T b, uint8_t f))
{
    if      (a == b)    return a;
    if      (f == 0x00) return a;
    else if (f == 0xFF) return b;
    return a + ((f * (b - a)) / 0xFF);
}

// [0.0f, 1.0f]
template<typename T> INLINE_DEC(T lerp(T a, T b, float f));
template<typename T> INLINE_DEF(T lerp(T a, T b, float f))
{
    if      (a == b)    return a;
    if      (f <= 0.0f) return a;
    else if (f >= 1.0f) return b;
    return a + (f * (b - a));
}

template<typename T> INLINE_DEC(T inl_min(const T a, const T b));
template<typename T> INLINE_DEF(T inl_min(const T a, const T b))
{
    return a > b ? b : a;
}

template<typename T> INLINE_DEC(T inl_max(const T a, const T b));
template<typename T> INLINE_DEF(T inl_max(const T a, const T b))
{
    return a > b ? a : b;
}

template<typename T> INLINE_DEC(range_t<T> inl_min(const range_t<T> &a, const range_t<T> &b));
template<typename T> INLINE_DEF(range_t<T> inl_min(const range_t<T> &a, const range_t<T> &b))
{
    return {
        inl_max(a.min, b.min),
        inl_min(a.max, b.max)
    };
}

template<typename T> INLINE_DEC(range_t<T> inl_max(const range_t<T> &a, const range_t<T> &b));
template<typename T> INLINE_DEF(range_t<T> inl_max(const range_t<T> &a, const range_t<T> &b))
{
    return {
        inl_min(a.min, b.min),
        inl_max(a.max, b.max)
    };
}

template<typename P, typename C> INLINE_DEC(P dot(const pixel_t<P, C> &a, const pixel_t<P, C> &b));
template<typename P, typename C> INLINE_DEF(P dot(const pixel_t<P, C> &a, const pixel_t<P, C> &b))
{
    return (a.x * b.x) + (a.y * b.y);
}

#endif // SOFTRASTER_UTILS_H