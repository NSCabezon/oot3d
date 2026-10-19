extern "C" {
#include "z_en_hs.hpp"

#define FLAGS 0x00000009

void FUN_00353c9c(Actor* actor, PlayState* play, SkelAnime* skelAnime, s32 cmbIndex, s32 csabIndex,
                  void* jointTable, void* morphTable, s32 limbCount);
void FUN_003ae4b8(EnHs* self, PlayState* play); // EnHs_SleepingInteract (N64)
void FUN_00112544(EnHs* self, PlayState* play); // EnHs_SleepingTalk (N64)
void FUN_0012d02c(EnHs* self, PlayState* play); // EnHs_SeeingCojiro (N64)
void FUN_0012d108(EnHs* self, PlayState* play); // EnHs_OddMushroomChoice (N64)
void FUN_003d0d0c(EnHs* self, PlayState* play); // EnHs_OddMushroomNo (N64)
void FUN_00104bf8(EnHs* self, PlayState* play); // EnHs_OddMushroomYes (N64)
void FUN_00104b8c(EnHs* self, PlayState* play); // EnHs_GivingOddMushroom (N64)
void FUN_003b02c0(EnHs* self, PlayState* play); // EnHs_SittingAwake (N64)
void FUN_0012cf7c(EnHs* self, PlayState* play); // EnHs_SittingAwakeTalk (N64)
s32 GiveItem(Actor* actor, PlayState* play, s32 getItemId, f32 xyRange, f32 yRange);
u32 FUN_00369a48(Actor* actor, PlayState* play); // Actor_TextboxIsClosing (N64)
u32 Actor_ProcessTalkRequest(Actor* actor, PlayState* play);
void FUN_0036bbd0(Actor* actor, PlayState* play, s32 exchangeItem, f32 range);
void FUN_0036f59c(Player* player, s32 sfxId);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void FUN_0036bcc8(PlayState* play, Actor* actor, Vec3s* headRot, Vec3s* torsoRot, Vec3f focusPos, s32 arg5);
void FUN_003625f8(MTX34* mtx, Vec3f* axis, f32 angle);
void FUN_0036c174(MTX34* dst, MTX34* src, MTX34* mtx);
void Animation_PlayLoop(SkelAnime* skelAnime, s32 idx);
void SkelAnime_SetCurrentFrame2(SkelAnime* skelAnime, f32 frame);
void SkelAnime_Free2(SkelAnime* skelAnime);
void GraphicsContext_UnknownStub(GraphicsContext* gfxCtx);
void VEC3TransformAsm(Vector3f*, MTX34*, Vector3f*);
s32 EnHs_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void EnHs_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);

void EnHs_Init(Actor* thisx, PlayState* play);
void EnHs_Destroy(Actor* thisx, PlayState* play);
void EnHs_Update(Actor* thisx, PlayState* play);
void EnHs_Draw(Actor* thisx, PlayState* play);


ActorInit En_Hs_InitVars = {
    ACTOR_EN_HS,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_HS,
    sizeof(EnHs),
    (ActorFunc)EnHs_Init,
    (ActorFunc)EnHs_Destroy,
    (ActorFunc)EnHs_Update,
    (ActorFunc)EnHs_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    { 0x0A, 0x00, 0x11, 0x39, 0x10, 0x01 },
    { 0, { 0x00000000, 0, 0 }, { 0xFFCFFFFF, 0, 0 }, 0, 1, 1 },
    { 40.0f, 40.0f, 0.0f, { 0.0f, 0.0f, 0.0f } },
};

// FUN_00370734 returns SkelAnime_Update's result; include declares it void.
static inline s32 EnHs_SkelAnimeUpdate(SkelAnime* skelAnime) {
    return FUN_00370734(skelAnime);
}

// N64 func_80A6E3A0. Init matches only with `self->actor.targetMode` (via thisx-> armcc merges the two actionFunc
// stores and the 6 lands in r1).
static inline void EnHs_SetActionFunc(EnHs* self, EnHsActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void EnHs_Init(Actor* thisx, PlayState* play) {
    EnHs* self = (EnHs*)thisx;

    ActorShape_Init(&thisx->shape, 0.0f, ActorShadow_DrawCircle, 36.0f);
    FUN_00372f38(thisx, play, &self->skelAnimModel, 0, 0);
    FUN_00353c9c(thisx, play, &self->skelAnime, 0, 2, self->jointTable, self->morphTable, 11);
    Animation_PlayLoop(&self->skelAnime, 2);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, thisx, &sCylinderInit);
    thisx->colChkInfo.mass = MASS_IMMOVABLE;
    Actor_SetScale(thisx, 0.01f);
    if (!LINK_IS_ADULT) {
        thisx->params = 0;
    } else {
        thisx->params = 1;
    }
    if (thisx->params == 1) {
        EnHs_SetActionFunc(self, FUN_003ae4b8);
        if (gSaveContext.itemGetInf[3] & 0x1) {
            Actor_Kill(thisx);
        }
    } else {
        EnHs_SetActionFunc(self, FUN_003ae4b8);
    }
    self->trackingFlags = 0;
    self->actor.targetMode = 6;
    self->unk_70C = 1;
}

void EnHs_Destroy(Actor* thisx, PlayState* play) {
    EnHs* self = (EnHs*)thisx;

    Collider_DestroyCylinder(play, &self->collider);
    FUN_003508b8(thisx, self->skelAnimModel, 0);
    SkelAnime_Free2(&self->skelAnime);
}

void EnHs_Update(Actor* thisx, PlayState* play) {
    EnHs* self = (EnHs*)thisx;

    Collider_UpdateCylinder(thisx, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    Actor_MoveForward(thisx);
    Actor_UpdateBgCheckInfo(play, thisx, 0.0f, 0.0f, 0.0f, 4);
    if (EnHs_SkelAnimeUpdate(&self->skelAnime)) {
        SkelAnime_SetCurrentFrame2(&self->skelAnime, 0.0f);
    }
    self->actionFunc(self, play);
    if (self->trackingFlags & 1) {
        FUN_0036bcc8(play, thisx, &self->headRot, &self->torsoRot, thisx->focus.pos, 0x4300);
        self->trackingFlags &= ~1;
    } else {
        Math_SmoothStepToS(&self->headRot.x, 0x3200, 6, 0x1838, 100);
        Math_SmoothStepToS(&self->headRot.y, 0, 6, 0x1838, 100);
        Math_SmoothStepToS(&self->torsoRot.x, 0, 6, 0x1838, 100);
        Math_SmoothStepToS(&self->torsoRot.y, 0, 6, 0x1838, 100);
    }
}

void EnHs_Draw(Actor* thisx, PlayState* play) {
    EnHs* self = (EnHs*)thisx;

    GraphicsContext_UnknownStub(play->state.gfxCtx);
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, EnHs_OverrideLimbDraw, EnHs_PostLimbDraw, thisx, 0);
}

s32 EnHs_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnHs* self = (EnHs*)thisx;

    if (limbIndex == 10) {
        u8 enabled = self->unk_70C;
        if (enabled) {
            MTX34 rotX;
            MTX34 rotZ;
            Vector3f axis(1.0f, 0.0f, 0.0f);
            Vec3f* p = (Vec3f*)&axis;
            f32 angle = self->headRot.y * 0.000095873802f; // BINANG_TO_RAD

            FUN_003625f8(&rotX, p, angle);
            axis = Vector3f(0.0f, 0.0f, 1.0f);
            FUN_003625f8(&rotZ, p, self->headRot.x * 0.000095873802f);
            FUN_0036c174(mtx, mtx, &rotZ);
            FUN_0036c174(mtx, mtx, &rotX);
        }
    }
    return 0;
}

// Skipped (PostLimbDraw): function-local static sFocusOffset needs a __cxa_guard (guard 0x00527380, object 0x0059AFF0);
// same link problem as En_Hs2 PostLimbDraw -> left as asm.
// Function-local static: guard 0x00527380 (TU .data, cut from the data chunk), object 0x0059AFF0 pinned in
// data/oot3d.ld (scripts/pin_static.py).
void EnHs_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    static Vector3f sMultVec(300.0f, 1000.0f, 0.0f);
    EnHs* self = (EnHs*)thisx;
    Vector3f* dst;

    if (limbIndex == 10) {
        dst = (Vector3f*)&self->actor.focus.pos;
        VEC3TransformAsm(dst, mtx, &sMultVec);
    }
}

void FUN_00112544(EnHs* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        self->actionFunc = FUN_003ae4b8;
    }
}

void FUN_003ae4b8(EnHs* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Actor* actor = &self->actor;

    if (Actor_ProcessTalkRequest(actor, play)) {
        if (FUN_0036bc84(play) == 7) {
            player->actor.textId = 0x10B2;
            self->actionFunc = FUN_0012d02c;
            Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, (f32)Animation_GetLastFrame(&self->skelAnime, 0), 0,
                                      8.0f);
            self->cojiroSfxTimer = 40;
            FUN_00372244(&play->unk_5FCC, 0x1E, 0x100048C); // NA_SE_SY_TRE_BOX_APPEAR
        } else {
            player->actor.textId = 0x10B1;
            self->actionFunc = FUN_00112544;
        }
    } else {
        s16 d = actor->yawTowardsPlayer - actor->shape.rot.y;
        u16* tid = &actor->textId;
        *tid = 0x10B1;
        if (ABS(d) < 0x27D9 && actor->xzDistToPlayer < 100.0f) {
            FUN_0036bbd0(actor, play, 7, 100.0f);
        }
    }
}

void FUN_0012d02c(EnHs* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (Message_GetState(&play->msgCtx) == 5 && Message_ShouldAdvance(play)) {
        Message_ContinueTextbox(play, 0x10B3);
        self->actionFunc = FUN_0012d108;
        Animation_Change_00375c08(&self->skelAnime, 1, 1.0f, 0.0f, (f32)Animation_GetLastFrame(&self->skelAnime, 1), 0,
                                  8.0f);
        self->unk_70C = 0;
    }
    if (self->cojiroSfxTimer > 0) {
        self->cojiroSfxTimer--;
        if (self->cojiroSfxTimer == 0) {
            FUN_0036f59c(player, 0x0100016A); // NA_SE_EV_CHICKEN_CRY_M
        }
    }
    self->trackingFlags |= 1;
}

void FUN_0012d108(EnHs* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 4 && Message_ShouldAdvance(play)) {
        switch (FUN_00369f3c(play)) {
            case 0:
                self->actionFunc = FUN_00104bf8;
                self->unk_70C = 1;
                GiveItem(&self->actor, play, 0x1F, 10000.0f, 50.0f);
                break;
            case 1:
                Message_ContinueTextbox(play, 0x10B4);
                self->actionFunc = FUN_003d0d0c;
                break;
        }
        Animation_Change_00375c08(&self->skelAnime, 2, 1.0f, 0.0f, (f32)Animation_GetLastFrame(&self->skelAnime, 2), 0,
                                  8.0f);
    }
    self->trackingFlags |= 1;
}

void FUN_003d0d0c(EnHs* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        self->actionFunc = FUN_003ae4b8;
    }
}

void FUN_00104bf8(EnHs* self, PlayState* play) {
    if (Actor_HasParent(&self->actor, play)) {
        self->actor.parent = NULL;
        self->actionFunc = FUN_00104b8c;
    } else {
        GiveItem(&self->actor, play, 0x1F, 10000.0f, 50.0f);
    }
    self->trackingFlags |= 1;
}

void FUN_00104b8c(EnHs* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 6 && Message_ShouldAdvance(play)) {
        FUN_00371e6c(180);
        self->actionFunc = FUN_003b02c0;
        gSaveContext.eventInf[1] &= ~1;
    }
    self->trackingFlags |= 1;
}

void FUN_0012cf7c(EnHs* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        self->actionFunc = FUN_003b02c0;
    }
    self->trackingFlags |= 1;
}

static inline s32 EnHs_SittingAwakeInteract(EnHs* self, PlayState* play, u16 textId, EnHsActionFunc actionFunc) {
    Actor* actor = &self->actor;

    if (Actor_ProcessTalkRequest(actor, play)) {
        self->actionFunc = actionFunc;
        return 1;
    }
    actor->textId = textId;
    s16 d = actor->yawTowardsPlayer - actor->shape.rot.y;
    if (ABS(d) < 0x2151 && actor->xzDistToPlayer < 100.0f) {
        self->trackingFlags |= 1;
        FUN_0036bb28(actor, play, 100.0f);
    }
    return 0;
}

void FUN_003b02c0(EnHs* self, PlayState* play) {
    EnHs_SittingAwakeInteract(self, play, 0x10B6, FUN_0012cf7c);
}

}
