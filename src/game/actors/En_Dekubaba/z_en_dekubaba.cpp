extern "C" {
#include "z_en_dekubaba.hpp"
#include "mtx34_trans.hpp"

#define FLAGS 0x00000005

void EnDekubaba_Init(Actor* thisx, PlayState* play);
void EnDekubaba_Destroy(Actor* thisx, PlayState* play);
void EnDekubaba_Update(Actor* thisx, PlayState* play);
void EnDekubaba_Draw(Actor* thisx, PlayState* play);

GLOBAL_ASM("../src/game/actors/En_Dekubaba/data_syms.s")
extern Vec3f EnDekubaba_sZeroVec;

ActorInit En_Dekubaba_InitVars = {
    ACTOR_EN_DEKUBABA,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_DEKUBABA,
    sizeof(EnDekubaba),
    (ActorFunc)EnDekubaba_Init,
    (ActorFunc)EnDekubaba_Destroy,
    (ActorFunc)EnDekubaba_Update,
    (ActorFunc)EnDekubaba_Draw,
};

typedef struct {
    u32 w[10];
    f32 radius;
    s32 unk_2C;
} DekubabaElemInit; // size = 0x30

typedef struct {
    u32 base[2];
    s32 count;
    DekubabaElemInit* elements;
} DekubabaJntSphInit;

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(targetArrowOffset, 1500, ICHAIN_STOP),
};


static CollisionCheckInfoInit sColChkInfoInit = { 2, 25, 25, 255 };


static DekubabaElemInit sJntSphElementsInit[7] = {
    { { 0x00000000, 0xFFCFFFFF, 0x00000800, 0xFFCFFFFF, 0x00000000, 0x00010109, 0x00000001, 0x00000000, 0x42C80000, 0x447A0000 }, 15.0f, 100 },
    { { 0x00000000, 0x00000000, 0x00000000, 0xFFCFFFFF, 0x00000000, 0x00010000, 0x00000033, 0x00000000, 0x00000000, 0x44BB8000 }, 8.0f, 100 },
    { { 0x00000000, 0x00000000, 0x00000000, 0xFFCFFFFF, 0x00000000, 0x00000000, 0x00000034, 0x00000000, 0x00000000, 0x43FA0000 }, 8.0f, 100 },
    { { 0x00000000, 0x00000000, 0x00000000, 0xFFCFFFFF, 0x00000000, 0x00000000, 0x00000035, 0x00000000, 0x00000000, 0x44BB8000 }, 8.0f, 100 },
    { { 0x00000000, 0x00000000, 0x00000000, 0xFFCFFFFF, 0x00000000, 0x00000000, 0x00000036, 0x00000000, 0x00000000, 0x43FA0000 }, 8.0f, 100 },
    { { 0x00000000, 0x00000000, 0x00000000, 0xFFCFFFFF, 0x00000000, 0x00000000, 0x00000037, 0x00000000, 0x00000000, 0x44BB8000 }, 8.0f, 100 },
    { { 0x00000000, 0x00000000, 0x00000000, 0xFFCFFFFF, 0x00000000, 0x00000000, 0x00000038, 0x00000000, 0x00000000, 0x43FA0000 }, 8.0f, 100 },
};

static DekubabaJntSphInit sJntSphInit = { { 0x39091106, 0x10 }, 7, sJntSphElementsInit };

static DamageTable sDamageTableNormal = { { 16, 2, 1, 2, 226, 2, 2, 2, 241, 242, 244, 36, 2, 2, 2, 2, 2, 36, 0, 0, 0, 0, 241, 244, 242, 242, 248, 244, 0, 0, 4, 0 } };

static DamageTable sDamageTableBig = { { 16, 2, 1, 2, 226, 2, 2, 16, 241, 242, 244, 36, 2, 2, 2, 2, 2, 36, 0, 0, 0, 0, 241, 244, 242, 242, 248, 244, 0, 0, 4, 0 } };

void FUN_003e62a8(EnDekubaba* self, PlayState* play);
void Collider_InitJntSph(PlayState* play, EnDekubabaColliderJntSph* collider);
void Collider_SetJntSph(PlayState* play, EnDekubabaColliderJntSph* collider, Actor* actor, DekubabaJntSphInit* init,
                        EnDekubabaColliderElement* elements);



void FUN_001cbf9c(EnDekubaba* self, PlayState* play);
void FUN_00346714(EnDekubaba* self, PlayState* play);
void FUN_003d8694(EnDekubaba* self, PlayState* play);
void FUN_003e62a8(EnDekubaba* self, PlayState* play);
void FUN_0036e88c(MTX34* mtx, s16 x, s16 y, s16 z, s32 mode);
void Matrix_RotateY(MTX34* mtx, s32 mode, f32 angle);
void Matrix_UpdatePosition(MTX34* dst, MTX34* src, Vec3f* translate);
void FUN_00192844(s32 unused0, s32 index, s32 unused2, EnDekubaba* self);
void Collider_UpdateSpheres(s32 index, EnDekubabaColliderJntSph* collider, MTX34* mtx);

// Draft: ~200 words off (MakeTrans/Scale stack-copy + per-multiply scale copy class, see CLAUDE.md MakeScale / SetUnkAC notes).
#if 0
static inline void EnDekubaba_ScaleApply(MTX34* m, f32 x, f32 y, f32 z) {
    m->data[0][0] *= x;
    m->data[1][0] *= x;
    m->data[2][0] *= x;
    m->data[0][1] *= y;
    m->data[1][1] *= y;
    m->data[2][1] *= y;
    m->data[0][2] *= z;
    m->data[1][2] *= z;
    m->data[2][2] *= z;
}

void EnDekubaba_Draw(Actor* thisx, PlayState* play) {
    EnDekubaba* self = (EnDekubaba*)thisx;
    f32 scale;

    if (self->actionFunc != FUN_00346714) {
        SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, NULL, (PostLimbDrawOpa)FUN_00192844, &self->actor, 0);
        if (self->actionFunc == FUN_003e62a8) {
            Vec3f pos;
            MTX34 mtx;

            scale = self->scaleFac * 0.01f;
            pos.x = self->actor.home.pos.x;
            pos.y = self->actor.home.pos.y + (-6.0f * self->scaleFac);
            pos.z = self->actor.home.pos.z;
            MTX34_MakeTrans(&mtx, &pos);
            FUN_0036e88c(&mtx, self->stemPartsRot[0], self->actor.shape.rot.y, 0, 1);
            EnDekubaba_ScaleApply(&mtx, scale, scale, scale);
            self->skelAnimModel[0]->SetUnkAC(1);
            self->skelAnimModel[0]->FUN_003721e0(&mtx);
            Actor_SetFocus(&self->actor, 0.0f);
            self->skelAnimModel[0]->FUN_00372170(0);
        } else {
            FUN_001cbf9c(self, play);
        }
        {
            Vec3f pos;
            MTX34 mtx;

            scale = self->scaleFac * 0.01f;
            pos.x = self->actor.home.pos.x;
            pos.y = self->actor.home.pos.y;
            pos.z = self->actor.home.pos.z;
            MTX34_MakeTrans(&mtx, &pos);
            Matrix_RotateY(&mtx, 1, self->actor.home.rot.y * 0.000095873802f);
            EnDekubaba_ScaleApply(&mtx, scale, scale, scale);
            self->skelAnimModel[3]->SetUnkAC(1);
            self->skelAnimModel[3]->FUN_003721e0(&mtx);
            self->skelAnimModel[3]->FUN_00372170(0);
            if (self->actionFunc == FUN_003d8694) {
                FUN_0036e88c(&mtx, self->stemPartsRot[2], self->actor.shape.rot.y, 0, 1);
                self->skelAnimModel[2]->SetUnkAC(1);
                self->skelAnimModel[2]->FUN_003721e0(&mtx);
                Collider_UpdateSpheres(55, &self->collider, &mtx);
                Collider_UpdateSpheres(56, &self->collider, &mtx);
            }
        }
    } else if ((self->actionState > 60) || ((self->actionState & 1) == 0)) {
        Vec3f pos;
        MTX34 mtx;

        MTX34CopyAsm(&mtx, &self->actor.modelMtx);
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.z = 200.0f;
        Matrix_UpdatePosition(&mtx, &mtx, &pos);
        self->skelAnimModel[4]->SetUnkAC(1);
        self->skelAnimModel[4]->FUN_003721e0(&mtx);
        self->skelAnimModel[4]->FUN_00372170(0);
    }
}
#endif
GLOBAL_ASM("asm/EnDekubaba_Draw.s")

GLOBAL_ASM("asm/FUN_001cbf9c.s")


void FUN_001e766c(EnDekubaba* self, PlayState* play);
void FUN_00121654(EnDekubaba* self, PlayState* play);
void FUN_003e62a8(EnDekubaba* self, PlayState* play);
void Collider_DestroyJntSph(PlayState* play, EnDekubabaColliderJntSph* collider);
void Collider_UpdateSpheres(s32 index, EnDekubabaColliderJntSph* collider, MTX34* mtx);
void Animation_MorphToPlayOnce(SkelAnime* skelAnime, s32 idx, f32 morph);
s32 Math_ScaledStepToS(s16* pValue, s16 target, s16 scale);
void SkelAnime_SetPlaySpeed(SkelAnime* skelAnime, f32 speed);
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);
void FUN_00346778(Actor* actor, PlayState* play, s32 getItemId); // Actor_OfferGetItemNearby

void EnDekubaba_Destroy(Actor* thisx, PlayState* play) {
    EnDekubaba* self = (EnDekubaba*)thisx;

    Collider_DestroyJntSph(play, &self->collider);
    SkelAnime_Free2(&self->skelAnime);
    FUN_00350f34(thisx, &self->skelAnimModel[0], &self->skelAnimModel[1], &self->skelAnimModel[2],
                 &self->skelAnimModel[3], &self->skelAnimModel[4], 0);
}

void FUN_0032d48c(EnDekubaba* self, s32 type) {
    Animation_MorphToPlayOnce(&self->skelAnime, 1, -5.0f);
    self->actionState = type;
    self->collider.base.acFlags &= ~AC_ON;
    Actor_SetScale(&self->actor, self->scaleFac * 0.01f);
    if (type == 2) {
        Actor_SetColorFilter(&self->actor, 0, 155, 0, 62);
    } else {
        Actor_SetColorFilter(&self->actor, 0x400000, 255, 0, 42);
    }
    self->actionFunc = FUN_00121654;
}

void FUN_00192844(s32 unused0, s32 index, s32 unused2, EnDekubaba* self) {
    if (index == 1) {
        Collider_UpdateSpheres(index, &self->collider, &self->actor.modelMtx);
    }
}

void FUN_00363e8c(EnDekubaba* self) {
    self->actionState = 12;
    SkelAnime_SetPlaySpeed(&self->skelAnime, 0.0f);
    self->actionFunc = FUN_001e766c;
}

void FUN_00346714(EnDekubaba* self, PlayState* play) {
    if (self->actionState != 0) {
        self->actionState--;
    }
    if (Actor_HasParent(&self->actor, play) || (self->actionState == 0)) {
        Actor_Kill(&self->actor);
    } else {
        FUN_00346778(&self->actor, play, 7);
    }
}

void FUN_00366044(EnDekubaba* self); // EnDekubaba_ComputeHeadPos, own TU (z_en_dekubaba_head.cpp)

void FUN_00180bd0(EnDekubaba* self) {
    s32 i;

    self->stemPartsRot[0] = self->stemPartsRot[1] = self->stemPartsRot[2] = self->actor.shape.rot.x = -0x4000;
    self->actor.world.pos.x = self->actor.home.pos.x;
    self->actor.world.pos.z = self->actor.home.pos.z;
    self->actor.world.pos.y = self->actor.home.pos.y + (14.0f * self->scaleFac);
    Actor_SetScale(&self->actor, self->scaleFac * 0.005f);
    self->collider.base.colType = 12;
    self->collider.base.acFlags |= AC_HARD;
    self->actionState = 0x44;
    i = 1;
    while (i < 7) {
        EnDekubabaColliderElement* elem = &self->collider.elements[i];
        elem->worldSphere.center.x = self->actor.world.pos.x;
        elem->worldSphere.center.y = self->actor.world.pos.y - 7.0f;
        elem->worldSphere.center.z = self->actor.world.pos.z;
        i++;
    }
    self->actionFunc = FUN_003e62a8;
}

void EnDekubaba_Init(Actor* thisx, PlayState* play) {
    s32 i;
    EnDekubaba* self = (EnDekubaba*)thisx;
    EnDekubaba* s2 = self;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 22.0f);
    FUN_00372f38(&self->actor, play, &self->skelAnimModel[0], 1, &self->skelAnimModel[1], 2, &self->skelAnimModel[2], 3,
                 &self->skelAnimModel[3], 4, &self->skelAnimModel[4], 5, 0);
    FUN_00353c9c(&self->actor, play, &self->skelAnime, 0, 0, self->jointTable, self->morphTable, 3);
    Collider_InitJntSph(play, &self->collider);
    Collider_SetJntSph(play, &self->collider, &self->actor, &sJntSphInit, self->colliderElements);
    if (self->actor.params == 1) {
        self->scaleFac = 2.5f;
        for (i = 0; i < sJntSphInit.count; i++) {
            self->collider.elements[i].worldSphere.radius = self->collider.elements[i].modelSphere.radius =
                (s16)(sJntSphElementsInit[i].radius * 2.5f);
        }
        if (!LINK_IS_ADULT) {
            sDamageTableBig.table[27] = 4;
        }
        CollisionCheck_SetInfo(&self->actor.colChkInfo, &sDamageTableBig, &sColChkInfoInit);
        self->actor.colChkInfo.health = 4;
        self->actor.naviEnemyId = 8;
        self->actor.targetMode = 2;
    } else {
        self->scaleFac = 1.0f;
        for (i = 0; i < sJntSphInit.count; i++) {
            self->collider.elements[i].worldSphere.radius = self->collider.elements[i].modelSphere.radius;
        }
        if (!LINK_IS_ADULT) {
            sDamageTableNormal.table[27] = 4;
        }
        CollisionCheck_SetInfo(&self->actor.colChkInfo, &sDamageTableNormal, &sColChkInfoInit);
        self->actor.naviEnemyId = 7;
        thisx->targetMode = 1;
    }
    FUN_00180bd0(self);
    self->actionState = 0;
    s2->homePos = s2->actor.home.pos;
    self->floorPoly = NULL;
}

void FUN_0011d284(EnDekubaba* self, PlayState* play);
void Animation_PlayOnce(SkelAnime* skelAnime, s32 idx);

void FUN_001e766c(EnDekubaba* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (self->actionState != 0) {
        self->actionState--;
    }
    Math_SmoothStepToS(&self->actor.shape.rot.x, 0x1800, 2, 0xE38, 0x71C);
    FUN_00370084(&self->actor.shape.rot.y, Math_Vec3f_Yaw(&self->actor.home.pos, &player->actor.world.pos), 2, 0xE38);
    Math_ScaledStepToS(&self->stemPartsRot[0], 0xAAA, 0x444);
    Math_ScaledStepToS(&self->stemPartsRot[1], -0x4718, 0x888);
    Math_ScaledStepToS(&self->stemPartsRot[2], -0x6AA4, 0x888);
    if (self->actionState == 0) {
        Animation_PlayOnce(&self->skelAnime, 1);
        self->actionState = 0;
        self->actionFunc = FUN_0011d284;
    }
    FUN_00366044(self);
}

void FUN_00159184(EnDekubaba* self, PlayState* play);
extern __pure float _fabsf(float);
static __inline __pure float EnDekubaba_Fabsf(float f) {
    return _fabsf(f);
}
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void Audio_PlayActorSound2(Actor* actor, s32 arg1);

void FUN_003e62a8(EnDekubaba* self, PlayState* play) {
    s32 i;

    if (self->actionState != 0) {
        self->actionState--;
    }
    self->actor.world.pos.x = self->actor.home.pos.x;
    self->actor.world.pos.z = self->actor.home.pos.z;
    self->actor.world.pos.y = self->actor.home.pos.y + (14.0f * self->scaleFac);
    if ((self->actionState == 0) && (self->actor.xzDistToPlayer < (200.0f * self->scaleFac)) &&
        (EnDekubaba_Fabsf(self->actor.yDistToPlayer) < (30.0f * self->scaleFac))) {
        // inlined EnDekubaba_SetupExitGround
        f32 end = Animation_GetLastFrame(&self->skelAnime, 0);
        f32 speed = Animation_GetLastFrame(&self->skelAnime, 0) * (1.0f / 15);

        Animation_Change_00375c08(&self->skelAnime, 0, speed, 0.0f, end, 2, 0.0f);
        self->actionState = 23;
        for (i = 2; i < 7; i++) {
            self->collider.elements[i].base.ocElemFlags |= OCELEM_ON;
        }
        self->collider.base.colType = 6;
        self->collider.base.acFlags &= ~AC_HARD;
        Audio_PlayActorSound2(&self->actor, 0x0100036f);
        self->actionFunc = FUN_00159184;
    }
}

void FUN_0013fbd8(EnDekubaba* self, PlayState* play);
void FUN_001283e0(EnDekubaba* self, PlayState* play);
void Animation_MorphToLoop(SkelAnime* skelAnime, s32 idx, f32 morph);
s32 FUN_003731e0(SkelAnime* skelAnime);
void SkelAnime_SetPlaySpeed(SkelAnime* skelAnime, f32 speed);

void FUN_001cc630(EnDekubaba* self, PlayState* play) {
    s16 temp;

    FUN_003731e0(&self->skelAnime);
    Math_ScaledStepToS(&self->actor.shape.rot.x, self->stemPartsRot[0], 0x71C);
    Math_ScaledStepToS(&self->stemPartsRot[0], self->stemPartsRot[1], 0x71C);
    Math_ScaledStepToS(&self->stemPartsRot[1], self->stemPartsRot[2], 0x71C);
    if (Math_ScaledStepToS(&self->stemPartsRot[2], self->wobbleTarget, 0x71C)) {
        self->wobbleTarget = (s16)(s32)(-16384.0f - ((self->wobbleTarget + 0x4000) * 0.8f));
    }
    temp = self->wobbleTarget + 0x4000;
    if (ABS(temp) < 0x100) {
        self->collider.base.acFlags |= AC_ON;
        if (self->actor.xzDistToPlayer < (80.0f * self->scaleFac)) {
            self->actionState = 12;
            SkelAnime_SetPlaySpeed(&self->skelAnime, 0.0f);
            self->actionFunc = FUN_001e766c;
        } else {
            SkelAnime_SetPlaySpeed(&self->skelAnime, -1.0f);
            self->actionState = 14;
            self->collider.base.acFlags |= AC_ON;
            self->actionFunc = FUN_0013fbd8;
        }
    }
    FUN_00366044(self);
}

void FUN_0013fbd8(EnDekubaba* self, PlayState* play) {
    s32 remains;

    FUN_003731e0(&self->skelAnime);
    if (self->actionState > 12) {
        remains = 0;
        remains |= Math_SmoothStepToS(&self->actor.shape.rot.x, 0x1800, 1, 0x11C6, 0x71C);
        remains |= Math_SmoothStepToS(&self->stemPartsRot[0], -0x1555, 1, 0xAAA, 0x71C);
        remains |= Math_SmoothStepToS(&self->stemPartsRot[1], -0x38E3, 1, 0xE38, 0x71C);
        remains |= Math_SmoothStepToS(&self->stemPartsRot[2], -0x5C71, 1, 0x11C6, 0x71C);
        if (remains == 0) {
            self->actionState = 12;
        }
    } else {
        if (self->actionState != 0) {
            self->actionState--;
        }
        if (self->actionState == 0) {
            s32 n = Animation_GetLastFrame(&self->skelAnime, 0) * 2;

            self->actionState = (n > 0) ? (s32)(0.5f + (n * 3.0f) * 0.5f) : (s32)((n * 3.0f) * 0.5f - 0.5f);
            Animation_MorphToLoop(&self->skelAnime, 0, -3.0f);
            self->actionFunc = FUN_001283e0;
        }
    }
    FUN_00366044(self);
}

void FUN_00363ec4(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 duration);
s32 Math_StepToF(f32* pValue, f32 target, f32 step);
EnItem00* Item_DropCollectible(PlayState* play, Vec3f* spawnPos, s16 params);
void Item_DropCollectibleRandom(PlayState* play, Actor* actor, Vec3f* spawnPos, s32 params);

void FUN_001756b0(EnDekubaba* self, PlayState* play) {
    Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y, self->scaleFac * 5.0f);
    if (Math_StepToF(&self->actor.scale.x, self->scaleFac * 0.1f * 0.01f, self->scaleFac * 0.1f * 0.01f)) {
        FUN_00363ec4(play, &self->actor.home.pos, &EnDekubaba_sZeroVec, &EnDekubaba_sZeroVec, self->scaleFac * 500.0f,
                     self->scaleFac * 100.0f);
        if (self->actor.unk_122 == 0) {
            Item_DropCollectible(play, &self->actor.world.pos, 0xC);
            if (self->actor.params == 1) {
                Item_DropCollectible(play, &self->actor.world.pos, 0xC);
                Item_DropCollectible(play, &self->actor.world.pos, 0xC);
            }
        } else {
            Item_DropCollectibleRandom(play, &self->actor, &self->actor.world.pos, 0x30);
        }
        Actor_Kill(&self->actor);
    }
    self->actor.scale.y = self->actor.scale.z = self->actor.scale.x;
    self->actor.shape.rot.z += 0x1C70;
    EffectSsHahen_SpawnBurst(play, &self->actor.home.pos, self->scaleFac * 3.0f, 0, self->scaleFac * 12.0f,
                             self->scaleFac * 5.0f, 1, -1, 10, NULL);
}

void FUN_0013e324(EnDekubaba* self, PlayState* play) {
    s32 i;

    FUN_003731e0(&self->skelAnime);
    if (self->actionState != 0) {
        self->actionState--;
    }
    if (self->actionState == 0) {
        i = 1;
        while (i < 7) {
            self->collider.elements[i].base.bumperFlags &= ~(1 << 0);
            i++;
        }
        if (self->actor.xzDistToPlayer < (80.0f * self->scaleFac)) {
            self->actionState = 12;
            SkelAnime_SetPlaySpeed(&self->skelAnime, 0.0f);
            self->actionFunc = FUN_001e766c;
        } else {
            SkelAnime_SetPlaySpeed(&self->skelAnime, -1.0f);
            self->actionState = 14;
            self->collider.base.acFlags |= AC_ON;
            self->actionFunc = FUN_0013fbd8;
        }
    }
}


void FUN_003d8694(EnDekubaba* self, PlayState* play) {
    s32 i;
    Vec3f effPos;
    f32 dx;
    f32 dz;
    f32 dy;

    Math_StepToF(&self->actor.speedXZ, 0.0f, self->scaleFac * 0.1f);
    if (self->actionState == 0) {
        Math_ScaledStepToS(&self->actor.shape.rot.x, 0x4800, 0x71C);
        Math_ScaledStepToS(&self->stemPartsRot[0], 0x4800, 0x71C);
        Math_ScaledStepToS(&self->stemPartsRot[1], 0x4800, 0x71C);
        EffectSsHahen_SpawnBurst(play, &self->actor.world.pos, self->scaleFac * 3.0f, 0, self->scaleFac * 12.0f,
                                 self->scaleFac * 5.0f, 1, -1, 10, NULL);
        if ((self->actor.scale.x > 0.005f) && ((self->actor.bgCheckFlags & 8) || (self->actor.bgCheckFlags & 2))) {
            self->actor.scale.x = self->actor.scale.y = self->actor.scale.z = 0.0f;
            self->actor.speedXZ = 0.0f;
            self->actor.flags &= ~5;
            EffectSsHahen_SpawnBurst(play, &self->actor.world.pos, self->scaleFac * 3.0f, 0, self->scaleFac * 12.0f,
                                     self->scaleFac * 5.0f, 15, -1, 10, NULL);
        }
        if (self->actor.bgCheckFlags & 2) {
            Audio_PlayActorSound2(&self->actor, 0x010002eb);
            self->actionState = 1;
        }
    } else if (self->actionState == 1) {
        effPos = self->actor.world.pos;
        dy = Math_SinS(self->actor.shape.rot.x) * 20.0f;
        dx = -20.0f * Math_CosS(self->actor.shape.rot.x) * Math_SinS(self->actor.shape.rot.y);
        dz = -20.0f * Math_CosS(self->actor.shape.rot.x) * Math_CosS(self->actor.shape.rot.y);
        for (i = 0; i < 4; i++) {
            FUN_00363ec4(play, &effPos, &EnDekubaba_sZeroVec, &EnDekubaba_sZeroVec, 500, 50);
            effPos.x += dx;
            effPos.y += dy;
            effPos.z += dz;
        }
        FUN_00363ec4(play, &self->actor.home.pos, &EnDekubaba_sZeroVec, &EnDekubaba_sZeroVec, self->scaleFac * 500.0f,
                     self->scaleFac * 100.0f);
        Actor_SetScale(&self->actor, 0.03f);
        self->actor.shape.yOffset = 1000.0f;
        self->actor.shape.shadowScale = 3.0f;
        self->actor.shape.rot.x -= 0x4000;
        self->actor.gravity = 0.0f;
        self->actor.velocity.y = 0.0f;
        Actor_ChangeType(play, &play->actorCtx, &self->actor, 8);
        self->actor.flags &= ~0x20;
        self->actionState = 300;
        self->actionFunc = FUN_00346714;
    }
}

void Animation_PlayLoopSetSpeed(SkelAnime* skelAnime, s32 idx, f32 speed);
f32* EnDekubaba_CurFramePtr(SkelAnime* skelAnime);
void FUN_00366150(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor,
                  Color_RGBA8* envColor, s16 scale, s16 scaleStep);
void FUN_00123adc(EnDekubaba* self, PlayState* play);

void FUN_0011d284(EnDekubaba* self, PlayState* play) {
    static Color_RGBA8 sEffPrimColor = { 105, 255, 105, 255 };
    static Color_RGBA8 sEffEnvColor = { 150, 250, 150, 0 };
    s32 lyingDown;
    s32 stepBase;
    Vec3f effVel;

    FUN_003731e0(&self->skelAnime);
    if (self->actionState == 0) {
        if (Animation_OnFrameImpl(&self->skelAnime, 1.0f, 1.0f)) {
            if (self->actor.params == 1) {
                Audio_PlayActorSound2(&self->actor, 0x010002cd);
            } else {
                Audio_PlayActorSound2(&self->actor, 0x010002d1);
            }
        }
        Math_ScaledStepToS(&self->actor.shape.rot.x, 0, 0x222);
        stepBase = (s32)(*EnDekubaba_CurFramePtr(&self->skelAnime) * 10.0f);
        lyingDown = true;
        lyingDown &= Math_ScaledStepToS(&self->stemPartsRot[0], -0xE38, stepBase + 0x38E);
        lyingDown &= Math_ScaledStepToS(&self->stemPartsRot[1], -0xE38, stepBase + 0x71C);
        lyingDown &= Math_ScaledStepToS(&self->stemPartsRot[2], -0xE38, stepBase + 0xE38);
        if (lyingDown) {
            Animation_PlayLoopSetSpeed(&self->skelAnime, 0, 4.0f);
            effVel.x = Math_SinS(self->actor.shape.rot.y) * 5.0f;
            effVel.y = 0.0f;
            effVel.z = Math_CosS(self->actor.shape.rot.y) * 5.0f;
            FUN_00366150(play, &self->actor.world.pos, &effVel, &EnDekubaba_sZeroVec, &sEffPrimColor, &sEffEnvColor, 1,
                         self->scaleFac * 100.0f);
            self->actionState = 1;
            self->collider.base.acFlags |= AC_ON;
        }
    } else if (self->actionState >= 17) {
        Animation_Change_00375c08(&self->skelAnime, 1, 1.0f, 15.0f, Animation_GetLastFrame(&self->skelAnime, 1), 2,
                                  -3.0f);
        self->actionState = 0;
        self->actionFunc = FUN_00123adc;
    } else {
        self->actionState++;
        if ((self->actionState >= 6) && !Actor_IsFacingPlayer(&self->actor, 0x16C)) {
            FUN_00370084(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 15, 0x71C);
        }
        if (Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f) || Animation_OnFrameImpl(&self->skelAnime, 12.0f, 1.0f)) {
            if (self->actor.params == 1) {
                Audio_PlayActorSound2(&self->actor, 0x010002cc);
            } else {
                Audio_PlayActorSound2(&self->actor, 0x010002d0);
            }
        }
    }
    FUN_00366044(self);
}

void FUN_0011d284(EnDekubaba* self, PlayState* play);
void FUN_00123adc(EnDekubaba* self, PlayState* play);
void FUN_00121654(EnDekubaba* self, PlayState* play);
void FUN_0013fbd8(EnDekubaba* self, PlayState* play);
void FUN_001cc630(EnDekubaba* self, PlayState* play);
void FUN_0013e324(EnDekubaba* self, PlayState* play);
void FUN_003742c4(Actor* actor, EnDekubabaColliderJntSph* collider, s32 flag);
void FUN_00375b70(PlayState* play, Actor* actor);
void EffectSsEnFire_SpawnVec3f(PlayState* play, Actor* actor, Vec3f* pos, s16 scale, s16 params, s16 flags,
                               s16 bodyPart, s16 unk);
s32 CollisionCheck_SetAT(PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider);
void Actor_UpdateBgCheckInfo(PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight,
                             s32 flags);

#if 0
static void EnDekubaba_SetupSoothe(EnDekubaba* self) {
    SkelAnime_SetPlaySpeed(&self->skelAnime, -1.0f);
    self->actionState = 14;
    self->collider.base.acFlags |= AC_ON;
    self->actionFunc = FUN_0013fbd8;
}

static void EnDekubaba_DisableStemColliderAC(EnDekubaba* self) {
    s32 i;

    for (i = 1; i < 7; i++) {
        self->collider.elements[i].base.bumperFlags &= ~1;
    }
}

static void EnDekubaba_SetupWobble(EnDekubaba* self) {
    self->wobbleTarget = -0x6000;
    self->stemPartsRot[2] = -0x5000;
    self->stemPartsRot[1] = -0x4800;
    EnDekubaba_DisableStemColliderAC(self);
    Actor_SetColorFilter(&self->actor, 0x400000, 255, 0, 35);
    self->collider.base.acFlags &= ~AC_ON;
    self->actionFunc = FUN_001cc630;
}

static void EnDekubaba_SetupDieDropStick(EnDekubaba* self) {
    SkelAnime_SetPlaySpeed(&self->skelAnime, 0.0f);
    self->actionState = 0;
    self->actor.gravity = -0.8f;
    self->actor.velocity.y = 4.0f;
    self->actor.world.rot.y = self->actor.shape.rot.y + 0x8000;
    self->actor.speedXZ = self->scaleFac * 3.0f;
    self->collider.base.acFlags &= ~AC_ON;
    self->actor.flags |= 0x30;
    self->actionFunc = FUN_003d8694;
}

static void EnDekubaba_CheckCollide(EnDekubaba* self, PlayState* play) {
    f32 scale;
    s32 newHealth;
    s32 i;

    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        FUN_003742c4(&self->actor, &self->collider, 1);
        if ((self->collider.base.colType != 12) &&
            (((self->actor.colChkInfo.damageEffect != 0)) || (self->actor.colChkInfo.damage != 0))) {
            newHealth = self->actor.colChkInfo.health - self->actor.colChkInfo.damage;
            if (self->actionFunc != FUN_0013e324) {
                if ((self->actor.colChkInfo.damageEffect == 14) || (self->actor.colChkInfo.damageEffect == 1)) {
                    if (self->actor.colChkInfo.damageEffect == 14) {
                        newHealth = self->actor.colChkInfo.health;
                    }
                    FUN_0032d48c(self, 2);
                } else if (self->actionFunc == FUN_00123adc) {
                    if (newHealth <= 0) {
                        newHealth = 1;
                    }
                    FUN_0032d48c(self, 1);
                } else {
                    FUN_0032d48c(self, 0);
                }
            } else if ((self->actor.colChkInfo.damageEffect == 14) || (self->actor.colChkInfo.damageEffect == 15)) {
                if (newHealth > 0) {
                    EnDekubaba_SetupWobble(self);
                } else {
                    EnDekubaba_SetupDieDropStick(self);
                }
            } else if (self->actor.colChkInfo.damageEffect != 1) {
                FUN_0032d48c(self, 0);
            } else {
                return;
            }
            if (newHealth < 0) {
                newHealth = 0;
            }
            self->actor.colChkInfo.health = newHealth;
            if (self->actor.colChkInfo.damageEffect == 2) {
                scale = self->scaleFac * 70.0f;
                for (i = 0; i < 4; i++) {
                    EffectSsEnFire_SpawnVec3f(play, &self->actor, &self->actor.world.pos, scale, 0, 0, i, 1);
                }
            }
        } else {
            return;
        }
    } else if ((play->actorCtx.unk_02 != 0) && (self->collider.base.colType != 12) &&
               (self->actionFunc != FUN_0013e324) && (self->actionFunc != FUN_00121654) &&
               (self->actor.colChkInfo.health != 0)) {
        self->actor.colChkInfo.health--;
        self->actor.unk_122 = 0;
        FUN_0032d48c(self, 1);
    } else {
        return;
    }
    if (self->actor.colChkInfo.health != 0) {
        if (self->actionState == 3) {
            Audio_PlayActorSound2(&self->actor, 0x0100030e);
        } else {
            Audio_PlayActorSound2(&self->actor, 0x010002ce);
        }
    } else {
        FUN_00375b70(play, &self->actor);
        if (self->actor.params == 1) {
            Audio_PlayActorSound2(&self->actor, 0x010002cf);
        } else {
            Audio_PlayActorSound2(&self->actor, 0x010002d2);
        }
    }
}

// Draft: 64 words off (entry regs r6/r7/r8 numbering, bic dest regs, Attacked/health predication).
void EnDekubaba_Update(Actor* thisx, PlayState* play) {
    EnDekubaba* self = (EnDekubaba*)thisx;
    EnDekubaba* s2 = self;

    if (self->collider.base.atFlags & AT_HIT) {
        self->collider.base.atFlags &= ~AT_HIT;
        EnDekubaba_SetupSoothe(self);
    }
    EnDekubaba_CheckCollide(self, play);
    self->actionFunc(self, play);
    if (self->actionFunc == FUN_003d8694) {
        Actor_MoveForward(&self->actor);
        Actor_UpdateBgCheckInfo(play, &self->actor, 10.0f, self->scaleFac * 15.0f, 10.0f, 5);
    } else if (self->actionFunc != FUN_00346714) {
        Actor_UpdateBgCheckInfo(play, &self->actor, 0.0f, 0.0f, 0.0f, 4);
        if (self->floorPoly == NULL) {
            s2->floorPoly = s2->actor.floorPoly;
        }
    }
    if (self->actionFunc == FUN_0011d284) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
        self->actor.flags |= 0x1000000;
    }
    if (self->collider.base.acFlags & AC_ON) {
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
    }
    if (self->actionFunc != FUN_00346714) {
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    }
    *((u8*)self->skelAnime.unk_28 + 0xAD) = (self->actor.isDrawn != 0);
}
#endif
GLOBAL_ASM("asm/EnDekubaba_Update.s")

}
