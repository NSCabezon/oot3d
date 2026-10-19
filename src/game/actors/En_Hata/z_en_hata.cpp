extern "C" {
#include "z_en_hata.hpp"

#define FLAGS 0x00000000

void EnHata_Init(Actor* actor, GameState* state);
void EnHata_Destroy(Actor* actor, GameState* state);
void EnHata_Update(Actor* actor, PlayState* play2);
void EnHata_Draw(Actor* actor, GameState* state);

ActorInit En_Hata_InitVars = {
    ACTOR_EN_HATA,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_HATA,
    sizeof(EnHata),
    (ActorFunc)EnHata_Init,
    (ActorFunc)EnHata_Destroy,
    (ActorFunc)EnHata_Update,
    (ActorFunc)EnHata_Draw,
};

void FUN_00358ea8(ZARInfo* zar, PlayState* play, SkelAnime* skel, void* cmb, void* p, s32 b, void* jointTable,
                  void* morphTable, s32 limbCount);
void* ZAR_GetCMBByIndex(ZARInfo* zar, s32 index);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void FUN_00353020(SkelAnime* skelAnime, void* animation, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void FUN_0036932c(SkeletonAnimationModel* model, s32 idx);

void EnHata_Init(Actor* actor, GameState* state) {
    EnHata* self = (EnHata*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;
    ZARInfo* zar = &play->GetObjectStatus(actor->objBankIndex)->zarInfo;
    f32 frameCount;

    self->unk_318 = 1.0f;
    Actor_SetScale(actor, 1.0f / 75.0f);
    FUN_00358ea8(zar, play, &self->skelAnime, ZAR_GetCMBByIndex(zar, 0), actor->unk_178, 0, NULL, NULL, 0);
    frameCount = Animation_GetLastFrame(&self->skelAnime, 0);
    if (actor->params == -1) {
        FUN_0036932c(self->skelAnime.unk_28, 1);
        FUN_0036932c(self->skelAnime.unk_28, 3);
    } else if ((actor->params & 0xFF) == 0xFF) {
        FUN_0036932c(self->skelAnime.unk_28, 0);
        FUN_0036932c(self->skelAnime.unk_28, 2);
        if (actor->params & 0x8000) {
            self->unk_318 = 5.0f;
        }
    }
    FUN_00353020(&self->skelAnime, (void*)0x004D9660, 1.0f, 0.0f, frameCount, 0, 0.0f);
    DynaPolyActor_Init(&self->dyna, 0);
    if (actor->params == -1) {
        ReadCollisionHeaderFromZsi(zar->FUN_003532c0(0), &colHeader);
        self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
        self->dyna.actor.uncullZoneScale = 500.0f;
        self->dyna.actor.uncullZoneDownward = 550.0f;
        self->dyna.actor.uncullZoneForward = 2200.0f;
    } else {
        self->dyna.actor.uncullZoneScale = 1000.0f;
        self->dyna.actor.uncullZoneDownward = 1100.0f;
        self->dyna.actor.uncullZoneForward = 3300.0f;
    }
    self->invScale = 6;
    self->maxStep = 1000;
    self->minStep = 1;
    self->unk_316 = Rand_ZeroOne() * 0xFFFF;
}

void EnHata_Destroy(Actor* actor, GameState* state) {
    EnHata* self = (EnHata*)actor;
    PlayState* play = (PlayState*)state;

    SkelAnime_Free(&self->skelAnime, play);
    if (actor->params == -1) {
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    }
}

// Wind fields of PlayState.envCtx (not typed in include/z3D.hpp yet)
typedef struct EnHataEnvWind {
    /* 0x0000 */ char unk_00[0x20];
    /* 0x0020 */ Vec3s windDirection;
    /* 0x0026 */ char unk_26[0x2];
    /* 0x0028 */ f32 windSpeed;
} EnHataEnvWind;

typedef struct EnHataEnvSpeed {
    /* 0x0000 */ char unk_00[0x228];
    /* 0x0228 */ f32 windSpeed;
} EnHataEnvSpeed;

static inline s16 EnHata_Vec3fPitch(Vec3f* a, Vec3f* b) {
    return Math_Atan2S(Math_Vec3f_DistXZ(a, b), a->y - b->y);
}

static inline s16 EnHata_Vec3fYaw(Vec3f* a, Vec3f* b) {
    return Math_Vec3f_Yaw(a, b);
}

void EnHata_Update(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnHata* self = (EnHata*)thisx;
    s32 pitch;
    Vector3f zeroVec(0.0f, 0.0f, 0.0f);
    Vec3f windVec;
    f32 sin;

    FUN_00370734(&self->skelAnime);
    self->limbs[FLAGPOLE_LIMB_FLAG_1_BASE].y = -0x4000;
    self->limbs[FLAGPOLE_LIMB_FLAG_2_BASE].y = -0x4000;
    windVec.x = ((EnHataEnvWind*)((u8*)play + 0x3200))->windDirection.x;
    windVec.y = ((EnHataEnvWind*)((u8*)play + 0x3200))->windDirection.y;
    windVec.z = ((EnHataEnvWind*)((u8*)play + 0x3200))->windDirection.z;

    ((EnHataEnvSpeed*)((u8*)play + 0x3000))->windSpeed = (((EnHataEnvSpeed*)((u8*)play + 0x3000))->windSpeed > 255.0f) ? 255.0f : ((EnHataEnvSpeed*)((u8*)play + 0x3000))->windSpeed;
    ((EnHataEnvSpeed*)((u8*)play + 0x3000))->windSpeed = (((EnHataEnvSpeed*)((u8*)play + 0x3000))->windSpeed > 0.0f) ? ((EnHataEnvSpeed*)((u8*)play + 0x3000))->windSpeed : 0.0f;

    if (Rand_ZeroOne() > 0.5f) {
        self->unk_316 += 4000;
    } else {
        self->unk_316 += 2000;
    }

    sin = Math_SinS(self->unk_316) * 80.0f;
    pitch = -EnHata_Vec3fPitch((Vec3f*)&zeroVec, &windVec);
    pitch = ((s32)((15000 - pitch) * (1.0f - (((EnHataEnvSpeed*)((u8*)play + 0x3000))->windSpeed / (255.0f - sin))))) + pitch;
    Math_SmoothStepToS(&self->limbs[FLAGPOLE_LIMB_FLAG_2_HOIST_END_BASE].y, pitch, self->invScale, self->maxStep,
                       self->minStep);
    self->limbs[FLAGPOLE_LIMB_FLAG_1_HOIST_END_BASE].y = self->limbs[FLAGPOLE_LIMB_FLAG_2_HOIST_END_BASE].y;
    self->limbs[FLAGPOLE_LIMB_FLAG_2_HOIST_END_BASE].z = -EnHata_Vec3fYaw((Vec3f*)&zeroVec, &windVec);
    self->limbs[FLAGPOLE_LIMB_FLAG_1_HOIST_END_BASE].z = self->limbs[FLAGPOLE_LIMB_FLAG_2_HOIST_END_BASE].z;
    self->skelAnime.playSpeed = (Rand_ZeroFloat(1.25f) + 2.75f) * (((EnHataEnvSpeed*)((u8*)play + 0x3000))->windSpeed / 255.0f);
}

void FUN_003625f8(MTX34* mtx, Vec3f* axis, f32 angle);
void FUN_0036c174(MTX34* dst, MTX34* src, MTX34* mtx);
s32 EnHata_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);

// Draw: 33/50 words; ROM re-copies scale (vmov s2,s0) before each column-0 product, ~60 helper/Vec3f/param variants failed.
#if 0
static inline void EnHata_MtxScale(MTX34* m, f32 x, f32 y, f32 z) {
    m->data[0][0] *= z;
    m->data[1][0] *= z;
    m->data[2][0] *= z;
    m->data[0][1] *= y;
    m->data[1][1] *= y;
    m->data[2][1] *= y;
    m->data[0][2] *= x;
    m->data[1][2] *= x;
    m->data[2][2] *= x;
}

void EnHata_Draw(Actor* actor, GameState* state) {
    EnHata* self = (EnHata*)actor;
    MTX34 mtx;
    f32 scale;
    f32 scaleY;

    MTX34CopyAsm(&mtx, &self->dyna.actor.modelMtx);
    scale = self->unk_318;
    scaleY = scale + 0.1f;
    EnHata_MtxScale(&mtx, scale, scaleY, scale);
    SkelAnime_DrawOpa(&self->skelAnime, &mtx, EnHata_OverrideLimbDraw, NULL, actor, 0);
}
#endif

GLOBAL_ASM("asm/EnHata_Draw.s")

static inline void EnHata_Vec3fSet(Vec3f* v, f32 x, f32 y, f32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

// Parameter order (x, z, y) picks the ROM's VFP registers for the second axis.
static inline void EnHata_Vec3fSetXZY(Vec3f* v, f32 x, f32 z, f32 y) {
    v->x = x;
    v->y = y;
    v->z = z;
}

static inline void EnHata_Rotate(MTX34* mtx, Vec3f* axis, f32 angle) {
    MTX34 rot;

    FUN_003625f8(&rot, axis, angle);
    FUN_0036c174(mtx, mtx, &rot);
}

s32 EnHata_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnHata* self = (EnHata*)thisx;

    if (limbIndex == FLAGPOLE_LIMB_FLAG_1_BASE || limbIndex == FLAGPOLE_LIMB_FLAG_2_BASE ||
        limbIndex == FLAGPOLE_LIMB_FLAG_1_HOIST_END_BASE || limbIndex == FLAGPOLE_LIMB_FLAG_2_HOIST_END_BASE) {
        Vec3f axis;
        Vec3f* p = &axis;

        EnHata_Vec3fSet(p, 1.0f, 0.0f, 0.0f);
        f32 angle = self->limbs[limbIndex].z * 0.000095873802f;
        EnHata_Rotate(mtx, p, angle);
        EnHata_Vec3fSetXZY(p, 0.0f, 1.0f, 0.0f);
        EnHata_Rotate(mtx, p, self->limbs[limbIndex].y * 0.000095873802f);
    }
    return 0;
}

}
