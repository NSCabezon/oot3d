extern "C" {
#include "z_en_syateki_man.hpp"
#include "../En_Syateki_Itm/z_en_syateki_itm.hpp"

#define FLAGS 0x08000019

// 3DS: Actor.attentionRangeType is not in the repo Actor struct yet (raw byte at 0x1F, debt).
#define ACTOR_ATTENTION_RANGE_TYPE(actor) (*(u8*)((u8*)(actor) + 0x1F))
// play->shootingGalleryStatus is not in the repo PlayState yet (raw byte at 0x5C74, debt).
#define SHOOTING_GALLERY_STATUS(play) (*(s8*)((u8*)(play) + 0x5C74))

typedef enum EnSyatekiManGameResult {
    /* 0 */ SYATEKI_RESULT_NONE,
    /* 1 */ SYATEKI_RESULT_WINNER,
    /* 2 */ SYATEKI_RESULT_ALMOST,
    /* 3 */ SYATEKI_RESULT_FAILURE,
    /* 4 */ SYATEKI_RESULT_REFUSE
} EnSyatekiManGameResult;

typedef enum EnSyatekiManTextIdx {
    /* 0 */ SYATEKI_TEXT_CHOICE,
    /* 1 */ SYATEKI_TEXT_START_GAME,
    /* 2 */ SYATEKI_TEXT_NO_RUPEES,
    /* 3 */ SYATEKI_TEXT_REFUSE
} EnSyatekiManTextIdx;

void EnSyatekiMan_Init(Actor* actor, GameState* state);
void EnSyatekiMan_Destroy(Actor* actor, GameState* state);
void EnSyatekiMan_Update(Actor* actor, GameState* state);
void EnSyatekiMan_Draw(Actor* actor, GameState* state);

void EnSyatekiMan_SetupIdle(EnSyatekiMan* self, PlayState* play);
void EnSyatekiMan_Idle(EnSyatekiMan* self, PlayState* play);
void EnSyatekiMan_Talk(EnSyatekiMan* self, PlayState* play);
void EnSyatekiMan_StopTalk(EnSyatekiMan* self, PlayState* play);
void EnSyatekiMan_StartGame(EnSyatekiMan* self, PlayState* play);
void EnSyatekiMan_WaitForGame(EnSyatekiMan* self, PlayState* play);
void EnSyatekiMan_EndGame(EnSyatekiMan* self, PlayState* play);
void EnSyatekiMan_GivePrize(EnSyatekiMan* self, PlayState* play);
void EnSyatekiMan_FinishPrize(EnSyatekiMan* self, PlayState* play);
void EnSyatekiMan_RestartGame(EnSyatekiMan* self, PlayState* play);
void EnSyatekiMan_BlinkWait(EnSyatekiMan* self);
void EnSyatekiMan_Start(EnSyatekiMan* self, PlayState* play);
void FUN_001310a0(EnSyatekiMan* self);
void FUN_0019efd8(EnSyatekiMan* self, PlayState* play);
s32 FUN_00284070(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);

void FUN_00350f34(Actor* actor, ...);
void FUN_003508b8(Actor* actor, ...);
void SkelAnime_Free2(SkelAnime* skelAnime);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
s32 FUN_003731e0(SkelAnime* skelAnime);
s32 FUN_0036bb28(Actor* actor, PlayState* play, f32 arg2);
s32 Actor_HasParent(Actor* actor, PlayState* play);
s32 GiveItem(Actor* actor, PlayState* play, s32 getItemId, f32 xyRange, f32 yRange);
u8 Message_GetState(MessageContext* msgCtx);
u8 Message_ShouldAdvance(PlayState* play);
void Message_ContinueTextbox(PlayState* play, u16 textId);
void FUN_003725e0(PlayState* play);
void OnePointCutscene_EndCutscene(PlayState* play, s32 camId);
s32 OnePointCutscene_Init(PlayState* play, s32 csId, s32 arg2, Actor* actor, s32 camId);
void Rupees_ChangeBy(s16 value);
void FUN_00367c7c(PlayState* play, u16 textID, Actor* actor);
s32 FUN_00369f3c(PlayState* play);
u32 FUN_00374be8(PlayState* play, u32 arg1);
void FUN_0034557c(PlayState* play, s32 arg);
f32 Rand_ZeroOne(void);
void FUN_0034e01c(MTX34* mtx, Vec3s* rot);
void FUN_0036bcc8(PlayState* play, Actor* actor, Vec3s* headRot, Vec3s* torsoRot, Vec3f focusPos, s32 yawLimit);
void FUN_0036932c(SkeletonAnimationModel* model, s32 idx);
void FUN_0037266c(SkeletonAnimationModel* model, s32 arg);
void FUN_00353c9c(Actor* actor, PlayState* play, SkelAnime* skelAnime, s32 cmbIndex, s32 csabIndex, void* joint,
                  void* morph, s32 limbCount);
void GraphicsContext_UnknownStub(GraphicsContext* gfxCtx);

ActorInit En_Syateki_Man_InitVars = {
    ACTOR_EN_SYATEKI_MAN,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_OSSAN,
    sizeof(EnSyatekiMan),
    (ActorFunc)EnSyatekiMan_Init,
    (ActorFunc)EnSyatekiMan_Destroy,
    (ActorFunc)EnSyatekiMan_Update,
    (ActorFunc)EnSyatekiMan_Draw,
};

// Text tables at 0x0052F4F8 (16 bytes, own section pinned in data/oot3d.ld). Both arrays must live in this TU
// and be non-const: the ROM folds sTextBoxCount to sTextIds + 8, and const folds the values away.
__attribute__((section("EnSyatekiManTextData"))) static s16 sTextIds[] = { 0x2B, 0x2E, 0xC8, 0x2D };
__attribute__((section("EnSyatekiManTextData"))) static s16 sTextBoxCount[] = { TEXT_STATE_CHOICE, TEXT_STATE_EVENT, TEXT_STATE_EVENT, TEXT_STATE_EVENT };

void EnSyatekiMan_Destroy(Actor* thisx, GameState* state) {
    EnSyatekiMan* self = (EnSyatekiMan*)thisx;

    FUN_003508b8(thisx, self->model, 0);
    SkelAnime_Free2(&self->skelAnime);
}

void EnSyatekiMan_Init(Actor* thisx, GameState* state) {
    EnSyatekiMan* self = (EnSyatekiMan*)thisx;
    PlayState* play = (PlayState*)state;

    ACTOR_ATTENTION_RANGE_TYPE(self) = 1;
    Actor_SetScale(&self->actor, 0.01f);
    FUN_00372f38(&self->actor, play, &self->model, 0, 0);
    FUN_00353c9c(&self->actor, play, &self->skelAnime, 0, 0, (void*)((u8*)self + 0x228), (void*)((u8*)self + 0x3C8),
                 8);
    if (!(gSaveContext.linkAge == 0)) {
        self->headRot.z = 20;
    }
    self->blinkTimer = 20;
    self->eyeState = 0;
    self->blinkFunc = EnSyatekiMan_BlinkWait;
    self->actor.colChkInfo.cylRadius = 100;
    self->actionFunc = EnSyatekiMan_Start;
}

void EnSyatekiMan_Start(EnSyatekiMan* self, PlayState* play) {
    f32 lastFrame = Animation_GetLastFrame(&self->skelAnime, 0);

    Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, lastFrame, 0, -10.0f);
    self->actionFunc = EnSyatekiMan_SetupIdle;
}

GLOBAL_ASM("../split/asm/FUN_00196A80.s")

void EnSyatekiMan_SetupIdle(EnSyatekiMan* self, PlayState* play) {
    if (self->gameResult == SYATEKI_RESULT_REFUSE) {
        self->textIdx = SYATEKI_TEXT_REFUSE;
    }

    self->actor.textId = sTextIds[self->textIdx];
    self->numTextBox = sTextBoxCount[self->textIdx];
    self->actionFunc = EnSyatekiMan_Idle;
}

void EnSyatekiMan_Idle(EnSyatekiMan* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        self->actionFunc = EnSyatekiMan_Talk;
    } else {
        FUN_0036bb28(&self->actor, play, 100.0f);
    }
}

void EnSyatekiMan_Talk(EnSyatekiMan* self, PlayState* play) {
    s16 nextState = 0;

    FUN_003731e0(&self->skelAnime);
    if (self->cameraHold) {
        SHOOTING_GALLERY_STATUS(play) = -2;
    }
    if ((self->numTextBox == Message_GetState(&play->msgCtx)) && Message_ShouldAdvance(play)) {
        if (self->textIdx == SYATEKI_TEXT_CHOICE) {
            switch (FUN_00369f3c(play)) {
                case 0:
                    if (gSaveContext.rupees >= 20) {
                        Rupees_ChangeBy(-20);
                        self->textIdx = SYATEKI_TEXT_START_GAME;
                        nextState = 1;
                    } else {
                        self->textIdx = SYATEKI_TEXT_NO_RUPEES;
                        nextState = 2;
                    }
                    self->actor.textId = sTextIds[self->textIdx];
                    self->numTextBox = sTextBoxCount[self->textIdx];
                    break;
                case 1:
                    self->actor.textId = sTextIds[SYATEKI_TEXT_REFUSE];
                    self->numTextBox = sTextBoxCount[SYATEKI_TEXT_REFUSE];
                    nextState = 2;
                    break;
            }
            Message_ContinueTextbox(play, self->actor.textId);
        } else {
            FUN_003725e0(play);
        }
        switch (nextState) {
            case 0:
                self->actionFunc = EnSyatekiMan_SetupIdle;
                break;
            case 1:
                self->actionFunc = EnSyatekiMan_StartGame;
                break;
            case 2:
                self->actionFunc = EnSyatekiMan_StopTalk;
                break;
        }
    }
}

void EnSyatekiMan_StopTalk(EnSyatekiMan* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if (self->cameraHold) {
        SHOOTING_GALLERY_STATUS(play) = -2;
    }
    if ((self->numTextBox == Message_GetState(&play->msgCtx)) && Message_ShouldAdvance(play)) {
        if (self->cameraHold) {
            OnePointCutscene_EndCutscene(play, self->subCamId);
            self->subCamId = -1;
            self->cameraHold = false;
        }
        FUN_003725e0(play);
        self->actionFunc = EnSyatekiMan_SetupIdle;
    }
}

void EnSyatekiMan_StartGame(EnSyatekiMan* self, PlayState* play) {
    EnSyatekiItm* gallery;

    FUN_003731e0(&self->skelAnime);
    if (self->cameraHold) {
        SHOOTING_GALLERY_STATUS(play) = -2;
    }
    if ((self->numTextBox == Message_GetState(&play->msgCtx)) && Message_ShouldAdvance(play)) {
        gallery = ((EnSyatekiItm*)self->actor.parent);
        if (self->cameraHold) {
            OnePointCutscene_EndCutscene(play, self->subCamId);
            self->subCamId = -1;
            self->cameraHold = false;
        }
        FUN_003725e0(play);
        if (gallery->actor.update != NULL) {
            gallery->signal = ENSYATEKI_START;
            if (FUN_00374be8(play, 0x17)) {
                self->counter = 3;
                self->actionFunc = FUN_0019efd8;
            } else {
                self->actionFunc = EnSyatekiMan_WaitForGame;
            }
        }
    }
}

void EnSyatekiMan_WaitForGame(EnSyatekiMan* self, PlayState* play) {
    EnSyatekiItm* gallery;

    FUN_003731e0(&self->skelAnime);

    gallery = ((EnSyatekiItm*)self->actor.parent);
    if ((gallery->actor.update == NULL) || (gallery->signal != ENSYATEKI_END)) {
        return;
    }

    self->subCamId = OnePointCutscene_Init(play, 8002, -99, &self->actor, 0);
    switch (gallery->hitCount) {
        case 10:
            self->gameResult = SYATEKI_RESULT_WINNER;
            self->actor.textId = 0x71AF;
            break;
        case 8:
        case 9:
            self->gameResult = SYATEKI_RESULT_ALMOST;
            self->actor.textId = 0x71AE;
            break;
        default:
            self->gameResult = SYATEKI_RESULT_FAILURE;
            self->actor.textId = 0x71AD;
            if (SHOOTING_GALLERY_STATUS(play) == 15 + 1) {
                self->gameResult = SYATEKI_RESULT_REFUSE;
                self->actor.textId = 0x2D;
            }
            break;
    }
    SHOOTING_GALLERY_STATUS(play) = -2;
    FUN_00367c7c(play, self->actor.textId, NULL);
    self->actionFunc = EnSyatekiMan_EndGame;
}

void EnSyatekiMan_EndGame(EnSyatekiMan* self, PlayState* play) {
    EnSyatekiItm* gallery;

    FUN_003731e0(&self->skelAnime);
    if ((self->numTextBox == Message_GetState(&play->msgCtx)) && Message_ShouldAdvance(play)) {
        if (self->gameResult != SYATEKI_RESULT_FAILURE) {
            OnePointCutscene_EndCutscene(play, self->subCamId);
            self->subCamId = -1;
        }
        FUN_003725e0(play);
        gallery = ((EnSyatekiItm*)self->actor.parent);
        if (gallery->actor.update != NULL) {
            gallery->signal = ENSYATEKI_RESULTS;
            self->textIdx = 0;
            switch (self->gameResult) {
                case SYATEKI_RESULT_WINNER:
                    self->tempGallery = self->actor.parent;
                    self->actor.parent = NULL;
                    if (!(gSaveContext.linkAge == 0)) {
                        if (!(gSaveContext.itemGetInf[0] & 0x2000)) {
                            if (CUR_UPG_VALUE(UPG_BULLET_BAG) == 1) {
                                self->getItemId = 0x60;
                            } else {
                                self->getItemId = 0x7B;
                            }
                        } else {
                            self->getItemId = 0x55;
                        }
                    } else {
                        if (!(gSaveContext.itemGetInf[0] & 0x4000)) {
                            switch (CUR_UPG_VALUE(UPG_QUIVER)) {
                                case 0:
                                    self->getItemId = 0x55;
                                    break;
                                case 1:
                                    self->getItemId = 0x30;
                                    break;
                                case 2:
                                    self->getItemId = 0x31;
                                    break;
                            }
                        } else {
                            self->getItemId = 0x55;
                        }
                    }
                    GiveItem(&self->actor, play, self->getItemId, 2000.0f, 1000.0f);
                    self->actionFunc = EnSyatekiMan_GivePrize;
                    break;
                case SYATEKI_RESULT_ALMOST:
                    self->timer = 30;
                    FUN_0034557c(play, 15);
                    self->actionFunc = EnSyatekiMan_RestartGame;
                    break;
                default:
                    if (self->gameResult == SYATEKI_RESULT_REFUSE) {
                        self->actionFunc = EnSyatekiMan_SetupIdle;
                    } else {
                        self->cameraHold = true;
                        self->actor.textId = sTextIds[self->textIdx];
                        self->numTextBox = sTextBoxCount[self->textIdx];
                        FUN_00367c7c(play, self->actor.textId, NULL);
                        self->actionFunc = EnSyatekiMan_Talk;
                    }
                    break;
            }
        }
    }
}

void EnSyatekiMan_GivePrize(EnSyatekiMan* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if (Actor_HasParent(&self->actor, play)) {
        self->actionFunc = EnSyatekiMan_FinishPrize;
    } else {
        GiveItem(&self->actor, play, self->getItemId, 2000.0f, 1000.0f);
    }
}

void EnSyatekiMan_FinishPrize(EnSyatekiMan* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if ((Message_GetState(&play->msgCtx) == 6) && Message_ShouldAdvance(play)) {
        if (!(gSaveContext.linkAge == 0)) {
            gSaveContext.itemGetInf[0] |= 0x2000;
        } else if ((self->getItemId == 0x30) || (self->getItemId == 0x31)) {
            gSaveContext.itemGetInf[0] |= 0x4000;
        }
        self->actor.parent = self->tempGallery;
        self->gameResult = SYATEKI_RESULT_NONE;
        self->actor.flags |= 1;
        self->actionFunc = EnSyatekiMan_SetupIdle;
    }
}

void EnSyatekiMan_RestartGame(EnSyatekiMan* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if (self->timer == 0) {
        EnSyatekiItm* gallery = ((EnSyatekiItm*)self->actor.parent);

        if (gallery->actor.update != NULL) {
            gallery->signal = ENSYATEKI_START;
            self->gameResult = SYATEKI_RESULT_NONE;
            self->actionFunc = EnSyatekiMan_WaitForGame;
        }
    }
}

void EnSyatekiMan_BlinkWait(EnSyatekiMan* self) {
    s16 decrBlinkTimer = self->blinkTimer - 1;

    if (decrBlinkTimer != 0) {
        self->blinkTimer = decrBlinkTimer;
        return;
    }
    self->blinkFunc = FUN_001310a0;
}

void FUN_001310a0(EnSyatekiMan* self) { // N64 EnSyatekiMan_Blink
    s16 decrBlinkTimer = self->blinkTimer - 1;

    if (decrBlinkTimer != 0) {
        self->blinkTimer = decrBlinkTimer;
    } else {
        s16 nextEyeState = self->eyeState + 1;

        if (nextEyeState >= 3) {
            self->eyeState = 0;
            self->blinkTimer = 20 + (s32)(Rand_ZeroOne() * 60.0f);
            self->blinkFunc = EnSyatekiMan_BlinkWait;
        } else {
            self->eyeState = nextEyeState;
            self->blinkTimer = 1;
        }
    }
}

void FUN_0019efd8(EnSyatekiMan* self, PlayState* play) {
    self->counter--;
    if (self->counter > 0) {
        return;
    }
    FUN_0034557c(play, 0xF);
    self->actionFunc = EnSyatekiMan_WaitForGame;
}

s32 FUN_00284070(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnSyatekiMan* self = (EnSyatekiMan*)thisx;

    if (limbIndex == 7) {
        Vec3s tmp;

        if (self->gameResult != SYATEKI_RESULT_REFUSE) {
            FUN_0034e01c(mtx, &self->headRot);
        } else {
            tmp = self->headRot;
            tmp.y = -tmp.y;
            FUN_0034e01c(mtx, &tmp);
        }
    } else if (limbIndex == 0) {
        FUN_0034e01c(mtx, &self->bodyRot);
    }
    return 0;
}

void EnSyatekiMan_Update(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnSyatekiMan* self = (EnSyatekiMan*)thisx;

    if (self->timer != 0) {
        self->timer--;
    }
    self->actionFunc(self, play);
    self->blinkFunc(self);
    self->actor.focus.pos.y = 70.0f;
    Actor_SetFocus(&self->actor, 70.0f);
    FUN_0036bcc8(play, &self->actor, &self->headRot, &self->bodyRot, self->actor.focus.pos, 0x4300);
}

void EnSyatekiMan_Draw(Actor* thisx, GameState* state) {
    EnSyatekiMan* self = (EnSyatekiMan*)thisx;
    PlayState* play = (PlayState*)state;

    GraphicsContext_UnknownStub(play->state.gfxCtx);
    FUN_0036932c(self->skelAnime.unk_28, 1);
    FUN_0036932c(self->skelAnime.unk_28, 2);
    s32 flag = (self->gameResult == SYATEKI_RESULT_REFUSE); // dead read kept by ROM
    FUN_0037266c(self->skelAnime.unk_28, 1);
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, FUN_00284070, NULL, thisx, 0);
}

}
