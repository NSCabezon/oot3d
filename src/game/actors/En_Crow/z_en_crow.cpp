extern "C" {
#include "z_en_crow.hpp"

#define FLAGS 0x00005005

void EnCrow_Init(Actor* thisx, PlayState* play);
void EnCrow_Destroy(Actor* thisx, PlayState* play);
void EnCrow_Update(Actor* thisx, PlayState* play);
void EnCrow_Draw(Actor* thisx, PlayState* play);

void EnCrow_FlyIdle(EnCrow* self, PlayState* play);
s32 EnCrow_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void EnCrow_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void SkelAnime_SetPlaySpeed(SkelAnime* skelAnime, f32 speed);
void Matrix_RotateY(MTX34* mtx, f32 angle, s32 mode);
f32 sinf(f32 x);
f32* EnCrow_CurFramePtr(SkelAnime* skelAnime);
void FUN_003662d4(EnCrow* self);
void EnCrow_TurnAway(EnCrow* self, PlayState* play);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
#define BGCHECKFLAG_WALL (1 << 3)
void Collider_DestroyJntSph(PlayState* play, void* collider);

GLOBAL_ASM("data/z_En_Crow.data.s")
GLOBAL_ASM("../src/game/actors/En_Crow/data_syms.s")

extern u32 En_Crow_sDeathCount;
extern CollisionCheckInfoInit En_Crow_sColChkInfoInit;
extern EnCrowJntSphInit En_Crow_sJntSphInit;
extern InitChainEntry En_Crow_sInitChain[];
extern Vec3f En_Crow_sZeroVecAccel;
void EnCrow_Die(EnCrow* self, PlayState* play);
void EnCrow_Damaged(EnCrow* self, PlayState* play);
void EnCrow_Respawn(EnCrow* self, PlayState* play);
s32 Math_ScaledStepToS(s16* value, s16 target, s16 step);
void EffectSsDeadDb_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep, s16 primR,
                          s16 primG, s16 primB, s16 primA, s16 envR, s16 envG, s16 envB, s32 unk, s32 life, s32 playSfx);
void Item_DropCollectibleRandom(PlayState* play, Actor* fromActor, Vec3f* spawnPos, s16 params);
s32 Actor_IsFacingPlayer(Actor* actor, s16 angle);
s16 Actor_WorldPitchTowardPoint(Actor* actor, Vec3f* point);
s32 Player_GetMask(PlayState* play);
void Audio_PlayActorSound2(Actor* actor, s32 sfxId);
void EnCrow_SetupTurnAway(EnCrow* self);
f32 Actor_WorldDistXZToPoint(Actor* actor, Vec3f* point);
s16 Actor_WorldYawTowardPoint(Actor* actor, Vec3f* point);
s32 Animation_OnFrameImpl(SkelAnime* skelAnime, f32 frame, f32 updateRate);
void EnCrow_DiveAttack(EnCrow* self, PlayState* play);
void FUN_00375fd0(Actor* actor, void* elem, s32 flag);
void FUN_00375b70(PlayState* play, Actor* actor);
void FUN_0033bd9c(Actor* actor);
void Actor_ApplyDamage(Actor* actor);
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);
void EffectSsEnIce_SpawnFlyingVec3f(PlayState* play, Actor* actor, Vec3f* pos, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                    s32 g, f32 scale);
void EffectSsEnFire_SpawnVec3f(PlayState* play, Actor* actor, Vec3f* pos, s16 scale, s16 params, s16 flags,
                               s16 bodyPart, s32 unk);
void Animation_PlayLoop(SkelAnime* skelAnime, s32 idx);
void Collider_InitJntSph(PlayState* play, void* collider);
void Collider_SetJntSph(PlayState* play, void* collider, Actor* actor, EnCrowJntSphInit* init, EnCrowElem* elements);

ActorInit En_Crow_InitVars = {
    ACTOR_EN_CROW,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_CROW,
    sizeof(EnCrow),
    (ActorFunc)EnCrow_Init,
    (ActorFunc)EnCrow_Destroy,
    (ActorFunc)EnCrow_Update,
    (ActorFunc)EnCrow_Draw,
};

static inline void EnCrow_SetupFlyIdle(Actor* thisx) {
    EnCrow* self = (EnCrow*)thisx;
    self->timer = 150;
    SkelAnime_SetPlaySpeed(&self->skelAnime, 1.0f);
    self->colliderAcFlags |= AC_ON;
    self->actionFunc = EnCrow_FlyIdle;
}

void EnCrow_Init(Actor* thisx, PlayState* play) {
    EnCrow* self = (EnCrow*)thisx;

    Actor_ProcessInitChain(&self->actor, En_Crow_sInitChain);
    FUN_00372f38(&self->actor, play, 0);
    FUN_00353c9c(&self->actor, play, &self->skelAnime, 0, 0, (u8*)self + 0x258, (u8*)self + 0x3F8, 8);
    Collider_InitJntSph(play, &self->unk_5A4);
    Collider_SetJntSph(play, &self->unk_5A4, &self->actor, &En_Crow_sJntSphInit, &self->colliderElemStorage);
    self->colliderElements->worldRadius = En_Crow_sJntSphInit.elements->modelRadius;
    CollisionCheck_SetInfo(&self->actor.colChkInfo, (DamageTable*)((u8*)&En_Crow_sColChkInfoInit + 0x18),
                           &En_Crow_sColChkInfoInit);
    ActorShape_Init(&self->actor.shape, 2000.0f, ActorShadow_DrawCircle, 20.0f);
    En_Crow_sDeathCount = 0;
    EnCrow_SetupFlyIdle(thisx);
}

void EnCrow_Destroy(Actor* thisx, PlayState* play) {
    EnCrow* self = (EnCrow*)thisx;

    Collider_DestroyJntSph(play, (u8*)self + 0x5A4);
    SkelAnime_Free2(&self->skelAnime);
    FUN_003508b8(thisx, 0);
}

// Update: far (63 words). Structure/regs match (self r4, thisx r7, play r6) but armcc hoists `&play->colChkCtx` into r8
// for SetAT/SetAC/SetOC where the ROM recomputes it each time (~25 variants: play2 blocks, raw casts, ctx locals);
// also `adds r1,r0,#0` vs `cmp` on unk_614. Draft kept under #if 0.
#if 0
static inline void EnCrow_UpdateDamage(EnCrow* self, PlayState* play) {
    if (self->colliderAcFlags & AC_HIT) {
        self->colliderAcFlags &= ~(AC_HIT | 4);
        FUN_00375fd0(&self->actor, self->colliderElements, 1);
        if ((self->actor.colChkInfo.damageEffect != 0) || (self->actor.colChkInfo.damage != 0)) {
            if (self->actor.colChkInfo.damageEffect == 1) {
                self->actor.speedXZ = 3.5f;
                self->timer = 150;
                self->aimRotX = -0x1000;
                self->aimRotY = self->actor.yawTowardsPlayer + 0x8000;
                SkelAnime_SetPlaySpeed(&self->skelAnime, 2.0f);
                self->actionFunc = EnCrow_TurnAway;
            } else {
                s32 i;
                f32 scale;
                Vec3f iceParticlePos;

                Actor_ApplyDamage(&self->actor);
                self->actor.flags &= ~1;
                FUN_00375b70(play, &self->actor);
                self->actor.speedXZ *= Math_CosS(self->actor.world.rot.x);
                self->actor.velocity.y = 0.0f;
                {
                    s32 animIdx = self->unk_614;
                    f32 speed = 0.4f;
                    f32 start = 0.0f;
                    f32 morph = -3.0f;

                    if (animIdx != 0) {
                        morph = 0.0f;
                        start = 6.0f;
                        animIdx = 1;
                        speed = morph;
                    }
                    Animation_Change_00375c08(&self->skelAnime, animIdx, speed, start, 0.0f, 1, morph);
                }
                scale = self->actor.scale.x * 100.0f;
                self->actor.bgCheckFlags &= ~1;
                self->actor.world.pos.y += 20.0f * scale;
                self->actor.shape.yOffset = 0.0f;
                self->actor.targetArrowOffset = 0.0f;
                Audio_PlayActorSound2(&self->actor, 0x0100035B);

                if (self->actor.colChkInfo.damageEffect == 3) {
                    Actor_SetColorFilter(&self->actor, 0, 255, 0, 40);
                    for (i = 0; i < 8; i++) {
                        iceParticlePos.x = ((i & 1 ? 7.0f : -7.0f) * scale) + self->actor.world.pos.x;
                        iceParticlePos.y = ((i & 2 ? 7.0f : -7.0f) * scale) + self->actor.world.pos.y;
                        iceParticlePos.z = ((i & 4 ? 7.0f : -7.0f) * scale) + self->actor.world.pos.z;
                        EffectSsEnIce_SpawnFlyingVec3f(play, &self->actor, &iceParticlePos, 150, 150, 150, 250, 235,
                                                       245, 255, ((Rand_ZeroOne() * 0.15f) + 0.85f) * scale);
                    }
                } else if (self->actor.colChkInfo.damageEffect == 2) {
                    Actor_SetColorFilter(&self->actor, 0x400000, 255, 0, 40);
                    for (i = 0; i < 4; i++) {
                        EffectSsEnFire_SpawnVec3f(play, &self->actor, &self->actor.world.pos, 50.0f * scale, 0, 0, i,
                                                  1);
                    }
                } else {
                    Actor_SetColorFilter(&self->actor, 0x400000, 255, 0, 40);
                }

                if (self->actor.flags & 0x8000) {
                    self->actor.speedXZ = 0.0f;
                }
                self->colliderAcFlags &= ~AC_ON;
                self->actor.flags |= 0x10;
                self->actionFunc = EnCrow_Damaged;
            }
        }
    }
}

void EnCrow_Update(Actor* thisx, PlayState* play) {
    EnCrow* self = (EnCrow*)thisx;
    f32 height;
    f32 scale;

    EnCrow_UpdateDamage(self, play);
    self->actionFunc(self, play);
    scale = thisx->scale.x * 100.0f;
    thisx->world.rot.y = thisx->shape.rot.y;
    thisx->world.rot.x = -thisx->shape.rot.x;

    if (self->actionFunc != EnCrow_Respawn) {
        if (thisx->colChkInfo.health != 0) {
            height = 20.0f * scale;
            FUN_0033bd9c(thisx);
        } else {
            height = 0.0f;
            Actor_MoveForward(thisx);
        }
        Actor_UpdateBgCheckInfo(play, thisx, 12.0f * scale, 25.0f * scale, 50.0f * scale, 7);
    } else {
        height = 0.0f;
    }

    self->colliderElements->worldCenter.x = thisx->world.pos.x;
    self->colliderElements->worldCenter.y = thisx->world.pos.y + height;
    self->colliderElements->worldCenter.z = thisx->world.pos.z;

    if (self->actionFunc == EnCrow_DiveAttack) {
        CollisionCheck_SetAT(play, &play->colChkCtx, (Collider*)&self->unk_5A4);
    }

    if (self->colliderAcFlags & AC_ON) {
        CollisionCheck_SetAC(play, &play->colChkCtx, (Collider*)&self->unk_5A4);
    }

    if (self->actionFunc != EnCrow_Respawn) {
        {
            PlayState* play2 = play;
            CollisionCheck_SetOC(play2, &play2->colChkCtx, (Collider*)&self->unk_5A4);
        }
    }

    Actor_SetFocus(thisx, height);

    if (thisx->colChkInfo.health != 0 && Animation_OnFrameImpl(&self->skelAnime, 3.0f, 1.0f)) {
        Audio_PlayActorSound2(thisx, 0x01000307);
    }
}
#endif

GLOBAL_ASM("asm/EnCrow_Update.s")

void EnCrow_Draw(Actor* thisx, PlayState* play) {
    EnCrow* self = (EnCrow*)thisx;

    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, EnCrow_OverrideLimbDraw, EnCrow_PostLimbDraw, thisx, 0);
}


void EnCrow_Damaged(EnCrow* self, PlayState* play) {
    Actor* thisx = &self->actor;

    FUN_003731e0(&self->skelAnime);
    Math_StepToF(&self->actor.speedXZ, 0.0f, 0.5f);
    *(s16*)((u8*)thisx + 0x11A) = 40;

    if (!(self->actor.flags & 0x8000)) {
        if (*(u32*)((u8*)thisx + 0x11C) & 0x400000) {
            if (self->unk_614 != 0) {
                self->actor.shape.rot.y += 0x1780;
            } else {
                Math_ScaledStepToS(&self->actor.shape.rot.x, 0x4000, 0x200);
                self->actor.shape.rot.z += 0x1780;
            }
        }
        if ((self->actor.bgCheckFlags & 1) || (self->actor.floorHeight == -32000.0f)) {
            EffectSsDeadDb_Spawn(play, &self->actor.world.pos, &En_Crow_sZeroVecAccel, &En_Crow_sZeroVecAccel,
                                 (s16)(self->actor.scale.x * 10000.0f), 0, 255, 255, 255, 255, 255, 0, 0, 1, 11, 1);
            *(s16*)((u8*)thisx + 0x11A) = 0;
            self->actionFunc = EnCrow_Die;
        }
    }
}

void EnCrow_Die(EnCrow* self, PlayState* play) {
    f32 step;

    if (self->actor.params != 0) {
        step = 0.006f;
    } else {
        step = 0.002f;
    }

    if (Math_StepToF(&self->actor.scale.x, 0.0f, step)) {
        if (self->actor.params == 0) {
            En_Crow_sDeathCount++;
            Item_DropCollectibleRandom(play, &self->actor, &self->actor.world.pos, 0);
        } else {
            Item_DropCollectible(play, &self->actor.world.pos, 2);
        }

        if (En_Crow_sDeathCount == 10) {
            self->actor.params = 1;
            En_Crow_sDeathCount = 0;
            {
                f32 r = En_Crow_sJntSphInit.elements->modelRadius * 3.0f;
                self->colliderElements->worldRadius = r;
            }
        } else {
            self->actor.params = 0;
            self->colliderElements->worldRadius = En_Crow_sJntSphInit.elements->modelRadius;
        }
        Animation_PlayLoop(&self->skelAnime, 0);
        Math_Vec3f_Copy(&self->actor.world.pos, &self->actor.home.pos);
        self->actor.shape.rot.x = 0;
        self->actor.shape.rot.z = 0;
        self->actor.shape.yOffset = 2000.0f;
        self->actor.targetArrowOffset = 500.0f;
        self->timer = 450;
        self->actionFunc = EnCrow_Respawn;
        self->actor.draw = NULL;
    }

    self->actor.scale.z = self->actor.scale.y = self->actor.scale.x;
}

// DiveAttack: far (62 words; whole fn is a player(r5)/self+0x500 base(r6) register swap, ~25 variants incl. dead
// aliases / greedy single-site search). Draft kept under #if 0.
#if 0
void EnCrow_DiveAttack(EnCrow* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 facingPlayer;

    FUN_003731e0(&self->skelAnime);
    if (self->timer != 0) {
        self->timer--;
    }

    facingPlayer = Actor_IsFacingPlayer(&self->actor, 0x2800);

    if (facingPlayer) {
        Vec3f pos;
        s16 target;

        pos.x = player->actor.world.pos.x;
        pos.y = player->actor.world.pos.y + 20.0f;
        pos.z = player->actor.world.pos.z;
        target = Actor_WorldPitchTowardPoint(&self->actor, &pos);
        if (target > 0x3000) {
            target = 0x3000;
        }
        FUN_00370084(&self->actor.shape.rot.x, target, 2, 0x400);
    } else {
        FUN_00370084(&self->actor.shape.rot.x, -0x1000, 2, 0x100);
    }

    if (facingPlayer || (self->actor.xzDistToPlayer > 80.0f)) {
        FUN_00370084(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 4, 0xC00);
    }

    if ((self->timer == 0) || (Player_GetMask(play) == 2) || (self->colliderAtFlags & AT_HIT) ||
        (self->actor.bgCheckFlags & 9) || (*(u32*)((u8*)player + 0x1710) & 0x800000) ||
        (self->actor.yDistToWater > -40.0f)) {
        if (self->colliderAtFlags & AT_HIT) {
            Audio_PlayActorSound2(&self->actor, 0x01000327);
            self->colliderAtFlags &= ~AT_HIT;
            if (self->colliderAtFlags & 4) {
                f32 frames;

                Animation_Change_00375c08(&self->skelAnime, 1, 1.0f, 0.0f, 0.0f, 0, 0.0f);
                frames = self->skelAnime.animLength;
                if (frames > 0.0f) {
                    self->timer = (s32)(0.5f + frames * 1.5f);
                } else {
                    self->timer = (s32)(frames * 1.5f - 0.5f);
                }
                self->actor.speedXZ = 0.0f;
                self->actor.shape.rot.x = 0;
                self->actor.shape.rot.z = 0;
                self->unk_614 = 1;
                Audio_PlayActorSound2(&self->actor, 0x01000464);
                self->actionFunc = (EnCrowActionFunc)EnCrow_SetupTurnAway;
                return;
            }
        }

        FUN_003662d4(self);
    }
}
#endif

GLOBAL_ASM("asm/EnCrow_DiveAttack.s")

void EnCrow_FlyIdle(EnCrow* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 skelanimeUpdated;
    s16 var;

    FUN_003731e0(&self->skelAnime);
    skelanimeUpdated = Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f);
    self->actor.speedXZ = (Rand_ZeroOne() * 1.5f) + 3.0f;

    if (self->actor.bgCheckFlags & 8) {
        self->aimRotY = self->actor.wallYaw;
    } else if (Actor_WorldDistXZToPoint(&self->actor, &self->actor.home.pos) > 300.0f) {
        self->aimRotY = Actor_WorldYawTowardPoint(&self->actor, &self->actor.home.pos);
    }

    if ((Math_SmoothStepToS(&self->actor.shape.rot.y, self->aimRotY, 5, 0x300, 0x10) == 0) && skelanimeUpdated &&
        (Rand_ZeroOne() < 0.1f)) {
        var = Actor_WorldYawTowardPoint(&self->actor, &self->actor.home.pos) - self->actor.shape.rot.y;
        if (var > 0) {
            self->aimRotY += 0x1000 + (0x1000 * Rand_ZeroOne());
        } else {
            self->aimRotY -= 0x1000 + (0x1000 * Rand_ZeroOne());
        }
        Audio_PlayActorSound2(&self->actor, 0x01000326);
    }

    if (self->actor.yDistToWater > -40.0f) {
        self->aimRotX = -0x1000;
    } else if (self->actor.world.pos.y < (self->actor.home.pos.y - 50.0f)) {
        self->aimRotX = -0x800 - (Rand_ZeroOne() * 0x800);
    } else if (self->actor.world.pos.y > (self->actor.home.pos.y + 50.0f)) {
        self->aimRotX = 0x800 + (Rand_ZeroOne() * 0x800);
    }

    if ((Math_SmoothStepToS(&self->actor.shape.rot.x, self->aimRotX, 10, 0x100, 8) == 0) && skelanimeUpdated &&
        (Rand_ZeroOne() < 0.1f)) {
        if (self->actor.world.pos.y > self->actor.home.pos.y) {
            self->aimRotX -= (0x400 * Rand_ZeroOne()) + 0x400;
        } else {
            self->aimRotX += (0x400 * Rand_ZeroOne()) + 0x400;
        }
        self->aimRotX = (self->aimRotX < -0x1000) ? -0x1000 : ((self->aimRotX > 0x1000) ? 0x1000 : self->aimRotX);
    }

    if (self->actor.bgCheckFlags & 1) {
        Math_ScaledStepToS(&self->actor.shape.rot.x, -0x100, 0x400);
    }

    if (self->timer != 0) {
        self->timer--;
    }
    if ((self->timer == 0) && (self->actor.xzDistToPlayer < 300.0f) &&
        !(*(u32*)((u8*)player + 0x1710) & 0x800000) && (self->actor.yDistToWater < -40.0f) &&
        (Player_GetMask(play) != 2)) {
        self->actor.speedXZ = 4.0f;
        self->timer = 450;
        SkelAnime_SetPlaySpeed(&self->skelAnime, 2.0f);
        self->actionFunc = EnCrow_DiveAttack;
    }
}

s32 EnCrow_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnCrow* self = (EnCrow*)thisx;

    if (self->actor.colChkInfo.health != 0) {
        f32 quarterPi = M_PI / 4;
        f32 rotScale = M_PI / 0x8000;

        if (limbIndex == 6) {
            s16 rot = (s16)(sinf(*EnCrow_CurFramePtr(&self->skelAnime) * quarterPi) * 3072.0f);
            Matrix_RotateY(mtx, rot * rotScale, 1);
        } else if (limbIndex == 7) {
            s16 rot = (s16)(sinf(2.5f + *EnCrow_CurFramePtr(&self->skelAnime) * quarterPi) * 5120.0f);
            Matrix_RotateY(mtx, rot * rotScale, 1);
        }
    }
    return 0;
}

// PostLimbDraw: draft matches 0 words (fastchk) but needs pin_static + cfg split (see STATUS.md), so it stays
// under #if 0 with the ROM asm. With it enabled, `En_Crow_sDeathCount` must be a real `static u32 ... = 0;`
// (guard 0x521B1C sits right after it in the same .data).
#if 0
static inline void EnCrow_MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}

void EnCrow_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnCrow* self = (EnCrow*)thisx;
    static Vector3f sHeadVec(2500.0f, 0.0f, 0.0f);
    f32 yOff = 20.0f;

    if (limbIndex == 1) {
        EnCrow_MultVec3f(&sHeadVec, (Vector3f*)&self->bodyPartsPos[0], mtx);
        self->bodyPartsPos[0].y -= yOff;
    } else if ((limbIndex == 5) || (limbIndex == 3) || (limbIndex == 7)) {
        Vec3f* vec = &self->bodyPartsPos[(limbIndex >> 1) - 1];
        EnCrow_MultVec3f((Vector3f*)&En_Crow_sZeroVecAccel, (Vector3f*)vec, mtx);
        vec->y -= yOff;
    }
}
#endif

GLOBAL_ASM("asm/EnCrow_PostLimbDraw.s")

void EnCrow_Respawn(EnCrow* self, PlayState* play) {
    f32 target;

    if (self->timer != 0) {
        self->timer--;
    }

    if (self->timer == 0) {
        FUN_003731e0(&self->skelAnime);
        self->actor.draw = (ActorFunc)EnCrow_Draw;
        if (self->actor.params != 0) {
            target = 0.03f;
        } else {
            target = 0.01f;
        }
        if (Math_StepToF(&self->actor.scale.x, target, target * 0.1f)) {
            self->actor.flags |= 1;
            self->actor.flags &= ~0x10;
            self->actor.colChkInfo.health = 1;
            self->timer = 150;
            SkelAnime_SetPlaySpeed(&self->skelAnime, 1.0f);
            self->colliderAcFlags |= AC_ON;
            self->actionFunc = EnCrow_FlyIdle;
        }
        self->actor.scale.z = self->actor.scale.y = self->actor.scale.x;
    }
}


void EnCrow_SetupTurnAway(EnCrow* self) {
    FUN_003731e0(&self->skelAnime);
    if (self->timer != 0) {
        self->timer--;
        if (self->timer != 0) {
            return;
        }
    }

    Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, 0.0f, 0, 0.0f);
    self->unk_614 = 0;
    self->actor.speedXZ = 3.5f;
    self->timer = 150;
    self->aimRotX = -0x1000;
    self->aimRotY = self->actor.yawTowardsPlayer + 0x8000;
    SkelAnime_SetPlaySpeed(&self->skelAnime, 2.0f);
    self->actionFunc = EnCrow_TurnAway;
}


void EnCrow_TurnAway(EnCrow* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);

    if (self->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
        self->aimRotY = self->actor.wallYaw;
    } else {
        self->aimRotY = self->actor.yawTowardsPlayer + 0x8000;
    }

    FUN_00370084(&self->actor.shape.rot.y, self->aimRotY, 3, 0xC00);
    FUN_00370084(&self->actor.shape.rot.x, self->aimRotX, 5, 0x100);

    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer == 0) {
        FUN_003662d4(self);
    }
}

}
