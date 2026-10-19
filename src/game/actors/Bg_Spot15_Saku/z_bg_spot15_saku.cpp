extern "C" {
#include "z_bg_spot15_saku.hpp"

#define FLAGS 0x80400000

// INFTABLE_71: gSaveContext.infTable[7], bit 1
#define INFTABLE_71_SET (gSaveContext.infTable[7] & 0x2)

void BgSpot15Saku_Init(Actor* actor, GameState* state);
void BgSpot15Saku_Destroy(Actor* actor, GameState* state);
void BgSpot15Saku_Update(Actor* actor, GameState* state);
void BgSpot15Saku_Draw(Actor* actor, GameState* state);

void FUN_003e8290(BgSpot15Saku* self, PlayState* play);
void FUN_003e02a0(BgSpot15Saku* self, PlayState* play);
void FUN_003e2020(BgSpot15Saku* self, PlayState* play);

ActorInit Bg_Spot15_Saku_InitVars = {
    ACTOR_BG_SPOT15_SAKU,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_SPOT15_OBJ,
    sizeof(BgSpot15Saku),
    (ActorFunc)BgSpot15Saku_Init,
    (ActorFunc)BgSpot15Saku_Destroy,
    (ActorFunc)BgSpot15Saku_Update,
    (ActorFunc)BgSpot15Saku_Draw,
};

void BgSpot15Saku_Init(Actor* actor, GameState* state) {
    BgSpot15Saku* self = (BgSpot15Saku*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, 0);
    FUN_00372f38(actor, play, &self->skelAnimModel, 1, 0);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 1), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    self->unk_170.x = actor->world.pos.x;
    self->unk_170.y = actor->world.pos.y;
    self->unk_170.z = actor->world.pos.z;
    actor->scale.x = 0.1f;
    actor->scale.y = 0.1f;
    actor->scale.z = 0.1f;
    if (INFTABLE_71_SET) {
        actor->world.pos.z = 2659.0f;
    }
    self->actionFunc = FUN_003e8290;
}

void BgSpot15Saku_Destroy(Actor* actor, GameState* state) {
    BgSpot15Saku* self = (BgSpot15Saku*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void FUN_003e8290(BgSpot15Saku* self, PlayState* play) {
    if (self->unk_168 != 0 && !INFTABLE_71_SET) {
        self->timer = 3;
        self->actionFunc = FUN_003e02a0;
    }
}

void FUN_003e02a0(BgSpot15Saku* self, PlayState* play) {
    if (self->timer == 0) {
        Audio_PlayActorSound2(&self->dyna.actor, 0x010001BE);
        self->dyna.actor.world.pos.z -= 2.0f;
        if (self->dyna.actor.world.pos.z < 2660.0f) {
            Audio_PlayActorSound2(&self->dyna.actor, 0x01000165);
            self->timer = 45;
            self->actionFunc = FUN_003e2020;
        }
    }
}

void FUN_003e2020(BgSpot15Saku* self, PlayState* play) {
    if (self->timer == 0) {
        self->unk_168 = 0;
        self->actionFunc = FUN_003e8290;
    }
}

void BgSpot15Saku_Update(Actor* actor, GameState* state) {
    BgSpot15Saku* self = (BgSpot15Saku*)actor;

    if (self->timer != 0) {
        self->timer--;
    }

    self->actionFunc(self, (PlayState*)state);
}

void BgSpot15Saku_Draw(Actor* actor, GameState* state) {
    BgSpot15Saku* self = (BgSpot15Saku*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}
}
