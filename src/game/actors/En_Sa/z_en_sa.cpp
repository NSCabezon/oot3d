extern "C" {
#include "z_en_sa.hpp"

#define FLAGS 0x02000019

void EnSa_Init(Actor* thisx, PlayState* play);
void EnSa_Destroy(Actor* actor, GameState* state);
void EnSa_Update(Actor* thisx, PlayState* play);
void EnSa_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_Sa.data.s")

GLOBAL_ASM("../src/game/actors/En_Sa/data_syms.s")

ActorInit En_Sa_InitVars = {
    ACTOR_EN_SA,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_SA,
    sizeof(EnSa),
    (ActorFunc)EnSa_Init,
    (ActorFunc)EnSa_Destroy,
    (ActorFunc)EnSa_Update,
    (ActorFunc)EnSa_Draw,
};

void FUN_003c2094(EnSa* self, PlayState* play);
void FUN_003c1e30(EnSa* self, PlayState* play);
void FUN_0012c8d0(EnSa* self, PlayState* play);
void FUN_0035fbb0(EnSa* self, s32 index);
s32 FUN_003603c0(SkelAnime* skelAnime, s32 index);
void FUN_00353e78(void* zar, PlayState* play, SkelAnime* skel, void* cmb, void* p, s32 b, void* jointTable,
                  void* morphTable, s32 limbCount);
void* ZAR_GetCMBByIndex(void* zar, s32 index);
void __ARM_common_memcpy2_6(void* dst, void* src);

extern ColliderCylinderInit EnSa_sCylinderInit;
extern CollisionCheckInfoInit2 EnSa_sColChkInfoInit;

// N64 EnSa_GetType (inlined in the ROM).
static inline s32 EnSa_GetType(EnSa* self, PlayState* play) {
    if (gSaveContext.cutsceneIndex >= 0xFFF0 && gSaveContext.cutsceneIndex != 0xFFFD) {
        if (play->sceneNum == 0x55) {
            return 4;
        }
        if (play->sceneNum == 0x56) {
            return 5;
        }
    }
    if (play->sceneNum == 0x29 && !LINK_IS_ADULT && INV_CONTENT(ITEM_OCARINA_FAIRY) == ITEM_OCARINA_FAIRY &&
        !(gSaveContext.eventChkInf[4] & 1)) {
        return 1;
    }
    if (play->sceneNum == 0x56 && (gSaveContext.eventChkInf[4] & 1)) {
        return CHECK_QUEST_ITEM(14) ? 2 : 5;
    }
    if (play->sceneNum == 0x55 && !CHECK_QUEST_ITEM(18)) {
        if (gSaveContext.infTable[0] & 1) {
            return 1;
        }
        return 4;
    }
    return 0;
}

void EnSa_Init(Actor* thisx, PlayState* play) {
    EnSa* self = (EnSa*)thisx;

    self->actor.unk_19A = 1;
    self->unk_C48 = 0;
    self->zar = &play->GetObjectStatus(self->actor.objBankIndex)->zarInfo;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 18.0f);
    FUN_00353e78(self->zar, play, &self->skelAnime, ZAR_GetCMBByIndex(self->zar, 0), self->actor.unk_178, -1,
                 self->jointTable, self->morphTable, 0x13);
    *((u8*)self->skelAnime.unk_28 + 0xAD) = 0;
    SkelAnime_SetFaceAnimations(&self->faceAnim, &self->skelAnime, 0, 1, -1);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, &EnSa_sCylinderInit);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, NULL, &EnSa_sColChkInfoInit);

    switch (EnSa_GetType(self, play)) {
        case 2:
            FUN_0035fbb0(self, 11);
            self->actionFunc = FUN_003c2094;
            break;
        case 5:
            FUN_0035fbb0(self, 11);
            self->actionFunc = FUN_0012c8d0;
            break;
        case 1:
            self->actor.gravity = -1.0f;
            FUN_0035fbb0(self, 0);
            self->actionFunc = FUN_003c2094;
            break;
        case 4:
            self->actor.gravity = -1.0f;
            self->cueId = 0;
            play->FUN_0037573c(play->GetObjectStatus(self->actor.objBankIndex)->zarInfo.GetQDBByIndex(1));
            self->unk_C49 = 1;
            gSaveContext.cutsceneTrigger = 1;
            FUN_0035fbb0(self, 4);
            self->actionFunc = FUN_003c1e30;
            break;
        case 3:
            self->actor.gravity = -1.0f;
            self->cueId = 0;
            FUN_0035fbb0(self, 0);
            self->actionFunc = FUN_003c1e30;
            break;
        case 0:
            Actor_Kill(&self->actor);
            return;
    }

    Actor_SetScale(&self->actor, 0.01f);
    thisx->targetMode = 6;
    self->interactInfo.talkState = 0;
    self->alpha = 255;
    __ARM_common_memcpy2_6(&self->initRot, &self->actor.shape.rot); // Vec3s copy (struct copy would emit a duplicate helper)
    Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, 0x18, self->actor.world.pos.x, self->actor.world.pos.y,
                       self->actor.world.pos.z, 0, 0, 0, 3);
}

// Collider_DestroyCylinder is a 2-insn stub that armlink inlines (no bl in the ROM).
void EnSa_Destroy(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnSa* self = (EnSa*)actor;

    Collider_DestroyCylinder(play, &self->collider);
    SkelAnime_Free2(&self->skelAnime);
    self->faceAnim.Destroy();
}

u16 FUN_00185964(PlayState* play, Actor* thisx);
s16 FUN_00185884(PlayState* play, Actor* thisx);
s16 FUN_00341c28(Actor* actor, PlayState* play, s16 alpha, f32 radius); // Actor_UpdateAlphaByDistance
void FUN_0034c664(Actor* actor, void* info, s32 arg2, s32 arg3);         // Npc_TrackPoint
void FUN_00342714(PlayState* play, Actor* actor, void* talkState, f32 range, void* getTextId,
                  void* updateTalkState); // Npc_UpdateTalking

void EnSa_Update(Actor* thisx, PlayState* play) {
    EnSa* self = (EnSa*)thisx;
    Player* player;
    s16 trackingMode;
    s16 yawDiff;

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);

    // Animation-end checks (N64: OcarinaToMouth -> PlayingOcarina); 3DS has three of them.
    if (self->skelAnime.animationType == 0xE) {
        f32 curFrame = self->skelAnime.curFrame;
        f32 lastFrame = (f32)FUN_003603c0(&self->skelAnime, 0xE);
        if (curFrame >= lastFrame) {
            FUN_0035fbb0(self, 6);
        }
    }
    if (self->skelAnime.animationType == 0x12) {
        f32 curFrame = self->skelAnime.curFrame;
        f32 lastFrame = (f32)FUN_003603c0(&self->skelAnime, 0x12);
        if (curFrame >= lastFrame) {
            FUN_0035fbb0(self, 12);
        }
    }
    if (self->skelAnime.animationType == 0x11) {
        f32 curFrame = self->skelAnime.curFrame;
        f32 lastFrame = (f32)FUN_003603c0(&self->skelAnime, 0x11);
        if (curFrame >= lastFrame) {
            FUN_0035fbb0(self, 13);
        }
    }

    if (self->actionFunc != FUN_003c1e30) {
        self->alpha = FUN_00341c28(&self->actor, play, self->alpha, 400.0f);
    } else {
        self->alpha = 255;
    }
    thisx->shape.shadowAlpha = self->alpha;
    if (self->alpha != 0) {
        FUN_003731e0(&self->skelAnime);
    }

    if (self->alpha == 0) {
        self->actor.flags |= 0x20000000;
    } else {
        self->actor.flags &= ~0x20000000;
    }

    if (self->actionFunc == FUN_003c1e30) {
        thisx->world.pos.x += self->actor.velocity.x;
        self->actor.world.pos.y += self->actor.velocity.y;
        self->actor.world.pos.z += self->actor.velocity.z;
    } else {
        FUN_0036b96c(&self->actor);
    }

    if (play->sceneNum != 0x56) {
        u8* tail = (u8*)self + 0xC00; // unk_C48 is read through the 0xC00 base (see ROM add r0,r4,#0xc00)
        if (self->unk_C49 != 0) {
            self->actor.gravity = -1.0f;
            Actor_UpdateBgCheckInfo(play, &self->actor, 0.0f, 0.0f, 0.0f, 4);
        } else if (tail[0x48] == 0) {
            Actor_UpdateBgCheckInfo(play, &self->actor, 0.0f, 0.0f, 0.0f, 4);
            ((EnSa*)thisx)->unk_C48 = 1;
            self->actor.gravity = 0.0f;
        }
    }

    if (!((self->skelAnime.animationType == 0x10 || self->skelAnime.animationType == 0xE) &&
          self->interactInfo.talkState == 0 && (self->blinkTimer = 0, self->rightEyeIndex == 2))) {
        if (DECR(self->blinkTimer) == 0) {
            if (play->state.frames & 1) {
                self->rightEyeIndex++;
                if (self->rightEyeIndex >= 3) {
                    self->blinkTimer = Rand_S16Offset_003702c8(30, 30);
                    self->rightEyeIndex = 0;
                }
            }
        }
    }

    self->actionFunc(self, play);

    if (play->sceneNum != 0x56 ||
        ((yawDiff = (s16)(self->actor.yawTowardsPlayer - thisx->shape.rot.y)), ABS(yawDiff) < 0x1555) ||
        self->interactInfo.talkState != 0) {
        FUN_00342714(play, &self->actor, &self->interactInfo.talkState, self->collider.dim.radius + 30.0f,
                     (void*)FUN_00185964, (void*)FUN_00185884);
    }

    player = GET_PLAYER(play);
    trackingMode = 0;
    if (play->sceneNum == 0x55) {
        trackingMode = (self->actionFunc == FUN_003c1e30) ? 1 : 4;
    }
    if (play->sceneNum == 0x56) {
        trackingMode = (self->skelAnime.animationType == 0x10) ? 1 : 3;
    }
    if (play->sceneNum == 0x56 && self->actionFunc == FUN_003c2094 && self->skelAnime.animationType == 0x11) {
        trackingMode = 1;
    }
    if (play->sceneNum == 0x56 && self->actionFunc == FUN_003c1e30 && self->skelAnime.animationType == 0xE) {
        trackingMode = 1;
    }
    self->interactInfo.trackPos = player->actor.world.pos;
    self->interactInfo.yOffset = 4.0f;
    FUN_0034c664(&self->actor, &self->interactInfo, 2, trackingMode);
    *((u8*)self->skelAnime.unk_28 + 0xAD) = (self->actor.isDrawn != 0);
}

// Matched: FUN_00185964 = EnSa_GetTextId, FUN_00185884 = EnSa_UpdateTalkState (3DS, moved from functions_180000s.cpp).
u16 FUN_00185964(PlayState* play, Actor* thisx) {
    EnSa* self = (EnSa*)thisx;
    u16 textId = Text_GetFaceReaction(play, 0x10);

    if (textId != 0) {
        return textId;
    }
    if (CHECK_QUEST_ITEM(14)) {
        return 0x10AD;
    }
    if (CHECK_QUEST_ITEM(18)) {
        self->messageIndex = 0;
        self->prevTextState = 0;
        if (gSaveContext.infTable[0] & 0x20) {
            return 0x1048;
        } else {
            return 0x1047;
        }
    }
    if (gSaveContext.eventChkInf[0] & 0x4) {
        self->messageIndex = 0;
        self->prevTextState = 0;
        if (gSaveContext.infTable[0] & 0x8) {
            return 0x1032;
        } else {
            return 0x1031;
        }
    }
    if (gSaveContext.infTable[0] & 0x1) {
        self->messageIndex = 0;
        self->prevTextState = 0;
        if (gSaveContext.infTable[0] & 0x2) {
            return 0x1003;
        } else {
            return 0x1002;
        }
    }
    return 0x1001;
}

s16 FUN_00185884(PlayState* play, Actor* thisx) {
    EnSa* self = (EnSa*)thisx;
    s16 talkState = 1;
    s16 textState = Message_GetState(&play->msgCtx);

    if (self->prevTextState == 10 || self->prevTextState == 5 || self->prevTextState == 2 ||
        self->prevTextState == 1) {
        if (textState != self->prevTextState) {
            self->messageIndex++;
        }
    }
    self->prevTextState = textState;

    switch (textState) {
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
        case 2:
            switch (self->actor.textId) {
                case 0x1002:
                    gSaveContext.infTable[0] |= 0x2;
                    talkState = 0;
                    break;
                case 0x1031:
                    gSaveContext.eventChkInf[0] |= 0x8;
                    gSaveContext.infTable[0] |= 0x8;
                    talkState = 0;
                    break;
                case 0x1047:
                    gSaveContext.infTable[0] |= 0x20;
                    talkState = 0;
                    break;
                default:
                    talkState = 0;
                    break;
            }
            break;
    }
    return talkState;
}

void FUN_0034e01c(MTX34* mtx, Vec3s* rot);
void FUN_0036932c(SkeletonAnimationModel* model, s32 arg);
void FUN_0037266c(SkeletonAnimationModel* model, s32 arg);
void* FUN_003687a8(SkeletonAnimationModel* model);
void FUN_003589cc(void* p, s32 arg);
void FUN_00358964(void* p, s32 arg, void* data);
s32 FUN_0021e968(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
s32 FUN_0023bca4(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx); // EnSa_OverrideLimbDraw (still asm)

extern u8 EnSa_sFaceRemap[];
extern f32 EnSa_sRgbaConst[];

typedef struct {
    u32 w[4];
} EnSaWords4;

void EnSa_Draw(Actor* actor, GameState* state) {
    EnSa* self = (EnSa*)actor;
    f32 buf[4];

    {
        FaceAnimations* fa = &self->faceAnim;
        fa->FUN_0035e3a4(0, self->rightEyeIndex);
    }
    {
        FaceAnimations* fa = &self->faceAnim;
        fa->FUN_0035e3a4(1, EnSa_sFaceRemap[self->leftEyeIndex]);
    }
    self->faceAnim.FUN_0035e330();

    if (self->alpha != 0) {
        *(EnSaWords4*)buf = *(EnSaWords4*)EnSa_sRgbaConst;
        buf[3] = (f32)self->alpha * (1.0f / 255.0f);
        {
            void* p = FUN_003687a8(self->skelAnime.unk_28);

            FUN_003589cc(p, 4);
            FUN_00358964(p, 4, buf);
        }
        if (self->alpha < 255) {
            FUN_0036932c(self->skelAnime.unk_28, 1);
        } else {
            FUN_0037266c(self->skelAnime.unk_28, 1);
        }
        SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, (OverrideLimbDrawOpa)FUN_0023bca4,
                          (PostLimbDrawOpa)FUN_0021e968, actor, 0);
    }
}

#if 0
// ---- draft, not matched (FUN_0023bca4 = EnSa_OverrideLimbDraw stays GLOBAL_ASM in functions_200000s.cpp) ----
// 24 words off after ~45 variants: ROM keeps limb==10 as a real branch (bne, bl) and predicates only limb==9
// (addeq/bleq); armcc here cross-jumps both bl FUN_0034e01c into one and predicates both. else-if / two ifs /
// switch / aliases / typed self param / thisx casts all give the same layout.
s32 FUN_0023bca4(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnSa* self = (EnSa*)thisx;

    if (limbIndex == 10) {
        FUN_0034e01c(mtx, &self->interactInfo.headRot);
    } else if (limbIndex == 9) {
        FUN_0034e01c(mtx, &self->interactInfo.torsoRot);
    }

    if (play->sceneNum == 0x56 && limbIndex == 18) {
        FUN_0037266c(self->skelAnime.unk_28, 5);
        FUN_0036932c(self->skelAnime.unk_28, 2);
    } else {
        FUN_0036932c(self->skelAnime.unk_28, 5);
        FUN_0037266c(self->skelAnime.unk_28, 2);
    }
    return 0;
}

#endif

}
