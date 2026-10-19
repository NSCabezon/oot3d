extern "C" {
#include "z_bg_mjin.hpp"

#define FLAGS 0x80400010

void BgMjin_Init(Actor* actor, GameState* state);
void BgMjin_Destroy(Actor* actor, GameState* state);
void BgMjin_Update(Actor* actor, GameState* state);

void FUN_0015b3cc(BgMjin* self, GameState* state); // N64: func_808A0850
void FUN_003f150c(BgMjin* self, PlayState* play);  // N64: BgMjin_DoNothing
void FUN_001ec57c(Actor* actor, GameState* state); // N64: BgMjin_Draw

s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
s32 Object_IsLoaded(ObjectContext* objectCtx, s32 bankIndex);
void FUN_003586ec(SkeletonAnimationModel_unk_0C* arg0);

ActorInit Bg_Mjin_InitVars = {
    ACTOR_BG_MJIN,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(BgMjin),
    (ActorFunc)BgMjin_Init,
    (ActorFunc)BgMjin_Destroy,
    (ActorFunc)BgMjin_Update,
    NULL,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 400, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 400, ICHAIN_STOP),
};

void BgMjin_Init(Actor* actor, GameState* state) {
    BgMjin* self = (BgMjin*)actor;
    PlayState* play = (PlayState*)state;
    s8 objBankIndex;

    Actor_ProcessInitChain(actor, sInitChain);
    objBankIndex = Object_GetIndex(&play->objectCtx, (actor->params != 0 ? OBJECT_MJIN : OBJECT_MJIN_OKA));
    self->requiredObjectSlot = objBankIndex;
    if (objBankIndex < 0) {
        Actor_Kill(actor);
    } else {
        self->actionFunc = (BgMjinActionFunc)FUN_0015b3cc;
    }
}

void BgMjin_Destroy(Actor* actor, GameState* state) {
    BgMjin* self = (BgMjin*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    if (self->skelAnimModel != NULL) {
        FUN_00350f34(&self->dyna.actor, &self->skelAnimModel, 0);
    }
}

GLOBAL_ASM("asm/FUN_0015b3cc.s")

// NON_MATCHING attempt for FUN_0015b3cc. Two scheduling diffs remain:
//  - after GetCMABByIndex, armcc hoists the unk_0C load into r2 (orig: "mov r1, r0" then load into r0);
//  - the params == 1 flag clear is conditionalized (ldreq/biceq/streq), orig uses a bne branch.
// Logic is believed equivalent. To build it, it also needs:
//  - Actor_Noop compiled from C (armlink --inline turns a bl to an empty C function into "mov r0, r0";
//    N64 Actor_SetObjectDependency(play, actor) is an empty function on 3DS);
//  - the absolute symbol gUnk_0054C8CC (0x0054C8CC has no data symbol; a constant address makes
//    armcc split it into 0x54C000 + 0x8CC), defined in src/game/shared_syms.s.
#if 0
#define Actor_SetObjectDependency(play, actor) ((void (*)(PlayState*, Actor*))Actor_Noop)(play, actor)

extern s32 gUnk_0054C8CC;

typedef struct {
    f32 frames[6];
} BgMjinFrames; // copied from rodata 0x004D7194

static inline void BgMjin_SetFrame(SkeletonAnimationModel_unk_0C* anim, f32 frame) {
    if (gUnk_0054C8CC == 0) {
        *(f32*)&anim->curFrame = frame;
        FUN_003586ec(anim);
    }
}

void FUN_0015b3cc(BgMjin* self, GameState* state) {
    PlayState* play = (PlayState*)state;
    ZARInfo* zarInfo;
    void* zsi;
    CollisionHeader* colHeader;

    if (Object_IsLoaded(&play->objectCtx, self->requiredObjectSlot)) {
        colHeader = NULL;
        self->dyna.actor.flags &= ~0x10;
        self->dyna.actor.objBankIndex = self->requiredObjectSlot;
        Actor_SetObjectDependency(play, &self->dyna.actor);
        DynaPolyActor_Init(&self->dyna, 0);
        if (self->dyna.actor.params != 0) {
            zarInfo = FUN_00372f38(&self->dyna.actor, play, &self->skelAnimModel, 0, 0);
            self->skelAnimModel->unk_0C->FUN_00372d94(zarInfo->GetCMABByIndex(0));
            BgMjinFrames frames = *(BgMjinFrames*)0x004d7194;
            self->skelAnimModel->unk_0C->SetAnimSpeed(0.0f);
            BgMjin_SetFrame(self->skelAnimModel->unk_0C, frames.frames[self->dyna.actor.params - 1]);
            zsi = zarInfo->FUN_003532c0(0);
            if (self->dyna.actor.params == 1) {
                u32 flags = self->dyna.actor.flags & ~0x80000000;
                self->dyna.actor.flags = flags;
            }
        } else if ((u8)self->dyna.actor.objBankIndex == OBJECT_MJIN_OKA) {
            // 3DS compares objBankIndex (not the object id) against OBJECT_MJIN_OKA here
            zarInfo = FUN_00372f38(&self->dyna.actor, play, &self->skelAnimModel, 0, 0);
            zsi = zarInfo->FUN_003532c0(0);
        }
        ReadCollisionHeaderFromZsi(zsi, &colHeader);
        self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);
        self->actionFunc = FUN_003f150c;
        self->dyna.actor.draw = FUN_001ec57c;
    }
}
#endif

void FUN_003f150c(BgMjin* self, PlayState* play) {
}

void BgMjin_Update(Actor* actor, GameState* state) {
    BgMjin* self = (BgMjin*)actor;
    PlayState* play = (PlayState*)state;

    self->actionFunc(self, play);
}

void FUN_001ec57c(Actor* actor, GameState* state) {
    BgMjin* self = (BgMjin*)actor;

    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&actor->modelMtx);
    self->skelAnimModel->FUN_00372170(0);
}
}
