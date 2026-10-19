extern "C" {
#include "z_en_daiku_kakariko.hpp"

extern EnDaikuKakarikoAnimInfo EnDaikuKakariko_sAnimationInfo[];
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);

void EnDaikuKakariko_SetAnimFromIndex(EnDaikuKakariko* self, s32 index, s32* currentIndex) {
    f32 morphFrames;

    if ((*currentIndex < 0) || (index == *currentIndex)) {
        morphFrames = 0.0f;
    } else {
        morphFrames = EnDaikuKakariko_sAnimationInfo[index].morphFrames;
    }
    Animation_Change_00375c08(&self->skelAnime, EnDaikuKakariko_sAnimationInfo[index].animation, 1.0f, 0.0f,
                              Animation_GetLastFrame(&self->skelAnime, EnDaikuKakariko_sAnimationInfo[index].animation),
                              EnDaikuKakariko_sAnimationInfo[index].mode, morphFrames);
    *currentIndex = index;

}

}
