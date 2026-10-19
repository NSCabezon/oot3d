extern "C" {
#include "global.hpp"

typedef struct DupAnimActor {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
} DupAnimActor;

s32 Animation_GetLastFrame(SkelAnime* skelAnime, void* anim);
void Animation_Change_00375c08(SkelAnime* skelAnime, void* anim, f32 speed, f32 startFrame, f32 endFrame, s32 mode,
                               f32 morphFrames);

void FUN_003411f8(Actor* thisx, void* animation, s32 mode, f32 morphFrames, s32 arg4) {
    DupAnimActor* self = (DupAnimActor*)thisx;
    f32 frameCount;
    f32 playbackSpeed;
    f32 unk0;
    f32 fc;
    SkelAnime* skelAnime = &self->skelAnime;

    frameCount = Animation_GetLastFrame(skelAnime, animation);

    if (!arg4) {
        unk0 = 0.0f;
        fc = frameCount;
        playbackSpeed = 1.0f;
    } else {
        unk0 = frameCount;
        fc = 0.0f;
        playbackSpeed = -1.0f;
    }

    Animation_Change_00375c08(skelAnime, animation, playbackSpeed, unk0, fc, mode, morphFrames);
}

}
