#ifndef SOFTRASTER_TEMPL_H
#define SOFTRASTER_TEMPL_H
#include "softraster.h"

// Linear interpolation functions
template<typename T>
inline T lerp(T a, T b, uint8_t f) // [0, 255]
{
    return a + ((f * (b - a)) / 0xFF);
}

template<typename T>
inline T lerp(T a, T b, float f) // [0.0f, 1.0f]
{
    return a + (f * (b - a));
}

// Color type conversions
template<typename C1, typename C2>
inline C1 convCol(const C2& c)
{
    C1 c1;
    return c1;
}

template<typename C1>
inline C1 convCol(const C1& c)
{
    return c;
}

template<> inline color16_t convCol(const color8_t& c)
{
    return (((c * 31) / 255) << 11) |
        (((c * 63) / 255) << 5) |
        ((c * 31) / 255);
}
template<> inline color24_t convCol (const color8_t& c)
{
    color24_t rtn;
    rtn.r = c;
    rtn.g = c;
    rtn.b = c;
    return rtn;
}
#if defined(COL8_ALPHACOLOR)
template<> inline color8_t convCol (const color32_t& c)
{
    return (((c.r + c.g + c.b) / 3) * c.a) / 255;
}
template<> inline color32_t convCol (const color8_t& c)
{
    color32_t rtn;
    rtn.r = c;
    rtn.g = c;
    rtn.b = c;
    rtn.a = c;
    return rtn;
}
#elif defined(COL8_COLOR)
template<> inline color8_t convCol (const color32_t& c)
{
    return (c.r + c.g + c.b) / 3;
}
template<> inline color32_t convCol (const color8_t& c)
{
    color32_t rtn;
    rtn.r = c;
    rtn.g = c;
    rtn.b = c;
    rtn.a = 0xFF;
    return rtn;
}
#else
template<> inline color8_t convCol (const color32_t& c)
{
    return c.a;
}
template<> inline color32_t convCol (const color8_t& c)
{
    color32_t rtn;
    rtn.r = 0xFF;
    rtn.g = 0xFF;
    rtn.b = 0xFF;
    rtn.a = c;
    return rtn;
}
#endif

template<> inline color8_t convCol (const color16_t& c)
{
    return (((((c & 0xF800) >> 11) * 255) / 31) + 
        ((((c & 0x07E0) >> 5 ) * 255) / 63) + 
        (((c & 0x001F) * 255) / 31)) / 3;
}
template<> inline color24_t convCol (const color16_t& c)
{
    color24_t rtn;
    rtn.r = (((c & 0xF800) >> 11) * 255) / 31;
    rtn.g = (((c & 0x07E0) >> 5 ) * 255) / 63;
    rtn.b = ((c & 0x001F) * 255) / 31;
    return rtn;
}
template<> inline color32_t convCol (const color16_t& c)
{
    color32_t rtn;
    rtn.r = (((c & 0xF800) >> 11) * 255) / 31;
    rtn.g = (((c & 0x07E0) >> 5 ) * 255) / 63;
    rtn.b = ((c & 0x001F) * 255) / 31;
    rtn.a = 0xFF;
    return rtn;
}

template<> inline color8_t convCol (const color24_t& c)
{
    return (c.r + c.g + c.b) / 3;
}
template<> inline color16_t convCol (const color24_t& c)
{
    return (((c.r * 31) / 255) << 11) |
        (((c.g * 63) / 255) << 5) |
        ((c.b * 31) / 255);
}
template<> inline color32_t convCol (const color24_t& c)
{
    color32_t rtn;
    rtn.r = c.r;
    rtn.g = c.g;
    rtn.b = c.b;
    rtn.a = 0xFF;
    return rtn;
}

template<> inline color16_t convCol (const color32_t& c)
{
    return (((c.r * 31) / 255) << 11) |
        (((c.g * 63) / 255) << 5) |
        ((c.b * 31) / 255);
}
template<> inline color24_t convCol (const color32_t& c)
{
    color24_t rtn;
    rtn.r = c.r;
    rtn.g = c.g;
    rtn.b = c.b;
    return rtn;
}

// Template the texture struct so we can use different color modes/types
template <typename COL_T>
struct texture_t
{
    bool isSetup = false;
    uint8_t colorMode = 0;
    size_t w, h;
    COL_T** col = NULL;
    void clear()
    {
        for(int i = 0; i < w; i++)
        {
            for(int j = 0; j < h; j++)
            {
                memset(&(col[i][j]), 0x0, colorMode);
            }
        }
    }
    void pre_init()
    {
        colorMode = sizeof(COL_T);
        col = (COL_T**)malloc(w*sizeof(COL_T*));
    }
    void init(size_t x, size_t y)
    {
        w = x;
        h = y;
        pre_init();
        for (size_t i = 0; i < w; i++)
        {
            col[i] = (COL_T*)malloc(h*sizeof(COL_T));
        }
        isSetup = true;
    }
    texture_templ_t() : col(NULL), isSetup(false){}
    texture_templ_t(size_t x, size_t y)
    {
        init(x, y);
    }
    ~texture_templ_t()
    {
        if (col != NULL && isSetup)
        {
            for (size_t i = 0; i < w; i++)
            {
                if (col[i] != NULL)
                {
                    free(col[i]);
                    col[i] = NULL;
                }
            }
            free(col);
            col = NULL;
        }
        isSetup = false;
    }
};

template<typename colType>
struct screen_t
{
    texture_t<colType>* buffer;
    clip_t* clip;
    size_t w;
    size_t h;
};

template<typename screenTexType, typename colType>
struct renderData_t
{
    screen_t<colType>* screen;
    texType* texture;
    ImVec4 clipRect;
};

template<typename screenColType>
void Softraster::renderRect(renderData_t<screenColType, void>* renderData, rectangle_t* rect)
{
    const screen_t<screenColType>* screen = renderData->screen;
    const ImVec4& clipRect = renderData->clipRect;

    const position_t minclipx = (clipRect.x > 0 ? clipRect.x : 0);
    const position_t minclipy = (clipRect.y > 0 ? clipRect.y : 0);

    const position_t maxclipx = (clipRect.z < screen->w ? clipRect.z : screen->w);
    const position_t maxclipy = (clipRect.w < screen->h ? clipRect.w : screen->h);

    if ((rect->p2.x < minclipx ||
        (rect->p2.y < minclipy) ||
        (rect->p1.x >= maxclipx) ||
        (rect->p1.y >= maxclipy))
        return;
    
    const position_t startx = (rect->p1.x > minclipx ? rect->p1.x : minclipx);
    const position_t starty = (rect->p1.y > minclipy ? rect->p1.y : minclipy);

    const position_t endx = (rect->p2.x < maxclipx ? rect->p2.x : maxclipx);
    const position_t endy = (rect->p2.y < maxclipy ? rect->p2.y : maxclipy);

    screen->clip->x1 = (startx < screen->clip->x1 ? startx : screen->clip->x1);
    screen->clip->y1 = (starty < screen->clip->y1 ? starty : screen->clip->y1);
    screen->clip->x2 = (endx > screen->clip->x2 ? endx-1 : screen->clip->x2);
    screen->clip->y2 = (endy > screen->clip->y2 ? endy-1 : screen->clip->y2);

    for (position_t x = startx; x < endx; x++)
    {
        for (position_t y = starty; y < endy; y++)
        {
            screen->texture[x][y] = convCol<screenColType>(rect->p1.c);
        }
    }
}

template<typename screenColType>
void Softraster::renderRectBlend(renderData_t<screenColType, void>* renderData, rectangle_t* rect)
{
    const screen_t<screenColType>* screen = renderData->screen;
    const ImVec4& clipRect = renderData->clipRect;

    const position_t minclipx = (clipRect.x > 0 ? clipRect.x : 0);
    const position_t minclipy = (clipRect.y > 0 ? clipRect.y : 0);

    const position_t maxclipx = (clipRect.z < screen->w ? clipRect.z : screen->w);
    const position_t maxclipy = (clipRect.w < screen->h ? clipRect.w : screen->h);

    if ((rect->p2.x < minclipx ||
        (rect->p2.y < minclipy) ||
        (rect->p1.x >= maxclipx) ||
        (rect->p1.y >= maxclipy))
        return;
    
    const position_t startx = (rect->p1.x > minclipx ? rect->p1.x : minclipx);
    const position_t starty = (rect->p1.y > minclipy ? rect->p1.y : minclipy);

    const position_t endx = (rect->p2.x < maxclipx ? rect->p2.x : maxclipx);
    const position_t endy = (rect->p2.y < maxclipy ? rect->p2.y : maxclipy);

    screen->clip->x1 = (startx < screen->clip->x1 ? startx : screen->clip->x1);
    screen->clip->y1 = (starty < screen->clip->y1 ? starty : screen->clip->y1);
    screen->clip->x2 = (endx > screen->clip->x2 ? endx-1 : screen->clip->x2);
    screen->clip->y2 = (endy > screen->clip->y2 ? endy-1 : screen->clip->y2);

    for (position_t x = startx; x < endx; x++)
    {
        for (position_t y = starty; y < endy; y++)
        {
            // could make a specialised lerp function for each color type
            // to reduce the number of color conversions per pixel
            color24_t prev = convCol<color24_t>(screen->buffer[x][y]);
            prev.r = lerp(prev.r, rect->p1.c.r, rect->p1.c.a);
            prev.g = lerp(prev.g, rect->p1.c.g, rect->p1.c.a);
            prev.b = lerp(prev.b, rect->p1.c.b, rect->p1.c.a);
            screen->buffer[x][y] = convCol<screenColType>(prev);
        }
    }
}

template<typename screenColType, typename texColType>
void Softraster::renderRectTex(renderData_t<screenColType, texColType>* renderData, rectangle_t* rect)
{
    const screen_t<screenColType>* screen = renderData->screen;
    const texture_t<texColType>* tex = renderData->texture;
    const ImVec4& clipRect = renderData->clipRect;

    const position_t minclipx = (clipRect.x > 0 ? clipRect.x : 0);
    const position_t minclipy = (clipRect.y > 0 ? clipRect.y : 0);

    const position_t maxclipx = (clipRect.z < screen->w ? clipRect.z : screen->w);
    const position_t maxclipy = (clipRect.w < screen->h ? clipRect.w : screen->h);

    if ((rect->p2.x < minclipx ||
        (rect->p2.y < minclipy) ||
        (rect->p1.x >= maxclipx) ||
        (rect->p1.y >= maxclipy))
        return;
    
    const position_t startx = (rect->p1.x > minclipx ? rect->p1.x : minclipx);
    const position_t starty = (rect->p1.y > minclipy ? rect->p1.y : minclipy);

    const position_t endx = (rect->p2.x < maxclipx ? rect->p2.x : maxclipx);
    const position_t endy = (rect->p2.y < maxclipy ? rect->p2.y : maxclipy);

    screen->clip->x1 = (startx < screen->clip->x1 ? startx : screen->clip->x1);
    screen->clip->y1 = (starty < screen->clip->y1 ? starty : screen->clip->y1);
    screen->clip->x2 = (endx > screen->clip->x2 ? endx-1 : screen->clip->x2);
    screen->clip->y2 = (endy > screen->clip->y2 ? endy-1 : screen->clip->y2);

    const float duDx = (rect->p2.u - rect->p1.u) / (float)(rect->p2.x - rect->p1.x);
    const float dvDy = (rect->p2.v - rect->p1.v) / (float)(rect->p2.y - rect->p1.y);

    const float startu = rect->p1.u + (duDx * (float)(rect->p1.x - minclipx < 0 ? minclipx + rect->p1.x : 0));
    const float startv = rect->p1.v + (dvDy * (float)(rect->p1.v - minclipx < 0 ? minclipx + rect->p1.v : 0));

    #ifdef TEXTURE_MODE_CLAMP
    position_t u = (position_t)((startu * tex->w) + 0.5f);
    position_t v = (position_t)((startv * tex->h) + 0.5f);
    if (u < 0) u = 0;
    else if (u > tex->w) u = tex->w - 1;
    if (v < 0) v = 0;
    else if (v > tex->h) v = tex->h - 1;
    #else
    position_t u = (position_t)((startu * tex->w) + 0.5f) % tex->w; 
    position_t v = (position_t)((startv * tex->h) + 0.5f) % tex->h;
    #endif

    bool blit = (((position_t)(duDx + 0.5f) == 1) && ((position_t)(dvDy + 0.5f) == 1));

    if (blit)
    {
        position_t u2 = u - startx, v2 = v - starty;
        for (position_t x = startx; x < endx; x++)
        {
            for (position_t y = starty; y < endy; y++)
            {
                color24_t c = convCol<color24_t>(tex[x+u2][y+v2]);
                c.r = (c.r * rect->p1.c.r) / 255;
                c.g = (c.g * rect->p1.c.g) / 255;
                c.b = (c.b * rect->p1.c.b) / 255;
                screen->buffer[x][y] = convCol<screenColType>(c);
            }
        }
    }
    else
    {
        float u2 = u, v2 = v;
        for (position_t x = startx; x < endx; x++)
        {
            for (position_t y = starty; y < endy; y++)
            {
                color24_t c = convCol<color24_t>(tex[u2][v2]);
                c.r = (c.r * rect->p1.c.r) / 255;
                c.g = (c.g * rect->p1.c.g) / 255;
                c.b = (c.b * rect->p1.c.b) / 255;
                screen->buffer[x][y] = convCol<screenColType>(c);
                v2 += dvDy;
            }
            u2 += duDx;
        }
    }
}

template<typename screenColType, typename texColType>
void Softraster::renderRectTexBlend(renderData_t<screenColType, texColType>* renderData, rectangle_t* rect)
{
    const screen_t<screenColType>* screen = renderData->screen;
    const texture_t<texColType>* tex = renderData->texture;
    const ImVec4& clipRect = renderData->clipRect;

    const position_t minclipx = (clipRect.x > 0 ? clipRect.x : 0);
    const position_t minclipy = (clipRect.y > 0 ? clipRect.y : 0);

    const position_t maxclipx = (clipRect.z < screen->w ? clipRect.z : screen->w);
    const position_t maxclipy = (clipRect.w < screen->h ? clipRect.w : screen->h);

    if ((rect->p2.x < minclipx ||
        (rect->p2.y < minclipy) ||
        (rect->p1.x >= maxclipx) ||
        (rect->p1.y >= maxclipy))
        return;
    
    const position_t startx = (rect->p1.x > minclipx ? rect->p1.x : minclipx);
    const position_t starty = (rect->p1.y > minclipy ? rect->p1.y : minclipy);

    const position_t endx = (rect->p2.x < maxclipx ? rect->p2.x : maxclipx);
    const position_t endy = (rect->p2.y < maxclipy ? rect->p2.y : maxclipy);

    screen->clip->x1 = (startx < screen->clip->x1 ? startx : screen->clip->x1);
    screen->clip->y1 = (starty < screen->clip->y1 ? starty : screen->clip->y1);
    screen->clip->x2 = (endx > screen->clip->x2 ? endx-1 : screen->clip->x2);
    screen->clip->y2 = (endy > screen->clip->y2 ? endy-1 : screen->clip->y2);

    const float duDx = (rect->p2.u - rect->p1.u) / (float)(rect->p2.x - rect->p1.x);
    const float dvDy = (rect->p2.v - rect->p1.v) / (float)(rect->p2.y - rect->p1.y);

    const float startu = rect->p1.u + (duDx * (float)(rect->p1.x - minclipx < 0 ? minclipx + rect->p1.x : 0));
    const float startv = rect->p1.v + (dvDy * (float)(rect->p1.v - minclipx < 0 ? minclipx + rect->p1.v : 0));

    #ifdef TEXTURE_MODE_CLAMP
    position_t u = (position_t)((startu * tex->w) + 0.5f);
    position_t v = (position_t)((startv * tex->h) + 0.5f);
    if (u < 0) u = 0;
    else if (u > tex->w) u = tex->w - 1;
    if (v < 0) v = 0;
    else if (v > tex->h) v = tex->h - 1;
    #else
    position_t u = (position_t)((startu * tex->w) + 0.5f) % tex->w; 
    position_t v = (position_t)((startv * tex->h) + 0.5f) % tex->h;
    #endif

    bool blit = (((position_t)(duDx + 0.5f) == 1) && ((position_t)(dvDy + 0.5f) == 1));

    if (blit)
    {
        position_t u2 = u - startx, v2 = v - starty;
        for (position_t x = startx; x < endx; x++)
        {
            for (position_t y = starty; y < endy; y++)
            {
                color32_t c = convCol<color32_t>(tex[x+u2][y+v2]);
                if (c.a == 0) continue;
                c.r = (c.r * rect->p1.c.r) / 255;
                c.g = (c.g * rect->p1.c.g) / 255;
                c.b = (c.b * rect->p1.c.b) / 255;
                if (c.a != 255)
                {
                    color24_t prev = convCol<color24_t>(screen->buffer[x][y])
                    c.r = lerp(prev.r, c.r, c.a);
                    c.g = lerp(prev.g, c.g, c.a);
                    c.b = lerp(prev.b, c.b, c.a);
                }
                screen->buffer[x][y] = convCol<screenColType>(c);
            }
        }
    }
    else
    {
        float u2 = u, v2 = v;
        for (position_t x = startx; x < endx; x++)
        {
            for (position_t y = starty; y < endy; y++)
            {
                color32_t c = convCol<color32_t>(tex[u2][v2]);
                if (c.a == 0) continue;
                c.r = (c.r * rect->p1.c.r) / 255;
                c.g = (c.g * rect->p1.c.g) / 255;
                c.b = (c.b * rect->p1.c.b) / 255;
                if (c.a != 255)
                {
                    color24_t prev = convCol<color24_t>(screen->buffer[x][y])
                    c.r = lerp(prev.r, c.r, c.a);
                    c.g = lerp(prev.g, c.g, c.a);
                    c.b = lerp(prev.b, c.b, c.a);
                }
                screen->buffer[x][y] = convCol<screenColType>(c);
                v2 += dvDy;
            }
            u2 += duDx;
        }
    }
}

template<typename T>
inline void swapTri(T* tri1, T* tri2)
{
    T temp;
    memcpy(&temp, tri1, sizeof(T));
    memcpy(tri1, tri2, sizeof(T));
    memcpy(tri2, &temp, sizeof(T));
} 

template<typename screenColType>
void Softraster::renderTri(renderData_t<screenColType, void>* renderData, triangle_t* tri)
{
    tri->p1.y = (position_t)(tri->p1.y + 0.5f);
    tri->p2.y = (position_t)(tri->p2.y + 0.5f);
    tri->p3.y = (position_t)(tri->p3.y + 0.5f);

    if (tri->p1.y > tri->p2.y) swap<pixel_t>(&(tri->p1), &(tri->p2));
    if (tri->p1.y > tri->p3.y) swap<pixel_t>(&(tri->p1), &(tri->p3));
    if (tri->p2.y > tri->p3.y) swap<pixel_t>(&(tri->p2), &(tri->p3));

    const screen_t<screenColType>* screen = renderData->screen;
    const ImVec4& clipRect = renderData->clipRect;

    const position_t minclipx = (clipRect.x > 0 ? clipRect.x : 0);
    const position_t minclipy = (clipRect.y > 0 ? clipRect.y : 0);

    const position_t maxclipx = (clipRect.z < screen->w ? clipRect.z : screen->w);
    const position_t maxclipy = (clipRect.w < screen->h ? clipRect.w : screen->h);

    if ((tri->p3.y < minclipy) ||
        (tri->p1.y >= maxclipy))
        return;
    
    if (tri->p2.y == tri->p3.y) // Flat bottom triangle
    {
        if (tri->p1.y == tri->p2.y) // Flat line
        {

        }
        else
        {
            if (tri->p2.x > tri->p3.x) swap<pixel_t>(&(tri->p2), &(tri->p3));
            // Slope version
            // float x1Inc = (tri->p2.x - tri->p1.x) / (float)(tri->p2.y - tri->p1.y);
            // float x2Inc = (tri->p3.x - tri->p1.x) / (float)(tri->p3.y - tri->p1.y);
            // float x1 = tri->p1.x;
            // float x2 = tri->p1.x;
            float f = (y - tri->p1.y) / (float)(tri->p2.y - tri->p1.y);
            // Lerp version
            position_t startx = lerp(tri->p1.x, tri->p2.x, f);
            position_t endx =   lerp(tri->p1.x, tri->p3.x, f);
            for (position_t x = startx; x < endx; x++)
            {
                position_t starty = (tri->p1.y > 0 ? tri->p1.y : 0);
                position_t endy = (tri->p2.y < maxclipy ? tri->p2.y : maxclipy);
                for (position_t y = starty; y < endy; y++)
                {
                    screen->texture[x][y] = convCol<screenColType>(rect->p1.c);
                }
            }
        }
        return;
    }
    else if (tri->p1.y == tri->p2.y) // Flat top triangle
    {
        if (tri->p2.x > tri->p3.x) swap<pixel_t>(&(tri->p2), &(tri->p3));

        return;
    }

    // Find 4th point to split the tri into flat top and flat bottom triangles
    float f = (tri->p2.y - tri->p1.y) / (tri->p3.y - tri->p1.y);

    pixel_t p4;
    p4.x = lerp(tri->p1.x, tri->p3.x, f);
    p4.y = tri->p2.y;
    p4.c = tri->p1.c;

    if (tri->p2.x > p4.x) swap<pixel_t>(&(tri->p2), &p4);

    triangle_t flat;
    // Flat bottom
    flat.p1 = tri->p1;
    flat.p2 = tri->p2;
    flat.p3 = p4;

    // Flat top
    flat.p1 = tri->p2;
    flat.p2 = p4;
    flat.p3 = tri->p3;

    const position_t startx = (rect->p1.x > minclipx ? rect->p1.x : minclipx);
    const position_t starty = (rect->p1.y > minclipy ? rect->p1.y : minclipy);

    const position_t endx = (rect->p2.x < maxclipx ? rect->p2.x : maxclipx);
    const position_t endy = (rect->p2.y < maxclipy ? rect->p2.y : maxclipy);

    screen->clip->x1 = (startx < screen->clip->x1 ? startx : screen->clip->x1);
    screen->clip->y1 = (starty < screen->clip->y1 ? starty : screen->clip->y1);
    screen->clip->x2 = (endx > screen->clip->x2 ? endx-1 : screen->clip->x2);
    screen->clip->y2 = (endy > screen->clip->y2 ? endy-1 : screen->clip->y2);

    for (position_t x = startx; x < endx; x++)
    {
        for (position_t y = starty; y < endy; y++)
        {
            
        }
    }
}

template<typename screenColType>
void Softraster::renderTriBlend(renderData_t<screenColType, void>* renderData, triangle_t* tri)
{

}

template<typename screenColType, typename texColType>
void Softraster::renderTriTex(renderData_t<screenColType, texColType>* renderData, triangle_t* tri)
{

}

template<typename screenColType, typename texColType>
void Softraster::renderTriTexBlend(renderData_t<screenColType, texColType>* renderData, triangle_t* tri)
{

}

template<typename screenColType, typename texColType>
void Softraster::renderDrawLists(ImDrawData* drawData, screen_t<screenColType>* screen)
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
            const ImDrawCmd* pcmd = &cmdList->CmdBuffer[cmdi];
            if (pcmd->UserCallback)
            {
                pcmd->UserCallback(cmdList, pcmd);
            }
            else
            {
                renderData_t renderData;
                renderData.screen = screen;
                renderData.texture = (texture_t*)pcmd->TextureId;
                renderData.clipRect = pcmd->ClipRect;

                for(unsigned int i = 0; i < pcmd->ElemCount; i += 3)
                {
                    const ImDrawVert* verts[] =
                    {
                        &vtx_buffer[idx_buffer[i]],
                        &vtx_buffer[idx_buffer[i+1]],
                        &vtx_buffer[idx_buffer[i+2]]
                    };

                    if (i < pcmd->ElemCount - 3)
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
                            rectangle_t rect;
                            rect.x1 = tlpos.x;
                            rect.y1 = tlpos.y;
                            rect.x2 = brpos.x;
                            rect.y2 = brpos.y;
                            rect.u1 = tluv.x;
                            rect.v1 = tluv.y;
                            rect.u2 = bruv.x;
                            rect.v2 = bruv.y;
                            rect.c.r = (verts[0]->col >> IM_COL32_R_SHIFT) & 0xFF;
                            rect.c.g = (verts[0]->col >> IM_COL32_G_SHIFT) & 0xFF;
                            rect.c.b = (verts[0]->col >> IM_COL32_B_SHIFT) & 0xFF;
                            rect.c.a = (verts[0]->col >> IM_COL32_A_SHIFT) & 0xFF;
                            renderRectangle(&renderData, &rect);
                            i += 3;
                            continue;
                        }
                    }

                    triangle_t tri;
                    tri.x1 = verts[0]->pos.x;
                    tri.y1 = verts[0]->pos.y;
                    tri.u1 = verts[0]->uv.x;
                    tri.v1 = verts[0]->uv.y;
                    tri.c1.r = (verts[0]->col >> IM_COL32_R_SHIFT) & 0xFF;
                    tri.c1.g = (verts[0]->col >> IM_COL32_G_SHIFT) & 0xFF;
                    tri.c1.b = (verts[0]->col >> IM_COL32_B_SHIFT) & 0xFF;
                    tri.c1.a = (verts[0]->col >> IM_COL32_A_SHIFT) & 0xFF;
                    tri.x2 = verts[1]->pos.x;
                    tri.y2 = verts[1]->pos.y;
                    tri.u2 = verts[1]->uv.x;
                    tri.v2 = verts[1]->uv.y;
                    tri.c2.r = (verts[1]->col >> IM_COL32_R_SHIFT) & 0xFF;
                    tri.c2.g = (verts[1]->col >> IM_COL32_G_SHIFT) & 0xFF;
                    tri.c2.b = (verts[1]->col >> IM_COL32_B_SHIFT) & 0xFF;
                    tri.c2.a = (verts[1]->col >> IM_COL32_A_SHIFT) & 0xFF;
                    tri.x3 = verts[2]->pos.x;
                    tri.y3 = verts[2]->pos.y;
                    tri.u3 = verts[2]->uv.x;
                    tri.v3 = verts[2]->uv.y;
                    tri.c3.r = (verts[2]->col >> IM_COL32_R_SHIFT) & 0xFF;
                    tri.c3.g = (verts[2]->col >> IM_COL32_G_SHIFT) & 0xFF;
                    tri.c3.b = (verts[2]->col >> IM_COL32_B_SHIFT) & 0xFF;
                    tri.c3.a = (verts[2]->col >> IM_COL32_A_SHIFT) & 0xFF;
                    renderTriangle(&renderData, &tri);
                }
            }
            idx_buffer += pcmd->ElemCount;
        }
    }
}

#endif