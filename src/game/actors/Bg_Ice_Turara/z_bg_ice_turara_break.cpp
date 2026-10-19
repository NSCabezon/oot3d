extern "C" {
#include "z_bg_ice_turara.hpp"

// Own TU: the guarded function-local statics give this object a .data section [guard 0x00512160, primColor,
// envColor] (ROM: env literal, prim = env - 4) and a .bss with accel (0x005699F8). Pinned in data/oot3d.ld via
// `*z_bg_ice_turara_break.o(+DATA/+BSS)` (scripts/pin_static.py); those 12 bytes are cut from the data chunk.

void EffectSsEnIce_Spawn(PlayState* play, Vec3f* pos, f32 scale, Vec3f* velocity, Vec3f* accel,
                         Color_RGBA8* primColor, Color_RGBA8* envColor, s32 life);

void BgIceTurara_Break(BgIceTurara* self, PlayState* play, f32 arg2) {
    static Vector3f accel(0.0f, -1.0f, 0.0f);
    static Color_RGBA8 primColor = { 170, 255, 255, 255 };
    static Color_RGBA8 envColor = { 0, 50, 100, 255 };
    Vec3f vel;
    Vec3f pos;
    s32 j;
    s32 i;

    Audio_PlaySoundAtPosition(play, &self->dyna.actor.world.pos, 30, 0x01000222); // NA_SE_EV_ICE_BROKEN
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 10; j++) {
            pos.x = self->dyna.actor.world.pos.x + Rand_CenteredFloat(8.0f);
            pos.y = self->dyna.actor.world.pos.y + (Rand_ZeroOne() * arg2) + (i * arg2);
            pos.z = self->dyna.actor.world.pos.z + Rand_CenteredFloat(8.0f);

            vel.x = Rand_CenteredFloat(7.0f);
            vel.z = Rand_CenteredFloat(7.0f);
            vel.y = (Rand_ZeroOne() * 4.0f) + 8.0f;

            EffectSsEnIce_Spawn(play, &pos, (Rand_ZeroOne() * 0.2f) + 0.1f, &vel, (Vec3f*)&accel, &primColor, &envColor, 30);
        }
    }
}

}
