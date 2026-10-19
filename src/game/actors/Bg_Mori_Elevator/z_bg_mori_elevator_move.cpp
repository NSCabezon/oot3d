// Bg_Mori_Elevator movement actions. Separate TU from z_bg_mori_elevator.cpp on purpose: the ROM code calls
// FUN_003600e4 (func_808A1800) / FUN_003600d4 (SetupSetPosition) as opaque externs (see note in main .cpp).
extern "C" {
#include "z_bg_mori_elevator.hpp"

void FUN_003600d4(BgMoriElevator* self); // N64: BgMoriElevator_SetupSetPosition
f32 FUN_003600e4(f32* pValue, f32 target, f32 scale, f32 maxStep, f32 minStep); // N64: func_808A1800
void FUN_0036ef10(PosRot* pos, s32 sfxId, f32 volume); // N64: func_800F436C
s32 FUN_0036e980(PlayState* play, Actor* actor, u8 arg2);

extern __pure float _fabsf(float);
static __inline __pure float BgMoriElevator_Fabsf(float f) {
    return _fabsf(f);
}

void FUN_0012adb4(BgMoriElevator* self, PlayState* play) { // N64: BgMoriElevator_MoveIntoGround
    f32 distTo;

    FUN_003600e4(&self->dyna.actor.velocity.y, 2.0f, 0.05f, 1.0f, 0.0f);
    distTo = FUN_003600e4(&self->dyna.actor.world.pos.y, 73.0f, 0.08f, self->dyna.actor.velocity.y, 1.5f);
    if (BgMoriElevator_Fabsf(distTo) < 0.001f) {
        FUN_003600d4(self);
        Audio_PlayActorSound2(&self->dyna.actor, 0x010001D1); // NA_SE_EV_ELEVATOR_STOP
    } else {
        f32 temp = BgMoriElevator_Fabsf(distTo) * 0.09f; // N64: func_808A18FC (inlined)
        FUN_0036ef10(&self->dyna.actor.world, 0x010001D0, CLAMP(temp, 0.0f, 1.0f));
    }
}

void FUN_0012b7e8(BgMoriElevator* self, PlayState* play) { // N64: BgMoriElevator_MoveAboveGround
    f32 distTo;

    FUN_003600e4(&self->dyna.actor.velocity.y, 2.0f, 0.05f, 1.0f, 0.0f);
    distTo = FUN_003600e4(&self->dyna.actor.world.pos.y, 233.0f, 0.08f, self->dyna.actor.velocity.y, 1.5f);
    if (BgMoriElevator_Fabsf(distTo) < 0.001f) {
        FUN_003600d4(self);
        Audio_PlayActorSound2(&self->dyna.actor, 0x010001D1); // NA_SE_EV_ELEVATOR_STOP
    } else {
        f32 temp = BgMoriElevator_Fabsf(distTo) * 0.09f; // N64: func_808A18FC (inlined)
        FUN_0036ef10(&self->dyna.actor.world, 0x010001D0, CLAMP(temp, 0.0f, 1.0f));
    }
}

void FUN_003f73b4(BgMoriElevator* self, PlayState* play) { // N64: func_808A2008 (StopMovement action)
    f32 distTo;

    FUN_003600e4(&self->dyna.actor.velocity.y, 12.0f, 0.1f, 1.0f, 0.0f);
    distTo = FUN_003600e4(&self->dyna.actor.world.pos.y, self->targetY, 0.1f, self->dyna.actor.velocity.y, 0.3f);
    if (BgMoriElevator_Fabsf(distTo) < 0.001f) {
        FUN_003600d4(self);
        Audio_PlayActorSound2(&self->dyna.actor, 0x010001D1); // NA_SE_EV_ELEVATOR_STOP
        FUN_0036e980(play, &self->dyna.actor, 7);
    } else {
        f32 temp = BgMoriElevator_Fabsf(distTo) * 0.09f; // N64: func_808A18FC (inlined)
        FUN_0036ef10(&self->dyna.actor.world, 0x010001D0, CLAMP(temp, 0.0f, 1.0f));
    }
}
}
