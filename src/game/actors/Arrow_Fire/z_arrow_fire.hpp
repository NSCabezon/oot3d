#ifndef _Z_ARROW_FIRE_H_
#define _Z_ARROW_FIRE_H_

#include "global.hpp"

struct ArrowFire;

extern "C++" {
struct ArrowFireModel {
    virtual void unk_vf0();
    virtual void Destroy();
};
}

typedef void (*ArrowFireActionFunc)(struct ArrowFire* self, PlayState* play);

typedef struct ArrowFire {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ Vec3f unkPos;
    /* 0x01B0 */ f32 unk_158;
    /* 0x01B4 */ f32 unk_15C;
    /* 0x01B8 */ ArrowFireActionFunc actionFunc;
    /* 0x01BC */ s16 radius;
    /* 0x01BE */ u16 timer;
    /* 0x01C0 */ u8 alpha;
    /* 0x01C4 */ ArrowFireModel* model; // virtual Destroy = slot 1
} ArrowFire; // size = 0x1C8

extern ActorInit Arrow_Fire_InitVars;

#endif
