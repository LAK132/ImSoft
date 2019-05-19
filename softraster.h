// Based on sronsse's software rasterizer for SDL https://github.com/sronsse/imgui/blob/sw_rasterizer_example/examples/sdl_sw_example/imgui_impl_sdl.cpp
#ifndef SOFTRASTER_H
#define SOFTRASTER_H

#include "stdint.h"
#include "math.h"
#include "imgui.h"
#include "color.hpp"

#ifdef ARDUINO
#   include "Arduino.h"
#   pragma GCC optimize("-03")
#   define INLINE_DEC(F) inline F __attribute__((always_inline))
#else
#   define INLINE_DEC(F) inline F
#endif
#define INLINE_DEF(F) inline F
#define INLINE(F) INLINE_DEC(F); INLINE_DEF(F)
#define INLINE_TEMPLATE(T, F) T INLINE_DEC(F); T INLINE_DEF(F)

#define position_t int16_t

struct range_t;
struct clip_t;
template<typename COLOR> struct pixel_t;
template<typename COLOR> struct triangle_t;
template<typename COLOR> struct quat_t;

struct texture_base_t;
template<typename COLOR> struct texture_t;
template<typename COLOR> struct bary_t;

enum texture_type_t { NONE = 0, ALPHA8, VALUE8, COLOR16, COLOR24, COLOR32 };

struct range_t
{
    position_t min, max;
};

struct clip_t
{
    range_t x, y;
};

template<typename COLOR>
struct pixel_t
{
    position_t x, y;
    float u, v;
    COLOR c;
};

template<typename COLOR>
struct triangle_t
{
    pixel_t<COLOR> p1, p2, p3;
};

template<typename COLOR>
struct quad_t
{
    pixel_t<COLOR> p1, p2;
};

struct texture_base_t
{
    texture_type_t type;
};

// Template the texture struct so we can use different color modes/types
template <typename COLOR>
struct texture_t : public texture_base_t
{
    size_t w = 0, h = 0;
    COLOR *pixels = nullptr;
    bool needFree = true;

    void clear()
    {
        memset(pixels, 0, w * h * sizeof(COLOR));
    }

    void init(size_t x, size_t y)
    {
        if (needFree && pixels != nullptr)
            free(pixels);
        w = x;
        h = y;
        pixels = (COLOR*)malloc(w * h * sizeof(COLOR));
        needFree = true;
    }

    void init(size_t x, size_t y, COLOR *data)
    {
        if (needFree && pixels != nullptr)
            free(pixels);
        w = x;
        h = y;
        pixels = data;
        needFree = false;
    }

    texture_t();

    texture_t(size_t x, size_t y);

    texture_t(size_t x, size_t y, COLOR *data);

    ~texture_t()
    {
        if (needFree && pixels != nullptr)
        {
            free(pixels);
            pixels = nullptr;
        }
    }

    INLINE_DEC(COLOR &at(size_t x, size_t y))
    {
        return pixels[x + (w * y)];
    }

    INLINE_DEC(const COLOR &at(size_t x, size_t y) const)
    {
        return pixels[x + (w * y)];
    }

    INLINE_DEC(COLOR &operator()(size_t x, size_t y))
    {
        return pixels[x + (w * y)];
    }

    INLINE_DEC(const COLOR &operator()(size_t x, size_t y) const)
    {
        return pixels[x + (w * y)];
    }
};

template<> texture_t<alpha8_t>::texture_t() : pixels(nullptr), needFree(false)  { type = texture_type_t::ALPHA8; }
template<> texture_t<alpha8_t>::texture_t(size_t x, size_t y)                   { type = texture_type_t::ALPHA8; init(x, y); }
template<> texture_t<alpha8_t>::texture_t(size_t x, size_t y, alpha8_t* data)   { type = texture_type_t::ALPHA8; init(x, y, data); }

template<> texture_t<value8_t>::texture_t() : pixels(nullptr), needFree(false)  { type = texture_type_t::VALUE8; }
template<> texture_t<value8_t>::texture_t(size_t x, size_t y)                   { type = texture_type_t::VALUE8; init(x, y); }
template<> texture_t<value8_t>::texture_t(size_t x, size_t y, value8_t* data)   { type = texture_type_t::VALUE8; init(x, y, data); }

template<> texture_t<color16_t>::texture_t() : pixels(nullptr), needFree(false) { type = texture_type_t::COLOR16; }
template<> texture_t<color16_t>::texture_t(size_t x, size_t y)                  { type = texture_type_t::COLOR16; init(x, y); }
template<> texture_t<color16_t>::texture_t(size_t x, size_t y, color16_t* data) { type = texture_type_t::COLOR16; init(x, y, data); }

template<> texture_t<color24_t>::texture_t() : pixels(nullptr), needFree(false) { type = texture_type_t::COLOR24; }
template<> texture_t<color24_t>::texture_t(size_t x, size_t y)                  { type = texture_type_t::COLOR24; init(x, y); }
template<> texture_t<color24_t>::texture_t(size_t x, size_t y, color24_t* data) { type = texture_type_t::COLOR24; init(x, y, data); }

template<> texture_t<color32_t>::texture_t() : pixels(nullptr), needFree(false) { type = texture_type_t::COLOR32; }
template<> texture_t<color32_t>::texture_t(size_t x, size_t y)                  { type = texture_type_t::COLOR32; init(x, y); }
template<> texture_t<color32_t>::texture_t(size_t x, size_t y, color32_t* data) { type = texture_type_t::COLOR32; init(x, y, data); }

template<typename COLOR>
struct bary_t
{
    pixel_t<COLOR> a, b, c, p0, p1;
    position_t d00, d01, d11;
    float denom;
};

template<typename T>
inline void swap(T* tri1, T* tri2)
{
    T temp;
    memcpy(&temp, tri1, sizeof(T));
    memcpy(tri1, tri2, sizeof(T));
    memcpy(tri2, &temp, sizeof(T));
}

INLINE_TEMPLATE(template<typename T>, T inl_min(const T a, const T b))
{
    return a > b ? b : a;
}

INLINE_TEMPLATE(template<typename T>, T inl_max(const T a, const T b))
{
    return a > b ? a : b;
}

INLINE(range_t inl_min(const range_t &a, const range_t &b))
{
    return {
        inl_max(a.min, b.min),
        inl_min(a.max, b.max)
    };
}

INLINE(range_t inl_max(const range_t &a, const range_t &b))
{
    return {
        inl_min(a.min, b.min),
        inl_max(a.max, b.max)
    };
}

INLINE_TEMPLATE(template<typename COLOR>, position_t dot(const pixel_t<COLOR>& a, const pixel_t<COLOR>& b))
{
    return (a.x * b.x) + (a.y * b.y);
}

template<typename COLOR>
inline bary_t<COLOR> baryPre(
    const pixel_t<COLOR>& a,
    const pixel_t<COLOR>& b,
    const pixel_t<COLOR>& c)
{
    bary_t<COLOR> bary;
    bary.p0.x = b.x - a.x;
    bary.p0.y = b.y - a.x;
    bary.p1.x = c.x - a.x;
    bary.p1.y = c.y - a.y;
    bary.d00 = dot(bary.p0, bary.p0);
    bary.d01 = dot(bary.p0, bary.p1);
    bary.d11 = dot(bary.p1, bary.p1);
    bary.denom = 1.0f / ((bary.d00 * bary.d11) - (bary.d01 * bary.d01));
    bary.a = a;
    bary.b = b;
    bary.c = c;
    return bary;
}

template<typename COLOR>
inline void barycentricCol(pixel_t<COLOR>& p, const bary_t<COLOR>& bary)
{
    pixel_t<COLOR> p2; p2.x = p.x - bary.a.x; p2.y = p.y - bary.a.y;
    position_t d20 = dot(p2, bary.p0);
    position_t d21 = dot(p2, bary.p1);
    float v = (bary.d11 * d20 - bary.d01 * d21) * bary.denom;
    float w = (bary.d00 * d21 - bary.d01 * d20) * bary.denom;
    float u = 1.0f - v - w;
    p.c = (bary.a.c * u) + (bary.b.c * v) + (bary.c.c * w);
}

template<typename COLOR>
inline void barycentricUV(pixel_t<COLOR>& p, const bary_t<COLOR>& bary)
{
    pixel_t<COLOR> p2; p2.x = p.x - bary.a.x; p2.y = p.y - bary.a.y;
    position_t d20 = dot(p2, bary.p0);
    position_t d21 = dot(p2, bary.p1);
    float v = (bary.d11 * d20 - bary.d01 * d21) * bary.denom;
    float w = (bary.d00 * d21 - bary.d01 * d20) * bary.denom;
    float u = 1.0f - v - w;
    p.u = (bary.a.u * u) + (bary.b.u * v) + (bary.c.u * w);
    p.v = (bary.a.v * u) + (bary.b.v * v) + (bary.c.v * w);
}

template<typename COLOR>
inline void barycentricUVCol(pixel_t<COLOR>& p, const bary_t<COLOR>& bary)
{
    pixel_t<COLOR> p2; p2.x = p.x - bary.a.x; p2.y = p.y - bary.a.y;
    position_t d20 = dot(p2, bary.p0);
    position_t d21 = dot(p2, bary.p1);
    float v = (bary.d11 * d20 - bary.d01 * d21) * bary.denom;
    float w = (bary.d00 * d21 - bary.d01 * d20) * bary.denom;
    float u = 1.0f - v - w;
    p.u = (bary.a.u * u) + (bary.b.u * v) + (bary.c.u * w);
    p.v = (bary.a.v * u) + (bary.b.v * v) + (bary.c.v * w);
    p.c = (bary.a.c * u) + (bary.b.c * v) + (bary.c.c * w);
}

template<typename SCREEN, typename TEXTURE, typename COLOR>
void renderQuadCore(
    texture_t<SCREEN>           &screen,
    const texture_t<TEXTURE>    &tex,
    const clip_t                &clip,
    const quad_t<COLOR>         &quad,
    const bool                  alphaBlend
)
{
    if ((quad.p2.x < clip.x.min) ||
        (quad.p2.y < clip.y.min) ||
        (quad.p1.x >= clip.x.max) ||
        (quad.p1.y >= clip.y.max))
        return;

    const range_t rx = inl_min({quad.p1.x, quad.p2.x}, clip.x);
    const range_t ry = inl_min({quad.p1.y, quad.p2.y}, clip.y);

    position_t p1u = (position_t)((quad.p1.u * tex.w) + 0.5f) % tex.w;
    position_t p1v = (position_t)((quad.p1.v * tex.h) + 0.5f) % tex.h;
    position_t p2u = (position_t)((quad.p2.u * tex.w) + 0.5f) % tex.w;
    position_t p2v = (position_t)((quad.p2.v * tex.h) + 0.5f) % tex.h;

    const float duDx = (float)(p2u - p1u) / (float)(quad.p2.x - quad.p1.x);
    const float dvDy = (float)(p2v - p1v) / (float)(quad.p2.y - quad.p1.y);

    const float startu = p1u + (duDx * (float)(rx.min - quad.p1.x > 0 ? rx.min - quad.p1.x : 0));
    const float startv = p1v + (dvDy * (float)(ry.min - quad.p1.y > 0 ? ry.min - quad.p1.y : 0));

    bool blit = ((duDx == 1.0f) && (dvDy == 1.0f));

    if (blit)
    {
        const position_t u = startu - rx.min;
        const position_t v = startv - ry.min;
        if (alphaBlend)
        {
            for (position_t y = ry.min; y < ry.max; ++y)
            {
                for (position_t x = rx.min; x < rx.max; ++x)
                {
                    screen.at(x, y) %= tex.at(x + u, y + v) * quad.p1.c;
                }
            }
        }
        else
        {
            for (position_t y = ry.min; y < ry.max; ++y)
            {
                for (position_t x = rx.min; x < rx.max; ++x)
                {
                    screen.at(x, y) = tex.at(x + u, y + v) * quad.p1.c;
                }
            }
        }
    }
    else
    {
        float u = startu;
        float v = startv;
        if (alphaBlend)
        {
            for (position_t y = ry.min; y < ry.max; ++y)
            {
                for (position_t x = rx.min; x < rx.max; ++x)
                {
                    screen.at(x, y) %= tex.at(x + u, y + v) * quad.p1.c;
                    v += dvDy;
                }
                u += duDx;
            }
        }
        else
        {
            for (position_t y = ry.min; y < ry.max; ++y)
            {
                for (position_t x = rx.min; x < rx.max; ++x)
                {
                    screen.at(x, y) = tex.at(x + u, y + v) * quad.p1.c;
                    v += dvDy;
                }
                u += duDx;
            }
        }
    }
}

template<typename SCREEN, typename COLOR>
void renderQuadCore(
    texture_t<SCREEN>   &screen,
    const clip_t        &clip,
    const quad_t<COLOR> &quad,
    const bool          alphaBlend
)
{
    if ((quad.p2.x < clip.x.min) ||
        (quad.p2.y < clip.y.min) ||
        (quad.p1.x >= clip.x.max) ||
        (quad.p1.y >= clip.y.max))
        return;

    const range_t rx = inl_min({quad.p1.x, quad.p2.x}, clip.x);
    const range_t ry = inl_min({quad.p1.y, quad.p2.y}, clip.y);

    if (alphaBlend)
    {
        for (position_t y = ry.min; y < ry.max; ++y)
        {
            for (position_t x = rx.min; x < rx.max; ++x)
            {
                screen.at(x, y) %= quad.p1.c;
            }
        }
    }
    else
    {
        for (position_t y = ry.min; y < ry.max; ++y)
        {
            for (position_t x = rx.min; x < rx.max; ++x)
            {
                screen.at(x, y) = quad.p1.c;
            }
        }
    }
}

template<typename SCREEN, typename COLOR>
void renderQuad(
    texture_t<SCREEN>       &screen,
    const texture_base_t    *tex,
    const ImVec4            &clipRect,
    const quad_t<COLOR>     &quad,
    const bool              alphaBlend
)
{
    const clip_t clip = {
        inl_min({clipRect.x, clipRect.z}, {0.0f, (position_t)screen.w}),
        inl_min({clipRect.y, clipRect.w}, {0.0f, (position_t)screen.w})
    };

    switch (tex == nullptr ? texture_type_t::NONE : tex->type)
    {
    case texture_type_t::ALPHA8:
        renderQuadCore(screen, *(texture_t<alpha8_t>*)tex, clip, quad, alphaBlend);
        break;

    case texture_type_t::VALUE8:
        renderQuadCore(screen, *(texture_t<value8_t>*)tex, clip, quad, alphaBlend);
        break;

    case texture_type_t::COLOR16:
        renderQuadCore(screen, *(texture_t<color16_t>*)tex, clip, quad, alphaBlend);
        break;

    case texture_type_t::COLOR24:
        renderQuadCore(screen, *(texture_t<color24_t>*)tex, clip, quad, alphaBlend);
        break;

    case texture_type_t::COLOR32:
        renderQuadCore(screen, *(texture_t<color32_t>*)tex, clip, quad, alphaBlend);
        break;

    default:
        renderQuadCore(screen, clip, quad, alphaBlend);
        break;
    }
}

template<typename SCREEN, typename TEXTURE, typename COLOR>
void renderTriCore(
    texture_t<SCREEN>           &screen,
    const texture_t<TEXTURE>    &tex,
    const clip_t                &clip,
    const range_t               &rY,
    const range_t               &rX1,
    const range_t               &rX2,
    const bary_t<COLOR>         &bary,
    const bool                  alphaBlend
)
{
    if (alphaBlend)
    {
        const range_t ry = inl_min(rY, clip.y);
        for (position_t y = ry.min; y < ry.max; ++y)
        {
            const float f = (y - rY.min) / (float)(rY.max - rY.min);
            const range_t rx =
                inl_min({lerp(rX1.min, rX2.min, f), lerp(rX1.max, rX2.max, f)}, clip.x);
            for (position_t x = rx.min; x < rx.max; ++x)
            {
                pixel_t<COLOR> p; p.x = x; p.y = y;
                barycentricUV(p, bary);

                position_t u = (position_t)((p.u * tex.w) + 0.5f) % tex.w;
                position_t v = (position_t)((p.v * tex.h) + 0.5f) % tex.h;

                screen.at(x, y) %= tex.at(u, v) * bary.a.c;
            }
        }
    }
    else
    {
        const range_t ry = inl_min(rY, clip.y);
        for (position_t y = ry.min; y < ry.max; ++y)
        {
            const float f = (y - rY.min) / (float)(rY.max - rY.min);
            const range_t rx =
                inl_min({lerp(rX1.min, rX2.min, f), lerp(rX1.max, rX2.max, f)}, clip.x);
            for (position_t x = rx.min; x < rx.max; ++x)
            {
                pixel_t<COLOR> p; p.x = x; p.y = y;
                barycentricUV(p, bary);

                position_t u = (position_t)((p.u * tex.w) + 0.5f) % tex.w;
                position_t v = (position_t)((p.v * tex.h) + 0.5f) % tex.h;

                screen.at(x, y) = tex.at(u, v) * bary.a.c;
            }
        }
    }
}

template<typename SCREEN, typename COLOR>
void renderTriCore(
    texture_t<SCREEN>   &screen,
    const clip_t        &clip,
    const range_t       &rY,
    const range_t       &rX1,
    const range_t       &rX2,
    const bary_t<COLOR> &bary,
    const bool          uvBlend,
    const bool          alphaBlend
)
{
    if (uvBlend)
    {
        if (alphaBlend)
        {
            const range_t ry = inl_min(rY, clip.y);
            for (position_t y = ry.min; y < ry.max; ++y)
            {
                const float f = (y - rY.min) / (float)(rY.max - rY.min);
                const range_t rx =
                    inl_min({lerp(rX1.min, rX2.min, f), lerp(rX1.max, rX2.max, f)}, clip.x);
                for (position_t x = rx.min; x < rx.max; ++x)
                {
                    pixel_t<COLOR> p; p.x = x; p.y = y;
                    barycentricCol(p, bary);
                    screen.at(x, y) %= p.c;
                }
            }
        }
        else
        {
            const range_t ry = inl_min(rY, clip.y);
            for (position_t y = ry.min; y < ry.max; ++y)
            {
                const float f = (y - rY.min) / (float)(rY.max - rY.min);
                const range_t rx =
                    inl_min({lerp(rX1.min, rX2.min, f), lerp(rX1.max, rX2.max, f)}, clip.x);
                for (position_t x = rx.min; x < rx.max; ++x)
                {
                    pixel_t<COLOR> p; p.x = x; p.y = y;
                    barycentricCol(p, bary);
                    screen.at(x, y) = p.c;
                }
            }
        }
    }
    else
    {
        if (alphaBlend)
        {
            const range_t ry = inl_min(rY, clip.y);
            for (position_t y = ry.min; y < ry.max; ++y)
            {
                const float f = (y - rY.min) / (float)(rY.max - rY.min);
                const range_t rx =
                    inl_min({lerp(rX1.min, rX2.min, f), lerp(rX1.max, rX2.max, f)}, clip.x);
                for (position_t x = rx.min; x < rx.max; ++x)
                {
                    screen.at(x, y) %= bary.a.c;
                }
            }
        }
        else
        {
            const range_t ry = inl_min(rY, clip.y);
            for (position_t y = ry.min; y < ry.max; ++y)
            {
                const float f = (y - rY.min) / (float)(rY.max - rY.min);
                const range_t rx =
                    inl_min({lerp(rX1.min, rX2.min, f), lerp(rX1.max, rX2.max, f)}, clip.x);
                for (position_t x = rx.min; x < rx.max; ++x)
                {
                    screen.at(x, y) = bary.a.c;
                }
            }
        }
    }
}

template<typename SCREEN, typename COLOR>
void renderTri(
    texture_t<SCREEN>       &screen,
    const texture_base_t    *tex,
    const clip_t            &clip,
    const range_t           &rY,
    const range_t           &rX1,
    const range_t           &rX2,
    const bary_t<COLOR>     &bary,
    const bool              uvBlend,
    const bool              alphaBlend
)
{
    switch (tex == nullptr ? texture_type_t::NONE : tex->type)
    {
    case texture_type_t::ALPHA8:
        renderTriCore(screen, *(texture_t<alpha8_t>*)tex, clip, rY, rX1, rX2, bary, alphaBlend);
        break;

    case texture_type_t::VALUE8:
        renderTriCore(screen, *(texture_t<value8_t>*)tex, clip, rY, rX1, rX2, bary, alphaBlend);
        break;

    case texture_type_t::COLOR16:
        renderTriCore(screen, *(texture_t<color16_t>*)tex, clip, rY, rX1, rX2, bary, alphaBlend);
        break;

    case texture_type_t::COLOR24:
        renderTriCore(screen, *(texture_t<color24_t>*)tex, clip, rY, rX1, rX2, bary, alphaBlend);
        break;

    case texture_type_t::COLOR32:
        renderTriCore(screen, *(texture_t<color32_t>*)tex, clip, rY, rX1, rX2, bary, alphaBlend);
        break;

    default:
        renderTriCore(screen, clip, rY, rX1, rX2, bary, uvBlend, alphaBlend);
        break;
    }
}

template<typename SCREEN, typename COLOR>
void renderTri(
    texture_t<SCREEN>       &screen,
    const texture_base_t    *tex,
    const ImVec4            &clipRect,
    triangle_t<COLOR>       &tri,
    const bool              uvBlend,
    const bool              alphaBlend
)
{
    if (tri.p1.y > tri.p2.y) swap(&(tri.p1), &(tri.p2));
    if (tri.p1.y > tri.p3.y) swap(&(tri.p1), &(tri.p3));
    if (tri.p2.y > tri.p3.y) swap(&(tri.p2), &(tri.p3));

    const clip_t clip = {
        inl_min({clipRect.x, clipRect.z}, {0.0f, (position_t)screen.w}),
        inl_min({clipRect.y, clipRect.w}, {0.0f, (position_t)screen.w})
    };

    if ((tri.p3.x < clip.x.min) ||
        (tri.p1.x >= clip.x.max))
        return;

    if (tri.p2.y == tri.p3.y) // Flat bottom triangle
    {
        if (tri.p1.y == tri.p2.y) // Flat line
        {
            if (tri.p1.x > tri.p2.x) swap(&(tri.p1), &(tri.p2));
            if (tri.p1.x > tri.p3.x) swap(&(tri.p1), &(tri.p3));
            if (tri.p2.x > tri.p3.x) swap(&(tri.p2), &(tri.p3));

            if (tri.p1.y < clip.x.min || tri.p1.y > clip.x.max) return;

            renderTri(screen, tex, clip,
                { tri.p1.y, tri.p1.y + 1 },
                { tri.p1.x, tri.p3.x },
                { tri.p1.x, tri.p3.x },
                baryPre(tri.p1, tri.p2, tri.p3),
                uvBlend, alphaBlend
            );
        }
        else // Flat bottom triangle
        {
            if (tri.p2.x > tri.p3.x) swap(&(tri.p2), &(tri.p3));

            renderTri(screen, tex, clip,
                { tri.p1.y, tri.p3.y },
                { tri.p1.x, tri.p1.x },
                { tri.p2.x, tri.p3.x },
                baryPre(tri.p1, tri.p2, tri.p3),
                uvBlend, alphaBlend
            );
        }
    }
    else if (tri.p1.y == tri.p2.y) // Flat top triangle
    {
        if (tri.p1.x > tri.p2.x) swap(&(tri.p1), &(tri.p2));

        renderTri(screen, tex, clip,
            { tri.p1.y, tri.p3.y },
            { tri.p1.x, tri.p2.x },
            { tri.p3.x, tri.p3.x },
            baryPre(tri.p1, tri.p2, tri.p3),
            uvBlend, alphaBlend
        );
    }
    else
    {
        // Find 4th point to split the tri into flat top and flat bottom triangles
        pixel_t<COLOR> p4;
        p4.x = lerp(tri.p1.x, tri.p3.x, (tri.p2.y - tri.p1.y) / (float)(tri.p3.y - tri.p1.y));
        p4.y = tri.p2.y;
        p4.c = tri.p1.c;

        if (tri.p2.x > p4.x) swap(&(tri.p2), &p4);

        renderTri(screen, tex, clip,
            { tri.p1.y, tri.p2.y },
            { tri.p1.x, tri.p1.x },
            { tri.p3.x, p4.x },
            baryPre(tri.p1, tri.p2, p4),
            uvBlend, alphaBlend
        );

        renderTri(screen, tex, clip,
            { tri.p2.y, tri.p3.y },
            { tri.p2.x, p4.x },
            { tri.p3.x, tri.p3.x },
            baryPre(tri.p2, p4, tri.p3),
            uvBlend, alphaBlend
        );
    }
}

template<typename SCREEN>
void renderCommand(
    texture_t<SCREEN> &screen,
    const texture_base_t *texture,
    const ImDrawVert *vtx_buffer,
    const ImDrawIdx *idx_buffer,
    const ImDrawCmd &pcmd
)
{
    for(unsigned int i = 0; i < pcmd.ElemCount; i += 3)
    {
        const ImDrawVert* verts[] =
        {
            &vtx_buffer[idx_buffer[i]],
            &vtx_buffer[idx_buffer[i+1]],
            &vtx_buffer[idx_buffer[i+2]]
        };

        // /*
        if (i < pcmd.ElemCount - 3)
        {
            ImVec2 tlpos = verts[0]->pos;
            ImVec2 brpos = verts[0]->pos;
            ImVec2 tluv = verts[0]->uv;
            ImVec2 bruv = verts[0]->uv;
            for (int v = 1; v < 3; v++)
            {
                if (verts[v]->pos.x < tlpos.x)
                {
                    tlpos.x = verts[v]->pos.x;
                    tluv.x = verts[v]->uv.x;
                }
                else if (verts[v]->pos.x > brpos.x)
                {
                    brpos.x = verts[v]->pos.x;
                    bruv.x = verts[v]->uv.x;
                }
                if (verts[v]->pos.y < tlpos.y)
                {
                    tlpos.y = verts[v]->pos.y;
                    tluv.y = verts[v]->uv.y;
                }
                else if (verts[v]->pos.y > brpos.y)
                {
                    brpos.y = verts[v]->pos.y;
                    bruv.y = verts[v]->uv.y;
                }
            }

            const ImDrawVert* nextVerts[] =
            {
                &vtx_buffer[idx_buffer[i+3]],
                &vtx_buffer[idx_buffer[i+4]],
                &vtx_buffer[idx_buffer[i+5]]
            };

            bool isRect = true;
            for (int v = 0; v < 3; v++)
            {
                if (((nextVerts[v]->pos.x != tlpos.x) && (nextVerts[v]->pos.x != brpos.x)) ||
                    ((nextVerts[v]->pos.y != tlpos.y) && (nextVerts[v]->pos.y != brpos.y)) ||
                    ((nextVerts[v]->uv.x != tluv.x) && (nextVerts[v]->uv.x != bruv.x)) ||
                    ((nextVerts[v]->uv.y != tluv.y) && (nextVerts[v]->uv.y != bruv.y)))
                {
                    isRect = false;
                    break;
                }
            }

            if (isRect)
            {
                quad_t<SCREEN> quad;
                quad.p1.x = tlpos.x;
                quad.p1.y = tlpos.y;
                quad.p2.x = brpos.x;
                quad.p2.y = brpos.y;
                quad.p1.u = tluv.x;
                quad.p1.v = tluv.y;
                quad.p2.u = bruv.x;
                quad.p2.v = bruv.y;
                quad.p1.c = SCREEN(color32_t(
                    verts[0]->col >> IM_COL32_R_SHIFT,
                    verts[0]->col >> IM_COL32_G_SHIFT,
                    verts[0]->col >> IM_COL32_B_SHIFT,
                    verts[0]->col >> IM_COL32_A_SHIFT
                ));
                quad.p2.c = quad.p1.c;

                const bool noUV = (quad.p1.u == quad.p2.u) && (quad.p1.v == quad.p2.v);
                const bool flatCol = noUV || (quad.p1.c == quad.p2.c);
                const bool alphaBlend = true;

                renderQuad(screen, noUV ? nullptr : texture, pcmd.ClipRect, quad, alphaBlend);

                // switch (texture == nullptr || noUV ? texture_type_t::NONE : texture->type)
                // {
                //     case texture_type_t::ALPHA8:
                //         renderQuad(screen, *(texture_t<alpha8_t>*)texture, pcmd.ClipRect, quad, alphaBlend);
                //         break;
                //     case texture_type_t::VALUE8:
                //         renderQuad(screen, *(texture_t<value8_t>*)texture, pcmd.ClipRect, quad, alphaBlend);
                //         break;
                //     case texture_type_t::COLOR16:
                //         renderQuad(screen, *(texture_t<color16_t>*)texture, pcmd.ClipRect, quad, alphaBlend);
                //         break;
                //     case texture_type_t::COLOR24:
                //         renderQuad(screen, *(texture_t<color24_t>*)texture, pcmd.ClipRect, quad, alphaBlend);
                //         break;
                //     case texture_type_t::COLOR32:
                //         renderQuad(screen, *(texture_t<color32_t>*)texture, pcmd.ClipRect, quad, alphaBlend);
                //         break;
                //     default:
                //         renderQuad(screen, pcmd.ClipRect, quad, alphaBlend);
                //         break;
                // }

                i += 3;
                continue;
            }
        }
        // */

        // /*
        triangle_t<SCREEN> tri;
        tri.p1.x = verts[0]->pos.x;
        tri.p1.y = verts[0]->pos.y;
        tri.p1.u = verts[0]->uv.x;
        tri.p1.v = verts[0]->uv.y;
        tri.p1.c = SCREEN(color32_t(
            verts[0]->col >> IM_COL32_R_SHIFT,
            verts[0]->col >> IM_COL32_G_SHIFT,
            verts[0]->col >> IM_COL32_B_SHIFT,
            verts[0]->col >> IM_COL32_A_SHIFT
        ));

        tri.p2.x = verts[1]->pos.x;
        tri.p2.y = verts[1]->pos.y;
        tri.p2.u = verts[1]->uv.x;
        tri.p2.v = verts[1]->uv.y;
        tri.p2.c = SCREEN(color32_t(
            verts[1]->col >> IM_COL32_R_SHIFT,
            verts[1]->col >> IM_COL32_G_SHIFT,
            verts[1]->col >> IM_COL32_B_SHIFT,
            verts[1]->col >> IM_COL32_A_SHIFT
        ));

        tri.p3.x = verts[2]->pos.x;
        tri.p3.y = verts[2]->pos.y;
        tri.p3.u = verts[2]->uv.x;
        tri.p3.v = verts[2]->uv.y;
        tri.p3.c = SCREEN(color32_t(
            verts[2]->col >> IM_COL32_R_SHIFT,
            verts[2]->col >> IM_COL32_G_SHIFT,
            verts[2]->col >> IM_COL32_B_SHIFT,
            verts[2]->col >> IM_COL32_A_SHIFT
        ));

        const bool noUV = (tri.p1.u == tri.p2.u) && (tri.p1.u == tri.p3.u) &&
                          (tri.p1.v == tri.p2.v) && (tri.p1.v == tri.p3.v);
        const bool flatCol = noUV || (tri.p1.c == tri.p2.c) && (tri.p1.c == tri.p3.c);
        const bool alphaBlend = true;

        renderTri(screen, noUV ? nullptr : texture, pcmd.ClipRect, tri, !flatCol, alphaBlend);
        // */
    }
}

template<typename SCREEN>
void renderDrawLists(ImDrawData* drawData, texture_t<SCREEN> &screen)
{
    ImGuiIO& io = ImGui::GetIO();
    int fbWidth = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    int fbHeight = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    if (fbWidth == 0 || fbHeight == 0)
        return;
    drawData->ScaleClipRects(io.DisplayFramebufferScale);

    for (int n = 0; n < drawData->CmdListsCount; n++)
    {
        const ImDrawList* cmdList = drawData->CmdLists[n];
        const ImDrawVert* vtx_buffer = cmdList->VtxBuffer.Data;
        const ImDrawIdx* idx_buffer = cmdList->IdxBuffer.Data;

        for (int cmdi = 0; cmdi < cmdList->CmdBuffer.Size; cmdi++)
        {
            const ImDrawCmd &pcmd = cmdList->CmdBuffer[cmdi];
            if (pcmd.UserCallback)
            {
                pcmd.UserCallback(cmdList, &pcmd);
            }
            else
            {
                renderCommand(screen, (texture_base_t*)pcmd.TextureId, vtx_buffer, idx_buffer, pcmd);
            }
            idx_buffer += pcmd.ElemCount;
        }
    }
}

#undef INLINE_DEC
#undef INLINE_DEF
#undef INLINE
#undef INLINE_TEMPLATE

#endif
