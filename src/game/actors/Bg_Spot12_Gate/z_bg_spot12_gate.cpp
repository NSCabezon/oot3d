extern "C" {
#include "z_bg_spot12_gate.hpp"

#define FLAGS 0x80400000

// Helpers not declared in include/ yet
s32 Quake_Add(Camera* camera, s32 type);
void Quake_SetSpeed(s32 quakeIndex, s32 speed);
void Quake_SetQuakeValues(s32 quakeIndex, s32 a, s32 b, s32 c, s32 d);
void Quake_SetCountdown(s32 quakeIndex, s32 countdown);
s32 OnePointCutscene_Init(PlayState* play, s32 csId, s32 arg2, Actor* actor, s32 camId);

void BgSpot12Gate_Init(Actor* actor, GameState* state);
void BgSpot12Gate_Destroy(Actor* actor, GameState* state);
void BgSpot12Gate_Update(Actor* actor, GameState* state);
void BgSpot12Gate_Draw(Actor* actor, GameState* state);
void BgSpot12Gate_DoNothing(BgSpot12Gate* self, PlayState* play);
void FUN_003c0aa0(BgSpot12Gate* self, PlayState* play);
void FUN_003cc710(BgSpot12Gate* self, PlayState* play);
void FUN_003af37c(BgSpot12Gate* self, PlayState* play);

ActorInit Bg_Spot12_Gate_InitVars = {
    ACTOR_BG_SPOT12_GATE,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SPOT12_OBJ,
    sizeof(BgSpot12Gate),
    (ActorFunc)BgSpot12Gate_Init,
    (ActorFunc)BgSpot12Gate_Destroy,
    (ActorFunc)BgSpot12Gate_Update,
    (ActorFunc)BgSpot12Gate_Draw,
};

InitChainEntry sBgSpot12GateInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 2500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1200, ICHAIN_STOP),
};

extern InitChainEntry sBgSpot12GateInitChain[];
static inline void BgSpot12Gate_SetupWait(BgSpot12Gate* self) {
    self->actionFunc = FUN_003c0aa0;
    self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y;
}
static inline void BgSpot12Gate_SetupOpen(BgSpot12Gate* self) {
    self->actionFunc = BgSpot12Gate_DoNothing;
    self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y + 200.0f;
}
void BgSpot12Gate_Init(Actor* actor, GameState* state) {
    BgSpot12Gate* self = (BgSpot12Gate*)actor;
    PlayState* play = (PlayState*)state;
    void* colInfo;

    FUN_00372f38(actor, play, &self->skelAnimModel, 0, 0);
    colInfo = DynaPolyInfo_Alloc(&self->dyna, state, 0);
    {
        CollisionHeader* colHeader = NULL;
        DynaPolyActor_Init(&self->dyna, 0);
        ReadCollisionHeaderFromZsi(colInfo, &colHeader);
        self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    }
    Actor_ProcessInitChain(actor, sBgSpot12GateInitChain);
    if (Flags_GetSwitch(play, actor->params & 0x3F)) {
        BgSpot12Gate_SetupOpen(self);
    } else {
        BgSpot12Gate_SetupWait(self);
    }
}

void BgSpot12Gate_Destroy(Actor* actor, GameState* state) {
    BgSpot12Gate* self = (BgSpot12Gate*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgSpot12Gate_Update(Actor* actor, GameState* state) {
    BgSpot12Gate* self = (BgSpot12Gate*)actor;
    PlayState* play = (PlayState*)state;

    if (self->timer > 0) {
        self->timer--;
    }
    self->actionFunc(self, play);
}

void BgSpot12Gate_Draw(Actor* actor, GameState* state) {
    BgSpot12Gate* self = (BgSpot12Gate*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->dyna.actor.modelMtx);
    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&modelMtx);
    self->skelAnimModel->FUN_00372170(0);
}

void BgSpot12Gate_DoNothing(BgSpot12Gate* self, PlayState* play) {
}

void FUN_003c0aa0(BgSpot12Gate* self, PlayState* play) {
    if (Flags_GetSwitch(play, self->dyna.actor.params & 0x3F)) {
        self->actionFunc = FUN_003cc710;
        self->timer = 60;
        OnePointCutscene_Init(play, 4160, -99, &self->dyna.actor, 0);
    }
}

void FUN_003cc710(BgSpot12Gate* self, PlayState* play) {
    if (self->timer <= 0) {
        self->actionFunc = FUN_003af37c;
    }
}

void FUN_003af37c(BgSpot12Gate* self, PlayState* play) {
    Math_StepToF(&self->dyna.actor.velocity.y, 1.6f, 0.03f);
    if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y + 200.0f,
                     self->dyna.actor.velocity.y)) {
        self->actionFunc = BgSpot12Gate_DoNothing;
        self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y + 200.0f;

        {
            s32 quakeIndex = Quake_Add(play->cameraPtrs[play->activeCamera], 3);

            Quake_SetSpeed(quakeIndex, -0x3CB0);
            Quake_SetQuakeValues(quakeIndex, 3, 0, 0, 0);
            Quake_SetCountdown(quakeIndex, 12);
        }

        Audio_PlayActorSound2(&self->dyna.actor, 0x01000165);
    } else {
        FUN_00373264(&self->dyna.actor, 0x010001be);
    }
}

}
