extern "C" {
#include "z_demo_gt.hpp"

#define FLAGS 0x00400030

void DemoGt_Init(Actor* actor, GameState* state);
void DemoGt_Destroy(Actor* actor, PlayState* play);
void DemoGt_Update(Actor* actor, PlayState* play);
void DemoGt_Draw(Actor* actor, PlayState* play);

typedef void (*DemoGtUpdateFunc)(DemoGt* self, PlayState* play);
typedef void (*DemoGtDrawFunc)(Actor* thisx, PlayState* play);

typedef struct DemoGtCsCue {
    /* 0x00 */ u16 id;
} DemoGtCsCue;

void FUN_0032a5d0(DemoGt* self, PlayState* play);
void FUN_0032ae1c(DemoGt* self, PlayState* play, s32 cueChannel);
void FUN_0032a998(DemoGt* self, s32 arg);
void FUN_00329d0c(DemoGt* self, PlayState* play);
void FUN_003291f4(DemoGt* self, PlayState* play);

GLOBAL_ASM("data/z_Demo_Gt.data.s")

// sUpdateFuncs / sDrawFuncs live in the data dump above (no labels) -> absolute symbols (EQU) in data_syms.s.
GLOBAL_ASM("../src/game/actors/Demo_Gt/data_syms.s")
extern DemoGtUpdateFunc Demo_Gt_sUpdateFuncs[];
extern DemoGtDrawFunc Demo_Gt_sDrawFuncs[];
extern InitChainEntry Demo_Gt_sInitChain[];

ActorInit Demo_Gt_InitVars = {
    ACTOR_DEMO_GT,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GT,
    sizeof(DemoGt),
    (ActorFunc)DemoGt_Init,
    (ActorFunc)DemoGt_Destroy,
    (ActorFunc)DemoGt_Update,
    (ActorFunc)DemoGt_Draw,
};

GLOBAL_ASM("asm/DemoGt_Init.s")

void DemoGt_Destroy(Actor* thisx, PlayState* play) {
    DemoGt* self = (DemoGt*)thisx;

    if ((self->actor.params == 1) || (self->actor.params == 2)) {
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->bgId);
    }

    FUN_00350f34(thisx, &self->unk_200[0], &self->unk_200[1], &self->skelAnimModel[0],
                 &self->skelAnimModel[1], &self->unk_210[0], &self->unk_210[1],
                 &self->unk_210[2], 0);
}

void DemoGt_Update(Actor* thisx, PlayState* play) {
    DemoGt* self = (DemoGt*)thisx;
    DemoGtUpdateFunc updateFunc;

    if ((self->updateMode < 0) || (self->updateMode >= 19) ||
        (updateFunc = Demo_Gt_sUpdateFuncs[self->updateMode]) == NULL) {
        return;
    }

    updateFunc(self, play);
}

void DemoGt_Draw(Actor* thisx, PlayState* play) {
    DemoGt* self = (DemoGt*)thisx;
    DemoGtDrawFunc drawFunc;

    if ((self->drawConfig < 0) || (self->drawConfig >= 9) ||
        (drawFunc = Demo_Gt_sDrawFuncs[self->drawConfig]) == NULL) {
        return;
    }

    drawFunc(thisx, play);
}

void caseD_a_003291c4(DemoGt* self, PlayState* play) {
    FUN_0032ae1c(self, play, 3);
    FUN_0032a998(self, 3);
    FUN_003291f4(self, play);
}

void caseD_2_002a0f70(Actor* thisx, PlayState* play) {
    DemoGt* self;

    ((DemoGt*)thisx)->skelAnimModel[0]->SetUnkAC(1);
    self = (DemoGt*)thisx;
    self->skelAnimModel[0]->FUN_003721e0(&((DemoGt*)thisx)->actor.modelMtx);
    self->skelAnimModel[0]->FUN_00372170(0);
}

void caseD_3_002a0fa4(Actor* thisx, PlayState* play) {
    DemoGt* self;

    ((DemoGt*)thisx)->skelAnimModel[1]->SetUnkAC(1);
    self = (DemoGt*)thisx;
    self->skelAnimModel[1]->FUN_003721e0(&((DemoGt*)thisx)->actor.modelMtx);
    self->skelAnimModel[1]->FUN_00372170(0);
}

void caseD_9_00329cd0(DemoGt* self, PlayState* play) {
    FUN_0032a5d0(self, play);
    FUN_0032ae1c(self, play, 2);
    FUN_0032a998(self, 2);
    FUN_00329d0c(self, play);
}

static inline DemoGtCsCue* DemoGt_GetCueFromCtx(PlayState* play, CutsceneContext* csCtx, u32 cueChannel) {
    DemoGtCsCue* cue = NULL;

    if (play->GetCutsceneState() != 0) {
        cue = (DemoGtCsCue*)csCtx->npcActions[cueChannel];
    }

    return cue;
}

void caseD_2_0028ee64(DemoGt* self, PlayState* play2) {
    PlayState* play = play2;

    FUN_0032a998(self, 3);
    FUN_003291f4(self, play);
    {
        DemoGtCsCue* cue = DemoGt_GetCueFromCtx(play, &play->csCtx, 3);

        if ((cue != NULL) && (cue->id == 2)) {
            DemoGt* s2 = self;

            s2->updateMode = 10;
        }
    }
}

void caseD_1_0028ee04(DemoGt* self, PlayState* play2) {
    PlayState* play = play2;

    FUN_0032a5d0(self, play);
    FUN_0032a998(self, 2);
    FUN_00329d0c(self, play);
    {
        DemoGtCsCue* cue = DemoGt_GetCueFromCtx(play, &play->csCtx, 2);

        if ((cue != NULL) && (cue->id == 2)) {
            DemoGt* s2 = self;

            s2->updateMode = 9;
        }
    }
}

void caseD_b_002a1288(DemoGt* self, PlayState* play2) {
    PlayState* play = play2;

    FUN_0032ae1c(self, play, 4);
    FUN_0032a998(self, 4);
    if (play->csCtx.frames == 0x29D) {
        Audio_PlaySoundGeneral(0x01000235, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               &DAT_0054ac24.unk_0);
    }
}

f32 FUN_0032c66c(u16 endFrame, u16 startFrame, u16 curFrame, s32 arg3, s32 arg4);

static inline CsCmdActorAction* DemoGt_GetCue(PlayState* play, CutsceneContext* csCtx, s32 cueChannel) {
    CsCmdActorAction* cue = NULL;

    if (play->GetCutsceneState() != 0) {
        cue = csCtx->npcActions[cueChannel];
    }
    return cue;
}

// Cue lerp of world.pos (N64 func_8097E744).
void FUN_0032ae1c(DemoGt* self, PlayState* play, s32 cueChannel) {
    CutsceneContext* csCtx = &play->csCtx;
    CsCmdActorAction* cue = DemoGt_GetCue(play, csCtx, cueChannel);
    f32 lerp;

    if (cue != NULL) {
        Vec3f* pos = &self->actor.world.pos;

        lerp = FUN_0032c66c(cue->endFrame, cue->startFrame, csCtx->frames, 8, 0);

        f32 startX = cue->startPos.x;
        f32 startY = cue->startPos.y;
        f32 startZ = cue->startPos.z;
        f32 endX = cue->endPos.x;
        f32 endY = cue->endPos.y;
        f32 endZ = cue->endPos.z;

        pos->x = ((endX - startX) * lerp) + startX;
        pos->y = ((endY - startY) * lerp) + startY;
        pos->z = ((endZ - startZ) * lerp) + startZ;
    }
}

f32 FUN_0032c66c(u16 endFrame, u16 startFrame, u16 curFrame, s32 arg3, s32 arg4);

// FUN_0032aee0 (N64 func_8097F280 shape, ADJUST 160/170): matched. The first ADJUST compare goes through an
// s32-parameter helper so the 480.0f constant gets s0 (ROM) instead of s1.
static inline s32 DemoGt_AdjustFrames(s32 n) {
    return (s32)((f32)n / SREG(30) + 0.5f);
}

void FUN_0032aee0(DemoGt* self, PlayState* play) {
    s32* unk178 = self->unk_1D0;
    s32* unk188 = self->unk_1E0;
    s32* unk198 = self->unk_1F0;
    CutsceneContext* csCtx = &play->csCtx;
    u16 frames = csCtx->frames;
    if (DemoGt_AdjustFrames(480) > frames) {
        unk178[0] = 100;
        unk178[1] = 255;
        unk178[2] = 200;

        unk188[0] = 255;
        unk188[1] = 120;
        unk188[2] = 100;

        unk198[0]++;
        unk198[1]--;
    } else if (ADJUST_FRAMECOUNT(170) > frames) {
        f32 temp_f0 = FUN_0032c66c(170, 160, frames, 0, 0);

        unk178[0] = (s32)((temp_f0 * -63.0f) + 163.0f);
        unk178[1] = (s32)((temp_f0 * -155.0f) + 255.0f);
        unk178[2] = (s32)((temp_f0 * -100.0f) + 200.0f);

        unk188[0] = (s32)((temp_f0 * -155.0f) + 255.0f);
        unk188[1] = (s32)((temp_f0 * -20.0f) + 120.0f);
        unk188[2] = 100;
    } else {
        unk178[0] = 100;
        unk178[1] = 100;
        unk178[2] = 100;

        unk188[0] = 100;
        unk188[1] = 100;
        unk188[2] = 100;
    }
}

// caseD_e_0027da2c (N64 func_80981D20 + cue ch7 + func_8097E824(7) + sfx at ADJUST(183)): NOT matched,
// structure close (predicated vnmlsle/vmlagt rounding OK) but ldrh/ldrsh, prologue reg order, and
// temp re-read order still differ. Draft under #if 0.
#if 0
// caseD_e_0027da2c: speed/rotation update (N64 func_80981D20, 2/3 speed step) + cue lerp ch7 +
// func_8097E824(7) + sfx at ADJUST_FRAMECOUNT(183) (N64 func_80981CEC).
void caseD_e_0027da2c(Actor* thisx, PlayState* play2) {
    DemoGt* self = (DemoGt*)thisx;
    PlayState* play = play2;
    f32 temp = self->unk_1CA;

    s32 delta;

    self->unk_1CC = temp * ((kREG(64) * 0.001f) + 0.048f) + (kREG(76) + 100.0f);
    if (self->unk_1CC > 0) {
        delta = (s32)((((f32)self->unk_1CC) * 2.0f / 3.0f) + 0.5f);
    } else {
        delta = (s32)((((f32)self->unk_1CC) * 2.0f / 3.0f) - 0.5f);
    }
    self->unk_1CA = self->unk_1CA + delta;
    if (self->unk_1CA > (s16)(kREG(80) + 0x4000)) {
        self->unk_1CA = kREG(80) + 0x4000;
    }

    {
        PlayState* play2 = play;

        FUN_0032ae1c(self, play2, 7);
    }
    FUN_0032a998(self, 7);

    if (ADJUST_FRAMECOUNT(183) == play->csCtx.frames) {
        Audio_PlaySoundGeneral(0x01000235, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               &DAT_0054ac24.unk_0);
    }
}

#endif

// FUN_00330768 (N64 func_8097D7D8, DemoGt_SpawnDust inlined): NOT matched, 66/79 words off (prologue reg
// numbering: ROM pos=r8, arg5=r6, life=r7 via ldrd; ours pos=r7). Draft under #if 0, GLOBAL_ASM kept.
#if 0
u8 FrameAdvance_IsEnabled(PlayState* play);
u64 FUN_00368d94(s32 a, s32 b);
void FUN_0034035c(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* prim, Color_RGBA8* env,
                  s16 scale, s16 scaleStep, s16 life);
extern Color_RGBA8 Demo_Gt_sRedEnv;

// Best draft (68 -> ~65 words off; only call-arg/regalloc left): prim = `&env - 1` gives ROM `sub r3,r0,#4`;
// accel *= scale (N64 had it, old draft dropped it); `(dustScale*0.2f) * Rand` gives ROM vmla order.
// Still: ROM pos=r8, arg5=r6, life=r7 via one `ldrd r6,r7,[sp,#0x50]`; ours life=r6, pos=r7, arg5=r8.
void FUN_00330768(PlayState* play, Vec3f* pos, Vec3f* velOffset, f32 scale, s32 arg4, s32 arg5, s16 life) {
    if (!FrameAdvance_IsEnabled(play)) {
        s32 frames = *(s32*)((u8*)play + 0x5BF4);
        s32 rem = (s32)(FUN_00368d94(frames, arg4) >> 32);

        if (ABS(rem) == arg5) {
            f32 dustScale = 300.0f * scale;
            s16 scaleStep = 15.0f * scale;
            Vector3f velocity(0.0f, 6.0f, 0.0f);
            Vector3f accel(0.0f, 0.0f, 0.0f);
            s16 dustScaleRand;

            velocity.x *= scale;
            velocity.y *= scale;
            velocity.z *= scale;

            velocity.x += velOffset->x;
            velocity.y += velOffset->y;
            velocity.z += velOffset->z;

            accel.x *= scale;
            accel.y *= scale;
            accel.z *= scale;

            dustScaleRand = (s16)(((dustScale * 0.2f) * Rand_ZeroOne()) + dustScale);
            FUN_0034035c(play, pos, (Vec3f*)&velocity, (Vec3f*)&accel, &Demo_Gt_sRedEnv - 1, &Demo_Gt_sRedEnv,
                         dustScaleRand, scaleStep, life);
        }
    }
}
#endif

// FUN_0032a6f4 (N64 func_8097F19C: EF40 dust at ADJUST(220) + F0AC explosions at ADJUST(140)/(176)): NOT matched,
// same size as ROM (676 B), ~97/169 words off. Left: ROM keeps self=r5/&pos=r4 (ours r4/r5), play=r7, no extra
// `sb`; ROM reloads the 0x51bb90 literal for each dust call (ours CSEs `&env` into sb); ROM frame 0x38 (ours 0x3c).
#if 0
void FUN_0034035c(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* prim, Color_RGBA8* env,
                  s16 scale, s16 scaleStep, s16 life);
void FUN_003308a4(PlayState* play, Vec3f* pos, f32 scale);
extern Color_RGBA8 Demo_Gt_sRedEnv;

static inline void DemoGt_SpawnDust(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, f32 scale,
                                    s16 scaleStep, s16 life) {
    FUN_0034035c(play, pos, velocity, accel, &Demo_Gt_sRedEnv - 1, &Demo_Gt_sRedEnv,
                 ((scale * 0.2f) * Rand_ZeroOne()) + scale, scaleStep, life);
}

static inline void DemoGt_Dust(DemoGt* self, PlayState* play) {
    CutsceneContext* csCtx = &play->csCtx;
    u16 frames = csCtx->frames;
    Vec3f dustPos;
    Vector3f velocity(0.0f, -16.0f, 0.0f);
    Vector3f accel(0.0f, 1.2f, 0.0f);
    Vec3f* pos = &self->actor.world.pos;

    if (ADJUST_FRAMECOUNT(220) == frames) {
        dustPos.x = pos->x + 256.0f;
        dustPos.y = pos->y + 679.0f;
        dustPos.z = pos->z + 82.0f;
        DemoGt_SpawnDust(play, &dustPos, (Vec3f*)&velocity, (Vec3f*)&accel, 1700.0f, 15, 30);

        dustPos.x = pos->x + 256.0f;
        dustPos.y = pos->y + 679.0f;
        dustPos.z = pos->z - 60.0f;
        DemoGt_SpawnDust(play, &dustPos, (Vec3f*)&velocity, (Vec3f*)&accel, 1700.0f, 15, 30);
    }
}

static inline void DemoGt_Explosions(DemoGt* self, PlayState* play) {
    Vec3f sp;
    CutsceneContext* csCtx = &play->csCtx;
    u16 frames = csCtx->frames;

    if (ADJUST_FRAMECOUNT(140) == frames) {
        sp.x = self->actor.world.pos.x + 260.0f;
        sp.y = self->actor.world.pos.y + 340.0f;
        sp.z = self->actor.world.pos.z + 45.0f;
        FUN_003308a4(play, &sp, 2.0f);
    }
    if (ADJUST_FRAMECOUNT(176) == frames) {
        sp.x = self->actor.world.pos.x + 260.0f;
        sp.y = self->actor.world.pos.y + 840.0f;
        sp.z = self->actor.world.pos.z + 45.0f;
        FUN_003308a4(play, &sp, 2.0f);
    }
}

void FUN_0032a6f4(DemoGt* self, PlayState* play) {
    DemoGt_Dust(self, play);
    DemoGt_Explosions(self, play);
}
#endif

// Tower texture-scroll/env colour (N64 func_8097FDDC).
void FUN_0032a5d0(DemoGt* self, PlayState* play) {
    s32* unk178 = self->unk_1D0;
    s32* unk198 = self->unk_1F0;
    CutsceneContext* csCtx = &play->csCtx;
    s32 frames = csCtx->frames;

    if (ADJUST_FRAMECOUNT(610) > frames) {
        unk178[0] = 163;
        unk178[1] = 193;
        unk178[2] = 193;
        unk198[0]++;
        unk198[1]--;
    } else if (ADJUST_FRAMECOUNT(620) > play->csCtx.frames) {
        f32 temp_f0 = FUN_0032c66c(620, 610, play->csCtx.frames, 0, 0);

        unk178[0] = (s32)((temp_f0 * (-13.0f)) + 163.0f);
        unk178[1] = (s32)((temp_f0 * (-43.0f)) + 193.0f);
        unk178[2] = (s32)((temp_f0 * (-43.0f)) + 193.0f);
    } else {
        unk178[0] = 150;
        unk178[1] = 150;
        unk178[2] = 150;
    }
}

void EffectSsBomb2_SpawnLayered(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep);

// Six explosion sfx ids in ROM rodata (data_syms.s EQU), copied to the stack by value.
typedef struct DemoGtSfxIds {
    u32 ids[6];
} DemoGtSfxIds;
extern DemoGtSfxIds Demo_Gt_sSfxIds;

// Explosion with random sfx (N64 DemoGt_SpawnExplosionWithSound + 6-entry sfx pick).
void FUN_003308a4(PlayState* play, Vec3f* pos, f32 scale) {
    s16 scale100 = (s16)(100.0f * scale);
    s16 scale15 = (s16)(15.0f * scale);
    Vector3f velocity(0.0f, 0.0f, 0.0f);
    Vector3f accel(0.0f, 0.0f, 0.0f);

    EffectSsBomb2_SpawnLayered(play, pos, (Vec3f*)&velocity, (Vec3f*)&accel, scale100, scale15);
    {
        DemoGtSfxIds sfx = Demo_Gt_sSfxIds;

        Audio_PlaySoundAtPosition(play, pos, 60, sfx.ids[(s32)(Rand_ZeroOne() * 6.0f) % 6]);
    }
}
// caseD_7 / caseD_8 / caseD_1 (N64 DemoGt_Draw7 / Draw8 / Draw1-shape).
// fabs rule: ROM `vabs s0,s0; vmov s18,s0` appears when the fabs result reaches the call through an inlined
// helper's float parameter (here a Matrix_RotateAxis-like helper with a mode arg); a direct call argument gives
// `vabs s18,s0`. The `u8 apply` + axis-local order reproduces the early `mov r5,#1` and r1-before-r0 arg setup.
void FUN_003625f8(MTX34* dst, f32 angle, Vec3f* axis);
void FUN_0036c174(MTX34* dst, MTX34* a, MTX34* b);
void FUN_003713fc(MTX34* m, s32 mode, f32 x, f32 y, f32 z);

static inline void DemoGt_RotateAxis(MTX34* mtx, f32 angle, Vec3f* axis, s32 mode) {
    MTX34 rot;
    u8 apply = (mode == 1);
    Vec3f* ax = axis;

    FUN_003625f8(apply ? &rot : mtx, angle, ax);
    if (apply) {
        FUN_0036c174(mtx, mtx, &rot);
    }
}

// caseD_7_002a0fd8 (N64 DemoGt_Draw7, unk_210[1]).
void caseD_7_002a0fd8(DemoGt* self, PlayState* play) {
    GraphicsContext* gfxCtx = play->state.gfxCtx;
    s16 sp6E;
    f32 sp68;
    f32 sp64;
    s16 sp62;
    s16 sp60;
    Vec3f sp50;
    Vec3f sp44;
    f32 sp40;
    MTX34 mtx;

    sp6E = self->unk_1CA;
    sp68 = __fabsf((f32)sp6E * (M_PI / 0x8000));
    sp64 = kREG(75);
    sp62 = kREG(74) + 0x7FEC;
    sp60 = (s16)(kREG(74) + 0x7FEC) + 0x4000;
    *(s32*)((u8*)gfxCtx + 0x0C) -= 0x40;
    sp40 = 1.0f - Math_CosS(sp6E);

    sp50.x = Math_CosS(sp60);
    sp50.y = 0.0f;
    sp50.z = Math_SinS(sp60);

    sp44.x = (Math_CosS(sp62) * sp64) * sp40;
    sp44.y = Math_SinS(sp6E) * sp64;
    sp44.z = (Math_SinS(sp62) * sp64) * sp40;

    MTX34CopyAsm(&mtx, &self->actor.modelMtx);
    DemoGt_RotateAxis(&mtx, sp68, &sp50, 1);
    FUN_003713fc(&mtx, 1, sp44.x, sp44.y, sp44.z);
    self->unk_210[1]->SetUnkAC(1);
    self->unk_210[1]->FUN_003721e0(&mtx);
    self->unk_210[1]->FUN_00372170(0);
}

// caseD_8_002a1130 (N64 DemoGt_Draw8, unk_210[2]).
void caseD_8_002a1130(DemoGt* self, PlayState* play) {
    GraphicsContext* gfxCtx = play->state.gfxCtx;
    s16 sp6E;
    f32 sp68;
    f32 sp64;
    s16 sp62;
    s16 sp60;
    Vec3f sp50;
    Vec3f sp44;
    f32 sp40;
    MTX34 mtx;

    sp6E = self->unk_1CA;
    sp68 = __fabsf((f32)sp6E * (M_PI / 0x8000));
    sp64 = kREG(78);
    sp62 = kREG(77) + 0xBE80;
    sp60 = (s16)(kREG(77) + 0xBE80) + 0x4000;
    *(s32*)((u8*)gfxCtx + 0x0C) -= 0x40;
    sp40 = 1.0f - Math_CosS(sp6E);

    sp50.x = Math_CosS(sp60);
    sp50.y = 0.0f;
    sp50.z = Math_SinS(sp60);

    sp44.x = (Math_CosS(sp62) * sp64) * sp40;
    sp44.y = Math_SinS(sp6E) * sp64;
    sp44.z = (Math_SinS(sp62) * sp64) * sp40;

    MTX34CopyAsm(&mtx, &self->actor.modelMtx);
    DemoGt_RotateAxis(&mtx, sp68, &sp50, 1);
    FUN_003713fc(&mtx, 1, sp44.x, sp44.y, sp44.z);
    self->unk_210[2]->SetUnkAC(1);
    self->unk_210[2]->FUN_003721e0(&mtx);
    self->unk_210[2]->FUN_00372170(0);
}

// caseD_1_002a0e08 (N64 DemoGt_Draw1 shape, two models unk_200[0..1]).
void caseD_1_002a0e08(DemoGt* self, PlayState* play) {
    GraphicsContext* gfxCtx = play->state.gfxCtx;
    s16 sp78;
    f32 sp74;
    f32 spBC;
    s16 spBA;
    s16 spB8;
    Vec3f spA8;
    Vec3f sp9C;
    f32 sp40;
    MTX34 mtx;

    sp78 = self->unk_1CA;
    sp74 = __fabsf((f32)sp78 * (M_PI / 0x8000));
    spBC = kREG(71);
    spBA = kREG(70);
    spB8 = kREG(70) + 0x4000;
    *(s32*)((u8*)gfxCtx + 0x0C) -= 0x40;
    sp40 = 1.0f - Math_CosS(sp78);

    spA8.x = Math_CosS(spB8);
    spA8.y = 0.0f;
    spA8.z = Math_SinS(spB8);

    sp9C.x = (Math_CosS(spBA) * spBC) * sp40;
    sp9C.y = Math_SinS(sp78) * spBC;
    sp9C.z = (Math_SinS(spBA) * spBC) * sp40;

    MTX34CopyAsm(&mtx, &self->actor.modelMtx);
    DemoGt_RotateAxis(&mtx, sp74, &spA8, 1);
    FUN_003713fc(&mtx, 1, sp9C.x, sp9C.y, sp9C.z);
    self->unk_200[0]->SetUnkAC(1);
    self->unk_200[0]->FUN_003721e0(&mtx);
    self->unk_200[0]->FUN_00372170(0);
    self->unk_200[1]->SetUnkAC(1);
    self->unk_200[1]->FUN_003721e0(&mtx);
    self->unk_200[1]->FUN_00372170(0);
}

// caseD_4_002aeda4 / caseD_5_002aeea8 (N64 DemoGt_Update4 / Update5 shape): NOT matched, 12 words off each.
// Structure and sizes now match (N64 inline helpers Sfx/Cue + csCtx local + `u16 frames`); only the callee-saved
// numbering differs: ROM play=r4 self=r5 csCtx=r6 cue=r7, ours self=r5 (ok) but play=r7 cue=r4. Draft under #if 0.
#if 0
static inline DemoGtCsCue* DemoGt_GetCue2(PlayState* play, CutsceneContext* csCtx, s32 cueChannel) {
    DemoGtCsCue* cue = NULL;

    if (play->GetCutsceneState() != 0) {
        cue = (DemoGtCsCue*)csCtx->npcActions[cueChannel];
    }
    return cue;
}

static inline u8 DemoGt_CueIs(PlayState* play, u16 cueId, s32 cueChannel) {
    DemoGtCsCue* cue = DemoGt_GetCue2(play, &play->csCtx, cueChannel);

    if ((cue != NULL) && (cue->id == cueId)) {
        return 1;
    } else {
        return 0;
    }
}

static inline void DemoGt_Sfx4(DemoGt* self, PlayState* play) {
    CutsceneContext* csCtx = &play->csCtx;
    u16 frames = csCtx->frames;

    if ((s32)((f32)(3 * 789) / SREG(30) + 0.5f) == frames) {
        Audio_PlaySoundGeneral(0x01000235, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               &DAT_0054ac24.unk_0);
    }
}

static inline void DemoGt_Cue4(DemoGt* self, PlayState* play) {
    if (DemoGt_CueIs(play, 2, 5)) {
        self->updateMode = 12;
    } else if (DemoGt_CueIs(play, 3, 5)) {
        self->updateMode = 17;
    }
}

void caseD_4_002aeda4(DemoGt* self, PlayState* play) {
    DemoGt* s2 = self;
    PlayState* play2 = play;

    FUN_0032ae1c(self, play, 5);
    FUN_0032a998(self, 5);
    DemoGt_Sfx4(self, play);
    DemoGt_Cue4(s2, play2);
}

// caseD_5_002aeea8: same shape with channel 6, ADJUST(845), modes 13/18.
static inline void DemoGt_Sfx5(DemoGt* self, PlayState* play) {
    CutsceneContext* csCtx = &play->csCtx;
    u16 frames = csCtx->frames;

    if ((s32)((f32)(3 * 845) / SREG(30) + 0.5f) == frames) {
        Audio_PlaySoundGeneral(0x01000235, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               &DAT_0054ac24.unk_0);
    }
}

static inline void DemoGt_Cue5(DemoGt* self, PlayState* play) {
    if (DemoGt_CueIs(play, 2, 6)) {
        self->updateMode = 13;
    } else if (DemoGt_CueIs(play, 3, 6)) {
        self->updateMode = 18;
    }
}

void caseD_5_002aeea8(DemoGt* self, PlayState* play) {
    DemoGt* s2 = self;
    PlayState* play2 = play;

    FUN_0032ae1c(self, play, 6);
    FUN_0032a998(self, 6);
    DemoGt_Sfx5(self, play);
    DemoGt_Cue5(s2, play2);
}
#endif

// caseD_6_0037d5ac: NOT matched. functions.tsv (cfg-merged, 980 B) spans the real head (~0x13C B) plus
// unrelated code after the pop, so a whole-function match is impossible without a cfg split. Head draft
// (55/245 words off, pool offsets differ) kept under #if 0; GLOBAL_ASM stays in functions_370000s.cpp.
static __inline __pure float DemoGt_Fabsf(float x) {
    return __fabsf(x);
}

void FUN_003625f8(MTX34* dst, f32 angle, Vec3f* axis);
void FUN_0036c174(MTX34* dst, MTX34* a, MTX34* b);
void FUN_003713fc(MTX34* m, s32 mode, f32 x, f32 y, f32 z);

// Pillar draw (N64 DemoGt_Draw6). caseD_6_0037d5ac in functions.tsv is cfg-merged (980 B) but the head is 0x13C B.
void caseD_6_0037d5ac(DemoGt* self, PlayState* play) {
    s16 sp78;
    f32 sp74;
    f32 sp70;
    s16 sp6E;
    s16 sp6C;
    Vec3f sp58;
    Vec3f sp4C;
    f32 sp48;
    MTX34 mtx;

    sp78 = self->unk_1CA;
    sp74 = __fabsf((f32)sp78 * (M_PI / 0x8000));
    sp70 = kREG(63);
    sp6E = kREG(60) + 0x4000;
    sp6C = (s16)(kREG(60) + 0x4000) + 0x4000;
    *(s32*)((u8*)play->state.gfxCtx + 0x0C) -= 0x40;
    sp48 = 1.0f - Math_CosS(sp78);

    sp58.x = Math_CosS(sp6C);
    sp58.y = 0.0f;
    sp58.z = Math_SinS(sp6C);

    sp4C.x = (Math_CosS(sp6E) * sp70) * sp48;
    sp4C.y = Math_SinS(sp78) * sp70;
    sp4C.z = (Math_SinS(sp6E) * sp70) * sp48;

    MTX34CopyAsm(&mtx, &self->actor.modelMtx);
    DemoGt_RotateAxis(&mtx, sp74, &sp58, 1);
    FUN_003713fc(&mtx, 1, sp4C.x, sp4C.y, sp4C.z);
    self->unk_210[0]->SetUnkAC(1);
    self->unk_210[0]->FUN_003721e0(&mtx);
    self->unk_210[0]->FUN_00372170(0);
}

// caseD_7 / caseD_6 / caseD_c: matched. Recipe: `u16 frames = csCtx->frames;` as its own local (csCtx local in the
// block) before the ADJUST compare; dead aliases `s2` / `thisx` / `play2` routed into single sites decide the
// callee-saved numbering (single-site mutation brute force).
void caseD_7_0028ef90(DemoGt* self, PlayState* play) {
    DemoGt* s2 = self;
    Actor* thisx = &self->actor;
    PlayState* play2 = play;

    FUN_0032a998(self, 9);
    {
        CutsceneContext* csCtx = &play->csCtx;
        u16 frames = csCtx->frames;

        if ((s32)((f32)(3 * 154) / SREG(30) + 0.5f) == frames) {
            Audio_PlaySoundGeneral(0x01000235, &thisx->world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                                   &DAT_0054ac24.unk_0);
        }
    }
    {
        DemoGtCsCue* cue = DemoGt_GetCueFromCtx(play2, &play->csCtx, 9);

        if ((cue != NULL) && (cue->id == 2)) {
            s2->updateMode = 15;
        }
    }
}

void caseD_6_0028eebc(DemoGt* self, PlayState* play) {
    PlayState* play2 = play;

    FUN_0032ae1c(self, play2, 7);
    FUN_0032a998(self, 7);
    {
        CutsceneContext* csCtx = &play->csCtx;
        u16 frames = csCtx->frames;

        if ((s32)((f32)(3 * 183) / SREG(30) + 0.5f) == frames) {
            Audio_PlaySoundGeneral(0x01000235, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                                   &DAT_0054ac24.unk_0);
        }
    }
    {
        DemoGtCsCue* cue = DemoGt_GetCueFromCtx(play, &play->csCtx, 7);

        if ((cue != NULL) && (cue->id == 2)) {
            self->updateMode = 14;
        }
    }
}

void caseD_c_002a12f8(DemoGt* self, PlayState* play2) {
    PlayState* play = play2;

    FUN_0032ae1c(self, play, 5);
    FUN_0032a998(self, 5);
    {
        u16 frames = play->csCtx.frames;

        if ((s32)((f32)(3 * 789) / SREG(30) + 0.5f) == frames) {
            Audio_PlaySoundGeneral(0x01000235, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                                   &DAT_0054ac24.unk_0);
        }
    }
}

// Drafts below: NOT matched. caseD_3: 8 words (self/play callee-saved swap at entry; ROM self=r7 play=r4,
// frames/cue structure now matches). 2026-10-14 sweep: greedy single-site + pair mutations, N64 inline helper shapes
// (CueIs/Sfx/ED64, elif/sep/s2-in-arm), Actor*/play2/state signatures, raw `(PlayState*)state` per call site,
// FUN_0032ae1c opaque (prototype only): all 8. R11: N64 Update3 shape (Sfx/Cues/CueIs inline helpers) 43, greedy+pair
// mutations incl. dead-alias deletes/decl swaps: 8. caseD_d: cfg-merged tail word (+13-word swap, same fix as caseD_c would apply).
// GLOBAL_ASM stays in functions_2A0000s.cpp / functions_280000s.cpp.
#if 0
void caseD_3_002aecd8(DemoGt* self, PlayState* play) {
    DemoGt* s2 = self;
    Actor* thisx = &self->actor;
    PlayState* play2 = play;

    FUN_0032ae1c(self, play, 4);
    FUN_0032a998(self, 4);
    {
        CutsceneContext* csCtx = &play->csCtx;
        u16 frames = csCtx->frames;

        if (frames == 0x29D) {
            Audio_PlaySoundGeneral(0x01000235, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                                   &DAT_0054ac24.unk_0);
        }
    }
    {
        DemoGtCsCue* cue = DemoGt_GetCueFromCtx(play, &play->csCtx, 4);

        if ((cue != NULL) && (cue->id == 2)) {
            self->updateMode = 11;
        } else {
            cue = DemoGt_GetCueFromCtx(play, &play->csCtx, 4);
            if ((cue != NULL) && (cue->id == 3)) {
                self->updateMode = 16;
            }
        }
    }
}
#endif

void caseD_d_002a13a0(DemoGt* self, PlayState* play2) {
    PlayState* play = play2;

    FUN_0032ae1c(self, play, 6);
    FUN_0032a998(self, 6);
    {
        u16 frames = play->csCtx.frames;

        if ((s32)((f32)(3 * 845) / SREG(30) + 0.5f) == frames) {
            Audio_PlaySoundGeneral(0x01000235, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                                   &DAT_0054ac24.unk_0);
        }
    }
}


// caseD_f_0027dbb0 + caseD_8_0027d884: NOT matched (cascade stage 1, batch 5). Drafts kept under #if 0,
// GLOBAL_ASM stays in functions_240000s.cpp. caseD_f (head split at 0x27dd34 -> FUN_0027dd34): 7 words off. The
// `s16 cur = *pSpeed;` read before the rounding if/else fixed the ldrsh reload (2026-10-14 near-miss sweep). Left: play
// r6 vs gGameInfo-literal r5 swap (ROM play r5); single-site/pair mutations, play2/thisx/state signatures, N64 inline
// helpers (speed/sfx/ED64), sfx frames forms: no change. R11: 6 real words (the r5/r6 swap only; pool word is a reloc);
// play2/state alias at every decl position, block play2 per call, Actor* thisx, N64 Speed15/Sfx15 helpers
// (inline/static, with/without play), greedy+pair mutations: 7 or worse. caseD_8: 11/106 words off (`u16 frames = play->csCtx.frames;` local fixed the
// gGameInfo/offset reg swap; remaining: ROM re-reads pSpeed with ldrsh and keeps the sxth result in r2 for the clamp
// compare, ours ldrh + r1).
#if 0
void FUN_0032aee0(DemoGt* self, PlayState* play);
void FUN_0032a6f4(DemoGt* self, PlayState* play);
void FUN_003674e4(s32 seqId);

// caseD_f_0027dbb0 (N64 DemoGt_Update15: speed func_809820E0 + cue ch9 + func_809820AC sfx at ADJUST(154)).
void caseD_f_0027dbb0(DemoGt* self, PlayState* play) {
    s16* pDir = &self->unk_1CC;
    s16* pSpeed = &self->unk_1CA;
    f32 temp = *pSpeed;
    s32 delta;

    *pDir = temp * ((kREG(64) * 0.001f) + 0.048f) + (kREG(79) + 100.0f);
    {
        s16 cur = *pSpeed;

        if (*pDir > 0) {
            delta = (s32)((((f32)*pDir) * 2.0f / 3.0f) + 0.5f);
        } else {
            delta = (s32)((((f32)*pDir) * 2.0f / 3.0f) - 0.5f);
        }
        *pSpeed = cur + delta;
    }
    if (*pSpeed > (s16)(kREG(81) + 0x4000)) {
        *pSpeed = kREG(81) + 0x4000;
    }

    FUN_0032ae1c(self, play, 9);
    FUN_0032a998(self, 9);

    {
        u16 frames = play->csCtx.frames;

        if ((s32)((f32)(3 * 154) / SREG(30) + 0.5f) == frames) {
            Audio_PlaySoundGeneral(0x01000235, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                                   &DAT_0054ac24.unk_0);
        }
    }
}

// caseD_8_0027d884 (F280 + speed (kREG 72/73) + cue ch1 + FUN_0032a6f4 + sfx 0x263 + tower seq at ADJUST(527)).
void caseD_8_0027d884(DemoGt* self, PlayState* play) {
    s32 delta;

    FUN_0032aee0(self, play);
    {
        s16* pSpeed = &self->unk_1CA;
        s16* pDir = &self->unk_1CC;
        f32 temp = *pSpeed;

        *pDir = temp * ((kREG(64) * 0.001f) + 0.048f) + (kREG(72) + 100.0f);
        if (*pDir > 0) {
            delta = (s32)((((f32)*pDir) * 2.0f / 3.0f) + 0.5f);
        } else {
            delta = (s32)((((f32)*pDir) * 2.0f / 3.0f) - 0.5f);
        }
        *pSpeed += delta;
        if (*pSpeed > (s16)(kREG(73) + 0x250)) {
            *pSpeed = kREG(73) + 0x250;
        }
    }

    FUN_0032ae1c(self, play, 1);
    FUN_0032a998(self, 1);
    FUN_0032a6f4(self, play);

    Audio_PlaySoundGeneral(0x01000263, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);

    {
        u16 frames = play->csCtx.frames;

        if ((s32)((f32)(3 * 527) / SREG(30) + 0.5f) == frames) {
            FUN_003674e4(13);
        }
    }
}
#endif

static inline u8 DemoGt_IsCutsceneLayer(void) {
    if (!(gSaveContext.sceneSetupIndex >= 4)) {
        return false;
    }
    return true;
}

void DemoGt_CollisionHeaderGetVirtual(CollisionHeader* collision, CollisionHeader** out);

u8 FUN_003430fc(Actor* thisx, PlayState* play, s32 updateMode, s32 drawConfig, CollisionHeader* collision) {
    DemoGt* self = (DemoGt*)thisx;
    CollisionHeader* colHeader;

    if (!DemoGt_IsCutsceneLayer()) {
        Actor_Kill(&self->actor);
        return false;
    }

    self->updateMode = updateMode;
    self->drawConfig = drawConfig;
    if (collision != NULL) {
        Actor_ProcessInitChain(&self->actor, Demo_Gt_sInitChain);
        DynaPolyActor_Init((DynaPolyActor*)self, 0);
        colHeader = NULL;
        DemoGt_CollisionHeaderGetVirtual(collision, &colHeader);
        self->bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->actor, colHeader);
    }
    return true;
}

}
