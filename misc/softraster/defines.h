#ifndef SOFTRASTER_DEFINES_H
#define SOFTRASTER_DEFINES_H

#ifdef ARDUINO
#   include "Arduino.h"
#   define INLINE_DEC(F) inline F __attribute__((always_inline))
#else
#   define INLINE_DEC(F) inline F
#endif
#define INLINE_DEF(F) inline F
#define INLINE(F) INLINE_DEC(F); INLINE_DEF(F)

#endif // SOFTRASTER_DEFINES_H