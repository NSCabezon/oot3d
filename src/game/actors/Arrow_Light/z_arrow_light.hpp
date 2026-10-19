#ifndef _Z_ARROW_LIGHT_H_
#define _Z_ARROW_LIGHT_H_

#include "global.hpp"

struct ArrowLight;

extern "C++" {
struct ArrowLightModel {
    virtual void unk_vf0();
    virtual void Destroy();
};
}

typedef void (*ArrowLightActionFunc)(struct ArrowLight* self, PlayState* play);

typedef struct ArrowLight {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ s16 radius;
    /* 0x01A6 */ u16 timer;
    /* 0x01A8 */ u8 alpha;
    /* 0x01A9 */ char unk_1A9[0x3];
    /* 0x01AC */ Vec3f unkPos;
    /* 0x01B8 */ f32 unk_160;
    /* 0x01BC */ f32 unk_164;
    /* 0x01C0 */ ArrowLightActionFunc actionFunc;
    /* 0x01C4 */ ArrowLightModel* model; // virtual Destroy = slot 1
    /* 0x01C8 */ SkeletonAnimationModel_unk_0C* anim;
} ArrowLight; // size = 0x1CC

extern ActorInit Arrow_Light_InitVars;

#endif
