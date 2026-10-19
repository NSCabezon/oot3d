extern "C" {
#include "z_en_xc.hpp"

#define FLAGS 0x00000010

void EnXc_Init(Actor* actor, GameState* state);
void EnXc_Destroy(Actor* actor, GameState* state);
void EnXc_Update(Actor* actor, GameState* state);
void EnXc_Draw(Actor* actor, GameState* state);

void FUN_0036932c(SkeletonAnimationModel* skelAnimModel, s32 arg1);
void* ZAR_GetCMBByIndex(void* zar, s32 index);
void FUN_00353e78(void* zar, PlayState* play, SkelAnime* skelAnime, void* cmb, void* arg4, s32 arg5, void* jointTable,
                  void* morphTable, s32 limbCount);
s32 FUN_0036ae14(SkelAnime* skelAnime, s32 arg1);
void FUN_00375c08(SkelAnime* skelAnime, s32 arg1, s32 arg2, f32 f0, f32 f1, f32 f2, f32 f3);
void Item_Give(PlayState* play, s32 item);
s32 Collider_SetCylinderType1(PlayState* play, ColliderCylinder* collider, Actor* actor, ColliderCylinderInit* src);
void FUN_003674e4(s32 arg0);
void FUN_00330370(Actor* actor);
s32 FUN_00326528(Actor* actor, GameState* state);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 index);
// 3DS Animation_Change wrapper (N64 argument order): (skel, anim index, speed, start, end, mode, morph).
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void FUN_003239a4(Actor* actor, GameState* state, s32 arg);
void FUN_003210a4(Actor* actor, GameState* state);
void FUN_0031cebc(Actor* actor);
void FUN_0031ebe4(Actor* actor, GameState* state);
void FUN_0032cd68(Actor* actor, GameState* state);
void Actor_MoveForward(Actor* actor);
u32 FUN_00341df0(CollisionContext* colCtx, CollisionPoly* poly, s32 bgId);
void FUN_00319564(Actor* actor, PlayState* play);
f32 FUN_0032c66c(u16 endFrame, u16 startFrame, u16 curFrame, s32 arg3, s32 arg4);
void FUN_003fd1b8(PlayState* play, Actor* actor, SkelAnime* skelAnime, f32 f0);
void FUN_0032cc6c(Actor* actor, GameState* state);
void FUN_0032cacc(PlayState* play);

GLOBAL_ASM("data/z_En_Xc.data.s")
GLOBAL_ASM("../src/game/actors/En_Xc/data_syms.s")

extern u8 EnXc_sCylinderInit[];
extern ActorFunc EnXc_sActionFuncs[];
extern ActorFunc EnXc_sDrawFuncs[];

ActorInit En_Xc_InitVars = {
    ACTOR_EN_XC,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_XC,
    sizeof(EnXc),
    (ActorFunc)EnXc_Init,
    (ActorFunc)EnXc_Destroy,
    (ActorFunc)EnXc_Update,
    (ActorFunc)EnXc_Draw,
};

#define R_UPDATE_RATE SREG(30)

// 3DS FUN_003731e0 returns the "animation finished" flag (include/functions.hpp declares it void).
static inline s32 EnXc_UpdateSkelAnime(EnXc* self) {
    return FUN_003731e0(&self->skelAnime);
}

// N64 EnXc_BgCheck. The block-scoped play2 alias changes the arg scheduling to the ROM order (mov r0,play first).
static inline void EnXc_BgCheck(EnXc* self, PlayState* play) {
    PlayState* play2 = play;
    Actor_UpdateBgCheckInfo(play2, &self->actor, 75.0f, 30.0f, 30.0f, 4);
}

// N64 EnXc_GetCue (play2 + csCtx locals give the in-place `add rN,rN,#0x2000/#0x298` ROM shape).
static inline CsCmdActorAction* EnXc_GetCue(PlayState* play, s32 channel) {
    PlayState* play2 = play;
    CutsceneContext* csCtx = &play2->csCtx;
    CsCmdActorAction* cue = NULL;

    if (play2->GetCutsceneState()) {
        cue = csCtx->npcActions[channel];
    }
    return cue;
}

// N64 EnXc_CheckAndSetAction. The separate load of `action` keeps armcc from tail-duplicating the join
// (ROM: ldr/cmp/movne/strne after an inlined block that also stores action).
static inline void EnXc_CheckAndSetAction(EnXc* self, s32 check, s32 set) {
    s32 action = self->action;

    if (action != check) {
        self->action = set;
    }
}

// N64 EnXc_SetupInitialHarpAction
static inline void EnXc_SetupInitialHarpAction(EnXc* self, s32 animFinished) {
    if (animFinished) {
        SkelAnime* skelAnime = &self->skelAnime;

        Animation_Change_00375c08(skelAnime, 5, 1.0f, 0.0f, (f32)Animation_GetLastFrame(skelAnime, 5), 2, 0.0f);
        self->action = 8;
        self->drawMode = 3;
    }
}

// N64 func_80B3DE00 (pulling out harp, reversed)
static inline void EnXc_SetupPullOutHarpReverse(EnXc* self, s32 animFinished) {
    if (animFinished) {
        SkelAnime* skelAnime = &self->skelAnime;

        Animation_Change_00375c08(skelAnime, 4, -1.0f, (f32)Animation_GetLastFrame(skelAnime, 4), 0.0f, 2, 0.0f);
        self->action = 0xC;
        self->drawMode = 2;
    }
}

// N64 func_80B3DE78 (idle)
static inline void EnXc_SetupIdle(EnXc* self, s32 animFinished) {
    if (animFinished) {
        SkelAnime* skelAnime = &self->skelAnime;

        Animation_Change_00375c08(skelAnime, 0, 1.0f, 0.0f, (f32)Animation_GetLastFrame(skelAnime, 0), 0, 0.0f);
        self->action = 0xD;
        self->drawMode = 1;
        self->timer = 0.0f;
    }
}

// N64 func_80B3DCA8
static inline void EnXc_SetupHarpHold(EnXc* self, PlayState* play) {
    PlayState* play2 = play;
    CutsceneContext* csCtx = &play2->csCtx;

    if (play2->GetCutsceneState()) {
        CsCmdActorAction* cue = csCtx->npcActions[4];

        if (cue != NULL && cue->action == 8) {
            SkelAnime* skelAnime = &self->skelAnime;
            f32 frameCount = (f32)Animation_GetLastFrame(skelAnime, 5);

            Animation_Change_00375c08(skelAnime, 5, 0.0f, frameCount, frameCount, 0, -8.0f);
            self->action = 10;
        }
    }
}

// N64 func_80B3DAF0
static inline void EnXc_SetupPullOutHarp(EnXc* self, PlayState* play) {
    CsCmdActorAction* cue = EnXc_GetCue(play, 4);

    if (cue != NULL) {
        u16 cueId = cue->action;

        if (cueId == 3 || cueId == 0xB || cueId == 0xC || cueId == 0xD || cueId == 0x17) {
            SkelAnime* skelAnime = &self->skelAnime;

            Animation_Change_00375c08(skelAnime, 4, 1.0f, 0.0f, (f32)Animation_GetLastFrame(skelAnime, 4), 2, -4.0f);
            self->action = 7;
            self->drawMode = 2;
        }
    }
}

static inline s32 EnXc_CheckForCue(EnXc* self, PlayState* play, u16 cueId, s32 channel) {
    CsCmdActorAction* cue = EnXc_GetCue(play, channel);

    if (cue != NULL && cue->action == cueId) {
        return 1;
    }
    return 0;
}

static inline s32 EnXc_CheckForNoCue(EnXc* self, PlayState* play, u16 cueId, s32 channel) {
    CsCmdActorAction* cue = EnXc_GetCue(play, channel);

    if (cue != NULL && cue->action != cueId) {
        return 1;
    }
    return 0;
}

// N64 EnXc_SetupWalkAction / EnXc_SetupReverseWalkAction
static inline void EnXc_SetupWalkSpeed(EnXc* self, s32 nextAction) {
    Actor* thisx = &self->actor;
    f32* timer = &self->timer;

    *timer += 1.0f;
    if (*timer >= 36.0f / R_UPDATE_RATE) {
        thisx->speedXZ = (kREG(2) * 0.01f) + 1.2f;
        self->action = nextAction;
    }
}

// N64 EnXc_SetupPlayingHarpAction (+ EnXc_GetCurrentHarpAnim)
static inline void EnXc_SetupPlayingHarpAction(EnXc* self, PlayState* play, s32 animFinished) {
    if (animFinished) {
        SkelAnime* skelAnime = &self->skelAnime;
        s32 animIdx = 6;
        CsCmdActorAction* cue = EnXc_GetCue(play, 4);

        if (cue != NULL) {
            u16 cueId = cue->action;

            if (cueId == 0xB) {
                animIdx = 8;
            } else if (cueId == 0xC) {
                animIdx = 9;
            } else if (cueId == 0xD) {
                animIdx = 7;
            } else if (cueId == 0x17) {
                animIdx = 0xA;
            }
        }
        Animation_Change_00375c08(skelAnime, animIdx, 1.0f, 0.0f, (f32)Animation_GetLastFrame(skelAnime, animIdx), 0, -8.0f);
        self->action = 9;
        self->drawMode = 3;
    }
}

// N64 EnXc_SetupReverseAccel
static inline void EnXc_SetupReverseAccel(EnXc* self, PlayState* play) {
    PlayState* play2 = play;
    CutsceneContext* csCtx = &play2->csCtx;

    if (play2->GetCutsceneState()) {
        CsCmdActorAction* cue = csCtx->npcActions[4];

        if (cue != NULL && cue->action == 4) {
            SkelAnime* skelAnime = &self->skelAnime;

            f32 frameCount = (f32)Animation_GetLastFrame(skelAnime, 1);

            Animation_Change_00375c08(skelAnime, 1, -1.0f, frameCount, 0.0f, 0, -36.0f / R_UPDATE_RATE);
            self->action = 0xE;
            self->timer = 0.0f;
            self->actor.world.rot.y += 0x8000;
        }
    }
}

// N64 EnXc_SetupHaltAction
static inline void EnXc_SetupHaltAction(EnXc* self) {
    f32 xzDistToPlayer = self->actor.xzDistToPlayer;

    if (xzDistToPlayer <= (kREG(3) + 95.0f)) {
        SkelAnime* skelAnime = &self->skelAnime;
        f32 frameCount = (f32)Animation_GetLastFrame(skelAnime, 0);

        Animation_Change_00375c08(skelAnime, 0, 1.0f, 0.0f, frameCount, 0, -12.0f);
        self->action = 5;
        self->timer = 0.0f;
    }
}

static inline void EnXc_SetupReverseHaltAction(EnXc* self) {
    Actor* thisx = &self->actor;
    f32 xzDistToPlayer = thisx->xzDistToPlayer;

    if (xzDistToPlayer >= kREG(5) + 140.0f) {
        SkelAnime* skelAnime = &self->skelAnime;
        f32 frameCount = (f32)Animation_GetLastFrame(skelAnime, 0);

        Animation_Change_00375c08(skelAnime, 0, 1.0f, 0.0f, frameCount, 0, -36.0f / R_UPDATE_RATE);
        self->action = 0x10;
        self->timer = 0.0f;
    }
}

// N64 EnXc_CalcXZSpeed
static inline void EnXc_CalcXZSpeed(EnXc* self) {
    f32 timer = self->timer;
    f32* speedXZ = &self->actor.speedXZ;

    if (timer < 9.0f / R_UPDATE_RATE) {
        *speedXZ = (9.0f / R_UPDATE_RATE - timer) * (((kREG(2) * 0.01f) + 1.2f) / (9.0f / R_UPDATE_RATE));
    } else {
        *speedXZ = 0.0f;
    }
    Actor_MoveForward(&self->actor);
}

// N64 EnXc_SetupStoppedAction
static inline void EnXc_SetupStoppedAction(EnXc* self) {
    Actor* thisx = &self->actor;
    f32* timer = &self->timer;

    *timer += 1.0f;
    if (*timer >= 36.0f / R_UPDATE_RATE) {
        self->action = 6;
        thisx->speedXZ = 0.0f;
    }
}

// N64 EnXc_SetupNutThrow
static inline void EnXc_SetupNutThrow(EnXc* self) {
    Actor* thisx = &self->actor;
    f32* timer = &self->timer;

    (*timer)++;
    if (*timer >= 36.0f / R_UPDATE_RATE) {
        SkelAnime* skelAnime = &self->skelAnime;

        Animation_Change_00375c08(skelAnime, 3, 1.0f, 0.0f, (f32)Animation_GetLastFrame(skelAnime, 3), 2, 0.0f);
        self->action = 0x11;
        *timer = 0.0f;
        thisx->speedXZ = 0.0f;
    }
}

// N64 EnXc_ChangeAnimation
static inline void EnXc_ChangeAnimation(EnXc* self, s32 idx, s32 mode, f32 morphFrames, s32 reverseFlag) {
    f32 frameCount = (f32)Animation_GetLastFrame(&self->skelAnime, idx);
    f32 playbackSpeed;
    f32 startFrame;
    f32 endFrame;

    if (!reverseFlag) {
        startFrame = 0.0f;
        endFrame = frameCount;
        playbackSpeed = 1.0f;
    } else {
        startFrame = frameCount;
        endFrame = 0.0f;
        playbackSpeed = -1.0f;
    }
    Animation_Change_00375c08(&self->skelAnime, idx, playbackSpeed, startFrame, endFrame, mode, morphFrames);
}

// 3DS EnXc_ShowTriforce sfx (N64 func_80B3FAE0 plus frame sfx)
static inline void EnXc_SetTriforceSFX(EnXc* self) {
    SkelAnime* skelAnime = &self->skelAnime;

    if (Animation_OnFrameImpl(skelAnime, 5.0f, 2.0f / 3.0f) || Animation_OnFrameImpl(skelAnime, 10.0f, 2.0f / 3.0f) ||
        Animation_OnFrameImpl(skelAnime, 14.0f, 2.0f / 3.0f) || Animation_OnFrameImpl(skelAnime, 20.0f, 2.0f / 3.0f) ||
        Animation_OnFrameImpl(skelAnime, 28.0f, 2.0f / 3.0f)) {
        Audio_PlaySoundGeneral(0x01000257, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               &DAT_0054ac24.unk_0);
    }
    if (Animation_OnFrameImpl(skelAnime, 33.0f, 2.0f / 3.0f)) {
        Audio_PlaySoundGeneral(0x01000258, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               &DAT_0054ac24.unk_0);
    }
    if (Animation_OnFrameImpl(skelAnime, 38.0f, 2.0f / 3.0f)) {
        Audio_PlaySoundGeneral(0x0100055F, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               &DAT_0054ac24.unk_0);
        FUN_003674e4(1);
    }
}

// N64 EnXc_SetupShowTriforceIdleAction
static inline void EnXc_SetupShowTriforceIdleAction(EnXc* self, s32 animFinished) {
    if (animFinished) {
        SkelAnime* skelAnime = &self->skelAnime;

        Animation_Change_00375c08(skelAnime, 0xC, 1.0f, 0.0f, (f32)Animation_GetLastFrame(skelAnime, 0xB), 0, 0.0f);
        self->action = 0x38;
    }
}



// N64 func_80B3C9EC
static inline void EnXc_SetupBlockPedestal(EnXc* self) {
    EnXc_ChangeAnimation(self, 0x12, 0, 0.0f, 0);
    self->action = 0x4F;
    self->drawMode = 1;
    self->unk_C70 = 1;
}

// N64 EnXc_InitTempleOfTime
static inline void EnXc_InitTempleOfTime(EnXc* self, PlayState* play) {
    if (gSaveContext.linkAge == 0) {
        if (!(gSaveContext.eventChkInf[12] & 0x20)) {
            gSaveContext.eventChkInf[12] |= 0x20;
            play->FUN_0037573c(play->sceneZARInfo.GetQDBByIndex(1));
            gSaveContext.cutsceneTrigger = 1;
            self->action = 0x14;
        } else if (!(gSaveContext.eventChkInf[5] & 0x20) && (gSaveContext.eventChkInf[4] & 0x100)) {
            gSaveContext.eventChkInf[5] |= 0x20;
            Item_Give(play, 0x5F);
            play->FUN_0037573c(play->sceneZARInfo.GetQDBByIndex(2));
            gSaveContext.cutsceneTrigger = 1;
            self->action = 0x1E;
        } else if (!(gSaveContext.eventChkInf[5] & 0x20)) {
            EnXc_SetupBlockPedestal(self);
        } else {
            Actor_Kill(&self->actor);
        }
    } else {
        Actor_Kill(&self->actor);
    }
}

void EnXc_Init(Actor* actor, GameState* state) {
    EnXc* self = (EnXc*)actor;
    PlayState* play = (PlayState*)state;
    void* zar;
    void* cmb;

    ActorShape_Init(&actor->shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    zar = &play->GetObjectStatus(actor->objBankIndex)->zarInfo;
    cmb = ZAR_GetCMBByIndex(zar, 0);
    FUN_00353e78(zar, play, &self->skelAnime, cmb, actor->unk_178, 0, self->unk_228, self->unk_604, 0x13);
    self->skelAnime.unk_75[0] = 0;
    SkelAnime_SetFaceAnimations((FaceAnimations*)self->unk_9E0, &self->skelAnime, 0, -1, -1);
    FUN_0036932c(self->skelAnime.unk_28, 6);
    FUN_0036932c(self->skelAnime.unk_28, 7);
    FUN_0036932c(self->skelAnime.unk_28, 2);
    FUN_0036932c(self->skelAnime.unk_28, 4);
    FUN_00372f38(actor, play, &self->skelAnimModel, 1, 0);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinderType1(play, &self->collider, actor, (ColliderCylinderInit*)EnXc_sCylinderInit);

    switch (actor->params) {
        case 0:
            break;
        case 1:
            self->action = 0x14;
            break;
        case 2:
            self->action = 0x1D;
            break;
        case 3:
            self->action = 0x2D;
            break;
        case 4:
            self->action = 0x35;
            self->unk_BFC = kREG(24) + 0x53FC;
            break;
        case 5:
            self->action = 0x39;
            self->drawMode = 5;
            break;
        case 6:
            if (!(gSaveContext.eventChkInf[5] & 1) && gSaveContext.linkAge == 0) {
                self->action = 0;
            } else {
                Actor_Kill(actor);
            }
            break;
        case 7:
            if (!(gSaveContext.eventChkInf[5] & 2) && gSaveContext.linkAge == 0) {
                self->action = 0;
            } else {
                Actor_Kill(actor);
            }
            break;
        case 8:
            if (!(gSaveContext.eventChkInf[5] & 4) && gSaveContext.linkAge == 0) {
                self->action = 0x1D;
            } else {
                Actor_Kill(actor);
            }
            break;
        case 9:
            EnXc_InitTempleOfTime(self, play);
            break;
        default:
            break;
    }
}

// N64 Destroy body through a helper taking the incoming Actor* (collider address computed from the param
// register lets armcc schedule the arg moves before the push, as in the ROM).
static inline void EnXc_DestroyCollider(Actor* thisx, PlayState* play) {
    EnXc* self = (EnXc*)thisx;

    Collider_DestroyCylinder(play, &self->collider);
}

void EnXc_Destroy(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnXc* self = (EnXc*)thisx;

    EnXc_DestroyCollider(thisx, play);
    SkelAnime_Free2(&self->skelAnime);
    ((FaceAnimations*)self->unk_9E0)->Destroy();
    FUN_003508b8(&self->actor, self->skelAnimModel, 0);
}
void EnXc_Update(Actor* actor, GameState* state) {
    EnXc* self = (EnXc*)actor;

    if ((u32)self->action < 0x51) {
        if (EnXc_sActionFuncs[self->action] != NULL) {
            EnXc_sActionFuncs[self->action](actor, state);
        }
    }
}
GLOBAL_ASM("../split/asm/FUN_001dc864.s")

void EnXc_Draw(Actor* actor, GameState* state) {
    EnXc* self = (EnXc*)actor;
    s32 mode = self->drawMode;

    if ((u32)mode < 6) {
        if (EnXc_sDrawFuncs[mode] != NULL) {
            FUN_0036932c(self->skelAnime.unk_28, 6);
            FUN_0036932c(self->skelAnime.unk_28, 7);
            FUN_0036932c(self->skelAnime.unk_28, 2);
            EnXc_sDrawFuncs[mode](actor, state);
        }
    }
}

void caseD_8_003238ac(EnXc* self, PlayState* play);
void caseD_d_00320fa0(EnXc* self, PlayState* play);
void caseD_10_0031a46c(EnXc* self, PlayState* play);

void caseD_43(Actor* actor, GameState* state) {
    EnXc* self = (EnXc*)actor;

    caseD_8_003238ac(self, (PlayState*)state);
    if (self->action != 0x43) {
        self->action = 0x44;
    }
}

void caseD_48(Actor* actor, GameState* state) {
    EnXc* self = (EnXc*)actor;

    caseD_d_00320fa0(self, (PlayState*)state);
    if (self->action != 0x48) {
        self->action = 0x49;
    }
}

void caseD_29(Actor* actor, GameState* state) {
    EnXc* self = (EnXc*)actor;

    caseD_10_0031a46c(self, (PlayState*)state);
    if (self->action != 0x29) {
        self->action = 0x2A;
    }
}

void caseD_21_0037e91c(Actor* actor, GameState* state) {
    EnXc* self = (EnXc*)actor;

    caseD_8_003238ac(self, (PlayState*)state);
    if (self->action != 0x21) {
        self->action = 0x22;
    }
}

void caseD_26(Actor* actor, GameState* state) {
    EnXc* self = (EnXc*)actor;

    caseD_d_00320fa0(self, (PlayState*)state);
    if (self->action != 0x26) {
        self->action = 0x27;
    }
}

void caseD_a_0031c07c(Actor* actor, GameState* state);

void caseD_23_0039d078(Actor* actor, GameState* state) {
    EnXc* self = (EnXc*)actor;

    caseD_a_0031c07c(actor, state);
    if (self->action != 0x23) {
        self->action = (self->action == 0xB) ? 0x24 : 0x22;
    }
}

void caseD_45(Actor* actor, GameState* state) {
    EnXc* self = (EnXc*)actor;

    caseD_a_0031c07c(actor, state);
    if (self->action != 0x45) {
        self->action = (self->action == 0xB) ? 0x46 : 0x44;
    }
}

void caseD_10_0031a46c(EnXc* self, PlayState* play);

void caseD_4b(Actor* actor, GameState* state) {
    EnXc* self = (EnXc*)actor;

    caseD_10_0031a46c(self, (PlayState*)state);
    if (self->action != 0x4B) {
        self->action = 0x4C;
    }
}

void caseD_3a(EnXc* self, PlayState* play) {
    FUN_003239a4(&self->actor, (GameState*)play, 4);
    EnXc_UpdateSkelAnime(self);
    EnXc_BgCheck(self, play);
    FUN_00326528(&self->actor, (GameState*)play);
}

void caseD_3b(EnXc* self, PlayState* play) {
    EnXc_UpdateSkelAnime(self);
    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    FUN_00326528(&self->actor, (GameState*)play);
}

void caseD_40(EnXc* self, PlayState* play) {
    EnXc_UpdateSkelAnime(self);
    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    FUN_00326528(&self->actor, (GameState*)play);
}

void caseD_7_0026c28c(EnXc* self, PlayState* play) {
    s32 animFinished = EnXc_UpdateSkelAnime(self);

    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    EnXc_SetupInitialHarpAction(self, animFinished);
}

void caseD_b_002b21a4(EnXc* self, PlayState* play) {
    s32 animFinished = EnXc_UpdateSkelAnime(self);

    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    EnXc_SetupPullOutHarpReverse(self, animFinished);
}

void caseD_c(EnXc* self, PlayState* play) {
    s32 animFinished = EnXc_UpdateSkelAnime(self);

    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    EnXc_SetupIdle(self, animFinished);
}

void caseD_9_0026c1d8(EnXc* self, PlayState* play) {
    EnXc_UpdateSkelAnime(self);
    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    EnXc_SetupHarpHold(self, play);
}

void caseD_20(EnXc* self, PlayState* play) {
    s32 animFinished = EnXc_UpdateSkelAnime(self);

    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    EnXc_SetupInitialHarpAction(self, animFinished);
    EnXc_CheckAndSetAction(self, 0x20, 0x21);
}

void caseD_24_0039a91c(EnXc* self, PlayState* play) {
    s32 animFinished = EnXc_UpdateSkelAnime(self);

    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    EnXc_SetupPullOutHarpReverse(self, animFinished);
    EnXc_CheckAndSetAction(self, 0x24, 0x25);
}

void caseD_25(EnXc* self, PlayState* play) {
    s32 animFinished = EnXc_UpdateSkelAnime(self);

    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    EnXc_SetupIdle(self, animFinished);
    EnXc_CheckAndSetAction(self, 0x25, 0x26);
}

void caseD_22_002b2390(EnXc* self, PlayState* play) {
    EnXc_UpdateSkelAnime(self);
    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    EnXc_SetupHarpHold(self, play);
    EnXc_CheckAndSetAction(self, 0x22, 0x23);
}


void caseD_6_00281bd4(EnXc* self, PlayState* play) {
    EnXc_UpdateSkelAnime(self);
    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    EnXc_SetupPullOutHarp(self, play);
}

void caseD_1f(EnXc* self, PlayState* play) {
    EnXc_UpdateSkelAnime(self);
    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    EnXc_SetupPullOutHarp(self, play);
    FUN_003239a4(&self->actor, (GameState*)play, 4);
    EnXc_CheckAndSetAction(self, 0x1F, 0x20);
}

void caseD_36(EnXc* self, PlayState* play) {
    EnXc_UpdateSkelAnime(self);
    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    if (EnXc_CheckForCue(self, play, 0xA, 4)) {
        SkelAnime* skelAnime = &self->skelAnime;

        Animation_Change_00375c08(skelAnime, 0xB, 1.0f, 0.0f, (f32)Animation_GetLastFrame(skelAnime, 0xB), 2, -8.0f);
        self->action = 0x37;
        self->drawMode = 4;
    }
}

void caseD_d_00320fa0(EnXc* self, PlayState* play) {
    EnXc_UpdateSkelAnime(self);
    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    FUN_0032cd68(&self->actor, (GameState*)play);
    EnXc_SetupReverseAccel(self, play);
}

void caseD_38(EnXc* self, PlayState* play) {
    EnXc_UpdateSkelAnime(self);
    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    FUN_003210a4(&self->actor, (GameState*)play);
    if (EnXc_CheckForCue(self, play, 9, 4)) {
        Actor_Kill(&self->actor);
    }
}

void caseD_3f(EnXc* self, PlayState* play) {
    EnXc_UpdateSkelAnime(self);
    s32 frames = play->csCtx.frames;
    if (frames == (s32)(4365.0f / R_UPDATE_RATE + 0.5f)) {
        FUN_003674e4(7);
    }
    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    FUN_00326528(&self->actor, (GameState*)play);
}

void caseD_1e(EnXc* self, PlayState* play) {
    PlayState* play2 = play;
    CutsceneContext* csCtx = &play2->csCtx;

    if (play2->GetCutsceneState()) {
        CsCmdActorAction* cue = csCtx->npcActions[4];

        if (cue != NULL && cue->action != 1) {
            PosRot* posRot = &self->actor.world;
            ActorShape* shape = &self->actor.shape;

            posRot->pos.x = cue->startPos.x;
            posRot->pos.y = cue->startPos.y;
            posRot->pos.z = cue->startPos.z;
            posRot->rot.y = shape->rot.y = cue->rot.y;
            self->action = 0x16;
            self->drawMode = 1;
        }
    }
    EnXc_CheckAndSetAction(self, 0x1E, 0x1F);
}

void caseD_35(EnXc* self, PlayState* play) {
    if (EnXc_CheckForNoCue(self, play, 1, 4)) {
        PlayState* play2 = play;
        CutsceneContext* csCtx = &play2->csCtx;

        if (play2->GetCutsceneState()) {
            CsCmdActorAction* cue = csCtx->npcActions[4];

            if (cue != NULL) {
                PosRot* posRot = &self->actor.world;
                ActorShape* shape = &self->actor.shape;

                posRot->pos.x = cue->startPos.x;
                posRot->pos.y = cue->startPos.y;
                posRot->pos.z = cue->startPos.z;
                posRot->rot.y = shape->rot.y = cue->rot.y;
            }
        }
        self->action = 0x36;
        self->drawMode = 1;
    }
}

void caseD_3_003a24d0(EnXc* self, PlayState* play) {
    FUN_0031cebc(&self->actor);
    EnXc_UpdateSkelAnime(self);
    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    FUN_0031ebe4(&self->actor, (GameState*)play);
    EnXc_SetupWalkSpeed(self, 4);
}

void caseD_e_00386ce0(EnXc* self, PlayState* play) {
    FUN_0031cebc(&self->actor);
    EnXc_UpdateSkelAnime(self);
    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    FUN_0031ebe4(&self->actor, (GameState*)play);
    FUN_0032cd68(&self->actor, (GameState*)play);
    EnXc_SetupWalkSpeed(self, 0xF);
}

void caseD_27_003a26f4(EnXc* self, PlayState* play) {
    FUN_0031cebc(&self->actor);
    EnXc_UpdateSkelAnime(self);
    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    FUN_0031ebe4(&self->actor, (GameState*)play);
    FUN_0032cd68(&self->actor, (GameState*)play);
    EnXc_SetupWalkSpeed(self, 0xF);
    EnXc_CheckAndSetAction(self, 0x27, 0x28);
}

void caseD_f_0037e71c(EnXc* self, PlayState* play) {
    Actor_MoveForward(&self->actor);
    EnXc_UpdateSkelAnime(self);
    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    FUN_0031ebe4(&self->actor, (GameState*)play);
    FUN_0032cd68(&self->actor, (GameState*)play);
    EnXc_SetupReverseHaltAction(self);
}

void caseD_42(EnXc* self, PlayState* play) {
    s32 animFinished = EnXc_UpdateSkelAnime(self);

    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    EnXc_SetupInitialHarpAction(self, animFinished);
    EnXc_CheckAndSetAction(self, 0x42, 0x43);
}

void caseD_44(EnXc* self, PlayState* play) {
    EnXc_UpdateSkelAnime(self);
    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    EnXc_SetupHarpHold(self, play);
    EnXc_CheckAndSetAction(self, 0x44, 0x45);
}

void caseD_41(EnXc* self, PlayState* play) {
    EnXc_UpdateSkelAnime(self);
    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    EnXc_SetupPullOutHarp(self, play);
    FUN_003239a4(&self->actor, (GameState*)play, 4);
    EnXc_CheckAndSetAction(self, 0x41, 0x42);
}

void caseD_49(EnXc* self, PlayState* play) {
    FUN_0031cebc(&self->actor);
    EnXc_UpdateSkelAnime(self);
    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    FUN_0031ebe4(&self->actor, (GameState*)play);
    FUN_0032cd68(&self->actor, (GameState*)play);
    EnXc_SetupWalkSpeed(self, 0xF);
    EnXc_CheckAndSetAction(self, 0x49, 0x4A);
}


void caseD_5_003a25a0(EnXc* self, PlayState* play) {
    EnXc_CalcXZSpeed(self);
    EnXc_UpdateSkelAnime(self);
    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    FUN_0031ebe4(&self->actor, (GameState*)play);
    EnXc_SetupStoppedAction(self);
}

void caseD_10_0031a46c(EnXc* self, PlayState* play) {
    EnXc_CalcXZSpeed(self);
    EnXc_UpdateSkelAnime(self);
    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    FUN_0031ebe4(&self->actor, (GameState*)play);
    FUN_0032cd68(&self->actor, (GameState*)play);
    EnXc_SetupNutThrow(self);
}


void caseD_30(EnXc* self, PlayState* play) {
    FUN_0031cebc(&self->actor);
    EnXc_UpdateSkelAnime(self);
    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    FUN_0031ebe4(&self->actor, (GameState*)play);
    EnXc_SetupWalkSpeed(self, 0x18);
    EnXc_CheckAndSetAction(self, 0x30, 0x31);
}
void caseD_8_003238ac(EnXc* self, PlayState* play) {
    s32 animFinished = EnXc_UpdateSkelAnime(self);

    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    EnXc_SetupPlayingHarpAction(self, play, animFinished);
}


void caseD_31(EnXc* self, PlayState* play) {
    Actor_MoveForward(&self->actor);
    EnXc_UpdateSkelAnime(self);
    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    FUN_0031ebe4(&self->actor, (GameState*)play);
    if (EnXc_CheckForCue(self, play, 0x10, 4)) {
        EnXc_ChangeAnimation(self, 0x11, 0, 0.0f, 0);
        self->action = 0x32;
    }
}

void caseD_37(EnXc* self, PlayState* play) {
    s32 animFinished = EnXc_UpdateSkelAnime(self);

    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    FUN_003210a4(&self->actor, (GameState*)play);
    EnXc_SetTriforceSFX(self);
    EnXc_SetupShowTriforceIdleAction(self, animFinished);
}

void caseD_4_0039ce8c(EnXc* self, PlayState* play) {
    Actor_MoveForward(&self->actor);
    EnXc_UpdateSkelAnime(self);
    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    FUN_0031ebe4(&self->actor, (GameState*)play);
    EnXc_SetupHaltAction(self);
}

s32 Gameplay_InCsMode(PlayState* play);

// N64 EnXc_MinuetCS. The `play2` alias declared inside the params block (passed only to Gameplay_InCsMode)
// schedules the `mov r0,play` arg setup like the ROM.
static inline s32 EnXc_MinuetCS(EnXc* self, PlayState* play) {
    Player* player;
    f32 playerPosZ;

    if (self->actor.params == 6) {
        PlayState* play2 = play;

        player = GET_PLAYER(play);
        playerPosZ = player->actor.world.pos.z;
        if (playerPosZ < -2225.0f) {
            if (!Gameplay_InCsMode(play2)) {
                play->FUN_0037573c(play->sceneZARInfo.GetQDBByIndex(0));
                gSaveContext.cutsceneTrigger = 1;
                gSaveContext.eventChkInf[5] |= 1;
                Item_Give(play, 0x5A);
                return 1;
            }
        }
        return 0;
    }
    return 1;
}

// N64 EnXc_BoleroCS (same block-scoped play2 alias).
static inline s32 EnXc_BoleroCS(EnXc* self, PlayState* play) {
    Player* player;
    PosRot* posRot;

    if (self->actor.params == 7) {
        PlayState* play2 = play;

        player = GET_PLAYER(play);
        posRot = &player->actor.world;
        if ((posRot->pos.x > -784.0f) && (posRot->pos.x < -584.0f) && (posRot->pos.y > 447.0f) &&
            (posRot->pos.y < 647.0f) && (posRot->pos.z > -446.0f) && (posRot->pos.z < -246.0f) &&
            !Gameplay_InCsMode(play2)) {
            play->FUN_0037573c(play->sceneZARInfo.GetQDBByIndex(1));
            gSaveContext.cutsceneTrigger = 1;
            gSaveContext.eventChkInf[5] |= 2;
            Item_Give(play, 0x5B);
            return 1;
        }
        return 0;
    }
    return 1;
}

// N64 EnXc_ActionFunc0
void caseD_0_0026c328(EnXc* self, PlayState* play) {
    FUN_0032cc6c(&self->actor, (GameState*)play);
    FUN_0032cacc(play);
    if (EnXc_MinuetCS(self, play) && EnXc_BoleroCS(self, play)) {
        self->action = 1;
    }
}

// N64 EnXc_SerenadeCS. Block-scoped play2 alias used for GET_PLAYER and Gameplay_InCsMode only.
static inline s32 EnXc_SerenadeCS(EnXc* self, PlayState* play) {
    if (self->actor.params == 8) {
        PlayState* play2 = play;
        Player* player = GET_PLAYER(play2);
        s32 stateFlags = player->stateFlags1;

        if (CHECK_OWNED_EQUIP(3, 1) && !(gSaveContext.eventChkInf[5] & 4) && !(stateFlags & 0x20000000) &&
            !Gameplay_InCsMode(play2)) {
            play->FUN_0037573c(play->sceneZARInfo.GetQDBByIndex(0));
            gSaveContext.cutsceneTrigger = 1;
            gSaveContext.eventChkInf[5] |= 4;
            Item_Give(play, 0x5C);
            return 1;
        } else {
            return 0;
        }
    }
    return 1;
}

// N64 EnXc_Serenade (func_80B3EE74 inlined)
void caseD_1d(EnXc* self, PlayState* play) {
    if (EnXc_SerenadeCS(self, play)) {
        self->action = 0x1E;
    }
}

#if 0 // Unmatched drafts (stage 2). These functions stay GLOBAL_ASM in src/functions/ (or asm/).
// 3DS joint table entry stride (0x34 bytes); only the y translation (+0x1C) is used here.
typedef struct {
    /* 0x00 */ f32 unk_00[13];
} EnXcJoint;

// N64 EnXc_SetupDisappear (Bolero CS end: fade away, else kill)
static inline void EnXc_SetupDisappear(EnXc* self, PlayState* play) {
    PlayState* play2 = play;
    CutsceneContext* csCtx = &play2->csCtx;

    if (play2->GetCutsceneState()) {
        CsCmdActorAction* cue = csCtx->npcActions[4];

        if (cue != NULL && cue->action == 9) {
            s16 sceneId = play->sceneNum;

            if (sceneId == 0x61) {
                self->action = 0x13;
                self->drawMode = 0;
                self->actor.shape.shadowAlpha = 0;
            } else {
                Actor_Kill(&self->actor);
            }
        }
    }
}

// N64 EnXc_SetupHarpPutawayAction
static inline void EnXc_SetupHarpPutawayAction(EnXc* self, PlayState* play) {
    f32 curFrame;
    f32 animFrameCount;

    if (EnXc_CheckForCue(self, play, 5, 4)) {
        curFrame = self->skelAnime.curFrame;
        animFrameCount = self->skelAnime.endFrame;
        if (curFrame >= animFrameCount) {
            SkelAnime* skelAnime = &self->skelAnime;

            Animation_Change_00375c08(skelAnime, 5, -1.0f, (f32)Animation_GetLastFrame(skelAnime, 5), 0.0f, 2, 0.0f);
            self->action = 0xB;
        }
    } else if (EnXc_CheckForNoCue(self, play, 8, 4)) {
        EnXc_SetupPlayingHarpAction(self, play, 1);
    }
}

// N64 EnXc_SetNutThrowSFX
static inline void EnXc_SetNutThrowSFX(EnXc* self, PlayState* play) {
    SkelAnime* skelAnime = &self->skelAnime;
    u32 sfxId;

    if (Animation_OnFrameImpl(skelAnime, 7.0f, 2.0f / 3.0f) && (self->actor.bgCheckFlags & 1)) {
        sfxId = 0x01000001;
        sfxId += FUN_00341df0(&play->colCtx, self->actor.floorPoly, self->actor.floorBgId);
        Audio_PlaySoundGeneral(sfxId, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               &DAT_0054ac24.unk_0);
    }
    if (Animation_OnFrameImpl(skelAnime, 20.0f, 2.0f / 3.0f)) {
        Audio_PlaySoundGeneral(0x0100055F, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               &DAT_0054ac24.unk_0);
    }
}

// N64 EnXc_SpawnNut
static inline void EnXc_SpawnNut(EnXc* self, PlayState* play) {
    Vec3f* pos = &self->actor.world.pos;
    s16 angle = self->actor.shape.rot.y;
    f32 x = (Math_SinS(angle) * 30.0f) + pos->x;
    f32 y = pos->y + 3.0f;
    f32 z = (Math_CosS(angle) * 30.0f) + pos->z;

    Actor_Spawn(&play->actorCtx, play, 0x16, x, y, z, 0xFA0, self->actor.shape.rot.y, 0, -10, 1);
}

// N64 func_80B3E164
static inline void EnXc_SetupDeleteAfterThrow(EnXc* self, PlayState* play) {
    f32* timer = &self->timer;

    (*timer)++;
    if (*timer >= 90.0f / R_UPDATE_RATE) {
        self->action = 0x12;
        EnXc_SpawnNut(self, play);
    }
}

// N64 func_80B3F668
static inline void EnXc_SetupReverseAccelInNocturne(EnXc* self, PlayState* play) {
    if (EnXc_CheckForCue(self, play, 4, 4)) {
        EnXc_ChangeAnimation(self, 1, 0, -36.0f / R_UPDATE_RATE, 1);
        self->action = 0x30;
        self->timer = 0.0f;
        self->actor.world.rot.y += 0x8000;
    }
}

// N64 func_80B3F534
static inline void EnXc_SpawnDoorWarp(PlayState* play) {
    CutsceneContext* csCtx = &play->csCtx;
    s32 curFrame = csCtx->frames;

    if (curFrame == (s32)(930.0f / R_UPDATE_RATE + 0.5f)) {
        Actor_Spawn(&play->actorCtx, play, 0x5D, -1044.0f, -1243.0f, 7458.0f, 0, 0, 0, 6, 1);
    }
}

// N64 func_80B3C620
static inline void EnXc_LerpPosFromCue(EnXc* self, PlayState* play, s32 channel) {
    CsCmdActorAction* cue = EnXc_GetCue(play, channel);
    Vec3f* worldPos = &self->actor.world.pos;
    f32 startX;
    f32 startY;
    f32 startZ;
    f32 endX;
    f32 endY;
    f32 endZ;
    f32 lerp;

    if (cue != NULL) {
        lerp = FUN_0032c66c(cue->endFrame, cue->startFrame, play->csCtx.frames, 0, 0);

        startX = cue->startPos.x;
        startY = cue->startPos.y;
        startZ = cue->startPos.z;

        endX = cue->endPos.x;
        endY = cue->endPos.y;
        endZ = cue->endPos.z;

        worldPos->x = ((endX - startX) * lerp) + startX;
        worldPos->y = ((endY - startY) * lerp) + startY;
        worldPos->z = ((endZ - startZ) * lerp) + startZ;
    }
}

// N64 EnXc_SetCrySFX
static inline void EnXc_SetCrySFX(EnXc* self, PlayState* play) {
    CutsceneContext* csCtx = &play->csCtx;
    s32 csCurFrame = csCtx->frames;

    if (csCurFrame == (s32)(2607.0f / R_UPDATE_RATE + 0.5f)) {
        Audio_PlaySoundGeneral(0x0100055B, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               &DAT_0054ac24.unk_0);
    } else if (csCurFrame == (s32)(2817.0f / R_UPDATE_RATE + 0.5f)) {
        Audio_PlaySoundGeneral(0x0100055C, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               &DAT_0054ac24.unk_0);
    }
}

// N64 func_80B3C924
static inline void EnXc_AddActorMovement(EnXc* self, PlayState* play) {
    *(u8*)((u8*)self + 0x1F6) |= 3;
    FUN_003fd1b8(play, &self->actor, &self->skelAnime, 1.0f);
}

// N64 func_80B3C8CC
static inline void EnXc_AddActorMovementIfGrounded(EnXc* self, PlayState* play) {
    PlayState* play2 = play;

    if (self->skelAnime.animationType == 2 && self->skelAnime.curFrame > 14.0f) {
        return;
    }
    {
        EnXcJoint* joint = &((EnXcJoint*)self->skelAnime.jointTable)[(u8)self->skelAnime.unk_75[0]];

        if (joint->unk_00[7] >= *(f32*)((u8*)&self->skelAnime + 0x68)) {
            *(u8*)((u8*)self + 0x1F6) |= 3;
            FUN_003fd1b8(play2, &self->actor, &self->skelAnime, 1.0f);
        }
    }
}

// 3DS Sfx_PlaySfxAtPos wrapper (N64 argument order)
static inline void EnXc_PlaySfxAtPos(Vec3f* pos, u32 sfxId) {
    Audio_PlaySoundGeneral(sfxId, pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
}

// N64 EnXc_SetLandingSFX
static inline void EnXc_SetLandingSFX(EnXc* self, PlayState* play) {
    u32 sfxId;
    s16 sceneId = play->sceneNum;

    if ((*(s32*)((u8*)&gSaveContext + 0x14E8) != 4) || (sceneId != 0x5C)) {
        if (Animation_OnFrameImpl(&self->skelAnime, 11.0f, 2.0f / 3.0f)) {
            sfxId = 0x01000001;
            sfxId += FUN_00341df0(&play->colCtx, self->actor.floorPoly, self->actor.floorBgId);
            EnXc_PlaySfxAtPos(&self->actor.world.pos, sfxId);
        }
    }
}

// N64 func_80B3D8A4
static inline void EnXc_SetupGracefulFallAction(EnXc* self, PlayState* play, s32 animFinished) {
    Actor* thisx = &self->actor;

    if (animFinished) {
        SkelAnime* skelAnime = &self->skelAnime;
        f32 frameCount = (f32)Animation_GetLastFrame(skelAnime, 1);

        Animation_Change_00375c08(skelAnime, 1, 1.0f, 0.0f, frameCount, 0, -8.0f);
        self->action = 3;
        self->timer = 0.0f;
        *(u8*)((u8*)self + 0x1F6) &= ~3;
        thisx->gravity = -((kREG(1) * 0.01f) + 13.0f);
        thisx->minVelocityY = -((kREG(1) * 0.01f) + 13.0f);
    } else {
        EnXc_AddActorMovementIfGrounded(self, play);
    }
}


// caseD_a_0031c07c (N64 ActionFunc10): block structure and instruction stream match, but callee-saved registers
// differ (ROM self=r6/play=r5/cue,animIdx=r4; build self=r4/play=r6/cue=r5): 36 words. ~40 source variants tried.
// Stage 3: ROM also computes `add r4,self,#0x1a4` (skelAnime, reusing the dead cue reg) BEFORE the curFrame>=endFrame
// compare; build 420 B vs 424. (EnXc*,PlayState*) signature is fine for callers caseD_23/45. Not solved.
void caseD_a_0031c07c(EnXc* self, PlayState* play) {
    EnXc_UpdateSkelAnime(self);
    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    EnXc_SetupHarpPutawayAction(self, play);
}

// caseD_11_003236d4 (N64 ActionFunc17, ThrowNut): stream matches, differences are fp callee-saved naming
// (ROM s16=30.0f/s17=2/3 vs build reversed) and r0/r1 temp choice: ~56 words.
void caseD_11_003236d4(EnXc* self, PlayState* play) {
    EnXc_UpdateSkelAnime(self);
    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    EnXc_SetNutThrowSFX(self, play);
    FUN_0032cd68(&self->actor, (GameState*)play);
    EnXc_SetupDeleteAfterThrow(self, play);
}

// caseD_12_0026bf1c / caseD_2b / caseD_4d (N64 EnXc_Delete flavours with SetupDisappear): ROM hoists `mov r7,#0`
// (zero shared by drawMode/shadowAlpha stores) to the function entry and pushes r7/r8; build rematerializes it
// in the branch. Everything else matches. caseD_33 has the same hoist (`mov sb,#0`).
// Stage 3 (Opus): the zero is materialised between the SkelAnime call and the BgCheck call (not at entry);
// in the eq path ROM uses r1 for 0x13 although r0 is dead. Tried zero/u8 locals, chained store, store order,
// dead ActorShape*/Actor* locals, and all 40 play/play2 alias placements: no change. Unsolved.
void caseD_12_0026bf1c(EnXc* self, PlayState* play) {
    EnXc_UpdateSkelAnime(self);
    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    FUN_0032cd68(&self->actor, (GameState*)play);
    EnXc_SetupDisappear(self, play);
}

void caseD_2b(EnXc* self, PlayState* play) {
    EnXc_UpdateSkelAnime(self);
    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    FUN_0032cd68(&self->actor, (GameState*)play);
    EnXc_SetupDisappear(self, play);
    EnXc_CheckAndSetAction(self, 0x2B, 0x2C);
}

void caseD_4d(EnXc* self, PlayState* play) {
    EnXc_UpdateSkelAnime(self);
    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    FUN_0032cd68(&self->actor, (GameState*)play);
    EnXc_SetupDisappear(self, play);
    EnXc_CheckAndSetAction(self, 0x4D, 0x4E);
}

// caseD_2f_ (N64 ActionFunc47): 10 words: gGameInfo ptr in r7/csCtx in r8 (build reversed) and r1 vs r0 temp.
// Stage 3 (Opus): ~25 more variants (play2 alias placements, SpawnDoorWarp frame-read forms, N64 store order,
// plain GetCue form) - none moved it.
void caseD_2f(EnXc* self, PlayState* play) {
    EnXc_SpawnDoorWarp(play);
    EnXc_UpdateSkelAnime(self);
    EnXc_BgCheck(self, play);
    FUN_003239a4(&self->actor, (GameState*)play, 4);
    EnXc_SetupReverseAccelInNocturne(self, play);
}

// caseD_33 (N64 ActionFunc51): same zero hoist as above plus self/play/pos-pointer register permutation.
void caseD_33(EnXc* self, PlayState* play) {
    EnXc_UpdateSkelAnime(self);
    EnXc_BgCheck(self, play);
    FUN_00319564(&self->actor, play);
    EnXc_LerpPosFromCue(self, play, 4);
    if (EnXc_CheckForCue(self, play, 9, 4)) {
        self->action = 0x34;
        self->drawMode = 0;
    }
}

// caseD_3c (N64 EnXc_Contort): ROM keeps csCtx (play+0x2298) in a register for the frame read in SetCrySFX; the
// build reads frames through the 0x22b8 literal instead.
void caseD_3c(EnXc* self, PlayState* play) {
    EnXc_SetCrySFX(self, play);
    EnXc_UpdateSkelAnime(self);
    FUN_00330370(&self->actor);
    if (!FUN_00326528(&self->actor, (GameState*)play)) {
        EnXc_AddActorMovement(self, play);
        EnXc_BgCheck(self, play);
    }
}



// caseD_2_002932ec (N64 ActionFunc2, GracefulFall): 21 words, only fp/int scratch register naming differs
// (-13.0f const in s2 vs s1, r0/r1 temps). Raw skeleton offsets are 3DS SkelAnime internals (+0x30 animationType,
// +0x68 baseTransl.y, +0x75 joint index, +0x78 jointTable with 0x34-byte entries, +0x52 movement flags).
void caseD_2_002932ec(EnXc* self, PlayState* play) {
    s32 animFinished = EnXc_UpdateSkelAnime(self);

    EnXc_BgCheck(self, play);
    FUN_00330370(&self->actor);
    EnXc_SetLandingSFX(self, play);
    FUN_0032cc6c(&self->actor, (GameState*)play);
    FUN_0032cacc(play);
    EnXc_SetupGracefulFallAction(self, play, animFinished);
}

// caseD_3d (N64 EnXc_FallInNocturne): not matched. Needs the frame table at 0x0052D2C8 ([7..10] = 6/20/25/31)
// via an EQU symbol; ROM plays 0x1000059 on frames [7]+[8] (+0x100055D on [7]) and 0x1000009 on [9]/[10],
// merged into a shared bl; then: UpdateSkelAnime, SetEyePattern, SFX, `if (!FUN_00326528(...)) {
// EnXc_AddActorMovementIfGrounded; EnXc_BgCheck }` (BgCheck tail-called).

#endif

void caseD_11_003236d4(EnXc* self, PlayState* play);

void caseD_2a(EnXc* self, PlayState* play) {
    caseD_11_003236d4(self, play);
    if (self->action != 0x2a) {
        self->action = 0x2b;
    }
}

// Empty action stubs (EnXc_sActionFuncs entries; split out of the preceding cfg-merged functions).
void FUN_0037e9a4(EnXc* self, PlayState* play) {
}

void FUN_003871a4(EnXc* self, PlayState* play) {
}

void FUN_003a3bdc(EnXc* self, PlayState* play) {
}

void FUN_0039d074(EnXc* self, PlayState* play) {
}

void FUN_003a28f0(EnXc* self, PlayState* play) {
}

void FUN_003a28f4(EnXc* self, PlayState* play) {
}

// Action setters (N64 func_80B3xxxx): `self->action = K`.
void FUN_0037ea2c(EnXc* self, PlayState* play) {
    self->action = 0x15;
}

void FUN_00391440(EnXc* self, PlayState* play) {
    self->action = 0x2e;
}

}
