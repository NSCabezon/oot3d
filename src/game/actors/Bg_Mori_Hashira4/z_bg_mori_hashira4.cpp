extern "C" {
#include "z_bg_mori_hashira4.hpp"

#define FLAGS 0x00000010
#define PARAMS_GET_U(params, start, count) (((params) >> (start)) & ((1 << (count)) - 1))

s32 Object_IsLoaded(ObjectContext* objectCtx, s32 bankIndex);
s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
s32 OnePointCutscene_Init(PlayState* play, s32 csId, s32 arg2, Actor* actor, s32 camId);

void BgMoriHashira4_Init(Actor* thisx, PlayState* play);
void BgMoriHashira4_Destroy(Actor* thisx, PlayState* play);
void BgMoriHashira4_Update(Actor* thisx, PlayState* play);
void FUN_0037ac08(Actor* thisx, PlayState* play);

// Action functions keep their ld-pinned FUN_ names (N64: WaitForMoriTex, GateWait, GateOpen, Draw)
void FUN_00110830(BgMoriHashira4* self, PlayState* play);
void FUN_0012b610(BgMoriHashira4* self, PlayState* play);
void FUN_0012b5d0(BgMoriHashira4* self, PlayState* play);
void FUN_003bf92c(BgMoriHashira4* self, PlayState* play);

// sUnkTimer (0x00513314, u16) and sInitChain (0x00513318) live in the data dump below. They have no
// symbols in the scatter file, so they are defined as absolute symbols (EQU) in data_syms.s.
extern u16 BgMoriHashira4_sUnkTimer;
extern InitChainEntry BgMoriHashira4_sInitChain[];
#define sUnkTimer BgMoriHashira4_sUnkTimer
#define sInitChain BgMoriHashira4_sInitChain

GLOBAL_ASM("data/z_Bg_Mori_Hashira4.data.s")
GLOBAL_ASM("../src/game/actors/Bg_Mori_Hashira4/data_syms.s")

ActorInit Bg_Mori_Hashira4_InitVars = {
    ACTOR_BG_MORI_HASHIRA4,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_MORI_OBJECTS,
    sizeof(BgMoriHashira4),
    (ActorFunc)BgMoriHashira4_Init,
    (ActorFunc)BgMoriHashira4_Destroy,
    (ActorFunc)BgMoriHashira4_Update,
    NULL,
};


static void BgMoriHashira4_InitDynaPoly(BgMoriHashira4* self, PlayState* play, void* zsi, s32 moveFlag) {
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, moveFlag);
    ReadCollisionHeaderFromZsi(zsi, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);
}

void BgMoriHashira4_Init(Actor* thisx, PlayState* play) {
    BgMoriHashira4* self = (BgMoriHashira4*)thisx;

    self->switchFlag = PARAMS_GET_U(thisx->params, 8, 6);
    thisx->params &= 0xFF;

    FUN_00372f38(thisx, play, self->skelAnimModel, 0, &self->skelAnimModel[1], 8, 0);

    if (thisx->params == 0) {
        BgMoriHashira4_InitDynaPoly(self, play, DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 0), 3);
    } else {
        BgMoriHashira4_InitDynaPoly(self, play, DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 6), 0);
    }
    Actor_ProcessInitChain(thisx, sInitChain);
    self->moriTexObjectSlot = Object_GetIndex(&play->objectCtx, OBJECT_MORI_TEX);
    if (self->moriTexObjectSlot < 0) {
        Actor_Kill(thisx);
        return;
    }
    if ((thisx->params != 0) && Flags_GetSwitch(play, self->switchFlag)) {
        Actor_Kill(thisx);
        return;
    }
    Actor_SetFocus(thisx, 50.0f);
    self->actionFunc = FUN_00110830;
    sUnkTimer = 0;
}

void BgMoriHashira4_Destroy(Actor* thisx, PlayState* play) {
    BgMoriHashira4* self = (BgMoriHashira4*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(thisx, &self->skelAnimModel[0], &self->skelAnimModel[1], 0);
}

// N64: BgMoriHashira4_WaitForMoriTex
void FUN_00110830(BgMoriHashira4* self, PlayState* play) {
    if (Object_IsLoaded(&play->objectCtx, self->moriTexObjectSlot)) {
        self->gateTimer = 0;
        if (self->dyna.actor.params == 0) {
            self->actionFunc = FUN_003bf92c;
        } else {
            self->actionFunc = FUN_0012b610;
        }
        self->dyna.actor.draw = (ActorFunc)FUN_0037ac08;
    }
}

// N64: BgMoriHashira4_PillarsRotate
void FUN_003bf92c(BgMoriHashira4* self, PlayState* play) {
    self->dyna.actor.shape.rot.y = self->dyna.actor.world.rot.y += 0x64;
    Audio_PlayActorSound2(&self->dyna.actor, 0x010001D9);
}

// N64: BgMoriHashira4_GateWait
void FUN_0012b610(BgMoriHashira4* self, PlayState* play) {
    if (Flags_GetSwitch(play, self->switchFlag) || (self->gateTimer != 0)) {
        self->gateTimer++;
        if (self->gateTimer > 45) {
            Audio_PlayActorSound2(&self->dyna.actor, 0x010001B0);
            self->actionFunc = FUN_0012b5d0;
            OnePointCutscene_Init(play, 0x177A, 20, &self->dyna.actor, 0);
            sUnkTimer++;
        }
    }
}

// N64: BgMoriHashira4_GateOpen
void FUN_0012b5d0(BgMoriHashira4* self, PlayState* play) {
    if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y + 120.0f, 10.0f)) {
        Actor_Kill(&self->dyna.actor);
    }
}

void BgMoriHashira4_Update(Actor* thisx, PlayState* play) {
    BgMoriHashira4* self = (BgMoriHashira4*)thisx;

    if (self->actionFunc != NULL) {
        self->actionFunc(self, play);
    }
}

// Model draw (3DS replacement of the N64 display-list draw)
void FUN_0037ac08(Actor* thisx, PlayState* play) {
    MTX34 mtx;
    u8* slot;
    MTX34CopyAsm(&mtx, &thisx->modelMtx);
    slot = (u8*)thisx + (thisx->params << 2);
    (*(SkeletonAnimationModel**)(slot + 0x1C4))->SetUnkAC(1);
    (*(SkeletonAnimationModel**)(slot + 0x1C4))->FUN_003721e0(&mtx);
    (*(SkeletonAnimationModel**)(slot + 0x1C4))->FUN_00372170(0);
}
}
