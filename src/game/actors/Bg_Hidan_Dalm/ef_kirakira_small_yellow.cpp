extern "C" {
#include "global.hpp"

// Kept in its own translation unit so armcc does not inline it into the actor (matches the original call shape).
void EffectSsKiraKira_SpawnDispersed(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel,
                                     Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s32 life);

// Color pair in the ROM at 0x004D916C (EQU in actor data_syms.s): pad word, primColor, envColor.
extern const struct {
    s32 pad;
    Color_RGBA8 primColor;
    Color_RGBA8 envColor;
} EffectSsKiraKira_sSmallYellowColors;

void EffectSsKiraKira_SpawnSmallYellow(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel) {
    Color_RGBA8 primColor = EffectSsKiraKira_sSmallYellowColors.primColor;
    Color_RGBA8 envColor = EffectSsKiraKira_sSmallYellowColors.envColor;

    EffectSsKiraKira_SpawnDispersed(play, pos, velocity, accel, &primColor, &envColor, 1000, 16);
}
}
