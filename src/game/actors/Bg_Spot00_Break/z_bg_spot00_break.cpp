extern "C" {
#include "z_bg_spot00_break.hpp"

#define FLAGS 0x00400030

void BgSpot00Break_Init(Actor* actor, GameState* state);
void BgSpot00Break_Destroy(Actor* actor, GameState* state);
void BgSpot00Break_Update(Actor* actor, GameState* state);
void BgSpot00Break_Draw(Actor* actor, GameState* state);

ActorInit Bg_Spot00_Break_InitVars = {
    ACTOR_BG_SPOT00_BREAK,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_SPOT00_BREAK,
    sizeof(BgSpot00Break),
    (ActorFunc)BgSpot00Break_Init,
    (ActorFunc)BgSpot00Break_Destroy,
    (ActorFunc)BgSpot00Break_Update,
    (ActorFunc)BgSpot00Break_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneScale, 1200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 2000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_STOP),
};

void BgSpot00Break_Init(Actor* actor, GameState* state) {
    BgSpot00Break* self = (BgSpot00Break*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(actor, sInitChain);
    FUN_00372f38(actor, play, &self->skelAnimModel, 1, &self->skelAnimModel2, 0, 0);
    DynaPolyActor_Init(&self->dyna, 0);
    if (actor->params == 1) {
        ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 1), &colHeader);
    } else {
        ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 0), &colHeader);
    }
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    if (!LINK_IS_ADULT) {
        Actor_Kill(actor);
    }
}

void BgSpot00Break_Destroy(Actor* actor, GameState* state) {
    BgSpot00Break* self = (BgSpot00Break*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, &self->skelAnimModel2, 0);
}

void BgSpot00Break_Update(Actor* actor, GameState* state) {
}

void BgSpot00Break_Draw(Actor* actor, GameState* state) {
    BgSpot00Break* self = (BgSpot00Break*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    if (actor->params == 1) {
        if (self->skelAnimModel != NULL) {
            self->skelAnimModel->SetUnkAC(1);
            self->skelAnimModel->FUN_003721e0(&modelMtx);
            self->skelAnimModel->FUN_00372170(0);
        }
    } else {
        if (self->skelAnimModel2 != NULL) {
            self->skelAnimModel2->SetUnkAC(1);
            self->skelAnimModel2->FUN_003721e0(&modelMtx);
            self->skelAnimModel2->FUN_00372170(0);
        }
    }
}
}
