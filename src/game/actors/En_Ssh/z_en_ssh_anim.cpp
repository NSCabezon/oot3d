extern "C" {
#include "z_en_ssh.hpp"

GLOBAL_ASM("../src/game/actors/En_Ssh/data_syms.s")

s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 animIndex);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 animIndex, f32 speed, f32 start, f32 end, s32 mode,
                               f32 morph);
extern s32 EnSsh_sAnimationIndexes[];
extern f32 EnSsh_sPlaybackSpeeds[];

struct EnSshAnimationIndexTable {
    s32 index[7];
};

struct EnSshPlaybackSpeedTable {
    f32 speed[7];
};

#if 0
f32 EnSsh_SetAnimation(EnSsh* self, s32 animIndex) {
    EnSshAnimationIndexTable animation = *(EnSshAnimationIndexTable*)EnSsh_sAnimationIndexes;
    EnSshPlaybackSpeedTable playbackSpeed = *(EnSshPlaybackSpeedTable*)EnSsh_sPlaybackSpeeds;
    u8 mode[] = { 3, 3, 1, 3, 1, 1, 1 };
    f32 frameCount = (f32)Animation_GetLastFrame(&self->skelAnime, animation.index[animIndex]);

    Animation_Change_00375c08(&self->skelAnime, animation.index[animIndex], playbackSpeed.speed[animIndex], 0.0f, frameCount, mode[animIndex],
                              -6.0f);
    return frameCount;
}
#endif

GLOBAL_ASM("asm/EnSsh_SetAnimation.s")

}
