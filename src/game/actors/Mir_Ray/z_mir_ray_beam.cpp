// Separate TU from z_mir_ray.cpp on purpose: MirRay_Destroy tail-calls FUN_00340ac8 (b), armcc would inline it otherwise.
extern "C" {
#include "z_mir_ray.hpp"

void FUN_00340ac8(Player* player, s32 arg1, Vec3f* arg2, Vec3f* arg3, f32 arg4) {
    u8* p = (u8*)player;
    Vec3f* saved;

    *(s32*)(p + 0x29EC) = arg1;
    saved = (Vec3f*)(p + 0x2A20);
    if (arg2 != NULL) {
        *saved = *arg2;
    }
    if (arg3 != NULL) {
        f32 dx = arg3->x - saved->x;
        f32 dz = arg3->z - saved->z;
        *(f32*)(p + 0x2800 + 0x238) = dx;
        *(f32*)(p + 0x2800 + 0x240) = dz;
        *(f32*)(p + 0x2800 + 0x23C) = 0.0f;
    }
    *(f32*)(p + 0x2800 + 0x244) = arg4;
}

}
