extern "C" {
#include "z_demo_ik.hpp"

#define FLAGS 0x00000010

typedef void (*DemoIkActionFunc)(DemoIk* self, PlayState* play);
typedef void (*DemoIkDrawFunc)(DemoIk* self, PlayState* play);

void DemoIk_Init(Actor* thisx, PlayState* play);
void DemoIk_Destroy(Actor* thisx, PlayState* play);
void DemoIk_Update(Actor* thisx, PlayState* play);
void DemoIk_Draw(Actor* thisx, PlayState* play);

ActorInit Demo_Ik_InitVars = {
    ACTOR_DEMO_IK,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_IK,
    sizeof(DemoIk),
    (ActorFunc)DemoIk_Init,
    (ActorFunc)DemoIk_Destroy,
    (ActorFunc)DemoIk_Update,
    (ActorFunc)DemoIk_Draw,
};

// Action/draw tables are the data at 0x0051BCE4 (6 action ptrs) and 0x0051BCFC (3 draw ptrs), still
// emitted by data/z_Demo_Ik.data.s. Entries point at functions in other blocks (raw addresses).
GLOBAL_ASM("data/z_Demo_Ik.data.s")

void DemoIk_Destroy(Actor* thisx, PlayState* play) {
    DemoIk* self = (DemoIk*)thisx;

    SkelAnime_Free2(&self->skelAnime);
}

// Update draft (matches in isolation) but stays asm: its ROM span is 0x50 B, and the last 0x28 B at
// 0x0012E23C are unlabeled code (no functions.tsv entry). A 0x28 B C body would drop them and shift the layout.
#if 0
void DemoIk_Update(Actor* thisx, PlayState* play) {
    DemoIk* self = (DemoIk*)thisx;
    DemoIkActionFunc* sActionFuncs;

    if ((u32)self->actionMode >= 6) {
        return;
    }
    sActionFuncs = (DemoIkActionFunc*)0x0051BCE4;
    if (sActionFuncs[self->actionMode] == NULL) {
        return;
    }
    sActionFuncs[self->actionMode](self, play);
}
#endif
GLOBAL_ASM("asm/DemoIk_Update.s")

void DemoIk_Draw(Actor* thisx, PlayState* play) {
    DemoIk* self = (DemoIk*)thisx;
    DemoIkDrawFunc* sDrawFuncs;

    if ((u32)self->drawMode >= 3) {
        return;
    }
    sDrawFuncs = (DemoIkDrawFunc*)0x0051BCFC;
    if (sDrawFuncs[self->drawMode] == NULL) {
        return;
    }
    sDrawFuncs[self->drawMode](self, play);
}

// Init draft (Opus closing pass): same size (532 B) as ROM, body/regalloc identical from 0x58 on; only the
// head differs: ROM keeps self in r0 through the object-bank lookup and copies it to r4 right before the
// params branch (so params is in r1 and &shape is computed into r0 before the Type1 switch). Ours copies at
// entry. Tried ~25 variants (thisx/self mixes, GameState* signature, inline zar helpers, early shape local,
// switch/if forms). Key wins kept: N64 Type1Init/Type2Init inlines, Type1 as switch, limbCount set per case,
// decl order cmbIdx/animIdx/limbCount, no `p` local, Animation_Change arg order (skel, idx, mode, speed,...).
#if 0
void* ZAR_GetCMBByIndex(void* zar, s32 idx);
void FUN_00358ea8(void* zar, PlayState* play, SkelAnime* skelAnime, void* cmb, void* p, s32 a, void* b, void* c, s32 d);
void FUN_00353e78(void* zar, PlayState* play, SkelAnime* skelAnime, void* cmb, void* p, s32 a, void* b, void* c, s32 d);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, s32 mode, f32 speed, f32 start, f32 end, f32 morph);

static inline u8* DemoIk_GetObjStatus(PlayState* play, s32 idx) {
    if ((u32)idx < 0x13 && *(s32*)((u8*)play + (idx << 7) + 0x3A64) != 0) {
        return (u8*)play + (idx << 7) + 0x3A5C;
    }
    return NULL;
}

static inline void DemoIk_Type1Init(DemoIk* self, PlayState* play) {
    SkelAnime* skel;
    s32 cmbIdx;
    s32 animIdx;
    s32 limbCount;
    f32 shadowScale;
    void* cmb;
    void* p;

    switch (self->actor.params) {
        case 0:
            cmbIdx = 2;
            animIdx = 0x14;
            limbCount = 1;
            shadowScale = 30.0f;
            break;
        case 1:
            cmbIdx = 0xD;
            animIdx = 0x16;
            limbCount = 1;
            shadowScale = 10.0f;
            break;
        default:
            cmbIdx = 0xC;
            animIdx = 0x15;
            limbCount = 1;
            shadowScale = 20.0f;
            break;
    }
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, shadowScale);
    skel = &self->skelAnime;
    cmb = ZAR_GetCMBByIndex(self->zarInfo, cmbIdx);
    FUN_00358ea8(self->zarInfo, play, skel, cmb, self->actor.unk_178, 0, self->jointTable, self->morphTable, limbCount);
    Animation_Change_00375c08(skel, animIdx, 2, 1.0f, 0.0f, (f32)Animation_GetLastFrame(skel, animIdx), 0.0f);
}

static inline void DemoIk_Type2Init(DemoIk* self, PlayState* play) {
    SkelAnime* skel = &self->skelAnime;
    s32 cmbIdx;
    s32 animIdx;
    s32 limbCount;
    void* cmb;
    void* p;

    switch (self->actor.params) {
        case 3:
            cmbIdx = 0xA;
            animIdx = 0x11;
            limbCount = 1;
            break;
        case 4:
            cmbIdx = 0xB;
            animIdx = 0x12;
            limbCount = 1;
            break;
        case 5:
            cmbIdx = 3;
            animIdx = 0x13;
            limbCount = 1;
            break;
        default:
            cmbIdx = 1;
            animIdx = 0x17;
            limbCount = 1;
            break;
    }
    cmb = ZAR_GetCMBByIndex(self->zarInfo, cmbIdx);
    FUN_00353e78(self->zarInfo, play, skel, cmb, self->actor.unk_178, 0, self->jointTable, self->morphTable, limbCount);
    Animation_Change_00375c08(skel, animIdx, 2, 1.0f, 0.0f, (f32)Animation_GetLastFrame(skel, animIdx), 0.0f);
    self->actionMode = 3;
    self->drawMode = 0;
}

void DemoIk_Init(Actor* thisx, PlayState* play) {
    DemoIk* self = (DemoIk*)thisx;
    s32 params;

    self->actor.unk_19A = 1;
    self->zarInfo = DemoIk_GetObjStatus(play, (u8)self->actor.objBankIndex) + 0x10;
    params = self->actor.params;
    if (params <= 2) {
        DemoIk_Type1Init(self, play);
    } else {
        DemoIk_Type2Init(self, play);
    }
}
#endif
GLOBAL_ASM("asm/DemoIk_Init.s")
}
