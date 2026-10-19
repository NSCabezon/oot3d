#ifndef _Z_EN_GM_H_
#define _Z_EN_GM_H_

#include "global.hpp"

struct EnGm;

typedef void (*EnGmActionFunc)(struct EnGm*, PlayState*);

typedef struct EnGm {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x7B8];
    /* 0x09E0 */ FaceAnimations faceAnims;
    /* 0x0BAC */ EnGmActionFunc actionFunc;
    /* 0x0BB0 */ char unk_BB0[0x4];
    /* 0x0BB4 */ ColliderCylinder collider;
    /* 0x0C0C */ char unk_C0C[0x14];
} EnGm; // size = 0xC20

extern ActorInit En_Gm_InitVars;

#endif
