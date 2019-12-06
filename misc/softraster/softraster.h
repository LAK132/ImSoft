#ifndef SOFTRASTER_H
#define SOFTRASTER_H

#include "defines.h"
#include "utils.h"
#include "color.h"
#include "texture.h"
#include "barycentric.h"

template<typename POS, typename SCREEN, typename TEXTURE, typename COLOR>
void renderQuadCore(
    texture_t<SCREEN>               &screen,
    const texture_t<TEXTURE>        &tex,
    const clip_t<POS>               &clip,
    const rectangle_t<POS, COLOR>   &quad,
    const bool                      alphaBlend
)
{
    if ((quad.p2.x < clip.x.min) ||
        (quad.p2.y < clip.y.min) ||
        (quad.p1.x >= clip.x.max) ||
        (quad.p1.y >= clip.y.max))
        return;

    const range_t<POS> qx = {quad.p1.x, quad.p2.x};
    const range_t<POS> qy = {quad.p1.y, quad.p2.y};

    const range_t<float> qu = {
        inl_max((float)quad.p1.u * tex.w, 0.0f),
        inl_min((float)quad.p2.u * tex.w, (float)tex.w)
    };

    const range_t<float> qv = {
        inl_max((float)quad.p1.v * tex.h, 0.0f),
        inl_min((float)quad.p2.v * tex.h, (float)tex.h)
    };

    const range_t<POS> rx = inl_min(qx, clip.x);
    const range_t<POS> ry = inl_min(qy, clip.y);

    const float duDx = (qu.max - qu.min) / (qx.max - qx.min);
    const float dvDy = (qv.max - qv.min) / (qy.max - qy.min);

    const float xoffset = (float)rx.min - (float)qx.min;
    const float yoffset = (float)ry.min - (float)qy.min;

    const float startu = qu.min + (xoffset > 0 ? duDx * xoffset : 0);
    const float startv = qv.min + (yoffset > 0 ? dvDy * yoffset : 0);

    bool blit = ((duDx == 1.0f) && (dvDy == 1.0f));

    if (blit)
    {
        const POS u = startu - rx.min;
        const POS v = startv - ry.min;
        if (alphaBlend)
        {
            for (POS y = ry.min; y < ry.max; ++y)
            {
                for (POS x = rx.min; x < rx.max; ++x)
                {
                    screen.at(x, y) %= quad.p1.c * tex.at(x + u, y + v);
                }
            }
        }
        else
        {
            for (POS y = ry.min; y < ry.max; ++y)
            {
                for (POS x = rx.min; x < rx.max; ++x)
                {
                    screen.at(x, y) = quad.p1.c * tex.at(x + u, y + v);
                }
            }
        }
    }
    else
    {
        if (alphaBlend)
        {
            float v = startv;
            POS y = ry.min;
            while (y < ry.max)
            {
                float u = startu;
                POS x = rx.min;
                while (x < rx.max)
                {
                    screen.at(x, y) %= quad.p1.c * tex.at(u, v);
                    ++x;
                    u += duDx;
                }
                ++y;
                v += dvDy;
            }
        }
        else
        {
            float v = startv;
            POS y = ry.min;
            while (y < ry.max)
            {
                float u = startu;
                POS x = rx.min;
                while (x < rx.max)
                {
                    screen.at(x, y) = quad.p1.c * tex.at(u, v);
                    ++x;
                    u += duDx;
                }
                ++y;
                v += dvDy;
            }
        }
    }
}

template<typename POS, typename SCREEN, typename COLOR>
void renderQuadCore(
    texture_t<SCREEN>               &screen,
    const clip_t<POS>               &clip,
    const rectangle_t<POS, COLOR>   &quad,
    const bool                      alphaBlend
)
{
    if ((quad.p2.x < clip.x.min) ||
        (quad.p2.y < clip.y.min) ||
        (quad.p1.x >= clip.x.max) ||
        (quad.p1.y >= clip.y.max))
        return;

    const range_t<POS> rx = inl_min({quad.p1.x, quad.p2.x}, clip.x);
    const range_t<POS> ry = inl_min({quad.p1.y, quad.p2.y}, clip.y);

    if (alphaBlend)
    {
        for (POS y = ry.min; y < ry.max; ++y)
        {
            for (POS x = rx.min; x < rx.max; ++x)
            {
                screen.at(x, y) %= quad.p1.c;
            }
        }
    }
    else
    {
        for (POS y = ry.min; y < ry.max; ++y)
        {
            for (POS x = rx.min; x < rx.max; ++x)
            {
                screen.at(x, y) = quad.p1.c;
            }
        }
    }
}

template<typename POS, typename SCREEN, typename COLOR>
void renderQuad(
    texture_t<SCREEN>               &screen,
    const texture_base_t            *tex,
    const clip_t<POS>               &clip,
    const rectangle_t<POS, COLOR>   &quad,
    const bool                      alphaBlend
)
{
    switch (tex == nullptr ? texture_type_t::NONE : tex->type)
    {
    case texture_type_t::ALPHA8:
        renderQuadCore(screen, *reinterpret_cast<const texture_alpha8_t*>(tex), clip, quad, alphaBlend);
        break;

    case texture_type_t::VALUE8:
        renderQuadCore(screen, *reinterpret_cast<const texture_value8_t*>(tex), clip, quad, alphaBlend);
        break;

    case texture_type_t::COLOR16:
        renderQuadCore(screen, *reinterpret_cast<const texture_color16_t*>(tex), clip, quad, alphaBlend);
        break;

    case texture_type_t::COLOR24:
        renderQuadCore(screen, *reinterpret_cast<const texture_color24_t*>(tex), clip, quad, alphaBlend);
        break;

    case texture_type_t::COLOR32:
        renderQuadCore(screen, *reinterpret_cast<const texture_color32_t*>(tex), clip, quad, alphaBlend);
        break;

    default:
        renderQuadCore(screen, clip, quad, alphaBlend);
        break;
    }
}

template<typename POS, typename SCREEN, typename TEXTURE, typename COLOR>
void renderTriCore(
    texture_t<SCREEN>           &screen,
    const texture_t<TEXTURE>    &tex,
    const clip_t<POS>           &clip,
    const range_t<POS>          &rY,
    const range_t<POS>          &rX1,
    const range_t<POS>          &rX2,
    const bary_t<POS, COLOR>    &bary,
    const bool                  alphaBlend
)
{
    if (alphaBlend)
    {
        const range_t<POS> ry = inl_min(rY, clip.y);
        for (POS y = ry.min; y < ry.max; ++y)
        {
            const float f = (y - rY.min) / (float)(rY.max - rY.min);
            const range_t<POS> rx =
                inl_min({lerp(rX1.min, rX2.min, f), lerp(rX1.max, rX2.max, f)}, clip.x);
            for (POS x = rx.min; x < rx.max; ++x)
            {
                pixel_t<POS, COLOR> p; p.x = x; p.y = y;
                barycentricUV(p, bary);

                POS u = (POS)((p.u * tex.w) + 0.5f) % tex.w;
                POS v = (POS)((p.v * tex.h) + 0.5f) % tex.h;

                screen.at(x, y) %= bary.a.c * tex.at(u, v);
            }
        }
    }
    else
    {
        const range_t<POS> ry = inl_min(rY, clip.y);
        for (POS y = ry.min; y < ry.max; ++y)
        {
            const float f = (y - rY.min) / (float)(rY.max - rY.min);
            const range_t<POS> rx =
                inl_min({lerp(rX1.min, rX2.min, f), lerp(rX1.max, rX2.max, f)}, clip.x);
            for (POS x = rx.min; x < rx.max; ++x)
            {
                pixel_t<POS, COLOR> p; p.x = x; p.y = y;
                barycentricUV(p, bary);

                POS u = (POS)((p.u * tex.w) + 0.5f) % tex.w;
                POS v = (POS)((p.v * tex.h) + 0.5f) % tex.h;

                screen.at(x, y) = bary.a.c * tex.at(u, v);
            }
        }
    }
}

template<typename POS, typename SCREEN, typename COLOR>
void renderTriCore(
    texture_t<SCREEN>           &screen,
    const clip_t<POS>           &clip,
    const range_t<POS>          &rY,
    const range_t<POS>          &rX1,
    const range_t<POS>          &rX2,
    const bary_t<POS, COLOR>    &bary,
    const bool                  uvBlend,
    const bool                  alphaBlend
)
{
    if (uvBlend)
    {
        if (alphaBlend)
        {
            const range_t<POS> ry = inl_min(rY, clip.y);
            for (POS y = ry.min; y < ry.max; ++y)
            {
                const float f = (y - rY.min) / (float)(rY.max - rY.min);
                const range_t<POS> rx =
                    inl_min({lerp(rX1.min, rX2.min, f), lerp(rX1.max, rX2.max, f)}, clip.x);
                for (POS x = rx.min; x < rx.max; ++x)
                {
                    pixel_t<POS, COLOR> p; p.x = x; p.y = y;
                    barycentricCol(p, bary);
                    screen.at(x, y) %= p.c;
                }
            }
        }
        else
        {
            const range_t<POS> ry = inl_min(rY, clip.y);
            for (POS y = ry.min; y < ry.max; ++y)
            {
                const float f = (y - rY.min) / (float)(rY.max - rY.min);
                const range_t<POS> rx =
                    inl_min({lerp(rX1.min, rX2.min, f), lerp(rX1.max, rX2.max, f)}, clip.x);
                for (POS x = rx.min; x < rx.max; ++x)
                {
                    pixel_t<POS, COLOR> p; p.x = x; p.y = y;
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
            const range_t<POS> ry = inl_min(rY, clip.y);
            for (POS y = ry.min; y < ry.max; ++y)
            {
                const float f = (y - rY.min) / (float)(rY.max - rY.min);
                const range_t<POS> rx =
                    inl_min({lerp(rX1.min, rX2.min, f), lerp(rX1.max, rX2.max, f)}, clip.x);
                for (POS x = rx.min; x < rx.max; ++x)
                {
                    screen.at(x, y) %= bary.a.c;
                }
            }
        }
        else
        {
            const range_t<POS> ry = inl_min(rY, clip.y);
            for (POS y = ry.min; y < ry.max; ++y)
            {
                const float f = (y - rY.min) / (float)(rY.max - rY.min);
                const range_t<POS> rx =
                    inl_min({lerp(rX1.min, rX2.min, f), lerp(rX1.max, rX2.max, f)}, clip.x);
                for (POS x = rx.min; x < rx.max; ++x)
                {
                    screen.at(x, y) = bary.a.c;
                }
            }
        }
    }
}

template<typename POS, typename SCREEN, typename COLOR>
void renderTri(
    texture_t<SCREEN>           &screen,
    const texture_base_t        *tex,
    const clip_t<POS>           &clip,
    const range_t<POS>          &rY,
    const range_t<POS>          &rX1,
    const range_t<POS>          &rX2,
    const bary_t<POS, COLOR>    &bary,
    const bool                  uvBlend,
    const bool                  alphaBlend
)
{
    switch (tex == nullptr ? texture_type_t::NONE : tex->type)
    {
    case texture_type_t::ALPHA8:
        renderTriCore(screen, *reinterpret_cast<const texture_alpha8_t*>(tex), clip, rY, rX1, rX2, bary, alphaBlend);
        break;

    case texture_type_t::VALUE8:
        renderTriCore(screen, *reinterpret_cast<const texture_value8_t*>(tex), clip, rY, rX1, rX2, bary, alphaBlend);
        break;

    case texture_type_t::COLOR16:
        renderTriCore(screen, *reinterpret_cast<const texture_color16_t*>(tex), clip, rY, rX1, rX2, bary, alphaBlend);
        break;

    case texture_type_t::COLOR24:
        renderTriCore(screen, *reinterpret_cast<const texture_color24_t*>(tex), clip, rY, rX1, rX2, bary, alphaBlend);
        break;

    case texture_type_t::COLOR32:
        renderTriCore(screen, *reinterpret_cast<const texture_color32_t*>(tex), clip, rY, rX1, rX2, bary, alphaBlend);
        break;

    default:
        renderTriCore(screen, clip, rY, rX1, rX2, bary, uvBlend, alphaBlend);
        break;
    }
}

template<typename POS, typename SCREEN, typename COLOR>
void renderTri(
    texture_t<SCREEN>       &screen,
    const texture_base_t    *tex,
    const clip_t<POS>       &clip,
    triangle_t<POS, COLOR>  &tri,
    const bool              uvBlend,
    const bool              alphaBlend
)
{
    if (tri.p1.y > tri.p2.y) swap(&(tri.p1), &(tri.p2));
    if (tri.p1.y > tri.p3.y) swap(&(tri.p1), &(tri.p3));
    if (tri.p2.y > tri.p3.y) swap(&(tri.p2), &(tri.p3));

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
        pixel_t<POS, COLOR> p4;
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

template<typename POS, typename SCREEN>
void renderCommand(
    texture_t<SCREEN>       &screen,
    const texture_base_t    *texture,
    const ImDrawVert        *vtx_buffer,
    const ImDrawIdx         *idx_buffer,
    const ImDrawCmd         &pcmd
)
{
    const clip_t<POS> clip = {
        {
            inl_max((POS)pcmd.ClipRect.x, (POS)0),
            inl_max((POS)pcmd.ClipRect.z, (POS)screen.w)
        },
        {
            inl_min((POS)pcmd.ClipRect.y, (POS)0),
            inl_min((POS)pcmd.ClipRect.w, (POS)screen.h)
        }
    };

    for(unsigned int i = 0; i < pcmd.ElemCount; i += 3)
    {
        const ImDrawVert* verts[] =
        {
            &vtx_buffer[idx_buffer[i]],
            &vtx_buffer[idx_buffer[i+1]],
            &vtx_buffer[idx_buffer[i+2]]
        };

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
                if (((nextVerts[v]->pos.x != tlpos.x) &&
                     (nextVerts[v]->pos.x != brpos.x)) ||
                    ((nextVerts[v]->pos.y != tlpos.y) &&
                     (nextVerts[v]->pos.y != brpos.y)) ||
                    ((nextVerts[v]->uv.x != tluv.x) &&
                     (nextVerts[v]->uv.x != bruv.x)) ||
                    ((nextVerts[v]->uv.y != tluv.y) &&
                     (nextVerts[v]->uv.y != bruv.y)))
                {
                    isRect = false;
                    break;
                }
            }

            if (isRect)
            {
                rectangle_t<POS, SCREEN> quad;
                quad.p1.x = tlpos.x;
                quad.p1.y = tlpos.y;
                quad.p2.x = brpos.x;
                quad.p2.y = brpos.y;
                quad.p1.u = tluv.x;
                quad.p1.v = tluv.y;
                quad.p2.u = bruv.x;
                quad.p2.v = bruv.y;
                quad.p1.c = color32_t(
                    verts[0]->col >> IM_COL32_R_SHIFT,
                    verts[0]->col >> IM_COL32_G_SHIFT,
                    verts[0]->col >> IM_COL32_B_SHIFT,
                    verts[0]->col >> IM_COL32_A_SHIFT
                );
                quad.p2.c = quad.p1.c;

                const bool noUV = (quad.p1.u == quad.p2.u) && (quad.p1.v == quad.p2.v);
                const bool alphaBlend = true;

                renderQuad(screen, noUV ? nullptr : texture, clip, quad, alphaBlend);

                i += 3;
                continue;
            }
        }

        triangle_t<POS, SCREEN> tri;
        // triangle_t<POS, color32_t> tri;
        tri.p1.x = verts[0]->pos.x;
        tri.p1.y = verts[0]->pos.y;
        tri.p1.u = verts[0]->uv.x;
        tri.p1.v = verts[0]->uv.y;
        tri.p1.c = color32_t(
            verts[0]->col >> IM_COL32_R_SHIFT,
            verts[0]->col >> IM_COL32_G_SHIFT,
            verts[0]->col >> IM_COL32_B_SHIFT,
            verts[0]->col >> IM_COL32_A_SHIFT
        );

        tri.p2.x = verts[1]->pos.x;
        tri.p2.y = verts[1]->pos.y;
        tri.p2.u = verts[1]->uv.x;
        tri.p2.v = verts[1]->uv.y;
        tri.p2.c = color32_t(
            verts[1]->col >> IM_COL32_R_SHIFT,
            verts[1]->col >> IM_COL32_G_SHIFT,
            verts[1]->col >> IM_COL32_B_SHIFT,
            verts[1]->col >> IM_COL32_A_SHIFT
        );

        tri.p3.x = verts[2]->pos.x;
        tri.p3.y = verts[2]->pos.y;
        tri.p3.u = verts[2]->uv.x;
        tri.p3.v = verts[2]->uv.y;
        tri.p3.c = color32_t(
            verts[2]->col >> IM_COL32_R_SHIFT,
            verts[2]->col >> IM_COL32_G_SHIFT,
            verts[2]->col >> IM_COL32_B_SHIFT,
            verts[2]->col >> IM_COL32_A_SHIFT
        );

        const bool noUV = (tri.p1.u == tri.p2.u) && (tri.p1.u == tri.p3.u) &&
                          (tri.p1.v == tri.p2.v) && (tri.p1.v == tri.p3.v);
        const bool flatCol = noUV || ((tri.p1.c == tri.p2.c) && (tri.p1.c == tri.p3.c));
        const bool alphaBlend = true;

        renderTri(screen, noUV ? nullptr : texture, clip, tri, !flatCol, alphaBlend);
    }
}

template<typename POS, typename SCREEN>
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
                renderCommand<POS>(
                    screen,
                    reinterpret_cast<const texture_base_t*>(pcmd.TextureId),
                    vtx_buffer,
                    idx_buffer,
                    pcmd);
            }
            idx_buffer += pcmd.ElemCount;
        }
    }
}

#endif // SOFTRASTER_H