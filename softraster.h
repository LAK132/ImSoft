// Based on sronsse's software rasterizer for SDL https://github.com/sronsse/imgui/blob/sw_rasterizer_example/examples/sdl_sw_example/imgui_impl_sdl.cpp 
#ifndef SOFTRASTER_H
#define SOFTRASTER_H 
#ifdef ARDUINO
#include "arduino.h"
#endif
#include "stdint.h"
#include "math.h"
#include "imgui.h"

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

struct clip_t
{
    int16_t x1, y1, x2, y2;
};

#define position_t int32_t

struct pixel_t
{
    position_t x, y;
    float u, v;
    color32_t c;
};

struct line_t
{
    pixel_t p1, p2;
};

struct triangle_t
{
    pixel_t p1, p2, p3;
};

struct rectangle_t
{
    pixel_t p1, p2;
};

//Allows multiple texture modes/types to be stored in a type that can be handled by ImGui
// struct texture_t
// {
//     bool isSetup = false;
//     uint8_t colorMode = 0;
//     union 
//     {
//         texture8_t tex8;
//         ctexture8_t ctex8;
//         texture16_t tex16;
//         texture24_t tex24;
//         texture32_t tex32;
//     };
//     size_t* w() 
//     {
//         switch(colorMode)
//         {
//             case COLOR8:
//                 return &tex8.w;
//                 break;
//             case COLOR16:
//                 return &tex16.w;
//                 break;
//             case COLOR24:
//                 return &tex24.w;
//                 break;
//             case COLOR32:
//                 return &tex32.w;
//                 break;
//             default: 
//                 return NULL;
//                 break;
//         }
//     }
//     size_t* h() 
//     {
//         switch(colorMode)
//         {
//             case COLOR8:
//                 return &tex8.h;
//                 break;
//             case COLOR16:
//                 return &tex16.h;
//                 break;
//             case COLOR24:
//                 return &tex24.h;
//                 break;
//             case COLOR32:
//                 return &tex32.h;
//                 break;
//             default: 
//                 return NULL;
//                 break;
//         }
//     }
//     void clear()
//     {
//         switch(colorMode)
//         {
//             case COLOR8:
//                 tex8.clear();
//                 break;
//             case COLOR16:
//                 tex16.clear();
//                 break;
//             case COLOR24:
//                 tex24.clear();
//                 break;
//             case COLOR32:
//                 tex32.clear();
//                 break;
//             default: break;
//         }
//     }
//     void pre_init(uint8_t mode)
//     {
//         colorMode = mode;
//         switch(colorMode)
//         {
//             case COLOR8:
//                 tex8.pre_init();
//                 break;
//             case COLOR16:
//                 tex16.pre_init();
//                 break;
//             case COLOR24:
//                 tex24.pre_init();
//                 break;
//             case COLOR32:
//                 tex32.pre_init();
//                 break;
//             default: break;
//         }
//         isSetup = true;
//     }
//     void init(size_t x, size_t y, uint8_t mode)
//     {
//         colorMode = mode;
//         switch(colorMode)
//         {
//             case COLOR8:
//                 tex8.init(x, y);
//                 break;
//             case COLOR16:
//                 tex16.init(x, y);
//                 break;
//             case COLOR24:
//                 tex24.init(x, y);
//                 break;
//             case COLOR32:
//                 tex32.init(x, y);
//                 break;
//             default: break;
//         }
//         isSetup = true;
//     }
//     texture_t() : isSetup(false){}
//     texture_t(size_t x, size_t y, uint8_t mode)
//     {
//         init(x, y, mode);
//     }
//     ~texture_t()
//     {
//         if (isSetup)
//         {
//             switch(colorMode)
//             {
//                 case COLOR8:
//                     delete &tex8;
//                     break;
//                 case COLOR16:
//                     delete &tex16;
//                     break;
//                 case COLOR24:
//                     delete &tex24;
//                     break;
//                 case COLOR32:
//                     delete &tex32;
//                     break;
//                 default: break;
//             }
//         }
//         memset(&tex32, NULL, sizeof(tex32));
//         isSetup = false;
//     }
// };

//extern texture_t fontAtlas;

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
    texture_t() : col(NULL), isSetup(false){}
    texture_t(size_t x, size_t y)
    {
        init(x, y);
    }
    ~texture_t()
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

struct void_texture_t
{
    uint8_t mode;
    void* texture;
};

template<typename colType>
struct screen_t
{
    texture_t<colType>* buffer;
    clip_t* clip;
    size_t w;
    size_t h;
};

template<typename screenColType, typename texColType>
struct renderData_t
{
    screen_t<screenColType>* screen;
    ImVec4 clipRect;
    texture_t<texColType>* texture;
    renderData_t(screen_t<screenColType>* s, texture_t<texColType>* t, ImVec4 c) : screen(s), texture(t), clipRect(c){}
};

class Softraster
{
public:
    template<typename screenColType, typename texColType>
    static void renderRect(renderData_t<screenColType, texColType>* renderData, rectangle_t* rect);
    
    template<typename screenColType, typename texColType>
    static void renderRectBlend(renderData_t<screenColType, texColType>* renderData, rectangle_t* rect);
    
    template<typename screenColType, typename texColType>
    static void renderRectTex(renderData_t<screenColType, texColType>* renderData, rectangle_t* rect);
    
    template<typename screenColType, typename texColType>
    static void renderRectTexBlend(renderData_t<screenColType, texColType>* renderData, rectangle_t* rect);
    
    template<typename screenColType, typename texColType>
    static void renderTri(renderData_t<screenColType, texColType>* renderData, triangle_t* tri);
    
    template<typename screenColType, typename texColType>
    static void renderTriBlend(renderData_t<screenColType, texColType>* renderData, triangle_t* tri);
    
    template<typename screenColType, typename texColType>
    static void renderTriTex(renderData_t<screenColType, texColType>* renderData, triangle_t* tri);
    
    template<typename screenColType, typename texColType>
    static void renderTriTexBlend(renderData_t<screenColType, texColType>* renderData, triangle_t* tri);

    template<typename screenColType, typename texColType>
    static void renderCommand(const ImDrawVert* vtx_buffer, const ImDrawIdx* idx_buffer, const ImDrawCmd* pcmd, renderData_t<screenColType, texColType>* renderData);
    
    template<typename screenColType>//, typename texColType>
    static void renderDrawLists(ImDrawData* drawData, screen_t<screenColType>* screen);
};

position_t dot(const pixel_t& a, const pixel_t& b);

#include "softraster_templ.hpp" 

// typedef texture_t<color8_t> texture8_t;
// typedef texture_t<const color8_t> ctexture8_t;
// typedef texture_t<color16_t> texture16_t;
// typedef texture_t<color24_t> texture24_t;
// typedef texture_t<color32_t> texture32_t;

#endif
