#ifndef _Z_MAGIC_FIRE_H_
#define _Z_MAGIC_FIRE_H_

#include "global.hpp"

struct MagicFire;

extern "C++" {
struct MagicFireModel {
    virtual void unk_vf0();
    virtual void Destroy();
};
}

typedef struct MagicFire {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ColliderCylinder collider;
    /* 0x01FC */ f32 alphaMultiplier;
    /* 0x0200 */ f32 screenTintIntensity;
    /* 0x0204 */ f32 scalingSpeed;
    /* 0x0208 */ s16 action;
    /* 0x020A */ s16 screenTintBehaviour;
    /* 0x020C */ s16 actionTimer;
    /* 0x020E */ s16 screenTintBehaviourTimer;
    /* 0x0210 */ MagicFireModel* model; // virtual Destroy = slot 1; _ZTI14MagicFireModel is an EQU in shared_syms.s
    /* 0x0214 */ SkeletonAnimationModel_unk_0C* anim;
} MagicFire; // size = 0x218

extern ActorInit Magic_Fire_InitVars;

#endif
