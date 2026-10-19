extern "C" {
#include "z_en_dekunuts.hpp"

void Animation_PlayLoop(SkelAnime* skelAnime, s32 idx);
void FUN_00123ef4(EnDekunuts* self, PlayState* play); // N64: EnDekunuts_Run (asm, not yet matched)
void FUN_0013e408(EnDekunuts* self, PlayState* play); // N64: EnDekunuts_Wait (asm, not yet matched)
void Animation_PlayOnceSetSpeed(SkelAnime* skelAnime, s32 idx, f32 speed);
void FUN_001c6b54(EnDekunuts* self, PlayState* play); // N64: EnDekunuts_Stand (in main TU)
void FUN_0011d56c(EnDekunuts* self, PlayState* play); // N64: EnDekunuts_ThrowNut (asm, not yet matched)
void Animation_MorphToLoop(SkelAnime* skelAnime, s32 idx, f32 morph);

// Own TU: keeps armcc from inlining this into its callers (FUN_003e0be8 tail-calls it as `b FUN_001c82dc`).
void FUN_001c82dc(EnDekunuts* self) {
    Animation_PlayLoop(&self->skelAnime, sEnDekunutsAnimData.animIdx24);
    self->animFlagAndTimer = 3;
    self->playWalkSfx = false;
    self->collider.base.acFlags |= AC_ON;
    self->actionFunc = FUN_00123ef4;
}

void FUN_001e8ac0(EnDekunuts* self) {
    f32 zero = 0.0f;

    Animation_PlayOnceSetSpeed(&self->skelAnime, sEnDekunutsAnimData.animUp, zero);
    self->animFlagAndTimer = Rand_S16Offset_003702c8(100, 50);
    self->collider.dim.height = zero;
    Math_Vec3f_Copy(&self->actor.world.pos, &self->actor.home.pos);
    self->collider.base.acFlags &= ~AC_ON;
    self->actionFunc = FUN_0013e408;
}

void FUN_00258b58(EnDekunuts* self) {
    f32 morph = -3.0f;

    Animation_MorphToLoop(&self->skelAnime, sEnDekunutsAnimData.animStand, morph);
    if (self->actionFunc == FUN_0011d56c) {
        self->animFlagAndTimer = 0x1003;
    } else {
        self->animFlagAndTimer = 2;
    }
    self->actionFunc = FUN_001c6b54;
}

}
