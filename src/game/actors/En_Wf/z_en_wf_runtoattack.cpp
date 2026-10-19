// Own TU: the ROM calls FUN_0034c440 out of line (bl from the backflip action), so armcc must not inline it
// into z_en_wf.cpp.
extern "C" {
#include "z_en_wf.hpp"

void FUN_0013d5d0(EnWf* self, PlayState* play);
void FUN_00353020(SkelAnime* skelAnime, void* anim, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
extern u8 EnWf_sAnimRun[];

void FUN_0034c440(EnWf* self, PlayState* play) {
    FUN_00353020(&self->skelAnime, EnWf_sAnimRun, 1.0f, 0.0f,
                 (f32)Animation_GetLastFrame(&self->skelAnime, 1), 1, -4.0f);
    self->action = 9;
    self->actionFunc = FUN_0013d5d0;
}
}
