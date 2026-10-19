#if 0
/* Sol best ARM-correct candidate: 416/420 B, 20 words; original ASM remains active.
 * Main-file drafts preserve earlier scheduling probes. */
extern "C" {
#include "z_en_torch2_state.hpp"
Actor* FUN_00346e2c(PlayState*, Actor*, f32, f32);
s32 FUN_0035f228(PlayState*, Actor*);
void FUN_0034f724(PlayState*);
struct EnTorch2WaitCameraView { u8 pad[0x184]; s16 yaw; };
struct EnTorch2WaitInitView { u8 pad[0x224C]; u8 ready; };
static inline Actor* EnTorch2_GetAttackItem(PlayState* play, Actor* actor) {
    Actor* target = FUN_00346e2c(play, actor, 4000.0f, 1.5f);
    if (target != 0) return target;
    else return FUN_00346d94(play, actor);
}
void FUN_00193740(Actor* thisx, PlayState* play, EnTorch2ControlInput* input) {
    EnTorch2State* self = (EnTorch2State*)thisx;
    Actor* target = EnTorch2_GetAttackItem(play, thisx);
    Camera* camera = Gameplay_GetCamera(play, 0);
    if (self->controlFreeze == 0) {
        self->actor.world.rot.y = self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
        self->skelAnime.curFrame = 0.0f;
        self->skelAnime.playSpeed = 0.0f;
        if (((EnTorch2WaitInitView*)thisx)->ready == 0) {
            ((EnTorch2WaitInitView*)self)->ready = 1;
            self->actor.world.pos.x = Math_SinS(self->actor.world.rot.y) * 25.0f + self->spawnPoint.x;
            self->actor.world.pos.z = Math_CosS(self->actor.world.rot.y) * 25.0f + self->spawnPoint.z;
        }
        if (self->actor.xzDistToPlayer <= 120.0f || FUN_0035f228(play, &self->actor) || target != 0) {
            if (target != 0) {
                self->dodgeRollState = 1;
                self->stickAngle = self->actor.yawTowardsPlayer;
                self->stickTilt = 127.0f;
                input->current.buttons = 1;
                self->zTargetFlag = 0;
                s16 angle = ((EnTorch2WaitCameraView*)camera)->yaw - self->stickAngle;
                input->current.stickX = (s8)(Math_SinS(angle) * self->stickTilt);
                input->current.stickY = (s8)(Math_CosS(angle) * self->stickTilt);
            }
            FUN_0034f724(play);
            self->actionState = 1;
        }
    }
}
}

#endif
