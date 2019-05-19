#ifndef COLOR_HPP
#define COLOR_HPP

#ifdef ARDUINO
#   pragma GCC optimize("-03")
#   define INLINE_DEC(F) inline F __attribute__((always_inline))
#else
#   define INLINE_DEC(F) inline F
#endif
#define INLINE_DEF(F) inline F
#define INLINE(F) INLINE_DEC(F); INLINE_DEF(F)
#define INLINE_TEMPLATE(T, F) T INLINE_DEC(F); T INLINE_DEF(F)

#define C16RMASK 0xF800
#define C16GMASK 0x07E0
#define C16BMASK 0x001F

#define C16R(C) ((C) & C16RMASK)
#define C16G(C) ((C) & C16GMASK)
#define C16B(C) ((C) & C16BMASK)

// Linear interpolation functions
INLINE_TEMPLATE(template<typename T>, T lerp(T a, T b, uint8_t f)) // [0, 255]
{
    if (a == b) return a;
    if (f == 0x00) return a;
    else if (f == 0xFF) return b;
    return a + ((f * (b - a)) / 0xFF);
}

INLINE_TEMPLATE(template<typename T>, T lerp(T a, T b, float f)) // [0.0f, 1.0f]
{
    if (a == b) return a;
    if (f <= 0.0f) return a;
    else if (f >= 1.0f) return b;
    return a + (f * (b - a));
}

struct alpha8_t;
struct value8_t;
struct color16_t;
struct color24_t;
struct color32_t;

struct alpha8_t
{
    uint8_t a;

    alpha8_t() = default;
    alpha8_t(const uint8_t alpha) : a(alpha) {}
    alpha8_t(const alpha8_t &rhs) : a(rhs.a) {}

    template<typename COLOR>
    explicit inline alpha8_t(const COLOR &rhs) : a(rhs.A()) { }

    template<typename COLOR>
    inline alpha8_t &operator=(const COLOR &rhs)
    {
        a = rhs.A();
        return *this;
    }

    template<typename COLOR>
    inline alpha8_t &operator+=(const COLOR &rhs)
    {
        a += rhs.A();
        return *this;
    }

    template<typename COLOR>
    inline alpha8_t &operator*=(const COLOR &rhs)
    {
        a = (a * rhs.V()) / 0xFF;
        return *this;
    }

    template<typename COLOR>
    inline alpha8_t &operator%=(const COLOR &rhs)
    {
        a = (a * rhs.A()) / 0xFF;
        return *this;
    }

    inline alpha8_t &operator*=(const float rhs) { a *= rhs; return *this; }

    inline alpha8_t &operator%=(const float rhs) { a *= rhs; return *this; }

    inline bool operator==(const alpha8_t &rhs) const { return a == rhs.a; }
    inline bool operator!=(const alpha8_t &rhs) const { return a != rhs.a; }

    inline uint8_t R() const { return 0xFF; }
    inline uint8_t G() const { return 0xFF; }
    inline uint8_t B() const { return 0xFF; }
    inline uint8_t A() const { return a; }
    inline uint8_t V() const { return a; }
    inline uint16_t RGB16() const
    { return (((a * 0x1F) / 0xFF) << 0xB) | (((a * 0x3F) / 0xFF) << 0x5) | ((a * 0x1F) / 0xFF); }
    inline uint32_t RGBA32() const { return 0xFFFFFF00 | a; }
};

template<typename COLOR>
inline alpha8_t operator+(alpha8_t lhs, const COLOR &rhs)
{
    return lhs += rhs;
}

template<typename COLOR>
inline alpha8_t operator*(alpha8_t lhs, const COLOR &rhs)
{
    return lhs *= rhs;
}

template<typename COLOR>
inline alpha8_t operator%(alpha8_t lhs, const COLOR &rhs)
{
    return lhs %= rhs;
}

inline alpha8_t operator*(alpha8_t lhs, const float rhs)
{
    return lhs *= rhs;
}

inline alpha8_t operator%(alpha8_t lhs, const float rhs)
{
    lhs.a *= rhs;
    return lhs;
}

struct value8_t
{
    uint8_t v;

    value8_t() = default;
    value8_t(const uint8_t value) : v(value) {}
    value8_t(const value8_t &rhs) : v(rhs.v) {}

    template<typename COLOR>
    explicit inline value8_t(const COLOR &rhs) : v(rhs.V()) { }

    template<typename COLOR>
    inline value8_t &operator=(const COLOR &rhs) { v = rhs.V(); return *this; }

    template<typename COLOR>
    inline value8_t &operator+=(const COLOR &rhs) { v += rhs.V(); return *this; }

    template<typename COLOR>
    inline value8_t &operator*=(const COLOR &rhs) { v = (v * rhs.V()) / 0xFF; return *this; }

    template<typename COLOR>
    inline value8_t &operator%=(const COLOR &rhs) { v = lerp(v, rhs.V(), rhs.A()); return *this; }

    inline value8_t &operator*=(const float rhs) { v *= rhs; return *this; }

    inline value8_t &operator%=(const float rhs) { return *this; }

    inline bool operator==(const value8_t &rhs) const { return v == rhs.v; }
    inline bool operator!=(const value8_t &rhs) const { return v != rhs.v; }

    inline uint8_t R() const { return v; }
    inline uint8_t G() const { return v; }
    inline uint8_t B() const { return v; }
    inline uint8_t A() const { return 0xFF; }
    inline uint8_t V() const { return v; }
    inline uint16_t RGB16() const
    { return (((v * 0x1F) / 0xFF) << 0xB) | (((v * 0x3F) / 0xFF) << 0x5) | ((v * 0x1F) / 0xFF); }
    inline uint32_t RGBA32() const { (v << 24) | (v << 16) | (v << 8) | v;}
};

template<typename COLOR>
inline value8_t operator+(value8_t lhs, const COLOR &rhs)
{
    return lhs += rhs;
}

template<typename COLOR>
inline value8_t operator*(value8_t lhs, const COLOR &rhs)
{
    return lhs *= rhs;
}

template<typename COLOR>
inline value8_t operator%(value8_t lhs, const COLOR &rhs)
{
    return lhs %= rhs;
}

inline value8_t operator*(value8_t lhs, const float rhs)
{
    lhs.v *= rhs;
    return lhs;
}

inline value8_t operator%(value8_t lhs, const float rhs)
{
    return lhs;
}

struct color16_t
{
    uint16_t rgb;

    color16_t() = default;
    color16_t(const uint16_t val) : rgb(val) {}
    color16_t(const color16_t &rhs) : rgb(rhs.rgb) {}

    template<typename COLOR>
    explicit inline color16_t(const COLOR &rhs) : rgb(rhs.RGB16()) { }

    template<typename COLOR>
    inline color16_t &operator=(const COLOR &rhs) { rgb = rhs.RGB16(); return *this; }

    template<typename COLOR>
    inline color16_t &operator+=(const COLOR &rhs)
    {
        const uint16_t v = rhs.RGB16();
        rgb = C16R(C16R(rgb) + C16R(v)) |
              C16G(C16G(rgb) + C16G(v)) |
              C16B(C16B(rgb) + C16B(v));
        return *this;
    }

    template<typename COLOR>
    inline color16_t &operator*=(const COLOR &rhs)
    {
        const uint16_t v = rhs.RGB16();
        rgb = C16R((C16R(rgb) + C16R(v)) / C16RMASK) |
              C16G((C16G(rgb) + C16G(v)) / C16GMASK) |
              C16B((C16B(rgb) + C16B(v)) / C16BMASK);
        return *this;
    }

    template<typename COLOR>
    inline color16_t &operator%=(const COLOR &rhs)
    {
        const uint16_t v = rhs.RGB16();
        const uint8_t a = rhs.A();
        if (a)
        {
            rgb = C16R(lerp(C16R(rgb), C16R(v), a)) |
                  C16G(lerp(C16G(rgb), C16G(v), a)) |
                  C16B(lerp(C16B(rgb), C16B(v), a));
        }
        return *this;
    }

    inline color16_t &operator*=(const float rhs)
    {
        rgb = C16R((uint16_t)(C16R(rgb) * rhs)) |
              C16G((uint16_t)(C16G(rgb) * rhs)) |
              C16B((uint16_t)(C16B(rgb) * rhs));
        return *this;
    }

    inline color16_t &operator%=(const float rhs)
    {
        return *this;
    }

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

template<typename COLOR>
inline color16_t operator+(color16_t lhs, const COLOR &rhs)
{
    return lhs += rhs;
}

template<typename COLOR>
inline color16_t operator*(color16_t lhs, const COLOR &rhs)
{
    return lhs *= rhs;
}

template<typename COLOR>
inline color16_t operator%(color16_t lhs, const COLOR &rhs)
{
    return lhs %= rhs;
}

inline color16_t operator*(color16_t lhs, const float rhs)
{
    lhs.rgb = C16R((uint16_t)(C16R(lhs.rgb) * rhs)) |
              C16G((uint16_t)(C16G(lhs.rgb) * rhs)) |
              C16B((uint16_t)(C16B(lhs.rgb) * rhs));
    return lhs;
}

inline color16_t operator%(color16_t lhs, const float rhs)
{
    return lhs;
}

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
    inline color24_t &operator=(const COLOR &rhs)
    {
        r = rhs.R();
        g = rhs.G();
        b = rhs.B();
        return *this;
    }

    template<typename COLOR>
    inline color24_t &operator+=(const COLOR &rhs)
    {
        r += rhs.R();
        g += rhs.G();
        b += rhs.B();
        return *this;
    }

    template<typename COLOR>
    inline color24_t &operator*=(const COLOR &rhs)
    {
        r = (r * rhs.R()) / 0xFF;
        g = (g * rhs.G()) / 0xFF;
        b = (b * rhs.B()) / 0xFF;
        return *this;
    }

    template<typename COLOR>
    inline color24_t &operator%=(const COLOR &rhs)
    {
        const uint8_t alpha = rhs.A();
        if (alpha)
        {
            r = lerp(r, rhs.R(), alpha);
            g = lerp(g, rhs.G(), alpha);
            b = lerp(b, rhs.B(), alpha);
        }
        return *this;
    }

    inline color24_t &operator*=(const float rhs)
    {
        r *= rhs;
        g *= rhs;
        b *= rhs;
        return *this;
    }

    inline color24_t &operator%=(const float rhs)
    {
        return *this;
    }

    inline bool operator==(const color24_t &rhs) const
    { return r == rhs.r && g == rhs.g && b == rhs.b; }
    inline bool operator!=(const color24_t &rhs) const
    { return r != rhs.r && g != rhs.g && b != rhs.b; }

    inline uint8_t R() const { return r; }
    inline uint8_t G() const { return g; }
    inline uint8_t B() const { return b; }
    inline uint8_t A() const { return 0xFF; }
    inline uint8_t V() const { return (R() + G() + B()) / 3; }
    inline uint16_t RGB16() const
    { return (((r * 0x1F) / 0xFF) << 0xB) | (((g * 0x3F) / 0xFF) << 0x5) | ((b * 0x1F) / 0xFF); }
    inline uint32_t RGBA32() const { return (r << 24) | (g << 16) | (b << 8) | 0xFF; }
};

template<typename COLOR>
inline color24_t operator+(color24_t lhs, const COLOR &rhs)
{
    return lhs += rhs;
}

template<typename COLOR>
inline color24_t operator*(color24_t lhs, const COLOR &rhs)
{
    return lhs *= rhs;
}

template<typename COLOR>
inline color24_t operator%(color24_t lhs, const COLOR &rhs)
{
    return lhs %= rhs;
}

inline color24_t operator*(color24_t lhs, const float rhs)
{
    lhs.r *= rhs;
    lhs.g *= rhs;
    lhs.b *= rhs;
    return lhs;
}

inline color24_t operator%(color24_t lhs, const float rhs)
{
    return lhs;
}

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
    inline color32_t &operator=(const COLOR &rhs)
    {
        r = rhs.R();
        g = rhs.G();
        b = rhs.B();
        a = rhs.A();
        return *this;
    }

    template<typename COLOR>
    inline color32_t &operator+=(const COLOR &rhs)
    {
        r += rhs.R();
        g += rhs.G();
        b += rhs.B();
        a += rhs.A();
        return *this;
    }

    template<typename COLOR>
    inline color32_t &operator*=(const COLOR &rhs)
    {
        r = (r * rhs.R()) / 0xFF;
        g = (g * rhs.G()) / 0xFF;
        b = (b * rhs.B()) / 0xFF;
        return *this;
    }

    template<typename COLOR>
    inline color32_t &operator%=(const COLOR &rhs)
    {
        const uint8_t alpha = rhs.A();
        if (alpha)
        {
            r = lerp(r, rhs.R(), alpha);
            g = lerp(g, rhs.G(), alpha);
            b = lerp(b, rhs.B(), alpha);
        }
        return *this;
    }

    inline color32_t &operator*=(const float rhs)
    {
        r *= rhs;
        g *= rhs;
        b *= rhs;
        return *this;
    }

    inline color32_t &operator%=(const float rhs)
    {
        a *= rhs;
        return *this;
    }

    inline bool operator==(const color32_t &rhs) const
    { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }
    inline bool operator!=(const color32_t &rhs) const
    { return r != rhs.r && g != rhs.g && b != rhs.b && a != rhs.a; }

    inline uint8_t R() const { return r; }
    inline uint8_t G() const { return g; }
    inline uint8_t B() const { return b; }
    inline uint8_t A() const { return a; }
    inline uint8_t V() const { return (R() + G() + B()) / 3; }
    inline uint16_t RGB16() const
    { return (((r * 0x1F) / 0xFF) << 0xB) | (((g * 0x3F) / 0xFF) << 0x5) | ((b * 0x1F) / 0xFF); }
    inline uint32_t RGBA32() const { return (r << 24) | (g << 16) | (b << 8) | a; }
};

template<typename COLOR>
inline color32_t operator+(color32_t lhs, const COLOR &rhs)
{
    return lhs += rhs;
}

template<typename COLOR>
inline color32_t operator*(color32_t lhs, const COLOR &rhs)
{
    return lhs *= rhs;
}

template<typename COLOR>
inline color32_t operator%(color32_t lhs, const COLOR &rhs)
{
    return lhs %= rhs;
}

inline color32_t operator*(color32_t lhs, const float rhs)
{
    lhs.r *= rhs;
    lhs.g *= rhs;
    lhs.b *= rhs;
    return lhs;
}

inline color32_t operator%(color32_t lhs, const float rhs)
{
    lhs.a *= rhs;
    return lhs;
}

#undef C16R
#undef C16G
#undef C16B

#undef C16RMASK
#undef C16GMASK
#undef C16BMASK

#undef INLINE_DEC
#undef INLINE_DEF
#undef INLINE
#undef INLINE_TEMPLATE

#endif // COLOR_HPP