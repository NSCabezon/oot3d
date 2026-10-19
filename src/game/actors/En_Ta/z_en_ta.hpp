#ifndef _Z_EN_TA_H_
#define _Z_EN_TA_H_

#include "global.hpp"

struct EnTa;

typedef void (*EnTaFunc)(void);

typedef struct EnTa {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ Vec3s jointTable[19];
    /* 0x029A */ char unk_29A[0x604 - 0x29A];
    /* 0x0604 */ Vec3s morphTable[19];
    /* 0x0676 */ char unk_676[0x9E0 - 0x676];
    /* 0x09E0 */ char unk_9E0[0xBAC - 0x9E0];
    /* 0x0BAC */ EnTaFunc actionFunc;
    /* 0x0BB0 */ EnTaFunc animFunc;
    /* 0x0BB4 */ ColliderCylinder collider;
    /* 0x0C0C */ EnTaFunc blinkFunc;
    /* 0x0C10 */ s16 unk_C10;
    /* 0x0C12 */ s16 blinkTimer;
    /* 0x0C14 */ Actor* superCuccos[3];
    /* 0x0C20 */ s16 superCuccoTimers[3];
    /* 0x0C26 */ char unk_C26[2];
    /* 0x0C28 */ s16 unk_C28;
    /* 0x0C2A */ s16 unk_C2A;
    /* 0x0C2C */ char unk_C2C[4];
    /* 0x0C30 */ Vec3s headRot;
    /* 0x0C36 */ Vec3s torsoRot;
    /* 0x0C3C */ u16 flags_C3C;
    /* 0x0C3E */ s16 unk_C3E;
    /* 0x0C40 */ void* unk_C40;
} EnTa; // size = 0xC44

extern ActorInit En_Ta_InitVars;

#endif
