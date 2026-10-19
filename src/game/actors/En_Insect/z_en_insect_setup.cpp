extern "C" {
#include "z_en_insect.hpp"

// Setup helpers live in their own TU so armcc does not inline them into the action functions
// (the ROM calls them as out-of-line tails from FUN_003cc0c4 / FUN_003ba2a0 / FUN_003be100).
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void Math_Vec3f_Copy(Vec3f* dest, Vec3f* src);
s16 Rand_S16Offset_003702c8(s16 base, s16 range);
void FUN_003bcadc(EnInsect* self, PlayState* play); // EnInsect_Dig
void FUN_003be100(EnInsect* self, PlayState* play); // EnInsect_SlowDown
void FUN_003bfe54(EnInsect* self, PlayState* play); // EnInsect_WalkOnWater
void FUN_003ba2a0(EnInsect* self, PlayState* play); // EnInsect_RunFromPlayer

void FUN_00352c20(EnInsect* self) { // EnInsect_SetupDig
    self->actionTimer = EN_INSECT_FRAMES(60.0f);
    Animation_Change_00375c08(&self->skelAnime, 10, 1.0f, 0.0f, 0.0f, 1, 0.0f);
    self->skelAnime.playSpeed = 1.9f * 2.0f / 3.0f;
    Audio_PlayActorSound2(&self->actor, 0x01000459); // NA_SE_EN_MUSI_SINK
    Math_Vec3f_Copy(&self->actor.home.pos, &self->actor.world.pos);
    self->actionFunc = FUN_003bcadc;
    self->insectFlags &= ~INSECT_FLAG_CRAWLING;
    self->insectFlags |= INSECT_FLAG_UNCATCHABLE;
}

void FUN_00352b58(EnInsect* self) { // EnInsect_SetupSlowDown
    self->actionTimer = Rand_S16Offset_003702c8(EN_INSECT_FRAMES(5.0f), EN_INSECT_FRAMES(35.0f));
    Animation_Change_00375c08(&self->skelAnime, 10, 1.0f, 0.0f, 0.0f, 1, 0.0f);
    self->actionFunc = FUN_003be100;
    self->insectFlags |= INSECT_FLAG_CRAWLING;
}

void FUN_00352ce8(EnInsect* self) { // EnInsect_SetupWalkOnWater
    self->actionTimer = Rand_S16Offset_003702c8(EN_INSECT_FRAMES(120.0f), EN_INSECT_FRAMES(50.0f));
    Animation_Change_00375c08(&self->skelAnime, 10, 1.0f, 0.0f, 0.0f, 1, 0.0f);
    self->unk_318 = 0;
    self->unk_316 = 0;
    self->actionFunc = FUN_003bfe54;
    self->insectFlags &= ~INSECT_FLAG_CRAWLING;
}

void FUN_00317908(EnInsect* self) { // EnInsect_SetupRunFromPlayer
    self->actionTimer = Rand_S16Offset_003702c8(EN_INSECT_FRAMES(10.0f), EN_INSECT_FRAMES(40.0f));
    Animation_Change_00375c08(&self->skelAnime, 10, 1.0f, 0.0f, 0.0f, 1, 0.0f);
    self->actionFunc = FUN_003ba2a0;
    self->insectFlags |= INSECT_FLAG_CRAWLING;
}


}
