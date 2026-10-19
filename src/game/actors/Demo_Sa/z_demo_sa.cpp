extern "C" {
#include "z_demo_sa.hpp"

#define FLAGS 0x00000010

void DemoSa_Init(Actor* actor, GameState* state);
void FUN_0036932c(SkeletonAnimationModel* model, s32 idx);
void* ZAR_GetCMBByIndex(void* zar, s32 index);
void FUN_00353e78(void* zar, PlayState* play, SkelAnime* skelAnime, void* cmb, void* p, s32 a, s32 b, s32 c, s32 d);
void FUN_00353020(SkelAnime* skelAnime, void* animation, s32 mode, f32 speed, f32 start, f32 end, f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 index);
void DemoSa_Destroy(Actor* actor, GameState* state);
void DemoSa_Update(Actor* actor, GameState* state);
void DemoSa_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Demo_Sa.data.s")

// Jump tables live in the data dump above (0x0051C448: 21 update fns, 0x0051C49C: 3 draw fns). No scatter
// symbols exist for them, so they are absolute symbols (EQU) in data_syms.s.
GLOBAL_ASM("../src/game/actors/Demo_Sa/data_syms.s")
extern DemoSaUpdateFunc DemoSa_sUpdateFuncs[];
extern DemoSaDrawFunc DemoSa_sDrawFuncs[];

ActorInit Demo_Sa_InitVars = {
    ACTOR_DEMO_SA,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_SA,
    sizeof(DemoSa),
    (ActorFunc)DemoSa_Init,
    (ActorFunc)DemoSa_Destroy,
    (ActorFunc)DemoSa_Update,
    (ActorFunc)DemoSa_Draw,
};

// Sonnet draft: 25/253 words differ, all register allocation (ROM keeps shape in r8 and the hoisted
// "2" in sb; C puts "2" in r8). Pointer-store helpers for updateIndex/drawIndex fixed the r0/r1 temps.
#if 0
static inline u8* DemoSa_GetObjStatus(PlayState* play, s32 idx) {
    if ((u32)idx < 0x13 && *(s32*)((u8*)play + (idx << 7) + 0x3A64) != 0) {
        return (u8*)play + (idx << 7) + 0x3A5C;
    }
    return NULL;
}

static inline void DemoSa_InitSkel(DemoSa* self, PlayState* play, SkelAnime* skel, s32 arg5) {
    void* zar = DemoSa_GetObjStatus(play, (u8)self->actor.objBankIndex) + 0x10;
    void* cmb = ZAR_GetCMBByIndex(zar, 0);
    void* p = self->actor.unk_178;

    FUN_00353e78(zar, play, skel, cmb, p, arg5, 0, 0, 0);
}
static inline void DemoSa_SetEyeTexIndex(DemoSa* self, s16 idx) { s16* p = &self->unk_3F4; *p = idx; }
static inline void DemoSa_SetMouthTexIndex(DemoSa* self, s16 idx) { s16* p = &self->unk_3F8; *p = idx; }

static inline void DemoSa_SetUpdateIndex(DemoSa* self, s32 v) { s32* p = &self->updateIndex; *p = v; }
static inline void DemoSa_SetDrawIndex(DemoSa* self, s32 v) { s32* p = &self->drawIndex; *p = v; }
static inline void DemoSa_InitCs_ChamberOfSages(DemoSa* self, PlayState* play) {
    SkelAnime* skel = (SkelAnime*)self->unk_1A4;
    ActorShape* shape = &self->actor.shape;
    DemoSa_InitSkel(self, play, skel, 0xF);
    shape->yOffset = -10000.0f;
    DemoSa_SetEyeTexIndex(self, 5);
    DemoSa_SetMouthTexIndex(self, 0);
}
static inline void DemoSa_InitCs_TrialOrSealing(DemoSa* self, PlayState* play) {
    ActorShape* shape = &self->actor.shape;
    SkelAnime* skel = (SkelAnime*)self->unk_1A4;
    DemoSa_InitSkel(self, play, skel, -1);
    FUN_00353020(skel, (void*)0x004D87D8, 2, 1.0f, 0.0f, (f32)Animation_GetLastFrame(skel, 0x1A), 0.0f);
    DemoSa_SetUpdateIndex(self, 7);
    shape->shadowAlpha = 0;
    DemoSa_SetEyeTexIndex(self, 2);
    DemoSa_SetMouthTexIndex(self, 2);
}
static inline void DemoSa_InitCs_Unused(DemoSa* self, PlayState* play) {
    DemoSa_InitSkel((DemoSa*)self, play, (SkelAnime*)self->unk_1A4, 0xF);
    DemoSa_SetUpdateIndex(self, 0xA);
    DemoSa_SetDrawIndex(self, 1);
}
static inline void DemoSa_InitCs_Credits(DemoSa* self, PlayState* play) {
    ActorShape* shape = &self->actor.shape;
    SkelAnime* skel = (SkelAnime*)self->unk_1A4;
    DemoSa_InitSkel(self, play, skel, 0x19);
    DemoSa_SetUpdateIndex(self, 0xB);
    DemoSa_SetDrawIndex(self, 0);
    shape->shadowAlpha = 0;
}
static inline void DemoSa_InitCs_Bridge(DemoSa* self, PlayState* play) {
    Vec3f* thisPos = &self->actor.world.pos;
    ActorShape* shape = &self->actor.shape;
    SkelAnime* skel = (SkelAnime*)self->unk_1A4;
    DemoSa_InitSkel(self, play, skel, 0x16);
    Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, ACTOR_EN_ELF, thisPos->x, thisPos->y, thisPos->z, 0, 0, 0, 3);
    DemoSa_SetUpdateIndex(self, 0x10);
    DemoSa_SetDrawIndex(self, 0);
    shape->shadowAlpha = 0;
    DemoSa_SetEyeTexIndex(self, 4);
    DemoSa_SetMouthTexIndex(self, 2);
}

void DemoSa_Init(Actor* actor, GameState* state) {
    DemoSa* self = (DemoSa*)actor;
    PlayState* play = (PlayState*)state;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    switch (self->actor.params) {
        case 2:
            DemoSa_InitCs_TrialOrSealing(self, play);
            break;
        case 3:
            DemoSa_InitCs_Unused(self, play);
            break;
        case 4:
            DemoSa_InitCs_Credits(self, play);
            break;
        case 5:
            DemoSa_InitCs_Bridge(self, play);
            break;
        default:
            DemoSa_InitCs_ChamberOfSages(self, play);
            break;
    }
    SkelAnime_SetFaceAnimations((FaceAnimations*)self->unk_228, (SkelAnime*)self->unk_1A4, 0, 1, -1);
    FUN_0036932c(((SkelAnime*)self->unk_1A4)->unk_28, 5);
}
#endif

GLOBAL_ASM("asm/DemoSa_Init.s")

void DemoSa_Destroy(Actor* actor, GameState* state) {
    DemoSa* self = (DemoSa*)actor;

    SkelAnime_Free((SkelAnime*)self->unk_1A4, (PlayState*)state);
    ((FaceAnimations*)self->unk_228)->Destroy();
}

void DemoSa_Update(Actor* actor, GameState* state) {
    DemoSa* self = (DemoSa*)actor;

    if ((u32)self->updateIndex < 0x15) {
        // 21 entries, action fns in functions_*.cpp.
        DemoSaUpdateFunc f = DemoSa_sUpdateFuncs[self->updateIndex];
        if (f != NULL) {
            f(self, (PlayState*)state);
        }
    }
}

static inline void DemoSa_FaceSet(FaceAnimations* f, s32 i, s32 v) { f->FUN_0035e3a4(i, v); }

void DemoSa_Draw(Actor* actor, GameState* state) {
    DemoSa* self = (DemoSa*)actor;

    if ((u32)self->drawIndex < 3) {
        DemoSaDrawFunc f = DemoSa_sDrawFuncs[self->drawIndex];
        if (f != NULL) {
            f(actor, state);
            DemoSa_FaceSet((FaceAnimations*)self->unk_228, 0, self->unk_3F4);
            DemoSa_FaceSet((FaceAnimations*)self->unk_228, 1, self->unk_3F8);
            ((FaceAnimations*)self->unk_228)->FUN_0035e330();
        }
    }
}

// ---- Update actions moved from functions_*.cpp (names are pinned by the scatter file) ----
void FUN_0031ed30(DemoSa* self, PlayState* play);

// N64: DemoSa_Action_RiseThroughBlueWarp (Rise + UpdateSkelAnime + EndRise)
static inline void DemoSa_Rise(DemoSa* self) {
    f32* y = &self->actor.shape.yOffset;
    *y += 250.0f / 3.0f;
}
static inline void DemoSa_EndRise(DemoSa* self) {
    f32* y = &self->actor.shape.yOffset;
    if (self->actor.shape.yOffset >= 0.0f) {
        self->updateIndex = 3;
        *y = 0.0f;
    }
}
void caseD_2_00245298(DemoSa* self, PlayState* play) {
    DemoSa_Rise(self);
    FUN_00370734((SkelAnime*)self->unk_1A4);
    DemoSa_EndRise(self);
}

// N64: DemoSa_Action_EndMedallionCs
void caseD_6_0027dfb0(DemoSa* self, PlayState* play) {
    Actor_UpdateBgCheckInfo(play, &self->actor, 75.0f, 30.0f, 30.0f, 5);
    FUN_00370734((SkelAnime*)self->unk_1A4);
}

void caseD_12_00385bec(DemoSa* self, PlayState* play) {
    PlayState* play2 = play;
    Actor_UpdateBgCheckInfo(play2, &self->actor, 75.0f, 30.0f, 30.0f, 5);
    FUN_00370734((SkelAnime*)self->unk_1A4);
    FUN_0031ed30(self, play);
}

void caseD_13_0037db6c(DemoSa* self, PlayState* play) {
    PlayState* play2 = play;
    Actor_UpdateBgCheckInfo(play2, &self->actor, 75.0f, 30.0f, 30.0f, 5);
    FUN_00370734((SkelAnime*)self->unk_1A4);
    FUN_0031ed30(self, play);
}

void FUN_00321478(DemoSa* self, PlayState* play);
static inline s32 DemoSa_SkelUpdate(SkelAnime* s) { return FUN_00370734(s); }
// N64: DemoSa_Blink
static inline void DemoSa_Blink(Actor* actor) {
    DemoSa* self = (DemoSa*)actor;
    s16* eyeIndex = &self->unk_3F4;
    s16* blinkTimer = (s16*)&self->unk_3F6;

    if (DECR(*blinkTimer) == 0) {
        *blinkTimer = Rand_S16Offset_003702c8(0x3C, 0x3C);
    }
    *eyeIndex = *blinkTimer;
    if (*eyeIndex >= 3) {
        *eyeIndex = 0;
    }
}


void FUN_0031ecbc(DemoSa* self, s32 idx, s32 mode, s32 rev, f32 morph);
static inline void DemoSa_SetEyeTexIndex(DemoSa* self, s16 idx) { s16* p = &self->unk_3F4; *p = idx; }
void caseD_a_0038d81c(DemoSa* self, PlayState* play) {
    PlayState* play2 = play;
    Actor_UpdateBgCheckInfo(play2, &self->actor, 75.0f, 30.0f, 30.0f, 5);
    FUN_00370734((SkelAnime*)self->unk_1A4);
    DemoSa_Blink(&self->actor);
}

void caseD_d_0037dad4(DemoSa* self, PlayState* play) {
    PlayState* play2 = play;
    Actor_UpdateBgCheckInfo(play2, &self->actor, 75.0f, 30.0f, 30.0f, 5);
    FUN_00370734((SkelAnime*)self->unk_1A4);
    DemoSa_Blink(&self->actor);
    FUN_00321478(self, play);
}

void caseD_e_00385a0c(DemoSa* self, PlayState* play) {
    PlayState* play2 = play;
    Actor_UpdateBgCheckInfo(play2, &self->actor, 75.0f, 30.0f, 30.0f, 5);
    FUN_00370734((SkelAnime*)self->unk_1A4);
    DemoSa_Blink(&self->actor);
    FUN_00321478(self, play);
}

void caseD_4_002a1980(DemoSa* self, PlayState* play) {
    PlayState* play2 = play;
    Actor_UpdateBgCheckInfo(play2, &self->actor, 75.0f, 30.0f, 30.0f, 5);
    if (DemoSa_SkelUpdate((SkelAnime*)self->unk_1A4)) {
        SkelAnime* skel = (SkelAnime*)self->unk_1A4;
        FUN_00353020(skel, (void*)0x004D87D0, 0, 1.0f, 0.0f, (f32)Animation_GetLastFrame(skel, 1), 0.0f);
        self->updateIndex = 5;
    }
}

void caseD_f_0039a6f8(DemoSa* self, PlayState* play) {
    PlayState* play2 = play;
    s32 done;
    Actor_UpdateBgCheckInfo(play2, &self->actor, 75.0f, 30.0f, 30.0f, 5);
    done = DemoSa_SkelUpdate((SkelAnime*)self->unk_1A4);
    DemoSa_Blink(&self->actor);
    if (done) {
        FUN_0031ecbc(self, 0x19, 0, 0, 0.0f);
        self->updateIndex = 0xD;
    }
}

void caseD_14_0037dbb4(DemoSa* self, PlayState* play) {
    PlayState* play2 = play;
    Actor_UpdateBgCheckInfo(play2, &self->actor, 75.0f, 30.0f, 30.0f, 5);
    if (DemoSa_SkelUpdate((SkelAnime*)self->unk_1A4)) {
        FUN_0031ecbc(self, 0x18, 0, 0, 0.0f);
    }
    FUN_0031ed30(self, play);
    if (*(s32*)(self->unk_1A4 + 0x30) == 0x17) {
        if (Animation_OnFrameImpl((SkelAnime*)self->unk_1A4, 4.0f, 1.0f)) {
            DemoSa_SetEyeTexIndex(self, 4);
        }
    }
}

// caseD_1 (N64 DemoSa_Action_AwaitBlueWarp): 5/35 words differ (ROM keeps cue pointer in r1, orders
// mov r2,r5 before mov r1,r4 in the Actor_SpawnAsChild call); stays asm.
#if 0
void caseD_1_0027df24(DemoSa* self, PlayState* play) {
    PlayState* play2 = play;
    CutsceneContext* csCtx = &play->csCtx;
    CsCmdActorAction* cue;
    if (play2->GetCutsceneState() != 0 && (cue = csCtx->npcActions[4]) != NULL && cue->action == 2) {
        self->updateIndex = 2;
        self->drawIndex = 1;
        {
            f32 x = self->actor.world.pos.x;
            f32 y = self->actor.world.pos.y;
            f32 z = self->actor.world.pos.z;
            Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, 0x5D, x, y, z, 0, 0, 0, 2);
        }
    }
}
#endif
GLOBAL_ASM("asm/caseD_1_0027df24.s")

// caseD_3 (6/45 words; csCtx pointer reuses play's register in ROM) and caseD_5 (7/55 words; player pos arg order)
// also stay asm. caseD_0/7/8/c/11 not attempted; caseD_9 is shared with Demo_Du.
}
