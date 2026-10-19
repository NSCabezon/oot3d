// Separate TU from z_en_ik.cpp on purpose: FUN_001c67f4 calls FUN_003468fc in the ROM, and armcc inlines it
// when both live in the same file.
extern "C" {
#include "z_en_ik.hpp"

s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void FUN_0010b758(EnIk* self, PlayState* play);

// Empty callee in its own TU: armlink --inline turns the call into `mov r0,r0` (see CLAUDE.md, empty-call stubs).
__attribute__((noreturn)) void EnIk_EmptyCall(PlayState* play, Vec3f* pos, s32 arg); // defined in z_en_ik.cpp

void FUN_00375f88(PlayState* play, Vec3f* pos) {
    EnIk_EmptyCall(play, pos, 8);
}

// Collider_Destroy{Cylinder,Quad} are 2-insn `mov r0,#1` stubs in the ROM, inlined by armlink from another TU.
s32 EnIk_ColliderDestroyStub(PlayState* play, void* collider) {
    return 1;
}

// Separate TU so callers in z_en_ik.cpp (FUN_0014be68) do not inline it.
extern u8 gUnk_004D86CC[];

void* FUN_00357a70(s32 idx) {
    return gUnk_004D86CC + idx * 48;
}

// Separate TU so FUN_001c67f4 (z_en_ik.cpp) does not inline it.
s32 FUN_0035b950(PlayState* play, Actor* actor, f32 range, s32 arg3, s32 arg4, s16 yaw);
void FUN_0013832c(EnIk* self, PlayState* play);

s32 FUN_0036d188(EnIk* self, PlayState* play) {
    f32 frames;

    if (self->unk_E0F == 0 && self->actor.params != 0) {
        return 0;
    }
    if (FUN_0035b950(play, &self->actor, 100.0f, 0x2710, 0x4000, self->actor.shape.rot.y) == 0) {
        return 0;
    }
    if (*(u32*)((u8*)play + 0x5BF4) & 1) {
        frames = (f32)Animation_GetLastFrame((SkelAnime*)self->unk_1A4, 6);
        self->unk_E12 = 0;
        self->actor.speedXZ = 0.0f;
        self->unk_E0C = 9;
        Animation_Change_00375c08((SkelAnime*)self->unk_1A4, 6, 1.0f, 0.0f, frames, 3, -4.0f);
        self->actionFunc = (void*)FUN_0013832c;
        return 1;
    }
    return 0;
}

void FUN_003468fc(EnIk* self) {
    f32 frames;

    frames = Animation_GetLastFrame((SkelAnime*)self->unk_1A4, 0xC);
    self->actor.flags |= 5;
    self->actor.speedXZ = 0.0f;
    self->unk_E0C = 4;
    Animation_Change_00375c08((SkelAnime*)self->unk_1A4, 0xC, 0.0f, 0.0f, frames, 0, 4.0f);
    self->actionFunc = (void*)FUN_0010b758;
}

#define R_UPDATE_RATE SREG(30)

// N64 Math_StepUntilS with the step scaled to the 3DS frame rate (R_UPDATE_RATE / 3, rounded). Own TU: the caller
// FUN_001374c0 (z_en_ik.cpp) calls it with `bl`. The rounding must be if/else into an s32 temp (a ternary swaps s0/s1).
s32 FUN_003c43bc(s16* pValue, s16 limit, s16 step) {
    s16 orig = *pValue;
    s32 s;

    if (step > 0) {
        s = (s32)(R_UPDATE_RATE * (f32)step / 3.0f + 0.5f);
    } else {
        s = (s32)(R_UPDATE_RATE * (f32)step / 3.0f - 0.5f);
    }
    *pValue += s;
    if (((*pValue - limit) * (orig - limit)) <= 0) {
        *pValue = limit;
        return 1;
    }
    return 0;
}

}
