extern "C" {
#include "z_en_ssh.hpp"

#define FLAGS 0x00000035

void EnSsh_Init(Actor* actor, GameState* state);
void EnSsh_Destroy(Actor* actor, GameState* state);
void EnSsh_Update(Actor* actor, GameState* state);
void EnSsh_Draw(Actor* actor, GameState* state);
void EnSsh_Idle(EnSsh* self, PlayState* play);
void EnSsh_Drop(EnSsh* self, PlayState* play);
f32 EnSsh_SetAnimation(EnSsh* self, s32 animIndex);
s32 EnSsh_IsCloseToLink(EnSsh* self, PlayState* play);
s32 FUN_00369a48(Actor* actor, PlayState* play);
void Collider_DestroyJntSph(PlayState* play, void* collider);
void Collider_UpdateSpheres(s32 index, void* collider, MTX34* mtx);
void Effect_Delete(PlayState* play, s32 blureIdx);

static void EnSsh_Bob(EnSsh* self, PlayState* play) {
    f32 bobVel = 0.5f;

    if ((play->state.frames & 8) != 0) {
        bobVel *= -1.0f;
    }
    Math_SmoothStepToF(&self->actor.velocity.y, bobVel, 0.4f, 1000.0f, 0.0f);
}

GLOBAL_ASM("data/z_En_Ssh.data.s")

ActorInit En_Ssh_InitVars = {
    ACTOR_EN_SSH,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_SSH,
    sizeof(EnSsh),
    (ActorFunc)EnSsh_Init,
    (ActorFunc)EnSsh_Destroy,
    (ActorFunc)EnSsh_Update,
    (ActorFunc)EnSsh_Draw,
};

GLOBAL_ASM("asm/EnSsh_Init.s")

void EnSsh_Destroy(Actor* actor, GameState* state) {
    EnSsh* self = (EnSsh*)actor;
    s32 i;

    Effect_Delete((PlayState*)state, self->blureIdx);
    for (i = 0; i < 6; i++) {
        Collider_DestroyCylinder((PlayState*)state, &self->colliderCylinders[i]);
    }
    Collider_DestroyJntSph((PlayState*)state, self->colliderJntSph);
    FUN_003508b8(actor, 0);
    SkelAnime_Free2(&self->skelAnime);
    ((FaceAnimations*)self->faceAnimations)->Destroy();
}

GLOBAL_ASM("asm/EnSsh_Update.s")

GLOBAL_ASM("asm/EnSsh_Draw.s")

GLOBAL_ASM("asm/EnSsh_Drop.s")

GLOBAL_ASM("asm/EnSsh_Idle.s")

void EnSsh_Land(EnSsh* self, PlayState* play) {
    if ((self->unkTimer != 0) && (--self->unkTimer == 0)) {
        EnSsh_SetAnimation(self, 2);
    }
    if ((self->animTimer != 0) && (--self->animTimer == 0)) {
        EnSsh_SetAnimation(self, 2);
    }
    if ((self->actor.floorHeight + self->floorHeightOffset) <= self->actor.world.pos.y) {
        self->actionFunc = EnSsh_Idle;
        self->sfxTimer = 0;
    } else {
        Math_SmoothStepToF(&self->actor.velocity.y, 2.0f, 0.6f, 1000.0f, 0.0f);
    }
}

s32 EnSsh_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    return false;
}

void EnSsh_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnSsh* self = (EnSsh*)thisx;
    Collider_UpdateSpheres(limbIndex, self->colliderJntSph, mtx);
}

GLOBAL_ASM("asm/EnSsh_Return.s")

GLOBAL_ASM("asm/EnSsh_SetColliderScale.s")

GLOBAL_ASM("asm/EnSsh_Start.s")

void EnSsh_Talk(EnSsh* self, PlayState* play) {
    EnSsh_Bob(self, play);
    if (FUN_00369a48(&self->actor, play)) {
        self->actionFunc = EnSsh_Idle;
        self->sfxTimer = 0;
    }
}

void EnSsh_Wait(EnSsh* self, PlayState* play) {
    if (EnSsh_IsCloseToLink(self, play)) {
        if (self->unkTimer == 0) {
            self->animTimer = (s16)EnSsh_SetAnimation(self, 4);
        }
        self->actor.velocity.y = -10.0f;
        self->actionFunc = EnSsh_Drop;
        self->sfxTimer = 0;
    } else {
        EnSsh_Bob(self, play);
    }
}

}
