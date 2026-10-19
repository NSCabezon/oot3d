extern "C" {
#include "z_en_zf.hpp"

void FUN_0024fa94(EnZf* self, PlayState* play);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);

void FUN_0034eb00(EnZf* self) {
    Animation_Change_00375c08(&self->skelAnime, 4, 1.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 4), 1, -4.0f);
    s32 t;

    self->action = 3;
    if ((s32)(Rand_ZeroOne() * 10.0f + 5.0f) > 0) {
        t = (s32)(0.5f + (f32)(s32)(Rand_ZeroOne() * 10.0f + 5.0f) * 3.0f * 0.5f);
    } else {
        t = (s32)((f32)(s32)(Rand_ZeroOne() * 10.0f + 5.0f) * 3.0f * 0.5f - 0.5f);
    }
    self->timer = t;
    self->actor.speedXZ = 0.0f;
    {
        s16 yaw = self->actor.shape.rot.y;
        self->actor.world.rot.y = yaw;
    }
    self->actionFunc = FUN_0024fa94;
}

}
