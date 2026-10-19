extern "C" {
#include "z_door_gerudo.hpp"

#define FLAGS 0x80400000

// Actor-internal MTX34 at 0x148 (field not named in Actor yet)
#define DOOR_GERUDO_MTX(self) ((MTX34*)((u8*)(self) + 0x148))

void FUN_00342968(void* unk);
void FUN_0034291c(void* unk);
void FUN_00353214(void* unk, Actor* actor, PlayState* play, s32 arg3);
void FUN_0033a05c(PlayState* play, s32 lockType, MTX34* mtx, void* unk);
void FUN_003f3fa0(DoorGerudo* self, PlayState* play);

static inline void DoorGerudo_ScaleMtx(MTX34* mtx, f32 x, f32 y, f32 z) {
    mtx->data[0][0] *= x;
    mtx->data[1][0] *= x;
    mtx->data[2][0] *= x;
    mtx->data[0][1] *= y;
    mtx->data[1][1] *= y;
    mtx->data[2][1] *= y;
    mtx->data[0][2] *= z;
    mtx->data[1][2] *= z;
    mtx->data[2][2] *= z;
}

void DoorGerudo_Init(Actor* actor, PlayState* play);
void DoorGerudo_Destroy(Actor* actor, PlayState* play);
void DoorGerudo_Update(Actor* actor, PlayState* play);
void DoorGerudo_Draw(Actor* actor, PlayState* play);

ActorInit Door_Gerudo_InitVars = {
    ACTOR_DOOR_GERUDO,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_DOOR_GERUDO,
    sizeof(DoorGerudo),
    (ActorFunc)DoorGerudo_Init,
    (ActorFunc)DoorGerudo_Destroy,
    (ActorFunc)DoorGerudo_Update,
    (ActorFunc)DoorGerudo_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F(scale, 1, ICHAIN_STOP),
};

void DoorGerudo_Init(Actor* actor, PlayState* play) {
    DoorGerudo* self = (DoorGerudo*)actor;
    CollisionHeader* colHeader = NULL;
    DoorGerudoLock* lock;

    Actor_ProcessInitChain(actor, sInitChain);
    ReadCollisionHeaderFromZsi(FUN_00372f38(actor, play, &self->skelAnimModel, 0, 0)->FUN_003532c0(0), &colHeader);
    // Placement-style construction of the lock sub-object (null-checked, as in EnDoor/DoorShutter Init)
    lock = &self->lock;
    if (lock != NULL) {
        FUN_00342968(lock);
    }
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);

    if (Flags_GetSwitch(play, self->dyna.actor.params & 0x3F)) {
        self->actionFunc = FUN_003f3fa0;
        self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y + 200.0f;
    } else {
        self->actionFunc = FUN_003f3fa0;
        self->unk_1BE = 15; // N64: 10
        FUN_00353214(&self->lock, actor, play, 0);
    }
}

void DoorGerudo_Destroy(Actor* actor, PlayState* play) {
    DoorGerudo* self = (DoorGerudo*)actor;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    if (self->skelAnimModel != NULL) {
        FUN_003508b8(actor, self->skelAnimModel, 0);
    }
    FUN_0034291c(&self->lock);
}

void DoorGerudo_Update(Actor* actor, PlayState* play) {
    DoorGerudo* self = (DoorGerudo*)actor;

    self->actionFunc(self, play);
}

void DoorGerudo_Draw(Actor* actor, PlayState* play) {
    DoorGerudo* self = (DoorGerudo*)actor;
    MTX34 mtx;

    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(DOOR_GERUDO_MTX(self));
    self->skelAnimModel->FUN_00372170(0);

    if (self->unk_1BE != 0) {
        MTX34CopyAsm(&mtx, DOOR_GERUDO_MTX(self));
        DoorGerudo_ScaleMtx(&mtx, 0.01f, 0.01f, 0.01f);
        FUN_0033a05c(play, self->unk_1BE, &mtx, &self->lock);
    }
}

}
