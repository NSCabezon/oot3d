extern "C" {
#include "z_en_ge2.hpp"

#define FLAGS 0x00000019

void EnGe2_Init(Actor* actor, GameState* state);
void EnGe2_Destroy(Actor* thisx, PlayState* play);
void EnGe2_Update(Actor* actor, GameState* state);
void EnGe2_Draw(Actor* thisx, PlayState* play);

// 3DS helpers (prototypes local to this actor)
void FUN_0033c764(PlayState* play);
void FUN_003725e0(PlayState* play);
void FUN_0034e01c(MTX34* mtx, Vec3s* rot);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
s32 BgCheck_LineTestImpl(CollisionContext* colCtx, s32 type, s32 unk2, Vec3f* posA, Vec3f* posB, Vec3f* posResult,
                         f32 unk6, CollisionPoly** poly, s32* bgId, Actor* actor, s32 flags);
void FUN_0010db7c(EnGe2* self, PlayState* play);
void FUN_0010da8c(EnGe2* self, PlayState* play);
void EnGe2_ChangeAction(EnGe2* self, s32 i);
void EnGe2_ForceTalk(EnGe2* self, PlayState* play);
s32 EnGe2_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void EnGe2_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void EnGe2_WaitLookAtPlayer(EnGe2* self, PlayState* play);
void EnGe2_UpdateFriendly(Actor* thisx, PlayState* play);
void FUN_0036bcc8(PlayState* play, Actor* actor, Vec3s* headRot, Vec3s* neckRot, Vec3f focusPos, s16 limit);

GLOBAL_ASM("../src/game/actors/En_Ge2/data_syms.s")

GLOBAL_ASM("data/z_En_Ge2.data.s")

ActorInit En_Ge2_InitVars = {
    ACTOR_EN_GE2,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_GLA,
    sizeof(EnGe2),
    (ActorFunc)EnGe2_Init,
    (ActorFunc)EnGe2_Destroy,
    (ActorFunc)EnGe2_Update,
    (ActorFunc)EnGe2_Draw,
};

GLOBAL_ASM("asm/EnGe2_Init.s")

void EnGe2_Destroy(Actor* thisx, PlayState* play) {
    EnGe2* self = (EnGe2*)thisx;

    Collider_DestroyCylinder(play, &self->collider);
    FUN_003508b8(thisx, self->skelAnimModel, 0);
    SkelAnime_Free2(&self->skelAnime);
    self->face.Destroy();
}

GLOBAL_ASM("asm/EnGe2_Update.s")

// Draft, 4 words off (ROM loads the FaceAnimations pointer before the eyeIndex; armcc does the reverse).
#if 0
void EnGe2_Draw(Actor* thisx, PlayState* play) {
    EnGe2* self = (EnGe2*)thisx;
    FaceAnimations* face;

    GraphicsContext_UnknownStub(play->state.gfxCtx);
    face = &self->face;
    face->FUN_0035e3a4(0, self->eyeIndex);
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, EnGe2_OverrideLimbDraw, EnGe2_PostLimbDraw, thisx, 0);
    face->FUN_0035e330();
}
#endif

GLOBAL_ASM("asm/EnGe2_Draw.s")

// Draft, 11 words off (playerSpotted/self+0x900 register assignment swapped vs ROM: r5 = playerSpotted there).
#if 0
s32 FUN_0036cd8c(PlayState* play);

static inline s32 EnGe2_DetectPlayerInAction(PlayState* play, EnGe2* self) {
    f32 visionScale = (!IS_DAY ? 0.75f : 1.5f);

    if (self->actor.xzDistToPlayer > (250.0f * visionScale)) {
        return 0;
    }

    if (50.0f > self->actor.xzDistToPlayer) {
        return 2;
    }

    if (FUN_0036cd8c(play)) {
        return 1;
    }
    return 0;
}

void EnGe2_AboutTurn(EnGe2* self, PlayState* play) {
    u8 playerSpotted;

    self->actor.speedXZ = 0.0f;
    playerSpotted = EnGe2_DetectPlayerInAction(play, self);

    if (playerSpotted != 0) {
        EnGe2_ChangeAction(self, 2);
        self->timer = 150;
        self->playerSpottedParam = playerSpotted;
        self->yawTowardsPlayer = self->actor.yawTowardsPlayer;
    } else if (self->stateFlags & 0x2) {
        Math_SmoothStepToS(&self->actor.world.rot.y, self->walkDirection, 2, 0x400, 0x200);
        self->actor.shape.rot.y = self->actor.world.rot.y;
    }

    if (self->actor.shape.rot.y == self->walkDirection) {
        EnGe2_ChangeAction(self, 0);
    }
}
#endif

GLOBAL_ASM("asm/EnGe2_AboutTurn.s")

void EnGe2_CaptureCharge(EnGe2* self, PlayState* play) {
    Math_SmoothStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 2, 0x400, 0x100);
    self->actor.shape.rot.y = self->actor.world.rot.y;

    if (self->actor.xzDistToPlayer < 50.0f) {
        EnGe2_ChangeAction(self, 6);
        self->actor.speedXZ = 0.0f;
    }

    if (self->timer != 0) {
        self->timer--;
    } else {
        FUN_0033c764(play);

        if ((INV_CONTENT(ITEM_HOOKSHOT) == ITEM_NONE) || (INV_CONTENT(ITEM_LONGSHOT) == ITEM_NONE)) {
            FUN_003716f0(play, 0x1A5, 0x14, 0x26);
        } else if (gSaveContext.eventChkInf[12] & 0x80) {
            FUN_003716f0(play, 0x5F8, 0x14, 0x26);
        } else {
            FUN_003716f0(play, 0x3B4, 0x14, 0x26);
        }
    }
}

void EnGe2_CaptureClose(EnGe2* self, PlayState* play) {
    if (self->timer > 0) {
        self->timer--;
    } else {
        FUN_0033c764(play);

        if ((INV_CONTENT(ITEM_HOOKSHOT) == ITEM_NONE) || (INV_CONTENT(ITEM_LONGSHOT) == ITEM_NONE)) {
            FUN_003716f0(play, 0x1A5, 0x14, 0x26);
        } else if (gSaveContext.eventChkInf[12] & 0x80) {
            FUN_003716f0(play, 0x5F8, 0x14, 0x26);
        } else {
            FUN_003716f0(play, 0x3B4, 0x14, 0x26);
        }
    }
}

void EnGe2_CaptureTurn(EnGe2* self, PlayState* play) {
    Math_SmoothStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 2, 0x400, 0x100);
    self->actor.shape.rot.y = self->actor.world.rot.y;

    if (self->actor.world.rot.y == self->actor.yawTowardsPlayer) {
        EnGe2_ChangeAction(self, 5);
        self->actor.speedXZ = 4.0f;
        self->timer = 75;
    }
}

// Draft (2 words off: ROM computes the mode-table pointer after the GetLastFrame call and reads the mode byte
// through it; armcc folds the index). Tables: EQU sActionFuncs 0x524C94, sAnimTable 0x524CB8 (9 s32 + 9 u8).
#if 0
void EnGe2_ChangeAction(EnGe2* self, s32 i) {
    self->actionFunc = EnGe2_sActionFuncs[i];
    Animation_Change_00375c08(&self->skelAnime, EnGe2_sAnimTable.anims[i], 1.0f, 0.0f,
                              Animation_GetLastFrame(&self->skelAnime, EnGe2_sAnimTable.anims[i]),
                              (((u8*)EnGe2_sAnimTable.anims) + 0x24)[i], -8.0f);
    self->stateFlags &= ~(1 << 1);
}
#endif

GLOBAL_ASM("asm/EnGe2_ChangeAction.s")

static void EnGe2_TurnToFacePlayer(EnGe2* self, PlayState* play) {
    s16 angleDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

    if (ABS(angleDiff) <= 0x4000) {
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 6, 0xFA0, 0x64);
        self->actor.world.rot.y = self->actor.shape.rot.y;
        FUN_0036bcc8(play, &self->actor, &self->headRot, &self->neckRot, self->actor.focus.pos, 0x4300);
    } else {
        if (angleDiff < 0) {
            Math_SmoothStepToS(&self->headRot.y, -0x2000, 6, 0x1838, 0x100);
        } else {
            Math_SmoothStepToS(&self->headRot.y, 0x2000, 6, 0x1838, 0x100);
        }

        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 12, 0x3E8, 0x64);
        self->actor.world.rot.y = self->actor.shape.rot.y;
    }
}

s32 FUN_00369a48(Actor* actor, PlayState* play);

// Draft, 2 words off (the shape.rot.y reload lands in r0 instead of the ROM's r1; alias/temp/cast variants tried).
#if 0
void EnGe2_ForceTalk(EnGe2* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        switch (self->actor.params & 0xFF) {
            case 0:
                EnGe2_ChangeAction(self, 1);
                break;
            case 1:
                EnGe2_ChangeAction(self, 7);
                break;
            case 2:
                self->actionFunc = EnGe2_WaitLookAtPlayer;
                break;
        }
        self->actor.update = (ActorFunc)EnGe2_UpdateFriendly;
        self->actor.flags &= ~0x10000;
    }
    EnGe2_TurnToFacePlayer(self, play);
}
#endif

GLOBAL_ASM("asm/EnGe2_ForceTalk.s")

s32 FUN_00359220(Actor* actor, PlayState* play, f32 arg2, f32 arg3, s32 item);
static void EnGe2_LookAtPlayer(EnGe2* self, PlayState* play);

void EnGe2_ForceTalk_003ee394(EnGe2* self, PlayState* play) {
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        self->actionFunc = FUN_0010da8c;
    } else {
        self->actor.textId = 0x6004;
        self->actor.flags |= 0x10000;
        FUN_00359220(&self->actor, play, 300.0f, 300.0f, 0);
    }
    EnGe2_LookAtPlayer(self, play);
}

void EffectSsKiraKira_SpawnDispersed(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel, Color_RGBA8* primColor,
                                     Color_RGBA8* envColor, s32 life, s32 scale);

// Draft, not matched yet: function-local statics (two guarded Vec3f, two Color_RGBA8) need the guard/data pins
// (ld + data chunk split) applied on main, orchestrator-only (see notes/En_Ge2.md).
#if 0
void EnGe2_KnockedOut(EnGe2* self, PlayState* play) {
    static Vec3f effectVelocity = { 0.0f, -0.05f, 0.0f };
    static Vec3f effectAccel = { 0.0f, -0.025f, 0.0f };
    static Color_RGBA8 effectPrimColor = { 255, 255, 255, 0 };
    static Color_RGBA8 effectEnvColor = { 255, 150, 0, 0 };
    s16 effectAngle;
    Vec3f effectPos;

    self->actor.flags &= ~0x1;
    if (self->stateFlags & 0x2) {
        effectAngle = (play->state.frames & 0x3F) * 0x2800;
        effectPos.x = self->actor.focus.pos.x + (Math_CosS(effectAngle) * 5.0f);
        effectPos.y = self->actor.focus.pos.y + 10.0f;
        effectPos.z = self->actor.focus.pos.z + (Math_SinS(effectAngle) * 5.0f);
        EffectSsKiraKira_SpawnDispersed(play, &effectPos, &effectVelocity, &effectAccel, &effectPrimColor,
                                        &effectEnvColor, 1000, 16);
    }
}
#endif

GLOBAL_ASM("asm/EnGe2_KnockedOut.s")

s32 EnGe2_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnGe2* self = (EnGe2*)thisx;

    if (limbIndex == 9) {
        FUN_0034e01c(mtx, &self->headRot);
    }
    return 0;
}

// Draft: all code matches except the function-local guard (ROM reads it as [0x524C48 + 0x10], armcc as a
// separate literal) which makes the function 4 B short. Needs the guard word at 0x524C58 pinned with the
// function in its own TU (z_en_ge2_postlimb.cpp) AND a matching base; not done (see STATUS.md).
#if 0
void EnGe2_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    static Vector3f D_80A343B0(600.0f, 700.0f, 0.0f);
    EnGe2* self = (EnGe2*)thisx;

    if (limbIndex == 9) {
        VEC3TransformAsm((Vector3f*)&self->actor.focus.pos, mtx, (Vector3f*)&D_80A343B0);
    }
}
#endif

GLOBAL_ASM("asm/EnGe2_PostLimbDraw.s")

void EnGe2_Stand(EnGe2* self, PlayState* play) {
    Math_SmoothStepToS(&self->actor.world.rot.y, self->walkDirection, 2, 0x400, 0x200);
}

s32 FUN_0036cd8c(PlayState* play);

static s32 EnGe2_DetectPlayerInAction(PlayState* play, EnGe2* self) {
    f32 visionScale = (!IS_DAY ? 0.75f : 1.5f);

    if (self->actor.xzDistToPlayer > (250.0f * visionScale)) {
        return 0;
    }

    if (50.0f > self->actor.xzDistToPlayer) {
        return 2;
    }

    if (FUN_0036cd8c(play)) {
        return 1;
    }
    return 0;
}

void EnGe2_TurnPlayerSpotted(EnGe2* self, PlayState* play) {
    s32 playerSpotted;

    self->actor.speedXZ = 0.0f;

    if (self->stateFlags & 0x10) {
        self->stateFlags &= ~0x10;
    } else {
        playerSpotted = EnGe2_DetectPlayerInAction(play, self);

        if (playerSpotted != 0) {
            self->timer = 150;
            self->yawTowardsPlayer = self->actor.yawTowardsPlayer;

            if (self->playerSpottedParam < playerSpotted) {
                self->playerSpottedParam = playerSpotted;
            }
        } else if (self->actor.world.rot.y == self->yawTowardsPlayer) {
            self->playerSpottedParam = 0;
            EnGe2_ChangeAction(self, 1);
            return;
        }
    }

    switch (self->playerSpottedParam) {
        case 1:
            Math_SmoothStepToS(&self->actor.world.rot.y, self->yawTowardsPlayer, 2, 0x200, 0x100);
            break;
        case 2:
            Math_SmoothStepToS(&self->actor.world.rot.y, self->yawTowardsPlayer, 2, 0x600, 0x180);
            break;
    }

    self->actor.shape.rot.y = self->actor.world.rot.y;
}

static void EnGe2_MaintainColliderAndSetAnimState(EnGe2* self, PlayState* play) {
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    f32 height = 40.0f;

    Actor_UpdateBgCheckInfo(play, &self->actor, height, 25.0f, height, 5);

    if (!(self->stateFlags & 0x2)) {
        if (FUN_00370734(&self->skelAnime)) {
            EnGe2* s2 = self;

            s2->stateFlags |= 0x2;
        }
    }
}

static void EnGe2_MoveAndBlink(EnGe2* self, PlayState* play) {
    Actor_MoveForward(&self->actor);

    if (DECR(self->blinkTimer) == 0) {
        self->blinkTimer = Rand_S16Offset_003702c8(60, 60);
    }
    self->eyeIndex = self->blinkTimer;

    if (self->eyeIndex >= 3) {
        self->eyeIndex = 0;
    }
}

// Draft, 4 bytes short: the `if (FUN_00370734(..)) stateFlags |= 2` store is if-converted (ldrhne/strhne) where
// ROM branches over it (`beq`). Several store forms tried (alias, local, |= vs =); all armcc-predicated.
#if 0
void EnGe2_UpdateAfterTalk(Actor* thisx, PlayState* play) {
    EnGe2* self = (EnGe2*)thisx;

    self->stateFlags |= 0x10;
    EnGe2_MaintainColliderAndSetAnimState(self, play);
    self->actionFunc(self, play);
    EnGe2_MoveAndBlink(self, play);
}
#endif

GLOBAL_ASM("asm/EnGe2_UpdateAfterTalk.s")

void EnGe2_UpdateAfterTalk(Actor* thisx, PlayState* play);

// Draft, 79 words off: the shared MaintainColliderAndSetAnimState flags store is if-converted (see UpdateAfterTalk).
#if 0
void EnGe2_UpdateFriendly(Actor* thisx, PlayState* play) {
    EnGe2* self = (EnGe2*)thisx;

    EnGe2_MaintainColliderAndSetAnimState(self, play);
    self->actionFunc(self, play);

    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        if ((self->actor.params & 0xFF) == 0) {
            self->actor.speedXZ = 0.0f;
            EnGe2_ChangeAction(self, 8);
        }
        self->actionFunc = EnGe2_ForceTalk;
        self->actor.update = (ActorFunc)EnGe2_UpdateAfterTalk;
    } else {
        self->actor.textId = 0x6005;

        if (self->actor.xzDistToPlayer < 100.0f) {
            Math_SmoothStepToS(&self->headRot.x, 0, 6, 0x1838, 100);
            Math_SmoothStepToS(&self->headRot.y, 0, 6, 0x1838, 100);
            FUN_0036bb28(&self->actor, play, 100.0f);
        }
    }
    EnGe2_MoveAndBlink(self, play);
}
#endif

GLOBAL_ASM("asm/EnGe2_UpdateFriendly.s")

// Draft, 15 words off (hit-position Vec3s->Vec3f conversion scheduled per component; ROM converts after all loads).
#if 0
void FUN_003741e4(PlayState* play, u32 dmgFlags, s32 arg2, Vec3f* pos, s32 arg4);

static s32 EnGe2_CheckAllCarpentersRescued(void) {
    return (gSaveContext.eventChkInf[9] & 0xF) == 0xF;
}

void EnGe2_UpdateStunned(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnGe2* self = (EnGe2*)thisx;

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    Actor_UpdateBgCheckInfo(play, &self->actor, 40.0f, 25.0f, 40.0f, 5);

    if (self->collider.base.acFlags & AC_HIT) {
        Vec3s* hitPos = (Vec3s*)((u8*)self + 0x1CA);
        ColliderInfo* acHit = self->collider.info.acHitInfo;
        Vec3f pos;

        pos.x = hitPos->x;
        pos.y = hitPos->y;
        pos.z = hitPos->z;

        if ((self->collider.info.acHitInfo != NULL) && (*(u32*)self->collider.info.acHitInfo & 0x80)) {
            FUN_003741e4(play, *(u32*)acHit, 1, &pos, 0);
        } else {
            *(u16*)((u8*)self + 0x11A) = 0;
            EnGe2_ChangeAction(self, 3);
            self->actor.speedXZ = 0.0f;
            self->timer = 150;
            self->stateFlags |= 4;
            Audio_PlayActorSound2(&self->actor, 0x0100055D);
            {
                u32 dmg = 0;

                if (acHit != NULL) {
                    dmg = *(u32*)acHit;
                }
                FUN_003741e4(play, dmg, 0, &pos, 0);
            }
        }
    }
    CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);

    if (EnGe2_CheckAllCarpentersRescued()) {
        self->actor.update = (ActorFunc)EnGe2_UpdateFriendly;
        self->actor.targetMode = 6;
        *(u16*)((u8*)self + 0x11A) = 0;
    } else if (*(u16*)((u8*)self + 0x11A) == 0) {
        self->actor.update = (ActorFunc)EnGe2_Update;
    }
}
#endif

GLOBAL_ASM("asm/EnGe2_UpdateStunned.s")

void FUN_0036bcc8(PlayState* play, Actor* actor, Vec3s* headRot, Vec3s* neckRot, Vec3f focusPos, s16 limit);

static void EnGe2_LookAtPlayer(EnGe2* self, PlayState* play) {
    s16 yawDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

    if ((ABS(yawDiff) <= 0x4300) && (self->actor.xzDistToPlayer < 200.0f)) {
        FUN_0036bcc8(play, &self->actor, &self->headRot, &self->neckRot, self->actor.focus.pos, 0x4300);
    } else {
        Math_SmoothStepToS(&self->headRot.x, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->headRot.y, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->neckRot.x, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->neckRot.y, 0, 6, 6200, 100);
    }
}

void EnGe2_WaitLookAtPlayer(EnGe2* self, PlayState* play) {
    EnGe2_LookAtPlayer(self, play);
}

// Draft, 5 words off (2 int-compare/pool register choices; ROM reuses r1 for the 50.0f literal, armcc takes r3).
#if 0
s32 FUN_0036cd8c(PlayState* play);

static inline s32 EnGe2_DetectPlayerInAction(PlayState* play, EnGe2* self) {
    f32 visionScale = (!IS_DAY ? 0.75f : 1.5f);

    if (self->actor.xzDistToPlayer > (250.0f * visionScale)) {
        return 0;
    }

    if (50.0f > self->actor.xzDistToPlayer) {
        return 2;
    }

    if (FUN_0036cd8c(play)) {
        return 1;
    }
    return 0;
}

void EnGe2_Walk(EnGe2* self, PlayState* play) {
    u8 playerSpotted;

    playerSpotted = EnGe2_DetectPlayerInAction(play, self);
    if (playerSpotted != 0) {
        self->actor.speedXZ = 0.0f;
        EnGe2_ChangeAction(self, 2);
        self->timer = 150;
        self->playerSpottedParam = playerSpotted;
        self->yawTowardsPlayer = self->actor.yawTowardsPlayer;
    } else if (self->walkTimer >= self->walkDuration) {
        self->walkTimer = 0;
        self->walkDirection += 0x8000;
        EnGe2_ChangeAction(self, 1);
        self->actor.speedXZ = 0.0f;
    } else {
        self->walkTimer++;
        self->actor.speedXZ = 2.0f;
    }
}
#endif

GLOBAL_ASM("asm/EnGe2_Walk.s")

s32 FUN_00298684(CollisionContext* colCtx, Vec3f* posA, Vec3f* posB, Vec3f* posResult, CollisionPoly** poly,
                 s32 chkOneFace) {
    s32 bgId;
    s32 flags = 7;

    if (chkOneFace) {
        flags = 0xF;
    }
    return BgCheck_LineTestImpl(colCtx, 0, 0, posA, posB, posResult, 1.0f, poly, &bgId, NULL, flags | 0x10);
}

void FUN_0010db7c(EnGe2* self, PlayState* play) {
    if (Actor_HasParent(&self->actor, play)) {
        self->actor.parent = NULL;
        self->actionFunc = EnGe2_ForceTalk;
    } else {
        GiveItem(&self->actor, play, 0x3A, 10000.0f, 50.0f);
    }
}

void FUN_0010da8c(EnGe2* self, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_EVENT) && Message_ShouldAdvance(play)) {
        FUN_003725e0(play);
        self->actor.flags &= ~0x10000;
        self->actionFunc = FUN_0010db7c;
        GiveItem(&self->actor, play, 0x3A, 10000.0f, 50.0f);
    }
}

}
