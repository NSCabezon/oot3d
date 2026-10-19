// Draft (not matched): FUN_00326138 9 words off (VFP numbering of 10.0f/20.0f/3.0f*0.5f timer constants). Kept under #if 0; GLOBAL_ASM stays in functions_320000s.cpp.
extern "C" {
#include "z_en_zf.hpp"

s32 FUN_0035e600(Actor* actor, PlayState* play, f32 dist, s16 yaw);
void FUN_0032fb3c(EnZf* self, PlayState* play);
void FUN_003a9efc(EnZf* self, PlayState* play);
void Animation_PlayLoop(SkelAnime* skelAnime, s32 idx);

void FUN_00326138(EnZf* self, PlayState* play) {
    s32 t;
    EnZf* s2 = self;

    if ((s2->actor.params >= 0) &&
        (FUN_0035e600(&self->actor, play, 40.0f, self->actor.shape.rot.y + 0x3FFF) == 0) &&
        (FUN_0035e600(&self->actor, play, -40.0f, self->actor.shape.rot.y + 0x3FFF) == 0)) {
        FUN_0032fb3c(self, play);
        return;
    }
    Animation_PlayLoop(&self->skelAnime, 12);
    self->actor.speedXZ = Rand_CenteredFloat(12.0f);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    if ((s32)(Rand_ZeroOne() * 10.0f + 20.0f) > 0) {
        t = (s32)(0.5f + (f32)(s32)(Rand_ZeroOne() * 10.0f + 20.0f) * 3.0f * 0.5f);
    } else {
        t = (s32)((f32)(s32)(Rand_ZeroOne() * 10.0f + 20.0f) * 3.0f * 0.5f - 0.5f);
    }
    self->timer = t;
    self->dodgeSpeed = 0.0f;
    self->actionState = 0;
    self->action = 7;
    self->actionFunc = FUN_003a9efc;
}

}
