#ifndef _Z_EN_AM_H_
#define _Z_EN_AM_H_

#include "global.hpp"

struct EnAm;

typedef void (*EnAmActionFunc)(struct EnAm*, struct PlayState*);

typedef struct EnAm {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ SkelAnime skelAnime;
    /* 0x0240 */ Vec3s jointTable[14];
    /* 0x0294 */ Vec3s morphTable[14];
    /* 0x02E8 */ char unk_2E8[0x160];
    /* 0x0448 */ s32 behavior;
    /* 0x044C */ EnAmActionFunc actionFunc;
    /* 0x0450 */ s16 unk_450; // slide angle (N64 unk_258 high part), counts down in steps of 0x555
    /* 0x0452 */ s16 unk_258;
    /* 0x0454 */ s16 cooldownTimer;
    /* 0x0456 */ s16 attackTimer; // start going back home after reaching 0
    /* 0x0458 */ s16 iceTimer;
    /* 0x045A */ s16 deathTimer; // explode after reaching 0
    /* 0x045C */ s16 panicSpinRot; // used when panicking before death
    /* 0x045E */ s16 unk_264;
    /* 0x0460 */ u16 unk_460; // effect toggle (0/1)
    /* 0x0462 */ u16 unk_462; // effect counter (0/1/2)
    /* 0x0464 */ u8 textureBlend; // 0 = statue textures; 255 = enemy textures
    /* 0x0465 */ u8 damageReaction;
    /* 0x0468 */ Vec3f homePos; // home position used by CanMove (ROM reads .y at 0x46C)
    /* 0x0474 */ ColliderCylinder hurtCollider;
    /* 0x04CC */ ColliderCylinder blockCollider;
    /* 0x0524 */ char unk_524[0x84]; // hitCollider (ColliderQuad) and tail, not yet typed
} EnAm; // size = 0x05A8

extern ActorInit En_Am_InitVars;

#endif
