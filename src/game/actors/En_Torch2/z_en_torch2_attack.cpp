#if 0
/* Sol complete semantic draft; original ASM remains active. */
extern "C" {
#include "z_en_torch2_state.hpp"
}
extern "C" {
Actor* FUN_00346e2c(PlayState*, Actor*, f32, f32);
void FUN_00346bb4(EnTorch2State*, EnTorch2ControlInput*, Actor*);
s32 FUN_00346c48(PlayState*, EnTorch2ControlInput*, EnTorch2State*);
s32 FUN_0035b950(PlayState*, Actor*, s32, s32, s16, f32);
s32 FUN_0035f228(PlayState*, Actor*);
void FUN_0036b4ec(SkelAnime*, PlayState*);
void FUN_0036aeb4(Vec3f*, s32);
MTX34* FUN_003478bc(SkeletonAnimationModel*, s32);
struct EnTorch2AttackPlayFrame { u8 pad[0x5BF4]; u32 gameplayFrames; };
struct EnTorch2AttackCameraView { u8 pad[0x184]; s16 yaw; };
struct EnTorch2AttackFilterView { u8 pad[0x11A]; u16 timer; };
void FUN_00192d38(EnTorch2State* self, PlayState* play, EnTorch2ControlInput* input) {
    EnTorch2State* player = (EnTorch2State*)GET_PLAYER(play);
    Actor* attackItem = FUN_00346e2c(play, &self->actor, 4000.0f, 1.5f);
    if (attackItem == 0) attackItem = FUN_00346d94(play, &self->actor);
    Camera* camera = Gameplay_GetCamera(play, 0);
    if (self->controlFreeze != 0) return;
    self->stickTilt = 0.0f;
    s16 relativeYaw = player->actor.shape.rot.y - self->actor.shape.rot.y;
    if ((self->meleeWeaponQuads[0].base.acFlags & 0x80) || (self->meleeWeaponQuads[1].base.acFlags & 0x80)) {
        self->meleeWeaponQuads[0].base.acFlags &= ~0x80;
        self->meleeWeaponQuads[1].base.acFlags &= ~0x80;
        self->meleeWeaponQuads[0].base.atFlags |= 4;
        self->meleeWeaponQuads[1].base.atFlags |= 4;
        self->cylinder.base.acFlags &= ~2;
        if (self->lastSwordAnim != self->meleeWeaponAnimation) {
            ++self->staggerCount;
            self->lastSwordAnim = self->meleeWeaponAnimation;
        }
        if (gSaveContext.health < 0x50) {
            if (self->counterState != 0) {
                self->counterState = 0;
                self->staggerTimer = 75;
            }
            goto ignoreHits;
        }
    }
    if (self->counterState != 0 && self->meleeWeaponState != 0 && ((EnTorch2AttackFilterView*)self)->timer == 0) {
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->meleeWeaponQuads[0].base);
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->meleeWeaponQuads[1].base);
    }
ignoreHits:
    if (self->invincibilityTimer < 0 && self->actionState != 3 && (self->cylinder.base.acFlags & 2)) {
        self->cylinder.base.acFlags &= ~2;
    }
    if (self->dodgeRollState != 0) self->stickTilt = 127.0f;
    else if (attackItem != 0) {
        self->dodgeRollState = 1;
        self->stickAngle = self->actor.yawTowardsPlayer;
        self->stickTilt = 127.0f;
        input->current.buttons = 1;
    } else if (self->jumpslashTimer == 0) {
        if ((player->meleeWeaponState != 0 || player->actor.velocity.y > -3.0f) && player->meleeWeaponAnimation == 17) {
            self->actor.world.rot.y = self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
            if (((EnTorch2AttackPlayFrame*)play)->gameplayFrames & 1) self->stickAngle = self->actor.yawTowardsPlayer + 0x4000;
            else self->stickAngle = self->actor.yawTowardsPlayer - 0x4000;
            input->current.buttons |= 1;
            self->stickTilt = 127.0f;
            self->jumpslashTimer = 23;
            self->jumpslashFlag = 0;
        } else if (self->swordJumpState != 0) {
            self->stickTilt = 0.0f;
            player->stateFlags3 |= 4;
            static Vector3f swordOffset(1800.0f, 400.0f, 0.0f);
            Vec3f pos;
            MTX34* limb = FUN_003478bc(player->skelAnime.unk_28, 16);
            VEC3TransformAsm((Vector3f*)&pos, limb, &swordOffset);
            Math_SmoothStepToF(&self->actor.world.pos.x, pos.x, 1.0f, 5.0f, 0.0f);
            Math_SmoothStepToF(&self->actor.world.pos.z, pos.z, 1.0f, 5.0f, 0.0f);
            --self->swordJumpTimer;
            if (self->swordJumpTimer == 0 || (player->invincibilityTimer > 0 && self->meleeWeaponState == 0)) {
                self->actor.world.rot.y = self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
                input->current.buttons = 1;
                player->stateFlags3 &= ~4;
                self->stickTilt = 127.0f;
                player->skelAnime.curFrame = 3.0f;
                self->stickAngle = self->actor.yawTowardsPlayer + 0x8000;
                self->swordJumpState = self->swordJumpTimer = 0;
                self->actor.flags |= 1;
            } else if (self->swordJumpState == 1) {
                if (self->swordJumpTimer <= 23) {
                    FUN_00346c48(play, input, self);
                    ++self->swordJumpState;
                } else if (self->swordJumpTimer == 29) {
                    FUN_0036aeb4(&self->actor.world.pos, 0x0100056C);
                }
            }
        } else {
            if (self->holdShieldTimer != 0) {
                --self->holdShieldTimer;
                input->current.buttons = 0x100;
            }
            if (FUN_0035b950(play, &self->actor, 0x7FFF, 0x7FFF, self->actor.world.rot.y, 120.0f)) {
                if (player->meleeWeaponAnimation == 12 && self->actor.xzDistToPlayer < 90.0f) {
                    if (self->meleeWeaponState == 0 && self->counterState == 0 && player->invincibilityTimer == 0 &&
                        self->actor.xzDistToPlayer <= 85.0f && FUN_0035f228(play, &self->actor)) {
                        self->stickTilt = 0.0f;
                        self->swordJumpState = 1;
                        player->stateFlags3 |= 4;
                        self->actor.flags &= ~1;
                        self->swordJumpTimer = 41;
                        player->meleeWeaponState = 0;
                        player->xzSpeed = 0.0f;
                        self->invincibilityTimer = -(s32)(21.0f / gGameInfo->data[126] + 0.5f);
                        self->xzSpeed = 0.0f;
                        player->skelAnime.curFrame = 2.0f;
                        FUN_0036b4ec(&player->skelAnime, play);
                        self->holdShieldTimer = 0;
                        input->current.buttons = 1;
                        self->attackDelay = gGameInfo->data[1249];
                    } else FUN_00346bb4(self, input, &self->actor);
                } else {
                    self->stickAngle = self->actor.yawTowardsPlayer;
                    input->current.buttons = 2;
                    if (player->meleeWeaponAnimation <= 3) self->stickTilt = 0.0f;
                    else if (player->meleeWeaponAnimation <= 7) {
                        self->stickTilt = 127.0f;
                        self->stickAngle += 0x4000;
                    } else if (player->meleeWeaponAnimation <= 11) {
                        self->stickTilt = 127.0f;
                        self->stickAngle -= 0x4000;
                    } else if (player->meleeWeaponAnimation <= 23) input->current.buttons = 0x100;
                    else FUN_00346bb4(self, input, &self->actor);
                    if (!(input->current.buttons & 0x101) && self->meleeWeaponState == 0 && player->meleeWeaponState != 0) self->counterState = 1;
                }
            } else {
                self->stickAngle = self->actor.yawTowardsPlayer;
                s32 absYaw = relativeYaw < 0 ? -relativeYaw : relativeYaw;
                if (self->actor.xzDistToPlayer <= 90.0f && self->actor.xzDistToPlayer > 70.0f && absYaw >= 0x7800 &&
                    (self->actor.isTargeted || !(player->stateFlags1 & 0x400000))) {
                    FUN_00346c48(play, input, self);
                } else {
                    f32 aggression = gGameInfo->data[1248];
                    if ((self->actor.xzDistToPlayer <= aggression + 70.0f || (self->actor.xzDistToPlayer <= aggression + 80.0f && player->meleeWeaponState)) && self->meleeWeaponState == 0) {
                        u32 avoidAttack = 0;
                        if (gGameInfo->data[1251] == 0) avoidAttack = FUN_00374be8(play, 13);
                        if (!(FUN_00346c48(play, input, self) | avoidAttack) && self->counterState == 0) FUN_00346bb4(self, input, &self->actor);
                    } else if (self->actor.xzDistToPlayer <= aggression + 50.0f) {
                        self->stickTilt = 127.0f;
                        self->stickAngle = self->actor.yawTowardsPlayer;
                        if (!self->actor.isTargeted) Math_SmoothStepToS(&self->stickAngle, player->actor.shape.rot.y + 0x7FFF, 1, 0x2328, 0);
                    } else if (self->actor.xzDistToPlayer > aggression + 100.0f) {
                        if (player->meleeWeaponState == 0 || player->meleeWeaponAnimation <= 23 || player->meleeWeaponAnimation > 27 || self->actor.xzDistToPlayer >= 280.0f) {
                            self->stickTilt = 127.0f;
                            self->stickAngle = self->actor.yawTowardsPlayer;
                            if (!self->actor.isTargeted) Math_SmoothStepToS(&self->stickAngle, player->actor.shape.rot.y + 0x7FFF, 1, 0x2328, 0);
                        } else FUN_00346bb4(self, input, &self->actor);
                    } else if ((absYaw < 0x7800 && absYaw >= 0x3000) || !FUN_00346c48(play, input, self)) {
                        self->stickAngle = self->actor.yawTowardsPlayer;
                        self->stickTilt = 127.0f;
                        if (!self->actor.isTargeted) Math_SmoothStepToS(&self->stickAngle, player->actor.shape.rot.y + 0x7FFF, 1, 0x2328, 0);
                    }
                }
            }
        }
    } else if (self->jumpslashFlag) {
        if (self->alpha == 255 && self->actor.velocity.y > 0.0f) input->current.buttons |= 2;
    } else if (self->actor.bgCheckFlags & 1) {
        self->actor.world.rot.y = self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
        self->stickAngle = self->actor.yawTowardsPlayer;
        if (self->alpha != 255) {
            self->stickAngle += 0x8000;
            self->stickTilt = 127.0f;
            self->zTargetFlag = 1;
        }
        input->current.buttons |= 1;
        self->jumpslashFlag = 1;
        self->invincibilityTimer = (s32)(30.0f / gGameInfo->data[126] + 0.5f);
    }
    s16 cameraAngle = ((EnTorch2AttackCameraView*)camera)->yaw - self->stickAngle;
    input->current.stickX = (s8)(Math_SinS(cameraAngle) * self->stickTilt);
    input->current.stickY = (s8)(Math_CosS(cameraAngle) * self->stickTilt);
    if (self->alpha != 255 && ((((EnTorch2AttackPlayFrame*)play)->gameplayFrames & 7) == 0)) ++self->alpha;
}
}

#endif
