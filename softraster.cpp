#include "softraster.h"

fontAtlas_t fontAtlas;
#include "Arduino.h"

uint8_t lerp(uint8_t a, uint8_t b, float f)
{
    return a + (uint8_t)(f * (b - a)); //(uint8_t)(((1.0f - fac) * (float)a) + (fac * (float)b));
}

float lerpf(float a, float b, float f)
{
    return a + (f * (b - a)); //(uint8_t)(((1.0f - fac) * (float)a) + (fac * (float)b));
}

void Softraster::sampleTexture(renderData_t* renderData, pixel_t* pixel)
{
    uint8_t srcR = pixel->c.r;
    uint8_t srcG = pixel->c.g;
    uint8_t srcB = pixel->c.b;
    uint8_t srcA = pixel->c.a;
    
    if (renderData->texture)
    {
    #if defined(TEXTURE_MODE_REPEAT)
        int u = (int)((pixel->u * renderData->texture->w) + 0.5f) % renderData->texture->w;
        int v = (int)((pixel->v * renderData->texture->h) + 0.5f) % renderData->texture->h;
    #endif
        int u = (int)((pixel->u * renderData->texture->w) + 0.5f);
        int v = (int)((pixel->v * renderData->texture->h) + 0.5f);
    #if defined(TEXTURE_MODE_CLAMP)
        if (u < 0) u = 0;
        else if (u > renderData->texture->w) u = renderData->texture->w - 1;
        if (v < 0) v = 0;
        else if (v > renderData->texture->h) v = renderData->texture->h - 1;
    #endif
        texel_t texel = renderData->texture->pixels[u + v * renderData->texture->w];
        if (sizeof(texel_t) == 4)
        {
            uint8_t tA = (texel >> IM_COL32_A_SHIFT) & 0xFF;
            uint8_t tR = (texel >> IM_COL32_R_SHIFT) & 0xFF;
            uint8_t tG = (texel >> IM_COL32_G_SHIFT) & 0xFF;
            uint8_t tB = (texel >> IM_COL32_B_SHIFT) & 0xFF;
            pixel->c.r = srcR * tR / 255;
            pixel->c.g = srcG * tG / 255;
            pixel->c.b = srcB * tB / 255;
            pixel->c.a = srcA * tA / 255;
        }
        else
        {
            pixel->c.a = srcA * (uint8_t)texel / 255;
        }
    }
}

void Softraster::renderPixel(renderData_t* renderData, pixel_t* pixel)
{
    if ((pixel->x < 0) ||
        (pixel->x >= renderData->screen->w) ||
        (pixel->x < renderData->clipRect.x) ||
        (pixel->x >= renderData->clipRect.z))
        return;
    
    // Get pointer to pixel for alpha blending
    uint16_t c = renderData->screen->buffer->col[pixel->x][pixel->y];
    uint8_t dstR;
    uint8_t dstG;
    uint8_t dstB;
    splitColor(c, dstR, dstG, dstB);

    sampleTexture(renderData, pixel);
    if (pixel->c.a == 0) return;
    
    uint8_t r = lerp(dstR, pixel->c.r, (float)pixel->c.a / 255.0f); 
    uint8_t g = lerp(dstG, pixel->c.g, (float)pixel->c.a / 255.0f); 
    uint8_t b = lerp(dstB, pixel->c.b, (float)pixel->c.a / 255.0f); 

    renderData->screen->buffer->col[pixel->x][pixel->y] = setColor(r, g, b);
}

void Softraster::renderLine(renderData_t* renderData, line_t* line)
{
    //Round x vals
    line->x1 = roundf(line->x1);
    line->x2 = roundf(line->x2);

    //Discard line if it is outside the frame buffer    
    if ((line->x2 < 0) || 
        (line->y < 0) ||
        (line->x1 >= renderData->screen->w) ||
        (line->y >= renderData->screen->h))
        return;
    
    //Discard line if it is outside of the clip area
    if ((line->x2 < renderData->clipRect.x) ||
        (line->y < renderData->clipRect.y) ||
        (line->x1 >= renderData->clipRect.z) ||
        (line->y >= renderData->clipRect.w))
        return;

    //See if we can do an optimised draw call
    /*if ((line->c1 == line->c2) && (line->u1 == line->u2) && (line->v1 == line->v2))
    {
        pixel_t pixel;
        pixel.c = line->c1;
        pixel.u = line->u1;
        pixel.v = line->v1;
        sampleTexture(renderData, &pixel);
        uint8_t r = (pixel.c.r * pixel.c.a) / 255;
        uint8_t g = (pixel.c.g * pixel.c.a) / 255;
        uint8_t b = (pixel.c.b * pixel.c.a) / 255;
        drawLine(line->x1, line->y, line->x2, line->y, setColor(r, g, b));
        return;
    }*/

    pixel_t pixel;
    color_t c;
    float u;
    float v;
    float f;

    Serial.print("LX1: ");Serial.println(line->x1);
    Serial.print("LX2: ");Serial.println(line->x2);
    Serial.print("LY: ");Serial.println(line->y);
    
    for (float x = line->x1; x < line->x2; x++)
    {
        //Interpolation factor
        f = fmaxf((x - line->x1) / (line->x2 - line->x1), 0.0f);

        c.r = lerp(line->c1.r, line->c2.r, f);
        c.g = lerp(line->c1.g, line->c2.g, f);
        c.b = lerp(line->c1.b, line->c2.b, f);
        c.a = lerp(line->c1.a, line->c2.a, f);

        u = lerpf(line->u1, line->u2, f);
        v = lerpf(line->v1, line->v2, f);

//        c.r = line->c1.r + (uint8_t)(f * (line->c2.r - line->c1.r));
//        c.g = line->c1.g + (uint8_t)(f * (line->c2.g - line->c1.g));
//        c.b = line->c1.b + (uint8_t)(f * (line->c2.b - line->c1.b));
//        c.a = lline->c1.a + (uint8_t)(f * (line->c2.a - line->c1.a));
//
//        u = line->u1 + f * (line->u2 - line->u1);
//        v = line->v1 + f * (line->v2 - line->v1);

        pixel.x = (int)x;
        pixel.y = (int)line->y;
        pixel.c = c;
        pixel.u = u;
        pixel.v = v;
        renderPixel(renderData, &pixel);
    }
}

void Softraster::renderTriangleFB(renderData_t* renderData, triangle_t* tri)
{
    //See if we can do an optimised draw call
    /*if ((tri->c1 == tri->c2) && (tri->c1 == tri->c3) && 
        (tri->u1 == tri->u2) && (tri->u1 == tri->u3) && 
        (tri->v1 == tri->v2) && (tri->v1 == tri->v3))
    {
        pixel_t pixel;
        pixel.c = tri->c1;
        pixel.u = tri->u1;
        pixel.v = tri->v1;
        sampleTexture(renderData, &pixel);
        uint8_t r = (pixel.c.r * pixel.c.a) / 255;
        uint8_t g = (pixel.c.g * pixel.c.a) / 255;
        uint8_t b = (pixel.c.b * pixel.c.a) / 255;
        fillTriangle(tri->x1, tri->y1, tri->x2, tri->y2, tri->x3, tri->y3, setColor(r, g, b));
        return;
    }*/
    
    if (tri->x3 < tri->x2)
    {
        float x = tri->x2;
        float y = tri->y2;
        color_t c = tri->c2;
        float u = tri->u2;
        float v = tri->v2;
        
        tri->x2 = tri->x3;
        tri->y2 = tri->y3;
        tri->c2 = tri->c3;
        tri->u2 = tri->u3;
        tri->v2 = tri->v3;
        
        tri->x3 = x;
        tri->y3 = y;
        tri->c3 = c;
        tri->u3 = u;
        tri->v3 = v;
    }
    
    //fillTriangle(tri->x1, tri->y1, tri->x2, tri->y2, tri->x3, tri->y3, setColor(tri->c1.r, tri->c1.g, tri->c1.b));

    float x1Inc = tri->x2 - tri->x1;
    x1Inc /= tri->y2 - tri->y1;
    float x2Inc = tri->x3 - tri->x1;
    x1Inc /= tri->y3 - tri->y1;

    float x1 = tri->x1;
    float x2 = tri->x1;
    
    line_t l;

    float ystart = (tri->y1 > 0 ? tri->y1 : 0);
    float yend = (tri->y2 < renderData->screen->h ? tri->y2 : renderData->screen->h);
    
    for (float y = ystart; y < yend; y++)
    {
        float f = y - tri->y1;
        if(tri->y1 != tri->y2)
          f /= tri->y2 - tri->y1;   

        l.x1 = lerpf(tri->x1, tri->x2, f); //x1;
        l.x2 = lerpf(tri->x1, tri->x3, f); //x2;
        l.y = y;

        //top from current-left
        float t1 = tri->x1 - x1;
        float t2 = tri->y1 - y;
        //left from top
        float t3 = tri->x2 - tri->x1;
        float t4 = tri->y2 - tri->y1;
        //(dist top from cur-left)/(dist left from top)
        f = sqrtf((t1 * t1) + (t2 * t2));
        if ((t3 != 0.0f) || (t4 != 0.0f))
            f /= sqrtf((t3 * t3) + (t4 * t4));

        l.c1.r = lerp(tri->c1.r, tri->c2.r, f);
        l.c1.g = lerp(tri->c1.g, tri->c2.g, f);
        l.c1.b = lerp(tri->c1.b, tri->c2.b, f);
        l.c1.a = lerp(tri->c1.a, tri->c2.a, f);

        l.u1 = lerpf(tri->u1, tri->u2, f);
        l.v1 = lerpf(tri->v1, tri->v2, f);

        //top from current-right
        t1 = tri->x1 - (float)x2;
        t2 = tri->y1 - y;
        //right from top
        t3 = tri->x3 - tri->x1;
        t4 = tri->y3 - tri->y1;
        //(dist top from cur-right)/(dist right from top)
        f = sqrtf((t1 * t1) + (t2 * t2));
        if ((t3 != 0.0f) || (t4 != 0.0f))
            f /= sqrtf((t3 * t3) + (t4 * t4));

        l.c2.r = lerp(tri->c1.r, tri->c3.r, f);
        l.c2.g = lerp(tri->c1.g, tri->c3.g, f);
        l.c2.b = lerp(tri->c1.b, tri->c3.b, f);
        l.c2.a = lerp(tri->c1.a, tri->c3.a, f);

        l.u2 = lerpf(tri->u1, tri->u3, f);
        l.v2 = lerpf(tri->v1, tri->v3, f); 

        x1 += x1Inc;
        x2 += x2Inc;
        //drawLine(l.x1, l.y, l.x2, l.y, setColor(l.c1.r, l.c1.g, l.c1.b));
        renderLine(renderData, &l);
    }
}

void Softraster::renderTriangleFT(renderData_t* renderData, triangle_t* tri)
{
    //See if we can do an optimised draw call
    /*if ((tri->c1 == tri->c2) && (tri->c1 == tri->c3) && 
        (tri->u1 == tri->u2) && (tri->u1 == tri->u3) && 
        (tri->v1 == tri->v2) && (tri->v1 == tri->v3))
    {
        pixel_t pixel;
        pixel.c = tri->c1;
        pixel.u = tri->u1;
        pixel.v = tri->v1;
        sampleTexture(renderData, &pixel);
        uint8_t r = (pixel.c.r * pixel.c.a) / 255;
        uint8_t g = (pixel.c.g * pixel.c.a) / 255;
        uint8_t b = (pixel.c.b * pixel.c.a) / 255;
        fillTriangle(tri->x1, tri->y1, tri->x2, tri->y2, tri->x3, tri->y3, setColor(r, g, b));
        return;
    }*/
    
    if (tri->x2 < tri->x1)
    {
        float x = tri->x1;
        float y = tri->y1;
        color_t c = tri->c1;
        float u = tri->u1;
        float v = tri->v1;
        
        tri->x1 = tri->x2;
        tri->y1 = tri->y2;
        tri->c1 = tri->c2;
        tri->u1 = tri->u2;
        tri->v1 = tri->v2;
        
        tri->x2 = x;
        tri->y2 = y;
        tri->c2 = c;
        tri->u2 = u;
        tri->v2 = v;
    }

    float x1Inc = tri->x1 - tri->x3;
    x1Inc /= tri->y1 - tri->y3;
    float x2Inc = tri->x2 - tri->x3;
    x1Inc /= tri->y2 - tri->y3;

    float x1 = tri->x3;
    float x2 = tri->x3;
    
    line_t l;

    float ystart = (tri->y3 < renderData->screen->h ? tri->y3 : renderData->screen->h);
    float yend = (tri->y1 > 0 ? tri->y1 : 0);
    
    for (float y = ystart; y >= yend; y--)
    {
        float f = y - tri->y1;
        if(tri->y1 != tri->y3)
          f /= tri->y3 - tri->y1;   

        l.x1 = lerpf(tri->x1, tri->x3, f); //x1;
        l.x2 = lerpf(tri->x2, tri->x3, f); //x2;
        l.y = y;

        float t1 = tri->x3 - x1;
        float t2 = tri->y3 - y;
        float t3 = tri->x3 - tri->x1;
        float t4 = tri->y3 - tri->y1;
        f = sqrtf((t1 * t1) + (t2 * t2));
        if ((t3 != 0.0f) || (t4 != 0.0f))
            f /= sqrtf((t3 * t3) + (t4 * t4));

        l.c1.r = lerp(tri->c3.r, tri->c1.r, f);
        l.c1.g = lerp(tri->c3.g, tri->c1.g, f);
        l.c1.b = lerp(tri->c3.b, tri->c1.b, f);
        l.c1.a = lerp(tri->c3.a, tri->c1.a, f);

        l.u1 = lerpf(tri->u3, tri->u1, f);
        l.v1 = lerpf(tri->v3, tri->v1, f);

        t1 = tri->x3 - (float)x2;
        t2 = tri->y3 - y;
        t3 = tri->x3 - tri->x2;
        t4 = tri->y3 - tri->y2;
        f = sqrtf((t1 * t1) + (t2 * t2));
        if ((t3 != 0.0f) || (t4 != 0.0f))
            f /= sqrtf((t3 * t3) + (t4 * t4));

        l.c2.r = lerp(tri->c3.r, tri->c2.r, f);
        l.c2.g = lerp(tri->c3.g, tri->c2.g, f);
        l.c2.b = lerp(tri->c3.b, tri->c2.b, f);
        l.c2.a = lerp(tri->c3.a, tri->c2.a, f);

        l.u2 = lerpf(tri->u3, tri->u2, f);
        l.v2 = lerpf(tri->v3, tri->v2, f);

        x1 += x1Inc;
        x2 += x2Inc;
        //drawLine(l.x1, l.y, l.x2, l.y, setColor(l.c1.r, l.c1.g, l.c1.b));
        renderLine(renderData, &l);
    }
}

void Softraster::renderTriangle(renderData_t* renderData, triangle_t* tri)
{
    //See if we can do an optimised draw call
    /*if ((tri->c1 == tri->c2) && (tri->c1 == tri->c3) && 
        (tri->u1 == tri->u2) && (tri->u1 == tri->u3) && 
        (tri->v1 == tri->v2) && (tri->v1 == tri->v3))
    {
        pixel_t pixel;
        pixel.c = tri->c1;
        pixel.u = tri->u1;
        pixel.v = tri->v1;
        sampleTexture(renderData, &pixel);
        uint8_t r = (pixel.c.r * pixel.c.a) / 255;
        uint8_t g = (pixel.c.g * pixel.c.a) / 255;
        uint8_t b = (pixel.c.b * pixel.c.a) / 255;
        fillTriangle(tri->x1, tri->y1, tri->x2, tri->y2, tri->x3, tri->y3, setColor(r, g, b));
        return;
    }*/
    
    tri->y1 = roundf(tri->y1);
    tri->y2 = roundf(tri->y2);
    tri->y3 = roundf(tri->y3);

    if (tri->y2 < tri->y1)
    {
        float x = tri->x1;
        float y = tri->y1;
        color_t c = tri->c1;
        float u = tri->u1;
        float v = tri->v1;
        
        tri->x1 = tri->x2;
        tri->y1 = tri->y2;
        tri->c1 = tri->c2;
        tri->u1 = tri->u2;
        tri->v1 = tri->v2;
        
        tri->x2 = x;
        tri->y2 = y;
        tri->c2 = c;
        tri->u2 = u;
        tri->v2 = v;
    }

    if (tri->y3 < tri->y1)
    {
        float x = tri->x1;
        float y = tri->y1;
        color_t c = tri->c1;
        float u = tri->u1;
        float v = tri->v1;
        
        tri->x1 = tri->x3;
        tri->y1 = tri->y3;
        tri->c1 = tri->c3;
        tri->u1 = tri->u3;
        tri->v1 = tri->v3;
        
        tri->x3 = x;
        tri->y3 = y;
        tri->c3 = c;
        tri->u3 = u;
        tri->v3 = v;
    }

    if (tri->y3 < tri->y2)
    {
        float x = tri->x2;
        float y = tri->y2;
        color_t c = tri->c2;
        float u = tri->u2;
        float v = tri->v2;
        
        tri->x2 = tri->x3;
        tri->y2 = tri->y3;
        tri->c2 = tri->c3;
        tri->u2 = tri->u3;
        tri->v2 = tri->v3;
        
        tri->x3 = x;
        tri->y3 = y;
        tri->c3 = c;
        tri->u3 = u;
        tri->v3 = v;
    }

    if (tri->y2 == tri->y3)
    {
        //fillTriangle(tri->x1, tri->y1, tri->x2, tri->y2, tri->x3, tri->y3, setColor(tri->c1.r, tri->c1.g, tri->c1.b));
        renderTriangleFB(renderData, tri);
        return;
    }

    if (tri->y1 == tri->y2)
    {
        //fillTriangle(tri->x1, tri->y1, tri->x2, tri->y2, tri->x3, tri->y3, setColor(tri->c1.r, tri->c1.g, tri->c1.b));
        renderTriangleFT(renderData, tri);
        return;
    }

    float f = tri->y2 - tri->y1;
    if (tri->y1 != tri->y3)
        f /= tri->y3 - tri->y1;
    Serial.print("F: ");Serial.println(f);
    float x = lerpf(tri->x1, tri->x3, f); //tri->x1 + f * (tri->x3 - tri->x1);
    float y = tri->y2;
    
    color_t c;

    c.r = lerp(tri->c1.r, tri->c3.r, f);
    c.g = lerp(tri->c1.g, tri->c3.g, f);
    c.b = lerp(tri->c1.b, tri->c3.b, f);
    c.a = lerp(tri->c1.a, tri->c3.a, f);

    float u = lerpf(tri->u1, tri->u3, f);
    float v = lerpf(tri->v1, tri->v3, f);
    
//    c.r = tri->c1.r + (uint8_t)(f * (tri->c3.r - tri->c1.r));
//    c.g = tri->c1.g + (uint8_t)(f * (tri->c3.g - tri->c1.g));
//    c.b = tri->c1.b + (uint8_t)(f * (tri->c3.b - tri->c1.b));
//    c.a = tri->c1.a + (uint8_t)(f * (tri->c3.a - tri->c1.a));
//    
//    float u = tri->u1 + (f * (tri->u3 - tri->u1));
//    float v = tri->v1 + (f * (tri->v3 - tri->v1));

    triangle_t flat;

    Serial.print("X1: "); Serial.println(tri->x1);
    Serial.print("X2: "); Serial.println(tri->x2);
    Serial.print("X3: "); Serial.println(x);
    Serial.print("X4: "); Serial.println(tri->x3);
    flat.x1 = tri->x1;
    flat.y1 = tri->y1;
    flat.c1 = tri->c1;
    flat.u1 = tri->u1;
    flat.v1 = tri->v1;
    flat.x2 = tri->x2;
    flat.y2 = tri->y2;
    flat.c2 = tri->c2;
    flat.u2 = tri->u2;
    flat.v2 = tri->v2;
    flat.x3 = x;
    flat.y3 = y;
    flat.c3 = c;
    flat.u3 = u;
    flat.v3 = v;
    //fillTriangle(flat.x1, flat.y1, flat.x2, flat.y2, flat.x3, flat.y3, setColor(flat.c1.r, flat.c1.g, flat.c1.b));
    renderTriangleFB(renderData, &flat);

    flat.x1 = tri->x2;
    flat.y1 = tri->y2;
    flat.c1 = tri->c2;
    flat.u1 = tri->u2;
    flat.v1 = tri->v2;
    flat.x2 = x;
    flat.y2 = y;
    flat.c2 = c;
    flat.u2 = u;
    flat.v2 = v;
    flat.x3 = tri->x3;
    flat.y3 = tri->y3;
    flat.c3 = tri->c3;
    flat.u3 = tri->u3;
    flat.v3 = tri->v3;
    //fillTriangle(flat.x1, flat.y1, flat.x2, flat.y2, flat.x3, flat.y3, setColor(flat.c1.r, flat.c1.g, flat.c1.b));
    renderTriangleFT(renderData, &flat);
}

void Softraster::renderRectangle(renderData_t* renderData, rectangle_t* rect)
{
    if ((rect->x2 < 0) || 
        (rect->y2 < 0) ||
        (rect->x1 >= renderData->screen->w) || 
        (rect->y1 >= renderData->screen->h))
        return;

    if ((rect->x2 < renderData->clipRect.x) ||
        (rect->y2 < renderData->clipRect.y) ||
        (rect->x1 >= renderData->clipRect.z) || 
        (rect->y1 >= renderData->clipRect.w))
        return;

    //See if we can do an optimised draw call
    /*if ((rect->u1 == rect->u2) && (rect->v1 == rect->v2));
    {
        pixel_t pixel;
        pixel.c = rect->c;
        pixel.u = rect->u1;
        pixel.v = rect->v1;
        sampleTexture(renderData, &pixel);
        uint8_t r = (pixel.c.r * pixel.c.a) / 255;
        uint8_t g = (pixel.c.g * pixel.c.a) / 255;
        uint8_t b = (pixel.c.b * pixel.c.a) / 255;
        TFT_22_ILI9225* tft = renderData->screen->tft;
        tft->fillRectangle(rect->x1, rect->y1, rect->x2, rect->y2, tft->setColor(r, g, b));
        return;
    }*/
                    
    for (float y = rect->y1; y < rect->y2; y++)
    {
        line_t l;
        l.x1 = rect->x1;
        l.x2 = rect->x2;
        l.y = y;
        l.c1 = rect->c;
        l.c2 = rect->c;
        l.u1 = rect->u1;
        l.u2 = rect->u2;

        float f = (y - rect->y1) / (rect->y2 - rect->y1);

        l.v1 = lerpf(rect->v1, rect->v2, f);//rect->v1 + f * (rect->v2 - rect->v1);
        l.v2 = l.v1;
        
        renderLine(renderData, &l);
    }
}

void Softraster::renderDrawLists(ImDrawData* drawData, screen_t* screen)
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
                renderData.texture = (fontAtlas_t*)pcmd->TextureId; //&fontAtlas;
                renderData.clipRect = pcmd->ClipRect;

                for(unsigned int i = 0; i < pcmd->ElemCount; i += 3)
                {
                    //Serial.println(i);
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
                            //Serial.println("Rect");
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
//                    Serial.print("Tri");
//                    Serial.println(verts[0]->col);
                    //fillTriangle(tri.x1, tri.y1, tri.x2, tri.y2, tri.x3, tri.y3, setColor(tri.c1.r, tri.c1.g, tri.c1.b));
                    renderTriangle(&renderData, &tri);
                }
            }
            idx_buffer += pcmd->ElemCount;
        }
    }
}
