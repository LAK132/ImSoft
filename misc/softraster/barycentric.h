#ifndef SOFTRASTER_BARYCENTRIC_H
#define SOFTRASTER_BARYCENTRIC_H

#include "utils.h"

template<typename POS, typename COL>
struct bary_t
{
  point_t<POS> p0, p1;
  pixel_t<POS, COL> a, b, c;
  POS d00, d01, d11;
  float denom;
};

template<typename POS, typename COL>
inline bary_t<POS, COL> baryPre(const pixel_t<POS, COL> &a,
                                const pixel_t<POS, COL> &b,
                                const pixel_t<POS, COL> &c)
{
  bary_t<POS, COL> bary;
  bary.p0.x  = b.x - a.x;
  bary.p0.y  = b.y - a.y;
  bary.p1.x  = c.x - a.x;
  bary.p1.y  = c.y - a.y;
  bary.d00   = dot(bary.p0, bary.p0);
  bary.d01   = dot(bary.p0, bary.p1);
  bary.d11   = dot(bary.p1, bary.p1);
  bary.denom = 1.0f / ((bary.d00 * bary.d11) - (bary.d01 * bary.d01));
  bary.a     = a;
  bary.b     = b;
  bary.c     = c;
  return bary;
}

template<typename POS, typename COL>
inline void barycentricCol(pixel_t<POS, COL> &p, const bary_t<POS, COL> &bary)
{
  pixel_t<POS, COL> p2;
  p2.x    = p.x - bary.a.x;
  p2.y    = p.y - bary.a.y;
  POS d20 = dot(p2, bary.p0);
  POS d21 = dot(p2, bary.p1);
  float v = (bary.d11 * d20 - bary.d01 * d21) * bary.denom;
  float w = (bary.d00 * d21 - bary.d01 * d20) * bary.denom;
  float u = 1.0f - v - w;
  p.c     = (bary.a.c * u) + (bary.b.c * v) + (bary.c.c * w);
}

template<typename POS, typename COL>
inline void barycentricUV(pixel_t<POS, COL> &p, const bary_t<POS, COL> &bary)
{
  pixel_t<POS, COL> p2;
  p2.x    = p.x - bary.a.x;
  p2.y    = p.y - bary.a.y;
  POS d20 = dot(p2, bary.p0);
  POS d21 = dot(p2, bary.p1);
  float v = (bary.d11 * d20 - bary.d01 * d21) * bary.denom;
  float w = (bary.d00 * d21 - bary.d01 * d20) * bary.denom;
  float u = 1.0f - v - w;
  p.u     = (bary.a.u * u) + (bary.b.u * v) + (bary.c.u * w);
  p.v     = (bary.a.v * u) + (bary.b.v * v) + (bary.c.v * w);
}

template<typename POS, typename COL>
inline void barycentricUVCol(pixel_t<POS, COL> &p,
                             const bary_t<POS, COL> &bary)
{
  pixel_t<POS, COL> p2;
  p2.x    = p.x - bary.a.x;
  p2.y    = p.y - bary.a.y;
  POS d20 = dot(p2, bary.p0);
  POS d21 = dot(p2, bary.p1);
  float v = (bary.d11 * d20 - bary.d01 * d21) * bary.denom;
  float w = (bary.d00 * d21 - bary.d01 * d20) * bary.denom;
  float u = 1.0f - v - w;
  p.u     = (bary.a.u * u) + (bary.b.u * v) + (bary.c.u * w);
  p.v     = (bary.a.v * u) + (bary.b.v * v) + (bary.c.v * w);
  p.c     = (bary.a.c * u) + (bary.b.c * v) + (bary.c.c * w);
}

#endif