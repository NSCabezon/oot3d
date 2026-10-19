extern "C" {
#include "z_bg_spot01_idohashira.hpp"

#define FLAGS 0x80400010

void BgSpot01Idohashira_Init(Actor* actor, GameState* state);
void BgSpot01Idohashira_Destroy(Actor* actor, GameState* state);
void BgSpot01Idohashira_Update(Actor* actor, GameState* state);
void BgSpot01Idohashira_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Bg_Spot01_Idohashira.data.s")

ActorInit Bg_Spot01_Idohashira_InitVars = {
    ACTOR_BG_SPOT01_IDOHASHIRA,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_SPOT01_OBJECTS,
    sizeof(BgSpot01Idohashira),
    (ActorFunc)BgSpot01Idohashira_Init,
    (ActorFunc)BgSpot01Idohashira_Destroy,
    (ActorFunc)BgSpot01Idohashira_Update,
    (ActorFunc)BgSpot01Idohashira_Draw,
};

// Tables live in the GLOBAL_ASM data block above. They have no symbols in the scatter file,
// so they are absolute symbols (EQU) in data_syms.s.
GLOBAL_ASM("../src/game/actors/Bg_Spot01_Idohashira/data_syms.s")
extern BgSpot01IdohashiraActionFunc BgSpot01Idohashira_sDrawFuncs[];
extern InitChainEntry BgSpot01Idohashira_sInitChain[];
extern BgSpot01IdohashiraActionFunc BgSpot01Idohashira_sActionFuncs[];
#define BG_SPOT01_IDOHASHIRA_INIT_CHAIN BgSpot01Idohashira_sInitChain
#define BG_SPOT01_IDOHASHIRA_DRAW_FUNCS BgSpot01Idohashira_sDrawFuncs
#define BG_SPOT01_IDOHASHIRA_ACTION_FUNCS BgSpot01Idohashira_sActionFuncs

void BgSpot01Idohashira_Init(Actor* actor, GameState* state) {
    BgSpot01Idohashira* self = (BgSpot01Idohashira*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader;

    Actor_ProcessInitChain(actor, BG_SPOT01_IDOHASHIRA_INIT_CHAIN);
    DynaPolyActor_Init(&self->dyna, 0);
    FUN_00372f38(actor, play, &self->skelAnimModel, 1, 0);
    colHeader = NULL;
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 0), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);

    if (gSaveContext.sceneSetupIndex < 4) {
        if ((gSaveContext.eventChkInf[5] & 0x10) && gSaveContext.linkAge == 0) {
            Actor_Kill(actor);
        } else {
            self->action = 0;
        }
    } else if (gSaveContext.sceneSetupIndex == 4) {
        self->action = 1;
        actor->shape.yOffset = -(kREG(10) + 1100.0f);
    } else if (gSaveContext.sceneSetupIndex == 6) {
        self->action = 0;
    } else {
        Actor_Kill(actor);
    }
}

void BgSpot01Idohashira_Destroy(Actor* actor, GameState* state) {
    BgSpot01Idohashira* self = (BgSpot01Idohashira*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgSpot01Idohashira_Update(Actor* actor, GameState* state) {
    BgSpot01Idohashira* self = (BgSpot01Idohashira*)actor;

    if ((u32)self->action < 4) {
        BgSpot01IdohashiraActionFunc* funcs = BG_SPOT01_IDOHASHIRA_ACTION_FUNCS;
        BgSpot01IdohashiraActionFunc func = funcs[self->action];

        if (func != NULL) {
            func(self, (PlayState*)state);
        }
    }
}

void BgSpot01Idohashira_Draw(Actor* actor, GameState* state) {
    BgSpot01Idohashira* self = (BgSpot01Idohashira*)actor;

    if (self->drawConfig == 0) {
        BgSpot01IdohashiraActionFunc* funcs = BG_SPOT01_IDOHASHIRA_DRAW_FUNCS;
        BgSpot01IdohashiraActionFunc func = funcs[0];

        if (func != NULL) {
            func(self, (PlayState*)state);
        }
    }
}

// ---------------------------------------------------------------------------
// Unmatched action functions (still GLOBAL_ASM in functions_3A0000s.cpp /
// functions_380000s.cpp). Drafts below are NOT compiled.
// ---------------------------------------------------------------------------
#if 0
// action 1 and 3 (caseD_1_003a7ae4 / caseD_3_003a5da0), N64 func_808AB444 "check demo".
// Draft compiles to 128-136 B vs 132 B in the original; register allocation differs
// (asm keeps self in r5, cue in r4, csCtx in r6). Draft variants tried: switch / if-chain,
// csCtx local vs play->csCtx, GameState signature: none byte-identical yet.
// Opus pass (2026-10-13, ~25 variants, best 24/33 words differ): `CutsceneContext* csCtx = &play->csCtx`
// inside a GetCue helper reproduces the prologue exactly (add r6,#0x2000/#0x298; ldrne [r6,#0x4c]).
// Still missing: ROM hoists the constants 1/0/2 BEFORE the `cue == NULL` test (blocking if-conversion),
// uses `streq r3,[r5,#0x1bc]!` writeback in case 2 and stores cueId through a COPY of self
// (`mov r6, r5`), i.e. two distinct pointer variables (cmp reads self->cueId, final store via the other).
// Storing cueId via `((BgSpot01Idohashira*)thisx)->cueId` with an (Actor* thisx) signature un-merges
// the NULL/cueId tests (separate beqs, like ROM) but the case blocks become branches instead of streq.
static void BgSpot01Idohashira_CheckDemo(BgSpot01Idohashira* self, PlayState* play) {
    CutsceneContext* csCtx = &play->csCtx;
    CsCmdActorAction* cue = NULL;
    u16 nextCueId;

    if (play->GetCutsceneState() != CS_STATE_IDLE) {
        cue = csCtx->npcActions[2];
    }
    if (cue != NULL) {
        nextCueId = cue->action;
        if (nextCueId != self->cueId) {
            if (nextCueId == 1) {
                self->action = 1;
                self->drawConfig = 0;
            } else if (nextCueId == 2) {
                self->action = 2;
                self->drawConfig = 0;
                self->unk_1C8 = 1;
            } else if (nextCueId == 3) {
                Actor_Kill(&self->dyna.actor);
            }
            self->cueId = nextCueId;
        }
    }
}

void caseD_1_003a7ae4_draft(BgSpot01Idohashira* self, PlayState* play) {
    BgSpot01Idohashira_CheckDemo(self, play);
}
#endif

#if 0
// action 2 (caseD_2_003a5b20), N64 func_808AB530 = func_808AB18C (rot spin) + func_808AB29C (free fall
// along cue) + func_808AB414 (effects, action = 3). Draft, not verified.
void caseD_2_003a5b20_draft(BgSpot01Idohashira* self, PlayState* play) {
    CsCmdActorAction* cue = NULL;
    Vec3f initPos;
    Vec3f pos;
    f32 t = 0.0f;
    s32 finished = false;

    self->dyna.actor.shape.rot.x += kREG(6);
    self->dyna.actor.shape.rot.y += (s16)(kREG(7) + 0x3E8);
    self->dyna.actor.shape.rot.z += (s16)(kREG(8) + 0x7D0);

    if (play->GetCutsceneState() != CS_STATE_IDLE) {
        cue = play->csCtx.npcActions[2];
    }
    if (cue != NULL) {
        t = Environment_LerpWeight(cue->endFrame, cue->startFrame, play->csCtx.frames);
        initPos = self->dyna.actor.home.pos;
        self->dyna.actor.world.pos.x = ((cue->endPos.x - initPos.x) * t) + initPos.x;
        // y: FreeFall(initPos.y, (kREG(10) + 1100.0f) / 10.0f + cue->endPos.y, ...), z: lerp like x
        finished = (t >= 1.0f);
    }
    if (finished) {
        // func_808AAE6C: sparks + sound, then action = 3, drawConfig = 0
        pos = self->dyna.actor.world.pos;
        pos.y += kREG(15);
        FUN_0037378c(play, &pos, kREG(11) + 350.0f, kREG(12) + 5, kREG(13) + 0x7D0, kREG(14) + 0x320, 0);
        FUN_003192f0(play, &pos, 5);
        Audio_PlaySoundAtPosition(play, &self->dyna.actor.world.pos, 60, NA_SE_EV_WOODBOX_BREAK);
        self->action = 3;
        self->drawConfig = 0;
    }
}
#endif

#if 0
// draw (FUN_0039e58c), N64 func_808AB700: Gfx_SetupDL_25Opa, MATRIX_FINALIZE_AND_LOAD, then
// func_808AAF34 (two side sparks when unk_1C8 != 0, positions via Matrix_MultVec3f), then the
// well arch display list. Not drafted in detail: 3DS gfx helpers unknown yet (516 B asm).
#endif
}
