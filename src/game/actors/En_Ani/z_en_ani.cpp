extern "C" {
#include "z_en_ani.hpp"

#define FLAGS 0x00000009

void EnAni_Init(Actor* actor, GameState* state);
void EnAni_Destroy(Actor* actor, GameState* state);
void EnAni_Update(Actor* actor, GameState* state);
void EnAni_Draw(Actor* actor, GameState* state);

s32 FUN_00369a48(Actor* actor, PlayState* play); // N64: Actor_TextboxIsClosing
void GraphicsContext_UnknownStub(GraphicsContext* gfxCtx);
void FUN_0034e01c(MTX34* mtx, Vec3s* rot);       // rotates mtx by rot (limb helper)
void EnAni_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void FUN_0015d56c(EnAni* self, PlayState* play);
void FUN_001a1438(EnAni* self, PlayState* play);

GLOBAL_ASM("data/z_En_Ani.data.s")

GLOBAL_ASM("../src/game/actors/En_Ani/data_syms.s")

// sInitChain / sCylinderInit live in the data dump above (no scatter symbols) -> absolute EQU symbols.
extern InitChainEntry EnAni_sInitChain[];
extern u8 EnAni_sCylinderInit[];
void Animation_PlayOnce(SkelAnime* skelAnime, s32 idx);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void SkelAnime_SetCurrentFrame2(SkelAnime* skelAnime, f32 frame);
void FUN_0036bcc8(PlayState* play, Actor* actor, Vec3s* headRot, Vec3s* torsoRot, Vec3f focusPos, s32 yawLimit);

static inline void EnAni_SetupAction(EnAni* self, EnAniActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

ActorInit En_Ani_InitVars = {
    ACTOR_EN_ANI,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_ANI,
    sizeof(EnAni),
    (ActorFunc)EnAni_Init,
    (ActorFunc)EnAni_Destroy,
    (ActorFunc)EnAni_Update,
    (ActorFunc)EnAni_Draw,
};

void EnAni_Init(Actor* actor, GameState* state) {
    EnAni* self = (EnAni*)actor;
    PlayState* play = (PlayState*)state;

    Actor_ProcessInitChain(actor, EnAni_sInitChain);
    ActorShape_Init(&actor->shape, -2800.0f, ActorShadow_DrawCircle, 36.0f);
    FUN_00372f38(actor, play, &self->skelAnimModel, 0, 0);
    FUN_00353c9c(actor, play, &self->skelAnime, 0, 2, (void*)((u8*)self + 0x280), (void*)((u8*)self + 0x58C), 15);
    Animation_PlayOnce(&self->skelAnime, 2);
    SkelAnime_SetFaceAnimations((FaceAnimations*)self->faceAnimations, &self->skelAnime, 0, -1, -1);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, actor, (ColliderCylinderInit*)EnAni_sCylinderInit);
    actor->colChkInfo.mass = 0xFF;
    if (gSaveContext.linkAge != 0) {
        EnAni_SetupAction(self, FUN_001a1438);
    } else {
        EnAni_SetupAction(self, FUN_0015d56c);
    }
    self->unk_8A6 = 0;
    self->unk_8A4 = 0;
    actor->minVelocityY = -1.0f;
    actor->velocity.y = -1.0f;
}

#if 0 // draft: not matched: ROM ends in bare bl (tail at 0x189844 belongs to FUN_00189844 = cfg tail)
void EnAni_Destroy(Actor* actor, GameState* state) {
    EnAni* self = (EnAni*)actor;
    PlayState* play = (PlayState*)state;

    Collider_DestroyCylinder(play, &self->collider);
    FUN_003508b8(actor, self->skelAnimModel, 0);
    SkelAnime_Free2(&self->skelAnime);
}
#endif

GLOBAL_ASM("asm/EnAni_Destroy.s")

// N64 func_809B0A28 (3DS: plays the sfx when the post-anim timer runs out).
static inline void EnAni_WaitAnim(EnAni* self, PlayState* play) {
    if (FUN_00370734(&self->skelAnime)) {
        self->unk_8A6++;
    }
    if (self->unk_8B0 != 0) {
        self->unk_8B0--;
        if (self->unk_8B0 == 0) {
            Audio_PlaySoundGeneral(0x0100003F, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        }
    }
}

void EnAni_Update(Actor* actor, GameState* state) {
    EnAni* self = (EnAni*)actor;
    PlayState* play = (PlayState*)state;

    Collider_UpdateCylinder(actor, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    Actor_MoveForward(actor);
    Actor_UpdateBgCheckInfo(play, actor, 0.0f, 0.0f, 0.0f, 4);
    if ((play->GetCutsceneState() != 0) && (play->csCtx.npcActions[0] != NULL)) {
        switch (self->unk_8A6) {
            case 0:
                if (FUN_00370734(&self->skelAnime)) {
                    SkelAnime_SetCurrentFrame2(&self->skelAnime, 0.0f);
                }
                if (play->csCtx.npcActions[0]->action == 2) {
                    Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f,
                                              (f32)Animation_GetLastFrame(&self->skelAnime, 0), 2, 0.0f);
                    self->unk_8B0 = 6;
                    self->unk_8A6++;
                    actor->shape.shadowDraw = NULL;
                }
                break;
            case 1:
                EnAni_WaitAnim(self, play);
                break;
            case 2:
                if (play->csCtx.npcActions[0]->action == 4) {
                    Animation_Change_00375c08(&self->skelAnime, 1, 1.0f, 0.0f,
                                              (f32)Animation_GetLastFrame(&self->skelAnime, 1), 2, -4.0f);
                    self->unk_8B0 = 0;
                    self->unk_8A6++;
                    actor->shape.shadowDraw = ActorShadow_DrawCircle;
                }
                break;
            case 3:
                EnAni_WaitAnim(self, play);
                break;
            case 4:
                break;
        }

        if (play->csCtx.frames == 150) {
            Audio_PlaySoundGeneral(0x01000137, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        }
    } else {
        if (FUN_00370734(&self->skelAnime)) {
            SkelAnime_SetCurrentFrame2(&self->skelAnime, 0.0f);
        }
        self->actionFunc(self, play);
    }

    if (self->unk_8A4 & 1) {
        FUN_0036bcc8(play, actor, &self->limbRot, &self->unk_89E, actor->focus.pos, 0x4300);
        self->unk_89E.z = 0;
        self->unk_89E.y = self->unk_89E.z;
        self->unk_89E.x = self->unk_89E.y;
    } else {
        Math_SmoothStepToS(&self->limbRot.x, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->limbRot.y, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->unk_89E.x, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->unk_89E.y, 0, 6, 6200, 100);
    }

    if (DECR(self->blinkTimer) == 0) {
        self->blinkTimer = Rand_S16Offset_003702c8(60, 60);
    }
    self->eyeIndex = self->blinkTimer;
    if (self->eyeIndex >= 3) {
        self->eyeIndex = 0;
    }
}

s32 EnAni_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnAni* self = (EnAni*)thisx;

    if (limbIndex == 0xE) {
        FUN_0034e01c(mtx, &self->limbRot);
    }
    return 0;
}

// Function-local static: guard 0x005205A4 (TU .data, cut from the data chunk), object 0x00598EAC pinned in
// data/oot3d.ld (scripts/pin_static.py).
void EnAni_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    static Vector3f sMultVec(800.0f, 500.0f, 0.0f);
    EnAni* self = (EnAni*)thisx;
    Vector3f* dst;

    if (limbIndex == 14) {
        dst = (Vector3f*)&self->actor.focus.pos;
        VEC3TransformAsm(dst, mtx, &sMultVec);
    }
}

void EnAni_Draw(Actor* actor, GameState* state) {
    EnAni* self = (EnAni*)actor;
    PlayState* play = (PlayState*)state;

    GraphicsContext_UnknownStub(play->state.gfxCtx);
    FaceAnimations* face = (FaceAnimations*)self->faceAnimations;
    f32 dummy = 0.0f; // dead local: moves the eyeIndex literal load (arg scheduling)
    face->FUN_0035e3a4(0, self->eyeIndex);
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, EnAni_OverrideLimbDraw, EnAni_PostLimbDraw, actor, 0);
    ((FaceAnimations*)self->faceAnimations)->FUN_0035e330();
}


void FUN_0015fb60(EnAni* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        self->actionFunc = FUN_0015d56c;
    }
}

void FUN_001a3850(EnAni* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        self->actionFunc = FUN_001a1438;
    }
}

void FUN_00117324(EnAni* self, PlayState* play) {
    if (Actor_HasParent(&self->actor, play)) {
        self->actor.parent = NULL;
        if (!LINK_IS_ADULT) {
            self->actionFunc = FUN_001a3850;
        } else {
            self->actionFunc = FUN_0015fb60;
        }
        gSaveContext.itemGetInf[1] |= 0x20;
    } else {
        GiveItem(&self->actor, play, 0x3E, 10000.0f, 200.0f);
    }
}

void FUN_00202634(EnAni* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        self->actionFunc = FUN_00117324;
    }
    GiveItem(&self->actor, play, 0x3E, 10000.0f, 200.0f);
}


static inline s32 EnAni_SetText(EnAni* self, PlayState* play, u16 textId) {
    self->actor.textId = textId;
    self->unk_8A4 |= 1;
    FUN_0036bb28(&self->actor, play, 100.0f);
    return 0;
}

// Adult action (N64 func_809B07F8).
void FUN_0015d56c(EnAni* self, PlayState* play) {
    s16 yawDiff;
    u16 textId;

    yawDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        if (self->actor.textId == 0x5056) {
            EnAni_SetupAction(self, FUN_0015fb60);
        } else if (self->actor.textId == 0x5055) {
            EnAni_SetupAction(self, FUN_00202634);
        } else {
            EnAni_SetupAction(self, FUN_0015fb60);
        }
    } else if (yawDiff > -0x36B0 && yawDiff < 0 && self->actor.xzDistToPlayer < 150.0f &&
               -80.0f < self->actor.yDistToPlayer) {
        if (gSaveContext.itemGetInf[1] & 0x20) {
            EnAni_SetText(self, play, 0x5056);
        } else {
            EnAni_SetText(self, play, 0x5055);
        }
    } else if (yawDiff > -0x3E8 && yawDiff < 0x36B0 && self->actor.xzDistToPlayer < 350.0f) {
        if (!(gSaveContext.eventChkInf[2] & 0x8000)) {
            textId = 0x5052;
        } else {
            textId = (gSaveContext.itemGetInf[1] & 0x20) ? 0x5054 : 0x5053;
        }
        EnAni_SetText(self, play, textId);
    }
}

// Child action (N64 func_809B064C).
void FUN_001a1438(EnAni* self, PlayState* play) {
    u16 textId = Text_GetFaceReaction(play, 10);
    s16 yawDiff;

    if (textId == 0) {
        textId = !IS_DAY ? 0x5051 : 0x5050;
    }

    yawDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        if (self->actor.textId == 0x5056) {
            EnAni_SetupAction(self, FUN_001a3850);
        } else if (self->actor.textId == 0x5055) {
            EnAni_SetupAction(self, FUN_00202634);
        } else {
            EnAni_SetupAction(self, FUN_001a3850);
        }
    } else if (yawDiff >= -0x36AF && yawDiff < 0 && self->actor.xzDistToPlayer < 150.0f &&
               -80.0f < self->actor.yDistToPlayer) {
        if (gSaveContext.itemGetInf[1] & 0x20) {
            EnAni_SetText(self, play, 0x5056);
        } else {
            EnAni_SetText(self, play, 0x5055);
        }
    } else if (yawDiff >= -0x3E7 && yawDiff < 0x36B0 && self->actor.xzDistToPlayer < 350.0f) {
        EnAni_SetText(self, play, textId);
    }
}

}
