// Based on sronsse's software rasterizer for SDL https://github.com/sronsse/imgui/blob/sw_rasterizer_example/examples/sdl_sw_example/imgui_impl_sdl.cpp 
#ifndef SOFTRASTER_H
#define SOFTRASTER_H 

#include "stdint.h"
#include "math.h"
#include "imgui.h"
#include <TFT_22_ILI9225.h>

#ifdef __cplusplus
  extern "C" {
#endif

//#define U8_TEXTURE

#define texel_t uint8_t

/*typedef struct texture
{
    int w;
    int h;
    texel_t* pixels;
} texture_t;*/

struct fontAtlas_t
{
    const texel_t* pixels;
    int w, h;
};

extern fontAtlas_t fontAtlas;

struct color_t
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
    bool operator==(const color_t& rhs) const
    {
        return (r == rhs.r) && (g == rhs.g) && (b == rhs.b) && (a == rhs.a);
    }
    bool operator!=(const color_t& rhs) const { return !(*this == rhs); }
};

struct texture_t
{
    int w, h;
    uint16_t** col;
    /*bool getUpd(int x, int y)
    { 
        return col[x][y].a > 0x0; 
    }
    void setCol(int x, int y, color_t c)
    {
        if (col[x][y] != c)
        {
            col[x][y] = c;
            col[x][y].a = 0xFF;
        }
    }
    bool getCol(int x, int y, color_t& c)
    {
        c = col[x][y];
        bool upd = getUpd(x, y);
        col[x][y].a = 0x0;
        return upd;
    }*/
    void clear()
    {
        for(int i = 0; i < w; i++)
        {
            for(int j = 0; j < h; j++)
            {
                col[i][j] = 0x0;
            }
        }
    }
    texture_t(){}
    texture_t(int x, int y)
    {
        w = x;
        h = y;
        col = (uint16_t**)malloc(x*sizeof(uint16_t*));
        for (int i = 0; i < x; i++)
        {
            col[i] = (uint16_t*)malloc(y*sizeof(uint16_t));
        }
    }
};

struct screen_t
{
    texture_t* buffer;
    uint16_t w;
    uint16_t h;
};

struct renderData_t
{
    screen_t* screen;
    fontAtlas_t* texture;
    ImVec4 clipRect;
};

struct pixel_t
{
    int x;
    int y;
    color_t c;
    float u;
    float v;
};

struct line_t
{
    float x1;
    float x2;
    float y;
    color_t c1;
    color_t c2;
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
    color_t c1;
    color_t c2;
    color_t c3;
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
    color_t c;
    float u1;
    float v1;
    float u2;
    float v2;
};

class Softraster : public TFT_22_ILI9225
{
public:
    using TFT_22_ILI9225::TFT_22_ILI9225;
    void sampleTexture(renderData_t* renderData, pixel_t* pixel);
    void renderPixel(renderData_t* renderData, pixel_t* pixel);
    void renderLine(renderData_t* renderData, line_t* line);
    void renderTriangleFB(renderData_t* renderData, triangle_t* tri);
    void renderTriangleFT(renderData_t* renderData, triangle_t* tri);
    void renderTriangle(renderData_t* renderData, triangle_t* tri);
    void renderRectangle(renderData_t* renderData, rectangle_t* rect);
    void renderDrawLists(ImDrawData* drawData, screen_t* screen);
};

#ifdef __cplusplus
  }
#endif

#endif
