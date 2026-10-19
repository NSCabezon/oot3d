extern "C" {
#include "z_demo_du.hpp"

#define FLAGS 0x00000010

void DemoDu_Init(Actor* actor, GameState* state);
void DemoDu_Destroy(Actor* actor, GameState* state);
void* ZAR_GetCMBByIndex(void* zar, s32 index);
void FUN_00353e78(void* zar, PlayState* play, SkelAnime* skelAnime, void* cmb, void* p, s32 a, s32 b, s32 c, s32 d);
void FUN_00353020(SkelAnime* skelAnime, void* animation, s32 mode, f32 speed, f32 start, f32 end, f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 index);
void DemoDu_Update(Actor* actor, GameState* state);
void DemoDu_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Demo_Du.data.s")

ActorInit Demo_Du_InitVars = {
    ACTOR_DEMO_DU,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_DU,
    sizeof(DemoDu),
    (ActorFunc)DemoDu_Init,
    (ActorFunc)DemoDu_Destroy,
    (ActorFunc)DemoDu_Update,
    (ActorFunc)DemoDu_Draw,
};

// Init draft (Opus stage 3): same size as ROM (216 words), 13 words differ. Fixes vs Sonnet draft:
// - yOffset is -10000.0f (N64), not 0.0f.
// - Eye/mouth stores through an `s16* p = &field; *p = v;` helper: stops armcc from hoisting a shared
//   r4+0x300 base, gives ROM's `mov r0,#0x3f8; strh rX,[r0,r4]` form (same idiom in DemoSa_Init, caseD_9_003858a0).
// - Per-case ActorShape* locals; skel passed as a param (cases 2/3) vs computed inside the helper (cases 1/default).
// - Store order (drawIndex before updateIndex; eye/mouth before yOffset) fixes the 0/3 hoisted-const regs.
// Remaining: case 3 cmb/p get fp/ip swapped (+ 0x18 in r0 instead of r1); default path schedules the
// yOffset vstr after the strh's (ROM: before). N64 order yOffset-first gives the right schedule but swaps regs.
#if 0
static inline u8* DemoDu_GetObjStatus(PlayState* play, s32 idx) {
    if ((u32)idx < 0x13 && *(s32*)((u8*)play + (idx << 7) + 0x3A64) != 0) {
        return (u8*)play + (idx << 7) + 0x3A5C;
    }
    return NULL;
}

static inline void DemoDu_InitSkel(DemoDu* self, PlayState* play, SkelAnime* skel, s32 arg5) {
    void* zar = DemoDu_GetObjStatus(play, (u8)self->actor.objBankIndex) + 0x10;
    void* cmb = ZAR_GetCMBByIndex(zar, 0);
    void* p = self->actor.unk_178;

    FUN_00353e78(zar, play, skel, cmb, p, arg5, 0, 0, 0);
    SkelAnime_SetFaceAnimations((FaceAnimations*)self->unk_228, (SkelAnime*)self->unk_1A4, 0, 1, 2);
}
static inline void DemoDu_InitSkelDefault(DemoDu* self, PlayState* play, s32 arg5) {
    void* zar = DemoDu_GetObjStatus(play, (u8)self->actor.objBankIndex) + 0x10;
    SkelAnime* skel = (SkelAnime*)self->unk_1A4;
    void* cmb = ZAR_GetCMBByIndex(zar, 0);
    void* p = self->actor.unk_178;

    FUN_00353e78(zar, play, skel, cmb, p, arg5, 0, 0, 0);
    SkelAnime_SetFaceAnimations((FaceAnimations*)self->unk_228, (SkelAnime*)self->unk_1A4, 0, 1, 2);
}
static inline void DemoDu_SetEyeTexIndex(DemoDu* self, s16 idx) { s16* p = &self->unk_3F4; *p = idx; }
static inline void DemoDu_SetMouthTexIndex(DemoDu* self, s16 idx) { s16* p = &self->unk_3F8; *p = idx; }

static inline void DemoDu_InitCs_FireMedallion(DemoDu* self, PlayState* play) {
    ActorShape* shape = &self->actor.shape;
    DemoDu_InitSkelDefault(self, play, 14);
    DemoDu_SetEyeTexIndex(self, 1);
    DemoDu_SetMouthTexIndex(self, 3);
    shape->yOffset = -10000.0f;
}
static inline void DemoDu_InitCs_GoronsRuby(DemoDu* self, PlayState* play) {
    DemoDu_InitSkelDefault(self, play, -1);
    self->updateIndex = 7;
}
static inline void DemoDu_InitCs_AfterGanon(DemoDu* self, PlayState* play) {
    ActorShape* shape = &self->actor.shape;
    SkelAnime* skel = (SkelAnime*)self->unk_1A4;
    DemoDu_InitSkel(self, play, skel, -1);
    FUN_00353020(skel, (void*)0x004D8604, 2, 1.0f, 0.0f, (f32)Animation_GetLastFrame(skel, 15), 0.0f);
    self->updateIndex = 0x15;
    shape->shadowAlpha = 0;
}
static inline void DemoDu_InitCs_Credits(DemoDu* self, PlayState* play) {
    ActorShape* shape = &self->actor.shape;
    SkelAnime* skel = (SkelAnime*)self->unk_1A4;
    DemoDu_InitSkel(self, play, skel, 12);
    self->drawIndex = 0;
    self->updateIndex = 0x18;
    shape->shadowAlpha = 0;
    DemoDu_SetMouthTexIndex(self, 3);
}

void DemoDu_Init(Actor* actor, GameState* state) {
    DemoDu* self = (DemoDu*)actor;
    PlayState* play = (PlayState*)state;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    switch (self->actor.params) {
        case 1:
            DemoDu_InitCs_GoronsRuby(self, play);
            break;
        case 2:
            DemoDu_InitCs_AfterGanon(self, play);
            break;
        case 3:
            DemoDu_InitCs_Credits(self, play);
            break;
        default:
            DemoDu_InitCs_FireMedallion(self, play);
            break;
    }
}
#endif

GLOBAL_ASM("asm/DemoDu_Init.s")

void DemoDu_Destroy(Actor* actor, GameState* state) {
    DemoDu* self = (DemoDu*)actor;

    SkelAnime_Free((SkelAnime*)self->unk_1A4, (PlayState*)state);
    ((FaceAnimations*)self->unk_228)->Destroy();
}

void DemoDu_Update(Actor* actor, GameState* state) {
    DemoDu* self = (DemoDu*)actor;

    if ((u32)self->updateIndex < 0x1D) {
        // Jump table at 0x0051B4C4 (29 entries, action fns in functions_*.cpp). Raw address: debt.
        DemoDuUpdateFunc* table = (DemoDuUpdateFunc*)0x0051B4C4;
        DemoDuUpdateFunc f = table[self->updateIndex];
        if (f != NULL) {
            f(self, (PlayState*)state);
        }
    }
}

void DemoDu_Draw(Actor* actor, GameState* state) {
    DemoDu* self = (DemoDu*)actor;
    if ((u32)self->drawIndex < 3) {
        // Jump table at 0x0051B538 (3 entries). Raw address: debt.
        DemoDuDrawFunc* table = (DemoDuDrawFunc*)0x0051B538;
        DemoDuDrawFunc f = table[self->drawIndex];
        if (f != NULL) {
            f(actor, state);
        }
    }
}
}
