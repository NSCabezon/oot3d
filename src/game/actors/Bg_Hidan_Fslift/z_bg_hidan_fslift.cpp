extern "C" {
#include "z_bg_hidan_fslift.hpp"

#define FLAGS 0x80400010

void BgHidanFslift_Init(Actor* actor, GameState* state);
void BgHidanFslift_Destroy(Actor* actor, GameState* state);
void BgHidanFslift_Update(Actor* actor, GameState* state);
void BgHidanFslift_Draw(Actor* actor, GameState* state);

void FUN_003e85ec(BgHidanFslift* self, PlayState* play); // N64: BgHidanFslift_Idle
void FUN_003d9eb4(BgHidanFslift* self, PlayState* play); // N64: BgHidanFslift_Descend
void FUN_003ba174(BgHidanFslift* self, PlayState* play); // N64: BgHidanFslift_Ascend
s32 FUN_0032d8d8(DynaPolyActor* dyna);                  // DynaPolyActorIsPlayerAbove
s32 FUN_0036adf4(DynaPolyActor* dyna);                  // DynaPolyActor_IsPlayerOnTop
void Camera_ChangeSetting(Camera* camera, s32 setting);

ActorInit Bg_Hidan_Fslift_InitVars = {
    ACTOR_BG_HIDAN_FSLIFT,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HIDAN_OBJECTS,
    sizeof(BgHidanFslift),
    (ActorFunc)BgHidanFslift_Init,
    (ActorFunc)BgHidanFslift_Destroy,
    (ActorFunc)BgHidanFslift_Update,
    (ActorFunc)BgHidanFslift_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 300, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 350, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 2000, ICHAIN_STOP),
};

void BgHidanFslift_Init(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    BgHidanFslift* self = (BgHidanFslift*)actor;
    CollisionHeader* colHeader = NULL;
    Actor* child;

    Actor_ProcessInitChain(actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, 1);
    FUN_00372f38(actor, play, &self->skelAnimModel, 0xC, 0);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 8), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    child = Actor_SpawnAsChild(&play->actorCtx, actor, play, 0x12D, self->dyna.actor.world.pos.x,
                               self->dyna.actor.world.pos.y + 40.0f, self->dyna.actor.world.pos.z + -28.0f, 0, 0, 0,
                               2);
    if (child != NULL) {
        self->actionFunc = FUN_003e85ec;
    } else {
        Actor_Kill(actor);
    }
}

// N64: BgHidanFslift_SetHookshotTargetPos
static void BgHidanFslift_SetHookshotTargetPos(BgHidanFslift* self) {
    if (self->dyna.actor.child != NULL && self->dyna.actor.child->update != NULL) {
        self->dyna.actor.child->world.pos.x = self->dyna.actor.world.pos.x;
        self->dyna.actor.child->world.pos.y = self->dyna.actor.world.pos.y + 40.0f;
        self->dyna.actor.child->world.pos.z = self->dyna.actor.world.pos.z + -28.0f;
    } else {
        self->dyna.actor.child = NULL;
    }
}

void BgHidanFslift_Destroy(Actor* actor, GameState* state) {
    BgHidanFslift* self = (BgHidanFslift*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgHidanFslift_Update(Actor* actor, GameState* state) {
    BgHidanFslift* self = (BgHidanFslift*)actor;
    PlayState* play = (PlayState*)state;

    self->actionFunc(self, play);
    if (FUN_0036adf4(&self->dyna)) {
        if (self->cameraSetting == 0) {
            self->cameraSetting = 3;
        }
        Camera_ChangeSetting(play->cameraPtrs[0], 0x30);
    } else if (!FUN_0036adf4(&self->dyna)) {
        if (self->cameraSetting != 0) {
            Camera_ChangeSetting(play->cameraPtrs[0], 3);
        }
        self->cameraSetting = 0;
    }
}

void BgHidanFslift_Draw(Actor* actor, GameState* state) {
    BgHidanFslift* self = (BgHidanFslift*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->dyna.actor.modelMtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}

// N64: BgHidanFslift_Idle
void FUN_003e85ec(BgHidanFslift* self, PlayState* play) {
    s32 nearHomePos;

    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer == 0) {
        nearHomePos = ((self->dyna.actor.world.pos.y - self->dyna.actor.home.pos.y) < 0.5f) ? 1 : 0;
        if (FUN_0032d8d8(&self->dyna) && nearHomePos) {
            self->actionFunc = FUN_003ba174;
        } else if (!nearHomePos) {
            self->actionFunc = FUN_003d9eb4;
        }
    }
}

// N64: BgHidanFslift_Descend
void FUN_003d9eb4(BgHidanFslift* self, PlayState* play) {
    if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y, 4.0f)) {
        Audio_PlayActorSound2(&self->dyna.actor, 0x0100018C);
        self->timer = 60;
        self->actionFunc = FUN_003e85ec;
    } else {
        FUN_00373264(&self->dyna.actor, 0x01000210);
    }
    BgHidanFslift_SetHookshotTargetPos(self);
}

// N64: BgHidanFslift_Ascend
void FUN_003ba174(BgHidanFslift* self, PlayState* play) {
    if (FUN_0032d8d8(&self->dyna)) {
        if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y + 790.0f, 4.0f)) {
            Audio_PlayActorSound2(&self->dyna.actor, 0x0100018C);
            self->timer = 60;
            self->actionFunc = FUN_003e85ec;
        } else {
            FUN_00373264(&self->dyna.actor, 0x01000210);
        }
    } else {
        self->timer = 60;
        self->actionFunc = FUN_003e85ec;
    }
    BgHidanFslift_SetHookshotTargetPos(self);
}
}
