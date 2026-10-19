extern "C" {
#include "z_en_wf.hpp"

void Animation_Change(SkelAnime* skelAnime, void* anim, f32 speed, f32 start, f32 end, s32 mode, f32 morph, s32 unk);
s32 FUN_0036ae18(SkelAnime* skelAnime, void* anim);

static inline void EnWf_AnimChangeToLast(SkelAnime* skelAnime, void* anim, f32 speed, f32 start, f32 morph) {
    Animation_Change(skelAnime, anim, speed, start, (f32)FUN_0036ae18(skelAnime, anim), 2, morph, 0);
}

void FUN_003ab92c(SkelAnime* skelAnime, void** animPtr, f32 speed) {
    EnWf_AnimChangeToLast(skelAnime, *animPtr, speed, 0.0f, 0.0f);
}
}
