extern "C" {
#include "z_en_test.hpp"

#define FLAGS 0x00000015

void EnTest_Init(Actor* thisx, PlayState* play);
void EnTest_Destroy(Actor* thisx, PlayState* play);
void EnTest_Update(Actor* thisx, PlayState* play);
void EnTest_Draw(Actor* thisx, PlayState* play);

void FUN_0034e01c(MTX34* mtx, Vec3s* rot);

GLOBAL_ASM("data/z_En_Test.data.s")

GLOBAL_ASM("../src/game/actors/En_Test/data_syms.s")

extern u8 EnTest_sBodyColliderInit[];
extern u8 EnTest_sShieldColliderInit[];
extern u8 EnTest_sSwordColliderInit[];
extern u8 EnTest_sDamageTable[];
extern InitChainEntry EnTest_sInitChain[];

ActorInit En_Test_InitVars = {
    ACTOR_EN_TEST,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_SK2,
    sizeof(EnTest),
    (ActorFunc)EnTest_Init,
    (ActorFunc)EnTest_Destroy,
    (ActorFunc)EnTest_Update,
    (ActorFunc)EnTest_Draw,
};

#define STALFOS_LIMB_NECK_UPPER 12
#define STALFOS_TYPE_2 2
#define STALFOS_TYPE_CEILING 3

s32 FUN_001c43f8(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnTest* self = (EnTest*)thisx;

    if (limbIndex == STALFOS_LIMB_NECK_UPPER) {
        FUN_0034e01c(mtx, &self->headRot);
    }
    return 0;
}

s32 BodyBreak_SpawnParts(Actor* actor, void* bodyBreak, PlayState* play, s32 params, s32 zero);
void Item_DropCollectibleRandom(PlayState* play, Actor* actor, Vec3f* pos, s32 params);

void FUN_002954a8(EnTest* self, PlayState* play) {
    self->actor.params = 1;

    if (BodyBreak_SpawnParts(&self->actor, self->bodyBreak, play, self->actor.params, 0)) {
        Item_DropCollectibleRandom(play, &self->actor, &self->actor.world.pos, 0xD0);

        if (self->actor.parent != NULL) {
            self->actor.parent->home.rot.z--;
        }

        Actor_Kill(&self->actor);
    }
}

s32 Effect_Add(PlayState* play, s32* pIndex, s32 type, u8 arg3, u8 arg4, void* initParams);
void Collider_InitQuad(PlayState* play, void* collider);
void Collider_SetQuad(PlayState* play, void* collider, Actor* actor, void* init);
void* ZAR_GetCMBByIndex(void* zar, s32 index);
void SkelAnime_InitLink(SkelAnime* skel, void* model, PlayState* play, void* cmb, void* a, s32 limbCount, s32 b,
                        void* joint, void* morph, s32 c);
void FUN_00341268(SkelAnime* skel, void* model, PlayState* play, void* cmb, s32 limbCount, s32 b, void* joint,
                  void* morph, s32 c);
void Animation_PlayLoop(SkelAnime* skelAnime, s32 animIndex);
void ActorShadow_DrawFeet(Actor* actor, Lights* lights, PlayState* play);

// 3DS EffectBlure init params (N64 EffectBlureInit1). The vertex array element has an
// out-of-line empty constructor in ROM (FUN_003fd780, _ZN15BlureInitVertexC1Ev in shared_syms.s).
struct BlureInitVertex {
    BlureInitVertex();
    u8 data[0x24];
};

typedef struct {
    /* 0x000 */ s32 unk_00;
    /* 0x004 */ BlureInitVertex vtx[16];
    /* 0x244 */ u8 p1StartColor[4];
    /* 0x248 */ u8 p2StartColor[4];
    /* 0x24C */ u8 p1EndColor[4];
    /* 0x250 */ u8 p2EndColor[4];
    /* 0x254 */ s32 elemDuration;
    /* 0x258 */ s32 unkFlag;
    /* 0x25C */ u8 calcMode;
    /* 0x260 */ s32 unk_260;
    /* 0x264 */ u8 unk_264;
} EnTestBlureInit; // size = 0x268

void FUN_0039b768(EnTest* self, PlayState* play);
void FUN_0039d8cc(EnTest* self, PlayState* play);

static inline void EnTest_SetupWaitGround(EnTest* self) {
    Animation_PlayLoop(&self->skelAnime, 18);
    self->actor.scale.y = 0.0f;
    self->unk_1C4C = 0;
    self->timer = 23;
    self->actor.world.pos.y = self->actor.home.pos.y - 3.5f;
    self->actor.flags &= ~1;
    self->actionFunc = FUN_0039b768;
}

static inline void EnTest_SetupWaitAbove(EnTest* self) {
    Animation_PlayLoop(&self->skelAnime, 18);
    self->actor.world.pos.y = self->actor.home.pos.y + 150.0f;
    self->unk_1C4C = 0;
    Actor_SetScale(&self->actor, 0.0f);
    self->actor.flags &= ~1;
    self->actionFunc = FUN_0039d8cc;
}

void EnTest_Init(Actor* thisx, PlayState* play) {
    EnTestBlureInit slashBlure;
    EnTest* self = (EnTest*)thisx;
    ZARInfo* zar;
    void* cmb;
    EnTest* s2 = self;

    Actor_ProcessInitChain(&self->actor, EnTest_sInitChain);

    zar = FUN_00372f38(&self->actor, play, &self->unk_1DC0, 0, 0);
    cmb = ZAR_GetCMBByIndex(zar, 0);
    SkelAnime_InitLink(&self->skelAnime, zar, play, cmb, self->actor.unk_178, 18, 1, (u8*)self + 0x2E8,
                       (u8*)self + 0x7FC, 25);
    self->skelAnime.unk_75[0] = 0;

    zar = FUN_00372f38(&self->actor, play, &self->unk_1DC4, 0, 0);
    FUN_00341268(&self->skelAnime2, zar, play, cmb, 18, 1, (u8*)self + 0x1224, (u8*)self + 0x1738, 25);
    self->skelAnime2.unk_75[0] = 0;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawFeet, 90.0f);

    self->actor.colChkInfo.cylRadius = 40;
    self->actor.colChkInfo.cylHeight = 100;
    self->actor.focus.pos = self->actor.world.pos;
    self->actor.focus.pos.y += 45.0f;
    self->actor.colChkInfo.damageTable = (DamageTable*)EnTest_sDamageTable;
    *(u16*)((u8*)self + 0x1DC8) = 0;

    Collider_InitCylinder(play, &self->shieldCollider);
    Collider_SetCylinder(play, &self->shieldCollider, &self->actor, (ColliderCylinderInit*)EnTest_sBodyColliderInit);

    Collider_InitCylinder(play, (ColliderCylinder*)self->swordCollider);
    Collider_SetCylinder(play, (ColliderCylinder*)self->swordCollider, &self->actor,
                         (ColliderCylinderInit*)EnTest_sShieldColliderInit);

    Collider_InitQuad(play, &self->bodyCollider);
    Collider_SetQuad(play, &self->bodyCollider, &self->actor, EnTest_sSwordColliderInit);

    self->actor.colChkInfo.mass = 0xFE;
    self->actor.colChkInfo.health = 10;

    slashBlure.p1StartColor[0] = slashBlure.p1StartColor[1] = slashBlure.p1StartColor[2] = slashBlure.p1StartColor[3] =
        slashBlure.p2StartColor[0] = slashBlure.p2StartColor[1] = slashBlure.p2StartColor[2] =
            slashBlure.p1EndColor[0] = slashBlure.p1EndColor[1] = slashBlure.p1EndColor[2] = slashBlure.p2EndColor[0] =
                slashBlure.p2EndColor[1] = slashBlure.p2EndColor[2] = 255;

    slashBlure.p1EndColor[3] = 0;
    slashBlure.p2EndColor[3] = 0;
    slashBlure.p2StartColor[3] = 64;

    slashBlure.elemDuration = 4;
    slashBlure.unkFlag = 0;
    slashBlure.calcMode = 2;
    slashBlure.unk_264 = 0x15;

    Effect_Add(play, &self->effectIndex, 1, 0, 0, &slashBlure);

    if (self->actor.params != STALFOS_TYPE_CEILING) {
        EnTest_SetupWaitGround(self);
    } else {
        EnTest_SetupWaitAbove(self);
    }

    if (s2->actor.params == 0) {
        self->actor.flags |= 0x80;
    }
}


Actor* Actor_FindNearby(PlayState* play, Actor* actor, s32 actorId, s32 actorCat, f32 dist);
void Effect_Delete(PlayState* play, s32 index);
void FUN_00373d0c(PlayState* play);
void FUN_0034eaf8(PlayState* play, void* collider); // Collider_DestroyQuad
void FUN_003504d0(Actor* actor, void* bodyBreak);

void EnTest_Destroy(Actor* thisx, PlayState* play) {
    EnTest* self = (EnTest*)thisx;

    if ((self->actor.params != STALFOS_TYPE_2) &&
        (Actor_FindNearby(play, &self->actor, ACTOR_EN_TEST, ACTORCAT_ENEMY, 8000.0f) == NULL)) {
        FUN_00373d0c(play);
    }

    Effect_Delete(play, self->effectIndex);
    FUN_0034eaf8(play, self->swordCollider);
    Collider_DestroyCylinder(play, &self->shieldCollider);
    Collider_DestroyCylinder(play, &self->bodyCollider);
    FUN_003504d0(&self->actor, self->bodyBreak);
    FUN_003508b8(&self->actor, self->unk_1DC0, 0);
    SkelAnime_Free2(&self->skelAnime);
    FUN_003508b8(&self->actor, self->unk_1DC4, 0);
    SkelAnime_Free2(&self->skelAnime2);
}

void* FUN_00362384(s32 index);
void FUN_0035eb74(void* blure);
void FUN_0034f184(void* bodyBreak, s32 limbIndex, s32 a, s32 b, s32 c, MTX34* mtx);
void FUN_0035479c(void* quad, Vector3f* v0, Vector3f* v1, Vector3f* v2, Vector3f* v3);
void FUN_003620f0(void* blure, Vec3f* p1, Vec3f* p2);
void FUN_0034cbb4(Actor* actor, MTX34* mtx, s32 limbIndex, s32 footL, Vector3f* footLPos, s32 ankleR,
                  Vector3f* ankleRPos);
void FUN_0036f00c(PlayState* play, Actor* actor, Vec3f* pos, f32 a, s32 b, f32 c, s32 d, s32 e, s32 f);

#if 0 // far (~180w incl. numbering shift) + needs 8 guarded function-local statics pinned (guards 0x52F620..0x52F63C, objs 0x59B674..0x59B6C8). ROM spills mtx (push {r0-r3}) and keeps self+0x1000/+0x1c00 bases in r6/r7
void FUN_001c4418(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    static Vector3f unused1(1100.0f, -700.0f, 0.0f);
    static Vector3f D_80864658(300.0f, 0.0f, 0.0f);
    static Vector3f D_80864664(3400.0f, 0.0f, 0.0f);
    static Vector3f D_80864670(0.0f, 0.0f, 0.0f);
    static Vector3f D_8086467C(7000.0f, 1000.0f, 0.0f);
    static Vector3f D_80864688(3000.0f, -2000.0f, -1000.0f);
    static Vector3f D_80864694(3000.0f, -2000.0f, 1000.0f);
    static Vector3f D_808646A0(-1300.0f, 1100.0f, 0.0f);
    s32 bodyPart = -1;
    Vec3f sp70;
    Vec3f sp64;
    EnTest* self = (EnTest*)thisx;
    Vec3f sp50;

    FUN_0034f184(self->bodyBreak, limbIndex, 0, 0x18, 0x19, mtx);

    if (limbIndex == 9) {
        VEC3TransformAsm((Vector3f*)((u8*)self + 0x1D34), mtx, &D_8086467C);
        VEC3TransformAsm((Vector3f*)((u8*)self + 0x1D28), mtx, &D_80864688);
        VEC3TransformAsm((Vector3f*)((u8*)self + 0x1D4C), mtx, &D_80864694);
        VEC3TransformAsm((Vector3f*)((u8*)self + 0x1D40), mtx, &D_808646A0);

        FUN_0035479c((u8*)self + 0x1CE8, (Vector3f*)((u8*)self + 0x1D28), (Vector3f*)((u8*)self + 0x1D34),
                     (Vector3f*)((u8*)self + 0x1D40), (Vector3f*)((u8*)self + 0x1D4C));

        VEC3TransformAsm((Vector3f*)&sp70, mtx, &D_80864664);
        VEC3TransformAsm((Vector3f*)&sp64, mtx, &D_80864670);

        if ((self->swordState >= 1) && ((self->actor.params != 0) || *((u8*)play + 0x208F))) {
            FUN_003620f0(FUN_00362384(self->effectIndex), &sp70, &sp64);
        } else if (self->swordState >= 0) {
            FUN_0035eb74(FUN_00362384(self->effectIndex));
            self->swordState = -1;
        }
    } else if ((limbIndex == 5) && (self->unk_1C62 != 0)) {
        VEC3TransformAsm((Vector3f*)&sp64, mtx, &D_80864670);
        *(Vec3f*)((u8*)self + 0x1DB4) = sp64;
    } else {
        FUN_0034cbb4(&self->actor, mtx, limbIndex, 0x14, &D_80864658, 0x18, &D_80864658);

        if ((limbIndex == 0x14) || (limbIndex == 0x18)) {
            if ((self->unk_1C4C == 0x15) || (self->unk_1C4C == 0x16)) {
                if (self->actor.speedXZ != 0.0f) {
                    VEC3TransformAsm((Vector3f*)&sp64, mtx, &D_80864658);
                    FUN_0036f00c(play, &self->actor, &sp64, 10.0f, 1, 8.0f, 100, 15, 0);
                }
            }
        }
    }

    if (self->iceTimer != 0) {
        switch (limbIndex) {
            case 0xC:
                bodyPart = 0;
                break;
            case 2:
                bodyPart = 1;
                break;
            case 9:
                bodyPart = 2;
                break;
            case 5:
                bodyPart = 3;
                break;
            case 7:
                bodyPart = 4;
                break;
            case 3:
                bodyPart = 5;
                break;
            case 0xF:
                bodyPart = 6;
                break;
            case 0x14:
                bodyPart = 7;
                break;
            case 0x18:
                bodyPart = 8;
                break;
        }

        if (bodyPart >= 0) {
            VEC3TransformAsm((Vector3f*)&sp50, mtx, &D_80864670);

            self->bodyPartsPos[bodyPart].x = sp50.x;
            self->bodyPartsPos[bodyPart].y = sp50.y;
            self->bodyPartsPos[bodyPart].z = sp50.z;
        }
    }
}
#endif

GLOBAL_ASM("asm/EnTest_Update.s")

void FUN_001c4418(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void GraphicsContext_UnknownStub(GraphicsContext* gfxCtx);
void FUN_00347d24(PlayState* play, Actor* actor, Vec3s* pos, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g,
                  f32 scale);

void FUN_002842d8(EnTest* self, PlayState* play);
void FUN_00284508(EnTest* self, PlayState* play);
void FUN_0034f724(PlayState* play);

void Animation_PlayOnceSetSpeed(SkelAnime* skelAnime, s32 animIndex, f32 speed);
void Audio_PlayActorSound2(Actor* actor, s32 sfxId);
void FUN_002b433c(EnTest* self, PlayState* play);

void FUN_002842d8(EnTest* self, PlayState* play) {
    Animation_PlayOnceSetSpeed(&self->skelAnime, 3, 0.0f);
    FUN_003731e0(&self->skelAnime);

    if (self->actor.world.pos.y <= self->actor.floorHeight) {
        self->skelAnime.playSpeed = 1.0f;
        self->unk_1C4C = 12;

        self->timer = ((s32)(self->unk_1C68 * 0.15f) > 0)
                          ? (s32)(0.5f + ((s32)(self->unk_1C68 * 0.15f) * 3.0f) * 0.5f)
                          : (s32)(((s32)(self->unk_1C68 * 0.15f) * 3.0f) * 0.5f - 0.5f);

        Audio_PlayActorSound2(&self->actor, 0x10002EA);
        self->actionFunc = FUN_002b433c;
    }
}


void FUN_0039b768(EnTest* self, PlayState* play) {
    FUN_00370734(&self->skelAnime);

    if ((self->timer == 0) && (ABS(self->actor.yDistToPlayer) < 150.0f)) {
        self->unk_1C4C = 3;
        self->actionFunc = FUN_00284508;
        self->actor.shape.rot.y = self->actor.world.rot.y = self->actor.yawTowardsPlayer;

        if (self->actor.params != STALFOS_TYPE_2) {
            FUN_0034f724(play);
        }
    } else {
        if (self->timer != 0) {
            self->timer--;
        }

        self->actor.world.pos.y = self->actor.home.pos.y - 3.5f;
    }
}

void FUN_0039d8cc(EnTest* self, PlayState* play) {
    FUN_00370734(&self->skelAnime);
    self->actor.world.pos.y = self->actor.home.pos.y + 150.0f;

    if ((self->actor.xzDistToPlayer < 200.0f) && (ABS(self->actor.yDistToPlayer) < 450.0f)) {
        self->actionFunc = FUN_002842d8;
        self->actor.shape.rot.y = self->actor.world.rot.y = self->actor.yawTowardsPlayer;
        self->actor.flags |= 1;
        Actor_SetScale(&self->actor, 0.015f);
    }
}

void EnTest_Draw(Actor* thisx, PlayState* play) {
    EnTest* self = (EnTest*)thisx;

    GraphicsContext_UnknownStub(play->state.gfxCtx);

    if ((thisx->params < 4) || (thisx->child == NULL)) {
        FUN_00357fd0(GET_PLAYER(play), thisx->unk_178, &thisx->world.pos);
        SkelAnime_DrawOpa(&self->skelAnime, &thisx->modelMtx, FUN_001c43f8, FUN_001c4418, thisx, 0);
    }

    if (self->iceTimer != 0) {
        *(u16*)((u8*)thisx + 0x11A) += 1;
        self->iceTimer--;

        if ((self->iceTimer % 4) == 0) {
            s32 iceIndex = self->iceTimer >> 2;

            FUN_00347d24(play, thisx, &self->bodyPartsPos[iceIndex], 150, 150, 150, 250, 235, 245, 255, 1.5f);
        }
    }
}

s32 FUN_0035ea64(Vec3f* pos, f32 cx, f32 cy, f32 cz, f32 hx, f32 hy, f32 hz) {
    s32 ret = 0;
    f32 dx = pos->x - cx;
    f32 dy = pos->y - cy;
    f32 dz = pos->z - cz;

    if (ABS(dx) < hx && ABS(dy) < hy && ABS(dz) < hz) {
        if (ABS(dx / hx) > ABS(dz / hz)) {
            if (ABS(dx) < hx) {
                pos->x = (dx < 0.0f) ? (cx - hx) : (cx + hx);
            }
        } else {
            if (ABS(dz) < hz) {
                pos->z = (dz < 0.0f) ? (cz - hz) : (cz + hz);
            }
        }
        ret = 1;
    }
    return ret;
}

void FUN_0035f090(Actor* actor);
void FUN_0035ed68(EnTest* self, PlayState* play);
void FUN_003231b8(EnTest* self);
s32 BodyBreak_SpawnParts(Actor* actor, void* bodyBreak, PlayState* play, s32 params, s32 zero);
void Item_DropCollectibleRandom(PlayState* play, Actor* actor, Vec3f* pos, s32 params);

#if 0 // 3 words off: ROM predicates the home.rot.x store (strheq), ours stores unconditionally + mov r3/add order
void FUN_0026e080(Actor* thisx, PlayState* play) {
    EnTest* self = (EnTest*)thisx;

    if (thisx->child == NULL) {
        s16 hx = thisx->home.rot.x;
        thisx->home.rot.x = (hx == 0) ? *(u16*)((u8*)self + 0x1C7C) : hx;
        if (BodyBreak_SpawnParts(thisx, self->bodyBreak, play, (s16)(thisx->params + 8), 0)) {
            thisx->child = thisx;
        }
    } else if (thisx->home.rot.x == 0) {
        thisx->colChkInfo.health = 10;

        if (thisx->params == 4) {
            thisx->params = -1;
        } else {
            Actor_ChangeType(play, (ActorContext*)((u8*)play + 0x208C), thisx, 5);
        }

        thisx->child = NULL;
        thisx->flags |= 1;
        FUN_0035f090(thisx);
    } else {
        if (thisx->params != 5) {
            return;
        }

        if (Actor_FindNearby(play, thisx, ACTOR_EN_TEST, ACTORCAT_ENEMY, 8000.0f) == NULL) {
            Item_DropCollectibleRandom(play, thisx, &thisx->world.pos, 0xD0);

            if (thisx->parent != NULL) {
                thisx->parent->home.rot.z--;
            }

            Actor_Kill(thisx);
        }
    }
}
#endif


void BodyBreak_Alloc(void* bodyBreak, s32 count, PlayState* play);

void FUN_003928cc(EnTest* self, PlayState* play) {
    if (FUN_00370734(&self->skelAnime)) {
        s32 t;
        if ((s32)(Rand_ZeroOne() * 10.0f + 10.0f) > 0) { t = (s32)(0.5f + ((s32)(Rand_ZeroOne() * 10.0f + 10.0f) * 3.0f) * 0.5f); } else { t = (s32)(((s32)(Rand_ZeroOne() * 10.0f + 10.0f) * 3.0f) * 0.5f - 0.5f); }
        self->timer = t;
        self->unk_1C4C = 7;
        self->actionFunc = FUN_002954a8;
        BodyBreak_Alloc(self->bodyBreak, 0x19, play);
    }

    if (Animation_OnFrameImpl(&self->skelAnime, 6.0f, 1.0f)) {
        Audio_PlayActorSound2(&self->actor, 0x10002EA);
    }
}



void FUN_002a6968(EnTest* self, PlayState* play) {
    if (FUN_00370734(&self->skelAnime)) {
        if (Rand_ZeroOne() > 0.7f) {
            FUN_003231b8(self);
            self->timer = ((s32)(Rand_ZeroOne() * 5.0f + 5.0f) > 0)
                              ? (s32)(0.5f + ((s32)(Rand_ZeroOne() * 5.0f + 5.0f) * 3.0f) * 0.5f)
                              : (s32)(((s32)(Rand_ZeroOne() * 5.0f + 5.0f) * 3.0f) * 0.5f - 0.5f);
        } else {
            if ((*(u32*)((u8*)play + 0x5BF4) & 1) && (self->actor.params != 3)) {
                FUN_0035f090(&self->actor);
            } else {
                FUN_0035ed68(self, play);
            }
        }
    }
}


s32 FUN_00328e08(PlayState* play, EnTest* self);
void FUN_003b77e4(EnTest* self, PlayState* play);
void FUN_0035f10c(EnTest* self);
void FUN_0031eb78(EnTest* self);

void FUN_0039b5f4(EnTest* self, PlayState* play) {
    PlayState* play2 = play;
    Player* player = GET_PLAYER(play2);

    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 0.1f, 0.0f);

    if (FUN_00370734(&self->skelAnime)) {
        self->actor.speedXZ = 0.0f;

        if (!FUN_00328e08(play, self)) {
            FUN_003b77e4(self, play);
        } else {
            return;
        }
    }

    u8* pl = (u8*)player + 0x2200;

    if (pl[0x27] == 0) {
        return;
    }

    if ((self->actor.bgCheckFlags & 8) &&
        ((ABS((s16)(self->actor.wallYaw - self->actor.shape.rot.y)) < 0x38A4) &&
         (self->actor.xzDistToPlayer < 80.0f))) {
        FUN_0031eb78(self);
    } else if ((Rand_ZeroOne() > 0.7f) && (self->actor.params != STALFOS_TYPE_CEILING) &&
               (pl[0x26] != 0x11)) {
        FUN_0035f090(&self->actor);
    } else {
        if (*(u16*)((u8*)self + 0x1DC8) != 0) {
            FUN_003b77e4(self, play);
        } else {
            FUN_0035f10c(self);
        }
    }

    self->unk_1C4C = 8;
}


#if 0 // 69 words off: ROM keeps 10.0f master in s19 (copies s20/s1 for Rand math), ours renumbers s16-s20
void FUN_00387fec(EnTest* self, PlayState* play) {
    if (FUN_00370734(&self->skelAnime)) {
        s32 t;
        if ((s32)(Rand_ZeroOne() * 10.0f + 10.0f) > 0) { t = (s32)(0.5f + ((s32)(Rand_ZeroOne() * 10.0f + 10.0f) * 3.0f) * 0.5f); } else { t = (s32)(((s32)(Rand_ZeroOne() * 10.0f + 10.0f) * 3.0f) * 0.5f - 0.5f); }
        self->timer = t;
        self->unk_1C4C = 7;
        self->actionFunc = FUN_002954a8;
        BodyBreak_Alloc(self->bodyBreak, 0x19, play);
    }

    if (Animation_OnFrameImpl(&self->skelAnime, 10.0f, 1.0f) || Animation_OnFrameImpl(&self->skelAnime, 22.0f, 1.0f)) {
        Audio_PlayActorSound2(&self->actor, 0x10002EA);
    }
}
#endif


void FUN_0026e080(Actor* thisx, PlayState* play);
void Animation_PlayOnce(SkelAnime* skelAnime, s32 animIndex);
void* FUN_00362384(s32 index);
void FUN_0035eb74(void* blure);

void FUN_0037fdfc(EnTest* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 1.0f, 0.0f);

    if (*(u16*)((u8*)self + 0x11A) == 0) {
        if (self->actor.colChkInfo.health == 0) {
            Animation_PlayOnce(&self->skelAnime, 9);
            Audio_PlayActorSound2(&self->actor, 0x10002AB);
            self->actor.speedXZ = 0.0f;
            self->unk_1C62 = 0;
            self->actor.flags &= ~1;
            *(u16*)((u8*)self + 0x11A) = 0;

            if (self->actor.params < 4) {
                self->unk_1C4C = 5;
                self->actionFunc = FUN_003928cc;
            } else {
                self->unk_1C4C = 2;
                BodyBreak_Alloc(self->bodyBreak, 0x19, play);
                self->actor.home.rot.x = 0;

                if (self->swordState >= 0) {
                    FUN_0035eb74(FUN_00362384(self->effectIndex));
                    self->swordState = -1;
                }

                self->actor.flags &= ~1;

                if (self->actor.params == 5) {
                    Actor_ChangeType(play, (ActorContext*)((u8*)play + 0x208C), &self->actor, 6);
                }

                self->actionFunc = (EnTestActionFunc)FUN_0026e080;
            }
        } else {
            u8* pl = (u8*)player + 0x2200;

            if (pl[0x27] != 0) {
                Actor* a = &self->actor;
                if ((self->actor.bgCheckFlags & 8) &&
                    ((ABS((s16)(a->wallYaw - self->actor.shape.rot.y)) < 0x38A4) &&
                     (self->actor.xzDistToPlayer < 80.0f))) {
                    FUN_0031eb78(self);
                } else if ((Rand_ZeroOne() > 0.7f) && (pl[0x26] != 0x11)) {
                    FUN_0035f090(&self->actor);
                } else {
                    if (*(u16*)((u8*)self + 0x1DC8) != 0) {
                        FUN_003b77e4(self, play);
                    } else {
                        FUN_0035f10c(self);
                    }
                }

                self->unk_1C4C = 8;
            } else {
                self->actor.speedXZ = 0.0f;

                if (!FUN_00328e08(play, self)) {
                    FUN_003b77e4(self, play);
                }
            }
        }
    }
}

void FUN_0039d978(EnTest* self, PlayState* play) {
    PlayState* play2 = play;
    Player* player = GET_PLAYER(play2);

    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 0.1f, 0.0f);

    if (FUN_00370734(&self->skelAnime)) {
        self->actor.speedXZ = 0.0f;

        if ((self->actor.bgCheckFlags & 8) &&
            ((ABS((s16)(self->actor.wallYaw - self->actor.shape.rot.y)) < 0x38A4) &&
             (self->actor.xzDistToPlayer < 80.0f))) {
            FUN_0031eb78(self);
        } else if (!FUN_00328e08(play, self)) {
            FUN_003b77e4(self, play);
        } else {
            return;
        }
    }

    u8* pl = (u8*)player + 0x2200;

    if (pl[0x27] == 0) {
        return;
    }

    if ((self->actor.bgCheckFlags & 8) &&
        ((ABS((s16)(self->actor.wallYaw - self->actor.shape.rot.y)) < 0x38A4) &&
         (self->actor.xzDistToPlayer < 80.0f))) {
        FUN_0031eb78(self);
    } else if ((Rand_ZeroOne() > 0.7f) && (self->actor.params != STALFOS_TYPE_CEILING) &&
               (pl[0x26] != 0x11)) {
        FUN_0035f090(&self->actor);
    } else {
        if (*(u16*)((u8*)self + 0x1DC8) != 0) {
            FUN_003b77e4(self, play);
        } else {
            FUN_0035f10c(self);
        }
    }

    self->unk_1C4C = 8;
}

}
