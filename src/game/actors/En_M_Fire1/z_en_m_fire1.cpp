extern "C" {
#include "z_en_m_fire1.hpp"

#define FLAGS 0x00000000

void EnMFire1_Init(Actor* actor, GameState* state);
void EnMFire1_Destroy(Actor* actor, GameState* state);
void EnMFire1_Update(Actor* actor, GameState* state);

ActorInit En_M_Fire1_InitVars = {
    ACTOR_EN_M_FIRE1,
    ACTORCAT_MISC,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnMFire1),
    (ActorFunc)EnMFire1_Init,
    (ActorFunc)EnMFire1_Destroy,
    (ActorFunc)EnMFire1_Update,
    NULL,
};

static ColliderCylinderInit sCylinderInit = {
    { 0x0A, 0x09, 0x00, 0x00, 0x08, 0x01 },
    { 2, { 0x00000001, 0, 0 }, { 0xFFCFFFFF, 0, 0 }, 25, 0, 0 },
    { 200.0f, 200.0f, 0.0f, { 0.0f, 0.0f, 0.0f } },
};

void EnMFire1_Init(Actor* actor, GameState* state) {
    EnMFire1* self = (EnMFire1*)actor;
    PlayState* play = (PlayState*)state;

    if (self->actor.params < 0) {
        Actor_ChangeType(play, &play->actorCtx, &self->actor, ACTORCAT_ITEMACTION);
    }

    FUN_00372f38(&self->actor, play, NULL);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, &sCylinderInit);
}

void EnMFire1_Destroy(Actor* actor, GameState* state) {
    EnMFire1* self = (EnMFire1*)actor;
    PlayState* play = (PlayState*)state;

    FUN_003508b8(&self->actor, NULL);
    Collider_DestroyCylinder(play, &self->collider);
}

// Update stays asm: functions.tsv merges ~0x1A8 bytes of unrelated code after it (cfg split needed).
// Draft (not matching as-is, see STATUS.md):
// Draft: first 0x58 bytes match ROM, but functions.tsv span (0x2B1898, 504 B) also covers an
// orphan 0x1A0-byte blob (0x2B18F0..0x2B1A90, no references in the dump). Dropping it from the link
// makes L6221E overlaps, so Update stays asm until the cfg is split.
void EnMFire1_Update(Actor* actor, GameState* state) {
    EnMFire1* self = (EnMFire1*)actor;
    PlayState* play = (PlayState*)state;

    if (Math_StepToF(&self->timer, 1.0f, 0.2f)) {
        Actor_Kill(&self->actor);
    } else {
        Collider_UpdateCylinder(&self->actor, &self->collider);
        CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
    }
}

GLOBAL_ASM("../split/asm/FUN_002b18f0.s")
}
