#ifndef Z_EN_TORCH2_STATE_HPP
#define Z_EN_TORCH2_STATE_HPP
#include <stddef.h>
#include "z_en_torch2.hpp"
// Native callback vtables have two method slots: Override and Post.
// Derived address point 0x4EC008, base 0x4EC030; preceding two words are zero.
// Adjacent 0x4EC018 resource-object table is a separate class.
// The implicit derived constructor permits value-initialization to clear all
// six words before the user-provided base constructor initializes its fields.
extern "C++" {
struct EnTorch2LimbCallbackBase {
    virtual void Override(s32 limbIndex, MTX34* mtx);
    virtual void Post(s32 limbIndex, MTX34* mtx);
    Actor* actor;
    PlayState* play;
    SkelAnime* skelAnime;
    OverrideLimbDrawOpa overrideLimbDraw;
    PostLimbDrawOpa postLimbDraw;
    void Initialize(Actor* owner, PlayState* state, SkelAnime* skeleton) {
        skelAnime = skeleton;
        actor = owner;
        play = state;
    }
    EnTorch2LimbCallbackBase() : actor(0), play(0), skelAnime(0), overrideLimbDraw(0), postLimbDraw(0) {}
};
struct EnTorch2LimbCallback : EnTorch2LimbCallbackBase {
    virtual void Override(s32 limbIndex, MTX34* mtx);
    virtual void Post(s32 limbIndex, MTX34* mtx);
};
typedef char EnTorch2LimbCallback_actor_offset[(offsetof(EnTorch2LimbCallbackBase, actor) == 0x4) ? 1 : -1];
typedef char EnTorch2LimbCallback_play_offset[(offsetof(EnTorch2LimbCallbackBase, play) == 0x8) ? 1 : -1];
typedef char EnTorch2LimbCallback_skelAnime_offset[(offsetof(EnTorch2LimbCallbackBase, skelAnime) == 0xC) ? 1 : -1];
typedef char EnTorch2LimbCallback_override_offset[(offsetof(EnTorch2LimbCallbackBase, overrideLimbDraw) == 0x10) ? 1 : -1];
typedef char EnTorch2LimbCallback_post_offset[(offsetof(EnTorch2LimbCallbackBase, postLimbDraw) == 0x14) ? 1 : -1];
typedef char EnTorch2LimbCallback_base_size[(sizeof(EnTorch2LimbCallbackBase) == 0x18) ? 1 : -1];
typedef char EnTorch2LimbCallback_size[(sizeof(EnTorch2LimbCallback) == 0x18) ? 1 : -1];
}
struct EnTorch2InputSample { u32 buttons, pressed, released; s16 stickX, stickY; };
struct EnTorch2ControlInput {
    EnTorch2InputSample current;
    EnTorch2InputSample previous;
    f32 scaledX, scaledY, relativeX, relativeY;
};
struct EnTorch2StateQuad { Collider base; ColliderInfo info; u8 geometry[0x40]; };
struct EnTorch2State {
    /* 0x0000 */ Actor actor;
    u8 pad01A4[0x2];
    /* 0x01A6 */ u8 currentShield;
    u8 pad01A7[0x2];
    /* 0x01A9 */ u8 heldItemAction;
    /* 0x01AA */ u8 heldItemId;
    u8 pad01AB[0xA9];
    /* 0x0254 */ SkelAnime skelAnime;
    u8 pad02D8[0xFE4];
    /* 0x12BC */ u8 csAction;
    u8 pad12BD[0x3];
    /* 0x12C0 */ Actor* csActor;
    u8 pad12C4[0x1E];
    /* 0x12E2 */ s16 haltActors;
    u8 pad12E4[0x2C];
    /* 0x1310 */ ColliderCylinder cylinder;
    /* 0x1368 */ EnTorch2StateQuad meleeWeaponQuads[2];
    u8 pad1468[0x100];
    /* 0x1568 */ EnTorch2StateQuad shieldQuad;
    u8 pad15E8[0x128];
    /* 0x1710 */ u32 stateFlags1;
    /* 0x1714 */ u32 stateFlags2;
    u8 pad1718[0x12];
    /* 0x172A */ u8 stateFlags3;
    u8 pad172B[0xAF1];
    /* 0x221C */ f32 xzSpeed;
    u8 pad2220[0x6];
    /* 0x2226 */ s8 meleeWeaponAnimation;
    /* 0x2227 */ u8 meleeWeaponState;
    u8 pad2228[0x68];
    /* 0x2290 */ u8 knockbackDamage;
    /* 0x2291 */ u8 knockbackType;
    /* 0x2292 */ s16 knockbackRot;
    /* 0x2294 */ f32 knockbackSpeed;
    /* 0x2298 */ f32 knockbackYVelocity;
    u8 pad229C[0x8];
    /* 0x22A4 */ u32 clear22A4;
    u8 pad22A8[0x30];
    /* 0x22D8 */ u32 clear22D8;
    u8 pad22DC[0x30];
    /* 0x230C */ u32 clear230C;
    u8 pad2310[0x178];
    /* 0x2488 */ s8 invincibilityTimer;
    u8 pad2489[0x53];
    /* 0x24DC */ void* modelCmb;
    /* 0x24E0 */ void* playerModelData;
    u8 pad24E4[0xC];
    /* 0x24F0 */ EnTorch2LimbCallback limbCallback;
    u8 pad2508[0x4C0];
    /* 0x29C8 */ EnTorch2ControlInput* controlInput;
    u8 pad29CC[0x80];
    /* 0x2A4C */ Vec3f spawnPoint;
    /* 0x2A58 */ EnTorch2ControlInput input;
    /* 0x2A88 */ f32 stickTilt;
    /* 0x2A8C */ s16 stickAngle;
    /* 0x2A8E */ s16 controlFreeze;
    /* 0x2A90 */ f32 swordJumpHeight;
    /* 0x2A94 */ s32 holdShieldTimer;
    /* 0x2A98 */ u8 swordJumpState;
    /* 0x2A99 */ u8 zTargetFlag;
    /* 0x2A9A */ u8 deathFlag;
    /* 0x2A9B */ u8 jumpslashTimer;
    /* 0x2A9C */ u8 jumpslashFlag;
    /* 0x2A9D */ u8 actionState;
    /* 0x2A9E */ u8 swordJumpTimer;
    /* 0x2A9F */ u8 counterState;
    /* 0x2AA0 */ u8 dodgeRollState;
    /* 0x2AA1 */ u8 staggerCount;
    /* 0x2AA2 */ u8 staggerTimer;
    /* 0x2AA3 */ s8 lastSwordAnim;
    /* 0x2AA4 */ u8 alpha;
    /* 0x2AA5 */ u8 attackDelay;
    /* 0x2AA6 */ u8 rollDelay;
    u8 pad2AA7[0x1];
};
typedef char EnTorch2State_actor_offset[(offsetof(EnTorch2State, actor) == 0x0) ? 1 : -1];
typedef char EnTorch2State_currentShield_offset[(offsetof(EnTorch2State, currentShield) == 0x1A6) ? 1 : -1];
typedef char EnTorch2State_heldItemAction_offset[(offsetof(EnTorch2State, heldItemAction) == 0x1A9) ? 1 : -1];
typedef char EnTorch2State_heldItemId_offset[(offsetof(EnTorch2State, heldItemId) == 0x1AA) ? 1 : -1];
typedef char EnTorch2State_skelAnime_offset[(offsetof(EnTorch2State, skelAnime) == 0x254) ? 1 : -1];
typedef char EnTorch2State_csAction_offset[(offsetof(EnTorch2State, csAction) == 0x12BC) ? 1 : -1];
typedef char EnTorch2State_csActor_offset[(offsetof(EnTorch2State, csActor) == 0x12C0) ? 1 : -1];
typedef char EnTorch2State_haltActors_offset[(offsetof(EnTorch2State, haltActors) == 0x12E2) ? 1 : -1];
typedef char EnTorch2State_cylinder_offset[(offsetof(EnTorch2State, cylinder) == 0x1310) ? 1 : -1];
typedef char EnTorch2State_meleeWeaponQuads_offset[(offsetof(EnTorch2State, meleeWeaponQuads) == 0x1368) ? 1 : -1];
typedef char EnTorch2State_shieldQuad_offset[(offsetof(EnTorch2State, shieldQuad) == 0x1568) ? 1 : -1];
typedef char EnTorch2State_stateFlags1_offset[(offsetof(EnTorch2State, stateFlags1) == 0x1710) ? 1 : -1];
typedef char EnTorch2State_stateFlags2_offset[(offsetof(EnTorch2State, stateFlags2) == 0x1714) ? 1 : -1];
typedef char EnTorch2State_stateFlags3_offset[(offsetof(EnTorch2State, stateFlags3) == 0x172A) ? 1 : -1];
typedef char EnTorch2State_xzSpeed_offset[(offsetof(EnTorch2State, xzSpeed) == 0x221C) ? 1 : -1];
typedef char EnTorch2State_meleeWeaponAnimation_offset[(offsetof(EnTorch2State, meleeWeaponAnimation) == 0x2226) ? 1 : -1];
typedef char EnTorch2State_meleeWeaponState_offset[(offsetof(EnTorch2State, meleeWeaponState) == 0x2227) ? 1 : -1];
typedef char EnTorch2State_knockbackDamage_offset[(offsetof(EnTorch2State, knockbackDamage) == 0x2290) ? 1 : -1];
typedef char EnTorch2State_knockbackType_offset[(offsetof(EnTorch2State, knockbackType) == 0x2291) ? 1 : -1];
typedef char EnTorch2State_knockbackRot_offset[(offsetof(EnTorch2State, knockbackRot) == 0x2292) ? 1 : -1];
typedef char EnTorch2State_knockbackSpeed_offset[(offsetof(EnTorch2State, knockbackSpeed) == 0x2294) ? 1 : -1];
typedef char EnTorch2State_knockbackYVelocity_offset[(offsetof(EnTorch2State, knockbackYVelocity) == 0x2298) ? 1 : -1];
typedef char EnTorch2State_clear22A4_offset[(offsetof(EnTorch2State, clear22A4) == 0x22A4) ? 1 : -1];
typedef char EnTorch2State_clear22D8_offset[(offsetof(EnTorch2State, clear22D8) == 0x22D8) ? 1 : -1];
typedef char EnTorch2State_clear230C_offset[(offsetof(EnTorch2State, clear230C) == 0x230C) ? 1 : -1];
typedef char EnTorch2State_invincibilityTimer_offset[(offsetof(EnTorch2State, invincibilityTimer) == 0x2488) ? 1 : -1];
typedef char EnTorch2State_modelCmb_offset[(offsetof(EnTorch2State, modelCmb) == 0x24DC) ? 1 : -1];
typedef char EnTorch2State_playerModelData_offset[(offsetof(EnTorch2State, playerModelData) == 0x24E0) ? 1 : -1];
typedef char EnTorch2State_limbCallback_offset[(offsetof(EnTorch2State, limbCallback) == 0x24F0) ? 1 : -1];
typedef char EnTorch2State_controlInput_offset[(offsetof(EnTorch2State, controlInput) == 0x29C8) ? 1 : -1];
typedef char EnTorch2State_spawnPoint_offset[(offsetof(EnTorch2State, spawnPoint) == 0x2A4C) ? 1 : -1];
typedef char EnTorch2State_input_offset[(offsetof(EnTorch2State, input) == 0x2A58) ? 1 : -1];
typedef char EnTorch2State_stickTilt_offset[(offsetof(EnTorch2State, stickTilt) == 0x2A88) ? 1 : -1];
typedef char EnTorch2State_stickAngle_offset[(offsetof(EnTorch2State, stickAngle) == 0x2A8C) ? 1 : -1];
typedef char EnTorch2State_controlFreeze_offset[(offsetof(EnTorch2State, controlFreeze) == 0x2A8E) ? 1 : -1];
typedef char EnTorch2State_swordJumpHeight_offset[(offsetof(EnTorch2State, swordJumpHeight) == 0x2A90) ? 1 : -1];
typedef char EnTorch2State_holdShieldTimer_offset[(offsetof(EnTorch2State, holdShieldTimer) == 0x2A94) ? 1 : -1];
typedef char EnTorch2State_swordJumpState_offset[(offsetof(EnTorch2State, swordJumpState) == 0x2A98) ? 1 : -1];
typedef char EnTorch2State_zTargetFlag_offset[(offsetof(EnTorch2State, zTargetFlag) == 0x2A99) ? 1 : -1];
typedef char EnTorch2State_deathFlag_offset[(offsetof(EnTorch2State, deathFlag) == 0x2A9A) ? 1 : -1];
typedef char EnTorch2State_jumpslashTimer_offset[(offsetof(EnTorch2State, jumpslashTimer) == 0x2A9B) ? 1 : -1];
typedef char EnTorch2State_jumpslashFlag_offset[(offsetof(EnTorch2State, jumpslashFlag) == 0x2A9C) ? 1 : -1];
typedef char EnTorch2State_actionState_offset[(offsetof(EnTorch2State, actionState) == 0x2A9D) ? 1 : -1];
typedef char EnTorch2State_swordJumpTimer_offset[(offsetof(EnTorch2State, swordJumpTimer) == 0x2A9E) ? 1 : -1];
typedef char EnTorch2State_counterState_offset[(offsetof(EnTorch2State, counterState) == 0x2A9F) ? 1 : -1];
typedef char EnTorch2State_dodgeRollState_offset[(offsetof(EnTorch2State, dodgeRollState) == 0x2AA0) ? 1 : -1];
typedef char EnTorch2State_staggerCount_offset[(offsetof(EnTorch2State, staggerCount) == 0x2AA1) ? 1 : -1];
typedef char EnTorch2State_staggerTimer_offset[(offsetof(EnTorch2State, staggerTimer) == 0x2AA2) ? 1 : -1];
typedef char EnTorch2State_lastSwordAnim_offset[(offsetof(EnTorch2State, lastSwordAnim) == 0x2AA3) ? 1 : -1];
typedef char EnTorch2State_alpha_offset[(offsetof(EnTorch2State, alpha) == 0x2AA4) ? 1 : -1];
typedef char EnTorch2State_attackDelay_offset[(offsetof(EnTorch2State, attackDelay) == 0x2AA5) ? 1 : -1];
typedef char EnTorch2State_rollDelay_offset[(offsetof(EnTorch2State, rollDelay) == 0x2AA6) ? 1 : -1];
typedef char EnTorch2State_size[(sizeof(EnTorch2State) == 0x2AA8) ? 1 : -1];
struct EnTorch2PlayCallbacks {
    u8 pad[0x5B90];
    void (*playerInit)(Actor*, PlayState*, void*);
    void (*playerUpdate)(Actor*, PlayState*, EnTorch2ControlInput*);
    u8 pad5B98[0x10];
    void (*playerReset)(Actor*, PlayState*, f32);
};
#endif
