#ifndef _Z_EN_BOOM_H_
#define _Z_EN_BOOM_H_

#include "global.hpp"

struct EnBoom;
struct SkeletonAnimationModel;

typedef void (*EnBoomActionFunc)(struct EnBoom*, struct PlayState*);

typedef struct EnBoom {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkeletonAnimationModel* model;
    /* 0x01A8 */ u8 collider[0x80]; // ColliderQuad
    /* 0x0228 */ Actor* moveTo; // actor boomerang moves toward
    /* 0x022C */ Actor* grabbed; // actor grabbed by the boomerang
    /* 0x0230 */ s32 effectIndex;
    /* 0x0234 */ char unk_234[0x34];
    /* 0x0268 */ EnBoomActionFunc actionFunc;
    /* 0x026C */ u8 unk_26C;
    /* 0x026D */ u8 activeTimer; // increments once every update
    /* 0x026E */ u8 unk_26E; // kill threshold for FUN_00128f0c
    /* 0x026F */ u8 unk_26F; // kill counter for FUN_00128f0c
    /* 0x0270 */ Vec3f unk_270;
    /* 0x027C */ s16 unk_27C;
    /* 0x027E */ char unk_27E[0x2];
} EnBoom; // size = 0x0280

extern ActorInit En_Boom_InitVars;

#endif
