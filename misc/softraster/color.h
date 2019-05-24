#ifndef SOFTRASTER_COLOR_H
#define SOFTRASTER_COLOR_H

#include "defines.h"
#include <stdint.h>
#include <utility>

#define C16RMASK 0xF800
#define C16GMASK 0x07E0
#define C16BMASK 0x001F

#define C16R(C) ((C) & C16RMASK)
#define C16G(C) ((C) & C16GMASK)
#define C16B(C) ((C) & C16BMASK)

struct alpha8_t;
struct value8_t;
struct color16_t;
struct color16_alpha8_t;
struct color24_t;
struct color32_t;

// color add

inline alpha8_t         operator+(alpha8_t          lhs, const alpha8_t         &rhs);
inline value8_t         operator+(value8_t          lhs, const value8_t         &rhs);
inline color16_t        operator+(color16_t         lhs, const color16_t        &rhs);
inline color16_alpha8_t operator+(color16_alpha8_t  lhs, const color16_alpha8_t &rhs);
inline color24_t        operator+(color24_t         lhs, const color24_t        &rhs);
inline color32_t        operator+(color32_t         lhs, const color32_t        &rhs);

// color scalar multiply

inline alpha8_t         operator*(alpha8_t          lhs, const float rhs);
inline value8_t         operator*(value8_t          lhs, const float rhs);
inline color16_t        operator*(color16_t         lhs, const float rhs);
inline color16_alpha8_t operator*(color16_alpha8_t  lhs, const float rhs);
inline color24_t        operator*(color24_t         lhs, const float rhs);
inline color32_t        operator*(color32_t         lhs, const float rhs);
inline alpha8_t         operator*(const float lhs, alpha8_t          rhs);
inline value8_t         operator*(const float lhs, value8_t          rhs);
inline color16_t        operator*(const float lhs, color16_t         rhs);
inline color16_alpha8_t operator*(const float lhs, color16_alpha8_t  rhs);
inline color24_t        operator*(const float lhs, color24_t         rhs);
inline color32_t        operator*(const float lhs, color32_t         rhs);

// color multiply

inline alpha8_t         operator*(      alpha8_t   lhs, const alpha8_t  &rhs);
inline color32_t        operator*(const alpha8_t  &lhs, const value8_t  &rhs);
inline color16_alpha8_t operator*(const alpha8_t  &lhs, const color16_t &rhs);
inline color32_t        operator*(const alpha8_t  &lhs, const color24_t &rhs);
inline color32_t        operator*(const alpha8_t  &lhs,       color32_t  rhs);

inline color32_t        operator*(const value8_t  &lhs, const alpha8_t  &rhs);
inline value8_t         operator*(      value8_t   lhs, const value8_t  &rhs);
inline color16_t        operator*(const value8_t  &lhs,       color16_t  rhs);
inline color24_t        operator*(const value8_t  &lhs,       color24_t  rhs);
inline color32_t        operator*(const value8_t  &lhs,       color32_t  rhs);

inline color16_alpha8_t operator*(const color16_t &lhs, const alpha8_t  &rhs);
inline color16_t        operator*(      color16_t  lhs, const value8_t  &rhs);
inline color16_t        operator*(      color16_t  lhs, const color16_t &rhs);
inline color24_t        operator*(const color16_t &lhs,       color24_t  rhs);
inline color32_t        operator*(const color16_t &lhs,       color32_t  rhs);

inline color32_t        operator*(const color24_t &lhs, const alpha8_t  &rhs);
inline color24_t        operator*(      color24_t  lhs, const value8_t  &rhs);
inline color24_t        operator*(      color24_t  lhs, const color16_t &rhs);
inline color24_t        operator*(      color24_t  lhs, const color24_t &rhs);
inline color32_t        operator*(const color24_t &lhs,       color32_t  rhs);

inline color32_t        operator*(      color32_t  lhs, const alpha8_t  &rhs);
inline color32_t        operator*(      color32_t  lhs, const value8_t  &rhs);
inline color32_t        operator*(      color32_t  lhs, const color16_t &rhs);
inline color32_t        operator*(      color32_t  lhs, const color24_t &rhs);
inline color32_t        operator*(      color32_t  lhs, const color32_t &rhs);

inline color16_alpha8_t operator*(color16_alpha8_t lhs, const color16_alpha8_t &rhs);
inline color16_alpha8_t operator*(color16_alpha8_t lhs, const alpha8_t  &rhs);
inline color16_alpha8_t operator*(color16_alpha8_t lhs, const value8_t  &rhs);
inline color16_alpha8_t operator*(color16_alpha8_t lhs, const color16_t &rhs);
inline color32_t        operator*(const color16_alpha8_t &lhs, color24_t rhs);
inline color32_t        operator*(const color16_alpha8_t &lhs, color32_t rhs);
inline color16_alpha8_t operator*(const alpha8_t  &lhs, color16_alpha8_t rhs);
inline color16_alpha8_t operator*(const value8_t  &lhs, color16_alpha8_t rhs);
inline color16_alpha8_t operator*(const color16_t &lhs, color16_alpha8_t rhs);
inline color32_t        operator*(      color24_t  lhs, const color16_alpha8_t &rhs);
inline color32_t        operator*(      color32_t  lhs, const color16_alpha8_t &rhs);

// alpha blend

//
// rhsA == 1 -> outRGBA = rhsRGBA
//
// lhsA == 1 -> outRGB = lhsRGB (1 - rhsA) + rhsRGB rhsA
//
// outA = lhsA (1 - rhsA) + rhsA
// outA == 0 -> outRGB = 0
// outA != 0 -> outRGB = (lhsRGB lhsA (1 - rhsA) + rhsRGB rhsA) / outA
//

//
// a + ((f * (b - a)) / 0xFF)
// a + (((f * b) - (f * a)) / 0xFF)
// a - ((f * a) / 0xFF) + ((f * b) / 0xFF)
// ((a * (0xFF - f)) / 0xFF) + ((f * b) / 0xFF)
// ((b * f) / 0xFF) + ((a * (0xFF - f)) / 0xFF)
//

inline alpha8_t         operator%(const alpha8_t  &lhs,       alpha8_t   rhs);
inline value8_t         operator%(const alpha8_t  &lhs,       value8_t   rhs);
inline color16_t        operator%(const alpha8_t  &lhs,       color16_t  rhs);
inline color24_t        operator%(const alpha8_t  &lhs,       color24_t  rhs);
inline color32_t        operator%(const alpha8_t  &lhs,       color32_t  rhs);

inline value8_t         operator%(      value8_t   lhs, const alpha8_t  &rhs);
inline value8_t         operator%(const value8_t  &lhs,       value8_t   rhs);
inline color16_t        operator%(const value8_t  &lhs,       color16_t  rhs);
inline color24_t        operator%(const value8_t  &lhs,       color24_t  rhs);
inline color32_t        operator%(const value8_t  &lhs,       color32_t  rhs);

inline color16_t        operator%(      color16_t  lhs, const alpha8_t  &rhs);
inline value8_t         operator%(const color16_t &lhs,       value8_t   rhs);
inline color16_t        operator%(const color16_t &lhs,       color16_t  rhs);
inline color24_t        operator%(const color16_t &lhs,       color24_t  rhs);
inline color32_t        operator%(const color16_t &lhs,       color32_t  rhs);

inline color24_t        operator%(      color24_t  lhs, const alpha8_t  &rhs);
inline value8_t         operator%(const color24_t &lhs,       value8_t   rhs);
inline color16_t        operator%(const color24_t &lhs,       color16_t  rhs);
inline color24_t        operator%(const color24_t &lhs,       color24_t  rhs);
inline color32_t        operator%(const color24_t &lhs,       color32_t  rhs);

inline color32_t        operator%(      color32_t  lhs, const alpha8_t  &rhs);
inline value8_t         operator%(const color32_t &lhs,       value8_t   rhs);
inline color16_t        operator%(const color32_t &lhs,       color16_t  rhs);
inline color24_t        operator%(const color32_t &lhs,       color24_t  rhs);
inline color32_t        operator%(      color32_t  lhs, const color32_t &rhs);

inline color16_alpha8_t operator%(color16_alpha8_t lhs, const color16_alpha8_t &rhs);
inline color16_alpha8_t operator%(color16_alpha8_t lhs, const alpha8_t  &rhs);
inline value8_t         operator%(const color16_alpha8_t &lhs, value8_t  rhs);
inline color16_t        operator%(const color16_alpha8_t &lhs, color16_t rhs);
inline color24_t        operator%(const color16_alpha8_t &lhs, color24_t rhs);
inline color32_t        operator%(const color16_alpha8_t &lhs, color32_t rhs);
inline color16_alpha8_t operator%(const alpha8_t  &lhs, color16_alpha8_t rhs);
inline color16_t        operator%(const value8_t  &lhs, color16_alpha8_t rhs);
inline color16_t        operator%(      color16_t  lhs, const color16_alpha8_t &rhs);
inline color24_t        operator%(      color24_t  lhs, const color16_alpha8_t &rhs);
inline color32_t        operator%(      color32_t  lhs, const color16_alpha8_t &rhs);

struct alpha8_t
{
    uint8_t a;

    alpha8_t() = default;
    alpha8_t(const uint8_t alpha) : a(alpha) {}
    alpha8_t(const signed int alpha) : a(alpha) {}
    alpha8_t(const unsigned int alpha) : a(alpha) {}
    alpha8_t(const alpha8_t &rhs) : a(rhs.a) {}

    template<typename COLOR>
    explicit inline alpha8_t(const COLOR &rhs) : a(rhs.A()) { }

    template<typename COLOR> inline alpha8_t &operator=(const COLOR &rhs) { a = rhs.A(); return *this; }
    template<typename T> inline alpha8_t &operator+=(const T &rhs) { return *this = *this + rhs; }
    template<typename T> inline alpha8_t &operator*=(const T &rhs) { return *this = *this * rhs; }
    template<typename T> inline alpha8_t &operator%=(const T &rhs) { return *this = *this % rhs; }

    inline bool operator==(const alpha8_t &rhs) const { return a == rhs.a; }
    inline bool operator!=(const alpha8_t &rhs) const { return a != rhs.a; }

    inline uint8_t R() const { return 0xFF; }
    inline uint8_t G() const { return 0xFF; }
    inline uint8_t B() const { return 0xFF; }
    inline uint8_t A() const { return a; }
    inline uint8_t V() const { return a; }
    inline uint16_t RGB16() const { return (((a * 0x1F) / 0xFF) << 0xB) | (((a * 0x3FU) / 0xFF) << 0x5) | ((a * 0x1F) / 0xFF); }
    inline uint32_t RGBA32() const { return 0xFFFFFF00 | a; }
};

struct value8_t
{
    uint8_t v;

    value8_t() = default;
    value8_t(const uint8_t value) : v(value) {}
    value8_t(const signed int value) : v(value) {}
    value8_t(const unsigned int value) : v(value) {}
    value8_t(const value8_t &rhs) : v(rhs.v) {}

    template<typename COLOR>
    explicit inline value8_t(const COLOR &rhs) : v(rhs.V()) { }

    template<typename COLOR> inline value8_t &operator=(const COLOR &rhs) { v = rhs.V(); return *this; }
    template<typename T> inline value8_t &operator+=(const T &rhs) { return *this = *this + rhs; }
    template<typename T> inline value8_t &operator*=(const T &rhs) { return *this = *this * rhs; }
    template<typename T> inline value8_t &operator%=(const T &rhs) { return *this = *this % rhs; }

    inline bool operator==(const value8_t &rhs) const { return v == rhs.v; }
    inline bool operator!=(const value8_t &rhs) const { return v != rhs.v; }

    inline uint8_t R() const { return v; }
    inline uint8_t G() const { return v; }
    inline uint8_t B() const { return v; }
    inline uint8_t A() const { return 0xFF; }
    inline uint8_t V() const { return v; }
    inline uint16_t RGB16() const { return (((v * 0x1F) / 0xFF) << 0xB) | (((v * 0x3F) / 0xFF) << 0x5) | ((v * 0x1F) / 0xFF); }
    inline uint32_t RGBA32() const { return (v << 24) | (v << 16) | (v << 8) | v;}
};

struct color16_t
{
    uint16_t rgb;

    color16_t() = default;
    color16_t(const uint16_t val) : rgb(val) {}
    color16_t(const signed int val) : rgb(val) {}
    color16_t(const unsigned int val) : rgb(val) {}
    color16_t(const color16_t &rhs) : rgb(rhs.rgb) {}

    template<typename COLOR>
    explicit inline color16_t(const COLOR &rhs) : rgb(rhs.RGB16()) { }

    template<typename COLOR> inline color16_t &operator=(const COLOR &rhs) { rgb = rhs.RGB16(); return *this; }
    template<typename T> inline color16_t &operator+=(const T &rhs) { return *this = *this + rhs; }
    template<typename T> inline color16_t &operator*=(const T &rhs) { return *this = *this * rhs; }
    template<typename T> inline color16_t &operator%=(const T &rhs) { return *this = *this % rhs; }

    inline bool operator==(const color16_t &rhs) const { return rgb == rhs.rgb; }
    inline bool operator!=(const color16_t &rhs) const { return rgb != rhs.rgb; }

    inline uint8_t R() const { return ((C16R(rgb) >> 0xB) * 0xFF) / 0x1F; }
    inline uint8_t G() const { return ((C16G(rgb) >> 0x5) * 0xFF) / 0x3F; }
    inline uint8_t B() const { return  (C16B(rgb)         * 0xFF) / 0x1F; }
    inline uint8_t A() const { return 0xFF; }
    inline uint8_t V() const { return (R() + G() + B()) / 3; }
    inline uint16_t RGB16() const { return rgb; }
    inline uint32_t RGBA32() const { return (R() << 24) | (G() << 16) | (B() << 8) | 0xFF; }
};

struct color16_alpha8_t
{
    color16_t color;
    alpha8_t alpha;

    color16_alpha8_t() = default;
    color16_alpha8_t(const uint16_t val, const uint8_t alpha) : color(val), alpha(alpha) {}
    color16_alpha8_t(const signed int val, const signed int alpha) : color(val), alpha(alpha) {}
    color16_alpha8_t(const unsigned int val, const unsigned int alpha) : color(val), alpha(alpha) {}
    color16_alpha8_t(const color16_t &val, const alpha8_t &alpha) : color(val), alpha(alpha) {}
    color16_alpha8_t(const color16_alpha8_t &rhs) : color(rhs.color), alpha(rhs.alpha) {}

    template<typename COLOR>
    explicit inline color16_alpha8_t(const COLOR &rhs) : color(rhs.RGB16()), alpha(rhs.A()) { }

    template<typename T> inline color16_alpha8_t &operator+=(const T &rhs) { return *this = *this + rhs; }
    template<typename T> inline color16_alpha8_t &operator*=(const T &rhs) { return *this = *this * rhs; }
    template<typename T> inline color16_alpha8_t &operator%=(const T &rhs) { return *this = *this % rhs; }

    inline bool operator==(const color16_alpha8_t &rhs) const { return color == rhs.color && alpha == rhs.alpha; }
    inline bool operator!=(const color16_alpha8_t &rhs) const { return color != rhs.color || alpha != rhs.alpha; }

    inline uint8_t R() const { return ((C16R(color.rgb) >> 0xB) * 0xFF) / 0x1F; }
    inline uint8_t G() const { return ((C16G(color.rgb) >> 0x5) * 0xFF) / 0x3F; }
    inline uint8_t B() const { return  (C16B(color.rgb)         * 0xFF) / 0x1F; }
    inline uint8_t A() const { return alpha.a; }
    inline uint8_t V() const { return (R() + G() + B()) / 3; }
    inline uint16_t RGB16() const { return color.rgb; }
    inline uint32_t RGBA32() const { return (R() << 24) | (G() << 16) | (B() << 8) | alpha.a; }
};

struct color24_t
{
    uint8_t r, g, b;

    color24_t() = default;
    color24_t(const uint8_t valR, const uint8_t valG, const uint8_t valB)
        : r(valR), g(valG), b(valB) {}
    color24_t(const color24_t &rhs) : r(rhs.r), g(rhs.g), b(rhs.b) {}

    template<typename COLOR>
    explicit inline color24_t(const COLOR &rhs) : r(rhs.R()), g(rhs.G()), b(rhs.B()) { }

    template<typename COLOR>
    inline color24_t &operator=(const COLOR &rhs) { r = rhs.R(); g = rhs.G(); b = rhs.B(); return *this;}
    template<typename T> inline color24_t &operator+=(const T &rhs) { return *this = *this + rhs; }
    template<typename T> inline color24_t &operator*=(const T &rhs) { return *this = *this * rhs; }
    template<typename T> inline color24_t &operator%=(const T &rhs) { return *this = *this % rhs; }

    inline bool operator==(const color24_t &rhs) const { return r == rhs.r && g == rhs.g && b == rhs.b; }
    inline bool operator!=(const color24_t &rhs) const { return r != rhs.r && g != rhs.g && b != rhs.b; }

    inline uint8_t R() const { return r; }
    inline uint8_t G() const { return g; }
    inline uint8_t B() const { return b; }
    inline uint8_t A() const { return 0xFF; }
    inline uint8_t V() const { return (R() + G() + B()) / 3; }
    inline uint16_t RGB16() const { return (((r * 0x1F) / 0xFF) << 0xB) | (((g * 0x3F) / 0xFF) << 0x5) | ((b * 0x1F) / 0xFF); }
    inline uint32_t RGBA32() const { return (r << 24) | (g << 16) | (b << 8) | 0xFF; }
};

struct color32_t
{
    uint8_t r, g, b, a;

    color32_t() = default;
    color32_t(const uint8_t valR, const uint8_t valG, const uint8_t valB, const uint8_t valA)
        : r(valR), g(valG), b(valB), a(valA) {}
    color32_t(const color32_t &rhs) : r(rhs.r), g(rhs.g), b(rhs.b), a(rhs.a) {}

    template<typename COLOR>
    explicit inline color32_t(const COLOR &rhs) : r(rhs.R()), g(rhs.G()), b(rhs.B()), a(rhs.A()) { }

    template<typename COLOR>
    inline color32_t &operator=(const COLOR &rhs) { r = rhs.R(); g = rhs.G(); b = rhs.B(); a = rhs.A(); return *this; }
    template<typename T> inline color32_t &operator+=(const T &rhs) { return *this = *this + rhs; }
    template<typename T> inline color32_t &operator*=(const T &rhs) { return *this = *this * rhs; }
    template<typename T> inline color32_t &operator%=(const T &rhs) { return *this = *this % rhs; }

    inline bool operator==(const color32_t &rhs) const { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }
    inline bool operator!=(const color32_t &rhs) const { return r != rhs.r && g != rhs.g && b != rhs.b && a != rhs.a; }

    inline uint8_t R() const { return r; }
    inline uint8_t G() const { return g; }
    inline uint8_t B() const { return b; }
    inline uint8_t A() const { return a; }
    inline uint8_t V() const { return (R() + G() + B()) / 3; }
    inline uint16_t RGB16() const { return (((r * 0x1FU) / 0xFFU) << 0x0BU) | (((g * 0x3FU) / 0xFFU) << 0x05U) | ((b * 0x1FU) / 0xFFU); }
    inline uint32_t RGBA32() const { return (r << 24U) | (g << 16U) | (b << 8U) | a; }
};

// color add

inline alpha8_t         operator+(alpha8_t          lhs, const alpha8_t         &rhs)
{
    lhs.a += rhs.a;
    return lhs;
}

inline value8_t         operator+(value8_t          lhs, const value8_t         &rhs)
{
    lhs.v += rhs.v;
    return lhs;
}

inline color16_t        operator+(color16_t         lhs, const color16_t        &rhs)
{
    lhs.rgb =   C16R(C16R(lhs.rgb) + C16R(rhs.rgb)) |
                C16G(C16G(lhs.rgb) + C16G(rhs.rgb)) |
                C16B(C16B(lhs.rgb) + C16B(rhs.rgb));
    return lhs;
}

inline color16_alpha8_t operator+(color16_alpha8_t  lhs, const color16_alpha8_t &rhs)
{
    lhs.color.rgb = C16R(C16R(lhs.color.rgb) + C16R(rhs.color.rgb)) |
                    C16G(C16G(lhs.color.rgb) + C16G(rhs.color.rgb)) |
                    C16B(C16B(lhs.color.rgb) + C16B(rhs.color.rgb));
    lhs.alpha.a += rhs.alpha.a;
    return lhs;
}

inline color24_t        operator+(color24_t         lhs, const color24_t        &rhs)
{
    lhs.r += rhs.r;
    lhs.g += rhs.g;
    lhs.b += rhs.b;
    return lhs;
}

inline color32_t        operator+(color32_t         lhs, const color32_t        &rhs)
{
    lhs.r += rhs.r;
    lhs.g += rhs.g;
    lhs.b += rhs.b;
    lhs.a += rhs.a;
    return lhs;
}


// color scalar multiply

inline alpha8_t         operator*(alpha8_t          lhs, const float rhs)
{
    lhs.a *= rhs;
    return lhs;
}

inline value8_t         operator*(value8_t          lhs, const float rhs)
{
    lhs.v *= rhs;
    return lhs;
}

inline color16_t        operator*(color16_t         lhs, const float rhs)
{
    lhs.rgb =   C16R(static_cast<uint8_t>(C16R(lhs.rgb) * rhs)) |
                C16G(static_cast<uint8_t>(C16G(lhs.rgb) * rhs)) |
                C16B(static_cast<uint8_t>(C16B(lhs.rgb) * rhs));
    return lhs;
}

inline color16_alpha8_t operator*(color16_alpha8_t  lhs, const float rhs)
{
    lhs.color.rgb = C16R(static_cast<uint8_t>(C16R(lhs.color.rgb) * rhs)) |
                    C16G(static_cast<uint8_t>(C16G(lhs.color.rgb) * rhs)) |
                    C16B(static_cast<uint8_t>(C16B(lhs.color.rgb) * rhs));
    lhs.alpha.a *= rhs;
    return lhs;
}

inline color24_t        operator*(color24_t         lhs, const float rhs)
{
    lhs.r *= rhs;
    lhs.g *= rhs;
    lhs.b *= rhs;
    return lhs;
}

inline color32_t        operator*(color32_t         lhs, const float rhs)
{
    lhs.r *= rhs;
    lhs.g *= rhs;
    lhs.b *= rhs;
    lhs.a *= rhs;
    return lhs;
}

inline alpha8_t         operator*(const float lhs, alpha8_t          rhs)
{
    rhs.a *= lhs;
    return rhs;
}

inline value8_t         operator*(const float lhs, value8_t          rhs)
{
    rhs.v *= lhs;
    return rhs;
}

inline color16_t        operator*(const float lhs, color16_t         rhs)
{
    rhs.rgb =   C16R(static_cast<uint8_t>(C16R(rhs.rgb) * lhs)) |
                C16G(static_cast<uint8_t>(C16G(rhs.rgb) * lhs)) |
                C16B(static_cast<uint8_t>(C16B(rhs.rgb) * lhs));
    return rhs;
}

inline color16_alpha8_t operator*(const float lhs, color16_alpha8_t  rhs)
{
    rhs.color.rgb = C16R(static_cast<uint8_t>(C16R(rhs.color.rgb) * lhs)) |
                    C16G(static_cast<uint8_t>(C16G(rhs.color.rgb) * lhs)) |
                    C16B(static_cast<uint8_t>(C16B(rhs.color.rgb) * lhs));
    rhs.alpha.a *= lhs;
    return rhs;
}

inline color24_t        operator*(const float lhs, color24_t         rhs)
{
    rhs.r *= lhs;
    rhs.g *= lhs;
    rhs.b *= lhs;
    return rhs;
}

inline color32_t        operator*(const float lhs, color32_t         rhs)
{
    rhs.r *= lhs;
    rhs.g *= lhs;
    rhs.b *= lhs;
    rhs.a *= lhs;
    return rhs;
}

// color multiply

// alpha8_t

inline alpha8_t         operator*(      alpha8_t   lhs, const alpha8_t  &rhs)
{
    lhs.a = (lhs.a * rhs.a) / 0xFFU;
    return lhs;
}

inline color32_t        operator*(const alpha8_t  &lhs, const value8_t  &rhs)
{
    return {rhs.v, rhs.v, rhs.v, lhs.a};
}

inline color16_alpha8_t operator*(const alpha8_t  &lhs, const color16_t &rhs)
{
    return {rhs, lhs};
}

inline color32_t        operator*(const alpha8_t  &lhs, const color24_t &rhs)
{
    return {rhs.r, rhs.g, rhs.b, lhs.a};
}

inline color32_t        operator*(const alpha8_t  &lhs,       color32_t  rhs)
{
    rhs.a = (rhs.a * lhs.a) / 0xFFU;
    return rhs;
}

// value8_t

inline color32_t        operator*(const value8_t  &lhs, const alpha8_t  &rhs)
{
    return {lhs.v, lhs.v, lhs.v, rhs.a};
}

inline value8_t         operator*(      value8_t   lhs, const value8_t  &rhs)
{
    lhs.v = (lhs.v * rhs.v) / 0xFFU;
    return lhs;
}

inline color16_t        operator*(const value8_t  &lhs,       color16_t  rhs)
{
    const uint16_t v = lhs.RGB16();
    rhs.rgb =   C16R((C16R(rhs.rgb) * (C16R(v) >> 0xB)) / 0x1FU) |
                C16G((C16G(rhs.rgb) * (C16G(v) >> 0x5)) / 0x3FU) |
                C16B((C16B(rhs.rgb) * (C16B(v) >> 0x0)) / 0x1FU);
    return rhs;
}

inline color24_t        operator*(const value8_t  &lhs,       color24_t  rhs)
{
    rhs.r = (rhs.r * lhs.v) / 0xFFU;
    rhs.g = (rhs.g * lhs.v) / 0xFFU;
    rhs.b = (rhs.b * lhs.v) / 0xFFU;
    return rhs;
}

inline color32_t        operator*(const value8_t  &lhs,       color32_t  rhs)
{
    rhs.r = (rhs.r * lhs.v) / 0xFFU;
    rhs.g = (rhs.g * lhs.v) / 0xFFU;
    rhs.b = (rhs.b * lhs.v) / 0xFFU;
    return rhs;
}

// color16_t

inline color16_alpha8_t operator*(const color16_t &lhs, const alpha8_t  &rhs)
{
    return {lhs, rhs};
}

inline color16_t        operator*(      color16_t  lhs, const value8_t  &rhs)
{
    const uint16_t v = rhs.RGB16();
    lhs.rgb =   C16R((C16R(lhs.rgb) * (C16R(v) >> 0xB)) / 0x1FU) |
                C16G((C16G(lhs.rgb) * (C16G(v) >> 0x5)) / 0x3FU) |
                C16B((C16B(lhs.rgb) * (C16B(v) >> 0x0)) / 0x1FU);
    return lhs;
}

inline color16_t        operator*(      color16_t  lhs, const color16_t &rhs)
{
    lhs.rgb =   C16R((C16R(lhs.rgb) * (C16R(rhs.rgb) >> 0xB)) / 0x1FU) |
                C16G((C16G(lhs.rgb) * (C16G(rhs.rgb) >> 0x5)) / 0x3FU) |
                C16B((C16B(lhs.rgb) * (C16B(rhs.rgb) >> 0x0)) / 0x1FU);
    return lhs;
}

inline color24_t        operator*(const color16_t &lhs,       color24_t  rhs)
{
    rhs.r = (rhs.r * lhs.R()) / 0xFFU;
    rhs.g = (rhs.g * lhs.G()) / 0xFFU;
    rhs.b = (rhs.b * lhs.B()) / 0xFFU;
    return rhs;
}

inline color32_t        operator*(const color16_t &lhs,       color32_t  rhs)
{
    rhs.r = (rhs.r * lhs.R()) / 0xFFU;
    rhs.g = (rhs.g * lhs.G()) / 0xFFU;
    rhs.b = (rhs.b * lhs.B()) / 0xFFU;
    return rhs;
}

// color24_t

inline color32_t        operator*(const color24_t &lhs, const alpha8_t  &rhs)
{
    return {lhs.r, lhs.g, lhs.b, rhs.a};
}

inline color24_t        operator*(      color24_t  lhs, const value8_t  &rhs)
{
    lhs.r = (lhs.r * rhs.v) / 0xFFU;
    lhs.g = (lhs.g * rhs.v) / 0xFFU;
    lhs.b = (lhs.b * rhs.v) / 0xFFU;
    return lhs;
}

inline color24_t        operator*(      color24_t  lhs, const color16_t &rhs)
{
    lhs.r = (lhs.r * rhs.R()) / 0xFFU;
    lhs.g = (lhs.g * rhs.G()) / 0xFFU;
    lhs.b = (lhs.b * rhs.B()) / 0xFFU;
    return lhs;
}

inline color24_t        operator*(      color24_t  lhs, const color24_t &rhs)
{
    lhs.r = (lhs.r * rhs.r) / 0xFFU;
    lhs.g = (lhs.g * rhs.g) / 0xFFU;
    lhs.b = (lhs.b * rhs.b) / 0xFFU;
    return lhs;
}

inline color32_t        operator*(const color24_t &lhs,       color32_t  rhs)
{
    rhs.r = (rhs.r * lhs.r) / 0xFFU;
    rhs.g = (rhs.g * lhs.g) / 0xFFU;
    rhs.b = (rhs.b * lhs.b) / 0xFFU;
    return rhs;
}

// color32_t

inline color32_t        operator*(      color32_t  lhs, const alpha8_t  &rhs)
{
    lhs.a = (lhs.a * rhs.a) / 0xFFU;
    return lhs;
}

inline color32_t        operator*(      color32_t  lhs, const value8_t  &rhs)
{
    lhs.r = (lhs.r * rhs.v) / 0xFFU;
    lhs.g = (lhs.g * rhs.v) / 0xFFU;
    lhs.b = (lhs.b * rhs.v) / 0xFFU;
    return lhs;
}

inline color32_t        operator*(      color32_t  lhs, const color16_t &rhs)
{
    lhs.r = (lhs.r * rhs.R()) / 0xFFU;
    lhs.g = (lhs.g * rhs.G()) / 0xFFU;
    lhs.b = (lhs.b * rhs.B()) / 0xFFU;
    return lhs;
}

inline color32_t        operator*(      color32_t  lhs, const color24_t &rhs)
{
    lhs.r = (lhs.r * rhs.r) / 0xFFU;
    lhs.g = (lhs.g * rhs.g) / 0xFFU;
    lhs.b = (lhs.b * rhs.b) / 0xFFU;
    return lhs;
}

inline color32_t        operator*(      color32_t  lhs, const color32_t &rhs)
{
    lhs.r = (lhs.r * rhs.r) / 0xFFU;
    lhs.g = (lhs.g * rhs.g) / 0xFFU;
    lhs.b = (lhs.b * rhs.b) / 0xFFU;
    lhs.a = (lhs.a * rhs.a) / 0xFFU;
    return lhs;
}

// color16_alpha8_t

inline color16_alpha8_t operator*(color16_alpha8_t lhs, const color16_alpha8_t &rhs)
{
    lhs.color = lhs.color * rhs.color;
    lhs.alpha = lhs.alpha * rhs.alpha;
    return lhs;
}

inline color16_alpha8_t operator*(color16_alpha8_t lhs, const alpha8_t  &rhs)
{
    lhs.alpha = lhs.alpha * rhs;
    return lhs;
}

inline color16_alpha8_t operator*(color16_alpha8_t lhs, const value8_t  &rhs)
{
    lhs.color = lhs.color * rhs;
    return lhs;
}

inline color16_alpha8_t operator*(color16_alpha8_t lhs, const color16_t &rhs)
{
    lhs.color = lhs.color * rhs;
    return lhs;
}

inline color32_t        operator*(const color16_alpha8_t &lhs, color24_t rhs)
{
    rhs = lhs.color * rhs;
    return {rhs.r, rhs.g, rhs.b, lhs.alpha.a};
}

inline color32_t        operator*(const color16_alpha8_t &lhs, color32_t rhs)
{
    rhs = lhs.color * (lhs.alpha * rhs);
    return rhs;
}

inline color16_alpha8_t operator*(const alpha8_t  &lhs, color16_alpha8_t rhs)
{
    rhs.alpha = lhs * rhs.alpha;
    return rhs;
}

inline color16_alpha8_t operator*(const value8_t  &lhs, color16_alpha8_t rhs)
{
    rhs.color = lhs * rhs.color;
    return rhs;
}

inline color16_alpha8_t operator*(const color16_t &lhs, color16_alpha8_t rhs)
{
    rhs.color = lhs * rhs.color;
    return rhs;
}

inline color32_t        operator*(      color24_t  lhs, const color16_alpha8_t &rhs)
{
    lhs = lhs * rhs.color;
    return {lhs.r, lhs.g, lhs.b, rhs.alpha.a};
}

inline color32_t        operator*(      color32_t  lhs, const color16_alpha8_t &rhs)
{
    lhs = (lhs * rhs.color) * rhs.alpha;
    return lhs;
}

// alpha blend

//
// rhsA == 1 -> outRGBA = rhsRGBA
//
// lhsA == 1 -> outRGB = lhsRGB (1 - rhsA) + rhsRGB rhsA
//
// outA = lhsA (1 - rhsA) + rhsA
// outA == 0 -> outRGB = 0
// outA != 0 -> outRGB = (lhsRGB lhsA (1 - rhsA) + rhsRGB rhsA) / outA
//

//
// a + ((f * (b - a)) / 0xFF)
// a + (((f * b) - (f * a)) / 0xFF)
// a - ((f * a) / 0xFF) + ((f * b) / 0xFF)
// ((a * (0xFF - f)) / 0xFF) + ((f * b) / 0xFF)
// ((b * f) / 0xFF) + ((a * (0xFF - f)) / 0xFF)
//

// alpha8_t

inline alpha8_t         operator%(const alpha8_t  &lhs,       alpha8_t   rhs)
{
    if (lhs.a == 0xFFU || rhs.a == 0xFFU)
    {
        return alpha8_t{0xFFU};
    }
    else
    {
        rhs.a += (lhs.a * (0xFFU - rhs.a)) / 0xFFU;
    }
    return lhs;
}

inline value8_t         operator%(const alpha8_t  &lhs,       value8_t   rhs)
{
    return rhs;
}

inline color16_t        operator%(const alpha8_t  &lhs,       color16_t  rhs)
{
    return rhs;
}

inline color24_t        operator%(const alpha8_t  &lhs,       color24_t  rhs)
{
    return rhs;
}

inline color32_t        operator%(const alpha8_t  &lhs,       color32_t  rhs)
{
    if (rhs.a == 0xFFU)
    {
        return rhs;
    }
    else if (lhs.a == 0xFFU)
    {
        rhs.r = (0xFFU - rhs.a) + ((rhs.r * rhs.a) / 0xFF);
        rhs.g = (0xFFU - rhs.a) + ((rhs.g * rhs.a) / 0xFF);
        rhs.b = (0xFFU - rhs.a) + ((rhs.b * rhs.a) / 0xFF);
    }
    else if (rhs.a == 0x00U)
    {
        return {0xFFU, 0xFFU, 0xFFU, lhs.a};
    }
    else
    {
        const uint8_t outA = ((lhs.a * (0xFFU - rhs.a)) / 0xFFU) + rhs.a;
        if (outA)
        {
            const uint8_t srcRGB = lhs.a;
            rhs.r = ((srcRGB * 0xFF) + (rhs.a * (rhs.r - srcRGB))) / outA;
            rhs.g = ((srcRGB * 0xFF) + (rhs.a * (rhs.g - srcRGB))) / outA;
            rhs.b = ((srcRGB * 0xFF) + (rhs.a * (rhs.b - srcRGB))) / outA;
        }
        rhs.a = outA;
    }
    return rhs;
}

// value8_t

inline value8_t         operator%(      value8_t   lhs, const alpha8_t  &rhs)
{
    lhs.v += rhs.a - ((lhs.v * rhs.a) / 0xFFU);
    return lhs;
}

inline value8_t         operator%(const value8_t  &lhs,       value8_t   rhs)
{
    return rhs;
}

inline color16_t        operator%(const value8_t  &lhs,       color16_t  rhs)
{
    return rhs;
}

inline color24_t        operator%(const value8_t  &lhs,       color24_t  rhs)
{
    return rhs;
}

inline color32_t        operator%(const value8_t  &lhs,       color32_t  rhs)
{
    if (rhs.a == 0xFFU)
    {
        return rhs;
    }
    else if (rhs.a == 0x00U)
    {
        return {lhs.v, lhs.v, lhs.v, 0xFFU};
    }
    else
    {
        const uint8_t v = ((0xFFU - rhs.a) * lhs.v) / 0xFFU;
        rhs.r = v + ((rhs.r * rhs.a) / 0xFFU);
        rhs.g = v + ((rhs.g * rhs.a) / 0xFFU);
        rhs.b = v + ((rhs.b * rhs.a) / 0xFFU);
    }
    return rhs;
}

// color16_t

inline color16_t        operator%(      color16_t  lhs, const alpha8_t  &rhs)
{
    lhs.rgb =   C16R(C16R(lhs.rgb) + ((rhs.a * (C16RMASK - C16R(lhs.rgb))) / 0xFFU)) |
                C16G(C16G(lhs.rgb) + ((rhs.a * (C16GMASK - C16G(lhs.rgb))) / 0xFFU)) |
                C16B(C16B(lhs.rgb) + ((rhs.a * (C16BMASK - C16B(lhs.rgb))) / 0xFFU));
    return lhs;
}

inline value8_t         operator%(const color16_t &lhs,       value8_t   rhs)
{
    return rhs;
}

inline color16_t        operator%(const color16_t &lhs,       color16_t  rhs)
{
    return rhs;
}

inline color24_t        operator%(const color16_t &lhs,       color24_t  rhs)
{
    return rhs;
}

inline color32_t        operator%(const color16_t &lhs,       color32_t  rhs)
{
    if (rhs.a == 0xFFU)
    {
        return rhs;
    }
    else if (rhs.a == 0x00U)
    {
        return {lhs.R(), lhs.G(), lhs.B(), 0xFFU};
    }
    else
    {
        rhs.r = (((0xFFU - rhs.a) * lhs.R()) / 0xFFU) + ((rhs.r * rhs.a) / 0xFFU);
        rhs.g = (((0xFFU - rhs.a) * lhs.G()) / 0xFFU) + ((rhs.g * rhs.a) / 0xFFU);
        rhs.b = (((0xFFU - rhs.a) * lhs.B()) / 0xFFU) + ((rhs.b * rhs.a) / 0xFFU);
    }
    return rhs;
}

// color24_t

inline color24_t        operator%(      color24_t  lhs, const alpha8_t  &rhs)
{
    lhs.r += (rhs.a * (0xFFU - lhs.r)) / 0xFFU;
    lhs.g += (rhs.a * (0xFFU - lhs.g)) / 0xFFU;
    lhs.b += (rhs.a * (0xFFU - lhs.b)) / 0xFFU;
    return lhs;
}

inline value8_t         operator%(const color24_t &lhs,       value8_t   rhs)
{
    return rhs;
}

inline color16_t        operator%(const color24_t &lhs,       color16_t  rhs)
{
    return rhs;
}

inline color24_t        operator%(const color24_t &lhs,       color24_t  rhs)
{
    return rhs;
}

inline color32_t        operator%(const color24_t &lhs,       color32_t  rhs)
{
    if (rhs.a == 0xFFU)
    {
        return rhs;
    }
    else if (rhs.a == 0x00U)
    {
        return {lhs.r, lhs.g, lhs.b, 0xFFU};
    }
    else
    {
        rhs.r = (((0xFFU - rhs.a) * lhs.r) / 0xFFU) + ((rhs.r * rhs.a) / 0xFFU);
        rhs.g = (((0xFFU - rhs.a) * lhs.g) / 0xFFU) + ((rhs.g * rhs.a) / 0xFFU);
        rhs.b = (((0xFFU - rhs.a) * lhs.b) / 0xFFU) + ((rhs.b * rhs.a) / 0xFFU);
    }
    return rhs;
}


// color32_t

inline color32_t        operator%(      color32_t  lhs, const alpha8_t  &rhs)
{
    if (rhs.a == 0xFFU)
    {
        return {0xFFU, 0xFFU, 0xFFU, rhs.a};
    }
    else if (rhs.a == 0x00U)
    {
        return lhs;
    }
    else if (lhs.a == 0xFFU)
    {
        lhs.r += rhs.a - ((lhs.r * rhs.a) / 0xFFU);
        lhs.g += rhs.a - ((lhs.g * rhs.a) / 0xFFU);
        lhs.b += rhs.a - ((lhs.b * rhs.a) / 0xFFU);
    }
    else
    {
        const uint8_t outA = ((lhs.a * (0xFFU - rhs.a)) / 0xFFU) + rhs.a;
        if (outA)
        {
            lhs.r = (((((lhs.r * lhs.a) / 0xFFU) * (0xFFU - rhs.a))) + (0xFFU * rhs.a)) / outA;
            lhs.g = (((((lhs.g * lhs.a) / 0xFFU) * (0xFFU - rhs.a))) + (0xFFU * rhs.a)) / outA;
            lhs.b = (((((lhs.b * lhs.a) / 0xFFU) * (0xFFU - rhs.a))) + (0xFFU * rhs.a)) / outA;
        }
        lhs.a = outA;
    }
    return lhs;
}

inline value8_t         operator%(const color32_t &lhs,       value8_t   rhs)
{
    return rhs;
}

inline color16_t        operator%(const color32_t &lhs,       color16_t  rhs)
{
    return rhs;
}

inline color24_t        operator%(const color32_t &lhs,       color24_t  rhs)
{
    return rhs;
}

inline color32_t        operator%(      color32_t  lhs, const color32_t &rhs)
{
    if (rhs.a == 0xFFU)
    {
        return rhs;
    }
    else if (rhs.a == 0x00U)
    {
        return lhs;
    }
    else if (lhs.a == 0xFFU)
    {
        lhs.r += ((rhs.a * (rhs.r - lhs.r)) / 0xFFU);
        lhs.g += ((rhs.a * (rhs.g - lhs.g)) / 0xFFU);
        lhs.b += ((rhs.a * (rhs.b - lhs.b)) / 0xFFU);
    }
    else
    {
        const uint8_t outA = ((lhs.a * (0xFFU - rhs.a)) / 0xFFU) + rhs.a;
        if (outA)
        {
            lhs.r = (((((lhs.r * lhs.a) / 0xFFU) * (0xFFU - rhs.a))) + (rhs.r * rhs.a)) / outA;
            lhs.g = (((((lhs.g * lhs.a) / 0xFFU) * (0xFFU - rhs.a))) + (rhs.g * rhs.a)) / outA;
            lhs.b = (((((lhs.b * lhs.a) / 0xFFU) * (0xFFU - rhs.a))) + (rhs.b * rhs.a)) / outA;
        }
        lhs.a = outA;
    }
    return lhs;
}


// color16_alpha8_t

inline color16_alpha8_t operator%(color16_alpha8_t lhs, const color16_alpha8_t &rhs)
{
    if (rhs.alpha.a == 0xFFU)
    {
        return rhs;
    }
    else if (rhs.alpha.a == 0x00U)
    {
        return lhs;
    }
    else if (lhs.alpha.a == 0xFFU)
    {
        lhs.color.rgb = C16R(((C16R(lhs.color.rgb) * (0xFFU - rhs.alpha.a)) + (C16R(rhs.color.rgb) * rhs.alpha.a)) / 0xFFU) |
                        C16G(((C16G(lhs.color.rgb) * (0xFFU - rhs.alpha.a)) + (C16G(rhs.color.rgb) * rhs.alpha.a)) / 0xFFU) |
                        C16B(((C16B(lhs.color.rgb) * (0xFFU - rhs.alpha.a)) + (C16B(rhs.color.rgb) * rhs.alpha.a)) / 0xFFU);
        lhs.alpha = lhs.alpha % rhs.alpha;
    }
    else
    {
        const uint8_t outA = (lhs.alpha % rhs.alpha).a;
        if (outA)
        {
            lhs.color.rgb = C16R(((((C16R(lhs.color.rgb) * lhs.alpha.a) / 0xFFU) * (0xFFU - rhs.alpha.a)) + (C16R(rhs.color.rgb) * rhs.alpha.a)) / outA) |
                            C16G(((((C16G(lhs.color.rgb) * lhs.alpha.a) / 0xFFU) * (0xFFU - rhs.alpha.a)) + (C16G(rhs.color.rgb) * rhs.alpha.a)) / outA) |
                            C16B(((((C16B(lhs.color.rgb) * lhs.alpha.a) / 0xFFU) * (0xFFU - rhs.alpha.a)) + (C16B(rhs.color.rgb) * rhs.alpha.a)) / outA);
        }
        lhs.alpha.a = outA;
    }
    return lhs;
}

inline color16_alpha8_t operator%(color16_alpha8_t lhs, const alpha8_t  &rhs)
{
    lhs.color = lhs.color % rhs;
    lhs.alpha = lhs.alpha % rhs;
    return lhs;
}

inline value8_t         operator%(const color16_alpha8_t &lhs, value8_t  rhs)
{
    return rhs;
}

inline color16_t        operator%(const color16_alpha8_t &lhs, color16_t rhs)
{
    return rhs;
}

inline color24_t        operator%(const color16_alpha8_t &lhs, color24_t rhs)
{
    return rhs;
}

inline color32_t        operator%(const color16_alpha8_t &lhs, color32_t rhs)
{
    if (rhs.a == 0xFFU)
    {
        return rhs;
    }
    else if (rhs.a == 0x00U)
    {
        return {lhs.color.R(), lhs.color.G(), lhs.color.B(), lhs.alpha.a};
    }
    else if (lhs.alpha.a == 0xFFU)
    {
        rhs.r = ((lhs.color.R() * (0xFFU - rhs.a)) / 0xFFU) + ((rhs.r * rhs.a) / 0xFFU);
        rhs.g = ((lhs.color.G() * (0xFFU - rhs.a)) / 0xFFU) + ((rhs.g * rhs.a) / 0xFFU);
        rhs.b = ((lhs.color.B() * (0xFFU - rhs.a)) / 0xFFU) + ((rhs.b * rhs.a) / 0xFFU);
    }
    else
    {
        const uint8_t outA = ((lhs.alpha.a * (0xFFU - rhs.a)) / 0xFFU) + rhs.a;
        if (outA)
        {
            rhs.r = (((((lhs.color.R() * lhs.alpha.a) / 0xFFU) * (0xFFU - rhs.a))) + (rhs.r * rhs.a)) / outA;
            rhs.g = (((((lhs.color.G() * lhs.alpha.a) / 0xFFU) * (0xFFU - rhs.a))) + (rhs.g * rhs.a)) / outA;
            rhs.b = (((((lhs.color.B() * lhs.alpha.a) / 0xFFU) * (0xFFU - rhs.a))) + (rhs.b * rhs.a)) / outA;
        }
        rhs.a = outA;
    }
    return rhs;
}

inline color16_alpha8_t operator%(const alpha8_t  &lhs, color16_alpha8_t rhs)
{
    if (rhs.alpha.a == 0xFFU)
    {
        return rhs;
    }
    else if (rhs.alpha.a == 0x00U)
    {
        return {0xFFFFU, lhs};
    }
    else if (lhs.a == 0xFFU)
    {
        rhs.color.rgb = C16R(((C16RMASK * (0xFFU - rhs.alpha.a)) + (C16R(rhs.color.rgb) * rhs.alpha.a)) / 0xFFU) |
                        C16G(((C16GMASK * (0xFFU - rhs.alpha.a)) + (C16G(rhs.color.rgb) * rhs.alpha.a)) / 0xFFU) |
                        C16B(((C16BMASK * (0xFFU - rhs.alpha.a)) + (C16B(rhs.color.rgb) * rhs.alpha.a)) / 0xFFU);
        rhs.alpha = lhs % rhs.alpha;
    }
    else
    {
        const uint8_t outA = (lhs % rhs.alpha).a;
        if (outA)
        {
            rhs.color.rgb = C16R(((((C16RMASK * lhs.a) / 0xFFU) * (0xFFU - rhs.alpha.a)) + (C16R(rhs.color.rgb) * rhs.alpha.a)) / outA) |
                            C16G(((((C16GMASK * lhs.a) / 0xFFU) * (0xFFU - rhs.alpha.a)) + (C16G(rhs.color.rgb) * rhs.alpha.a)) / outA) |
                            C16B(((((C16BMASK * lhs.a) / 0xFFU) * (0xFFU - rhs.alpha.a)) + (C16B(rhs.color.rgb) * rhs.alpha.a)) / outA);
        }
        rhs.alpha.a = outA;
    }
    return rhs;
}

inline color16_t        operator%(const value8_t  &lhs, color16_alpha8_t rhs)
{
    if (rhs.alpha.a == 0xFFU)
    {
        return rhs.color;
    }
    else if (rhs.alpha.a == 0x00U)
    {
        return {lhs.RGB16()};
    }
    else
    {
        rhs.color.rgb = C16R(((((C16RMASK * lhs.v) / 0xFFU) * (0xFFU - rhs.alpha.a)) + (C16R(rhs.color.rgb) * rhs.alpha.a)) / 0xFFU) |
                        C16G(((((C16GMASK * lhs.v) / 0xFFU) * (0xFFU - rhs.alpha.a)) + (C16G(rhs.color.rgb) * rhs.alpha.a)) / 0xFFU) |
                        C16B(((((C16BMASK * lhs.v) / 0xFFU) * (0xFFU - rhs.alpha.a)) + (C16B(rhs.color.rgb) * rhs.alpha.a)) / 0xFFU);
    }
    return rhs.color;
}

inline color16_t        operator%(      color16_t  lhs, const color16_alpha8_t &rhs)
{
    if (rhs.alpha.a == 0xFFU)
    {
        return rhs.color;
    }
    else if (rhs.alpha.a == 0x00U)
    {
        return lhs;
    }
    else
    {
        lhs.rgb =   C16R(((C16R(lhs.rgb) * (0xFFU - rhs.alpha.a)) + (C16R(rhs.color.rgb) * rhs.alpha.a)) / 0xFFU) |
                    C16G(((C16G(lhs.rgb) * (0xFFU - rhs.alpha.a)) + (C16G(rhs.color.rgb) * rhs.alpha.a)) / 0xFFU) |
                    C16B(((C16B(lhs.rgb) * (0xFFU - rhs.alpha.a)) + (C16B(rhs.color.rgb) * rhs.alpha.a)) / 0xFFU);
    }
    return lhs;
}

inline color24_t        operator%(      color24_t  lhs, const color16_alpha8_t &rhs)
{
    if (rhs.alpha.a == 0xFFU)
    {
        return {rhs.color.R(), rhs.color.G(), rhs.color.B()};
    }
    else if (rhs.alpha.a == 0x00U)
    {
        return lhs;
    }
    else
    {
        lhs.r += ((rhs.alpha.a * (rhs.color.R() - lhs.r)) / 0xFFU);
        lhs.g += ((rhs.alpha.a * (rhs.color.G() - lhs.g)) / 0xFFU);
        lhs.b += ((rhs.alpha.a * (rhs.color.B() - lhs.b)) / 0xFFU);
    }
    return lhs;
}

inline color32_t        operator%(      color32_t  lhs, const color16_alpha8_t &rhs)
{
    if (rhs.alpha.a == 0xFFU)
    {
        return {rhs.color.R(), rhs.color.G(), rhs.color.B(), rhs.alpha.a};
    }
    else if (rhs.alpha.a == 0x00U)
    {
        return lhs;
    }
    else if (lhs.a == 0xFFU)
    {
        lhs.r += ((rhs.alpha.a * (rhs.color.R() - lhs.r)) / 0xFFU);
        lhs.g += ((rhs.alpha.a * (rhs.color.G() - lhs.g)) / 0xFFU);
        lhs.b += ((rhs.alpha.a * (rhs.color.B() - lhs.b)) / 0xFFU);
    }
    else
    {
        const uint8_t outA = ((lhs.a * (0xFFU - rhs.alpha.a)) / 0xFFU) + rhs.alpha.a;
        if (outA)
        {
            lhs.r = (((((lhs.r * lhs.a) / 0xFFU) * (0xFFU - rhs.alpha.a))) + (rhs.color.R() * rhs.alpha.a)) / outA;
            lhs.g = (((((lhs.g * lhs.a) / 0xFFU) * (0xFFU - rhs.alpha.a))) + (rhs.color.G() * rhs.alpha.a)) / outA;
            lhs.b = (((((lhs.b * lhs.a) / 0xFFU) * (0xFFU - rhs.alpha.a))) + (rhs.color.B() * rhs.alpha.a)) / outA;
        }
        lhs.a = outA;
    }
    return lhs;
}

#undef C16R
#undef C16G
#undef C16B

#undef C16RMASK
#undef C16GMASK
#undef C16BMASK

#endif // SOFTRASTER_COLOR_H