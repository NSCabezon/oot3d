extern "C" {
#include "z_en_ma2.hpp"

#define FLAGS 0x02000039

void EnMa2_Init(Actor* thisx, PlayState* play);
void EnMa2_Destroy(Actor* actor, GameState* state);
void EnMa2_Update(Actor* thisx, PlayState* play);
void EnMa2_Draw(Actor* thisx, PlayState* play);

GLOBAL_ASM("data/z_En_Ma2.data.s")
GLOBAL_ASM("../src/game/actors/En_Ma2/data_syms.s")

ActorInit En_Ma2_InitVars = {
    ACTOR_EN_MA2,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_MA2,
    sizeof(EnMa2),
    (ActorFunc)EnMa2_Init,
    (ActorFunc)EnMa2_Destroy,
    (ActorFunc)EnMa2_Update,
    (ActorFunc)EnMa2_Draw,
};

typedef struct EnMa2AnimInfo {
    /* 0x00 */ s32 animIdx;
    /* 0x04 */ f32 playbackSpeed;
    /* 0x08 */ u8 mode;
    /* 0x0C */ f32 morphFrames;
} EnMa2AnimInfo; // size = 0x10

// Rodata tables pinned via EQU symbols in data_syms.s (debt: real statics need a data split).
extern EnMa2AnimInfo EnMa2_sAnimationInfo[];
extern ColliderCylinderInit EnMa2_sCylinderInit;
extern CollisionCheckInfoInit2 EnMa2_sColChkInfoInit;
void FUN_00353e78(void* zar, PlayState* play, SkelAnime* skel, void* cmb, void* p, s32 b, void* jointTable,
                  void* morphTable, s32 limbCount);
void* ZAR_GetCMBByIndex(void* zar, s32 index);
DamageTable* DamageTable_Get(s32 index);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void FUN_003c6218(EnMa2* self, PlayState* play); // N64 EnMa2_WaitToEndTalk
void FUN_001c53ec(EnMa2* self, PlayState* play);

static inline u8* EnMa2_GetObjBank(PlayState* play, u32 idx) {
    if (idx < 0x13 && *(s32*)((u8*)play + (idx << 7) + 0x3A64) != 0) {
        return (u8*)play + (idx << 7) + 0x3A5C;
    }
    return NULL;
}

// N64 EnMa2_ChangeAnim (inlined in ROM).
#define sAnimationInfo EnMa2_sAnimationInfo
static inline void EnMa2_ChangeAnim(EnMa2* self, s32 index) {
    f32 frameCount = (f32)Animation_GetLastFrame(&self->skelAnime, sAnimationInfo[index].animIdx);

    Animation_Change_00375c08(&self->skelAnime, sAnimationInfo[index].animIdx, 1.0f, 0.0f, frameCount,
                              sAnimationInfo[index].mode, sAnimationInfo[index].morphFrames);
}

// N64 EnMa2_GetSpawnIndex (inlined in ROM).
static inline u16 EnMa2_GetSpawnIndex(EnMa2* self, PlayState* play) {
    if (LINK_IS_CHILD) {
        return 0;
    }
    if (!(gSaveContext.eventChkInf[1] & 0x100) && (play->sceneNum == 0x36) && IS_DAY &&
        (self->actor.shape.rot.z == 5)) {
        return 1;
    }
    if (!(gSaveContext.eventChkInf[1] & 0x100) && (play->sceneNum == 0x63) && IS_NIGHT &&
        (self->actor.shape.rot.z == 6)) {
        return 2;
    }
    if (!(gSaveContext.eventChkInf[1] & 0x100) || (play->sceneNum != 0x63)) {
        return 0;
    }
    if ((self->actor.shape.rot.z == 7) && IS_DAY) {
        return 3;
    }
    if ((self->actor.shape.rot.z == 8) && IS_NIGHT) {
        return 3;
    }
    return 0;
}

void EnMa2_Init(Actor* thisx, PlayState* play) {
    EnMa2* self = (EnMa2*)thisx;
    PlayState* p2 = play;
    u8* zar;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 18.0f);
    zar = EnMa2_GetObjBank(play, (u8)thisx->objBankIndex) + 0x10;
    SkelAnime* skel = &self->skelAnime;
    void* cmb = ZAR_GetCMBByIndex(zar, 0);
    void* ud = thisx->unk_178;
    FUN_00353e78(zar, p2, skel, cmb, ud, -1, NULL, NULL, 0);
    SkelAnime_SetFaceAnimations((FaceAnimations*)self->unk_228, &self->skelAnime, 0, 1, -1);
    self->singingDisabled = 1;
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, thisx, &EnMa2_sCylinderInit);
    CollisionCheck_SetInfo2(&thisx->colChkInfo, DamageTable_Get(22), &EnMa2_sColChkInfoInit);

    switch (EnMa2_GetSpawnIndex(self, play)) {
        case 1:
            EnMa2_ChangeAnim(self, 2);
            self->actionFunc = FUN_003c6218;
            break;
        case 2:
            EnMa2_ChangeAnim(self, 3);
            ((EnMa2*)thisx)->actionFunc = FUN_001c53ec;
            break;
        case 3:
            if (gSaveContext.infTable[8] & 0x2000) {
                EnMa2_ChangeAnim(self, 0);
            } else {
                EnMa2_ChangeAnim(self, 3);
            }
            self->actionFunc = FUN_003c6218;
            break;
        case 0:
            Actor_Kill(&self->actor);
            return;
    }

    Actor_UpdateBgCheckInfo(play, &self->actor, 0.0f, 0.0f, 0.0f, 4);
    Actor_SetScale(&self->actor, 0.01f);
    self->actor.targetMode = 6;
    self->interactInfo.talkState = 0;
}


void EnMa2_Destroy(Actor* thisx, GameState* state) {
    EnMa2* self = (EnMa2*)thisx;

    SkelAnime_Free2(&self->skelAnime);
    ((FaceAnimations*)self->unk_228)->Destroy();
    Collider_DestroyCylinder((PlayState*)state, &self->collider);
}

void FUN_0034e01c(MTX34* mtx, Vec3s* rot);

s32 FUN_00233648(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnMa2* self = (EnMa2*)thisx;

    if (limbIndex == 8) {
        FUN_0034e01c(mtx, &self->interactInfo.torsoRot);
    } else if (limbIndex == 7) {
        FUN_0034e01c(mtx, &self->interactInfo.headRot);
    }
    return 0;
}

// UpdateTalkState: only Message_GetState CLOSING (2) changes the state.
s16 FUN_00195dac(PlayState* play, Actor* thisx) {
    s16 talkState = 1;

    switch (Message_GetState(&play->msgCtx)) {
        case 0:
        case 1:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            break;
        case 2: // TEXT_STATE_CLOSING
            switch (thisx->textId) {
                case 0x2051:
                    gSaveContext.infTable[8] |= 0x1000;
                    talkState = 2;
                    break;
                case 0x2053:
                    gSaveContext.infTable[8] |= 0x2000;
                    talkState = 0;
                    break;
                default:
                    talkState = 0;
                    break;
            }
            break;
        default:
            break;
    }
    return talkState;
}

// N64 Matrix_MultVec3f(src, dest) argument order; the wrapper gives the predicated call shape.
static inline void EnMa2_MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}

void FUN_002172bc(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnMa2* self = (EnMa2*)thisx;
    Vector3f offset(900.0f, 0.0f, 0.0f);

    if (limbIndex == 8) {
        EnMa2_MultVec3f(&offset, (Vector3f*)&self->actor.focus.pos, mtx);
    }
}

void FUN_0037073c(PlayState* play, s32 arg);
void FUN_00152300(EnMa2* self, PlayState* play); // N64 EnMa2_ForceTalkAfterSong (not yet in C)
void FUN_001c53ec(EnMa2* self, PlayState* play);
void FUN_001c5338(EnMa2* self, PlayState* play);

// N64 EnMa2_WaitForOcarina.
void FUN_001c53ec(EnMa2* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (player->stateFlags2 & 0x1000000) {
        player->stateFlags2 |= 0x2000000;
        *(Actor**)((u8*)player + 0x1740) = &self->actor;
        FUN_0037073c(play, 0x23);
        self->actionFunc = FUN_001c5338;
    } else if (self->actor.xzDistToPlayer < self->collider.dim.radius + 30.0f) {
        player->stateFlags2 |= 0x800000;
    }
}

// Message context view at play+0x2B00 (ocarinaMode at +0x7E).
struct EnMa2MsgView {
    u8 pad[0x7E];
    u16 mode;
};

static inline EnMa2MsgView* EnMa2_Msg(PlayState* p) {
    return (EnMa2MsgView*)((u8*)p + 0x2B00);
}

// N64 EnMa2_WaitForEponasSong.
void FUN_001c5338(EnMa2* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (EnMa2_Msg(play)->mode >= 4) {
        self->actionFunc = FUN_001c53ec;
        EnMa2_Msg(play)->mode = 4;
        *(Actor**)((u8*)player + 0x1740) = NULL;
    } else if (EnMa2_Msg(play)->mode == 3) {
        FUN_00372244(&play->unk_5FCC, 0x1E, 0x01000487); // NA_SE_SY_CORRECT_CHIME
        self->timer = 0x1E;
        gSaveContext.infTable[8] |= 0x4000;
        self->actionFunc = FUN_00152300;
        EnMa2_Msg(play)->mode = 4;
    } else {
        player->stateFlags2 |= 0x800000;
    }
}

void FUN_0033d270(s32 arg);                                       // Audio_ToggleMalonSinging
void FUN_0034c664(Actor* actor, void* info, s32 arg2, s32 mode); // Npc_TrackPoint
void FUN_00342714(PlayState* play, Actor* actor, void* talkState, f32 range, void* getTextId, void* updateTalkState);
s16 FUN_00195dac(PlayState* play, Actor* thisx); // UpdateTalkState

// N64 EnMa2_IsSinging (inlined in ROM).
static inline s32 EnMa2_IsSinging(EnMa2* self) {
    if (self->skelAnime.animationType != 3) {
        return false;
    }
    if (self->interactInfo.talkState != 0) {
        return false;
    }
    self->blinkTimer = 0;
    if (self->eyeIndex != 2) {
        return false;
    }
    self->mouthIndex = 2;
    return true;
}

static inline void EnMa2_UpdateEyes(EnMa2* self) {
    if (!EnMa2_IsSinging(self) && (DECR(self->blinkTimer) == 0)) {
        self->eyeIndex++;
        if (self->eyeIndex >= 3) {
            self->blinkTimer = Rand_S16Offset_003702c8(30, 30);
            self->eyeIndex = 0;
        }
    }
}

static inline void EnMa2_UpdateSinging(EnMa2* self) {
    if (self->skelAnime.animationType == 3) {
        if (self->interactInfo.talkState == 0) {
            if (self->singingDisabled) {
                FUN_0033d270(false);
                self->singingDisabled = false;
            }
        } else {
            if (!self->singingDisabled) {
                FUN_0033d270(true);
                self->singingDisabled = true;
            }
        }
    }
}

static inline void EnMa2_UpdateTracking(EnMa2* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 trackingMode;

    if ((self->interactInfo.talkState == 0) && (self->skelAnime.animationType == 3)) {
        trackingMode = 1;
    } else {
        trackingMode = 0;
    }
    self->interactInfo.trackPos = player->actor.world.pos;
    self->interactInfo.yOffset = 0.0f;
    FUN_0034c664(&self->actor, &self->interactInfo, 0, trackingMode);
}

void EnMa2_Update(Actor* thisx, PlayState* play) {
    EnMa2* self = (EnMa2*)thisx;

    Collider_UpdateCylinder(thisx, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, (Collider*)&self->collider);
    FUN_003731e0(&self->skelAnime);
    EnMa2_UpdateEyes(self);

    self->actionFunc(self, play);

    EnMa2_UpdateSinging(self);
    EnMa2_UpdateTracking(self, play);
    if (self->actionFunc != FUN_001c5338) {
        // GetTextId is the unlabeled function after UpdateTalkState's literal pool (0x195E2C).
        FUN_00342714(play, thisx, &self->interactInfo.talkState, self->collider.dim.radius + 30.0f,
                     (void*)0x00195E2C, (void*)FUN_00195dac);
    }
}

// FaceAnimations calls go through this helper (arg order).
static inline void EnMa2_SetFace(FaceAnimations* f, s32 a, s32 b) {
    f->FUN_0035e3a4(a, b);
}

void FUN_0036932c(SkeletonAnimationModel* model, s32 idx);
void FUN_0037266c(SkeletonAnimationModel* model, s32 idx);
void FUN_0033d364(f32 distFromCamEye, u32 bgmId); // Audio_UpdateMalonSinging

void EnMa2_Draw(Actor* thisx, PlayState* play) {
    EnMa2* self = (EnMa2*)thisx;
    Camera* activeCam = play->cameraPtrs[play->activeCamera];
    f32 distFromCamEye = Math_Vec3f_DistXZ(&thisx->world.pos, &activeCam->eye);

    FUN_0033d364(distFromCamEye, 0x0100059D); // Audio_UpdateMalonSinging(dist, NA_BGM_LONLON)
    if (self->skelAnime.animationType == 2) {
        FUN_0037266c(self->skelAnime.unk_28, 2);
    } else {
        FUN_0036932c(self->skelAnime.unk_28, 2);
    }
    EnMa2_SetFace((FaceAnimations*)self->unk_228, 0, self->eyeIndex);
    EnMa2_SetFace((FaceAnimations*)self->unk_228, 1, self->mouthIndex);
    ((FaceAnimations*)self->unk_228)->FUN_0035e330();
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, FUN_00233648, FUN_002172bc, &self->actor, 0);
}
}
