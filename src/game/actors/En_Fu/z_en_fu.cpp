extern "C" {
#include "z_en_fu.hpp"

#define FLAGS 0x02000019

#define FU_RESET_LOOK_ANGLE (1 << 0)
#define FU_WAIT (1 << 1)

#define EN_FU_OCARINA_MODE(play) (*(u16*)((u8*)(play) + 0x2B7E))

static inline u8* EnFu_GetObjBank(PlayState* play, u32 idx) {
    if (idx < 0x13 && *(s32*)((u8*)play + (idx << 7) + 0x3A64) != 0) {
        return (u8*)play + (idx << 7) + 0x3A5C;
    }
    return NULL;
}

void EnFu_Init(Actor* thisx, PlayState* play);
void EnFu_Destroy(Actor* thisx, PlayState* play);
void EnFu_Update(Actor* thisx, PlayState* play);
void EnFu_Draw(EnFu* self, PlayState* play);

void EnFu_WaitChild(EnFu* self, PlayState* play);
void EnFu_WaitAdult(EnFu* self, PlayState* play);
void EnFu_TeachSong(EnFu* self, PlayState* play);
void EnFu_WaitForPlayback(EnFu* self, PlayState* play);
void FUN_001124c4(EnFu* self, PlayState* play);
void FUN_001172d4(EnFu* self, PlayState* play);
void FUN_00104720(EnFu* self, PlayState* play);
void FUN_00145a4c(EnFu* self, PlayState* play);
void* ZAR_GetCMBByIndex(ZARInfo* zar, s32 index);
void Actor_MoveForward(Actor* actor);
s32 FUN_003731e0(SkelAnime* skelAnime);
s32 FUN_0036bcc8(PlayState* play, Actor* actor, Vec3s* lookAngle, Vec3s* unk, Vec3f pos, s32 arg);
void Animation_PlayLoop(SkelAnime* skelAnime, s32 idx);
void FUN_00353e78(ZARInfo* zar, PlayState* play, SkelAnime* skel, void* cmb, void* arg4, s32 arg5, Vec3s* jointTable,
                  Vec3s* morphTable, s32 limbCount);

s32 FUN_00369a48(Actor* actor, PlayState* play);
void FUN_0034e01c(MTX34* mtx, Vec3s* rot);
void FUN_003523dc(s32 instrument);
void FUN_0037073c(PlayState* play, s32 action);
void FUN_00372244(void* arg0, u16 arg1, u32 sfxId);
void FUN_00367c7c(PlayState* play, u16 textId, Actor* actor);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
s32 Item_Give(PlayState* play, s32 item);

GLOBAL_ASM("data/z_En_Fu.data.s")

ActorInit En_Fu_InitVars = {
    ACTOR_EN_FU,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_FU,
    sizeof(EnFu),
    (ActorFunc)EnFu_Init,
    (ActorFunc)EnFu_Destroy,
    (ActorFunc)EnFu_Update,
    (ActorFunc)EnFu_Draw,
};

static const ColliderCylinderInit* const sEnFuCylinderInit = (const ColliderCylinderInit*)0x00523A24; // debt: raw address into the actor data block

void EnFu_Init(Actor* thisx, PlayState* play) {
    EnFu* self = (EnFu*)thisx;
    u8* bank;
    ZARInfo* zar;
    void* cmb;

    ActorShape_Init(&thisx->shape, 0.0f, ActorShadow_DrawCircle, 36.0f);
    bank = EnFu_GetObjBank(play, (u8)thisx->objBankIndex);
    zar = (ZARInfo*)(bank + 0x10);
    cmb = ZAR_GetCMBByIndex(zar, 0);
    thisx->unk_19A = 1;
    FUN_00372f38(thisx, play, &self->sub.skelAnimModel, 0, 0);
    FUN_00353e78(zar, play, &self->skelAnime, cmb, thisx->unk_178, 1, self->jointTable, self->morphTable, 14);
    Animation_PlayLoop(&self->skelAnime, 1);
    self->skelAnime.playSpeed = 1.5f;
    SkelAnime_SetFaceAnimations(&self->sub.faceAnim, &self->skelAnime, 0, 1, -1);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, thisx, (ColliderCylinderInit*)sEnFuCylinderInit);
    thisx->colChkInfo.mass = MASS_IMMOVABLE;
    Actor_SetScale(thisx, 0.01f);
    s32 age = gSaveContext.linkAge;
    if (age != 0) {
        self->sub.actionFunc = EnFu_WaitChild;
        self->sub.facialExpression = 0;
    } else {
        self->sub.actionFunc = EnFu_WaitAdult;
        self->sub.facialExpression = 1;
        self->skelAnime.playSpeed = 3.0f;
    }
    self->sub.behaviorFlags = 0;
    thisx->targetMode = 6;
}

void EnFu_Destroy(Actor* thisx, PlayState* play) {
    EnFu* self = (EnFu*)thisx;
    {
        PlayState* play2 = play;
        Collider_DestroyCylinder(play2, &self->collider);
    }
    FUN_003508b8(&self->actor, self->sub.skelAnimModel, 0);
    SkelAnime_Free2(&self->skelAnime);
    self->sub.faceAnim.Destroy();
}

void FUN_00145a4c(EnFu* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play) != 0) {
        self->sub.actionFunc = EnFu_WaitAdult;
    }
}

static inline s32 EnFu_TalkCheck(EnFu* self, PlayState* play, u16 textId, EnFuActionFunc actionFunc) {
    s16 yawDiff;
    Actor* thisx = &self->actor;

    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        self->sub.actionFunc = actionFunc;
        return 1;
    }
    self->actor.textId = textId;
    yawDiff = thisx->yawTowardsPlayer - thisx->shape.rot.y;

    if ((ABS(yawDiff) < 0x2301) && (self->actor.xzDistToPlayer < 100.0f)) {
        FUN_0036bb28(&self->actor, play, 100.0f);
    } else {
        self->sub.behaviorFlags |= FU_RESET_LOOK_ANGLE;
    }
    return 0;
}

void FUN_001124c4(EnFu* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        self->sub.behaviorFlags &= ~FU_WAIT;
        self->sub.actionFunc = EnFu_WaitChild;

        if (self->skelAnime.animationType == 0) {
            Animation_Change_00375c08(&self->skelAnime, 1, 1.5f, 0.0f,
                                      Animation_GetLastFrame(&self->skelAnime, 1), 0, -4.0f);
        }
    }
}

void EnFu_WaitChild(EnFu* self, PlayState* play) {
    u16 textId = Text_GetFaceReaction(play, 0xB);

    if (textId == 0) {
        textId = (gSaveContext.eventChkInf[6] & 0x80) ? 0x5033 : 0x5032;
    }

    if (EnFu_TalkCheck(self, play, textId, FUN_001124c4)) {
        if (textId == 0x5033) {
            Animation_Change_00375c08(&self->skelAnime, 0, 1.5f, 0.0f,
                                      Animation_GetLastFrame(&self->skelAnime, 0), 0, -4.0f);
        }
    }
}

void FUN_001172d4(EnFu* self, PlayState* play) {
    if (play->GetCutsceneState() == 0) {
        self->sub.actionFunc = EnFu_WaitAdult;
        gSaveContext.eventChkInf[5] |= 0x800;
        EN_FU_OCARINA_MODE(play) = 4;
    }
}

void EnFu_TeachSong(EnFu* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    player->stateFlags2 |= 0x800000;
    if (Message_GetState(&play->msgCtx) == 2) {
        self->sub.behaviorFlags &= ~FU_WAIT;
        FUN_003523dc(4);
        FUN_0037073c(play, 0xD);
        self->sub.actionFunc = EnFu_WaitForPlayback;
    }
}

void EnFu_WaitForPlayback(EnFu* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    player->stateFlags2 |= 0x800000;
    if (Message_GetState(&play->msgCtx) == 7) {
        FUN_0037073c(play, 0x1A);
        self->sub.actionFunc = FUN_00104720;
    }
}

void FUN_00104720(EnFu* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    u16 mode = EN_FU_OCARINA_MODE(play);

    if (mode >= 4) {
        self->sub.actionFunc = EnFu_WaitAdult;
        EN_FU_OCARINA_MODE(play) = 4;
        self->actor.flags &= ~0x10000;
    } else if (mode == 3) {
        FUN_00372244(&play->unk_5FCC, 0x1E, 0x1000487);
        self->sub.actionFunc = FUN_001172d4;
        self->actor.flags &= ~0x10000;
        {
            u8* bank = EnFu_GetObjBank(play, (u8)self->actor.objBankIndex);
            ZARInfo* zar = (ZARInfo*)(bank + 0x10);
            play->FUN_0037573c(zar->GetQDBByIndex(0));
        }
        *(u8*)((u8*)&gSaveContext + 0x1000 + 0x5A2) = 1;
        Item_Give(play, 0x65);
        EN_FU_OCARINA_MODE(play) = 0;
        gSaveContext.eventChkInf[6] |= 0x20;
    } else if (mode == 2) {
        player->stateFlags2 &= ~0x1000000;
        self->sub.actionFunc = EnFu_WaitAdult;
    } else if (mode == 1) {
        player->stateFlags2 |= 0x800000;
    }
}

// Function-local static yawDiff + the PostLimbDraw guard live in the TU .data (0x00523A1C..0x00523A24, cut out of the data chunk with
// pin_static.py split); the PostLimbDraw object is pinned in .bss at 0x00599128.
void EnFu_WaitAdult(EnFu* self, PlayState* play) {
    static s16 yawDiff;
    Player* player = GET_PLAYER(play);

    yawDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
    if (gSaveContext.eventChkInf[5] & 0x800) {
        EnFu_TalkCheck(self, play, 0x508E, FUN_00145a4c);
    } else if (player->stateFlags2 & 0x1000000) {
        self->actor.textId = 0x5035;
        FUN_00367c7c(play, 0x5035, NULL);
        self->sub.actionFunc = EnFu_TeachSong;
        self->sub.behaviorFlags |= FU_WAIT;
    } else if (Actor_ProcessTalkRequest(&self->actor, play)) {
        self->sub.actionFunc = FUN_00145a4c;
    } else if (ABS(yawDiff) < 0x2301) {
        if (self->actor.xzDistToPlayer < 100.0f) {
            self->actor.textId = 0x5034;
            FUN_0036bb28(&self->actor, play, 100.0f);
            player->stateFlags2 |= 0x800000;
        }
    }
}

s32 EnFu_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnFu* self = (EnFu*)thisx;

    if (limbIndex == 0xD || limbIndex == 7) {
        return 0;
    }
    if (limbIndex == 0xC) {
        FUN_0034e01c(mtx, &self->sub.lookAngleOffset);
    }
    return 0;
}

static inline void EnFu_MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}

void EnFu_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnFu* self = (EnFu*)thisx;
    static Vector3f sMtxSrc(700.0f, 700.0f, 0.0f);

    if (limbIndex == 0xC) {
        EnFu_MultVec3f(&sMtxSrc, (Vector3f*)&self->actor.focus.pos, mtx);
    }
}

void EnFu_Draw(EnFu* self, PlayState* play) {
    {
        FaceAnimations* fa = &self->sub.faceAnim;
        fa->FUN_0035e3a4(0, self->sub.facialExpression);
    }
    {
        FaceAnimations* fa = &self->sub.faceAnim;
        fa->FUN_0035e3a4(1, self->sub.facialExpression);
    }
    GraphicsContext_UnknownStub(play->state.gfxCtx);
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, EnFu_OverrideLimbDraw, EnFu_PostLimbDraw, &self->actor, 0);
    self->sub.faceAnim.FUN_0035e330();
}

// near miss (4 insns): ROM does `mov r1,#1; strb r1,[r0,#0x27d]; add r1,r0,#0x1a4` straight off the incoming r0 (no r4 copy);
// ours emits `mov r0,#1; strb r0,[r4,..]; add r1,r4,..; mov r0,r4`. ~35 variants (helper, thisx/self, aliases) did not move it.
// recipe_near4 2026-10-15: `(EnFu* self, PlayState*)` signature + `self->skelAnime.unk_80[1] = 1` + dead `EnFu* s2 = self` used only
// in `Collider_UpdateCylinder(&s2->actor, ..)` fixes the prologue, size-exact, but then play/literal share r6 (push r4-r7, 18 words).
#if 0
void EnFu_Update(Actor* thisx, PlayState* play) {
    EnFu* self = (EnFu*)thisx;
    Vec3s* unk;

    *(u8*)((u8*)thisx + 0x27D) = 1;
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(play, &self->actor, 0.0f, 0.0f, 0.0f, 4);
    if (!(self->sub.behaviorFlags & FU_WAIT) && FUN_003731e0(&self->skelAnime)) {
        Animation_Change_00375c08(&self->skelAnime, self->skelAnime.animationType, 1.5f, self->skelAnime.curFrame,
                                  Animation_GetLastFrame(&self->skelAnime, self->skelAnime.animationType), 2, 0.0f);
    }
    {
        PlayState* play2 = play;
        self->sub.actionFunc(self, play2);
    }
    unk = &self->sub.unk_36;
    if (self->sub.behaviorFlags & FU_RESET_LOOK_ANGLE) {
        Math_SmoothStepToS(&self->sub.lookAngleOffset.x, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->sub.lookAngleOffset.y, 0, 6, 6200, 100);
        Math_SmoothStepToS(&unk->x, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->sub.unk_36.y, 0, 6, 6200, 100);
        self->sub.behaviorFlags &= ~FU_RESET_LOOK_ANGLE;
    } else {
        FUN_0036bcc8(play, &self->actor, &self->sub.lookAngleOffset, unk, thisx->focus.pos, 0x4300);
    }
}
#endif
GLOBAL_ASM("asm/EnFu_Update.s")

}
