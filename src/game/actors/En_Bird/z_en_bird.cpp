extern "C" {
#include "z_en_bird.hpp"

#define FLAGS 0x80000000

#include <math.h>
void EnBird_Init(EnBird* self, PlayState* play);
void EnBird_Destroy(EnBird* self, PlayState* play);
void EnBird_Update(EnBird* self, PlayState* play);
void EnBird_Draw(EnBird* self, PlayState* play);

GLOBAL_ASM("data/z_En_Bird.data.s")

ActorInit En_Bird_InitVars = {
    ACTOR_EN_BIRD,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_BIRD,
    sizeof(EnBird),
    (ActorFunc)EnBird_Init,
    (ActorFunc)EnBird_Destroy,
    (ActorFunc)EnBird_Update,
    (ActorFunc)EnBird_Draw,
};

void FUN_001d51e8(EnBird* self, PlayState* play); // N64: EnBird_Idle
void FUN_002479f8(EnBird* self, PlayState* play); // N64: EnBird_Move
__attribute__((noinline)) void FUN_002667dc(EnBird* self, s16 params); // N64: EnBird_SetupIdle

s16 Math_StepToAngleS(s16* pValue, s16 target, s16 step);

void EnBird_Init(EnBird* self, PlayState* play) {
    Actor_ProcessInitChain(&self->actor, (InitChainEntry*)0x00520f2c);
    Actor_SetScale(&self->actor, 0.01f);
    ActorShape_Init(&self->actor.shape, 5500.0f, ActorShadow_DrawCircle, 4.0f);
    self->unk_1A8 = 0;
    self->timer = 0;
    self->posYMag = 0.0f;
    self->rotYMag = 0.0f;
    self->speedTarget = 1.5f;
    self->speedStep = 0.5f;
    self->flightDistance = 40.0f;
    self->posYPhaseStep = 0.0f;
    self->unk_1D0 = 70.0f;
    self->rotYStep = 2500;
    self->actor.colChkInfo.mass = 0;
    f32 playbackSpeed = self->unk_1B0 ? 0.0f : 1.0f;
    self->timer = Rand_S16Offset_003702c8(5, 35);
    self->actionFunc = FUN_001d51e8;
}

void EnBird_Destroy(EnBird* self, PlayState* play) {
}

void EnBird_Update(EnBird* self, PlayState* play) {
    self->posYPhase += self->posYPhaseStep;
    self->actionFunc(self, play);
}

void EnBird_Draw(EnBird* self, PlayState* play) {
}

void FUN_001d51e8(EnBird* self, PlayState* play) {
    self->actor.shape.yOffset += sinf(self->posYPhase) * self->posYMag;
    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 0.1f, 0.5f, 0.0f);
    self->timer--;
    if (self->timer <= 0) {
        self->timer = Rand_S16Offset_003702c8(20, 45);
        self->actionFunc = FUN_002479f8;
    }
}

void FUN_002479f8(EnBird* self, PlayState* play) {
    self->actor.shape.yOffset += sinf(self->posYPhase) * self->posYMag;
    Math_SmoothStepToF(&self->actor.speedXZ, self->speedTarget, 0.1f, self->speedStep, 0.0f);
    f32 dx = self->actor.home.pos.x - self->actor.world.pos.x;
    f32 dz = self->actor.home.pos.z - self->actor.world.pos.z;
    if (self->flightDistance < __sqrtf(SQ(dx) + SQ(dz)) || self->timer < 4) {
        Math_StepToAngleS(&self->actor.world.rot.y, Math_Vec3f_Yaw(&self->actor.world.pos, &self->actor.home.pos),
                          self->rotYStep);
    } else {
        self->actor.world.rot.y += (s16)(sinf(self->posYPhase) * self->rotYMag);
    }
    self->actor.shape.rot.y = self->actor.world.rot.y;
    self->timer--;
    if (self->timer < 0) {
        FUN_002667dc(self, self->actor.params);
    }
}

void FUN_002667dc(EnBird* self, s16 params) {
    f32 playbackSpeed = self->unk_1B0 ? 0.0f : 1.0f;
    self->timer = Rand_S16Offset_003702c8(5, 35);
    self->actionFunc = FUN_001d51e8;
}
}
