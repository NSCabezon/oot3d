#ifndef _Z_EN_KAKASI3_H_
#define _Z_EN_KAKASI3_H_

#include "global.hpp"

struct EnKakasi3;

typedef void (*EnKakasi3ActionFunc)(struct EnKakasi3*, PlayState*);

typedef struct EnKakasi3 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnKakasi3ActionFunc actionFunc;
    /* 0x01A8 */ SkelAnime skelAnime;
    /* 0x022C */ u8 unk_22C;
    /* 0x022D */ u8 unk_22D;
    /* 0x022E */ s16 dialogState;
    /* 0x0230 */ s16 unk_230;
    /* 0x0232 */ s16 unk_232;
    /* 0x0234 */ s16 unk_234[4];
    /* 0x023C */ s16 unk_23C;
    /* 0x023E */ s16 unk_23E;
    /* 0x0240 */ s16 unk_240;
    /* 0x0242 */ s16 unk_242;
    /* 0x0244 */ char unk_244[0x2];
    /* 0x0246 */ s16 unk_246;
    /* 0x0248 */ Vec3s rot;
    /* 0x0250 */ f32 unk_250;
    /* 0x0254 */ ColliderCylinder collider;
    /* 0x02AC */ s16 subCamId;
} EnKakasi3; // size = 0x2B0

extern ActorInit En_Kakasi3_InitVars;

#endif
