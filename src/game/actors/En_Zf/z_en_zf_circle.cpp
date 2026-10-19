extern "C" {
#include "z_en_zf.hpp"

s32 FUN_0035e600(Actor* actor, PlayState* play, f32 dist, s16 yaw);
void FUN_002b53c4(EnZf* self, PlayState* play);
void FUN_0032fb3c(EnZf* self, PlayState* play);
void Animation_PlayLoop(SkelAnime* skelAnime, s32 idx);

void FUN_0032ffc0(EnZf* self, PlayState* play) {
    s16 playerYaw;
    Player* player;
    s32 t;

    if ((self->actor.params >= 0) &&
        (FUN_0035e600(&self->actor, play, 40.0f, self->actor.shape.rot.y + 0x3FFF) == 0) &&
        (FUN_0035e600(&self->actor, play, -40.0f, self->actor.shape.rot.y + 0x3FFF) == 0)) {
        FUN_0032fb3c(self, play);
        return;
    }
    Animation_PlayLoop(&self->skelAnime, 12);
    player = GET_PLAYER(play);
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0xFA0, 1);
    playerYaw = player->actor.shape.rot.y;
    if (Math_SinS(playerYaw - self->actor.shape.rot.y) >= 0.0f) {
        self->actor.speedXZ = -6.0f;
    } else if (Math_SinS(playerYaw - self->actor.shape.rot.y) < 0.0f) {
        self->actor.speedXZ = 6.0f;
    }
    self->actor.world.rot.y = self->actor.shape.rot.y + 0x3FFF;
    self->dodgeSpeed = 0.0f;
    self->actionState = 0;
    if ((s32)(Rand_ZeroOne() * 10.0f + 5.0f) > 0) {
        t = (s32)(0.5f + (f32)(s32)(Rand_ZeroOne() * 10.0f + 5.0f) * 3.0f * 0.5f);
    } else {
        t = (s32)((f32)(s32)(Rand_ZeroOne() * 10.0f + 5.0f) * 3.0f * 0.5f - 0.5f);
    }
    self->timer = t;
    self->action = 12;
    self->actionFunc = FUN_002b53c4;
}

}
