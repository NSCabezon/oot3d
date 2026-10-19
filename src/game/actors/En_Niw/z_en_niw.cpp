extern "C" {
#include "z_en_niw.hpp"

#define FLAGS 0x00800010

void EnNiw_Init(Actor* thisx, PlayState* play);
void EnNiw_Destroy(Actor* thisx, PlayState* play);
void EnNiw_Update(Actor* actor, GameState* state);
void EnNiw_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_Niw.data.s")

GLOBAL_ASM("../src/game/actors/En_Niw/data_syms.s")

ActorInit En_Niw_InitVars = {
    ACTOR_EN_NIW,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_NIW,
    sizeof(EnNiw),
    (ActorFunc)EnNiw_Init,
    (ActorFunc)EnNiw_Destroy,
    (ActorFunc)EnNiw_Update,
    (ActorFunc)EnNiw_Draw,
};

void FUN_0013ebe0(EnNiw* self, PlayState* play);

void OnePointCutscene_Init(PlayState* play, s32 csId, s32 duration, Actor* actor, s32 camId);
void FUN_002885f0(void* self, PlayState* play);
#define R_UPDATE_RATE SREG(30)

void FUN_0038a788(EnNiw* self, PlayState* play) {
    OnePointCutscene_Init(play, 0x8F2, -99, &self->actor, 0);
    *(s16*)((u8*)self + 0x5DC) = (s16)(300.0f / R_UPDATE_RATE + 0.5f);
    *(u16*)((u8*)self + 0x620) = 1;
    *(void**)((u8*)self + 0x5D0) = (void*)FUN_002885f0;
}

void FUN_00239004(EnNiw* self) {
    if (self->actor.bgCheckFlags & 1) {
        *(s16*)((u8*)self + 0x622) = (s16)Rand_ZeroFloat(3.99f) + 5;
        *(void**)((u8*)self + 0x5D0) = (void*)FUN_0013ebe0;
    }
}

void FUN_00345420(EnNiw* self, PlayState* play);
void FUN_003631d0(EnNiw* self, PlayState* play, s32 arg);
void FUN_0020594c(EnNiw* self, PlayState* play);
void FUN_001e9aac(EnNiw* self, PlayState* play);
void EffectSsGRipple_Spawn(PlayState* play, Vec3f* pos, s16 radius, s16 radiusMax, s16 life);

#define EN_NIW_U16(s, off) (*(u16*)((u8*)(s) + (off)))
#define EN_NIW_S16(s, off) (*(s16*)((u8*)(s) + (off)))
#define EN_NIW_ACTIONFUNC(s) (*(void**)((u8*)(s) + 0x5D0))

void FUN_0019f930(EnNiw* self, PlayState* play) {
    Vec3f pos;

    if (EN_NIW_U16(self, 0x626) != 0) {
        FUN_00345420(self, play);
    }

    self->actor.speedXZ = 2.0f;

    if (self->actor.bgCheckFlags & 0x20) {
        self->actor.gravity = 0.0f;

        if (self->actor.yDistToWater > 15.0f) {
            self->actor.world.pos.y += 2.0f;
        }
        if (EN_NIW_S16(self, 0x5DA) == 0) {
            EN_NIW_S16(self, 0x5DA) = 30 * 3.0f / R_UPDATE_RATE + 0.5f;
            Math_Vec3f_Copy(&pos, &self->actor.world.pos);
            pos.y += self->actor.yDistToWater;
            EffectSsGRipple_Spawn(play, &pos, 100, 500, 30);
        }
        if (self->actor.bgCheckFlags & 8) {
            self->actor.velocity.y = 10.0f;
            self->actor.speedXZ = 1.0f;
        }
    } else {
        self->actor.gravity = -2.0f;

        if (self->actor.bgCheckFlags & 8) {
            self->actor.velocity.y = 10.0f;
            self->actor.speedXZ = 1.0f;
            self->actor.gravity = 0.0f;
        } else {
            self->actor.speedXZ = 4.0f;
        }
        if (self->actor.bgCheckFlags & 1) {
            self->actor.velocity.y = 0.0f;
            self->actor.gravity = -2.0f;
            EN_NIW_S16(self, 0x5DE) = 300.0f / R_UPDATE_RATE + 0.5f;
            EN_NIW_S16(self, 0x5DA) = 0;
            if (EN_NIW_U16(self, 0x626) == 0) {
                EN_NIW_ACTIONFUNC(self) = (void*)FUN_001e9aac;
            } else {
                EN_NIW_ACTIONFUNC(self) = (void*)FUN_0020594c;
            }
        }
    }

    FUN_003631d0(self, play, 2);
}

#if 0 // Init draft: 9 words off (callee-saved numbering in the Kakariko loop: ROM table r8 / flags sb / self+0x600 ip / gSaveContext r3; ours sb / r3 / r8 / ip). Mutations tried: play2 aliases (best mask), thisx on Kill/SetCylinder, flagList ptr
void* ZAR_GetCMBByIndex(void* zar, s32 index);
void FUN_00353e78(void* zar, PlayState* play, SkelAnime* skel, void* cmb, void* p, s32 anim, void* jointTable,
                  void* morphTable, s32 limbCount);
extern u8 EnNiw_sInitChain;
extern ColliderCylinderInit EnNiw_sCylinderInit1;
extern ColliderCylinderInit EnNiw_sCylinderInit2;

typedef struct {
    Vec3f pos[7];
    s16 flags[7];
} EnNiwKakarikoTable;
extern EnNiwKakarikoTable EnNiw_sKakarikoPosList;

extern __pure float _fabsf(float);
static __inline __pure float EnNiw_Fabsf(float f) {
    return _fabsf(f);
}

void EnNiw_Init(Actor* thisx, PlayState* play) {
    EnNiw* self = (EnNiw*)thisx;
    s32 i;
    PlayState* play2 = play;

    *((u8*)self + 0xB94) = 0;
    if (play->sceneNum == 0x20) {
        *((u8*)self + 0xB94) = 1;
    }

    if (self->actor.params < 0) {
        self->actor.params = 0;
    }

    if (self->actor.params == 0xB) {
        if (*((u8*)play2 + 0x7F5E)) {
            Actor_Kill(thisx);
            return;
        }
        *((u8*)play2 + 0x7F5E) = 1;
        self->actor.room = -1;
    }

    if (self->actor.params == 0xC) {
        if (*((u8*)play2 + 0x7F5F)) {
            Actor_Kill(thisx);
            return;
        }
        *((u8*)play2 + 0x7F5F) = 1;
        self->actor.room = -1;
    }

    Actor_ProcessInitChain(&self->actor, (InitChainEntry*)&EnNiw_sInitChain);
    self->actor.flags |= 1;
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 25.0f);
    {
        ObjectStatus* status = play2->GetObjectStatus(self->actor.objBankIndex);
        void* zar = &status->zarInfo;
        void* cmb = ZAR_GetCMBByIndex(zar, 0);

        FUN_00353e78(zar, play, (SkelAnime*)((u8*)self + 0x1A4), cmb, self->actor.unk_178, 0, (u8*)self + 0x228, (u8*)self + 0x3FC, 9);
    }
    for (i = 0; i < 20; i++) {
        FUN_00372f38(&self->actor, play, (u8*)self + i * 60 + 0x71C, 1, 0);
    }

    if (play->sceneNum == 0x52) {
        s16* flagList = EnNiw_sKakarikoPosList.flags;

        for (i = 0; i < 7; i++) {
            if (EnNiw_Fabsf(self->actor.world.pos.x - EnNiw_sKakarikoPosList.pos[i].x) < 40.0f &&
                EnNiw_Fabsf(self->actor.world.pos.z - EnNiw_sKakarikoPosList.pos[i].z) < 40.0f) {
                EN_NIW_S16(self, 0x628) = i;
                if (gSaveContext.infTable[25] & flagList[i]) {
                    self->actor.world.pos.x = 300.0f;
                    self->actor.world.pos.y = 100.0f;
                    self->actor.world.pos.z = 1530.0f;
                    self->actor.params = 0;
                }
            }
        }
    }

    Math_Vec3f_Copy((Vec3f*)((u8*)self + 0x62C), &self->actor.world.pos);
    Math_Vec3f_Copy((Vec3f*)((u8*)self + 0x638), &self->actor.world.pos);
    *(f32*)((u8*)self + 0x684) = 10.0f;
    Actor_SetScale(&self->actor, 0.01f);
    EN_NIW_S16(self, 0x622) = (s16)Rand_ZeroFloat(3.99f) + 5;

    if (EN_NIW_S16(self, 0x622) < 0) {
        EN_NIW_S16(self, 0x622) = 1;
    }

    switch (self->actor.params) {
        case 2:
            if (gSaveContext.nightFlag == 0) {
                Actor_Kill(&self->actor);
            }
            break;
        case 1:
            if (gSaveContext.eventChkInf[1] & 0x10) {
                Actor_Kill(&self->actor);
            }
            break;
        case 3:
            if (!(gSaveContext.eventChkInf[1] & 0x10)) {
                Actor_Kill(&self->actor);
            }
            break;
        case 5:
            if (gSaveContext.eventChkInf[1] & 0x100) {
                Actor_Kill(&self->actor);
            }
            break;
        case 7:
            if (!(gSaveContext.eventChkInf[1] & 0x100)) {
                Actor_Kill(&self->actor);
            }
            break;
        case 0xD:
            self->actor.gravity = 0.0f;
        case 0xE:
            self->actor.colChkInfo.mass = 0;
            self->actor.flags &= ~1;
            break;
        case 4:
            self->actor.gravity = 0.0f;
            break;
    }

    Collider_InitCylinder(play2, (ColliderCylinder*)((u8*)self + 0x68C));

    switch (self->actor.params) {
        case 0xA:
            self->actor.colChkInfo.mass = 0xFF;
        case 0xD:
        case 0xE:
            Collider_SetCylinder(play2, (ColliderCylinder*)((u8*)self + 0x68C), thisx, &EnNiw_sCylinderInit2);
            if (play->sceneNum == 0x34 && !(gSaveContext.eventChkInf[1] & 0x4000)) {
                Actor_Kill(thisx);
            }
            break;
        default:
            Collider_SetCylinder(play2, (ColliderCylinder*)((u8*)self + 0x68C), thisx, &EnNiw_sCylinderInit1);
            break;
    }

    EN_NIW_ACTIONFUNC(self) = (void*)FUN_0013ebe0;
}
#endif
GLOBAL_ASM("asm/EnNiw_Init.s")

void EnNiw_Destroy(Actor* thisx, PlayState* play) {
    EnNiw* self = (EnNiw*)thisx;
    s32 i;

    Collider_DestroyCylinder(play, (ColliderCylinder*)((u8*)self + 0x68C));
    SkelAnime_Free2((SkelAnime*)((u8*)self + 0x1A4));
    for (i = 0; i < 20; i++) {
        FUN_00350f34(&self->actor, (u8*)self + i * 60 + 0x71C, 0);
    }
}

GLOBAL_ASM("asm/EnNiw_Update.s")

#if 0 // Draw draft: far (136 words; stack layout mtx@0 overlapping the outgoing-arg slots + const remat of 0/1.0 in r7/r8 vs ours; MakeTrans/MakeScale family)
#include "model_manager.hpp"
#include "mtx34_trans.hpp"

extern "C++" {
float sinf(float);
float cosf(float);
}
void FUN_00357878(Actor* actor, Vec3f* pos, Vec3f* scale, s32 alpha, PlayState* play);
void FUN_0019c4ac(EnNiw* self, PlayState* play);
s32 EnNiw_OverrideLimbDraw(void* arg0, s32 limbIndex, MTX34* mtx, Actor* thisx);

typedef struct {
    /* 0x00 */ u8 type;
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ Vec3f vel;
    /* 0x1C */ Vec3f accel;
    /* 0x28 */ f32 unk_28;
    /* 0x2C */ f32 scale;
    /* 0x30 */ f32 rot;
    /* 0x34 */ s32 unk_34;
    /* 0x38 */ SkeletonAnimationModel* model;
} EnNiwFeather; // size = 0x3C

void EnNiw_Draw(Actor* thisx, GameState* state) {
    EnNiw* self = (EnNiw*)thisx;
    PlayState* play = (PlayState*)state;
    s16 i;
    Vector3f shadowScale(0.15f, 0.15f, 0.15f);
    EnNiwFeather* feather = (EnNiwFeather*)((u8*)self + 0x6E4);

    SkelAnime_DrawOpa((SkelAnime*)((u8*)self + 0x1A4), &self->actor.modelMtx, (OverrideLimbDrawOpa)EnNiw_OverrideLimbDraw, NULL, thisx, 0);
    if (EN_NIW_ACTIONFUNC(self) == (void*)FUN_0019c4ac) {
        FUN_00357878(thisx, &self->actor.world.pos, (Vec3f*)&shadowScale, 255, play);
    }
    if (*((u8*)self + 0xB94) != 0) {
        FUN_00357878(thisx, &self->actor.world.pos, (Vec3f*)&shadowScale, 255, play);
    }
    for (i = 0; i < 20; i++, feather++) {
        if (feather->type == 1) {
            Vec3f pos;
            Vec3f off;
            MTX34 mtx;
            f32 a;

            MTX34CopyAsm(&mtx, &self->actor.modelMtx);
            pos.x = feather->pos.x;
            pos.y = feather->pos.y;
            pos.z = feather->pos.z;
            MTX34_MakeTrans(&mtx, &pos);
            mtx.data[0][0] *= feather->scale;
            mtx.data[1][0] *= feather->scale;
            mtx.data[2][0] *= feather->scale;
            mtx.data[0][1] *= feather->scale;
            mtx.data[1][1] *= feather->scale;
            mtx.data[2][1] *= feather->scale;
            a = feather->rot;
            if (a != 0.0f) {
                f32 sn = sinf(a);
                f32 cs = cosf(a);
                f32 t0 = mtx.data[0][0], t1 = mtx.data[0][1];
                mtx.data[0][0] = t0 * cs + t1 * sn;
                mtx.data[0][1] = t1 * cs - t0 * sn;
                t0 = mtx.data[1][0]; t1 = mtx.data[1][1];
                mtx.data[1][0] = t0 * cs + t1 * sn;
                mtx.data[1][1] = t1 * cs - t0 * sn;
                t0 = mtx.data[2][0]; t1 = mtx.data[2][1];
                mtx.data[2][0] = t0 * cs + t1 * sn;
                mtx.data[2][1] = t1 * cs - t0 * sn;
            }
            off.x = 0.0f;
            off.y = -1000.0f;
            off.z = 0.0f;
            Matrix_UpdatePosition(&mtx, &mtx, &off);
            feather->model->SetUnkAC(1);
            feather->model->FUN_003721e0(&mtx);
            feather->model->FUN_00372170(0);
        }
    }
}
#endif
GLOBAL_ASM("asm/EnNiw_Draw.s")

void FUN_003625f8(MTX34* dst, f32 angle, Vec3f* axis);
void FUN_0036c174(MTX34* dst, MTX34* a, MTX34* b);
void FUN_0032e438(MTX34* dst, Vec3f* rot);

#if 0 // OverrideLimbDraw draft: 9 words off (zero const lands in s1 + vmov copy s2; ROM keeps zero in s2 + copy s1; ~35 variants)
static inline void EnNiw_SetVec(Vec3f* v, f32 x, f32 y, f32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

s32 EnNiw_OverrideLimbDraw(void* arg0, s32 limbIndex, MTX34* mtx, Actor* thisx) {
    EnNiw* self = (EnNiw*)thisx;
    MTX34 rot;
    Vec3f vec;
    f32 zero = 0.0f;
    f32 k = 9.58738019107841e-05f;

    if (limbIndex == 7) {
        EnNiw_SetVec(&vec, zero, zero, -1.0f);
        FUN_003625f8(&rot, self->unk_65C * k, &vec);
        FUN_0036c174(mtx, mtx, &rot);
    } else if (limbIndex == 8) {
        EnNiw_SetVec(&vec, zero, 1.0f, zero);
        FUN_003625f8(&rot, self->unk_660 * k, &vec);
        FUN_0036c174(mtx, mtx, &rot);
    } else if (limbIndex == 4) {
        EnNiw_SetVec(&vec, self->unk_658 * -k, self->unk_654 * -k, self->unk_650 * k);
        FUN_0032e438(&rot, &vec);
        FUN_0036c174(mtx, mtx, &rot);
    } else if (limbIndex == 6) {
        EnNiw_SetVec(&vec, self->unk_64C * k, self->unk_648 * k, self->unk_644 * k);
        FUN_0032e438(&rot, &vec);
        FUN_0036c174(mtx, mtx, &rot);
    }
    return 0;
}
#endif
GLOBAL_ASM("asm/EnNiw_OverrideLimbDraw.s")

}
