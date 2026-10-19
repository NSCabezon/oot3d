// Own TU: the ROM calls SetupIdle out of line from the other EnWf functions.
extern "C" {
#include "z_en_wf.hpp"

void FUN_00362a4c(SkelAnime* skelAnime, void* anim, f32 morph);
void FUN_003e35e0(EnWf* self, PlayState* play);
extern u8 EnWf_sAnimIdle[];

void FUN_00364938(EnWf* self) {
    FUN_00362a4c(&self->skelAnime, EnWf_sAnimIdle, -4.0f);
    self->action = 6;
    self->timer = ((s32)(2.0f + (Rand_ZeroOne() * 10.0f)) > 0)
                      ? (s32)(0.5f + ((f32)((s32)(2.0f + (Rand_ZeroOne() * 10.0f))) * 3.0f) * 0.5f)
                      : (s32)(((f32)((s32)(2.0f + (Rand_ZeroOne() * 10.0f))) * 3.0f) * 0.5f - 0.5f);
    self->actor.speedXZ = 0.0f;
    s16 yaw = self->actor.shape.rot.y;
    self->actor.world.rot.y = yaw;
    self->actionFunc = FUN_003e35e0;
}
}
