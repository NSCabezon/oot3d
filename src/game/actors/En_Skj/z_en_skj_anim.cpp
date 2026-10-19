extern "C" {
#include "z_en_skj.hpp"

s32 Animation_GetLastFrame(SkelAnime* skelAnime, void* anim);
void Animation_Change_00375c08(SkelAnime* skelAnime, void* anim, f32 speed, f32 start, f32 end, s32 mode, f32 morph);

extern EnSkjAnimInfo EnSkj_sAnimationInfo[];

void FUN_003686a8(EnSkj* self, u8 index) {
    f32 endFrame = Animation_GetLastFrame((SkelAnime*)&self->unk_1A4, EnSkj_sAnimationInfo[index].animation);

    self->animIndex = index;
    Animation_Change_00375c08((SkelAnime*)&self->unk_1A4, EnSkj_sAnimationInfo[index].animation, 1.0f, 0.0f, endFrame,
                              EnSkj_sAnimationInfo[index].mode, EnSkj_sAnimationInfo[index].morphFrames);
}

}
