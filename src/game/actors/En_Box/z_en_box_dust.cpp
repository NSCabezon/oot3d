extern "C" {
#include "z_en_box.hpp"

#define R_UPDATE_RATE SREG(30)

void FUN_003356d4(PlayState* play, s32 type, Vec3f* pos, Vec3f* vel, Vec3f* accel, void* a, void* b, s32 c,
                  s32 d, s32 e, s32 f);

void FUN_0027f34c(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel, s32 a, s32 b, s32 c) {
    s32 life = (s32)((f32)c * 3.0f / R_UPDATE_RATE + 0.5f);

    FUN_003356d4(play, 4, pos, vel, accel, (void*)0x00520234, (void*)0x00520238, a, b, (s16)life, 0);
}

}
