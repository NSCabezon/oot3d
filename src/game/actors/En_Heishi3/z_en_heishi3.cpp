extern "C" {
#include "z_en_heishi3.hpp"

#define FLAGS 0x00000000

void EnHeishi3_Init(Actor* actor, PlayState* play);
void EnHeishi3_SetupGuardType(EnHeishi3* self, PlayState* play);
void EnHeishi3_Destroy(Actor* actor, GameState* state);
void EnHeishi3_Update(Actor* actor, PlayState* play);
void FUN_00353c9c(Actor* actor, PlayState* play, SkelAnime* skelAnime, s32 cmbIndex, s32 csabIndex, void* jointTable, void* morphTable, s32 limbCount);
void EnHeishi3_Draw(Actor* actor, GameState* state);
s32 EnHeishi3_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void FUN_0034e01c(MTX34* mtx, Vec3s* rot);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void EnHeishi3_StandSentinelInGrounds(EnHeishi3* self, PlayState* play);
void EnHeishi3_StandSentinelInCastle(EnHeishi3* self, PlayState* play);
void EnHeishi3_ResetAnimationToIdle(EnHeishi3* self, PlayState* play);


ActorInit En_Heishi3_InitVars = {
    ACTOR_EN_HEISHI3,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_SD,
    sizeof(EnHeishi3),
    (ActorFunc)EnHeishi3_Init,
    (ActorFunc)EnHeishi3_Destroy,
    (ActorFunc)EnHeishi3_Update,
    (ActorFunc)EnHeishi3_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    { 0x0A, 0x00, 0x00, 0x39, 0x20, 0x01 },
    { 0, { 0x00000000, 0, 0 }, { 0x00000000, 0, 0 }, 0, 0, 1 },
    { 15.0f, 70.0f, 0.0f, { 0.0f, 0.0f, 0.0f } },
};

void EnHeishi3_Init(Actor* thisx, PlayState* play) {
    EnHeishi3* self = (EnHeishi3*)thisx;

    *(s16*)((u8*)play + 0x7FC4) = 0;
    if (self->actor.params <= 0) {
        self->unk_278 = 0;
    } else {
        self->unk_278 = 1;
        if (self->actor.world.pos.x < -290.0f) {
            self->unk_278 = 2;
        }
    }
    Actor_SetScale(thisx, 0.01f);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    FUN_00372f38(thisx, play, &self->skelAnimModel, 0, 0);
    FUN_00353c9c(thisx, play, &self->skelAnime, 0, 0, self->jointTable, self->morphTable, 16);
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    self->actor.targetMode = 6;
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, thisx, &sCylinderInit);
    self->actor.gravity = -3.0f;
    self->actor.focus.pos = self->actor.world.pos;
    self->actionFunc = EnHeishi3_SetupGuardType;
}


void EnHeishi3_Destroy(Actor* actor, GameState* state) {
    EnHeishi3* self = (EnHeishi3*)actor;
    PlayState* play = (PlayState*)state;

    Collider_DestroyCylinder(play, &self->collider);
    FUN_003508b8(actor, self->skelAnimModel, 0);
    SkelAnime_Free2(&self->skelAnime);
}

void FUN_0010a998(EnHeishi3* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if ((Message_GetState(&play->msgCtx) == 5) && Message_ShouldAdvance(play) && (self->respawnFlag == 0)) {
        gSaveContext.eventChkInf[4] |= 0x4000;
        FUN_003716f0(play, 0x47E, 0x14, 0x2E);
        self->respawnFlag = 1;
        *((u8*)&gSaveContext + 0x15AB) = 0x2E;
    }
}

void FUN_00107498(EnHeishi3* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if (Animation_OnFrameImpl(&self->skelAnime, 1.0f, 1.0f) || Animation_OnFrameImpl(&self->skelAnime, 17.0f, 1.0f)) {
        Audio_PlayActorSound2(&self->actor, 0x01000247);
    }
    if (self->caughtTimer == 0) {
        self->actor.speedXZ = 0.0f;
        self->actionFunc = EnHeishi3_ResetAnimationToIdle;
    } else {
        Math_SmoothStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 5, 3000, 0);
    }
}

void EnHeishi3_Update(Actor* thisx, PlayState* play) {
    EnHeishi3* self = (EnHeishi3*)thisx;

    Actor_SetFocus(thisx, 60.0f);
    self->unk_274++;
    if (self->caughtTimer != 0) {
        self->caughtTimer--;
    }
    self->actionFunc(self, play);
    self->actor.shape.rot = self->actor.world.rot;
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(play, &self->actor, 20.0f, 20.0f, 50.0f, 0x1C);
    Collider_UpdateCylinder(thisx, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
}



void EnHeishi3_Draw(Actor* actor, GameState* state) {
    EnHeishi3* self = (EnHeishi3*)actor;
    PlayState* play = (PlayState*)state;

    GraphicsContext_UnknownStub(play->state.gfxCtx);
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, EnHeishi3_OverrideLimbDraw, NULL, actor, 0);
}

void EnHeishi3_CatchStart(EnHeishi3* self, PlayState* play) {
    s32 frameCount = Animation_GetLastFrame(&self->skelAnime, 1);

    Animation_Change_00375c08(&self->skelAnime, 1, 1.0f, 0.0f, frameCount, 0, -10.0f);
    self->caughtTimer = 30;
    self->actor.speedXZ = 2.5f;
    self->actionFunc = FUN_00107498;
}

s32 EnHeishi3_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnHeishi3* self = (EnHeishi3*)thisx;

    if (limbIndex == 8) {
        FUN_0034e01c(mtx, &self->limb8Rot);
    }
    if (limbIndex == 15) {
        FUN_0034e01c(mtx, &self->limb15Rot);
    }
    return 0;
}

void EnHeishi3_ResetAnimationToIdle(EnHeishi3* self, PlayState* play) {
    s32 frameCount = Animation_GetLastFrame(&self->skelAnime, 0);

    Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, frameCount, 0, -10.0f);
    self->actionFunc = FUN_0010a998;
}

void EnHeishi3_SetupGuardType(EnHeishi3* self, PlayState* play) {
    s32 frameCount = Animation_GetLastFrame(&self->skelAnime, 0);

    Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, frameCount, 0, -10.0f);
    if (self->unk_278 == 0) {
        self->actionFunc = EnHeishi3_StandSentinelInGrounds;
    } else {
        self->actionFunc = EnHeishi3_StandSentinelInCastle;
    }
}

extern __pure float _fabsf(float);

static __inline __pure float EnHeishi3_Fabsf(float f) {
    return _fabsf(f);
}

static inline void EnHeishi3_PlaySfxCentered(u32 sfxId) {
    Audio_PlaySoundGeneral(sfxId, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, (s8*)&DAT_0054ac24);
}

void EnHeishi3_StandSentinelInCastle(EnHeishi3* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    FUN_003731e0(&self->skelAnime);
    if ((player->actor.world.pos.x < -190.0f) && (player->actor.world.pos.x > -380.0f) &&
        (EnHeishi3_Fabsf(player->actor.world.pos.y - self->actor.world.pos.y) < 100.0f) &&
        (player->actor.world.pos.z < 1020.0f) && (player->actor.world.pos.z > 700.0f) &&
        (*(s16*)((u8*)play + 0x7FC4) == 0)) {
        if (self->unk_278 == 1) {
            if (player->actor.world.pos.x < -290.0f) {
                return;
            }
        } else {
            if (player->actor.world.pos.x > -290.0f) {
                return;
            }
        }
        *(s16*)((u8*)play + 0x7FC4) = 1;
        FUN_00367c7c(play, 0x702D, &self->actor);
        EnHeishi3_PlaySfxCentered(0x010004B1);
        FUN_0036e980(play, &self->actor, 1);
        self->actionFunc = EnHeishi3_CatchStart;
    }
}


void EnHeishi3_StandSentinelInGrounds(EnHeishi3* self, PlayState* play) {
    Player* player;
    s16 yawDiff;
    s16 yawDiffNew;
    f32 sightRange;

    player = GET_PLAYER(play);
    FUN_003731e0(&self->skelAnime);
    yawDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
    yawDiffNew = ABS(yawDiff);
    if (yawDiffNew < 0x4300) {
        if (IS_DAY) {
            sightRange = 250.0f;
        } else {
            sightRange = 200.0f;
        }
    } else {
        if (IS_DAY) {
            sightRange = 150.0f;
        } else {
            sightRange = 100.0f;
        }
    }
    if ((self->actor.xzDistToPlayer < sightRange) &&
        (EnHeishi3_Fabsf(player->actor.world.pos.y - self->actor.world.pos.y) < 100.0f) &&
        (*(s16*)((u8*)play + 0x7FC4) == 0) && !(*(u32*)((u8*)player + 0x1710) & 0x20000000)) {
        *(s16*)((u8*)play + 0x7FC4) = 1;
        FUN_00367c7c(play, 0x702D, &self->actor);
        self->actor.flags |= 0x10;
        EnHeishi3_PlaySfxCentered(0x010004B1);
        FUN_0036e980(play, &self->actor, 1);
        self->actionFunc = EnHeishi3_CatchStart;
    }
}



}
