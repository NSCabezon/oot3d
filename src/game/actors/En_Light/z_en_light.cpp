extern "C" {
#include "z_en_light.hpp"

#define FLAGS 0x00000000

void EnLight_Init(Actor* actor, GameState* state);
void EnLight_Destroy(Actor* thisx, PlayState* play);
void EnLight_Update(Actor* thisx, PlayState* play);
void EnLight_Draw(Actor* thisx, PlayState* play);
void MTX34CopyAsm(MTX34* dst, MTX34* src);
void Matrix_RotateY(MTX34* mtx, s32 mode, f32 angle);
void SkeletonAnimationModel_FUN_003721e0(void* model, MTX34* mtx) __asm("_ZN22SkeletonAnimationModel12FUN_003721e0EP5MTX34");
void SkeletonAnimationModel_FUN_00372170(void* model, s32 arg) __asm("_ZN22SkeletonAnimationModel12FUN_00372170Ei");
u32 PauseContext_GetState(void);
#define BINANG_TO_RAD_LOCAL(x) ((x) * (3.14159265f / 32768.0f))
void LightContext_RemoveLight(PlayState* play, void* lightCtx, void* lightNode);
void Lights_PointSetColorAndRadius(void* lightInfo, u8 r, u8 g, u8 b, s16 radius, s32 arg6);
s32 Camera_GetCamDirYaw(void* cam);
// GET_ACTIVE_CAM(play): play->cameraPtrs[play->activeCamId] (raw offsets, as in the ROM)
static inline void* EnLight_GetActiveCam(PlayState* play) {
    return *(void**)((u8*)play + 0xA54 + *(s16*)((u8*)play + 0xA64) * 4);
}

typedef struct FlameParams {
    /* 0x00 */ Color_RGBA8 primColor;
    /* 0x04 */ Color_RGB8 envColor;
    /* 0x07 */ u8 scale;
    /* 0x08 */ u8 unk_08[0x4]; // 3DS table stride is 12 bytes
} FlameParams;

extern FlameParams EnLight_sFlameParams[];


GLOBAL_ASM("data/z_En_Light.data.s")

// Model at +0x1A4: real virtual call, slot 1. _ZTI12EnLightModel is an EQU in syms.s so armlink VFE
// (--vfemode=force, --no_rtti_data) knows the call class (same recipe as Item_Ocarina / Item_Etcetera).
extern "C++" {
struct EnLightModel {
    virtual void unk_vf0();
    virtual void Destroy();
};
}

GLOBAL_ASM("../src/game/actors/En_Light/syms.s")

ActorInit En_Light_InitVars = {
    ACTOR_EN_LIGHT,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnLight),
    (ActorFunc)EnLight_Init,
    (ActorFunc)EnLight_Destroy,
    (ActorFunc)EnLight_Update,
    (ActorFunc)EnLight_Draw,
};

GLOBAL_ASM("asm/EnLight_Init.s")

void EnLight_Destroy(Actor* thisx, PlayState* play) {
    EnLight* self = (EnLight*)thisx;

    LightContext_RemoveLight(play, (void*)((u8*)play + 0xA70), self->lightNode);
    if (self->model != NULL) {
        ((EnLightModel*)self->model)->Destroy();
        self->model = NULL;
    }
}

// Update: draft, not matched yet (13 words off: radius/scale schedule and timer register). Stays asm.
#if 0
// Update draft: 3DS flame, light radius/colour scaled from the table; UpdatePosRot inlined.
void EnLight_Update(Actor* thisx, PlayState* play) {
    f32 intensity;
    FlameParams* flameParams;
    s16 radius;
    EnLight* self = (EnLight*)thisx;

    flameParams = &EnLight_sFlameParams[self->actor.params & 0xF];
    intensity = (Rand_ZeroOne() * 0.5f) + 0.5f;
    if (!(self->actor.params & 0x1000)) {
        radius = (self->actor.params < 0) ? 100 : 300;
        Lights_PointSetColorAndRadius(&self->lightInfo, (u8)(flameParams->primColor.r * intensity),
                                      (u8)(flameParams->primColor.g * intensity),
                                      (u8)(flameParams->primColor.b * intensity),
                                      (s16)(radius * ((f32)flameParams->scale * (1.0f / 75.0f))), 0);
    }
    self->actor.shape.rot.y = Camera_GetCamDirYaw(EnLight_GetActiveCam(play)) + 0x8000;
    if (self->actor.parent != NULL) {
        Math_Vec3f_Copy(&self->actor.world.pos, &self->actor.parent->world.pos);
        self->actor.world.pos.y += 17.0f;
    }
    self->timer++;

    if (self->actor.params >= 0) {
        Audio_PlayActorSound2(&self->actor, 0x01000188);
    }
}
#endif
GLOBAL_ASM("asm/EnLight_Update.s")


// 3DS draw (matrix from modelMtx, rotate by camera yaw, animation speed 2.0 or 0.0 when paused).
void EnLight_Draw(Actor* thisx, PlayState* play) {
    EnLight* self = (EnLight*)thisx;
    MTX34 mtx;
    f32 speed;

    MTX34CopyAsm(&mtx, &self->actor.modelMtx);
    s32 yaw = Camera_GetCamDirYaw(EnLight_GetActiveCam(play));
    Matrix_RotateY(&mtx, 1, BINANG_TO_RAD_LOCAL((s16)((yaw - thisx->shape.rot.y) + 0x8000)));
    if (self->actor.params & 1) {
        Matrix_RotateY(&mtx, 1, 3.14159265f);
    }
    speed = PauseContext_GetState() == 0 ? 2.0f : 0.0f;
    ((f32*)self->unk_1A8)[3] = speed;
    SkeletonAnimationModel_FUN_003721e0(self->model, &mtx);
    {
        void* model = self->model;

        *((u8*)model + 0xAC) = 1;
    }
    SkeletonAnimationModel_FUN_00372170(self->model, 0);
}

GLOBAL_ASM("asm/EnLight_UpdateSwitch.s")

}
