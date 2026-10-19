extern "C" {
#include "z_demo_tre_lgt.hpp"

#define FLAGS 0x00000010

void DemoTreLgt_Init(Actor* actor, GameState* state);
void DemoTreLgt_Destroy(Actor* actor, GameState* state);
void DemoTreLgt_Update(Actor* actor, GameState* state);
void DemoTreLgt_Draw(Actor* actor, GameState* state);

// 3DS-only helpers (declared locally, see AGENT_BRIEF)
void* ZAR_GetCMBByIndex(ZARInfo* zar, s32 index);
void FUN_003357dc(void* skelCurve);
void FUN_00337200(void* skelCurve, PlayState* play, void* cmb, s32 arg3, s32 arg4, void* arg5, s32 arg6);

GLOBAL_ASM("data/z_Demo_Tre_Lgt.data.s")

ActorInit Demo_Tre_Lgt_InitVars = {
    ACTOR_DEMO_TRE_LGT,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_BOX,
    sizeof(DemoTreLgt),
    (ActorFunc)DemoTreLgt_Init,
    (ActorFunc)DemoTreLgt_Destroy,
    (ActorFunc)DemoTreLgt_Update,
    (ActorFunc)DemoTreLgt_Draw,
};

#if 0
// Draft (Haiku): best C for Init, not byte-matching yet (see STATUS.md)
void DemoTreLgt_Init(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    DemoTreLgt* self = (DemoTreLgt*)actor;
    ObjectStatus* status;
    ZARInfo* zar;
    u8 bank;
    s32 unk178;

    bank = (u8)actor->objBankIndex;
    status = play->GetObjectStatus(bank);
    zar = &status->zarInfo;
    self->cmb = ZAR_GetCMBByIndex(zar, 0);
    FUN_003357dc(&self->unk_1A8);
    self->zarInfo = zar;
    actor->unk_19A = 1;
    unk178 = *(s32*)((u8*)actor + 0x178);
    FUN_00337200(&self->unk_1A8, play, self->cmb, unk178, *(s32*)0x0051C4C8, self->unk_1F0, 0x13);
    *((u8*)self->skelAnimModel + 0xAD) = 0;
    self->skelAnimModel->unk_0C->FUN_00372d94(zar->GetCMABByIndex(0));
    self->skelAnimModel->unk_0C->SetAnimType(1);
    self->unk_170 = 255.0f;
    self->unk_174 = 255.0f;
    self->status = 0;
    self->action = 0;
}

#endif

GLOBAL_ASM("asm/DemoTreLgt_Init.s")

#if 0
// Draft (Haiku): byte-identical in fastchk, but full link emits `b stub` where ROM has `mov r0,r0` (no call inlining)
void DemoTreLgt_Destroy(Actor* actor, GameState* state) {
    DemoTreLgt* self = (DemoTreLgt*)actor;

    ((void (*)(void*, PlayState*))GraphicsContext_UnknownStub)(&self->unk_1A8, (PlayState*)state);
}

#endif

GLOBAL_ASM("asm/DemoTreLgt_Destroy.s")

#if 0
// Draft (Haiku): compiles but emits call instead of asm tail call (bx r2)
void DemoTreLgt_Update(Actor* actor, GameState* state) {
    DemoTreLgt* self = (DemoTreLgt*)actor;
    DemoTreLgtActionFunc* actionFuncs = (DemoTreLgtActionFunc*)0x0051C4D0; // raw: table lives in z_Demo_Tre_Lgt.data
    actionFuncs[self->action](self, (PlayState*)state);
}

#endif

GLOBAL_ASM("asm/DemoTreLgt_Update.s")

GLOBAL_ASM("../split/asm/DemoTreLgt_Draw.s")
GLOBAL_ASM("../split/asm/FUN_002af310.s")
}
