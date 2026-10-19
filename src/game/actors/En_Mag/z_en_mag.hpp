#ifndef _Z_EN_MAG_H_
#define _Z_EN_MAG_H_

#include "global.hpp"

struct EnMag;

typedef struct EnMag {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ struct EnMagObj* unk_1A4; // virtual objects, Destroy (slot 1) in EnMag_Destroy
    /* 0x01A8 */ struct EnMagObj* unk_1A8;
    /* 0x01AC */ struct EnMagObj* unk_1AC;
    /* 0x01B0 */ char unk_1B0[0x4];
    /* 0x01B4 */ s32 unk_1B4;
    /* 0x01B8 */ s32 unk_1B8;
    /* 0x01BC */ s16 unk_1BC;
    /* 0x01BE */ s16 unk_1BE;
    /* 0x01C0 */ s16 unk_1C0;
    /* 0x01C2 */ s16 unk_1C2;
    /* 0x01C4 */ s16 unk_1C4;
    /* 0x01C6 */ s16 unk_1C6;
    /* 0x01C8 */ s16 unk_1C8;
    /* 0x01CA */ s16 unk_1CA;
    /* 0x01CC */ s16 unk_1CC;
    /* 0x01CE */ u8 unk_1CE;
    /* 0x01CF */ char unk_1CF[1];
    /* 0x01D0 */ f32 unk_1D0;
    /* 0x01D4 */ f32 unk_1D4;
    /* 0x01D8 */ f32 unk_1D8;
    /* 0x01DC */ f32 unk_1DC;
} EnMag; // size = 0x1E0

extern ActorInit En_Mag_InitVars;

#endif
