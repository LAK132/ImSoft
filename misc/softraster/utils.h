#ifndef SOFTRASTER_UTILS_H
#define SOFTRASTER_UTILS_H

#ifdef ARDUINO
#  pragma GCC optimize("-03")
#endif

#include <cmath>
#include "defines.h"

template<typename T>
struct range_t
{
  T min, max;
};

template<typename T>
struct clip_t
{
  range_t<T> x, y;
};

template<typename T>
struct point_t
{
  T x, y;
};

template<typename T>
struct edge_t
{
  point_t<T> p1, p2;
};

template<typename POS, typename COL>
struct pixel_t
{
  POS x, y;
  float u, v;
  COL c;
};

template<typename POS, typename COL>
struct triangle_t
{
  pixel_t<POS, COL> p1, p2, p3;
};

template<typename POS, typename COL>
struct rectangle_t
{
  pixel_t<POS, COL> p1, p2;
};

template<typename T>
inline void swap(T *tri1, T *tri2)
{
  T temp;
  memcpy(static_cast<void*>(&temp), tri1, sizeof(T));
  memcpy(static_cast<void*>(tri1), tri2, sizeof(T));
  memcpy(static_cast<void*>(tri2), &temp, sizeof(T));
}

// [0x00, 0xFF]
template<typename T>
FORCE_INLINE T lerp(T a, T b, uint8_t f)
{
  return a + ((f * (b - a)) / 0xFF);
}

template<typename T>
FORCE_INLINE T lerp(T a, T b, float f)
{
  return ((1.0f - f) * a) + (f * b);
}

template<typename T>
FORCE_INLINE T inl_min(const T a, const T b)
{
  return a > b ? b : a;
}

template<typename T>
FORCE_INLINE T inl_max(const T a, const T b)
{
  return a > b ? a : b;
}

template<typename T>
FORCE_INLINE range_t<T> inl_min(const range_t<T> &a, const range_t<T> &b)
{
  return {inl_max(a.min, b.min), inl_min(a.max, b.max)};
}

template<typename T>
FORCE_INLINE range_t<T> inl_max(const range_t<T> &a, const range_t<T> &b)
{
  return {inl_min(a.min, b.min), inl_max(a.max, b.max)};
}

template<typename P, typename C>
FORCE_INLINE P dot(const pixel_t<P, C> &a, const pixel_t<P, C> &b)
{
  return (a.x * b.x) + (a.y * b.y);
}

template<typename P>
FORCE_INLINE P dot(const point_t<P> &a, const point_t<P> &b)
{
  return (a.x * b.x) + (a.y * b.y);
}

template<typename P, typename C>
FORCE_INLINE P dot(const pixel_t<P, C> &a, const point_t<P> &b)
{
  return (a.x * b.x) + (a.y * b.y);
}

template<typename P, typename C>
FORCE_INLINE P dot(const point_t<P> &a, const pixel_t<P, C> &b)
{
  return (a.x * b.x) + (a.y * b.y);
}

FORCE_INLINE float mod(const float a, const float b)
{
  return fmodf(a, b);
}

template<typename T>
FORCE_INLINE T mod(const T a, const T b)
{
  return a % b;
}

// [Negative half-space]
// E.p0 *-------->* E.p1
// [Positive half-space]
template<typename T>
FORCE_INLINE T halfspace(const edge_t<T> &edge, const point_t<T> &point)
{
  return ((edge.p2.x - edge.p1.x) * (point.y - edge.p1.y)) -
         ((edge.p2.y - edge.p1.y) * (point.x - edge.p1.x));
}

FORCE_INLINE bool equivalent(float a, float b)
{
  return fabs(a - b) < FLT_EPSILON;
}

template<typename T>
FORCE_INLINE bool equivalent(T a, T b)
{
  return a == b;
}

template<typename T>
FORCE_INLINE bool is_top_left(const edge_t<T> &edge,
                              const point_t<T> &other_point)
{
  // if (equivalent(edge.p1.y, edge.p2.y))
  if (edge.p1.y == edge.p2.y)
  {
    // edge is horizontal.
    if (edge.p1.x < edge.p2.x)
    {
      // other_point must be in the positive half-sace to be below edge.
      return halfspace(edge, other_point) >= 0;
    }
    else
    {
      // other_point must be in the negative half-space to be below edge.
      return halfspace(edge, other_point) <= 0;
    }
  }
  else
  {
    // edge is not horizontal.
    if (edge.p1.y > edge.p2.y)
    {
      // other_point must be in the positive half-sace to be right of edge.
      return halfspace(edge, other_point) >= 0;
    }
    else
    {
      // other_point must be in the negative half-space to be right of edge.
      return halfspace(edge, other_point) <= 0;
    }
  }
}

FORCE_INLINE float orient(const edge_t<float> &edge,
                          const point_t<float> &other_point,
                          const point_t<float> &point)
{
  return -halfspace(edge, point) + (is_top_left(edge, other_point) ? 0 : -0.5);
}

template<typename T>
FORCE_INLINE T orient(const edge_t<T> &edge,
                      const point_t<T> &other_point,
                      const point_t<T> &point)
{
  return -halfspace(edge, point) + (is_top_left(edge, other_point) ? 0 : -1);
}

template<typename T>
FORCE_INLINE bool triangle_hit(const point_t<T> &p0,
                               const point_t<T> &p1,
                               const point_t<T> &p2,
                               const point_t<T> &test)
{
  return orient({p1, p2}, p0, test) >= 0 && orient({p2, p0}, p1, test) >= 0 &&
         orient({p0, p1}, p2, test) >= 0;
}

template<typename POS, typename COL>
struct bary_t;

template<typename P, typename C, typename T>
FORCE_INLINE bool triangle_hit(const bary_t<P, C> &bary,
                               const T &x,
                               const T &y)
{
  return triangle_hit(point_t<P>{bary.a.x, bary.a.y},
                      point_t<P>{bary.b.x, bary.b.y},
                      point_t<P>{bary.c.x, bary.c.y},
                      point_t<P>{(P)x, (P)y});
}

#endif