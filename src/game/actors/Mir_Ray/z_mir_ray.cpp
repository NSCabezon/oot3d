extern "C" {
#include "z_mir_ray.hpp"

#define FLAGS 0x00000030

void MirRay_Init(Actor* actor, GameState* state);
void MirRay_Destroy(MirRay* self, PlayState* play);
void MirRay_Update(Actor* actor, GameState* state);
void MirRay_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Mir_Ray.data.s")
GLOBAL_ASM("../src/game/actors/Mir_Ray/data_syms.s")
extern MirRayDataEntry MirRay_sMirRayData[];
extern u8 MirRay_sUnk;
f32 FUN_00340404(Vec3f* a, Vec3f* b);
s32 FUN_00339474(Vec3f* vecA, Vec3f* vecB, f32 pointx, f32 pointy, f32 pointz, s16 radiusA, s16 radiusB);
void FUN_0034a928(Actor* actor, s32 sfxId);
void Lights_PointNoGlowSetInfo(void* light, f32 x, f32 y, f32 z, u8 r, u8 g, u8 b, s16 radius, s32 pad);
void Lights_PointSetColorAndRadius(void* lightInfo, u8 r, u8 g, u8 b, s16 radius, s32 arg6);
s32 Math_StepToS(s16* pValue, s16 target, s16 step);
void LightContext_RemoveLight(PlayState* play, void* lightCtx, void* lightNode);
void Collider_DestroyJntSph(PlayState* play, void* collider);
s32 FUN_0034eaf8(PlayState* play, void* quad); // Collider_DestroyQuad (C stub in En_GeldB TU, inlined)
void FUN_00340ac8(Player* player, s32 arg1, Vec3f* arg2, Vec3f* arg3, f32 arg4); // z_mir_ray_beam.cpp


ActorInit Mir_Ray_InitVars = {
    ACTOR_MIR_RAY,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_MIR_RAY,
    sizeof(MirRay),
    (ActorFunc)MirRay_Init,
    (ActorFunc)MirRay_Destroy,
    (ActorFunc)MirRay_Update,
    (ActorFunc)MirRay_Draw,
};

GLOBAL_ASM("asm/MirRay_Init.s")

void MirRay_Destroy(MirRay* self, PlayState* play) {
    Actor* thisx = &self->actor;
    s32 i;

    LightContext_RemoveLight(play, (u8*)play + 0xA70, *(void**)((u8*)self + 0x304));
    if (MirRay_sMirRayData[thisx->params].params & 2) {
        Collider_DestroyJntSph(play, (u8*)self + 0x1A4);
    }
    FUN_0034eaf8(play, (u8*)self + 0x214);
    for (i = 0; i < 1; i++) {
        FUN_003508b8(thisx, *(void**)((u8*)self + 0x328 + i * 4), NULL);
    }
    if (*(Player**)((u8*)play + 0x20AC) != NULL) {
        FUN_00340ac8(*(Player**)((u8*)play + 0x20AC), 0, NULL, NULL, 0.0f);
    }
}



#if 0 // MirRay_Update draft: ~46 words off (needs sUnk/sMirRayData as adjacent real statics: ROM derives add r7,r1,#0x70; plus fp scheduling in SetupCollider/MakeShieldLight)
static inline void MirRay_SetupCollider(MirRay* self) {
    Vec3f colliderOffset;
    MirRayDataEntry* dataEntry = &MirRay_sMirRayData[self->actor.params];

    colliderOffset.x = (self->poolPt.x - self->sourcePt.x) * dataEntry->unk_10;
    colliderOffset.y = (self->poolPt.y - self->sourcePt.y) * dataEntry->unk_10;
    colliderOffset.z = (self->poolPt.z - self->sourcePt.z) * dataEntry->unk_10;
    self->jntSphElems->center.x = colliderOffset.x + self->sourcePt.x;
    self->jntSphElems->center.y = colliderOffset.y + self->sourcePt.y;
    self->jntSphElems->center.z = colliderOffset.z + self->sourcePt.z;
    self->jntSphElems->radius = dataEntry->unk_14 * self->jntSphElems->scale;
}

static inline void MirRay_MakeShieldLight(MirRay* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    MirRayDataEntry* dataEntry = &MirRay_sMirRayData[self->actor.params];
    Vec3f reflectionPt;
    s16 lightPtX;
    s16 lightPtY;
    s16 lightPtZ;

    if (FUN_00339474(&self->sourcePt, &self->poolPt, player->actor.world.pos.x, player->actor.world.pos.y + 30.0f,
                     player->actor.world.pos.z, self->sourceEndRad, self->poolEndRad)) {
        if (dataEntry->params & 8) {
            reflectionPt.x = player->actor.world.pos.x - self->sourcePt.x;
            reflectionPt.y = player->actor.world.pos.y - self->sourcePt.y;
            reflectionPt.z = player->actor.world.pos.z - self->sourcePt.z;
        } else {
            reflectionPt.x = self->poolPt.x - self->sourcePt.x;
            reflectionPt.y = self->poolPt.y - self->sourcePt.y;
            reflectionPt.z = self->poolPt.z - self->sourcePt.z;
        }
        lightPtX = (dataEntry->unk_18 * reflectionPt.x) + self->sourcePt.x;
        lightPtY = (dataEntry->unk_18 * reflectionPt.y) + self->sourcePt.y;
        lightPtZ = (dataEntry->unk_18 * reflectionPt.z) + self->sourcePt.z;

        Math_StepToS(&self->lightPointRad, dataEntry->lgtPtMaxRad, 6);
        Lights_PointNoGlowSetInfo(self->lightInfo, lightPtX, lightPtY, lightPtZ, dataEntry->color.r, dataEntry->color.g,
                                  dataEntry->color.b, self->lightPointRad, 0);
    } else {
        Math_StepToS(&self->lightPointRad, 0, 6);
        Lights_PointSetColorAndRadius(self->lightInfo, dataEntry->color.r, dataEntry->color.g, dataEntry->color.b,
                                      self->lightPointRad, 0);
    }
}

void MirRay_Update(MirRay* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    MirRay_sUnk = 0;

    if (!self->unLit) {
        if (MirRay_sMirRayData[self->actor.params].params & 2) {
            if (MirRay_sMirRayData[self->actor.params].params & 4) {
                MirRay_SetupCollider(self);
            }
            CollisionCheck_SetAT(play, &play->colChkCtx, (Collider*)self->colliderJntSph);
        }
        if (self->reflectIntensity > 0.0f) {
            CollisionCheck_SetAT(play, &play->colChkCtx, (Collider*)((u8*)self + 0x214));
        }
        MirRay_MakeShieldLight(self, play);
        if (self->reflectIntensity > 0.0f) {
            FUN_0034a928(&player->actor, 0x0100013B);
        }
    }
}

#endif
GLOBAL_ASM("asm/MirRay_Update.s")

GLOBAL_ASM("asm/MirRay_Draw.s")

#if 0 // MirRay_Init draft (~145 words off: ROM reloads sourcePt for the Lights call, no forwarding; needs MirRay_Update proto change)

extern u8 MirRay_sJntSphInit[];
extern u8 MirRay_sQuadInit[];
void Collider_InitJntSph(PlayState* play, void* collider);
void Collider_SetJntSph(PlayState* play, void* collider, Actor* actor, void* init, void* elements);
void Collider_InitQuad(PlayState* play, void* collider);
s32 Collider_SetQuad(PlayState* play, void* collider, Actor* actor, void* src);
void* LightContext_InsertLight(PlayState* play, void* lightCtx, void* info);
void* ZAR_GetCMBByIndex(void* zar, s32 index);

void MirRay_Init(MirRay* self, PlayState* play) {
    MirRayDataEntry* dataEntry = &MirRay_sMirRayData[self->actor.params];
    ZARInfo* zar;

    Actor_ProcessInitChain(&self->actor, (InitChainEntry*)(MirRay_sMirRayData + 10));
    ActorShape_Init(&self->actor.shape, 0.0f, NULL, 0.0f);

    if (self->actor.params >= 0xA) {
        Actor_Kill(&self->actor);
    }

    self->sourcePt.x = dataEntry->sourcePoint.x;
    self->sourcePt.y = dataEntry->sourcePoint.y;
    self->sourcePt.z = dataEntry->sourcePoint.z;
    self->sourceEndRad = dataEntry->sourceEndRadius;

    self->poolPt.x = dataEntry->poolPoint.x;
    self->poolPt.y = dataEntry->poolPoint.y;
    self->poolPt.z = dataEntry->poolPoint.z;
    self->poolEndRad = dataEntry->poolEndRadius;

    Lights_PointNoGlowSetInfo(self->lightInfo, (s16)self->sourcePt.x, (s16)self->sourcePt.y, (s16)self->sourcePt.z, 255,
                              255, 255, 100, 0);
    self->lightNode = LightContext_InsertLight(play, (u8*)play + 0xA70, self->lightInfo);

    self->shieldCorners[0].x = -536.0f;
    self->shieldCorners[0].y = -939.0f;

    self->shieldCorners[1].x = -1690.0f;
    self->shieldCorners[1].y = 0.0f;

    self->shieldCorners[2].x = -536.0f;
    self->shieldCorners[2].y = 938.0f;

    self->shieldCorners[3].x = 921.0f;
    self->shieldCorners[3].y = 0.0f;

    self->shieldCorners[4].x = 758.0f;
    self->shieldCorners[4].y = 800.0f;

    self->shieldCorners[5].x = 758.0f;
    self->shieldCorners[5].y = -800.0f;

    if (dataEntry->params & 2) {
        Collider_InitJntSph(play, self->colliderJntSph);
        Collider_SetJntSph(play, self->colliderJntSph, &self->actor, MirRay_sJntSphInit, self->jntSphElemStorage);
        if (!(dataEntry->params & 4)) {
            MirRay_SetupCollider(self);
        }
    }

    Collider_InitQuad(play, self->shieldRay);
    Collider_SetQuad(play, self->shieldRay, &self->actor, MirRay_sQuadInit);

    if ((self->actor.params == 5) || (self->actor.params == 7) || (self->actor.params == 8)) {
        self->actor.room = -1;
    }

    zar = FUN_00372f38(&self->actor, play, self->model, 0, 0);
    self->actor.unk_19A = 1;
    self->cmb = ZAR_GetCMBByIndex(zar, 0);
}
#endif

void FUN_003fcbf8(void* ptr) {
}

#if 0 // draft, does not match yet
void FUN_0010004c(Vec3f* dst, f32 (*src)[4]) {
    Vec3f a = *(Vec3f*)src[0];
    Vec3f b = *(Vec3f*)src[1];
    Vec3f c = *(Vec3f*)src[2];
    dst[0] = a;
    dst[1] = b;
    dst[2] = c;
}
#endif
GLOBAL_ASM("asm/FUN_0010004c.s")

#if 0 // draft: 18 words off (sqrt dist operand order, 2nd sp10C block s0/s1 numbering); also needs the MTX34_Identity guard pinned (orchestrator) before it can be enabled
#include "mtx34_identity.hpp"
void FUN_00359450(void* player, MTX34* out);
void FUN_0035479c(void* quad, Vec3f* a, Vec3f* b, Vec3f* c, Vec3f* d);
s32 Math3D_LineSegVsPlane(f32 nx, f32 ny, f32 nz, f32 dist, Vec3f* a, Vec3f* b, Vec3f* out, s32 flag);

typedef struct CollPolyLocal {
    /* 0x00 */ char unk_00[0x0A];
    /* 0x0A */ s16 nx;
    /* 0x0C */ s16 ny;
    /* 0x0E */ s16 nz;
    /* 0x10 */ f32 dist;
} CollPolyLocal;

typedef struct MirRayReflection {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ MTX34 mtx;
    /* 0x3C */ CollPolyLocal* reflectionPoly;
    /* 0x40 */ u8 opacity;
} MirRayReflection; // size = 0x44

void FUN_00150d58(MirRay* self, PlayState* play, MirRayReflection* reflection) {
    Player* player = GET_PLAYER(play);
    s32 i;
    f32 temp_f0;
    Vec3f vecB;
    Vec3f vecD;
    Vec3f sp118;
    Vec3f sp10C;
    Vec3f sp100;
    Vec3f intersection;
    f32 spE8_0, spE8_1, spE8_2;
    f32 polyNormal[3];
    MTX34 shieldMtx;

    FUN_00359450(player, &shieldMtx);

    spE8_0 = -(shieldMtx.data[0][2] * self->reflectRange) * self->reflectIntensity * 400.0f;
    spE8_1 = -(shieldMtx.data[1][2] * self->reflectRange) * self->reflectIntensity * 400.0f;
    spE8_2 = -(shieldMtx.data[2][2] * self->reflectRange) * self->reflectIntensity * 400.0f;

    vecB.x = shieldMtx.data[0][3];
    vecB.y = shieldMtx.data[1][3];
    vecB.z = shieldMtx.data[2][3];

    vecD.x = vecB.x + spE8_0;
    vecD.y = vecB.y + spE8_1;
    vecD.z = vecB.z + spE8_2;

    {
        Vec3f vecA;
        Vec3f vecC;

        vecA.x = vecB.x + (shieldMtx.data[0][0] * 300.0f);
        vecA.y = vecB.y + (shieldMtx.data[1][0] * 300.0f);
        vecA.z = vecB.z + (shieldMtx.data[2][0] * 300.0f);

        vecC.x = vecD.x + (shieldMtx.data[0][0] * 300.0f);
        vecC.y = vecD.y + (shieldMtx.data[1][0] * 300.0f);
        vecC.z = vecD.z + (shieldMtx.data[2][0] * 300.0f);

        FUN_0035479c(self->shieldRay, &vecA, &vecB, &vecC, &vecD);

        for (i = 0; i < 6; i++) {
            if (reflection[i].reflectionPoly != NULL) {
                polyNormal[0] = reflection[i].reflectionPoly->nx * (1.0f / 32767.0f);
                polyNormal[1] = reflection[i].reflectionPoly->ny * (1.0f / 32767.0f);
                polyNormal[2] = reflection[i].reflectionPoly->nz * (1.0f / 32767.0f);

                if (Math3D_LineSegVsPlane(polyNormal[0], polyNormal[1], polyNormal[2],
                                          reflection[i].reflectionPoly->dist, &vecB, &vecD, &sp118, 1)) {
                    reflection[i].pos.x = sp118.x;
                    reflection[i].pos.y = sp118.y;
                    reflection[i].pos.z = sp118.z;

                    temp_f0 = sqrtf(SQ(sp118.x - vecB.x) + SQ(sp118.y - vecB.y) + SQ(sp118.z - vecB.z));

                    if (temp_f0 < (self->reflectIntensity * 600.0f)) {
                        reflection[i].opacity = 200;
                    } else {
                        reflection[i].opacity = (s32)(800.0f - temp_f0);
                    }

                    sp10C.x = (shieldMtx.data[0][0] * 100.0f) + vecB.x;
                    sp10C.y = (shieldMtx.data[1][0] * 100.0f) + vecB.y;
                    sp10C.z = (shieldMtx.data[2][0] * 100.0f) + vecB.z;

                    sp100.x = (spE8_0 * 4.0f) + sp10C.x;
                    sp100.y = (spE8_1 * 4.0f) + sp10C.y;
                    sp100.z = (spE8_2 * 4.0f) + sp10C.z;

                    reflection[i].mtx = MTX34_Identity();

                    if (Math3D_LineSegVsPlane(polyNormal[0], polyNormal[1], polyNormal[2],
                                              reflection[i].reflectionPoly->dist, &sp10C, &sp100, &intersection, 1)) {
                        reflection[i].mtx.data[0][0] = intersection.x - sp118.x;
                        reflection[i].mtx.data[1][0] = intersection.y - sp118.y;
                        reflection[i].mtx.data[2][0] = intersection.z - sp118.z;
                    }

                    sp10C.x = (shieldMtx.data[0][1] * 100.0f) + vecB.x;
                    sp10C.y = (shieldMtx.data[1][1] * 100.0f) + vecB.y;
                    sp10C.z = (shieldMtx.data[2][1] * 100.0f) + vecB.z;

                    sp100.x = (spE8_0 * 4.0f) + sp10C.x;
                    sp100.y = (spE8_1 * 4.0f) + sp10C.y;
                    sp100.z = (spE8_2 * 4.0f) + sp10C.z;

                    if (Math3D_LineSegVsPlane(polyNormal[0], polyNormal[1], polyNormal[2],
                                              reflection[i].reflectionPoly->dist, &sp10C, &sp100, &intersection, 1)) {
                        reflection[i].mtx.data[0][1] = intersection.x - sp118.x;
                        reflection[i].mtx.data[1][1] = intersection.y - sp118.y;
                        reflection[i].mtx.data[2][1] = intersection.z - sp118.z;
                    }
                } else {
                    reflection[i].reflectionPoly = NULL;
                }
            }
        }
    }
}
#endif
GLOBAL_ASM("asm/FUN_00150d58.s")

s32 FUN_00288394(PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (((u8*)player)[0x1B5] == 0xA && ((u8*)player)[0x1A6] == 3) {
        return 1;
    }
    return 0;
}

#if 0 // draft: 4 words off (only r7/r8 callee-saved numbering of chkWall/chkCeil)
s32 BgCheck_LineTestImpl(CollisionContext* colCtx, s32 type, s32 unk2, Vec3f* posA, Vec3f* posB, Vec3f* posResult,
                         f32 unk6, CollisionPoly** poly, s32* bgId, Actor* actor, s32 flags);

static s32 MirRay_GetBccFlags(s32 chkWall, s32 chkFloor, s32 chkCeil, s32 chkOneFace) {
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

s32 FUN_003393ec(CollisionContext* colCtx, Vec3f* posA, Vec3f* posB, Vec3f* posResult, CollisionPoly** poly,
                 s32 chkWall, s32 chkFloor, s32 chkCeil, s32 chkOneFace, s32* bgId) {
    s32 f = MirRay_GetBccFlags(chkWall, chkFloor, chkCeil, chkOneFace);
    s32* b = bgId;
    return BgCheck_LineTestImpl(colCtx, 0, 0, posA, posB, posResult, 1.0f, poly, b, NULL, f);
}
#endif
GLOBAL_ASM("asm/FUN_003393ec.s")

f32 FUN_00340404(Vec3f* a, Vec3f* b);

s32 FUN_00339474(Vec3f* vecA, Vec3f* vecB, f32 pointx, f32 pointy, f32 pointz, s16 radiusA, s16 radiusB) {
    f32 coneRadius;
    f32 closestPtx;
    f32 closestPty;
    f32 closestPtz;
    f32 dx, dy, dz;
    f32 dist;
    Vec3f sp5C;
    Vec3f sp50;
    Vec3f sp44;

    dx = vecB->x - vecA->x;
    dy = vecB->y - vecA->y;
    dz = vecB->z - vecA->z;

    dist = SQ(dx) + SQ(dy) + SQ(dz);

    if (dist == 0.0f) {
        return 0;
    }

    dist = (((pointx - vecA->x) * dx) + ((pointy - vecA->y) * dy) + ((pointz - vecA->z) * dz)) / dist;

    closestPtx = (dx * dist) + vecA->x;
    closestPty = (dy * dist) + vecA->y;
    closestPtz = (dz * dist) + vecA->z;

    coneRadius = ((radiusB - radiusA) * dist) + radiusA;

    if ((SQ(closestPtx - pointx) + SQ(closestPty - pointy) + SQ(closestPtz - pointz)) <= SQ(coneRadius)) {
        sp5C.x = vecB->x - vecA->x;
        sp5C.y = vecB->y - vecA->y;
        sp5C.z = vecB->z - vecA->z;

        sp50.x = pointx - vecA->x;
        sp50.y = pointy - vecA->y;
        sp50.z = pointz - vecA->z;

        if (FUN_00340404(&sp5C, &sp50) < 0.0f) {
            return 0;
        }

        sp44.x = pointx - vecB->x;
        sp44.y = pointy - vecB->y;
        sp44.z = pointz - vecB->z;

        if (FUN_00340404(&sp5C, &sp44) > 0.0f) {
            return 0;
        }
        return 1;
    }
    return 0;
}



}
