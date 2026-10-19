extern "C" {
#include "z_en_st.hpp"

#define R_UPDATE_RATE SREG(30)

extern struct_80034EC0_Entry EnSt_sAnimationInfo[];

#define FLAGS 0x00000035

void EnSt_Init(Actor* actor, GameState* state);
void EnSt_Destroy(Actor* thisx, PlayState* play);
void EnSt_Update(Actor* actor, GameState* state);
void EnSt_Draw(Actor* thisx, PlayState* play);

GLOBAL_ASM("data/z_En_St.data.s")
GLOBAL_ASM("../src/game/actors/En_St/data_syms.s")

ActorInit En_St_InitVars = {
    ACTOR_EN_ST,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_ST,
    sizeof(EnSt),
    (ActorFunc)EnSt_Init,
    (ActorFunc)EnSt_Destroy,
    (ActorFunc)EnSt_Update,
    (ActorFunc)EnSt_Draw,
};

// 3DS EffectBlure init params (N64 EffectBlureInit1); vertex ctor FUN_003fd780 = _ZN15BlureInitVertexC1Ev (shared_syms.s).
struct BlureInitVertex {
    BlureInitVertex();
    u8 data[0x24];
};

typedef struct {
    /* 0x000 */ s32 unk_00;
    /* 0x004 */ BlureInitVertex vtx[16];
    /* 0x244 */ Color_RGBA8 p1StartColor;
    /* 0x248 */ Color_RGBA8 p2StartColor;
    /* 0x24C */ Color_RGBA8 p1EndColor;
    /* 0x250 */ Color_RGBA8 p2EndColor;
    /* 0x254 */ s32 elemDuration;
    /* 0x258 */ s32 unkFlag;
    /* 0x25C */ u8 calcMode;
    /* 0x260 */ s32 unk_260;
    /* 0x264 */ u8 unk_264;
} EnStBlureInit; // size = 0x268

typedef struct {
    ColliderCylinderInit* p[6];
} EnStCylInitPtrs;

extern CollisionCheckInfoInit2 EnSt_sColChkInit;
extern u8 EnSt_sJntSphInit[];
extern EnStCylInitPtrs EnSt_sCylInitPtrs;
s32 Effect_Add(PlayState* play, s32* pIndex, s32 type, u8 arg3, u8 arg4, void* initParams);
void Collider_InitJntSph(PlayState* play, void* collider);
void Collider_SetJntSph(PlayState* play, void* collider, Actor* actor, void* init, void* elements);
s32 BgCheck_EntityLineTest1(void* colCtx, Vec3f* posA, Vec3f* posB, Vec3f* posResult, CollisionPoly** outPoly, s32 checkWall,
                            s32 checkFloor, s32 checkCeil, s32 checkOneFace, s32* bgId);
DamageTable* DamageTable_Get(s32 index);
void EnSt_StartOnCeilingOrGround(EnSt* self, PlayState* play);

#if 0 // EnSt_Init draft: 219/306 words off (blure colour init + ctor order unsolved), see STATUS.md
void EnSt_Init(Actor* thisx, GameState* state) {
    EnSt* self = (EnSt*)thisx;
    PlayState* play = (PlayState*)state;
    EnStCylInitPtrs inits;
    f32 scale;
    s32 i;

    FUN_00372f38(thisx, play, 0);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 14.0f);
    FUN_00353c9c(&self->actor, play, &self->skelAnime, 0, 0, &self->unk_228[0], &self->unk_228[0x23C], 11);
    FUN_003717ac(&self->skelAnime, EnSt_sAnimationInfo, 0);

    EnStBlureInit blureInit;
    {
        Color_RGBA8 p1Start = { 255, 255, 255, 75 };
        Color_RGBA8 p2Start = { 255, 255, 255, 75 };
        Color_RGBA8 p1End = { 255, 255, 255, 0 };
        Color_RGBA8 p2End = { 255, 255, 255, 0 };

        blureInit.p1StartColor = p1Start;
        blureInit.p2StartColor = p2Start;
        blureInit.p1EndColor = p1End;
        blureInit.p2EndColor = p2End;
    }
    blureInit.elemDuration = 6;
    blureInit.unkFlag = 0;
    blureInit.calcMode = 3;
    blureInit.unk_264 = 22;
    Effect_Add(play, &self->blureIdx, 1, 0, 0, &blureInit);

    inits = EnSt_sCylInitPtrs;
    for (i = 0; i < 6; i++) {
        Collider_InitCylinder(play, &self->colliderCylinders[i]);
        Collider_SetCylinder(play, &self->colliderCylinders[i], &self->actor, inits.p[i]);
    }

    self->colliderCylinders[0].info.bumper.dmgFlags = 0x0003F8F9;
    self->colliderCylinders[1].info.bumper.dmgFlags = 0xFFC00706;
    self->colliderCylinders[2].base.colType = 9;
    self->colliderCylinders[2].info.bumperFlags = 0xD;
    self->colliderCylinders[2].info.elemType = 2;
    self->colliderCylinders[2].info.bumper.dmgFlags = 0xFFCC0706;

    CollisionCheck_SetInfo2(&self->actor.colChkInfo, DamageTable_Get(2), &EnSt_sColChkInit);
    Collider_InitJntSph(play, &self->colliderJntSph);
    Collider_SetJntSph(play, &self->colliderJntSph, &self->actor, EnSt_sJntSphInit, (u8*)self + 0x8D4);

    self->unk_980 = (self->actor.params >> 8) & 1;
    self->actor.params &= 0xF;
    if (self->actor.params == 2) {
        self->actor.flags |= 0x80;
        self->actor.naviEnemyId = 4;
    } else if (self->actor.params == 1) {
        self->actor.naviEnemyId = 5;
    } else {
        self->actor.naviEnemyId = 4;
    }

    {
        Vec3f lineEnd;
        CollisionPoly* poly;
        s32 bgId;

        lineEnd.x = self->actor.world.pos.x;
        lineEnd.y = self->actor.world.pos.y + 1000.0f;
        lineEnd.z = self->actor.world.pos.z;
        if (BgCheck_EntityLineTest1(&play->colCtx, &self->actor.world.pos, &lineEnd, &self->lineHitPos, &poly, 0, 0, 1,
                                    1, &bgId)) {
            self->ceilingPos = *(Vec3f*)&self->actor.world.pos;
            self->ceilingPos.y -= 100.0f;
        }
    }

    self->actor.flags |= 0x1004000;
    scale = (self->actor.params == 1) ? 1.4f : 1.0f;
    {
        f32* sphRadius = (f32*)(*(u8**)((u8*)self + 0x8D0) + 0x34);
        *sphRadius = (s16)(*sphRadius * scale);
    }
    for (i = 0; i < 6; i++) {
        ColliderCylinder* cyl = &self->colliderCylinders[i];

        cyl->dim.radius = (s16)(cyl->dim.radius * scale);
        cyl->dim.height = (s16)(cyl->dim.height * scale);
        cyl->dim.yShift = (s16)(cyl->dim.yShift * scale);
    }
    Actor_SetScale(&self->actor, scale * 0.04f);
    self->colliderScale = scale;
    self->floorHeightOffset = scale * 32.0f;
    self->actor.gravity = 0.0f;
    self->initialYaw = self->actor.world.rot.y;
    self->unk_97C = 1.0f;
    self->actor.targetArrowOffset = (self->actor.params == 1) ? 560.0f : 400.0f;
    self->actionFunc = EnSt_StartOnCeilingOrGround;
}
#endif

GLOBAL_ASM("asm/EnSt_Init.s")

void Effect_Delete(PlayState* play, s32 index);
void Collider_DestroyJntSph(PlayState* play, void* collider);

void EnSt_Destroy(Actor* thisx, PlayState* play) {
    EnSt* self = (EnSt*)thisx;
    s32 i;

    Effect_Delete(play, self->blureIdx);
    for (i = 0; i < 6; i++) {
        Collider_DestroyCylinder(play, &self->colliderCylinders[i]);
    }
    Collider_DestroyJntSph(play, &self->colliderJntSph);
    SkelAnime_Free2(&self->skelAnime);
    FUN_00350f34(thisx, 0);
}

GLOBAL_ASM("asm/EnSt_Update.s")

#if 0 // EnSt_Draw: 55/68 words off (best of ~15 variants), see STATUS.md
#define DMG_STICK_SRC 0x2
void FUN_00342be0(EnSt* self, s32 type, s32 zero, f32 r, f32 g, f32 b, f32 alpha);
void FUN_002341a8(PlayState* play, s32 limbIndex, MTX34* mtx, EnSt* self);

void EnSt_Draw(Actor* thisx, PlayState* play) {
    EnSt* self = (EnSt*)thisx;
    u32 flags = self->colliderCylinders[0].info.bumper.dmgFlags;
    ColliderInfo* info0 = &self->colliderCylinders[0].info;
    ColliderInfo* info1 = &self->colliderCylinders[1].info;
    ColliderInfo* info2 = &self->colliderCylinders[2].info;
    u16 stick = *(u16*)((u8*)GET_PLAYER(play) + 0x2248);

    if (stick != 0) {
        info0->bumper.dmgFlags = flags | DMG_STICK_SRC;
        info1->bumper.dmgFlags &= ~DMG_STICK_SRC;
        info2->bumper.dmgFlags &= ~DMG_STICK_SRC;
    } else {
        info0->bumper.dmgFlags = flags & ~DMG_STICK_SRC;
        info1->bumper.dmgFlags |= DMG_STICK_SRC;
        info2->bumper.dmgFlags |= DMG_STICK_SRC;
    }

    FUN_00357fd0(GET_PLAYER(play), self->actor.unk_178, (Vec3f*)&self->actor.world);
    FUN_00342be0(self, 4, 0, self->teethR * (1.0f / 255.0f), self->teethG * (1.0f / 255.0f),
                 self->teethB * (1.0f / 255.0f), self->unk_97C);
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, NULL, (PostLimbDrawOpa)FUN_002341a8, thisx, 0);
}
#endif

GLOBAL_ASM("asm/EnSt_Draw.s")

static inline s32 EnSt_IsCloseToPlayer(EnSt* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 yDist;

    if (self->takeDamageSpinTimer != 0) {
        return false;
    } else if (self->actor.xzDistToPlayer > 160.0f) {
        return false;
    }

    yDist = self->actor.world.pos.y - player->actor.world.pos.y;
    if (yDist < -200.0f || yDist > 400.0f) {
        return false;
    }

    if (player->actor.world.pos.y + 10.0f < self->actor.floorHeight) {
        return false;
    }
    return true;
}

void FUN_003f8e24(EnSt* self, PlayState* play);

// EnSt_WaitOnCeiling (3DS: bob is period 24 frames)
void FUN_00144ffc(EnSt* self, PlayState* play) {
    if (EnSt_IsCloseToPlayer(self, play)) {
        FUN_003717ac(&self->skelAnime, EnSt_sAnimationInfo, 4);
        self->animFrames = self->skelAnime.animLength;
        self->sfxTimer = 0;
        self->actor.velocity.y = -10.0f;
        self->actionFunc = FUN_003f8e24;
    } else {
        f32 ySpeedTarget = 0.5f;

        if ((play->state.frames % 24) < 12) {
            ySpeedTarget = -0.5f;
        }
        Math_SmoothStepToF(&self->actor.velocity.y, ySpeedTarget, 0.4f, 1000.0f, 0.0f);
    }
}

void FUN_00185dd8(EnSt* self, PlayState* play);

// EnSt_LandOnGround
void FUN_0012c948(EnSt* self, PlayState* play) {
    if (self->animFrames != 0) {
        self->animFrames--;
        if (self->animFrames == 0) {
            FUN_003717ac(&self->skelAnime, EnSt_sAnimationInfo, 3);
        }
    }

    if (self->takeDamageSpinTimer != 0) {
        self->takeDamageSpinTimer--;
        if (self->takeDamageSpinTimer == 0) {
            FUN_003717ac(&self->skelAnime, EnSt_sAnimationInfo, 3);
        }
    }

    self->sfxTimer++;
    if (self->sfxTimer == 14) {
        Audio_PlayActorSound2(&self->actor, 0x01000451);
    }

    if ((self->actor.floorHeight + self->floorHeightOffset) < self->actor.world.pos.y) {
        self->sfxTimer = 0;
        self->actionFunc = FUN_00185dd8;
    } else {
        Math_SmoothStepToF(&self->actor.velocity.y, 2.0f, 0.3f, 1.0f, 0.0f);
    }
}

static inline s32 EnSt_IsCloseToGround(EnSt* self) {
    f32 velY = self->actor.velocity.y;
    f32 checkY = self->actor.world.pos.y + (velY * 2.0f);

    if (checkY - self->actor.floorHeight <= self->floorHeightOffset) {
        return true;
    }
    return false;
}

void EnSt_StartOnCeilingOrGround(EnSt* self, PlayState* play) {
    if (!EnSt_IsCloseToGround(self)) {
        self->rotAwayTimer = 180.0f / R_UPDATE_RATE + 0.5f;
        self->actionFunc = FUN_00144ffc;
        FUN_00144ffc(self, play);
        return;
    }

    self->actor.world.pos.y = self->actor.floorHeight + self->floorHeightOffset;
    FUN_003717ac(&self->skelAnime, EnSt_sAnimationInfo, 4);
    self->animFrames = self->skelAnime.animLength;
    self->sfxTimer = 0;
    self->actionFunc = FUN_0012c948;

    if (self->animFrames != 0) {
        self->animFrames--;
        if (self->animFrames == 0) {
            FUN_003717ac(&self->skelAnime, EnSt_sAnimationInfo, 3);
        }
    }

    if (self->takeDamageSpinTimer != 0) {
        self->takeDamageSpinTimer--;
        if (self->takeDamageSpinTimer == 0) {
            FUN_003717ac(&self->skelAnime, EnSt_sAnimationInfo, 3);
        }
    }

    self->sfxTimer++;
    if (self->sfxTimer == 14) {
        Audio_PlayActorSound2(&self->actor, 0x01000451);
    }

    if ((self->actor.floorHeight + self->floorHeightOffset) < self->actor.world.pos.y) {
        self->sfxTimer = 0;
        self->actionFunc = FUN_00185dd8;
    } else {
        Math_SmoothStepToF(&self->actor.velocity.y, 2.0f, 0.3f, 1.0f, 0.0f);
    }
}

s32 FUN_0035f97c(EnSt* self, PlayState* play);
void FUN_0035fb14(Actor* actor);
void FUN_003c26a4(EnSt* self, PlayState* play);
// Vector3f-like ctor; the parameter order (y, x, z) selects the ROM's VFP register numbering.
struct EnStZeroVec {
    f32 x, y, z;
    EnStZeroVec(f32 y, f32 x, f32 z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

void FUN_0012ca40(EnSt* self, PlayState* play) {
    EnStZeroVec zeroVec(0.0f, 0.0f, 0.0f);

    if (DECR(self->deathTimer) == 0) {
        self->actor.velocity = *(Vec3f*)&zeroVec;
        self->finishDeathTimer = 12;
        self->actionFunc = FUN_003c26a4;
        return;
    }

    if (DECR(self->setTargetYawTimer) == 0) {
        self->deathYawTarget = Math_Vec3f_Yaw(&self->actor.world.pos, &self->actor.home.pos);
        self->setTargetYawTimer = 12;
    }

    Math_SmoothStepToS(&self->actor.world.rot.x, 0x3FFC, 4, 0x2710, 1);
    Math_SmoothStepToS(&self->actor.world.rot.z, 0, 4, 0x2710, 1);
    Math_SmoothStepToS(&self->actor.world.rot.y, self->deathYawTarget, 10, 0x2710, 1);

    self->actor.shape.rot = self->actor.world.rot;

    FUN_0035fb14(&self->actor);
    self->groundBounces = 2;
    FUN_0035f97c(self, play);
}

void FUN_0015f5d8(EnSt* self, PlayState* play) {
    f32 frame = self->skelAnime.curFrame;

    if (frame == 0.0f || (frame > 7.9f && frame < 8.1f) || (frame > 16.6f && frame < 16.7f) ||
        (frame > 20.6f && frame < 20.7f) || (frame > 22.6f && frame < 22.7f) || (frame > 25.3f && frame < 25.4f)) {
        Audio_PlayActorSound2(&self->actor, 0x010002eb);
    }

    if (DECR(self->deathTimer) == 0) {
        self->finishDeathTimer = 12;
        self->actionFunc = FUN_003c26a4;
    }
}

void FUN_003c2844(EnSt* self, PlayState* play) {
    *(s16*)((u8*)self + 0x11A) = self->deathTimer;
    FUN_0035fb14(&self->actor);
    self->actor.shape.rot = self->actor.world.rot;

    if (FUN_0035f97c(self, play)) {
        FUN_003717ac(&self->skelAnime, EnSt_sAnimationInfo, 1);
        {
            f32 endFrame = self->skelAnime.endFrame;
            f32 k, half;
            half = 0.5f;
            k = 1.5f;
            self->deathTimer = (endFrame > 0.0f) ? (s32)(half + (endFrame * k)) : (s32)((endFrame * k) - half);
        }
        Audio_PlayActorSound2(&self->actor, 0x010002eb);
        self->actionFunc = FUN_0015f5d8;
    } else {
        Math_SmoothStepToF(&self->actor.shape.yOffset, 400.0f, 0.4f, 10000.0f, 0.0f);
    }
}

// EnSt_ReturnToCeiling
static inline s32 EnSt_IsCloseToInitialPos_(EnSt* self) {
    f32 velY = self->actor.velocity.y;
    f32 checkY = self->actor.world.pos.y + (velY * 2.0f);
    if (self->actor.home.pos.y - checkY <= 0.0f) {
        return true;
    }
    return false;
}

void FUN_003cd79c(EnSt* self, PlayState* play) {
    f32* anim = (f32*)((u8*)&self->skelAnime + 0x30);
    f32 animPctDone = anim[3] / (anim[7] - 1.0f);

    if (animPctDone == 1.0f) {
        Audio_PlayActorSound2(&self->actor, 0x010002d9);
        FUN_003717ac(&self->skelAnime, EnSt_sAnimationInfo, 3);
    }

    if (!EnSt_IsCloseToPlayer(self, play)) {
        if (EnSt_IsCloseToInitialPos_(self)) {
            FUN_003717ac(&self->skelAnime, EnSt_sAnimationInfo, 3);
            self->actionFunc = FUN_00144ffc;
        } else {
            self->actor.velocity.y = 4.0f * animPctDone;
        }
    } else {
        FUN_003717ac(&self->skelAnime, EnSt_sAnimationInfo, 4);
        self->animFrames = self->skelAnime.animLength;
        self->sfxTimer = 0;
        self->actor.velocity.y = -10.0f;
        self->actionFunc = FUN_003f8e24;
    }
}

void FUN_00365d20(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* acc, Color_RGBA8* c1, Color_RGBA8* c2, s16 scale, s32 a, s32 b);

#if 0 // FUN_0035f97c (EnSt_IsDoneBouncing): 9 words off (play r7/step r8 swap + sxtah)
extern u32 EnSt_sDustColors[];

s32 FUN_0035f97c(EnSt* self, PlayState* play) {
    if (self->actor.velocity.y > 0.0f || self->groundBounces == 0 || !(self->actor.bgCheckFlags & 0x1)) {
        return false;
    }

    Audio_PlayActorSound2(&self->actor, 0x010002eb);
    {
        u32 primColor = EnSt_sDustColors[0];
        u32 envColor = EnSt_sDustColors[1];
        Vector3f dustVel(0.0f, 0.0f, 0.0f);
        Vector3f dustAccel(0.0f, 0.3f, 0.0f);
        Vec3f dustPos;
        s16 yAngle;
        s32 i;

        yAngle = (Rand_ZeroOne() - 0.5f) * 65536.0f;
        dustPos.y = self->actor.floorHeight;
        s16 step = 0x10000 / 10;
        for (i = 10; i >= 0; i--, yAngle += step) {
            dustAccel.x = (Rand_ZeroOne() - 0.5f) * 4.0f;
            dustAccel.z = (Rand_ZeroOne() - 0.5f) * 4.0f;
            dustPos.x = self->actor.world.pos.x + (Math_SinS(yAngle) * 22.0f);
            dustPos.z = self->actor.world.pos.z + (Math_CosS(yAngle) * 22.0f);
            FUN_00365d20(play, &dustPos, (Vec3f*)&dustVel, (Vec3f*)&dustAccel, (Color_RGBA8*)&primColor,
                         (Color_RGBA8*)&envColor, 120, 40, 10);
        }
    }
    self->actor.velocity.y = 0.0f;
    return true;
}
#endif

// FUN_00185dd8 (EnSt_WaitOnGround): 98 words off (3DS body larger), not matched

void FUN_003cd79c(EnSt* self, PlayState* play);
void FUN_00185dd8(EnSt* self, PlayState* play) {
    if (self->takeDamageSpinTimer != 0) {
        self->takeDamageSpinTimer--;
        if (self->takeDamageSpinTimer == 0) {
            FUN_003717ac(&self->skelAnime, EnSt_sAnimationInfo, 3);
        }
    }

    if (self->animFrames != 0) {
        self->animFrames--;
        if (self->animFrames == 0) {
            FUN_003717ac(&self->skelAnime, EnSt_sAnimationInfo, 3);
        }
    }

    if (!EnSt_IsCloseToPlayer(self, play)) {
        Audio_PlayActorSound2(&self->actor, 0x010002d9);
        FUN_003717ac(&self->skelAnime, EnSt_sAnimationInfo, 3);
        self->actionFunc = FUN_003cd79c;
        return;
    }

    if (DECR(self->sfxTimer) == 0) {
        Audio_PlayActorSound2(&self->actor, 0x010002da);
        self->sfxTimer = 96;
    }

    {
        f32 ySpeedTarget = 0.5f;

        if ((play->state.frames % 24) < 12) {
            ySpeedTarget = -0.5f;
        }
        Math_SmoothStepToF(&self->actor.velocity.y, ySpeedTarget, 0.4f, 1000.0f, 0.0f);
    }

    {
        f32 yDiff = (self->actor.world.pos.y - GET_PLAYER(play)->actor.world.pos.y) - 19.0f;

        if (yDiff < 0.0f) {
            self->actor.velocity.y = 3.0f;
        } else if (yDiff > 30.0f) {
            self->actor.velocity.y = -3.0f;
        }
    }
}

// FUN_003c26a4 (EnSt_Die): 80 words off (tail fades unk_97C and +0x54..0x5c, unidentified)

void Item_DropCollectibleRandom(PlayState* play, Actor* fromActor, Vec3f* spawnPos, s16 params);
void EffectSsDeadDb_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep,
                          s16 primR, s16 primG, s16 primB, s16 primA, s16 envR, s16 envG, s16 envB, s16 unused,
                          s16 life, s32 arg);

static inline void EnSt_SpawnDeadEffect_(EnSt* self, PlayState* play) {
    Vector3f zeroVec(0.0f, 0.0f, 0.0f);
    Vec3f firePos;

    firePos.x = self->actor.world.pos.x + ((Rand_ZeroOne() - 0.5f) * 60.0f);
    firePos.y = ((Rand_ZeroOne() - 0.5f) * 45.0f + 10.0f) + self->actor.world.pos.y;
    firePos.z = self->actor.world.pos.z + ((Rand_ZeroOne() - 0.5f) * 60.0f);
    EffectSsDeadDb_Spawn(play, &firePos, (Vec3f*)&zeroVec, (Vec3f*)&zeroVec, 100, 0, 255, 255, 255, 255, 255, 0, 0, 1,
                         11, true);
}

void FUN_003c26a4(EnSt* self, PlayState* play) {
    if (DECR(self->finishDeathTimer) != 0) {
        if (self->finishDeathTimer % 3 != 0) {
            EnSt_SpawnDeadEffect_(self, play);
        }
        self->actor.scale.x *= 0.9f;
        self->actor.scale.y *= 0.9f;
        self->actor.scale.z *= 0.9f;
        self->unk_97C *= 0.9f;
    } else {
        Item_DropCollectibleRandom(play, NULL, &self->actor.world.pos, 0xE0);
        Actor_Kill(&self->actor);
    }
}

// FUN_002341a8 (PostLimbDraw blur): 42 words off

void VEC3TransformAsm(Vector3f*, MTX34*, Vector3f*);
void Collider_UpdateSpheres(s32 limb, void* collider, MTX34* mtx);
void* FUN_00362384(s32 index);
void FUN_003620f0(void* effect, Vector3f* p1, Vector3f* p2);

void FUN_002341a8(PlayState* play, s32 limbIndex, MTX34* mtx, EnSt* self) {
    if (limbIndex == 0 && self->gaveDamageSpinTimer != 0 && self->swayTimer == 0 && self->gaveDamageSpinTimer > 1) {
        Vector3f v1(834.0f, 834.0f, 0.0f);
        Vector3f v2(834.0f, -584.0f, 0.0f);
        Vec3f p1;
        Vec3f p2;

        v1.x *= self->colliderScale;
        v1.y *= self->colliderScale;
        v1.z *= self->colliderScale;
        v2.x *= self->colliderScale;
        v2.y *= self->colliderScale;
        v2.z *= self->colliderScale;
        VEC3TransformAsm((Vector3f*)&p1, mtx, &v1);
        VEC3TransformAsm((Vector3f*)&p2, mtx, &v2);
        FUN_003620f0(FUN_00362384(self->blureIdx), (Vector3f*)&p1, (Vector3f*)&p2);
    }
    Collider_UpdateSpheres(limbIndex, &self->colliderJntSph, mtx);
}

}
