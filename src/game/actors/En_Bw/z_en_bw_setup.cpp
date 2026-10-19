// FUN_00350348 is called out of line (bl) by the ROM from FUN_0027fd24 and EnBw_Init: keep it in its own TU so armcc
// does not inline it into the callers (same-file inlining changes their frame and register allocation).
extern "C" {
#include "z_en_bw.hpp"

void Animation_MorphToLoop(SkelAnime* skelAnime, s32 idx, f32 morph);
void FUN_002483fc(EnBw* self, PlayState* play);

void FUN_00350348(EnBw* self) {
    Animation_MorphToLoop(&self->skelAnime, 0, -2.0f);
    self->unk_444 = 2;
    self->unk_446 = (Rand_ZeroOne() * 200.0f + 200.0f) > 0.0f
                        ? (s32)(0.5f + ((Rand_ZeroOne() * 200.0f + 200.0f) * 3.0f) * 0.5f)
                        : (s32)(((Rand_ZeroOne() * 200.0f + 200.0f) * 3.0f) * 0.5f - 0.5f);
    self->actor.speedXZ = 0.0f;
    self->unk_456 = 0;
    self->actionFunc = FUN_002483fc;
}
}
