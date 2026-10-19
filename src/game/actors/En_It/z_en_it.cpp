extern "C" {
#include "z_en_it.hpp"

#define FLAGS 0x00000000

void EnIt_Init(Actor* actor, GameState* state);
void EnIt_Destroy(Actor* actor, GameState* state);
void EnIt_Update(Actor* actor, GameState* state);

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_NO_PUSH,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 40, 10, 0, { 0, 0, 0 } },
};

// 3DS CollisionCheckInfoInit2 is 0xC bytes: trailing s16 at 0xA is -1 here (and in many other actors' ROM data)
// (repo header only has the N64 0xA-byte layout).
typedef struct {
    /* 0x00 */ CollisionCheckInfoInit2 base;
    /* 0x0A */ s16 unk_0A;
} EnItColChkInfoInit2; // size = 0xC

static EnItColChkInfoInit2 sColChkInfoInit = { { 0, 0, 0, 0, MASS_IMMOVABLE }, -1 };

ActorInit En_It_InitVars = {
    ACTOR_EN_IT,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnIt),
    (ActorFunc)EnIt_Init,
    (ActorFunc)EnIt_Destroy,
    (ActorFunc)EnIt_Update,
    NULL,
};

void EnIt_Init(Actor* actor, GameState* state) {
    EnIt* self = (EnIt*)actor;
    PlayState* play = (PlayState*)state;

    self->actor.params = 0x0D05;
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, &sCylinderInit);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, NULL, &sColChkInfoInit.base);
}

void EnIt_Destroy(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnIt* self = (EnIt*)actor;

    Collider_DestroyCylinder(play, &self->collider);
}

void EnIt_Update(Actor* actor, GameState* state) {
    EnIt* self = (EnIt*)actor;
    PlayState* play = (PlayState*)state;

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
}
}
