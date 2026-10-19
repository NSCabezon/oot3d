extern "C" {
#include "z_en_shopnuts.hpp"

void EnShopnuts_Wait(EnShopnuts* self, PlayState* play);
void EnShopnuts_Stand(EnShopnuts* self, PlayState* play);
void EnShopnuts_ThrowNut(EnShopnuts* self, PlayState* play);

void Animation_PlayOnceSetSpeed(SkelAnime* skelAnime, s32 idx, f32 speed);
void Animation_MorphToLoop(SkelAnime* skelAnime, s32 idx, f32 morph);

extern s32 EnShopnuts_sAnimIndices[];

// Own TU: keeps armcc from inlining these into their callers (Burrow calls
// EnShopnuts_SetupWait, FUN_00258c08 is called by EnShopnuts_ThrowNut).

void EnShopnuts_SetupWait(EnShopnuts* self) {
    f32 zero = 0.0f;

    Animation_PlayOnceSetSpeed(&self->skelAnime, EnShopnuts_sAnimIndices[4], zero);
    self->animFlagAndTimer = Rand_S16Offset_003702c8(100, 50);
    self->collider.dim.height = zero;
    self->collider.base.acFlags &= ~AC_ON;
    self->actionFunc = EnShopnuts_Wait;
}

// N64 EnShopnuts_SetupPeek. 3DS: timer gets 0x1003 vs 2, actionFunc = EnShopnuts_Stand.
void FUN_00258c08(EnShopnuts* self) {
    f32 morph = -3.0f;

    Animation_MorphToLoop(&self->skelAnime, EnShopnuts_sAnimIndices[5], morph);

    if (self->actionFunc == EnShopnuts_ThrowNut) {
        self->animFlagAndTimer = 0x1003;
    } else {
        self->animFlagAndTimer = 2;
    }

    self->actionFunc = EnShopnuts_Stand;
}

}
