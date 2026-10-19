extern "C" {
#include "z_en_zl3.hpp"

#define FLAGS 0x00000010

void EnZl3_Init(Actor* actor, GameState* state);
void EnZl3_Destroy(Actor* actor, GameState* state);
void EnZl3_Update(Actor* actor, GameState* state);
void EnZl3_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_Zl3.data.s")

ActorInit En_Zl3_InitVars = {
    ACTOR_EN_ZL3,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_ZL2,
    sizeof(EnZl3),
    (ActorFunc)EnZl3_Init,
    (ActorFunc)EnZl3_Destroy,
    (ActorFunc)EnZl3_Update,
    (ActorFunc)EnZl3_Draw,
};

// Unmatched draft (Haiku pass). Asm: asm/EnZl3_Init.s
// Known from asm: unk_19A = 1; zarIndex computed from actor->objBankIndex and play->(0x3A64 + idx*0x80)
// (raw arithmetic, not understood yet); ActorShape_Init(shape, 0, ActorShadow_DrawCircle, 30.0f) then
// shadowAlpha = 0; Collider_InitCylinder + SetCylinderType1 (sCylinderInit at 0x530994); SkelAnime_Init
// via FUN_00353e78 with ZAR_GetCMBByIndex(zarIndex, 0); SkelAnime_SetFaceAnimations(face, skel, 1, 2, 0);
// then switch on actor->params & 0xF (1: gSaveContext subtimer off; 3: Flags_GetSwitch(0x37) and a
// one-shot Audio_PlaySoundGeneral(0x010004E6) under conditions).
#if 0
void EnZl3_Init(Actor* actor, GameState* state) {
    EnZl3* self = (EnZl3*)actor;
    PlayState* play = (PlayState*)state;

    actor->unk_19A = 1;
    /* zarIndex / ZAR_GetCMBByIndex / FUN_00353e78 / SkelAnime_SetFaceAnimations: see asm */
    (void)self;
    (void)play;
}
#else
GLOBAL_ASM("asm/EnZl3_Init.s")
#endif

// Matches asm/EnZl3_Destroy.s (SkelAnime_Free2, FaceAnimations::Destroy, Collider_DestroyCylinder tail call).
void EnZl3_Destroy(Actor* actor, GameState* state) {
    EnZl3* self = (EnZl3*)actor;
    PlayState* play = (PlayState*)state;

    SkelAnime_Free2(&self->skelAnime);
    ((FaceAnimations*)self->unk_228)->Destroy();
    Collider_DestroyCylinder(play, &self->collider);
}

// Update: asm/EnZl3_Update.s. Action-table dispatch, table at raw 0x005309f0 (0x28 entries).
// Bounds check before the table load does not reproduce the ROM addressing (tried 4 variants). Tech debt.
#if 0
void EnZl3_Update(Actor* actor, GameState* state) {
    EnZl3* self = (EnZl3*)actor;
    EnZl3ActionFunc func;

    if ((u32)self->action >= 0x28) {
        return;
    }
    func = ((EnZl3ActionFunc*)0x005309F0)[self->action];
    if (func == NULL) {
        return;
    }
    func(self, (PlayState*)state);
}
#else
GLOBAL_ASM("asm/EnZl3_Update.s")
#endif

// Unmatched draft (Haiku pass). Asm: asm/EnZl3_Draw.s
// drawConfig (0xF64): 0 = nothing; 1 = opaque path (FUN_00342be0 + SkelAnime_DrawOpa, or FUN_0037266c/FUN_0036932c
// depending on self+0x1018); 2 = alpha path (FUN_00342be0 with alpha from 0xF6C, then SkelAnime_DrawOpa).
// Face: FaceAnimations::FUN_0035e3a4 for eye/mouth/brow indices from tables 0x4D9F43 / 0x4D9F40 (u8 arrays).
#if 0
void EnZl3_Draw(Actor* actor, GameState* state) {
    EnZl3* self = (EnZl3*)actor;
    (void)self;
    (void)state;
}
#else
GLOBAL_ASM("asm/EnZl3_Draw.s")
#endif

}
