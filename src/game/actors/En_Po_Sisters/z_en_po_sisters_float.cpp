extern "C" {
#include "z_en_po_sisters.hpp"

// Own TU: FUN_0036efa8 is a tail-call target of FUN_003e7a78 in the ROM; in the same TU armcc inlines it.
s16 Rand_S16Offset_003702c8(s16 base, s16 range);
void Animation_MorphToLoop(SkelAnime* skelAnime, s32 idx, f32 morph);
void FUN_003ca1e4(EnPoSisters* self, PlayState* play);
void FUN_00107f3c(EnPoSisters* self, PlayState* play);

void FUN_0036efa8(EnPoSisters* self) {
    if (self->actionFunc != (EnPoSistersActionFunc)FUN_00107f3c) {
        Animation_MorphToLoop(&self->skelAnime, 2, -3.0f);
    }
    self->unk_19A = Rand_S16Offset_003702c8(0xF, 3);
    self->unk_199 |= 7;
    self->actionFunc = (EnPoSistersActionFunc)FUN_003ca1e4;
}

}
