#ifndef SOFTRASTER_TEXTURE_H
#define SOFTRASTER_TEXTURE_H

#include "defines.h"
#include "color.h"

enum texture_type_t { NONE = 0, ALPHA8, VALUE8, COLOR16, COLOR24, COLOR32 };

#if defined(__cplusplus) && __cplusplus >= 201103L
#   define INLINE_CONSTEXPR constexpr
#else
#   define INLINE_CONSTEXPR inline
#endif

template<typename T> INLINE_CONSTEXPR texture_type_t TextureType() { return NONE; }
template<> INLINE_CONSTEXPR texture_type_t TextureType<alpha8_t>() { return ALPHA8; }
template<> INLINE_CONSTEXPR texture_type_t TextureType<value8_t>() { return VALUE8; }
template<> INLINE_CONSTEXPR texture_type_t TextureType<color16_t>() { return COLOR16; }
template<> INLINE_CONSTEXPR texture_type_t TextureType<color24_t>() { return COLOR24; }
template<> INLINE_CONSTEXPR texture_type_t TextureType<color32_t>() { return COLOR32; }

#undef INLINE_CONSTEXPR

struct texture_base_t
{
    texture_type_t type = NONE;
    void *pixels = nullptr;
    size_t w = 0, h = 0, size = 0;
    bool needFree = true;

    inline void clear()
    {
        memset(pixels, 0, w * h * size);
    }

    inline ~texture_base_t()
    {
        if (needFree && pixels != nullptr)
            free(pixels);
    }
};

template<typename COLOR>
struct texture_t : public texture_base_t
{
    inline void init(size_t x, size_t y)
    {
        if (needFree && pixels != nullptr)
            free(pixels);
        w = x;
        h = y;
        size = sizeof(COLOR);
        pixels = malloc(w * y * size);
        needFree = true;
        type = TextureType<COLOR>();
    }

    inline void init(size_t x, size_t y, COLOR *data)
    {
        if (needFree && pixels != nullptr)
            free(pixels);
        w = x;
        h = y;
        size = sizeof(COLOR);
        pixels = data;
        needFree = false;
        type = TextureType<COLOR>();
    }

    INLINE_DEC(COLOR &at(size_t x, size_t y))
    {
        return reinterpret_cast<COLOR*>(pixels)[x + (w * y)];
    }

    INLINE_DEC(const COLOR &at(size_t x, size_t y) const)
    {
        return reinterpret_cast<COLOR*>(pixels)[x + (w * y)];
    }
};

using texture_alpha8_t = texture_t<alpha8_t>;
using texture_value8_t = texture_t<value8_t>;
using texture_color16_t = texture_t<color16_t>;
using texture_color24_t = texture_t<color24_t>;
using texture_color32_t = texture_t<color32_t>;

#endif // SOFTRASTER_TEXTURE_H