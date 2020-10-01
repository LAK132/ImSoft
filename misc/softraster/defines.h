#ifndef SOFTRASTER_DEFINES_H
#define SOFTRASTER_DEFINES_H

#ifdef ARDUINO
#  include "Arduino.h"
#endif

#if defined(ARDUINO) || defined(__clang__) || defined(__GNUC__)
#  define FORCE_INLINE inline __attribute__((always_inline))
#elif defined(_MSC_VER)
#  define FORCE_INLINE inline __forceinline
#else
#  define FORCE_INLINE inline
#endif

#if defined(__cplusplus) && __cplusplus >= 201103L
#  define INLINE_CONSTEXPR constexpr inline
#else
#  define INLINE_CONSTEXPR inline
#endif

#endif