extern "C" {
#include "z_en_syateki_niw.hpp"
#include "model_manager.hpp"
#include "mtx34_trans.hpp"

#define FLAGS 0x00000010
#define R_UPDATE_RATE SREG(30)
#define SHOOTING_GALLERY_STATUS(p) (*(s8*)((u8*)(p) + 0x5C74)) // PlayState, no field in repo header
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define FRAMES(n) ((n) * 3.0f / R_UPDATE_RATE + 0.5f)

void EnSyatekiNiw_Init(Actor* thisx, PlayState* play);
void EnSyatekiNiw_Destroy(Actor* thisx, PlayState* play);
void EnSyatekiNiw_Update(Actor* thisx, GameState* state);
void EnSyatekiNiw_Draw(Actor* actor, GameState* state);

void FUN_00353e78(void* zar, PlayState* play, SkelAnime* skel, void* cmb, void* p, s32 b, void* jointTable,
                  void* morphTable, s32 limbCount);
void* ZAR_GetCMBByIndex(void* zar, s32 index);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void FUN_003344f4(EnSyatekiNiw* self, PlayState* play, s16 animationType);
void FUN_003eafa4(EnSyatekiNiw* self, PlayState* play);
void FUN_003ca7b0(EnSyatekiNiw* self, PlayState* play);
void FUN_00181800(EnSyatekiNiw* self, PlayState* play);
void FUN_0021d804(EnSyatekiNiw* self, PlayState* play);

GLOBAL_ASM("data/z_En_Syateki_Niw.data.s")
GLOBAL_ASM("../src/game/actors/En_Syateki_Niw/data_syms.s")

extern ColliderCylinderInit EnSyatekiNiw_sCylinderInit;
extern InitChainEntry EnSyatekiNiw_sInitChain[];

ActorInit En_Syateki_Niw_InitVars = {
    ACTOR_EN_SYATEKI_NIW,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_NIW,
    sizeof(EnSyatekiNiw),
    (ActorFunc)EnSyatekiNiw_Init,
    (ActorFunc)EnSyatekiNiw_Destroy,
    (ActorFunc)EnSyatekiNiw_Update,
    (ActorFunc)EnSyatekiNiw_Draw,
};

static inline u8* EnSyatekiNiw_GetObjBank(PlayState* play, u32 idx) {
    if (idx < 0x13 && *(s32*)((u8*)play + (idx << 7) + 0x3A64) != 0) {
        return (u8*)play + (idx << 7) + 0x3A5C;
    }
    return NULL;
}

void EnSyatekiNiw_Init(Actor* thisx, PlayState* play) {
    EnSyatekiNiw* self = (EnSyatekiNiw*)thisx;
    u8* zar;
    void* cmb;
    s32 i;

    Actor_ProcessInitChain(thisx, EnSyatekiNiw_sInitChain);
    self->actor.flags &= ~1;
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 25.0f);
    zar = EnSyatekiNiw_GetObjBank(play, (u8)self->actor.objBankIndex) + 0x10;
    cmb = ZAR_GetCMBByIndex(zar, 0);
    FUN_00353e78(zar, play, &self->skelAnime, cmb, self->actor.unk_178, 0, self->jointTable, self->morphTable, 9);
    for (i = 0; i < 5; i++) {
        FUN_00372f38(&self->actor, play, &self->effects[i].model, 1, 0);
    }

    self->minigameType = self->actor.params;
    if (self->minigameType < 0) {
        self->minigameType = SYATEKI_MINIGAME_ARCHERY;
    }

    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, thisx, &EnSyatekiNiw_sCylinderInit);
    if (self->minigameType == SYATEKI_MINIGAME_ARCHERY) {
        Actor_SetScale(&self->actor, 0.01f);
    } else {
        self->actor.colChkInfo.mass = MASS_IMMOVABLE;
        Actor_SetScale(&self->actor, 0.01f);
    }

    self->initPos = self->actor.world.pos;
    self->targetPos = self->actor.world.pos;
    self->actionFunc = FUN_00181800;
}

static inline void EnSyatekiNiw_DestroyCollider(EnSyatekiNiw* self, PlayState* play) {
    Collider_DestroyCylinder(play, &self->collider);
}

void EnSyatekiNiw_Destroy(Actor* thisx, PlayState* play) {
    EnSyatekiNiw* self = (EnSyatekiNiw*)thisx;
    s32 i;

    EnSyatekiNiw_DestroyCollider(self, play);
    SkelAnime_Free2(&self->skelAnime);
    for (i = 0; i < 5; i++) {
        FUN_003508b8(&self->actor, self->effects[i].model, 0);
    }
}

void FUN_00181800(EnSyatekiNiw* self, PlayState* play) {
    f32 end = (f32)Animation_GetLastFrame(&self->skelAnime, 0);

    Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, end, 0, -10.0f);
    if (self->minigameType != SYATEKI_MINIGAME_ARCHERY) {
        Actor_SetScale(&self->actor, self->scale);
    }
    self->actionFunc = FUN_003eafa4;
}

void FUN_00206320(EnSyatekiNiw* self, PlayState* play) {
    f32 end = (f32)Animation_GetLastFrame(&self->skelAnime, 0);

    Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, end, 0, -10.0f);
    self->unkArcheryFloat = -10000.0f;
    self->rightWingRot.z = self->rightWingRotZTarget = 6000.0f;
    self->rightWingRot.y = 10000.0f;
    self->rightWingRot.x = self->rightWingRotXTarget = -10000.0f;
    self->leftWingRot.z = self->leftWingRotZTarget = 6000.0f;
    self->leftWingRot.y = self->leftWingRot.x = self->leftWingRotXTarget = -10000.0f;
    self->actionFunc = FUN_0021d804;
}

void FUN_00158508(EnSyatekiNiw* self, PlayState* play) {
    s16 screenX;
    s16 screenY;

    Actor_SetFocus(&self->actor, self->focusYOffset);
    Actor_GetScreenPos(play, &self->actor, &screenX, &screenY);
    if ((self->actor.unk_EC.z > 200.0f) && (self->actor.unk_EC.z < 800.0f)) {
        if ((screenX > 0) && (screenX < SCREEN_WIDTH) && (screenY > 0) && (screenY < SCREEN_HEIGHT)) {
        self->actor.speedXZ = 5.0f;
        self->rotYFlip = Rand_ZeroFloat(1.99f);
        self->removeStateYaw = Rand_CenteredFloat(8000.0f) + -10000.0f;
        self->cluckTimer = FRAMES(30);
        self->movementTimer = FRAMES(100);
        self->actionFunc = FUN_003ca7b0;
        }
    }
}

void FUN_003ca7b0(EnSyatekiNiw* self, PlayState* play) {
    EnSyatekiNiw* s2 = self;
    f32 focus = self->focusYOffset;
    s16 screenX;
    s16 screenY;
    f32 rotYTargetOffset;
    f32 rotYTarget;

    Actor_SetFocus(&self->actor, focus);
    Actor_GetScreenPos(play, &self->actor, &screenX, &screenY);
    if ((s2->movementTimer == 0) || (self->actor.unk_EC.z < -70.0f) || ((u32)screenX > SCREEN_WIDTH) ||
        (screenY < 0) || (screenY > SCREEN_HEIGHT)) {
        Actor_Kill(&self->actor);
        return;
    }
    self->spawnFeathers = true;
    if (self->hopTimer == 0) {
        self->rotYFlip++;
        self->rotYFlip &= 1;
        self->hopTimer = FRAMES((s16)Rand_CenteredFloat(4.0f) + 5);
        if ((Rand_ZeroFloat(5.0f) < 1.0f) && (self->actor.bgCheckFlags & 1)) {
            self->actor.velocity.y = 4.0f;
        }
    }

    rotYTargetOffset = (self->rotYFlip == 0) ? 5000.0f : -5000.0f;
    rotYTarget = self->removeStateYaw + rotYTargetOffset;
    Math_SmoothStepToS(&self->actor.world.rot.y, rotYTarget, 3, self->posRotStep.y, 0);
    Math_ApproachF(&self->posRotStep.y, 3000.0f, 1.0f, 500.0f);
    FUN_003344f4(self, play, 2);
}

// FUN_00394300 (3DS SyatekiNiw_OverrideLimbDraw): head of the cfg split (tail FUN_003944F8 at 0x3944F8, 88 B, C below).
// NOT matching (95 of 126 words, 424 B vs 504 B): ROM inlines the 3DS Matrix_RotateAxis (FUN_003625f8: angle * 40.7437 *
// (pi/128) * 40.7437, sincos via FUN_0036c258, normalise axis (0,0,-1)) WITHOUT constant-folding the axis
// (vsqrt/vdiv + all products stay runtime, x/y copied with vmov); armcc folds them for every local/static/ctor shape tried.
// Limbs 4/6 shapes (FUN_0032e438 + FUN_0036c174) are right (right wing: z,y negated, x positive; left: all positive).
#if 0
void FUN_0036c174(MTX34* dst, MTX34* a, MTX34* b);
void FUN_0032e438(MTX34* dst, Vec3f* rot);
void FUN_0036c258(f32* sinOut, f32* cosOut, f32 angle);

// 3DS Matrix_RotateAxis (FUN_003625f8): mirror of the asm body; inlined here with a local axis.
static inline void EnSyatekiNiw_RotateAxis(MTX34* out, Vec3f* axis, f32 angle) {
    f32 sinA;
    f32 cosA;
    f32 omc;
    f32 inv;
    f32 x;
    f32 y;
    f32 z;
    f32 ax;
    f32 ay;
    f32 az;
    f32 tAx;
    f32 tAxAy;
    f32 tAxAz;
    f32 tAyP;
    f32 tAyAz;

    FUN_0036c258(&sinA, &cosA, angle * 40.743664f * 0.024543693f * 40.743664f);
    x = axis->x;
    y = axis->y;
    z = axis->z;
    omc = 1.0f - cosA;
    inv = 1.0f / sqrtf(x * x + y * y + z * z);
    ax = x * inv;
    ay = y * inv;
    az = z * inv;
    tAx = omc * ax;
    tAxAy = tAx * ay;
    tAxAz = tAx * az;
    tAyP = omc * ay;
    tAyAz = tAyP * az;
    out->data[0][0] = cosA + omc * (ax * ax);
    out->data[0][1] = tAxAy - sinA * az;
    out->data[0][2] = tAxAz + sinA * ay;
    out->data[0][3] = 0.0f;
    out->data[1][0] = tAxAy + sinA * az;
    out->data[1][1] = cosA + omc * (ay * ay);
    out->data[1][2] = tAyAz - sinA * ax;
    out->data[1][3] = 0.0f;
    out->data[2][0] = tAxAz - sinA * ay;
    out->data[2][1] = tAyAz + sinA * ax;
    out->data[2][2] = cosA + omc * (az * az);
    out->data[2][3] = 0.0f;
}

s32 FUN_00394300(PlayState* play, s32 limbIndex, MTX34* mtx, EnSyatekiNiw* self) {
    Vec3f rot;
    MTX34 m;

    if (limbIndex == 7) {
        Vec3f axis;

        axis.x = 0.0f;
        axis.y = 0.0f;
        axis.z = -1.0f;
        EnSyatekiNiw_RotateAxis(&m, &axis, self->headRot.x * 0.000095873802f);
        FUN_0036c174(mtx, mtx, &m);
    } else if (limbIndex == 4) {
        rot.x = self->rightWingRot.z * -0.000095873802f;
        rot.y = self->rightWingRot.y * -0.000095873802f;
        rot.z = self->rightWingRot.x * 0.000095873802f;
        FUN_0032e438(&m, &rot);
        FUN_0036c174(mtx, mtx, &m);
    } else if (limbIndex == 6) {
        rot.x = self->leftWingRot.z * 0.000095873802f;
        rot.y = self->leftWingRot.y * 0.000095873802f;
        rot.z = self->leftWingRot.x * 0.000095873802f;
        FUN_0032e438(&m, &rot);
        FUN_0036c174(mtx, mtx, &m);
    }
    return 0;
}
#endif



static void EnSyatekiNiw_SpawnFeather(EnSyatekiNiw* self, Vec3f* pos, Vec3f* vel, Vec3f* accel, f32 scale) {
    s16 i;
    EnSyatekiNiwEffect* effect = &self->effects[0];

    for (i = 0; i < 5; i++, effect++) {
        if (effect->state == 0) {
            effect->state = 1;
            effect->pos = *pos;
            effect->vel = *vel;
            effect->accel = *accel;
            effect->timer = 0;
            effect->scale = scale / 1000.0f;
            effect->lifespan = (s16)Rand_ZeroFloat(20.0f) + 40;
            effect->rotPulse = Rand_ZeroFloat(1000.0f);
            return;
        }
    }
}

static void EnSyatekiNiw_UpdateEffects(EnSyatekiNiw* self, PlayState* play) {
    s16 i;
    EnSyatekiNiwEffect* effect = &self->effects[0];

    for (i = 0; i < 5; i++, effect++) {
        if (effect->state != 0) {
            effect->timer++;
            effect->pos.x += effect->vel.x;
            effect->pos.y += effect->vel.y;
            effect->pos.z += effect->vel.z;
            effect->vel.x += effect->accel.x;
            effect->vel.y += effect->accel.y;
            effect->vel.z += effect->accel.z;
            if (effect->state == 1) {
                effect->rotPulse++;
                Math_ApproachF(&effect->vel.x, 0.0f, 1.0f, 0.05f);
                Math_ApproachF(&effect->vel.z, 0.0f, 1.0f, 0.05f);
                if (effect->vel.y < -0.5f) {
                    effect->vel.y = 0.5f;
                }
                effect->rot = (Math_SinS(effect->rotPulse * 3000) * M_PI) * 0.2f;
                if (effect->timer > effect->lifespan) {
                    effect->state = 0;
                }
            }
        }
    }
}

static void EnSyatekiNiw_CheckHit(EnSyatekiNiw* self, PlayState* play) {
    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        switch (self->minigameType) {
            case SYATEKI_MINIGAME_ARCHERY:
                if (!self->isFalling) {
                    self->cluckTimer = FRAMES(30);
                    Audio_PlayActorSound2(&self->actor, 0x01000169);
                    self->actor.gravity = -3.0f;
                    self->isFalling = true;
                    self->spawnFeathers = true;
                    self->actionFunc = FUN_00206320;
                }
                break;
            case SYATEKI_MINIGAME_ALLEY:
                self->cluckTimer = FRAMES(30);
                self->unkAlleyHitByte = 1;
                Audio_PlayActorSound2(&self->actor, 0x01000169);
                self->spawnFeathers = true;
                self->sootTimer = FRAMES(100);
                self->movementTimer = self->sootTimer;
                break;

        }
    }
}

void FUN_003eafa4(EnSyatekiNiw* self, PlayState* play);

void EnSyatekiNiw_Update(Actor* thisx, GameState* state) {
    EnSyatekiNiw* self = (EnSyatekiNiw*)thisx;
    PlayState* play = (PlayState*)state;
    s16 i;
    Vector3f sp90(0.0f, 0.0f, 0.0f); // unused in the N64 source too
    Vector3f sp84(0.0f, 0.0f, 0.0f);
    Vec3f pos;
    Vec3f vel;
    Vec3f accel;

    EnSyatekiNiw_UpdateEffects(self, play);
    self->lifetime++;
    if (self->peckTimer != 0) {
        self->peckTimer--;
    }
    if (self->flapTimer != 0) {
        self->flapTimer--;
    }
    if (self->archeryTimer != 0) {
        self->archeryTimer--;
    }
    if (self->hopTimer != 0) {
        self->hopTimer--;
    }
    if (self->movementTimer != 0) {
        self->movementTimer--;
    }
    if (self->cluckTimer != 0) {
        self->cluckTimer--;
    }
    if (self->sootTimer != 0) {
        self->sootTimer--;
    }

    self->actor.shape.shadowScale = 15.0f;
    self->actor.shape.rot = self->actor.world.rot;
    self->actionFunc(self, play);
    Actor_MoveForward(thisx);
    Actor_UpdateBgCheckInfo(play, &self->actor, 20.0f, 20.0f, 60.0f, 0x1D);

    if (self->spawnFeathers) {
        for (i = 0; i < 20; i++) {
            pos.x = Rand_CenteredFloat(10.0f) + self->actor.world.pos.x;
            pos.y = Rand_CenteredFloat(10.0f) + 20.0f + self->actor.world.pos.y;
            pos.z = Rand_CenteredFloat(10.0f) + self->actor.world.pos.z;
            vel.x = Rand_CenteredFloat(3.0f);
            vel.y = (Rand_ZeroFloat(2.0f) * 0.5f) + 2.0f;
            vel.z = Rand_CenteredFloat(3.0f);
            accel.x = 0.0f;
            accel.y = -0.15f;
            accel.z = 0.0f;
            EnSyatekiNiw_SpawnFeather(self, &pos, &vel, &accel, Rand_ZeroFloat(8.0f) + 8.0f);
        }
        self->spawnFeathers = false;
    }

    EnSyatekiNiw_CheckHit(self, play);
    if (self->cluckTimer == 0) {
        if (self->actionFunc == FUN_003eafa4) {
            self->cluckTimer = FRAMES(300);
            Audio_PlayActorSound2(&self->actor, 0x01000168);
        } else {
            self->cluckTimer = FRAMES(30);
            Audio_PlayActorSound2(&self->actor, 0x01000169);
        }
    }

    i = false;
    switch (self->minigameType) {
        case SYATEKI_MINIGAME_ARCHERY:
            if (SHOOTING_GALLERY_STATUS(play) != 0) {
                i = true;
            }
            break;

        case SYATEKI_MINIGAME_ALLEY:
            i = true;
            break;
    }

    if (i) {
        Collider_UpdateCylinder(thisx, &self->collider);
        CollisionCheck_SetAC((PlayState*)state, &play->colChkCtx, &self->collider.base);
        CollisionCheck_SetOC((PlayState*)state, &play->colChkCtx, &self->collider.base);
    }
}

// EnSyatekiNiw_Draw: NOT matching (111 of 146 words, mnemonic diff 25). Draft below compiles to 604 B vs 584 B ROM.
// Blocker = known zero/1.0 remat class: ROM keeps only `1` (SetUnkAC) hoisted in r7 and re-materialises 0 / 0x3F800000
// inside the loop (strd r0,r1 pairs, MTX34 at sp+0, push {r4-r7,lr}); armcc hoists 0 (r7) and 1.0 (r8) instead.
// Tried: Vec3f/Vector3f/EnSyatekiNiwVec pos, SetComp-style helper, Matrix_Translate inline copy with mode param,
// scale/rot as inline helpers, `u8 one` local, mtx fn-scope, index vs pointer loop, plain static helpers.
// Needs 0x004D9D2C EnSyatekiNiw_sColors EQU (3 black colors of the TU rodata; sootShade = [2]) in data_syms.s.
#if 0
f32 __hardfp_sinf(f32 x);
f32 __hardfp_cosf(f32 x);
void FUN_0032d184(PlayState* play, Actor* actor, void* color, s32 a, s32 b); // func_80026230 (fog)
void FUN_0032d278(PlayState* play, Actor* actor); // func_80026608, empty stub (linker-inlined)
void FUN_00394300(PlayState* play, s32 limbIndex, MTX34* mtx, EnSyatekiNiw* self);

extern u32 EnSyatekiNiw_sColors[];

// Inline copy of the 3DS Matrix_Translate (FUN_003713fc): mode 1 = apply, otherwise identity + translation.
static void EnSyatekiNiw_Translate(MTX34* m, s32 mode, f32 x, f32 y, f32 z) {
    Vec3f t;

    t.x = x;
    t.y = y;
    t.z = z;
    if (mode == 1) {
        Matrix_UpdatePosition(m, m, &t);
    } else {
        MTX34_MakeTrans(m, &t);
    }
}

static void EnSyatekiNiw_ScaleXY(MTX34* m, f32 sx, f32 sy) {
    m->data[0][0] *= sx;
    m->data[1][0] *= sx;
    m->data[2][0] *= sx;
    m->data[0][1] *= sy;
    m->data[1][1] *= sy;
    m->data[2][1] *= sy;
}

static void EnSyatekiNiw_RotateZ(MTX34* m, f32 angle) {
    if (angle != 0.0f) {
        f32 sn = __hardfp_sinf(angle);
        f32 cs = __hardfp_cosf(angle);
        f32 a0 = m->data[0][0];
        f32 b0 = m->data[0][1];
        f32 a1;
        f32 b1;
        f32 a2;
        f32 b2;

        m->data[0][0] = a0 * cs + b0 * sn;
        m->data[0][1] = b0 * cs - a0 * sn;
        a1 = m->data[1][0];
        b1 = m->data[1][1];
        m->data[1][0] = a1 * cs + b1 * sn;
        m->data[1][1] = b1 * cs - a1 * sn;
        a2 = m->data[2][0];
        b2 = m->data[2][1];
        m->data[2][0] = a2 * cs + b2 * sn;
        m->data[2][1] = b2 * cs - a2 * sn;
    }
}

static void EnSyatekiNiw_DrawEffects(EnSyatekiNiw* self, PlayState* play) {
    EnSyatekiNiwEffect* effect = &self->effects[0];
    s16 i;

    for (i = 0; i < 5; i++, effect++) {
        if (effect->state == 1) {
            MTX34 mtx;

            MTX34CopyAsm(&mtx, &self->actor.modelMtx);
            EnSyatekiNiw_Translate(&mtx, 0, effect->pos.x, effect->pos.y, effect->pos.z);
            EnSyatekiNiw_ScaleXY(&mtx, effect->scale, effect->scale);
            EnSyatekiNiw_RotateZ(&mtx, effect->rot);
            EnSyatekiNiw_Translate(&mtx, 1, 0.0f, -1000.0f, 0.0f);
            effect->model->SetUnkAC(1);
            effect->model->FUN_003721e0(&mtx);
            effect->model->FUN_00372170(0);
        }
    }
}

void EnSyatekiNiw_Draw(Actor* thisx, GameState* state) {
    EnSyatekiNiw* self = (EnSyatekiNiw*)thisx;
    PlayState* play = (PlayState*)state;
    u32 sootShade = EnSyatekiNiw_sColors[2];

    if (self->actionFunc != FUN_00158508) {
        if (self->sootTimer != 0) {
            FUN_0032d184(play, thisx, &sootShade, 0, 20);
        }
        SkelAnime_DrawOpa(&self->skelAnime, &thisx->modelMtx, (OverrideLimbDrawOpa)FUN_00394300, NULL, thisx, 0);
        FUN_0032d278(play, thisx);
        EnSyatekiNiw_DrawEffects(self, play);
    }
}
#endif

void Matrix_RotateZ(MTX34* mtx, s32 mode, f32 angle);
void Matrix_RotateY(MTX34* mtx, s32 mode, f32 angle);

typedef struct FUN_003944F8_Obj {
    /* 0x00 */ char unk_00[0x14];
    /* 0x14 */ f32 rotZ;
    /* 0x18 */ s16 rotY;
} FUN_003944F8_Obj;

s32 FUN_003944F8(PlayState* play, s32 limbIndex, MTX34* mtx, Actor* thisx) {
    FUN_003944F8_Obj* obj = *(FUN_003944F8_Obj**)((u8*)thisx + 0x16B4);

    if (limbIndex == 0) {
        Matrix_RotateZ(mtx, 1, obj->rotZ);
        Matrix_RotateY(mtx, 1, obj->rotY * 0.000095873802f);
    }
    return 0;
}

GLOBAL_ASM("asm/EnSyatekiNiw_Draw.s")
}
