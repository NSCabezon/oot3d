extern "C" {
#include "z_en_goma.hpp"

// Own TU: ROM tail-calls SetupStand from Land, armcc would inline it into the same TU.
extern EnGomaData gEnGomaData; // 0x00525E58 (EQU in data_syms.s)

void Animation_Change_00375c08(SkelAnime* skelAnime, void* anim, f32 speed, f32 startFrame, f32 endFrame, s32 mode,
                               f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, void* anim);
void FUN_003e44e0(EnGoma* self, PlayState* play); // Stand

void FUN_001807e4(EnGoma* self) {
    f32 lastFrame = Animation_GetLastFrame(&self->skelanime, gEnGomaData.anim[0]);

    self->actionTimer = Rand_S16Offset_003702c8(10, 30);
    Animation_Change_00375c08(&self->skelanime, gEnGomaData.anim[0], 1.0f, 0.0f, lastFrame, 0, -5.0f);
    self->actionFunc = FUN_003e44e0;
    self->gomaType = 0;
}
}
