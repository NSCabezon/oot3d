extern "C" {
#include "z_bg_zg.hpp"

#define FLAGS 0x80000010

// Data dump (data/z_Bg_Zg.data.s): sInitChain 0x0051437C, sDrawFuncs 0x00514380, sActionFuncs 0x00514384.
// sActionFuncs = { caseD_0_001a6260 (action 0), 0x00188CC8 (action 1, raw code inside FUN_00188ad4.s) }.
extern InitChainEntry BgZg_sInitChain[];
extern BgZgDrawFunc BgZg_sDrawFuncs[];
extern BgZgActionFunc BgZg_sActionFuncs[];
#define sInitChain BgZg_sInitChain
#define sDrawFuncs BgZg_sDrawFuncs
#define sActionFuncs BgZg_sActionFuncs

void BgZg_Init(Actor* actor, GameState* state);
void BgZg_Destroy(Actor* actor, GameState* state);
void BgZg_Update(Actor* actor, GameState* state);
void BgZg_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Bg_Zg.data.s")
GLOBAL_ASM("../src/game/actors/Bg_Zg/BgZg_data.s")

ActorInit Bg_Zg_InitVars = {
    ACTOR_BG_ZG,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_ZG,
    sizeof(BgZg),
    (ActorFunc)BgZg_Init,
    (ActorFunc)BgZg_Destroy,
    (ActorFunc)BgZg_Update,
    (ActorFunc)BgZg_Draw,
};

// Unmatched: GLOBAL_ASM below. Draft (Opus closing pass) is 4 words off: only the register pair in the
// type check (orig `ldrsh r1 / and r0, r1` and `andne r0, r1`; C gives `ldrsh r0 / and r1, r0`). Stack layout,
// colHeader slot ([sp,#4], late `colHeader = NULL`) and the armlink-inlined ReadCollisionHeaderFromZsi match.
// Same register quirk as caseD_0_001a6260 below. ~20 variants tried (signatures, helper bodies/types/order).
#if 0
#define PARAMS_GET_U(params, start, count) (((params) >> (start)) & ((1 << (count)) - 1))

static s32 BgZg_GetSwitchFlag(BgZg* self, PlayState* play) { // N64: func_808C0C98
    s32 flag = PARAMS_GET_U(self->dyna.actor.params, 8, 8);

    return Flags_GetSwitch(play, flag);
}

static s32 BgZg_GetType(BgZg* self) { // N64: func_808C0CC8
    s32 flag = PARAMS_GET_U(self->dyna.actor.params, 0, 8);

    return flag;
}

void BgZg_Init(Actor* actor, GameState* state) {
    BgZg* self = (BgZg*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader;

    Actor_ProcessInitChain(actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, 0);
    actor->unk_19A = 1;
    FUN_00372f38(actor, play, &self->skelAnimModel, 0, 0);
    colHeader = NULL;
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 0), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    if (BgZg_GetType(self) == 8 || BgZg_GetType(self) == 9) {
        actor->scale.x *= 1.3f;
        actor->scale.z *= 1.3f;
        actor->scale.y *= 1.2f;
    }
    self->action = 0;
    self->drawConfig = 0;
    if (BgZg_GetSwitchFlag(self, play)) {
        Actor_Kill(actor);
    }
}
#endif

GLOBAL_ASM("asm/BgZg_Init.s")

void BgZg_Destroy(Actor* actor, GameState* state) {
    BgZg* self = (BgZg*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgZg_Update(Actor* actor, GameState* state) {
    BgZg* self = (BgZg*)actor;

    if ((u32)self->action < 2 && sActionFuncs[self->action] != NULL) {
        sActionFuncs[self->action](self, (PlayState*)state);
    }
}
GLOBAL_ASM("../split/asm/FUN_0010036c.s")

void BgZg_DrawFn(BgZg* self, PlayState* play) {
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->dyna.actor.modelMtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}

// Matched once sDrawFuncs became a real (EQU) symbol: with a raw-address cast armcc folded the base into a
// 0x514000 literal + [#0x380].
void BgZg_Draw(Actor* actor, GameState* state) {
    BgZg* self = (BgZg*)actor;
    PlayState* play = (PlayState*)state;

    if (self->drawConfig == 0 && sDrawFuncs[0] != NULL) {
        sDrawFuncs[0](self, play);
    }
}

// Draft for action 0 (caseD_0_001a6260 = N64 func_808C0CD4, still GLOBAL_ASM in functions_180000s.cpp): 3 words
// differ, only the register of the params ldrh/lsl temp (orig r2, C r1). Opus pass tried inlined N64 helpers
// (func_808C0C98 / func_808C0C50), helpers defined after use, u16/u32/s32 locals, casts, GameState/Actor* sigs.
#if 0
static void BgZg_PlayOpenSfx(BgZg* self) { // N64: func_808C0C50
    Audio_PlaySoundGeneral(0x010001B0, &self->dyna.actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                           &DAT_0054ac24.unk_0);
}

void caseD_0_001a6260(BgZg* self, PlayState* play) {
    if (BgZg_GetSwitchFlag(self, play) != 0) {
        self->action = 1;
        BgZg_PlayOpenSfx(self);
    }
}
#endif

// Draft for action 1 (0x00188CC8, raw code inside FUN_00188ad4.s; needs a cfg split): N64 func_808C0D08.
#if 0
void BgZg_FallAway(BgZg* self, PlayState* play) {
    self->dyna.actor.world.pos.y += (kREG(16) + 20.0f) * 1.2f;
    if ((((kREG(17) + 200.0f) * 1.2f) + self->dyna.actor.home.pos.y) <= self->dyna.actor.world.pos.y) {
        Actor_Kill(&self->dyna.actor);
    }
}
#endif

}
