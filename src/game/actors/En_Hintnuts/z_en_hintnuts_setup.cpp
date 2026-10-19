extern "C" {
#include "z_en_hintnuts.hpp"

void Animation_PlayOnceSetSpeed(SkelAnime* skelAnime, s32 idx, f32 speed);
void EnHintnuts_Wait(EnHintnuts* self, PlayState* play);
void EnHintnuts_Stand(EnHintnuts* self, PlayState* play);
void EnHintnuts_ThrowNut(EnHintnuts* self, PlayState* play);
void Animation_MorphToLoop(SkelAnime* skelAnime, s32 idx, f32 morph);

// Own TU: ROM calls this out of line (bl from EnHintnuts_Burrow); armcc would inline it in the main TU.
void FUN_001e8b34(EnHintnuts* self) {
    f32 zero = 0.0f;

    Animation_PlayOnceSetSpeed(&self->skelAnime, EnHintnuts_sData.upAnim, zero);
    self->animFlagAndTimer = Rand_S16Offset_003702c8(100, 50);
    self->collider.dim.height = zero;
    self->actor.world.pos = self->actor.home.pos;
    self->collider.base.acFlags &= ~AC_ON;
    self->actionFunc = EnHintnuts_Wait;
}

// Own TU too (ROM tail-calls it from EnHintnuts_ThrowNut): keeps armcc from inlining it into the main TU.
void FUN_00258bb0(EnHintnuts* self) {
    f32 morph = -3.0f;

    Animation_MorphToLoop(&self->skelAnime, EnHintnuts_sData.standAnim, morph);
    if (self->actionFunc == EnHintnuts_ThrowNut) {
        self->animFlagAndTimer = 0x1003;
    } else {
        self->animFlagAndTimer = 3;
    }
    self->actionFunc = EnHintnuts_Stand;
}

}
