extern "C" {
#include "z_en_diving_game.hpp"

#define FLAGS 0x00000019

void EnDivingGame_Init(Actor* actor, GameState* state);
void EnDivingGame_Destroy(Actor* actor, GameState* state);
void EnDivingGame_Update(Actor* thisx, GameState* state);
void EnDivingGame_Draw(Actor* thisx, GameState* state);

// 3DS helpers (see functions_*.cpp); FUN_00239714 = N64 EnDivingGame_Reset (action func).
void FUN_0034fe20(Actor* actor, PlayState* play, SkelAnime* skelAnime, s32 cmbIndex, s32 csabIndex, void* jointTable,
                  void* morphTable, s32 limbCount);
void FUN_00239714(EnDivingGame* self, PlayState* play);
void FUN_00274ff8(EnDivingGame* self, PlayState* play);
s32 FUN_00334950(Actor* actor, PlayState* play);  // EnDivingGame_HasMinigameFinished? (shared 3DS fn)
void FUN_002991d4(EnDivingGame* self, PlayState* play);
void FUN_0021c5f8(EnDivingGame* self, PlayState* play);
void FUN_001cd0a4(EnDivingGame* self, PlayState* play);
void FUN_00358e70(void);                                                              // Audio_SetFastTempoForTimedMinigame
void FUN_0034c664(Actor* actor, EnDivingGameInteractInfo* info, s32 arg2, s32 arg3);  // Npc_TrackPoint
void EffectSsGRipple_Spawn(PlayState* play, Vec3f* pos, s32 a, s32 b, s32 c);
void __ARM_common_memcpy2_6(void* dst, void* src);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
s32 FUN_00394190(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg);

ActorInit En_Diving_Game_InitVars = {
    ACTOR_EN_DIVING_GAME,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_ZO,
    sizeof(EnDivingGame),
    (ActorFunc)EnDivingGame_Init,
    (ActorFunc)EnDivingGame_Destroy,
    (ActorFunc)EnDivingGame_Update,
    (ActorFunc)EnDivingGame_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    { 0x0A, 0x00, 0x00, 0x39, 0x20, 0x01 },
    { 0, { 0x00000000, 0, 0 }, { 0x00000000, 0, 0 }, 0, 0, 1 },
    { 10.0f, 10.0f, 0.0f, { 0.0f, 0.0f, 0.0f } },
};

// Matched: 3DS action func (N64 EnDivingGame_Reset). Its asm is no longer in functions_200000s.cpp.
void FUN_00239714(EnDivingGame* self, PlayState* play) {
    f32 frameCount = Animation_GetLastFrame(&self->skelAnime, 0);

    Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, frameCount, 0, -10.0f);
    self->notPlayingMinigame = true;
    self->actionFunc = FUN_00274ff8;
}

void EnDivingGame_Init(Actor* actor, GameState* state) {
    EnDivingGame* self = (EnDivingGame*)actor;
    PlayState* play = (PlayState*)state;

    // ROM passes only (actor, play, 0): r3 is never set.
    FUN_00372f38(actor, play, NULL);
    self->actor.gravity = -3.0f;
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    FUN_0034fe20(actor, play, &self->skelAnime, 0, 0, self->jointTable, self->morphTable, 21);
    SkelAnime_SetFaceAnimations((FaceAnimations*)self->faceAnimations, &self->skelAnime, 0, -1, -1);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, &sCylinderInit);
    self->actor.room = -1;
    self->actor.scale.x = 0.01f;
    self->actor.scale.y = 0.012999999f;
    self->actor.scale.z = 0.0139999995f;
    if (*(s32*)((u8*)play + 0x7F78) != 0) {  // N64 sIsInitialized, moved into PlayState on 3DS
        self->isDuplicate = 1;
        Actor_Kill(actor);
    } else {
        *(s32*)((u8*)play + 0x7F78) = 1;
        self->actor.targetMode = 0;
        self->actor.colChkInfo.mass = MASS_IMMOVABLE;
        if (gSaveContext.eventChkInf[3] & 0x8000) {
            self->state = 1;
        }
        self->actionFunc = FUN_00239714;
    }
}

void EnDivingGame_Destroy(Actor* actor, GameState* state) {
    EnDivingGame* self = (EnDivingGame*)actor;

    if (!self->isDuplicate) {
        gSaveContext.timer1State = 0;
    }
    ((FaceAnimations*)self->faceAnimations)->Destroy();
    SkelAnime_Free2(&self->skelAnime);
    FUN_00350f34(actor, NULL);  // ROM passes only (actor, 0)
    Collider_DestroyCylinder((PlayState*)state, &self->collider);
}

// N64 EnDivingGame_Idle.
void FUN_00274ff8(EnDivingGame* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if (self->state != 2 || !FUN_00334950(&self->actor, play)) {
        if (Actor_ProcessTalkRequest(&self->actor, play)) {
            if (self->nextTextState != 6) {
                switch (self->state) {
                    case 2:
                        self->actionFunc = FUN_0021c5f8;
                        break;
                    case 1:
                        self->actionFunc = FUN_002991d4;
                        break;
                    case 0:
                        FUN_0036e980(play, NULL, 8);
                        self->actionFunc = FUN_001cd0a4;
                        break;
                }
            }
        } else {
            if (Text_GetFaceReaction(play, 0x1D) != 0) {
                self->actor.textId = Text_GetFaceReaction(play, 0x1D);
                self->nextTextState = 6;
            } else {
                switch (self->state) {
                    case 0:
                        self->nextTextState = 4;
                        if (!(gSaveContext.eventChkInf[3] & 0x100)) {
                            self->actor.textId = 0x4053;
                            self->phase = 1;
                        } else {
                            self->actor.textId = 0x405C;
                            self->phase = 2;
                        }
                        break;
                    case 1:
                        self->actor.textId = 0x4056;
                        self->nextTextState = 5;
                        break;
                    case 2:
                        self->actor.textId = 0x405B;
                        self->nextTextState = 5;
                        break;
                }
            }
            FUN_0036bb28(&self->actor, play, 80.0f);
        }
    }
}

// 3DS timer scaling (30 vs 20 fps): n * 3 / 2, rounded to nearest. Duplicates n like the ROM does.
#define FR(n) ((n) > 0 ? (s32)(0.5f + (f32)(n) * 3.0f / 2.0f) : (s32)((f32)(n) * 3.0f / 2.0f - 0.5f))

void EnDivingGame_Update(Actor* thisx, GameState* state) {
    EnDivingGame* self = (EnDivingGame*)thisx;
    PlayState* play = (PlayState*)state;
    Player* player = GET_PLAYER(play);
    Vec3f pos;

    if (self->subCamTimer != 0) {
        self->subCamTimer--;
    }
    if (self->throwTimer != 0) {
        self->throwTimer--;
    }
    if (self->eyeTimer != 0) {
        self->eyeTimer--;
    }
    if (self->spawnRuppyTimer != 0) {
        self->spawnRuppyTimer--;
    }
    if (gSaveContext.timer1Value == 10) {
        FUN_00358e70();
    }
    if (self->eyeTimer == 0) {
        self->eyeTimer = 2;
        self->eyeTexIndex++;
        if (self->eyeTexIndex > 2) {
            self->eyeTexIndex = 0;
            self->eyeTimer = FR((s16)Rand_ZeroFloat(60.0f) + 20);
        }
    }
    self->actionFunc(self, play);
    Actor_SetFocus(&self->actor, 70.0f);
    self->interactInfo.trackPos = player->actor.world.pos;
    self->interactInfo.trackPos.y = player->actor.world.pos.y;
    FUN_0034c664(&self->actor, &self->interactInfo, 2, 4);
    self->headRot.x = self->interactInfo.headRot.x;
    self->headRot.y = self->interactInfo.headRot.y;
    self->headRot.z = self->interactInfo.headRot.z;
    __ARM_common_memcpy2_6(&self->torsoRot, &self->interactInfo.torsoRot);  // Vec3s copy
    if ((*(u32*)((u8*)play + 0x5BF4) % 16) == 0) {  // play->gameplayFrames
        pos = self->actor.world.pos;
        pos.y += 20.0f;
        EffectSsGRipple_Spawn(play, &pos, 100, 500, 30);
    }
    self->unk_C8C++;
    Actor_UpdateBgCheckInfo(play, &self->actor, 20.0f, 20.0f, 60.0f, 0x1D);
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
}

void EnDivingGame_Draw(Actor* thisx, GameState* state) {
    EnDivingGame* self = (EnDivingGame*)thisx;
    FaceAnimations* fa = (FaceAnimations*)self->faceAnimations;

    fa->FUN_0035e3a4(0, self->eyeTexIndex);
    ((FaceAnimations*)self->faceAnimations)->FUN_0035e330();
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, (OverrideLimbDrawOpa)FUN_00394190, NULL, thisx, 0);
}
}
