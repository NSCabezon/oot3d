extern "C" {
#include "z_bg_jya_haheniron.hpp"

// Own TU: the two guarded function-local statics give this object a .data section holding only the two guards
// (0x005128D8 / 0x005128DC, ROM addresses the second as 0x005128D8 + 4) and a .bss with the two Vec3f
// (0x00569A10 / 0x00569A1C). Both pinned in data/oot3d.ld via `*z_bg_jya_haheniron_crumble.o(+DATA/+BSS)`
// (scripts/pin_static.py); the guards are cut from the data chunk.

void FUN_0036a154(PlayState* play, Vec3f* pos, Vec3f* vel);

void BgJyaHaheniron_PillarCrumble(BgJyaHaheniron* self, PlayState* play) {
    static Vector3f sVec(0.0f, 14.0f, 0.0f);

    if (self->timer > 11) {
        Actor_MoveForward(&self->actor);
    }
    self->actor.shape.rot.y += 0x258;
    self->actor.shape.rot.x += 0x3E8;
}

void BgJyaHaheniron_RubbleCollide(BgJyaHaheniron* self, PlayState* play) {
    static Vector3f sVec(0.0f, 8.0f, 0.0f);

    if (self->timer > 24) {
        FUN_0036a154(play, &self->actor.world.pos, (Vec3f*)&sVec);
        Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 80, 0x0100041C); // NA_SE_EN_IRONNACK_BREAK_PILLAR2
        Actor_Kill(&self->actor);
    }
}

}
