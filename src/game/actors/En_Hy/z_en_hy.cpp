extern "C" {
#include "z_en_hy.hpp"

#define FLAGS 0x00000019

#define ENHY_GET_TYPE(actor) ((actor)->params & 0x7F)

typedef struct {
    /* 0x00 */ s16 objectId;
    /* 0x02 */ char unk_02[0x2];
    /* 0x04 */ s32 cmbIndex;
    /* 0x08 */ s32 csabIndex;
    /* 0x0C */ s32 limbCount;
    /* 0x10 */ s16 faceAnimIndex;
    /* 0x12 */ s16 eyeCount;
    /* 0x14 */ s16 headLimb;
    /* 0x16 */ s16 torsoLimb;
} EnHyTypeInfo; // size = 0x18

typedef struct {
    /* 0x00 */ Vec3s offset;
    /* 0x06 */ s16 radius;
    /* 0x08 */ s16 height;
} EnHyColliderInfo; // size = 0xA

typedef struct {
    /* 0x00 */ u8 presetIndex;
    /* 0x04 */ f32 childYOffset;
    /* 0x08 */ f32 adultYOffset;
} EnHyTrackingInfo; // size = 0xC

typedef struct {
    /* 0x00 */ f32 shadowScale;
    /* 0x04 */ Vec3f modelOffset;
    /* 0x10 */ f32 scale;
    /* 0x14 */ u8 targetMode;
    /* 0x18 */ f32 interactRange;
} EnHyInit2Info; // size = 0x1C

typedef struct {
    /* 0x00 */ s8 showMesh[2];
    /* 0x02 */ s8 colorIndex[2];
    /* 0x04 */ f32 envColor0[4];
    /* 0x14 */ f32 envColor1[4];
    /* 0x24 */ u8 animInfoIndex;
} EnHyModelInfo; // size = 0x28

typedef struct {
    f32 v[4];
} EnHyColor;

typedef struct {
    s16 v[4];
} __attribute__((aligned(4))) EnHyRewards;

typedef struct {
    /* 0x00 */ s16 items[4];
    /* 0x08 */ EnHyRewards rewards;
} EnHyBeggarData; // size = 0x10

extern EnHyBeggarData EnHy_sBeggarData;
extern EnHyColor EnHy_sWhiteColor;
extern EnHyTypeInfo EnHy_sTypeInfo[];
extern EnHyColliderInfo EnHy_sColliderInfo[];
extern EnHyTrackingInfo EnHy_sPlayerTrackingInfo[];
extern EnHyInit2Info EnHy_sInit2Info[];
extern EnHyModelInfo EnHy_sModelInfo[];
extern ColliderCylinderInit EnHy_sColliderCylinderInit;
s32 EnHy_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void EnHy_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
extern CollisionCheckInfoInit2 EnHy_sColChkInfoInit;
extern struct_80034EC0_Entry EnHy_sAnimationInfo[];

void FUN_0034e01c(MTX34* mtx, Vec3s* rot);
void FUN_0036932c(SkeletonAnimationModel* model, s32 index);
void FUN_0037266c(SkeletonAnimationModel* model, s32 index);
void FUN_00357a50(SkelAnime* skelAnime, s32 index, s32 count, f32* color, s32 arg4);
void Matrix_UpdatePosition(MTX34* dst, MTX34* src, Vec3f* translate);
void FUN_00372244(void* arg0, s16 arg1, u32 sfxId);
void FUN_0035c528(u32 seqId);
void Rupees_ChangeBy(s16 amount);
void Player_UpdateBottleHeld(PlayState* play, Player* player, s32 item, s32 itemAction);
void FUN_0034c998(Player* player);
void FUN_00150830(EnHy* self, PlayState* play);
s32 Object_IsLoaded(ObjectContext* objectCtx, s32 slot);
Path* Path_GetByIndex(PlayState* play, s32 index, s32 max);
void FUN_0034c664(Actor* actor, EnHyInteractInfo* info, s32 presetIndex, s32 trackingMode);
s32 FUN_00342714(PlayState* play, Actor* actor, void* talkState, f32 range, void* getTextId, void* updateTalkState);
s32 FUN_00341e1c(s32 item);
s32 Player_GetMask(PlayState* play);
void EnHy_DoNothing(EnHy* self, PlayState* play);
void FUN_003ac640(EnHy* self, PlayState* play);
u16 FUN_00170c44(PlayState* play, Actor* thisx);
s16 FUN_0017079c(PlayState* play, Actor* thisx);
void FUN_001c4074(EnHy* self, PlayState* play);
void FUN_00116870(EnHy* self, PlayState* play);
void FUN_003c4344(EnHy* self, PlayState* play);
void FUN_003c3840(EnHy* self, PlayState* play);

void EnHy_Init(Actor* thisx, PlayState* play);
void EnHy_Destroy(Actor* thisx, PlayState* play);
void EnHy_Update(Actor* thisx, GameState* state);
void EnHy_Draw(Actor* thisx, GameState* state);

GLOBAL_ASM("data/z_En_Hy.data.s")
GLOBAL_ASM("../src/game/actors/En_Hy/data_syms.s")

ActorInit En_Hy_InitVars = {
    ACTOR_EN_HY,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnHy),
    (ActorFunc)EnHy_Init,
    (ActorFunc)EnHy_Destroy,
    (ActorFunc)EnHy_Update,
    (ActorFunc)EnHy_Draw,
};

s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);

static inline s32 EnHy_ShouldSpawn(EnHy* self, PlayState* play) {
    switch (play->sceneNum) {
        case 0x52:
            if (ENHY_GET_TYPE(&self->actor) == 9 || ENHY_GET_TYPE(&self->actor) == 10 ||
                ENHY_GET_TYPE(&self->actor) == 12 || ENHY_GET_TYPE(&self->actor) == 2 ||
                ENHY_GET_TYPE(&self->actor) == 7) {
                if (gSaveContext.linkAge == 0) {
                    if (ENHY_GET_TYPE(&self->actor) != 12) {
                        if (gSaveContext.nightFlag == 1) {
                            return 0;
                        }
                    }
                }
            }
            return 1;
        case 0x37:
            if (ENHY_GET_TYPE(&self->actor) == 10 && (gSaveContext.linkAge == 1 || gSaveContext.nightFlag == 0)) {
                return 0;
            }
            return 1;
        case 0x35:
            if (ENHY_GET_TYPE(&self->actor) == 0 && gSaveContext.nightFlag == 0) {
                return 0;
            }
            return 1;
        case 0x2A:
            if (ENHY_GET_TYPE(&self->actor) == 0) {
                s32 adult = gSaveContext.linkAge == 0;
                if (!adult) {
                    return 0;
                }
            } else if (ENHY_GET_TYPE(&self->actor) == 9 || ENHY_GET_TYPE(&self->actor) == 2 ||
                       ENHY_GET_TYPE(&self->actor) == 7) {
                if (gSaveContext.nightFlag == 0 || gSaveContext.linkAge == 1) {
                    return 0;
                }
            }
            return 1;
        case 0x1E:
        case 0x1F:
            if (ENHY_GET_TYPE(&self->actor) == 14) {
                if (gSaveContext.nightFlag == 1) {
                    return 0;
                }
                if ((gSaveContext.eventChkInf[8] & 1) && !(gSaveContext.eventChkInf[4] & 0x20)) {
                    return 0;
                }
            }
            return 1;
        default:
            if ((ENHY_GET_TYPE(&self->actor) == 19 || ENHY_GET_TYPE(&self->actor) == 20) &&
                gSaveContext.linkAge == 0) {
                return 0;
            }
            return 1;
    }
}

void EnHy_Init(Actor* thisx, PlayState* play) {
    EnHy* self = (EnHy*)thisx;

    self->unk_107C = 1;
    if (ENHY_GET_TYPE(thisx) >= 0x15 ||
        (self->unk_D90 = Object_GetIndex(&play->objectCtx, EnHy_sTypeInfo[ENHY_GET_TYPE(thisx)].objectId)) < 0) {
        Actor_Kill(thisx);
    }

    if (!EnHy_ShouldSpawn(self, play)) {
        Actor_Kill(thisx);
    }

    self->actionFunc = FUN_003ac640;
}

void EnHy_Destroy(Actor* thisx, PlayState* play) {
    EnHy* self = (EnHy*)thisx;

    Collider_DestroyCylinder(play, &self->collider);
    FUN_003508b8(thisx, self->unk_EAC, 0);
    SkelAnime_Free2(&self->skelAnime);
    self->faceAnim.Destroy();
}

static inline void EnHy_InitSetProperties(EnHy* self) {
    u8 type = ENHY_GET_TYPE(&self->actor);

    self->actor.shape.shadowScale = EnHy_sInit2Info[type].shadowScale;
    Actor_SetScale(&self->actor, EnHy_sInit2Info[type].scale);
    self->actor.targetMode = EnHy_sInit2Info[type].targetMode;
    Vec3f* dst = &self->modelOffset;
    dst->x = EnHy_sInit2Info[type].modelOffset.x;
    dst->y = EnHy_sInit2Info[type].modelOffset.y;
    dst->z = EnHy_sInit2Info[type].modelOffset.z;
    self->interactRange = EnHy_sInit2Info[type].interactRange;
    self->interactRange += self->collider.dim.radius;
}

void FUN_003ac640(EnHy* self, PlayState* play) {
    EnHy* s2 = self;
    PlayState* play2 = play;
    if (Object_IsLoaded(&play->objectCtx, self->unk_D90)) {
        self->actor.objBankIndex = self->unk_D90;
        EnHyTypeInfo* typeInfo = &EnHy_sTypeInfo[ENHY_GET_TYPE(&self->actor)];
        s32 cmbIndex = typeInfo->cmbIndex;
        s32 csabIndex = typeInfo->csabIndex;
        s32 limbCount = typeInfo->limbCount;
        s16 faceAnimIndex = typeInfo->faceAnimIndex;

        FUN_00372f38(&self->actor, play, &self->unk_EAC, cmbIndex, 0);
        FUN_00353c9c(&self->actor, play, &self->skelAnime, cmbIndex, csabIndex, self->jointTable, self->morphTable,
                     limbCount);
        if (faceAnimIndex >= 0) {
            SkelAnime_SetFaceAnimations(&self->faceAnim, &self->skelAnime, faceAnimIndex, -1, -1);
        }
        ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 0.0f);
        Collider_InitCylinder(play, &self->collider);
        Collider_SetCylinder(play, &self->collider, &self->actor, &EnHy_sColliderCylinderInit);
        self->collider.dim.radius = EnHy_sColliderInfo[ENHY_GET_TYPE(&s2->actor)].radius;
        self->collider.dim.height = EnHy_sColliderInfo[ENHY_GET_TYPE(&self->actor)].height;
        CollisionCheck_SetInfo2(&self->actor.colChkInfo, NULL, &EnHy_sColChkInfoInit);
        {
            EnHyModelInfo* mi = &EnHy_sModelInfo[ENHY_GET_TYPE(&self->actor)];
            FUN_003717ac(&self->skelAnime, (struct_80034EC0_Entry*)((u8*)EnHy_sModelInfo - 0x288), mi->animInfoIndex);
        }

        if (play->sceneNum == 0x1E || play->sceneNum == 0x20) {
            self->actor.flags &= ~0x10;
            self->actor.uncullZoneScale = 40.0f;
            s32 type = ENHY_GET_TYPE(&self->actor);
            if (type != 0 && type == 3) {
                self->actor.uncullZoneScale = 50.0f;
            }
        }

        if (play->sceneNum == 0x2A) {
            self->talonEventChkInf = gSaveContext.eventChkInf[6];
        }

        EnHy_InitSetProperties(self);
        self->path = Path_GetByIndex(play2, (self->actor.params & 0x780) >> 7, 0xF);

        switch (ENHY_GET_TYPE(&self->actor)) {
            case 3:
                if (self->path != NULL) {
                    self->actor.speedXZ = 3.0f;
                }
                self->actionFunc = FUN_001c4074;
                break;

            case 7:
                self->pathReverse = 0;
                self->actionFunc = FUN_00116870;
                break;

            case 0:
                if (play->sceneNum == 0x20) {
                    self->actionFunc = FUN_003c4344;
                    break;
                }
                // fallthrough
            case 1:
            case 2:
            case 4:
            case 6:
            case 8:
            case 13:
            case 14:
            case 15:
            case 16:
            case 17:
            case 18:
            case 19:
            case 20:
                self->actionFunc = FUN_003c3840;
                break;

            case 5:
            case 9:
            case 10:
            case 11:
            case 12:
                self->actionFunc = EnHy_DoNothing;
                break;

            default:
                Actor_Kill(&self->actor);
                break;
        }
    }
}

GLOBAL_ASM("asm/EnHy_Update.s")

#if 0 // draft: 99 words off, see STATUS.md
static void EnHy_UpdateEyes(EnHy* self) {
    if (DECR(self->nextEyeIndexTimer) == 0) {
        s16 eyeCount = EnHy_sTypeInfo[ENHY_GET_TYPE(&self->actor)].eyeCount;

        if (eyeCount != 0) {
            self->curEyeIndex++;
            if (self->curEyeIndex >= eyeCount) {
                self->nextEyeIndexTimer = Rand_S16Offset_003702c8(30, 30);
                self->curEyeIndex = 0;
            }
        }
    }
}

static void EnHy_OfferBuyBottledItem(EnHy* self, PlayState* play) {
    PlayState* play2 = play;
    Player* player = GET_PLAYER(play);

    if (ENHY_GET_TYPE(&self->actor) == 5) {
        if (!FUN_00341e1c(0x1C) && !FUN_00341e1c(0x1D) && !FUN_00341e1c(0x19)) {
            switch (FUN_0036bc84(play)) {
                case 0x1B:
                case 0x1C:
                case 0x1D:
                    self->actor.textId = 0x70EF;
                    break;

                default:
                    if (Player_GetMask(play2) == 0) {
                        self->actor.textId = 0x70ED;
                    }
                    break;
            }
        } else {
            switch (FUN_0036bc84(play)) {
                case 0x19:
                    self->actor.textId = 0x70F0;
                    break;

                case 0x18:
                    self->actor.textId = 0x70F1;
                    break;

                case 0x1A:
                    self->actor.textId = 0x70F2;
                    break;

                case 0x1B:
                case 0x1C:
                case 0x1D:
                    self->actor.textId = 0x70EF;
                    break;

                default:
                    if (Player_GetMask(play) == 0) {
                        self->actor.textId = 0x700C;
                    }
                    break;
            }
        }

        player->actor.textId = self->actor.textId;
    }
}

static void EnHy_UpdateNPC(EnHy* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 trackingMode;

    switch (ENHY_GET_TYPE(&self->actor)) {
        case 3:
        case 7:
        case 9:
        case 10:
            trackingMode = (self->interactInfo.talkState == 0) ? 1 : 2;
            break;

        case 12:
            trackingMode = 1;
            break;

        case 2:
        case 17:
            trackingMode = 4;
            break;

        case 0:
        case 18:
            trackingMode = (self->interactInfo.talkState == 0) ? 2 : 4;
            break;

        default:
            trackingMode = 2;
            break;
    }

    self->interactInfo.trackPos = player->actor.world.pos;

    if (gSaveContext.linkAge == 0) {
        self->interactInfo.yOffset = EnHy_sPlayerTrackingInfo[ENHY_GET_TYPE(&self->actor)].adultYOffset;
    } else {
        self->interactInfo.yOffset = EnHy_sPlayerTrackingInfo[ENHY_GET_TYPE(&self->actor)].childYOffset;
    }

    FUN_0034c664(&self->actor, &self->interactInfo, EnHy_sPlayerTrackingInfo[ENHY_GET_TYPE(&self->actor)].presetIndex,
                 trackingMode);

    if (FUN_00342714(play, &self->actor, &self->interactInfo.talkState, self->interactRange, (void*)FUN_00170c44,
                     (void*)FUN_0017079c)) {
        EnHy_OfferBuyBottledItem(self, play);
    }
}

static void EnHy_UpdateCollider(EnHy* self, PlayState* play) {
    EnHyColliderInfo* info = &EnHy_sColliderInfo[ENHY_GET_TYPE(&self->actor)];

    f32 x = self->actor.world.pos.x + info->offset.x;
    f32 y = self->actor.world.pos.y + info->offset.y;
    f32 z = self->actor.world.pos.z + info->offset.z;
    self->collider.dim.pos.x = x;
    self->collider.dim.pos.y = y;
    self->collider.dim.pos.z = z;
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
}

void EnHy_Update(Actor* thisx, GameState* state) {
    EnHy* self = (EnHy*)thisx;
    PlayState* play = (PlayState*)state;

    if (self->actionFunc != FUN_003ac640) {
        FUN_003731e0(&self->skelAnime);
        EnHy_UpdateEyes(self);

        if (self->interactInfo.talkState == 0) {
            Actor_MoveForward(&self->actor);
        }

        if (self->unk_107C != 0) {
            Actor_UpdateBgCheckInfo(play, &self->actor, 0.0f, 0.0f, 0.0f, 4);
            if (play->sceneNum == 0x20) {
                self->unk_107C = 0;
            }
        }
    }

    self->actionFunc(self, play);
    EnHy_UpdateNPC(self, play);
    EnHy_UpdateCollider(self, play);
}
#endif






static inline void EnHy_ShowMesh(EnHy* self, s32 idx) {
    if (idx >= 0) {
        FUN_0037266c(self->skelAnime.unk_28, idx);
    }
}

static inline void EnHyFace_Set(FaceAnimations* face, s32 a, s32 b) {
    face->FUN_0035e3a4(a, b);
}

void EnHy_Draw(Actor* thisx, GameState* state) {
    EnHy* self = (EnHy*)thisx;
    EnHy* s2 = self;
    if (s2->actionFunc != FUN_003ac640) {
        GraphicsContext_UnknownStub(state->gfxCtx);
        {
            MTX34* mtx = &self->actor.modelMtx;
            Vector3f offset(self->modelOffset.x, self->modelOffset.y, self->modelOffset.z);
            Matrix_UpdatePosition(mtx, mtx, (Vec3f*)&offset);
        }

        EnHyModelInfo* modelInfo = (EnHyModelInfo*)((u8*)EnHy_sTypeInfo - 0x348);
        s32 type = ENHY_GET_TYPE(thisx);

        switch (EnHy_sTypeInfo[type].objectId) {
            case 0x111:
                FUN_0036932c(self->skelAnime.unk_28, 8);
                FUN_0036932c(self->skelAnime.unk_28, 1);
                FUN_0036932c(self->skelAnime.unk_28, 2);
                FUN_0036932c(self->skelAnime.unk_28, 3);
                FUN_0036932c(self->skelAnime.unk_28, 4);
                FUN_0036932c(self->skelAnime.unk_28, 5);
                FUN_0036932c(self->skelAnime.unk_28, 6);
                FUN_0036932c(self->skelAnime.unk_28, 7);
                if (modelInfo[type].showMesh[0] >= 0) {
                    FUN_0037266c(self->skelAnime.unk_28, modelInfo[type].showMesh[0]);
                }
                EnHy_ShowMesh(self, modelInfo[type].showMesh[1]);
                break;
            case 0x107:
                FUN_0036932c(self->skelAnime.unk_28, 1);
                FUN_0036932c(self->skelAnime.unk_28, 2);
                FUN_0036932c(self->skelAnime.unk_28, 3);
                FUN_0036932c(self->skelAnime.unk_28, 4);
                if (modelInfo[type].showMesh[0] >= 0) {
                    FUN_0037266c(self->skelAnime.unk_28, modelInfo[type].showMesh[0]);
                }
                break;

            case 0x10C:
                FUN_0036932c(self->skelAnime.unk_28, 1);
                FUN_0036932c(self->skelAnime.unk_28, 2);
                if (modelInfo[type].showMesh[0] >= 0) {
                    FUN_0037266c(self->skelAnime.unk_28, modelInfo[type].showMesh[0]);
                }
                break;

            case 0x108:
                FUN_0036932c(self->skelAnime.unk_28, 2);
                FUN_0036932c(self->skelAnime.unk_28, 3);
                if (modelInfo[type].showMesh[0] >= 0) {
                    FUN_0037266c(self->skelAnime.unk_28, modelInfo[type].showMesh[0]);
                }
                break;

        }

        switch (type) {
            case 2:
            case 3:
            case 4:
            case 5:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 16:
            case 17:
            case 19:
            case 20:
                if (modelInfo[type].colorIndex[0] >= 0) {
                    FUN_00357a50(&self->skelAnime, modelInfo[type].colorIndex[0], 4,
                                 modelInfo[type].envColor0, 1);
                }
                if (modelInfo[type].colorIndex[1] >= 0) {
                    FUN_00357a50(&self->skelAnime, modelInfo[type].colorIndex[1], 3,
                                 modelInfo[type].envColor1, 1);
                }
                if (type == 8) {
                    FUN_00357a50(&self->skelAnime, 2, 2, EnHy_sModelInfo[8].envColor0, 1);
                } else if (type == 11) {
                    EnHyColor white = EnHy_sWhiteColor;

                    FUN_00357a50(&self->skelAnime, 2, 2, white.v, 1);
                }
                break;
        }

        EnHyFace_Set(&self->faceAnim, 0, self->curEyeIndex);
        SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, EnHy_OverrideLimbDraw, EnHy_PostLimbDraw,
                          thisx, 0);
        self->faceAnim.FUN_0035e330();
    }
}


void EnHy_DoNothing(EnHy* self, PlayState* play) {
}

s32 EnHy_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnHy* self = (EnHy*)thisx;
    s16 headLimb = EnHy_sTypeInfo[ENHY_GET_TYPE(&self->actor)].headLimb;
    s16 torsoLimb = EnHy_sTypeInfo[ENHY_GET_TYPE(&self->actor)].torsoLimb;

    if (limbIndex == headLimb) {
        FUN_0034e01c(mtx, &self->interactInfo.headRot);
    }

    if (limbIndex == torsoLimb) {
        FUN_0034e01c(mtx, &self->interactInfo.torsoRot);
    }

    return 0;
}

GLOBAL_ASM("asm/EnHy_PostLimbDraw.s")

#if 0 // draft (15 words off, see STATUS.md)
static inline void EnHy_MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}

void EnHy_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnHy* self = (EnHy*)thisx;
    Vector3f* src;
    Vector3f* dst;
    Vector3f focusOffset(400.0f, 0.0f, 0.0f);

    if (limbIndex == EnHy_sTypeInfo[ENHY_GET_TYPE(&self->actor)].headLimb) {
        dst = (Vector3f*)&self->actor.focus.pos;
        if (ENHY_GET_TYPE(&self->actor) != 0) {
            src = &focusOffset;
        } else {
            Vector3f focusOffset2(1200.0f, 0.0f, 0.0f);
            src = &focusOffset2;
        }
        EnHy_MultVec3f(src, dst, mtx);
    }
}
#endif

#if 0 // draft: 185 words off; FUN_0017079c stays GLOBAL_ASM in src/functions/functions_140000s.cpp, see STATUS.md
extern __pure float _fabsf(float);
static __inline __pure float EnHy_Fabsf(float f) {
    return _fabsf(f);
}

s16 FUN_0017079c(PlayState* play, Actor* thisx) {
    EnHy* self = (EnHy*)thisx;
    Player* player = GET_PLAYER(play);
    EnHyRewards beggarRewards = EnHy_sBeggarData.rewards;

    switch (Message_GetState(&play->msgCtx)) {
        case 0:
        case 1:
        case 4:
            return 1;

        case 3:
            switch (self->actor.textId) {
                case 0x709E:
                case 0x709F:
                    if (!self->playedSfx) {
                        FUN_00372244(&play->unk_5FCC, 0x1E, self->actor.textId == 0x709F ? 0x01000487 : 0x0100048B);
                        self->playedSfx = 1;
                    }
                    break;

                case 0x70F0:
                case 0x70F1:
                case 0x70F2:
                case 0x70F3:
                    if (self->skelAnime.animationType != 3) {
                        FUN_003717ac(&self->skelAnime, EnHy_sAnimationInfo, 0x17);
                        FUN_0035c528(0x01000590);
                    }
                    break;
            }
            return 1;

        case 2:
            switch (self->actor.textId) {
                case 0x70F0:
                case 0x70F1:
                case 0x70F2:
                case 0x70F3:
                    Rupees_ChangeBy(beggarRewards.v[self->actor.textId - 0x70F0]);
                    FUN_003717ac(&self->skelAnime, EnHy_sAnimationInfo, 0x11);
                    Player_UpdateBottleHeld(play, player, 0x14, 0x1E);
                    FUN_0034c998(player);
                    break;

                case 0x7016:
                    gSaveContext.infTable[12] |= 0x0001;
                    break;

                case 0x7045:
                    gSaveContext.infTable[12] |= 0x0002;
                    break;

                case 0x701B:
                    gSaveContext.infTable[12] |= 0x0004;
                    break;

                case 0x7047:
                    gSaveContext.infTable[12] |= 0x0008;
                    break;

                case 0x70EB:
                    gSaveContext.infTable[12] |= 0x0010;
                    break;

                case 0x7023:
                    gSaveContext.infTable[12] |= 0x0020;
                    break;

                case 0x7021:
                    gSaveContext.infTable[12] |= 0x0040;
                    break;

                case 0x7027:
                    gSaveContext.infTable[12] |= 0x0080;
                    break;

                case 0x701D:
                    gSaveContext.infTable[12] |= 0x0100;
                    break;

                case 0x7048:
                    gSaveContext.infTable[12] |= 0x0200;
                    break;

                case 0x701F:
                    gSaveContext.infTable[12] |= 0x0400;
                    break;

                case 0x7018:
                    gSaveContext.infTable[12] |= 0x2000;
                    break;

                case 0x70A4:
                    if (CHECK_QUEST_ITEM(12)) {
                        gSaveContext.infTable[12] |= 0x1000;
                    }
                    break;

                case 0x5057:
                    gSaveContext.infTable[22] |= 0x0001;
                    break;

                case 0x5059:
                    gSaveContext.infTable[22] |= 0x0002;
                    break;

                case 0x505B:
                    gSaveContext.infTable[22] |= 0x0004;
                    break;

                case 0x505D:
                    gSaveContext.infTable[22] |= 0x0008;
                    break;

                case 0x5060:
                    gSaveContext.infTable[22] |= 0x0010;
                    break;

                case 0x508B:
                    gSaveContext.infTable[12] |= 0x0800;
                    break;

                case 0x709E:
                    *(u16*)((u8*)&gSaveContext + 0x154E) = 0;
                    break;

                case 0x709F:
                    self->getItemId = (gSaveContext.infTable[25] & 0x0002) ? 0x4D : 0x3E;
                    GiveItem(&self->actor, play, self->getItemId, self->actor.xzDistToPlayer + 1.0f,
                             EnHy_Fabsf(self->actor.yDistToPlayer) + 1.0f);
                    self->actionFunc = FUN_00150830;
                    break;
            }
            return 0;

        case 5:
            if (!Message_ShouldAdvance(play)) {
                return 1;
            }
            return 2;

        default:
            return 1;
    }
}
#endif

u16 FUN_00170c44(PlayState* play, Actor* thisx) {
    Player* player = GET_PLAYER(play);
    EnHy* self = (EnHy*)thisx;
    u16 textId = Text_GetFaceReaction(play, ENHY_GET_TYPE(thisx) + 0x25);

    if (textId != 0) {
        if (ENHY_GET_TYPE(&self->actor) == 5) {
            player->exchangeItemId = 0x19;
        }
        return textId;
    }

    switch (ENHY_GET_TYPE(&self->actor)) {
        case 0:
            if (play->sceneNum == 0x2A) {
                return (self->talonEventChkInf & 0x0800) ? 0x508D : ((gSaveContext.infTable[12] & 0x0800) ? 0x508C : 0x508B);
            } else if (play->sceneNum == 0x20) {
                return (gSaveContext.eventInf[3] & 1) ? 0x709B : 0x709C;
            } else if (*((u8*)&gSaveContext + 0x15AA)) {
                s16 followingDog = (*(u16*)((u8*)&gSaveContext + 0x154E) & 0x0F00) >> 8;

                if (followingDog != 0) {
                    self->playedSfx = 0;
                    return (followingDog == 1) ? 0x709F : 0x709E;
                } else {
                    return 0x709D;
                }
            } else {
                return 0x70A0;
            }

        case 1:
            if (gSaveContext.eventChkInf[8] & 1) {
                return (gSaveContext.infTable[12] & 0x0002) ? 0x7017 : 0x7045;
            } else {
                return (gSaveContext.infTable[12] & 0x0001) ? 0x7017 : 0x7016;
            }

        case 2:
            if (play->sceneNum == 0x2A) {
                return 0x5086;
            } else if (play->sceneNum == 0x52) {
                return 0x5085;
            } else if (gSaveContext.eventChkInf[8] & 1) {
                return (gSaveContext.infTable[12] & 0x0008) ? 0x701A : 0x7047;
            } else if (gSaveContext.eventChkInf[1] & 0x0010) {
                return 0x701A;
            } else if (gSaveContext.eventChkInf[1] & 0x0001) {
                return 0x701B;
            } else if (gSaveContext.infTable[12] & 0x0004) {
                return 0x701C;
            } else {
                return 0x701A;
            }

        case 3:
            return (gSaveContext.eventChkInf[8] & 1) ? ((gSaveContext.infTable[12] & 0x0010) ? 0x7001 : 0x70EB) : 0x7001;

        case 4:
            return (gSaveContext.eventChkInf[8] & 1) ? 0x704B : ((gSaveContext.infTable[12] & 0x0020) ? 0x7024 : 0x7023);

        case 5:
            player->exchangeItemId = 0x19;
            return 0x700C;

        case 6:
            return (gSaveContext.eventChkInf[8] & 1) ? 0x704A : ((gSaveContext.infTable[12] & 0x0040) ? 0x7022 : 0x7021);

        case 7:
            if (play->sceneNum == 0x2A) {
                return 0x5088;
            } else if (play->sceneNum == 0x52) {
                return 0x5087;
            } else {
                return (gSaveContext.eventChkInf[8] & 1) ? 0x704D : ((gSaveContext.infTable[12] & 0x0080) ? 0x7028 : 0x7027);
            }

        case 8:
            if (gSaveContext.eventChkInf[8] & 1) {
                return (gSaveContext.infTable[12] & 0x0200) ? 0x701E : 0x7048;
            } else {
                return (gSaveContext.infTable[12] & 0x0100) ? 0x701E : 0x701D;
            }

        case 9:
            if (play->sceneNum == 0x2A) {
                return (gSaveContext.eventChkInf[10] & 0x0400) ? 0x5082 : 0x5081;
            } else if (play->sceneNum == 0x52) {
                return CHECK_QUEST_ITEM(4) ? 0x5080 : 0x507F;
            } else {
                return (gSaveContext.eventChkInf[8] & 1) ? 0x7049 : ((gSaveContext.infTable[12] & 0x0400) ? 0x7020 : 0x701F);
            }

        case 10:
            if (play->sceneNum == 0x37) {
                return (gSaveContext.eventChkInf[10] & 0x0400) ? 0x507E : 0x507D;
            } else if (play->sceneNum == 0x52) {
                return CHECK_QUEST_ITEM(4) ? 0x507C : 0x507B;
            } else {
                return (gSaveContext.eventChkInf[8] & 1) ? 0x7046 : ((gSaveContext.infTable[12] & 0x2000) ? 0x7019 : 0x7018);
            }

        case 11:
            return (gSaveContext.infTable[8] & 0x0800) ? ((gSaveContext.eventChkInf[1] & 0x0010) ? 0x7014 : 0x70A4) : 0x7014;

        case 12:
            if ((u16)play->sceneNum == 0x52) {
                return gSaveContext.nightFlag ? 0x5084 : 0x5083;
            } else {
                return (gSaveContext.eventChkInf[8] & 1) ? 0x7044 : 0x7015;
            }

        case 13:
            return 0x7055;

        case 14:
            return 0x7089;

        case 15:
            return 0x708A;

        case 16:
            return 0x700E;

        case 17:
            if (gSaveContext.linkAge != 0) {
                if (!gSaveContext.nightFlag) {
                    return (gSaveContext.infTable[22] & 0x0001) ? 0x5058 : 0x5057;
                } else {
                    return (gSaveContext.infTable[22] & 0x0002) ? 0x505A : 0x5059;
                }
            } else if (!gSaveContext.nightFlag) {
                return (gSaveContext.infTable[22] & 0x0004) ? 0x505C : 0x505B;
            } else {
                return 0x5058;
            }

        case 18:
            if (gSaveContext.linkAge != 0) {
                return (gSaveContext.eventChkInf[8] & 1) ? 0x505F : ((gSaveContext.infTable[22] & 0x0008) ? 0x505E : 0x505D);
            } else {
                return (self->talonEventChkInf & 0x0800) ? 0x5062 : ((gSaveContext.infTable[22] & 0x0010) ? 0x5061 : 0x5060);
            }

        case 19:
            return 0x7120;

        case 20:
            return 0x7121;

        default:
            return 0;
    }
}

}
