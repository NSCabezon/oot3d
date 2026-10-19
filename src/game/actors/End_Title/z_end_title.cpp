extern "C" {
#include "z_end_title.hpp"
#include "model_manager.hpp"
#include <math.h>

#define R_UPDATE_RATE SREG(30)
#define FLAGS 0x00000030

void EndTitle_Init(Actor* actor, GameState* state);
void EndTitle_Destroy(Actor* actor, GameState* state);
void EndTitle_Update(Actor* actor, GameState* state);
void EndTitle_Draw(Actor* actor, GameState* state);

void* ZAR_GetCTXBByIndex(ZARInfo* zar, s32 index);
void FUN_002504ac(EndTitle* self, PlayState* play);
void FUN_00250540(EndTitle* self, PlayState* play);

GLOBAL_ASM("data/z_End_Title.data.s")
GLOBAL_ASM("../src/game/actors/End_Title/data_syms.s")

// Absolute symbol (data_syms.s) for the 0x005C1878 global; the word at +0xF3C selects the End Title variant.
extern u8 gUnk_005C1878;

ActorInit End_Title_InitVars = {
    ACTOR_END_TITLE,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EndTitle),
    (ActorFunc)EndTitle_Init,
    (ActorFunc)EndTitle_Destroy,
    (ActorFunc)EndTitle_Update,
    (ActorFunc)EndTitle_Draw,
};

void EndTitle_Init(Actor* actor, GameState* state) {
    EndTitle* self = (EndTitle*)actor;
    ZARInfo* zar;
    s32 i;
    s32 idx;
    s32 sel;

    self->endAlpha = self->tlozAlpha = self->ootAlpha = 0.0f;
    zar = &((PlayState*)state)->GetObjectStatus(actor->objBankIndex)->zarInfo;
    self->skelAnimModel = NULL;
    for (i = 0; i < 4; i++) {
        self->zarCtxb[i] = ZAR_GetCTXBByIndex(zar, 0x37 + i);
    }
    GetModelManager();
    idx = 0x3B;
    sel = *(s32*)((u8*)&gUnk_005C1878 + 0xF3C);
    switch (sel) {
        case 0:
        case 1:
        case 2:
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            idx = 0x3C;
            break;
        case 8:
            idx = 0x3D;
            break;
        case 3:
            idx = 0x3E;
            break;
    }
    self->unk_1B8 = ZAR_GetCTXBByIndex(zar, idx);
    if (actor->params == 1) {
        actor->draw = (ActorFunc)FUN_002504ac;
    } else if (actor->params == 2) {
        actor->draw = (ActorFunc)FUN_00250540;
    } else {
        FUN_00372f38(actor, (PlayState*)state, &self->skelAnimModel, 0x3B, 0);
    }
}

void EndTitle_Destroy(Actor* actor, GameState* state) {
    EndTitle* self = (EndTitle*)actor;

    if (self->skelAnimModel != NULL) {
        FUN_003508b8(actor, self->skelAnimModel, 0);
        self->skelAnimModel = NULL;
    }
}

void EndTitle_Update(Actor* actor, GameState* state) {
}

// Draft (NOT compiled): 12/37 words differ (stage 3). Constants in locals (step, max) fix pool order + 1.0f preload
// + separate clamp store; t computed after frames and `t < frames` fixes compare. Only diff left: ROM converts frames
// (vmov s1,r2; vcvt s5,s1) AFTER the vdiv, armcc hoists the conversion right after the ldrh (~40 orderings/types tried).
#if 0
void FUN_002504ac(EndTitle* self, PlayState* play) {
    s32 frames = play->csCtx.frames;
    f32 t = 0.5f + (3.0f / R_UPDATE_RATE) * 1100.0f;
    f32 step = 1.0f / 255.0f;
    f32 max = 1.0f;

    if (t < frames) {
        f32 a = self->endAlpha;
        if (a < max) {
            self->endAlpha = a + R_UPDATE_RATE * step;
        } else {
            self->endAlpha = max;
        }
    }
}
#endif

// Typed ctx struct (count[] re-read through the pointer), Vector3f locals declared at point of use (center, size) and
// MTX34 declared AFTER them give frame 0x54 and the exact slots. The block-scoped `zero` local keeps the 0 for unk170
// live during the 48-byte copy (ROM 6+6 regs, dst in r7).
typedef struct { u32 w[4]; } EndTitleBlk16;
typedef struct { u32 w[12]; } EndTitleBlk48;
typedef struct {
    char pad0[0x3C];
    Vector3f pos;        // 0x3C
    Vector3f scale;      // 0x48
    MTX34 mtx;           // 0x54
    char pad84[0xF0 - 0x84];
    EndTitleBlk16 unkF0; // 0xF0
    char pad100[0x10];
    EndTitleBlk48 unk110; // 0x110
    char pad140[0x170 - 0x140];
    s32 unk170;          // 0x170
} EndTitleObj;
typedef struct {
    char pad0[0x10];
    u32 total;               // 0x10
    u32 count[0x20];         // 0x14
    void* models[0x48];      // 0x94
    EndTitleObj* pool[24];   // 0x1B4
    EndTitleObj* list[7][24]; // 0x214
    s32 list2[7][24];        // 0x4B4
} EndTitleCtx;
void FUN_003332b4(MTX34* mtx, f32 x, f32 y, f32 z);
s32 FUN_00348a64(void* model, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
s32 FUN_0030fda8(EndTitleCtx* ctx, s32 idx);

s32 FUN_003fdad0(EndTitleCtx* ctx, s32 idx, EndTitleBlk16* a2, f32* rect, f32 f, s32 a4, s32* a5, EndTitleBlk48* a6, s32 a7) {
    EndTitleObj* o;

    if (ctx->count[idx] >= 24 || ctx->total >= 24) {
        return 0;
    }
    o = ctx->pool[ctx->total];
    Vector3f center(rect[2] + (rect[3] - rect[2]) * 0.5f, rect[0] + (rect[1] - rect[0]) * 0.5f, 0.0f);
    o->pos = center;
    Vector3f size(rect[3] - rect[2], rect[1] - rect[0], 1.0f);
    o->scale = size;
    MTX34 mtx;
    o->unkF0 = *a2;
    FUN_003332b4(&mtx, 0.0f, 0.0f, f);
    o->mtx = mtx;
    FUN_00348a64(ctx->models[ctx->total], 0, a4, a5[1], a5[0], a5[2], a5[3]);
    {
        s32 zero = 0;

        o->unk110 = *a6;
        o->unk170 = zero;
    }
    ctx->list[idx][ctx->count[idx]] = o;
    ctx->list2[idx][ctx->count[idx]] = a7;
    ctx->count[idx]++;
    ctx->total++;
    FUN_0030fda8(ctx, idx);
    return 1;
}

s32 FUN_003fda88(void* ctx, s32 idx, void* a2, f32* rect, s32 a4, s32* a5, void* a6, s32 a7) {
    return FUN_003fdad0((EndTitleCtx*)ctx, idx, (EndTitleBlk16*)a2, rect, 0.0f, a4, a5, (EndTitleBlk48*)a6, a7);
}

typedef struct {
    u32 unk_00; // frames of the logo fade (limit for logoTimer)
    f32 unk_04;
} EndTitleGlobal;
extern EndTitleGlobal gUnk_005311b0;

// FUN_00250540 (params == 2 draw): draft in z_end_title_logo.cpp (own TU: a same-TU FUN_003fda88 changes its call
// setup); still GLOBAL_ASM, 16 words off in the inlined identity-MTX34 init.

// Draft (NOT compiled): thresholds/rates/rotation/scale now structurally match (pos via SetComp index helper gives
// the r6=1 idiom; 1-in-r6 + angle sinf/cosf inside a block fixes the rotation reg alloc); remaining diff is only
// literal/reg allocation of the three alpha fades (ROM: s3=3.0, s1=0.5, s4=threshold-mult; mine s1,s2,s4).
#if 0
void Matrix_UpdatePosition(MTX34* dst, MTX34* src, Vec3f* pos);
void FUN_003720a4(void* obj, MTX34* mtx);

static inline void EndTitle_SetComp(Vec3f* v, s32 i, f32 val) {
    (&v->x)[i] = val;
}

static inline void EndTitle_RotRow(MTX34* mtx, s32 row, f32 s, f32 c) {
    f32 a = mtx->data[row][1];
    f32 b = mtx->data[row][2];
    mtx->data[row][1] = a * c + b * s;
    mtx->data[row][2] = b * c - a * s;
}

static inline void EndTitle_RotCols(MTX34* mtx, s32 c1, s32 c2, f32 s, f32 c) {
    f32 a, b;
    a = mtx->data[0][c1]; b = mtx->data[0][c2];
    mtx->data[0][c1] = a * c + b * s;
    mtx->data[0][c2] = b * c - a * s;
    a = mtx->data[1][c1]; b = mtx->data[1][c2];
    mtx->data[1][c1] = a * c + b * s;
    mtx->data[1][c2] = b * c - a * s;
    a = mtx->data[2][c1]; b = mtx->data[2][c2];
    mtx->data[2][c1] = a * c + b * s;
    mtx->data[2][c2] = b * c - a * s;
}

static inline void EndTitle_ScaleCol(MTX34* mtx, s32 col, f32 scale) {
    mtx->data[0][col] *= scale;
    mtx->data[1][col] *= scale;
    mtx->data[2][col] *= scale;
}

void EndTitle_Draw(Actor* actor, GameState* state) {
    EndTitle* self = (EndTitle*)actor;
    PlayState* play = (PlayState*)state;
    MTX34 mtx;
    Vec3f pos;
    Vec3f scale;
    s32 frames = play->csCtx.frames;
    f32 s;
    f32 c;
    s32 i;

    FUN_003720a4(*(void**)((u8*)play + 0x20AC), &mtx);
    {
        f32 x = gUnk_005311b0.unk_04;
        f32 y = x + 150.0f;
        f32 z = x + 170.0f;
        EndTitle_SetComp(&pos, 0, x);
        EndTitle_SetComp(&pos, 1, y);
        EndTitle_SetComp(&pos, 2, z);
    }
    Matrix_UpdatePosition(&mtx, &mtx, &pos);
    scale.x = 0.13f;
    scale.y = 0.13f;
    scale.z = 0.13f;
    EndTitle_ScaleCol(&mtx, 0, scale.x);
    EndTitle_ScaleCol(&mtx, 1, scale.y);
    EndTitle_ScaleCol(&mtx, 2, scale.z);
    {
        f32 ang = 0.2873326241970062f;
        f32 sv = sinf(ang);
        f32 cv = cosf(ang);
        EndTitle_RotCols(&mtx, 1, 2, sv, cv);
    }
    self->skelAnimModel->FUN_003721e0(&mtx);
    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_00372170(0);

    if (0.5f + (3.0f / R_UPDATE_RATE) * 890.0f < (f32)frames) {
        if (self->endAlpha < 0.7843138f) {
            self->endAlpha += R_UPDATE_RATE * 2.3333333f * (1.0f / 255.0f);
        } else {
            self->endAlpha = 0.7843138f;
        }
    }
    if (0.5f + (3.0f / R_UPDATE_RATE) * 810.0f < (f32)frames) {
        if (self->tlozAlpha < 0.7843138f) {
            self->tlozAlpha += R_UPDATE_RATE * 5.0f * (1.0f / 255.0f);
        } else {
            self->tlozAlpha = 0.7843138f;
        }
    }
    if (0.5f + (3.0f / R_UPDATE_RATE) * 850.0f < (f32)frames) {
        if (self->ootAlpha < 0.7843138f) {
            self->ootAlpha += R_UPDATE_RATE * 5.0f * (1.0f / 255.0f);
        } else {
            self->ootAlpha = 0.7843138f;
        }
    }
}
#endif

GLOBAL_ASM("asm/EndTitle_Draw.s")
}
