#if 0
/* Astra constructor reconstruction: 552/552 B, 9 register-allocation words.
 * Original ASM remains active until an exact full-link match is established. */
extern "C" {
#include "z_en_torch2_state.hpp"
}
inline void* operator new(unsigned int, void* p) throw() { return p; }
extern "C" {
void Player_SetModelGroup(EnTorch2State*, s32);
void* ZAR_GetCMBByIndex(ZARInfo*, s32);
void FUN_0036932c(SkeletonAnimationModel*, s32);
void FUN_0034775c(SkelAnime*, EnTorch2LimbCallbackBase*);
extern DamageTable EnTorch2_sDamageTable;
void EnTorch2_Init(EnTorch2State* self, PlayState* play) {
    Actor* thisx = &self->actor;
    self->input.current.buttons = self->input.current.pressed = self->input.current.released = 0;
    self->input.current.stickX = self->input.current.stickY = 0;
    self->currentShield = 2;
    self->heldItemAction = self->heldItemId = 3;
    Player_SetModelGroup(self, 2);
    ObjectStatus* object = play->GetObjectStatus((u8)self->actor.objBankIndex);
    self->playerModelData = ((EnTorch2State*)GET_PLAYER(play))->playerModelData;
    void* model = ZAR_GetCMBByIndex(&object->zarInfo, 0);
    self->modelCmb = model;
    ((EnTorch2PlayCallbacks*)play)->playerInit(thisx, play, model);
    FUN_0036932c(self->skelAnime.unk_28, 2);
    FUN_0036932c(self->skelAnime.unk_28, 3);
    new (&self->limbCallback) EnTorch2LimbCallback();
    self->limbCallback.Initialize(thisx, play, &self->skelAnime);
    FUN_0034775c(&self->skelAnime, &self->limbCallback);
    self->actor.naviEnemyId = 0x26;
    self->cylinder.base.acFlags = 9;
    self->meleeWeaponQuads[0].base.atFlags = self->meleeWeaponQuads[1].base.atFlags = 0x11;
    self->meleeWeaponQuads[0].base.acFlags = self->meleeWeaponQuads[1].base.acFlags = 0xD;
    self->meleeWeaponQuads[0].base.colType = self->meleeWeaponQuads[1].base.colType = 9;
    self->meleeWeaponQuads[0].info.toucher.damage = self->meleeWeaponQuads[1].info.toucher.damage = 8;
    self->meleeWeaponQuads[0].info.bumperFlags = self->meleeWeaponQuads[1].info.bumperFlags = 1;
    self->shieldQuad.base.atFlags = 0x11;
    self->shieldQuad.base.acFlags = 0xD;
    self->actor.colChkInfo.damageTable = &EnTorch2_sDamageTable;
    self->actor.colChkInfo.health = (s16)gSaveContext.healthCapacity >> 3;
    self->actor.colChkInfo.cylRadius = 60;
    self->actor.colChkInfo.cylHeight = 100;
    ((EnTorch2PlayCallbacks*)play)->playerReset(thisx, play, -6.0f);
    self->actionState = 0;
    self->dodgeRollState = 0;
    self->swordJumpHeight = 0.0f;
    self->swordJumpState = 0;
    self->jumpslashTimer = 0;
    self->jumpslashFlag = 0;
    self->staggerCount = self->staggerTimer = self->counterState = 0;
    self->lastSwordAnim = 0;
    self->alpha = 95;
    self->spawnPoint = self->actor.home.pos;
}
}

#endif
