#ifndef _Z_EN_IK_H_
#define _Z_EN_IK_H_

#include "global.hpp"

struct EnIk;

typedef struct EnIk {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0xC68]; // SkelAnime at 0x01A4
    /* 0x0E0C */ u8 unk_E0C;
    /* 0x0E0D */ u8 unk_E0D;
    /* 0x0E0E */ char unk_E0E;
    /* 0x0E0F */ u8 unk_E0F;
    /* 0x0E10 */ char unk_E10[0x2];
    /* 0x0E12 */ u8 unk_E12;
    /* 0x0E13 */ char unk_E13;
    /* 0x0E14 */ u8 unk_E14;
    /* 0x0E15 */ u8 unk_E15;
    /* 0x0E16 */ char unk_E16[0x6];
    /* 0x0E1C */ void* actionFunc;
    /* 0x0E20 */ char unk_E20[0x14];
    /* 0x0E34 */ ColliderCylinder bodyCollider;
    /* 0x0E8C */ char axeCollider[0x80];
    /* 0x0F0C */ char shieldCollider[0xDC];
    /* 0x0FE8 */ u32 updateIndex;
    /* 0x0FEC */ u32 drawIndex;
    /* 0x0FF0 */ char unk_FF0[0xC];
} EnIk; // size = 0xFFC

extern ActorInit En_Ik_InitVars;

#endif
