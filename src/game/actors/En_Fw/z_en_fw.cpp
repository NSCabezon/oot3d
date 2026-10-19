extern "C" {
#include "z_en_fw.hpp"

#define FLAGS 0x00000215

void EnFw_Init(Actor* actor, GameState* state);
void EnFw_Destroy(Actor* actor, GameState* state);
void EnFw_Update(Actor* actor, GameState* state);
void EnFw_Draw(Actor* actor, GameState* state);
void EnFw_Run(EnFw* self, PlayState* play);
void EnFw_Bounce(EnFw* self, PlayState* play);

GLOBAL_ASM("../src/game/actors/En_Fw/data_syms.s")

extern struct_80034EC0_Entry EnFw_sAnimationInfo[];

GLOBAL_ASM("data/z_En_Fw.data.s")

ActorInit En_Fw_InitVars = {
    ACTOR_EN_FW,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_FW,
    sizeof(EnFw),
    (ActorFunc)EnFw_Init,
    (ActorFunc)EnFw_Destroy,
    (ActorFunc)EnFw_Update,
    (ActorFunc)EnFw_Draw,
};

void Collider_UpdateSpheres(s32 limbIndex, void* collider, MTX34* mtx);

static inline void EnFw_MultVec3f(Vector3f* src, Vec3f* dest, MTX34* mtx) {
    VEC3TransformAsm((Vector3f*)dest, mtx, src);
}

// PostLimbDraw (ROM name kept: FUN_00217014).
void FUN_00217014(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnFw* self = (EnFw*)thisx;
    Vector3f zeroVec(0.0f, 0.0f, 0.0f);

    if (limbIndex == 1) {
        EnFw_MultVec3f(&zeroVec, &self->bompPos, mtx);
    } else if (limbIndex == 2) {
        EnFw_MultVec3f(&zeroVec, &self->actor.focus.pos, mtx);
    }

    Collider_UpdateSpheres(limbIndex, &self->collider, mtx);
}

void* SystemArena_Malloc_0035010c(s32 size);
void* FUN_003500c4(void* p);
void FUN_0034ff2c(void* obj, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void FUN_0034fea8(void* obj, s32 bank, GameState* state, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7);
DamageTable* DamageTable_Get(s32 index);
void Collider_InitJntSph(PlayState* play, void* collider);
void Collider_SetJntSph(PlayState* play, void* collider, Actor* actor, void* init, void* elements);

extern u8 EnFw_sJntSphInit[];
extern CollisionCheckInfoInit2 EnFw_sColChkInfoInit;

void EnFw_Init(Actor* actor, GameState* state) {
    EnFw* self = (EnFw*)actor;
    PlayState* play = (PlayState*)state;
    void* obj = SystemArena_Malloc_0035010c(0x28);

    if (obj != NULL) {
        obj = FUN_003500c4(obj);
    }
    self->unk_B30 = obj;
    FUN_0034ff2c(self->unk_B30, 4, 4, 0x14, 1, 0);
    FUN_0034fea8(self->unk_B30, 0, state, 0x10, 0x2601, 0x2601, 0x812F, 0x812F);
    FUN_00353c9c(actor, play, &self->skelAnime, 1, 0, self->jointTable, self->morphTable, 10);
    FUN_003717ac(&self->skelAnime, (struct_80034EC0_Entry*)EnFw_sAnimationInfo, 0);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 20.0f);
    Collider_InitJntSph(play, &self->collider);
    Collider_SetJntSph(play, &self->collider, &self->actor, EnFw_sJntSphInit, &self->unk_65C);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, DamageTable_Get(0x10), &EnFw_sColChkInfoInit);
    Actor_SetScale(&self->actor, 0.01f);
    self->actor.gravity = -1.0f;
    self->runDirection = -self->actor.params;
    self->actor.naviEnemyId = 0x5D;
    self->actionFunc = EnFw_Bounce;
}

void FUN_0034fc7c(void* p);
void SystemArena_Free(void* p);
void SkelAnime_Free2(SkelAnime* skelAnime);
void Collider_DestroyJntSph(PlayState* play, void* collider);

void EnFw_Destroy(Actor* actor, GameState* state) {
    EnFw* self = (EnFw*)actor;

    SkelAnime_Free2(&self->skelAnime);
    if (self->unk_B30 != NULL) {
        FUN_0034fc7c(self->unk_B30);
        SystemArena_Free(self->unk_B30);
    }
    self->unk_B30 = NULL;
    Collider_DestroyJntSph((PlayState*)state, &self->collider);
}

void EnFw_Update(Actor* actor, GameState* state) {
    EnFw* self = (EnFw*)actor;
    PlayState* play = (PlayState*)state;

    FUN_003731e0(&self->skelAnime);
    if (!(self->actor.flags & 0x2000)) {
        Actor_MoveForward(&self->actor);
        Actor_UpdateBgCheckInfo(play, &self->actor, 10.0f, 20.0f, 0.0f, 5);
        self->actionFunc(self, play);
        if (self->damageTimer == 0 && self->explosionTimer == 0 && self->actionFunc == EnFw_Run) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
        }
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    }
}

inline void* operator new(unsigned int, void* p) { return p; }
void FUN_0035619c(PlayState* play, f32* primOut, f32* envOut, s32 r, s32 g, s32 b, s32 a, s32 er, s32 eg, s32 eb);
void FUN_00342988(void* batch, f32* color, s32 arg);
void FUN_003693b4(void* batch, Vec3f* pos, s32 arg2, Vec3f* scale, f32* color, s32 imageIdx);
void FUN_00371eac(void* arg, s32 arg1);
void SkelAnime_DrawOpa(SkelAnime* skelAnime, MTX34* modelMtx, OverrideLimbDrawOpa overrideLimbDraw,
                       PostLimbDrawOpa postLimbDraw, Actor* actor, s32 arg5);
void FUN_00217014(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);

void EnFw_Draw(Actor* actor, GameState* state) {
    EnFw* self = (EnFw*)actor;
    PlayState* play = (PlayState*)state;
    EnFwEffect* eff;
    Vec3f pos;
    Vec3f scale;
    f32 primColor[4];
    f32 envColor[4];
    s16 i;

    eff = self->effects;
    for (i = 0; i < EN_FW_EFFECT_COUNT; i++, eff++) {
        if (eff->type != 0) {
            eff->timer--;
            if (eff->timer == 0) {
                eff->type = 0;
            }
            eff->accel.x = Rand_ZeroOne() * 0.4f - 0.2f;
            eff->accel.z = Rand_ZeroOne() * 0.4f - 0.2f;
            eff->pos.x += eff->velocity.x * (SREG(30) * (1.0f / 3.0f));
            eff->pos.y += eff->velocity.y * (SREG(30) * (1.0f / 3.0f));
            eff->pos.z += eff->velocity.z * (SREG(30) * (1.0f / 3.0f));
            eff->velocity.x += eff->accel.x * (SREG(30) * (1.0f / 3.0f));
            eff->velocity.y += eff->accel.y * (SREG(30) * (1.0f / 3.0f));
            eff->velocity.z += eff->accel.z * (SREG(30) * (1.0f / 3.0f));
            eff->scale += eff->scaleStep * (SREG(30) * (1.0f / 3.0f));
        }
    }

    eff = self->effects;
    FUN_0035619c(play, primColor, envColor, 170, 130, 90, 255, 100, 60, 20);
    FUN_00342988(self->unk_B30, envColor, -1);
    for (i = 0; i < EN_FW_EFFECT_COUNT; i++, eff++) {
        if (eff->type == 0) {
            continue;
        }
        new (&pos) Vector3f(eff->pos.x, eff->pos.y, eff->pos.z);
        new (&scale) Vector3f(eff->scale * 40.0f, eff->scale * 40.0f, 1.0f);
        FUN_003693b4(self->unk_B30, &pos, 0, &scale, primColor, (s16)(16 - (s16)(eff->timer * (16.0f / eff->initialTimer))));
    }
    FUN_00371eac(*(void**)((u8*)self->unk_B30 + 8), 0);
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, NULL, (PostLimbDrawOpa)FUN_00217014, actor, 0);
}

static inline s32 EnFw_DoBounce(EnFw* self, s32 totalBounces, f32 yVelocity) {
    if (!(self->actor.bgCheckFlags & 1) || (self->actor.velocity.y > 0.0f)) {
        // not on the ground or moving upwards.
        return false;
    }

    Audio_PlayActorSound2(&self->actor, 0x010002EB);
    self->bounceCnt--;
    if (self->bounceCnt <= 0) {
        if (self->bounceCnt == 0) {
            self->bounceCnt = 0;
            self->actor.velocity.y = 0.0f;
            return true;
        }
        self->bounceCnt = totalBounces;
    }
    self->actor.velocity.y = yVelocity;
    self->actor.velocity.y *= (f32)self->bounceCnt / totalBounces;
    return true;
}

void EnFw_Bounce(EnFw* self, PlayState* play) {
    if (EnFw_DoBounce(self, 3, 8.0f) && self->bounceCnt == 0) {
        self->returnToParentTimer = Rand_S16Offset_003702c8(300, 150);
        self->actionFunc = EnFw_Run;
    }
}

void EnFw_JumpToParentInitPos(EnFw* self, PlayState* play) {
    if ((self->actor.bgCheckFlags & 1) && self->actor.velocity.y <= 0.0f) {
        self->actor.parent->params |= 0x8000;
        Actor_Kill(&self->actor);
    } else {
        Math_SmoothStepToF(&self->actor.world.pos.x, self->actor.parent->home.pos.x, 0.6f, 8.0f, 0.0f);
        Math_SmoothStepToF(&self->actor.world.pos.z, self->actor.parent->home.pos.z, 0.6f, 8.0f, 0.0f);
    }
}

s32 Actor_ApplyDamage(Actor* actor);
void FUN_00375b70(PlayState* play, Actor* actor); // Enemy_StartFinishingBlow
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);
void Item_DropCollectibleRandom(PlayState* play, Actor* fromActor, Vec3f* pos, s32 params);
s32 BgCheck_EntityLineTest1(void* colCtx, Vec3f* posA, Vec3f* posB, Vec3f* posResult, void** outPoly, s32 checkWall,
                            s32 checkFloor, s32 checkCeil, s32 actorBgId, s32* bgId);
void EnFw_TurnToParentInitPos(EnFw* self, PlayState* play);
s32 EnFw_SpawnDust(EnFw* self, u8 timer, f32 scale, f32 scaleStep, s32 dustCnt, f32 radius, f32 xzAccel, f32 yAccel);

#define EN_FW_FRAMES23(n) (((n) > 0) ? (s32)(0.5f + (((n) * 2.0f) * (1.0f / 3.0f))) : (s32)((((n) * 2.0f) * (1.0f / 3.0f)) - 0.5f))
#define ENFW_DECR(x) ((x) == 0 ? 0 : --(x))

static inline s32 EnFw_CheckCollider(EnFw* self, PlayState* play) {
    if (self->collider.base.acFlags & 2) {
        u8* elem = (u8*)self->collider.elements;

        if (**(u32**)(elem + 0x24) & 0x80) {
            self->lastDmgHook = true;
        } else {
            self->lastDmgHook = false;
        }
        self->collider.base.acFlags &= ~2;
        if (Actor_ApplyDamage(&self->actor) <= 0) {
            if (self->actor.parent->colChkInfo.health <= 8) {
                FUN_00375b70(play, &self->actor);
                self->actor.parent->colChkInfo.health = 0;
            } else {
                self->actor.parent->colChkInfo.health -= 8;
            }
            self->returnToParentTimer = 0;
        }
        return true;
    }
    return false;
}

static inline s32 EnFw_PlayerInRange(EnFw* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    void* poly;
    s32 bgId;
    Vec3f collisionPos;

    if (self->actor.xzDistToPlayer > 300.0f) {
        return false;
    }
    s16 yd = (s16)((f32)self->actor.yawTowardsPlayer - (f32)self->actor.shape.rot.y);
    if (ABS(yd) > 0x1C70) {
        return false;
    }
    if (BgCheck_EntityLineTest1(&play->colCtx, &self->actor.world.pos, &player->actor.world.pos, &collisionPos, &poly, 1,
                                0, 0, 1, &bgId)) {
        return false;
    }
    return true;
}

static inline Vec3f* EnFw_GetPosAdjAroundCircle(Vec3f* dst, EnFw* self, f32 radius, s16 dir) {
    s16 angle;
    Vec3f posAdj;

    angle = Math_Vec3f_Yaw(&self->actor.parent->home.pos, &self->actor.world.pos) + (dir * 0x1554);
    posAdj.x = (radius * Math_SinS(angle)) + self->actor.parent->home.pos.x;
    posAdj.z = (radius * Math_CosS(angle)) + self->actor.parent->home.pos.z;
    posAdj.x -= self->actor.world.pos.x;
    posAdj.z -= self->actor.world.pos.z;
    dst->x = posAdj.x;
    dst->z = posAdj.z;
    return dst;
}

void EnFw_Run(EnFw* self, PlayState* play) {
    f32 tmpAngle;
    f32 facingDir;
    Actor* bomb;

    Math_SmoothStepToF(&self->skelAnime.playSpeed, 1.0f, 0.1f, 1.0f, 0.0f);
    if (self->skelAnime.animationType == 5) {
        if (Animation_OnFrameImpl(&self->skelAnime, self->skelAnime.endFrame, 1.0f) == 0) {
            self->runRadius = Math_Vec3f_DistXYZ(&self->actor.world.pos, &self->actor.parent->world.pos);
            FUN_003717ac(&self->skelAnime, (struct_80034EC0_Entry*)EnFw_sAnimationInfo, 2);
        }
        return;
    }

    if (self->damageTimer == 0 && self->explosionTimer == 0 && EnFw_CheckCollider(self, play)) {
        if (self->actor.parent->colChkInfo.health > 0) {
            if (!self->lastDmgHook) {
                self->actor.velocity.y = 6.0f;
            }
            Audio_PlayActorSound2(&self->actor, 0x010003F0);
            self->damageTimer = 30;
        } else {
            Audio_PlayActorSound2(&self->actor, 0x010003F0);
            self->explosionTimer = 9;
        }
        self->actor.speedXZ = 0.0f;
    }

    if (self->explosionTimer != 0) {
        self->skelAnime.playSpeed = 0.0f;
        Math_SmoothStepToF(&self->actor.scale.x, 0.024999999f, 0.08f, 0.6f, 0.0f);
        Actor_SetScale(&self->actor, self->actor.scale.x);
        if (*(u16*)((u8*)self + 0x11A) == 0) {
            Actor_SetColorFilter(&self->actor, 0x400000, 200, 0, (s16)EN_FW_FRAMES23(self->explosionTimer));
            self->explosionTimer--;
        }

        if (self->explosionTimer == 0) {
            bomb = Actor_Spawn(&play->actorCtx, play, 0x10, self->bompPos.x, self->bompPos.y, self->bompPos.z, 0, 0,
                               0x600, 0, 1);
            if (bomb != NULL) {
                *(s16*)((u8*)bomb + 0x26C) = 0;
            }
            self->actor.parent->params |= 0x4000;
            Item_DropCollectibleRandom(play, NULL, &self->actor.world.pos, 0xA0);
            Actor_Kill(&self->actor);
            return;
        }
    } else {
        if (!(self->actor.bgCheckFlags & 1) || self->actor.velocity.y > 0.0f) {
            Actor_SetColorFilter(&self->actor, 0x400000, 200, 0, (s16)EN_FW_FRAMES23(self->damageTimer));
            return;
        }
        ENFW_DECR(self->damageTimer);
        if ((200.0f - self->runRadius) < 0.9f) {
            if (ENFW_DECR(self->returnToParentTimer) == 0) {
                self->actor.speedXZ = 0.0f;
                self->actionFunc = EnFw_TurnToParentInitPos;
                return;
            }
        }

        Math_SmoothStepToF(&self->runRadius, 200.0f, 0.3f, 100.0f, 0.0f);

        if (self->turnAround) {
            Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 0.1f, 1.0f, 0.0f);
            tmpAngle = (s16)(self->actor.world.rot.y ^ 0x8000);
            facingDir = self->actor.shape.rot.y;
            tmpAngle = Math_SmoothStepToF(&facingDir, tmpAngle, 0.1f, 10000.0f, 0.0f);
            self->actor.shape.rot.y = facingDir;
            if (tmpAngle > 0x1554) {
                return;
            }
            self->turnAround = false;
        } else {
            Vec3f sp48;

            EnFw_GetPosAdjAroundCircle(&sp48, self, self->runRadius, self->runDirection);
            Math_SmoothStepToS(&self->actor.shape.rot.y, (s16)(Math_FAtan2F(sp48.x, sp48.z) * (65536.0f / (2.0f * 3.14159274f))),
                               4, 0xFA0, 1);
        }

        self->actor.world.rot = self->actor.shape.rot;

        if (self->slideTimer == 0 && EnFw_PlayerInRange(self, play)) {
            Audio_PlayActorSound2(&self->actor, 0x010003FB);
            self->slideSfxTimer = 12;
            self->slideTimer = 12;
        }

        if (self->slideTimer != 0) {
            if (ENFW_DECR(self->slideSfxTimer) == 0) {
                Audio_PlayActorSound2(&self->actor, 0x010003F2);
                self->slideSfxTimer = 6;
            }
            Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 0.1f, 1.0f, 0.0f);
            self->skelAnime.playSpeed = 0.0f;
            EnFw_SpawnDust(self, (s32)(8 * 3.0f / SREG(30) + 0.5f), 0.16f, 0.2f, 3, 8.0f, 20.0f,
                           ((Rand_ZeroOne() - 0.5f) * 0.2f) + 0.3f);
            self->slideTimer--;
            if (self->slideTimer == 0) {
                self->turnAround = true;
                self->runDirection = -self->runDirection;
            }
        } else {
            Math_SmoothStepToF(&self->actor.speedXZ, 6.0f, 0.1f, 1.0f, 0.0f);
            if ((s16)self->skelAnime.curFrame == 1 || (s16)self->skelAnime.curFrame == 4) {
                Audio_PlayActorSound2(&self->actor, 0x010003EF);
                EnFw_SpawnDust(self, (s32)(8 * 3.0f / SREG(30) + 0.5f), 0.16f, 0.1f, 1, 0.0f, 20.0f, 0.0f);
            }
        }
    }
}

// Plain `static` (not inline): armcc inlines it late, which hoists the &local pointers out of the outer loop.
static void EnFw_SpawnEffectDust(EnFw* self, Vec3f* initialPos, Vec3f* initialSpeed, Vec3f* accel, u8 initialTimer,
                                        f32 scale, f32 scaleStep) {
    EnFwEffect* eff = self->effects;
    s16 i;

    for (i = 0; i < EN_FW_EFFECT_COUNT; i++, eff++) {
        if (eff->type != 1) {
            eff->scale = scale;
            eff->scaleStep = scaleStep;
            eff->initialTimer = eff->timer = initialTimer;
            eff->type = 1;
            eff->pos = *initialPos;
            eff->accel = *accel;
            eff->velocity = *initialSpeed;
            return;
        }
    }
}

s32 EnFw_SpawnDust(EnFw* self, u8 timer, f32 scale, f32 scaleStep, s32 dustCnt, f32 radius, f32 xzAccel, f32 yAccel) {
    Vector3f pos(0.0f, 0.0f, 0.0f);
    Vector3f velocity(0.0f, 0.0f, 0.0f);
    Vector3f accel(0.0f, 0.0f, 0.0f);
    s16 angle;
    s32 i;

    *(Vec3f*)&pos = self->actor.world.pos;
    pos.y = self->actor.floorHeight + 2.0f;
    angle = ((Rand_ZeroOne() - 0.5f) * 0x10000);
    i = dustCnt;
    while (i >= 0) {
        accel.x = (Rand_ZeroOne() - 0.5f) * xzAccel;
        accel.y = yAccel;
        accel.z = (Rand_ZeroOne() - 0.5f) * xzAccel;
        pos.x = (Math_SinS(angle) * radius) + self->actor.world.pos.x;
        pos.z = (Math_CosS(angle) * radius) + self->actor.world.pos.z;
        EnFw_SpawnEffectDust(self, (Vec3f*)&pos, (Vec3f*)&velocity, (Vec3f*)&accel, timer, scale, scaleStep);
        i--;
        angle += (s16)(0x10000 / dustCnt);
    }
    return 0;
}


void EnFw_TurnToParentInitPos(EnFw* self, PlayState* play) {
    s16 angleToParentInit;

    angleToParentInit = Math_Vec3f_Yaw(&self->actor.world.pos, &self->actor.parent->home.pos);
    Math_SmoothStepToS(&self->actor.shape.rot.y, angleToParentInit, 4, 0xFA0, 1);
    if (ABS(angleToParentInit - self->actor.shape.rot.y) < 0x65) {
        // angle to parent init pos is ~0.5 degrees
        self->actor.world.rot = self->actor.shape.rot;
        self->actor.velocity.y = 14.0f;
        self->actor.home.pos = self->actor.world.pos;
        Audio_PlayActorSound2(&self->actor, 0x010002DC);
        FUN_003717ac(&self->skelAnime, (struct_80034EC0_Entry*)EnFw_sAnimationInfo, 1);
        self->actionFunc = EnFw_JumpToParentInitPos;
    }
}

}
