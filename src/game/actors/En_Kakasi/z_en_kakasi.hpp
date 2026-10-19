#ifndef _Z_EN_KAKASI_H_
#define _Z_EN_KAKASI_H_

#include "global.hpp"

struct EnKakasi;

typedef void (*EnKakasiActionFunc)(struct EnKakasi*, PlayState*);

typedef struct EnKakasi {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnKakasiActionFunc actionFunc;
    /* 0x01A8 */ SkelAnime skelanime;
    /* 0x022C */ u8 unk_22C;
    /* 0x022E */ s16 unk_22E;
    /* 0x0230 */ s16 unk_230;
    /* 0x0232 */ s16 unk_232;
    /* 0x0234 */ s16 unk_234[4];
    /* 0x023C */ s16 unk_23C;
    /* 0x023E */ s16 unk_23E;
    /* 0x0240 */ s16 unk_240;
    /* 0x0242 */ char unk_242[0x2];
    /* 0x0244 */ s16 unk_244;
    /* 0x0246 */ Vec3s rot;
    /* 0x024C */ f32 height;
    /* 0x0250 */ f32 unk_250;
    /* 0x0254 */ ColliderCylinder collider;
    /* 0x02AC */ s16 subCamId;
} EnKakasi; // size = 0x2B0

extern ActorInit En_Kakasi_InitVars;

#endif
