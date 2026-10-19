extern "C" {
#include "z_bg_spot18_futa.hpp"

#define FLAGS 0x80400000

void BgSpot18Futa_Init(Actor* actor, GameState* state);
void BgSpot18Futa_Destroy(Actor* actor, GameState* state);
void BgSpot18Futa_Update(Actor* actor, GameState* state);
void BgSpot18Futa_Draw(Actor* actor, GameState* state);

ActorInit Bg_Spot18_Futa_InitVars = {
    ACTOR_BG_SPOT18_FUTA,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_SPOT18_OBJ,
    sizeof(BgSpot18Futa),
    (ActorFunc)BgSpot18Futa_Init,
    (ActorFunc)BgSpot18Futa_Destroy,
    (ActorFunc)BgSpot18Futa_Update,
    (ActorFunc)BgSpot18Futa_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_STOP),
};

void BgSpot18Futa_Init(Actor* actor, GameState* state) {
    BgSpot18Futa* self = (BgSpot18Futa*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, 0);
    FUN_00372f38(actor, play, &self->skelAnimModel, 0, NULL);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 0), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    Actor_ProcessInitChain(actor, sInitChain);
}

void BgSpot18Futa_Destroy(Actor* actor, GameState* state) {
    BgSpot18Futa* self = (BgSpot18Futa*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgSpot18Futa_Update(Actor* actor, GameState* state) {
    BgSpot18Futa* self = (BgSpot18Futa*)actor;

    if (self->dyna.actor.parent == NULL) {
        if (Math_StepToF(&self->dyna.actor.scale.x, 0.0f, 0.005f)) {
            Actor_Kill(actor);
        } else {
            f32 scaleX = self->dyna.actor.scale.x;

            self->dyna.actor.scale.z = scaleX;
            self->dyna.actor.scale.y = scaleX;
        }
    }
}

void BgSpot18Futa_Draw(Actor* actor, GameState* state) {
    BgSpot18Futa* self = (BgSpot18Futa*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->dyna.actor.modelMtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}
}
