#ifndef _Z_EN_BIGOKUTA_H_
#define _Z_EN_BIGOKUTA_H_

#include "global.hpp"

struct EnBigokuta;

typedef void (*EnBigokutaActionFunc)(struct EnBigokuta*, PlayState*);

// JntSph collider types are not in the repo headers yet (3DS layout, local for now).
typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ void* elements;
} EnBigokutaColliderJntSph; // size = 0x20

typedef struct EnBigokuta {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x820];
    /* 0x0A48 */ EnBigokutaActionFunc actionFunc;
    /* 0x0A4C */ s8 unk_194;
    /* 0x0A4D */ u8 unk_195;
    /* 0x0A4E */ s16 unk_196;
    /* 0x0A50 */ s16 unk_198;
    /* 0x0A52 */ s16 unk_19A;
    /* 0x0A54 */ EnBigokutaColliderJntSph collider;
    /* 0x0A74 */ char unk_A74[0x50]; // ColliderJntSphElement[1]
    /* 0x0AC4 */ ColliderCylinder colliderCylinders[2];
} EnBigokuta; // size = 0xB74

extern ActorInit En_Bigokuta_InitVars;

static inline void func_809BCE3C(EnBigokuta* self) {
    self->actor.world.rot.y = self->actor.shape.rot.y + self->unk_194 * -0x4000;
    self->actor.world.pos.x = Math_SinS(self->actor.world.rot.y) * 263.0f + self->actor.home.pos.x;
    self->actor.world.pos.z = Math_CosS(self->actor.world.rot.y) * 263.0f + self->actor.home.pos.z;
}

#endif
