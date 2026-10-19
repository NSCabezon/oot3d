extern "C" {
#include "z_bg_mori_bigst.hpp"

#define FLAGS 0x80400010

// Helpers not declared in include/ yet
s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
s32 Object_IsLoaded(ObjectContext* objectCtx, s32 slot);
s32 FUN_0032d8d8(DynaPolyActor* dyna); // DynaPolyActor_IsPlayerAbove
void FUN_0032b13c(PlayState* play, s32 arg1); // Environment_ChangeLightSetting
void FUN_0028c230(Actor* actor, GameState* state); // Draw
void FUN_001105e0(BgMoriBigst* self, PlayState* play); // N64: BgMoriBigst_WaitForMoriTex
void BgMoriBigst_StalfosFight(BgMoriBigst* self, PlayState* play); // unlabeled 0x0015E1DC inside FUN_0015e154 (EQU in BgMoriBigst_data.s)

GLOBAL_ASM("../src/game/actors/Bg_Mori_Bigst/BgMoriBigst_data.s")

void BgMoriBigst_Init(Actor* actor, GameState* state);
void BgMoriBigst_Destroy(Actor* actor, GameState* state);
void BgMoriBigst_Update(Actor* actor, GameState* state);

ActorInit Bg_Mori_Bigst_InitVars = {
    ACTOR_BG_MORI_BIGST,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_MORI_OBJECTS,
    sizeof(BgMoriBigst),
    (ActorFunc)BgMoriBigst_Init,
    (ActorFunc)BgMoriBigst_Destroy,
    (ActorFunc)BgMoriBigst_Update,
    NULL,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 3000, ICHAIN_CONTINUE),  ICHAIN_F32(uncullZoneScale, 3000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 3000, ICHAIN_CONTINUE),  ICHAIN_F32_DIV1000(gravity, -500, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(minVelocityY, -12000, ICHAIN_CONTINUE), ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_STOP),
};

void BgMoriBigst_Init(Actor* actor, GameState* state) {
    BgMoriBigst* self = (BgMoriBigst*)actor;
    PlayState* play = (PlayState*)state;

    FUN_00372f38(actor, play, &self->skelAnimModel, 1, 0);
    {
        CollisionHeader* colHeader;
        void* colInfo = DynaPolyInfo_Alloc(&self->dyna, state, 1);

        colHeader = NULL;
        DynaPolyActor_Init(&self->dyna, 0);
        ReadCollisionHeaderFromZsi(colInfo, &colHeader);
        self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    }
    Actor_ProcessInitChain(actor, sInitChain);
    self->moriTexObjectSlot = Object_GetIndex(&play->objectCtx, OBJECT_MORI_TEX);
    if (self->moriTexObjectSlot < 0) {
        Actor_Kill(actor);
        return;
    }
    if (Flags_GetSwitch(play, (((u32)actor->params << 18) >> 26))) {
        actor->world.pos.y = 827.0f;
    } else {
        actor->world.pos.y = actor->home.pos.y + 270.0f;
    }
    Actor_SetFocus(actor, 50.0f);
    self->actionFunc = FUN_001105e0;
}

void BgMoriBigst_Destroy(Actor* actor, GameState* state) {
    BgMoriBigst* self = (BgMoriBigst*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgMoriBigst_Update(Actor* actor, GameState* state) {
    BgMoriBigst* self = (BgMoriBigst*)actor;
    PlayState* play = (PlayState*)state;

    Actor_SetFocus(actor, 50.0f);
    if (self->waitTimer > 0) {
        self->waitTimer--;
    }
    if (FUN_0032d8d8(&self->dyna)) {
        FUN_0032b13c(play, 6);
    }
    if (self->actionFunc != NULL) {
        self->actionFunc(self, play);
    }
}

void FUN_0028c230(Actor* actor, GameState* state) {
    BgMoriBigst* self = (BgMoriBigst*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&modelMtx);
    self->skelAnimModel->FUN_00372170(0);
}

static inline void BgMoriBigst_SetupAction(BgMoriBigst* self, BgMoriBigstActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

static inline void BgMoriBigst_SetupNoop(BgMoriBigst* self, PlayState* play) {
    BgMoriBigst_SetupAction(self, NULL);
}

static inline void BgMoriBigst_SetupDone(BgMoriBigst* self, PlayState* play) {
    BgMoriBigst_SetupAction(self, NULL);
}

static inline void BgMoriBigst_SetupStalfosFight(BgMoriBigst* self, PlayState* play) {
    Actor* stalfos;

    BgMoriBigst_SetupAction(self, BgMoriBigst_StalfosFight);
    Flags_UnsetClear(play, self->dyna.actor.room);
    stalfos = Actor_SpawnAsChild(&play->actorCtx, &self->dyna.actor, play, ACTOR_EN_TEST, 209.0f, 827.0f, -3320.0f, 0,
                                 0, 0, 1);
    if (stalfos != NULL) {
        self->dyna.actor.home.rot.z++;
        self->dyna.actor.child = NULL;
    }
    Flags_SetClear(play, self->dyna.actor.room);
}

void FUN_001105e0(BgMoriBigst* self, PlayState* play) {
    Actor* actor = &self->dyna.actor;

    if (Object_IsLoaded(&play->objectCtx, self->moriTexObjectSlot)) {
        actor->draw = (ActorFunc)FUN_0028c230;
        if (Flags_GetClear(play, actor->room) && (GET_PLAYER(play)->actor.world.pos.y > 700.0f)) {
            if (Flags_GetSwitch(play, (((u32)actor->params << 18) >> 26))) {
                BgMoriBigst_SetupDone(self, play);
            } else {
                BgMoriBigst_SetupStalfosFight(self, play);
            }
        } else {
            BgMoriBigst_SetupNoop(self, play);
        }
    }
}
}
