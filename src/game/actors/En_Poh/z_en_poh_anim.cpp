// Tiny accessor in its own TU: armlink --inline turns calls to it into `add r0, r0, #0x3c` (ROM reads curFrame as
// `add r0,rX,#0x1a4; add r0,r0,#0x3c; vldr sN,[r0]`). Same trick as En_Horse z_en_horse_anim.cpp.
extern "C" {
#include "z_en_poh.hpp"

f32* EnPoh_CurFramePtr(SkelAnime* skelAnime) {
    return &skelAnime->curFrame;
}

f32* EnPoh_PlaySpeedPtr(SkelAnime* skelAnime) {
    return &skelAnime->playSpeed;
}

void Animation_PlayLoop(SkelAnime* skelAnime, void* anim);
void FUN_003c70bc(EnPoh* self, PlayState* play);

// Kept out of z_en_poh.cpp: same-TU callers (FUN_0013f9c4) would inline it, ROM calls it.
void FUN_0036f32c(EnPoh* self) {
    Animation_PlayLoop(&self->skelAnime, self->unk_940);
    self->unk_920 = Rand_S16Offset_003702c8(15, 3);
    self->actionFunc = FUN_003c70bc;
}

// FUN_0036f364 (N64 EnPoh_MoveTowardsPlayerHeight). The 2/3 phase constants are declared (third, two, half) but
// assigned (two, third, half): declaration order picks the VFP regs, assignment order the literal pool order.
void FUN_0036f364(EnPoh* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 phase;
    f32 third, two, half;

    Math_StepToF(&self->actor.world.pos.y, player->actor.world.pos.y, 1.0f);
    two = 2.0f;
    third = 1.0f / 3.0f;
    half = 0.5f;
    u32 timer = self->unk_91D;
    phase = timer;
    if (timer != 0) {
        phase = (s32)(half + (timer * two) * third);
    }
    self->actor.world.pos.y += 2.5f * Math_SinS(phase * 0x800);
    if (self->unk_91D != 0) {
        self->unk_91D -= 1;
    }
    if (self->unk_91D == 0) {
        self->unk_91D = 0x30;
    }
}
}
