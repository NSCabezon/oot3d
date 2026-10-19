extern "C" {
#include "z_bg_menkuri_nisekabe.hpp"

#define FLAGS 0x80400030

void BgMenkuriNisekabe_Init(Actor* actor, GameState* state);
void BgMenkuriNisekabe_Destroy(Actor* actor, GameState* state);
void BgMenkuriNisekabe_Update(Actor* actor, GameState* state);
void BgMenkuriNisekabe_Draw(Actor* actor, GameState* state);

ActorInit Bg_Menkuri_Nisekabe_InitVars = {
    ACTOR_BG_MENKURI_NISEKABE,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_MENKURI_OBJECTS,
    sizeof(BgMenkuriNisekabe),
    (ActorFunc)BgMenkuriNisekabe_Init,
    (ActorFunc)BgMenkuriNisekabe_Destroy,
    (ActorFunc)BgMenkuriNisekabe_Update,
    (ActorFunc)BgMenkuriNisekabe_Draw,
};

static s32 sModelIndices[] = { 1, 2 };

void BgMenkuriNisekabe_Init(Actor* actor, GameState* state) {
    BgMenkuriNisekabe* self = (BgMenkuriNisekabe*)actor;
    PlayState* play = (PlayState*)state;

    Actor_SetScale(&self->actor, 0.1f);
    s32 idx = sModelIndices[actor->params & 0xFF];
    FUN_00372f38(actor, play, &self->skelAnimModel, idx, 0);
}

void BgMenkuriNisekabe_Destroy(Actor* actor, GameState* state) {
    BgMenkuriNisekabe* self = (BgMenkuriNisekabe*)actor;

    FUN_00350f34(&self->actor, &self->skelAnimModel, 0);
}

void BgMenkuriNisekabe_Update(Actor* actor, GameState* state) {
    BgMenkuriNisekabe* self = (BgMenkuriNisekabe*)actor;
    PlayState* play = (PlayState*)state;

    if (play->actorCtx.unk_03 != 0) {
        self->actor.flags |= 0x80;
    } else {
        self->actor.flags &= ~0x80;
    }
}

void BgMenkuriNisekabe_Draw(Actor* actor, GameState* state) {
    BgMenkuriNisekabe* self = (BgMenkuriNisekabe*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->actor.modelMtx);
    if ((self->actor.flags & 0x80) == 0x80) {
        if (self->skelAnimModel != NULL) {
            self->skelAnimModel->SetUnkAC(1);
            self->skelAnimModel->FUN_003721e0(&modelMtx);
            self->skelAnimModel->FUN_00372170(0);
        }
    } else if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}
}
