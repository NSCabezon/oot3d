#if 0
/* Sol complete semantic draft; original ASM remains active. */
extern "C" {
#include "z_en_torch2_state.hpp"
}
extern "C" {
void FUN_00193740(EnTorch2State*, PlayState*, EnTorch2ControlInput*);
void FUN_00192d38(EnTorch2State*, PlayState*, EnTorch2ControlInput*);
void FUN_001e2f1c(EnTorch2State*, PlayState*, EnTorch2ControlInput*);
s32 Actor_ApplyDamage(Actor*);
void FUN_00373d0c(PlayState*);
void FUN_0034f724(PlayState*);
void FUN_00375b70(PlayState*, Actor*);
void Item_DropCollectibleRandom(PlayState*, Actor*, Vec3f*, s32);
void FUN_00375fd0(Actor*, ColliderInfo*, s32);
void Actor_SetColorFilter(Actor*, s32, s32, s32, s32);
void FUN_0036b4ec(SkelAnime*, PlayState*);
void FUN_0036aeb4(Vec3f*, s32);
void Collider_ResetQuadAT(PlayState*, EnTorch2StateQuad*);
Vec3f* FUN_0036f57c(EnTorch2State*, s32);
extern f32 EnTorch2_sSwordYOffset;
struct EnTorch2SaveRuntime { u8 pad[0x80]; u16 magicState; };
extern EnTorch2SaveRuntime EnTorch2_sSaveRuntime;
struct EnTorch2ActorFilterView { u8 pad[0x11A]; u16 timer; u32 params; };
void EnTorch2_Update(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnTorch2State* self = (EnTorch2State*)thisx;
    EnTorch2State* player = (EnTorch2State*)GET_PLAYER(play);
    EnTorch2ControlInput* input = &self->input;
    input->current.buttons = 0;
    switch (self->actionState) {
    case 0: FUN_00193740(self, play, input); break;
    case 1: FUN_00192d38(self, play, input); break;
    case 3: FUN_001e2f1c(self, play, input); break;
    case 2:
        if (self->alpha - 9 <= 0) {
            self->alpha = 0;
            Actor_Kill(thisx);
        } else {
            self->alpha -= 9;
            thisx->shape.shadowAlpha -= 9;
        }
        break;
    }
    if (self->heldItemAction == 0) self->heldItemAction = 3;
    if (self->actionState != 0) {
        if (EnTorch2_sSaveRuntime.magicState == 3 &&
            (player->meleeWeaponState == 0 || player->meleeWeaponAnimation <= 23 || player->meleeWeaponAnimation > 27)) {
            self->stickTilt = 0.0f;
            input->current.stickX = 0;
            input->current.stickY = 0;
            input->current.buttons = 0x100;
        }
        if (self->actionState == 1 && thisx->xzDistToPlayer <= 610.0f && self->zTargetFlag) {
            input->current.buttons |= 0x200;
        }
    }
    u16 changed = input->previous.buttons ^ input->current.buttons;
    input->current.pressed = input->current.buttons & changed;
    if (input->current.buttons & 0x100) {
        input->current.buttons = (!self->counterState && !self->meleeWeaponState) ? 0x100 : input->current.buttons ^ 0x100;
    }
    input->current.released = input->previous.buttons & changed;
    input->previous.buttons = input->current.buttons & 0xFFFC;
    self->controlInput = input;
    input->scaledX = (s16)(input->current.stickX * 62.0f);
    self->controlInput->scaledY = (s16)(self->controlInput->current.stickY * 62.0f);
    self->controlInput->relativeX += self->controlInput->scaledX - self->controlInput->relativeX;
    self->controlInput->relativeY += self->controlInput->scaledY - self->controlInput->relativeY;
    if (self->attackDelay) --self->attackDelay;
    if (self->rollDelay) --self->rollDelay;
    if (self->attackDelay) input->current.buttons &= ~1;
    if (self->rollDelay) input->current.buttons &= ~2;
    self->controlInput->previous = self->controlInput->current;
    if (thisx->colChkInfo.health == 0 && self->deathFlag) {
        self->csAction = 24;
        self->csActor = &player->actor;
        self->haltActors = 1;
        self->deathFlag = 0;
    }
    if (self->dodgeRollState != 0) {
        if (self->dodgeRollState == 1) self->invincibilityTimer = (s32)(60.0f / gGameInfo->data[126] + 0.5f);
        self->dodgeRollState = self->invincibilityTimer > 0 ? 2 : 0;
    }
    if (self->invincibilityTimer == 0 && thisx->colChkInfo.health != 0 &&
        (self->cylinder.base.acFlags & 2) && !(self->stateFlags1 & 0x4000000) &&
        !(self->meleeWeaponQuads[0].base.atFlags & 2) && !(self->meleeWeaponQuads[1].base.atFlags & 2)) {
        if (!Actor_ApplyDamage(thisx)) {
            FUN_00373d0c(play);
            thisx->flags &= ~5;
            self->knockbackDamage = thisx->colChkInfo.damage;
            self->knockbackType = 2;
            self->knockbackSpeed = 6.0f;
            self->knockbackRot = thisx->yawTowardsPlayer + 0x8000;
            self->knockbackYVelocity = 6.0f;
            ++self->deathFlag;
            self->actionState = 2;
            FUN_00375b70(play, thisx);
            Item_DropCollectibleRandom(play, thisx, &thisx->world.pos, 0xC0);
            self->stateFlags3 &= ~4;
        } else {
            FUN_0034f724(play);
            if (thisx->colChkInfo.damageEffect == 1) {
                if (self->alpha == 255) Actor_SetColorFilter(thisx, 0, 255, 0, 80);
                else Actor_SetColorFilter(thisx, 0, 255, 0x200000, 80);
            } else {
                thisx->flags &= ~1;
                self->knockbackDamage = thisx->colChkInfo.damage;
                self->knockbackType = 1;
                self->knockbackSpeed = 8.0f;
                self->knockbackRot = thisx->yawTowardsPlayer + 0x8000;
                self->knockbackYVelocity = 6.0f;
                FUN_00375fd0(thisx, &self->cylinder.info, 1);
                self->stateFlags3 &= ~4;
                self->stateFlags3 |= 1;
                self->actionState = 3;
                if (self->alpha == 255) Actor_SetColorFilter(thisx, 0x400000, 255, 0, 12);
                else Actor_SetColorFilter(thisx, 0x400000, 255, 0x200000, 12);
            }
        }
        thisx->colChkInfo.damage = 0;
        self->knockbackDamage = 0;
    }
    EnTorch2ActorFilterView* filter = (EnTorch2ActorFilterView*)thisx;
    if (filter->timer == 0 || (filter->params & 0x400000)) {
        if (self->stateFlags3 & 4) self->rollDelay = 2;
        self->stateFlags3 &= ~4;
    } else {
        self->stateFlags3 |= 4;
        self->stateFlags1 &= ~0x4000000;
        self->invincibilityTimer = 0;
        self->stateFlags2 &= ~0x20000;
        if (self->meleeWeaponState) self->meleeWeaponState = 0;
        self->clear22A4 = 0;
        self->clear22D8 = 0;
        self->clear230C = 0;
        input->current.buttons = 0;
        input->current.pressed = 0;
        self->xzSpeed = 0.0f;
    }
    self->controlFreeze = 0;
    s32 keepYaw = 0;
    s16 yaw;
    if ((self->stateFlags3 & 4) && self->swordJumpState == 0) {
        keepYaw = 1;
        self->controlFreeze = 1;
        yaw = thisx->shape.rot.y;
    }
    ((EnTorch2PlayCallbacks*)play)->playerUpdate(thisx, play, input);
    if (keepYaw && thisx->shape.rot.y != yaw) thisx->shape.rot.y = yaw;
    if (self->xzSpeed == -18.0f) {
        u8 threshold = (u32)Rand_CenteredFloat(2.0f) + 6;
        if (gSaveContext.health < 0x50) threshold = (u32)Rand_CenteredFloat(2.0f) + 3;
        if (thisx->xzDistToPlayer > 80.0f) self->xzSpeed = 1.2f;
        else if (thisx->xzDistToPlayer < 70.0f) self->xzSpeed = -1.5f;
        else self->xzSpeed = 1.0f;
        if (threshold < self->staggerCount) {
            self->skelAnime.playSpeed *= 0.6f;
            FUN_0036aeb4(&thisx->world.pos, 0x01000040);
            self->staggerTimer = 0;
            self->staggerCount = 0;
        }
    }
    if (player->xzSpeed == -18.0f) {
        if (thisx->xzDistToPlayer > 80.0f) player->xzSpeed = 1.2f;
        else if (thisx->xzDistToPlayer < 70.0f) player->xzSpeed = -1.5f;
        else player->xzSpeed = 1.0f;
    }
    if (self->counterState != 0 && self->counterState == 1) {
        if (self->meleeWeaponState == 0) self->counterState = 0;
        else {
            self->counterState = 2;
            self->skelAnime.curFrame = player->skelAnime.curFrame - player->skelAnime.playSpeed;
            self->skelAnime.playSpeed = player->skelAnime.playSpeed;
            FUN_0036b4ec(&self->skelAnime, play);
            Collider_ResetQuadAT(play, &self->meleeWeaponQuads[0]);
            Collider_ResetQuadAT(play, &self->meleeWeaponQuads[1]);
        }
    }
    if (self->staggerTimer != 0) {
        --self->staggerTimer;
        if (self->staggerTimer == 0) {
            self->counterState = 0;
            self->staggerCount = 0;
        }
    }
    if (self->invincibilityTimer != 0) {
        self->cylinder.base.colType = 10;
        self->cylinder.info.elemType = 5;
    } else {
        self->cylinder.base.colType = 5;
        self->cylinder.info.elemType = 1;
    }
    if (self->swordJumpState != 0) {
        Vec3f pos = *FUN_0036f57c((EnTorch2State*)GET_PLAYER(play), 16);
        pos.y += EnTorch2_sSwordYOffset;
        Math_SmoothStepToF(&self->swordJumpHeight, (pos.y - thisx->world.pos.y) * 100.0f, 1.0f, 2000.0f, 0.0f);
        thisx->velocity.y -= 0.6f;
    } else if (self->swordJumpHeight != 0.0f) {
        thisx->world.pos.y += self->swordJumpHeight * 0.01f;
        self->swordJumpHeight = 0;
    }
    self->zTargetFlag = self->actionState != 0 && self->invincibilityTimer >= 0;
    if (self->jumpslashTimer) --self->jumpslashTimer;
    thisx->focus.pos = thisx->world.pos;
    thisx->focus.pos.y += 20.0f;
    thisx->shape.yOffset = self->swordJumpHeight;
}
}

#endif
