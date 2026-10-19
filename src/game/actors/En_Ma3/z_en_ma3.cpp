extern "C" {
#include "z_en_ma3.hpp"

#define FLAGS 0x00000039

#define ENMA3_ANIM_0 0
typedef struct {
    void* animation;
    f32 frameCount;
    u8 mode;
    u8 pad[3];
    f32 morphFrames;
} EnMa3AnimInfo;
void* ZAR_GetCMBByIndex(void* zar, s32 index);
void FUN_00353e78(void* zar, PlayState* play, SkelAnime* skelAnime, void* cmb, void* p, s32 a, s32 b, s32 c, s32 d);
f32 Animation_GetLastFrame(SkelAnime* skelAnime, void* animation);
DamageTable* DamageTable_Get(s32 index);
void Animation_Change_00375c08(SkelAnime* skelAnime, void* animation, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void FUN_003C6240(EnMa3* self, PlayState* play);
void FUN_0034e01c(PlayState* play, void* p);

void EnMa3_Init(Actor* actor, GameState* state);
void EnMa3_Destroy(Actor* actor, GameState* state);
void EnMa3_Update(Actor* actor, GameState* state);
void EnMa3_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_Ma3.data.s")

ActorInit En_Ma3_InitVars = {
    ACTOR_EN_MA3,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_MA2,
    sizeof(EnMa3),
    (ActorFunc)EnMa3_Init,
    (ActorFunc)EnMa3_Destroy,
    (ActorFunc)EnMa3_Update,
    (ActorFunc)EnMa3_Draw,
};

// Init draft (Haiku barrido): 98/127 words differ, size 472 vs 508. Not matched.
#if 0
static inline void EnMa3_ChangeAnim(EnMa3* self, s32 index) {
    EnMa3AnimInfo* info = &((EnMa3AnimInfo*)0x0052C308)[index];
    f32 frameCount = Animation_GetLastFrame((SkelAnime*)self->unk_1A4, info->animation);

    Animation_Change_00375c08((SkelAnime*)self->unk_1A4, info->animation, 1.0f, 0.0f, frameCount, info->mode,
                              info->morphFrames);
}

static inline u8* EnMa3_GetObjBank(PlayState* play, s32 idx) {
    if ((u32)idx < 0x13 && *(s32*)((u8*)play + (idx << 7) + 0x3A64) != 0) {
        return (u8*)play + (idx << 7) + 0x405C;
    }
    return NULL;
}

void EnMa3_Init(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnMa3* self = (EnMa3*)actor;
    u8* zar = EnMa3_GetObjBank(play, self->actor.objBankIndex) + 0x10;
    void* cmb = ZAR_GetCMBByIndex(zar, 0);

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 18.0f);
    FUN_00353e78(zar, play, (SkelAnime*)self->unk_1A4, cmb, self->actor.unk_178, -1, 0, 0, 0);
    SkelAnime_SetFaceAnimations((FaceAnimations*)self->unk_228, (SkelAnime*)self->unk_1A4, 0, 1, -1);
    self->unk_47A = 1;
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, actor, (ColliderCylinderInit*)0x0052C294);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, DamageTable_Get(22), (CollisionCheckInfoInit2*)0x0052C2CC);

    if (*(s32*)(0x00587958 + 4) == 1 || (*(u16*)(0x00588758 + 0xEE) & 0x100) == 0) {
        Actor_Kill(actor);
        return;
    }
    EnMa3_ChangeAnim(self, ENMA3_ANIM_0);
    self->actionFunc = FUN_003C6240;
    Actor_UpdateBgCheckInfo(play, actor, 0.0f, 0.0f, 0.0f, 4);
    Actor_SetScale(actor, 0.01f);
    self->unk_450 = 0;
}
#endif

GLOBAL_ASM("asm/EnMa3_Init.s")

void EnMa3_Destroy(Actor* actor, GameState* state) {
    EnMa3* self = (EnMa3*)actor;

    SkelAnime_Free2((SkelAnime*)self->unk_1A4);
    ((FaceAnimations*)self->unk_228)->Destroy();
    Collider_DestroyCylinder((PlayState*)state, &self->collider);
}

// Moved from functions_180000s.cpp (keeps ld-pinned name). EnMa3_UpdateTalkState.
s16 FUN_00195eb8(PlayState* play, Actor* thisx) {
    s16 talkState = 1;

    switch (Message_GetState(&play->msgCtx)) {
        case 5: // TEXT_STATE_EVENT
            if (play->nextEntranceIndex < 0) {
                if (Message_ShouldAdvance(play)) {
                    FUN_003716f0(play, 0x157, 0x14, 0x26);
                    gSaveContext.nextCutsceneIndex = 0xFFF0;
                    gSaveContext.eventInf[0] |= 0x400;
                    gSaveContext.timer1State = 0xF;
                }
            }
            break;
        case 4: // TEXT_STATE_CHOICE
            if (Message_ShouldAdvance(play)) {
                gSaveContext.infTable[11] |= 0x200;
                if (FUN_00369f3c(play) == 0) {
                    if (gSaveContext.eventChkInf[1] & 0x4000) {
                        Message_ContinueTextbox(play, 0x2091);
                    } else if (gSaveContext.unk_EDC == 0) {
                        Message_ContinueTextbox(play, 0x2092);
                    } else {
                        Message_ContinueTextbox(play, 0x2090);
                    }
                }
            }
            break;
        case 2: // TEXT_STATE_CLOSING
            switch (thisx->textId) {
                case 0x2000:
                    gSaveContext.infTable[11] |= 0x100;
                    talkState = 0;
                    break;
                case 0x208F:
                    gSaveContext.eventChkInf[1] |= 0x4000;
                    // fallthrough
                case 0x2004:
                case 0x2012:
                    {
                        s32 best = gSaveContext.unk_EDC;
                        s32 cur = gSaveContext.timer1Value;
                        gSaveContext.unk_EDC = (best > cur) ? cur : best;
                    }
                    // fallthrough
                case 0x208E:
                    gSaveContext.eventInf[0] &= ~0x400;
                    thisx->flags &= ~0x10000;
                    talkState = 0;
                    gSaveContext.timer1State = 10;
                    break;
                case 0x2002:
                    gSaveContext.infTable[11] |= 0x200;
                    // fallthrough
                case 0x2003:
                    if (!(gSaveContext.eventInf[0] & 0x400)) {
                        talkState = 0;
                    }
                    break;
                default:
                    talkState = 0;
                    break;
            }
            break;
        case 0:
        case 1:
        case 3:
        case 6:
        case 7:
        case 8:
        case 9:
            break;
    }
    return talkState;
}

GLOBAL_ASM("asm/EnMa3_Update.s")

s32 EnMa3_OverrideLimbDraw(s32 unused, s32 limbIndex, PlayState* play, EnMa3* self) {
    if (limbIndex == 8) {
        FUN_0034e01c(play, (u8*)self + 0x458);
    } else if (limbIndex == 7) {
        FUN_0034e01c(play, (u8*)self + 0x45E);
    }
    return 0;
}

// N64 Matrix_MultVec3f(src, dest) argument order; the wrapper gives the predicated call shape.
static inline void EnMa3_MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}

void EnMa3_PostDrawLimb(s32 unused, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnMa3* self = (EnMa3*)thisx;
    Vector3f vec(900.0f, 0.0f, 0.0f);

    if (limbIndex == 8) {
        EnMa3_MultVec3f(&vec, (Vector3f*)&self->actor.focus.pos, mtx);
    }
}

// FaceAnimations calls go through this helper (arg order).
static inline void EnMa3_SetFace(FaceAnimations* f, s32 a, s32 b) {
    f->FUN_0035e3a4(a, b);
}

void FUN_0036932c(SkeletonAnimationModel* model, s32 idx);
void FUN_0037266c(SkeletonAnimationModel* model, s32 idx);
void FUN_0033d364(f32 distFromCamEye, u32 bgmId);

void EnMa3_Draw(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnMa3* self = (EnMa3*)thisx;
    Camera* activeCam = play->cameraPtrs[play->activeCamera];
    f32 distFromCamEye = Math_Vec3f_DistXZ(&thisx->world.pos, &activeCam->eye);

    FUN_0033d364(distFromCamEye, 0x0100059D);
    if (((SkelAnime*)self->unk_1A4)->animationType == 2) {
        FUN_0037266c(((SkelAnime*)self->unk_1A4)->unk_28, 2);
    } else {
        FUN_0036932c(((SkelAnime*)self->unk_1A4)->unk_28, 2);
    }
    EnMa3_SetFace((FaceAnimations*)self->unk_228, 0, self->eyeIndex);
    EnMa3_SetFace((FaceAnimations*)self->unk_228, 1, self->mouthIndex);
    ((FaceAnimations*)self->unk_228)->FUN_0035e330();
    SkelAnime_DrawOpa((SkelAnime*)self->unk_1A4, &self->actor.modelMtx, (OverrideLimbDrawOpa)EnMa3_OverrideLimbDraw, (PostLimbDrawOpa)EnMa3_PostDrawLimb, &self->actor, 0);
}

}
