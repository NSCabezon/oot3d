extern "C" {
#include "z_en_du.hpp"

GLOBAL_ASM("../src/game/actors/En_Du/data_syms.s")

#define FLAGS 0x02000009
#define R_UPDATE_RATE SREG(30)
#define ENDU_OCARINA_MODE(play) (*(u16*)((u8*)(play) + 0x2B7E))

void EnDu_Init(Actor* thisx, PlayState* play);
void EnDu_Destroy(Actor* actor, PlayState* play);
void EnDu_Update(Actor* thisx, PlayState* play);
void EnDu_Draw(Actor* actor, PlayState* play);

extern ColliderCylinderInit EnDu_sCylinderInit0;
extern ColliderCylinderInit EnDu_sCylinderInit1;
extern ColliderCylinderInit EnDu_sCylinderInit2;
extern CollisionCheckInfoInit2 EnDu_sColChkInfoInit;
extern struct_80034EC0_Entry EnDu_sAnimationInfo[];

void FUN_00194824(EnDu* self, PlayState* play);
void FUN_00194994(EnDu* self, PlayState* play);
void FUN_001508c8(EnDu* self, PlayState* play);
void FUN_00116904(EnDu* self, PlayState* play);
void FUN_0013351c(EnDu* self, PlayState* play);
void FUN_001c4398(EnDu* self, PlayState* play);
void FUN_003b83d0(EnDu* self, PlayState* play);
void FUN_00150c38(EnDu* self, PlayState* play);
void FUN_00353e78(void* zar, PlayState* play, SkelAnime* skel, void* cmb, void* p, s32 b, void* jointTable,
                  void* morphTable, s32 limbCount);
void* ZAR_GetCMBByIndex(void* zar, s32 index);
DamageTable* DamageTable_Get(s32 index);
void FUN_00343868(PlayState* play, s32 id);
void FUN_0034e01c(void* arg, void* pos);
void FUN_003428a0(PlayState* play, SkelAnime* skel, void* ovr, void* post, void* arg, s32 alpha);
void FUN_0035c528(u32 seqId);

extern __pure float _fabsf(float);
static __inline __pure float EnDu_Fabsf(float f) {
    return _fabsf(f);
}

GLOBAL_ASM("data/z_En_Du.data.s")

ActorInit En_Du_InitVars = {
    ACTOR_EN_DU,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_DU,
    sizeof(EnDu),
    (ActorFunc)EnDu_Init,
    (ActorFunc)EnDu_Destroy,
    (ActorFunc)EnDu_Update,
    (ActorFunc)EnDu_Draw,
};

static inline u8* EnDu_GetObjBank(PlayState* play, u32 idx) {
    if (idx < 0x13 && *(s32*)((u8*)play + (idx << 7) + 0x3A64) != 0) {
        return (u8*)play + (idx << 7) + 0x3A5C;
    }
    return NULL;
}

void EnDu_Init(Actor* thisx, PlayState* play) {
    EnDu* self = (EnDu*)thisx;
    s32 i;
    EnDu* s2 = self;
    PlayState* p2 = play;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    {
        void* zar = EnDu_GetObjBank(play, (u8)self->actor.objBankIndex) + 0x10;
        SkelAnime* skel = &self->skelAnime;
        void* cmb = ZAR_GetCMBByIndex(zar, 0);
        void* p = s2->actor.unk_178;

        FUN_00353e78(zar, p2, skel, cmb, p, -1, NULL, NULL, 0);
    }
    SkelAnime_SetFaceAnimations(&self->faceAnim, &self->skelAnime, 0, 1, 2);
    for (i = 0; i < 3; i++) {
        Collider_InitCylinder(play, &self->collider[i]);
    }
    Collider_SetCylinder(play, &self->collider[0], &self->actor, &EnDu_sCylinderInit0);
    Collider_SetCylinder(play, &self->collider[1], &self->actor, &EnDu_sCylinderInit1);
    Collider_SetCylinder(play, &self->collider[2], &self->actor, &EnDu_sCylinderInit2);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, DamageTable_Get(0x16), &EnDu_sColChkInfoInit);

    // N64 func_809FDDB4 (inlined)
    if (play->sceneNum == 0x62) {
        if (!LINK_IS_CHILD) {
            Actor_Kill(&self->actor);
            return;
        }
    } else if (play->sceneNum == 4) {
        if ((gSaveContext.infTable[17] & 0x400) || !LINK_IS_ADULT) {
            Actor_Kill(&self->actor);
            return;
        }
    } else {
        Actor_Kill(&self->actor);
        return;
    }
    FUN_003717ac(&self->skelAnime, EnDu_sAnimationInfo, 0);
    Actor_SetScale(thisx, 0.01f);
    self->actor.targetMode = 1;
    {
        s16 talkState = 0;

        self->interactInfo.talkState = talkState;
    }

    if (gSaveContext.cutsceneIndex >= 0xFFF0) {
        play->FUN_0037573c(play->sceneZARInfo.GetQDBByIndex(1));
        gSaveContext.cutsceneTrigger = 1;
        self->actionFunc = FUN_001508c8;
    } else if (play->sceneNum == 4) {
        self->actionFunc = FUN_00150c38;
    } else {
        EnDuActionFunc f = !LINK_IS_ADULT ? FUN_00194994 : FUN_003b83d0;
        self->actionFunc = f;
    }
}

void FUN_003b83d0(EnDu* self, PlayState* play) {
}

void FUN_0013351c(EnDu* self, PlayState* play) {
    if (self->interactInfo.talkState == 3) {
        self->interactInfo.talkState = 0;
        self->actionFunc = FUN_00194994;
    }
}

void FUN_001c4398(EnDu* self, PlayState* play) {
    if (Actor_HasParent(&self->actor, play)) {
        self->actor.parent = NULL;
        self->actionFunc = FUN_0013351c;
    } else {
        f32 xzRange = self->actor.xzDistToPlayer + 1.0f;

        GiveItem(&self->actor, play, 0x54, xzRange, EnDu_Fabsf(self->actor.yDistToPlayer) + 1.0f);
    }
}

void FUN_00257ee8(EnDu* self, PlayState* play) {
    if (self->interactInfo.talkState == 2) {
        FUN_0036e980(play, &self->actor, 7);
        self->actionFunc = FUN_001c4398;
        if (Actor_HasParent(&self->actor, play)) {
            self->actor.parent = NULL;
            self->actionFunc = FUN_0013351c;
        } else {
            f32 xzRange = self->actor.xzDistToPlayer + 1.0f;

            GiveItem(&self->actor, play, 0x54, xzRange, EnDu_Fabsf(self->actor.yDistToPlayer) + 1.0f);
        }
    }
}

void FUN_00194994(EnDu* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (player->stateFlags2 & 0x1000000) {
        FUN_00343868(play, 0x22);
        player->stateFlags2 |= 0x2000000;
        *(Actor**)((u8*)player + 0x1740) = &self->actor;
        self->actionFunc = FUN_00194824;
        return;
    }
    if (self->interactInfo.talkState == 2) {
        FUN_0036e980(play, &self->actor, 7);
        self->interactInfo.talkState = 0;
    }
    if (self->actor.xzDistToPlayer < self->collider[0].dim.radius + 116.0f) {
        player->stateFlags2 |= 0x800000;
    }
}

s32 FUN_002331a0(PlayState* play, s32 limbIndex, void* mtx, EnDu* self) {
    if (limbIndex == 13) {
        FUN_0034e01c(mtx, &self->interactInfo.headRot);
    } else if (limbIndex == 10) {
        FUN_0034e01c(mtx, &self->interactInfo.torsoRot);
    }
    return 0;
}

void FUN_00150c38(EnDu* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (player->stateFlags1 & 0x20000000) {
        return;
    }
    play->SetCutsceneState(2);
    player->actor.world.rot.y = self->actor.world.rot.y + 0x7FFF;
    player->actor.shape.rot.y = player->actor.world.rot.y;
    FUN_0035c528(0x010005be);
    self->unk_508 = 0;
    self->actionFunc = FUN_00116904;
    self->unk_500 = (s16)(150.0f / R_UPDATE_RATE + 0.5f);
    FUN_00116904(self, play);
}

s16 FUN_0016ecd4(PlayState* play, Actor* actor) {
    switch (Message_GetState(&play->msgCtx)) {
        case TEXT_STATE_NONE:
        case TEXT_STATE_DONE_HAS_NEXT:
            break;
        case TEXT_STATE_CLOSING:
            switch (actor->textId) {
                case 0x301A:
                    gSaveContext.infTable[17] |= 8;
                    break;
                case 0x301C:
                case 0x301F:
                    return 2;
                case 0x3020:
                    gSaveContext.eventChkInf[2] |= 0x0004;
                    break;
            }
            return 0;
        case TEXT_STATE_DONE_FADING:
        case TEXT_STATE_CHOICE:
        case TEXT_STATE_EVENT:
            break;
        case TEXT_STATE_DONE:
            if (Message_ShouldAdvance(play)) {
                return 3;
            }
            break;
        case TEXT_STATE_SONG_DEMO_DONE:
        case TEXT_STATE_8:
        case TEXT_STATE_9:
            break;
    }
    return 1;
}

u16 FUN_0016eda0(PlayState* play, Actor* actor) {
    u16 textId = Text_GetFaceReaction(play, 0x21);

    if (textId != 0) {
        return textId;
    }
    if (CUR_UPG_VALUE(UPG_STRENGTH) != 0) {
        if (CHECK_QUEST_ITEM(QUEST_GORON_RUBY)) {
            return 0x301E;
        } else {
            return 0x301D;
        }
    }
    if (gSaveContext.infTable[17] & 8) {
        return 0x301B;
    } else {
        return 0x301A;
    }
}

void EnDu_Destroy(Actor* thisx, PlayState* play) {
    EnDu* self = (EnDu*)thisx;
    s32 i;

    SkelAnime_Free2(&self->skelAnime);
    self->faceAnim.Destroy();
    for (i = 0; i < 3; i++) {
        Collider_DestroyCylinder(play, &self->collider[i]);
    }
}

typedef struct {
    f32 v[3];
} EnDuColliderOffsets;
extern EnDuColliderOffsets EnDu_sColliderOffsets;

void FUN_0034c664(Actor* actor, EnDuInteractInfo* info, s16 arg2, s32 arg3);
void FUN_00342714(PlayState* play, Actor* actor, void* talkState, f32 range, void* getTextId, void* updateTalkState);
s16 FUN_0016ecd4(PlayState* play, Actor* actor);
u16 FUN_0016eda0(PlayState* play, Actor* actor);

void EnDu_Update(Actor* thisx, PlayState* play) {
    EnDu* self = (EnDu*)thisx;
    PlayState* play2 = play;
    {
        EnDuColliderOffsets offsets = EnDu_sColliderOffsets;
        s32 i;

        for (i = 0; i < 3; i++) {
            Collider_UpdateCylinder(thisx, &self->collider[i]);
            self->collider[i].dim.pos.x = self->collider[0].dim.pos.x + offsets.v[i] * Math_CosS(self->actor.shape.rot.y);
            self->collider[i].dim.pos.z = self->collider[0].dim.pos.z - offsets.v[i] * Math_SinS(self->actor.shape.rot.y);
            CollisionCheck_SetOC(play2, &play->colChkCtx, &self->collider[i].base);
        }
    }

    if (self->skelAnime.animationType == 7 &&
        Animation_OnFrameImpl(&self->skelAnime, self->skelAnime.endFrame, 1.0f)) {
        FUN_003717ac(&self->skelAnime, EnDu_sAnimationInfo, 1);
    }

    FUN_00370734(&self->skelAnime);

    // N64 func_809FDE9C (inlined)
    {
        s16 timer = self->blinkTimer;

        timer = (timer > 0) ? timer - 1 : 0;
        self->blinkTimer = timer;
        if (timer < 3) {
            self->unk_50D = timer;
        }

        switch (self->unk_50A) {
            case 0:
                if (timer == 0) {
                    self->blinkTimer = Rand_S16Offset_003702c8(30, 30);
                }
                break;
            case 1:
                if (timer == 0) {
                    self->unk_50D = 2;
                }
                break;
            case 2:
                if (timer == 0) {
                    self->unk_50D = 2;
                }
                break;
            case 3:
                if (timer == 0) {
                    self->unk_50D = 0;
                }
                break;
        }
    }

    switch (self->unk_50B) {
        case 1:
            self->unk_50E = 1;
            break;
        case 2:
            self->unk_50E = 2;
            break;
        case 3:
            self->unk_50E = 3;
            break;
        default:
            self->unk_50E = 0;
            break;
    }

    if (self->unk_50C != 1) {
        self->unk_50F = 0;
    } else {
        self->unk_50F = 1;
    }

    // N64 func_809FDE24 (inlined)
    {
        Player* player = GET_PLAYER(play);
        s32 trackingMode = 0;

        if (self->interactInfo.talkState == 0) {
            trackingMode = 1;
        }
        if (self->actionFunc == FUN_001508c8) {
            trackingMode = 1;
        }
        self->interactInfo.trackPos = player->actor.world.pos;
        self->interactInfo.yOffset = 10.0f;
        FUN_0034c664(&self->actor, &self->interactInfo, 3, trackingMode);
    }

    if (self->actionFunc == FUN_001508c8) {
        thisx->world.pos.x += thisx->velocity.x;
        thisx->world.pos.y += thisx->velocity.y;
        thisx->world.pos.z += thisx->velocity.z;
    } else {
        FUN_0036b96c(&self->actor);
    }

    Actor_UpdateBgCheckInfo(play, &self->actor, 0.0f, 0.0f, 0.0f, 4);

    if (self->actionFunc != FUN_00194824) {
        FUN_00342714(play, &self->actor, &self->interactInfo.talkState, self->collider[0].dim.radius + 116.0f,
                     (void*)FUN_0016eda0, (void*)FUN_0016ecd4);
    }
    self->actionFunc(self, play);
}

static inline void EnDu_SetAction(EnDu* self, EnDuActionFunc func) {
    self->actionFunc = func;
}

static inline void* EnDu_GetQdb(PlayState* play, u8 bank, s32 idx) {
    return ((ZARInfo*)((u8*)play->GetObjectStatus(bank) + 0x10))->GetQDBByIndex(idx);
}

void FUN_00194824(EnDu* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (ENDU_OCARINA_MODE(play) == 4) {
        ENDU_OCARINA_MODE(play) = 0;
        self->actionFunc = FUN_00194994;
    } else if (ENDU_OCARINA_MODE(play) >= 6) {
        play->FUN_0037573c(EnDu_GetQdb(play, self->actor.objBankIndex, 2));
        gSaveContext.cutsceneTrigger = 1;
        self->unk_506 = 1;
        EnDu_SetAction(self, FUN_001508c8);
        ENDU_OCARINA_MODE(play) = 4;
    } else if (ENDU_OCARINA_MODE(play) == 3) {
        Audio_PlaySoundGeneral(0x01000487, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        play->FUN_0037573c(EnDu_GetQdb(play, self->actor.objBankIndex, 1));
        gSaveContext.cutsceneTrigger = 1;
        self->unk_506 = 0;
        self->actionFunc = FUN_001508c8;
        ENDU_OCARINA_MODE(play) = 4;
    } else {
        player->stateFlags2 |= 0x800000;
    }
}

// N64 Matrix_MultVec3f(src, dest) argument order; the wrapper gives the predicated call shape.
static inline void EnDu_MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}

void FUN_00216d44(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnDu* self = (EnDu*)thisx;
    Vector3f D(0.0f, -1000.0f, 0.0f);

    if (limbIndex == 13) {
        EnDu_MultVec3f(&D, (Vector3f*)&self->actor.focus.pos, mtx);
    }
}

// FaceAnimations calls go through this helper (arg order).
static inline void EnDu_SetFace(FaceAnimations* f, s32 a, s32 b) {
    f->FUN_0035e3a4(a, b);
}

void EnDu_Draw(Actor* thisx, PlayState* play) {
    EnDu* self = (EnDu*)thisx;

    EnDu_SetFace(&self->faceAnim, 0, self->unk_50D);
    EnDu_SetFace(&self->faceAnim, 1, self->unk_50E);
    EnDu_SetFace(&self->faceAnim, 2, self->unk_50F);
    self->faceAnim.FUN_0035e330();
    FUN_003428a0(play, &self->skelAnime, (void*)FUN_002331a0, (void*)FUN_00216d44, self, 255);
}
}
