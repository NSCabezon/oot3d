extern "C" {
#include "z_en_vm.hpp"

#define FLAGS 0x00000011

void EnVm_Init(Actor* actor, GameState* state);
void EnVm_Destroy(Actor* actor, GameState* state);
void EnVm_Update(Actor* actor, GameState* state);
void EnVm_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_Vm.data.s")

ActorInit En_Vm_InitVars = {
    ACTOR_EN_VM,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_VM,
    sizeof(EnVm),
    (ActorFunc)EnVm_Init,
    (ActorFunc)EnVm_Destroy,
    (ActorFunc)EnVm_Update,
    (ActorFunc)EnVm_Draw,
};

// DRAFT (not compiled): ~10 words off. Remaining diffs: (1) ROM stores `animModel.unk_00 = mat` AFTER GetCMABByIndex through a
// pointer copy (`mov r1,r6; mov r6,r0; str r1,[r0]`; armcc hoists the address/store instead); (2) FIXED (R11): the scene
// check reads through the raw `state` param -> real branches like ROM. Still ~9 insns off in (1): R11 tried placement-new
// ctor (mat, cmab), member Set()/ref params/this-returning helper, static (non-inline) helper, param-order perms, am
// pointer local, greedy alias mutations: none reproduces the cmab/am copies in r7/r6.
#if 0
extern u8 EnVm_sCylinderInit[];
extern u8 EnVm_sQuadInit1[];
extern u8 EnVm_sQuadInit2[];
s32 Collider_InitQuad(PlayState* play, EnVmQuad* quad);
s32 Collider_SetQuad(PlayState* play, EnVmQuad* dest, Actor* actor, void* src);
void FUN_0039bb60(EnVm* self);

void EnVm_Init(Actor* thisx, GameState* state) {
    EnVm* self = (EnVm*)thisx;
    PlayState* play = (PlayState*)state;

    FUN_00372f38(thisx, play, &self->skelAnimModel, 0, 0);
    FUN_00353c9c(thisx, play, &self->skelAnime, 0, 0, self->jointTable, self->morphTable, 6);
    AnimatedMaterial* mat = self->skelAnime.unk_28->unk_10;
    void* cmab = self->skelAnime.zarInfo->GetCMABByIndex(0);
    self->animModel.unk_00 = mat;
    self->animModel.FUN_00372d94(cmab);
    self->unk_4EC = 0;
    thisx->unk_19B = 2;

    if (*(u16*)((u8*)state + 0x104) == 7 && *(u8*)&((PlayState*)state)->roomCtx == 9) {
        thisx->flags |= 0x20;
    }

    ActorShape_Init(&thisx->shape, 0.0f, NULL, 0.0f);
    Collider_InitCylinder(play, &self->colliderCylinder);
    Collider_SetCylinder(play, &self->colliderCylinder, thisx, (ColliderCylinderInit*)EnVm_sCylinderInit);
    Collider_InitQuad(play, &self->colliderQuad1);
    Collider_SetQuad(play, &self->colliderQuad1, thisx, EnVm_sQuadInit1);
    Collider_InitQuad(play, &self->colliderQuad2);
    Collider_SetQuad(play, &self->colliderQuad2, thisx, EnVm_sQuadInit2);
    self->beamSightRange = (f32)(thisx->params >> 8) * 40.0f;
    thisx->params &= 0xFF;
    thisx->naviEnemyId = 0x39;

    if (thisx->params == 0) {
        thisx->colChkInfo.health = 2;
        Actor_SetScale(thisx, 0.014f);
    } else {
        thisx->colChkInfo.health = 1;
        Actor_SetScale(thisx, 0.01f);
    }

    FUN_0039bb60(self);
}
#endif
GLOBAL_ASM("asm/EnVm_Init.s")

void FUN_003685a0(void* arg);

void EnVm_Destroy(Actor* actor, GameState* state) {
    EnVm* self = (EnVm*)actor;
    PlayState* play = (PlayState*)state;

    Collider_DestroyCylinder(play, &self->colliderCylinder);
    FUN_003508b8(actor, self->skelAnimModel, 0);
    SkelAnime_Free2(&self->skelAnime);
    FUN_003685a0(&self->animModel);
}

s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void FUN_002a8568(EnVm* self, PlayState* play);

void FUN_0039bb60(EnVm* self) {
    f32 frameCount = Animation_GetLastFrame(&self->skelAnime, 0);

    Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, frameCount, frameCount, 2, 0.0f);
    self->unk_25E = self->unk_260 = 0;
    self->timer = 15;
    self->actionFunc = FUN_002a8568;
    self->unk_21C = 0;
}

void FUN_00285a7c(EnVm* self, PlayState* play) {
    if (self->timer == 0) {
        if (FUN_003731e0(&self->skelAnime)) {
            self->unk_25E++;
            if (self->unk_25E == 3) {
                f32 frameCount = Animation_GetLastFrame(&self->skelAnime, 0);

                Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, frameCount, frameCount, 2, 0.0f);
                self->unk_25E = self->unk_260 = 0;
                self->unk_21C = 0;
                self->timer = 15;
                self->actionFunc = FUN_002a8568;
            } else if (self->unk_25E == 1) {
                Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 0),
                                          2, 0.0f);
            } else {
                self->skelAnime.curFrame = 0.0f;
                self->skelAnime.playSpeed = 2.0f;
                self->timer = 15;
            }
        }
    } else {
        Math_SmoothStepToS(&self->beamRot.x, 0, 10, 0x5DC, 0);
        self->timer--;
        FUN_003731e0(&self->skelAnime);
    }
}

typedef struct EnVmBomb {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0xC8];
    /* 0x026C */ s16 timer;
} EnVmBomb;

void Item_DropCollectibleRandom(PlayState* play, Actor* parent, Vec3f* pos, s32 params);

void FUN_002503e4(EnVm* self, PlayState* play) {
    EnVmBomb* bomb;

    self->beamRot.x += 0x5DC;
    self->headRotY += 0x683;
    Actor_MoveForward(&self->actor);

    if (--self->timer == 0) {
        bomb = (EnVmBomb*)Actor_Spawn(&play->actorCtx, play, ACTOR_EN_BOM, self->actor.world.pos.x,
                                      self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0x6FF, 0, 1);

        if (bomb != NULL) {
            bomb->timer = 0;
        }

        Item_DropCollectibleRandom(play, &self->actor, &self->actor.world.pos, 0xA0);
        Actor_Kill(&self->actor);
    }
}

__attribute__((noinline)) void FUN_001acac4(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 a4, s16 a5, s32 a6, s32 a7, s32 a8,
                  s32 a9, s32 a10, s32 a11, s32 a12, s16 a13, s32 a14);
void FUN_002b680c(EnVm* self, PlayState* play);

static inline s16 EnVm_Vec3f_Pitch(Vec3f* a, Vec3f* b) {
    f32 by = b->y;
    f32 ay = a->y;
    return Math_Atan2S(Math_Vec3f_DistXZ(a, b), ay - by);
}

void FUN_002a8568(EnVm* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    static Vector3f D_80B2EAEC(0.0f, 0.0f, 0.0f);
    f32 dist;
    s16 headRot;
    s16 pitch;
    s16 step;

    switch (self->unk_25E) {
        case 0:
            Math_SmoothStepToS(&self->beamRot.x, 0, 10, 0x5DC, 0);
            headRot = self->actor.yawTowardsPlayer - (self->headRotY + self->actor.shape.rot.y);
            pitch = EnVm_Vec3f_Pitch(&self->beamPos1, &player->actor.world.pos);

            if (pitch > 0x1B91) {
                pitch = 0x1B91;
            }

            dist = self->beamSightRange - self->actor.xzDistToPlayer;
            step = (s16)(dist * 180.0f);

            if (self->actor.xzDistToPlayer <= self->beamSightRange && ABS(headRot) <= 0x2710 && pitch >= 0xE38 &&
                self->actor.yDistToPlayer <= 80.0f && self->actor.yDistToPlayer >= -160.0f) {
                Math_SmoothStepToS(&self->beamRot.x, pitch, 10, 0xFA0, 0);
                if (Math_SmoothStepToS(&self->headRotY, self->actor.yawTowardsPlayer - self->actor.shape.rot.y, 1,
                                       (ABS(step) / 3) + 0xFA0, 0) <= 5460) {
                    self->timer--;
                    if (self->timer == 0) {
                        self->skelAnime.curFrame = 0.0f;
                        self->skelAnime.startFrame = 0.0f;
                        self->skelAnime.playSpeed = 2.0f;
                        self->unk_25E++;
                        Audio_PlayActorSound2(&self->actor, 0x010003E3);
                    }
                }
            } else {
                self->headRotY -= 0x14D;
            }

            FUN_003731e0(&self->skelAnime);
            return;
        case 1:
            break;
        default:
            return;
    }

    Math_SmoothStepToS(&self->headRotY, self->actor.yawTowardsPlayer - self->actor.shape.rot.y, 1, 0x1F40, 0);

    if (FUN_003731e0(&self->skelAnime)) {
        self->skelAnime.curFrame = 0.0f;
        self->unk_260++;
    }

    if (self->unk_260 == 2) {
        self->beamRot.y = self->actor.yawTowardsPlayer;
        self->beamRot.x = EnVm_Vec3f_Pitch(&self->beamPos1, &player->actor.world.pos);

        if (self->beamRot.x > 0x1B91) {
            self->beamRot.x = 0x1B91;
        }

        if (self->beamRot.x < 0xAAA) {
            self->unk_25E = self->unk_260 = 0;
            self->skelAnime.startFrame = self->skelAnime.curFrame = self->skelAnime.endFrame;
            {
                f32 one = 1.0f;
                self->skelAnime.playSpeed = one;
            }
            self->timer = 15;
        } else {
            self->skelAnime.curFrame = 6.0f;
            FUN_001acac4(play, &self->beamPos2, (Vec3f*)&D_80B2EAEC, (Vec3f*)&D_80B2EAEC, 150, -25, 0, 0, 255, 0, 255,
                         255, 255, 16, 20);
            Animation_Change_00375c08(&self->skelAnime, 0, 3.0f, 3.0f, 7.0f, 2, 0.0f);
            self->timer = 458;
            self->beamScale.x = 0.6f;
            self->beamSpeed = 40.0f;
            self->unk_21C = 1;
            self->colliderQuad1.atFlags &= ~2;
            self->actionFunc = FUN_002b680c;
        }
    }
}


// DRAFT (not compiled): 28 instrs off (CheckHealth/SetupStun/SetupDie inlined). Diffs: `mov r0,r4` hoisted before the
// explosive branch + real branches (ours if-converts ldrbne/subne/strbne), bic via second alias (`bic r1,r0`), r0/r1 choice for
// the constants stored after Audio_PlayActorSound2's `mov r0,r4`, colChkCtx address kept in r1 then copied to r5.
// FUN_001acb6c must be __attribute__((noinline)) at the call site (same-TU definition is inlined otherwise).
#if 0
Actor* Actor_GetCollidedExplosive(PlayState* play, Collider* collider);
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);
void FUN_00285a7c(EnVm* self, PlayState* play);
void FUN_002503e4(EnVm* self, PlayState* play);
__attribute__((noinline)) void FUN_001acb6c(PlayState* play, Vec3f* pos, s16 a, s16 b, s16 c, s32 d);

static inline void EnVm_SetupStun(EnVm* self) {
    Animation_Change_00375c08(&self->skelAnime, 0, -1.0f, Animation_GetLastFrame(&self->skelAnime, 0), 0.0f, 2, 0.0f);
    self->unk_260 = 0;
    self->timer = 270;
    self->unk_25E = self->unk_260;
    self->unk_21C = 2;
    self->beamScale.z = 0.0f;
    self->beamScale.y = 0.0f;
    Audio_PlayActorSound2(&self->actor, 0x0100030E);
    self->actionFunc = FUN_00285a7c;
}

static inline void EnVm_SetupDie(EnVm* self) {
    Animation_Change_00375c08(&self->skelAnime, 0, -1.0f, Animation_GetLastFrame(&self->skelAnime, 0), 0.0f, 2, 0.0f);
    self->timer = 50;
    self->unk_25E = self->unk_260 = 0;
    self->unk_21C = 3;
    self->beamScale.z = 0.0f;
    self->beamScale.y = 0.0f;
    self->actor.shape.yOffset = -5000.0f;
    self->actor.world.pos.y += 5000.0f * self->actor.scale.y;
    self->actor.velocity.y = 8.0f;
    self->actor.gravity = -0.5f;
    self->actor.speedXZ = Rand_ZeroOne() + 1.0f;
    self->actor.world.rot.y = Rand_CenteredFloat(65535.0f);
    self->actionFunc = FUN_002503e4;
}

static inline void EnVm_CheckHealth(EnVm* self, PlayState* play) {
    EnVmBomb* bomb;

    if (Actor_GetCollidedExplosive(play, (Collider*)&self->colliderCylinder) != NULL) {
        self->actor.colChkInfo.health--;
    } else {
        if (!(self->colliderQuad2.acFlags & 2) || self->unk_21C == 2) {
            return;
        }
        self->colliderQuad2.acFlags &= ~2;
    }

    if (self->actor.colChkInfo.health != 0) {
        Actor_SetColorFilter(&self->actor, 0x400000, 255, 0, 8);
        EnVm_SetupStun(self);
    } else {
        bomb = (EnVmBomb*)Actor_Spawn(&play->actorCtx, play, ACTOR_EN_BOM, self->actor.world.pos.x,
                                      self->actor.world.pos.y + 20.0f, self->actor.world.pos.z, 0, 0, 0x601, 0, 1);

        if (bomb != NULL) {
            bomb->timer = 0;
        }

        EnVm_SetupDie(self);
    }
}

void EnVm_Update(Actor* thisx, GameState* state) {
    EnVm* self = (EnVm*)thisx;
    PlayState* play = (PlayState*)state;
    CollisionCheckContext* colChkCtx;

    if (self->actor.colChkInfo.health != 0) {
        EnVm_CheckHealth(self, play);
    }

    if (self->unk_260 == 4) {
        FUN_001acb6c(play, &self->beamPos3, 20, -1, 255, 20);
        FUN_0037378c(play, &self->beamPos3, 6.0f, 1, 120, 20, 1);
        Audio_PlayActorSound2(&self->actor, 0x010003E2);
    }

    self->actionFunc(self, play);
    self->beamTexScroll += 12;

    if (self->actor.colChkInfo.health != 0 && self->unk_21C != 2) {
        Audio_PlayActorSound2(&self->actor, 0x010003E0);
    }

    Collider_UpdateCylinder(&self->actor, &self->colliderCylinder);
    colChkCtx = &play->colChkCtx;
    CollisionCheck_SetOC(play, colChkCtx, (Collider*)&self->colliderCylinder);

    if (*(u16*)((u8*)&self->actor + 0x11A) == 0 && self->actor.colChkInfo.health != 0) {
        CollisionCheck_SetAC(play, colChkCtx, (Collider*)&self->colliderCylinder);
    }

    CollisionCheck_SetAC(play, colChkCtx, (Collider*)&self->colliderQuad2);
    self->actor.focus.pos = self->actor.world.pos;
    self->actor.focus.pos.y += (6500.0f + self->actor.shape.yOffset) * self->actor.scale.y;
}
#endif
GLOBAL_ASM("asm/EnVm_Update.s")

// DRAFTS (not compiled). EnVm_Draw + FUN_0016f3d0 (PostLimbDraw) use function-local guarded statics whose guards
// share ONE .data section with the guard of FUN_002a8568 (Wait): ROM Draw reads its guard as `*(0x52FF5C + 0x24)`
// and PostLimbDraw's 8 guards are 0x52FF60..0x52FF7C. EnVm_Draw matches (modulo the static layout) with the draft
// below, but it can only be enabled together with PostLimbDraw in C (needs `pin --guard 0x52FF5C --size 0x28`,
// objects 0x59B778..0x59B7F0). PostLimbDraw draft is ~100 instrs off: ROM hoists `mov sb,#0` (poly = NULL) above the
// limb test + integer-store identity/translate MTX34 (see CLAUDE.md MakeTrans).
#if 0
f32 sinf(f32 x);
f32 cosf(f32 x);
typedef struct EnVmBgPoly EnVmBgPoly;
s32 BgCheck_EntityLineTest1(void* colCtx, Vec3f* posA, Vec3f* posB, Vec3f* posResult, EnVmBgPoly** outPoly,
                            s32 checkWall, s32 checkFloor, s32 checkCeil, s32 checkOneFace, s32* bgId);
void FUN_0035479c(EnVmQuad* quad, Vec3f* a, Vec3f* b, Vec3f* c, Vec3f* d); // Collider_SetQuadVertices

static inline void EnVm_MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}

static inline void EnVm_MtxTranslate(MTX34* mtx, Vec3f* t) {
    u32* d = (u32*)mtx;
    u32* s = (u32*)t;

    d[0] = 0x3F800000;
    d[1] = 0;
    d[2] = 0;
    d[3] = s[0];
    d[4] = 0;
    d[5] = 0x3F800000;
    d[6] = 0;
    d[7] = s[1];
    d[8] = 0;
    d[9] = 0;
    d[10] = 0x3F800000;
    d[11] = s[2];
}

void Matrix_RotateX(MTX34* mtx, s32 mode, f32 angle);
void Matrix_RotateY(MTX34* mtx, s32 mode, f32 angle);

static inline void EnVm_MtxRotateZ(MTX34* mtx, f32 angle) {
    f32 sinZ = sinf(angle);
    f32 cosZ = cosf(angle);
    s32 i;

    for (i = 0; i < 3; i++) {
        f32 a = mtx->data[i][0];
        f32 b = mtx->data[i][1];

        mtx->data[i][0] = a * cosZ + b * sinZ;
        mtx->data[i][1] = b * cosZ - a * sinZ;
    }
}

void FUN_0016f3d0(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnVm* self = (EnVm*)thisx;
    Vector3f sp80(0.0f, 0.0f, 0.0f);
    static Vector3f D_80B2EB1C(0.0f, 0.0f, 0.0f);
    static Vector3f D_80B2EB28(0.0f, 0.0f, 1600.0f);
    static Vector3f D_80B2EB34(1000.0f, 700.0f, 2500.0f);
    static Vector3f D_80B2EB40(1000.0f, -700.0f, 2500.0f);
    static Vector3f D_80B2EB4C(-1000.0f, 700.0f, 2500.0f);
    static Vector3f D_80B2EB58(-1000.0f, -700.0f, 2500.0f);
    static Vector3f D_80B2EB64(500.0f, 0.0f, 0.0f);
    static Vector3f D_80B2EB70(-500.0f, 0.0f, 0.0f);
    Vector3f sp74(500.0f, 0.0f, 0.0f);
    Vector3f sp68(-500.0f, 0.0f, 0.0f);
    Vec3f posResult;
    EnVmBgPoly* poly;
    s32 bgId;
    f32 dist;

    if (limbIndex == 2) {
        EnVm_MultVec3f(&D_80B2EB1C, (Vector3f*)&self->beamPos1, mtx);
        EnVm_MultVec3f(&D_80B2EB28, (Vector3f*)&self->beamPos2, mtx);

        if (self->unk_260 >= 3) {
            poly = NULL;
            sp80.z = self->actor.scale.y * ((self->beamScale.z + 500.0f) * 10000.0f);
            EnVm_MultVec3f(&sp80, (Vector3f*)&self->beamPos3, mtx);

            if (BgCheck_EntityLineTest1(&play->colCtx, &self->beamPos1, &self->beamPos3, &posResult, &poly, 1, 1, 0, 1,
                                        &bgId) == 1) {
                self->beamScale.z = Math_Vec3f_DistXYZ(&self->beamPos1, &posResult) - 5.0f;
                self->unk_260 = 4;
                self->beamPos3 = posResult;
            }
            if (self->beamScale.z != 0.0f) {
                dist = 100.0f;
                if (self->actor.scale.y > 0.01f) {
                    dist = 70.0f;
                }
                sp74.z = sp68.z = Math_Vec3f_DistXYZ(&self->beamPos1, &self->beamPos3) * dist;
                EnVm_MultVec3f(&D_80B2EB64, (Vector3f*)&self->colliderQuad1.quad[3], mtx);
                EnVm_MultVec3f(&D_80B2EB70, (Vector3f*)&self->colliderQuad1.quad[2], mtx);
                EnVm_MultVec3f(&sp74, (Vector3f*)&self->colliderQuad1.quad[1], mtx);
                EnVm_MultVec3f(&sp68, (Vector3f*)&self->colliderQuad1.quad[0], mtx);
                FUN_0035479c(&self->colliderQuad1, &self->colliderQuad1.quad[0], &self->colliderQuad1.quad[1],
                             &self->colliderQuad1.quad[2], &self->colliderQuad1.quad[3]);
            }
        }
        EnVm_MultVec3f(&D_80B2EB34, (Vector3f*)&self->colliderQuad2.quad[1], mtx);
        EnVm_MultVec3f(&D_80B2EB40, (Vector3f*)&self->colliderQuad2.quad[0], mtx);
        EnVm_MultVec3f(&D_80B2EB4C, (Vector3f*)&self->colliderQuad2.quad[3], mtx);
        EnVm_MultVec3f(&D_80B2EB58, (Vector3f*)&self->colliderQuad2.quad[2], mtx);
        FUN_0035479c(&self->colliderQuad2, &self->colliderQuad2.quad[0], &self->colliderQuad2.quad[1],
                     &self->colliderQuad2.quad[2], &self->colliderQuad2.quad[3]);
    } else if (limbIndex == 5) {
        Vec3f pos = self->beamPos1;
        s16 rotZ;
        s16 rotY;
        s16 rotX;
        f32 angle;

        EnVm_MtxTranslate(mtx, &pos);
        rotZ = self->beamRot.z;
        rotX = self->beamRot.x;
        rotY = self->beamRot.y;
        angle = (f32)rotZ * (3.14159265f / 32768.0f);
        if (angle != 0.0f) {
            EnVm_MtxRotateZ(mtx, angle);
        }
        if (rotY != 0) {
            Matrix_RotateY(mtx, 1, (f32)rotY * (3.14159265f / 32768.0f));
        }
        if (rotX != 0) {
            Matrix_RotateX(mtx, 1, (f32)rotX * (3.14159265f / 32768.0f));
        }
        {
            f32 scaleXY = self->beamScale.x * 0.1f;
            f32 scaleZ = self->beamScale.z * 0.0015f;
            s32 i;

            for (i = 0; i < 3; i++) {
                mtx->data[i][0] *= scaleXY;
            }
            for (i = 0; i < 3; i++) {
                mtx->data[i][1] *= scaleXY;
            }
            for (i = 0; i < 3; i++) {
                mtx->data[i][2] *= scaleZ;
            }
        }
    }
}

extern s32 gUnk_0054C8CC;
void FUN_00357fd0(Player* player, void* arg1, Vec3f* pos);
void FUN_003586ec(void* sub);
void FUN_00373bec(void* sub);
void FUN_0036932c(void* arg0, s32 arg1);
void FUN_00357878(Actor* actor, Vec3f* pos, Vec3f* scale, s32 alpha, PlayState* play);
s32 FUN_0016f36c(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void FUN_0016f3d0(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);

typedef struct EnVmSub {
    /* 0x00 */ char unk_00[0x8];
    /* 0x08 */ f32 unk_08;
    /* 0x0C */ f32 unk_0C;
} EnVmSub;

void EnVm_Draw(Actor* actor, GameState* state) {
    EnVm* self = (EnVm*)actor;
    PlayState* play = (PlayState*)state;
    static Vector3f sScale(0.0f, 0.0f, 0.0f);
    EnVmSub* sub;
    Vec3f actorPos;

    FUN_00357fd0(GET_PLAYER(play), actor->unk_178, &actor->world.pos);

    sub = (EnVmSub*)&self->animModel;
    sub->unk_0C = 0.4f;
    f32 flip = (f32)self->unk_4EC;
    if (gUnk_0054C8CC == 0) {
        sub->unk_08 = flip;
        FUN_003586ec(sub);
    }
    self->unk_4EC = 1 - self->unk_4EC;
    FUN_00373bec(sub);

    if (self->unk_21C == 3) {
        FUN_0036932c(self->skelAnime.unk_28, 0);
    }

    SkelAnime_DrawOpa(&self->skelAnime, &actor->modelMtx, (OverrideLimbDrawOpa)FUN_0016f36c,
                      (PostLimbDrawOpa)FUN_0016f3d0, actor, 0);
    actorPos = actor->world.pos;
    FUN_00357878(actor, &actorPos, (Vec3f*)&sScale, 255, play);
}
#endif

GLOBAL_ASM("asm/EnVm_Draw.s")

GLOBAL_ASM("../src/game/actors/En_Vm/data_syms.s")

extern Vec3f EnVm_sZero;

void EffectSs_Spawn(PlayState* play, s32 type, s32 priority, void* initParams);

typedef struct EnVmEffectParams {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ Vec3f velocity;
    /* 0x18 */ Vec3f accel;
    /* 0x24 */ s16 unk_24;
    /* 0x26 */ s16 unk_26;
    /* 0x28 */ s16 unk_28;
    /* 0x2C */ s32 unk_2C;
} EnVmEffectParams;

void FUN_001acb6c(PlayState* play, Vec3f* pos, s16 a, s16 b, s16 c, s32 d) {
    EnVmEffectParams params;

    Math_Vec3f_Copy(&params.pos, pos);
    Math_Vec3f_Copy(&params.velocity, &EnVm_sZero);
    Math_Vec3f_Copy(&params.accel, &EnVm_sZero);
    params.unk_24 = a;
    params.unk_26 = b;
    params.unk_28 = c;
    params.unk_2C = d;
    EffectSs_Spawn(play, 0x22, 0x64, &params);
}

typedef struct EnVmEffectParams2 {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ Vec3f velocity;
    /* 0x18 */ Vec3f accel;
    /* 0x24 */ s16 unk_24;
    /* 0x26 */ s16 unk_26;
    /* 0x28 */ u8 unk_28;
    /* 0x29 */ u8 unk_29;
    /* 0x2A */ u8 unk_2A;
    /* 0x2B */ u8 unk_2B;
    /* 0x2C */ u8 unk_2C;
    /* 0x2D */ u8 unk_2D;
    /* 0x2E */ u8 unk_2E;
    /* 0x2F */ char unk_2F[0x1];
    /* 0x30 */ s16 unk_30;
    /* 0x32 */ char unk_32[0x2];
    /* 0x34 */ s32 unk_34;
    /* 0x38 */ char unk_38[0x8];
    /* 0x40 */ u8 unk_40;
    /* 0x41 */ char unk_41[0x3];
} EnVmEffectParams2;

void FUN_001acac4(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 a4, s16 a5, s32 a6, s32 a7, s32 a8,
                  s32 a9, s32 a10, s32 a11, s32 a12, s16 a13, s32 a14) {
    EnVmEffectParams2 params;

    Math_Vec3f_Copy(&params.pos, pos);
    Math_Vec3f_Copy(&params.velocity, velocity);
    Math_Vec3f_Copy(&params.accel, accel);
    params.unk_24 = a4;
    params.unk_26 = a5;
    params.unk_28 = a6;
    params.unk_29 = a7;
    params.unk_2A = a8;
    params.unk_2B = a9;
    params.unk_2C = a10;
    params.unk_2D = a11;
    params.unk_2E = a12;
    params.unk_30 = a13;
    params.unk_34 = a14;
    params.unk_40 = 0;
    EffectSs_Spawn(play, 0x21, 0x78, &params);
}

void Matrix_RotateX(MTX34* mtx, s32 mode, f32 angle);
void Matrix_RotateY(MTX34* mtx, s32 mode, f32 angle);

s32 FUN_0016f36c(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnVm* self = (EnVm*)thisx;

    if (limbIndex == 2) {
        Matrix_RotateY(mtx, 1, (f32)self->headRotY * (3.14159265f / 32768.0f));
        Matrix_RotateX(mtx, 1, (f32)self->beamRot.x * (3.14159265f / 32768.0f));
    }

    return false;
}

}
