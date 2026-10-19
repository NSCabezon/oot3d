extern "C" {
#include "z_en_ganon_organ.hpp"

#define FLAGS 0x00400030

// Parent is Boss_Ganon; only organAlpha (at +0x10A0) is used here.
typedef struct {
    char unk_000[0x10A0];
    s16 organAlpha;
} EnGanonOrganParent;

typedef struct {
    f32 x, y, z, w;
} EnGanonOrganColor;

void FUN_00342be0(Actor* actor, s32 arg1, s32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6);
void FUN_00357388(Actor* actor, EnGanonOrganColor* color, s32 arg2, s32 arg3);

void EnGanonOrgan_Init(Actor* thisx, PlayState* play);
void EnGanonOrgan_Destroy(Actor* thisx, PlayState* play);
void EnGanonOrgan_Update(Actor* thisx, PlayState* play);
void EnGanonOrgan_Draw(Actor* thisx, PlayState* play);

ActorInit En_Ganon_Organ_InitVars = {
    ACTOR_EN_GANON_ORGAN,
    ACTORCAT_BOSS,
    FLAGS,
    OBJECT_GANON,
    sizeof(EnGanonOrgan),
    (ActorFunc)EnGanonOrgan_Init,
    (ActorFunc)EnGanonOrgan_Destroy,
    (ActorFunc)EnGanonOrgan_Update,
    (ActorFunc)EnGanonOrgan_Draw,
};

void EnGanonOrgan_Init(Actor* thisx, PlayState* play) {
    thisx->flags &= ~1;
    FUN_00372f38(thisx, play, &((EnGanonOrgan*)thisx)->skelAnimModel, 0x11, 0);
}

void EnGanonOrgan_Destroy(Actor* thisx, PlayState* play) {
    FUN_00350f34(thisx, &((EnGanonOrgan*)thisx)->skelAnimModel, 0);
}

void EnGanonOrgan_Update(Actor* thisx, PlayState* play) {
    EnGanonOrganParent* dorf;

    if (thisx->params == 1) {
        dorf = (EnGanonOrganParent*)thisx->parent;
        if (dorf->organAlpha == 0) {
            Actor_Kill(thisx);
        }
    }
}

GLOBAL_ASM("asm/EnGanonOrgan_Draw.s")

#if 0 // Draw: best draft (Opus closing pass), 12/84 words differ (see notes/En_Ganon_Organ.md)
// Frame (0x10, r6 = 1 hoisted) and s0/s1/s2 scale regs match. Only diff: ROM holds the matrix base as a
// copy of the actor pointer (mov r0, r4; [r0, #0x170]); this draft gets add r0, r4, #0x148 and rebases
// the later columns on r0. Needs data_syms.s: `EnGanonOrgan_sColor EQU 0x004D9604` (16-byte {1,1,1,0}).
extern const EnGanonOrganColor EnGanonOrgan_sColor;

static inline void EnGanonOrgan_ScaleCol(MTX34* mtx, s32 col, f32 scale) {
    mtx->data[0][col] *= scale;
    mtx->data[1][col] *= scale;
    mtx->data[2][col] *= scale;
}

static inline void EnGanonOrgan_ScaleMtx(MTX34* mtx, Vec3f* scale) {
    EnGanonOrgan_ScaleCol(mtx, 0, scale->x);
    EnGanonOrgan_ScaleCol(mtx, 1, scale->y);
    EnGanonOrgan_ScaleCol(mtx, 2, scale->z);
}

void EnGanonOrgan_Draw(Actor* thisx, PlayState* play) {
    EnGanonOrgan* self = (EnGanonOrgan*)thisx;
    EnGanonOrganParent* dorf = (EnGanonOrganParent*)self->actor.parent;
    EnGanonOrganColor color;
    Vec3f scale;

    if (self->actor.params == 1) {
        FUN_00342be0(&self->actor, 5, 0, 0.0f, 0.0f, 0.0f, dorf->organAlpha * (1.0f / 255.0f));
    }
    scale.x = 100.0f;
    scale.y = 100.0f;
    scale.z = 100.0f;
    EnGanonOrgan_ScaleMtx(&thisx->modelMtx, &scale);
    color = EnGanonOrgan_sColor;
    color.w = dorf->organAlpha * (1.0f / 255.0f);
    FUN_00357388(&self->actor, &color, 4, 2);
    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&self->actor.modelMtx);
    self->skelAnimModel->FUN_00372170(0);
}
#endif
}
