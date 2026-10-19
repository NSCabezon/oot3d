extern "C" {
#include "z_bg_spot09_obj.hpp"

#define FLAGS 0x00400000

#define BG_SPOT09_OBJ_BRIDGE_SIDES 0
#define BG_SPOT09_OBJ_BRIDGE_BROKEN 1
#define BG_SPOT09_OBJ_BRIDGE_CHILD 2
#define BG_SPOT09_OBJ_TENT 3
#define BG_SPOT09_OBJ_BRIDGE_REPAIRED 4

void BgSpot09Obj_Init(Actor* actor, GameState* state);
void BgSpot09Obj_Destroy(Actor* actor, GameState* state);
void BgSpot09Obj_Update(Actor* actor, GameState* state);
void BgSpot09Obj_Draw(Actor* actor, GameState* state);

s32 FUN_00257b4c(BgSpot09Obj* self, GameState* state);
s32 FUN_00257ab0(BgSpot09Obj* self, GameState* state);
s32 FUN_001ffcd8(BgSpot09Obj* self, GameState* state);

ActorInit Bg_Spot09_Obj_InitVars = {
    ACTOR_BG_SPOT09_OBJ,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SPOT09_OBJ,
    sizeof(BgSpot09Obj),
    (ActorFunc)BgSpot09Obj_Init,
    (ActorFunc)BgSpot09Obj_Destroy,
    (ActorFunc)BgSpot09Obj_Update,
    (ActorFunc)BgSpot09Obj_Draw,
};

static s32 sDynaColIdx[] = {
    -1, 1, 0, 3, 2,
};

static s32 (*sSetupFuncs[])(BgSpot09Obj* self, GameState* state) = {
    FUN_00257b4c,
    FUN_00257ab0,
    FUN_001ffcd8,
};

static InitChainEntry sInitChain1[] = {
    ICHAIN_F32(uncullZoneForward, 7200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 3000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 7200, ICHAIN_STOP),
};

static InitChainEntry sInitChain2[] = {
    ICHAIN_F32(uncullZoneForward, 7200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 800, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1500, ICHAIN_STOP),
};

void BgSpot09Obj_Init(Actor* actor, GameState* state) {
    BgSpot09Obj* self = (BgSpot09Obj*)actor;
    PlayState* play = (PlayState*)state;
    u32 i;

    self->dyna.actor.params &= 0xFF;

    FUN_00372f38(actor, play, &self->skelAnimModel[0], 0, &self->skelAnimModel[1], 2,
                 &self->skelAnimModel[2], 1, &self->skelAnimModel[3], 4, &self->skelAnimModel[4], 3,
                 &self->skelAnimModel[5], 5, NULL);

    for (i = 0; i < ARRAY_COUNTU(sSetupFuncs); i++) {
        if (!sSetupFuncs[i](self, state)) {
            Actor_Kill(actor);
            return;
        }
    }

    if (self->dyna.actor.params == BG_SPOT09_OBJ_TENT) {
        Actor_ProcessInitChain(actor, sInitChain2);
    } else {
        Actor_ProcessInitChain(actor, sInitChain1);
    }
}

void BgSpot09Obj_Destroy(Actor* actor, GameState* state) {
    BgSpot09Obj* self = (BgSpot09Obj*)actor;
    PlayState* play = (PlayState*)state;

    if (self->dyna.actor.params != BG_SPOT09_OBJ_BRIDGE_SIDES) {
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    }
    FUN_00350f34(actor, &self->skelAnimModel[0], &self->skelAnimModel[1], &self->skelAnimModel[2],
                 &self->skelAnimModel[3], &self->skelAnimModel[4], &self->skelAnimModel[5], NULL);
}

void BgSpot09Obj_Update(Actor* actor, GameState* state) {
}

void BgSpot09Obj_Draw(Actor* actor, GameState* state) {
    BgSpot09Obj* self = (BgSpot09Obj*)actor;
    MTX34 modelMtx;
    s32 idx;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    idx = actor->params;
    self->skelAnimModel[idx]->SetUnkAC(1);
    self->skelAnimModel[idx]->FUN_003721e0(&modelMtx);
    self->skelAnimModel[idx]->FUN_00372170(0);

    if (actor->params == BG_SPOT09_OBJ_TENT) {
        self->skelAnimModel[5]->SetUnkAC(1);
        self->skelAnimModel[5]->FUN_003721e0(&modelMtx);
        self->skelAnimModel[5]->FUN_00372170(0);
    }
}

s32 FUN_00257b4c(BgSpot09Obj* self, GameState* state) {
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    if (sDynaColIdx[self->dyna.actor.params] > -1) {
        DynaPolyActor_Init(&self->dyna, 0);
        ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, sDynaColIdx[self->dyna.actor.params]), &colHeader);
        self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);
    }
    return true;
}

s32 FUN_00257ab0(BgSpot09Obj* self, GameState* state) {
    PlayState* play = (PlayState*)state;
    s32 carpentersRescued;

    if (gSaveContext.sceneSetupIndex >= 4) { // IS_CUTSCENE_LAYER
        return self->dyna.actor.params == BG_SPOT09_OBJ_BRIDGE_SIDES;
    }

    carpentersRescued = (gSaveContext.eventChkInf[9] & 0xF) == 0xF;

    if (gSaveContext.linkAge == 0) {
        switch (self->dyna.actor.params) {
            case BG_SPOT09_OBJ_BRIDGE_SIDES:
                return false;
            case BG_SPOT09_OBJ_BRIDGE_BROKEN:
                return !carpentersRescued;
            case BG_SPOT09_OBJ_BRIDGE_REPAIRED:
                return carpentersRescued;
            case BG_SPOT09_OBJ_TENT:
                return true;
        }
    } else {
        return self->dyna.actor.params == BG_SPOT09_OBJ_BRIDGE_CHILD;
    }

    return false;
}

s32 FUN_001ffcd8(BgSpot09Obj* self, GameState* state) {
    if (self->dyna.actor.params == BG_SPOT09_OBJ_TENT) {
        Actor_SetScale(&self->dyna.actor, 0.1f);
    } else {
        Actor_SetScale(&self->dyna.actor, 1.0f);
    }
    return 1;
}
}
