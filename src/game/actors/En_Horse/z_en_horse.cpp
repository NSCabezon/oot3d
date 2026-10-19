extern "C" {
#include "z_en_horse.hpp"

#define FLAGS 0x80000010

#define PLAYER_STATE1_0 (1 << 0)
#define PLAYER_STATE1_20 (1 << 20)
#define ACTOR_FLAG_TALK (1 << 8)

void SkelAnime_SetCurrentFrame(SkelAnime* skelAnime, f32 frame);
f32* EnHorse_CurFramePtr(SkelAnime* skelAnime);

void EnHorse_Init(Actor* actor, GameState* state);
void EnHorse_Destroy(Actor* actor, GameState* state);
void EnHorse_Update(Actor* actor, GameState* state);
void EnHorse_Draw(EnHorse* self, PlayState* play);

void Collider_DestroyJntSph(PlayState* play, EnHorseColliderJntSph* collider);
extern void** En_Horse_sAnimationHeaders[];
typedef struct {
    f32 f[2];
} EnHorsePair;
extern EnHorsePair EnHorse_sWalkFrames;
f32 Rand_ZeroOne(void);
f32 Actor_WorldDistXZToActor(Actor* a, Actor* b);
void EnHorse_StartMovingAnimation(EnHorse* self, u8 animId, f32 morphFrames, f32 startFrame);
// SetFollowAnimation passes its s32 animId without masking (ROM `mov r1,r4`), so it calls through this view of the prototype.
typedef void (*EnHorseStartMovingAnimFunc)(EnHorse*, s32, f32, f32);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, void* anim);
void Animation_Change_00375c08(SkelAnime* skelAnime, void* anim, f32 speed, f32 startFrame, f32 endFrame, s32 mode,
                               f32 morphFrames);
void Audio_PlaySoundGeneral(u32 sfxId, Vec3f* a1, u8 a2, f32* a3, f32* a4, s8* a5);
extern f32 FLOAT_0054ac20;
extern DAT_0054ac24_struct DAT_0054ac24;
s32 FUN_0033bd6c(Player* player);

GLOBAL_ASM("data/z_En_Horse.data_005265BC.s")
GLOBAL_ASM("data/z_En_Horse.data_00526F0C.s")
GLOBAL_ASM("../src/game/actors/En_Horse/data_syms.s")

void EnHorse_StartMountedIdle(EnHorse* self);
void EnHorse_PlayIdleAnimation(EnHorse* self, u8 anim, f32 morphFrames, f32 startFrame);

static inline void EnHorse_StartMountedIdleResetAnim(EnHorse* self) {
    SkelAnime_SetCurrentFrame(&self->skin, 0.0f);
    self->curFrame = 0.0f;
    EnHorse_StartMountedIdle(self);
    self->stateFlags &= ~ENHORSE_SANDDUST_SOUND;
}

s32 EnHorse_PlayerCanMove(EnHorse* self, PlayState* play);
void EnHorse_StickDirection(Vec2f* curStick, f32* stickMag, s16* angle) __attribute__((noinline));
void EnHorse_StartReversingInterruptable(EnHorse* self) __attribute__((noinline));
void EnHorse_StartWalking(EnHorse* self) __attribute__((noinline));

static inline void EnHorse_StartTurning(EnHorse* self) {
    self->action = ENHORSE_ACT_MOUNTED_TURN;
    self->unk_E7C = 0;
    self->animationIdx = ENHORSE_ANIM_WALK;
    Animation_Change_00375c08(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx], 1.0f, 0.0f,
                              Animation_GetLastFrame(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx]), 2,
                              -3.0f);
}

static inline void EnHorse_StartWalkingFromIdle(EnHorse* self) {
    if (!(self->stateFlags & (ENHORSE_FLAG_8 | ENHORSE_FLAG_9))) {
        self->stateFlags |= ENHORSE_FLAG_9;
        self->waitTimer = 12;
        self->noInputTimerMax = self->noInputTimer = 0;
        self->action = ENHORSE_ACT_MOUNTED_WALK;
        self->unk_E7C = 0;
    } else {
        self->waitTimer = 0;
        EnHorse_StartWalking(self);
    }
}

void SkelAnime_SetPlaySpeed(SkelAnime* skelAnime, f32 speed);
void FUN_0037308c(PlayState* play, f32 randScale, Vec3f* srcPos);

typedef struct {
    /* 0x00 */ s16 zMin;
    /* 0x02 */ s16 zMax;
    /* 0x04 */ s16 xMin;
    /* 0x06 */ s16 xMax;
    /* 0x08 */ s16 xOffset;
    /* 0x0A */ s16 angle;
    /* 0x0C */ s16 angleRange;
    /* 0x0E */ s16 unk_0E;
    /* 0x10 */ Vec3f pos;
} EnHorseBridgeJumpPoint; // size = 0x1C
extern EnHorseBridgeJumpPoint EnHorse_sBridgeJumps[];

typedef struct {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ f32 f4;
    /* 0x08 */ f32 f8;
    /* 0x0C */ f32 fC; // 48.7f
    /* 0x10 */ f32 f10;
    /* 0x14 */ f32 f14;
    /* 0x18 */ f32 f18;
    /* 0x1C */ f32 f1C;
} EnHorseData;
extern EnHorseData EnHorse_sData;

static inline void EnHorse_StartReversing(EnHorse* self) {
    self->action = ENHORSE_ACT_REVERSE;
    self->animationIdx = ENHORSE_ANIM_WALK;
    self->unk_E7C = 0;
    Animation_Change_00375c08(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx], 1.0f, 0.0f,
                              Animation_GetLastFrame(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx]), 0,
                              -3.0f);
}

#define ENHORSE_CLAMP(x, min, max) ((x) < (min) ? (min) : (x) > (max) ? (max) : (x))

void EnHorse_PlayWalkingSound(EnHorse* self) __attribute__((noinline));

extern __pure float _fabsf(float);
static __inline __pure float EnHorse_Fabsf(float f) {
    return _fabsf(f);
}

void EnHorse_UpdateSpeed(EnHorse* self, PlayState* play, f32 brakeDecel, f32 brakeAngle, f32 minStickMag, f32 decel, f32 baseSpeed,
                         s16 turnSpeed);
void EnHorse_StartBraking(EnHorse* self) __attribute__((noinline));
// ROM callers pass play in r1 although StartBraking only takes the horse.
typedef void (*EnHorseStartBrakingFunc)(EnHorse*, PlayState*);

typedef struct {
    /* 0x0 */ s16 sceneId;
    /* 0x2 */ Vec3s pos;
    /* 0x8 */ s16 angle;
} EnHorseSpawnpoint; // size = 0xA
extern EnHorseSpawnpoint EnHorse_sHorseSpawns[];

void FUN_0033b11c(void* mtx, Vec3f* in, Vec3f* out, f32* w);
f32 BgCheck_EntityRaycastFloor4(void* colCtx, CollisionPoly** outPoly, s32* bgId, Actor* actor, Vec3f* pos);
s16 Actor_WorldYawTowardActor(Actor* a, Actor* b);
s32 FUN_0031d150(f32 projW, PlayState* play, Actor* actor, Vec3f* projPos);

static inline s32 EnHorse_IsSpawnBlocked(PlayState* play, EnHorse* self, Vec3f* pos) {
    Vec3f projPos;
    f32 projW;
    f32 dist;

    FUN_0033b11c((u8*)play + 0x5BB4, pos, &projPos, &projW);
    if (EnHorse_Fabsf(projW) < 8.0e-5f) {
        return false;
    }
    dist = Math3D_Vec3f_DistXYZ(pos, &play->view.eye);
    return FUN_0031d150(projW, play, &self->actor, &projPos) != 0 || dist < 100.0f;
}

#define R_EPONAS_SONG_PLAYED DREG(53)

s32 EnHorse_Spawn(EnHorse* self, PlayState* play);
void EnHorse_SetFollowAnimation(EnHorse* self, PlayState* play) __attribute__((noinline));
void Camera_SetParam(Camera* camera, s32 param, Actor* actor);
void Camera_ChangeSetting(Camera* camera, s32 setting);
void Camera_SetCameraData(Camera* camera, s32 which, void* a, void* b, s32 c, s32 d, s32 e);

void EnHorse_IdleAnimSounds(EnHorse* self, PlayState* play) __attribute__((noinline));

#define R_UPDATE_RATE SREG(30)

void Animation_PlayOnce(SkelAnime* skelAnime, void* anim);
typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 z;
    /* 0x6 */ s16 speedXZ;
    /* 0x8 */ s16 angle;
} EnHorseRaceWaypoint; // size = 0xA

typedef struct {
    /* 0x0 */ s32 numWaypoints;
    /* 0x4 */ EnHorseRaceWaypoint* waypoints;
} EnHorseRaceInfo;

extern EnHorseRaceInfo EnHorse_sIngoRace;
extern EnHorseRaceInfo EnHorse_sHbaInfo;
extern u16 EnHorse_sIngoAnimations[];

void Math3D_RotateXZPlane(Vec3f* pos, s16 angle, f32* a, f32* b, f32* d);
void Math3D_PointDistToLine2D(f32 x, f32 z, f32 x0, f32 z0, f32 x1, f32 z1, f32* out);
void EnHorse_UpdateIngoHorseAnim(EnHorse* self) __attribute__((noinline));

static inline void EnHorse_RaceWaypointPos(EnHorseRaceWaypoint* waypoints, s32 idx, Vec3f* pos) {
    pos->x = waypoints[idx].x;
    pos->y = waypoints[idx].y;
    pos->z = waypoints[idx].z;
}

ActorInit En_Horse_InitVars = {
    ACTOR_EN_HORSE,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HORSE,
    sizeof(EnHorse),
    (ActorFunc)EnHorse_Init,
    (ActorFunc)EnHorse_Destroy,
    (ActorFunc)EnHorse_Update,
    (ActorFunc)EnHorse_Draw,
};

GLOBAL_ASM("asm/EnHorse_Init.s")

void FUN_0049fa58(Vec3f* pos);

void EnHorse_Destroy(Actor* actor, GameState* state) {
    EnHorse* self = (EnHorse*)actor;
    PlayState* play = (PlayState*)state;

    if (self->stateFlags & ENHORSE_DRAW) {
        FUN_0049fa58(&self->unk_E80);
    }
    SkelAnime_Free2(&self->skin);
    ((FaceAnimations*)self->unk_C74)->Destroy();
    Collider_DestroyCylinder(play, &self->colliderCylinder1);
    Collider_DestroyCylinder(play, &self->colliderCylinder2);
    Collider_DestroyJntSph(play, &self->colliderJntSph);
}

GLOBAL_ASM("asm/EnHorse_Update.s")

void Gfx_SetupDL_25Opa(void* gfxCtx);
void FUN_00341e68(EnHorse* self, PlayState* play, SkelAnime* skelAnime, void* overrideLimbDraw, void* postLimbDraw,
                  s32 flag);
f32* FUN_003478bc(void* model, s32 limbIndex);
void EnHorse_OverrideLimbDraw(void);
void EnHorse_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);

#if 0 // 2 words off: stateFlags |= DRAW result goes to r0, ROM uses r1 (orr r1,r0; str r1). (EnHorse*,PlayState*) sig + Vector3f ctor fixed the pos regs (Opus: 10 -> 2)
void EnHorse_Draw(EnHorse* self, PlayState* play) {
    if (!(self->stateFlags & ENHORSE_INACTIVE)) {
        Gfx_SetupDL_25Opa(play->state.gfxCtx);
        self->stateFlags |= ENHORSE_DRAW;
        FUN_00341e68(self, play, (SkelAnime*)((u8*)self + 0x1B4), EnHorse_OverrideLimbDraw, EnHorse_PostLimbDraw, 0);

        {
            MTX34* mtx = (MTX34*)FUN_003478bc(self->skin.unk_28, 9);
            Vector3f* pos = (Vector3f*)&self->unk_1088;

            *pos = Vector3f(mtx->data[0][3], mtx->data[1][3], mtx->data[2][3]);
        }
        if (self->postDrawFunc != NULL) {
            self->postDrawFunc(self, play);
        }
    }
}
#endif
GLOBAL_ASM("asm/EnHorse_Draw.s")


s32 EnHorse_BgCheckBridgeJumpPoint(EnHorse* self, PlayState* play) {
    f32 xMin;
    f32 xMax;
    s32 i;

    if (play->sceneNum != 0x5A) {
        return false;
    }
    if (self->actor.speedXZ < 12.8f) {
        return false;
    }
    if ((gSaveContext.eventChkInf[9] & 0xF) == 0xF) {
        return false;
    }

    for (i = 0; i < 2; i++) {
        xMin = EnHorse_sBridgeJumps[i].xMin;
        xMax = (xMin + EnHorse_sBridgeJumps[i].xMax) + EnHorse_sBridgeJumps[i].xOffset;
        if (xMin > xMax) {
            f32 temp = xMin;

            xMin = xMax;
            xMax = temp;
        }
        if (self->actor.world.pos.z > EnHorse_sBridgeJumps[i].zMin && self->actor.world.pos.z < EnHorse_sBridgeJumps[i].zMax) {
            if (self->actor.world.pos.x > xMin && self->actor.world.pos.x < xMax) {
                if (EnHorse_sBridgeJumps[i].angle - EnHorse_sBridgeJumps[i].angleRange < self->actor.world.rot.y &&
                    self->actor.world.rot.y < EnHorse_sBridgeJumps[i].angle + EnHorse_sBridgeJumps[i].angleRange) {
                    return true;
                }
            }
        }
    }
    return false;
}

static inline void EnHorse_BridgeJumpMove(EnHorse* self, PlayState* play) {
    f32 interp;
    f32 timeSq;

    interp = self->bridgeJumpTimer / 30.0f;
    timeSq = (self->bridgeJumpTimer * self->bridgeJumpTimer);

    self->actor.world.pos.x =
        ((EnHorse_sBridgeJumps[self->bridgeJumpIdx].pos.x - self->bridgeJumpStart.x) * interp) + self->bridgeJumpStart.x;
    self->actor.world.pos.z =
        ((EnHorse_sBridgeJumps[self->bridgeJumpIdx].pos.z - self->bridgeJumpStart.z) * interp) + self->bridgeJumpStart.z;

    self->actor.world.pos.y =
        (self->bridgeJumpStart.y + (self->bridgeJumpYVel * self->bridgeJumpTimer) + (-0.4f * timeSq));

    self->actor.world.rot.y = self->actor.shape.rot.y =
        (EnHorse_sBridgeJumps[self->bridgeJumpIdx].angle + ((1.0f - interp) * self->bridgeJumpRelAngle));
    SkelAnime_SetCurrentFrame(&self->skin, 23.0f * interp);
    FUN_003731e0(&self->skin);
    if (self->bridgeJumpTimer <= 29) {
        self->stateFlags |= ENHORSE_FLAG_24;
    }
}

static inline void EnHorse_CheckBridgeJumpLanding(EnHorse* self, PlayState* play) {
    self->actor.speedXZ = 8.0f;
    SkelAnime_SetPlaySpeed(&self->skin, 1.5f);
    if (FUN_003731e0(&self->skin)) {
        self->stateFlags &= ~ENHORSE_JUMPING;
        self->unk_E40 &= ~2;
        self->actor.gravity = -3.5f;
        self->actor.world.pos.y = EnHorse_sBridgeJumps[self->bridgeJumpIdx].pos.y;
        FUN_0037308c(play, 25.0f, &self->actor.world.pos);
        self->action = ENHORSE_ACT_MOUNTED_GALLOP;
        self->animationIdx = 7;
        Animation_Change_00375c08(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx], 1.5f, 0.0f,
                                  Animation_GetLastFrame(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx]), 2,
                                  0.0f);
        self->postDrawFunc = NULL;
        Audio_PlaySoundGeneral(0x01000170, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
    }
}

void EnHorse_BridgeJump(EnHorse* self, PlayState* play) {
    self->bridgeJumpTimer++;
    if (self->bridgeJumpTimer < 30) {
        EnHorse_BridgeJumpMove(self, play);
        return;
    }
    EnHorse_CheckBridgeJumpLanding(self, play);
}

void EnHorse_BridgeJumpInit(EnHorse* self, PlayState* play) {
    f32 y;

    FUN_0037308c(play, 25.0f, &self->actor.world.pos);
    self->action = ENHORSE_ACT_BRIDGE_JUMP;
    self->animationIdx = 13;
    self->stateFlags |= ENHORSE_JUMPING;
    self->unk_E40 |= 2;
    y = *(f32*)((u8*)self->skin.jointTable + 0x1C);
    y *= 0.01f;
    self->bridgeJumpStart = self->actor.world.pos;
    self->bridgeJumpStart.y += y;
    self->bridgeJumpYVel =
        (((EnHorse_sBridgeJumps[self->bridgeJumpIdx].pos.y + EnHorse_sData.fC) - self->bridgeJumpStart.y) - -360.0f) / 30.0f;
    self->stateFlags |= ENHORSE_CALC_RIDER_POS;
    self->riderPos.y -= y;
    self->bridgeJumpRelAngle = self->actor.world.rot.y - EnHorse_sBridgeJumps[self->bridgeJumpIdx].angle;
    self->bridgeJumpTimer = 0;
    self->actor.gravity = 0.0f;
    self->actor.speedXZ = 0;
    Animation_Change_00375c08(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx], 1.5f, 0.0f,
                              Animation_GetLastFrame(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx]), 2,
                              -3.0f);
    self->unk_E80 = self->unk_E8C;
    if (self->stateFlags & ENHORSE_DRAW) {
        Audio_PlaySoundGeneral(0x0100015C, &self->unk_E80, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
    }
    Audio_PlaySoundGeneral(0x0100016F, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
    self->postDrawFunc = NULL;
}

typedef struct {
    /* 0x00 */ u16 id;
    /* 0x02 */ u16 startFrame;
    /* 0x04 */ u16 endFrame;
    /* 0x06 */ Vec3s rot;
    /* 0x0C */ s32 startPos[3];
} EnHorseCsCue; // size >= 0x18

typedef struct {
    /* 0x0 */ s32 cueId;
    /* 0x4 */ u8 csFuncIdx;
} EnHorseCsActionEntry; // size = 0x8

typedef void (*EnHorseCsFunc)(EnHorse*, PlayState*, EnHorseCsCue*);

extern u8 EnHorse_sResetNoInput[];
extern EnHorseCsFunc EnHorse_sCutsceneInitFuncs[];
extern EnHorseCsFunc EnHorse_sCutsceneActionFuncs[];
extern EnHorseCsActionEntry EnHorse_sCsActionTable[];

static inline s32 EnHorse_GetCutsceneFunctionIndex(s32 cueId) {
    s32 numActions = 5;
    s32 i;

    for (i = 0; i < numActions; i++) {
        if (cueId == EnHorse_sCsActionTable[i].cueId) {
            return EnHorse_sCsActionTable[i].csFuncIdx;
        }
        if (cueId < EnHorse_sCsActionTable[i].cueId) {
            return 0;
        }
    }
    return 0;
}

static inline void EnHorse_Freeze(EnHorse* self) {
    if (self->action != ENHORSE_ACT_CS_UPDATE && self->action != ENHORSE_ACT_HBA) {
        if (!(EnHorse_sResetNoInput[self->actor.params] == 0 || self->actor.params == 4)) {
            self->noInputTimer = 0;
            self->noInputTimerMax = 0;
        }
        self->prevAction = self->action;
        self->action = ENHORSE_ACT_FROZEN;
        self->colliderCylinder1.base.ocFlags1 &= ~1;
        self->colliderCylinder2.base.ocFlags1 &= ~1;
        self->colliderJntSph.base.ocFlags1 &= ~1;
        self->animationIdx = ENHORSE_ANIM_IDLE;
    }
}

#if 0 // 6 words off: register naming of scene/&gSaveContext (r0/r2) in the 0x51 cutscene block (Sonnet). Opus: local types/order, inline helper, direct condition (much worse) - no change
void EnHorse_CutsceneUpdate(EnHorse* self, PlayState* play) {
    s32 csFunctionIdx;
    EnHorseCsCue* playerCue = *(EnHorseCsCue**)((u8*)play + 0x22D8);

    if (play->GetCutsceneState() == 3) {
        self->playerControlled = 1;
        self->actor.params = 10;
        self->action = ENHORSE_ACT_IDLE;
        EnHorse_Freeze(self);
        return;
    }
    if (playerCue != NULL) {
        s32 csIdx;
        s32 scene;

        scene = play->sceneNum;
        csIdx = gSaveContext.cutsceneIndex & 0xF;
        if (scene == 0x51 && csIdx == 0xD && *(s32*)((u8*)self + 0x1F4) == 9 && *(u16*)((u8*)play + 0x22B8) == 0x45) {
            if (*((u8*)self + 0x1006) != 0 && *((u8*)self + 0x1005) == 0) {
                *((u8*)self + 0x1005) = 1;
                Audio_PlaySoundGeneral(0x01000170, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
                Audio_PlaySoundGeneral(0x0100025D, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
                self->stateFlags &= ~ENHORSE_JUMPING;
            }
        }
        csFunctionIdx = EnHorse_GetCutsceneFunctionIndex(playerCue->id);

        if (csFunctionIdx != 0) {
            if (self->cutsceneAction != csFunctionIdx) {
                if (self->cutsceneAction == 0) {
                    self->actor.world.pos.x = playerCue->startPos[0];
                    self->actor.world.pos.y = playerCue->startPos[1];
                    self->actor.world.pos.z = playerCue->startPos[2];

                    self->actor.world.rot.y = playerCue->rot.y;
                    self->actor.shape.rot = self->actor.world.rot;

                    self->actor.prevPos = self->actor.world.pos;
                }
                self->cutsceneAction = csFunctionIdx;
                EnHorse_sCutsceneInitFuncs[self->cutsceneAction](self, play, playerCue);
            }
            EnHorse_sCutsceneActionFuncs[self->cutsceneAction](self, play, playerCue);
        }
    }
}
#endif
GLOBAL_ASM("asm/EnHorse_CutsceneUpdate.s")

GLOBAL_ASM("../split/asm/EnHorse_FleePlayer.s")
GLOBAL_ASM("../split/asm/FUN_003a8f5c.s")

f32 Actor_WorldDistXZToActor(Actor* a, Actor* b);
void EnHorse_RotateToPoint(EnHorse* self, Vec3f* pos, s16 turnAmount) __attribute__((noinline));

void EnHorse_FollowPlayer(EnHorse* self, PlayState* play) {
    f32 distToPlayer;

    R_EPONAS_SONG_PLAYED = false;
    distToPlayer = Actor_WorldDistXZToActor(&self->actor, &GET_PLAYER(play)->actor);

    if ((self->playerDir == 2 || self->playerDir == 3) &&
        (distToPlayer > 300.0f && !(self->stateFlags & ENHORSE_TURNING_TO_PLAYER))) {
        f32 angleDiff;

        self->animationIdx = ENHORSE_ANIM_REARING;
        self->stateFlags |= ENHORSE_TURNING_TO_PLAYER;
        self->angleToPlayer = Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(play)->actor);
        angleDiff = (f32)self->angleToPlayer - (f32)self->actor.world.rot.y;
        if (angleDiff > 32767.f) {
            angleDiff -= 32767.0f;
        } else if (angleDiff < -32767) {
            angleDiff += 32767;
        }

        self->followPlayerTurnSpeed =
            angleDiff / Animation_GetLastFrame(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx]);
        Animation_PlayOnce(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx]);
        SkelAnime_SetPlaySpeed(&self->skin, 1.0f);
        self->stateFlags &= ~ENHORSE_LAND2_SOUND;
        self->unk_E80 = self->unk_E8C;
    } else if (self->stateFlags & ENHORSE_TURNING_TO_PLAYER) {
        self->actor.world.rot.y += self->followPlayerTurnSpeed;
        self->actor.shape.rot.y = self->actor.world.rot.y;
        if (self->curFrame > 25.0f) {
            if (!(self->stateFlags & ENHORSE_LAND2_SOUND)) {
                self->stateFlags |= ENHORSE_LAND2_SOUND;
                Audio_PlaySoundGeneral(0x01000182, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
            }
        }
    } else {
        EnHorse_RotateToPoint(self, &GET_PLAYER(play)->actor.world.pos, 267);
        if (self->stateFlags & ENHORSE_OBSTACLE) {
            self->actor.world.rot.y += 534.0f;
        }
        self->actor.shape.rot.y = self->actor.world.rot.y;
    }

    if (self->animationIdx == ENHORSE_ANIM_GALLOP || self->animationIdx == 9) {
        self->actor.speedXZ = 8.0f;
        SkelAnime_SetPlaySpeed(&self->skin, 3.6000001f);
    } else if (self->animationIdx == ENHORSE_ANIM_TROT) {
        self->actor.speedXZ = 6.0f;
        SkelAnime_SetPlaySpeed(&self->skin, 2.25f);
    } else if (self->animationIdx == ENHORSE_ANIM_WALK) {
        self->actor.speedXZ = 3.0f;
        EnHorse_PlayWalkingSound(self);
        SkelAnime_SetPlaySpeed(&self->skin, self->actor.speedXZ * 0.75f);
    } else {
        self->actor.speedXZ = 0;
        SkelAnime_SetPlaySpeed(&self->skin, 1.0f);
    }

    if (!(self->stateFlags & ENHORSE_TURNING_TO_PLAYER) && (self->followTimer = self->followTimer + 1, self->followTimer) > (s32)(900.0f / R_UPDATE_RATE + 0.5f)) {
        self->animationIdx = 4;
        EnHorse_PlayIdleAnimation(self, self->animationIdx, 0.0f, 0.0f);
        self->stateFlags &= ~ENHORSE_UNRIDEABLE;
        self->unk_E80 = self->unk_E8C;

        if (self->stateFlags & ENHORSE_DRAW) {
            Audio_PlaySoundGeneral(0x0100015C, &self->unk_E80, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        }
    }

    if (FUN_003731e0(&self->skin)) {
        if (self->animationIdx == ENHORSE_ANIM_GALLOP || self->animationIdx == 9) {
            if (self->unk_1094 == 0) {
                Audio_PlaySoundGeneral(0x0100015B, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
            }
        } else if (self->animationIdx == ENHORSE_ANIM_TROT) {
            if (self->unk_1094 == 0) {
                Audio_PlaySoundGeneral(0x0100015B, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
            }
        }
        self->stateFlags &= ~ENHORSE_TURNING_TO_PLAYER;
        if (distToPlayer < 100.0f) {
            self->animationIdx = 4;
            EnHorse_PlayIdleAnimation(self, self->animationIdx, 0.0f, 0.0f);
            self->stateFlags &= ~ENHORSE_UNRIDEABLE;
        } else {
            EnHorse_SetFollowAnimation(self, play);
        }
    }
}

static inline void EnHorse_StartGalloping(EnHorse* self) {
    f32 morphFrames;
    void* anim;

    self->action = ENHORSE_ACT_MOUNTED_GALLOP;
    if (self->animationIdx == ENHORSE_ANIM_TROT) {
        morphFrames = 0.0f;
        self->animationIdx = 8;
    } else {
        self->animationIdx = ENHORSE_ANIM_GALLOP;
        morphFrames = -3.0f;
    }
    self->unk_E98 = 0;
    anim = En_Horse_sAnimationHeaders[self->type][self->animationIdx];
    Animation_Change_00375c08(&self->skin, anim, 1.5f, 0.0f, Animation_GetLastFrame(&self->skin, anim), 2, morphFrames);
}

void EnHorse_Frozen(EnHorse* self, PlayState* play) {
    self->noInputTimer--;
    self->actor.speedXZ = 0.0f;
    if (self->noInputTimer < 0) {
        self->colliderCylinder1.base.ocFlags1 |= 1;
        self->colliderCylinder2.base.ocFlags1 |= 1;
        self->colliderJntSph.base.ocFlags1 |= 1;
        if (self->playerControlled == true) {
            self->stateFlags &= ~ENHORSE_FLAG_7;
            if (self->actor.params == 4) {
                EnHorse_StartMountedIdleResetAnim(self);
            } else if (self->actor.params == 9) {
                self->actor.params = 5;
                if (play->GetCutsceneState() != 0) {
                    EnHorse_StartMountedIdle(self);
                } else {
                    self->actor.speedXZ = 8.0f;
                    EnHorse_StartGalloping(self);
                }
            } else if (self->prevAction == 2) {
                EnHorse_StartMountedIdle(self);
            } else {
                EnHorse_StartMountedIdleResetAnim(self);
            }
            if (self->actor.params != 0) {
                self->actor.params = 0;
                return;
            }
        } else {
            if (self->prevAction == 5) {
                EnHorse_PlayIdleAnimation(self, 0, 0.0f, self->curFrame);
                return;
            }
            if (self->prevAction == 6) {
                EnHorse_PlayIdleAnimation(self, 0, 0.0f, self->curFrame);
                return;
            }
            EnHorse_PlayIdleAnimation(self, 0, 0.0f, self->curFrame);
        }
    }
}
void FUN_003913a8(void) {}

void EnHorse_HighJump(EnHorse* self, PlayState* play) {
    f32 curFrame;

    self->stateFlags |= ENHORSE_JUMPING;
    self->unk_E40 |= 2;
    self->unk_EC4 = 0;
    self->actor.speedXZ = 13.0f;
    curFrame = *EnHorse_CurFramePtr(&self->skin);
    if (curFrame > 23.0f) {
        self->actor.gravity = -3.5f;
        if (self->actor.velocity.y == 0.0f) {
            self->actor.velocity.y = -10.5f;
        }
        if (self->actor.floorHeight + 90.0f > self->actor.world.pos.y) {
            SkelAnime_SetPlaySpeed(&self->skin, 1.5f);
        } else {
            SkelAnime_SetPlaySpeed(&self->skin, 0.0f);
        }
    } else {
        self->unk_EC4 = 1;
        self->actor.world.pos.y = self->jumpStartY;
    }

    if (FUN_003731e0(&self->skin) ||
        (curFrame > 23.0f && self->actor.floorHeight - self->actor.velocity.y + 80.0f > self->actor.world.pos.y)) {
        Audio_PlaySoundGeneral(0x01000170, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        self->stateFlags &= ~ENHORSE_JUMPING;
        self->unk_E40 &= ~2;
        self->actor.gravity = -3.5f;
        self->actor.world.pos.y = self->actor.floorHeight;
        FUN_0037308c(play, 25.0f, &self->actor.world.pos);
        self->action = ENHORSE_ACT_MOUNTED_GALLOP;
        self->animationIdx = 11;
        Animation_Change_00375c08(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx], 1.5f, 0.0f,
                                  Animation_GetLastFrame(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx]), 2,
                                  0.0f);
        self->postDrawFunc = NULL;
    }
}
GLOBAL_ASM("../split/asm/FUN_003a1f84.s")
GLOBAL_ASM("../split/asm/FUN_003a1fc4.s")

void EnHorse_HighJumpInit(EnHorse* self) {
    f32 zero = 0.0f;

    SkelAnime_SetCurrentFrame(&self->skin, zero);
    self->action = ENHORSE_ACT_HIGH_JUMP;
    self->animationIdx = 13;
    f32 curFrame = *EnHorse_CurFramePtr(&self->skin);
    Animation_Change_00375c08(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx], 1.5f, curFrame,
                              Animation_GetLastFrame(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx]), 2, -3.0f);
    self->jumpStartY = self->actor.world.pos.y;
    self->actor.gravity = zero;
    self->actor.velocity.y = zero;
    self->postDrawFunc = NULL;
    self->stateFlags |= ENHORSE_CALC_RIDER_POS;
    Audio_PlaySoundGeneral(0x0100016F, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
}

extern u8 EnHorse_sIdleAnimIds[];

void EnHorse_Idle(EnHorse* self, PlayState* play) {
    self->actor.speedXZ = 0.0f;
    EnHorse_IdleAnimSounds(self, play);

    if (R_EPONAS_SONG_PLAYED && self->type == HORSE_EPONA) {
        R_EPONAS_SONG_PLAYED = false;
        if (!EnHorse_IsSpawnBlocked(play, self, &self->actor.world.pos)) {
            if (EnHorse_Spawn(self, play)) {
                Audio_PlaySoundGeneral(0x0100015C, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
                self->followTimer = 0;
                EnHorse_SetFollowAnimation(self, play);
                Camera_SetParam(play->cameraPtrs[0], 8, &self->actor);
                Camera_ChangeSetting(play->cameraPtrs[0], 0x38);
                Camera_SetCameraData(play->cameraPtrs[0], 4, NULL, NULL, 0x51, 0, 0);
            }
        } else {
            Audio_PlaySoundGeneral(0x0100015C, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
            self->followTimer = 0;
            EnHorse_StartMovingAnimation(self, 7, -3.0f, 0.0f);
        }
    }

    if (FUN_003731e0(&self->skin)) {
        s32 idleAnimIdx = 0;

        if (self->animationIdx != ENHORSE_ANIM_IDLE) {
            if (self->animationIdx == ENHORSE_ANIM_WHINNY) {
                idleAnimIdx = 1;
            } else if (self->animationIdx == ENHORSE_ANIM_REARING) {
                idleAnimIdx = 2;
            }
        }

        EnHorse_PlayIdleAnimation(self, EnHorse_sIdleAnimIds[(Rand_ZeroOne() > 0.5f ? 0 : 1) + idleAnimIdx * 2], 0.0f, 0.0f);
    }
}

s32 Animation_OnFrameImpl(SkelAnime* skelAnime, f32 frame, f32 updateRate);

void EnHorse_IdleAnimSounds(EnHorse* self, PlayState* play) {
    if (self->animationIdx == ENHORSE_ANIM_IDLE &&
        ((Animation_OnFrameImpl(&self->skin, 71.0f, 2.0f / 3.0f) && self->type == HORSE_EPONA) ||
         (Animation_OnFrameImpl(&self->skin, 87.0f, 2.0f / 3.0f) && self->type == HORSE_EPONA) ||
         (Animation_OnFrameImpl(&self->skin, 47.0f, 2.0f / 3.0f) && self->type == HORSE_HNI) ||
         (Animation_OnFrameImpl(&self->skin, 57.5f, 2.0f / 3.0f) && self->type == HORSE_HNI)) &&
        !(self->stateFlags & ENHORSE_SANDDUST_SOUND)) {
        Audio_PlaySoundGeneral(0x01000183, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
    } else if (self->animationIdx == ENHORSE_ANIM_REARING && self->curFrame > 25.0f &&
               !(self->stateFlags & ENHORSE_LAND2_SOUND)) {
        self->stateFlags |= ENHORSE_LAND2_SOUND;
        Audio_PlaySoundGeneral(0x01000182, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
    }
}

void EnHorse_Inactive(EnHorse* self, PlayState* play2) {
    PlayState* play = play2;

    if (R_EPONAS_SONG_PLAYED && self->type == HORSE_EPONA) {
        R_EPONAS_SONG_PLAYED = false;
        if (EnHorse_Spawn(self, play)) {
            Audio_PlaySoundGeneral(0x0100015C, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
            self->stateFlags &= ~ENHORSE_INACTIVE;
            gSaveContext.horseData.scene = play->sceneNum;

            Camera_SetParam(play->cameraPtrs[0], 8, &self->actor);
            Camera_ChangeSetting(play->cameraPtrs[0], 0x38);
            Camera_SetCameraData(play->cameraPtrs[0], 4, NULL, NULL, 0x51, 0, 0);
        }
    }
    if (!(self->stateFlags & ENHORSE_INACTIVE)) {
        self->followTimer = 0;
        EnHorse_SetFollowAnimation(self, play);
        self->actor.params = 0;
        self->colliderCylinder1.base.ocFlags1 |= 1;
        self->colliderCylinder2.base.ocFlags1 |= 1;
        self->colliderJntSph.base.ocFlags1 |= 1;
    }
}

#define EnHorse_sPlaybackSpeeds ((f32*)((u8*)En_Horse_sAnimationHeaders + 0x28))

#if 0 // 2 words off: order of the 0.0f/1.5f constant loads at entry (vldr s0 before vldr s17) (Sonnet). Opus: literal 0.0f compare fixes code order but swaps pool order (4 words); late scale decl/no locals much worse
void EnHorse_InitHorsebackArchery(EnHorse* self) {
    s32 animChanged = 0;
    f32 animSpeed;
    f32 zero = 0.0f;
    f32 scale = 1.5f;

    self->action = ENHORSE_ACT_HBA;
    if (self->actor.speedXZ == zero) {
        if (self->animationIdx != ENHORSE_ANIM_IDLE) {
            animChanged = true;
        }
        self->animationIdx = ENHORSE_ANIM_IDLE;
    } else if (self->actor.speedXZ <= 3.0f) {
        if (self->animationIdx != ENHORSE_ANIM_WALK) {
            animChanged = true;
        }
        self->animationIdx = ENHORSE_ANIM_WALK;
    } else if (self->actor.speedXZ <= 6.0f) {
        if (self->animationIdx != ENHORSE_ANIM_TROT) {
            animChanged = true;
        }
        self->animationIdx = ENHORSE_ANIM_TROT;
    } else {
        if (self->animationIdx != ENHORSE_ANIM_GALLOP) {
            animChanged = true;
        }
        self->animationIdx = ENHORSE_ANIM_GALLOP;
    }

    if (self->animationIdx == ENHORSE_ANIM_WALK) {
        animSpeed = self->actor.speedXZ * 0.5f;
    } else if (self->animationIdx == ENHORSE_ANIM_TROT) {
        animSpeed = self->actor.speedXZ * 0.25f;
        Audio_PlaySoundGeneral(0x0100015B, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
    } else if (self->animationIdx == ENHORSE_ANIM_GALLOP) {
        animSpeed = self->actor.speedXZ * 0.3f;
        Audio_PlaySoundGeneral(0x0100015B, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
    } else {
        animSpeed = 1.0f;
    }

    f32* speeds = EnHorse_sPlaybackSpeeds;
    if (animChanged == true) {
        Animation_Change_00375c08(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx],
                                  speeds[self->animationIdx] * (animSpeed * scale), 0,
                                  Animation_GetLastFrame(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx]), 2,
                                  -3.0f);
    } else {
        Animation_Change_00375c08(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx],
                                  speeds[self->animationIdx] * (animSpeed * scale), 0,
                                  Animation_GetLastFrame(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx]), 2, 0);
    }
}
#endif
GLOBAL_ASM("asm/EnHorse_InitHorsebackArchery.s")

void EnHorse_LowJump(EnHorse* self, PlayState* play) {
    f32 curFrame;

    self->stateFlags |= ENHORSE_JUMPING;
    self->unk_E40 |= 2;
    self->actor.speedXZ = 12.0f;
    self->unk_EC4 = 0;
    curFrame = *EnHorse_CurFramePtr(&self->skin);
    if (curFrame > 17.0f) {
        self->actor.gravity = -3.5f;
        if (self->actor.velocity.y == 0.0f) {
            self->actor.velocity.y = -6.0f;
        }
        if (self->actor.floorHeight + 90.0f > self->actor.world.pos.y) {
            SkelAnime_SetPlaySpeed(&self->skin, 1.5f);
        } else {
            SkelAnime_SetPlaySpeed(&self->skin, 0.0f);
        }
    } else {
        self->unk_EC4 = 1;
    }

    if (FUN_003731e0(&self->skin) ||
        (curFrame > 17.0f && self->actor.floorHeight - self->actor.velocity.y + 80.0f > self->actor.world.pos.y)) {
        Audio_PlaySoundGeneral(0x01000170, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        self->stateFlags &= ~ENHORSE_JUMPING;
        self->unk_E40 &= ~2;
        self->actor.gravity = -3.5f;
        self->actor.world.pos.y = self->actor.floorHeight;
        FUN_0037308c(play, 25.0f, &self->actor.world.pos);
        self->action = ENHORSE_ACT_MOUNTED_GALLOP;
        self->animationIdx = 10;
        Animation_Change_00375c08(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx], 1.5f, 0.0f,
                                  Animation_GetLastFrame(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx]), 2,
                                  0.0f);
        self->postDrawFunc = NULL;
    }
}

#if 1
typedef struct {
    /* 0x00 */ s32 animationType;
    /* 0x04 */ char unk_04[0x8];
    /* 0x0C */ f32 curFrame;
} EnHorseAnimState;

void EnHorse_LowJumpInit(EnHorse* self) {
    f32 zero = 0.0f;

    SkelAnime_SetCurrentFrame(&self->skin, zero);
    self->action = ENHORSE_ACT_LOW_JUMP;
    self->animationIdx = 12;
    f32 curFrame = *EnHorse_CurFramePtr(&self->skin);
    Animation_Change_00375c08(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx], 1.5f, curFrame,
                              Animation_GetLastFrame(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx]), 2, -3.0f);
    self->jumpStartY = self->actor.world.pos.y;
    self->actor.gravity = zero;
    self->actor.velocity.y = zero;
    self->postDrawFunc = NULL;
    Audio_PlaySoundGeneral(0x0100016F, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
}
#endif






// The 3DS tunables / tables block as one object (armcc then derives the table address from the data base address).
typedef struct {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ f32 f4;
    /* 0x08 */ f32 f8;
    /* 0x0C */ f32 fC;
    /* 0x10 */ f32 f10;
    /* 0x14 */ f32 f14;
    /* 0x18 */ f32 f18;
    /* 0x1C */ f32 f1C;
    /* 0x20 */ u8 faceTable[12];
    /* 0x2C */ void** headers[2];
    /* 0x34 */ char unk_34[0x54 - 0x34];
    /* 0x54 */ f32 speeds[9];
} EnHorseDataAll;
extern EnHorseDataAll EnHorse_sDataAll;

void FUN_00317dbc(SkelAnime* skelAnime, void* anim);


static inline void EnHorse_StartTrotting(EnHorse* self, void*** hdrs) {
    self->action = ENHORSE_ACT_MOUNTED_TROT;
    self->animationIdx = (self->animationIdx == 7) ? 6 : 5;
    Animation_Change_00375c08(&self->skin, hdrs[self->type][self->animationIdx], 1.0f, 0.0f,
                              Animation_GetLastFrame(&self->skin, hdrs[self->type][self->animationIdx]), 2,
                              -3.0f);
}

static inline void EnHorse_MountedGallopReset(EnHorse* self, void*** hdrs) {
    self->noInputTimerMax = self->noInputTimer = 0;
    self->action = ENHORSE_ACT_MOUNTED_GALLOP;
    self->animationIdx = 7;
    self->unk_E98 = 0;
    if ((void*)self->skin.animationType == hdrs[self->type][7] && *((u8*)&self->skin + 0x70) == 0) {
        return;
    }
    if ((self->stateFlags & ENHORSE_BOOST) || self->skin.playSpeed > EnHorse_sDataAll.f4) {
        self->animationIdx = 9;
    }
    Animation_Change_00375c08(&self->skin, hdrs[self->type][self->animationIdx], 1.5f, 0.0f,
                              Animation_GetLastFrame(&self->skin, hdrs[self->type][self->animationIdx]), 0,
                              0.0f);
}

#if 0 // 130 words off: const reg numbering (s16/s17, sl/fp, r6/r7 swapped), 'ps > f4' compiles as bhs not ble (Sonnet)
void EnHorse_MountedGallop(EnHorse* self, PlayState* play) {
    f32 stickMag;
    s16 stickAngle;
    f32 playSpeed;
    f32 prevFrame;
    s32 wrapped;
    void*** hdrs;

    EnHorse_StickDirection(&self->curStick, &stickMag, &stickAngle);

    if (self->noInputTimer > 0) {
        self->actor.speedXZ = 8.0f;
        self->noInputTimer--;
    } else {
        EnHorse_UpdateSpeed(self, play, 0.2f, -0.5f, 10.0f, 0.04f, 8.0f, 267);
    }

    hdrs = EnHorse_sDataAll.headers;
    if (self->animationIdx != 10 && self->animationIdx != 11) {
        playSpeed = self->actor.speedXZ * 0.45f;
        {
            f32 hi = EnHorse_sDataAll.f14;
            if (playSpeed > hi) {
                playSpeed = hi;
            }
        }
        {
            f32 lo = EnHorse_sDataAll.f8;
            if (playSpeed < lo) {
                playSpeed = lo;
            }
        }
        SkelAnime_SetPlaySpeed(&self->skin, playSpeed);
        if ((self->stateFlags & ENHORSE_BOOST) || !(playSpeed <= EnHorse_sDataAll.f4)) {
            self->animationIdx = 9;
            FUN_00317dbc(&self->skin, hdrs[self->type][9]);
        } else if (self->animationIdx != 8) {
            self->animationIdx = 7;
            FUN_00317dbc(&self->skin, hdrs[self->type][7]);
        }
    }

    prevFrame = self->skin.curFrame;
    wrapped = FUN_003731e0(&self->skin);
    if (self->skin.curFrame < prevFrame || wrapped) {
        if (self->unk_1094 == 0) {
            Audio_PlaySoundGeneral(0x0100015B, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        }
        if (EnHorse_PlayerCanMove(self, play) == true) {
            if (stickMag >= 10.0f && Math_CosS(stickAngle) <= -0.5f) {
                ((EnHorseStartBrakingFunc)EnHorse_StartBraking)(self, play);
            } else if (self->actor.speedXZ < 6.0f) {
                EnHorse_StartTrotting(self, hdrs);
            } else {
                EnHorse_MountedGallopReset(self, hdrs);
            }
            return;
        }
        EnHorse_MountedGallopReset(self, hdrs);
    }
}
#endif
GLOBAL_ASM("asm/EnHorse_MountedGallop.s")

void EnHorse_MountedIdle(EnHorse* self, PlayState* play) {
    f32 stickMag;
    s16 stickAngle = 0;

    self->actor.speedXZ = 0;
    EnHorse_StickDirection(&self->curStick, &stickMag, &stickAngle);
    if (stickMag > 10.0f && EnHorse_PlayerCanMove(self, play) == true) {
        if (Math_CosS(stickAngle) <= -0.5f) {
            EnHorse_StartReversingInterruptable(self);
        } else if (Math_CosS(stickAngle) <= 0.7071f) {
            EnHorse_StartTurning(self);
        } else {
            EnHorse_StartWalkingFromIdle(self);
        }
    }
    if (FUN_003731e0(&self->skin)) {
        f32 curFrame;

        SkelAnime_SetCurrentFrame(&self->skin, 0.0f);
        self->action = ENHORSE_ACT_MOUNTED_IDLE_WHINNEYING;
        self->animationIdx = ENHORSE_ANIM_WHINNY;
        curFrame = *EnHorse_CurFramePtr(&self->skin);
        Animation_Change_00375c08(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx], 1.0f, curFrame,
                                  Animation_GetLastFrame(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx]), 2,
                                  -3.0f);
        self->unk_E80 = self->unk_E8C;
        if (self->stateFlags & ENHORSE_DRAW) {
            Audio_PlaySoundGeneral(0x0100016D, &self->unk_E80, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        }
    }
}

void EnHorse_MountedIdleWhinneying(EnHorse* self, PlayState* play) {
    f32 stickMag;
    s16 stickAngle = 0;

    self->actor.speedXZ = 0;
    EnHorse_StickDirection(&self->curStick, &stickMag, &stickAngle);
    if (stickMag > 10.0f && EnHorse_PlayerCanMove(self, play) == true) {
        if (Math_CosS(stickAngle) <= -0.5f) {
            EnHorse_StartReversingInterruptable(self);
        } else if (Math_CosS(stickAngle) <= 0.7071f) {
            EnHorse_StartTurning(self);
        } else {
            EnHorse_StartWalkingFromIdle(self);
        }
    }
    if (FUN_003731e0(&self->skin)) {
        EnHorse_StartMountedIdleResetAnim(self);
    }
}

static inline void EnHorse_StartWalkingInl(EnHorse* self) {
    self->action = ENHORSE_ACT_MOUNTED_WALK;
    self->unk_E7C = 0;
    self->animationIdx = ENHORSE_ANIM_WALK;
    self->waitTimer = 0;
    Animation_Change_00375c08(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx], 1.0f, 0.0f,
                              Animation_GetLastFrame(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx]), 2,
                              -3.0f);
}

void EnHorse_MountedRearing(EnHorse* self, PlayState* play) {
    f32 stickMag;
    s16 stickAngle;

    self->actor.speedXZ = 0.0f;
    if (self->curFrame > 25.0f) {
        if (!(self->stateFlags & ENHORSE_LAND2_SOUND)) {
            self->stateFlags |= ENHORSE_LAND2_SOUND;
            Audio_PlaySoundGeneral(0x01000182, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        }
    }

    EnHorse_StickDirection(&self->curStick, &stickMag, &stickAngle);
    if (FUN_003731e0(&self->skin)) {
        if (EnHorse_PlayerCanMove(self, play) == true) {
            if (self->stateFlags & ENHORSE_FORCE_REVERSING) {
                self->noInputTimer = 100;
                self->noInputTimerMax = 100;
                self->stateFlags &= ~ENHORSE_FORCE_REVERSING;
                EnHorse_StartReversing(self);
            } else if (self->stateFlags & ENHORSE_FORCE_WALKING) {
                self->noInputTimer = 100;
                self->noInputTimerMax = 100;
                self->stateFlags &= ~ENHORSE_FORCE_WALKING;
                EnHorse_StartWalkingInl(self);
            } else if (Math_CosS(stickAngle) <= -0.5f) {
                EnHorse_StartReversingInterruptable(self);
            } else {
                EnHorse_StartMountedIdleResetAnim(self);
            }
            return;
        }
        EnHorse_StartMountedIdleResetAnim(self);
    }
}

typedef struct {
    f32 f[5];
} EnHorseF5;
extern struct {
    EnHorseF5 a;
    EnHorseF5 b;
} EnHorse_sTrotFramesA;
f32 FUN_0036b4d0(SkelAnime* skelAnime, f32 frame);
void Animation_PlayLoop(SkelAnime* skelAnime);
void EnHorse_UpdateSpeed(EnHorse* self, PlayState* play, f32 brakeDecel, f32 brakeAngle, f32 minStickMag, f32 decel, f32 baseSpeed,
                         s16 turnSpeed);

#if 0 // 71 words off: armcc hoists 3.0f into fp (ROM hoists integer 0 into fp: mov fp,#0); rest is structurally identical (Sonnet)
// Opus: both speedXZ < 3.0f tests are int-bit compares; ROM reloads the 3.0f literal (ldr r0,=0x40400000) at each and CSEs 0 instead. Not solved.
void EnHorse_MountedTrot(EnHorse* self, PlayState* play) {
    f32 stickMag;
    s16 stickAngle;
    EnHorseF5 framesA = EnHorse_sTrotFramesA.a;
    EnHorseF5 framesB = EnHorse_sTrotFramesA.b;
    f32 playSpeed;
    f32 prevFrame;

    EnHorse_UpdateSpeed(self, play, 0.2f, -0.5f, 10.0f, 0.04f, 6.0f, 267);
    EnHorse_StickDirection(&self->curStick, &stickMag, &stickAngle);
    if (self->actor.speedXZ < 3.0f) {
        s32 best = -1;
        f32 bestVal = 1000.0f;
        s32 i;

        for (i = 0; i < 5; i++) {
            f32 d = EnHorse_Fabsf(FUN_0036b4d0(&self->skin, framesA.f[i]));

            if (bestVal > d) {
                bestVal = d;
                best = i;
            }
        }
        if (best != -1) {
            f32 startFrame = framesB.f[best];

            self->noInputTimer = 0;
            self->noInputTimerMax = 0;
            self->action = ENHORSE_ACT_MOUNTED_WALK;
            self->unk_E7C = 0;
            self->animationIdx = ENHORSE_ANIM_WALK;
            self->waitTimer = 0;
            Animation_Change_00375c08(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx], 1.0f, startFrame,
                                      Animation_GetLastFrame(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx]),
                                      2, -3.0f);
        } else {
            EnHorse_StartWalking(self);
        }
    }

    playSpeed = self->actor.speedXZ * 0.375f;
    if (self->animationIdx == 6 && playSpeed < EnHorse_sData.f8 && self->skin.curFrame < 30.0f) {
        f32 t = self->skin.curFrame / 30.0f;

        playSpeed = (1.0f - t) * EnHorse_sData.f8 + playSpeed * t;
    }
    SkelAnime_SetPlaySpeed(&self->skin, playSpeed);
    prevFrame = self->skin.curFrame;
    s32 wrapped = FUN_003731e0(&self->skin);
    if (self->skin.curFrame < prevFrame || wrapped) {
        if (self->unk_1094 == 0) {
            Audio_PlaySoundGeneral(0x0100015B, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        }
        if (self->actor.speedXZ >= 6.0f) {
            self->noInputTimerMax = self->noInputTimer = 0;
            EnHorse_StartGalloping(self);
        } else if (self->actor.speedXZ < 3.0f) {
            EnHorse_StartWalking(self);
        } else {
            self->action = ENHORSE_ACT_MOUNTED_TROT;
            self->animationIdx = ENHORSE_ANIM_TROT;
            if (!((void*)self->skin.animationType == En_Horse_sAnimationHeaders[self->type][ENHORSE_ANIM_TROT] &&
                  *((u8*)&self->skin + 0x70) == 0)) {
                Animation_PlayLoop(&self->skin);
            }
        }
    }
}
#endif
GLOBAL_ASM("asm/EnHorse_MountedTrot.s")

void EnHorse_MountedTurn(EnHorse* self, PlayState* play) {
    f32 stickMag;
    s16 clampedYaw;
    s16 stickAngle;

    self->actor.speedXZ = 0;
    EnHorse_PlayWalkingSound(self);
    EnHorse_StickDirection(&self->curStick, &stickMag, &stickAngle);
    if (stickMag > 10.0f) {
        if (!EnHorse_PlayerCanMove(self, play)) {
            EnHorse_StartMountedIdleResetAnim(self);
        } else if (Math_CosS(stickAngle) <= -0.5f) {
            EnHorse_StartReversingInterruptable(self);
        } else if (Math_CosS(stickAngle) <= 0.7071f) {
            clampedYaw = ENHORSE_CLAMP(stickAngle, -534.0f, 534.0f);
            self->actor.world.rot.y += clampedYaw;
            self->actor.shape.rot.y = self->actor.world.rot.y;
        } else {
            EnHorse_StartWalking(self);
        }
    }

    if (FUN_003731e0(&self->skin)) {
        if (Math_CosS(stickAngle) <= 0.7071f) {
            EnHorse_StartTurning(self);
        } else {
            EnHorse_StartMountedIdleResetAnim(self);
        }
    }
}

static inline void EnHorse_StartTrottingW(EnHorse* self) {
    self->action = ENHORSE_ACT_MOUNTED_TROT;
    if (self->animationIdx != 7) {
        self->animationIdx = 5;
    } else {
        self->animationIdx = 6;
    }
    Animation_Change_00375c08(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx], 1.0f, 0.0f,
                              Animation_GetLastFrame(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx]), 2,
                              -3.0f);
}

#if 0 // 175 words off: armcc hoists different constants into sl/fp (ROM keeps 9 and 6 in registers across the two inlined trot starts) (Sonnet)
void EnHorse_MountedWalk(EnHorse* self, PlayState* play) {
    f32 stickMag;
    s16 stickAngle;

    if (self->animationIdx == ENHORSE_ANIM_WALK) {
        EnHorse_PlayWalkingSound(self);
    }
    EnHorse_StickDirection(&self->curStick, &stickMag, &stickAngle);
    if (self->noInputTimerMax == 0.0f ||
        (self->noInputTimer > 0.0f && self->noInputTimer < self->noInputTimerMax - 30.0f)) {
        EnHorse_UpdateSpeed(self, play, 0.2f, -0.5f, 10.0f, 0.04f, 3.0f, 267);
    } else {
        self->actor.speedXZ = 3.0f;
    }

    if (self->actor.speedXZ == 0.0f) {
        self->stateFlags &= ~ENHORSE_FLAG_9;
        EnHorse_StartMountedIdleResetAnim(self);
        self->noInputTimer = 0;
        self->noInputTimerMax = 0;
        FUN_003731e0(&self->skin);
        return;
    } else if (self->actor.speedXZ > 3.0f) {
        self->stateFlags &= ~ENHORSE_FLAG_9;
        EnHorse_StartTrottingW(self);
        self->noInputTimer = 0;
        self->noInputTimerMax = 0;
    } else if (self->noInputTimer > 0) {
        self->noInputTimer--;
        if (self->noInputTimer <= 0) {
            self->noInputTimerMax = 0;
        }
    }

    if (self->waitTimer <= 0) {
        self->stateFlags &= ~ENHORSE_FLAG_9;
        SkelAnime_SetPlaySpeed(&self->skin, self->actor.speedXZ * 0.75f);
        if (FUN_003731e0(&self->skin) || self->actor.speedXZ == 0.0f) {
            if (self->noInputTimer <= 0) {
                if (self->actor.speedXZ > 3.0f) {
                    EnHorse_StartTrottingW(self);
                    self->noInputTimer = 0;
                    self->noInputTimerMax = 0;
                } else if ((stickMag < 10.0f) || (Math_CosS(stickAngle) <= -0.5f)) {
                    EnHorse_StartMountedIdleResetAnim(self);
                    self->noInputTimer = 0;
                    self->noInputTimerMax = 0;
                } else {
                    self->action = ENHORSE_ACT_MOUNTED_WALK;
                    self->unk_E7C = 0;
                    self->animationIdx = ENHORSE_ANIM_WALK;
                    self->waitTimer = 0;
                    Animation_PlayOnce(&self->skin, En_Horse_sAnimationHeaders[self->type][ENHORSE_ANIM_WALK]);
                }
            }
        }
    } else {
        self->actor.speedXZ = 0.0f;
        self->waitTimer--;
        FUN_003731e0(&self->skin);
        if (self->waitTimer == 0) {
            self->action = ENHORSE_ACT_MOUNTED_WALK;
            self->unk_E7C = 0;
            self->animationIdx = ENHORSE_ANIM_WALK;
            Animation_Change_00375c08(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx], 1.0f, 0.0f,
                                      Animation_GetLastFrame(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx]),
                                      2, -3.0f);
        }
    }
}
#endif
GLOBAL_ASM("asm/EnHorse_MountedWalk.s")

s32 EnHorse_BgCheckBridgeJumpPoint(EnHorse* self, PlayState* play);

static inline void EnHorse_StartRearing(EnHorse* self) {
    self->action = ENHORSE_ACT_MOUNTED_REARING;
    self->animationIdx = ENHORSE_ANIM_REARING;
    self->stateFlags &= ~ENHORSE_LAND2_SOUND;
    self->unk_E80 = self->unk_E8C;
    if (self->stateFlags & ENHORSE_DRAW) {
        Audio_PlaySoundGeneral(0x0100015C, &self->unk_E80, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
    }
    Animation_Change_00375c08(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx], 1.0f, 0.0f,
                              Animation_GetLastFrame(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx]), 2,
                              -3.0f);
}

void EnHorse_ObstructMovement(EnHorse* self, PlayState* play, s32 obstacleType, s32 galloping) {
    if (self->action == ENHORSE_ACT_CS_UPDATE || EnHorse_BgCheckBridgeJumpPoint(self, play)) {
        return;
    }

    self->actor.world.pos = self->lastPos;
    self->actor.shape.rot.y = self->lastYaw;
    self->actor.world.rot.y = self->lastYaw;
    self->stateFlags |= ENHORSE_OBSTACLE;

    if (self->playerControlled == 0 || self->action == ENHORSE_ACT_MOUNTED_REARING) {
        return;
    }
    if (self->stateFlags & ENHORSE_JUMPING) {
        self->stateFlags &= ~ENHORSE_JUMPING;
        self->unk_E40 &= ~2;
        self->actor.gravity = -3.5f;
        self->actor.world.pos.y = self->actor.floorHeight;
        self->unk_EC4 = 0;
    }
    if (obstacleType == 1 || obstacleType == 4) {
        self->stateFlags |= ENHORSE_FORCE_REVERSING;
    } else if (obstacleType == 2 || obstacleType == 5) {
        self->stateFlags |= ENHORSE_FORCE_WALKING;
    }
    if (galloping == true) {
        EnHorse_StartRearing(self);
    }
}

GLOBAL_ASM("asm/EnHorse_OverrideLimbDraw.s")

void EnHorse_PlayIdleAnimation(EnHorse* self, u8 anim, f32 morphFrames, f32 startFrame) {
    self->action = ENHORSE_ACT_IDLE;
    self->actor.speedXZ = 0.0f;
    if (anim != ENHORSE_ANIM_IDLE && anim != ENHORSE_ANIM_WHINNY && anim != ENHORSE_ANIM_REARING) {
        anim = ENHORSE_ANIM_IDLE;
    }
    if (anim != self->animationIdx) {
        self->animationIdx = anim;
        if (self->animationIdx == ENHORSE_ANIM_IDLE) {
            self->stateFlags &= ~ENHORSE_SANDDUST_SOUND;
        } else if (self->animationIdx == ENHORSE_ANIM_WHINNY) {
            self->unk_E80 = self->unk_E8C;
            if (self->stateFlags & ENHORSE_DRAW) {
                Audio_PlaySoundGeneral(0x0100016D, &self->unk_E80, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
            }
        } else if (self->animationIdx == ENHORSE_ANIM_REARING) {
            self->unk_E80 = self->unk_E8C;
            if (self->stateFlags & ENHORSE_DRAW) {
                Audio_PlaySoundGeneral(0x0100015C, &self->unk_E80, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
            }
            self->stateFlags &= ~ENHORSE_LAND2_SOUND;
        }

        Animation_Change_00375c08(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx], 1.0f, startFrame,
                                  Animation_GetLastFrame(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx]), 2,
                                  morphFrames);
    }
}

void EnHorse_PlayWalkingSound(EnHorse* self) {
    EnHorsePair frames = EnHorse_sWalkFrames;
    f32 curFrame = self->curFrame;
    s32 idx = self->unk_E7C;

    if (frames.f[idx] < self->curFrame) {
        if (idx != 0 || self->curFrame <= frames.f[1]) {
            Audio_PlaySoundGeneral(0x0100015A, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
            self->unk_E7C = self->unk_E7C + 1;
            if (self->unk_E7C >= 2) {
                self->unk_E7C = 0;
            }
        }
    }
}


s32 EnHorse_PlayerCanMove(EnHorse* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if ((*(u32*)((u8*)player + 0x1710) & PLAYER_STATE1_0) || FUN_0033bd6c(GET_PLAYER(play)) == true ||
        (*(u32*)((u8*)player + 0x1710) & PLAYER_STATE1_20) || ((self->stateFlags & ENHORSE_FLAG_19) && !self->inRace) ||
        self->action == ENHORSE_ACT_HBA || player->actor.flags & ACTOR_FLAG_TALK ||
        play->GetCutsceneState() != CS_STATE_IDLE) {
        return false;
    }
    return true;
}
GLOBAL_ASM("../split/asm/FUN_00326bc0.s")

extern u8 EnHorse_sFaceTable[];
void FUN_0037266c(SkeletonAnimationModel* model, s32 idx);
void FUN_0036932c(SkeletonAnimationModel* model, s32 idx);

#if 0 // 2 words off: mov r0,r4 / mov r1,#0 order before FaceAnimations::FUN_0035e3a4 (Sonnet; Opus tried local orders/types, inline wrapper, recast, no change)
void EnHorse_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnHorse* self = (EnHorse*)thisx;

    if (limbIndex == 0xD && self->type == 0) {
        u8 faceIdx = *(u8*)((u8*)self + 0x1004);
        FaceAnimations* face = (FaceAnimations*)self->unk_C74;

        face->FUN_0035e3a4(0, EnHorse_sFaceTable[faceIdx]);
        face->FUN_0035e330();
    } else if (self->type == 1 && limbIndex == 0xE) {
        if (self->stateFlags & ENHORSE_FLAG_18) {
            FUN_0037266c(self->skin.unk_28, 0);
            FUN_0037266c(self->skin.unk_28, 2);
            FUN_0036932c(self->skin.unk_28, 1);
            FUN_0036932c(self->skin.unk_28, 3);
        } else {
            FUN_0036932c(self->skin.unk_28, 0);
            FUN_0036932c(self->skin.unk_28, 2);
            FUN_0037266c(self->skin.unk_28, 1);
            FUN_0037266c(self->skin.unk_28, 3);
        }
    }
}
#endif
GLOBAL_ASM("asm/EnHorse_PostLimbDraw.s")

typedef struct {
    /* 0x00 */ char unk_00[0xA];
    /* 0x0A */ Vec3s normal;
    /* 0x10 */ f32 dist;
} EnHorseColPoly;

f32 Math3D_DistPlaneToPos(f32 nx, f32 ny, f32 nz, f32 d, Vec3f* pos);

#define ENHORSE_COLPOLY_NORMAL(n) ((n) * 0.00003051851f)

void EnHorse_ResolveCollision(EnHorse* self, PlayState* play, EnHorseColPoly* colPoly) {
    f32 dist;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 offset;

    nx = ENHORSE_COLPOLY_NORMAL(colPoly->normal.x);
    ny = ENHORSE_COLPOLY_NORMAL(colPoly->normal.y);
    nz = ENHORSE_COLPOLY_NORMAL(colPoly->normal.z);
    if (!(Math_CosS(self->actor.world.rot.y - (s16)(Math_FAtan2F(colPoly->normal.x, colPoly->normal.z) * (0x8000 / M_PI)) - 0x7FFF) <
          0.7071f)) {
        dist = Math3D_DistPlaneToPos(nx, ny, nz, colPoly->dist, &self->actor.world.pos);
        offset = (1.0f / sqrtf(SQ(nx) + SQ(nz)));
        offset = (30.0f - dist) * offset;
        self->actor.world.pos.x += offset * nx;
        self->actor.world.pos.z += offset * nz;
    }
}


#if 0 // 133 words off: block layout - ROM puts the player-flags/reset exit chunk after the function's main epilogue (ours inline) (Sonnet)
void EnHorse_Reverse(EnHorse* self, PlayState* play) {
    f32 stickMag;
    s16 stickAngle;
    s16 turnAmount;
    Player* player = GET_PLAYER(play);

    EnHorse_PlayWalkingSound(self);
    EnHorse_StickDirection(&self->curStick, &stickMag, &stickAngle);
    if (EnHorse_PlayerCanMove(self, play) == true) {
        if (self->noInputTimerMax == 0.0f ||
            (self->noInputTimer > 0.0f && self->noInputTimer < self->noInputTimerMax - 30.0f)) {
            if (stickMag < 10.0f && self->noInputTimer <= 0.0f) {
                EnHorse_StartMountedIdleResetAnim(self);
                self->actor.speedXZ = 0.0f;
                return;
            }
            if (stickMag < 10.0f) {
                stickAngle = -0x7FFF;
            } else if (Math_CosS(stickAngle) > -0.5f) {
                self->noInputTimerMax = 0;
                EnHorse_StartMountedIdleResetAnim(self);
                self->actor.speedXZ = 0.0f;
                return;
            }
        } else if (stickMag < 10.0f) {
            stickAngle = -0x7FFF;
        }
    } else if (player->actor.flags & 0x100) {
        EnHorse_StartMountedIdleResetAnim(self);
        self->actor.speedXZ = 0.0f;
        return;
    } else {
        stickAngle = -0x7FFF;
    }

    self->actor.speedXZ = -2.0f;
    turnAmount = 0x7FFF - stickAngle;
    turnAmount = ENHORSE_CLAMP(turnAmount, -801.0f, 801.0f);
    self->actor.world.rot.y += turnAmount;
    self->actor.shape.rot.y = self->actor.world.rot.y;

    if (self->noInputTimer > 0.0f) {
        self->noInputTimer--;
        if (self->noInputTimer <= 0.0f) {
            self->noInputTimerMax = 0;
        }
    }
    SkelAnime_SetPlaySpeed(&self->skin, self->actor.speedXZ * 0.5f * 1.5f);
    if (FUN_003731e0(&self->skin) && (f32)self->noInputTimer <= 0.0f &&
        EnHorse_PlayerCanMove(self, play) == true) {
        if (stickMag > 10.0f && Math_CosS(stickAngle) <= -0.5f) {
            self->noInputTimerMax = 0;
            EnHorse_StartReversingInterruptable(self);
        } else if (stickMag < 10.0f) {
            self->noInputTimerMax = 0;
            EnHorse_StartMountedIdleResetAnim(self);
        } else {
            EnHorse_StartReversing(self);
        }
    }
}
#endif
GLOBAL_ASM("asm/EnHorse_Reverse.s")

void EnHorse_RotateToPoint(EnHorse* self, Vec3f* pos, s16 turnAmount) {
    s16 x = Math_Vec3f_Yaw(&self->actor.world.pos, pos) - self->actor.world.rot.y;

    if (x > turnAmount) {
        self->actor.world.rot.y += turnAmount;
    } else if (x < -turnAmount) {
        self->actor.world.rot.y -= turnAmount;
    } else {
        self->actor.world.rot.y += x;
    }

    self->actor.shape.rot.y = self->actor.world.rot.y;
}

void EnHorse_SetFollowAnimation(EnHorse* self, PlayState* play) {
    s32 animId = ENHORSE_ANIM_WALK;
    f32 distToPlayer = Actor_WorldDistXZToActor(&self->actor, &GET_PLAYER(play)->actor);

    if (distToPlayer > 400.0f) {
        animId = ENHORSE_ANIM_GALLOP;
    } else if (!(distToPlayer <= 300.0f)) {
        if (distToPlayer <= 400.0f) {
            animId = ENHORSE_ANIM_TROT;
        }
    }

    if (self->animationIdx == ENHORSE_ANIM_GALLOP) {
        if (distToPlayer > 400.0f) {
            animId = ENHORSE_ANIM_GALLOP;
        } else {
            animId = ENHORSE_ANIM_TROT;
        }
    } else if (self->animationIdx == ENHORSE_ANIM_TROT) {
        if (distToPlayer > 400.0f) {
            animId = ENHORSE_ANIM_GALLOP;
        } else if (distToPlayer < 300.0f) {
            animId = ENHORSE_ANIM_WALK;
        } else {
            animId = ENHORSE_ANIM_TROT;
        }
    } else if (self->animationIdx == ENHORSE_ANIM_WALK) {
        if (distToPlayer > 300.0f) {
            animId = ENHORSE_ANIM_TROT;
        } else {
            animId = ENHORSE_ANIM_WALK;
        }
    }
    ((EnHorseStartMovingAnimFunc)EnHorse_StartMovingAnimation)(self, animId, -3.0f, 0.0f);
}

s32 EnHorse_Spawn(EnHorse* self, PlayState* play) {
    f32 minDist = 1e38f;
    s32 spawn = false;
    f32 dist;
    u32 i;
    Player* player;
    Vec3f spawnPos;

    for (i = 0; i < 169; i++) {
        EnHorseSpawnpoint* hs = EnHorse_sHorseSpawns;
        if (hs[i].sceneId == play->sceneNum) {
            player = GET_PLAYER(play);
            if (play->sceneNum != 0x63 ||
                (Flags_GetEventChkInf(0x18) &&
                 ((gSaveContext.eventInf[0] & 0xF) != 6 || Flags_GetEventChkInf(0x18))) ||
                ((EnHorse_sHorseSpawns[i].pos.x == 856 && EnHorse_sHorseSpawns[i].pos.y == 0 &&
                  EnHorse_sHorseSpawns[i].pos.z == -918) ||
                 (EnHorse_sHorseSpawns[i].pos.x == -1003 && EnHorse_sHorseSpawns[i].pos.y == 0 &&
                  EnHorse_sHorseSpawns[i].pos.z == -755))) {

                spawnPos.x = EnHorse_sHorseSpawns[i].pos.x;
                spawnPos.y = EnHorse_sHorseSpawns[i].pos.y;
                spawnPos.z = EnHorse_sHorseSpawns[i].pos.z;
                dist = Math_Vec3f_DistXYZ(&player->actor.world.pos, &spawnPos);

                if ((minDist < dist) || EnHorse_IsSpawnBlocked(play, self, &spawnPos)) {
                    continue;
                }

                minDist = dist;
                self->actor.world.pos.x = EnHorse_sHorseSpawns[i].pos.x;
                self->actor.world.pos.y = EnHorse_sHorseSpawns[i].pos.y;
                self->actor.world.pos.z = EnHorse_sHorseSpawns[i].pos.z;
                {
                    CollisionPoly* floorPoly;
                    s32 bgId;
                    Vec3f rayPos = self->actor.world.pos;

                    rayPos.y += EnHorse_sData.f10;
                    self->actor.world.pos.y =
                        BgCheck_EntityRaycastFloor4(&play->colCtx, &floorPoly, &bgId, &self->actor, &rayPos);
                }
                self->actor.prevPos = self->actor.world.pos;
                self->actor.world.rot.y = EnHorse_sHorseSpawns[i].angle;
                self->actor.shape.rot.y = Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(play)->actor);
                spawn = true;
                FUN_0033b11c((u8*)play + 0x5BB4, &self->actor.world.pos, (Vec3f*)&self->actor.unk_EC, &self->actor.unk_F8);
            }
        }
    }

    return spawn;
}

void EnHorse_StartBraking(EnHorse* self) {
    self->action = ENHORSE_ACT_STOPPING;
    self->animationIdx = (self->animationIdx == 9) ? 2 : 14;

    Audio_PlaySoundGeneral(0x01000171, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
    Animation_Change_00375c08(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx], 1.5f, 0.0f,
                              Animation_GetLastFrame(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx]), 2, -3.0f);

    self->stateFlags |= ENHORSE_STOPPING_NEIGH_SOUND;
    self->stateFlags &= ~ENHORSE_BOOST;
}

void EnHorse_StartMountedIdle(EnHorse* self) {
    f32 curFrame;

    self->action = ENHORSE_ACT_MOUNTED_IDLE;
    self->animationIdx = ENHORSE_ANIM_IDLE;
    self->unk_EC4 = 0;
    if ((self->curFrame > 35.0f && self->type == HORSE_EPONA) || (self->curFrame > 28.0f && self->type == HORSE_HNI)) {
        if (!(self->stateFlags & ENHORSE_SANDDUST_SOUND)) {
            self->stateFlags |= ENHORSE_SANDDUST_SOUND;
            Audio_PlaySoundGeneral(0x01000183, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        }
    }
    curFrame = *EnHorse_CurFramePtr(&self->skin);
    if ((void*)self->skin.animationType != En_Horse_sAnimationHeaders[self->type][self->animationIdx]) {
        Animation_Change_00375c08(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx], 1.0f, curFrame,
                                  Animation_GetLastFrame(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx]), 2, -3.0f);
    }
}

void EnHorse_StartMovingAnimation(EnHorse* self, u8 animId, f32 morphFrames, f32 startFrame) {
    f32 speed;

    self->action = ENHORSE_ACT_FOLLOW_PLAYER;
    self->stateFlags &= ~ENHORSE_TURNING_TO_PLAYER;
    if (animId != ENHORSE_ANIM_TROT && animId != ENHORSE_ANIM_GALLOP && animId != 9 && animId != ENHORSE_ANIM_WALK) {
        animId = ENHORSE_ANIM_WALK;
    }
    speed = 1.0f;
    if (animId == ENHORSE_ANIM_TROT || animId == ENHORSE_ANIM_GALLOP || animId == 9 || animId == ENHORSE_ANIM_WALK) {
        speed = 1.5f;
    }
    if (self->animationIdx != animId) {
        self->animationIdx = animId;
        Animation_Change_00375c08(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx], speed, startFrame,
                                  Animation_GetLastFrame(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx]), 2,
                                  morphFrames);
    } else {
        Animation_Change_00375c08(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx], speed, startFrame,
                                  Animation_GetLastFrame(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx]), 2,
                                  0.0f);
    }
}

void EnHorse_StartReversingInterruptable(EnHorse* self) {
    self->noInputTimer = 0;
    self->noInputTimerMax = 0;
    self->action = ENHORSE_ACT_REVERSE;
    self->animationIdx = ENHORSE_ANIM_WALK;
    self->unk_E7C = 0;
    Animation_Change_00375c08(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx], 1.0f, 0.0f,
                              Animation_GetLastFrame(&self->skin, En_Horse_sAnimationHeaders[self->type][ENHORSE_ANIM_WALK]), 0, -3.0f);
}

void EnHorse_StartWalking(EnHorse* self) {
    f32 startFrame = 0.0f;

    self->noInputTimer = 0;
    self->noInputTimerMax = 0;
    self->action = ENHORSE_ACT_MOUNTED_WALK;
    self->unk_E7C = 0;
    self->animationIdx = ENHORSE_ANIM_WALK;
    self->waitTimer = 0;
    Animation_Change_00375c08(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx], 1.0f, startFrame,
                              Animation_GetLastFrame(&self->skin, En_Horse_sAnimationHeaders[self->type][ENHORSE_ANIM_WALK]), 2, -3.0f);
}


void EnHorse_StickDirection(Vec2f* curStick, f32* stickMag, s16* angle) {
    f32 dist;

    dist = sqrtf(SQ(curStick->x) + SQ(curStick->y));

    *stickMag = dist;
*stickMag = (dist > 60.0f) ? 60.0f : dist;

    *angle = (s16)(Math_FAtan2F(-curStick->x, curStick->y) * (0x8000 / M_PI));
    if (*angle != 0) {
        if (*angle > 0x8000 - 182 * EnHorse_sData.unk_00 || *angle < -0x8000 + 182 * EnHorse_sData.unk_00) {
            *angle = -0x8000;
        }
    }
}

void EnHorse_Stopping(EnHorse* self, PlayState* play) {
    if (self->actor.speedXZ > 0.0f) {
        self->actor.speedXZ *= EnHorse_sData.f18;
        if (self->actor.speedXZ <= 0.0f) {
            self->actor.speedXZ = 0.0f;
        }
    }

    if (self->stateFlags & ENHORSE_STOPPING_NEIGH_SOUND && *EnHorse_CurFramePtr(&self->skin) > 29.0f) {
        self->actor.speedXZ = 0.0f;
        if (Rand_ZeroOne() > 0.5) {
            self->unk_E80 = self->unk_E8C;
            if (self->stateFlags & ENHORSE_DRAW) {
                Audio_PlaySoundGeneral(0x0100015C, &self->unk_E80, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
            }
            self->stateFlags &= ~ENHORSE_STOPPING_NEIGH_SOUND;
        } else {
            EnHorse_StartMountedIdleResetAnim(self);
        }
    }

    if (*EnHorse_CurFramePtr(&self->skin) > 29.0f) {
        self->actor.speedXZ = 0.0f;
    } else if (self->actor.speedXZ > 3.0f && self->stateFlags & ENHORSE_FORCE_REVERSING) {
        self->actor.speedXZ = 3.0f;
    }

    if (FUN_003731e0(&self->skin)) {
        if (self->stateFlags & ENHORSE_FORCE_REVERSING) {
            self->noInputTimer = 100;
            self->noInputTimerMax = 100;
            EnHorse_StartReversing(self);
            self->stateFlags &= ~ENHORSE_FORCE_REVERSING;
        } else {
            EnHorse_StartMountedIdleResetAnim(self);
        }
    }
}

s32 BgCheck_EntityLineTest1(void* colCtx, Vec3f* posA, Vec3f* posB, Vec3f* posResult, EnHorseColPoly** outPoly, s32 checkWall,
                            s32 checkFloor, s32 checkCeil, s32 checkOneFace, s32* bgId);
f32 BgCheck_EntityRaycastFloor3(void* colCtx, EnHorseColPoly** outPoly, s32* bgId, Vec3f* pos);
s32 SurfaceType_IsHorseBlocked(void* colCtx, EnHorseColPoly* poly, s32 bgId);
s32 FUN_0035ea34(void* colCtx, EnHorseColPoly* poly, s32 bgId); // SurfaceType_GetFloorType
Actor* DynaPoly_GetActor(void* colCtx, s32 bgId);
void EnHorse_ResolveCollision(EnHorse* self, PlayState* play, EnHorseColPoly* colPoly) __attribute__((noinline));
void EnHorse_LowJumpInit(EnHorse* self);
void EnHorse_HighJumpInit(EnHorse* self);

#if 0 // 254 words off (+8 B): structurally N64-identical incl. inlined BgCheckSlowMoving/Stub calls; callee-saved VFP/GPR assignment differs (ROM shares s16 between 40.0f and intersectDist, NULL const in r8) (Sonnet)
void EnHorse_UpdateBgCheckInfo(EnHorse* self, PlayState* play) {
    Vec3f startPos;
    Vec3f endPos;
    Vec3f obstaclePos;
    f32 intersectDist;
    EnHorseColPoly* wall = NULL;
    EnHorseColPoly* obstacleFloor = NULL;
    s32 bgId;
    f32 obstacleHeight;
    f32 behindObstacleHeight;
    f32 ny;
    s32 movingFast;
    Actor* dynaPoly;
    Vec3f intersect;
    Vec3f obstacleTop;

    Actor_UpdateBgCheckInfo(play, &self->actor, play->sceneNum == 0x63 ? 19.0f : 40.0f, 35.0f, 100.0f, 0x1D);

    if (EnHorse_BgCheckBridgeJumpPoint(self, play)) {
        return;
    }

    if ((self->actor.bgCheckFlags & 8) && Math_CosS(self->actor.wallYaw - self->actor.world.rot.y) < -0.3f) {
        if (self->actor.speedXZ > 4.0f) {
            self->actor.speedXZ -= 1.0f;
            if (*((u8*)play + 0x107) == 0) {
                Audio_PlaySoundGeneral(0x01000183, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
            }
        }
    }

    if (self->stateFlags & ENHORSE_JUMPING || !self->playerControlled) {
        return;
    }

    if (self->actor.speedXZ < 0.0f) {
        return;
    }

    if (self->action == ENHORSE_ACT_STOPPING || self->action == ENHORSE_ACT_MOUNTED_REARING) {
        return;
    }

    if (self->actor.speedXZ > 8.0f) {
        if (self->actor.speedXZ < 13.8f) {
            intersectDist = 160.0f;
            movingFast = 0;
        } else {
            intersectDist = 230.0f;
            movingFast = 1;
        }
    } else {
        f32 yOffset;
        Vec3f start;
        Vec3f end;
        Vec3f intersect2;
        EnHorseColPoly* colPoly;
        s32 bgId2;

        if (play->sceneNum == 0x63) {
            yOffset = 19.0f;
        } else {
            yOffset = 40.0f;
        }
        Math_Vec3f_Copy(&start, &self->actor.world.pos);
        start.y += yOffset;

        Math_Vec3f_Copy(&end, &start);
        end.x += 30.0f * Math_SinS(self->actor.world.rot.y);
        end.y += 30.0f * Math_SinS(-self->actor.shape.rot.x);
        end.z += 30.0f * Math_CosS(self->actor.world.rot.y);
        if (BgCheck_EntityLineTest1(&play->colCtx, &start, &end, &intersect2, &colPoly, 1, 0, 0, 1, &bgId2) != 0) {
            EnHorse_ResolveCollision(self, play, colPoly);
        }
        return;
    }

    startPos = self->actor.world.pos;
    startPos.y += 19.0f;
    endPos = startPos;
    endPos.x += (intersectDist * Math_SinS(self->actor.world.rot.y));
    endPos.y += (intersectDist * Math_SinS(-self->actor.shape.rot.x));
    endPos.z += (intersectDist * Math_CosS(self->actor.world.rot.y));
    intersect = endPos;
    wall = NULL;
    if (BgCheck_EntityLineTest1(&play->colCtx, &startPos, &endPos, &intersect, &wall, 1, 0, 0, 1, &bgId) == 1) {
        intersectDist = sqrtf(SQ(startPos.x - intersect.x) + SQ(startPos.y - intersect.y) + SQ(startPos.z - intersect.z));
        self->stateFlags |= ENHORSE_OBSTACLE;
    }

    if (wall != NULL) {
        if (intersectDist < 30.0f) {
            EnHorse_ResolveCollision(self, play, wall);
        }
        if ((Math_CosS(self->actor.world.rot.y - (s16)(Math_FAtan2F(wall->normal.x, wall->normal.z) * (0x8000 / M_PI)) - 0x7FFF) < 0.5f) ||
            SurfaceType_IsHorseBlocked(&play->colCtx, wall, bgId) != 0) {
            return;
        }

        if ((!movingFast && intersectDist < 80.0f) || (movingFast == true && intersectDist < 150.0f)) {
            if (!movingFast) {
                self->stateFlags |= ENHORSE_FORCE_REVERSING;
            } else if (movingFast == true) {
                self->stateFlags |= ENHORSE_FORCE_REVERSING;
                ((EnHorseStartBrakingFunc)EnHorse_StartBraking)(self, play);
            }
            return;
        }

        dynaPoly = DynaPoly_GetActor(&play->colCtx, bgId);
        if ((self->stateFlags & ENHORSE_FLAG_26) && ((dynaPoly && dynaPoly->id != 0x108) || dynaPoly == NULL)) {
            if (!movingFast) {
                self->stateFlags |= ENHORSE_FORCE_REVERSING;
            } else if (movingFast == true) {
                self->stateFlags |= ENHORSE_FORCE_REVERSING;
                ((EnHorseStartBrakingFunc)EnHorse_StartBraking)(self, play);
            }
            return;
        }
    }

    intersectDist += 5.0f;
    obstaclePos = startPos;
    obstaclePos.x += intersectDist * Math_SinS(self->actor.world.rot.y);
    obstaclePos.y = self->actor.world.pos.y + 120.0f;
    obstaclePos.z += intersectDist * Math_CosS(self->actor.world.rot.y);
    obstacleTop = obstaclePos;
    obstacleTop.y = BgCheck_EntityRaycastFloor3(&play->colCtx, &obstacleFloor, &bgId, &obstaclePos);
    if (obstacleTop.y == -32000.0f) {
        return;
    }
    obstacleHeight = obstacleTop.y - self->actor.world.pos.y;
    if (self->actor.floorPoly == NULL || obstacleFloor == NULL) {
        return;
    }

    if (Math3D_DistPlaneToPos(ENHORSE_COLPOLY_NORMAL(((EnHorseColPoly*)self->actor.floorPoly)->normal.x),
                              ENHORSE_COLPOLY_NORMAL(((EnHorseColPoly*)self->actor.floorPoly)->normal.y),
                              ENHORSE_COLPOLY_NORMAL(((EnHorseColPoly*)self->actor.floorPoly)->normal.z),
                              ((EnHorseColPoly*)self->actor.floorPoly)->dist, &obstacleTop) < -40.0f &&
        Math3D_DistPlaneToPos(ENHORSE_COLPOLY_NORMAL(obstacleFloor->normal.x), ENHORSE_COLPOLY_NORMAL(obstacleFloor->normal.y),
                              ENHORSE_COLPOLY_NORMAL(obstacleFloor->normal.z), obstacleFloor->dist,
                              &self->actor.world.pos) > 40.0f) {
        if (movingFast == true && self->action != ENHORSE_ACT_STOPPING) {
            self->stateFlags |= ENHORSE_FORCE_REVERSING;
            ((EnHorseStartBrakingFunc)EnHorse_StartBraking)(self, play);
        }
        self->stateFlags |= ENHORSE_OBSTACLE;
        return;
    }

    ny = ENHORSE_COLPOLY_NORMAL(obstacleFloor->normal.y);
    if (ny < 0.81915206f || (SurfaceType_IsHorseBlocked(&play->colCtx, obstacleFloor, bgId) != 0) ||
        (FUN_0035ea34(&play->colCtx, obstacleFloor, bgId) == 7)) {
        if (movingFast == true && self->action != ENHORSE_ACT_STOPPING) {
            self->stateFlags |= ENHORSE_FORCE_REVERSING;
            ((EnHorseStartBrakingFunc)EnHorse_StartBraking)(self, play);
        }
        return;
    }

    if (wall == NULL || obstacleTop.y < intersect.y || (self->stateFlags & ENHORSE_CANT_JUMP)) {
        return;
    }

    obstaclePos = startPos;
    obstaclePos.y = self->actor.world.pos.y + 120.0f;
    if (!movingFast) {
        obstaclePos.x += (276.0f * Math_SinS(self->actor.world.rot.y));
        obstaclePos.z += (276.0f * Math_CosS(self->actor.world.rot.y));
    } else {
        obstaclePos.x += (390.0f * Math_SinS(self->actor.world.rot.y));
        obstaclePos.z += (390.0f * Math_CosS(self->actor.world.rot.y));
    }

    obstacleTop = obstaclePos;
    obstacleTop.y = BgCheck_EntityRaycastFloor3(&play->colCtx, &obstacleFloor, &bgId, &obstaclePos);
    if (obstacleTop.y == -32000.0f) {
        return;
    }

    behindObstacleHeight = obstacleTop.y - self->actor.world.pos.y;

    if (obstacleFloor == NULL) {
        return;
    }

    ny = ENHORSE_COLPOLY_NORMAL(obstacleFloor->normal.y);
    if (ny < 0.81915206f || SurfaceType_IsHorseBlocked(&play->colCtx, obstacleFloor, bgId) ||
        FUN_0035ea34(&play->colCtx, obstacleFloor, bgId) == 7) {
        if (movingFast == true && self->action != ENHORSE_ACT_STOPPING) {
            self->stateFlags |= ENHORSE_FORCE_REVERSING;
            ((EnHorseStartBrakingFunc)EnHorse_StartBraking)(self, play);
        }
    } else if (behindObstacleHeight < -DREG(4)) {
        if (movingFast == true && self->action != ENHORSE_ACT_STOPPING) {
            self->stateFlags |= ENHORSE_FORCE_REVERSING;
            ((EnHorseStartBrakingFunc)EnHorse_StartBraking)(self, play);
        }
    } else if (!movingFast && obstacleHeight > 19.0f && obstacleHeight <= 40.0f) {
        self->postDrawFunc = (EnHorsePostdrawFunc)EnHorse_LowJumpInit;
    } else if ((movingFast == true && self->actor.speedXZ < 13.8f && obstacleHeight > 19.0f && obstacleHeight <= 72.0f) ||
               (self->actor.speedXZ > 13.8f && obstacleHeight <= 112.0f)) {
        self->postDrawFunc = (EnHorsePostdrawFunc)EnHorse_HighJumpInit;
    }
}
#endif
GLOBAL_ASM("asm/EnHorse_UpdateBgCheckInfo.s")

s32 FUN_001ff084(u32 sfxId) __attribute__((noinline));
void EnHorse_InitHorsebackArchery(EnHorse* self) __attribute__((noinline));

#define R_DREG8 DREG(8)

#if 0 // 18 words off: structurally identical (size equal), callee-saved register permutation only (play/hba base/sfx const/race info/fanfare/save base) (Sonnet)
void EnHorse_UpdateHorsebackArchery(EnHorse* self, PlayState* play2) {
    f32 playSpeed;
    s32 isFanfarePlaying;
    PlayState* play = play2;

    if (self->animationIdx == ENHORSE_ANIM_WALK) {
        EnHorse_PlayWalkingSound(self);
    }
    if (*(u16*)((u8*)play + 0x2E1C) == 0) {
        self->hbaTimer++;
    }

    isFanfarePlaying = FUN_001ff084(0x010005AE);
    {
        EnHorseRaceInfo* raceInfo;
        Vec3f pos;
        f32 px;
        f32 pz;
        f32 d;

        raceInfo = &EnHorse_sHbaInfo;
        EnHorse_RaceWaypointPos(raceInfo->waypoints, self->curRaceWaypoint, &pos);
        Math3D_RotateXZPlane(&pos, raceInfo->waypoints[self->curRaceWaypoint].angle, &px, &pz, &d);

        if (self->curRaceWaypoint >= raceInfo->numWaypoints - 1 && Math_Vec3f_DistXYZ(&pos, &self->actor.world.pos) < R_DREG8) {
            self->hbaFlags |= 2;
        }

        if (((self->actor.world.pos.x * px) + (pz * self->actor.world.pos.z) + d) > 0.0f) {
            self->curRaceWaypoint++;
            if (self->curRaceWaypoint >= raceInfo->numWaypoints) {
                self->hbaFlags |= 1;
                goto afterRace;
            }
        }

        if (!(self->hbaFlags & 1)) {
            EnHorse_RotateToPoint(self, &pos, 534);
        }

        self->actor.shape.rot.y = self->actor.world.rot.y;
        if (self->actor.speedXZ < raceInfo->waypoints[self->curRaceWaypoint].speedXZ && !(self->hbaFlags & 1)) {
            self->actor.speedXZ += 0.26666668f;
        } else {
            self->actor.speedXZ -= 0.26666668f;
            if (self->actor.speedXZ < 0.0f) {
                self->actor.speedXZ = 0.0f;
            }
        }
    }

afterRace:
    if ((self->hbaFlags & 1) || (self->hbaTimer > 68)) {
        if ((isFanfarePlaying != true) && (*(u16*)((u8*)&gSaveContext + 0x1594) != 3)) {
            gSaveContext.cutsceneIndex = 0;
            FUN_003716f0(play, 0x3B0, 0x14, 0x20);
        }
    }

    if (*(u16*)((u8*)play + 0x2E1C) != 0) {
        if (!(self->hbaFlags & 2)) {
            if (*(u16*)((u8*)&gSaveContext + 0xF42 - 0) & 1) {
                if (*(u16*)((u8*)&gSaveContext + 0x1596) >= 1500) {
                    self->hbaFlags |= 4;
                }
            } else {
                if (*(u16*)((u8*)&gSaveContext + 0x1596) >= 1000) {
                    self->hbaFlags |= 4;
                }
            }
        }
    }

    if ((*(u16*)((u8*)play + 0x2E1C) == 0) || (self->hbaFlags & 2)) {
        if (self->hbaFlags & 4) {
            self->hbaFlags &= ~4;
            Audio_QueueSeqCmd(0, 0x010005AE, 0);
        }
    }

    if (!self->hbaStarted) {
        self->actor.speedXZ = 0.0f;
        if (self->animationIdx != ENHORSE_ANIM_IDLE) {
            EnHorse_InitHorsebackArchery(self);
        }
    }

    if (self->animationIdx == ENHORSE_ANIM_WALK) {
        playSpeed = self->actor.speedXZ * 0.75f;
    } else if (self->animationIdx == ENHORSE_ANIM_TROT) {
        playSpeed = self->actor.speedXZ * 0.375f;
    } else if (self->animationIdx == ENHORSE_ANIM_GALLOP || self->animationIdx == 9) {
        playSpeed = self->actor.speedXZ * 0.3f;
    } else {
        playSpeed = 1.0f;
    }

    SkelAnime_SetPlaySpeed(&self->skin, playSpeed);
    if (FUN_003731e0(&self->skin) || (self->animationIdx == ENHORSE_ANIM_IDLE && self->actor.speedXZ != 0.0f)) {
        EnHorse_InitHorsebackArchery(self);
    }
}
#endif
GLOBAL_ASM("asm/EnHorse_UpdateHorsebackArchery.s")

void EnHorse_UpdateIngoHorseAnim(EnHorse* self) {
    s32 animChanged = 0;
    f32 animSpeed;
    f32 zero = 0.0f;
    f32 scale = 1.5f;

    self->action = ENHORSE_ACT_INGO_RACE;
    self->stateFlags &= ~ENHORSE_SANDDUST_SOUND;
    if (self->actor.speedXZ == zero) {
        if (self->animationIdx != ENHORSE_ANIM_IDLE) {
            animChanged = true;
        }
        self->animationIdx = ENHORSE_ANIM_IDLE;
    } else if (self->actor.speedXZ <= 3.0f) {
        if (self->animationIdx != ENHORSE_ANIM_WALK) {
            animChanged = true;
        }
        self->animationIdx = ENHORSE_ANIM_WALK;
    } else if (self->actor.speedXZ <= 6.0f) {
        if (self->animationIdx != ENHORSE_ANIM_TROT) {
            animChanged = true;
        }
        self->animationIdx = ENHORSE_ANIM_TROT;
    } else {
        if (self->animationIdx != ENHORSE_ANIM_GALLOP) {
            animChanged = true;
        }
        self->animationIdx = ENHORSE_ANIM_GALLOP;
    }

    if (self->animationIdx == ENHORSE_ANIM_WALK) {
        animSpeed = self->actor.speedXZ * 0.5f;
    } else if (self->animationIdx == ENHORSE_ANIM_TROT) {
        animSpeed = self->actor.speedXZ * 0.25f;
        Audio_PlaySoundGeneral(0x0100015B, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
    } else if (self->animationIdx == ENHORSE_ANIM_GALLOP) {
        animSpeed = self->actor.speedXZ * 0.3f;
        Audio_PlaySoundGeneral(0x0100015B, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
    } else {
        animSpeed = 1.0f;
    }

    f32* speeds = EnHorse_sPlaybackSpeeds;
    if (animChanged == true) {
        Animation_Change_00375c08(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx],
                                  speeds[self->animationIdx] * (animSpeed * scale), 0,
                                  Animation_GetLastFrame(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx]), 2,
                                  -3.0f);
    } else {
        Animation_Change_00375c08(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx],
                                  speeds[self->animationIdx] * (animSpeed * scale), 0,
                                  Animation_GetLastFrame(&self->skin, En_Horse_sAnimationHeaders[self->type][self->animationIdx]), 2, 0);
    }
}

static inline void EnHorse_SetIngoAnimation(s32 index, f32 curFrame, s32 isLashing, s16* animIdxOut, f32* curFrameOut) {
    *animIdxOut = EnHorse_sIngoAnimations[index];
    *curFrameOut = curFrame;
    if ((index == 3) || (index == 12) || (index == 13) || (index == 4)) {
        *curFrameOut = 0.0f;
    }
    if (isLashing == 1) {
        if (index == 5) {
            *animIdxOut = 4;
            *curFrameOut = curFrame;
        } else if (index == 7 || index == 9) {
            *animIdxOut = 3;
            *curFrameOut = curFrame;
        }
    }
}

void EnHorse_UpdateIngoRace(EnHorse* self, PlayState* play) {
    f32 playSpeed;
    EnHorseRaceInfo* raceInfo;
    Vec3f curWaypointPos;
    Vec3f prevWaypointPos;
    s32 prevWaypoint;
    f32 sp50;
    s16 relPlayerYaw;
    f32 px;
    f32 pz;
    f32 d;
    f32 distSq;

    if (self->animationIdx == ENHORSE_ANIM_IDLE || self->animationIdx == ENHORSE_ANIM_WHINNY) {
        EnHorse_IdleAnimSounds(self, play);
    } else if (self->animationIdx == ENHORSE_ANIM_WALK) {
        EnHorse_PlayWalkingSound(self);
    }

    raceInfo = &EnHorse_sIngoRace;
    EnHorse_RaceWaypointPos(raceInfo->waypoints, self->curRaceWaypoint, &curWaypointPos);
    Math3D_RotateXZPlane(&curWaypointPos, raceInfo->waypoints[self->curRaceWaypoint].angle, &px, &pz, &d);
    if (((self->actor.world.pos.x * px) + (pz * self->actor.world.pos.z) + d) > 0.0f) {
        self->curRaceWaypoint++;
        if (self->curRaceWaypoint >= raceInfo->numWaypoints) {
            self->curRaceWaypoint = 0;
        }
    }

    EnHorse_RaceWaypointPos(raceInfo->waypoints, self->curRaceWaypoint, &curWaypointPos);

    prevWaypoint = self->curRaceWaypoint - 1;
    if (prevWaypoint < 0) {
        prevWaypoint = raceInfo->numWaypoints - 1;
    }
    EnHorse_RaceWaypointPos(raceInfo->waypoints, prevWaypoint, &prevWaypointPos);
    Math3D_PointDistToLine2D(self->actor.world.pos.x, self->actor.world.pos.z, prevWaypointPos.x, prevWaypointPos.z,
                             curWaypointPos.x, curWaypointPos.z, &distSq);
    EnHorse_RotateToPoint(self, &curWaypointPos, 267);

    if (distSq < SQ(300.0f)) {
        if (self->actor.xzDistToPlayer < 130.0f || (*(u8*)((u8*)self->colliderJntSph.elements + 0x17) & 2)) {
            if (Math_SinS(self->actor.yawTowardsPlayer - self->actor.world.rot.y) > 0.0f) {
                self->actor.world.rot.y -= 186;
            } else {
                self->actor.world.rot.y += 186;
            }
        } else if (self->actor.xzDistToPlayer < 300.0f) {
            if (Math_SinS(self->actor.yawTowardsPlayer - self->actor.world.rot.y) > 0.0f) {
                self->actor.world.rot.y += 186;
            } else {
                self->actor.world.rot.y -= 186;
            }
        }
        self->actor.shape.rot.y = self->actor.world.rot.y;
    }

    sp50 = Actor_WorldDistXZToActor(&self->actor, &GET_PLAYER(play)->actor);
    relPlayerYaw = Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(play)->actor) - self->actor.world.rot.y;
    if (sp50 <= 200.0f || (EnHorse_Fabsf(Math_SinS(relPlayerYaw)) < 0.8f && Math_CosS(relPlayerYaw) > 0.0f)) {
        if (self->actor.speedXZ < self->ingoHorseMaxSpeed) {
            self->actor.speedXZ += 0.31333333f;
        } else {
            self->actor.speedXZ -= 0.31333333f;
        }
        self->ingoRaceFlags |= 1;
    } else {
        if (self->actor.speedXZ < raceInfo->waypoints[self->curRaceWaypoint].speedXZ) {
            self->actor.speedXZ += 0.26666668f;
        } else {
            self->actor.speedXZ -= 0.26666668f;
        }
        self->ingoRaceFlags &= ~1;
    }

    if (!self->inRace) {
        self->actor.speedXZ = 0.0f;
        self->rider->speedXZ = 0.0f;
        if (self->animationIdx != ENHORSE_ANIM_IDLE) {
            EnHorse_UpdateIngoHorseAnim(self);
        }
    }

    if (self->animationIdx == ENHORSE_ANIM_WALK) {
        playSpeed = self->actor.speedXZ * 0.75f;
    } else if (self->animationIdx == ENHORSE_ANIM_TROT) {
        playSpeed = self->actor.speedXZ * 0.375f;
    } else if (self->animationIdx == ENHORSE_ANIM_GALLOP || self->animationIdx == 9) {
        playSpeed = self->actor.speedXZ * 0.3f;
    } else {
        playSpeed = 1.0f;
    }
    SkelAnime_SetPlaySpeed(&self->skin, playSpeed);
    if (FUN_003731e0(&self->skin) || (self->animationIdx == ENHORSE_ANIM_IDLE && self->actor.speedXZ != 0.0f)) {
        EnHorse_UpdateIngoHorseAnim(self);
    }

    if (self->stateFlags & ENHORSE_INGO_WON) {
        *(s16*)((u8*)self->rider + 0x45E) = 7;
        *(f32*)((u8*)self->rider + 0x458) = 0.0f;
        return;
    }

    EnHorse_SetIngoAnimation(self->animationIdx, *EnHorse_CurFramePtr(&self->skin), self->ingoRaceFlags & 1,
                             (s16*)((u8*)self->rider + 0x45E), (f32*)((u8*)self->rider + 0x458));
}

static inline f32 EnHorse_SlopeSpeedMultiplier(EnHorse* self) {
    f32 multiplier = 1.0f;

    if (Math_CosS(self->actor.shape.rot.x) < 0.939262f && Math_SinS(self->actor.shape.rot.x) < 0.0f) {
        multiplier = 0.7f;
    }
    return multiplier;
}

#if 0 // 248 words off, +84 B: float register allocation/scheduling differs from the first instruction (Sonnet)
void EnHorse_UpdateSpeed(EnHorse* self, PlayState* play, f32 brakeDecel, f32 brakeAngle, f32 minStickMag, f32 decel,
                         f32 baseSpeed, s16 turnSpeed) {
    s16* stickAnglePtr;
    f32 stickMag;
    s16 stickAngle;
    f32 temp_f12;
    f32 traction;
    s16 turn;

    if (!EnHorse_PlayerCanMove(self, play)) {
        if (self->actor.speedXZ > 8) {
            self->actor.speedXZ -= decel;
        } else if (self->actor.speedXZ < 0) {
            self->actor.speedXZ = 0;
        }

        return;
    }

    stickAnglePtr = &stickAngle;

    baseSpeed *= EnHorse_SlopeSpeedMultiplier(self);
    EnHorse_StickDirection(&self->curStick, &stickMag, &stickAngle);
    if (Math_CosS(stickAngle) <= brakeAngle) {
        self->actor.speedXZ -= brakeDecel;
        self->actor.speedXZ = (self->actor.speedXZ > 0.0f) ? self->actor.speedXZ : 0.0f;
        return;
    }

    if (stickMag < minStickMag) {
        self->stateFlags &= ~(ENHORSE_BOOST | ENHORSE_BOOST_DECEL);
        self->actor.speedXZ -= decel;
        if (self->actor.speedXZ < 1.5f) {
            self->actor.speedXZ = 0.0f;
        }

        return;
    }

    if (self->stateFlags & ENHORSE_BOOST) {
        if (((s32)(48.0f / R_UPDATE_RATE + 0.5f) - self->boostTimer) > 0) {
            self->actor.speedXZ = (EnHorse_SlopeSpeedMultiplier(self) * self->boostSpeed - self->actor.speedXZ) /
                                      ((s32)(48.0f / R_UPDATE_RATE + 0.5f) - self->boostTimer) +
                                  self->actor.speedXZ;
        } else {
            self->actor.speedXZ = EnHorse_SlopeSpeedMultiplier(self) * self->boostSpeed;
        }

        if ((EnHorse_SlopeSpeedMultiplier(self) * self->boostSpeed) <= self->actor.speedXZ) {
            self->stateFlags &= ~ENHORSE_BOOST;
            self->stateFlags |= ENHORSE_BOOST_DECEL;
        }

    } else if (self->stateFlags & ENHORSE_BOOST_DECEL) {
        if (baseSpeed < self->actor.speedXZ) {
            temp_f12 = self->actor.speedXZ;
            self->actor.speedXZ = temp_f12 - 0.04f;
        } else if (self->actor.speedXZ < baseSpeed) {
            self->actor.speedXZ = baseSpeed;
            self->stateFlags &= ~ENHORSE_BOOST_DECEL;
        }
    } else {
        self->actor.speedXZ += (self->actor.speedXZ <= baseSpeed * (1.0f / 54.0f) * stickMag ? 1.0f : -1.0f) * 50.0f * 0.01f;
        if (self->actor.speedXZ <= 1.5f) {
            self->actor.speedXZ = 0.0f;
        }
        if (baseSpeed < self->actor.speedXZ) {
            self->actor.speedXZ -= decel;
            if (self->actor.speedXZ < baseSpeed) {
                self->actor.speedXZ = baseSpeed;
            }
        }
    }

    temp_f12 = *stickAnglePtr * (1 / 32236.f);
    traction = 2.2f - (self->actor.speedXZ * (1.0f / self->boostSpeed));
    turn = *stickAnglePtr * temp_f12 * temp_f12 * traction;
    turn = ENHORSE_CLAMP(turn, -turnSpeed * traction, turnSpeed * traction);
    self->actor.world.rot.y += turn;
    self->actor.shape.rot.y = self->actor.world.rot.y;
}
#endif
GLOBAL_ASM("asm/EnHorse_UpdateSpeed.s")

extern u8 DAT_0054ad68[];
s32 FUN_00366684(s32 flag);

static inline u8 EnHorse_SfxFlags(u32 sfxId) {
    u32 idx = sfxId - 0x01000585;
    return DAT_0054ad68[idx > 0x54 ? 0 : idx];
}

s32 FUN_001ff084(u32 sfxId) {
    s32 flag = 0;

    if ((EnHorse_SfxFlags(sfxId) & 2) || (EnHorse_SfxFlags(sfxId) & 4)) {
        flag = 1;
    }
    return FUN_00366684(flag) == sfxId;
}


void FUN_0033258c(Vec3f* src, Vec3f* dst, f32 amount) {
    f32 r = Rand_ZeroOne();

    dst->x = src->x + (amount * 2.0f) * r - amount;
    r = Rand_ZeroOne();
    dst->y = src->y + (amount * 2.0f) * r - amount;
    r = Rand_ZeroOne();
    dst->z = src->z + (amount * 2.0f) * r - amount;
}

extern Color_RGBA8 DAT_00520238;
extern Color_RGBA8 DAT_00520234;
void FUN_003356d4(PlayState* play, s32 type, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor,
                  Color_RGBA8* envColor, s16 scale, s16 scaleStep, s16 life, s32 unk);

// N64 func_80028A54 (EffectSsDust, 20 particles)
#if 0 // 10 words off: ROM derives prim as env-4 (sub r2,r3,#4), r5/r6 swap (Sonnet). Opus: prim/env as one array/struct symbol -> armcc hoists the base (69 words); ROM shape = colors were section-anchored statics in this TU (needs real data symbols)
void FUN_0037308c(PlayState* play, f32 randScale, Vec3f* srcPos) {
    s32 i;
    Vec3f pos;
    Vec3f velocity;
    Vec3f accel;

    for (i = 0; i < 20; i++) {
        s16 randAngle;
        f32 rand = Rand_ZeroOne() * randScale;

        randAngle = (Rand_ZeroOne() * 65536.0f);
        pos = *srcPos;
        pos.x += Math_SinS(randAngle) * rand;
        pos.z += Math_CosS(randAngle) * rand;
        velocity.y = 1.0f;
        velocity.x = Math_SinS(randAngle);
        velocity.z = Math_CosS(randAngle);
        accel.x = accel.y = accel.z = 0.0f;
        FUN_003356d4(play, 5, &pos, &velocity, &accel, &DAT_00520234, &DAT_00520238, 100, 30,
                     (s16)(21.0f / R_UPDATE_RATE + 0.5f), 0);
    }
}
#endif
GLOBAL_ASM("asm/FUN_0037308c.s")

GLOBAL_ASM("asm/FUN_00408828.s")

}
