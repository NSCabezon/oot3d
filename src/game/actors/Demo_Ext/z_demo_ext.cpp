extern "C" {
#include "z_demo_ext.hpp"

#define FLAGS 0x80000010

// REG group 27 on 3DS (N64 kREG(r)): kREG(16), kREG(18), kREG(19..21), kREG(28), kREG(32)
#define DEMO_EXT_KREG(r) (gGameInfo->data[27 * REG_PER_GROUP + (r)])

void DemoExt_Init(Actor* actor, GameState* state);
void DemoExt_Destroy(Actor* actor, GameState* state);
void DemoExt_Update(Actor* actor, GameState* state);
void DemoExt_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Demo_Ext.data.s")

// Dispatch tables live in the data dump above (0x51B93C: draw {DrawNothing, DrawVortex},
// 0x51B944: update {Wait, Maintain, Dispell}); no scatter-file symbols, so they are EQU in data_syms.s.
GLOBAL_ASM("../src/game/actors/Demo_Ext/data_syms.s")
extern DemoExtFunc DemoExt_sDrawFuncs[];
extern DemoExtFunc DemoExt_sUpdateFuncs[];

ActorInit Demo_Ext_InitVars = {
    ACTOR_DEMO_EXT,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_FHG,
    sizeof(DemoExt),
    (ActorFunc)DemoExt_Init,
    (ActorFunc)DemoExt_Destroy,
    (ActorFunc)DemoExt_Update,
    (ActorFunc)DemoExt_Draw,
};

void DemoExt_Init(Actor* actor, GameState* state) {
    DemoExt* self = (DemoExt*)actor;
    s16* scrollIncr = self->scrollIncr;
    Vec3f* scale = &self->scale;

    scrollIncr[0] = 25;
    scrollIncr[1] = 40;
    scrollIncr[2] = 5;
    scrollIncr[3] = 30;
    self->primAlpha = DEMO_EXT_KREG(28) + 255;
    self->envAlpha = DEMO_EXT_KREG(32) + 255;
    scale->x = DEMO_EXT_KREG(19) + 400.0f;
    scale->y = DEMO_EXT_KREG(20) + 100.0f;
    scale->z = DEMO_EXT_KREG(21) + 400.0f;

    ZARInfo* zarInfo = FUN_00372f38(actor, (PlayState*)state, &self->skelAnimModel, 1, 0);
    self->skelAnimModelUnk0C = self->skelAnimModel->unk_0C;
    self->skelAnimModelUnk0C->FUN_00372d94(zarInfo->GetCMABByIndex(1));
    self->skelAnimModelUnk0C->SetAnimType(1);
}

// Destroy: C matches the first 12 B, but ROM span is 24 B (3x bx lr padding) and the
// padding can't be emitted from C. Kept as asm; C draft below. (Haiku pass, see STATUS.md)
// Opus pass: the 3 trailing `bx lr` words (0x1d21c8..) are most likely 3 unlabeled empty functions merged
// into this cfg span; matching needs a functions.tsv split (out of scope).
GLOBAL_ASM("../split/asm/FUN_001d21c8.s")

void DemoExt_Destroy(Actor* actor, GameState* state) {
    DemoExt* self = (DemoExt*)actor;

    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

// Update: ROM span is 212 B: 40 B dispatcher followed by an unlabeled 172 B blob merged by the cfg
// (functions.tsv). C can only emit the 40 B dispatcher and bincombined then shifts all later code.
GLOBAL_ASM("../split/asm/FUN_001493f0.s")

void DemoExt_Update(Actor* actor, GameState* state) {
    DemoExt* self = (DemoExt*)actor;
    DemoExtFunc* table;
    DemoExtFunc func;

    if ((u32)self->action < 3) {
        table = DemoExt_sUpdateFuncs;
        func = table[self->action];
        if (func != NULL) {
            func(actor, state);
        }
    }
}

void DemoExt_Draw(Actor* actor, GameState* state) {
    DemoExt* self = (DemoExt*)actor;
    DemoExtFunc* table;
    DemoExtFunc func;

    if ((u32)self->drawMode < 2) {
        table = DemoExt_sDrawFuncs;
        func = table[self->drawMode];
        if (func != NULL) {
            func(actor, state);
        }
    }
}

// ---- Unmatched action functions: GLOBAL_ASM in src/functions/functions_*.cpp; drafts below (not compiled) ----
// N64 names: EXT_WAIT / EXT_MAINTAIN / EXT_DISPELL. Offsets are 3DS (N64 + 0x58 for Actor-based fields).
#if 0
#define DEMO_EXT_CUE(play) (*(CsCmdActorAction**)((u8*)(play) + 0x2298 + 0x58))

// ---- Opus closing pass (2026-10-13): N64-structured drafts, best fastchk results ----
// Wait caseD_0_001f0880: 30/55 words, built 212 B vs 220 B. Frame (incl. sl = 8-byte-align pad), csCtx base
// (play+0x2298) in a callee-saved reg and the shared NULL/0 constant all match now. Remaining: ROM copies
// `self` into r1 (`mov r1, r4`) right after the 2nd GetCutsceneState and stores action/drawMode for cue 2
// via r1 (no tail-merge with cue 3's drawMode store); that shifts the reg allocation (ROM: r7=1, r8=play,
// sb=csCtx; ours r8=1, sb=play, r7=csCtx). Tried: Actor*/GameState* entry, param order swaps, Actor* alias,
// Vec3f* alias, pos-from-cue helper, explicit default case, NULL-init/if/ternary GetCue (ternary = 48 words).
// Maintain caseD_1_00245094: same HandleCues tail (same mov r1,r4) + extra float/scroll reg swaps: 54/107.
// Dispell caseD_2_001f0678: 10/130 with this draft (scrollIncr declared before curScroll). Left: the two
// scroll-pointer adds/loads are scheduled in swapped order (4 words), primAlpha/envAlpha results go to r2
// instead of r3 (4 words), and the FinishClosing compare is `vcmpe s1,s0; bhi` instead of ROM
// `vcmpe s0,s1; blt` (2 words). Every compare form that fixes the last (`>=`, `!(<)`) and the curScroll-first
// decl order flips the gGameInfo/self+0x100 registers instead (20..28 words). ~45 variants incl. a 36-combo grid.
static CsCmdActorAction* DemoExt_GetCue(PlayState* play, s32 cueChannel) {
    CutsceneContext* csCtx = &play->csCtx;
    if (play->GetCutsceneState() != 0) {
        return csCtx->npcActions[cueChannel];
    }
    return NULL;
}

static void DemoExt_SetupWait(DemoExt* self) {
    self->action = 0;
    self->drawMode = 0;
}

static void DemoExt_SetupMaintainVortex(DemoExt* self, PlayState* play) {
    CsCmdActorAction* cue = DemoExt_GetCue(play, 5);

    if (cue != NULL) {
        self->actor.world.pos.x = cue->startPos.x;
        self->actor.world.pos.y = cue->startPos.y;
        self->actor.world.pos.z = cue->startPos.z;
        self->actor.world.rot.y = self->actor.shape.rot.y = cue->rot.y;
    }
    self->action = 1;
    self->drawMode = 1;
}

static void DemoExt_SetupDispellVortex(DemoExt* self) {
    self->action = 2;
    self->drawMode = 1;
}

static void DemoExt_HandleCues(DemoExt* self, PlayState* play) {
    CsCmdActorAction* cue = DemoExt_GetCue(play, 5);
    s32 nextCueId;
    s32 currentCueId;

    if (cue != NULL) {
        nextCueId = cue->action;
        currentCueId = self->cueId;

        if (nextCueId != currentCueId) {
            switch (nextCueId) {
                case 1:
                    DemoExt_SetupWait(self);
                    break;
                case 2:
                    DemoExt_SetupMaintainVortex(self, play);
                    break;
                case 3:
                    DemoExt_SetupDispellVortex(self);
                    break;
            }
            self->cueId = nextCueId;
        }
    }
}

static void DemoExt_PlayVortexSFX(DemoExt* self) {
    if (self->alphaTimer <= (DEMO_EXT_KREG(35) + 40.0f) - 15.0f) {
        Audio_PlaySoundGeneral(0x01000197, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               &DAT_0054ac24.unk_0); // NA_SE_EV_FANTOM_WARP_L - SFX_FLAG
    }
}

static void DemoExt_SetScrollAndRotation(DemoExt* self) {
    s16* scrollIncr = self->scrollIncr; // decl order matters: curScroll first -> 26 words (r0/r1 swap)
    s16* curScroll = self->curScroll;
    s32 i;

    for (i = 3; i != 0; i--) { // index 0 is never scrolled (same as N64)
        curScroll[i] += scrollIncr[i];
    }
    self->rotationPitch += (s16)(DEMO_EXT_KREG(34) + 1000);
}

static void DemoExt_SetColorsAndScales(DemoExt* self) {
    Vec3f* scale = &self->scale;
    f32 shrinkFactor;

    shrinkFactor = ((DEMO_EXT_KREG(35) + 40.0f) - self->alphaTimer) / (DEMO_EXT_KREG(35) + 40.0f);
    if (shrinkFactor < 0.0f) {
        shrinkFactor = 0.0f;
    }

    self->primAlpha = (u32)(DEMO_EXT_KREG(28) + 255) * shrinkFactor;
    self->envAlpha = (u32)(DEMO_EXT_KREG(32) + 255) * shrinkFactor;
    scale->x = (DEMO_EXT_KREG(19) + 400.0f) * shrinkFactor;
    scale->y = (DEMO_EXT_KREG(20) + 100.0f) * shrinkFactor;
    scale->z = (DEMO_EXT_KREG(21) + 400.0f) * shrinkFactor;
}

static void DemoExt_FinishClosing(DemoExt* self) {
    self->alphaTimer += 1.0f;
    if ((DEMO_EXT_KREG(35) + 40.0f) <= self->alphaTimer) {
        Actor_Kill(&self->actor);
    }
}

// EXT_WAIT
void caseD_0_001f0880(DemoExt* self, PlayState* play) {
    DemoExt_HandleCues(self, play);
}

// EXT_MAINTAIN
void caseD_1_00245094(DemoExt* self, PlayState* play) {
    DemoExt_PlayVortexSFX(self);
    DemoExt_SetScrollAndRotation(self);
    DemoExt_HandleCues(self, play);
}

// EXT_DISPELL
void caseD_2_001f0678(DemoExt* self, PlayState* play) {
    DemoExt_PlayVortexSFX(self);
    DemoExt_SetScrollAndRotation(self);
    DemoExt_SetColorsAndScales(self);
    DemoExt_FinishClosing(self);
}

// Vortex draw: DemoExt_DrawVortex (N64 lines 460-512). asm: caseD_1_002247f8 (812 B). Draft only.
// Matrix_* / Gfx_* helpers are the 3DS equivalents (FUN_ names not yet identified); MTX34 copy of modelMtx first.
void caseD_1_002247f8(Actor* actor, GameState* state) {
    DemoExt* self = (DemoExt*)actor;
    PlayState* play = (PlayState*)state;
    Mtx* mtx;
    GraphicsContext* gfxCtx;

    // N64 body: Matrix_Push; Matrix_Scale(scale); Matrix_RotateZYX(kREG16+0x4000, rotationPitch, kREG18);
    // Matrix_Translate(kREG22..24); MATRIX_TO_MTX; Matrix_Pop; Gfx_SetupDL_25Xlu;
    // gDPSetPrimColor(..., kREG33+128, kREG25+140, kREG26+80, kREG27+140, primAlpha);
    // gDPSetEnvColor(..., kREG29+90, kREG30+50, kREG31+95, envAlpha);
    // gSPSegment(0x08, Gfx_TwoTexScroll(...curScroll...)); gSPMatrix; gSPDisplayList(gPhantomWarpDL); gSPPopMatrix.
    (void)play; (void)mtx; (void)gfxCtx;
}
#endif
}
