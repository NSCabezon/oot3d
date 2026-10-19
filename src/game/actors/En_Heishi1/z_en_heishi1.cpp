extern "C" {
#include "z_en_heishi1.hpp"

#define FLAGS 0x00000010

void EnHeishi1_Init(Actor* actor, GameState* state);
void EnHeishi1_Destroy(Actor* actor, GameState* state);
void EnHeishi1_Update(Actor* actor, GameState* state);
void EnHeishi1_Draw(Actor* actor, GameState* state);

s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void EnHeishi1_Kick(EnHeishi1* self, PlayState* play);
void EnHeishi1_WaitNight(EnHeishi1* self, PlayState* play);
void FUN_0034e01c(MTX34* mtx, Vec3s* rot);
void GraphicsContext_UnknownStub(GraphicsContext* gfxCtx);
void FUN_00357878(Actor* actor, Vec3f* pos, Vec3f* scale, s32 alpha, PlayState* play);
void EnHeishi1_Walk(EnHeishi1* self, PlayState* play);
void EnHeishi1_Wait(EnHeishi1* self, PlayState* play);
void EnHeishi1_SetupWalk(EnHeishi1* self, PlayState* play);
void EnHeishi1_SetupKick(EnHeishi1* self, PlayState* play);
void FUN_001073f8(EnHeishi1* self, PlayState* play);
void FUN_003da4f4(EnHeishi1* self, PlayState* play);
void FUN_003725e0(PlayState* play);
void FUN_0034c294(PlayState* play, s32 action);
void EffectSs_Spawn(PlayState* play, s32 type, s32 priority, void* initParams);

ActorInit En_Heishi1_InitVars = {
    ACTOR_PLAYER,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_SD,
    sizeof(EnHeishi1),
    (ActorFunc)EnHeishi1_Init,
    (ActorFunc)EnHeishi1_Destroy,
    (ActorFunc)EnHeishi1_Update,
    (ActorFunc)EnHeishi1_Draw,
};

static f32 sAnimParamsInit[][8] = {
    { 1.0f, -10.0f, 3.0f, 0.5f, 1000.0f, 200.0f, 0.3f, 1000.0f },
    { 3.0f, -3.0f, 6.0f, 0.8f, 2000.0f, 400.0f, 0.5f, 2000.0f },
    { 1.0f, -10.0f, 3.0f, 0.5f, 1000.0f, 200.0f, 0.3f, 1000.0f },
    { 3.0f, -3.0f, 6.0f, 0.8f, 2000.0f, 400.0f, 0.5f, 2000.0f },
};

static s16 sBaseHeadTimers[] = { 20, 10, 20, 10, 13, 0 };

static Vec3f sRupeePositions[] = {
    { 0.0f, 0.0f, 90.0f },  { -55.0f, 0.0f, 90.0f }, { -55.0f, 0.0f, 30.0f }, { -55.0f, 0.0f, -30.0f },
    { 0.0f, 0.0f, -30.0f }, { 55.0f, 0.0f, -30.0f }, { 55.0f, 0.0f, 30.0f },  { 55.0f, 0.0f, 90.0f },
};

__attribute__((used)) static s32 sBgCamIndices[] = {
    7, 7, 2, 2, 2, 2, 3, 3, 4, 4, 5, 6, 4, 4, 5, 6,
};

__attribute__((used)) static s16 sWaypoints[] = { 0, 4, 1, 5, 2, 6, 3, 7 };

void EnHeishi1_SetupWalk(EnHeishi1* self, PlayState* play);
void EnHeishi1_SetupWaitNight(EnHeishi1* self, PlayState* play);

// Member-wise Vec3f copy constructor: ROM copies each rupee position field by field (vldr/vstr) into a temporary,
// then struct-copies it into the local.
class EnHeishi1Vec3 {
public:
    EnHeishi1Vec3(const Vec3f& v) : x(v.x), y(v.y), z(v.z) {}
    f32 x;
    f32 y;
    f32 z;
};

void EnHeishi1_Init(Actor* thisx, GameState* state) {
    EnHeishi1* self = (EnHeishi1*)thisx;
    PlayState* play = (PlayState*)state;
    Vec3f rupeePos;
    s32 i;

    Actor_SetScale(&self->actor, 0.01f);
    FUN_00372f38(thisx, play, &self->skelAnimModel, 0, 0);
    FUN_00353c9c(thisx, play, &self->skelAnime, 0, 0, (u8*)self + 0x228, (u8*)self + 0x568, 16);
    self->unk_8FC = 1;
    self->type = (self->actor.params >> 8) & 0xFF;
    self->path = self->actor.params & 0xFF;

    for (i = 0; i < 8; i++) {
        (&self->animSpeed)[i] = sAnimParamsInit[self->type][i];
    }

    if (self->path == 3) {
        for (i = 0; i < 8; i++) {
            rupeePos = *(Vec3f*)&EnHeishi1Vec3(sRupeePositions[i]);
            Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, ACTOR_EN_EX_RUPPY, rupeePos.x, rupeePos.y,
                               rupeePos.z, 0, 0, 0, 3);
        }
    }

    if (self->type != 5) {
        if (((gSaveContext.dayTime < 0xB888) || IS_DAY) && !(gSaveContext.eventChkInf[8] & 1)) {
            self->actionFunc = EnHeishi1_SetupWalk;
        } else {
            Actor_Kill(&self->actor);
        }
    } else {
        if ((gSaveContext.dayTime > 0xB888) || !IS_DAY || (gSaveContext.eventChkInf[8] & 1)) {
            self->actionFunc = EnHeishi1_SetupWaitNight;
        } else {
            Actor_Kill(&self->actor);
        }
    }
}

void EnHeishi1_Destroy(Actor* thisx, GameState* state) {
    EnHeishi1* self = (EnHeishi1*)thisx;

    FUN_003508b8(thisx, self->skelAnimModel, 0);
    SkelAnime_Free2(&self->skelAnime);
}

extern __pure float _fabsf(float);
static __inline __pure float EnHeishi1_Fabsf(float f) {
    return _fabsf(f);
}

// EnHeishi1_Update draft: ~47 instruction-diff (structure matches). ROM keeps constants 0 and 1 in callee-saved
// sb/sl (8-reg push, frame 0x68, mtx at sp+8; ours push 7 regs + 4-byte pad, mtx at sp+0xc), the timers loop uses
// r0 as i (ours hoists mov r2,#0) and accel/mult zero stores use s1/s2/s3 copies. Tried: local true/false vars,
// setter helper, decl order, accel/mult swap, raw Vec3f vs Vector3f, i scoping. FUN_003902e0 lives in
// z_en_heishi1_effect.cpp (called out of line).
#if 0
f32 __hardfp_sinf(f32);
f32 __hardfp_cosf(f32);
void FUN_003902e0(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 arg4, s32 arg5);
void FUN_001755e4(EnHeishi1* self, PlayState* play);

static inline void EnHeishi1_RotateYNew(MTX34* m, f32 angle) {
    f32 sn;
    f32 cs;

    if (angle == 0.0f) {
        sn = 0.0f;
        cs = 1.0f;
    } else {
        sn = __hardfp_sinf(angle);
        cs = __hardfp_cosf(angle);
    }
    m->data[0][0] = cs;
    m->data[0][1] = 0.0f;
    m->data[0][2] = sn;
    m->data[0][3] = 0.0f;
    m->data[1][1] = 1.0f;
    m->data[1][0] = 0.0f;
    m->data[1][2] = 0.0f;
    m->data[1][3] = 0.0f;
    m->data[2][0] = -sn;
    m->data[2][1] = 0.0f;
    m->data[2][2] = cs;
    m->data[2][3] = 0.0f;
}

void EnHeishi1_Update(Actor* thisx, GameState* state) {
    EnHeishi1* self = (EnHeishi1*)thisx;
    PlayState* play = (PlayState*)state;
    Player* player = GET_PLAYER(play);
    u8 i;
    s16 path;
    Camera* activeCam;

    self->activeTimer++;

    for (i = 0; i < 3; i++) {
        DECR((&self->headTimer)[i]);
    }

    DECR(self->waypointTimer);

    activeCam = play->cameraPtrs[play->activeCamera];

    if (player->actor.freezeTimer == 0) {
        self->actionFunc(self, play);

        self->actor.uncullZoneForward = 800.0f;
        self->actor.uncullZoneScale = 350.0f;
        self->actor.uncullZoneDownward = 700.0f;

        if (self->type != 5) {
            path = self->path * 2;
            if ((*(s16*)((u8*)activeCam + 0x190) == sBgCamIndices[path]) ||
                (*(s16*)((u8*)activeCam + 0x190) == sBgCamIndices[path + 1])) {
                if (*(s32*)((u8*)play + 0x7FC0) == 0) {
                    if ((self->actionFunc == EnHeishi1_Walk) || (self->actionFunc == EnHeishi1_Wait)) {
                        Vec3f searchBallVel;
                        Vector3f searchBallAccel(0.0f, 0.0f, 0.0f);
                        Vector3f searchBallMult(0.0f, 0.0f, 20.0f);
                        Vec3f searchBallPos;
                        MTX34 mtx;

                        searchBallPos.x = self->actor.world.pos.x;
                        searchBallPos.y = self->actor.world.pos.y + 60.0f;
                        searchBallPos.z = self->actor.world.pos.z;

                        MTX34CopyAsm(&mtx, &self->actor.modelMtx);
                        EnHeishi1_RotateYNew(&mtx, ((f32)self->actor.shape.rot.y + self->headAngle) * 3.0517578e-05f *
                                                       3.1415927f);
                        searchBallMult.z = 30.0f;
                        VEC3TransformAsm((Vector3f*)&searchBallVel, &mtx, &searchBallMult);

                        FUN_003902e0(play, &searchBallPos, &searchBallVel, (Vec3f*)&searchBallAccel, 2,
                                     (s32)&self->linkDetected);

                        if (self->actor.xzDistToPlayer < 60.0f) {
                            self->linkDetected = true;
                        } else if (self->actor.xzDistToPlayer < 70.0f) {
                            if (player->actor.velocity.y > -4.0f) {
                                self->linkDetected = true;
                            }
                        }

                        if (self->linkDetected) {
                            if (!(player->actor.velocity.y > -3.9f)) {
                                self->linkDetected = false;
                                if (EnHeishi1_Fabsf(player->actor.world.pos.y - self->actor.world.pos.y) < 60.0f) {
                                    Audio_PlaySoundGeneral(0x010004B1, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                                                           (s8*)&DAT_0054ac24);
                                    FUN_0036e980(play, &self->actor, 1);
                                    *(s32*)((u8*)play + 0x7FC0) = 1;
                                    self->actionFunc = FUN_001755e4;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
#endif

GLOBAL_ASM("asm/EnHeishi1_Update.s")

s32 FUN_00210cd4(s32 unused, s32 which, MTX34* mtx, EnHeishi1* self) {
    if (which == 0xf) {
        Vec3s rot;

        rot.x = 0;
        rot.y = (s16)self->headAngle;
        rot.z = 0;
        FUN_0034e01c(mtx, &rot);
    }
    return 0;
}

void EnHeishi1_Draw(Actor* thisx, GameState* state) {
    EnHeishi1* self = (EnHeishi1*)thisx;
    PlayState* play = (PlayState*)state;
    Vector3f matrixScale(0.3f, 0.3f, 0.3f);

    GraphicsContext_UnknownStub(play->state.gfxCtx);
    SkelAnime_DrawOpa(&self->skelAnime, &thisx->modelMtx, (OverrideLimbDrawOpa)FUN_00210cd4, NULL, thisx, 0);
    FUN_00357878(thisx, &thisx->world.pos, (Vec3f*)&matrixScale, 0xFF, play);
}

void EnHeishi1_Kick(EnHeishi1* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if (!self->loadStarted) {
        // if dialog state is 5 and textbox has been advanced, kick player out
        if ((Message_GetState(&play->msgCtx) == 5) && Message_ShouldAdvance(play)) {
            FUN_003725e0(play);
            if (!self->loadStarted) {
                gSaveContext.eventChkInf[4] |= 0x4000;
                FUN_003716f0(play, 0x4FA, 0x14, 0x2E);
                self->loadStarted = true;
                *(s32*)((u8*)play + 0x7FC0) = 0;
                *(u8*)((u8*)&gSaveContext + 0x15AB) = 0x2E;
            }
        }
    }
}

void EnHeishi1_SetupKick(EnHeishi1* self, PlayState* play) {
    f32 frames = Animation_GetLastFrame(&self->skelAnime, 0);

    Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, frames, 0, -10.0f);
    self->actionFunc = EnHeishi1_Kick;
}

void EnHeishi1_SetupWait(EnHeishi1* self, PlayState* play) {
    s32 frames = Animation_GetLastFrame(&self->skelAnime, 0);

    Animation_Change_00375c08(&self->skelAnime, 0, self->animSpeed, 0.0f, frames, 0, self->animMorphFrames);
    self->headBehaviorDecided = false;
    self->headDirection = Rand_ZeroFloat(1.99f);
    self->waitTimer = ((s16)Rand_ZeroFloat(50.0f) + 50 > 0)
                          ? (s32)(0.5f + (((s16)Rand_ZeroFloat(50.0f) + 50) * 3.0f) * 0.5f)
                          : (s32)((((s16)Rand_ZeroFloat(50.0f) + 50) * 3.0f) * 0.5f - 0.5f);
    self->actionFunc = EnHeishi1_Wait;
}

void EnHeishi1_SetupWaitNight(EnHeishi1* self, PlayState* play) {
    f32 frames = Animation_GetLastFrame(&self->skelAnime, 0);

    Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, frames, 0, -10.0f);
    self->actionFunc = EnHeishi1_WaitNight;
}

void EnHeishi1_SetupWalk(EnHeishi1* self, PlayState* play) {
    s32 frames = Animation_GetLastFrame(&self->skelAnime, 1);

    Animation_Change_00375c08(&self->skelAnime, 1, self->animSpeed, 0.0f, frames, 0, self->animMorphFrames);
    self->bodyTurnSpeed = 0.0f;
    self->moveSpeed = 0.0f;
    self->headDirection = Rand_ZeroFloat(1.99f);
    self->actionFunc = EnHeishi1_Walk;
}

void FUN_00198104(EnHeishi1* self, PlayState* play) {
    f32 frames = Animation_GetLastFrame(&self->skelAnime, 0);

    Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, frames, 0, -10.0f);
    self->kickTimer = 45;
    self->actionFunc = FUN_001073f8;
}

void FUN_001073f8(EnHeishi1* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if (self->type != 5) {
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 3, self->bodyTurnSpeed, 0);
        Math_ApproachF(&self->bodyTurnSpeed, 3000.0f, 1.0f, 300.0f);
        Math_ApproachZeroF(&self->headAngle, 0.5f, 2000.0f);
    }
    if (self->kickTimer == 0) {
        self->actionFunc = EnHeishi1_SetupKick;
    }
}

void FUN_001755e4(EnHeishi1* self, PlayState* play) {
    f32 frames = Animation_GetLastFrame(&self->skelAnime, 1);

    Animation_Change_00375c08(&self->skelAnime, 1, 3.0f, 0.0f, frames, 0, -3.0f);
    self->bodyTurnSpeed = 0.0f;
    self->moveSpeed = 0.0f;
    FUN_00367c7c(play, 0x702D, &self->actor);
    FUN_0034c294(play, 0x12);
    self->actionFunc = FUN_003da4f4;
}

void FUN_003da4f4(EnHeishi1* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    FUN_003731e0(&self->skelAnime);
    Math_ApproachF(&self->actor.world.pos.x, player->actor.world.pos.x, 1.0f, self->moveSpeed);
    Math_ApproachF(&self->actor.world.pos.z, player->actor.world.pos.z, 1.0f, self->moveSpeed);
    Math_ApproachF(&self->moveSpeed, 6.0f, 1.0f, 0.4f);
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 3, self->bodyTurnSpeed, 0);
    Math_ApproachF(&self->bodyTurnSpeed, 3000.0f, 1.0f, 300.0f);
    Math_ApproachZeroF(&self->headAngle, 0.5f, 2000.0f);
    if (self->actor.xzDistToPlayer < 70.0f) {
        self->actionFunc = FUN_00198104;
    }
}

void EnHeishi1_WaitNight(EnHeishi1* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if (self->actor.xzDistToPlayer < 100.0f) {
        FUN_00367c7c(play, 0x702D, &self->actor);
        Audio_PlaySoundGeneral(0x010004B1, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, (s8*)&DAT_0054ac24);
        FUN_0036e980(play, &self->actor, 1);
        self->actionFunc = EnHeishi1_SetupKick;
    }
}

void EnHeishi1_Walk(EnHeishi1* self, PlayState* play) {
    Path* path;
    Vec3s* pointPos;
    f32 pathDiffX;
    f32 pathDiffZ;

    FUN_003731e0(&self->skelAnime);
    {
        s32 a = Animation_OnFrameImpl(&self->skelAnime, 2.0f, 1.0f);
        s32 b = Animation_OnFrameImpl(&self->skelAnime, 19.0f, 1.0f);

        if (a + b) {
            if (self->unk_8FC == 0) {
                if (b) {
                    self->unk_8FC = 1;
                    Audio_PlayActorSound2(&self->actor, 0x01000247);
                }
            } else if (a) {
                self->unk_8FC = 0;
                Audio_PlayActorSound2(&self->actor, 0x01000247);
            }
        }
    }

    if (*(s32*)((u8*)play + 0x7FC0) == 0) {
        path = &play->setupPathList[self->path];
        pointPos = path->points;
        pointPos += self->waypoint;

        Math_ApproachF(&self->actor.world.pos.x, pointPos->x, 1.0f, self->moveSpeed);
        Math_ApproachF(&self->actor.world.pos.z, pointPos->z, 1.0f, self->moveSpeed);

        Math_ApproachF(&self->moveSpeed, self->moveSpeedTarget, 1.0f, self->moveSpeedMax);

        pathDiffX = pointPos->x - self->actor.world.pos.x;
        pathDiffZ = pointPos->z - self->actor.world.pos.z;
        Math_SmoothStepToS(&self->actor.shape.rot.y, (s16)(Math_FAtan2F(pathDiffX, pathDiffZ) * 10430.378f), 3,
                           self->bodyTurnSpeed, 0);

        Math_ApproachF(&self->bodyTurnSpeed, self->bodyTurnSpeedTarget, 1.0f, self->bodyTurnSpeedMax);

        if (self->headTimer == 0) {
            self->headAngleTarget = 8192.0f;
            self->headDirection++;
            if ((self->headDirection & 1) != 0) {
                self->headAngleTarget *= -1;
            }
            self->headTimer = ((sBaseHeadTimers[self->type] + (s16)Rand_ZeroFloat(30.0f)) > 0)
                                  ? (s32)(0.5f + ((sBaseHeadTimers[self->type] + (s16)Rand_ZeroFloat(30.0f)) * 3.0f) * 0.5f)
                                  : (s32)(((sBaseHeadTimers[self->type] + (s16)Rand_ZeroFloat(30.0f)) * 3.0f) * 0.5f - 0.5f);
        }

        Math_ApproachF(&self->headAngle, self->headAngleTarget, self->headTurnSpeedScale, self->headTurnSpeedMax);

        if ((EnHeishi1_Fabsf(pathDiffX) < 20.0f) && (EnHeishi1_Fabsf(pathDiffZ) < 20.0f)) {
            if (self->waypointTimer == 0) {
                if (self->type >= 2) {
                    if ((self->waypoint > 3) && (Rand_ZeroFloat(1.99f) > 1.0f)) {
                        if (self->waypoint == 7) {
                            self->waypoint = 0;
                        } else if (self->waypoint >= 4) {
                            self->waypoint -= 3;
                        }
                        self->waypointTimer = 8;
                        return;
                    }
                }
                self->actionFunc = EnHeishi1_SetupWait;
            }
        }
    }
}

// EnHeishi1_Wait draft: 5 words off (near). Waypoint-search loop: ROM hoists `ldr r1,=sWaypoints; ldrsh r2,[waypoint]`
// (table r1 / waypoint r2); ours swaps them (table r2 / waypoint r1) -> 5 words (0x168..0x190). ~25 variants tried
// (wp local / s32 wp / table ptr / while / do-while / loop var scope / dead Self alias / operand order).
void EnHeishi1_Wait(EnHeishi1* self, PlayState* play) {
    s32 i;

    FUN_003731e0(&self->skelAnime);
    if (*(s32*)((u8*)play + 0x7FC0) == 0) {
        switch (self->headBehaviorDecided) {
            case false:
                self->headDirection++;
                self->headAngleTarget = (self->headDirection & 1) ? 9472.0f : -9472.0f;
                self->headTimer = ((sBaseHeadTimers[self->type] + (s16)Rand_ZeroFloat(30.0f)) > 0)
                                      ? (s32)(0.5f + ((sBaseHeadTimers[self->type] + (s16)Rand_ZeroFloat(30.0f)) * 3.0f) * 0.5f)
                                      : (s32)(((sBaseHeadTimers[self->type] + (s16)Rand_ZeroFloat(30.0f)) * 3.0f) * 0.5f - 0.5f);
                self->headBehaviorDecided = true;
                break;
            case true:
                if (self->headTimer == 0) {
                    if (self->waitTimer == 0) {
                        if ((self->type == 0) || (self->type == 1)) {
                            EnHeishi1* s2 = self;
                            s2->waypoint++;
                            if (self->waypoint > 3) {
                                self->waypoint = 0;
                            }
                        } else {
                            for (i = 0; i < 8; i++) {
                                if (self->waypoint == sWaypoints[i]) {
                                    i++;
                                    if (i >= 8) {
                                        i = 0;
                                    }
                                    self->waypoint = sWaypoints[i];
                                    break;
                                }
                            }
                            self->waypointTimer = 8;
                        }
                        self->actionFunc = EnHeishi1_SetupWalk;
                    } else {
                        self->headBehaviorDecided = false;
                    }
                }
                break;
        }
        Math_ApproachF(&self->headAngle, self->headAngleTarget, self->headTurnSpeedScale,
                       self->headTurnSpeedMax * 2.0f);
    }
}


}
