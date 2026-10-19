extern "C" {
#include "z_en_geldb.hpp"

#define BGCHECKFLAG_GROUND 0x0001
#define BGCHECKFLAG_GROUND_TOUCH 0x0002
#define BGCHECKFLAG_WALL 0x0008

s32 FUN_00369608(PlayState* play, Actor* actor);
Actor* FUN_00346e2c(PlayState* play, Actor* actor, f32 range, f32 range2);
s16 Actor_WorldYawTowardActor(Actor* actor, Actor* other);
f32 Actor_WorldDistXYZToPoint(Actor* actor, Vec3f* point);
void EnGeldB_SetupSideStep(EnGeldB* self, PlayState* play);
void EnGeldB_SetupJump(EnGeldB* self);
void EnGeldB_SetupBlock(EnGeldB* self);
void EnGeldB_SetupRollBack(EnGeldB* self);
void EnGeldB_Sidestep(EnGeldB* self, PlayState* play);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void EnGeldB_SetupReady(EnGeldB* self);
void EnGeldB_SetupAdvance(EnGeldB* self, PlayState* play);
void EnGeldB_SpinAttack(EnGeldB* self, PlayState* play);
s32 EnGeldB_ReactToPlayer(PlayState* play, EnGeldB* self, s16 arg2);
void Animation_PlayOnce(SkelAnime* skelAnime, s32 idx);
void FUN_003ff758(Vec3f* pos, s32 sfxId);
void EnGeldB_Slash(EnGeldB* self, PlayState* play);
s32 FUN_0035e600(Actor* actor, PlayState* play, f32 speed, s16 yaw);
void EnGeldB_SetupRollBack(EnGeldB* self);
void EnGeldB_Circle(EnGeldB* self, PlayState* play);
void EnGeldB_SetupCircle(EnGeldB* self);

// Collider_DestroyQuad: 8-byte `return 1` stub (moved here from functions_340000s.cpp). It must be C in a file other than its
// caller so armlink inlines it into EnGeldB_Destroy (dead `mov r0,#1`).
s32 FUN_0034eaf8(PlayState* play, void* quad) {
    return 1;
}

s32 EnGeldB_DodgeRanged(PlayState* play, EnGeldB* self) {
    Actor* actor = FUN_00346e2c(play, &self->actor, 800.0f, 1.0f);

    if (actor != NULL) {
        s16 angleToFacing;
        f32 dist;

        angleToFacing = Actor_WorldYawTowardActor(&self->actor, actor) - self->actor.shape.rot.y;
        self->actor.world.rot.y = (u16)self->actor.shape.rot.y & 0xFFFF;
        dist = Actor_WorldDistXYZToPoint(&self->actor, &actor->world.pos);
        if ((ABS(angleToFacing) < 0x2EE0) && (__sqrtf(dist) < 600.0f)) {
            if (actor->id == 0x66) {
                EnGeldB_SetupJump(self);
            } else {
                EnGeldB_SetupBlock(self);
            }
        } else {
            self->actor.world.rot.y = self->actor.shape.rot.y + 0x3FFF;
            if ((ABS(angleToFacing) < 0x2000) || (ABS(angleToFacing) > 0x5FFF)) {
                EnGeldB_SetupSideStep(self, play);
                self->actor.speedXZ *= 3.0f;
            } else if (ABS(angleToFacing) < 0x5FFF) {
                EnGeldB_SetupRollBack(self);
            }
        }
        return true;
    }
    return false;
}

void EnGeldB_SetupCircle(EnGeldB* self) {
    f32 lastFrame = Animation_GetLastFrame(&self->skelAnime, 7);

    Animation_Change_00375c08(&self->skelAnime, 7, 1.0f, 0.0f, lastFrame, 1, 0.0f);
    self->actor.speedXZ = Rand_CenteredFloat(12.0f);
    self->skelAnime.playSpeed = self->actor.speedXZ * -0.5f;
    s16 yaw = self->actor.shape.rot.y;
    self->actor.world.rot.y = yaw;
    // Timer rounding with compile-time literals (R_UPDATE_RATE folded): the Rand argument is evaluated once per use.
    s32 timer;
    if (((s32)(30.0f + Rand_ZeroOne() * 30.0f)) > 0) {
        timer = (s32)(0.5f + ((s32)(30.0f + Rand_ZeroOne() * 30.0f) * 3.0f) * 0.5f);
    } else {
        timer = (s32)(((s32)(30.0f + Rand_ZeroOne() * 30.0f) * 3.0f) * 0.5f - 0.5f);
    }
    self->timer = timer;
    self->approachRate = 0.0f;
    self->action = 10;
    self->actionFunc = EnGeldB_Circle;
}

// Lives here (not in z_en_geldb.cpp) so armcc does not inline it into EnGeldB_RollBack.
void EnGeldB_SetupSideStep(EnGeldB* self, PlayState* play) {
    s16 playerRotY;
    Player* player;
    Actor* thisx = &self->actor;
    f32 lastFrame = Animation_GetLastFrame(&self->skelAnime, 7);

    Animation_Change_00375c08(&self->skelAnime, 7, 1.0f, 0.0f, lastFrame, 1, 0.0f);
    player = GET_PLAYER(play);
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0xFA0, 1);
    playerRotY = player->actor.shape.rot.y;
    if (Math_SinS(playerRotY - self->actor.shape.rot.y) > 0.0f) {
        self->actor.speedXZ = -6.0f;
    } else {
        f32 v;
        if (Math_SinS(playerRotY - self->actor.shape.rot.y) < 0.0f) {
            v = 6.0f;
        } else {
            v = Rand_CenteredFloat(12.0f);
        }
        self->actor.speedXZ = v;
    }
    // Reading through thisx (not self->actor) keeps the ROM's reload of speedXZ after the join.
    self->skelAnime.playSpeed = thisx->speedXZ * -0.5f;
    self->actor.world.rot.y = self->actor.shape.rot.y + 0x3FFF;
    self->approachRate = 0.0f;
    self->timer = ((s32)(5.0f + Rand_ZeroOne() * 10.0f)) > 0
                      ? (s32)(0.5f + ((s32)(5.0f + Rand_ZeroOne() * 10.0f) * 3.0f) * 0.5f)
                      : (s32)(((s32)(5.0f + Rand_ZeroOne() * 10.0f) * 3.0f) * 0.5f - 0.5f);
    self->action = 13;
    self->actionFunc = EnGeldB_Sidestep;
}

void EnGeldB_RollForward(EnGeldB* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 facingAngleToLink = player->actor.shape.rot.y - self->actor.shape.rot.y;

    if (FUN_003731e0(&self->skelAnime)) {
        self->actor.speedXZ = 0.0f;
        self->invisible = false;
        if (!Actor_IsFacingPlayer(&self->actor, 0x1554)) {
            EnGeldB_SetupReady(self);
            self->timer = ((s32)(5.0f + Rand_ZeroOne() * 5.0f)) > 0
                              ? (s32)(0.5f + ((s32)(5.0f + Rand_ZeroOne() * 5.0f) * 3.0f) * 0.5f)
                              : (s32)(((s32)(5.0f + Rand_ZeroOne() * 5.0f) * 3.0f) * 0.5f - 0.5f);
            if (ABS(facingAngleToLink) < 0x38E0) {
                self->lookTimer = 30;
            }
        } else if (!FUN_00369608(play, &self->actor) && (Rand_ZeroOne() > 0.5f || ABS(facingAngleToLink) < 0x3FFC)) {
            Animation_PlayOnce(&self->skelAnime, 0);
            *(u8*)&self->swordCollider[0x10] &= ~4;
            self->action = 7;
            self->actor.speedXZ = 0.0f;
            self->spinAttackState = 0;
            FUN_003ff758(&self->actor.world.pos, 0x01000436);
            self->actionFunc = EnGeldB_Slash;
        } else {
            EnGeldB_SetupAdvance(self, play);
        }
    }
    if ((*(u32*)((u8*)play + 0x5BF4) & 0x5F) == 0) {
        Audio_PlayActorSound2(&self->actor, 0x01000436);
    }
}

void EnGeldB_SpinDodge(EnGeldB* self, PlayState* play) {
    Actor* thisx = &self->actor;  // reading xzDistToPlayer through thisx fixes the ROM float/int register order
    s16 phi_v1;
    s32 prevFrame;
    s32 beforeCurFrame;
    s32 afterPrevFrame;

    self->actor.world.rot.y = self->actor.yawTowardsPlayer + 0x3A98;
    if ((self->actor.bgCheckFlags & BGCHECKFLAG_WALL) ||
        !FUN_0035e600(&self->actor, play, self->actor.speedXZ, self->actor.shape.rot.y + 0x3E80)) {
        if (self->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
            if (self->actor.speedXZ >= 0.0f) {
                phi_v1 = self->actor.shape.rot.y + 0x3E80;
            } else {
                phi_v1 = self->actor.shape.rot.y - 0x3E80;
            }
            phi_v1 = self->actor.wallYaw - phi_v1;
        } else {
            self->actor.speedXZ *= -0.8f;
            phi_v1 = 0;
        }
        if (ABS(phi_v1) > 0x4000) {
            EnGeldB_SetupJump(self);
            return;
        }
    }
    if (thisx->xzDistToPlayer <= 45.0f) {
        Math_SmoothStepToF(&self->approachRate, -4.0f, 1.0f, 1.5f, 0.0f);
    } else if (self->actor.xzDistToPlayer > 40.0f) {
        Math_SmoothStepToF(&self->approachRate, 4.0f, 1.0f, 1.5f, 0.0f);
    } else {
        Math_SmoothStepToF(&self->approachRate, 0.0f, 1.0f, 6.65f, 0.0f);
    }
    if (self->approachRate != 0.0f) {
        self->actor.world.pos.x += Math_SinS(self->actor.yawTowardsPlayer) * self->approachRate;
        self->actor.world.pos.z += Math_CosS(self->actor.yawTowardsPlayer) * self->approachRate;
    }
    SkelAnime* sa = &self->skelAnime;
    if (ABS(self->approachRate) < ABS(self->actor.speedXZ)) {
        sa->playSpeed = self->actor.speedXZ * -0.5f;
    } else {
        sa->playSpeed = self->approachRate * -0.5f;
    }
    self->skelAnime.playSpeed = CLAMP(self->skelAnime.playSpeed, -3.0f, 3.0f);

    prevFrame = (s32)self->skelAnime.curFrame;
    FUN_003731e0(&self->skelAnime);
    beforeCurFrame = (s32)(self->skelAnime.curFrame - ABS(self->skelAnime.playSpeed));
    afterPrevFrame = (s32)ABS(self->skelAnime.playSpeed) + prevFrame;

    if ((prevFrame != (s32)self->skelAnime.curFrame) &&
        (((beforeCurFrame < 0) && (afterPrevFrame >= 1)) || ((beforeCurFrame < 5) && (afterPrevFrame >= 6)))) {
        Audio_PlayActorSound2(&self->actor, 0x01000410);
    }

    if ((*(u32*)((u8*)play + 0x5BF4) & 0x5F) == 0) {
        Audio_PlayActorSound2(&self->actor, 0x01000436);
    }
    self->timer--;
    if (self->timer == 0) {
        self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
        if (!EnGeldB_DodgeRanged(play, self)) {
            if (!FUN_00369608(play, &self->actor) && (self->actor.xzDistToPlayer <= 70.0f)) {
                Animation_PlayOnce(&self->skelAnime, 0);
                *(u8*)&self->swordCollider[0x10] &= ~4;
                self->action = 7;
                self->actor.speedXZ = 0.0f;
                self->spinAttackState = 0;
                FUN_003ff758(&self->actor.world.pos, 0x01000436);
                self->actionFunc = EnGeldB_Slash;
            } else {
                EnGeldB_SetupRollBack(self);
            }
        }
    } else {
        if (self->actor.speedXZ >= 0.0f) {
            self->actor.shape.rot.y += 0x4000;
        } else {
            self->actor.shape.rot.y -= 0x4000;
        }
    }
}

void EnGeldB_Sidestep(EnGeldB* self, PlayState* play) {
    s16 behindLinkAngle;
    s16 phi_v1;
    Player* player = GET_PLAYER(play);
    s32 prevFrame;
    s32 beforeCurFrame;
    s32 afterPrevFrame;

    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0xBB8, 1);
    behindLinkAngle = player->actor.shape.rot.y + 0x8000;
    if (Math_SinS(behindLinkAngle - self->actor.shape.rot.y) > 0.0f) {
        self->actor.speedXZ += 0.125f;
    } else if (Math_SinS(behindLinkAngle - self->actor.shape.rot.y) <= 0.0f) {
        self->actor.speedXZ -= 0.125f;
    }

    if ((self->actor.bgCheckFlags & BGCHECKFLAG_WALL) ||
        !FUN_0035e600(&self->actor, play, self->actor.speedXZ, self->actor.shape.rot.y + 0x3E80)) {
        if (self->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
            if (self->actor.speedXZ >= 0.0f) {
                phi_v1 = self->actor.shape.rot.y + 0x3E80;
            } else {
                phi_v1 = self->actor.shape.rot.y - 0x3E80;
            }
            phi_v1 = self->actor.wallYaw - phi_v1;
        } else {
            self->actor.speedXZ *= -0.8f;
            phi_v1 = 0;
        }
        if (ABS(phi_v1) > 0x4000) {
            self->actor.speedXZ *= -0.8f;
            if (self->actor.speedXZ < 0.0f) {
                self->actor.speedXZ -= 0.5f;
            } else {
                self->actor.speedXZ += 0.5f;
            }
        }
    }
    if (self->actor.speedXZ >= 0.0f) {
        self->actor.world.rot.y = self->actor.shape.rot.y + 0x3E80;
    } else {
        self->actor.world.rot.y = self->actor.shape.rot.y - 0x3E80;
    }
    if (self->actor.xzDistToPlayer <= 45.0f) {
        Math_SmoothStepToF(&self->approachRate, -4.0f, 1.0f, 1.5f, 0.0f);
    } else if (self->actor.xzDistToPlayer > 40.0f) {
        Math_SmoothStepToF(&self->approachRate, 4.0f, 1.0f, 1.5f, 0.0f);
    } else {
        Math_SmoothStepToF(&self->approachRate, 0.0f, 1.0f, 6.65f, 0.0f);
    }
    if (self->approachRate != 0.0f) {
        self->actor.world.pos.x += Math_SinS(self->actor.shape.rot.y) * self->approachRate;
        self->actor.world.pos.z += Math_CosS(self->actor.shape.rot.y) * self->approachRate;
    }
    SkelAnime* sa = &self->skelAnime;  // storing playSpeed through a separate pointer keeps the clamp's reload (ROM)
    if (ABS(self->approachRate) < ABS(self->actor.speedXZ)) {
        sa->playSpeed = self->actor.speedXZ * -0.5f;
    } else {
        sa->playSpeed = self->approachRate * -0.5f;
    }
    self->skelAnime.playSpeed = CLAMP(self->skelAnime.playSpeed, -3.0f, 3.0f);

    prevFrame = (s32)self->skelAnime.curFrame;
    FUN_003731e0(&self->skelAnime);
    beforeCurFrame = (s32)(self->skelAnime.curFrame - ABS(self->skelAnime.playSpeed));
    afterPrevFrame = (s32)ABS(self->skelAnime.playSpeed) + prevFrame;

    if (!EnGeldB_DodgeRanged(play, self) && !EnGeldB_ReactToPlayer(play, self, 0)) {
        if (--self->timer == 0) {
            s16 angleFacingPlayer = player->actor.shape.rot.y - self->actor.shape.rot.y;

            angleFacingPlayer = ABS(angleFacingPlayer);
            if (angleFacingPlayer >= 0x3A98) {
                EnGeldB_SetupReady(self);
                self->timer = ((s32)(1.0f + Rand_ZeroOne() * 5.0f)) > 0
                                  ? (s32)(0.5f + ((s32)(1.0f + Rand_ZeroOne() * 5.0f) * 3.0f) * 0.5f)
                                  : (s32)(((s32)(1.0f + Rand_ZeroOne() * 5.0f) * 3.0f) * 0.5f - 0.5f);
            } else {
                Player* player2 = GET_PLAYER(play);
                s16 angleFacingPlayer2 = player2->actor.shape.rot.y - self->actor.shape.rot.y;

                self->actor.world.rot.y = self->actor.shape.rot.y;
                if ((self->actor.xzDistToPlayer <= 45.0f) && !FUN_00369608(play, &self->actor) &&
                    (!(*(u32*)((u8*)play + 0x5BF4) & 3) || (ABS(angleFacingPlayer2) < 0x38E0))) {
                    Animation_PlayOnce(&self->skelAnime, 0);
                    *(u8*)&self->swordCollider[0x10] &= ~4;
                    self->action = 7;
                    self->actor.speedXZ = 0.0f;
                    self->spinAttackState = 0;
                    FUN_003ff758(&self->actor.world.pos, 0x01000436);
                    self->actionFunc = EnGeldB_Slash;
                } else if ((210.0f > self->actor.xzDistToPlayer) && (self->actor.xzDistToPlayer > 150.0f) &&
                           !(*(u32*)((u8*)play + 0x5BF4) & 1)) {
                    if (FUN_00369608(play, &self->actor) || (Rand_ZeroOne() > 0.5f) || (ABS(angleFacingPlayer2) < 0x38E0)) {
                        f32 lastFrame = Animation_GetLastFrame(&self->skelAnime, 2);

                        Animation_Change_00375c08(&self->skelAnime, 2, -1.0f, lastFrame, 0.0f, 2, -3.0f);
                        self->timer = 0;
                        self->invisible = true;
                        self->actor.speedXZ = 10.0f;
                        self->action = 14;
                        self->actor.world.rot.y = self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
                        Audio_PlayActorSound2(&self->actor, 0x010002DC);
                        self->actionFunc = EnGeldB_RollForward;
                    } else {
                        f32 lastFrame = Animation_GetLastFrame(&self->skelAnime, 1);

                        Animation_Change_00375c08(&self->skelAnime, 1, 1.0f, 0.0f, lastFrame, 3, 0.0f);
                        *(u8*)&self->swordCollider[0x10] &= ~6;
                        self->action = 12;
                        self->actor.speedXZ = 0.0f;
                        self->spinAttackState = 0;
                        self->actionFunc = EnGeldB_SpinAttack;
                    }
                } else {
                    EnGeldB_SetupAdvance(self, play);
                }
            }
        }
        if ((prevFrame != (s32)self->skelAnime.curFrame) &&
            (((beforeCurFrame < 0) && (afterPrevFrame >= 1)) || ((beforeCurFrame < 5) && (afterPrevFrame >= 6)))) {
            Audio_PlayActorSound2(&self->actor, 0x01000410);
        }
        if ((*(u32*)((u8*)play + 0x5BF4) & 0x5F) == 0) {
            Audio_PlayActorSound2(&self->actor, 0x01000436);
        }
    }
}

void EnGeldB_Damaged(EnGeldB* self, PlayState* play) {
    s16 angleToWall;

    if (self->actor.bgCheckFlags & BGCHECKFLAG_GROUND_TOUCH) {
        self->actor.speedXZ = 0.0f;
    }
    if (self->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        if (self->actor.speedXZ < 0.0f) {
            self->actor.speedXZ += 0.05f;
        }
        self->invisible = false;
    }
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0x1194, 0);
    if (!EnGeldB_DodgeRanged(play, self) && !EnGeldB_ReactToPlayer(play, self, 0) &&
        FUN_003731e0(&self->skelAnime) && (self->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
        angleToWall = self->actor.wallYaw - self->actor.shape.rot.y;
        if ((self->actor.bgCheckFlags & BGCHECKFLAG_WALL) && (ABS(angleToWall) < 0x2EE0) &&
            (self->actor.xzDistToPlayer < 90.0f)) {
            EnGeldB_SetupJump(self);
        } else if (!EnGeldB_DodgeRanged(play, self)) {
            if ((self->actor.xzDistToPlayer <= 45.0f) && !FUN_00369608(play, &self->actor) &&
                (*(u32*)((u8*)play + 0x5BF4) & 7)) {
                Animation_PlayOnce(&self->skelAnime, 0);
                *(u8*)&self->swordCollider[0x10] &= ~4;
                self->action = 7;
                self->actor.speedXZ = 0.0f;
                self->spinAttackState = 0;
                FUN_003ff758(&self->actor.world.pos, 0x01000436);
                self->actionFunc = EnGeldB_Slash;
            } else {
                EnGeldB_SetupRollBack(self);
            }
        }
    }
}


void EnGeldB_Circle(EnGeldB* self, PlayState* play) {
    PlayState* play2 = play;
    Actor* thisx = &self->actor;
    s16 angleBehindLink;
    s16 phi_v1;
    s32 afterPrevFrame;
    s32 prevFrame;
    s32 beforeCurFrame;
    Player* player = GET_PLAYER(play);

    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0xFA0, 1);
    if (!EnGeldB_DodgeRanged(play, self) && !EnGeldB_ReactToPlayer(play, self, 0)) {
        self->actor.world.rot.y = self->actor.shape.rot.y + 0x3A98;
        angleBehindLink = player->actor.shape.rot.y + 0x8000;
        if (Math_SinS(angleBehindLink - self->actor.shape.rot.y) >= 0.0f) {
            self->actor.speedXZ -= 0.25f;
            if (self->actor.speedXZ < -8.0f) {
                self->actor.speedXZ = -8.0f;
            }
        } else if (Math_SinS(angleBehindLink - self->actor.shape.rot.y) < 0.0f) {
            self->actor.speedXZ += 0.25f;
            if (self->actor.speedXZ > 8.0f) {
                self->actor.speedXZ = 8.0f;
            }
        }
        if ((self->actor.bgCheckFlags & BGCHECKFLAG_WALL) ||
            !FUN_0035e600(&self->actor, play2, self->actor.speedXZ, self->actor.shape.rot.y + 0x3E80)) {
            if (self->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
                if (self->actor.speedXZ >= 0.0f) {
                    phi_v1 = self->actor.shape.rot.y + 0x3E80;
                } else {
                    phi_v1 = self->actor.shape.rot.y - 0x3E80;
                }
                phi_v1 = self->actor.wallYaw - phi_v1;
            } else {
                self->actor.speedXZ *= -0.8f;
                phi_v1 = 0;
            }
            if (ABS(phi_v1) > 0x4000) {
                self->actor.speedXZ *= -0.8f;
                if (self->actor.speedXZ < 0.0f) {
                    self->actor.speedXZ -= 0.5f;
                } else {
                    self->actor.speedXZ += 0.5f;
                }
            }
        }
        if (self->actor.xzDistToPlayer <= 45.0f) {
            Math_SmoothStepToF(&self->approachRate, -4.0f, 1.0f, 1.5f, 0.0f);
        } else if (self->actor.xzDistToPlayer > 40.0f) {
            Math_SmoothStepToF(&self->approachRate, 4.0f, 1.0f, 1.5f, 0.0f);
        } else {
            Math_SmoothStepToF(&self->approachRate, 0.0f, 1.0f, 6.65f, 0.0f);
        }
        if (self->approachRate != 0.0f) {
            self->actor.world.pos.x += Math_SinS(self->actor.shape.rot.y) * self->approachRate;
            self->actor.world.pos.z += Math_CosS(self->actor.shape.rot.y) * self->approachRate;
        }
        SkelAnime* sa = &self->skelAnime;
        if (ABS(self->approachRate) < ABS(self->actor.speedXZ)) {
            sa->playSpeed = self->actor.speedXZ * -0.5f;
        } else {
            sa->playSpeed = self->approachRate * -0.5f;
        }
        self->skelAnime.playSpeed = CLAMP(self->skelAnime.playSpeed, -3.0f, 3.0f);

        prevFrame = (s32)self->skelAnime.curFrame;
        FUN_003731e0(&self->skelAnime);
        beforeCurFrame = (s32)(self->skelAnime.curFrame - ABS(self->skelAnime.playSpeed));
        afterPrevFrame = (s32)ABS(self->skelAnime.playSpeed) + prevFrame;

        if ((prevFrame != (s32)self->skelAnime.curFrame) &&
            (((beforeCurFrame < 0) && (afterPrevFrame >= 1)) || ((beforeCurFrame < 5) && (afterPrevFrame >= 6)))) {
            Audio_PlayActorSound2(&self->actor, 0x01000410);
        }

        if ((*(u32*)((u8*)play + 0x5BF4) & 0x5F) == 0) {
            Audio_PlayActorSound2(&self->actor, 0x01000436);
        }
        if ((Math_CosS(angleBehindLink - self->actor.shape.rot.y) < -0.85f) &&
            !FUN_00369608(play, &self->actor) && (self->actor.xzDistToPlayer <= 45.0f)) {
            Animation_PlayOnce(&self->skelAnime, 0);
            *(u8*)&self->swordCollider[0x10] &= ~4;
            self->action = 7;
            self->actor.speedXZ = 0.0f;
            self->spinAttackState = 0;
            FUN_003ff758(&self->actor.world.pos, 0x01000436);
            self->actionFunc = EnGeldB_Slash;
        } else if (--self->timer == 0) {
            if (FUN_00369608(play, &self->actor) && (Rand_ZeroOne() > 0.5f)) {
                EnGeldB_SetupRollBack(self);
            } else {
                EnGeldB_SetupReady(self);
            }
        }
    }
}

}
