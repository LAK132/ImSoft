// Based on sronsse's software rasterizer for SDL https://github.com/sronsse/imgui/blob/sw_rasterizer_example/examples/sdl_sw_example/imgui_impl_sdl.cpp 
#ifndef SOFTRASTER_H
#define SOFTRASTER_H 

#include "stdint.h"
#include "math.h"
#include "imgui.h"

template<typename T>
T lerp(T a, T b, uint8_t f) // [0, 255]
{
    return a + ((f * (b - a)) / 0xFF);
}

template<typename T>
T lerp(T a, T b, float f) // [0.0f, 1.0f]
{
    return a + (f * (b - a));
}

typedef uint8_t color8_t; //256 color

typedef uint16_t color16_t; //65k color (high color)

struct color24_t { //16M color (true color)
    color8_t r;
    color8_t g;
    color8_t b;
    bool operator==(const color24_t& rhs) const
    {
        return (r == rhs.r) && (g == rhs.g) && (b == rhs.b);
    }
    bool operator!=(const color24_t& rhs) const { return !(*this == rhs); }
};

struct color32_t { //16M color (true color) + 256 alpha
    color8_t r;
    color8_t g;
    color8_t b;
    color8_t a;
    bool operator==(const color32_t& rhs) const
    {
        return (r == rhs.r) && (g == rhs.g) && (b == rhs.b) && (a == rhs.a);
    }
    bool operator!=(const color32_t& rhs) const { return !(*this == rhs); }
};

#define COLOR8 sizeof(color8_t)
#define COLOR16 sizeof(color16_t)
#define COLOR24 sizeof(color24_t)
#define COLOR32 sizeof(color32_t)

color16_t   col8to16    (const color8_t& c);
color24_t   col8to24    (const color8_t& c);
color32_t   col8to32    (const color8_t& c);
color32_t   alp8to32    (const color8_t& c);
color8_t    col16to8    (const color16_t& c);
color24_t   col16to24   (const color16_t& c);
color32_t   col16to32   (const color16_t& c);
color8_t    col24to8    (const color24_t& c);
color16_t   col24to16   (const color24_t& c);
color32_t   col24to32   (const color24_t& c);
color8_t    col32to8    (const color32_t& c);
color16_t   col32to16   (const color32_t& c);
color24_t   col32to24   (const color32_t& c);

template<typename COL1, typename COL2>
void convertColor(COL1& c1, COL2& c2)
{
    switch(c1->colorMode)
    {
        case COLOR8:
            switch(c2->colorMode)
            {
                case COLOR8:
                    c2 = c1;
                    break;
                case COLOR16:
                    c2 = col8to16(c1);
                    break;
                case COLOR24:
                    c2 = col8to24(c1);
                    break;
                case COLOR32:
                    c2 = col8to32(c1);
                    break;
                default: return;
            }
            break;
        case COLOR16:
            switch(c2->colorMode)
            {
                case COLOR8:
                    c2 = col16to8(c1);
                    break;
                case COLOR16:
                    c2 = c1;
                    break;
                case COLOR24:
                    c2 = col16to24(c1);
                    break;
                case COLOR32:
                    c2 = col16to32(c1);
                    break;
                default: return;
            }
            break;
        case COLOR24:
            switch(c2->colorMode)
            {
                case COLOR8:
                    c2 = col24to8(c1);
                    break;
                case COLOR16:
                    c2 = col24to16(c1);
                    break;
                case COLOR24:
                    c2 = c1;
                    break;
                case COLOR32:
                    c2 = col24to32(c1);
                    break;
                default: return;
            }
            break;
        case COLOR32:
            switch(c2->colorMode)
            {
                case COLOR8:
                    c2 = col32to8(c1);
                    break;
                case COLOR16:
                    c2 = col32to16(c1);
                    break;
                case COLOR24:
                    c2 = col32to24(c1);
                    break;
                case COLOR32:
                    c2 = c1;
                    break;
                default: return;
            }
            break;
        default: return;
    }
}

//Template the texture struct so we can use different color modes/types
template <typename COL_T>
struct texture_templ_t
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

typedef texture_templ_t<color8_t> texture8_t;
typedef texture_templ_t<const color8_t> ctexture8_t;
typedef texture_templ_t<color16_t> texture16_t;
typedef texture_templ_t<color24_t> texture24_t;
typedef texture_templ_t<color32_t> texture32_t;

//Allows multiple texture modes/types to be stored in a type that can be handled by ImGui
struct texture_t
{
    bool isSetup = false;
    uint8_t colorMode = 0;
    union 
    {
        texture8_t tex8;
        ctexture8_t ctex8;
        texture16_t tex16;
        texture24_t tex24;
        texture32_t tex32;
    };
    size_t* w() 
    {
        switch(colorMode)
        {
            case COLOR8:
                return &tex8.w;
                break;
            case COLOR16:
                return &tex16.w;
                break;
            case COLOR24:
                return &tex24.w;
                break;
            case COLOR32:
                return &tex32.w;
                break;
            default: 
                return NULL;
                break;
        }
    }
    size_t* h() 
    {
        switch(colorMode)
        {
            case COLOR8:
                return &tex8.h;
                break;
            case COLOR16:
                return &tex16.h;
                break;
            case COLOR24:
                return &tex24.h;
                break;
            case COLOR32:
                return &tex32.h;
                break;
            default: 
                return NULL;
                break;
        }
    }
    void clear()
    {
        switch(colorMode)
        {
            case COLOR8:
                tex8.clear();
                break;
            case COLOR16:
                tex16.clear();
                break;
            case COLOR24:
                tex24.clear();
                break;
            case COLOR32:
                tex32.clear();
                break;
            default: break;
        }
    }
    void pre_init(uint8_t mode)
    {
        colorMode = mode;
        switch(colorMode)
        {
            case COLOR8:
                tex8.pre_init();
                break;
            case COLOR16:
                tex16.pre_init();
                break;
            case COLOR24:
                tex24.pre_init();
                break;
            case COLOR32:
                tex32.pre_init();
                break;
            default: break;
        }
        isSetup = true;
    }
    void init(size_t x, size_t y, uint8_t mode)
    {
        colorMode = mode;
        switch(colorMode)
        {
            case COLOR8:
                tex8.init(x, y);
                break;
            case COLOR16:
                tex16.init(x, y);
                break;
            case COLOR24:
                tex24.init(x, y);
                break;
            case COLOR32:
                tex32.init(x, y);
                break;
            default: break;
        }
        isSetup = true;
    }
    texture_t() : isSetup(false){}
    texture_t(size_t x, size_t y, uint8_t mode)
    {
        init(x, y, mode);
    }
    ~texture_t()
    {
        if (isSetup)
        {
            switch(colorMode)
            {
                case COLOR8:
                    delete &tex8;
                    break;
                case COLOR16:
                    delete &tex16;
                    break;
                case COLOR24:
                    delete &tex24;
                    break;
                case COLOR32:
                    delete &tex32;
                    break;
                default: break;
            }
        }
        memset(&tex32, NULL, sizeof(tex32));
        isSetup = false;
    }
};

extern texture_t fontAtlas;

struct screen_t
{
    texture_t* buffer;
    size_t w;
    size_t h;
};

struct renderData_t
{
    screen_t* screen;
    texture_t* texture;
    ImVec4 clipRect;
};

struct pixel_t
{
    int x;
    int y;
    color32_t c;
    float u;
    float v;
};

struct line_t
{
    float x1;
    float x2;
    float y;
    color32_t c1;
    color32_t c2;
    float u1;
    float u2;
    float v1;
    float v2;
};

struct triangle_t
{
    float x1;
    float y1;
    float x2;
    float y2;
    float x3;
    float y3;
    color32_t c1;
    color32_t c2;
    color32_t c3;
    float u1;
    float v1;
    float u2;
    float v2;
    float u3;
    float v3;
};

struct rectangle_t
{
    float x1;
    float y1;
    float x2;
    float y2;
    color32_t c;
    float u1;
    float v1;
    float u2;
    float v2;
};

class Softraster
{
public:
    static void sampleTexture(texture_t* tex, pixel_t* pixel);
    static void renderPixel(renderData_t* renderData, pixel_t* pixel);
    static void renderLine(renderData_t* renderData, line_t* line);
    static void renderTriangleFB(renderData_t* renderData, triangle_t* tri);
    static void renderTriangleFT(renderData_t* renderData, triangle_t* tri);
    static void renderTriangle(renderData_t* renderData, triangle_t* tri);
    static void renderRectangle(renderData_t* renderData, rectangle_t* rect);
    static void renderDrawLists(ImDrawData* drawData, screen_t* screen);
};

#endif
