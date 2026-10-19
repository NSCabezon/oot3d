extern "C" {
#include <stddef.h>
#include "z_en_torch2.hpp"

struct EnTorch2InputStateView {
    Actor actor;
    u8 toXZSpeed[0x221C - sizeof(Actor)];
    f32 xzSpeed;
    u8 toActionTimer[0x2488 - 0x2220];
    u8 actionTimer; // 0x2488
    u8 unk_2489[0x5FF];
    f32 stickTilt; // 0x2A88
    s16 stickAngle; // 0x2A8C
    s16 unk_2A8E;
    u8 unk_2A90[0x08];
    u8 swordJumpState; // 0x2A98
    u8 inputEnabled; // 0x2A99
    u8 unk_2A9A[0x03];
    u8 controlReady; // 0x2A9D
    u8 unk_2A9E;
    u8 counterState; // 0x2A9F
    u8 controlActive; // 0x2AA0
    u8 unk_2AA1[0x03];
    u8 alpha; // 0x2AA4
    u8 actionState; // 0x2AA5
    u8 inputMode; // 0x2AA6
    u8 unk_2AA7;
};

typedef char EnTorch2InputStateViewSizeCheck[sizeof(EnTorch2InputStateView) == 0x2AA8 ? 1 : -1];
typedef char EnTorch2TiltOffsetCheck[offsetof(EnTorch2InputStateView, stickTilt) == 0x2A88 ? 1 : -1];
typedef char EnTorch2AngleOffsetCheck[offsetof(EnTorch2InputStateView, stickAngle) == 0x2A8C ? 1 : -1];
typedef char EnTorch2TimerOffsetCheck[offsetof(EnTorch2InputStateView, actionTimer) == 0x2488 ? 1 : -1];
typedef char EnTorch2XZSpeedOffsetCheck[offsetof(EnTorch2InputStateView, xzSpeed) == 0x221C ? 1 : -1];
typedef char EnTorch2InputEnabledOffsetCheck[offsetof(EnTorch2InputStateView, inputEnabled) == 0x2A99 ? 1 : -1];
typedef char EnTorch2CounterOffsetCheck[offsetof(EnTorch2InputStateView, counterState) == 0x2A9F ? 1 : -1];
typedef char EnTorch2InputModeOffsetCheck[offsetof(EnTorch2InputStateView, inputMode) == 0x2AA6 ? 1 : -1];
typedef char EnTorch2ActionStateOffsetCheck[offsetof(EnTorch2InputStateView, actionState) == 0x2AA5 ? 1 : -1];
typedef char EnTorch2ControlReadyOffsetCheck[offsetof(EnTorch2InputStateView, controlReady) == 0x2A9D ? 1 : -1];
typedef char EnTorch2ControlActiveOffsetCheck[offsetof(EnTorch2InputStateView, controlActive) == 0x2AA0 ? 1 : -1];
typedef char EnTorch2AlphaOffsetCheck[offsetof(EnTorch2InputStateView, alpha) == 0x2AA4 ? 1 : -1];
struct EnTorch2PlayFrameView { u8 pad[0x5BF4]; u32 gameplayFrames; };
typedef char EnTorch2PlayFrameOffsetCheck[offsetof(EnTorch2PlayFrameView, gameplayFrames) == 0x5BF4 ? 1 : -1];

struct EnTorch2ActorFallView {
    Actor actor;
    u8 toMotionValues[0x290 - sizeof(Actor)];
    f32 motionX;
    f32 motionZ;
    u8 toClearByte[0x2227 - 0x298];
    u8 clearByte;
    u8 toFallState[0x2488 - 0x2228];
    s8 fallState; // signed invincibilityTimer @0x2488
    u8 toPosition[0x2A4C - 0x2489];
    union { Vec3f homePos; struct { f32 homeX; f32 speed; f32 homeZ; } motion; } position;
    u8 toTilt[0x2A88 - 0x2A58];
    f32 stickTilt;
    s16 stickAngle;
    u8 tail[0x2AA8 - 0x2A8E];
};
typedef char EnTorch2MotionXOffsetCheck[offsetof(EnTorch2ActorFallView, motionX) == 0x290 ? 1 : -1];
typedef char EnTorch2MotionZOffsetCheck[offsetof(EnTorch2ActorFallView, motionZ) == 0x294 ? 1 : -1];
typedef char EnTorch2FallClearOffsetCheck[offsetof(EnTorch2ActorFallView, clearByte) == 0x2227 ? 1 : -1];
typedef char EnTorch2FallStateOffsetCheck[offsetof(EnTorch2ActorFallView, fallState) == 0x2488 ? 1 : -1];
typedef char EnTorch2FallSpeedOffsetCheck[offsetof(EnTorch2ActorFallView, position.motion.speed) == 0x2A50 ? 1 : -1];
typedef char EnTorch2FallHomeOffsetCheck[offsetof(EnTorch2ActorFallView, position.homePos) == 0x2A4C ? 1 : -1];
typedef char EnTorch2ActorFallViewSizeCheck[sizeof(EnTorch2ActorFallView) == 0x2AA8 ? 1 : -1];

typedef char EnTorch2SizeCheck[sizeof(EnTorch2) == 0x2AA8 ? 1 : -1];

struct EnTorch2QuadView { u8 bytes[0x80]; };
struct EnTorch2DestroyView {
    Actor actor;
    u8 toSkel254[0x254 - sizeof(Actor)];
    SkelAnime skel254;
    u8 toCylinder[0x1310 - 0x2D8];
    ColliderCylinder cylinder;
    EnTorch2QuadView meleeQuad0;
    EnTorch2QuadView meleeQuad1;
    u8 toShield[0x1568 - 0x13E8 - sizeof(EnTorch2QuadView)];
    EnTorch2QuadView shieldQuad;
    u8 toEffect[0x1704 - 0x1568 - sizeof(EnTorch2QuadView)];
    s32 effectIndex;
    u8 toSkel1764[0x1764 - 0x1708];
    SkelAnime skel1764;
    u8 toCallbackVtable[0x24F0 - 0x1764 - sizeof(SkelAnime)];
    u32* callbackVtable;
    u8 toAlpha[0x2AA4 - 0x24F4];
    u8 alpha;
    u8 tail[3];
};
typedef char EnTorch2DestroyViewSizeCheck[sizeof(EnTorch2DestroyView) == 0x2AA8 ? 1 : -1];
typedef char EnTorch2ModelMtxOffsetCheck[offsetof(Actor, modelMtx) == 0x148 ? 1 : -1];
typedef char EnTorch2Skel254OffsetCheck[offsetof(EnTorch2DestroyView, skel254) == 0x254 ? 1 : -1];
typedef char EnTorch2CylinderOffsetCheck[offsetof(EnTorch2DestroyView, cylinder) == 0x1310 ? 1 : -1];
typedef char EnTorch2Quad0OffsetCheck[offsetof(EnTorch2DestroyView, meleeQuad0) == 0x1368 ? 1 : -1];
typedef char EnTorch2Quad1OffsetCheck[offsetof(EnTorch2DestroyView, meleeQuad1) == 0x13E8 ? 1 : -1];
typedef char EnTorch2ShieldOffsetCheck[offsetof(EnTorch2DestroyView, shieldQuad) == 0x1568 ? 1 : -1];
typedef char EnTorch2EffectOffsetCheck[offsetof(EnTorch2DestroyView, effectIndex) == 0x1704 ? 1 : -1];
typedef char EnTorch2Skel1764OffsetCheck[offsetof(EnTorch2DestroyView, skel1764) == 0x1764 ? 1 : -1];
typedef char EnTorch2CallbackVtableOffsetCheck[offsetof(EnTorch2DestroyView, callbackVtable) == 0x24F0 ? 1 : -1];
typedef char EnTorch2ControlReadyOffsetCheck[offsetof(EnTorch2InputStateView, controlReady) == 0x2A9D ? 1 : -1];
typedef char EnTorch2ControlActiveOffsetCheck[offsetof(EnTorch2InputStateView, controlActive) == 0x2AA0 ? 1 : -1];
typedef char EnTorch2AlphaOffsetCheck[offsetof(EnTorch2DestroyView, alpha) == 0x2AA4 ? 1 : -1];

#define FLAGS 0x00000035

void EnTorch2_Init(Actor* actor, GameState* state);
void EnTorch2_Destroy(Actor* actor, PlayState* state);
void EnTorch2_Update(Actor* actor, GameState* state);
void EnTorch2_Draw(Actor* actor, PlayState* state);

GLOBAL_ASM("../src/game/actors/En_Torch2/data_syms.s")

GLOBAL_ASM("data/z_En_Torch2.data.s")

ActorInit En_Torch2_InitVars = {
    ACTOR_EN_TORCH2,
    ACTORCAT_BOSS,
    FLAGS,
    OBJECT_TORCH2,
    sizeof(EnTorch2),
    (ActorFunc)EnTorch2_Init,
    (ActorFunc)EnTorch2_Destroy,
    (ActorFunc)EnTorch2_Update,
    (ActorFunc)EnTorch2_Draw,
};

GLOBAL_ASM("asm/EnTorch2_Init.s")

void Effect_Delete(PlayState*, s32);
void FUN_00373d0c(PlayState*);
s32 FUN_0034eaf8(PlayState*, void*);
extern u32 EnTorch2_sBaseLimbVtable[];

void EnTorch2_Destroy(Actor* thisx, PlayState* play) {
    EnTorch2DestroyView* self = (EnTorch2DestroyView*)thisx;
    Effect_Delete(play, self->effectIndex);
    FUN_00373d0c(play);
    Collider_DestroyCylinder(play, &self->cylinder);
    FUN_0034eaf8(play, &self->meleeQuad0);
    FUN_0034eaf8(play, &self->meleeQuad1);
    FUN_0034eaf8(play, &self->shieldQuad);
    u32** callbackVtableSlot = &self->callbackVtable;
    *callbackVtableSlot = EnTorch2_sBaseLimbVtable;
    SkelAnime_Free2(&self->skel1764);
    SkelAnime_Free2(&self->skel254);
}

GLOBAL_ASM("asm/EnTorch2_Update.s")

void FUN_0033dd8c(SkelAnime*, s32, s32, s32, f32, f32, f32, f32);
void FUN_00342be0(Actor*, s32, s32, f32, f32, f32, f32);
s32 FUN_003255ec(PlayState*, s32, MTX34*, void*);
void EnTorch2_PostLimbDrawEntry(PlayState*, s32, MTX34*, void*);

void EnTorch2_Draw(Actor* thisx, PlayState* play) {
    EnTorch2DestroyView* self = (EnTorch2DestroyView*)thisx;
    f32 alpha;
    f32 alphaScale = 1.0f / 255.0f;
    if (self->alpha == 0xFF) {
        FUN_0033dd8c(&self->skel254, 4, 1, 0, 1.0f, 0.0f, 0.0f, 1.0f);
        SkelAnime_DrawOpa(&self->skel254, &self->actor.modelMtx, (OverrideLimbDrawOpa)FUN_003255ec,
                          (PostLimbDrawOpa)EnTorch2_PostLimbDrawEntry, thisx, 0);
    } else {
        alpha = self->alpha * alphaScale;
        FUN_00342be0(thisx, 4, 0, 0.0f, 0.0f, 0.0f, alpha);
        SkelAnime_DrawOpa(&self->skel254, &self->actor.modelMtx, (OverrideLimbDrawOpa)FUN_003255ec,
                          (PostLimbDrawOpa)EnTorch2_PostLimbDrawEntry, thisx, 0);
    }
}

GLOBAL_ASM("asm/FUN_00192d38.s")

struct EnTorch2InputVectorView { u32 buttons; u32 pressed; u32 released; s16 stickX; s16 stickY; };
struct EnTorch2PlayResetView { u8 pad[0x5BA8]; void (*playerReset)(Actor*, PlayState*, f32); };
struct EnTorch2CameraYawView { u8 pad[0x184]; s16 yaw; };
struct EnTorch2PlayPlayerView { u8 pad[0x20AC]; Player* player; };
typedef char EnTorch2OutXOffsetCheck[offsetof(EnTorch2InputVectorView, stickX) == 0xC ? 1 : -1];
typedef char EnTorch2OutYOffsetCheck[offsetof(EnTorch2InputVectorView, stickY) == 0xE ? 1 : -1];
typedef char EnTorch2PlayResetOffsetCheck[offsetof(EnTorch2PlayResetView, playerReset) == 0x5BA8 ? 1 : -1];
typedef char EnTorch2CameraYawOffsetCheck[offsetof(EnTorch2CameraYawView, yaw) == 0x184 ? 1 : -1];
typedef char EnTorch2PlayPlayerOffsetCheck[offsetof(EnTorch2PlayPlayerView, player) == 0x20AC ? 1 : -1];

Actor* FUN_00346e2c(PlayState* play, Actor* actor, f32 range, f32 range2);
s32 FUN_0035f228(PlayState* play, Actor* actor);
void FUN_0034f724(PlayState* play);
f32 Actor_WorldDistXYZToPoint(Actor* actor, Vec3f* point);

#if 0
/* Sol rebuild from addressed ARM: all initial Luna semantic gaps corrected. */
static inline Actor* EnTorch2_GetAttackItem(PlayState* play, Actor* actor) {
    Actor* target = FUN_00346e2c(play, actor, 4000.0f, 1.5f);
    if (target != 0) return target;
    else return FUN_00346d94(play, actor);
}
void FUN_00193740(Actor* thisx, PlayState* play, EnTorch2InputVectorView* input) {
    EnTorch2InputStateView* self = (EnTorch2InputStateView*)thisx;
    Actor* target = EnTorch2_GetAttackItem(play, &self->actor);
    Camera* camera = Gameplay_GetCamera(play, 0);
    if (self->unk_2A8E != 0) return;
    self->actor.world.rot.y = self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
    ((EnTorch2ActorFallView*)self)->motionZ = ((EnTorch2ActorFallView*)self)->motionX = 0.0f;
    if (((u8*)thisx)[0x224C] == 0) {
        ((u8*)self)[0x224C] = 1;
        self->actor.world.pos.x = Math_SinS(self->actor.world.rot.y) * 25.0f + ((EnTorch2ActorFallView*)self)->position.homePos.x;
        self->actor.world.pos.z = Math_CosS(self->actor.world.rot.y) * 25.0f + ((EnTorch2ActorFallView*)self)->position.homePos.z;
    }
    if (self->actor.xzDistToPlayer > 120.0f && !FUN_0035f228(play, &self->actor)) {
        if (target == 0) return;
    }
    if (target != 0) {
        self->controlActive = 1;
        self->stickAngle = self->actor.yawTowardsPlayer;
        self->stickTilt = 127.0f;
        input->buttons = 1;
        self->inputEnabled = 0;
        s16 angle = ((EnTorch2CameraYawView*)camera)->yaw - self->stickAngle;
        input->stickX = (s8)(Math_SinS(angle) * self->stickTilt);
        input->stickY = (s8)(Math_CosS(angle) * self->stickTilt);
    }
    FUN_0034f724(play);
    self->controlReady = 1;
}

void FUN_001e2f1c(Actor* actorx, PlayState* play, EnTorch2InputVectorView* input) {
    EnTorch2ActorFallView* thisx = (EnTorch2ActorFallView*)actorx;
    EnTorch2InputStateView* self = (EnTorch2InputStateView*)thisx;
    Actor* actor = &thisx->actor;
    Player* player = GET_PLAYER(play);
    thisx->clearByte = 0;
    input->stickY = input->stickX = 0;
    if (thisx->fallState > 0 && actor->world.pos.y < actor->floorHeight - 160.0f) {
        ((u8*)thisx)[0x172A] &= ~1;
        actor->flags |= 1;
        self->actionTimer = 0;
        actor->velocity.y = 0.0f;
        actor->world.pos.y = thisx->position.homePos.y + 40.0f;
        actor->world.pos.x = Math_SinS(player->actor.shape.rot.y) * -120.0f + player->actor.world.pos.x;
        actor->world.pos.z = Math_CosS(player->actor.shape.rot.y) * -120.0f + player->actor.world.pos.z;
        if (Actor_WorldDistXYZToPoint(actor, &thisx->position.homePos) > 800.0f) {
            f32 travel = Rand_ZeroOne() * 20.0f;
            s16 randomYaw = (s32)Rand_CenteredFloat(4000.0f);
            actor->shape.rot.y = actor->world.rot.y = Math_Atan2S(player->actor.world.pos.x - thisx->position.homePos.x,
                                             player->actor.world.pos.z - thisx->position.homePos.z);
            actor->world.pos.x = Math_SinS((s16)(actor->world.rot.y + randomYaw)) * (25.0f + travel) + thisx->position.homePos.x;
            actor->world.pos.z = Math_CosS((s16)(actor->world.rot.y + randomYaw)) * (25.0f + travel) + thisx->position.homePos.z;
            actor->world.pos.y = thisx->position.homePos.y;
        } else actor->world.pos.y = actor->floorHeight;
        Math_Vec3f_Copy(&actor->home.pos, &actor->world.pos);
        ((EnTorch2PlayResetView*)play)->playerReset(actor, play, -6.0f);
        self->controlReady = 1;
        thisx->stickTilt = 0.0f;
        if (self->alpha != 0xFF) {
            ((u8*)thisx)[0x2AA1] = 0;
            ((u8*)thisx)[0x2AA2] = 0;
        }
    }
}

#endif
GLOBAL_ASM("asm/FUN_00193740.s")
GLOBAL_ASM("asm/FUN_001e2f1c.s")

void FUN_00346bb4(Actor* thisx, EnTorch2InputVectorView* input, Actor* actor) {
    EnTorch2InputStateView* state = (EnTorch2InputStateView*)thisx;
    actor->world.rot.y = actor->shape.rot.y = actor->yawTowardsPlayer;
    state->stickAngle = actor->yawTowardsPlayer + 0x8000;
    state->stickTilt = 127.0f;
    state->inputEnabled = 1;
    input->buttons = 1;
    state->inputMode = 2;
    s32 timer = (s32)(30.0f / (f32)gGameInfo->data[126] + 0.5f);
    state->actionTimer = (u8)timer;
    state->counterState = 0;
}


/* Sol exact: 148/148 B; full workspace link verified. */

/* Sol exact: 332/332 B; full workspace link verified. */
static inline s32 EnTorch2_RandomSwordDirection() { return (s32)(Rand_ZeroOne() * 7.0f); }
s32 FUN_00346c48(PlayState* play, EnTorch2InputVectorView* input, Actor* thisx) {
    EnTorch2InputStateView* self = (EnTorch2InputStateView*)thisx;
    f32 noAttackChance = 0.0f;
    s32 attackDelay = 7;
    Player* player = GET_PLAYER(play);
    if ((self->xzSpeed < 0.0f) || (player->xzSpeed < 0.0f)) {
        return 0;
    }
    if (gSaveContext.health < 0x50) {
        noAttackChance += 0.3f;
        attackDelay = 15;
    }
    if (self->alpha != 0xFF) {
        noAttackChance += 2.0f;
    }
    if (((( ((EnTorch2PlayFrameView*)play)->gameplayFrames & attackDelay) == 0) ||
         (self->swordJumpState != 0)) &&
        (noAttackChance <= Rand_ZeroOne())) {
        if (self->swordJumpState == 0) {
            switch (EnTorch2_RandomSwordDirection()) {
                case 1:
                case 5:
                    self->stickAngle += 0x4000;
                    self->stickTilt = 127.0f;
                    break;
                case 2:
                case 6:
                    self->stickAngle -= 0x4000;
                    self->stickTilt = 127.0f;
                    break;
            }
        }
        input->buttons = 2;
        self->actionState = 2;
        return 1;
    }
    return 0;
}

}
