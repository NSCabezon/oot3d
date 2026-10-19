#ifndef _Z_EN_OSSAN_H_
#define _Z_EN_OSSAN_H_

#include "global.hpp"

struct EnOssan;

typedef struct EnOssan {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x84];
    /* 0x0228 */ void (*actionFunc)(struct EnOssan*, GameState*);
    /* 0x022C */ char unk_22C[0x5C];
    /* 0x0288 */ s16 timer;
    /* 0x028A */ char unk_28A[0x11E];
    /* 0x03A8 */ struct SkeletonAnimationModel* skelModels[4];
    /* 0x03B8 */ char unk_3B8[0x1CC];
} EnOssan; // size = 0x584

extern ActorInit En_Ossan_InitVars;

#endif
