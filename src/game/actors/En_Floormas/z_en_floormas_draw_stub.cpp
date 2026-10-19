extern "C" {
#include "z_en_floormas.hpp"

void FUN_0032d278(GameState* state, Actor* actor) {}
// Keep actor helpers opaque to the calling TU, as in the original separate functions.
void EnFloormas_Run(EnFloormas* self, GameState* state);
void EnFloormas_BigWalk(EnFloormas* self, GameState* state);
void Animation_PlayLoopSetSpeed(SkelAnime* skelAnime, s32 idx, f32 speed);
void FUN_0036f4e4(SkelAnime* skelAnime, f32 speed);
s16 Rand_S16Offset_003702c8(s16 base, s16 range);
typedef void (*EnFloormasActionFunc)(EnFloormas*, GameState*);

void FUN_00178650(EnFloormas* self) {
    if (*(EnFloormasActionFunc*)((u8*)self + 0x7D8) != EnFloormas_Run) {
        Animation_PlayLoopSetSpeed((SkelAnime*)((u8*)self + 0x1A4), 7, 1.5f);
    } else {
        FUN_0036f4e4((SkelAnime*)((u8*)self + 0x1A4), 1.5f);
    }
    *(s16*)((u8*)self + 0x7DC) = Rand_S16Offset_003702c8(2, 4);
    *(f32*)((u8*)self + 0x6C) = 1.5f;
    *(EnFloormasActionFunc*)((u8*)self + 0x7D8) = EnFloormas_BigWalk;
}
void EnFloormas_Charge(EnFloormas* self, GameState* state);
void EnFloormas_Land(EnFloormas* self, GameState* state);
void EnFloormas_Stand(EnFloormas* self, GameState* state);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, s32 mode, f32 speed, f32 start, f32 end, f32 morph);
void Animation_MorphToPlayOnce(SkelAnime* skelAnime, s32 idx, f32 morph);

void EnFloormas_SetupLand(EnFloormas* self) {
    Animation_Change_00375c08((SkelAnime*)((u8*)self + 0x1A4), 2, 2, 1.0f, 41.0f, 42.0f, 5.0f);
    if (*(f32*)((u8*)self + 0x6C) < 0.0f || *(EnFloormasActionFunc*)((u8*)self + 0x7D8) != EnFloormas_Charge) {
        *(s16*)((u8*)self + 0x7DC) = 45;
    } else {
        *(s16*)((u8*)self + 0x7DC) = 68;
    }
    *(f32*)((u8*)self + 0x70) = -1.0f;
    *(EnFloormasActionFunc*)((u8*)self + 0x7D8) = EnFloormas_Land;
}

void EnFloormas_SetupStand(EnFloormas* self) {
    Animation_MorphToPlayOnce((SkelAnime*)((u8*)self + 0x1A4), 11, -3.0f);
    *(EnFloormasActionFunc*)((u8*)self + 0x7D8) = EnFloormas_Stand;
}

f32* EnFloormas_PlaySpeedPtr(SkelAnime* skelAnime) {
    return &skelAnime->playSpeed;
}

f32* EnFloormas_CurFramePtr(SkelAnime* skelAnime) {
    return &skelAnime->curFrame;
}

f32* EnFloormas_EndFramePtr(SkelAnime* skelAnime) {
    return &skelAnime->endFrame;
}

#if 0 // Companion to disabled SetupSplit draft; requires enabling only once caller matches.
u8 EnFloormas_InitialHealth(CollisionCheckInfoInit* info) {
    return info->health;
}
#endif

}
