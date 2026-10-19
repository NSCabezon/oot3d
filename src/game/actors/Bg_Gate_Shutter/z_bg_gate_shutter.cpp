extern "C" {
#include "z_bg_gate_shutter.hpp"

#define FLAGS 0x80400000

// INFTABLE_76: gSaveContext.infTable[7], bit 0x40
#define INFTABLE_76_SET (gSaveContext.infTable[7] & 0x40)
// EVENTCHKINF_OBTAINED_MASTER_SWORD: gSaveContext.eventChkInf[4], bit 0x20
#define EVENTCHKINF_OBTAINED_MASTER_SWORD_SET (gSaveContext.eventChkInf[4] & 0x20)
#define SCENE_KAKARIKO_VILLAGE 0x52

void BgGateShutter_Init(Actor* actor, GameState* state);
void BgGateShutter_Destroy(Actor* actor, GameState* state);
void BgGateShutter_Update(Actor* actor, GameState* state);
void BgGateShutter_Draw(Actor* actor, GameState* state);

void FUN_003e858c(BgGateShutter* self, PlayState* play); // N64: func_8087828C
void FUN_003e041c(BgGateShutter* self, PlayState* play); // N64: func_80878300
void FUN_003e2050(BgGateShutter* self, PlayState* play); // N64: func_808783AC
void FUN_003ca4a4(BgGateShutter* self, PlayState* play); // N64: func_808783D4

ActorInit Bg_Gate_Shutter_InitVars = {
    ACTOR_BG_GATE_SHUTTER,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_SPOT01_MATOYAB,
    sizeof(BgGateShutter),
    (ActorFunc)BgGateShutter_Init,
    (ActorFunc)BgGateShutter_Destroy,
    (ActorFunc)BgGateShutter_Update,
    (ActorFunc)BgGateShutter_Draw,
};

void BgGateShutter_Init(Actor* actor, GameState* state) {
    BgGateShutter* self = (BgGateShutter*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, 0);
    FUN_00372f38(actor, play, &self->skelAnimModel, 1, 0);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 1), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    self->somePos.x = actor->world.pos.x;
    self->somePos.y = actor->world.pos.y;
    self->somePos.z = actor->world.pos.z;
    if ((INFTABLE_76_SET || EVENTCHKINF_OBTAINED_MASTER_SWORD_SET) && play->sceneNum == SCENE_KAKARIKO_VILLAGE) {
        actor->world.pos.x = -89.0f;
        actor->world.pos.z = -1375.0f;
    }
    actor->scale.x = 1.0f;
    actor->scale.y = 1.0f;
    actor->scale.z = 1.0f;
    self->actionFunc = FUN_003e858c;
}

void BgGateShutter_Destroy(Actor* actor, GameState* state) {
    BgGateShutter* self = (BgGateShutter*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

// N64: func_8087828C. Early returns (not the N64 else-if chain) reproduce the shared store block layout.
void FUN_003e858c(BgGateShutter* self, PlayState* play) {
    if (self->openingState == 1 && !INFTABLE_76_SET) {
        self->timer = 3;
        self->actionFunc = FUN_003e041c;
        return;
    }
    if (self->openingState == 2) {
        self->timer = 3;
        self->actionFunc = FUN_003e041c;
        return;
    }
    if (self->openingState < 0) {
        self->timer = 3;
        self->actionFunc = FUN_003ca4a4;
    }
}

void FUN_003e041c(BgGateShutter* self, PlayState* play) {
    Actor* thisx = &self->dyna.actor;

    if (self->timer == 0) {
        Audio_PlayActorSound2(thisx, 0x010001BE); // NA_SE_EV_METALGATE_OPEN - SFX_FLAG
        thisx->world.pos.x -= 2.0f;
        Math_ApproachF(&thisx->world.pos.z, -1375.0f, 0.8f, 0.3f);
        if (thisx->world.pos.x < -89.0f) {
            Audio_PlayActorSound2(thisx, 0x01000165); // NA_SE_EV_BRIDGE_OPEN_STOP
            self->timer = 45;
            self->actionFunc = FUN_003e2050;
        }
    }
}

void FUN_003e2050(BgGateShutter* self, PlayState* play) {
    if (self->timer == 0) {
        self->openingState = 0;
        self->actionFunc = FUN_003e858c;
    }
}

void FUN_003ca4a4(BgGateShutter* self, PlayState* play) {
    Actor* thisx = &self->dyna.actor;

    if (self->timer == 0) {
        Audio_PlayActorSound2(thisx, 0x010001BE); // NA_SE_EV_METALGATE_OPEN - SFX_FLAG
        thisx->world.pos.x += 2.0f;
        Math_ApproachF(&thisx->world.pos.z, -1350.0f, 0.8f, 0.3f);
        if (thisx->world.pos.x > 90.0f) {
            thisx->world.pos.x = 91.0f;
            Audio_PlayActorSound2(thisx, 0x01000165); // NA_SE_EV_BRIDGE_OPEN_STOP
            self->timer = 45;
            self->actionFunc = FUN_003e2050;
        }
    }
}

void BgGateShutter_Update(Actor* actor, GameState* state) {
    BgGateShutter* self = (BgGateShutter*)actor;

    if (self->timer != 0) {
        self->timer--;
    }
    self->actionFunc(self, (PlayState*)state);
}

void BgGateShutter_Draw(Actor* actor, GameState* state) {
    BgGateShutter* self = (BgGateShutter*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}
}
