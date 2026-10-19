extern "C" {
#include "z_bg_hidan_syoku.hpp"

#define FLAGS 0x80400010

void BgHidanSyoku_Init(Actor* actor, GameState* state);
void BgHidanSyoku_Destroy(Actor* actor, GameState* state);
void BgHidanSyoku_Update(Actor* actor, GameState* state);
void BgHidanSyoku_Draw(Actor* actor, GameState* state);

void FUN_003e7e54(BgHidanSyoku* self, PlayState* play); // N64: func_8088F4B8
void FUN_003ba0e4(BgHidanSyoku* self, PlayState* play); // N64: func_8088F514
void FUN_003e1fbc(BgHidanSyoku* self, PlayState* play); // N64: func_8088F62C
void FUN_003d9a14(BgHidanSyoku* self, PlayState* play); // N64: func_8088F5A0
__attribute__((noinline)) void FUN_00180574(BgHidanSyoku* self); // N64: func_8088F47C
s32 FUN_0036adf4(DynaPolyActor* dyna);                  // DynaPolyActor_IsPlayerOnTop
void Camera_ChangeSetting(Camera* camera, s32 setting);

extern InitChainEntry BgHidanSyoku_sInitChain[];
f32 cosf(f32 x);

GLOBAL_ASM("data/z_Bg_Hidan_Syoku.data.s")
GLOBAL_ASM("../src/game/actors/Bg_Hidan_Syoku/BgHidanSyoku_data.s")

ActorInit Bg_Hidan_Syoku_InitVars = {
    ACTOR_BG_HIDAN_SYOKU,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HIDAN_OBJECTS,
    sizeof(BgHidanSyoku),
    (ActorFunc)BgHidanSyoku_Init,
    (ActorFunc)BgHidanSyoku_Destroy,
    (ActorFunc)BgHidanSyoku_Update,
    (ActorFunc)BgHidanSyoku_Draw,
};

void BgHidanSyoku_Init(Actor* actor, GameState* state) {
    BgHidanSyoku* self = (BgHidanSyoku*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(actor, BgHidanSyoku_sInitChain);
    DynaPolyActor_Init(&self->dyna, 1);
    FUN_00372f38(actor, play, &self->skelAnimModel, 0xD, 0);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 0xC), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    self->dyna.actor.home.pos.y += 540.0f;
    self->actionFunc = FUN_003e7e54;
}

void BgHidanSyoku_Destroy(Actor* actor, GameState* state) {
    BgHidanSyoku* self = (BgHidanSyoku*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgHidanSyoku_Update(Actor* actor, GameState* state) {
    BgHidanSyoku* self = (BgHidanSyoku*)actor;
    PlayState* play = (PlayState*)state;

    self->actionFunc(self, play);
    if (FUN_0036adf4(&self->dyna)) {
        if (self->unk_168 == 0) {
            self->unk_168 = 3;
        }
        Camera_ChangeSetting(play->cameraPtrs[0], 0x30);
    } else if (!FUN_0036adf4(&self->dyna)) {
        if (self->unk_168 != 0) {
            Camera_ChangeSetting(play->cameraPtrs[0], 3);
        }
        self->unk_168 = 0;
    }
}

void BgHidanSyoku_Draw(Actor* actor, GameState* state) {
    BgHidanSyoku* self = (BgHidanSyoku*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->dyna.actor.modelMtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}

void FUN_003e7e54(BgHidanSyoku* self, PlayState* play) { // N64: func_8088F4B8
    if (Flags_GetClear(play, self->dyna.actor.room) && FUN_0036adf4(&self->dyna)) {
        self->timer = 210;
        self->actionFunc = FUN_003ba0e4;
    }
}

void FUN_003ba0e4(BgHidanSyoku* self, PlayState* play) { // N64: func_8088F514
    if (self->timer != 0) {
        self->timer--;
    }
    self->dyna.actor.world.pos.y = (cosf(self->timer * (M_PI / 210)) * 540.0f) + self->dyna.actor.home.pos.y;
    if (self->timer == 0) {
        self->timer = 90;
        Audio_PlayActorSound2(&self->dyna.actor, 0x0100018C);
        self->actionFunc = FUN_003e1fbc;
    } else {
        FUN_00373264(&self->dyna.actor, 0x01000210);
    }
}

void FUN_003e1fbc(BgHidanSyoku* self, PlayState* play) { // N64: func_8088F62C
    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer == 0) {
        self->timer = 210;
        if (self->dyna.actor.world.pos.y - self->dyna.actor.home.pos.y < 0.0f) {
            self->actionFunc = FUN_003ba0e4;
        } else {
            self->actionFunc = FUN_003d9a14;
        }
    }
}

void FUN_003d9a14(BgHidanSyoku* self, PlayState* play) { // N64: func_8088F5A0
    if (self->timer != 0) {
        self->timer--;
    }
    self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y - (cosf(self->timer * (M_PI / 210)) * 540.0f);
    if (self->timer == 0) {
        FUN_00180574(self);
    } else {
        FUN_00373264(&self->dyna.actor, 0x01000210);
    }
}

void FUN_00180574(BgHidanSyoku* self) { // N64: func_8088F47C
    self->timer = 90;
    Audio_PlayActorSound2(&self->dyna.actor, 0x0100018C);
    self->actionFunc = FUN_003e1fbc;
}
}
