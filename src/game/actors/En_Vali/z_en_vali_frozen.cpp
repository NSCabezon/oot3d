extern "C" {
#include "z_en_vali.hpp"

void FUN_00364538(EnVali* self, PlayState* play);
void EffectSsEnIce_SpawnFlyingVec3f(PlayState* play, Actor* actor, Vec3f* pos, s32 a, s32 b, s32 c, s32 d, s32 e,
                                    s32 f, s32 g, f32 scale);
f32 Rand_ZeroOne(void);

// Actor::colorFilterTimer is not in the repo Actor struct yet (raw offset 0x11A on 3DS).
static inline void EnVali_SetColorFilterTimer(EnVali* self, s16 value) {
    *(s16*)((u8*)&self->actor + 0x11A) = value;
}

// FUN_001565fc (EnVali_Frozen) draft, NOT applied: 13/86 words differ (ColorFilter value is 54 = 36*3/2, timer
// reload fixed with `tt` local). Left: ROM keeps play in r6 / &actor+0x100 in r5 (ours swapped) and the reloaded timer in
// r0 with temp in r1 (ours r1/r0). ~45 variants (dead aliases, play2, cf pointer, sig, decl order). GLOBAL_ASM stays.
#if 0
void FUN_001565fc(EnVali* self, PlayState* play) {
    Vec3f effPos;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 tt;

    if (self->timer != 0) {
        self->timer--;
    }
    EnVali_SetColorFilterTimer(self, 54);
    tt = self->timer;
    temp_v1 = tt - 30;
    if (temp_v1 > 0) {
        temp_v0_2 = temp_v1 >> 1;
        if (self->timer & 1) {
            effPos.y = (self->actor.world.pos.y - 20.0f) + ((8 - temp_v0_2) * 5);
            effPos.x = self->actor.world.pos.x + ((temp_v0_2 & 2) ? 12.0f : -12.0f);
            effPos.z = self->actor.world.pos.z + ((temp_v0_2 & 1) ? 12.0f : -12.0f);
            EffectSsEnIce_SpawnFlyingVec3f(play, &self->actor, &effPos, 150, 150, 150, 250, 235, 245, 255,
                                           (Rand_ZeroOne() * 0.2f) + 1.3f);
        }
    } else if (self->timer == 0) {
        self->actor.velocity.y += 1.0f;
        if (Math_StepToF(&self->actor.world.pos.y, self->actor.floorHeight, self->actor.velocity.y)) {
            FUN_00364538(self, play);
            EnVali_SetColorFilterTimer(self, 0);
        }
    }
}
#endif

}
