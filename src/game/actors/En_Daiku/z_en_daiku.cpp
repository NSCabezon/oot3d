extern "C" {
#include "z_en_daiku.hpp"
void FUN_0034df48(void* dList, void* p);
void EnDaiku_EscapeRun(EnDaiku* self, PlayState* play);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, void* anim);
void Animation_Change_00375c08(SkelAnime* skelAnime, void* anim, f32 speed, f32 startFrame, f32 endFrame, s32 mode, f32 morph);
void EnDaiku_UpdateText(EnDaiku* self, PlayState* play);
void EnDaiku_WaitFreedom(EnDaiku* self, PlayState* play);
void EnDaiku_Change(EnDaiku* self, s32 index, s32* currentIndex);
void FUN_0034c664(void* self, void* info, s32 a, s32 b);
void FUN_0036932c(void* model, s32 idx);
void FUN_0037266c(void* model, s32 idx);
void FUN_00357a50(void* skel, s32 a, s32 b, void* entry, s32 c);
void EnDaiku_PostLimbDraw(PlayState* play, s32 limb, MTX34* mtx, void* thisx);
s32 EnDaiku_OverrideLimbDraw(PlayState* play, s32 limb, void** dList, void* thisx);
u32 FUN_00117e44(Actor* actor);
void FUN_0034c664(void* self, void* info, s32 a, s32 b);

void Matrix_RotateY(MTX34* mtx, f32 angle, s32 mode);
void EnDaiku_EscapeSuccess(EnDaiku* self, PlayState* play);
void FUN_0035c528(s32 fanfare);
void FUN_00354220(PlayState* play, s16 camId, f32 fov);
void EnDaiku_EscapeRotate(EnDaiku* self, PlayState* play);
void EnDaiku_InitEscape(EnDaiku* self, PlayState* play);
void EnDaiku_Jailed(EnDaiku* self, PlayState* play);
void EnDaiku_TentIdle(EnDaiku* self, PlayState* play);
void* ZAR_GetCMBByIndex(void* zar, s32 index);
void FUN_00353e78(void* zar, PlayState* play, SkelAnime* skelAnime, void* cmb, void* p, s32 a, void* b, void* c, s32 d);

typedef struct EnDaikuAnimInfo {
    /* 0x00 */ void* animation;
    /* 0x04 */ f32 playSpeed;
    /* 0x08 */ u8 mode;
    /* 0x0C */ f32 morphFrames;
} EnDaikuAnimInfo;

extern EnDaikuAnimInfo EnDaiku_sAnimInfo[];
extern u8 EnDaiku_sColorTable[];
extern ColliderCylinderInit EnDaiku_sCylinderInit;
extern DamageTable EnDaiku_sDamageTable;
extern CollisionCheckInfoInit2 EnDaiku_sColChkInit;

#define FLAGS 0x00000019

void EnDaiku_Init(Actor* thisx, PlayState* play);
void EnDaiku_Destroy(Actor* thisx, PlayState* play);
void EnDaiku_Update(Actor* thisx, PlayState* play);
void EnDaiku_Draw(Actor* thisx, PlayState* play);

GLOBAL_ASM("data/z_En_Daiku.data.s")
GLOBAL_ASM("../src/game/actors/En_Daiku/data_syms.s")

ActorInit En_Daiku_InitVars = {
    ACTOR_EN_DAIKU,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_DAIKU,
    sizeof(EnDaiku),
    (ActorFunc)EnDaiku_Init,
    (ActorFunc)EnDaiku_Destroy,
    (ActorFunc)EnDaiku_Update,
    (ActorFunc)EnDaiku_Draw,
};

#if 0 /* NON-MATCHING draft */
/* 7 words: initRot halfword copy regs (r1 base) */
void EnDaiku_Init(Actor* thisx, PlayState* play) {
    EnDaiku* self = (EnDaiku*)thisx;
    ObjectStatus* status;
    void* zar;
    void* cmb;
    s32 noKill = 1;
    s32 isRescued = 0;

    if ((self->actor.params & 3) == 0 && (gSaveContext.eventChkInf[9] & 1)) {
        isRescued = 1;
    } else if ((self->actor.params & 3) == 1 && (gSaveContext.eventChkInf[9] & 2)) {
        isRescued = 1;
    } else if ((self->actor.params & 3) == 2 && (gSaveContext.eventChkInf[9] & 4)) {
        isRescued = 1;
    } else if ((self->actor.params & 3) == 3 && (gSaveContext.eventChkInf[9] & 8)) {
        isRescued = 1;
    }

    if (isRescued == 1 && play->sceneNum == 0xC) {
        noKill = 0;
    } else if (!isRescued && play->sceneNum == 0x39) {
        noKill = 0;
    }

    self->startFightSwitchFlag = self->actor.shape.rot.z & 0x3F;
    self->actor.shape.rot.z = 0;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 40.0f);

    status = play->GetObjectStatus(self->actor.objBankIndex);
    zar = (void*)((u8*)status + 0x10);
    cmb = ZAR_GetCMBByIndex(zar, 0);
    self->actor.unk_19A = 1;
    FUN_00372f38(&self->actor, play, &self->skelAnimModel[0], 0, 0);
    FUN_00353e78(zar, play, &self->skelAnime, cmb, self->actor.unk_178, 2, self->unk_228, self->unk_638, 20);

    if (!noKill) {
        Actor_Kill(&self->actor);
        return;
    }

    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, &EnDaiku_sCylinderInit);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, &EnDaiku_sDamageTable, &EnDaiku_sColChkInit);

    Animation_Change_00375c08(&self->skelAnime, EnDaiku_sAnimInfo[0].animation, 1.0f, 0.0f,
                              Animation_GetLastFrame(&self->skelAnime, EnDaiku_sAnimInfo[0].animation),
                              EnDaiku_sAnimInfo[0].mode, EnDaiku_sAnimInfo[0].morphFrames);

    Actor_UpdateBgCheckInfo(play, &self->actor, 0.0f, 0.0f, 0.0f, 4);

    self->runSpeed = 5.0f;
    self->actor.targetMode = 6;
    self->currentAnimIndex = -1;
    self->initRot.x = self->actor.world.rot.x;
    self->initRot.y = self->actor.world.rot.y;
    self->initRot.z = self->actor.world.rot.z;
    self->initPos = self->actor.world.pos;

    if (play->sceneNum == 0xC) {
        EnDaiku_Change(self, 1, &self->currentAnimIndex);
        self->stateFlags |= 6;
        self->actionFunc = EnDaiku_Jailed;
    } else {
        if ((self->actor.params & 3) == 1 || (self->actor.params & 3) == 3) {
            EnDaiku_Change(self, 4, &self->currentAnimIndex);
            self->stateFlags |= 2;
        } else {
            EnDaiku_Change(self, 0, &self->currentAnimIndex);
            self->stateFlags |= 6;
        }
        {
            f32 end = self->skelAnime.endFrame;
            f32 r = Rand_ZeroOne();
            self->skelAnime.curFrame = (s32)(end * r);
        }
        self->actionFunc = EnDaiku_TentIdle;
    }
}
#endif
GLOBAL_ASM("asm/EnDaiku_Init.s")

void EnDaiku_Destroy(Actor* thisx, PlayState* play) {
    EnDaiku* self = (EnDaiku*)thisx;
    Collider_DestroyCylinder(play, &self->collider);
    FUN_003508b8(thisx, self->skelAnimModel[0], self->skelAnimModel[1], self->skelAnimModel[2], self->skelAnimModelRest, 0);
    SkelAnime_Free2(&self->skelAnime);
}

void EnDaiku_Update(Actor* thisx, PlayState* play) {
    EnDaiku* self = (EnDaiku*)thisx;
    Player* player = GET_PLAYER(play);

    if (self->currentAnimIndex == 3) {
        if ((s32)self->skelAnime.curFrame == 6 || (s32)self->skelAnime.curFrame == 15) {
            Audio_PlayActorSound2(thisx, 0x01000328);
        }
    }

    Collider_UpdateCylinder(thisx, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);

    self->actionFunc(self, play);

    if (self->stateFlags & 2) {
        f32 px = player->actor.focus.pos.x;
        u8* base = (u8*)self + 0x800;
        *(f32*)(base + 0x320) = px;
        *(f32*)(base + 0x324) = player->actor.focus.pos.y;
        *(f32*)(base + 0x328) = player->actor.focus.pos.z;

        if (self->stateFlags & 4) {
            FUN_0034c664(self, (u8*)self + 0xB08, 0, 4);
        } else {
            FUN_0034c664(self, (u8*)self + 0xB08, 0, 2);
        }
    }
}

void EnDaiku_Draw(Actor* thisx, PlayState* play) {
    EnDaiku* self = (EnDaiku*)thisx;
    s32 type;
    s32 r5;

    GraphicsContext_UnknownStub(play->state.gfxCtx);
    type = self->actor.params & 3;
    FUN_0036932c(self->skelAnime.unk_28, 0);
    FUN_0036932c(self->skelAnime.unk_28, 1);
    FUN_0036932c(self->skelAnime.unk_28, 2);
    FUN_0036932c(self->skelAnime.unk_28, 3);
    r5 = 0;
    switch (type) {
        case 0:
            FUN_0037266c(self->skelAnime.unk_28, 0);
            break;
        case 1:
            FUN_0037266c(self->skelAnime.unk_28, 1);
            r5 = 1;
            break;
        case 2:
            FUN_0037266c(self->skelAnime.unk_28, 2);
            r5 = 2;
            break;
        case 3:
            FUN_0037266c(self->skelAnime.unk_28, 3);
            r5 = 3;
            break;
    }
    FUN_00357a50(&self->skelAnime, 0, 4, EnDaiku_sColorTable + r5 * 16, 1);
    SkelAnime_DrawOpa(&self->skelAnime, (MTX34*)((u8*)self + 0x148), (OverrideLimbDrawOpa)EnDaiku_OverrideLimbDraw, (PostLimbDrawOpa)EnDaiku_PostLimbDraw, thisx, 0);
}


#if 0 /* NON-MATCHING draft */
/* 2 words: ROM table base in r1, ours r0 (~40 variants) */
void EnDaiku_Change(EnDaiku* self, s32 index, s32* currentIndex) {
    f32 zero = 0.0f;
    EnDaikuAnimInfo* info = &EnDaiku_sAnimInfo[index];
    f32 morphFrames;
    if (*currentIndex < 0 || *currentIndex == index) { morphFrames = zero; } else { morphFrames = info->morphFrames; }
    Animation_Change_00375c08(&self->skelAnime, info->animation, 1.0f, zero, Animation_GetLastFrame(&self->skelAnime, info->animation), info->mode, morphFrames);
    *currentIndex = index;
}
#endif
GLOBAL_ASM("asm/EnDaiku_Change.s")

void EnDaiku_EscapeRotate(EnDaiku* self, PlayState* play) {
    s16 diff = Math_SmoothStepToS(&self->actor.shape.rot.y, self->rotYtowardsPath, 1, 0x1388, 0);
    FUN_00370734(&self->skelAnime);
    if (diff == 0) {
        self->actionFunc = EnDaiku_EscapeRun;
        EnDaiku_EscapeRun(self, play);
    }
}

/* 5 words: ldrh r1 vs r0 + vldr 0.0f hoist order after Math_SmoothStepToS */
void EnDaiku_EscapeRun(EnDaiku* self, PlayState* play) {
    Path* path;
    s16 ry;
    f32 dx;
    f32 dz;
    f32 dxz;
    Vec3s* pointPos;
    EnDaiku* s2 = self;
    path = &play->setupPathList[(self->actor.params >> 4) & 0xF];
    pointPos = &path->points[self->waypoint];
    dx = pointPos->x - self->actor.world.pos.x;
    dz = pointPos->z - self->actor.world.pos.z;
    ry = Math_FAtan2F(dx, dz) * 10430.378f;
    dxz = sqrtf(dx * dx + dz * dz);
    if (dxz <= 20.88f) {
        self->waypoint++;
        if (self->waypoint >= path->count) {
            if (self->subCamActive) {
                EnDaiku_EscapeSuccess(self, play);
            }
            Actor_Kill(&self->actor);
            return;
        }
    }

    Math_SmoothStepToS(&self->actor.shape.rot.y, ry, 1, 0xFA0, 0);
    s2->actor.world.rot.y = self->actor.shape.rot.y;
    Math_SmoothStepToF(&self->actor.speedXZ, self->runSpeed, 0.6f, dxz, 0.0f);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(play, &self->actor, 0.0f, 0.0f, 0.0f, 4);

    if (self->subCamActive) {
        self->subCamAtNext.x = self->actor.world.pos.x;
        self->subCamAtNext.y = self->actor.world.pos.y + 60.0f;
        self->subCamAtNext.z = self->actor.world.pos.z;
        Math_SmoothStepToF(&self->subCamAt.x, self->subCamAtNext.x, 1.0f, 1000.0f, 0.0f);
        Math_SmoothStepToF(&self->subCamAt.y, self->subCamAtNext.y, 1.0f, 1000.0f, 0.0f);
        Math_SmoothStepToF(&self->subCamAt.z, self->subCamAtNext.z, 1.0f, 1000.0f, 0.0f);
        Gameplay_CameraSetAtEye(play, self->subCamId, &self->subCamAt, &self->subCamEye);
        if (self->escapeSubCamTimer-- <= 0) {
            EnDaiku_EscapeSuccess(self, play);
        }
    }
    FUN_00370734(&self->skelAnime);
}

#if 0 /* matches 0 words in fastchk; needs pin_static (guard 0x521C4C obj 0x59903C/12) */
void EnDaiku_EscapeSuccess(EnDaiku* self, PlayState* play) {
    Gameplay_ClearCamera(play, self->subCamId);
    Gameplay_ChangeCameraStatus(play, 0, 7);
    self->subCamActive = 0;

    if ((gSaveContext.eventChkInf[9] & 0xF) == 0xF) {
        static Vector3f sVec(0.0f, 0.0f, 120.0f);
        Actor* guard;
        Vec3f vec;
        MTX34 mtx;

        Matrix_RotateY(&mtx, self->initRot.y * 9.5873802e-05f, 0);
        VEC3TransformAsm((Vector3f*)&vec, &mtx, &sVec);
        guard = Actor_Spawn(&play->actorCtx, play, 0x1D0, self->initPos.x + vec.x, self->initPos.y + vec.y,
                            self->initPos.z + vec.z, 0, (s16)(Math_FAtan2F(-vec.x, -vec.z) * 10430.378f), 0, 2, 1);
        if (guard == NULL) {
            Actor_Kill(&self->actor);
        }
        return;
    }
    FUN_0036e980(play, &self->actor, 7);
}
#endif
GLOBAL_ASM("asm/EnDaiku_EscapeSuccess.s")


typedef struct EnDaikuSubCamParam {
    f32 x;
    f32 y;
    f32 z;
    s32 maxFramesActive;
} EnDaikuSubCamParam;

extern EnDaikuSubCamParam EnDaiku_sEscapeSubCamParams[];
#define ENDAIKU_SUBCAM_PARAMS EnDaiku_sEscapeSubCamParams

static inline void EnDaiku_InitSubCamera(EnDaiku* self, PlayState* play) {
    Actor* thisx = &self->actor;
    Vec3f eyePosDeltaLocal;
    Vec3f eyePosDeltaWorld;
    MTX34 mtx;

    self->subCamActive = 1;
    self->escapeSubCamTimer = ENDAIKU_SUBCAM_PARAMS[self->actor.params & 3].maxFramesActive;

    eyePosDeltaLocal.x = ENDAIKU_SUBCAM_PARAMS[self->actor.params & 3].x;
    eyePosDeltaLocal.y = ENDAIKU_SUBCAM_PARAMS[self->actor.params & 3].y;
    eyePosDeltaLocal.z = ENDAIKU_SUBCAM_PARAMS[self->actor.params & 3].z;
    Matrix_RotateY(&mtx, self->actor.world.rot.y * 9.5873802e-05f, 0);
    VEC3TransformAsm((Vector3f*)&eyePosDeltaWorld, &mtx, (Vector3f*)&eyePosDeltaLocal);

    self->subCamEyeInit.x = self->subCamEye.x = thisx->world.pos.x + eyePosDeltaWorld.x;
    self->subCamEyeInit.y = self->subCamEye.y = thisx->world.pos.y + eyePosDeltaWorld.y;
    self->subCamEyeInit.z = self->subCamEye.z = thisx->world.pos.z + eyePosDeltaWorld.z;

    self->subCamAtNext.x = self->subCamAt.x = self->actor.world.pos.x;
    self->subCamAtNext.y = self->subCamAt.y = self->actor.world.pos.y + 60.0f;
    self->subCamAtNext.z = self->subCamAt.z = self->actor.world.pos.z;

    self->subCamId = Gameplay_CreateSubCamera(play);
    Gameplay_ChangeCameraStatus(play, 0, 1);
    Gameplay_ChangeCameraStatus(play, self->subCamId, 7);
    Gameplay_CameraSetAtEye(play, self->subCamId, &self->subCamAt, &self->subCamEye);
    FUN_00354220(play, self->subCamId, *(f32*)((u8*)play + 0x4A8));
    FUN_0036e980(play, &self->actor, 1);
}

void EnDaiku_InitEscape(EnDaiku* self, PlayState* play) {
    Path* path;
    f32 dxz;
    f32 dx;
    f32 dz;
    Vec3s* pointPos;
    s32 exitLoop;

    FUN_0035c528(0x010005BE);
    EnDaiku_Change(self, 3, &self->currentAnimIndex);
    self->stateFlags &= ~6;

    gSaveContext.eventChkInf[9] |= 1 << (self->actor.params & 3);

    self->actor.gravity = -1.0f;
    self->escapeSubCamTimer = ENDAIKU_SUBCAM_PARAMS[self->actor.params & 3].maxFramesActive;
    EnDaiku_InitSubCamera(self, play);

    exitLoop = 0;
    path = &play->setupPathList[(self->actor.params >> 4) & 0xF];
    while (!exitLoop) {
        pointPos = &path->points[self->waypoint];
        dx = pointPos->x - self->actor.world.pos.x;
        dz = pointPos->z - self->actor.world.pos.z;
        self->rotYtowardsPath = Math_FAtan2F(dx, dz) * 10430.378f;
        dxz = sqrtf(dx * dx + dz * dz);
        if (dxz > 10.0f) {
            exitLoop = 1;
        } else {
            self->waypoint++;
        }
    }
    self->actionFunc = EnDaiku_EscapeRotate;
}
GLOBAL_ASM("../split/asm/FUN_0025ab78.s")

void EnDaiku_Jailed(EnDaiku* self, PlayState* play) {
    Actor* gerudo;

    if (!(self->stateFlags & 8)) {
        EnDaiku_UpdateText(self, play);
    }
    FUN_00370734(&self->skelAnime);

    gerudo = Actor_Find(&play->actorCtx, ACTOR_EN_GELDB, ACTORCAT_ENEMY);
    if (gerudo == NULL) {
        self->stateFlags &= ~8;
        self->stateFlags |= 0x10;
        EnDaiku_Change(self, 2, &self->currentAnimIndex);
        self->actionFunc = EnDaiku_WaitFreedom;
    } else if (!(self->stateFlags & 8) && FUN_00117e44(gerudo) != 0 && *(u16*)((u8*)gerudo + 0xC14) == 0) {
        self->stateFlags |= 8;
        self->actor.flags &= ~9;
    }
}

s32 EnDaiku_OverrideLimbDraw(PlayState* play, s32 limb, void** dList, void* thisx) {
    if (limb == 8) {
        FUN_0034df48(dList, (u8*)thisx + 0xB16);
    } else if (limb == 9) {
        FUN_0034df48(dList, (u8*)thisx + 0xB10);
    }
    return 0;
}

#if 0 /* NON-MATCHING draft */
/* 6 words: VFP numbering of guarded static Vector3f(700,0,0); pin: guard 0x521C50 obj 0x599048/12 */
static inline void EnDaiku_MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}

void EnDaiku_PostLimbDraw(PlayState* play, s32 limb, MTX34* mtx, void* thisx) {
    EnDaiku* self = (EnDaiku*)thisx;
    static Vector3f sMultVec(700.0f, 0.0f, 0.0f);

    if (limb == 9) {
        EnDaiku_MultVec3f(&sMultVec, (Vector3f*)&self->actor.focus.pos, mtx);
    }
}
#endif
GLOBAL_ASM("asm/EnDaiku_PostLimbDraw.s")

void EnDaiku_EscapeRun(EnDaiku* self, PlayState* play);

void EnDaiku_TentIdle(EnDaiku* self, PlayState* play) {
    FUN_00370734(&self->skelAnime);
    EnDaiku_UpdateText(self, play);
}

static inline s32 EnDaiku_UpdateTalking(EnDaiku* self, PlayState* play) {
    s32 newTalkState = 2;

    if (Message_GetState(&play->msgCtx) == 6) {
        if (play->sceneNum == 0xC) {
            if (Message_ShouldAdvance(play)) {
                switch (self->actor.textId) {
                    case 0x6007:
                        Flags_SetSwitch(play, self->startFightSwitchFlag);
                        newTalkState = 0;
                        break;
                    default:
                        self->actionFunc = EnDaiku_InitEscape;
                        newTalkState = 3;
                        break;
                }
            }
        } else if (play->sceneNum == 0x39) {
            if (Message_ShouldAdvance(play)) {
                switch (self->actor.textId) {
                    case 0x6061:
                        gSaveContext.infTable[23] |= 0x40;
                        break;
                    case 0x6064:
                        gSaveContext.infTable[23] |= 0x100;
                        break;
                }
                newTalkState = 0;
            }
        }
    }
    return newTalkState;
}

void EnDaiku_UpdateText(EnDaiku* self, PlayState* play) {
    s32 carpenterType;
    s32 rescuedCount;
    s16 screenX;
    s16 screenY;

    if (self->talkState == 2) {
        self->talkState = EnDaiku_UpdateTalking(self, play);
    } else if (Actor_ProcessTalkRequest(&self->actor, play)) {
        self->talkState = 2;
    } else {
        Actor_GetScreenPos(play, &self->actor, &screenX, &screenY);
        if ((screenX >= 0) && (screenX <= 320) && (screenY >= 0) && (screenY <= 240) && (self->talkState == 0) &&
            (FUN_0036bb28(&self->actor, play, 100.0f) == 1)) {
            if (play->sceneNum == 0xC) {
                if (self->stateFlags & 0x10) {
                    rescuedCount = 0;
                    for (carpenterType = 0; carpenterType < 4; carpenterType++) {
                        if (gSaveContext.eventChkInf[9] & (1 << carpenterType)) {
                            rescuedCount++;
                        }
                    }
                    switch (rescuedCount) {
                        case 0:
                            self->actor.textId = 0x605B;
                            break;
                        case 1:
                            self->actor.textId = 0x605C;
                            break;
                        case 2:
                            self->actor.textId = 0x605D;
                            break;
                        case 3:
                            self->actor.textId = 0x605E;
                            break;
                    }
                } else if (!(self->stateFlags & 0x18)) {
                    self->actor.textId = 0x6007;
                }
            } else if (play->sceneNum == 0x39) {
                switch (self->actor.params & 3) {
                    case 0:
                        if (CHECK_QUEST_ITEM(QUEST_MEDALLION_SPIRIT)) {
                            self->actor.textId = 0x6060;
                        } else {
                            self->actor.textId = 0x605F;
                        }
                        break;
                    case 1:
                        if (CHECK_QUEST_ITEM(QUEST_MEDALLION_SPIRIT)) {
                            self->actor.textId = 0x6063;
                        } else {
                            if (!(gSaveContext.infTable[23] & 0x40)) {
                                self->actor.textId = 0x6061;
                            } else {
                                self->actor.textId = 0x6062;
                            }
                        }
                        break;
                    case 2:
                        if (CHECK_QUEST_ITEM(QUEST_MEDALLION_SPIRIT)) {
                            self->actor.textId = 0x6066;
                        } else {
                            if (!(gSaveContext.infTable[23] & 0x100)) {
                                self->actor.textId = 0x6064;
                            } else {
                                self->actor.textId = 0x6065;
                            }
                        }
                        break;
                    case 3:
                        if (CHECK_QUEST_ITEM(QUEST_MEDALLION_SPIRIT)) {
                            self->actor.textId = 0x6068;
                        } else {
                            self->actor.textId = 0x6067;
                        }
                        break;
                }
            }
        }
    }
}


void EnDaiku_WaitFreedom(EnDaiku* self, PlayState* play) {
    FUN_00370734(&self->skelAnime);
    if (self->currentAnimIndex == 2) {
        if (Animation_OnFrameImpl(&self->skelAnime, 14.0f, 1.0f)) {
            Audio_PlayActorSound2(&self->actor, 0x0100025C);
        } else if (Animation_OnFrameImpl(&self->skelAnime, 19.0f, 1.0f)) {
            Audio_PlayActorSound2(&self->actor, 0x0100046E);
        }
    }
    if (Flags_GetSwitch(play, (self->actor.params >> 8) & 0x3F)) {
        self->actor.flags |= 9;
        EnDaiku_UpdateText(self, play);
    }
}


}
