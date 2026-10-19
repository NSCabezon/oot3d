extern "C" {
#include "z_en_zf.hpp"

s32 FUN_0032fbc0(PlayState* play, EnZf* self);
s32 FUN_0032fdf8(PlayState* play, EnZf* self);
void FUN_0032fb3c(EnZf* self, PlayState* play);
void FUN_00326138(EnZf* self, PlayState* play);
s16 FUN_00373fa4(Vec3f* pos, s16 preferred);
s32 FUN_003731e0(SkelAnime* skelAnime);
s32 Actor_IsFacingPlayer(Actor* actor, s16 angle);

void FUN_003806d8(EnZf* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 relYawTowardsPlayer;
    s16 yawSpeed;
    f32 animPlaySpeed;

    if (!FUN_0032fdf8(play, self) && !((self->actor.params == -2) && FUN_0032fbc0(play, self))) {
        relYawTowardsPlayer = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
        if (relYawTowardsPlayer > 0) {
            yawSpeed = (relYawTowardsPlayer * 0.25f) + 2000.0f;
        } else {
            yawSpeed = (relYawTowardsPlayer * 0.25f) - 2000.0f;
        }
        self->actor.shape.rot.y += yawSpeed;
        self->actor.world.rot.y = self->actor.shape.rot.y;
        if (relYawTowardsPlayer > 0) {
            animPlaySpeed = yawSpeed * 1.5f;
            if (animPlaySpeed > 2.0f) {
                animPlaySpeed = 2.0f;
            }
        } else {
            animPlaySpeed = yawSpeed * 1.5f;
            if (animPlaySpeed < -2.0f) {
                animPlaySpeed = -2.0f;
            }
        }
        self->skelAnime.playSpeed = -animPlaySpeed;
        FUN_003731e0(&self->skelAnime);
        if (self->actor.params >= 0) {
            self->curPlatform = FUN_00373fa4(&self->actor.world.pos, self->curPlatform);
            if (FUN_00373fa4(&player->actor.world.pos, -1) != self->curPlatform) {
                FUN_0032fb3c(self, play);
                return;
            }
        }
        if (Actor_IsFacingPlayer(&self->actor, 0x1555)) {
            if (Rand_ZeroOne() > 0.8f) {
                FUN_00326138(self, play);
            } else {
                FUN_0032fb3c(self, play);
            }
        }
        if ((*(u32*)((u8*)play + 0x5BF4) & 0x5F) == 0) {
            Audio_PlayActorSound2(&self->actor, 0x01000299);
        }
    }
}

}
