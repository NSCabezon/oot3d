extern "C" {
#include "z_en_heishi1.hpp"

void EffectSs_Spawn(PlayState* play, s32 type, s32 priority, void* initParams);

typedef struct {
    Vec3f pos;
    Vec3f velocity;
    Vec3f accel;
    s16 unk_24;
    s32 unk_28;
} EnHeishi1EffectParams;

void FUN_003902e0(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 arg4, s32 arg5) {
    EnHeishi1EffectParams params;

    Math_Vec3f_Copy(&params.pos, pos);
    Math_Vec3f_Copy(&params.velocity, velocity);
    Math_Vec3f_Copy(&params.accel, accel);
    params.unk_24 = arg4;
    params.unk_28 = arg5;
    EffectSs_Spawn(play, 0x18, 0x80, &params);
}

}
