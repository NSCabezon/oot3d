extern "C" {
#include "z_en_po_sisters.hpp"

void EffectSsDeadDb_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep,
                          s16 primR, s16 primG, s16 primB, s16 primA, s16 envR, s16 envG, s16 envB, s16 unused,
                          s16 life, s32 arg);
void Lights_PointSetColorAndRadius(void* lightInfo, u8 r, u8 g, u8 b, s16 radius, s32 arg6);
void FUN_0017823c(EnPoSisters* self, PlayState* play);
s32 FUN_003731e0(SkelAnime* skelAnime);
s32 Math_ScaledStepToS(s16* pValue, s16 target, s16 scale);

// Own TU: guarded function-local statics (sZeroVec guard 0x52DFEC obj 0x59B5D4; FUN_001e80d4 sPos guard 0x52DFF0 obj 0x59B5E0; FUN_00389ef0 sPos guard 0x52DFF4 obj 0x59B5EC) pinned by
// pin_static --guard-sel "z_en_po_sisters_death.o(+DATA)" (see STATUS.md).
// FUN_00370170 = func_80AD9C24 (N64): death / dust effect, sets actionFunc FUN_0017823c.
void FUN_00370170(EnPoSisters* self, PlayState* play) {
    static Vector3f sZeroVec(0.0f, 0.0f, 0.0f);
    Vec3f vec;

    self->actor.draw = NULL;
    self->actor.flags &= ~1;
    self->unk_19C = 150;
    self->unk_199 = 32;
    self->collider.base.colType = 3;
    self->collider.base.acFlags &= ~AC_HARD;
    if (play != NULL) {
        vec.x = self->actor.world.pos.x;
        vec.y = self->actor.world.pos.y + 45.0f;
        vec.z = self->actor.world.pos.z;
        EffectSsDeadDb_Spawn(play, &vec, (Vec3f*)&sZeroVec, (Vec3f*)&sZeroVec, 150, 0, 255, 255, 255, 155, 150, 150,
                             150, 1, 11, 0);
    }
    Lights_PointSetColorAndRadius(&self->lightInfo, 0, 0, 0, 0, 0);
    self->actionFunc = (EnPoSistersActionFunc)FUN_0017823c;
}


void FUN_001e80d4(EnPoSisters* self, PlayState* play) {
    static Vector3f sPos(120.0f, 250.0f, -1420.0f);

    FUN_003731e0(&self->skelAnime);
    self->unk_19A--;
    Math_ScaledStepToS(&self->actor.shape.rot.y, self->actor.world.rot.y, 0x500);
    if (self->unk_19A == 0) {
        if (self->unk_194 == 0) {
            *(u8*)((u8*)play + 0x3237) = 4;
        }
    }
    if (self->unk_19A < 0) {
        Math_StepToF(&self->actor.speedXZ, 5.0f, 0.2f);
    }
    if (self->unk_19A == -0x69) {
        if (self->unk_194 == 1) {
            Audio_PlaySoundAtPosition(play, (Vec3f*)&sPos, 0x28, 0x0100035C);
        }
    }
    if (self->unk_19A < -0xB4) {
        Actor_Kill(&self->actor);
    }
}

void FUN_0013f280(EnPoSisters*, PlayState*);
void FUN_001e7fc8(EnPoSisters*, PlayState*);
void FUN_003dc3e0(EnPoSisters*, PlayState*);
void FUN_00199aa8(EnPoSisters*, PlayState*);
void FUN_0020584c(EnPoSisters*, PlayState*);
void Lights_PointGlowSetInfo(void* light, f32 x, f32 y, f32 z, u8 r, u8 g, u8 b, s16 radius, s32 pad);
void Lights_PointNoGlowSetInfo(void* light, f32 x, f32 y, f32 z, u8 r, u8 g, u8 b, s16 radius, s32 pad);
f32 Rand_ZeroOne(void);
void VEC3TransformAsm(Vector3f*, MTX34*, Vector3f*);
extern Color_RGBA8 D_EnPoSisters_ColorTbl[];

static inline void EnPoSisters_MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}

void FUN_00389ef0(PlayState* play, s32 limbIndex, MTX34* mtx, Actor* thisx) {
    EnPoSisters* self = (EnPoSisters*)thisx;
    s32 i;
    static Vector3f sPos(800.0f, -2080.0f, 0.0f);

    if (limbIndex == 1) {
        *(MTX34*)self->unk_AC8 = *mtx;
    } else if (limbIndex == 7 && self->actionFunc != (EnPoSistersActionFunc)FUN_0013f280) {
        if (self->unk_199 & 0x20) {
            for (i = self->unk_198 - 1; i > 0; i--) {
                self->unk_9F0[i] = self->unk_9F0[i - 1];
            }
            EnPoSisters_MultVec3f(&sPos, (Vector3f*)&self->unk_9F0[0], mtx);
        } else if (self->actionFunc == (EnPoSistersActionFunc)FUN_001e7fc8) {
            EnPoSisters_MultVec3f(&sPos, (Vector3f*)&self->actor.home.pos, mtx);
        }
        if (self->unk_198 != 0) {
            Color_RGBA8* color = &D_EnPoSisters_ColorTbl[self->unk_194];
            f32 flicker = Rand_ZeroOne() * 0.3f + 0.7f;

            if (self->actionFunc == (EnPoSistersActionFunc)FUN_003dc3e0 ||
                self->actionFunc == (EnPoSistersActionFunc)FUN_00199aa8 ||
                self->actionFunc == (EnPoSistersActionFunc)FUN_0020584c) {
                Lights_PointNoGlowSetInfo(&self->lightInfo, self->unk_9F0[0].x, self->unk_9F0[0].y + 15.0f, self->unk_9F0[0].z, color->r * flicker,
                                          color->g * flicker, color->b * flicker, 200, 0);
            } else {
                Lights_PointGlowSetInfo(&self->lightInfo, self->unk_9F0[0].x, self->unk_9F0[0].y + 15.0f, self->unk_9F0[0].z, color->r * flicker,
                                        color->g * flicker, color->b * flicker, 200, 0);
            }
        } else {
            Lights_PointSetColorAndRadius(&self->lightInfo, 0, 0, 0, 0, 0);
        }
    }
}
}
