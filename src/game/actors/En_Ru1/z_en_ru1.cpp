extern "C" {
#include "z_en_ru1.hpp"

#define FLAGS 0x04000011

#define BGCHECKFLAG_GROUND 0x0001
#define BGCHECKFLAG_WALL 0x0008
#define BGCHECKFLAG_CEILING 0x0010

void EnRu1_Init(Actor* actor, GameState* state);
void EnRu1_Destroy(Actor* thisx, PlayState* play);
void EnRu1_Update(Actor* actor, GameState* state);
void EnRu1_Draw(Actor* thisx, PlayState* play);

GLOBAL_ASM("data/z_En_Ru1.data.s")

ActorInit En_Ru1_InitVars = {
    ACTOR_EN_RU1,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_RU1,
    sizeof(EnRu1),
    (ActorFunc)EnRu1_Init,
    (ActorFunc)EnRu1_Destroy,
    (ActorFunc)EnRu1_Update,
    (ActorFunc)EnRu1_Draw,
};

GLOBAL_ASM("asm/EnRu1_Init.s")

void EnRu1_Destroy(Actor* thisx, PlayState* play) {
    EnRu1* self = (EnRu1*)thisx;

    Collider_DestroyCylinder(play, &self->standingCollider);
    Collider_DestroyCylinder(play, &self->sittingCollider);
    SkelAnime_Free2((SkelAnime*)self->unk_1A4);
    ((FaceAnimations*)self->unk_9E4)->Destroy();
}

typedef void (*EnRu1ActionFuncEntry)(EnRu1*, PlayState*);
extern EnRu1ActionFuncEntry EnRu1_sActionFuncs[];

void EnRu1_Update(Actor* actor, GameState* state) {
    EnRu1* self = (EnRu1*)actor;
    PlayState* play = (PlayState*)state;
    s32 action = self->action;

    if ((u32)action >= 0x2F) {
        return;
    }
    if (EnRu1_sActionFuncs[action] == NULL) {
        return;
    }
    if ((u32)(action - 12) <= 2 && *(u16*)((u8*)play + 0x22B8) == 0x17C) {
        Audio_PlaySoundGeneral(0x010001C7, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        Audio_PlaySoundGeneral(0x0100054D, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
    }
    EnRu1_sActionFuncs[action](self, play);
}

GLOBAL_ASM("../src/game/actors/En_Ru1/data_syms.s")

extern EnRu1DrawFunc EnRu1_sDrawFuncs[];

void EnRu1_Draw(Actor* thisx, PlayState* play) {
    EnRu1* self = (EnRu1*)thisx;

    if ((u32)self->drawConfig < 3) {
        EnRu1DrawFunc drawFunc = EnRu1_sDrawFuncs[self->drawConfig];

        if (drawFunc != NULL) {
            drawFunc(self, play);
        }
    }
}

void FUN_0039b400(void) {
}

void FUN_0039280c(EnRu1* self) {
    self->action = 0x17;
}

void FUN_0039d3f8(EnRu1* self) {
    self->action = 0x10;
}

void FUN_003927ec(EnRu1* self) {
    self->action = 0x18;
    self->drawConfig = 1;
    self->actor.flags |= 9;
}

void FUN_0034e01c(void* a, void* b, s32 c, void* d);

void FUN_001b9324(EnRu1* self, s32 unused, s32 mode, void* arg) {
    u8* base = (u8*)self + 0xCE4;
    u8* p9 = base + 0xE;
    u8* p10 = base + 0x8;

    if (mode == 9) {
        FUN_0034e01c(arg, p9, mode, arg);
    } else if (mode == 10) {
        FUN_0034e01c(arg, p10, mode, arg);
    }
}

// FUN_0039be64 (override-limb callback trampoline): callback typed (Actor*, ...) in a local, no self alias.
extern void* EnRu1_sCallbackPtr;

s32 FUN_0039be64(s32 a0, s32 a1, s32 a2, Actor* thisx) {
    EnRu1* self = (EnRu1*)thisx;

    if (self->unk_BE8 == 0) {
        if (EnRu1_sCallbackPtr != NULL) {
            void (*cb)(Actor*, s32, s32, s32) = (void (*)(Actor*, s32, s32, s32))EnRu1_sCallbackPtr;

            cb(thisx, a0, a1, a2);
        }
    }
    return 0;
}


// FUN_003264c8 (cfg tail): 0-word C, but ROM has a trailing `mov r0,r0` at 0x326524 (separate cfg block);
// C compiles to 92 B and shifts every later function. Needs cfg split at 0x326524 (orchestrator). Draft under #if 0.
#if 0
void FUN_003264c8(EnRu1* self) {
    s16* blinkTimer = &self->blinkTimer;
    s16* eyes = &self->eyes;

    if (DECR(*blinkTimer) == 0) {
        *blinkTimer = Rand_S16Offset_003702c8(60, 60);
    }

    *eyes = *blinkTimer;
    if (*eyes >= 3) {
        *eyes = 0;
    }
}
#endif

// FUN_003a9e9c (= EnRu1_SapphireRoom_Waiting): the helper is passed (EnRu1*)thisx (dead Actor* alias) so armcc
// reloads self->bigOctoPlatform for the cam store instead of forwarding it.
static inline EnRu1BigOctoPlatform* EnRu1_FindBigOctoPlatform(PlayState* play) {
    Actor* actorIter = play->actorCtx.actorLists[ACTORCAT_BG].head;

    while (actorIter != NULL) {
        if (actorIter->id == ACTOR_BG_BDAN_OBJECTS && actorIter->params == 0) {
            return (EnRu1BigOctoPlatform*)actorIter;
        }
        actorIter = actorIter->next;
    }
    return NULL;
}

static void EnRu1_SetPlatformCamSetting(EnRu1* self, s32 cameraSetting) {
    if (self->bigOctoPlatform != NULL) {
        self->bigOctoPlatform->cameraSetting = cameraSetting;
    }
}

void FUN_003a9e9c(EnRu1* self, PlayState* play) {
    Actor* thisx = &self->actor;
    EnRu1BigOctoPlatform* found = EnRu1_FindBigOctoPlatform(play);

    self->bigOctoPlatform = found;
    if (found != NULL) {
        self->action = 0x2A;
        self->drawConfig = 1;
        EnRu1_SetPlatformCamSetting((EnRu1*)thisx, 1);
    }
}


// FUN_00392198 (= EnRu1_BossRoom_RiseThroughBlueWarp): FUN_00370734 takes (SkelAnime*, action) like the other sites.
static void EnRu1_SetBlueWarpState(EnRu1* self, s32 state) {
    if (self->blueWarp != NULL) {
        self->blueWarp->rutoWarpState = state;
    }
}

void FUN_00392198(EnRu1* self) {
    {
        f32* yOffset = &self->actor.shape.yOffset;

        *yOffset += 250.0f / 3.0f;
    }
    ((void (*)(SkelAnime*, s32))FUN_00370734)((SkelAnime*)self->unk_1A4, self->action);
    {
        f32* yOffset = &self->actor.shape.yOffset;

        if (self->actor.shape.yOffset >= 0.0f) {
            self->action = 0x12;
            *yOffset = 0.0f;
            EnRu1_SetBlueWarpState(self, 1);
        }
    }
}


s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);

void FUN_00347f48(EnRu1* self, s32* animation, u8 mode, f32 morphFrames, s32 playReversed) {
    SkelAnime* skelAnime = (SkelAnime*)self->unk_1A4;
    s32 animIdx = *animation;
    f32 frameCount = Animation_GetLastFrame((SkelAnime*)self->unk_1A4, animIdx);
    f32 playbackSpeed;
    f32 startFrame;
    f32 endFrame;

    if (!playReversed) {
        startFrame = 0.0f;
        endFrame = frameCount;
        playbackSpeed = 1.0f;
    } else {
        startFrame = frameCount;
        endFrame = 0.0f;
        playbackSpeed = -1.0f;
    }

    Animation_Change_00375c08(skelAnime, animIdx, playbackSpeed, startFrame, endFrame, mode, morphFrames);
}

// FUN_00394288 (= EnRu1_PostLimbDraw, head focus): 25/30 words off. ROM shrink-wraps nothing: it pushes {r4,lr}
// before the limb==10 compare and shares the epilogue; armcc emits `bxne lr` early-out. ~8 variants tried
// (hoisted locals, mtx alias, per-field multVec init, arg order). Draft kept under #if 0.
#if 0
void FUN_00394288(s32 unused, s32 limbIndex, MTX34* mtx, EnRu1* self) {
    Vec3f multVec;
    Vec3f focusPos;
    MTX34* mtxPtr = mtx;

    if (limbIndex == 10) {
        multVec.x = 0.0f;
        multVec.y = 10.0f;
        multVec.z = 0.0f;
        VEC3TransformAsm((Vector3f*)&focusPos, mtxPtr, (Vector3f*)&multVec);
        self->actor.focus.pos = focusPos;
        self->actor.focus.rot.x = self->actor.world.rot.x;
        self->actor.focus.rot.y = self->actor.world.rot.y;
        self->actor.focus.rot.z = self->actor.world.rot.z;
    }
}
#endif

// caseD_1_00294fb4 (= EnRu1_DrawOpa): 22/33 words off; ROM keeps skel in r6, face in r5 and loads eyes/mouth
// with register offsets before the face pointer. Field-typed, raw-offset and split-init variants tried (~5).
// Draft kept under #if 0.
#if 0
s32 FUN_0039be64(s32 a0, s32 a1, s32 a2, Actor* thisx);
void FUN_00394288(s32 unused, s32 limbIndex, MTX34* mtx, EnRu1* self);

void caseD_1_00294fb4(EnRu1* self, PlayState* play) {
    SkelAnime* skelAnime = (SkelAnime*)self->unk_1A4;
    s16 eyes = *(s16*)((u8*)self + 0xBB4);
    s16 mouth = *(s16*)((u8*)self + 0xBB8);
    FaceAnimations* face;
    s32 pad;

    (void)&pad;
    face = (FaceAnimations*)self->unk_9E4;
    face->FUN_0035e3a4(0, eyes);
    face->FUN_0035e3a4(1, mouth);
    face->FUN_0035e330();
    SkelAnime_DrawOpa(skelAnime, (MTX34*)((u8*)self + 0x148), (OverrideLimbDrawOpa)FUN_0039be64,
                      (PostLimbDrawOpa)FUN_00394288, (Actor*)self, 0);
}
#endif

void FUN_0031a3dc(EnRu1* self);

void FUN_0039d788(EnRu1* self, PlayState* play) {
    FUN_0031a3dc(self);

    if (Message_GetState(&play->msgCtx) == TEXT_STATE_CLOSING) {
        if (self->bigOctoPlatform != NULL) {
            self->bigOctoPlatform->cameraSetting = 4;
        }
        Actor_Kill(&self->actor);
    }
}

typedef struct EnRu1PlayerSub {
    /* 0x0000 */ char unk_00[0x21C];
    /* 0x021C */ f32 speedXZ; // player+0x221C (N64 player->speedXZ)
    /* 0x0220 */ char unk_220[0x9B8 - 0x220];
    /* 0x09B8 */ u32 flags;
} EnRu1PlayerSub;

void FUN_003a53d4(Actor* thisx, PlayState* play) {
    if (play->GetCutsceneState() == 0) {
        Actor_Kill(thisx);
        Player* player = GET_PLAYER(play);
        EnRu1PlayerSub* sub = (EnRu1PlayerSub*)((u8*)player + 0x2000);
        sub->flags &= ~0x400;
    }
}

extern u8 EnRu1_sRutoChildWait2Anim[];
void FUN_00353020(SkelAnime* skelAnime, void* anim, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void FUN_003264c8(EnRu1* self);

void FUN_003a3f1c(EnRu1* self, PlayState* play) {
    s32 animFinished;

    FUN_0031a3dc(self);
    {
        PlayState* play2 = play;
        f32 velocityYHeld = self->actor.velocity.y;

        self->actor.velocity.y = -4.0f;
        Actor_UpdateBgCheckInfo(play2, &self->actor, 19.0f, 25.0f, 30.0f, 7);
        self->actor.velocity.y = velocityYHeld;
    }
    animFinished = ((s32 (*)(SkelAnime*, s32))FUN_00370734)((SkelAnime*)self->unk_1A4, self->action);
    FUN_003264c8(self);
    if (animFinished != 0) {
        SkelAnime* skel = (SkelAnime*)self->unk_1A4;

        FUN_00353020(skel, EnRu1_sRutoChildWait2Anim, 1.0f, 0.0f, Animation_GetLastFrame(skel, 0), 0, -8.0f);
        self->action = 0x28;
    }
}

// FUN_0039acb4 = EnRu1_RiseWithLink. ROM tail-calls FUN_00370734 with self->action in r1.
void FUN_0039acb4(EnRu1* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Vec3f* playerPos = &player->actor.world.pos;
    s16 shapeRotY = player->actor.shape.rot.y;
    f32 xzDistToPlayer = self->xzDistToPlayerInBlueWarp;
    Vec3f* pos = &self->actor.world.pos;

    pos->x = (xzDistToPlayer * Math_SinS(shapeRotY)) + playerPos->x;
    pos->y = playerPos->y;
    pos->z = (xzDistToPlayer * Math_CosS(shapeRotY)) + playerPos->z;

    {
        PlayState* play2 = play;
        f32 velocityYHeld = self->actor.velocity.y;

        self->actor.velocity.y = -4.0f;
        Actor_UpdateBgCheckInfo(play2, &self->actor, 19.0f, 25.0f, 30.0f, 7);
        self->actor.velocity.y = velocityYHeld;
    }
    ((void (*)(SkelAnime*, s32))FUN_00370734)((SkelAnime*)self->unk_1A4, self->action);
}

s32 Gameplay_InCsMode(PlayState* play);

// FUN_003920ac = EnRu1_CheckStartFirstEncounter (IsPlayerInRangeForFirstEncounter inlined). 47/59 words off:
// prologue/player load now ROM-shaped, but the range test compiles to an int compare (ROM: vcmpe + bhi) and the
// s0/s1 float order is swapped. Draft kept under #if 0.
#if 0
void FUN_003920ac(EnRu1* self, PlayState* play) {
    u8 cutsceneTrigger = 1;
    Player* player = GET_PLAYER(play);

    self->actor.flags &= ~1;
    {
        Player* rangePlayer = GET_PLAYER(play);
        f32 thisPosZ = self->actor.world.pos.z;
        f32 playerPosZ = rangePlayer->actor.world.pos.z;

        if (!((playerPosZ - thisPosZ <= 265.0f) && (rangePlayer->actor.world.pos.y >= self->actor.world.pos.y))) {
            return;
        }
    }
    {
        if (!Gameplay_InCsMode(play) && !(player->stateFlags1 & 0x00206000) &&
            (player->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
            self->actor.flags |= 1;
            play->FUN_0037573c(self->zarInfo->GetQDBByIndex(0));
            {
                EnRu1PlayerSub* sub = (EnRu1PlayerSub*)((u8*)player + 0x2000);

                sub->flags |= 0x400;
                gSaveContext.cutsceneTrigger = cutsceneTrigger;
                sub->speedXZ = 0.0f;
            }
            self->action = 8;
        }
    }
}
#endif

// FUN_00319f20 lives in z_en_ru1_interact.cpp (armcc inlined it into same-TU callers).


s32 FUN_00319f20(EnRu1* self, PlayState* play);
void FUN_0034df30(Actor* actor, PlayState* play);
void* DynaPoly_GetActor(void* colCtx, s32 bgId);

// FUN_003a6b64 = EnRu1_Sitting_SinkingInWater (StopMoving + CheckHitBottomUnderwater + ResetBgCheckFlags inlined).
void FUN_003a6b64(EnRu1* self, PlayState* play) {
    FUN_0031a3dc(self);
    {
        PlayState* play2 = play;
        f32 velocityYHeld = self->actor.velocity.y;

        self->actor.velocity.y = -4.0f;
        Actor_UpdateBgCheckInfo(play2, &self->actor, 19.0f, 25.0f, 30.0f, 7);
        self->actor.velocity.y = velocityYHeld;
    }
    Actor_MoveForward(&self->actor);
    ((void (*)(SkelAnime*, s32))FUN_00370734)((SkelAnime*)self->unk_1A4, self->action);
    FUN_003264c8(self);
    if (self->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        self->actor.velocity.x = 0.0f;
        self->actor.velocity.y = 0.0f;
        self->actor.velocity.z = 0.0f;
        self->actor.speedXZ = 0.0f;
        self->actor.gravity = 0.0f;
        self->actor.minVelocityY = 0.0f;
        FUN_0034df30(&self->actor, play);
        self->action = 0x1B;
        self->isSittingOCActive = 0;
    }
    FUN_00319f20(self, play);
    {
        DynaPolyActor* dynaPolyActor = (DynaPolyActor*)DynaPoly_GetActor(&play->colCtx, self->actor.floorBgId);

        if (dynaPolyActor == NULL || dynaPolyActor->actor.id == ACTOR_EN_BOX) {
            self->actor.bgCheckFlags &= ~(BGCHECKFLAG_GROUND | BGCHECKFLAG_WALL | BGCHECKFLAG_CEILING);
        }
    }
}

void FUN_0034c664(Actor* actor, EnRu1InteractInfo* info, s16 trackingPreset, s32 mode);

// FUN_00295038 = EnRu1_ThroneRoom_Talk (TurnFullBodyTowardPlayer inlined, mode 4). Flags written back from a
// `u32 flags` local so the action constant is materialised after the flag store.
void FUN_00295038(EnRu1* self, PlayState* play) {
    EnRu1InteractInfo* info = &self->interactInfo;
    Player* player = GET_PLAYER(play);
    s16 trackingPreset;

    info->trackPos = player->actor.world.pos;
    info->yOffset = kREG(16) - 3.0f;
    trackingPreset = kREG(17) + 0xC;
    FUN_0034c664(&self->actor, info, trackingPreset, 4);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    ((void (*)(SkelAnime*, s32))FUN_00370734)((SkelAnime*)self->unk_1A4, self->action);
    FUN_003264c8(self);
    {
        PlayState* play2 = play;
        f32 velocityYHeld = self->actor.velocity.y;

        self->actor.velocity.y = -4.0f;
        Actor_UpdateBgCheckInfo(play2, &self->actor, 19.0f, 25.0f, 30.0f, 7);
        self->actor.velocity.y = velocityYHeld;
    }
    if (Message_GetState(&play->msgCtx) == TEXT_STATE_CLOSING) {
        u32 flags = self->actor.flags;

        self->actor.flags = flags & ~9;
        self->action = 0x2C;
    }
}


void FUN_00319b54(EnRu1* self, PlayState* play);
void FUN_0031a014(EnRu1* self, PlayState* play);

// FUN_0039ad64: 6/70 words off, all in the kill branch. ROM derives 0x1AE from the 0x2E action register with
// different reg numbering (ROM: 0x2E in r1, 0x1AE in r2 before the action store); static helper, alias, store-order
// permutations, shared StartKill(self, skip) helper tried (~100 variants). Draft kept under #if 0.
#if 0
static void FUN_0039ad64_Kill(EnRu1* self) {
    EnRu1* s2 = self;

    s2->action = 0x2E;
    self->unk_BB0 = 0x1AE;
    self->unk_BB2 = 0x3C;
}

// FUN_0039ad64 = EnRu1_Sitting_DisappearingInWater (FadeWhileSinking inlined; Actor_Kill inlined as action 0x2E).
void FUN_0039ad64(EnRu1* self, PlayState* play) {
    FUN_0031a3dc(self);
    {
        PlayState* play2 = play;
        f32 velocityYHeld = self->actor.velocity.y;

        self->actor.velocity.y = -4.0f;
        Actor_UpdateBgCheckInfo(play2, &self->actor, 19.0f, 25.0f, 30.0f, 7);
        self->actor.velocity.y = velocityYHeld;
    }
    FUN_0031a014(self, play);
    ((void (*)(SkelAnime*, s32))FUN_00370734)((SkelAnime*)self->unk_1A4, self->action);
    FUN_003264c8(self);
    if (FUN_00319f20(self, play)) {
        self->sinkTimer += 1.0f;
        if (self->sinkTimer < 13.0f) {
            u32 alpha = ((13.0f - self->sinkTimer) * 255.0f) / 13.0f;

            self->alpha = alpha;
            self->actor.shape.shadowAlpha = alpha;
        } else {
            FUN_0039ad64_Kill(self);
        }
    }
    FUN_00319f20(self, play);
}
#endif


// FUN_0039d7cc: state update (TurnUpperBodyForward, SkelAnime, BgCheck, DynaPoly floor check, eyes), tail-calls
// FUN_00319b54.
void FUN_0039d7cc(EnRu1* self, PlayState* play) {
    Actor* thisx = &self->actor;

    FUN_0031a3dc(self);
    ((void (*)(SkelAnime*, s32))FUN_00370734)((SkelAnime*)self->unk_1A4, self->action);
    {
        PlayState* play2 = play;
        f32 velocityYHeld = thisx->velocity.y;

        thisx->velocity.y = -4.0f;
        Actor_UpdateBgCheckInfo(play2, thisx, 19.0f, 25.0f, 30.0f, 7);
        thisx->velocity.y = velocityYHeld;
    }
    {
        u16 flags;

        if ((self->actor.bgCheckFlags & BGCHECKFLAG_GROUND) && (self->actor.floorBgId != 0x32)) {
            DynaPolyActor* dynaPolyActor = (DynaPolyActor*)DynaPoly_GetActor(&play->colCtx, self->actor.floorBgId);

            if ((dynaPolyActor != NULL) && (dynaPolyActor->actor.id == 0xE6) &&
                (((u16)dynaPolyActor->actor.params >> 8) & 0x3F) == 0x38) {
                flags = gSaveContext.infTable[20] | 1;
                goto store;
            }
        }
        flags = gSaveContext.infTable[20] & ~1;
    store:
        gSaveContext.infTable[20] = flags;
    }
    FUN_003264c8(self);
    if (!(gSaveContext.infTable[20] & 0x80)) {
        s8 curRoomNum = *(s8*)((u8*)play + 0x4C30);

        if (curRoomNum == 2) {
            gSaveContext.infTable[20] |= 0x80;
        }
    }
    FUN_00319b54(self, play);
}

extern u8 EnRu1_sAnim4D9BF8[];
extern u8 EnRu1_sAnim4D9BFC[];

// FUN_003a28f8 = EnRu1_BossRoom_WhatTookYouSoLong (AdvanceAngryAnimation inlined). 41/77 words off: ROM reloads
// rutoWarpState into a 2nd reg (`ldr r2,[r1,#0x4cc]`), 12 bytes bigger; float const load order and the
// blueWarp/state shape not reproduced in ~6 variants. Draft kept under #if 0.
#if 0
void FUN_003a28f8(EnRu1* self, PlayState* play) {
    s32 isTalking;
    f32 speed;
    f32 start;
    f32 morph;

    {
        PlayState* play2 = play;
        f32 velocityYHeld = self->actor.velocity.y;

        self->actor.velocity.y = -4.0f;
        Actor_UpdateBgCheckInfo(play2, &self->actor, 19.0f, 25.0f, 30.0f, 7);
        self->actor.velocity.y = velocityYHeld;
    }
    isTalking = ((s32 (*)(SkelAnime*, s32))FUN_00370734)((SkelAnime*)self->unk_1A4, self->action);
    morph = -8.0f;
    start = 0.0f;
    speed = 1.0f;
    {
        EnRu1BlueWarp* blueWarp = self->blueWarp;

        if (blueWarp != NULL && blueWarp->rutoWarpState == 4) {
            if (isTalking != 0) {
                SkelAnime* skel = (SkelAnime*)self->unk_1A4;

                FUN_00353020(skel, EnRu1_sAnim4D9BF8, speed, start, Animation_GetLastFrame(skel, 0x14), 0, morph);
            }
        } else if (blueWarp != NULL && blueWarp->rutoWarpState == 5) {
            SkelAnime* skel = (SkelAnime*)self->unk_1A4;

            FUN_00353020(skel, EnRu1_sAnim4D9BFC, speed, start, Animation_GetLastFrame(skel, 0x15), 2, morph);
            self->xzDistToPlayerInBlueWarp = self->actor.xzDistToPlayer;
            self->action = 0x15;
        }
    }
}
#endif

s32 FUN_0036e5e0(SkelAnime* skel, f32 a, f32 b);
void AnimationContext_SetMoveActor(PlayState* play, Actor* actor, SkelAnime* skel, f32 speed);
extern u8 EnRu1_sAnim4D9BE8[];
extern u8 EnRu1_sAnim4D9BD4[];

// FUN_003a2c98 = EnRu1_SpeakableJabu_TakingSeat (N64 z_en_ru1.c). ROM inlines Sfx_PlaySfxAtPos, HoldSittingPose,
// func_80AED6DC and DisableSittingOC.
void FUN_003725e0(PlayState* play);
extern u8 EnRu1_sAnim4D9BB8[];

void FUN_003a2c98(EnRu1* self, PlayState* play) {
    s32 animFinished;

    FUN_0031a3dc(self);
    animFinished = ((s32 (*)(SkelAnime*, s32))FUN_00370734)((SkelAnime*)self->unk_1A4, self->action);
    {
        SkelAnime* skel0 = (SkelAnime*)self->unk_1A4;

        if (FUN_0036e5e0(skel0, 11.0f, 1.0f)) {
            Audio_PlaySoundGeneral(0x01000203, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        }
    }
    FUN_003264c8(self);
    {
        PlayState* play2 = play;
        f32 velocityYHeld = self->actor.velocity.y;

        self->actor.velocity.y = -4.0f;
        Actor_UpdateBgCheckInfo(play2, &self->actor, 19.0f, 25.0f, 30.0f, 7);
        self->actor.velocity.y = velocityYHeld;
    }
    if (animFinished != 0) {
        SkelAnime* skel = (SkelAnime*)self->unk_1A4;

        FUN_00353020(skel, EnRu1_sAnim4D9BB8, 1.0f, 0.0f, Animation_GetLastFrame(skel, 3), 0, 0.0f);
        {
            PlayState* play2 = play;

            FUN_003725e0(play2);
        }
        {
            u16* infTable = (u16*)((u8*)&gSaveContext + 0xF00);

            infTable[28] |= 8;
        }
        s8 curRoomNum = *(s8*)((u8*)play + 0x4C30);

        self->unk_BE0 = 0.0f;
        self->roomNum2 = curRoomNum;
        FUN_0034df30(&self->actor, play);
        self->action = 0x1B;
        self->isSittingOCActive = 0;
    }
}

// FUN_003a2dfc = EnRu1_SapphireRoom_ThrownOntoPlatform (N64). ROM inlines SetupRetrievingSapphireAnimation and
// StartWalkingTowardSapphire (AnimationContext_SetMoveActor tail). 60/98 words off: ROM pushes sb/sl (csCtx and a
// spare reg kept across GetCutsceneState), predicated cue load, frame 392 B vs 308 B. ~5 variants. Draft under #if 0.
#if 0
void FUN_003a2dfc(EnRu1* self, PlayState* play) {
    FUN_0031a3dc(self);
    ((s32 (*)(SkelAnime*, s32))FUN_00370734)((SkelAnime*)self->unk_1A4, self->action);
    FUN_003264c8(self);
    {
        CsCmdActorAction* cue = (play->GetCutsceneState() != 0) ? play->csCtx.npcActions[3] : NULL;

        if (cue != NULL && cue->action != 1) {
            SkelAnime* skel = (SkelAnime*)self->unk_1A4;
            f32 frameCount = Animation_GetLastFrame(skel, 9);
            cue = play->csCtx.npcActions[3];
            self->actor.world.pos.x = cue->startPos.x;
            self->actor.world.pos.y = cue->startPos.y;
            self->actor.world.pos.z = cue->startPos.z;
            self->actor.shape.rot.x = cue->rot.x;
            self->actor.world.rot.x = cue->rot.x;
            self->actor.shape.rot.y = cue->rot.y;
            self->actor.world.rot.y = cue->rot.y;
            self->actor.shape.rot.z = cue->rot.z;
            self->actor.world.rot.z = cue->rot.z;
            FUN_00353020(skel, EnRu1_sAnim4D9BD4, 1.0f, 0.0f, frameCount, 2, 0.0f);
            *(u8*)((u8*)self + 0x1F6) |= 1;
            AnimationContext_SetMoveActor(play, &self->actor, skel, 1.0f);
            self->action = 0x25;
            self->drawConfig = 1;
            self->actor.shape.shadowAlpha = 255;
        }
    }
}
#endif

// FUN_003a3fe0 = EnRu1_SapphireRoom_ReadyToGoHome (N64). ROM inlines RespondToSapphirePlatformMoving and
// RespondToAbduction (cameraSetting 2 = NORMAL1, 3 = DUNGEON0). ROM calls FUN_00347f48 and FUN_0031a3dc opaquely,
// so this must sit in a TU without them inlined. 48-49/90 words off: play in r6 (ROM r5), drawConfig zero in r5
// (ROM r6), pool offsets. ~8 variants. Draft kept under #if 0.
#if 0
static void EnRu1_SetEyes(EnRu1* self, s16 eyes) {
    self->eyes = eyes;
}

static void EnRu1_SetMouth(EnRu1* self, s16 mouth) {
    self->mouth = mouth;
}

void FUN_003a3fe0(EnRu1* self, PlayState* play) {
    FUN_0031a3dc(self);
    {
        PlayState* play2 = play;
        f32 velocityYHeld = self->actor.velocity.y;

        self->actor.velocity.y = -4.0f;
        Actor_UpdateBgCheckInfo(play2, &self->actor, 19.0f, 25.0f, 30.0f, 7);
        self->actor.velocity.y = velocityYHeld;
    }
    ((s32 (*)(SkelAnime*, s32))FUN_00370734)((SkelAnime*)self->unk_1A4, self->action);
    FUN_003264c8(self);
    {
        s32 drawOff = 0;

        if (self->bigOctoPlatform != NULL && self->bigOctoPlatform->cameraSetting == 2) {
            EnRu1_SetEyes(self, 3);
            EnRu1_SetMouth(self, 2);
            if (*(u8*)((u8*)self + 0x214) != 2) {
                FUN_00347f48(self, (s32*)EnRu1_sAnim4D9BD8, 2, -8.0f, 0);
                Audio_PlaySoundGeneral(0x01000550, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                                       &DAT_0054ac24.unk_0);
            }
        }
        if (self->bigOctoPlatform != NULL && self->bigOctoPlatform->cameraSetting == 3) {
            self->actor.flags |= 9;
            self->actor.textId = 0x4048;
            FUN_00367c7c(play, 0x4048, NULL);
            Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 0x14, 0x0100054D);
            self->action = 0x2B;
            self->drawConfig = drawOff;
        }
    }
}
#endif

// FUN_00295138 = EnRu1_ThroneRoom_Idle (N64). ROM inlines TurnUpperBodyTowardPlayer, UpdateStandingOC and
// ThroneRoomTalkOfferAccepted. 31/89 words off: the textId store reg (ROM keeps Text_GetFaceReaction result in r0 and
// stores via r1 = self+0x100) and the standing-collider base reg (r1 then mov r6). ~8 variants. Draft under #if 0.
#if 0
void FUN_0034c664(Actor* actor, EnRu1InteractInfo* info, s16 preset, s32 mode);
void FUN_00363cb8(Actor* actor, PlayState* play);

static s32 EnRu1_ThroneRoomTalkOfferAccepted(EnRu1* self, PlayState* play) {
    if (!Actor_ProcessTalkRequest(&self->actor, play)) {
        self->actor.flags |= 9;
        u16 faceReaction = Text_GetFaceReaction(play, 0x1F);

        self->actor.textId = faceReaction;
        if (faceReaction == 0) {
            self->actor.textId = 0x402C;
        }
        FUN_00363cb8(&self->actor, play);
        return 0;
    }
    return 1;
}

void FUN_00295138(EnRu1* self, PlayState* play) {
    EnRu1InteractInfo* info = &self->interactInfo;
    Player* player = GET_PLAYER(play);
    s16 trackingPreset;
    s32 talkAccepted;

    info->trackPos = player->actor.world.pos;
    info->yOffset = kREG(16) - 3.0f;
    trackingPreset = kREG(17) + 0xC;
    FUN_0034c664(&self->actor, info, trackingPreset, 2);
    ((s32 (*)(SkelAnime*, s32))FUN_00370734)((SkelAnime*)self->unk_1A4, self->action);
    FUN_003264c8(self);
    {
        Collider* standingBase = &self->standingCollider.base;
        CollisionCheckContext* colChkCtx = &play->colChkCtx;

        Collider_UpdateCylinder(&self->actor, &self->standingCollider);
        CollisionCheck_SetOC(play, colChkCtx, standingBase);
    }
    {
        PlayState* play2 = play;
        f32 velocityYHeld = self->actor.velocity.y;

        self->actor.velocity.y = -4.0f;
        Actor_UpdateBgCheckInfo(play2, &self->actor, 19.0f, 25.0f, 30.0f, 7);
        self->actor.velocity.y = velocityYHeld;
    }
    talkAccepted = EnRu1_ThroneRoomTalkOfferAccepted(self, play);
    if (talkAccepted) {
        self->action = 0x2D;
    }
}
#endif

// FUN_003a5604 = EnRu1_BossRoom_LinkWalksToPointInBlueWarp (N64). ROM inlines IsLinkInBlueWarp and
// StartCrossingArmsAndLegs. 63/89 words off: player/linkPos register plan and float copy order. Draft under #if 0.
#if 0
extern u8 EnRu1_sAnim4D9BF4[];

void FUN_003a5604(EnRu1* self, PlayState* play) {
    s32 shouldStart = 0;

    ((s32 (*)(SkelAnime*, s32))FUN_00370734)((SkelAnime*)self->unk_1A4, self->action);
    {
        Player* player = GET_PLAYER(play);

        self->unk_BC4 += 1.0f;
        if ((player->actor.speedXZ == 0.0f) && (self->unk_BC4 >= 3.0f)) {
            Vec3f* linkPos = (Vec3f*)((u8*)player + 0x12C8);
            s16* shapeRotY;
            f32 dx;
            f32 dz;
            s16 targetRotY;

            player->actor.world.pos.x = linkPos->x;
            player->actor.world.pos.y = linkPos->y;
            player->actor.world.pos.z = linkPos->z;
            shapeRotY = &player->actor.shape.rot.y;
            dx = self->actor.world.pos.x - player->actor.world.pos.x;
            dz = self->actor.world.pos.z - player->actor.world.pos.z;
            targetRotY = (s16)(Math_FAtan2F(dx, dz) * 10430.378f);
            if (*shapeRotY != targetRotY) {
                Math_SmoothStepToS(shapeRotY, targetRotY, 0x14, 0x1838, 0x64);
                player->actor.world.rot.y = *shapeRotY;
            } else {
                shouldStart = 1;
            }
        }
    }
    if (shouldStart) {
        SkelAnime* skel = (SkelAnime*)self->unk_1A4;

        FUN_00353020(skel, EnRu1_sAnim4D9BF4, 1.0f, 0.0f, Animation_GetLastFrame(skel, 0x13), 2, -8.0f);
        self->action = 0x14;
        if (self->blueWarp != NULL) {
            self->blueWarp->rutoWarpState = 3;
        }
    }
}
#endif

// FUN_003a3d88 = EnRu1_SpeakableJabu_Talking (N64). ROM inlines TurnFullBodyTowardPlayer, EndJabuTalk and
// BecomingCarrier. 54/101 words off: play sits in r6 (ROM r5) and the interact copy uses r5 (ROM r6). Draft under #if 0.
#if 0
void FUN_00370778(PlayState* play);
extern u8 EnRu1_sAnim4D9BBC[];

void FUN_003a3d88(EnRu1* self, PlayState* play) {
    EnRu1InteractInfo* info = &self->interactInfo;
    Player* player = GET_PLAYER(play);
    s16 trackingPreset;

    info->trackPos = player->actor.world.pos;
    info->yOffset = kREG(16) - 3.0f;
    trackingPreset = kREG(17) + 0xC;
    FUN_0034c664(&self->actor, info, trackingPreset, 4);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    ((s32 (*)(SkelAnime*, s32))FUN_00370734)((SkelAnime*)self->unk_1A4, self->action);
    FUN_003264c8(self);
    {
        PlayState* play2 = play;
        f32 velocityYHeld = self->actor.velocity.y;

        self->actor.velocity.y = -4.0f;
        Actor_UpdateBgCheckInfo(play2, &self->actor, 19.0f, 25.0f, 30.0f, 7);
        self->actor.velocity.y = velocityYHeld;
    }
    if (Message_GetState(&play->msgCtx) == 5 && Message_ShouldAdvance(play)) {
        u16* infTable = gSaveContext.infTable;

        if (infTable[20] & 4) {
            f32 frameCount = Animation_GetLastFrame((SkelAnime*)self->unk_1A4, 2);

            FUN_00353020((SkelAnime*)self->unk_1A4, EnRu1_sAnim4D9BBC, 1.0f, 0.0f, frameCount, 2, -8.0f);
            FUN_00370778(play);
            self->action = 0x1A;
            self->actor.flags &= ~9;
        } else {
            FUN_003725e0(play);
            infTable[20] |= 4;
            self->action = 0x18;
        }
    }
}
#endif

// FUN_003922d0 (3DS-only rise state: BgCheck + sound + SetMoveActor tail; not in the N64 source). Draft from the asm.
// 54/88 words off, 20 bytes short: the frame/reg plan of the sound call and the unknown FUN_0036e5e0 args
// (5.0f,1.0f from pool) and the 0x21C pointer read do not line up. Draft kept under #if 0.
#if 0
void FUN_003922d0(EnRu1* self, PlayState* play) {
    s32 animFinished;

    animFinished = ((s32 (*)(SkelAnime*, s32))FUN_00370734)((SkelAnime*)self->unk_1A4, self->action);
    {
        PlayState* play2 = play;
        f32 velocityYHeld = self->actor.velocity.y;

        self->actor.velocity.y = -4.0f;
        Actor_UpdateBgCheckInfo(play2, &self->actor, 19.0f, 25.0f, 30.0f, 7);
        self->actor.velocity.y = velocityYHeld;
    }
    if (FUN_0036e5e0((SkelAnime*)self->unk_1A4, 5.0f, 1.0f)) {
        Audio_PlaySoundGeneral(0x01000064, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
    }
    if (animFinished != 0) {
        SkelAnime* skel = (SkelAnime*)self->unk_1A4;

        FUN_00353020(skel, EnRu1_sAnim4D9BE8, 1.0f, 0.0f, (f32)Animation_GetLastFrame(skel, 13), 0, -8.0f);
        self->action = 3;
    } else {
        if (*(f32*)((u8*)*(void**)((u8*)self + 0x21C) + 0x1C) > *(f32*)((u8*)self + 0x20C)) {
            *(u8*)((u8*)self + 0x1F6) = (*(u8*)((u8*)self->unk_1A4 + 0x52)) | 3;
            AnimationContext_SetMoveActor(play, &self->actor, 1.0f);
        }
    }
}
#endif

// FUN_00391cbc (= EnRu1_SetupPositionForFirstEncounter): 29/35 words off. ROM keeps csCtx in r6 and a NULL cue in r5
// across GetCutsceneState, then a predicated `ldrne` of the cue; armcc emits a branch and drops the NULL init.
// ~12 variants (ternary select, csCtx alias, decl order, direct play->csCtx). Draft kept under #if 0.
#if 0
typedef struct EnRu1CsCue {
    /* 0x00 */ u16 id;
    /* 0x02 */ u16 startFrame;
    /* 0x04 */ u16 endFrame;
    /* 0x06 */ Vec3s rot;
    /* 0x0C */ Vec3i startPos;
} EnRu1CsCue;

void FUN_00391cbc(EnRu1* self, PlayState* play) {
    CutsceneContext* csCtx = &play->csCtx;
    EnRu1CsCue* cue = NULL;

    if (play->GetCutsceneState() != 0) {
        cue = (EnRu1CsCue*)csCtx->npcActions[3];
    }
    if (cue != NULL && cue->id != 1) {
        cue = (EnRu1CsCue*)csCtx->npcActions[3];
        self->actor.world.pos.x = cue->startPos.x;
        self->actor.world.pos.y = cue->startPos.y;
        self->actor.world.pos.z = cue->startPos.z;
        {
            s16 newRotY = cue->rot.y;

            self->actor.shape.rot.y = newRotY;
            self->actor.world.rot.y = newRotY;
        }
        self->action = 9;
        self->drawConfig = 1;
    }
}
#endif

// FUN_002a5e68: 28/40 words off (zero-register choice: ROM moves play to r0 and zeroes r1). Draft kept under #if 0.
#if 0
void FUN_002a5e68(EnRu1* self, PlayState* play) {
    s32 count = 0;
    Actor* actorIter;

    self->drawConfig = 0;
    self->actor.shape.shadowAlpha = 0;
    if (self->unk_BB2 > 0) {
        self->unk_BB2 = self->unk_BB2 - 1;
        if (self->unk_BB2 == 0) {
            actorIter = play->actorCtx.actorLists[ACTORCAT_NPC].head;
            while (actorIter != NULL) {
                if (actorIter->id == ACTOR_EN_RU1) {
                    count++;
                }
                actorIter = actorIter->next;
            }
            if (count == 1) {
                FUN_00367c7c(play, self->unk_BB0, NULL);
            }
            Actor_Kill(&self->actor);
        }
    } else if (self->unk_BB2 == -1) {
        Actor_Kill(&self->actor);
    }
}
#endif

// caseD_2_00283a94 (draw with shadow alpha, no post-limb): 33/46 words off. ROM loads eyes/mouth with register offsets
// (`ldr r6,=0xbb4; ldrsh r6,[r0,r6]`) and builds the face pointer late; armcc folds the offsets into base+imm. Draft under #if 0.
#if 0
s32 FUN_0039be64(s32 a0, s32 a1, s32 a2, Actor* thisx);
void FUN_00342be0(EnRu1* self, s32 a1, s32 a2, f32 f0, f32 f1, f32 f2, f32 f3);

void caseD_2_00283a94(EnRu1* self, PlayState* play) {
    SkelAnime* skelAnime = (SkelAnime*)self->unk_1A4;
    s16 eyes = self->eyes;
    s16 mouth = self->mouth;
    FaceAnimations* face;

    FUN_00342be0(self, 4, 2, 1.0f, 1.0f, 1.0f, (f32)self->alpha * (1.0f / 255.0f));
    face = (FaceAnimations*)self->unk_9E4;
    face->FUN_0035e3a4(0, eyes);
    face->FUN_0035e3a4(1, mouth);
    face->FUN_0035e330();
    SkelAnime_DrawOpa(skelAnime, (MTX34*)((u8*)self + 0x148), (OverrideLimbDrawOpa)FUN_0039be64, NULL, (Actor*)self, 0);
}
#endif

// FUN_0039b404 (= EnRu1_SapphireRoom_Retrieving tail, cfg tail): functions.tsv merges unrelated code after its literal
// pool at 0x39B4D0, so it needs a cfg split before C can replace it (GLOBAL_ASM kept). Draft: 28/124 words off
// (play copy reg, room-num reg). Draft kept under #if 0.
#if 0
void FUN_0039b404(EnRu1* self, PlayState* play) {
    s8 curRoomNum;

    FUN_0031a3dc(self);
    {
        PlayState* play2 = play;
        f32 velocityYHeld = self->actor.velocity.y;

        self->actor.velocity.y = -4.0f;
        Actor_UpdateBgCheckInfo(play2, &self->actor, 19.0f, 25.0f, 30.0f, 7);
        self->actor.velocity.y = velocityYHeld;
    }
    ((void (*)(SkelAnime*, s32))FUN_00370734)((SkelAnime*)self->unk_1A4, self->action);
    FUN_003264c8(self);
    if (play->GetCutsceneState() == 0) {
        curRoomNum = *(s8*)((u8*)play + 0x4C30);
        gSaveContext.infTable[28] |= 0x20;
        Flags_SetSwitch(play, ((u16)self->actor.params >> 8) & 0xFF);
        if (self->bigOctoPlatform != NULL) {
            self->bigOctoPlatform->cameraSetting = 1;
        }
        self->action = 0x2A;
        self->actor.room = curRoomNum;
    }
}
#endif

extern u8 EnRu1_sAnim4D9BB4[];
extern u8 EnRu1_sAnim4D9BB8[];
extern u8 EnRu1_sAnim4D9BC4[];
extern u8 EnRu1_sAnim4D9BCC[];

void FUN_00391fd4(EnRu1* self, PlayState* play) {
    s32 animFinished;
    SkelAnime* skel0 = (SkelAnime*)self->unk_1A4;

    animFinished = ((s32 (*)(SkelAnime*, s32))FUN_00370734)(skel0, self->action);
    FUN_003264c8(self);
    {
        PlayState* play2 = play;
        f32 velocityYHeld = self->actor.velocity.y;

        self->actor.velocity.y = -4.0f;
        Actor_UpdateBgCheckInfo(play2, &self->actor, 19.0f, 25.0f, 30.0f, 7);
        self->actor.velocity.y = velocityYHeld;
    }
    if (animFinished != 0) {
        SkelAnime* skel = (SkelAnime*)self->unk_1A4;

        FUN_00353020(skel, EnRu1_sAnim4D9BB4, 1.0f, 0.0f, Animation_GetLastFrame(skel, 7), 0, -8.0f);
        self->actor.world.rot.y += 0x8000;
        self->action = 0xB;
        self->unk_BC4 = 0.0f;
    }
}

// FUN_0039d23c, FUN_0039d6ac: cue helper declares csCtx, cue, then `PlayState* play2 = play` (use order matters for
// the play r5 / cue r6 / csCtx r7 numbering).

extern u8 EnRu1_sAnim4D9BC4[];
void FUN_0031cddc(EnRu1* self, PlayState* play);

static inline CsCmdActorAction* EnRu1_GetNpcCue3(PlayState* play) {
    CutsceneContext* csCtx = &play->csCtx;
    CsCmdActorAction* cue = NULL;
    PlayState* play2 = play;

    if (play2->GetCutsceneState() != 0) {
        cue = csCtx->npcActions[3];
    }
    return cue;
}


void FUN_0039d23c(EnRu1* self, PlayState* play) {
    SkelAnime* skel0 = (SkelAnime*)self->unk_1A4;

    ((s32 (*)(SkelAnime*, s32))FUN_00370734)(skel0, self->action);
    FUN_003264c8(self);
    {
        PlayState* play2 = play;
        f32 velocityYHeld = self->actor.velocity.y;

        self->actor.velocity.y = -4.0f;
        Actor_UpdateBgCheckInfo(play2, &self->actor, 19.0f, 25.0f, 30.0f, 7);
        self->actor.velocity.y = velocityYHeld;
    }
    {
        CsCmdActorAction* cue = EnRu1_GetNpcCue3(play);

        if (cue != NULL && cue->action == 3) {
            SkelAnime* skel = (SkelAnime*)self->unk_1A4;

            FUN_00353020(skel, EnRu1_sAnim4D9BC4, 1.0f, 0.0f, Animation_GetLastFrame(skel, 6), 2, -8.0f);
            self->action = 0xA;
        }
    }
}

extern u8 EnRu1_sAnim4D9BCC[];

void FUN_0039d6ac(EnRu1* self, PlayState* play) {
    FUN_0031a3dc(self);
    {
        PlayState* play2 = play;
        f32 velocityYHeld = self->actor.velocity.y;

        self->actor.velocity.y = -4.0f;
        Actor_UpdateBgCheckInfo(play2, &self->actor, 19.0f, 25.0f, 30.0f, 7);
        self->actor.velocity.y = velocityYHeld;
    }
    ((s32 (*)(SkelAnime*, s32))FUN_00370734)((SkelAnime*)self->unk_1A4, self->action);
    {
        CsCmdActorAction* cue = EnRu1_GetNpcCue3(play);

        if (cue != NULL && cue->action != 2) {
            SkelAnime* skel = (SkelAnime*)self->unk_1A4;

            FUN_00353020(skel, EnRu1_sAnim4D9BCC, 1.0f, 0.0f, Animation_GetLastFrame(skel, 0xB), 2, -8.0f);
            self->action = 0x27;
        }
    }
}



extern u8 EnRu1_sAnim4D9BB8[];

void FUN_0039ae7c(EnRu1* self, PlayState* play) {
    s8 curRoomNum;

    FUN_0031a3dc(self);
    ((s32 (*)(SkelAnime*, s32))FUN_00370734)((SkelAnime*)self->unk_1A4, self->action);
    {
        PlayState* play2 = play;
        f32 velocityYHeld = self->actor.velocity.y;

        self->actor.velocity.y = -4.0f;
        Actor_UpdateBgCheckInfo(play2, &self->actor, 19.0f, 25.0f, 30.0f, 7);
        self->actor.velocity.y = velocityYHeld;
    }
    FUN_003264c8(self);
    curRoomNum = *(s8*)((u8*)play + 0x4C30);
    if (play->GetCutsceneState() == 0) {
        SkelAnime* skel = (SkelAnime*)self->unk_1A4;

        FUN_00353020(skel, EnRu1_sAnim4D9BB8, 1.0f, 0.0f, Animation_GetLastFrame(skel, 3), 0, -8.0f);
        gSaveContext.infTable[20] |= 0x10;
        self->action = 0x1F;
    }
    self->unk_BDE = curRoomNum;
}

// FUN_00392200: 23/52 words off (animFinished reg r8 vs ROM r7), SIZE 192 vs 208. Draft kept under #if 0, GLOBAL_ASM kept.

extern u8 EnRu1_sAnim4D9BB4[];

#if 0
void FUN_00392200(EnRu1* self, PlayState* play) {
    SkelAnime* skel0 = (SkelAnime*)self->unk_1A4;
    s32 animFinished = ((s32 (*)(SkelAnime*, s32))FUN_00370734)(skel0, self->action);
    {
        PlayState* play2 = play;
        f32 velocityYHeld = self->actor.velocity.y;

        self->actor.velocity.y = -4.0f;
        Actor_UpdateBgCheckInfo(play2, &self->actor, 19.0f, 25.0f, 30.0f, 7);
        self->actor.velocity.y = velocityYHeld;
    }
    FUN_003264c8(self);
    FUN_0031cddc(self, play);
    {
        CsCmdActorAction* cue = EnRu1_GetNpcCue3(play);
        s32 inCue = 1;

        if (cue == NULL || cue->action != 8) {
            inCue = 0;
        }
        if (inCue != 0 && animFinished != 0) {
            Actor_Kill(&self->actor);
        }
    }
}
#endif

void FUN_003a67e0(EnRu1* self, PlayState* play) {
    SkelAnime* skel0 = (SkelAnime*)self->unk_1A4;

    ((s32 (*)(SkelAnime*, s32))FUN_00370734)(skel0, self->action);
    if (self->blueWarp != NULL && self->blueWarp->rutoWarpState == 2) {
        Vec3f* linkPos;
        f32 sinScale;
        f32 cosScale;
        s16 rotY;
        Vec3f* pos = &self->actor.world.pos;

        self->unk_BC4 = 0.0f;
        self->action = 0x13;
        linkPos = (Vec3f*)((u8*)GET_PLAYER(play) + 0x12C8);
        rotY = self->actor.shape.rot.y;
        sinScale = Math_SinS(rotY);
        linkPos->x = pos->x + ((f32)kREG(2) + 30.0f) * sinScale;
        cosScale = Math_CosS(rotY);
        linkPos->z = pos->z + ((f32)kREG(2) + 30.0f) * cosScale;
    }
}

void FUN_0039d320(EnRu1* self, PlayState* play) {
    s16 yaw;
    f32 spawnX;
    f32 spawnY;
    f32 spawnZ;

    self->action = 0x11;
    self->drawConfig = 1;
    yaw = self->actor.yawTowardsPlayer;
    self->actor.world.rot.y = yaw;
    self->actor.shape.rot.y = yaw;
    {
        Vec3f* pos = &self->actor.world.pos;
        Vec3f* p2 = pos;

        spawnX = pos->x + ((f32)kREG(1) + 12.0f) * Math_SinS(yaw);
        spawnY = p2->y;
        spawnZ = pos->z + ((f32)kREG(1) + 12.0f) * Math_CosS(yaw);
    }
    self->blueWarp = (EnRu1BlueWarp*)Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, ACTOR_DOOR_WARP1, spawnX,
                                                        spawnY, spawnZ, 0, yaw, 0, 5);
}

}
