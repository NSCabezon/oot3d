extern "C" {
#include "z_obj_elevator.hpp"

#define FLAGS 0x80400000

void ObjElevator_Init(Actor* actor, GameState* state);
void ObjElevator_Destroy(Actor* actor, GameState* state);
void ObjElevator_Update(Actor* actor, GameState* state);
void ObjElevator_Draw(Actor* actor, GameState* state);

void FUN_003bef24(ObjElevator* self, PlayState* play); // N64: func_80B92C80 (wait for player on top)
void FUN_003f70f8(ObjElevator* self, PlayState* play); // N64: func_80B92D44 (move)
__attribute__((noinline)) void FUN_00144618(ObjElevator* self); // N64: func_80B92C5C (set wait action)

ActorInit Obj_Elevator_InitVars = {
    ACTOR_OBJ_ELEVATOR,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_D_ELEVATOR,
    sizeof(ObjElevator),
    (ActorFunc)ObjElevator_Init,
    (ActorFunc)ObjElevator_Destroy,
    (ActorFunc)ObjElevator_Update,
    (ActorFunc)ObjElevator_Draw,
};

static f32 sScales[] = { 0.1f, 0.05f };

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 2000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 600, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 2000, ICHAIN_STOP),
};

void ObjElevator_Init(Actor* actor, GameState* state) {
    ObjElevator* self = (ObjElevator*)actor;
    PlayState* play = (PlayState*)state;
    f32 temp;

    FUN_00372f38(actor, play, &self->skelAnimModel, 0, 0);
    {
        void* info = DynaPolyInfo_Alloc(&self->dyna, state, 0);
        CollisionHeader* colHeader = NULL;

        DynaPolyActor_Init(&self->dyna, 1);
        ReadCollisionHeaderFromZsi(info, &colHeader);
        self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    }
    Actor_SetScale(actor, sScales[(actor->params >> 0) & 1]);
    Actor_ProcessInitChain(actor, sInitChain);
    temp = (f32)((actor->params >> 8) & 0xF);
    self->unk_1C4 = temp * 2.0f;
    self->actionFunc = FUN_003bef24;
}

void ObjElevator_Destroy(Actor* actor, GameState* state) {
    ObjElevator* self = (ObjElevator*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void FUN_00144618(ObjElevator* self) {
    self->actionFunc = FUN_003bef24;
}

void FUN_003bef24(ObjElevator* self, PlayState* play) {
    f32 diff;

    if ((self->dyna.unk_1B8 & 2) && !(self->unk_1C8 & 2)) {
        diff = self->dyna.actor.world.pos.y - self->dyna.actor.home.pos.y;
        if (__fabsf(diff) < 0.1f) {
            self->unk_1C0 = self->dyna.actor.home.pos.y + (f32)((self->dyna.actor.params >> 12) & 0xF) * 80.0f;
        } else {
            self->unk_1C0 = self->dyna.actor.home.pos.y;
        }
        self->actionFunc = FUN_003f70f8;
    }
}

void FUN_003f70f8(ObjElevator* self, PlayState* play) {
    Actor* actor = &self->dyna.actor;

    if (__fabsf(Math_SmoothStepToF(&actor->world.pos.y, self->unk_1C0, 1.0f, self->unk_1C4, 0.0f)) < 0.001f) {
        Audio_PlayActorSound2(actor, 0x0100016C); // NA_SE_EV_FOOT_SWITCH
        FUN_00144618(self);
    } else {
        Audio_PlayActorSound2(actor, 0x01000175); // NA_SE_EV_STONE_STATUE_OPEN - SFX_FLAG
    }
}

void ObjElevator_Update(Actor* actor, GameState* state) {
    ObjElevator* self = (ObjElevator*)actor;

    if (self->actionFunc != NULL) {
        self->actionFunc(self, (PlayState*)state);
    }
    self->unk_1C8 = self->dyna.unk_1B8;
}

void ObjElevator_Draw(Actor* actor, GameState* state) {
    ObjElevator* self = (ObjElevator*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&modelMtx);
    self->skelAnimModel->FUN_00372170(0);
}
}
