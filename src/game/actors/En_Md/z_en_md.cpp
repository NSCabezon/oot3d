extern "C" {
#include "z_en_md.hpp"

#define FLAGS 0x02000019

void EnMd_Init(EnMd* self, PlayState* play);
void EnMd_Destroy(Actor* thisx, PlayState* play);
void EnMd_Update(Actor* thisx, PlayState* play);
void EnMd_Draw(Actor* thisx, PlayState* play);

void FUN_0034e01c(MTX34* mtx, Vec3s* rot);
void FUN_00370f5c(PlayState* play, s16* arg1, s16* arg2, s32 arg3); // Actor_UpdateFidgetTables
s16 FUN_00341c28(Actor* actor, PlayState* play, s16 alpha, f32 radius); // Actor_UpdateAlphaByDistance
void FUN_0034c664(Actor* actor, void* info, s32 arg2, s32 arg3); // Npc_TrackPoint
void FUN_00342714(PlayState* play, Actor* actor, void* talkState, f32 range, void* getTextId, void* updateTalkState); // Npc_UpdateTalking
extern s32 gDbgCamEnabled;
s16 FUN_001c4ddc(s32 index);

GLOBAL_ASM("data/z_En_Md.data.s")

// Statics live in the data dump (see data_syms.s).
extern ColliderCylinderInit EnMd_sCylinderInit;
extern CollisionCheckInfoInit2 EnMd_sColChkInfoInit;
extern struct_80034EC0_Entry EnMd_sAnimationInfo[];
void FUN_00353e78(void* zar, PlayState* play, SkelAnime* skel, void* cmb, void* p, s32 b, void* jointTable,
                  void* morphTable, s32 limbCount);
void* ZAR_GetCMBByIndex(void* zar, s32 index);

ActorInit En_Md_InitVars = {
    ACTOR_EN_MD,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_MD,
    sizeof(EnMd),
    (ActorFunc)EnMd_Init,
    (ActorFunc)EnMd_Destroy,
    (ActorFunc)EnMd_Update,
    (ActorFunc)EnMd_Draw,
};

GLOBAL_ASM("../src/game/actors/En_Md/data_syms.s")

void EnMd_Destroy(Actor* thisx, PlayState* play) {
    EnMd* self = (EnMd*)thisx;

    Collider_DestroyCylinder(play, &self->collider);
    SkelAnime_Free2(&self->skelAnime);
    self->faceAnim.Destroy();
}


typedef struct {
    f32 v[4];
} EnMdColor;

void* FUN_003687a8(struct SkeletonAnimationModel* model);
void FUN_003589cc(void* arg0, s32 arg1);
void FUN_00358964(void* arg0, s32 arg1, void* arg2);
s32 EnMd_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, EnMd* self);
void EnMd_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, EnMd* self);

void EnMd_Draw(Actor* thisx, PlayState* play) {
    EnMd* self = (EnMd*)thisx;
    void* model;

    {
        FaceAnimations* fa = &self->faceAnim;
        fa->FUN_0035e3a4(0, self->eyeTexIndex);
    }
    self->faceAnim.FUN_0035e330();

    if (self->alpha != 0) {
        EnMdColor color = { { 1.0f, 1.0f, 1.0f, 0.0f } };

        color.v[3] = self->alpha * (1.0f / 255.0f);
        model = FUN_003687a8(self->skelAnime.unk_28);
        FUN_003589cc(model, 4);
        FUN_00358964(model, 4, &color);
        SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, (OverrideLimbDrawOpa)EnMd_OverrideLimbDraw,
                          (PostLimbDrawOpa)EnMd_PostLimbDraw, &self->actor, 1);
    }
}

s32 EnMd_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, EnMd* self) {
    if (limbIndex == 9) {
        FUN_0034e01c(mtx, &self->interactInfo.headRot);
    } else if (limbIndex == 8) {
        FUN_0034e01c(mtx, &self->interactInfo.torsoRot);
    }
    return 0;
}

static inline void EnMd_SetVec(Vec3f* v, f32 x, f32 y, f32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

void EnMd_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, EnMd* self) {
    if (limbIndex == 9) {
        Vector3f* dst = (Vector3f*)&self->actor.focus;
        Vec3f vec;

        EnMd_SetVec(&vec, 400.0f, 0.0f, 0.0f);
        VEC3TransformAsm(dst, mtx, (Vector3f*)&vec);
    }
}
GLOBAL_ASM("../split/asm/FUN_0021e968.s")

void FUN_00105bd0(EnMd* self, PlayState* play);
void FUN_00370d60(EnMd* self);
void FUN_00370818(EnMd* self);
void FUN_00370778(PlayState* play);
void FUN_003725e0(PlayState* play);
void FUN_0037073c(PlayState* play, u16 action);
void FUN_003d2ae4(EnMd* self, PlayState* play);
void FUN_00196354(EnMd* self, GameState* state);
void FUN_00372244(void* arg0, u16 arg1, u32 sfxId);
s32 FUN_0036bb28(Actor* actor, PlayState* play, f32 arg2);

typedef struct {
    /* 0x00 */ char unk_00[0x7E];
    /* 0x7E */ u16 state;
} EnMdPlaySub;

void FUN_00196354(EnMd* self, GameState* state) {
    PlayState* play = (PlayState*)state;
    Player* player = GET_PLAYER((PlayState*)state);
    EnMdPlaySub* sub = (EnMdPlaySub*)((u8*)(PlayState*)state + 0x2B00);

    if (sub->state >= 4) {
        self->actionFunc = (EnMdActionFunc)FUN_00105bd0;
        sub->state = 4;
    } else if (sub->state == 3) {
        FUN_00372244((u8*)(PlayState*)state + 0x5C00 + 0x3CC, 0x1E, 0x01000487);
        *(u16*)((u8*)self + 0x116) = 0x1067;
        FUN_0036bb28(&self->actor, play, *(f32*)((u8*)self + 0x438) + 30.0f);
        self->actionFunc = (EnMdActionFunc)FUN_00105bd0;
        sub->state = 4;
    } else {
        player->stateFlags2 |= 0x800000;
    }
}

void FUN_00370cf4(EnMd* self, PlayState* play) {
    if (self->skelAnime.animationType == 0) {
        FUN_00370f5c(play, self->fidgetTableY, self->fidgetTableZ, 0x12);
    } else if (self->interactInfo.talkState == 0) {
        if (self->animSequence != 7) {
            self->animSequence = 7;
            self->animSequenceEntry = 0;
        }
    }
    FUN_00370d60(self);
}

void FUN_003707dc(EnMd* self, PlayState* play) {
    if (self->skelAnime.animationType == 0) {
        FUN_00370f5c(play, self->fidgetTableY, self->fidgetTableZ, 0x12);
    }
    FUN_00370818(self);
}

static inline u16 EnMd_GetTextIdKokiriForest(PlayState* play, Actor* thisx) {
    EnMd* self = (EnMd*)thisx;
    u16 textId = Text_GetFaceReaction(play, 0x11);

    if (textId != 0) {
        return textId;
    }

    self->messageEntry = 0;
    self->messageState = 0;

    if (CHECK_QUEST_ITEM(18)) {
        return 0x1045;
    }

    if (gSaveContext.eventChkInf[0] & 0x10) {
        return 0x1034;
    }

    if ((CUR_EQUIP_VALUE(1) == 1) && (CUR_EQUIP_VALUE(0) == 1)) {
        return 0x1033;
    }

    if (gSaveContext.infTable[0] & 0x1000) {
        return 0x1030;
    }

    return 0x102F;
}

static inline u16 EnMd_GetTextIdMidosHouse(PlayState* play, Actor* thisx) {
    EnMd* self = (EnMd*)thisx;
    self->messageEntry = 0;
    self->messageState = 0;

    if (gSaveContext.eventChkInf[4] & 1) {
        return 0x1028;
    }

    return 0x1046;
}

static inline u16 EnMd_GetTextIdLostWoods(PlayState* play, Actor* thisx) {
    EnMd* self = (EnMd*)thisx;
    self->messageEntry = 0;
    self->messageState = 0;

    if (gSaveContext.eventChkInf[4] & 0x100) {
        if (gSaveContext.infTable[1] & 0x200) {
            return 0x1071;
        }
        return 0x1070;
    }

    if (gSaveContext.eventChkInf[0] & 0x400) {
        return 0x1068;
    }

    if (gSaveContext.infTable[1] & 0x20) {
        return 0x1061;
    }

    return 0x1060;
}

u16 FUN_001738c4(PlayState* play, Actor* thisx) {
    switch (play->sceneNum) {
        case 0x55:
            return EnMd_GetTextIdKokiriForest(play, thisx);
        case 0x28:
            return EnMd_GetTextIdMidosHouse(play, thisx);
        case 0x5B:
            return EnMd_GetTextIdLostWoods(play, thisx);
        default:
            return 0;
    }
}

static inline s16 EnMd_TrackMessageState(EnMd* self, PlayState* play) {
    s16 messageState = Message_GetState(&play->msgCtx);

    if ((self->messageState == 0xA) || (self->messageState == 5) || (self->messageState == 2) ||
        (self->messageState == 1)) {
        if (self->messageState != messageState) {
            self->messageEntry++;
        }
    }

    self->messageState = messageState;
    return messageState;
}

s16 FUN_00173778(PlayState* play, Actor* thisx) {
    EnMd* self = (EnMd*)thisx;

    switch (EnMd_TrackMessageState(self, play)) {
        case 0:
        case 1:
        case 3:
        case 4:
        case 6:
        case 7:
        case 8:
        case 9:
            return 1;
        case 2:
            switch (self->actor.textId) {
                case 0x1028:
                    gSaveContext.eventChkInf[0] |= 0x8000;
                    break;
                case 0x102F:
                    gSaveContext.eventChkInf[0] |= 4;
                    gSaveContext.infTable[0] |= 0x1000;
                    break;
                case 0x1060:
                    gSaveContext.infTable[1] |= 0x20;
                    break;
                case 0x1070:
                    gSaveContext.infTable[1] |= 0x200;
                    break;
                case 0x1033:
                case 0x1067:
                    return 2;
            }
            return 0;
        case 5:
            if (Message_ShouldAdvance(play)) {
                return 2;
            }
        default:
            return 1;
    }
}

static __inline __pure float EnMd_Fabsf(float x) {
    return __fabsf(x);
}

void FUN_00105bd0(EnMd* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 temp;
    Actor* actorToBlock = &GET_PLAYER(play)->actor;
    s16 yaw;

    FUN_00370d60(self);

    if (self->interactInfo.talkState == 0) {
        self->actor.world.rot.y = self->actor.yawTowardsPlayer;
        self->actor.shape.rot.y = self->actor.yawTowardsPlayer;

        yaw = Math_Vec3f_Yaw(&self->actor.home.pos, &actorToBlock->world.pos);

        self->actor.world.pos.x = self->actor.home.pos.x;
        self->actor.world.pos.x += 60.0f * Math_SinS(yaw);

        self->actor.world.pos.z = self->actor.home.pos.z;
        self->actor.world.pos.z += 60.0f * Math_CosS(yaw);

        temp = EnMd_Fabsf((f32)self->actor.yawTowardsPlayer - yaw) * 0.001f * 3.0f;
        {
            f32* sa = (f32*)&self->skelAnime.animationType;
            sa[4] = CLAMP(temp, 1.0f, 3.0f);
        }
    }

    if (self->interactInfo.talkState == 2) {
        if (CHECK_QUEST_ITEM(18) && !(gSaveContext.eventChkInf[1] & 0x1000) && (play->sceneNum == 0x55)) {
            FUN_00370778(play);
        }

        if (play->sceneNum == 0x55) {
            gSaveContext.eventChkInf[0] |= 0x10;
        }
        if (play->sceneNum == 0x5B) {
            gSaveContext.eventChkInf[0] |= 0x400;
        }

        self->animSequence = 3;
        self->animSequenceEntry = 0;
        FUN_00370818(self);
        self->actor.speedXZ = 1.5f;
        self->waypoint = 1;
        self->interactInfo.talkState = 0;
        self->actionFunc = (EnMdActionFunc)FUN_003d2ae4;
        return;
    }

    if (self->skelAnime.animationType == 0) {
        FUN_00370f5c(play, self->fidgetTableY, self->fidgetTableZ, 0x12);
    }

    if ((self->interactInfo.talkState == 0) && (play->sceneNum == 0x5B)) {
        if (player->stateFlags2 & 0x1000000) {
            player->stateFlags2 |= 0x2000000;
            *(Actor**)((u8*)player + 0x1740) = &self->actor;
            FUN_0037073c(play, 0x22);
            self->actionFunc = (EnMdActionFunc)FUN_00196354;
            return;
        }

        if (self->actor.xzDistToPlayer < (30.0f + self->collider.dim.radius)) {
            player->stateFlags2 |= 0x800000;
        }
    }
}

static inline u8 EnMd_FollowPath(EnMd* self, PlayState* play) {
    Path* path;
    Vec3s* pointPos;
    f32 pathDiffX;
    f32 pathDiffZ;
    f32 distSq;

    if (((s16)self->actor.params & 0xFF00) == 0xFF00) {
        return 0;
    }

    path = &play->setupPathList[(self->actor.params & 0xFF00) >> 8];
    pointPos = path->points;
    pointPos += self->waypoint;

    pathDiffX = pointPos->x - self->actor.world.pos.x;
    pathDiffZ = pointPos->z - self->actor.world.pos.z;
    distSq = SQ(pathDiffX) + SQ(pathDiffZ);
    Math_SmoothStepToS(&self->actor.world.rot.y, Math_FAtan2F(pathDiffX, pathDiffZ) * (65536.0f / (2 * 3.14159274f)), 4,
                       4000, 1);

    if (distSq < 100.0f) {
        self->waypoint++;
        if (self->waypoint >= path->count) {
            self->waypoint = 0;
        }
        return 1;
    }
    return 0;
}

void FUN_003d2ae4(EnMd* self, PlayState* play) {
    FUN_00370f5c(play, self->fidgetTableY, self->fidgetTableZ, 0x12);
    FUN_00370818(self);

    if (!(EnMd_FollowPath(self, play)) || (self->waypoint != 0)) {
        self->actor.shape.rot = self->actor.world.rot;
        return;
    }

    if (CHECK_QUEST_ITEM(18) && !(gSaveContext.eventChkInf[1] & 0x1000) && (play->sceneNum == 0x55)) {
        FUN_003725e0(play);
        gSaveContext.eventChkInf[1] |= 0x1000;
        Actor_Kill(&self->actor);
        return;
    }

    self->animSequence = 11;
    self->animSequenceEntry = 0;

    self->skelAnime.playSpeed = 0.0f;
    self->actor.speedXZ = 0.0f;
    self->actor.home.pos = self->actor.world.pos;
    self->actionFunc = (EnMdActionFunc)FUN_003707dc;
}

void EnMd_Update(Actor* thisx, PlayState* play) {
    EnMd* self = (EnMd*)thisx;
    Player* player;
    s16 absYawDiff;
    s16 trackingMode;
    s16 canUpdateTalking;
    s16 yawDiff;
    f32 yOffset;

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);

    if (DECR(self->blinkTimer) == 0) {
        if (play->state.frames & 1) {
            self->eyeTexIndex++;
            if (self->eyeTexIndex >= 3) {
                self->blinkTimer = Rand_S16Offset_003702c8(30, 30);
                self->eyeTexIndex = 0;
            }
        }
    }

    if (play->sceneNum != 0x28) {
        f32 radius = (CHECK_QUEST_ITEM(18) && !(gSaveContext.eventChkInf[1] & 0x1000) && (play->sceneNum == 0x55))
                         ? 100.0f
                         : 400.0f;
        self->alpha = FUN_00341c28(&self->actor, play, self->alpha, radius);
        self->actor.shape.shadowAlpha = self->alpha;
    } else {
        self->alpha = 255;
        self->actor.shape.shadowAlpha = self->alpha;
    }
    if (self->alpha != 0) {
        FUN_003731e0(&self->skelAnime);
    }

    if (self->alpha == 0) {
        self->actor.flags |= 0x20000000;
    } else {
        self->actor.flags &= ~0x20000000;
    }

    Actor_MoveForward(&self->actor);

    player = GET_PLAYER(play);
    if (self->actor.xzDistToPlayer < 170.0f) {
        absYawDiff = ABS((s16)((f32)self->actor.yawTowardsPlayer - self->actor.shape.rot.y));

        trackingMode = absYawDiff <= FUN_001c4ddc(2) ? 2 : 1;
        canUpdateTalking = 1;
    } else {
        trackingMode = 1;
        canUpdateTalking = 0;
    }

    if (self->interactInfo.talkState != 0) {
        trackingMode = 4;
    }

    if (self->actionFunc == (EnMdActionFunc)FUN_003d2ae4) {
        trackingMode = 1;
        canUpdateTalking = 0;
    }
    if (self->actionFunc == (EnMdActionFunc)FUN_003707dc) {
        trackingMode = 4;
        canUpdateTalking = 1;
    }

    if ((play->GetCutsceneState() != 0) || gDbgCamEnabled) {
        self->interactInfo.trackPos = *(Vec3f*)&play->view.eye;
        yOffset = 40.0f;
        trackingMode = 2;
    } else {
        self->interactInfo.trackPos = *(Vec3f*)&player->actor.world.pos;
        yOffset = (gSaveContext.linkAge > 0) ? 0 : -18;
    }

    self->interactInfo.yOffset = yOffset;
    FUN_0034c664(&self->actor, &self->interactInfo, 2, trackingMode);
    if (self->actionFunc != (EnMdActionFunc)FUN_00196354) {
        if (canUpdateTalking) {
            FUN_00342714(play, &self->actor, &self->interactInfo.talkState, self->collider.dim.radius + 30.0f,
                         (void*)FUN_001738c4, (void*)FUN_00173778);
        }
    }

    if (self->initialized == 0) {
        Actor_UpdateBgCheckInfo(play, &self->actor, 0.0f, 0.0f, 0.0f, 4);
        self->initialized = 1;
        self->actor.gravity = 0.0f;
    }
    self->actionFunc(self, play);
}

static inline u8* EnMd_GetObjBank(PlayState* play, u32 idx) {
    if (idx < 0x13 && *(s32*)((u8*)play + (idx << 7) + 0x3A64) != 0) {
        return (u8*)play + (idx << 7) + 0x3A5C;
    }
    return NULL;
}

static inline u8 EnMd_ShouldSpawn(EnMd* self, PlayState* play) {
    if (play->sceneNum == 0x55) {
        if (!(gSaveContext.eventChkInf[1] & 0x1000) && !(gSaveContext.eventChkInf[4] & 1)) {
            return 1;
        }
    }

    if (play->sceneNum == 0x28) {
        if ((gSaveContext.eventChkInf[1] & 0x1000) || (gSaveContext.eventChkInf[4] & 1)) {
            if (!LINK_IS_ADULT) {
                return 1;
            }
        }
    }

    if (play->sceneNum == 0x5B) {
        return 1;
    }

    return 0;
}

void EnMd_Init(EnMd* self, PlayState* play) {
    EnMd* s2 = self;

    self->actor.unk_19A = 1;
    self->initialized = 0;
    self->zar = EnMd_GetObjBank(play, (u8)self->actor.objBankIndex) + 0x10;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 24.0f);
    FUN_00353e78(self->zar, play, &self->skelAnime, ZAR_GetCMBByIndex(self->zar, 0), self->actor.unk_178, -1,
                 self->jointTable, self->morphTable, 0x12);
    SkelAnime_SetFaceAnimations(&self->faceAnim, &self->skelAnime, 0, -1, -1);

    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, &EnMd_sCylinderInit);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, NULL, &EnMd_sColChkInfoInit);
    if (!EnMd_ShouldSpawn(self, play)) {
        Actor_Kill(&self->actor);
        return;
    }

    FUN_003717ac(&self->skelAnime, EnMd_sAnimationInfo, 0);
    Actor_SetScale(&self->actor, 0.01f);
    self->actor.targetMode = 6;
    s2->alpha = 255;
    Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, 0x18, self->actor.world.pos.x, self->actor.world.pos.y,
                       self->actor.world.pos.z, 0, 0, 0, 3);

    if (((play->sceneNum == 0x55) && !(gSaveContext.eventChkInf[0] & 0x10)) ||
        ((play->sceneNum == 0x55) && (gSaveContext.eventChkInf[0] & 0x10) && CHECK_QUEST_ITEM(18)) ||
        ((play->sceneNum == 0x5B) && !(gSaveContext.eventChkInf[0] & 0x400))) {
        if (LINK_IS_ADULT) {
            self->collider.dim.height *= 1.5f;
        }
        self->actor.home.pos = self->actor.world.pos;
        self->actionFunc = (EnMdActionFunc)FUN_00105bd0;
        return;
    }

    if (play->sceneNum != 0x28) {
        if (((s16)self->actor.params & 0xFF00) != 0xFF00) {
            Path* path = &play->setupPathList[(self->actor.params & 0xFF00) >> 8];
            Vec3s* lastPointPos = path->points;

            lastPointPos += path->count - 1;
            self->actor.world.pos.x = lastPointPos->x;
            self->actor.world.pos.y = lastPointPos->y;
            self->actor.world.pos.z = lastPointPos->z;
        }
    }

    self->actionFunc = (EnMdActionFunc)FUN_00370cf4;
}

static inline void EnMd_ReverseAnimation(EnMd* self) {
    f32 startFrame;
    SkelAnime* sa = &self->skelAnime;

    startFrame = sa->startFrame;
    sa->startFrame = sa->endFrame;
    sa->curFrame = sa->endFrame;
    sa->endFrame = startFrame;
    sa->playSpeed = -1.0f;
}

static inline void EnMd_UpdateAnimSequence_IdleToHalt(EnMd* self) {
    switch (self->animSequenceEntry) {
        case 0:
            FUN_003717ac(&self->skelAnime, EnMd_sAnimationInfo, 2);

            self->animSequenceEntry++;
        case 1:
            if (Animation_OnFrameImpl(&self->skelAnime, self->skelAnime.endFrame, 1.0f)) {
                FUN_003717ac(&self->skelAnime, EnMd_sAnimationInfo, 3);
                self->animSequenceEntry++;
            }
            break;
    }
}

static inline void EnMd_UpdateAnimSequence_HaltToCurious(EnMd* self) {
    switch (self->animSequenceEntry) {
        case 0:
            FUN_003717ac(&self->skelAnime, EnMd_sAnimationInfo, 4);

            self->animSequenceEntry++;
        case 1:
            if (Animation_OnFrameImpl(&self->skelAnime, self->skelAnime.endFrame, 1.0f)) {
                FUN_003717ac(&self->skelAnime, EnMd_sAnimationInfo, 5);
                self->animSequenceEntry++;
            }
            break;
    }
}

static inline void EnMd_UpdateAnimSequence_WalkAway(EnMd* self) {
    switch (self->animSequenceEntry) {
        case 0:
            FUN_003717ac(&self->skelAnime, EnMd_sAnimationInfo, 7);

            self->animSequenceEntry++;
        case 1:
            if (Animation_OnFrameImpl(&self->skelAnime, self->skelAnime.endFrame, 1.0f)) {
                FUN_003717ac(&self->skelAnime, EnMd_sAnimationInfo, 8);
                self->animSequenceEntry++;
            }
            break;
    }
}

static inline void EnMd_UpdateAnimSequence_TwitchIdleUnused(EnMd* self) {
    switch (self->animSequenceEntry) {
        case 0:
            FUN_003717ac(&self->skelAnime, EnMd_sAnimationInfo, 7);

            self->animSequenceEntry++;
        case 1:
            if (Animation_OnFrameImpl(&self->skelAnime, self->skelAnime.endFrame, 1.0f)) {
                FUN_003717ac(&self->skelAnime, EnMd_sAnimationInfo, 10);
                self->animSequenceEntry++;
            }
            break;
    }
}

static inline void EnMd_UpdateAnimSequence_HaltToIdle(EnMd* self) {
    switch (self->animSequenceEntry) {
        case 0:
            FUN_003717ac(&self->skelAnime, EnMd_sAnimationInfo, 2);
            EnMd_ReverseAnimation(self);
            self->animSequenceEntry++;
        case 1:
            if (Animation_OnFrameImpl(&self->skelAnime, self->skelAnime.endFrame, 1.0f)) {
                FUN_003717ac(&self->skelAnime, EnMd_sAnimationInfo, 10);
                self->animSequenceEntry++;
            }
            break;
    }
}

static inline void EnMd_UpdateAnimSequence_SurpriseToAnnoyed(EnMd* self) {
    switch (self->animSequenceEntry) {
        case 0:
            FUN_003717ac(&self->skelAnime, EnMd_sAnimationInfo, 9);

            self->animSequenceEntry++;
        case 1:
            if (Animation_OnFrameImpl(&self->skelAnime, self->skelAnime.endFrame, 1.0f)) {
                FUN_003717ac(&self->skelAnime, EnMd_sAnimationInfo, 6);
                self->animSequenceEntry++;
            }
            break;
    }
}

static inline void EnMd_UpdateAnimSequence_SurpriseToIdle(EnMd* self) {
    switch (self->animSequenceEntry) {
        case 0:
            FUN_003717ac(&self->skelAnime, EnMd_sAnimationInfo, 9);
            EnMd_ReverseAnimation(self);
            self->animSequenceEntry++;
        case 1:
            if (Animation_OnFrameImpl(&self->skelAnime, self->skelAnime.endFrame, 1.0f)) {
                FUN_003717ac(&self->skelAnime, EnMd_sAnimationInfo, 10);
                self->animSequenceEntry++;
            }
            break;
    }
}

static inline void EnMd_UpdateAnimSequence_CuriousToAnnoyed(EnMd* self) {
    switch (self->animSequenceEntry) {
        case 0:
            FUN_003717ac(&self->skelAnime, EnMd_sAnimationInfo, 11);

            self->animSequenceEntry++;
        case 1:
            if (Animation_OnFrameImpl(&self->skelAnime, self->skelAnime.endFrame, 1.0f)) {
                FUN_003717ac(&self->skelAnime, EnMd_sAnimationInfo, 6);
                self->animSequenceEntry++;
            }
            break;
    }
}

static inline void EnMd_UpdateAnimSequence_AnnoyedToHalt(EnMd* self) {
    switch (self->animSequenceEntry) {
        case 0:
            FUN_003717ac(&self->skelAnime, EnMd_sAnimationInfo, 12);

            self->animSequenceEntry++;
        case 1:
            if (Animation_OnFrameImpl(&self->skelAnime, self->skelAnime.endFrame, 1.0f)) {
                FUN_003717ac(&self->skelAnime, EnMd_sAnimationInfo, 3);
                self->animSequenceEntry++;
            }
            break;
    }
}

static inline void EnMd_UpdateAnimSequence_IdleToAnnoyed(EnMd* self) {
    switch (self->animSequenceEntry) {
        case 0:
            FUN_003717ac(&self->skelAnime, EnMd_sAnimationInfo, 13);

            self->animSequenceEntry++;
        case 1:
            if (Animation_OnFrameImpl(&self->skelAnime, self->skelAnime.endFrame, 1.0f)) {
                FUN_003717ac(&self->skelAnime, EnMd_sAnimationInfo, 6);
                self->animSequenceEntry++;
            }
            break;
    }
}

static inline void EnMd_UpdateAnimSequence_StopWalking(EnMd* self) {
    switch (self->animSequenceEntry) {
        case 0:
            FUN_003717ac(&self->skelAnime, EnMd_sAnimationInfo, 7);
            EnMd_ReverseAnimation(self);
            self->animSequenceEntry++;
        case 1:
            if (Animation_OnFrameImpl(&self->skelAnime, self->skelAnime.endFrame, 1.0f)) {
                FUN_003717ac(&self->skelAnime, EnMd_sAnimationInfo, 10);
                self->animSequenceEntry++;
            }
            break;
    }
}

void FUN_00370818(EnMd* self) {
    switch (self->animSequence) {
        case 1:
            EnMd_UpdateAnimSequence_IdleToHalt(self);
            break;
        case 2:
            EnMd_UpdateAnimSequence_HaltToCurious(self);
            break;
        case 3:
            EnMd_UpdateAnimSequence_WalkAway(self);
            break;
        case 4:
            EnMd_UpdateAnimSequence_TwitchIdleUnused(self);
            break;
        case 5:
            EnMd_UpdateAnimSequence_HaltToIdle(self);
            break;
        case 6:
            EnMd_UpdateAnimSequence_SurpriseToAnnoyed(self);
            break;
        case 7:
            EnMd_UpdateAnimSequence_SurpriseToIdle(self);
            break;
        case 8:
            EnMd_UpdateAnimSequence_CuriousToAnnoyed(self);
            break;
        case 9:
            EnMd_UpdateAnimSequence_AnnoyedToHalt(self);
            break;
        case 10:
            EnMd_UpdateAnimSequence_IdleToAnnoyed(self);
            break;
        case 11:
            EnMd_UpdateAnimSequence_StopWalking(self);
    }
}

// Draft (FUN_00370d60 = EnMd_UpdateAnimSequence_WithTalking; far: ~35 insns off, case block layout / const regs differ; GLOBAL_ASM stays in functions_370000s.cpp):
#if 0
static inline void EnMd_SetAnimSequence(EnMd* self, u8 state) {
    self->animSequence = state;
    self->animSequenceEntry = 0;
}

void FUN_00370d60(EnMd* self) {
    if (self->interactInfo.talkState != 0) {
        switch (self->actor.textId) {
            case 0x102F:
                if ((self->messageEntry == 0) && (self->animSequence != 1)) {
                    EnMd_SetAnimSequence(self, 1);
                }
                if ((self->messageEntry == 2) && (self->animSequence != 2)) {
                    EnMd_SetAnimSequence(self, 2);
                }
                if ((self->messageEntry == 5) && (self->animSequence != 8)) {
                    EnMd_SetAnimSequence(self, 8);
                }
                if ((self->messageEntry == 11) && (self->animSequence != 9)) {
                    EnMd_SetAnimSequence(self, 9);
                }
                break;
            case 0x1046:
                if ((self->messageEntry == 0) && (self->animSequence != 6)) {
                    EnMd_SetAnimSequence(self, 6);
                }
                break;
            case 0x1030:
            case 0x1034:
            case 0x1045:
                if ((self->messageEntry == 0) && (self->animSequence != 1)) {
                    EnMd_SetAnimSequence(self, 1);
                }
                break;
            case 0x1033:
                if ((self->messageEntry == 0) && (self->animSequence != 1)) {
                    EnMd_SetAnimSequence(self, 1);
                }
                if ((self->messageEntry == 1) && (self->animSequence != 2)) {
                    EnMd_SetAnimSequence(self, 2);
                }
                if ((self->messageEntry == 5) && (self->animSequence != 10)) {
                    EnMd_SetAnimSequence(self, 10);
                }
                if ((self->messageEntry == 7) && (self->animSequence != 9)) {
                    EnMd_SetAnimSequence(self, 9);
                }
                break;
        }
    } else if (self->skelAnime.animationType != 0) {
        FUN_003717ac(&self->skelAnime, EnMd_sAnimationInfo, 10);
        EnMd_SetAnimSequence(self, 0);
    }

    FUN_00370818(self);
}
#endif

}
