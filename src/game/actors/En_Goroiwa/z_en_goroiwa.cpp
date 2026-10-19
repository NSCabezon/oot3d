extern "C" {
#include "z_en_goroiwa.hpp"

#define FLAGS 0x00000010

#define PARAMS_GET_U(params, shift, count) (((params) >> (shift)) & ((1 << (count)) - 1))
#define ENGOROIWA_ENABLE_AT (1 << 0)
#define ENGOROIWA_ENABLE_OC (1 << 1)
#define AT_HIT (1 << 1)
#define NA_SE_PL_BODY_HIT 0x0100003F
#define ENGOROIWA_LOOPMODE_ONEWAY 0
#define ENGOROIWA_LOOPMODE_ONEWAY_BREAK 1
#define ENGOROIWA_LOOPMODE_ROUNDTRIP 3

void EnGoroiwa_Init(Actor* actor, GameState* state);
void EnGoroiwa_Destroy(Actor* actor, GameState* state);
void EnGoroiwa_Update(Actor* actor, GameState* state);
void EnGoroiwa_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_Goroiwa.data.s")

GLOBAL_ASM("../src/game/actors/En_Goroiwa/data_syms.s")

ActorInit En_Goroiwa_InitVars = {
    ACTOR_EN_GOROIWA,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GOROIWA,
    sizeof(EnGoroiwa),
    (ActorFunc)EnGoroiwa_Init,
    (ActorFunc)EnGoroiwa_Destroy,
    (ActorFunc)EnGoroiwa_Update,
    (ActorFunc)EnGoroiwa_Draw,
};

extern CollisionCheckInfoInit EnGoroiwa_sColChkInfoInit;
extern f32 EnGoroiwa_sInitYOffsets[];
extern char EnGoroiwa_sJntSphInit[];
extern InitChainEntry EnGoroiwa_sInitChain[];
void Collider_InitJntSph(PlayState* play, EnGoroiwaColliderJntSph* collider);
void Collider_SetJntSph(PlayState* play, EnGoroiwaColliderJntSph* collider, Actor* actor, void* init,
                        EnGoroiwaColliderElement* elements);
void FUN_003c0b64(EnGoroiwa* self, PlayState* play);

void Collider_DestroyJntSph(PlayState* play, EnGoroiwaColliderJntSph* collider);

void EnGoroiwa_Destroy(Actor* actor, GameState* state) {
    EnGoroiwa* self = (EnGoroiwa*)actor;
    PlayState* play = (PlayState*)state;

    FUN_00350f34(actor, &self->skelAnimModel, 0);
    Collider_DestroyJntSph(play, &self->collider);
}

void FUN_001eb56c(EnGoroiwa* self, PlayState* play);
f32 BgCheck_EntityRaycastFloor4(CollisionContext* colCtx, CollisionPoly** floorPoly, s32* bgId, Actor* actor,
                                Vec3f* pos);

extern f32 EnGoroiwa_sYOffsets[];

static inline void EnGoroiwa_UpdateCollider(EnGoroiwa* self) {
    EnGoroiwaSphere* worldSphere = &self->collider.elements[0].worldSphere;

    worldSphere->center.x = self->actor.world.pos.x;
    worldSphere->center.y = self->actor.world.pos.y + EnGoroiwa_sYOffsets[PARAMS_GET_U(self->actor.params, 10, 1)];
    worldSphere->center.z = self->actor.world.pos.z;
}

void EnGoroiwa_Update(Actor* actor, GameState* state) {
    EnGoroiwa* self = (EnGoroiwa*)actor;
    PlayState* play = (PlayState*)state;
    Player* player = GET_PLAYER(play);
    s32 bgId;

    if (!(player->stateFlags1 & 0x300000C0)) {
        if (self->collisionDisabledTimer > 0) {
            self->collisionDisabledTimer--;
        }
        self->actionFunc(self, play);
        switch (PARAMS_GET_U(self->actor.params, 10, 1)) {
            case 1:
                Actor_UpdateBgCheckInfo(play, &self->actor, 0.0f, 0.0f, 0.0f, 0x1C);
                break;
            case 0:
                self->actor.floorHeight = BgCheck_EntityRaycastFloor4(&play->colCtx, &self->actor.floorPoly, &bgId,
                                                                     &self->actor, &self->actor.world.pos);
                break;
        }
        FUN_001eb56c(self, play);
        if (self->actor.xzDistToPlayer < 300.0f) {
            EnGoroiwa_UpdateCollider(self);
            u8 flags = self->stateFlags;

            if ((flags & ENGOROIWA_ENABLE_AT) && self->collisionDisabledTimer <= 0) {
                CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
            }
            if ((self->stateFlags & ENGOROIWA_ENABLE_OC) && self->collisionDisabledTimer <= 0) {
                CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
            }
        }
    }
}

void EnGoroiwa_Draw(Actor* actor, GameState* state) {
    EnGoroiwa* self = (EnGoroiwa*)actor;

    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&actor->modelMtx);
    self->skelAnimModel->FUN_00372170(0);
}

// FUN_001eb56c (UpdateRotation): not drafted - contains a guarded function-local static Vec3f (guard 0x525F9C, obj 0x59AF90)
// and inlined Matrix_RotateAxis; needs pin_static infra.
GLOBAL_ASM("asm/FUN_001eb56c.s")


void FUN_003c0b64(EnGoroiwa* self, PlayState* play);

#define ENGOROIWA_ENABLE_AT (1 << 0)
#define ENGOROIWA_ENABLE_OC (1 << 1)
#define ENGOROIWA_PLAYER_IN_THE_WAY (1 << 2)
#define BGCHECKFLAG_GROUND (1 << 0)

void FUN_00359420(EnGoroiwa* self) {
    self->actionFunc = FUN_003c0b64;
    self->stateFlags &= ~(ENGOROIWA_ENABLE_AT | ENGOROIWA_ENABLE_OC);
    self->stateFlags |= ENGOROIWA_ENABLE_AT | ENGOROIWA_ENABLE_OC;
    self->rollRotSpeed = 1.0f;
}

void FUN_003ba7ac(EnGoroiwa* self, PlayState* play);
void FUN_001440fc(EnGoroiwa* self, PlayState* play);
s32 FUN_001a3154(EnGoroiwa* self, PlayState* play);

extern s16 EnGoroiwa_sWaitDurations[];
extern f32 EnGoroiwa_sSpeeds[];

void FUN_00143374(EnGoroiwa* self) {
    self->actionFunc = FUN_003ba7ac;
    self->stateFlags &= ~(ENGOROIWA_ENABLE_AT | ENGOROIWA_ENABLE_OC);
    self->stateFlags |= ENGOROIWA_ENABLE_OC;
    self->actor.gravity = -0.86f;
    self->actor.minVelocityY = -15.0f;
    self->actor.speedXZ *= 0.15f;
    self->actor.velocity.y = 5.0f;
    self->rollRotSpeed = 1.0f;
}

// recipe_near4 2026-10-15: still 10. Tried ternary/if-else/t local, repeated table expr, s32/f32 n, n*3/2, inline helper,
// table pointer local, statement swaps. Likely needs the wait-duration table as real TU data (EQU base load is hoisted).
// FUN_00316c64 (SetupWait): 10 words off (near-miss, 30 variants). Both 3.0f and 0.5f constants are
// hoisted to the prologue by armcc (ROM loads them after the flag store) and the table base reg differs.
#if 0
void FUN_00316c64(EnGoroiwa* self) {
    s16 n;

    self->actor.speedXZ = 0.0f;
    self->actionFunc = FUN_001440fc;
    self->stateFlags &= ~(ENGOROIWA_ENABLE_AT | ENGOROIWA_ENABLE_OC);
    self->stateFlags |= ENGOROIWA_ENABLE_OC;
    n = EnGoroiwa_sWaitDurations[self->actor.home.rot.z & 1];
    self->waitTimer = n > 0 ? (s32)(0.5f + (n * 3.0f) * 0.5f) : (s32)((n * 3.0f) * 0.5f - 0.5f);
    self->rollRotSpeed = 0.0f;
}
#endif

void FUN_0035fb14(Actor* actor);

static inline s32 EnGoroiwa_MoveAndFall(EnGoroiwa* self, PlayState* play) {
    Path* path;
    s32 result;
    Vec3s* nextPointPos;
    u8 idx;

    idx = self->isInKokiri;
    Math_StepToF(&self->actor.speedXZ, EnGoroiwa_sSpeeds[idx], 0.3f);
    FUN_0035fb14(&self->actor);
    path = &play->setupPathList[self->actor.params & 0xFF];
    nextPointPos = path->points + self->nextWaypoint;
    result = true;
    result &= Math_StepToF(&self->actor.world.pos.x, nextPointPos->x, __fabsf(self->actor.velocity.x));
    result &= Math_StepToF(&self->actor.world.pos.z, nextPointPos->z, __fabsf(self->actor.velocity.z));
    {
        Vec3f* p = &self->actor.world.pos;
        p->y += self->actor.velocity.y;
    }
    return result;
}

s32 FUN_001a3154(EnGoroiwa* self, PlayState* play) {
    return EnGoroiwa_MoveAndFall(self, play);
}
void FUN_00316c64(EnGoroiwa* self);

static inline void EnGoroiwa_ReverseDirection(EnGoroiwa* self) {
    self->currentWaypoint = self->nextWaypoint;
    self->pathDirection *= -1;
    self->nextWaypoint = self->currentWaypoint + self->pathDirection;
}

static inline void EnGoroiwa_FaceNextWaypoint(EnGoroiwa* self, PlayState* play) {
    Path* path = &play->setupPathList[self->actor.params & 0xFF];
    Vec3s* nextPos = path->points + self->nextWaypoint;
    Vec3f nextPosF;

    nextPosF.x = nextPos->x;
    nextPosF.y = nextPos->y;
    nextPosF.z = nextPos->z;
    self->actor.world.rot.y = Math_Vec3f_Yaw(&self->actor.world.pos, &nextPosF);
}

// FUN_003ba7ac (MoveAndFallToGround): matched 0/87.
void FUN_003ba7ac(EnGoroiwa* self, PlayState* play) {
    EnGoroiwa_MoveAndFall(self, play);
    if ((self->actor.bgCheckFlags & BGCHECKFLAG_GROUND) && self->actor.velocity.y < 0.0f) {
        if ((self->stateFlags & ENGOROIWA_PLAYER_IN_THE_WAY) && (self->actor.home.rot.z & 1) == 1) {
            EnGoroiwa_ReverseDirection(self);
            EnGoroiwa_FaceNextWaypoint(self, play);
        }
        FUN_00316c64(self);
    }
}

static inline f32 EnGoroiwa_Vec3fDistSq(Vec3f* a, Vec3f* b) {
    f32 dx = a->x - b->x;
    f32 dy = a->y - b->y;
    f32 dz = a->z - b->z;

    return SQ(dx) + SQ(dy) + SQ(dz);
}

static inline void EnGoroiwa_Vec3fDiff(Vec3f* a, Vec3f* b, Vec3f* dest) {
    dest->x = a->x - b->x;
    dest->y = a->y - b->y;
    dest->z = a->z - b->z;
}

static inline s32 EnGoroiwa_Vec3fNormalize(Vec3f* ret, Vec3f* a) {
    f32 magnitude = __sqrtf(SQ(a->x) + SQ(a->y) + SQ(a->z));
    f32 scale;

    if (magnitude < 0.001f) {
        return false;
    }

    scale = 1.0f / magnitude;

    ret->x = a->x * scale;
    ret->y = a->y * scale;
    ret->z = a->z * scale;

    return true;
}

// FUN_0021ee20 (EnGoroiwa_Move): 90/114 words off (far; ROM s-reg numbering of nextPointPosF and diff CSE differ, ~15 variants).
// Asm stays in functions_200000s.cpp.
#if 0
s32 FUN_0021ee20(EnGoroiwa* self, PlayState* play) {
    Path* path = &play->setupPathList[self->actor.params & 0xFF];
    Vec3s* nextPointPos = path->points + self->nextWaypoint;
    Vec3s* currentPointPos = path->points + self->currentWaypoint;
    s32 nextPointReached;
    Vec3f posDiff;
    Vec3f nextPointPosF;

    nextPointPosF.x = nextPointPos->x;
    nextPointPosF.y = nextPointPos->y;
    nextPointPosF.z = nextPointPos->z;
    Math_StepToF(&self->actor.speedXZ, EnGoroiwa_sSpeeds[self->isInKokiri], 0.3f);
    if (EnGoroiwa_Vec3fDistSq(&nextPointPosF, &self->actor.world.pos) < SQ(5.0f)) {
        EnGoroiwa_Vec3fDiff(&nextPointPosF, &self->actor.world.pos, &posDiff);
    } else {
        posDiff.x = nextPointPosF.x - currentPointPos->x;
        posDiff.y = nextPointPosF.y - currentPointPos->y;
        posDiff.z = nextPointPosF.z - currentPointPos->z;
    }
    EnGoroiwa_Vec3fNormalize(&self->actor.velocity, &posDiff);
    self->actor.velocity.x *= self->actor.speedXZ;
    self->actor.velocity.y *= self->actor.speedXZ;
    self->actor.velocity.z *= self->actor.speedXZ;
    nextPointReached = true;
    nextPointReached &= Math_StepToF(&self->actor.world.pos.x, nextPointPosF.x, __fabsf(self->actor.velocity.x));
    nextPointReached &= Math_StepToF(&self->actor.world.pos.y, nextPointPosF.y, __fabsf(self->actor.velocity.y));
    nextPointReached &= Math_StepToF(&self->actor.world.pos.z, nextPointPosF.z, __fabsf(self->actor.velocity.z));
    return nextPointReached;
}
#endif

static inline void EnGoroiwa_CheckEndOfPath(EnGoroiwa* self) {
    s16 loopMode = PARAMS_GET_U(self->actor.params, 8, 2);

    if (self->nextWaypoint < 0) {
        if (loopMode == ENGOROIWA_LOOPMODE_ONEWAY || loopMode == ENGOROIWA_LOOPMODE_ONEWAY_BREAK) {
            self->currentWaypoint = self->endWaypoint;
            self->nextWaypoint = self->endWaypoint - 1;
            self->pathDirection = -1;
        } else if (loopMode == ENGOROIWA_LOOPMODE_ROUNDTRIP) {
            self->currentWaypoint = 0;
            self->nextWaypoint = 1;
            self->pathDirection = 1;
        }
    } else if (self->nextWaypoint > self->endWaypoint) {
        if (loopMode == ENGOROIWA_LOOPMODE_ONEWAY || loopMode == ENGOROIWA_LOOPMODE_ONEWAY_BREAK) {
            self->currentWaypoint = 0;
            self->nextWaypoint = 1;
            self->pathDirection = 1;
        } else if (loopMode == ENGOROIWA_LOOPMODE_ROUNDTRIP) {
            self->currentWaypoint = self->endWaypoint;
            self->nextWaypoint = self->endWaypoint - 1;
            self->pathDirection = -1;
        }
    }
}

static inline void EnGoroiwa_SetNextWaypoint(EnGoroiwa* self) {
    self->currentWaypoint = self->nextWaypoint;
    self->nextWaypoint += self->pathDirection;
    EnGoroiwa_CheckEndOfPath(self);
}

static inline void EnGoroiwa_TeleportToWaypoint(EnGoroiwa* self, PlayState* play, s32 waypoint) {
    Path* path = &play->setupPathList[self->actor.params & 0xFF];
    Vec3s* pointPos = path->points + waypoint;

    self->actor.world.pos.x = pointPos->x;
    self->actor.world.pos.y = pointPos->y;
    self->actor.world.pos.z = pointPos->z;
}

void EnGoroiwa_Init(Actor* actor, GameState* state) {
    EnGoroiwa* self = (EnGoroiwa*)actor;
    PlayState* play = (PlayState*)state;
    s32 pathIdx;

    FUN_00372f38(&self->actor, play, &self->skelAnimModel, 0, 0);
    Actor_ProcessInitChain(&self->actor, EnGoroiwa_sInitChain);
    Collider_InitJntSph(play, &self->collider);
    Collider_SetJntSph(play, &self->collider, &self->actor, EnGoroiwa_sJntSphInit, self->colliderElements);
    EnGoroiwa_UpdateCollider(self);
    self->collider.elements[0].worldSphere.radius = 58.0f;
    pathIdx = self->actor.params;
    if ((pathIdx & 0xFF) == 0xFF) {
        Actor_Kill(&self->actor);
        return;
    }
    if (play->setupPathList[pathIdx & 0xFF].count < 2) {
        Actor_Kill(&self->actor);
        return;
    }
    CollisionCheck_SetInfo(&self->actor.colChkInfo, NULL, &EnGoroiwa_sColChkInfoInit);
    ActorShape_Init(&self->actor.shape, EnGoroiwa_sInitYOffsets[PARAMS_GET_U(self->actor.params, 10, 1)],
                    ActorShadow_DrawCircle, 9.4f);
    actor->shape.shadowAlpha = 200;
    if (play->sceneNum != 0x55) {
        self->isInKokiri = 0;
    } else {
        self->isInKokiri = 1;
    }
    self->endWaypoint = play->setupPathList[self->actor.params & 0xFF].count - 1;
    self->currentWaypoint = 0;
    self->nextWaypoint = 1;
    self->pathDirection = 1;
    EnGoroiwa_TeleportToWaypoint(self, play, 0);
    ((f32*)self->unk_21C)[0] = 1.0f;
    self->rollRotSpeed = 1.0f;
    EnGoroiwa_FaceNextWaypoint(self, play);
    self->actionFunc = FUN_003c0b64;
    self->stateFlags &= ~(ENGOROIWA_ENABLE_AT | ENGOROIWA_ENABLE_OC);
    self->stateFlags |= ENGOROIWA_ENABLE_AT | ENGOROIWA_ENABLE_OC;
    self->rollRotSpeed = 1.0f;
    if (play->sceneNum == 9 && gSaveContext.masterQuestFlag == 1 && *(s8*)((u8*)play + 0x4C30) == 8 &&
        self->actor.params == 0xD03) {
        self->actor.speedXZ = 10.0f;
    }
}


// FUN_00317708 (EnGoroiwa_NextWaypoint): 73/95 words off (far; ROM loads params with ldrsh and recomputes &0xFF, ~12 variants).
// Asm stays in functions_310000s.cpp.
#if 0
void FUN_00317708(EnGoroiwa* self, PlayState* play) {
    s16 params = *(s16*)&self->actor.params;
    s16 loopMode = (params >> 8) & 3;

    EnGoroiwa_SetNextWaypoint(self);

    if (loopMode == ENGOROIWA_LOOPMODE_ONEWAY || loopMode == ENGOROIWA_LOOPMODE_ONEWAY_BREAK) {
        if (self->currentWaypoint == 0 || self->currentWaypoint == self->endWaypoint) {
            Path* path = &play->setupPathList[params & 0xFF];
            Vec3s* pointPos = path->points + self->currentWaypoint;

            self->actor.world.pos.x = pointPos->x;
            self->actor.world.pos.y = pointPos->y;
            self->actor.world.pos.z = pointPos->z;
        }
    }

    EnGoroiwa_FaceNextWaypoint(self, play);
}
#endif

void FUN_00368fc0(PlayState* play, Actor* actor, f32 knockbackSpeed, s16 yaw, f32 upwardSpeed, s32 arg5);
void FUN_0036f59c(Player* player, s32 sfxId);
void FUN_00317708(EnGoroiwa* self, PlayState* play);
void FUN_00359420(EnGoroiwa* self) __attribute__((noinline));

static inline s32 EnGoroiwa_MoveUpToNextWaypoint(EnGoroiwa* self, PlayState* play) {
    Path* path = &play->setupPathList[self->actor.params & 0xFF];
    Vec3s* nextPointPos = path->points + self->nextWaypoint;

    Math_StepToF(&self->actor.velocity.y, EnGoroiwa_sSpeeds[self->isInKokiri] * 0.5f, 0.18f);
    self->actor.world.pos.x = nextPointPos->x;
    self->actor.world.pos.z = nextPointPos->z;
    return Math_StepToF(&self->actor.world.pos.y, nextPointPos->y, __fabsf(self->actor.velocity.y));
}

void FUN_003af238(EnGoroiwa* self, PlayState* play) {
    if (self->collider.base.atFlags & AT_HIT) {
        self->collider.base.atFlags &= ~AT_HIT;
        FUN_00368fc0(play, &self->actor, 2.0f, self->actor.yawTowardsPlayer, 0.0f, 4);
        FUN_0036f59c(GET_PLAYER(play), NA_SE_PL_BODY_HIT);
        if ((self->actor.home.rot.z & 1) == 1) {
            self->collisionDisabledTimer = 75;
        }
    } else if (EnGoroiwa_MoveUpToNextWaypoint(self, play)) {
        FUN_00317708(self, play);
        FUN_00359420(self);
        self->actor.speedXZ = 0.0f;
    }
}

}
