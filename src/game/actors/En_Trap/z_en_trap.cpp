extern "C" {
#include "z_en_trap.hpp"

#define FLAGS 0x00000010

void EnTrap_Init(Actor* thisx, GameState* state);
void EnTrap_Destroy(Actor* thisx, PlayState* play);
void EnTrap_Update(Actor* actor, GameState* state);
void EnTrap_Draw(Actor* thisx, PlayState* play);

GLOBAL_ASM("data/z_En_Trap.data.s")
GLOBAL_ASM("../src/game/actors/En_Trap/data_syms.s")

extern ColliderCylinderInit EnTrap_sCylinderInit;

ActorInit En_Trap_InitVars = {
    ACTOR_EN_TRAP,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_TRAP,
    sizeof(EnTrap),
    (ActorFunc)EnTrap_Init,
    (ActorFunc)EnTrap_Destroy,
    (ActorFunc)EnTrap_Update,
    (ActorFunc)EnTrap_Draw,
};

void EnTrap_Init(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnTrap* self = (EnTrap*)thisx;
    f32 trapDist;
    f32 trapSpeed;
    s16 zSpeed;
    s16 xSpeed;

    self->upperParams = (thisx->params >> 8) & 0xFF;
    thisx->params &= 0xFF;
    Actor_SetScale(thisx, 0.1f);
    thisx->gravity = -2.0f;
    FUN_00372f38(thisx, play, &self->model, 1, 0);
    if (thisx->params & 0x10) {
        thisx->speedXZ = self->moveSpeedForwardBack.z = self->upperParams & 0xF;
        Audio_PlayActorSound2(thisx, 0x010001d4);
    } else if (thisx->params & 0x20) {
        self->genericVar2 = (self->upperParams & 0xF) * 40.0f;
        self->upperParams = ((self->upperParams & 0xF0) + 0x10) << 5;
        thisx->world.pos.x = thisx->home.pos.x + (Math_SinS(0) * self->genericVar2);
        thisx->world.pos.z = thisx->home.pos.z + (Math_CosS(0) * self->genericVar2);
    } else {
        if (self->upperParams != 0) {
            trapDist = (self->upperParams >> 4) * 40;
            trapSpeed = (self->upperParams & 0xF);
        } else {
            trapDist = 200.0f;
            trapSpeed = 10.0f;
            thisx->params = 0xF;
        }
        Actor_UpdateBgCheckInfo(play, thisx, 10.0f, 20.0f, 20.0f, 0x1D);
        thisx->home.pos = thisx->world.pos;
        self->targetPosLeft.x = thisx->world.pos.x + (Math_CosS(thisx->world.rot.y) * trapDist);
        self->targetPosLeft.z = thisx->world.pos.z - (Math_SinS(thisx->world.rot.y) * trapDist);
        self->targetPosRight.x = thisx->world.pos.x + (Math_CosS(thisx->world.rot.y + 0x8000) * trapDist);
        self->targetPosRight.z = thisx->world.pos.z - (Math_SinS(thisx->world.rot.y + 0x8000) * trapDist);
        self->targetPosFwd.x = thisx->world.pos.x + (Math_SinS(thisx->world.rot.y) * trapDist);
        self->targetPosFwd.z = thisx->world.pos.z + (Math_CosS(thisx->world.rot.y) * trapDist);
        self->targetPosBack.x = thisx->world.pos.x + (Math_SinS(thisx->world.rot.y + 0x8000) * trapDist);
        self->targetPosBack.z = thisx->world.pos.z + (Math_CosS(thisx->world.rot.y + 0x8000) * trapDist);

        zSpeed = trapSpeed * Math_CosS(thisx->world.rot.y);
        xSpeed = trapSpeed * Math_SinS(thisx->world.rot.y);
        zSpeed = ABS(zSpeed);
        xSpeed = ABS(xSpeed);
        self->moveSpeedLeftRight.x = self->moveSpeedForwardBack.z = zSpeed;
        self->moveSpeedLeftRight.z = self->moveSpeedForwardBack.x = xSpeed;
    }
    thisx->focus.pos = thisx->world.pos;
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, thisx, &EnTrap_sCylinderInit);
    ActorShape_Init(&thisx->shape, 0.0f, ActorShadow_DrawCircle, 0.0f);
    thisx->targetMode = 3;
    thisx->colChkInfo.mass = 0xFF;
}

void EnTrap_Destroy(Actor* thisx, PlayState* play) {
    EnTrap* self = (EnTrap*)thisx;

    Collider_DestroyCylinder(play, &self->collider);
    FUN_003508b8(thisx, self->model, 0);
}

GLOBAL_ASM("asm/EnTrap_Update.s")

void EnTrap_Draw(Actor* thisx, PlayState* play) {
    EnTrap* self = (EnTrap*)thisx;

    FUN_00357fd0(GET_PLAYER(play), thisx->unk_178, &thisx->world.pos);
    {
        SkeletonAnimationModel* model = self->model;
        model->unk_AC = 1;
    }
    self->model->FUN_003721e0(&thisx->modelMtx);
    self->model->FUN_00372170(0);
}

// Draft (15/16 words off, far): ROM keeps r3 (pValue) and s8 (target) live across
// bl Math_SmoothStepToF, i.e. armcc saw the callee's register usage (same TU or
// analysed). Opaque extern callee forces callee-saved copies (push {r4,lr}; vpush {d8}).
// Math_SmoothStepToF (0x36e168, still asm) only uses s0-s6/r0-r2, hence ROM's s7/s8/r3: prerequisite = Math_SmoothStepToF as C in the same TU.
#if 0
f32 FUN_0035e6a0(f32* pValue, f32 target, f32 fraction, f32 step, f32 minStep, f32 threshold) {
    f32 ret = Math_SmoothStepToF(pValue, target, fraction, step, minStep);

    if (ABS(ret) < threshold) {
        *pValue = target;
        return 0.0f;
    }
    return ret;
}
#endif

GLOBAL_ASM("asm/FUN_0035e6a0.s")

}
