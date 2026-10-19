extern "C" {
#include "z_en_box.hpp"

void EffectSs_Spawn(PlayState* play, s32 type, s32 priority, void* initParams);

typedef struct EnBoxSmokeInit {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ Vec3f velocity;
    /* 0x18 */ Vec3f accel;
    /* 0x24 */ s16 life;
} EnBoxSmokeInit;

void FUN_00290ce0(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 life) {
    EnBoxSmokeInit initParams;

    Math_Vec3f_Copy(&initParams.pos, pos);
    Math_Vec3f_Copy(&initParams.velocity, velocity);
    Math_Vec3f_Copy(&initParams.accel, accel);
    initParams.life = life;
    EffectSs_Spawn(play, 0x24, 0x80, &initParams);
}

}
