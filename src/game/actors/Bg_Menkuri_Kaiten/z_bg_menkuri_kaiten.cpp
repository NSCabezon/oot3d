extern "C" {
#include "z_bg_menkuri_kaiten.hpp"

#define FLAGS 0x80400030

void BgMenkuriKaiten_Init(Actor* actor, GameState* state);
void BgMenkuriKaiten_Destroy(Actor* actor, GameState* state);
void BgMenkuriKaiten_Update(Actor* actor, GameState* state);
void BgMenkuriKaiten_Draw(Actor* actor, GameState* state);

// Only referenced from this actor (N64 counterpart: DynaPolyActor_IsPlayerAbove)
s32 FUN_0032d8d8(Actor* actor);

ActorInit Bg_Menkuri_Kaiten_InitVars = {
    ACTOR_BG_MENKURI_KAITEN,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_MENKURI_OBJECTS,
    sizeof(BgMenkuriKaiten),
    (ActorFunc)BgMenkuriKaiten_Init,
    (ActorFunc)BgMenkuriKaiten_Destroy,
    (ActorFunc)BgMenkuriKaiten_Update,
    (ActorFunc)BgMenkuriKaiten_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgMenkuriKaiten_Init(Actor* actor, GameState* state) {
    BgMenkuriKaiten* self = (BgMenkuriKaiten*)actor;
    PlayState* play = (PlayState*)state;
    ZARInfo* zarInfo;

    CollisionHeader* colHeader = NULL;
    Actor_ProcessInitChain(actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, 3);
    zarInfo = FUN_00372f38(actor, play, &self->skelAnimModel, 4, 0);
    self->skelAnimModel->unk_0C->FUN_00372d94(zarInfo->GetCMABByIndex(0));
    self->skelAnimModel->unk_0C->SetAnimType(1);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 0), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
}

void BgMenkuriKaiten_Destroy(Actor* actor, GameState* state) {
    BgMenkuriKaiten* self = (BgMenkuriKaiten*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgMenkuriKaiten_Update(Actor* actor, GameState* state) {
    BgMenkuriKaiten* self = (BgMenkuriKaiten*)actor;
    PlayState* play = (PlayState*)state;

    if (!Flags_GetSwitch(play, self->dyna.actor.params) && FUN_0032d8d8(actor)) {
        FUN_00373264(actor, 0x0100017B);
        self->dyna.actor.shape.rot.y += 0x55;
    }
}

void BgMenkuriKaiten_Draw(Actor* actor, GameState* state) {
    BgMenkuriKaiten* self = (BgMenkuriKaiten*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->dyna.actor.modelMtx);
    f32 animSpeed = 1.0f;
    if (PauseContext_GetState() != 0) {
        animSpeed = 0.0f;
    }
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->unk_0C->SetAnimSpeed(animSpeed);
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}
}
