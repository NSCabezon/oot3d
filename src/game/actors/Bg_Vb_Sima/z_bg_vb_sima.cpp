extern "C" {
#include "z_bg_vb_sima.hpp"

#define FLAGS 0x80400000

void BgVbSima_Init(Actor* actor, GameState* state);
void BgVbSima_Destroy(Actor* actor, GameState* state);
void BgVbSima_Update(Actor* actor, GameState* state);
void BgVbSima_Draw(Actor* actor, GameState* state);

ActorInit Bg_Vb_Sima_InitVars = {
    ACTOR_BG_VB_SIMA,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_FD,
    sizeof(BgVbSima),
    (ActorFunc)BgVbSima_Init,
    (ActorFunc)BgVbSima_Destroy,
    (ActorFunc)BgVbSima_Update,
    (ActorFunc)BgVbSima_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

static Color_RGBA8 sColors[] = {
    { 255, 255, 0, 255 },
    { 255, 10, 0, 255 },
};

void BgVbSima_Init(Actor* actor, GameState* state) {
    BgVbSima* self = (BgVbSima*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, 1);
    FUN_00372f38(actor, play, &self->skelAnimModel, 0, 0);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 0), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
}

void BgVbSima_Destroy(Actor* actor, GameState* state) {
    BgVbSima* self = (BgVbSima*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgVbSima_Draw(Actor* actor, GameState* state) {
    BgVbSima* self = (BgVbSima*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->dyna.actor.modelMtx);
    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&modelMtx);
    self->skelAnimModel->FUN_00372170(0);
}

// BossFd effect (0x4C) passed to FUN_003a3200: model pointer at 0x00, flags written at 0x36..0x43. Layout from the asm only.
typedef struct BgVbSimaModelState {
    /* 0x00 */ SkeletonAnimationModel* model;
    /* 0x04 */ f32 scale;
    /* 0x08 */ char unk_08[0x8];
    /* 0x10 */ Vec3f pos;
    /* 0x1C */ Vec3f velocity;
    /* 0x28 */ Vec3f accel;
    /* 0x34 */ u8 type;
    /* 0x35 */ u8 timer1;
    /* 0x36 */ u16 unk_36;
    /* 0x38 */ char unk_38[0x4];
    /* 0x3C */ u8 unk_3C;
    /* 0x3D */ u8 unk_3D;
    /* 0x3E */ u8 unk_3E;
    /* 0x3F */ char unk_3F[0x4];
    /* 0x43 */ u8 unk_43;
    /* 0x44 */ char unk_44[0x8];
} BgVbSimaModelState; // size = 0x4C

void FUN_0036932c(SkeletonAnimationModel* model, s32 idx);
void FUN_0037266c(SkeletonAnimationModel* model, s32 arg);

void FUN_003a3200(BgVbSimaModelState* self, s32 arg1) {
    s32 i;

    for (i = 0; i < 5; i++) {
        FUN_0036932c(self->model, i);
    }
    FUN_0037266c(self->model, arg1);
    self->unk_36 = 0xFF;
    self->unk_3E = 0xFF;
    self->unk_3D = 0xFF;
    self->unk_3C = 0xFF;
    self->unk_43 = 0;
}

__attribute__((noinline)) void FUN_003a3200(BgVbSimaModelState* self, s32 arg1);

void FUN_00365d20(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* acc, Color_RGBA8* c1, Color_RGBA8* c2, s16 scale, s32 a, s32 b);

typedef struct BgVbSimaBossFd {
    /* 0x000 */ Actor actor;
    /* 0x1A4 */ char unk_1A4[0x864 - 0x1A4];
    /* 0x864 */ void* effectCmab;
    /* 0x868 */ char unk_868[0x93E - 0x868];
    /* 0x93E */ u8 platformSignal;
    /* 0x93F */ char unk_93F[0x2924 - 0x93F];
    /* 0x2924 */ BgVbSimaModelState effects[110];
} BgVbSimaBossFd;

static void BgVbSima_SpawnEmber(BgVbSimaBossFd* bossFd, Vec3f* position, Vec3f* velocity, Vec3f* acceleration,
                                f32 scale) {
    BgVbSimaModelState* effect = bossFd->effects;
    s32 i;

    for (i = 0; i < 110; i++, effect++) {
        if (effect->type == 0 && effect->model != NULL && bossFd->effectCmab != NULL) {
            effect->type = 5;
            effect->pos = *position;
            effect->velocity = *velocity;
            effect->accel = *acceleration;
            effect->scale = scale / 1000.0f;
            effect->unk_36 = 255;
            effect->timer1 = (s16)Rand_ZeroFloat(10.0f);
            FUN_003a3200(effect, 1);
            SkeletonAnimationModel_unk_0C* m = effect->model->unk_0C;
            m->FUN_00372d94(bossFd->effectCmab);
            m->SetAnimType(1);
            break;
        }
    }
}

void BgVbSima_Update(Actor* thisx, GameState* state) {
    BgVbSima* self = (BgVbSima*)thisx;
    PlayState* play = (PlayState*)state;
    BgVbSimaBossFd* bossFd = (BgVbSimaBossFd*)thisx->parent;
    s16 i1;
    s16 i2;

    self->shakeTimer++;
    if (!Flags_GetClear(play, *(s8*)&play->roomCtx)) {
        u8 signal = bossFd->platformSignal;

        if (signal == 1) {
            Math_SmoothStepToF(&self->dyna.actor.world.pos.y, -1000.0f, 1.0f, 1.5f, 0.0f);
            self->dyna.actor.world.pos.z += 2.0f * Math_CosS(self->shakeTimer * 0x8000);
            self->dyna.actor.shape.rot.x = (s32)Math_SinS(self->shakeTimer * 0x7000) * 0x37;
            self->dyna.actor.shape.rot.z = (s32)Math_SinS(self->shakeTimer * 0x5000) * 0x37;
            Audio_PlaySoundGeneral(0x010001B3, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        } else if (signal == 2) {
            Actor_Kill(thisx);
        }
        if (bossFd->platformSignal != 0) {
            // Ember scale base as a local: makes armcc hoist `mov r8,#8` out of the outer loop (as in ROM).
            s32 emberScaleBase = 8;

            for (i1 = 0; i1 < 7; i1++) {
                Vec3f splashVel;
                Vec3f splashAcc;
                Vec3f splashPos;
                f32 edgeX;
                f32 edgeZ;

                if (Rand_ZeroOne() < 0.33f) {
                    edgeX = -80.0f;
                    edgeZ = Rand_CenteredFloat(160.0f);
                } else {
                    edgeZ = 80.0f;
                    if (Rand_ZeroOne() < 0.5f) {
                        edgeZ = -80.0f;
                    }
                    edgeX = Rand_CenteredFloat(160.0f);
                }

                splashVel.x = edgeX * 0.05f;
                splashVel.y = Rand_ZeroFloat(3.0f) + 3.0f;
                splashVel.z = edgeZ * 0.05f;

                splashAcc.y = -0.3f;
                splashAcc.x = splashVel.x;
                splashAcc.z = splashVel.z;

                splashPos.x = self->dyna.actor.world.pos.x + edgeX;
                splashPos.y = -80.0f;
                splashPos.z = self->dyna.actor.world.pos.z + edgeZ;

                FUN_00365d20(play, &splashPos, &splashVel, &splashAcc, &sColors[0], &sColors[1],
                             (s16)Rand_ZeroFloat(100.0f) + 500, 10, 20);

                for (i2 = 0; i2 < 1; i2++) {
                    Vec3f emberPos;
                    Vec3f emberVel;
                    Vec3f emberAcc;

                    emberVel.x = splashVel.x;
                    emberVel.y = Rand_ZeroFloat(5.0f);
                    emberVel.z = splashVel.z;

                    emberAcc.y = 0.4f;
                    emberAcc.x = Rand_CenteredFloat(0.5f);
                    emberAcc.z = Rand_CenteredFloat(0.5f);

                    emberPos.x = Rand_CenteredFloat(60.0f) + splashPos.x;
                    emberPos.y = Rand_ZeroFloat(40.0f) + splashPos.y;
                    emberPos.z = Rand_CenteredFloat(60.0f) + splashPos.z;

                    BgVbSima_SpawnEmber(bossFd, &emberPos, &emberVel, &emberAcc, (s16)Rand_ZeroFloat(2.0f) + emberScaleBase);
                }
            }
        }
    }
}

}
