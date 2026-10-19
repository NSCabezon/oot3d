#ifndef _Z_BG_YDAN_HASI_H_
#define _Z_BG_YDAN_HASI_H_

#include "global.hpp"

struct BgYdanHasi;

typedef void (*BgYdanHasiActionFunc)(struct BgYdanHasi*, PlayState*);

// Real WaterBox stride is 0x10 (ySurface at +0x02); include/z3Dbgcheck.hpp's WaterBox is only 4 bytes.
typedef struct BgYdanHasiWaterBox {
    /* 0x00 */ char unk_00[0x2];
    /* 0x02 */ s16 ySurface;
    /* 0x04 */ char unk_04[0xC];
} BgYdanHasiWaterBox; // size = 0x10

typedef struct BgYdanHasi {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ BgYdanHasiActionFunc actionFunc;
    /* 0x01C0 */ SkeletonAnimationModel* model;
    /* 0x01C4 */ s16 timer;
    /* 0x01C6 */ u16 unk_1C6;
    /* 0x01C8 */ u8 type;
    /* 0x01C9 */ char unk_1C9[0x3];
} BgYdanHasi; // size = 0x1CC

extern ActorInit Bg_Ydan_Hasi_InitVars;

#endif
