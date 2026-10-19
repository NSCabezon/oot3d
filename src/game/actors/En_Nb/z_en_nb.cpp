extern "C" {
#include "z_en_nb.hpp"

#define FLAGS 0x00000010

void EnNb_Init(Actor* actor, GameState* state);
void EnNb_Destroy(Actor* actor, GameState* state);
void EnNb_Update(Actor* actor, GameState* state);
void EnNb_Draw(Actor* actor, GameState* state);

__attribute__((noinline)) void FUN_0033526c(EnNb* self);
void FUN_00318010(EnNb* self, PlayState* play);
void FUN_0034e01c(void* arg, void* pos);
void AnimationContext_SetMoveActor(PlayState* play, Actor* actor, SkelAnime* skelAnime, f32 speed);

GLOBAL_ASM("data/z_En_Nb.data.s")

ActorInit En_Nb_InitVars = {
    ACTOR_EN_NB,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_NB,
    sizeof(EnNb),
    (ActorFunc)EnNb_Init,
    (ActorFunc)EnNb_Destroy,
    (ActorFunc)EnNb_Update,
    (ActorFunc)EnNb_Draw,
};

GLOBAL_ASM("asm/EnNb_Init.s")

void EnNb_Destroy(Actor* thisx, GameState* state) {
    EnNb* self = (EnNb*)thisx;

    Collider_DestroyCylinder((PlayState*)state, (ColliderCylinder*)self->collider);
    SkelAnime_Free2(&self->skelAnime);
    ((FaceAnimations*)self->faceAnimations)->Destroy();
}

void EnNb_Update(Actor* thisx, GameState* state) {
    EnNb* self = (EnNb*)thisx;

    if ((u32)self->action < 0x1F) {
        ActorFunc* table = (ActorFunc*)0x0052CF10;
        ActorFunc func = table[self->action];

        if (func != NULL) {
            func(thisx, state);
        }
    }
}

void EnNb_Draw(Actor* thisx, GameState* state) {
    EnNb* self = (EnNb*)thisx;

    if ((u32)self->drawMode < 5) {
        ActorFunc* table = (ActorFunc*)0x0052CF8C;
        ActorFunc func = table[self->drawMode];

        if (func != NULL) {
            func(thisx, state);
        }
    }
}

s32 FUN_00297e90(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg) {
    return 0;
}

// EnNb_PostLimbDraw: head limb copies a point (0, 10, 0) through the limb matrix into focus.pos.
static inline void Nb_SetVec(Vec3f* v, f32 x, f32 y, f32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

void FUN_00256384(PlayState* play, s32 limbIndex, MTX34* mtx, EnNb* self) {
    if (limbIndex == 0xA) {
        EnNb* s = self;
        Vec3f vec1;
        Vec3f vec2;
        Nb_SetVec(&vec1, 0.0f, 10.0f, 0.0f);
        VEC3TransformAsm((Vector3f*)&vec2, mtx, (Vector3f*)&vec1);
        s->actor.focus.pos = vec2;
        s->actor.focus.rot.x = s->actor.world.rot.x;
        s->actor.focus.rot.y = s->actor.world.rot.y;
        s->actor.focus.rot.z = s->actor.world.rot.z;
    }
}

s32 FUN_00271e18(s32 unused, s32 id, s32 arg2, EnNb* self) {
    u8* ip = (u8*)self->unk_D7C;
    u8* pos;

    if (self->unk_D5C != 0) {
        if (id == 9) {
            pos = ip + 0xE;
            FUN_0034e01c((void*)arg2, pos);
        } else if (id == 0xA) {
            pos = ip + 0x8;
            FUN_0034e01c((void*)arg2, pos);
        }
    }

    return 0;
}

void FUN_0033526c(EnNb* self) {
    s16* blinkTimer = &self->blinkTimer;
    s16* eyeIdx = &self->eyeIdx;

    if (DECR(*blinkTimer) == 0) {
        *blinkTimer = Rand_S16Offset_003702c8(60, 60);
    }

    *eyeIdx = *blinkTimer;
    if (*eyeIdx >= 3) {
        *eyeIdx = 0;
    }
}

void caseD_2_002121a4(EnNb* self, PlayState* play) {
    {
        f32* yOffset = &self->actor.shape.yOffset;

        *yOffset += 250.0f / 3.0f;
    }
    FUN_003731e0(&self->skelAnime);
    FUN_0033526c(self);
    {
        f32* yOffset = &self->actor.shape.yOffset;

        if (*yOffset >= 0.0f) {
            self->action = 3;
            *yOffset = 0.0f;
        }
    }
}

// caseD_e_003a9088 (action 14 = N64 func_80AB2C18): eyes, move actor, bg check, confrontation check.
// Needs a `SkelAnime* skelAnime` local for the flag store (ROM adds r2 before the orr) + scoped play2 per call.
void caseD_e_003a9088(EnNb* self, PlayState* play) {
    FUN_0033526c(self);
    {
        PlayState* play2 = play;
        SkelAnime* skelAnime = &self->skelAnime;

        skelAnime->unk_50[2] |= 1;
        AnimationContext_SetMoveActor(play2, &self->actor, skelAnime, 1.0f);
    }
    {
        PlayState* play2 = play;

        Actor_UpdateBgCheckInfo(play2, &self->actor, 75.0f, 30.0f, 30.0f, 4);
    }
    FUN_00318010(self, play);
}

void FUN_0037266c(void* model, s32 idx);
void FUN_0036932c(void* model, s32 idx);

void FUN_00256384(PlayState* play, s32 limbIndex, MTX34* mtx, EnNb* self);

void caseD_1_00281938(EnNb* self, PlayState* play) {
    s16 eyeIdx = self->eyeIdx;
    SkelAnime* skelAnime = &self->skelAnime;

    if (self->skelAnime.animationType == 0x13 || self->skelAnime.animationType == 0x15) {
        FUN_0037266c(skelAnime->unk_28, 5);
        FUN_0036932c(skelAnime->unk_28, 4);
    }

    SkelAnime_DrawOpa(skelAnime, &self->actor.modelMtx, (OverrideLimbDrawOpa)FUN_00271e18,
                      (PostLimbDrawOpa)FUN_00256384, &self->actor, 0);
    FaceAnimations* fa = (FaceAnimations*)&self->faceAnimations;
    fa->FUN_0035e3a4(0, eyeIdx);
    fa = (FaceAnimations*)&self->faceAnimations;
    fa->FUN_0035e330();
}

void caseD_4_002930a8(EnNb* self, PlayState* play) {
    s16 eyeIdx = self->eyeIdx;
    SkelAnime* skelAnime = &self->skelAnime;

    FUN_0036932c(self->skelAnime.unk_28, 4);
    FUN_0037266c(skelAnime->unk_28, 5);

    SkelAnime_DrawOpa(skelAnime, &self->actor.modelMtx, (OverrideLimbDrawOpa)FUN_00297e90, (PostLimbDrawOpa)0,
                      &self->actor, 0);

    FaceAnimations* fa = (FaceAnimations*)&self->faceAnimations;
    fa->FUN_0035e3a4(0, eyeIdx);
    fa = (FaceAnimations*)&self->faceAnimations;
    fa->FUN_0035e330();
}

void FUN_0031cfb8(EnNb* self, PlayState* play);

#if 0 // caseD_7_002b1a90 (cfg tail split FUN_002b1b28): 13 words off; ROM does unpredicated `mov r1,#0` before the cue test (0 shared by alpha/shadowAlpha/NULL pos), scratch in r2 not r0
// N64 EnNb_SetupHide body (action 7): cue 4 on channel 1 switches to the seal-hide state.
void caseD_7_002b1a90(EnNb* self, PlayState* play) {
    CutsceneContext* csCtx = &play->csCtx;

    if (play->GetCutsceneState() != 0) {
        CsCmdActorAction* cue = csCtx->npcActions[1];
        s32 zero = 0;

        if (cue != NULL && cue->action == 4) {
            self->action = 8;
            self->drawMode = 2;
            self->alphaTimer = 0.0f;
            self->alpha = zero;
            self->actor.shape.shadowAlpha = zero;
            Audio_PlaySoundGeneral(0x010004B9, (Vec3f*)zero, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        }
    }
}
#endif

#if 0 // caseD_1_0024cdf4 (cfg tail split FUN_0024ce80): 3 words off (play2 around Spawn fixes arg order; R11: dead `play2x` alias declared BEFORE csCtx and used for GetCutsceneState fixes the entry `mov r0,r1` order, 5->3). Left: ROM cue in r1 (`ldrne r1,[r6,#0x48]; cmpne r1; ldrh r0,[r1]`), ours r0. 2026-10-14: N64 GetCue/SpawnBlueWarp helpers (inline/static), nested/early-return/cue-id helper forms, signatures, greedy mutations: all >=5. R11: greedy+pair mutations from the 3-word form, cue decl hoisted, npcActions array local, nested ifs, s2 store alias: no gain
// N64 EnNb_SetupChamberWarpImpl (action 1), with EnNb_SpawnBlueWarp inlined.
void caseD_1_0024cdf4(EnNb* self, PlayState* play) {
    PlayState* play2x = play;
    CutsceneContext* csCtx = &play->csCtx;

    if (play2x->GetCutsceneState() != 0) {
        CsCmdActorAction* cue = csCtx->npcActions[1];

        if (cue != NULL && cue->action == 2) {
            self->action = 2;
            self->drawMode = 1;
            {
                f32 posX = self->actor.world.pos.x;
                f32 posY = self->actor.world.pos.y;
                f32 posZ = self->actor.world.pos.z;

                PlayState* play2 = play;

                Actor_SpawnAsChild(&play2->actorCtx, &self->actor, play2, 0x5D, posX, posY, posZ, 0, 0, 0, 2);
            }
        }
    }
}
#endif


// N64 func_80AB23A8: cry SFX on frame 3 (3DS: 9.0 / R_UPDATE_RATE), then CheckKidnapCsMode tail call.
void caseD_a_00386c40(EnNb* self, GameState* state) {
    PlayState* play = (PlayState*)state;
    CutsceneContext* csCtx = &play->csCtx;

    if (csCtx->frames == (s32)(9.0f / SREG(30) + 0.5f)) {
        Audio_PlaySoundGeneral(0x01000546, (Vec3f*)&self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               &DAT_0054ac24.unk_0);
    }
    FUN_0031cfb8(self, play);
}

#if 0 // FUN_0031cfb8 (EnNb_CheckKidnapCsMode): draft 27/102 words, shape exact (cue/cueId branches: `EnNb* s2 = self` INSIDE `if (cue != NULL)` + `s2->cueId` store gives ROM's mov r6,r4 and kills the if-conversion; shape local before SetStartPos in case 1; play2 scoped in case 1). Remaining: callee-saved numbering only (ours play r6/s2 r7/0 r8/1 sb, ROM s2 r6/play r7/1 r8/0 sb)
s32 Animation_GetLastFrame(SkelAnime* skelAnime, void* anim);
void Animation_Change_00375c08(SkelAnime* skelAnime, void* anim, f32 speed, f32 startFrame, f32 endFrame, s32 mode,
                               f32 morphFrames);

static inline CsCmdActorAction* EnNb_GetCue31(PlayState* play, s32 cueChannel) {
    CutsceneContext* csCtx = &play->csCtx;

    if (play->GetCutsceneState() != 0) {
        return csCtx->npcActions[cueChannel];
    }
    return NULL;
}

static inline void EnNb_SetStartPosRotFromCue31(EnNb* self, PlayState* play, s32 cueChannel) {
    CsCmdActorAction* cue = EnNb_GetCue31(play, cueChannel);
    Actor* thisx = &self->actor;

    if (cue != NULL) {
        thisx->world.pos.x = cue->startPos.x;
        thisx->world.pos.y = cue->startPos.y;
        thisx->world.pos.z = cue->startPos.z;

        thisx->world.rot.y = thisx->shape.rot.y = cue->rot.y;
    }
}

static inline void EnNb_SetupCaptureCutsceneState31(EnNb* self, PlayState* play) {
    ActorShape* shape = &self->actor.shape;

    EnNb_SetStartPosRotFromCue31(self, play, 1);
    self->action = 0xA;
    self->drawMode = 0;
    shape->shadowAlpha = 0;
}

static inline void EnNb_SetupLookAroundInKidnap31(EnNb* self) {
    SkelAnime* skelAnime = &self->skelAnime;

    Animation_Change_00375c08(skelAnime, (void*)0x14, 1.0f, 0.0f,
                              Animation_GetLastFrame(skelAnime, (void*)0x14), 0, -8.0f);
    self->action = 0xB;
    self->drawMode = 1;
}

static inline void EnNb_SetupKidnap31(EnNb* self) {
    SkelAnime* skelAnime = &self->skelAnime;

    Animation_Change_00375c08(skelAnime, (void*)0x15, 1.0f, 0.0f,
                              Animation_GetLastFrame(skelAnime, (void*)0x15), 2, -8.0f);
    self->action = 0xC;
    self->drawMode = 1;
}

void FUN_0031cfb8(EnNb* self, PlayState* play) {
    CsCmdActorAction* cue = EnNb_GetCue31(play, 1);
    s32 nextCueId;
    s32 currentCueId;

    if (cue != NULL) {
        EnNb* s2 = self;

        nextCueId = cue->action;
        currentCueId = self->cueId;

        if (nextCueId != currentCueId) {
            switch (nextCueId) {
                case 1: {
                    PlayState* play2 = play;

                    EnNb_SetupCaptureCutsceneState31(self, play2);
                    break;
                }
                case 7:
                    EnNb_SetupLookAroundInKidnap31(self);
                    break;
                case 8:
                    EnNb_SetupKidnap31(self);
                    break;
                case 9:
                    Actor_Kill(&self->actor);
                    break;
            }
            s2->cueId = nextCueId;
        }
    }
}
#endif

#if 0 // FUN_00318010 (EnNb_CheckConfrontationCsMode): draft 105/175 words, size 700 = ROM (s2 at function top + lastFrame local then shape ptr in each setup stops the if-conversion). Remaining: ROM's `mov r6,r4` self copy for the cueId store (s2 inside the if re-enables if-conversion here), float regs (ROM 0.0=s16, 1.0=s18, lastFrame s17), ROM doesn't hoist 3, SetStartPos stores via r4 not a thisx copy
s32 Animation_GetLastFrame(SkelAnime* skelAnime, void* anim);
void Animation_Change_00375c08(SkelAnime* skelAnime, void* anim, f32 speed, f32 startFrame, f32 endFrame, s32 mode,
                               f32 morphFrames);

static inline CsCmdActorAction* EnNb_GetCue18(PlayState* play, s32 cueChannel) {
    CutsceneContext* csCtx = &play->csCtx;

    if (play->GetCutsceneState() != 0) {
        return csCtx->npcActions[cueChannel];
    }
    return NULL;
}

static inline void EnNb_SetStartPosRotFromCue18(EnNb* self, PlayState* play, s32 cueChannel) {
    CsCmdActorAction* cue = EnNb_GetCue18(play, cueChannel);
    Actor* thisx = &self->actor;

    if (cue != NULL) {
        thisx->world.pos.x = cue->startPos.x;
        thisx->world.pos.y = cue->startPos.y;
        thisx->world.pos.z = cue->startPos.z;

        thisx->world.rot.y = thisx->shape.rot.y = cue->rot.y;
    }
}

static inline void EnNb_Setup18_01(EnNb* self) {
    ActorShape* shape = &self->actor.shape;
    self->action = 0xD;
    self->drawMode = 0;
    shape->shadowAlpha = 0;
}

static inline void EnNb_Setup18_0A(EnNb* self, PlayState* play) {
    SkelAnime* skelAnime = &self->skelAnime;
    f32 lastFrame = Animation_GetLastFrame(skelAnime, (void*)0x10);
    ActorShape* shape = &self->actor.shape;

    EnNb_SetStartPosRotFromCue18(self, play, 1);
    Animation_Change_00375c08(skelAnime, (void*)0x10, 1.0f, 0.0f, lastFrame, 2, 0.0f);
    self->action = 0xE;
    self->drawMode = 3;
    shape->shadowAlpha = 255;
}

static inline void EnNb_Setup18_0B(EnNb* self) {
    SkelAnime* skelAnime = &self->skelAnime;
    f32 lastFrame = Animation_GetLastFrame(skelAnime, (void*)0x10);
    ActorShape* shape = &self->actor.shape;

    Animation_Change_00375c08(skelAnime, (void*)0x10, 1.0f, 0.0f, lastFrame, 2, 0.0f);
    self->action = 0xF;
    self->drawMode = 3;
    shape->shadowAlpha = 255;
}

static inline void EnNb_Setup18_0C(EnNb* self) {
    SkelAnime* skelAnime = &self->skelAnime;
    f32 lastFrame = Animation_GetLastFrame(skelAnime, (void*)6);
    ActorShape* shape = &self->actor.shape;

    Animation_Change_00375c08(skelAnime, (void*)6, 1.0f, 0.0f, lastFrame, 2, -8.0f);
    self->action = 0x10;
    self->drawMode = 1;
    shape->shadowAlpha = 255;
}

static inline void EnNb_Setup18_0D(EnNb* self) {
    SkelAnime* skelAnime = &self->skelAnime;
    f32 lastFrame = Animation_GetLastFrame(skelAnime, (void*)4);
    ActorShape* shape = &self->actor.shape;

    Animation_Change_00375c08(skelAnime, (void*)4, 1.0f, 0.0f, lastFrame, 2, -8.0f);
    self->action = 0x11;
    self->drawMode = 4;
    shape->shadowAlpha = 255;
}

static inline void EnNb_Setup18_0E(EnNb* self) {
    SkelAnime* skelAnime = &self->skelAnime;
    f32 lastFrame = Animation_GetLastFrame(skelAnime, (void*)0xD);
    ActorShape* shape = &self->actor.shape;

    Animation_Change_00375c08(skelAnime, (void*)0xD, 1.0f, 0.0f, lastFrame, 2, -8.0f);
    self->action = 0x12;
    self->drawMode = 4;
    shape->shadowAlpha = 255;
}

static inline void EnNb_Setup18_09(EnNb* self) {
    ActorShape* shape = &self->actor.shape;
    self->action = 0x13;
    self->drawMode = 0;
    shape->shadowAlpha = 0;
}

void FUN_00318010(EnNb* self, PlayState* play) {
    CsCmdActorAction* cue = EnNb_GetCue18(play, 1);
    s32 nextCueId;
    s32 currentCueId;
    EnNb* s2 = self;

    if (cue != NULL) {
        nextCueId = cue->action;
        currentCueId = self->cueId;

        if (nextCueId != currentCueId) {
            switch (nextCueId) {
                case 1:
                    EnNb_Setup18_01(self);
                    break;
                case 10:
                    EnNb_Setup18_0A(self, play);
                    break;
                case 11:
                    EnNb_Setup18_0B(self);
                    break;
                case 12:
                    EnNb_Setup18_0C(self);
                    break;
                case 13:
                    EnNb_Setup18_0D(self);
                    break;
                case 14:
                    EnNb_Setup18_0E(self);
                    break;
                case 9:
                    EnNb_Setup18_09(self);
                    break;
            }
            s2->cueId = nextCueId;
        }
    }
}
#endif

void caseD_3_00281a5c(EnNb* self, PlayState* play) {
    u8 index = 3;
    SkelAnime* skelAnime = &self->skelAnime;
    FaceAnimations* fa = (FaceAnimations*)&self->faceAnimations;

    fa->FUN_0035e3a4(0, index);
    fa = (FaceAnimations*)&self->faceAnimations;
    fa->FUN_0035e330();
    FUN_0037266c(self->skelAnime.unk_28, 5);
    FUN_0036932c(self->skelAnime.unk_28, 4);
    SkelAnime_DrawOpa(skelAnime, &self->actor.modelMtx, (OverrideLimbDrawOpa)0, (PostLimbDrawOpa)0, &self->actor, 0);
}


s32 Animation_GetLastFrame(SkelAnime* skelAnime, void* anim);
__attribute__((noinline)) void EnNb_SetCurrentAnim(Actor* thisx, void* animation, s32 mode, f32 morphFrames, s32 arg4);
void Animation_Change_00375c08(SkelAnime* skelAnime, void* anim, f32 speed, f32 startFrame, f32 endFrame, s32 mode,
                               f32 morphFrames);

void EnNb_SetCurrentAnim(Actor* thisx, void* animation, s32 mode, f32 morphFrames, s32 arg4) {
    EnNb* self = (EnNb*)thisx;
    f32 frameCount;
    f32 playbackSpeed;
    f32 unk0;
    f32 fc;
    SkelAnime* skelAnime = &self->skelAnime;

    frameCount = Animation_GetLastFrame(skelAnime, animation);

    if (!arg4) {
        unk0 = 0.0f;
        fc = frameCount;
        playbackSpeed = 1.0f;
    } else {
        unk0 = frameCount;
        fc = 0.0f;
        playbackSpeed = -1.0f;
    }

    Animation_Change_00375c08(skelAnime, animation, playbackSpeed, unk0, fc, mode, morphFrames);
}

void FUN_00342be0(Actor* actor, s32 mode, s32 type, f32 r, f32 g, f32 b, f32 a);
void Item_Give(PlayState* play, s32 item);

// N64 EnNb_SetupChamberCsImpl: spirit medallion chamber cutscene (3DS: GetObjectStatus + ZAR lookup).
void caseD_0_0024cec0(EnNb* self, PlayState* play) {
    Player* player;
    u8 a = gSaveContext.unk_15A3[0];
    if (a == CHAMBER_CS_SPIRIT && gSaveContext.sceneSetupIndex < 4) {
        PlayState* play2 = play;

        player = GET_PLAYER(play2);
        self->action = 1;
        play2->FUN_0037573c(play2->GetObjectStatus(self->actor.objBankIndex)->zarInfo.GetQDBByIndex(0));
        gSaveContext.cutsceneTrigger = 2;
        Item_Give(play2, ITEM_MEDALLION_SPIRIT);
        player->actor.world.rot.y = player->actor.shape.rot.y = self->actor.world.rot.y + 0x8000;
    }
}

static inline void EnNb_FaceEyes14(EnNb* self, s32 eyeIdx) {
    FaceAnimations* face = (FaceAnimations*)&self->faceAnimations;

    face->FUN_0035e3a4(0, eyeIdx);
    face->FUN_0035e330();
}


// N64 EnNb_LookLeft: eyes, skel update, loop anim 5 on finish, move + bg check, then confrontation check.

void caseD_11_003aa4c4(EnNb* self, PlayState* play) {
    s32 animFinished;

    FUN_0033526c(self);
    animFinished = FUN_003731e0(&self->skelAnime);
    f32 speed = 1.0f;

    if (animFinished) {
        SkelAnime* skelAnime = &self->skelAnime;

        Animation_Change_00375c08(skelAnime, (void*)5, speed, 0.0f, Animation_GetLastFrame(skelAnime, (void*)5), 0,
                                  0.0f);
    }
    {
        PlayState* play2 = play;

        self->skelAnime.unk_50[2] |= 1;
        AnimationContext_SetMoveActor(play2, &self->actor, &self->skelAnime, speed);
    }
    {
        PlayState* play2 = play;

        Actor_UpdateBgCheckInfo(play2, &self->actor, 75.0f, 30.0f, 30.0f, 4);
    }
    FUN_00318010(self, play);
}

// caseD_f_003a51b8: Kneel-like (anim 8, drawMode 3), bound SFX on frames 18/25 in ONCE mode, move + bg check.
void caseD_f_003a51b8(EnNb* self, PlayState* play) {
    s32 animFinished;
    f32 speed;

    FUN_0033526c(self);
    animFinished = FUN_003731e0(&self->skelAnime);
    speed = 1.0f;
    if (animFinished) {
        SkelAnime* skelAnime = &self->skelAnime;

        Animation_Change_00375c08(skelAnime, (void*)8, speed, 0.0f, Animation_GetLastFrame(skelAnime, (void*)8), 0,
                                  0.0f);
        self->drawMode = 3;
    }
    {
        u8 mode = self->skelAnime.unk_50[0x20];
        SkelAnime* skel = &self->skelAnime;

        if (mode == 2 && (Animation_OnFrameImpl(skel, 18.0f, speed) || Animation_OnFrameImpl(skel, 25.0f, speed))) {
            Audio_PlaySoundGeneral(0x010001ED, (Vec3f*)&self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                                   &DAT_0054ac24.unk_0);
        }
    }
    {
        PlayState* play2 = play;

        self->skelAnime.unk_50[2] |= 1;
        AnimationContext_SetMoveActor(play2, &self->actor, &self->skelAnime, speed);
    }
    {
        PlayState* play2 = play;

        Actor_UpdateBgCheckInfo(play2, &self->actor, 75.0f, 30.0f, 30.0f, 4);
    }
    FUN_00318010(self, play);
}

// caseD_12_003a36c0: cue-frame SFX (frames == 9 / SREG(30)), eyes, skel update, sound on OnFrame, move + bg check,
// then one-shot Demo_6K spawn when the skel update finished (flag at 0xCF8).
void caseD_12_003a36c0(EnNb* self, PlayState* play) {
    CutsceneContext* csCtx = &play->csCtx;
    s32 animFinished;
    f32 speed;

    if (csCtx->frames == (s32)(1644.0f / SREG(30) + 0.5f)) {
        Audio_PlaySoundGeneral(0x01000546, (Vec3f*)&self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               &DAT_0054ac24.unk_0);
        Audio_PlaySoundGeneral(0x01000318, (Vec3f*)&self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               &DAT_0054ac24.unk_0);
    }
    FUN_0033526c(self);
    animFinished = FUN_003731e0(&self->skelAnime);
    speed = 1.0f;
    {
        SkelAnime* skel = &self->skelAnime;

        if (Animation_OnFrameImpl(skel, 9.0f, speed) || Animation_OnFrameImpl(skel, 13.0f, speed)) {
            Audio_PlaySoundGeneral(0x01000003, (Vec3f*)&self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                                   &DAT_0054ac24.unk_0);
        }
    }
    {
        PlayState* play2 = play;

        self->skelAnime.unk_50[2] |= 1;
        AnimationContext_SetMoveActor(play2, &self->actor, &self->skelAnime, speed);
    }
    {
        PlayState* play2 = play;

        Actor_UpdateBgCheckInfo(play2, &self->actor, 75.0f, 30.0f, 30.0f, 4);
    }
    if (self->unk_CF8 == 0 && animFinished) {
        Vec3f* pos = &self->actor.world.pos;

        Actor_Spawn(&play->actorCtx, play, 0xF5, pos->x, kREG(21) + 22.0f + self->actor.world.pos.y, pos->z, 0, 0, 0,
                    0xB, 1);
        s32* flag = &self->unk_CF8;
        *flag = 1;
    }
    FUN_00318010(self, play);
}

// N64 EnNb_Kneel: eyes, skel update, on-all-fours loop anim 7 + draw kneel on finish, bound SFX, move + bg check.
void caseD_10_003aa694(EnNb* self, PlayState* play) {
    s32 animFinished;
    f32 speed;

    FUN_0033526c(self);
    animFinished = FUN_003731e0(&self->skelAnime);
    speed = 1.0f;
    if (animFinished) {
        SkelAnime* skelAnime = &self->skelAnime;

        Animation_Change_00375c08(skelAnime, (void*)7, speed, 0.0f, Animation_GetLastFrame(skelAnime, (void*)7), 0,
                                  0.0f);
        self->drawMode = 4;
    }
    {
        u8 mode = self->skelAnime.unk_50[0x20];
        SkelAnime* skel = &self->skelAnime;

        if (mode == 2 && Animation_OnFrameImpl(skel, 9.0f, speed)) {
            Audio_PlaySoundGeneral(0x01000003, (Vec3f*)&self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                                   &DAT_0054ac24.unk_0);
        }
    }
    {
        PlayState* play2 = play;

        self->skelAnime.unk_50[2] |= 1;
        AnimationContext_SetMoveActor(play2, &self->actor, &self->skelAnime, speed);
    }
    {
        PlayState* play2 = play;

        Actor_UpdateBgCheckInfo(play2, &self->actor, 75.0f, 30.0f, 30.0f, 4);
    }
    FUN_00318010(self, play);
}

f32 FUN_0032c66c(u16 endFrame, u16 startFrame, u16 curFrame, s32 arg3, s32 arg4);

#if 0 // caseD_15_002a4170 (cfg tail split FUN_002a427c): 15 words off, ptr regs (ROM timer r0/alpha r2/shadow r1, int temp r0)
// N64 EnNb_Fade (3DS): bg check, skel/eyes, alpha timer ramps up, then go to action 22 once full.
void caseD_15_002a4170(EnNb* self, PlayState* play) {
    Actor_UpdateBgCheckInfo(play, &self->actor, 75.0f, 30.0f, 30.0f, 4);
    FUN_003731e0(&self->skelAnime);
    FUN_0033526c(self);
    f32* alphaTimer = &self->alphaTimer;
    *alphaTimer += 1.0f;
    s32* alphaPtr = &self->alpha;
    u8* shadowAlpha = &self->actor.shape.shadowAlpha;
    if (*alphaTimer >= kREG(17) + 10.0f) {
        *alphaPtr = 255;
        *shadowAlpha = 255;
    } else {
        s32 alpha = (*alphaTimer / (kREG(17) + 10.0f)) * 255.0f;

        *alphaPtr = alpha;
        *shadowAlpha = alpha;
    }
    {
        f32* alphaTimer2 = &self->alphaTimer;

        if (*alphaTimer2 >= kREG(17) + 10.0f) {
            self->action = 0x16;
            self->drawMode = 1;
        }
    }
}
#endif


// N64 EnNb_(cry/agony) cue frames: two SFX checks on csCtx->frames, then eyes, skel update, FUN_0031cfb8.
void caseD_b_003a22b4(EnNb* self, PlayState* play) {
    PlayState* play2 = play;
    CutsceneContext* csCtx = &play2->csCtx;

    if (csCtx->frames == (s32)(9.0f / SREG(30) + 0.5f)) {
        Audio_PlaySoundGeneral(0x01000546, (Vec3f*)&self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               &DAT_0054ac24.unk_0);
    }
    if (csCtx->frames == (s32)(1260.0f / SREG(30) + 0.5f)) {
        Audio_PlaySoundGeneral(0x01000545, (Vec3f*)&self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               &DAT_0054ac24.unk_0);
    }
    FUN_0033526c(self);
    FUN_003731e0(&self->skelAnime);
    FUN_0031cfb8(self, play);
}


static inline CsCmdActorAction* EnNb_GetCueC(PlayState* play, s32 cueChannel) {
    CutsceneContext* csCtx = &play->csCtx;

    if (play->GetCutsceneState() != 0) {
        return csCtx->npcActions[cueChannel];
    }
    return NULL;
}

static inline void EnNb_SetPosInPortalC(EnNb* self, PlayState* play) {
    CutsceneContext* csCtx = &play->csCtx;
    CsCmdActorAction* cue = EnNb_GetCueC(play, 1);

    if (cue != NULL) {
        Vec3f* pos = &self->actor.world.pos;
        f32 lerp;
        Vec3f startPos;
        Vec3f endPos;

        lerp = FUN_0032c66c(cue->endFrame, cue->startFrame, csCtx->frames, 4, 4);
        startPos.x = cue->startPos.x;
        startPos.y = cue->startPos.y;
        startPos.z = cue->startPos.z;

        endPos.x = cue->endPos.x;
        endPos.y = cue->endPos.y;
        endPos.z = cue->endPos.z;

        pos->x = ((endPos.x - startPos.x) * lerp) + startPos.x;
        pos->y = ((endPos.y - startPos.y) * lerp) + startPos.y;
        pos->z = ((endPos.z - startPos.z) * lerp) + startPos.z;
    }
}

// N64 EnNb_MovingInPortal + EnNb_SetPosInPortal: anim 19 on finish, lerp pos along cue, then CheckKidnapCsMode.
// Needs: csCtx local in GetCue AND in SetPosInPortal (ROM hoists csCtx into r8), pos declared inside the `if`,
// play2 scoped around the SetPosInPortal call (mov r0,play between the two csCtx adds).
void caseD_c_00386b28(EnNb* self, PlayState* play) {
    FUN_0033526c(self);
    if (FUN_003731e0(&self->skelAnime)) {
        SkelAnime* skelAnime = &self->skelAnime;

        Animation_Change_00375c08(skelAnime, (void*)0x13, 1.0f, 0.0f, Animation_GetLastFrame(skelAnime, (void*)0x13), 0,
                                  0.0f);
    }
    {
        PlayState* play2 = play;

        EnNb_SetPosInPortalC(self, play2);
    }
    FUN_0031cfb8(self, play);
}


// N64 EnNb_DrawTransparency (3DS): alpha-scaled white env, opaque skel draw, then eyes.
void caseD_2_0026b1dc(EnNb* self, PlayState* play) {
    s16 eyeIdx = self->eyeIdx;
    SkelAnime* skelAnime = &self->skelAnime;

    FUN_00342be0(&self->actor, 4, 2, 1.0f, 1.0f, 1.0f, (f32)(u32)self->alpha * (1.0f / 255.0f));
    SkelAnime_DrawOpa(skelAnime, &self->actor.modelMtx, (OverrideLimbDrawOpa)0, (PostLimbDrawOpa)0, &self->actor, 0);
    FaceAnimations* fa = (FaceAnimations*)&self->faceAnimations;
    fa->FUN_0035e3a4(0, eyeIdx);
    fa = (FaceAnimations*)&self->faceAnimations;
    fa->FUN_0035e330();
}

#if 0 // caseD_14_00293124: draft 29/59 words (Actor* sig, play2 scoped at case 15, alias `s` for FadeIn stores gives ROM's mov r7,r5 copy); missing: case-16 self copy (mov r4,r5) and ROM's self=r5/zero=r4 order, ROM if-converts the cue/cueId check here
// N64 EnNb_CheckCreditsCsModeImpl: credits cue 15 (fade in, reposition from cue) / 16 (head turn).
static inline CsCmdActorAction* EnNb_GetCue14(PlayState* play, s32 cueChannel) {
    CutsceneContext* csCtx = &play->csCtx;

    if (play->GetCutsceneState() != 0) {
        return csCtx->npcActions[cueChannel];
    }
    return NULL;
}

static inline void EnNb_SetStartPosRotFromCue14(EnNb* self, PlayState* play, s32 cueChannel) {
    CsCmdActorAction* cue = EnNb_GetCue14(play, cueChannel);
    Actor* thisx = &self->actor;

    if (cue != NULL) {
        thisx->world.pos.x = cue->startPos.x;
        thisx->world.pos.y = cue->startPos.y;
        thisx->world.pos.z = cue->startPos.z;

        thisx->world.rot.y = thisx->shape.rot.y = cue->rot.y;
    }
}

static inline void EnNb_SetupCreditsFadeIn14(EnNb* self, PlayState* play) {
    EnNb* s = self;

    EnNb_SetStartPosRotFromCue14(self, play, 1);
    s->action = 0x15;
    s->drawMode = 2;
}

static inline void EnNb_SetupCreditsHeadTurn14(EnNb* self) {
    EnNb_SetCurrentAnim(&self->actor, (void*)0xC, 2, -8.0f, 0);
    self->action = 0x17;
}

void caseD_14_00293124(Actor* thisx, PlayState* play) {
    EnNb* self = (EnNb*)thisx;
    CsCmdActorAction* cue = EnNb_GetCue14(play, 1);
    s32 nextCueId;
    s32 currentCueId;

    if (cue != NULL) {
        nextCueId = cue->action;
        currentCueId = ((EnNb*)thisx)->cueId;

        if (nextCueId != currentCueId) {
            switch (nextCueId) {
                case 15: {
                    PlayState* play2 = play;

                    EnNb_SetupCreditsFadeIn14(self, play2);
                    break;
                }
                case 16:
                    EnNb_SetupCreditsHeadTurn14(self);
                    break;
            }
            self->cueId = nextCueId;
        }
    }
}
#endif

#if 0 // caseD_1c_002a439c (action 28 = EnNb_FollowPath): draft ~0.84 structural match, 920 vs 928 B. N64 helpers inlined + NB_FRAMES macro (n*3/2 rounding, kREG(17)+25 u16). Remaining: ROM keeps fewer hoisted pointers (push r4-sl only, no fp), s16/s17 consts + one stack slot; header got typed initialPos/finalPos/pathYaw/movementTimer/headRot/torsoRot
f32 FUN_0032c66c(u16 endFrame, u16 startFrame, u16 curFrame, s32 arg3, s32 arg4);

#define NB_PATH_N ((u16)(kREG(17) + 25))
#define NB_FRAMES(x) (((x) > 0) ? (s32)(0.5f + ((x) * 3.0f) * 0.5f) : (s32)(((x) * 3.0f) * 0.5f - 0.5f))

static inline void func_80AB359C(EnNb* self) {
    PosRot* world = &self->actor.world;
    Vec3f* initialPos = &self->initialPos;
    Vec3f* finalPos = &self->finalPos;
    f32 lerp;

    self->movementTimer++;

    if (self->movementTimer <= NB_FRAMES(NB_PATH_N)) {
        lerp = FUN_0032c66c(NB_FRAMES(NB_PATH_N), 0, self->movementTimer, 3, 3);

        world->pos.x = initialPos->x + (lerp * (finalPos->x - initialPos->x));
        world->pos.y = initialPos->y + (lerp * (finalPos->y - initialPos->y));
        world->pos.z = initialPos->z + (lerp * (finalPos->z - initialPos->z));
    }
}

static inline void func_80AB1284(EnNb* self, PlayState* play) {
    Actor_UpdateBgCheckInfo(play, &self->actor, 75.0f, 30.0f, 30.0f, 4);
}

static inline void EnNb_UpdateCollider(EnNb* self, PlayState* play) {
    ColliderCylinder* collider = (ColliderCylinder*)self->collider;

    Collider_UpdateCylinder(&self->actor, collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &collider->base);
}

static inline void func_80AB36DC(EnNb* self, PlayState* play) {
    u16 moveTime = self->movementTimer;

    if ((u16)(NB_FRAMES(NB_PATH_N) - 4) > moveTime) {
        s16 invScale = 4 - moveTime;

        if (invScale > 0) {
            Math_SmoothStepToS(&self->actor.shape.rot.y, self->pathYaw, invScale, 6200, 100);
        }
    } else {
        s16 invScale = (u16)NB_FRAMES(NB_PATH_N) - moveTime;

        if (invScale > 0) {
            Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.home.rot.y, invScale, 6200, 100);
        }
    }
}

static inline void func_80AB10C4(EnNb* self) {
    s16* headRot = self->headRot;
    s16* torsoRot = self->torsoRot;

    Math_SmoothStepToS(&headRot[0], 0, 20, 6200, 100);
    Math_SmoothStepToS(&headRot[1], 0, 20, 6200, 100);
    Math_SmoothStepToS(&torsoRot[0], 0, 20, 6200, 100);
    Math_SmoothStepToS(&torsoRot[1], 0, 20, 6200, 100);
}

static inline void func_80AB3A7C(EnNb* self, PlayState* play, s32 animFinished) {
    u16 movementTimer = self->movementTimer;

    if (NB_FRAMES(NB_PATH_N) > movementTimer) {
        if (animFinished) {
            EnNb_SetCurrentAnim(&self->actor, (void*)9, 0, 0.0f, 0);
        }
    } else {
        EnNb_SetCurrentAnim(&self->actor, (void*)0x11, 0, -8.0f, 0);
        self->action = 0x1D;
    }
}

void caseD_1c_002a439c(EnNb* self, PlayState* play) {
    s32 animFinished;

    func_80AB359C(self);
    func_80AB1284(self, play);
    EnNb_UpdateCollider(self, play);
    func_80AB36DC(self, play);
    func_80AB10C4(self);
    animFinished = FUN_003731e0(&self->skelAnime);
    FUN_0033526c(self);
    func_80AB3A7C(self, play, animFinished);
}
#endif

GLOBAL_ASM("asm/caseD_14_00293124.s")

}
