extern "C" {
#include "z_en_zl2.hpp"

#define FLAGS 0x00000010

void EnZl2_Init(Actor* actor, GameState* state);
void EnZl2_Destroy(Actor* actor, GameState* state);
void EnZl2_Update(Actor* actor, GameState* state);
void EnZl2_Draw(Actor* actor, GameState* state);

// N64 tables that have no symbol in the scatter file. Defined as absolute symbols in syms.s
// (a constant-address macro makes armcc split the address and not match).
extern EnZl2ActionFunc sEnZl2ActionFuncs[];
extern EnZl2DrawFunc sEnZl2DrawFuncs[];
extern u8 sEnZl2EyeTexIndexes[];

GLOBAL_ASM("data/z_En_Zl2.data.s")
GLOBAL_ASM("../src/game/actors/En_Zl2/syms.s")

ActorInit En_Zl2_InitVars = {
    ACTOR_EN_ZL2,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_ZL2,
    sizeof(EnZl2),
    (ActorFunc)EnZl2_Init,
    (ActorFunc)EnZl2_Destroy,
    (ActorFunc)EnZl2_Update,
    (ActorFunc)EnZl2_Draw,
};

#if 0 // Haiku draft, does not match yet: 256 B vs 264 B; register allocation differs (ROM keeps
// &skelAnime in r7 and &shape in r8, and stores zar in r8 for the later calls). Tried ~5 variants.
void* ZAR_GetCMBByIndex(void* zar, s32 idx);
void FUN_00353e78(void* zar, PlayState* play, SkelAnime* skelAnime, void* cmb, void* p, s32 a, s32 b, s32 c, s32 d);
void FUN_00347cc4(s32 idx);

void EnZl2_Init(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnZl2* self = (EnZl2*)actor;
    SkelAnime* skel = &self->skelAnime;
    u8* zarBase = NULL;
    void* zar;
    void* cmb;

    ActorShape_Init(&actor->shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    actor->shape.shadowAlpha = 0;
    actor->unk_19A = 1;
    if ((u8)actor->objBankIndex < 0x13) {
        zarBase = (u8*)play + ((u8)actor->objBankIndex << 7);
        if (*(s32*)(zarBase + 0x3A64) != 0) {
            zarBase += 0x3A5C;
        }
    }
    zar = zarBase + 0x10;
    self->zar = zar;
    cmb = ZAR_GetCMBByIndex(zar, 0);
    FUN_00353e78(zar, play, skel, cmb, actor->unk_178, 0, 0, 0, 0);
    SkelAnime_SetFaceAnimations(&self->faceAnims, skel, 0, 2, 1);
    if (actor->params == 1) {
        FUN_00347cc4(0x6F);
        return;
    }
    if (actor->params == 4) {
        *(s16*)(0x00588E58 + 0x62) = 0;
    }
}
#endif
GLOBAL_ASM("asm/EnZl2_Init.s")

void EnZl2_Destroy(Actor* actor, GameState* state) {
    EnZl2* self = (EnZl2*)actor;

    SkelAnime_Free2(&self->skelAnime);
    self->faceAnims.Destroy();
}

void EnZl2_Update(Actor* actor, GameState* state) {
    EnZl2* self = (EnZl2*)actor;

    if ((u32)self->action < 0x24) {
        EnZl2ActionFunc func = sEnZl2ActionFuncs[self->action];

        if (func != NULL) {
            func(self, (PlayState*)state);
        }
    }
}

#if 0 // Haiku draft, does not match yet: 188 B vs 184 B, and the eye/mouth index loads get
// different registers than the ROM (ROM loads the s16 index into r1 first).
void FUN_0036932c(SkeletonAnimationModel* model, s32 idx);
void FUN_0033ce74(void* a, s32 b);

void EnZl2_Draw(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnZl2* self = (EnZl2*)actor;
    s32 drawConfig = self->drawConfig;

    if ((u32)drawConfig < 3 && sEnZl2DrawFuncs[drawConfig] != NULL) {
        self->faceAnims.FUN_0035e3a4(0, sEnZl2EyeTexIndexes[self->unk_548]);
        self->faceAnims.FUN_0035e3a4(2, sEnZl2EyeTexIndexes[self->unk_54C]);
        self->faceAnims.FUN_0035e3a4(1, (sEnZl2EyeTexIndexes - 3)[self->unk_550]);
        self->faceAnims.FUN_0035e330();
        FUN_0036932c(self->skelAnime.unk_28, 7);
        FUN_0033ce74(*(void**)((u8*)play + 0x20AC), 0x14);
        sEnZl2DrawFuncs[drawConfig](self, play);
    }
}
#endif
GLOBAL_ASM("asm/EnZl2_Draw.s")
}
