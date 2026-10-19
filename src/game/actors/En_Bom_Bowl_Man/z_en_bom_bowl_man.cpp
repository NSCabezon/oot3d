extern "C" {
#include "z_en_bom_bowl_man.hpp"

#define FLAGS 0x08000039

s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void GraphicsContext_UnknownStub(GraphicsContext* gfxCtx);
void FUN_0034df48(MTX34* mtx, Vec3s* rot);
s32 FUN_003405ec(Actor* actor, PlayState* play, s16 angle, f32 range);
void FUN_003725e0(PlayState* play);
void FUN_00370778(PlayState* play);
void FUN_00239534(EnBomBowlMan* self, PlayState* play);
void FUN_0036bcc8(PlayState* play, Actor* actor, Vec3s* headRot, Vec3s* torsoRot, s32 arg5, Vec3f pos);
f32* EnBomBowlMan_CurFramePtr(SkelAnime* skelAnime);
void FUN_0036ae48(Camera* camera);
s32 FUN_00369f3c(PlayState* play);
void OnePointCutscene_Init(PlayState* play, s32 csId, s32 arg, Actor* actor, s32 camId);
void Rupees_ChangeBy(s32 amount);
void EffectSsBomb2_SpawnLayered(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel, s16 life, s16 scale);

void EnBomBowlMan_Init(Actor* thisx, PlayState* play2);
void EnBomBowlMan_Destroy(Actor* thisx, GameState* state);
void EnBomBowlMan_Update(Actor* thisx, PlayState* play);
void EnBomBowlMan_HandlePlayChoice(EnBomBowlMan* self, PlayState* play);
void EnBomBowlMan_SetupChooseShowPrize(EnBomBowlMan* self, PlayState* play);
void EnBomBowlMan_ChooseShowPrize(EnBomBowlMan* self, PlayState* play);
void EnBomBowlMan_CheckBeatenDC(EnBomBowlMan* self, PlayState* play);
void FUN_00288c08(EnBomBowlMan* self, PlayState* play);
void FUN_00274648(EnBomBowlMan* self, PlayState* play);
extern Vec3f EnBomBowlMan_sRewardPos[5]; // data_syms.s (0x0052105C)
extern s16 EnBomBowlMan_sRewardYaw[5];   // data_syms.s (0x00521158)
void EnBomBowlMan_Draw(Actor* thisx, PlayState* play);
s32 EnBomBowlMan_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void EnBomBowlMan_SetupWaitAsleep(EnBomBowlMan* self, PlayState* play);
void EnBomBowlMan_BlinkAwake(EnBomBowlMan* self, PlayState* play);
void EnBomBowlMan_WaitAsleep(EnBomBowlMan* self, PlayState* play);
void EnBomBowlMan_TalkAsleep(EnBomBowlMan* self, PlayState* play);
void EnBomBowlMan_WakeUp(EnBomBowlMan* self, PlayState* play);
void EnBomBowlMan_WaitNotBeatenDC(EnBomBowlMan* self, PlayState* play);
void FUN_001e8e98(EnBomBowlMan* self, PlayState* play);
void FUN_002aa808(EnBomBowlMan* self, PlayState* play);

GLOBAL_ASM("../src/game/actors/En_Bom_Bowl_Man/z_En_Bom_Bowl_Man_data_tail.s")
GLOBAL_ASM("../src/game/actors/En_Bom_Bowl_Man/data_syms.s")

ActorInit En_Bom_Bowl_Man_InitVars = {
    ACTOR_EN_BOM_BOWL_MAN,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_BG,
    sizeof(EnBomBowlMan),
    (ActorFunc)EnBomBowlMan_Init,
    (ActorFunc)EnBomBowlMan_Destroy,
    (ActorFunc)EnBomBowlMan_Update,
    (ActorFunc)EnBomBowlMan_Draw,
};

typedef struct CuccoColliderDim {
    f32 radius;
    f32 height;
    s32 unk_8; // unused
} CuccoColliderDim;

// Adjacent statics right after InitVars (ROM derives all three bases from one literal).
static f32 sCuccoScales[2] = { 0.01f, 0.03f };
static CuccoColliderDim sCuccoDims[2] = { { 16.0f, 46.0f, 0 }, { 36.0f, 56.0f, 0 } };
static Vec3f sCuccoPositions[2] = { { 60.0f, -60.0f, -430.0f }, { 0.0f, -120.0f, -620.0f } };

void EnBomBowlMan_Init(Actor* thisx, PlayState* play2) {
    EnBomBowlMan* self = (EnBomBowlMan*)thisx;
    PlayState* play = play2;
    s32 i;
    Actor* cucco;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    FUN_00372f38(&self->actor, play, &self->unk_688, 0, 0);
    FUN_00353c9c(&self->actor, play, &self->skelAnime, 0, 1, &self->unk_228, (u8*)self + 0x430, 10);
    SkelAnime_SetFaceAnimations((FaceAnimations*)self->faceAnimations, &self->skelAnime, 0, -1, -1);
    self->homePos = self->actor.world.pos;
    self->actor.shape.yOffset = -60.0f;
    Actor_SetScale(&self->actor, 0.013f);
    for (i = 0; i < 2; i++) {
        cucco = Actor_Spawn(&play->actorCtx, play, ACTOR_EN_SYATEKI_NIW, sCuccoPositions[i].x, sCuccoPositions[i].y,
                            sCuccoPositions[i].z, 0, 0, 0, 1, 1);
        if (cucco != NULL) {
            *(f32*)((u8*)cucco + 0x674) = sCuccoScales[i];
            *(f32*)((u8*)cucco + 0x6BC) = (s16)sCuccoDims[i].radius;
            *(f32*)((u8*)cucco + 0x6C0) = (s16)sCuccoDims[i].height;
        }
    }
    self->rewardState = (s16)Rand_ZeroFloat(4.99f);
    self->actor.targetMode = 1;
    self->actionFunc = EnBomBowlMan_SetupWaitAsleep;
}

void EnBomBowlMan_Destroy(Actor* thisx, GameState* state) {
    EnBomBowlMan* self = (EnBomBowlMan*)thisx;

    ((FaceAnimations*)self->faceAnimations)->Destroy();
    FUN_003508b8(&self->actor, self->unk_688, 0);
    SkelAnime_Free2(&self->skelAnime);
}

void EnBomBowlMan_Update(Actor* thisx, PlayState* play) {
    EnBomBowlMan* self = (EnBomBowlMan*)thisx;

    self->unk_650++;
    thisx->focus.pos.y = 60.0f;
    Actor_SetFocus(thisx, 60.0f);
    switch (self->eyesState) {
        case 0:
            self->eyeTexIndex = 0x200;
            break;
        case 1:
            if (self->eyeTexIndex >= 0xA9) {
                self->eyeTexIndex -= 0xA9;
            } else {
                self->eyeTexIndex = 0;
                self->eyesTimer = 30;
                self->eyesState = 2;
            }
            break;
        case 2:
            if (self->eyesTimer == 0 && self->eyeTexIndex >= 0xA9) {
                self->eyeTexIndex -= 0xA9;
            }
            break;
        default:
            if (self->eyesTimer == 0) {
                self->eyeTexIndex += 0xA9;
                if (self->eyeTexIndex > 0x2A9) {
                    self->eyeTexIndex = 0;
                    self->eyesTimer = (s16)Rand_ZeroFloat(90.0f) + 20;
                }
            }
            FUN_0036bcc8(play, &self->actor, &self->headRot, &self->torsoRot, 0x4300, self->actor.focus.pos);
            break;
    }
    if (self->rewardAppearTimer != 0) {
        self->rewardAppearTimer--;
    }
    if (self->eyesTimer != 0) {
        self->eyesTimer--;
    }
    self->actionFunc(self, play);
}

void EnBomBowlMan_Draw(Actor* thisx, PlayState* play) {
    EnBomBowlMan* self = (EnBomBowlMan*)thisx;
    FaceAnimations* face = (FaceAnimations*)self->faceAnimations;

    ((FaceAnimations*)((EnBomBowlMan*)thisx)->faceAnimations)->FUN_0035e3a4(0, self->eyeTexIndex >> 8);
    GraphicsContext_UnknownStub(play->state.gfxCtx);
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, EnBomBowlMan_OverrideLimbDraw, NULL, &self->actor, 0);
    face->FUN_0035e330();
}

void FUN_002aa808(EnBomBowlMan* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if (self->gameStartStatus != 0) {
        self->actor.textId = 0x19;
        self->waitMessageState = 5;
    } else {
        self->actor.textId = (self->hasPlayedBefore == 0) ? 0x18 : 0x1A;
        self->waitMessageState = 4;
    }
    self->actionFunc = FUN_00239534;
}

void EnBomBowlMan_BlinkAwake(EnBomBowlMan* self, PlayState* play) {
    f32 prevFrame = *EnBomBowlMan_CurFramePtr(&self->skelAnime);

    FUN_003731e0(&self->skelAnime);
    if (prevFrame >= 23.0f) {
        self->waitMessageState = 5;
        if ((gSaveContext.eventChkInf[2] & 0x20) || (BREG(2) != 0)) {
            self->actor.textId = 0xBF;
        } else {
            self->actor.textId = 0x7058;
        }
        Message_ContinueTextbox(play, self->actor.textId);
    }
    if ((self->eyeTexIndex >> 8) == 0 && self->eyesState == 2 && self->eyesTimer == 0) {
        self->eyeTexIndex = 0x200;
        self->wakingUpTimer++;
        if (self->wakingUpTimer > 2) {
            self->actionFunc = EnBomBowlMan_CheckBeatenDC;
        }
    }
}

void EnBomBowlMan_CheckBeatenDC(EnBomBowlMan* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if ((self->waitMessageState == Message_GetState(&play->msgCtx)) && Message_ShouldAdvance(play)) {
        FUN_003725e0(play);
        self->curAnimFrameCount = Animation_GetLastFrame(&self->skelAnime, 2);
        Animation_Change_00375c08(&self->skelAnime, 2, 1.0f, 0.0f, self->curAnimFrameCount, 0, -10.0f);
        self->eyesState = 3;
        self->eyesTimer = (s16)Rand_ZeroFloat(60.0f) + 20;
        if (!((gSaveContext.eventChkInf[2] & 0x20) || (BREG(2) != 0))) {
            self->actionFunc = EnBomBowlMan_WaitNotBeatenDC;
        } else {
            self->actor.textId = 0x18;
            self->waitMessageState = 4;
            Message_ContinueTextbox(play, self->actor.textId);
            self->actionFunc = EnBomBowlMan_HandlePlayChoice;
        }
    }
}

void EnBomBowlMan_ChooseShowPrize(EnBomBowlMan* self, PlayState* play) {
    s32 prize = 0;

    FUN_003731e0(&self->skelAnime);
    if (self->rewardAppearTimer == 0) {
        switch (self->rewardState) {
            case 0:
                if (gSaveContext.itemGetInf[1] & 2) {
                    prize = 4;
                }
                break;
            case 1:
                prize = 4;
                break;
            case 2:
                prize = 2;
                break;
            case 3:
                prize = 1;
                if (gSaveContext.itemGetInf[1] & 4) {
                    prize = 4;
                }
                break;
            case 4:
                prize = 3;
                break;
        }
        self->reward = prize;
        if (BREG(7) != 0) {
            self->reward = BREG(7) - 1;
        }
        if (self->reward == 0) {
            if (CUR_CAPACITY(UPG_BOMB_BAG) == 30) {
                self->reward = 20;
            }
        }
        self->rewardItemOnCounter = Actor_SpawnAsChild(
            &play->actorCtx, &self->actor, play, 0x168, EnBomBowlMan_sRewardPos[self->reward].x + 148.0f,
            EnBomBowlMan_sRewardPos[self->reward].y + 40.0f, EnBomBowlMan_sRewardPos[self->reward].z + 300.0f, 0,
            EnBomBowlMan_sRewardYaw[self->reward], 0, (s16)(self->reward + 5));
        if (self->hasPlayedBefore == 0) {
            self->finalTargetManager = Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, 0x14C, 0.0f, 90.0f,
                                                          -860.0f, 0, 0, 0, 0);
            if (self->finalTargetManager != NULL) {
                *(s16*)((u8*)self->finalTargetManager + 0x1B2) = self->reward;
            }
        } else {
            *(s16*)((u8*)self->finalTargetManager + 0x1B2) = self->reward;
        }
        *(s16*)((u8*)self->finalTargetManager + 0x1B4) = 1;
        self->gameStartStatus = 2;
        self->actor.textId = 0x405A;
        Message_ContinueTextbox(play, self->actor.textId);
        self->waitMessageState = 5;
        self->rewardState++;
        if (self->rewardState > 4) {
            self->rewardState = 0;
        }
        self->actionFunc = FUN_00274648;
    }
}

void EnBomBowlMan_HandlePlayChoice(EnBomBowlMan* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if ((self->waitMessageState == Message_GetState(&play->msgCtx)) && Message_ShouldAdvance(play)) {
        FUN_003725e0(play);
        switch (FUN_00369f3c(play)) {
            case 0:
                if (gSaveContext.rupees >= 30) {
                    *((u8*)play + 0x7F40) = 0;
                    Rupees_ChangeBy(-30);
                    self->gameStartStatus = 1;
                    self->wallsState[1] = 0;
                    self->wallsState[0] = 0;
                    *((u8*)play + 0x5C75) = 10;
                    Flags_SetSwitch(play, 0x38);
                    if (!self->hasPlayedBefore && !self->isAskingToPlayAgain) {
                        self->actor.textId = 0x19;
                        Message_ContinueTextbox(play, self->actor.textId);
                        self->waitMessageState = 5;
                        self->actionFunc = FUN_00288c08;
                    } else {
                        self->actor.textId = 0x1B;
                        Message_ContinueTextbox(play, self->actor.textId);
                        self->waitMessageState = 5;
                        OnePointCutscene_Init(play, 0x1F4A, -99, NULL, 0);
                        FUN_0036e980(play, NULL, 8);
                        self->actionFunc = EnBomBowlMan_SetupChooseShowPrize;
                    }
                } else {
                    self->isAskingToPlayAgain = 0;
                    self->actor.textId = 0x85;
                    Message_ContinueTextbox(play, self->actor.textId);
                    self->waitMessageState = 5;
                    self->actionFunc = FUN_00288c08;
                }
                break;
            case 1:
                self->isAskingToPlayAgain = 0;
                self->actor.textId = 0x2D;
                Message_ContinueTextbox(play, self->actor.textId);
                self->waitMessageState = 5;
                self->actionFunc = FUN_00288c08;
                break;
        }
    }
}

s32 EnBomBowlMan_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnBomBowlMan* self = (EnBomBowlMan*)thisx;

    if (limbIndex == 3) {
        FUN_0034df48(mtx, &self->headRot);
    }
    return 0;
}

void FUN_00288c08(EnBomBowlMan* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if ((self->waitMessageState == Message_GetState(&play->msgCtx)) && Message_ShouldAdvance(play)) {
        FUN_003725e0(play);
        if (self->actor.textId == 0x2D || self->actor.textId == 0x85) {
            if (Flags_GetSwitch(play, 0x38)) {
                Flags_UnsetSwitch(play, 0x38);
            }
        }
        if (self->gameStartStatus == 1) {
            self->actor.textId = 0x1B;
            Message_ContinueTextbox(play, self->actor.textId);
            self->waitMessageState = 5;
            OnePointCutscene_Init(play, 0x1F4A, -99, NULL, 0);
            FUN_0036e980(play, NULL, 8);
            self->actionFunc = EnBomBowlMan_SetupChooseShowPrize;
        } else {
            if (self->gameEndStatus == 2) {
                FUN_0036e980(play, NULL, 7);
            }
            self->actionFunc = FUN_002aa808;
        }
    }
}

void FUN_00274648(EnBomBowlMan* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if ((self->waitMessageState == Message_GetState(&play->msgCtx)) && Message_ShouldAdvance(play)) {
        FUN_003725e0(play);
        FUN_0036ae48(play->cameraPtrs[play->activeCamera]);
        self->hasPlayedBefore = 1;
        if (BREG(2) != 0) {
            BREG(2) = 0;
        }
        FUN_0036e980(play, NULL, 7);
        self->actionFunc = FUN_002aa808;
    }
}

// Near-miss (head matches via `f32 r = range;` copy; tail 4 words off, ~45 variants): ROM stores the zero from
// ip (`strb ip,[..]; mov ip,#1`), ours rematerialises `mov r1,#0` and hoists `mov ip,#1`.
// The function lives in src/functions/functions_340000s.cpp, which keeps its GLOBAL_ASM line.
#if 0
static __inline __pure float EnBomBowlMan_Fabsf(float x) {
    return __fabsf(x);
}

s32 FUN_003405ec(Actor* actor, PlayState* play, s16 angle, f32 range) {
    Player* player = GET_PLAYER(play);
    s32 ret = 0;
    s32 yawDiff;
    f32 r = range;

    if (!(player->actor.flags & 0x100)) {
        if (actor->isTargeted == 0) {
            if (r < EnBomBowlMan_Fabsf(actor->yDistToPlayer)) {
                return ret;
            }
            if (actor->xzDistToPlayer > *(f32*)((u8*)player + 0x1730) || actor->xzDistToPlayer > range) {
                return ret;
            }
        }
        yawDiff = (s16)(actor->yawTowardsPlayer - player->actor.shape.rot.y + 0x8000);
        if (yawDiff < 0) {
            yawDiff = -yawDiff;
        }
        if (yawDiff > angle) {
            return ret;
        }
        *(Actor**)((u8*)player + 0x172C) = actor;
        *(f32*)((u8*)player + 0x1730) = actor->xzDistToPlayer;
        *(s8*)((u8*)player + 0x172B) = ret;
        ret = 1;
    }
    return ret;
}
#endif

void FUN_00239534(EnBomBowlMan* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    self->gameEndStatus = 0;
    if (self->finalTargetManager != NULL) {
        if (self->wallsState[0] != 1 && self->wallsState[1] != 1) {
            if (*((u8*)self->finalTargetManager + 0x1BC) == 2) {
                self->gameEndStatus = 1;
                *((u8*)self->finalTargetManager + 0x1BC) = 0;
            }
        }
        if (*(s8*)((u8*)play + 0x5C75) == -1 && *(s32*)((u8*)play + 0x20B0) == 0 &&
            *((u8*)self->finalTargetManager + 0x1BC) == 0 && self->wallsState[0] != 1 && self->wallsState[1] != 1) {
            self->gameEndStatus = 2;
        }
    }
    EnBomBowlManActionFunc nextFunc = EnBomBowlMan_HandlePlayChoice;
    if (self->gameEndStatus != 0) {
        self->actor.textId = 0x1A;
        self->waitMessageState = 4;
        self->gameStartStatus = 0;
        if (self->rewardItemOnCounter != NULL && self->rewardItemOnCounter->update != NULL) {
            *(u16*)((u8*)self->rewardItemOnCounter + 0x1D0) = 1;
            self->rewardItemOnCounter = NULL;
        }
        *(s8*)((u8*)play + 0x5C75) = 0;
        self->isAskingToPlayAgain = 1;
        FUN_00367c7c(play, self->actor.textId, NULL);
        if (self->gameEndStatus == 2) {
            FUN_0036e980(play, NULL, 8);
        }
    } else if (Actor_ProcessTalkRequest(&self->actor, play)) {
        if (self->gameStartStatus != 0) {
            self->actionFunc = FUN_00288c08;
            return;
        }
    } else {
        s16 relYaw = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
        s16 absRelYaw = ABS(relYaw);
        if (!(self->actor.xzDistToPlayer > 120.0f) && (absRelYaw < 0x4300)) {
            FUN_0036bb28(&self->actor, play, 120.0f);
        }
        return;
    }
    self->actionFunc = nextFunc;
}

void EnBomBowlMan_SetupChooseShowPrize(EnBomBowlMan* self, PlayState* play) {
    Vector3f effAccel(0.0f, 0.1f, 0.0f);
    Vector3f effVel(0.0f, 0.0f, 0.0f);
    Vec3f effPos;

    FUN_003731e0(&self->skelAnime);
    if ((self->waitMessageState == Message_GetState(&play->msgCtx)) && Message_ShouldAdvance(play)) {
        effPos.x = 148.0f;
        effPos.y = 40.0f;
        effPos.z = 300.0f;
        EffectSsBomb2_SpawnLayered(play, &effPos, (Vec3f*)&effVel, (Vec3f*)&effAccel, 50, 15);
        Audio_PlaySoundGeneral(0x0100013D, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, (s8*)&DAT_0054ac24.unk_0);
        self->rewardAppearTimer = 10;
        self->actionFunc = EnBomBowlMan_ChooseShowPrize;
    }
}

void EnBomBowlMan_SetupWaitAsleep(EnBomBowlMan* self, PlayState* play) {
    self->curAnimFrameCount = Animation_GetLastFrame(&self->skelAnime, 1);
    Animation_Change_00375c08(&self->skelAnime, 1, 1.0f, 0.0f, self->curAnimFrameCount, 0, -10.0f);
    self->actor.textId = 0xC0;
    self->waitMessageState = 5;
    self->actionFunc = EnBomBowlMan_WaitAsleep;
}

void EnBomBowlMan_TalkAsleep(EnBomBowlMan* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if ((self->waitMessageState == Message_GetState(&play->msgCtx)) && Message_ShouldAdvance(play)) {
        FUN_00370778(play);
        self->actionFunc = EnBomBowlMan_WakeUp;
    }
}

void EnBomBowlMan_WakeUp(EnBomBowlMan* self, PlayState* play) {
    self->curAnimFrameCount = Animation_GetLastFrame(&self->skelAnime, 0);
    Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, self->curAnimFrameCount, 2, -10.0f);
    self->eyesState = 1;
    self->actionFunc = EnBomBowlMan_BlinkAwake;
}

void EnBomBowlMan_WaitAsleep(EnBomBowlMan* self, PlayState* play) {
    s16 relYaw;
    s16 absRelYaw;

    FUN_003731e0(&self->skelAnime);
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        self->actionFunc = EnBomBowlMan_TalkAsleep;
    } else {
        relYaw = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
        absRelYaw = ABS(relYaw);
        if (!(self->actor.xzDistToPlayer > 120.0f) && (absRelYaw < 0x4300)) {
            FUN_0036bb28(&self->actor, play, 120.0f);
        }
    }
}

void EnBomBowlMan_WaitNotBeatenDC(EnBomBowlMan* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        self->actionFunc = FUN_001e8e98;
    } else {
        FUN_003405ec(&self->actor, play, 0x4300, 120.0f);
    }
}

void FUN_001e8e98(EnBomBowlMan* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if ((self->waitMessageState == Message_GetState(&play->msgCtx)) && Message_ShouldAdvance(play)) {
        FUN_003725e0(play);
        self->actionFunc = EnBomBowlMan_WaitNotBeatenDC;
    }
}

}
