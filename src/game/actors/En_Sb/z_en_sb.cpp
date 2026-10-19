extern "C" {
#include "z_en_sb.hpp"

#define FLAGS 0x00000005

void EnSb_Init(EnSb* self, PlayState* play);
void EnSb_Destroy(EnSb* self, PlayState* play);
void EnSb_Update(Actor* actor, GameState* state);
void EnSb_Draw(EnSb* self, PlayState* play);

void EnSb_WaitClosed(EnSb* self, PlayState* play);
void EnSb_Open(EnSb* self, PlayState* play);
void EnSb_WaitOpen(EnSb* self, PlayState* play);
void EnSb_TurnAround(EnSb* self, PlayState* play);
void EnSb_Lunge(EnSb* self, PlayState* play);
void EnSb_Bounce(EnSb* self, PlayState* play);
void EnSb_Cooldown(EnSb* self, PlayState* play);
void FUN_001895a4(EnSb* self);
f32* EnSb_CurFramePtr(SkelAnime* skelAnime);
void EnSb_SetupWaitClosed(EnSb* self);
void EnSb_PostLimbDraw(PlayState* play, s32 limbIndex, void* dList, void* thisx);

void FUN_003508b8(Actor* actor, ...);
void FUN_003504d0(Actor* actor, void* bodyBreak);
void FUN_0034f558(SkelAnime* skelAnime, PlayState* play);
void FUN_0034f184(void* bodyBreak, s32 limbIndex, s32 minLimb, s32 maxLimb, s32 count, MTX34* mtx);
void FUN_0034fe20(Actor* actor, PlayState* play, SkelAnime* skelAnime, s32 cmbIndex, s32 csabIndex, void* jointTable,
                  void* morphTable, s32 limbCount);
void Collider_SetCylinderType1(PlayState* play, ColliderCylinder* collider, Actor* actor, ColliderCylinderInit* init);
void EffectSsEnFire_SpawnVec3f(PlayState* play, Actor* actor, Vec3f* pos, s16 scale, s16 params, s16 flags, s16 bodyPart,
                               s16 extra);
void EffectSsBubble_Spawn(PlayState* play, Vec3f* pos, f32 a, f32 b, f32 c, f32 d);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Audio_PlayActorSound2(Actor* actor, s32 arg1);
extern InitChainEntry EnSb_sInitChain[];
extern DamageTable EnSb_sDamageTable[];
extern ColliderCylinderInit EnSb_sCylinderInit;
extern Vec3f EnSb_sFlamePosOffsets[];

GLOBAL_ASM("data/z_En_Sb.data.s")
GLOBAL_ASM("../src/game/actors/En_Sb/data_syms.s")

ActorInit En_Sb_InitVars = {
    ACTOR_EN_SB,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_SB,
    sizeof(EnSb),
    (ActorFunc)EnSb_Init,
    (ActorFunc)EnSb_Destroy,
    (ActorFunc)EnSb_Update,
    (ActorFunc)EnSb_Draw,
};

void EnSb_Init(EnSb* self, PlayState* play) {
    Actor_ProcessInitChain(&self->actor, EnSb_sInitChain);
    self->actor.colChkInfo.damageTable = EnSb_sDamageTable;
    self->actor.colChkInfo.health = 2;
    FUN_00372f38(&self->actor, play, 0);
    FUN_0034fe20(&self->actor, play, &self->skelAnime, 0, 1, NULL, NULL, 0);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinderType1(play, &self->collider, &self->actor, &EnSb_sCylinderInit);
    self->isDead = 0;
    self->actor.colChkInfo.mass = 0;
    Actor_SetScale(&self->actor, 0.006f);
    self->actor.shape.rot.y = 0;
    self->actor.speedXZ = 0.0f;
    self->actor.gravity = -0.35f;
    self->actor.velocity.y = -1.0f;
    self->fire = 0;
    self->hitByWindArrow = false;
    EnSb_SetupWaitClosed(self);
}

void EnSb_Destroy(EnSb* self, PlayState* play) {
    FUN_003508b8(&self->actor, 0);
    FUN_003504d0(&self->actor, self->bodyBreak);
    FUN_0034f558(&self->skelAnime, play);
    Collider_DestroyCylinder(play, &self->collider);
}

#if 0 // draft: ~57 insn-lines off (fp hoists 19.0f instead of 3.0f), see STATUS.md
u8 Actor_ApplyDamage(Actor* actor);
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);
s32 BodyBreak_SpawnParts(Actor* actor, void* bodyBreak, PlayState* play, s32 params, s32 zero);
void FUN_00375e18(void* bodyBreak, s32 count, PlayState* play);
void Item_DropCollectibleRandom(PlayState* play, Actor* actor, Vec3f* pos, s32 params);
void FUN_00375b70(PlayState* play, Actor* actor);
void FUN_00375f90(PlayState* play, Vec3f* pos, s32 a);
void FUN_003741e4(PlayState* play, u32 dmgFlags, s32 idx, Vec3f* pos, s32 zero);

static inline void EnSb_SetupCooldown(EnSb* self, s32 changeSpeed) {
    if (self->behavior != 1) {
        Animation_Change_00375c08(&self->skelAnime, 2, 1.0f, 0.0f, (f32)Animation_GetLastFrame(&self->skelAnime, 2), 2, -0.0f);
    }
    self->behavior = 1;
    if (changeSpeed) {
        if (self->actor.yDistToWater > 0.0f) {
            self->actor.speedXZ = -5.0f;
            if (self->actor.velocity.y < 0.0f) {
                self->actor.velocity.y = 2.1f;
            }
        } else {
            self->actor.speedXZ = -6.0f;
            if (self->actor.velocity.y < 0.0f) {
                self->actor.velocity.y = 1.4f;
            }
        }
    }
    self->timer = 90;
    self->actionFunc = EnSb_Cooldown;
}

static inline s32 EnSb_IsVulnerable(EnSb* self) {
    switch (self->behavior) {
        case 0:
            if ((*EnSb_CurFramePtr(&self->skelAnime) >= 2.0f) && (*EnSb_CurFramePtr(&self->skelAnime) <= 5.0f)) {
                return 1;
            }
            break;
        case 1:
            if ((*EnSb_CurFramePtr(&self->skelAnime) >= 0.0f) && (*EnSb_CurFramePtr(&self->skelAnime) <= 1.0f)) {
                return 1;
            }
            break;
        case 2:
            if ((*EnSb_CurFramePtr(&self->skelAnime) >= 0.0f) && (*EnSb_CurFramePtr(&self->skelAnime) <= 19.0f)) {
                return 1;
            }
            break;
        case 3:
            if (*EnSb_CurFramePtr(&self->skelAnime) == 0.0f) {
                return 1;
            }
            break;
        case 4:
            if ((*EnSb_CurFramePtr(&self->skelAnime) >= 3.0f) && (*EnSb_CurFramePtr(&self->skelAnime) <= 5.0f)) {
                return 1;
            }
            break;
    }
    return 0;
}

static inline s32 EnSb_UpdateDamage(EnSb* self, PlayState* play) {
    Vec3f hitPoint;
    f32 hitY;
    s16 yawDiff;
    s32 yawD;
    s32 tookDamage;
    u8 hitByWindArrow;
    ColliderInfo* acHitInfo;

    if (self->collider.base.atFlags & 2) {
        EnSb_SetupCooldown(self, 1);
        return 1;
    }

    if (self->collider.base.acFlags & 2) {
        tookDamage = 0;
        hitByWindArrow = 0;
        self->collider.base.acFlags &= ~2;

        switch (self->actor.colChkInfo.damageEffect) {
            case 14:
                hitByWindArrow = 1;
            case 15:
                if (EnSb_IsVulnerable(self)) {
                    hitY = self->collider.info.bumper.hitPos.y - self->actor.world.pos.y;
                    yawD = self->actor.yawTowardsPlayer - self->actor.shape.rot.y; yawDiff = ABS(yawD);
                    if ((hitY < 30.0f) && (hitY > 10.0f) && (yawDiff < 0x471E)) {
                        Actor_ApplyDamage(&self->actor);
                        Actor_SetColorFilter(&self->actor, 0x400000, 255, 0x200000, 80);
                        tookDamage = 1;
                    }
                }
                break;
            case 2:
                self->fire = 4;
                Actor_ApplyDamage(&self->actor);
                Actor_SetColorFilter(&self->actor, 0x400000, 255, 0x200000, 80);
                tookDamage = 1;
                break;
            case 1:
            case 13:
                if (EnSb_IsVulnerable(self)) {
                    hitY = self->collider.info.bumper.hitPos.y - self->actor.world.pos.y;
                    yawD = self->actor.yawTowardsPlayer - self->actor.shape.rot.y; yawDiff = ABS(yawD);
                    if ((hitY < 30.0f) && (hitY > 10.0f) && (yawDiff < 0x471E)) {
                        Actor_ApplyDamage(&self->actor);
                        Actor_SetColorFilter(&self->actor, 0x400000, 255, 0x200000, 80);
                        tookDamage = 1;
                        EnSb_SetupCooldown(self, 0);
                    }
                }
                break;
            default:
                break;
        }
        hitPoint.x = self->collider.info.bumper.hitPos.x;
        hitPoint.y = self->collider.info.bumper.hitPos.y;
        hitPoint.z = self->collider.info.bumper.hitPos.z;
        acHitInfo = self->collider.info.acHitInfo;
        if (self->actor.colChkInfo.health == 0) {
            self->hitByWindArrow = hitByWindArrow;
            FUN_00375e18(self->bodyBreak, 3, play);
            self->isDead = 1;
            FUN_00375b70(play, &self->actor);
            Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 0x28, 0x010002BA);
            FUN_003741e4(play, acHitInfo->toucher.dmgFlags, 0, &hitPoint, 0);
            return 1;
        }
        if (!tookDamage) {
            FUN_00375f90(play, &hitPoint, 8);
            FUN_003741e4(play, acHitInfo->toucher.dmgFlags, 1, &hitPoint, 0);
        }
    }
    return 0;
}

void EnSb_Update(Actor* thisx, GameState* state) {
    EnSb* self = (EnSb*)thisx;
    PlayState* play = (PlayState*)state;

    if (self->isDead) {
        if (self->actor.yDistToWater > 0.0f) {
            thisx->params = 4;
        } else {
            thisx->params = 1;
        }
        if (BodyBreak_SpawnParts(&self->actor, self->bodyBreak, play, self->actor.params, 0)) {
            if (!self->hitByWindArrow) {
                Item_DropCollectibleRandom(play, &self->actor, &self->actor.world.pos, 0x80);
            } else {
                Item_DropCollectible(play, &self->actor.world.pos, 8);
            }
            Actor_Kill(&self->actor);
        }
    } else {
        Actor_SetFocus(&self->actor, 20.0f);
        Actor_SetScale(&self->actor, 0.006f);
        Actor_MoveForward(&self->actor);
        self->actionFunc(self, play);
        Actor_UpdateBgCheckInfo(play, &self->actor, 20.0f, 20.0f, 20.0f, 5);
        EnSb_UpdateDamage(self, play);
        Collider_UpdateCylinder(&self->actor, &self->collider);
        CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
        FUN_003731e0(&self->skelAnime);
    }
}
#endif
GLOBAL_ASM("asm/EnSb_Update.s")

void EnSb_Draw(EnSb* self, PlayState* play) {
    Vec3f flamePos;
    s16 fireDecr;
    s32 idx;

    SkelAnime_DrawOpa(&self->skelAnime, (MTX34*)&self->actor.modelMtx, NULL, (PostLimbDrawOpa)EnSb_PostLimbDraw,
                      &self->actor, 0);
    if (self->fire != 0) {
        (*(u16*)((u8*)self + 0x11A))++;
        fireDecr = self->fire - 1;
        if ((fireDecr & 1) == 0) {
            idx = fireDecr & 3;
            flamePos.x = Rand_CenteredFloat(5.0f) + (self->actor.world.pos.x + EnSb_sFlamePosOffsets[idx].x);
            flamePos.y = Rand_CenteredFloat(5.0f) + (self->actor.world.pos.y + EnSb_sFlamePosOffsets[idx].y);
            flamePos.z = Rand_CenteredFloat(5.0f) + (self->actor.world.pos.z + EnSb_sFlamePosOffsets[idx].z);
            EffectSsEnFire_SpawnVec3f(play, &self->actor, &flamePos, 100, 0, 0, -1, 1);
        }
    }
}

void EnSb_PostLimbDraw(PlayState* play, s32 limbIndex, void* dList, void* thisx) {
    EnSb* self = (EnSb*)thisx;

    FUN_0034f184(self->bodyBreak, limbIndex, 2, 4, 5, (MTX34*)dList);
}

void EnSb_SetupWaitClosed(EnSb* self) {
    Animation_Change_00375c08(&self->skelAnime, 2, 1.0f, 0.0f, (f32)Animation_GetLastFrame(&self->skelAnime, 2), 2, -0.0f);
    self->behavior = 1;
    self->actionFunc = EnSb_WaitClosed;
}

static inline void EnSb_SetupOpen(EnSb* self) {
    Animation_Change_00375c08(&self->skelAnime, 1, 1.0f, 0.0f, (f32)Animation_GetLastFrame(&self->skelAnime, 1), 2, -0.0f);
    self->behavior = 0;
    self->actionFunc = EnSb_Open;
    Audio_PlayActorSound2(&self->actor, 0x010002B9);
}

static inline void EnSb_SetupWaitOpen(EnSb* self) {
    Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, (f32)Animation_GetLastFrame(&self->skelAnime, 0), 0, -0.0f);
    self->behavior = 2;
    self->actionFunc = EnSb_WaitOpen;
}

static inline void EnSb_SetupBounce(EnSb* self) {
    Animation_Change_00375c08(&self->skelAnime, 4, 1.0f, 0.0f, (f32)Animation_GetLastFrame(&self->skelAnime, 4), 2, -0.0f);
    self->behavior = 4;
    self->actionFunc = EnSb_Bounce;
}

static inline void EnSb_SpawnBubbles(PlayState* play, EnSb* self) {
    s32 i;

    if (self->actor.yDistToWater > 0.0f) {
        for (i = 0; i < 10; i++) {
            EffectSsBubble_Spawn(play, &self->actor.world.pos, 10.0f, 10.0f, 30.0f, 0.25f);
        }
    }
}

void EnSb_WaitClosed(EnSb* self, PlayState* play) {
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 0xA, 0x7D0, 0x0);

    if ((self->actor.xzDistToPlayer <= 160.0f) && (self->actor.xzDistToPlayer > 40.0f)) {
        EnSb_SetupOpen(self);
    }
}

void EnSb_Open(EnSb* self, PlayState* play) {
    f32 currentFrame = *EnSb_CurFramePtr(&self->skelAnime);
    f32 lastFrame;

    lastFrame = (f32)Animation_GetLastFrame(&self->skelAnime, 1);
    if (currentFrame >= lastFrame) {
        self->timer = 23;
        EnSb_SetupWaitOpen(self);
    } else {
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 0xA, 0x7D0, 0x0);
        if ((self->actor.xzDistToPlayer > 160.0f) || (self->actor.xzDistToPlayer <= 40.0f)) {
            EnSb_SetupWaitClosed(self);
        }
    }
}

void EnSb_WaitOpen(EnSb* self, PlayState* play) {
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 0xA, 0x7D0, 0x0);

    if ((self->actor.xzDistToPlayer > 160.0f) || (self->actor.xzDistToPlayer <= 40.0f)) {
        EnSb_SetupWaitClosed(self);
    }

    if (self->timer != 0) {
        self->timer--;
    } else {
        self->attackYaw = self->actor.yawTowardsPlayer;
        self->actionFunc = EnSb_TurnAround;
    }
}

void EnSb_TurnAround(EnSb* self, PlayState* play) {
    s16 invertedYaw;

    invertedYaw = self->attackYaw + 0x8000;
    Math_SmoothStepToS(&self->actor.shape.rot.y, invertedYaw, 0x1, 0x1F40, 0xA);

    if (self->actor.shape.rot.y == invertedYaw) {
        self->actor.world.rot.y = self->attackYaw;
        if (self->actor.yDistToWater > 0.0f) {
            self->actor.velocity.y = 3.0f;
            self->actor.speedXZ = 5.0f;
            self->actor.gravity = -0.35f;
        } else {
            self->actor.velocity.y = 2.0f;
            self->actor.speedXZ = 6.0f;
            self->actor.gravity = -2.0f;
        }
        EnSb_SpawnBubbles(play, self);
        self->bouncesLeft = 3;
        FUN_001895a4(self);
    }
}

void EnSb_Lunge(EnSb* self, PlayState* play) {
    Math_StepToF(&self->actor.speedXZ, 0.0f, 0.2f);
    if ((self->actor.velocity.y <= -0.1f) || (self->actor.bgCheckFlags & 2)) {
        if (!(self->actor.yDistToWater > 0.0f)) {
            Audio_PlayActorSound2(&self->actor, 0x010002EB);
        }
        self->actor.bgCheckFlags &= ~2;
        EnSb_SetupBounce(self);
    }
}

void EnSb_Bounce(EnSb* self, PlayState* play) {
    f32 currentFrame = *EnSb_CurFramePtr(&self->skelAnime);
    s32 lastFrameInt = Animation_GetLastFrame(&self->skelAnime, 4);
    f32 frameCount = (f32)lastFrameInt;

    Math_StepToF(&self->actor.speedXZ, 0.0f, 0.2f);

    if (currentFrame == frameCount) {
        if (self->bouncesLeft != 0) {
            self->bouncesLeft--;
            self->timer = 1;
            if (self->actor.yDistToWater > 0.0f) {
                self->actor.velocity.y = 3.0f;
                self->actor.speedXZ = 5.0f;
                self->actor.gravity = -0.35f;
            } else {
                self->actor.velocity.y = 2.0f;
                self->actor.speedXZ = 6.0f;
                self->actor.gravity = -2.0f;
            }
            EnSb_SpawnBubbles(play, self);
            FUN_001895a4(self);
        } else if (self->actor.bgCheckFlags & 1) {
            self->actor.bgCheckFlags &= ~2;
            self->actor.speedXZ = 0.0f;
            self->timer = 1;
            EnSb_SetupWaitClosed(self);
        }
    }
}

void EnSb_Cooldown(EnSb* self, PlayState* play) {
    if (self->timer != 0) {
        self->timer--;
        if (self->actor.bgCheckFlags & 1) {
            self->actor.bgCheckFlags &= ~1;
            self->actor.speedXZ = 0.0f;
        }
    } else {
        if (self->actor.bgCheckFlags & 1) {
            self->actor.bgCheckFlags &= ~1;
            self->actor.speedXZ = 0.0f;
            self->actionFunc = EnSb_WaitClosed;
        }
    }
}

void FUN_001895a4(EnSb* self) {
    f32 frameCount = (f32)Animation_GetLastFrame(&self->skelAnime, 3);
    f32 playbackSpeed = self->actor.yDistToWater > 0.0f ? 1.0f : 0.0f;

    Animation_Change_00375c08(&self->skelAnime, 3, playbackSpeed, 0.0f, frameCount, 2, -0.0f);
    self->behavior = 3;
    self->actionFunc = EnSb_Lunge;
    Audio_PlayActorSound2(&self->actor, 0x010002B9);
}

}
