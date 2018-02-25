#include "softraster.h"

position_t dot(const pixel_t& a, const pixel_t& b)
{
    return (a.x * b.x) + (a.y * b.y);
}

//texture_t fontAtlas;


// color24_t colto24(const color8_t& c)
// color8_t colto8(const color16_t& c)
// color24_t colto24(const color16_t& c)
// color32_t colto32(const color16_t& c)
// color8_t colto8(const color24_t& c)
// color16_t colto16(const color24_t& c)
// color32_t colto32(const color24_t& c)
// #if defined(COL8_ALPHACOLOR)
// color8_t colto8(const color32_t& c)
// color32_t colto32(const color8_t& c)
// #elif defined(COL8_COLOR)
// color8_t colto8(const color32_t& c)
// color32_t colto32(const color8_t& c)
// #else
// color8_t colto8(const color32_t& c)
// color32_t colto32(const color8_t& c)
// #endif
// color16_t colto16(const color32_t& c)
// color24_t colto24(const color32_t& c)

// void Softraster::sampleTexture(texture_t* tex, pixel_t* pixel)
// {
//     uint8_t srcR = pixel->c.r;
//     uint8_t srcG = pixel->c.g;
//     uint8_t srcB = pixel->c.b;
//     uint8_t srcA = pixel->c.a;
//     if (tex)
//     {
//         size_t w = *(tex->w());
//         size_t h = *(tex->h());
//     #ifdef TEXTURE_MODE_CLAMP
//         int u = (int)((pixel->u * w) + 0.5f);
//         int v = (int)((pixel->v * h) + 0.5f);
//         if (u < 0) u = 0;
//         else if (u > w) u = w - 1;
//         if (v < 0) v = 0;
//         else if (v > h) v = h - 1;
//     #else
//         int u = (int)((pixel->u * w) + 0.5f) % w;
//         int v = (int)((pixel->v * h) + 0.5f) % h;
//     #endif
//         switch(tex->colorMode)
//         {
//             case COLOR8: {
//                     pixel->c.a = (srcA * tex->tex8.col[u][v]) / 255;
//                     break;
//                 }
//             case COLOR16: {
//                     color24_t col1 = convCol<color24_t>(tex->tex16.col[u][v]);
//                     pixel->c.r = col1.r;
//                     pixel->c.g = col1.g;
//                     pixel->c.b = col1.b;
//                     pixel->c.a = 0xFF;
//                     break;
//                 }
//             case COLOR24: {
//                     color24_t col2 = tex->tex24.col[u][v];
//                     pixel->c.r = col2.r;
//                     pixel->c.g = col2.g;
//                     pixel->c.b = col2.b;
//                     pixel->c.a = 0xFF;
//                     break;
//                 }
//             case COLOR32: {
//                     color32_t col3 = tex->tex32.col[u][v];
//                     pixel->c.r = lerp(srcR, col3.r, col3.a);
//                     pixel->c.g = lerp(srcG, col3.g, col3.a);
//                     pixel->c.b = lerp(srcB, col3.b, col3.a);
//                     pixel->c.a = col3.a;
//                     break;
//                 }
//             default: break;
//         }
//     }
// }

// void Softraster::renderPixel(renderData_t* renderData, pixel_t* pixel)
// {
//     if ((pixel->x < 0) ||
//         (pixel->x >= renderData->screen->w) ||
//         (pixel->x < renderData->clipRect.x) ||
//         (pixel->x >= renderData->clipRect.z))
//         return;

//     sampleTexture(renderData->texture, pixel);
    
//     if (pixel->c.a == 0) return; //Pixel won't change color if alpha is 0 so return

//     clip_t* clip = renderData->screen->clip;

//     if (pixel->x < clip->x1 || clip->x1 < 0)
//     {
//         clip->x1 = pixel->x;
//     }
//     else if (pixel->x > clip->x2 || clip->x2 < 0)
//     {
//         clip->x2 = pixel->x;
//     }
    
//     if (pixel->y < clip->y1 || clip->y1 < 0)
//     {
//         clip->y1 = pixel->y;
//     }
//     else if (pixel->y > clip->y2 || clip->y2 < 0)
//     {
//         clip->y2 = pixel->y;
//     }
    
//     texture_t* buff = renderData->screen->buffer;
    
//     if (pixel->c.a == 255) //Skip alpha blending if alpha is 255
//     {
//         switch(buff->colorMode)
//         {
//             case COLOR8:
//                 buff->tex8.col[pixel->x][pixel->y] = convCol<color8_t>(pixel->c);
//                 break;
//             case COLOR16:
//                 buff->tex16.col[pixel->x][pixel->y] = convCol<color16_t>(pixel->c);
//                 break;
//             case COLOR24:
//                 buff->tex24.col[pixel->x][pixel->y] = convCol<color24_t>(pixel->c);
//                 break;
//             case COLOR32:
//                 buff->tex32.col[pixel->x][pixel->y] = pixel->c;
//                 break;
//             default: return;
//         }
//     }
//     else
//     {
//         color24_t prev;
        
//         //Convert to 24bit mode for alpha blending
//         switch(buff->colorMode)
//         {
//             case COLOR8:
//                 prev = convCol<color24_t>(buff->tex8.col[pixel->x][pixel->y]);
//                 break;
//             case COLOR16:
//                 prev = convCol<color24_t>(buff->tex16.col[pixel->x][pixel->y]);
//                 break;
//             case COLOR24:
//                 prev = buff->tex24.col[pixel->x][pixel->y];
//                 break;
//             case COLOR32:
//                 prev = convCol<color24_t>(buff->tex32.col[pixel->x][pixel->y]);
//                 break;
//             default: return;
//         }

//         color32_t next = pixel->c;
        
//         next.r = lerp(prev.r, pixel->c.r, pixel->c.a); 
//         next.g = lerp(prev.g, pixel->c.g, pixel->c.a); 
//         next.b = lerp(prev.b, pixel->c.b, pixel->c.a); 
        
//         switch(buff->colorMode) //Convert 32bit pixel to screenBuffer color mode
//         {
//             case COLOR8:
//                 buff->tex8.col[pixel->x][pixel->y] = convCol<color8_t>(next);
//                 break;
//             case COLOR16:
//                 buff->tex16.col[pixel->x][pixel->y] = convCol<color16_t>(next);
//                 break;
//             case COLOR24:
//                 buff->tex24.col[pixel->x][pixel->y] = convCol<color24_t>(next);
//                 break;
//             case COLOR32:
//                 buff->tex32.col[pixel->x][pixel->y] = next;
//                 break;
//             default: return;
//         }
//     }
// }

// void Softraster::renderLine(renderData_t* renderData, line_t* line)
// {

//     //Discard line if it is outside the frame buffer    
//     if ((line->x2 < 0) || 
//         (line->y < 0) ||
//         (line->x1 >= renderData->screen->w) ||
//         (line->y >= renderData->screen->h))
//         return;
    
//     //Discard line if it is outside of the clip area
//     if ((line->x2 < renderData->clipRect.x) ||
//         (line->y < renderData->clipRect.y) ||
//         (line->x1 >= renderData->clipRect.z) ||
//         (line->y >= renderData->clipRect.w))
//         return;

//     pixel_t pixel;
//     color32_t c;
//     float u;
//     float v;
//     float f;
    
//     bool optimC = (line->c1 == line->c2);
//     bool optimU = (line->u1 == line->u2);
//     bool optimV = (line->v1 == line->v2);
//     bool optimF = optimC && optimU && optimV;
    
//     for (float x = line->x1; x < line->x2; x++)
//     {
//         //Interpolation factor
//         if(!optimF)
//             f = fmaxf((x - line->x1) / (line->x2 - line->x1), 0.0f);

//         if(optimC)
//         {
//             c = line->c1;
//         }
//         else
//         {
//             c.r = lerp(line->c1.r, line->c2.r, f);
//             c.g = lerp(line->c1.g, line->c2.g, f);
//             c.b = lerp(line->c1.b, line->c2.b, f);
//             c.a = lerp(line->c1.a, line->c2.a, f);
//         }

//         u = optimU ? line->u1 : lerp(line->u1, line->u2, f);
//         v = optimV ? line->v1 : lerp(line->v1, line->v2, f);

//         pixel.x = (int)x;
//         pixel.y = (int)line->y;
//         pixel.c = c;
//         pixel.u = u;
//         pixel.v = v;
//         renderPixel(renderData, &pixel);
//     }
// }

// void Softraster::renderTriangleFB(renderData_t* renderData, triangle_t* tri)
// {    
//     if (tri->x3 < tri->x2)
//     {
//         float x = tri->x2;
//         float y = tri->y2;
//         color32_t c = tri->c2;
//         float u = tri->u2;
//         float v = tri->v2;
        
//         tri->x2 = tri->x3;
//         tri->y2 = tri->y3;
//         tri->c2 = tri->c3;
//         tri->u2 = tri->u3;
//         tri->v2 = tri->v3;
        
//         tri->x3 = x;
//         tri->y3 = y;
//         tri->c3 = c;
//         tri->u3 = u;
//         tri->v3 = v;
//     }

//     float x1Inc = tri->x2 - tri->x1;
//     x1Inc /= tri->y2 - tri->y1;
//     float x2Inc = tri->x3 - tri->x1;
//     x1Inc /= tri->y3 - tri->y1;

//     float x1 = tri->x1;
//     float x2 = tri->x1;

//     bool flatU = tri->u1 == tri->u2 && tri->u1 == tri->u3;
//     bool flatV = tri->v1 == tri->v2 && tri->v1 == tri->v3;
//     bool flatC = tri->c1 == tri->c2 && tri->c1 == tri->c3;
//     bool flatF = flatU && flatV && flatC;
    
//     line_t l;
//     float f, t1, t2, t3, t4;

//     float ystart = (tri->y1 > 0 ? tri->y1 : 0);
//     float yend = (tri->y2 < renderData->screen->h ? tri->y2 : renderData->screen->h);
    
//     for (float y = ystart; y < yend; y++)
//     {
//         f = y - tri->y1;
//         if(tri->y1 != tri->y2)
//           f /= tri->y2 - tri->y1;   

//         l.x1 = lerp(tri->x1, tri->x2, f); //x1;
//         l.x2 = lerp(tri->x1, tri->x3, f); //x2;
//         l.y = y;

//         if (!flatF)
//         {
//             //top from current-start
//             t1 = tri->x1 - x1;
//             t2 = tri->y1 - y;
//             //left from top
//             t3 = tri->x2 - tri->x1;
//             t4 = tri->y2 - tri->y1;
//             //(dist top from cur-start)/(dist left from top)
//             f = sqrtf((t1 * t1) + (t2 * t2));
//             if ((t3 != 0.0f) || (t4 != 0.0f))
//                 f /= sqrtf((t3 * t3) + (t4 * t4));
//         }

//         if (flatC)
//         {
//             l.c1 = tri->c1;
//             l.c2 = tri->c1;
//         }
//         else
//         {
//             l.c1.r = lerp(tri->c1.r, tri->c2.r, f);
//             l.c1.g = lerp(tri->c1.g, tri->c2.g, f);
//             l.c1.b = lerp(tri->c1.b, tri->c2.b, f);
//             l.c1.a = lerp(tri->c1.a, tri->c2.a, f);
//         }

//         l.u1 = flatU ? tri->u1 : lerp(tri->u1, tri->u2, f);
//         l.v1 = flatV ? tri->v1 : lerp(tri->v1, tri->v2, f);

//         if (!flatF)
//         {
//             //top from current-end
//             t1 = tri->x1 - x2;
//             t2 = tri->y1 - y;
//             //right from top
//             t3 = tri->x3 - tri->x1;
//             t4 = tri->y3 - tri->y1;
//             //(dist top from cur-end)/(dist right from top)
//             f = sqrtf((t1 * t1) + (t2 * t2));
//             if ((t3 != 0.0f) || (t4 != 0.0f))
//                 f /= sqrtf((t3 * t3) + (t4 * t4));
//         }

//         if (!flatC)
//         {
//             l.c2.r = lerp(tri->c1.r, tri->c3.r, f);
//             l.c2.g = lerp(tri->c1.g, tri->c3.g, f);
//             l.c2.b = lerp(tri->c1.b, tri->c3.b, f);
//             l.c2.a = lerp(tri->c1.a, tri->c3.a, f);
//         }

//         l.u2 = flatU ? tri->u1 : lerp(tri->u1, tri->u3, f);
//         l.v2 = flatV ? tri->v1 : lerp(tri->v1, tri->v3, f); 

//         x1 += x1Inc;
//         x2 += x2Inc;
//         renderLine(renderData, &l);
//     }
// }

// void Softraster::renderTriangleFT(renderData_t* renderData, triangle_t* tri)
// {    
//     if (tri->x2 < tri->x1)
//     {
//         float x = tri->x1;
//         float y = tri->y1;
//         color32_t c = tri->c1;
//         float u = tri->u1;
//         float v = tri->v1;
        
//         tri->x1 = tri->x2;
//         tri->y1 = tri->y2;
//         tri->c1 = tri->c2;
//         tri->u1 = tri->u2;
//         tri->v1 = tri->v2;
        
//         tri->x2 = x;
//         tri->y2 = y;
//         tri->c2 = c;
//         tri->u2 = u;
//         tri->v2 = v;
//     }

//     float x1Inc = tri->x1 - tri->x3;
//     x1Inc /= tri->y1 - tri->y3;
//     float x2Inc = tri->x2 - tri->x3;
//     x1Inc /= tri->y2 - tri->y3;

//     float x1 = tri->x3;
//     float x2 = tri->x3;

//     bool flatU = tri->u1 == tri->u2 && tri->u1 == tri->u3;
//     bool flatV = tri->v1 == tri->v2 && tri->v1 == tri->v3;
//     bool flatC = tri->c1 == tri->c2 && tri->c1 == tri->c3;
//     bool flatF = flatU && flatV && flatC;
    
//     line_t l;
//     float f, t1, t2, t3, t4;

//     float ystart = (tri->y3 < renderData->screen->h ? tri->y3 : renderData->screen->h);
//     float yend = (tri->y1 > 0 ? tri->y1 : 0);
    
//     bool optimX1 = tri->x1 == tri->x3;
//     bool optimX2 = tri->x2 == tri->x3;

//     for (float y = ystart; y >= yend; y--)
//     {
//         f = y - tri->y1;
//         if(tri->y1 != tri->y3)
//           f /= tri->y3 - tri->y1;   

//         l.x1 = lerp(tri->x1, tri->x3, f); //x1;
//         l.x2 = lerp(tri->x2, tri->x3, f); //x2;
//         l.y = y;

//         if (!flatF)
//         {
//             t1 = tri->x3 - x1;
//             t2 = tri->y3 - y;
//             t3 = tri->x3 - tri->x1;
//             t4 = tri->y3 - tri->y1;
//             f = sqrtf((t1 * t1) + (t2 * t2));
//             if ((t3 != 0.0f) || (t4 != 0.0f))
//                 f /= sqrtf((t3 * t3) + (t4 * t4));
//         }

//         if (flatC)
//         {
//             l.c1 = tri->c1;
//             l.c2 = tri->c1;
//         }
//         else
//         {
//             l.c1.r = lerp(tri->c3.r, tri->c1.r, f);
//             l.c1.g = lerp(tri->c3.g, tri->c1.g, f);
//             l.c1.b = lerp(tri->c3.b, tri->c1.b, f);
//             l.c1.a = lerp(tri->c3.a, tri->c1.a, f);
//         }

//         l.u1 = flatU ? tri->u1 : lerp(tri->u3, tri->u1, f);
//         l.v1 = flatV ? tri->v1 : lerp(tri->v3, tri->v1, f);

//         if (!flatF)
//         {
//             t1 = tri->x3 - (float)x2;
//             t2 = tri->y3 - y;
//             t3 = tri->x3 - tri->x2;
//             t4 = tri->y3 - tri->y2;
//             f = sqrtf((t1 * t1) + (t2 * t2));
//             if ((t3 != 0.0f) || (t4 != 0.0f))
//                 f /= sqrtf((t3 * t3) + (t4 * t4));
//         }

//         if (!flatC)
//         {
//             l.c2.r = lerp(tri->c3.r, tri->c2.r, f);
//             l.c2.g = lerp(tri->c3.g, tri->c2.g, f);
//             l.c2.b = lerp(tri->c3.b, tri->c2.b, f);
//             l.c2.a = lerp(tri->c3.a, tri->c2.a, f);
//         }

//         l.u2 = flatU ? tri->u1 : lerp(tri->u3, tri->u2, f);
//         l.v2 = flatV ? tri->v1 : lerp(tri->v3, tri->v2, f);

//         x1 += x1Inc;
//         x2 += x2Inc;
//         renderLine(renderData, &l);
//     }
// }

// void Softraster::renderTriangle(renderData_t* renderData, triangle_t* tri)
// {    
//     tri->y1 = roundf(tri->y1);
//     tri->y2 = roundf(tri->y2);
//     tri->y3 = roundf(tri->y3);

//     if (tri->y2 < tri->y1)
//     {
//         float x = tri->x1;
//         float y = tri->y1;
//         color32_t c = tri->c1;
//         float u = tri->u1;
//         float v = tri->v1;
        
//         tri->x1 = tri->x2;
//         tri->y1 = tri->y2;
//         tri->c1 = tri->c2;
//         tri->u1 = tri->u2;
//         tri->v1 = tri->v2;
        
//         tri->x2 = x;
//         tri->y2 = y;
//         tri->c2 = c;
//         tri->u2 = u;
//         tri->v2 = v;
//     }

//     if (tri->y3 < tri->y1)
//     {
//         float x = tri->x1;
//         float y = tri->y1;
//         color32_t c = tri->c1;
//         float u = tri->u1;
//         float v = tri->v1;
        
//         tri->x1 = tri->x3;
//         tri->y1 = tri->y3;
//         tri->c1 = tri->c3;
//         tri->u1 = tri->u3;
//         tri->v1 = tri->v3;
        
//         tri->x3 = x;
//         tri->y3 = y;
//         tri->c3 = c;
//         tri->u3 = u;
//         tri->v3 = v;
//     }

//     if (tri->y3 < tri->y2)
//     {
//         float x = tri->x2;
//         float y = tri->y2;
//         color32_t c = tri->c2;
//         float u = tri->u2;
//         float v = tri->v2;
        
//         tri->x2 = tri->x3;
//         tri->y2 = tri->y3;
//         tri->c2 = tri->c3;
//         tri->u2 = tri->u3;
//         tri->v2 = tri->v3;
        
//         tri->x3 = x;
//         tri->y3 = y;
//         tri->c3 = c;
//         tri->u3 = u;
//         tri->v3 = v;
//     }

//     if (tri->y2 == tri->y3)
//     {
//         renderTriangleFB(renderData, tri);
//         return;
//     }

//     if (tri->y1 == tri->y2)
//     {
//         renderTriangleFT(renderData, tri);
//         return;
//     }

//     float f = tri->y2 - tri->y1;
//     if (tri->y1 != tri->y3)
//         f /= tri->y3 - tri->y1;
    
//     float x = lerp(tri->x1, tri->x3, f);
//     float y = tri->y2;
    
//     color32_t c;

//     c.r = lerp(tri->c1.r, tri->c3.r, f);
//     c.g = lerp(tri->c1.g, tri->c3.g, f);
//     c.b = lerp(tri->c1.b, tri->c3.b, f);
//     c.a = lerp(tri->c1.a, tri->c3.a, f);

//     float u = lerp(tri->u1, tri->u3, f);
//     float v = lerp(tri->v1, tri->v3, f);

//     triangle_t flat;

//     flat.x1 = tri->x1;
//     flat.y1 = tri->y1;
//     flat.c1 = tri->c1;
//     flat.u1 = tri->u1;
//     flat.v1 = tri->v1;
//     flat.x2 = tri->x2;
//     flat.y2 = tri->y2;
//     flat.c2 = tri->c2;
//     flat.u2 = tri->u2;
//     flat.v2 = tri->v2;
//     flat.x3 = x;
//     flat.y3 = y;
//     flat.c3 = c;
//     flat.u3 = u;
//     flat.v3 = v;
//     renderTriangleFB(renderData, &flat);

//     flat.x1 = tri->x2;
//     flat.y1 = tri->y2;
//     flat.c1 = tri->c2;
//     flat.u1 = tri->u2;
//     flat.v1 = tri->v2;
//     flat.x2 = x;
//     flat.y2 = y;
//     flat.c2 = c;
//     flat.u2 = u;
//     flat.v2 = v;
//     flat.x3 = tri->x3;
//     flat.y3 = tri->y3;
//     flat.c3 = tri->c3;
//     flat.u3 = tri->u3;
//     flat.v3 = tri->v3;
//     renderTriangleFT(renderData, &flat);
// }

// void Softraster::renderRectangle(renderData_t* renderData, rectangle_t* rect)
// {
//     if ((rect->x2 < 0) || 
//         (rect->y2 < 0) ||
//         (rect->x1 >= renderData->screen->w) || 
//         (rect->y1 >= renderData->screen->h))
//         return;

//     if ((rect->x2 < renderData->clipRect.x) ||
//         (rect->y2 < renderData->clipRect.y) ||
//         (rect->x1 >= renderData->clipRect.z) || 
//         (rect->y1 >= renderData->clipRect.w))
//         return;
                    
//     for (float y = rect->y1; y < rect->y2; y++)
//     {
//         line_t l;
//         l.x1 = rect->x1;
//         l.x2 = rect->x2;
//         l.y = y;
//         l.c1 = rect->c;
//         l.c2 = rect->c;
//         l.u1 = rect->u1;
//         l.u2 = rect->u2;

//         float f = (y - rect->y1) / (rect->y2 - rect->y1);

//         l.v1 = lerp(rect->v1, rect->v2, f);
//         l.v2 = l.v1;
        
//         renderLine(renderData, &l);
//     }
// }

// void Softraster::renderTrapezoid(renderData_t* renderData, trapezoid_t* trap)
// {
//     vert_t& vmin = trap->v1;
//     vert_t& vmax = trap->v2;

//     if ((vmax.x < 0) || 
//         (vmax.y < 0) ||
//         (vmin.x >= renderData->screen->w) || 
//         (vmin.y >= renderData->screen->h))
//         return;

//     if ((trap->v2.x < renderData->clipRect.x) ||
//         (trap->v2.y < renderData->clipRect.y) ||
//         (trap->v1.x >= renderData->clipRect.z) || 
//         (trap->v1.y >= renderData->clipRect.w))
//         return;
    
//     const screen_t* screen = renderData->screen;
//     const ImVec4& clipRect = renderData->clipRect;

//     // No float trapezoid renderer by pervorgnsen https://gist.github.com/pervognsen
//     //const uint32_t one = 1 << 16;
//     const fixed_t oneDx = FIXED_POINT / (vmax.x - vmin.x);
//     const fixed_t oneDy = FIXED_POINT / (vmax.y - vmin.y);

//     const fixed_t duDx = (vmax.u - vmin.u) * oneDx;
//     const fixed_t dvDy = (vmax.v - vmin.v) * oneDy;

//     const fixed_t minclipx = (clipRect.x > 0 ? clipRect.x : 0);
//     const fixed_t minclipy = (clipRect.y > 0 ? clipRect.y : 0);

//     const fixed_t maxclipx = (clipRect.z < screen->w ? clipRect.z : screen->w);
//     const fixed_t maxclipy = (clipRect.w < screen->h ? clipRect.w : screen->h);

//     fixed_t u = (vmin.u + (duDx * (vmin.x - minclipx < 0 ? minclipx + vmin.x : 0))) * FIXED_POINT;
//     fixed_t v = (vmin.v + (dvDy * (vmin.y - minclipy < 0 ? minclipy + vmin.y : 0))) * FIXED_POINT;
//     const fixed_t u2 = u;

//     const fixed_t startx = (vmin.x > minclipx ? vmin.x : minclipx);
//     const fixed_t starty = (vmin.y > minclipy ? vmin.y : minclipy);

//     const fixed_t endx = (vmax.x < maxclipx ? vmax.x : maxclipx);
//     const fixed_t endy = (vmax.y < maxclipy ? vmax.y : maxclipy);

//     //line_t l;

//     screen->clip->x1 = (startx < screen->clip->x1 ? startx : screen->clip->x1);
//     screen->clip->y1 = (starty < screen->clip->y1 ? starty : screen->clip->y1);
//     screen->clip->x2 = (endx > screen->clip->x2 ? endx-1 : screen->clip->x2);
//     screen->clip->y2 = (endy > screen->clip->y2 ? endy-1 : screen->clip->y2);

//     pixel32_t p;

//     for (fixed_t y = starty; y < endy; y++)
//     {
//         p.y = y;
//         p.v = v;

//         v += dvDy; // iterators must be iterated before any continue statements
//         u = u2;

//         for (fixed_t x = startx; x < endx; x++)
//         {
//             p.x = x;
//             p.c = trap->c;
//             p.u = u;
//             u += duDx;

//             sampleTexture(renderData->texture, &p);
            
//             if (p.c.a == 0)
//             {
//                 continue;
//             }
//             else if (p.c.a == 255)
//             {
//                 screen->buffer->tex16.col[x][y] = colto16(p.c);
//             }
//             else
//             {
//                 color24_t prev = colto24(screen->buffer->tex16.col[x][y]);
//                 color32_t next = p.c;
//                 next.r = lerp(prev.r, p.c.r, p.c.a); 
//                 next.g = lerp(prev.g, p.c.g, p.c.a); 
//                 next.b = lerp(prev.b, p.c.b, p.c.a);
//                 screen->buffer->tex16.col[x][y] = colto16(next);
//             }
//         }
//     }
// }
