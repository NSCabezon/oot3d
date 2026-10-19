extern "C" {
#include "z_bg_spot01_idosoko.hpp"

#define FLAGS 0x80400010

void BgSpot01Idosoko_Init(Actor* actor, GameState* state);
void BgSpot01Idosoko_Destroy(Actor* actor, GameState* state);
void BgSpot01Idosoko_Update(Actor* actor, GameState* state);
void BgSpot01Idosoko_Draw(Actor* actor, GameState* state);
void BgSpot01Idosoko_DoNothing(BgSpot01Idosoko* self, PlayState* play);

ActorInit Bg_Spot01_Idosoko_InitVars = {
    ACTOR_BG_SPOT01_IDOSOKO,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SPOT01_MATOYA,
    sizeof(BgSpot01Idosoko),
    (ActorFunc)BgSpot01Idosoko_Init,
    (ActorFunc)BgSpot01Idosoko_Destroy,
    (ActorFunc)BgSpot01Idosoko_Update,
    (ActorFunc)BgSpot01Idosoko_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgSpot01Idosoko_Init(Actor* actor, GameState* state) {
    BgSpot01Idosoko* self = (BgSpot01Idosoko*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, 1);
    Actor_ProcessInitChain(actor, sInitChain);
    FUN_00372f38(actor, play, &self->skelAnimModel, 2, 0);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 1), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    if (LINK_IS_ADULT) {
        self->actionFunc = BgSpot01Idosoko_DoNothing;
    } else {
        Actor_Kill(actor);
    }
}

void BgSpot01Idosoko_Destroy(Actor* actor, GameState* state) {
    BgSpot01Idosoko* self = (BgSpot01Idosoko*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgSpot01Idosoko_Update(Actor* actor, GameState* state) {
    BgSpot01Idosoko* self = (BgSpot01Idosoko*)actor;
    PlayState* play = (PlayState*)state;

    self->actionFunc(self, play);
}

void BgSpot01Idosoko_Draw(Actor* actor, GameState* state) {
    BgSpot01Idosoko* self = (BgSpot01Idosoko*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->dyna.actor.modelMtx);
    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&modelMtx);
    self->skelAnimModel->FUN_00372170(0);
}

void BgSpot01Idosoko_DoNothing(BgSpot01Idosoko* self, PlayState* play) {
}
}
