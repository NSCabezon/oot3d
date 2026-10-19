extern "C" {
#include "z_en_boom.hpp"

#define FLAGS 0x00000030

void EnBoom_Init(Actor* actor, PlayState* play);
void EnBoom_Destroy(Actor* actor, PlayState* play);
void EnBoom_Update(Actor* actor, PlayState* play);
void EnBoom_Draw(Actor* actor, PlayState* play);
void EnBoom_Fly(EnBoom* self, PlayState* play);
void FUN_00128f0c(EnBoom* self, PlayState* play);
void Effect_Delete(PlayState* play, s32 index);
void FUN_0034eaf8(PlayState* play, void* collider);
s32 Effect_Add(PlayState* play, s32* pIndex, s32 type, u8 arg3, u8 arg4, void* initParams);
void Collider_InitQuad(PlayState* play, void* collider);
void Collider_SetQuad(PlayState* play, void* collider, Actor* actor, void* init);

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
} EnBoomBlureInit; // size = 0x268

GLOBAL_ASM("data/z_En_Boom.data.s")

GLOBAL_ASM("../src/game/actors/En_Boom/data_syms.s")

extern InitChainEntry EnBoom_sInitChain[];

extern u8 EnBoom_sQuadInit[];

ActorInit En_Boom_InitVars = {
    ACTOR_EN_BOOM,
    ACTORCAT_MISC,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnBoom),
    (ActorFunc)EnBoom_Init,
    (ActorFunc)EnBoom_Destroy,
    (ActorFunc)EnBoom_Update,
    (ActorFunc)EnBoom_Draw,
};

#define PLAYER_FLAGS3(p) (*(u32*)((u8*)(p) + 0x29B8))
extern f32 EnBoom_sOffsetScale;
extern f32 EnBoom_sCatchFrames;
extern f32 EnBoom_sTrailLength;
f32 __hardfp_sinf(f32 x);
f32 __hardfp_cosf(f32 x);
f32* FUN_003478bc(void* model, s32 limbIndex);
void* FUN_00362384(s32 index);
void FUN_003620f0(void* blure, Vec3f* p1, Vec3f* p2);
s32 FUN_0033a480(PlayState* play, void* collider, void* weaponInfo, Vec3f* posA, Vec3f* posB);
void Matrix_RotateY(MTX34* mtx, s32 mode, f32 angle);
void VEC3TransformAsm(Vector3f* dest, MTX34* mtx, Vector3f* src);
void MTX34CopyAsm(MTX34* dst, MTX34* src);
s16 Math_ScaledStepToS_s16(s16*, s16, s16);
s32 Math_ScaledStepToS(s16* pValue, s16 target, s16 step);
s16 Actor_WorldYawTowardPoint(Actor* actor, Vec3f* point);
s16 Actor_WorldPitchTowardPoint(Actor* actor, Vec3f* point);
void FUN_0036c648(Actor* actor, f32 speed);
s32 BgCheck_EntityLineTest1(void* colCtx, Vec3f* posA, Vec3f* posB, Vec3f* posResult, CollisionPoly** outPoly, s32 checkWall,
                            s32 checkFloor, s32 checkCeil, s32 checkOneFace, s32* bgId);
s32 FUN_00314c68(PlayState* play, Actor* actor, CollisionPoly* poly, s32 bgId, Vec3f* pos);
DynaPolyActor* DynaPoly_GetActor(void* colCtx, s32 bgId);
s32 FUN_00314c14(void* colCtx, CollisionPoly* poly, s32 bgId);
void FUN_00314c28(PlayState* play, Vec3f* pos, s32 a);
void FUN_00375f90(PlayState* play, Vec3f* pos, s32 a);
void FUN_0033af2c(PlayState* play, Vec3f* pos, s32 a);

static inline void EnBoom_SetupAction(EnBoom* self, EnBoomActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void EnBoom_Init(Actor* thisx, PlayState* play) {
    EnBoom* self = (EnBoom*)thisx;
    Player* player;

    thisx->unk_19A = 1;

    {
        EnBoomBlureInit blureInit;

        self->actor.room = -1;
        Actor_ProcessInitChain(&self->actor, EnBoom_sInitChain);
        FUN_00372f38(thisx, play, &self->model, 3, 0);

        blureInit.p1StartColor[0] = 255;
        blureInit.p1StartColor[1] = 255;
        blureInit.p1StartColor[2] = 100;
        blureInit.p1StartColor[3] = 255;

        blureInit.p2StartColor[0] = 255;
        blureInit.p2StartColor[1] = 255;
        blureInit.p2StartColor[2] = 100;
        blureInit.p2StartColor[3] = 64;

        blureInit.p1EndColor[0] = 255;
        blureInit.p1EndColor[1] = 255;
        blureInit.p1EndColor[2] = 100;
        blureInit.p1EndColor[3] = 0;

        blureInit.p2EndColor[0] = 255;
        blureInit.p2EndColor[1] = 255;
        blureInit.p2EndColor[2] = 100;
        blureInit.p2EndColor[3] = 0;

        blureInit.elemDuration = 8;
        blureInit.unkFlag = 0;
        blureInit.calcMode = 0;
        blureInit.unk_264 = 12;

        Effect_Add(play, &self->effectIndex, 1, 0, 0, &blureInit);
    }

    Collider_InitQuad(play, &self->collider);
    Collider_SetQuad(play, &self->collider, &self->actor, EnBoom_sQuadInit);

    player = GET_PLAYER(play);
    self->unk_270 = player->actor.world.pos;
    self->unk_27C = self->actor.params;

    EnBoom_SetupAction(self, EnBoom_Fly);
}

void FUN_00128f0c(EnBoom* self, PlayState* play) {
    self->unk_26F++;
    if (self->unk_26E == 3) {
        Actor_Kill(&self->actor);
    }
}

void EnBoom_Destroy(Actor* thisx, PlayState* play) {
    EnBoom* self = (EnBoom*)thisx;
    Player* player;

    FUN_00350f34(thisx, &self->model, 0);
    player = GET_PLAYER(play);
    if (player != NULL) {
        *(u32*)((u8*)player + 0x29B8) &= ~0x10000;
    }
    Effect_Delete(play, self->effectIndex);
    FUN_0034eaf8(play, &self->collider);
}

#if 0
// Near-miss (9 words, ~70 variants): ROM computes player+0x1000 (r1) and player+0x2000 (r0) bases before the
// stateFlags1 RMW (temp r2) and reuses r1 as the flags value; ours loads the RMW into r0 and the 0x2000 base lands in r2.
void EnBoom_Update(Actor* thisx, PlayState* play) {
    EnBoom* self = (EnBoom*)thisx;
    Player* player = GET_PLAYER(play);
    Actor* grabbed;
    u32 flags;
    Actor* a2;

    if (self->actor.type == 6) {
        if (self->actionFunc != FUN_00128f0c) {
            grabbed = self->grabbed;
            if (grabbed != NULL) {
                Math_Vec3f_Copy(&grabbed->world.pos, &player->actor.world.pos);
                if (grabbed->id == 0x15) {
                    grabbed->gravity = -0.9f;
                    grabbed->bgCheckFlags &= ~3;
                } else {
                    grabbed->flags &= ~0x2000;
                }
            }
            player->stateFlags1 &= ~0x2000000;
            flags = *(u32*)((u8*)player + 0x29B8) | 0x10000;
            *(u32*)((u8*)player + 0x29B8) = flags;
            if (flags & 0x1000) {
                *(u32*)((u8*)player + 0x29B8) = flags & ~0x1000;
                EnBoom_SetupAction(self, FUN_00128f0c);
                return;
            }
        }
        a2 = thisx;
        Actor_Kill(a2);
        return;
    }

    if (!(player->stateFlags1 & 0x20000000)) {
        self->actionFunc(self, play);
        Actor_SetFocus(&self->actor, 0.0f);
        self->activeTimer++;
    }
}
#endif

GLOBAL_ASM("asm/EnBoom_Update.s")

#if 0
// Draft, far (335 of 412 words, 1700 vs 1648 B): decoded from ROM, no N64 counterpart for the 3DS catch/trail branch (unk_26E state machine).
// Blockers: FOUR guarded function-local statics (guards 0x521338/0x52133C/0x521340/0x521344, objects 0x598F54/0x598F48/0x598F6C/0x598F60)
// plus two float constants share the actor .data block 0x521334..0x52134C (ROM reaches all of them through ONE base reg r7=0x521334) ->
// needs real statics in the TU + chunk split/pins (orchestrator).
void EnBoom_Draw(Actor* thisx, PlayState* play) {
    static Vector3f sPosAOffset(-960.0f, 0.0f, 0.0f);
    static Vector3f sPosBOffset(960.0f, 0.0f, 0.0f);
    EnBoom* self = (EnBoom*)thisx;
    Player* player = GET_PLAYER(play);
    Vec3f posA;
    Vec3f posB;
    s32 i;

    if (self->actor.type == 6) {
        return;
    }

    if (self->actionFunc == FUN_00128f0c) {
        static Vector3f sCatchPosA(600.0f, 1200.0f, 100.0f);
        static Vector3f sCatchPosB(-400.0f, 320.0f, -72.0f);
        MTX34 mtx;
        Vec3f p10;
        Vec3f p4;
        Vec3f p28;
        Vec3f p1c;

        if (self->unk_26E == 0) {
            self->unk_26E = 1;
            return;
        }

        if (self->unk_26E == 1) {
            f32 dx;
            f32 dy;
            f32 dz;
            f32 inv;
            f32 len;
            void* effect;

            MTX34CopyAsm(&mtx, (MTX34*)FUN_003478bc(*(void**)((u8*)player + 0x27C), 0x10));
            VEC3TransformAsm((Vector3f*)&p10, &mtx, &sCatchPosB);
            VEC3TransformAsm((Vector3f*)&p4, &mtx, &sCatchPosA);
            VEC3TransformAsm((Vector3f*)&p28, &self->actor.modelMtx, &sPosAOffset);
            VEC3TransformAsm((Vector3f*)&p1c, &self->actor.modelMtx, &sPosBOffset);

            dx = p28.x - p10.x;
            dy = p28.y - p10.y;
            dz = p28.z - p10.z;
            len = sqrtf(dx * dx + dy * dy + dz * dz);
            inv = 1.0f / len;
            p28.x = p10.x + dx * inv * EnBoom_sTrailLength;
            p28.y = p10.y + dy * inv * EnBoom_sTrailLength;
            p28.z = p10.z + dz * inv * EnBoom_sTrailLength;

            dx = p1c.x - p4.x;
            dy = p1c.y - p4.y;
            dz = p1c.z - p4.z;
            len = sqrtf(dx * dx + dy * dy + dz * dz);
            inv = 1.0f / len;
            p1c.x = p4.x + dx * inv * EnBoom_sTrailLength;
            p1c.y = p4.y + dy * inv * EnBoom_sTrailLength;
            p1c.z = p4.z + dz * inv * EnBoom_sTrailLength;

            effect = FUN_00362384(self->effectIndex);
            *((u8*)effect + 0x282) = 12;

            for (i = 0; i < 5; i++) {
                u8* elem = (u8*)effect + i * 0x24;

                posA.x = p28.x + ((f32)i * 0.2f) * (p10.x - p28.x);
                posA.y = p28.y + ((f32)i * 0.2f) * (p10.y - p28.y);
                posA.z = p28.z + ((f32)i * 0.2f) * (p10.z - p28.z);
                posB.x = p1c.x + ((f32)i * 0.2f) * (p4.x - p1c.x);
                posB.y = p1c.y + ((f32)i * 0.2f) * (p4.y - p1c.y);
                posB.z = p1c.z + ((f32)i * 0.2f) * (p4.z - p1c.z);

                elem[0] = 1;
                *(s32*)(elem + 4) = i + 1;
                *(Vec3f*)(elem + 8) = posA;
                *(Vec3f*)(elem + 0x14) = posB;
            }

            *((u8*)effect + 0x25E) = 5;
            self->unk_26E++;
            return;
        }

        if (self->unk_26E == 2) {
            if ((f32)self->unk_26F >= EnBoom_sCatchFrames) {
                self->unk_26E = 3;
            }
        }

        MTX34CopyAsm(&mtx, (MTX34*)FUN_003478bc(*(void**)((u8*)player + 0x27C), 0x10));
        VEC3TransformAsm((Vector3f*)&posA, &mtx, &sCatchPosB);
        VEC3TransformAsm((Vector3f*)&posB, &mtx, &sCatchPosA);
        *((u8*)FUN_00362384(self->effectIndex) + 0x282) = 12;
        FUN_003620f0(FUN_00362384(self->effectIndex), &posA, &posB);
    } else {
        f32* m = (f32*)&self->actor.modelMtx;
        f32 angle;
        f32 sinAngle;
        f32 cosAngle;
        f32 a;
        f32 b;

        Matrix_RotateY(&self->actor.modelMtx, 1, (f32)self->actor.world.rot.y * (M_PI / 32768.0f));

        angle = 0.7669903f;
        sinAngle = __hardfp_sinf(angle);
        cosAngle = __hardfp_cosf(angle);
        a = m[0]; b = m[1];
        m[0] = a * cosAngle + b * sinAngle;
        m[1] = b * cosAngle - a * sinAngle;
        a = m[4]; b = m[5];
        m[4] = a * cosAngle + b * sinAngle;
        m[5] = b * cosAngle - a * sinAngle;
        a = m[8]; b = m[9];
        m[8] = a * cosAngle + b * sinAngle;
        m[9] = b * cosAngle - a * sinAngle;

        angle = (f32)self->actor.world.rot.x * (M_PI / 32768.0f);
        if (angle != 0.0f) {
            sinAngle = __hardfp_sinf(angle);
            cosAngle = __hardfp_cosf(angle);
            a = m[1]; b = m[2];
            m[1] = a * cosAngle + b * sinAngle;
            m[2] = b * cosAngle - a * sinAngle;
            a = m[5]; b = m[6];
            m[5] = a * cosAngle + b * sinAngle;
            m[6] = b * cosAngle - a * sinAngle;
            a = m[9]; b = m[10];
            m[9] = a * cosAngle + b * sinAngle;
            m[10] = b * cosAngle - a * sinAngle;
        }

        VEC3TransformAsm((Vector3f*)&posA, &self->actor.modelMtx, &sPosAOffset);
        VEC3TransformAsm((Vector3f*)&posB, &self->actor.modelMtx, &sPosBOffset);

        if (FUN_0033a480(play, &self->collider, (u8*)self + 0x234, &posA, &posB)) {
            *((u8*)FUN_00362384(self->effectIndex) + 0x282) = 12;
            FUN_003620f0(FUN_00362384(self->effectIndex), &posA, &posB);
        }

        Matrix_RotateY(&self->actor.modelMtx, 1, (f32)(u32)(self->activeTimer * 12000) * (M_PI / 32768.0f));

        self->model->FUN_003721e0(&self->actor.modelMtx);
        self->model->SetUnkAC(1);
        self->model->FUN_00372170(0);
    }
}
#endif

GLOBAL_ASM("asm/EnBoom_Draw.s")

#if 0
// Draft: 73 of 373 words differ (checked with fastchk, unpinned). Structure (switch/cross-jump tail, tail-called Math_Vec3f_Copy) matches;
// open: guarded zero Vector3f static (guard 0x55A208 / obj 0x5BE26C, shared COMDAT with FUN_001cf9ac, FUN_003fb408,
// InitializeGyroscopeReaderConstants, FUN_002c0804 -> needs shared header + ld pin), VFP numbering of that init, stack-slot layout of
// point/offset/hitPoint/hitDynaID, play->sl / zero->sb callee-saved swap, mat-0 block layout.
void EnBoom_Fly(EnBoom* self, PlayState* play) {
    Actor* target;
    Player* player = GET_PLAYER(play);
    s32 collided;
    s16 yawTarget;
    s16 yawDiff;
    s16 pitchTarget;
    s16 pitchDiff;
    f32 distXYZScale;
    f32 distFromLink;

    target = self->moveTo;

    if (target != NULL) {
        f32 dx = player->actor.world.pos.x - self->unk_270.x;
        f32 dy = player->actor.world.pos.y - self->unk_270.y;
        f32 dz = player->actor.world.pos.z - self->unk_270.z;
        f32 moved = dx * dx + dy * dy + dz * dz;
        static Vector3f sZero(0.0f, 0.0f, 0.0f);
        Vector3f offset = sZero;
        Vec3f point;

        if (target == &player->actor && moved < 3.0f) {
            s16 yaw;

            if (self->unk_27C != 0) {
                yaw = player->actor.shape.rot.y + 0x38E;
            } else {
                yaw = player->actor.shape.rot.y - 0x38E;
            }
            offset.x = Math_SinS(yaw) * EnBoom_sOffsetScale;
            offset.z = Math_CosS(yaw) * EnBoom_sOffsetScale;
        }

        point.x = target->focus.pos.x + offset.x;
        point.y = target->focus.pos.y + offset.y;
        point.z = target->focus.pos.z + offset.z;

        yawTarget = Actor_WorldYawTowardPoint(&self->actor, &point);
        yawDiff = self->actor.world.rot.y - yawTarget;

        pitchTarget = Actor_WorldPitchTowardPoint(&self->actor, &point);
        pitchDiff = self->actor.world.rot.x - pitchTarget;

        distXYZScale = (200.0f - Math_Vec3f_DistXYZ(&self->actor.world.pos, &point)) * 0.005f;
        if (distXYZScale < 0.12f) {
            distXYZScale = 0.12f;
        }

        if ((target != &player->actor) && ((target->update == NULL) || (ABS(yawDiff) > 0x4000))) {
            self->moveTo = NULL;
        } else {
            Math_ScaledStepToS(&self->actor.world.rot.y, yawTarget, (s16)(ABS(yawDiff) * distXYZScale));
            Math_ScaledStepToS(&self->actor.world.rot.x, pitchTarget, (s16)(ABS(pitchDiff) * distXYZScale));
        }
    }

    FUN_0036c648(&self->actor, 12.0f);
    Actor_MoveForward(&self->actor);
    FUN_00373264(&self->actor, 0x01000102);

    collided = (self->collider[0x10] & 2) >> 1;
    if (collided) {
        Actor* at = *(Actor**)&self->collider[4];

        if (at->id == 0x15 || at->id == 0x19C) {
            self->grabbed = at;
            if (at->id == 0x19C) {
                at->flags |= 0x2000;
            }
        }
    }

    if (DECR(self->unk_26C) == 0 || (PLAYER_FLAGS3(player) & 0x1000)) {
        distFromLink = Math_Vec3f_DistXYZ(&self->actor.world.pos, &player->actor.focus.pos);
        self->moveTo = &player->actor;

        if (distFromLink < 40.0f || (PLAYER_FLAGS3(player) & 0x1000)) {
            target = self->grabbed;
            if (target != NULL) {
                Math_Vec3f_Copy(&target->world.pos, &player->actor.world.pos);
                if (target->id == 0x15) {
                    target->gravity = -0.9f;
                    target->bgCheckFlags &= ~3;
                } else {
                    target->flags &= ~0x2000;
                }
            }
            player->stateFlags1 &= ~0x2000000;
            PLAYER_FLAGS3(player) |= 0x10000;
            if (PLAYER_FLAGS3(player) & 0x1000) {
                PLAYER_FLAGS3(player) &= ~0x1000;
                EnBoom_SetupAction(self, FUN_00128f0c);
            } else {
                Actor_Kill(&self->actor);
            }
        }
    } else {
        collided = (self->collider[0x10] & 2) >> 1;
        if (collided) {
            Math_Vec3f_Copy(&self->actor.world.pos, &self->actor.prevPos);
        } else {
            Vec3f hitPoint;
            s32 hitDynaID;
            DynaPolyActor* hitActor;
            u8 mat;

            collided = BgCheck_EntityLineTest1(&play->colCtx, &self->actor.prevPos, &self->actor.world.pos, &hitPoint,
                                               &self->actor.wallPoly, 1, 1, 1, 1, &hitDynaID);
            if (collided) {
                if (FUN_00314c68(play, &self->actor, self->actor.wallPoly, hitDynaID, &hitPoint) != 0 ||
                    (hitDynaID != 0x32 && ((hitActor = DynaPoly_GetActor(&play->colCtx, hitDynaID)) != NULL) &&
                     hitActor->actor.id == 0xC8 && hitActor->actor.params == 0)) {
                    collided = 0;
                } else {
                    hitActor = DynaPoly_GetActor(&play->colCtx, hitDynaID);
                    mat = (hitActor != NULL) ? (u8)hitActor->actor.unk_19B : (u8)0;
                    switch (mat) {
                        case 1:
                            FUN_00314c28(play, &hitPoint, 8);
                            break;
                        case 2:
                            FUN_00375f90(play, &hitPoint, 8);
                            break;
                        case 3:
                            FUN_0033af2c(play, &hitPoint, 8);
                            break;
                        case 0:
                            if (FUN_00314c14(&play->colCtx, self->actor.wallPoly, hitDynaID) == 0xB) {
                                FUN_00375f90(play, &hitPoint, 8);
                            } else {
                                FUN_0033af2c(play, &hitPoint, 8);
                            }
                            break;
                    }
                }
            }
        }

        if (collided) {
            self->actor.world.rot.x = -self->actor.world.rot.x;
            self->actor.world.rot.y += 0x8000;
            self->moveTo = &player->actor;
            self->unk_26C = 0;
        }
    }

    target = self->grabbed;
    if (target != NULL) {
        if (target->update == NULL) {
            self->grabbed = NULL;
        } else {
            Math_Vec3f_Copy(&target->world.pos, &self->actor.world.pos);
        }
    }
}
#endif

GLOBAL_ASM("asm/EnBoom_Fly.s")

}
