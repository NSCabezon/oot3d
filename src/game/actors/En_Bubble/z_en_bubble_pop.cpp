extern "C" {
#include "z_en_bubble.hpp"

s16 Rand_S16Offset(s16 base, s16 range);
void EffectSsDtBubble_SpawnCustomColor(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel, Color_RGBA8* prim,
                                       Color_RGBA8* env, s16 scale, s16 life, s16 colorIdx);
void Item_DropCollectibleRandom(PlayState* play, Actor* fromActor, Vec3f* pos, s32 params);

static Color_RGBA8 sEffectPrimColor = { 255, 255, 255, 255 };
static Color_RGBA8 sEffectEnvColor = { 150, 150, 150, 0 };

static inline s32 EnBubble_Explosion(EnBubble* self, PlayState* play) {
    s32 i;
    Vector3f effectAccel(0.0f, -0.5f, 0.0f);
    Vec3f effectVel;
    Vec3f effectPos;
    Math_SmoothStepToF(&self->expansionWidth, 4.0f, 0.1f, 1000.0f, 0.0f);
    Math_SmoothStepToF(&self->expansionHeight, 4.0f, 0.1f, 1000.0f, 0.0f);
    Math_SmoothStepToF(&self->graphicRotSpeed, 54.0f, 0.1f, 1000.0f, 0.0f);
    Math_SmoothStepToF(&self->graphicEccentricity, 0.2f, 0.1f, 1000.0f, 0.0f);
    self->actor.shape.yOffset = ((self->expansionHeight + 1.0f) * 16.0f);

    if (DECR(self->explosionCountdown) != 0) {
        return -1;
    }
    effectPos.x = self->actor.world.pos.x;
    effectPos.y = self->actor.world.pos.y + self->actor.shape.yOffset;
    effectPos.z = self->actor.world.pos.z;
    for (i = 0; i < 20; i++) {
        effectVel.x = (Rand_ZeroOne() - 0.5f) * 7.0f;
        effectVel.y = Rand_ZeroOne() * 7.0f;
        effectVel.z = (Rand_ZeroOne() - 0.5f) * 7.0f;
        EffectSsDtBubble_SpawnCustomColor(play, &effectPos, &effectVel, (Vec3f*)&effectAccel, &sEffectPrimColor,
                                          &sEffectEnvColor, Rand_S16Offset(100, 50), 0x19, 0);
    }
    Item_DropCollectibleRandom(play, NULL, &self->actor.world.pos, 0x50);
    self->actor.flags &= ~1;
    return Rand_S16Offset_003702c8(90, 60);
}

void EnBubble_Pop(EnBubble* self, PlayState* play) {
    if (EnBubble_Explosion(self, play) >= 0) {
        Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 60, 0x010003B9);
        Actor_Kill(&self->actor);
    }
}

}
