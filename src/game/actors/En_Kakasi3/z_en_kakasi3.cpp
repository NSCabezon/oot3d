extern "C" {
#include "z_en_kakasi3.hpp"

#define FLAGS 0x02000009

void EnKakasi3_Init(Actor* actor, GameState* state);
void EnKakasi3_Destroy(Actor* actor, GameState* state);
void EnKakasi3_Update(EnKakasi3* self, PlayState* play);
void EnKakasi3_Draw(Actor* actor, GameState* state);
s32 EnKakasi3_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg);
void EnKakasi3_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg);

// Unknown callees (3DS addresses) whose real prototypes differ from include/functions.hpp.
void FUN_00180b78(EnKakasi3* self, PlayState* play); // action: N64 func_80A911F0 (entry)
void FUN_003e56c0(Actor* actor, GameState* state); // action: N64 func_80A91348 (talk)
void FUN_0029964c(EnKakasi3* self, PlayState* play); // action: N64 func_80A91284 (idle)
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 index);
// 3DS Animation_Change wrapper: same argument order as N64 (mode after endFrame).
void FUN_00353020(SkelAnime* skelAnime, void* animRef, f32 speed, f32 startFrame, f32 endFrame, s32 mode,
                  f32 morphFrames);
void FUN_0034fe20(Actor* actor, PlayState* play, SkelAnime* skelAnime, s32 cmbIndex, s32 csabIndex, void* jointTable,
                  void* morphTable, s32 limbCount);

GLOBAL_ASM("../src/game/actors/En_Kakasi3/kk3_symbols.s")

extern u8 kk3AnimRef[];

// ROM data order: sCylinderInit precedes InitVars (0x0052B438).
static ColliderCylinderInit sCylinderInit = {
    { 0x0A, 0x00, 0x09, 0x39, 0x20, 0x01 },
    { 0, { 0xFFCFFFFF, 0, 0 }, { 0xFFCFFFFF, 0, 0 }, 0, 5, 1 },
    { 20.0f, 70.0f, 0.0f, { 0.0f, 0.0f, 0.0f } },
};

ActorInit En_Kakasi3_InitVars = {
    ACTOR_EN_KAKASI3,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_KA,
    sizeof(EnKakasi3),
    (ActorFunc)EnKakasi3_Init,
    (ActorFunc)EnKakasi3_Destroy,
    (ActorFunc)EnKakasi3_Update,
    (ActorFunc)EnKakasi3_Draw,
};

// N64: func_80A911F0 (entry). Moved from functions_180000s.cpp.
void FUN_00180b78(EnKakasi3* self, PlayState* play) {
    f32 frameCount = Animation_GetLastFrame(&self->skelAnime, 0);

    FUN_00353020(&self->skelAnime, kk3AnimRef, 1.0f, 0.0f, frameCount, 0, -10.0f);
    self->actionFunc = (EnKakasi3ActionFunc)FUN_0029964c;
}

// N64: func_80A91284 (idle): picks the talk text and hands over to FUN_003e56c0.
// Moved from functions_280000s.cpp.
void FUN_0029964c(EnKakasi3* self, PlayState* play) {
    FUN_00370734(&self->skelAnime);
    self->actor.textId = 0x40A1;
    self->dialogState = TEXT_STATE_DONE;
    self->unk_232 = 0;
    if (!LINK_IS_ADULT) {
        self->unk_22C = false;
        if (*((u8*)&gSaveContext + 0x12DD) != 0) { // save.info.scarecrowSpawnSongSet (raw offset, debt)
            self->actor.textId = 0x40A0;
            self->dialogState = TEXT_STATE_EVENT;
            self->unk_240 = 1;
        }
    } else {
        self->unk_22C = true;
        if (*((u8*)&gSaveContext + 0x12DD) != 0) {
            if (self->unk_22D) {
                self->actor.textId = 0x40A2;
            } else {
                self->actor.textId = 0x40A3;
            }
        }
    }
    self->actionFunc = (EnKakasi3ActionFunc)FUN_003e56c0;
}

// N64 func_80A91348 (talk). Still GLOBAL_ASM in functions_3E0000s.cpp. Draft below matches except ONE
// instruction: ROM passes &skelAnime.curFrame as `add r0,r4,#0x1a8; add r0,r0,#0x3c` (rematerialized CSE),
// this C folds it to `add r0,r4,#0x1e4` (688 B vs 692 B, everything after is shifted by 4).
// Hoisting `SkelAnime* skel = &self->skelAnime` into the helper gives the right size but keeps skel in r8
// (16 words off). Tried: struct/u8 casts, f32* local, inline wrappers, fn-level skel local.
// Needs: void FUN_0036f4e4(SkelAnime*, f32); void FUN_00370778(PlayState*); OnePointCutscene_Init decl.
#if 0
static inline void func_80A90E28(EnKakasi3* self) {
    FUN_0036f4e4(&self->skelAnime, 0.0f);
    self->unk_242 = self->unk_246 = 0;
    self->unk_23C = 0;
    Math_ApproachZeroF(&self->skelAnime.curFrame, 0.5f, 1.0f);
    Math_SmoothStepToS(&self->actor.shape.rot.x, self->rot.x, 5, 0x2710, 0);
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->rot.y, 5, 0x2710, 0);
    Math_SmoothStepToS(&self->actor.shape.rot.z, self->rot.z, 5, 0x2710, 0);
}
// base
void FUN_003e56c0(EnKakasi3* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    func_80A90E28(self);
    FUN_00370734(&self->skelAnime);
    self->subCamId = -1;
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        if (!self->unk_22C) {
            if (self->unk_240 == 0) {
                self->actionFunc = (EnKakasi3ActionFunc)FUN_0029964c;
            } else {
                self->actionFunc = (EnKakasi3ActionFunc)FUN_002aa864;
            }
        } else {
            self->actionFunc = (EnKakasi3ActionFunc)FUN_0029964c;
        }
    } else {
        s16 angleTowardsLink = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

        if (!(self->actor.xzDistToPlayer > 120.0f)) {
            s16 absAngleTowardsLink = ABS(angleTowardsLink);

            if (absAngleTowardsLink < 0x4300) {
                if (!self->unk_22C) {
                    if (player->stateFlags2 & 0x1000000) {
                        self->subCamId = OnePointCutscene_Init(play, 2260, -99, &self->actor, 0);
                        FUN_00370778(play);
                        self->unk_250 = 0.0f;
                        self->dialogState = TEXT_STATE_EVENT;
                        FUN_00367c7c(play, 0x40A4, NULL);
                        player->stateFlags2 |= 0x800000;
                        self->actionFunc = (EnKakasi3ActionFunc)FUN_00274fa4;
                        return;
                    }
                    if (self->actor.xzDistToPlayer < 80.0f) {
                        player->stateFlags2 |= 0x800000;
                    }
                } else if (*((u8*)&gSaveContext + 0x12DD) != 0 && !self->unk_22D) {
                    if (player->stateFlags2 & 0x1000000) {
                        self->subCamId = OnePointCutscene_Init(play, 2260, -99, &self->actor, 0);
                        FUN_00370778(play);
                        self->unk_250 = 0.0f;
                        self->dialogState = TEXT_STATE_EVENT;
                        FUN_00367c7c(play, 0x40A8, NULL);
                        player->stateFlags2 |= 0x800000;
                        self->actionFunc = (EnKakasi3ActionFunc)FUN_0037989c;
                        return;
                    }
                    if (self->actor.xzDistToPlayer < 80.0f) {
                        player->stateFlags2 |= 0x800000;
                    }
                }
                FUN_0036bb28(&self->actor, play, 100.0f);
            }
        }
    }
}
#endif

void EnKakasi3_Init(Actor* actor, GameState* state) {
    EnKakasi3* self = (EnKakasi3*)actor;
    PlayState* play = (PlayState*)state;

    actor->targetMode = 6;
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, actor, &sCylinderInit);
    FUN_00372f38(actor, play, NULL);
    FUN_0034fe20(actor, play, &self->skelAnime, 0, 0, NULL, NULL, 0);
    actor->flags |= 0x400;
    self->rot = actor->world.rot;
    actor->colChkInfo.mass = MASS_IMMOVABLE;
    Actor_SetScale(actor, 0.01f);
    self->actionFunc = (EnKakasi3ActionFunc)FUN_00180b78;
}

void EnKakasi3_Destroy(Actor* actor, GameState* state) {
    EnKakasi3* self = (EnKakasi3*)actor;
    PlayState* play = (PlayState*)state;

    FUN_003508b8(actor, NULL);
    SkelAnime_Free2(&self->skelAnime);
    Collider_DestroyCylinder(play, &self->collider);
}

// DECR keeps armcc from fully unrolling the 4-timer loop (same fix as EnKakasi_Update).
void EnKakasi3_Update(EnKakasi3* self, PlayState* play) {
    s32 i;

    self->unk_230++;
    self->actor.world.rot = self->actor.shape.rot;
    for (i = 0; i < ARRAY_COUNT(self->unk_234); i++) {
        DECR(self->unk_234[i]);
    }

    Actor_SetFocus(&self->actor, 60.0f);
    self->actionFunc(self, play);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(play, &self->actor, 50.0f, 50.0f, 100.0f, 0x1C);
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
}

void EnKakasi3_Draw(Actor* actor, GameState* state) {
    EnKakasi3* self = (EnKakasi3*)actor;
    void* unk = actor->unk_178;
    GraphicsContext* gfx = *(GraphicsContext**)((u8*)state + 0x20AC);

    ((void (*)(GraphicsContext*, void*, Vec3f*))FUN_00357fd0)(gfx, unk, &actor->world.pos);
    SkelAnime_DrawOpa(&self->skelAnime, &actor->modelMtx, EnKakasi3_OverrideLimbDraw, EnKakasi3_PostLimbDraw, actor, 0);
}

s32 EnKakasi3_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg) {
}

void EnKakasi3_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg) {
}

}
