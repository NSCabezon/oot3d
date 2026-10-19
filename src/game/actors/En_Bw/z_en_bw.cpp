extern "C" {
#include "z_en_bw.hpp"
#include "mtx34_trans.hpp"

#define FLAGS 0x00000015

void EnBw_Init(EnBw* self, PlayState* play);
void EnBw_Destroy(Actor* actor, GameState* state);
void EnBw_Update(Actor* actor, GameState* state);
void EnBw_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("../src/game/actors/En_Bw/data_syms.s")

GLOBAL_ASM("data/z_En_Bw.data.s")

ActorInit En_Bw_InitVars = {
    ACTOR_EN_BW,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_BW,
    sizeof(EnBw),
    (ActorFunc)EnBw_Init,
    (ActorFunc)EnBw_Destroy,
    (ActorFunc)EnBw_Update,
    (ActorFunc)EnBw_Draw,
};

s32 BgCheck_LineTestImpl(CollisionContext* colCtx, s32 type, s32 unk2, Vec3f* posA, Vec3f* posB, Vec3f* posResult,
                         f32 unk6, CollisionPoly** poly, s32* bgId, Actor* actor, s32 flags);

static inline s32 EnBw_GetBccFlags(s32 chkWall, s32 chkFloor, s32 chkCeil, s32 chkOneFace) {
    s32 result = 0;

    if (chkWall) {
        result |= 1;
    }
    if (chkFloor) {
        result |= 2;
    }
    if (chkCeil) {
        result |= 4;
    }
    if (chkOneFace) {
        result |= 8;
    }
    return result | 0x10;
}

s32 FUN_003303cc(CollisionContext* colCtx, Vec3f* posA, Vec3f* posB, Vec3f* posResult, CollisionPoly** poly,
                 s32 chkWall, s32 chkFloor, s32 chkCeil, s32 chkOneFace) {
    s32 bgId;

    return BgCheck_LineTestImpl(colCtx, 0, 0, posA, posB, posResult, 1.0f, poly, &bgId, NULL,
                                EnBw_GetBccFlags(chkWall, chkFloor, chkCeil, chkOneFace));
}

void FUN_00350348(EnBw* self);

void FUN_0027fd24(EnBw* self, PlayState* play) {
    s16 t;

    u8 t450 = (u8)(self->unk_450 - 1);
    self->unk_450 = t450;
    self->unk_451 = (u8)(self->unk_451 + 4);
    self->unk_452 = (u8)(self->unk_452 + 5);
    if (t450 < 200) {
        self->unk_450 = 200;
    }
    if (self->unk_451 > 200) {
        self->unk_451 = 200;
    }
    if (self->unk_452 > 230) {
        self->unk_452 = 230;
    }
    if (*(u32*)((u8*)self + 0x11C) & 0x400000) {
        if ((*(u32*)((u8*)play + 0x5BF4) & 0x7F) == 0) {
            self->unk_480 = 0.5f + Rand_ZeroOne() * 0.25f;
        }
        self->unk_47C = self->unk_47C + self->unk_480;
        Math_SmoothStepToF(&self->unk_484, 0.075f, 1.0f, 0.005f, 0.0f);
    }
    t = self->unk_446 - 1;
    self->unk_446 = t;
    if (t != 0) {
        return;
    }
    FUN_00350348(self);
    self->unk_451 = 200;
    self->unk_450 = 200;
    self->unk_452 = 0xFF;
    self->unk_448 = 900;
    self->unk_445 = 1;
    self->unk_474 = 0.7f;
    self->unk_45A++;
}


void FUN_00248264(EnBw* self) {
    if (self->unk_454 != 0) {
        self->actor.scale.x += 0.00013333333f;
        self->actor.scale.y -= 0.00013333333f;
        self->actor.scale.z += 0.00013333333f;
    }
    u8 f = self->unk_453;
    f -= 4;
    self->unk_453 = f;
    *((u8*)&self->actor + 0xD0) = f;
    s16 t = *(s16*)((u8*)self + 0x446) - 1;
    *(s16*)((u8*)self + 0x446) = t;
    if (t > 0) {
        return;
    }
    Actor_Kill(&self->actor);
}

extern s32 EnBw_sSlugGroup;
extern ColliderCylinderInit EnBw_sCylinderInit1;
extern ColliderCylinderInit EnBw_sCylinderInit2;
extern DamageTable EnBw_sDamageTable;
void FUN_00350508(void* p, PlayState* play, s32 a, s32 b);
void FUN_00350348(EnBw* self);

void EnBw_Init(EnBw* self, PlayState* play) {
    ZARInfo* zar;

    Actor_SetScale(&self->actor, 0.012999999f);
    self->actor.gravity = -2.0f;
    self->actor.naviEnemyId = 0x23;
    zar = FUN_00372f38(&self->actor, play, 0);
    FUN_00350508(&self->fireModel, play, 0, 11);
    FUN_00353c9c(&self->actor, play, &self->skelAnime, 0, 0, self->jointTable, self->morphTable, 5);
    self->unk_430 = self->skelAnime.unk_28->unk_0C;
    self->unk_430->FUN_00372d94(zar->GetCMABByIndex(0));
    self->unk_430->SetAnimType(1);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 40.0f);
    self->actor.colChkInfo.damageTable = &EnBw_sDamageTable;
    self->actor.colChkInfo.health = 6;
    self->actor.colChkInfo.mass = 0xFE;
    self->actor.focus.pos = self->actor.world.pos;
    FUN_00350348(self);
    self->unk_450 = 255;
    self->unk_453 = 255;
    self->unk_452 = 0;
    self->unk_451 = 0;
    self->unk_46C = 0.6f;
    self->unk_445 = 3;
    Collider_InitCylinder(play, &self->collider1);
    Collider_SetCylinder(play, &self->collider1, &self->actor, &EnBw_sCylinderInit1);
    Collider_SetCylinder(play, &self->collider2, &self->actor, &EnBw_sCylinderInit2);
    self->unk_45A = self->actor.world.rot.y;
    self->actor.params = EnBw_sSlugGroup;
    EnBw_sSlugGroup = (EnBw_sSlugGroup + 1) & 3;
}


void FUN_0035046c(void* p);

f32 Math_SinF(f32 angle);

#if 0 // FUN_002482d0: 13 words off (VFP numbering: ROM s17..s20 = 2.0,1/3,0.001,0.013 and s16 = 0.0035; ours s16 = 0.001); GLOBAL_ASM stays in functions_240000s.cpp
void FUN_002482d0(EnBw* self) {
    s32 doTail = 0;

    if (FUN_003731e0(&self->skelAnime)) {
        if (self->unk_446 <= 0) {
            self->unk_446 = 150;
            doTail = 1;
        } else {
            self->unk_446--;
        }
    }
    if (doTail || self->unk_446 >= 20) {
        s16 v = (s16)(self->unk_448 - 0xA7);

        self->unk_448 = v;
        if (v < 0) {
            self->unk_448 = 0;
        }
    }
    f32 milli;
    f32 two;
    f32 third;

    two = 2.0f;
    third = 0.3333333f;
    milli = 0.001f;
    self->actor.scale.x = 0.013f - Math_SinF((f32)self->unk_448 * two * third * milli) * 0.0035f;
    self->actor.scale.y = 0.013f + Math_SinF((f32)self->unk_448 * two * third * milli) * 0.0245f;
    self->actor.scale.z = 0.013f - Math_SinF((f32)self->unk_448 * two * third * milli) * 0.0035f;
    if (self->unk_448 == 0) {
        FUN_00350348(self);
        self->unk_448 = 300;
    }
}
#endif

void FUN_0020eb70(EnBw* self, PlayState* play);
void FUN_0036f00c(PlayState* play, Actor* actor, Vec3f* pos, f32 a, s32 b, f32 c, s32 d, s32 e, s32 f);
s32 FUN_003731e0(SkelAnime* skelAnime);

void FUN_00266b14(EnBw* self, PlayState* play) {
    f32 zero;

    Math_SmoothStepToS((s16*)((u8*)self + 0xC0), 0, 1, 4000, 0);
    zero = 0.0f;
    Math_SmoothStepToF((f32*)((u8*)self + 0x46C), 0.6f, 1.0f, 0.05f, zero);
    FUN_003731e0(&self->skelAnime);
    if (self->actor.bgCheckFlags & 3) {
        FUN_0036f00c(play, &self->actor, &self->actor.world.pos, 30.0f, 11, 4.0f, 0, 0, 0);
        self->unk_446 = 4500;
        self->unk_474 = 0.0f;
        Audio_PlayActorSound2(&self->actor, 0x10002EB);
        self->actionFunc = FUN_0020eb70;
    }
    if (self->unk_450 < 0xF7) {
        self->unk_450 += 8;
    } else {
        self->unk_450 = 0xFF;
    }
    if (self->unk_451 < 0x20) {
        self->unk_451 = 0;
    } else {
        self->unk_451 -= 0x20;
    }
    if (self->unk_452 < 0x28) {
        self->unk_452 = 0;
    } else {
        self->unk_452 -= 0x28;
    }
    if (*(f32*)((u8*)self + 0xC4) > 0.0f) {
        *(f32*)((u8*)self + 0xC4) = *(f32*)((u8*)self + 0xC4) - 200.0f;
    }
}

void Item_DropCollectibleRandom(PlayState* play, Actor* fromActor, Vec3f* pos, s16 params);
void Animation_MorphToLoop(SkelAnime* skelAnime, s32 idx, f32 morph);
void FUN_00266b14(EnBw* self, PlayState* play);
s32 FUN_00342abc(PlayState* play, Actor* actor);

void FUN_002a3a58(EnBw* self, PlayState* play) {
    EnBwActionFunc fn;

    if (*(u16*)((u8*)self + 0x11A) != 0) {
        return;
    }
    self->unk_460 = 0;
    if (*((u8*)self + 0xB7) != 0) {
        if (self->unk_444 != 5) {
            if (self->unk_444 == 6) {
                fn = FUN_00266b14;
            } else {
                FUN_00350348(self);
                self->unk_451 = 200;
                self->unk_450 = 200;
                self->unk_452 = 0xFF;
                self->unk_448 = 900;
                self->unk_445 = 1;
                self->unk_474 = 0.7f;
                self->unk_45A++;
                return;
            }
        } else {
            Animation_MorphToLoop(&self->skelAnime, 2, -1.0f);
            self->unk_444 = 6;
            self->actor.speedXZ = 0.0f;
            self->unk_446 = 1500;
            self->unk_445 = 3;
            self->actor.velocity.y = 11.0f;
            Audio_PlayActorSound2(&self->actor, 0x10003E6);
            *(u16*)((u8*)self + 0x90) = *(u16*)((u8*)self + 0x90) & ~1;
            self->actionFunc = FUN_00266b14;
            return;
        }
    } else {
        if (!FUN_00342abc(play, (Actor*)&self->collider2)) {
            self->unk_454 = 1;
        } else {
            self->unk_454 = 0;
            self->actor.scale.y = self->actor.scale.y - 0.009f;
            FUN_0036f00c(play, &self->actor, &self->actor.world.pos, 30.0f, 11, 4.0f, 0, 0, 0);
        }
        Item_DropCollectibleRandom(play, &self->actor, &self->actor.world.pos, 0x90);
        self->unk_444 = 0;
        self->actor.speedXZ = 0.0f;
        self->unk_446 = 60;
        self->actor.flags &= ~1;
        Audio_PlayActorSound2(&self->actor, 0x10003E8);
        fn = (EnBwActionFunc)FUN_00248264;
    }
    self->actionFunc = fn;
}

void EnBw_Destroy(Actor* actor, GameState* state) {
    EnBw* self = (EnBw*)actor;
    PlayState* play = (PlayState*)state;

    Collider_DestroyCylinder(play, &self->collider1);
    Collider_DestroyCylinder(play, &self->collider2);
    SkelAnime_Free2(&self->skelAnime);
    FUN_003508b8(actor, 0);
    FUN_0035046c(&self->fireModel);
}

void Matrix_RotateX(MTX34* mtx, s32 mode, f32 angle);
void Matrix_RotateY(MTX34* mtx, s32 mode, f32 angle);
void Matrix_RotateZ(MTX34* mtx, s32 mode, f32 angle);
void MTX34CopyAsm(MTX34* dst, MTX34* src);

static inline void EnBw_ScaleApply(MTX34* m, f32 x, f32 y, f32 z) {
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

#if 0 // FUN_00233910: far (86 words): inlined Matrix_Scale-apply emits per-multiply VFP copies (vmov s2,s0 before each product), same scale-copy class as EnPoDesert_OverrideLimbDraw; GLOBAL_ASM stays in functions_200000s.cpp
s32 FUN_00233910(PlayState* play, s32 limbIndex, MTX34* mtx, EnBw* self) {
    MTX34 m;

    if (limbIndex == 0 && (self->unk_444 == 1 || self->unk_444 == 5)) {
        MTX34CopyAsm(&m, mtx);
        EnBw_ScaleApply(&m, 1.0f, 1.0f, 1.0f);
        Matrix_RotateX(&m, 1, self->unk_47C * 0.115f);
        Matrix_RotateY(&m, 1, self->unk_47C * 0.13f);
        Matrix_RotateZ(&m, 1, self->unk_47C * 0.1f);
        EnBw_ScaleApply(&m, 1.0f - self->unk_484, 1.0f + self->unk_484, 1.0f - self->unk_484);
        Matrix_RotateZ(&m, 1, self->unk_47C * -0.1f);
        Matrix_RotateY(&m, 1, self->unk_47C * -0.13f);
        Matrix_RotateX(&m, 1, self->unk_47C * -0.115f);
        return 1;
    }
    return 0;
}
#endif

void FUN_0026680c(EnBw* self, PlayState* play) {
    Math_SmoothStepToS((s16*)((u8*)self + 0xC0), 0x7FFF, 1, 4000, 0);
    Math_SmoothStepToF((f32*)((u8*)self + 0x46C), 0.0f, 1.0f, 0.05f, 0.0f);
    FUN_003731e0(&self->skelAnime);
    if ((*(u16*)((u8*)self + 0x90) & 3) && *(s32*)((u8*)self + 0x440) >= 2) {
        if ((*(u32*)((u8*)play + 0x5BF4) & 0x7F) == 0) {
            self->unk_480 = 0.7f + Rand_ZeroOne() * 0.25f;
        }
        self->unk_47C = self->unk_47C + self->unk_480;
        self->unk_445 = 4;
        Math_SmoothStepToF(&self->unk_484, 0.075f, 1.0f, 0.005f, 0.0f);
        if (*(u16*)((u8*)self + 0x90) & 2) {
            FUN_0036f00c(play, &self->actor, &self->actor.world.pos, 30.0f, 11, 4.0f, 0, 0, 0);
            Audio_PlayActorSound2(&self->actor, 0x010002EB);
        }
        if (self->unk_448 >= 0) {
            s16 v = (s16)(self->unk_448 - 0xA7);

            self->unk_448 = v;
            if (v < 0) {
                self->unk_448 = 0;
            }
            self->actor.scale.x = 0.013f + Math_SinF((f32)self->unk_448 * 2.0f * 0.33333334f * 0.001f) * 0.0069999998f;
            self->actor.scale.y = 0.013f - Math_SinF((f32)self->unk_448 * 2.0f * 0.33333334f * 0.001f) * 0.0069999998f;
            self->actor.scale.z = 0.013f + Math_SinF((f32)self->unk_448 * 2.0f * 0.33333334f * 0.001f) * 0.0069999998f;
        }
        s16 t = self->unk_446 - 1;
        self->unk_446 = t;
        if (t == 0) {
            Animation_MorphToLoop(&self->skelAnime, 2, -1.0f);
            self->unk_444 = 6;
            self->actor.speedXZ = 0.0f;
            self->unk_446 = 1500;
            self->unk_445 = 3;
            self->actor.velocity.y = 11.0f;
            Audio_PlayActorSound2(&self->actor, 0x010003E6);
            *(u16*)((u8*)self + 0x90) = *(u16*)((u8*)self + 0x90) & ~1;
            self->actionFunc = FUN_00266b14;
        }
    } else {
        self->unk_450 -= 8;
        self->unk_451 += 32;
        if (self->unk_450 < 200) {
            self->unk_450 = 200;
        }
        if (self->unk_451 > 200) {
            self->unk_451 = 200;
        }
        if (self->unk_452 > 235) {
            self->unk_452 = 255;
        } else {
            self->unk_452 += 40;
        }
        f32 y = *(f32*)((u8*)self + 0xC4);
        if (y < 1000.0f) {
            *(f32*)((u8*)self + 0xC4) = y + 200.0f;
        }
    }
    *(s32*)((u8*)self + 0x440) = *(s32*)((u8*)self + 0x440) + 1;
}

GLOBAL_ASM("asm/EnBw_Update.s")

typedef struct {
    f32 r, g, b, a;
} EnBwColorF;

extern Vec3f EnBw_sIceOffsets[];
void FUN_00373bec(void* obj);
void FUN_00357388(Actor* actor, EnBwColorF* color, s32 arg2, s32 arg3);
void FUN_00371fac(MTX34* mtx, void* arg1);
u32 PauseContext_GetState(void);
void EffectSsEnIce_SpawnFlyingVec3f(PlayState* play, Actor* actor, Vec3f* pos, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                    s32 g, f32 scale);
s32 FUN_00233910(PlayState* play, s32 limbIndex, MTX34* mtx, EnBw* self);

#if 0 // EnBw_Draw: far (234 words; size 1032 vs 1060): SetUnkAC-family (mov r6,#1 hoisted, per-multiply scale copies, MakeTrans int stores); GLOBAL_ASM stays
void EnBw_Draw(Actor* thisx, GameState* state) {
    EnBw* self = (EnBw*)thisx;
    PlayState* play = (PlayState*)state;
    Vector3f spAC(0.0f, 0.0f, 0.0f);
    EnBwColorF color;
    Vec3f t;
    MTX34 mtx;
    Vec3f icePos;
    s32 iceIndex = 0;

    FUN_00373bec(self->unk_430);
    FUN_00357fd0(GET_PLAYER(play), thisx->unk_178, &thisx->world.pos);
    color.r = self->unk_450 * (1.0f / 255.0f);
    color.g = self->unk_451 * (1.0f / 255.0f);
    color.b = self->unk_452 * (1.0f / 255.0f);
    color.a = self->unk_453 * (1.0f / 255.0f);
    FUN_00357388(thisx, &color, 4, 0);
    SkelAnime_DrawOpa(&self->skelAnime, &thisx->modelMtx, (OverrideLimbDrawOpa)FUN_00233910, NULL, thisx, 0);
    if (((*(u32*)((u8*)play + 0x5BF4) + 1) & 3) == (u32)thisx->params) {
        spAC.z = thisx->scale.z * 375000.0f;
        VEC3TransformAsm((Vector3f*)&self->unk_488, &thisx->modelMtx, &spAC);
        spAC.z = thisx->scale.z * 150000.0f;
        VEC3TransformAsm((Vector3f*)&self->unk_494, &thisx->modelMtx, &spAC);
        spAC.x = thisx->scale.x * 550000.0f;
        VEC3TransformAsm((Vector3f*)&self->unk_4AC, &thisx->modelMtx, &spAC);
        spAC.x = -spAC.x;
        VEC3TransformAsm((Vector3f*)&self->unk_4A0, &thisx->modelMtx, &spAC);
    }
    t.x = thisx->world.pos.x;
    t.y = thisx->world.pos.y + (thisx->scale.y - 0.013f) * 1000.0f;
    t.z = thisx->world.pos.z;
    MTX34_MakeTrans(&mtx, &t);
    {
        f32 s = self->unk_46C * 0.01f;

        mtx.data[0][0] *= s;
        mtx.data[1][0] *= s;
        mtx.data[2][0] *= s;
        mtx.data[0][1] *= s;
        mtx.data[1][1] *= s;
        mtx.data[2][1] *= s;
        mtx.data[0][2] *= s;
        mtx.data[1][2] *= s;
        mtx.data[2][2] *= s;
    }
    FUN_00371fac(&mtx, (u8*)play + 0x2FC);
    if (PauseContext_GetState() != 0) {
        self->skelAnime.unk_28->unk_0C->SetAnimSpeed(0.0f);
        self->fireAnim->SetAnimSpeed(0.0f);
    } else {
        self->skelAnime.unk_28->unk_0C->SetAnimSpeed(1.0f);
        self->fireAnim->SetAnimSpeed(1.0f);
    }
    FUN_00373bec(self->fireAnim);
    self->fireModel->SetUnkAC(1);
    self->fireModel->FUN_003721e0(&mtx);
    self->fireModel->FUN_00372170(0);
    if (self->iceTimer != 0) {
        *(u16*)((u8*)self + 0x11A) = *(u16*)((u8*)self + 0x11A) + 1;
        self->iceTimer--;
        if ((self->iceTimer & 5) == 0) {
            if (self->iceTimer != 0) {
                iceIndex = (s32)(0.5f + (self->iceTimer * (1.0f / 3.0f)) * 2.0f);
            }
            iceIndex = iceIndex >> 2;
            icePos.x = thisx->world.pos.x + EnBw_sIceOffsets[iceIndex].x;
            icePos.y = thisx->world.pos.y + EnBw_sIceOffsets[iceIndex].y;
            icePos.z = thisx->world.pos.z + EnBw_sIceOffsets[iceIndex].z;
            EffectSsEnIce_SpawnFlyingVec3f(play, thisx, &icePos, 150, 150, 150, 250, 235, 245, 255, 1.3f);
        }
    }
}
#endif

GLOBAL_ASM("asm/EnBw_Draw.s")
}
