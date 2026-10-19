#ifndef _Z_BG_PO_SYOKUDAI_H_
#define _Z_BG_PO_SYOKUDAI_H_

#include "global.hpp"

struct BgPoSyokudai;

typedef struct BgPoSyokudai {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ u8 flameColor;
    /* 0x01A5 */ char unk_1A5[0x1];
    /* 0x01A6 */ u16 flameTextureScroll;
    /* 0x01A8 */ void* lightNode;
    /* 0x01AC */ char unk_1AC[0x18];
    /* 0x01C4 */ ColliderCylinder collider;
    /* 0x021C */ void* skelAnim;
    /* 0x0220 */ char unk_220[0xC];
} BgPoSyokudai; // size = 0x22C

extern ActorInit Bg_Po_Syokudai_InitVars;

#endif
