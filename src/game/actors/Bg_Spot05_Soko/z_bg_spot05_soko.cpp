extern "C" {
#include "z_bg_spot05_soko.hpp"

#define FLAGS 0x80400000

#define NA_SE_EV_METALDOOR_CLOSE 0x010001B1 // 3DS sfx id (N64 name)
#define ACTOR_FLAG_UPDATE_CULLING_DISABLED 0x10

void OnePointCutscene_Attention(PlayState* play, Actor* actor, s32 arg2, s32 arg3);

void BgSpot05Soko_Init(Actor* actor, GameState* state);
void BgSpot05Soko_Destroy(Actor* actor, GameState* state);
void BgSpot05Soko_Update(Actor* actor, GameState* state);
void BgSpot05Soko_Draw(Actor* actor, GameState* state);
void BgSpot05Soko_DoNothing(BgSpot05Soko* self, PlayState* play);
void FUN_003e8214(BgSpot05Soko* self, PlayState* play);
void FUN_003e0854(BgSpot05Soko* self, PlayState* play);

ActorInit Bg_Spot05_Soko_InitVars = {
    ACTOR_BG_SPOT05_SOKO,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_SPOT05_OBJECTS,
    sizeof(BgSpot05Soko),
    (ActorFunc)BgSpot05Soko_Init,
    (ActorFunc)BgSpot05Soko_Destroy,
    (ActorFunc)BgSpot05Soko_Update,
    (ActorFunc)BgSpot05Soko_Draw,
};

InitChainEntry sBgSpot05SokoInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgSpot05Soko_Init(Actor* actor, GameState* state) {
    BgSpot05Soko* self = (BgSpot05Soko*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(actor, sBgSpot05SokoInitChain);
    self->switchFlag = (u8)(actor->params >> 8);
    actor->params &= 0xFF;
    DynaPolyActor_Init(&self->dyna, 0);
    FUN_00372f38(actor, play, &self->skelAnimModel, 0, &self->skelAnimModel2, 1, 0);
    if (actor->params == 0) {
        ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 0), &colHeader);
        if (LINK_IS_ADULT) {
            Actor_Kill(actor);
        } else {
            self->actionFunc = BgSpot05Soko_DoNothing;
        }
    } else {
        ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 1), &colHeader);
        if (Flags_GetSwitch(play, self->switchFlag) != 0) {
            Actor_Kill(actor);
        } else {
            self->actionFunc = FUN_003e8214;
            actor->flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
        }
    }
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
}

void BgSpot05Soko_Destroy(Actor* actor, GameState* state) {
    BgSpot05Soko* self = (BgSpot05Soko*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, &self->skelAnimModel2, 0);
}

void BgSpot05Soko_DoNothing(BgSpot05Soko* self, PlayState* play) {
}

void FUN_003e8214(BgSpot05Soko* self, PlayState* play) {
    if (Flags_GetSwitch(play, self->switchFlag)) {
        Audio_PlaySoundAtPosition(play, &self->dyna.actor.world.pos, 30, NA_SE_EV_METALDOOR_CLOSE);
        Actor_SetFocus(&self->dyna.actor, 50.0f);
        OnePointCutscene_Attention(play, &self->dyna.actor, 0, 0);
        self->dyna.actor.speedXZ = 0.5f;
        self->actionFunc = FUN_003e0854;
    }
}

void FUN_003e0854(BgSpot05Soko* self, PlayState* play) {
    self->dyna.actor.speedXZ *= 1.5f;
    if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y - 120.0f, self->dyna.actor.speedXZ) !=
        0) {
        Actor_Kill(&self->dyna.actor);
    }
}

void BgSpot05Soko_Update(Actor* actor, GameState* state) {
    BgSpot05Soko* self = (BgSpot05Soko*)actor;

    self->actionFunc(self, (PlayState*)state);
}

void BgSpot05Soko_Draw(Actor* actor, GameState* state) {
    BgSpot05Soko* self = (BgSpot05Soko*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    if (actor->params == 0) {
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
