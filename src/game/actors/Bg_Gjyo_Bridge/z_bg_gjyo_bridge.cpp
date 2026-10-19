extern "C" {
#include "z_bg_gjyo_bridge.hpp"

#define FLAGS 0x80000020

// gSaveContext.eventChkInf[4], bit 0x2000 (set once the rainbow bridge has been created)
#define EVENTCHKINF_CREATED_RAINBOW_BRIDGE 0x2000

s32 Gameplay_InCsMode(PlayState* play);

void BgGjyoBridge_Init(Actor* actor, GameState* state);
void BgGjyoBridge_Destroy(Actor* actor, GameState* state);
void BgGjyoBridge_Update(Actor* actor, GameState* state);
void BgGjyoBridge_Draw(Actor* actor, GameState* state);

void BgGjyoBridge_TriggerCutscene(BgGjyoBridge* self, PlayState* play);
void BgGjyoBridge_SpawnBridge(BgGjyoBridge* self, PlayState* play);

// data/z_Bg_Gjyo_Bridge.data.s: init chain has no label in the disassembly, referenced by address
#define sInitChain ((InitChainEntry*)0x00510F88)

ActorInit Bg_Gjyo_Bridge_InitVars = {
    ACTOR_BG_GJYO_BRIDGE,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GJYO_OBJECTS,
    sizeof(BgGjyoBridge),
    (ActorFunc)BgGjyoBridge_Init,
    (ActorFunc)BgGjyoBridge_Destroy,
    (ActorFunc)BgGjyoBridge_Update,
    (ActorFunc)BgGjyoBridge_Draw,
};

GLOBAL_ASM("data/z_Bg_Gjyo_Bridge.data.s")

void FUN_003e1fb4(BgGjyoBridge* self, PlayState* play) {
}

void BgGjyoBridge_Init(Actor* actor, GameState* state) {
    BgGjyoBridge* self = (BgGjyoBridge*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;
    ZARInfo* zar;

    Actor_ProcessInitChain(actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, 0);
    zar = FUN_00372f38(actor, play, &self->skelAnimModel, 0, 0);
    self->skelAnimModel->unk_0C->FUN_00372d94(zar->GetCMABByIndex(0));
    self->skelAnimModel->unk_0C->SetAnimType(1);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 0), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    if (gSaveContext.eventChkInf[4] & EVENTCHKINF_CREATED_RAINBOW_BRIDGE) {
        self->actionFunc = FUN_003e1fb4;
    } else {
        actor->draw = NULL;
        FUN_0036b940(play, &play->colCtx.dyna, self->dyna.bgId);
        self->actionFunc = BgGjyoBridge_TriggerCutscene;
    }
}

void BgGjyoBridge_Destroy(Actor* actor, GameState* state) {
    BgGjyoBridge* self = (BgGjyoBridge*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgGjyoBridge_TriggerCutscene(BgGjyoBridge* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (CHECK_QUEST_ITEM(QUEST_MEDALLION_SPIRIT) && CHECK_QUEST_ITEM(QUEST_MEDALLION_SHADOW) &&
        (INV_CONTENT(ITEM_ARROW_LIGHT) == ITEM_ARROW_LIGHT) && (player->actor.world.pos.x > -70.0f) &&
        (player->actor.world.pos.x < 300.0f) && (player->actor.world.pos.y > 1340.0f) &&
        (player->actor.world.pos.z > 1340.0f) && (player->actor.world.pos.z < 1662.0f) && !Gameplay_InCsMode(play)) {
        play->FUN_0037573c(play->sceneZARInfo.GetQDBByIndex(0));
        gSaveContext.cutsceneTrigger = 1;
        self->actionFunc = BgGjyoBridge_SpawnBridge;
    }
}

void BgGjyoBridge_SpawnBridge(BgGjyoBridge* self, PlayState* play) {
    // csCtx.actorCues[2] (play + 0x22E4): cue struct starts with u16 id
    if (play->GetCutsceneState() != 0 && *(void**)((u8*)play + 0x22E4) != NULL && **(u16**)((u8*)play + 0x22E4) == 2) {
        self->dyna.actor.draw = BgGjyoBridge_Draw;
        FUN_0036d15c(play, &play->colCtx.dyna, self->dyna.bgId);
        gSaveContext.eventChkInf[4] |= EVENTCHKINF_CREATED_RAINBOW_BRIDGE;
    }
}

void BgGjyoBridge_Update(Actor* actor, GameState* state) {
    BgGjyoBridge* self = (BgGjyoBridge*)actor;

    self->actionFunc(self, (PlayState*)state);
}

void BgGjyoBridge_Draw(Actor* actor, GameState* state) {
    BgGjyoBridge* self = (BgGjyoBridge*)actor;
    MTX34 modelMtx;
    f32 animSpeed;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    animSpeed = 1.0f;
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
