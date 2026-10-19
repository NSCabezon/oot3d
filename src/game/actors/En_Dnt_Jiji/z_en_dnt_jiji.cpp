extern "C" {
#include "z_en_dnt_jiji.hpp"
#include "../En_Dnt_Demo/z_en_dnt_demo.hpp"
#include "mtx34_trans.hpp"

#define FLAGS 0x00000019

#define BGCHECKFLAG_GROUND 0x01
#define DNT_LEADER_ACTION_NONE 0
#define DNT_LEADER_ACTION_ATTACK 3

s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
f32* EnDntJiji_CurFramePtr(SkelAnime* skelAnime);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
s32 Gameplay_InCsMode(PlayState* play);
void FUN_003725e0(PlayState* play);
void FUN_0036ae48(Camera* camera);
void FUN_00367c7c(PlayState* play, u16 textId, Actor* actor);
s32 OnePointCutscene_Init(PlayState* play, s32 textId, s32 priority, Actor* actor, s32 camId);

void EnDntJiji_Init(EnDntJiji* self, PlayState* play);
void EnDntJiji_Destroy(Actor* thisx, PlayState* play);
void EnDntJiji_Update(EnDntJiji* self, PlayState* play);
void EnDntJiji_Draw(Actor* thisx, PlayState* play);
s32 EnDntJiji_OverrideLimbDraw(Actor* thisx, s32 limbIndex, void* dList, Vec3s* rot, void* arg);
void EnDntJiji_PostLimbDraw(Actor* thisx, s32 limbIndex, void* dList, Vec3s* rot, void* arg);

void EnDntJiji_Wait(EnDntJiji* self, PlayState* play);
void EnDntJiji_SetupCower(EnDntJiji* self, PlayState* play);
void EnDntJiji_SetupWalk(EnDntJiji* self, PlayState* play);
void EnDntJiji_SetupReturn(EnDntJiji* self, PlayState* play);
void EnDntJiji_Return(EnDntJiji* self, PlayState* play);
void EnDntJiji_SetupWait(EnDntJiji* self, PlayState* play);
void EnDntJiji_Hide(EnDntJiji* self, PlayState* play);
void EnDntJiji_Talk(EnDntJiji* self, PlayState* play);
void EnDntJiji_GivePrize(EnDntJiji* self, PlayState* play);
void EnDntJiji_Walk(EnDntJiji* self, PlayState* play);
void EnDntJiji_Cower(EnDntJiji* self, PlayState* play);
void EnDntJiji_SetFlower(EnDntJiji* self, PlayState* play);
void EnDntJiji_SetupGivePrize(EnDntJiji* self, PlayState* play);
void EnDntJiji_Talk(EnDntJiji* self, PlayState* play);
void EnDntJiji_Burrow(EnDntJiji* self, PlayState* play);
void EnDntJiji_Up(EnDntJiji* self, PlayState* play);
void EnDntJiji_Unburrow(EnDntJiji* self, PlayState* play);
void EnDntJiji_SetupTalk(EnDntJiji* self, PlayState* play);
void EnDntJiji_SetupUnburrow(EnDntJiji* self, PlayState* play);
void EnDntJiji_SetupUp(EnDntJiji* self, PlayState* play);
void EnDntJiji_SetupBurrow(EnDntJiji* self, PlayState* play);
void EnDntJiji_SetupHide(EnDntJiji* self, PlayState* play);
void EnDntJiji_Unburrow(EnDntJiji* self, PlayState* play);

GLOBAL_ASM("data/z_En_Dnt_Jiji.data.s")
GLOBAL_ASM("../src/game/actors/En_Dnt_Jiji/data_syms.s")

extern ColliderCylinderInit EnDntJiji_sCylinderInit;

ActorInit En_Dnt_Jiji_InitVars = {
    ACTOR_EN_DNT_JIJI,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_DNS,
    sizeof(EnDntJiji),
    (ActorFunc)EnDntJiji_Init,
    (ActorFunc)EnDntJiji_Destroy,
    (ActorFunc)EnDntJiji_Update,
    (ActorFunc)EnDntJiji_Draw,
};

void EnDntJiji_Init(EnDntJiji* self, PlayState* play) {
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 0.0f);
    FUN_00372f38(&self->actor, play, &self->model, 2, 0);
    FUN_00353c9c(&self->actor, play, &self->skelAnime, 0, 8, self->jointTable, self->morphTable, 11);
    {
        char* p = &self->skelAnime.unk_75[0];
        *p = 0;
    }
    SkelAnime_SetFaceAnimations((FaceAnimations*)self->faceAnims, &self->skelAnime, 0, -1, -1);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, &EnDntJiji_sCylinderInit);
    self->stage = (EnDntDemo*)self->actor.parent;
    self->actor.flags &= ~1;
    self->actor.gravity = -2.0f;
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    self->actor.targetMode = 6;
    self->actionFunc = EnDntJiji_SetFlower;
}

void EnDntJiji_Destroy(Actor* thisx, PlayState* play) {
    EnDntJiji* self = (EnDntJiji*)thisx;

    FUN_003508b8(&self->actor, self->model, 0);
    SkelAnime_Free2(&self->skelAnime);
    ((FaceAnimations*)self->faceAnims)->Destroy();
    Collider_DestroyCylinder(play, &self->collider);
}

#define JIJI_BLINK_N (20 + (s16)Rand_ZeroFloat(60.0f))
void EnDntJiji_Update(EnDntJiji* self, PlayState* play) {
    EnDntJiji* s2 = self;

    Actor_SetScale(&self->actor, 0.015f);
    self->unkTimer++;
    if (self->timer > 1) {
        self->timer--;
    }
    if (self->sfxTimer != 0) {
        self->sfxTimer--;
    }
    if (self->blinkTimer != 0) {
        self->blinkTimer--;
    }
    switch (self->stageSignal) {
    case 0:
        break;
    case 1:
        self->isSolid = true;
        self->action = 1;
        s2->actionFunc = EnDntJiji_SetupUp;
        break;
    case 2:
        self->isSolid = false;
        self->action = 0;
        s2->actionFunc = EnDntJiji_SetupBurrow;
        break;
    case 3:
        s2->actionFunc = EnDntJiji_SetupReturn;
        break;
    }
    if (self->actor.textId != 0) {
        Actor_SetFocus(&self->actor, 30.0f);
    }
    if (self->stageSignal != 0) {
        self->stageSignal = 0;
    }
    if (self->blinkTimer == 0) {
        self->eyeState++;
        if (self->eyeState > 2) {
            self->eyeState = 0;
            s32 t;
            if (JIJI_BLINK_N > 0) {
                t = (s32)(0.5f + ((f32)JIJI_BLINK_N * 3.0f) * 0.5f);
            } else {
                t = (s32)(((f32)JIJI_BLINK_N * 3.0f) * 0.5f - 0.5f);
            }
            self->blinkTimer = t;
        }
    }
    self->actionFunc(self, play);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(play, &self->actor, 20.0f, 20.0f, 60.0f, 0x1D);
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
}

// Draw: NOT MATCHED (41/82 words off). Frame layout now matches (0xb0: scale sp+8, pos sp+0x14, scaleMtx sp+0x20,
// transMtx sp+0x50, mtx sp+0x80: MTX34s declared at function scope, Vector3f locals declared after DrawOpa).
// Left: eye index load (ROM `mov r1,#0x1c4; ldrsh r1,[r1,r4]`, ours add r0,#0x100; ldrsh), transMtx d[7]/d[8] store
// order, and the MakeScale block (ROM re-materialises `mov r0,#0` and loads pos.z/scale via r0/r1 differently) =
// the documented zero-remat MakeScale blocker.
#if 0
extern u8 EnDntJiji_sEyeIndices[];
void FUN_0036c174(MTX34* dst, MTX34* a, MTX34* b); // MTX34 multiply

void EnDntJiji_Draw(Actor* thisx, PlayState* play) {
    EnDntJiji* self = (EnDntJiji*)thisx;
    MTX34 mtx;
    MTX34 transMtx;
    MTX34 scaleMtx;
    ((FaceAnimations*)self->faceAnims)->FUN_0035e3a4(0, EnDntJiji_sEyeIndices[self->eyeState]);
    ((FaceAnimations*)self->faceAnims)->FUN_0035e330();
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, (OverrideLimbDrawOpa)EnDntJiji_OverrideLimbDraw,
                      (PostLimbDrawOpa)EnDntJiji_PostLimbDraw, &self->actor, 0);
    Vector3f pos(self->flowerPos.x, self->flowerPos.y, self->flowerPos.z);
    Vector3f scale(0.01f, 0.01f, 0.01f);
    MTX34_MakeTrans(&transMtx, (Vec3f*)&pos);
    MTX34_MakeScale(&scaleMtx, (Vec3f*)&scale);
    FUN_0036c174(&mtx, &transMtx, &scaleMtx);
    self->model->SetUnkAC(1);
    self->model->FUN_003721e0(&mtx);
    self->model->FUN_00372170(0);
}
#endif

GLOBAL_ASM("asm/EnDntJiji_Draw.s")

void EnDntJiji_Burrow(EnDntJiji* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
}

void EnDntJiji_Cower(EnDntJiji* self, PlayState* play) {
    f32 frame = *EnDntJiji_CurFramePtr(&self->skelAnime);

    FUN_003731e0(&self->skelAnime);
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 3, 0x1388, 0);
    if (frame >= self->endFrame) {
        if (Actor_ProcessTalkRequest(&self->actor, play)) {
            self->actionFunc = EnDntJiji_SetupTalk;
        } else {
            FUN_0036bb28(&self->actor, play, 100.0f);
        }
    }
}

void EnDntJiji_GivePrize(EnDntJiji* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if ((Message_GetState(&play->msgCtx) == 6) && Message_ShouldAdvance(play)) {
        if ((self->getItemId == 0x79) || (self->getItemId == 0x7A)) {
            gSaveContext.itemGetInf[1] |= 0x8000;
        } else {
            gSaveContext.itemGetInf[1] |= 0x4000;
        }
        self->actor.textId = 0;
        if ((self->stage != NULL) && (self->stage->actor.update != NULL)) {
            self->stage->action = 0;
            if (!self->unburrow) {
                self->stage->leaderSignal = DNT_SIGNAL_HIDE;
            } else {
                self->stage->leaderSignal = DNT_SIGNAL_RETURN;
            }
        }
        self->actor.flags &= ~1;
        if (!self->unburrow) {
            self->actionFunc = EnDntJiji_SetupHide;
        } else {
            self->actionFunc = EnDntJiji_SetupReturn;
        }
    }
}

void EnDntJiji_Hide(EnDntJiji* self, PlayState* play) {
    f32 frame = *EnDntJiji_CurFramePtr(&self->skelAnime);

    FUN_003731e0(&self->skelAnime);
    if (self->endFrame <= frame) {
        self->actionFunc = EnDntJiji_SetupWait;
    }
}

s32 EnDntJiji_OverrideLimbDraw(Actor* thisx, s32 limbIndex, void* dList, Vec3s* rot, void* arg) {
    return 0;
}

void EnDntJiji_PostLimbDraw(Actor* thisx, s32 limbIndex, void* dList, Vec3s* rot, void* arg) {
}

extern __pure float _fabsf(float);
static __inline __pure float EnDntJiji_Fabsf(float f) {
    return _fabsf(f);
}

void EnDntJiji_Return(EnDntJiji* self, PlayState* play) {
    f32 dx;
    f32 dz;

    FUN_003731e0(&self->skelAnime);
    dx = self->flowerPos.x - self->actor.world.pos.x;
    dz = self->flowerPos.z - self->actor.world.pos.z;
    Math_SmoothStepToS(&self->actor.shape.rot.y, (s16)(Math_FAtan2F(dx, dz) * 10430.378f), 1, 0xBB8, 0);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    if ((self->actor.bgCheckFlags & 8) && (self->actor.bgCheckFlags & 1)) {
        self->actor.velocity.y = 9.0f;
        self->actor.speedXZ = 3.0f;
    }
    if (self->sfxTimer == 0) {
        self->sfxTimer = 5;
        Audio_PlayActorSound2(&self->actor, 0x010002EF);
    }
    if ((EnDntJiji_Fabsf(dx) < 5.0f) && (EnDntJiji_Fabsf(dz) < 5.0f)) {
        self->actor.world.pos.x = self->flowerPos.x;
        self->actor.world.pos.z = self->flowerPos.z;
        if (self->attackFlag) {
            if ((self->stage->actor.update != NULL) && (self->stage->leaderSignal == 0)) {
                self->stage->leaderSignal = 4;
                self->stage->action = 2;
                Audio_QueueSeqCmd(0, 0x01000588, 0);
            }
        }
        self->actor.speedXZ = 0.0f;
        self->isSolid = 0;
        self->actionFunc = EnDntJiji_SetupBurrow;
    }
}

void EnDntJiji_SetFlower(EnDntJiji* self, PlayState* play) {
    if (self->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        self->flowerPos = self->actor.world.pos;
        self->actionFunc = EnDntJiji_SetupWait;
    }
}

void EnDntJiji_SetupBurrow(EnDntJiji* self, PlayState* play) {
    s32 frames = Animation_GetLastFrame(&self->skelAnime, 1);

    self->endFrame = frames;
    Animation_Change_00375c08(&self->skelAnime, 1, 1.0f, 0.0f, frames, 2, -10.0f);
    EffectSsHahen_SpawnBurst(play, &self->actor.world.pos, 6.0f, 0, 15, 5, 20, -1, 10, NULL);
    Audio_PlayActorSound2(&self->actor, 0x010002EC);
    Audio_PlayActorSound2(&self->actor, 0x010002ED);
    self->actionFunc = EnDntJiji_Burrow;
}

void EnDntJiji_SetupCower(EnDntJiji* self, PlayState* play) {
    s32 frames = Animation_GetLastFrame(&self->skelAnime, 3);
    EnDntJiji* s2 = self;

    s2->endFrame = frames;
    Animation_Change_00375c08(&self->skelAnime, 3, 1.0f, 0.0f, frames, 2, -10.0f);
    EffectSsHahen_SpawnBurst(play, &self->actor.world.pos, 3.0f, 0, 9, 3, 10, -1, 10, NULL);
    Audio_PlayActorSound2(&self->actor, 0x010002EC);
    if ((CUR_UPG_VALUE(UPG_NUTS) == 1) || (CUR_UPG_VALUE(UPG_NUTS) == 0)) {
        self->getItemId = 0x79;
    } else {
        self->getItemId = 0x7A;
    }
    self->actor.flags |= 1;
    self->actor.textId = 0x10DB;
    self->unused = 5;
    self->actionFunc = EnDntJiji_Cower;
}

void EnDntJiji_SetupGivePrize(EnDntJiji* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if (Actor_HasParent(&self->actor, play)) {
        self->actionFunc = EnDntJiji_GivePrize;
    } else {
        GiveItem(&self->actor, play, self->getItemId, 400.0f, 200.0f);
    }
}

void EnDntJiji_SetupHide(EnDntJiji* self, PlayState* play) {
    s32 frames = Animation_GetLastFrame(&self->skelAnime, 4);

    self->endFrame = frames;
    Animation_Change_00375c08(&self->skelAnime, 4, 1.0f, 0.0f, frames, 2, -10.0f);
    self->actionFunc = EnDntJiji_Hide;
}

void EnDntJiji_SetupReturn(EnDntJiji* self, PlayState* play) {
    s32 frames = Animation_GetLastFrame(&self->skelAnime, 9);

    self->endFrame = frames;
    Animation_Change_00375c08(&self->skelAnime, 9, 1.0f, 0.0f, frames, 0, -10.0f);
    self->actor.speedXZ = 2.0f;
    self->isSolid = self->unburrow = true;
    self->actionFunc = EnDntJiji_Return;
}

void EnDntJiji_SetupTalk(EnDntJiji* self, PlayState* play) {
    s32 frames = Animation_GetLastFrame(&self->skelAnime, 5);

    self->endFrame = frames;
    Animation_Change_00375c08(&self->skelAnime, 5, 1.0f, 0.0f, frames, 0, -10.0f);
    self->actionFunc = EnDntJiji_Talk;
}

void EnDntJiji_SetupWait(EnDntJiji* self, PlayState* play) {
    s32 frames = Animation_GetLastFrame(&self->skelAnime, 1);

    self->endFrame = frames;
    Animation_Change_00375c08(&self->skelAnime, 1, 0.0f, 0.0f, frames, 0, -10.0f);
    self->skelAnime.curFrame = 8.0f;
    self->isSolid = self->action = DNT_LEADER_ACTION_NONE;
    self->actionFunc = EnDntJiji_Wait;
}

void EnDntJiji_SetupUnburrow(EnDntJiji* self, PlayState* play) {
    s32 frames = Animation_GetLastFrame(&self->skelAnime, 6);

    self->endFrame = frames;
    Animation_Change_00375c08(&self->skelAnime, 6, 1.0f, 0.0f, frames, 2, -10.0f);
    EffectSsHahen_SpawnBurst(play, &self->actor.world.pos, 6.0f, 0, 15, 5, 20, -1, 10, NULL);
    Audio_PlayActorSound2(&self->actor, 0x010002EC);
    self->actionFunc = EnDntJiji_Unburrow;
}

void EnDntJiji_SetupUp(EnDntJiji* self, PlayState* play) {
    s32 frames = Animation_GetLastFrame(&self->skelAnime, 7);

    self->endFrame = frames;
    Animation_Change_00375c08(&self->skelAnime, 7, 1.0f, 0.0f, frames, 2, -10.0f);
    EffectSsHahen_SpawnBurst(play, &self->actor.world.pos, 6.0f, 0, 15, 5, 20, -1, 10, NULL);
    Audio_PlayActorSound2(&self->actor, 0x010002EC);
    self->actionFunc = EnDntJiji_Up;
}

void EnDntJiji_SetupWalk(EnDntJiji* self, PlayState* play) {
    s32 frames = Animation_GetLastFrame(&self->skelAnime, 9);

    self->endFrame = frames;
    Animation_Change_00375c08(&self->skelAnime, 9, 1.0f, 0.0f, frames, 0, -10.0f);
    self->actor.speedXZ = 1.0f;
    self->isSolid = true;
    self->unburrow = true;
    self->actionFunc = EnDntJiji_Walk;
}

void EnDntJiji_Talk(EnDntJiji* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 3, 0x1388, 0);
    if ((Message_GetState(&play->msgCtx) == 5) && Message_ShouldAdvance(play)) {
        FUN_0036ae48(play->cameraPtrs[play->activeCamera]);
        FUN_003725e0(play);
        FUN_0036e980(play, NULL, 7);
        self->actor.parent = NULL;
        GiveItem(&self->actor, play, self->getItemId, 400.0f, 200.0f);
        self->actionFunc = EnDntJiji_SetupGivePrize;
    }
}

void EnDntJiji_Unburrow(EnDntJiji* self, PlayState* play) {
    f32 frame = *EnDntJiji_CurFramePtr(&self->skelAnime);

    FUN_003731e0(&self->skelAnime);
    if (self->endFrame <= frame) {
        if (self->action != DNT_LEADER_ACTION_ATTACK) {
            self->actionFunc = EnDntJiji_SetupWalk;
        } else {
            self->actionFunc = EnDntJiji_SetupReturn;
        }
    }
}

void EnDntJiji_Up(EnDntJiji* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 3, 0x1388, 0);
    if (self->actor.xzDistToPlayer < 150.0f) {
        self->actionFunc = EnDntJiji_SetupCower;
    }
}

void EnDntJiji_Wait(EnDntJiji* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    FUN_003731e0(&self->skelAnime);
    if ((self->timer == 1) && (self->actor.xzDistToPlayer < 150.0f) && !Gameplay_InCsMode(play) &&
        !(player->stateFlags1 & 0x800)) {
        OnePointCutscene_Init(play, 2230, -99, &self->actor, 0);
        self->timer = 0;
        FUN_0036e980(play, NULL, 8);
        self->actionFunc = EnDntJiji_SetupUnburrow;
    }
}

void EnDntJiji_Walk(EnDntJiji* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 5, 0x3E8, 0);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    Math_ApproachF(&self->actor.speedXZ, 1.0f, 0.2f, 0.4f);
    if (self->sfxTimer == 0) {
        self->sfxTimer = 8;
        Audio_PlayActorSound2(&self->actor, 0x010002EF);
    }
    if ((self->actor.bgCheckFlags & 8) && (self->actor.bgCheckFlags & 1)) {
        self->actor.velocity.y = 9.0f;
        self->actor.speedXZ = 3.0f;
    }
    if (self->actor.xzDistToPlayer < 100.0f) {
        if (CUR_UPG_VALUE(UPG_STICKS) == 1) {
            self->getItemId = 0x77;
        } else {
            self->getItemId = 0x78;
        }
        self->actor.textId = 0x104D;
        FUN_00367c7c(play, self->actor.textId, NULL);
        self->unused = 5;
        self->actor.speedXZ = 0.0f;
        self->actionFunc = EnDntJiji_Talk;
    }
}

}
