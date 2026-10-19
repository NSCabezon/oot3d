extern "C" {
#include "z_bg_mizu_bwall.hpp"

#define FLAGS 0x80400010

void BgMizuBwall_Init(Actor* actor, GameState* state);
void BgMizuBwall_Destroy(Actor* actor, GameState* state);
void BgMizuBwall_Update(Actor* actor, GameState* state);
void BgMizuBwall_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Bg_Mizu_Bwall.data.s")

ActorInit Bg_Mizu_Bwall_InitVars = {
    ACTOR_BG_MIZU_BWALL,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_MIZU_OBJECTS,
    sizeof(BgMizuBwall),
    (ActorFunc)BgMizuBwall_Init,
    (ActorFunc)BgMizuBwall_Destroy,
    (ActorFunc)BgMizuBwall_Update,
    (ActorFunc)BgMizuBwall_Draw,
};

GLOBAL_ASM("asm/BgMizuBwall_Init.s")

s32 Collider_DestroyTris(PlayState* play, ColliderTris* collider);

void FUN_001980c4(BgMizuBwall* self, PlayState* play) {
}

void FUN_001741a8(BgMizuBwall* self, PlayState* play) {
    if (self->breakTimer > 0) {
        self->breakTimer--;
    } else {
        self->actionFunc = FUN_001980c4;
    }
}

void BgMizuBwall_Destroy(Actor* actor, GameState* state) {
    BgMizuBwall* self = (BgMizuBwall*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyTris(play, &self->collider);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgMizuBwall_Update(Actor* actor, GameState* state) {
    BgMizuBwall* self = (BgMizuBwall*)actor;
    PlayState* play = (PlayState*)state;

    self->actionFunc(self, play);
}

void BgMizuBwall_Draw(Actor* actor, GameState* state) {
    BgMizuBwall* self = (BgMizuBwall*)actor;
    MTX34 mtx;

    MTX34CopyAsm(&mtx, &actor->modelMtx);
    f32 speed = 1.0f;
    if (PauseContext_GetState() != 0) {
        speed = 0.0f;
    }
    if (self->skelAnimModel != NULL) {
        if (self->unk_314 == 1) {
            if (self->scrollAlpha1 == 255) {
                FUN_003695cc(self->skelAnimModel, 1, 4, 0, 0.0f, 0.0f, 0.0f, 0.0f);
            }
        } else if (self->unk_314 == 2 && self->scrollAlpha1 == 255) {
            FUN_003695cc(self->skelAnimModel, 1, 4, 0, 0.0f, 0.0f, 0.0f, 0.0f);
        }
        self->skelAnimModel->unk_0C->SetAnimSpeed(speed);
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&mtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}
}
