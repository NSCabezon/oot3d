extern "C" {
#include "z_en_zf.hpp"

void FUN_0024f78c(EnZf* self, PlayState* play);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);


void FUN_003301b8(EnZf* self) {
    Animation_Change_00375c08(&self->skelAnime, 3, 1.25f, 0.0f, (f32)Animation_GetLastFrame(&self->skelAnime, 3), 2, -4.0f);
    if (self->actor.params == -2) {
        self->skelAnime.playSpeed = 1.75f;
    }
    self->swordCollider.atFlags &= ~4;
    self->action = 9;
    Audio_PlayActorSound2(&self->actor, 0x01000299);
    self->actor.speedXZ = 0.0f;
    self->actionFunc = FUN_0024f78c;
}

}
