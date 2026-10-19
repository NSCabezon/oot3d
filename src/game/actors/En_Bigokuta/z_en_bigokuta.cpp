extern "C" {
#include "z_en_bigokuta.hpp"

#define FLAGS 0x00000035

void EnBigokuta_Init(Actor* actor, GameState* state);
void EnBigokuta_Destroy(Actor* thisx, PlayState* play);
void EnBigokuta_Update(Actor* actor, GameState* state);
void EnBigokuta_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_Bigokuta.data_00520D58.s")
GLOBAL_ASM("data/z_En_Bigokuta.data_00520E40.s")

ActorInit En_Bigokuta_InitVars = {
    ACTOR_EN_BIGOKUTA,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_BIGOKUTA,
    sizeof(EnBigokuta),
    (ActorFunc)EnBigokuta_Init,
    (ActorFunc)EnBigokuta_Destroy,
    (ActorFunc)EnBigokuta_Update,
    (ActorFunc)EnBigokuta_Draw,
};

void Collider_DestroyJntSph(PlayState* play, void* collider);

void EnBigokuta_Destroy(Actor* thisx, PlayState* play) {
    EnBigokuta* self = (EnBigokuta*)thisx;
    s32 i;

    Collider_DestroyJntSph(play, &self->collider);
    for (i = 0; i < ARRAY_COUNT(self->colliderCylinders); i++) {
        Collider_DestroyCylinder(play, &self->colliderCylinders[i]);
    }
    SkelAnime_Free2(&self->skelAnime);
    FUN_003508b8(thisx, 0);
}


void FUN_00362998(EnBigokuta* self);
s32 Math_StepToF(f32* pValue, f32 target, f32 step);

void FUN_003b9cb0(EnBigokuta* self) {
    if (Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y, 10.0f)) {
        self->actor.flags |= 1;
        FUN_00362998(self);
    }
}

void FUN_0034f724(PlayState* play);
void FUN_00362998(EnBigokuta* self);

void FUN_003e6250(EnBigokuta* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if (self->unk_196 != 0) {
        self->unk_196--;
    }
    if (self->unk_196 == 0) {
        if (self->actor.params == 3) {
            FUN_0034f724(play);
        }
        FUN_00362998(self);
    }
}

void FUN_00199b8c(EnBigokuta* self, PlayState* play);

void FUN_00199e90(EnBigokuta* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    self->unk_196--;
    if (self->unk_196 == 20 || self->unk_196 == -30) {
        Audio_PlayActorSound2(&self->actor, 0x01000376);
    }
    if (self->unk_196 == 1) {
        FUN_0034f724(play);
    }
    if (self->actor.params == 1) {
        self->unk_196 = 32;
        Audio_PlayActorSound2(&self->actor, 0x010002DC);
        self->actionFunc = FUN_00199b8c;
    }
}

s32 Math_ScaledStepToS(s16* pValue, s16 target, s16 scale);
void FUN_003e6250(EnBigokuta* self, PlayState* play);

void FUN_00199da4(EnBigokuta* self, PlayState* play) {
    if (Math_ScaledStepToS(&self->actor.shape.rot.y, self->actor.home.rot.y + 0x4000, 0x400)) {
        if (self->unk_196 != 0) {
            self->unk_196--;
        }
        if (self->unk_196 == 0) {
            func_809BCE3C(self);
            self->actor.home.pos.y = self->actor.world.pos.y;
            Actor_ChangeType(play, &play->actorCtx, &self->actor, ACTORCAT_ENEMY);
            self->actor.params = 2;
            Audio_PlayActorSound2(&self->actor, 0x01000376);
            self->unk_196 = 40 * 3.0f / 2.0f;
            self->actionFunc = FUN_003e6250;
        }
    }
}

GLOBAL_ASM("asm/EnBigokuta_Init.s")

GLOBAL_ASM("asm/EnBigokuta_Update.s")

GLOBAL_ASM("asm/EnBigokuta_Draw.s")
}
