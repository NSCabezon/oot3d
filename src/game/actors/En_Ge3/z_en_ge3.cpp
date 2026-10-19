extern "C" {
#include "z_en_ge3.hpp"

#define FLAGS 0x00000019

void FUN_0034df48(MTX34* mtx, Vec3s* rot);
void Animation_PlayLoop(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
extern u8 En_Ge3_sAnimationModes[];
extern EnGe3ActionFunc En_Ge3_sActionFuncs[];
extern s32 En_Ge3_sAnimations[];
extern ColliderCylinderInit En_Ge3_sCylinderInit;
void FUN_0036bcc8(PlayState* play, Actor* actor, Vec3s* headRot, Vec3s* torsoRot, f32 x, f32 y, f32 z, s32 arg7);
s32 FUN_00369a48(Actor* actor, PlayState* play);
void FUN_003725e0(PlayState* play);
void FUN_00359220(Actor* actor, PlayState* play, s32 arg, f32 a, f32 b);
void FUN_0036932c(SkeletonAnimationModel* model, s32 index);
void EnGe3_ForceTalk(EnGe3* self, PlayState* play);
s32 EnGe3_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void EnGe3_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);

void EnGe3_Init(Actor* actor, GameState* state);
void EnGe3_Destroy(Actor* actor, GameState* state);
void EnGe3_Update(Actor* actor, GameState* state);
void EnGe3_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_Ge3.data.s")

GLOBAL_ASM("../src/game/actors/En_Ge3/data_syms.s")

ActorInit En_Ge3_InitVars = {
    ACTOR_EN_GE3,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_GELDB,
    sizeof(EnGe3),
    (ActorFunc)EnGe3_Init,
    (ActorFunc)EnGe3_Destroy,
    (ActorFunc)EnGe3_Update,
    (ActorFunc)EnGe3_Draw,
};

#if 0 // draft: 13 words off (sub r0,r6,#8 for the mode byte + r5/r6 swap; needs real adjacent statics, zero-init static would go to .bss)
static void EnGe3_ChangeAction(EnGe3* self, s32 i) {
    self->actionFunc = En_Ge3_sActionFuncs[i];

    Animation_Change_00375c08(&self->skelAnime, En_Ge3_sAnimations[i], 1.0f, 0.0f,
                              (f32)Animation_GetLastFrame(&self->skelAnime, En_Ge3_sAnimations[i]),
                              En_Ge3_sAnimationModes[i], -8.0f);

    self->unk_30C &= ~2;
}

void EnGe3_Init(Actor* actor, GameState* state) {
    EnGe3* self = (EnGe3*)actor;
    PlayState* play = (PlayState*)state;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 36.0f);
    FUN_00372f38(actor, play, &self->model, 0, 0);
    FUN_00353c9c(actor, play, &self->skelAnime, 0, 9, self->jointTable, self->morphTable, 0x13);
    Animation_PlayLoop(&self->skelAnime, 9);
    SkelAnime_SetFaceAnimations(&self->faceAnimations, &self->skelAnime, 0, -1, -1);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, &En_Ge3_sCylinderInit);
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    Actor_SetScale(&self->actor, 0.01f);
    self->actor.world.rot.z = 0;
    self->actor.shape.rot.z = 0;
    EnGe3_ChangeAction(self, 0);
    self->actionFunc = EnGe3_ForceTalk;
    self->actor.minVelocityY = -4.0f;
    self->actor.gravity = -1.0f;
    self->unk_30C = 0;
    self->actor.targetMode = 6;
}
#endif

GLOBAL_ASM("asm/EnGe3_Init.s")

void EnGe3_Destroy(Actor* actor, GameState* state) {
    EnGe3* self = (EnGe3*)actor;
    PlayState* play = (PlayState*)state;

    Collider_DestroyCylinder(play, &self->collider);
    FUN_003508b8(actor, self->model, 0);
    SkelAnime_Free2(&self->skelAnime);
    self->faceAnimations.Destroy();
}

static void EnGe3_UpdateCollision(EnGe3* self, PlayState* play) {
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    Actor_UpdateBgCheckInfo(play, &self->actor, 40.0f, 25.0f, 40.0f, 5);

    EnGe3* s2 = self;
    if (!(s2->unk_30C & 2) && FUN_00370734(&self->skelAnime)) {
        s2->unk_30C |= 2;
    }
}

static void EnGe3_MoveAndBlink(EnGe3* self, PlayState* play) {
    Actor_MoveForward(&self->actor);

    if (DECR(self->blinkTimer) == 0) {
        self->blinkTimer = Rand_S16Offset_003702c8(60, 60);
    }

    self->eyeIndex = self->blinkTimer;

    if (self->eyeIndex >= 3) {
        self->eyeIndex = 0;
    }
}

void EnGe3_Update(Actor* thisx, GameState* state) {
    EnGe3* self = (EnGe3*)thisx;
    PlayState* play = (PlayState*)state;

    EnGe3_UpdateCollision(self, play);
    self->actionFunc(self, play);
    EnGe3_MoveAndBlink(self, play);
}

void EnGe3_Draw(Actor* actor, GameState* state) {
    EnGe3* self = (EnGe3*)actor;

    GraphicsContext_UnknownStub(state->gfxCtx);
    FaceAnimations* face = &self->faceAnimations;
    face->FUN_0035e3a4(0, self->eyeIndex);
    FUN_0036932c(self->skelAnime.unk_28, 3);
    FUN_0036932c(self->skelAnime.unk_28, 4);
    FUN_0036932c(self->skelAnime.unk_28, 5);
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, EnGe3_OverrideLimbDraw, EnGe3_PostLimbDraw, actor, 0);
    self->faceAnimations.FUN_0035e330();
}

static inline void EnGe3_LookAtPlayer(EnGe3* self, PlayState* play);
void EnGe3_GiveCard(EnGe3* self, PlayState* play);
void EnGe3_Wait(EnGe3* self, PlayState* play);
void EnGe3_ForceTalk(EnGe3* self, PlayState* play);

void EnGe3_ForceTalk(EnGe3* self, PlayState* play) {
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        self->actionFunc = EnGe3_GiveCard;
    } else {
        if (!(self->unk_30C & 4)) {
            FUN_0036e980(play, &self->actor, 7);
            self->unk_30C |= 4;
        }
        self->actor.textId = 0x6004;
        self->actor.flags |= 0x10000;
        FUN_00359220(&self->actor, play, 0, 300.0f, 300.0f);
    }
    EnGe3_LookAtPlayer(self, play);
}

void EnGe3_WaitTillCardGiven(EnGe3* self, PlayState* play);

void EnGe3_GiveCard(EnGe3* self, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_EVENT) && Message_ShouldAdvance(play)) {
        FUN_003725e0(play);
        self->actor.flags &= ~0x10000;
        self->actionFunc = EnGe3_WaitTillCardGiven;
        GiveItem(&self->actor, play, 0x3A, 10000.0f, 50.0f);
    }
}

void FUN_0034df48(MTX34* mtx, Vec3s* rot);

s32 EnGe3_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnGe3* self = (EnGe3*)thisx;

    if (limbIndex == 9) {
        FUN_0034df48(mtx, &self->headRot);
    }
    return 0;
}

static inline void EnGe3_MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}

void EnGe3_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnGe3* self = (EnGe3*)thisx;
    Vector3f src(600.0f, 700.0f, 0.0f);

    if (limbIndex == 9) {
        EnGe3_MultVec3f(&src, (Vector3f*)&self->actor.focus.pos, mtx);
    }
}

void EnGe3_UpdateWhenNotTalking(Actor* thisx, PlayState* play) {
    EnGe3* self = (EnGe3*)thisx;

    EnGe3_UpdateCollision(self, play);
    self->actionFunc(self, play);

    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        self->actionFunc = EnGe3_Wait;
        thisx->update = (ActorFunc)EnGe3_Update;
    } else {
        thisx->textId = 0x6005;
        if (self->actor.xzDistToPlayer < 100.0f) {
            FUN_0036bb28(&self->actor, play, 100.0f);
        }
    }

    EnGe3_MoveAndBlink(self, play);
}

void EnGe3_UpdateWhenNotTalking(Actor* thisx, PlayState* play);
void EnGe3_WaitLookAtPlayer(EnGe3* self, PlayState* play);

void EnGe3_Wait(EnGe3* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        self->actionFunc = EnGe3_WaitLookAtPlayer;
        self->actor.update = (ActorFunc)EnGe3_UpdateWhenNotTalking;
        self->actor.flags &= ~0x10000;
    }

    s16 angleDiff = (s16)(self->actor.yawTowardsPlayer - self->actor.shape.rot.y);
    if (ABS(angleDiff) <= 0x4000) {
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 6, 4000, 100);
        s16* p = &self->actor.world.rot.y;
        *p = self->actor.shape.rot.y;
        FUN_0036bcc8(play, &self->actor, &self->headRot, &self->unk_306, self->actor.focus.pos.x,
                     self->actor.focus.pos.y, self->actor.focus.pos.z, 0x4300);
    } else {
        if (angleDiff < 0) {
            Math_SmoothStepToS(&self->headRot.y, -0x2000, 6, 6200, 0x100);
        } else {
            Math_SmoothStepToS(&self->headRot.y, 0x2000, 6, 6200, 0x100);
        }

        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 12, 1000, 100);
        self->actor.world.rot.y = self->actor.shape.rot.y;
    }
}

static inline void EnGe3_LookAtPlayer(EnGe3* self, PlayState* play) {
    s16 angleDiff = (s16)(self->actor.yawTowardsPlayer - self->actor.shape.rot.y);
    if ((ABS(angleDiff) <= 0x2300) &&
        (self->actor.xzDistToPlayer < 100.0f)) {
        FUN_0036bcc8(play, &self->actor, &self->headRot, &self->unk_306, self->actor.focus.pos.x,
                     self->actor.focus.pos.y, self->actor.focus.pos.z, 0x4300);
    } else {
        Math_SmoothStepToS(&self->headRot.x, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->headRot.y, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->unk_306.x, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->unk_306.y, 0, 6, 6200, 100);
    }
}

void EnGe3_WaitLookAtPlayer(EnGe3* self, PlayState* play) {
    EnGe3_LookAtPlayer(self, play);
}

void EnGe3_Wait(EnGe3* self, PlayState* play);

void EnGe3_WaitTillCardGiven(EnGe3* self, PlayState* play) {
    if (Actor_HasParent(&self->actor, play)) {
        self->actor.parent = NULL;
        self->actionFunc = EnGe3_Wait;
    } else {
        GiveItem(&self->actor, play, 0x3A, 10000.0f, 50.0f);
    }
}

}
