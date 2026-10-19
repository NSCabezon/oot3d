extern "C" {
#include "z_bg_spot12_saku.hpp"

#define FLAGS 0x80400000

// Helpers not declared in include/ yet
s32 OnePointCutscene_Init(PlayState* play, s32 csId, s32 arg2, Actor* actor, s32 camId);

void BgSpot12Saku_Init(Actor* actor, GameState* state);
void BgSpot12Saku_Destroy(Actor* actor, GameState* state);
void BgSpot12Saku_Update(Actor* actor, GameState* state);
void BgSpot12Saku_Draw(Actor* actor, GameState* state);
void BgSpot12Saku_DoNothing(BgSpot12Saku* self, PlayState* play);
void FUN_003c0afc(BgSpot12Saku* self, PlayState* play);
void FUN_003cc43c(BgSpot12Saku* self, PlayState* play);

ActorInit Bg_Spot12_Saku_InitVars = {
    ACTOR_BG_SPOT12_SAKU,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SPOT12_OBJ,
    sizeof(BgSpot12Saku),
    (ActorFunc)BgSpot12Saku_Init,
    (ActorFunc)BgSpot12Saku_Destroy,
    (ActorFunc)BgSpot12Saku_Update,
    (ActorFunc)BgSpot12Saku_Draw,
};

InitChainEntry sBgSpot12SakuInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_STOP),
};

static inline void BgSpot12Saku_SetupWait(BgSpot12Saku* self) {
    self->actionFunc = FUN_003c0afc;
    self->dyna.actor.scale.x = 0.1f;
    self->dyna.actor.world.pos.x = self->dyna.actor.home.pos.x;
    self->dyna.actor.world.pos.z = self->dyna.actor.home.pos.z;
}
void BgSpot12Saku_Init(Actor* actor, GameState* state) {
    BgSpot12Saku* self = (BgSpot12Saku*)actor;
    PlayState* play = (PlayState*)state;
    void* colInfo;

    FUN_00372f38(actor, play, &self->skelAnimModel, 1, 0);
    colInfo = DynaPolyInfo_Alloc(&self->dyna, state, 1);
    {
        CollisionHeader* colHeader = NULL;
        DynaPolyActor_Init(&self->dyna, 0);
        ReadCollisionHeaderFromZsi(colInfo, &colHeader);
        self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    }
    Actor_ProcessInitChain(actor, sBgSpot12SakuInitChain);
    if (Flags_GetSwitch(play, actor->params & 0x3F)) {
        self->actionFunc = BgSpot12Saku_DoNothing;
        self->dyna.actor.scale.x = 0.001f / 0.14f;
        self->dyna.actor.world.pos.x = self->dyna.actor.home.pos.x - (Math_SinS(self->dyna.actor.shape.rot.y + 0x4000) * 78.0f);
        self->dyna.actor.world.pos.z = self->dyna.actor.home.pos.z - (Math_CosS(self->dyna.actor.shape.rot.y + 0x4000) * 78.0f);
    } else {
        BgSpot12Saku_SetupWait(self);
    }
}

void BgSpot12Saku_Destroy(Actor* actor, GameState* state) {
    BgSpot12Saku* self = (BgSpot12Saku*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgSpot12Saku_Update(Actor* actor, GameState* state) {
    BgSpot12Saku* self = (BgSpot12Saku*)actor;
    PlayState* play = (PlayState*)state;

    if (self->timer > 0) {
        self->timer--;
    }
    self->actionFunc(self, play);
}

void BgSpot12Saku_Draw(Actor* actor, GameState* state) {
    BgSpot12Saku* self = (BgSpot12Saku*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->dyna.actor.modelMtx);
    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&modelMtx);
    self->skelAnimModel->FUN_00372170(0);
}

void BgSpot12Saku_DoNothing(BgSpot12Saku* self, PlayState* play) {
}

void FUN_003c0afc(BgSpot12Saku* self, PlayState* play) {
    if (Flags_GetSwitch(play, self->dyna.actor.params & 0x3F)) {
        if (self->timer == 0) {
            self->actionFunc = FUN_003cc43c;
        }
        self->timer = 30;
        OnePointCutscene_Init(play, 4170, -99, &self->dyna.actor, 0);
    }
}

void FUN_003cc43c(BgSpot12Saku* self, PlayState* play) {
    f32 temp_ret = Math_SmoothStepToF(&self->dyna.actor.scale.x, 0.001f / 0.14f, 0.16f, 0.0022f, 0.001f);
    f32 temp_f18 = ((0.1f - self->dyna.actor.scale.x) * 840.0f);

    self->dyna.actor.world.pos.x = self->dyna.actor.home.pos.x - (temp_f18 * Math_SinS(self->dyna.actor.shape.rot.y + 0x4000));
    self->dyna.actor.world.pos.z = self->dyna.actor.home.pos.z - (temp_f18 * Math_CosS(self->dyna.actor.shape.rot.y + 0x4000));
    if (__fabsf(temp_ret) < 0.0001f) {
        self->actionFunc = BgSpot12Saku_DoNothing;
        self->dyna.actor.scale.x = 0.001f / 0.14f;
        self->dyna.actor.world.pos.x = self->dyna.actor.home.pos.x - (Math_SinS(self->dyna.actor.shape.rot.y + 0x4000) * 78.0f);
        self->dyna.actor.world.pos.z = self->dyna.actor.home.pos.z - (Math_CosS(self->dyna.actor.shape.rot.y + 0x4000) * 78.0f);
        Audio_PlayActorSound2(&self->dyna.actor, 0x01000165);
    } else {
        FUN_00373264(&self->dyna.actor, 0x010001be);
    }
}

}
