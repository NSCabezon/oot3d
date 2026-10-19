extern "C" {
#include "z_bg_ydan_maruta.hpp"

#define FLAGS 0x80000010

void BgYdanMaruta_Init(Actor* actor, GameState* state);
void BgYdanMaruta_Destroy(Actor* actor, GameState* state);

void BgYdanMaruta_Update(Actor* actor, GameState* state);
void BgYdanMaruta_Draw(Actor* actor, GameState* state);
void FUN_00128194(BgYdanMaruta* self, PlayState* play); // func_808BEFF4 (spike roll)
void FUN_001ca970(BgYdanMaruta* self, PlayState* play); // func_808BF078 (wait for AC hit)
void FUN_001ca96c(BgYdanMaruta* self, PlayState* play); // BgYdanMaruta_DoNothing

s32 Collider_DestroyTris(PlayState* play, ColliderTris* collider);
void FUN_001ca9fc(BgYdanMaruta* self, PlayState* play); // func_808BF108 (shake)
void FUN_001ca91c(BgYdanMaruta* self, PlayState* play); // func_808BF1EC (fall)
void FUN_00374bb8(PlayState* play, Actor* actor, f32 arg2, s16 arg3, f32 arg4);
void FUN_00373264(Actor* actor, s32 arg1);
void FUN_00372244(void* arg0, u16 arg1, u32 sfxId);
s32 CollisionCheck_SetAT(PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider);
s32 CollisionCheck_SetAC(PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider);
void Flags_SetSwitch(PlayState* play, s32 flag);
void OnePointCutscene_Init(PlayState* play, s32 csId, s32 duration, Actor* actor, s32 camId);

GLOBAL_ASM("data/z_Bg_Ydan_Maruta.data.s")

ActorInit Bg_Ydan_Maruta_InitVars = {
    ACTOR_BG_YDAN_MARUTA,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_YDAN_OBJECTS,
    sizeof(BgYdanMaruta),
    (ActorFunc)BgYdanMaruta_Init,
    (ActorFunc)BgYdanMaruta_Destroy,
    (ActorFunc)BgYdanMaruta_Update,
    (ActorFunc)BgYdanMaruta_Draw,
};

GLOBAL_ASM("asm/BgYdanMaruta_Init.s")

void FUN_00128194(BgYdanMaruta* self, PlayState* play) {
    if (self->collider.base.atFlags & AT_HIT) {
        FUN_00374bb8(play, &self->dyna.actor, 7.0f, self->dyna.actor.shape.rot.y, 6.0f);
    }
    self->dyna.actor.shape.rot.x += 0x360;
    CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
    FUN_00373264(&self->dyna.actor, 0x01000243);
}

void FUN_001ca970(BgYdanMaruta* self, PlayState* play) {
    if (self->collider.base.acFlags & AC_HIT) {
        self->unk_29C = 30;
        Flags_SetSwitch(play, self->switchFlag);
        FUN_00372244((u8*)play + 0x5FCC, 0x2D, 0x01000487);
        self->actionFunc = FUN_001ca9fc;
        OnePointCutscene_Init(play, 3010, 50, &self->dyna.actor, 0);
    } else {
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
    }
}

void FUN_001ca9fc(BgYdanMaruta* self, PlayState* play) {
    s16 temp;

    if (self->unk_29C == 0 || (--self->unk_29C == 0)) {
        self->actionFunc = FUN_001ca91c;
    }

    temp = (self->unk_29C % 4) - 2;
    if (temp == -2) {
        temp = 0;
    } else {
        temp *= 2;
    }

    self->dyna.actor.world.pos.x = self->dyna.actor.home.pos.x + (temp * Math_CosS(self->dyna.actor.shape.rot.y));
    self->dyna.actor.world.pos.z = self->dyna.actor.home.pos.z + (temp * Math_SinS(self->dyna.actor.shape.rot.y));

    FUN_00373264(&self->dyna.actor, 0x010001AF);
}

void BgYdanMaruta_Destroy(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    BgYdanMaruta* self = (BgYdanMaruta*)thisx;

    Collider_DestroyTris(play, &self->collider);
    if (self->dyna.actor.params == 1) {
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    }
    FUN_00350f34(thisx, &self->skelAnimModel, 0);
}

void FUN_001ca96c(BgYdanMaruta* self, PlayState* play) {
}

void BgYdanMaruta_Update(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    BgYdanMaruta* self = (BgYdanMaruta*)thisx;

    self->actionFunc(self, play);
}

void BgYdanMaruta_Draw(Actor* thisx, GameState* state) {
    BgYdanMaruta* self = (BgYdanMaruta*)thisx;

    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->FUN_003721e0(&self->dyna.actor.modelMtx);
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_00372170(0);
    }
}
}
