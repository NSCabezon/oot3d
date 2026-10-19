extern "C" {
#include "z_en_daiku_kakariko.hpp"

#define FLAGS 0x00000019

void EnDaikuKakariko_Init(EnDaikuKakariko* self, PlayState* play);
void EnDaikuKakariko_Destroy(Actor* thisx, PlayState* play);
void EnDaikuKakariko_Update(Actor* actor, GameState* state);
void EnDaikuKakariko_Draw(Actor* thisx, PlayState* play);

void EnDaikuKakariko_HandleTalking(EnDaikuKakariko* self, PlayState* play);
void EnDaikuKakariko_Talk(EnDaikuKakariko* self, PlayState* play);
void EnDaikuKakariko_Run(EnDaikuKakariko* self, PlayState* play);
void EnDaikuKakariko_StopRunning(EnDaikuKakariko* self, PlayState* play);
f32 FUN_00299ec0(PlayState* play, Actor* actor);
void EnDaikuKakariko_Wait(EnDaikuKakariko* self, PlayState* play);
f32 BgCheck_EntityRaycastFloor4(CollisionContext* colCtx, CollisionPoly** outPoly, s32* bgId, Actor* actor, Vec3f* pos);
void FUN_0036932c(SkeletonAnimationModel* model, s32 idx);
void FUN_0037266c(SkeletonAnimationModel* model, s32 idx);
void FUN_00357a50(SkelAnime* skelAnime, s32 idx, s32 count, void* colors, s32 arg4);
s32 EnDaikuKakariko_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void EnDaikuKakariko_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
extern u8 EnDaikuKakariko_sColors[];
extern f32 EnDaikuKakariko_sNeckMax;
extern u16 EnDaikuKakariko_sInitFlags[];
extern ColliderCylinderInit EnDaikuKakariko_sCylinderInit;
extern DamageTable EnDaikuKakariko_sDamageTable;
extern CollisionCheckInfoInit2 EnDaikuKakariko_sColChkInit;
void* ZAR_GetCMBByIndex(void* zar, s32 index);
void FUN_00353e78(void* zar, PlayState* play, SkelAnime* skelAnime, void* cmb, void* p, s32 a, void* b, void* c, s32 d);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
extern s32 EnDaikuKakariko_sMaskReactionSets[];
void FUN_0034c664(Actor* actor, EnDaikuKakarikoInteractInfo* info, s32 arg2, s32 arg3);
void FUN_0034df48(MTX34* mtx, Vec3s* rot);
void __ARM_common_memcpy2_6(void* dst, void* src);
void EnDaikuKakariko_SetAnimFromIndex(EnDaikuKakariko* self, s32 index, s32* currentIndex);
extern EnDaikuKakarikoAnimInfo EnDaikuKakariko_sAnimationInfo[];

GLOBAL_ASM("data/z_En_Daiku_Kakariko.data.s")
GLOBAL_ASM("../src/game/actors/En_Daiku_Kakariko/data_syms.s")

ActorInit En_Daiku_Kakariko_InitVars = {
    ACTOR_EN_DAIKU_KAKARIKO,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_DAIKU,
    sizeof(EnDaikuKakariko),
    (ActorFunc)EnDaikuKakariko_Init,
    (ActorFunc)EnDaikuKakariko_Destroy,
    (ActorFunc)EnDaikuKakariko_Update,
    (ActorFunc)EnDaikuKakariko_Draw,
};

void EnDaikuKakariko_Init(EnDaikuKakariko* self, PlayState* play) {
    ObjectStatus* status;
    void* zar;
    void* cmb;
    Actor* thisx = &self->actor;
    EnDaikuKakariko* s2 = self;

    if (gSaveContext.linkAge != 0) {
        switch (play->sceneNum) {
            case 0x52:
                if (gSaveContext.nightFlag == 0) {
                    self->flags |= 1;
                    self->flags |= EnDaikuKakariko_sInitFlags[self->actor.params & 3];
                }
                break;
            case 0x2A:
                if (gSaveContext.nightFlag == 1) {
                    self->flags |= 2;
                }
                break;
            case 0x30:
                self->flags |= 4;
                break;
        }
    }

    if (!(self->flags & 7)) {
        Actor_Kill(&self->actor);
    }

    if (gSaveContext.nightFlag == 1) {
        self->flags |= 8;
    }

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 40.0f);

    status = play->GetObjectStatus(self->actor.objBankIndex);
    zar = (void*)((u8*)status + 0x10);
    cmb = ZAR_GetCMBByIndex(zar, 0);
    thisx->unk_19A = 1;
    FUN_00372f38(&self->actor, play, &self->skelAnimModel[0], 0, 0);
    FUN_00353e78(zar, play, &self->skelAnime, cmb, self->actor.unk_178, 2, self->unk_228, self->unk_638, 20);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, &EnDaikuKakariko_sCylinderInit);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, &EnDaikuKakariko_sDamageTable, &EnDaikuKakariko_sColChkInit);

    Animation_Change_00375c08(&self->skelAnime, EnDaikuKakariko_sAnimationInfo[0].animation, 1.0f, 0.0f,
                              Animation_GetLastFrame(&self->skelAnime, EnDaikuKakariko_sAnimationInfo[0].animation),
                              EnDaikuKakariko_sAnimationInfo[0].mode, EnDaikuKakariko_sAnimationInfo[0].morphFrames);

    Actor_UpdateBgCheckInfo(play, &self->actor, 0.0f, 0.0f, 0.0f, 4);

    thisx->targetMode = 6;
    s2->currentAnimIndex = -1;
    self->actor.gravity = 0.0f;
    self->runSpeed = 3.0f;
    self->actor.uncullZoneForward = 1200.0f;
    self->interactInfo.headRot.z = 0;
    self->interactInfo.headRot.y = 0;
    self->interactInfo.headRot.x = 0;
    self->interactInfo.torsoRot.z = 0;
    self->interactInfo.torsoRot.y = 0;
    self->interactInfo.torsoRot.x = 0;

    if (self->flags & 0x40) {
        Actor* thisx = &self->actor;
        thisx->gravity = -1.0f;
    }

    if (self->flags & 0x10) {
        EnDaikuKakariko_SetAnimFromIndex(self, 3, &self->currentAnimIndex);
        self->actionFunc = EnDaikuKakariko_Run;
    } else {
        if (self->flags & 8) {
            if (((self->actor.params & 3) == 1) || ((self->actor.params & 3) == 3)) {
                EnDaikuKakariko_SetAnimFromIndex(self, 5, &self->currentAnimIndex);
                self->flags |= 0x800;
            } else {
                EnDaikuKakariko_SetAnimFromIndex(self, 1, &self->currentAnimIndex);
            }
        } else {
            EnDaikuKakariko_SetAnimFromIndex(self, 0, &self->currentAnimIndex);
        }

        self->flags |= 0x100;
        self->actionFunc = EnDaikuKakariko_Wait;
    }
}

void EnDaikuKakariko_Destroy(Actor* thisx, PlayState* play) {
    EnDaikuKakariko* self = (EnDaikuKakariko*)thisx;

    Collider_DestroyCylinder(play, &self->collider);
    FUN_003508b8(thisx, self->skelAnimModel[0], self->skelAnimModel[1], self->skelAnimModel[2],
                 self->skelAnimModelRest, 0);
    SkelAnime_Free2(&self->skelAnime);
}

void EnDaikuKakariko_Update(Actor* actor, GameState* state) {
    EnDaikuKakariko* self = (EnDaikuKakariko*)actor;
    PlayState* play = (PlayState*)state;
    Player* player = GET_PLAYER(play);

    if (self->currentAnimIndex == 3) {
        if (((s32)self->skelAnime.curFrame == 6) || ((s32)self->skelAnime.curFrame == 15)) {
            Audio_PlayActorSound2(&self->actor, 0x01000328);
        }
    }

    Collider_UpdateCylinder(&self->actor, &self->collider);

    if (self->flags & 4) {
        EnDaikuKakariko* s2 = self;

        self->collider.dim.pos.x += -27.0f;
        self->collider.dim.pos.z += -27.0f;
        s2->collider.dim.radius = 63.0f;
    }

    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);

    self->actionFunc(self, play);

    self->interactInfo.trackPos.x = player->actor.focus.pos.x;
    self->interactInfo.trackPos.y = player->actor.focus.pos.y;
    self->interactInfo.trackPos.z = player->actor.focus.pos.z;

    if (self->flags & 0x100) {
        self->neckAngleTarget.x = 5900;
        self->flags |= 0x1000;
        FUN_0034c664(&self->actor, &self->interactInfo, 0, 2);
    } else if (self->flags & 0x200) {
        self->neckAngleTarget.x = 5900;
        self->flags |= 0x1000;
        FUN_0034c664(&self->actor, &self->interactInfo, 0, 4);
    }

    Math_SmoothStepToS(&self->neckAngle.x, self->neckAngleTarget.x, 1, 1820, 0);
}

void EnDaikuKakariko_Draw(Actor* thisx, PlayState* play) {
    EnDaikuKakariko* self = (EnDaikuKakariko*)thisx;
    s32 type;

    GraphicsContext_UnknownStub(play->state.gfxCtx);
    FUN_0036932c(self->skelAnime.unk_28, 0);
    FUN_0036932c(self->skelAnime.unk_28, 1);
    FUN_0036932c(self->skelAnime.unk_28, 2);
    FUN_0036932c(self->skelAnime.unk_28, 3);

    type = 0;
    switch (thisx->params & 3) {
        case 0:
            FUN_0037266c(self->skelAnime.unk_28, 0);
            break;
        case 1:
            FUN_0037266c(self->skelAnime.unk_28, 1);
            type = 1;
            break;
        case 2:
            FUN_0037266c(self->skelAnime.unk_28, 2);
            type = 2;
            break;
        case 3:
            FUN_0037266c(self->skelAnime.unk_28, 3);
            type = 3;
            break;
    }

    FUN_00357a50(&self->skelAnime, 0, 4, &EnDaikuKakariko_sColors[type * 16], 0);
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, (OverrideLimbDrawOpa)EnDaikuKakariko_OverrideLimbDraw,
                      (PostLimbDrawOpa)EnDaikuKakariko_PostLimbDraw, &self->actor, 0);
}

s32 EnDaikuKakariko_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnDaikuKakariko* self = (EnDaikuKakariko*)thisx;
    Vec3s angle;

    switch (limbIndex) {
        case 8:
            __ARM_common_memcpy2_6(&angle, &self->interactInfo.torsoRot);
            FUN_0034df48(mtx, &angle);
            break;
        case 9:
            angle = self->interactInfo.headRot;
            if (self->flags & 0x1000) {
                angle.y += self->neckAngle.y;
                angle.x += self->neckAngle.x;
                angle.x = ((f32)angle.x > EnDaikuKakariko_sNeckMax)
                              ? EnDaikuKakariko_sNeckMax
                              : (((f32)angle.x > -EnDaikuKakariko_sNeckMax) ? (f32)angle.x : -EnDaikuKakariko_sNeckMax);
            }
            FUN_0034df48(mtx, &angle);
            break;
    }
    return 0;
}

// PostLimbDraw: draft, 6/35 words off (all VFP numbering of the guarded Vec3f init: ROM x->s0,y->s1,z->s2, ours x->s2,y->s0,z->s1;
// ~75 ctor/param/store-order variants tried). ROM loads base 0x521D48 and reads the guard at +4: both the f32 at 0x521D48
// (EnDaikuKakariko_sNeckMax, EQU today) and the guard 0x521D4C must be C statics of this TU (static f32 sNeckMax = 2000.0f declared
// before use) with the Vector3f object 0x599074 (12 B) in the TU .bss; needs pin_static (orchestrator). Not enabled.
#if 0
// N64 Matrix_MultVec3f shape: (src, dest) order drives the arg scheduling of the tail call
static inline void EnDaikuKakariko_MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}

void EnDaikuKakariko_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnDaikuKakariko* self = (EnDaikuKakariko*)thisx;
    static Vector3f sUnkVec(700.0f, 1100.0f, 0.0f);

    if (limbIndex == 9) {
        EnDaikuKakariko_MultVec3f(&sUnkVec, (Vector3f*)&self->actor.focus.pos, mtx);
    }
}
#endif

GLOBAL_ASM("asm/EnDaikuKakariko_PostLimbDraw.s")

void EnDaikuKakariko_Run(EnDaikuKakariko* self, PlayState* play) {
    Path* path;
    Vec3s* pathPos;
    f32 xDist;
    f32 zDist;
    s16 runAngle;
    f32 runDist;
    s16 angleStepDiff;
    s32 run;

    EnDaikuKakariko* s2 = self;
    do {
        path = &play->setupPathList[(self->actor.params & 0xFF00) >> 8];
        pathPos = &path->points[self->waypoint];
        xDist = pathPos->x - self->actor.world.pos.x;
        zDist = pathPos->z - self->actor.world.pos.z;
        runAngle = Math_FAtan2F(xDist, zDist) * 10430.378f;
        runDist = sqrtf((xDist * xDist) + (zDist * zDist));

        run = false;

        if (runDist <= 10.0f) {
            if (!self->pathContinue) {
                self->waypoint++;

                if (self->waypoint >= path->count) {
                    if (self->flags & 0x20) {
                        self->waypoint = path->count - 2;
                        self->pathContinue = true;
                        self->run = run = false;

                        if (self->flags & 0x400) {
                            self->timer = 2;
                            EnDaikuKakariko_SetAnimFromIndex(self, 0, &self->currentAnimIndex);
                            self->actionFunc = EnDaikuKakariko_StopRunning;
                            return;
                        }
                    } else {
                        self->waypoint = 0;
                        run = true;
                    }
                } else {
                    self->run = run = true;
                }
            } else {
                self->waypoint--;

                if (self->waypoint < 0) {
                    self->waypoint = 1;
                    self->pathContinue = false;
                    self->run = run = false;

                    if (self->flags & 0x400) {
                        self->timer = 2;
                        EnDaikuKakariko_SetAnimFromIndex(self, 0, &self->currentAnimIndex);
                        self->actionFunc = EnDaikuKakariko_StopRunning;
                        return;
                    }
                } else {
                    run = true;
                }
            }
        }
    } while (run);

    angleStepDiff = Math_SmoothStepToS(&self->actor.shape.rot.y, runAngle, 1, 5000, 0);

    s2->actor.world.rot.y = self->actor.shape.rot.y;

    if (!self->run) {
        EnDaikuKakariko* s3 = self;

        if (angleStepDiff == 0) {
            s3->run = true;
        } else {
            s3->actor.speedXZ = 0.0f;
        }
    }

    if (self->run == true) {
        Math_SmoothStepToF(&self->actor.speedXZ, self->runSpeed, 0.8f, runDist, 0.0f);
    }

    Actor_MoveForward(&self->actor);

    if (self->flags & 0x40) {
        Actor_UpdateBgCheckInfo(play, &self->actor, 0.0f, 0.0f, 0.0f, 4);
    } else if (self->flags & 0x80) {
        self->runFlag |= 1;
        self->flags &= ~0x0080;
    } else if (self->runFlag & 1) {
        Actor_UpdateBgCheckInfo(play, &self->actor, 0.0f, 0.0f, 0.0f, 4);
        self->runFlag &= ~1;
    } else if (!(*(u32*)((u8*)play + 0x5BF4) & 3)) {
        FUN_00299ec0(play, &self->actor);
    }

    FUN_00370734(&self->skelAnime);
    EnDaikuKakariko_HandleTalking(self, play);

    if (self->talkState != 0) {
        self->flags |= 0x200;
        EnDaikuKakariko_SetAnimFromIndex(self, 4, &self->currentAnimIndex);
        self->actionFunc = EnDaikuKakariko_Talk;
    }
}

// StopRunning draft: 24/40 words off (ROM branches out the curFrame copy; armcc if-converts it to vldrgt/vstrgt;
// ~15 variants: arm order, local/ptr/inline copy, --timer, SkelAnime* local; none stops the predication)
#if 0
void EnDaikuKakariko_StopRunning(EnDaikuKakariko* self, PlayState* play) {
    if (FUN_00370734(&self->skelAnime)) {
        self->timer--;

        if (self->timer <= 0) {
            EnDaikuKakariko_SetAnimFromIndex(self, 3, &self->currentAnimIndex);
            self->actionFunc = EnDaikuKakariko_Run;
        } else {
            SkelAnime* skel = &self->skelAnime;

            skel->curFrame = skel->startFrame;
        }
    }

    EnDaikuKakariko_HandleTalking(self, play);

    if (self->talkState != 0) {
        self->flags |= 0x200;
        EnDaikuKakariko_SetAnimFromIndex(self, 4, &self->currentAnimIndex);
        self->actionFunc = EnDaikuKakariko_Talk;
    }
}

#endif

GLOBAL_ASM("asm/EnDaikuKakariko_StopRunning.s")

void EnDaikuKakariko_Talk(EnDaikuKakariko* self, PlayState* play) {
    if (FUN_00370734(&self->skelAnime)) {
        EnDaikuKakariko_SetAnimFromIndex(self, 3, &self->currentAnimIndex);
    }

    EnDaikuKakariko_HandleTalking(self, play);

    if (self->talkState == 0) {
        if (self->flags & 0x10) {
            EnDaikuKakariko_SetAnimFromIndex(self, 3, &self->currentAnimIndex);
            self->flags &= ~0x0300;
            self->flags &= ~0x1000;
            self->interactInfo.headRot.z = 0;
            self->interactInfo.headRot.y = 0;
            self->interactInfo.headRot.x = 0;
            self->interactInfo.torsoRot.z = 0;
            self->interactInfo.torsoRot.y = 0;
            self->interactInfo.torsoRot.x = 0;
            self->actionFunc = EnDaikuKakariko_Run;
        } else {
            if (!(self->flags & 8)) {
                EnDaikuKakariko_SetAnimFromIndex(self, 0, &self->currentAnimIndex);
            }

            if (!(self->flags & 0x800)) {
                self->flags &= ~0x200;
                self->flags |= 0x100;
            }

            self->actionFunc = EnDaikuKakariko_Wait;
        }
    }
}

void EnDaikuKakariko_Wait(EnDaikuKakariko* self, PlayState* play) {
    EnDaikuKakariko_HandleTalking(self, play);

    if (FUN_00370734(&self->skelAnime)) {
        EnDaikuKakariko_SetAnimFromIndex(self, 0, &self->currentAnimIndex);
    }

    if (self->talkState != 0) {
        if (!(self->flags & 8)) {
            EnDaikuKakariko_SetAnimFromIndex(self, 4, &self->currentAnimIndex);
        }

        if (!(self->flags & 0x800)) {
            self->flags |= 0x200;
            self->flags &= ~0x0100;
        }

        self->actionFunc = EnDaikuKakariko_Talk;
    }
}

}
