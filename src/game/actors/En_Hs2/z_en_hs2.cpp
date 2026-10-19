extern "C" {
#include "z_en_hs2.hpp"

#define FLAGS 0x00000009

void FUN_00353c9c(Actor* actor, PlayState* play, SkelAnime* skelAnime, s32 cmbIndex, s32 csabIndex,
                  void* jointTable, void* morphTable, s32 limbCount);
void FUN_003ae5f8(EnHs2* self, PlayState* play); // EnHs2_Idle (N64)
void FUN_00112564(EnHs2* self, PlayState* play); // EnHs2_Talking (N64)
void FUN_0036bcc8(PlayState* play, Actor* actor, Vec3s* headRot, Vec3s* torsoRot, Vec3f focusPos, s32 arg5);
void FUN_003625f8(MTX34* mtx, Vec3f* axis, f32 angle);
void FUN_0036c174(MTX34* dst, MTX34* src, MTX34* mtx);
s32 FUN_0036bb28(Actor* actor, PlayState* play, f32 arg2);
u32 FUN_00369a48(Actor* actor, PlayState* play); // Actor_TextboxIsClosing (N64)
void Animation_PlayLoop(SkelAnime* skelAnime, s32 idx);
u16 Text_GetFaceReaction(PlayState* play, u32 reactionSet);
u32 Actor_ProcessTalkRequest(Actor* actor, PlayState* play);
void SkelAnime_SetCurrentFrame2(SkelAnime* skelAnime, f32 frame);
void SkelAnime_Free2(SkelAnime* skelAnime);
void GraphicsContext_UnknownStub(GraphicsContext* gfxCtx);
void VEC3TransformAsm(Vector3f*, MTX34*, Vector3f*);
s32 EnHs2_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void EnHs2_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);

void EnHs2_Init(Actor* thisx, PlayState* play);
void EnHs2_Destroy(Actor* thisx, PlayState* play);
void EnHs2_Update(Actor* thisx, PlayState* play);
void EnHs2_Draw(Actor* thisx, PlayState* play);

// 0x005273DC: guard byte of PostLimbDraw's function-local static (PostLimbDraw is asm).

ActorInit En_Hs2_InitVars = {
    ACTOR_EN_HS2,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_HS,
    sizeof(EnHs2),
    (ActorFunc)EnHs2_Init,
    (ActorFunc)EnHs2_Destroy,
    (ActorFunc)EnHs2_Update,
    (ActorFunc)EnHs2_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    { 0x0A, 0x00, 0x11, 0x39, 0x10, 0x01 },
    { 0, { 0x00000000, 0, 0 }, { 0xFFCFFFFF, 0, 0 }, 0, 1, 1 },
    { 40.0f, 40.0f, 0.0f, { 0.0f, 0.0f, 0.0f } },
};

// FUN_00370734 returns SkelAnime_Update's result; include declares it void.
static inline s32 EnHs2_SkelAnimeUpdate(SkelAnime* skelAnime) {
    return FUN_00370734(skelAnime);
}

void EnHs2_Init(Actor* thisx, PlayState* play) {
    EnHs2* self = (EnHs2*)thisx;

    ActorShape_Init(&thisx->shape, 0.0f, ActorShadow_DrawCircle, 36.0f);
    FUN_00372f38(thisx, play, &self->skelAnimModel, 0, 0);
    FUN_00353c9c(thisx, play, &self->skelAnime, 0, 2, self->jointTable, self->morphTable, 11);
    Animation_PlayLoop(&self->skelAnime, 2);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, thisx, &sCylinderInit);
    thisx->colChkInfo.mass = MASS_IMMOVABLE;
    Actor_SetScale(thisx, 0.01f);
    self->actionFunc = FUN_003ae5f8;
    self->trackingFlags = 0;
    thisx->targetMode = 6;
}

void EnHs2_Destroy(Actor* thisx, PlayState* play) {
    EnHs2* self = (EnHs2*)thisx;

    Collider_DestroyCylinder(play, &self->collider);
    FUN_003508b8(thisx, self->skelAnimModel, 0);
    SkelAnime_Free2(&self->skelAnime);
}

void EnHs2_Update(Actor* thisx, PlayState* play) {
    EnHs2* self = (EnHs2*)thisx;

    Collider_UpdateCylinder(thisx, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    Actor_MoveForward(thisx);
    Actor_UpdateBgCheckInfo(play, thisx, 0.0f, 0.0f, 0.0f, 4);
    if (EnHs2_SkelAnimeUpdate(&self->skelAnime)) {
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

void EnHs2_Draw(Actor* thisx, PlayState* play) {
    EnHs2* self = (EnHs2*)thisx;

    GraphicsContext_UnknownStub(play->state.gfxCtx);
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, EnHs2_OverrideLimbDraw, EnHs2_PostLimbDraw, thisx, 0);
}

s32 EnHs2_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnHs2* self = (EnHs2*)thisx;

    if (limbIndex == 10) {
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
    return 0;
}

// Skipped (PostLimbDraw): `static EnHs2Vec s(300,1000,0)` with a ctor makes armcc emit the exact code (32/34 words; only the 2
// pool words differ) with a __cxa_guard, but guard (.data) + object (.bss, 12 B) land in this TU's own sections:
// the guard duplicates the word already in z_En_Hs2.data.s (0x5273DC) and the .bss has no scatter slot (L6224E, object
// is at 0x59AFFC outside the actor). Needs a linker-script/data-symbol change -> left as asm.
// Function-local static: guard 0x005273DC (TU .data, cut from the data chunk), object 0x0059AFFC pinned in
// data/oot3d.ld (scripts/pin_static.py).
void EnHs2_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    static Vector3f sMultVec(300.0f, 1000.0f, 0.0f);
    EnHs2* self = (EnHs2*)thisx;
    Vector3f* dst;

    if (limbIndex == 10) {
        dst = (Vector3f*)&self->actor.focus.pos;
        VEC3TransformAsm(dst, mtx, &sMultVec);
    }
}

void FUN_00112564(EnHs2* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        self->actionFunc = FUN_003ae5f8;
    }
    self->trackingFlags |= 1;
}

void FUN_003ae5f8(EnHs2* self, PlayState* play) {
    u16 textId = Text_GetFaceReaction(play, 9);

    if (textId == 0) {
        textId = 0x5069;
    }
    EnHs2ActionFunc talking = (EnHs2ActionFunc)FUN_00112564;
    Actor* actor = &self->actor;
    if (Actor_ProcessTalkRequest(actor, play)) {
        self->actionFunc = talking;
        return;
    }
    actor->textId = textId;
    s16 d = actor->yawTowardsPlayer - actor->shape.rot.y;
    if (ABS(d) < 0x2151 && actor->xzDistToPlayer < 100.0f) {
        self->trackingFlags |= 1;
        FUN_0036bb28(actor, play, 100.0f);
    }
}

}
