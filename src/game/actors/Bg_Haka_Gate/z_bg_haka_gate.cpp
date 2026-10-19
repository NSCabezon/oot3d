extern "C" {
#include "z_bg_haka_gate.hpp"
#include <math.h>

#define FLAGS 0x80400000

#define BGHAKAGATE_STATUE 0
#define BGHAKAGATE_SKULL 3
#define ACTOR_FLAG_UPDATE_CULLING_DISABLED 0x10
#define ACTOR_FLAG_REACT_TO_LENS 0x80
#define SKULL_OF_TRUTH_FOUND 100
#define NA_SE_EV_METALDOOR_STOP 0x0100018E
#define NA_SE_EV_METALDOOR_SLIDE_FLAGGED 0x0100018D


s32 Math_StepToS(s16* pValue, s16 target, s16 step);
s32 Math_ScaledStepToS(s16* pValue, s16 target, s16 scale);
extern __pure float _fabsf(float);
s32 OnePointCutscene_Attention(PlayState* play, Actor* actor, s32 arg2, s32 arg3);
void Flags_SetSwitch(PlayState* play, s32 flag);
void FUN_0035046c(SkeletonAnimationModel** skelAnimModel);

void BgHakaGate_Init(Actor* thisx, PlayState* play);
void BgHakaGate_Destroy(Actor* thisx, PlayState* play);
void BgHakaGate_Update(Actor* thisx, PlayState* play);
void BgHakaGate_Draw(Actor* thisx, PlayState* play);
void FUN_003e1f20(BgHakaGate* self, PlayState* play);
void FUN_00176a54(BgHakaGate* self, PlayState* play);
void FUN_00176ac0(BgHakaGate* self, PlayState* play);
void FUN_00181c4c(BgHakaGate* self, PlayState* play);
void FUN_00181e38(BgHakaGate* self, PlayState* play);
void FUN_0019a798(BgHakaGate* self, PlayState* play);
void FUN_0019ae14(BgHakaGate* self, PlayState* play);
void FUN_001cd7ec(BgHakaGate* self, PlayState* play);
void FUN_002070e8(BgHakaGate* self, PlayState* play);
void FUN_0021dd58(BgHakaGate* self, PlayState* play);

// Statics live in the data dump at 0x005110BC (see data_syms.s).
typedef struct BgHakaGateStatics {
    /* 0x00 */ u8 puzzleState;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ s16 skullOfTruthRotY;
    /* 0x04 */ s16 statueRotY;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ f32 statueDistToPlayer;
} BgHakaGateStatics;
extern BgHakaGateStatics BgHakaGate_sStatics;
#define sPuzzleState (BgHakaGate_sStatics.puzzleState)
#define sSkullOfTruthRotY (BgHakaGate_sStatics.skullOfTruthRotY)
#define sStatueDistToPlayer (BgHakaGate_sStatics.statueDistToPlayer)
#define sStatueRotY (BgHakaGate_sStatics.statueRotY)
#define DEG_TO_BINANG(deg) ((s16)((deg) * 182.04445f))

GLOBAL_ASM("data/z_Bg_Haka_Gate.data.s")

GLOBAL_ASM("../src/game/actors/Bg_Haka_Gate/data_syms.s")

ActorInit Bg_Haka_Gate_InitVars = {
    ACTOR_BG_HAKA_GATE,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_HAKA_OBJECTS,
    sizeof(BgHakaGate),
    (ActorFunc)BgHakaGate_Init,
    (ActorFunc)BgHakaGate_Destroy,
    (ActorFunc)BgHakaGate_Update,
    (ActorFunc)BgHakaGate_Draw,
};

void FUN_0034f94c(void* model, PlayState* play, Actor* actor, s32 type);
extern InitChainEntry BgHakaGate_sInitChainRom;

void BgHakaGate_Init(Actor* thisx, PlayState* play) {
    BgHakaGate* self = (BgHakaGate*)thisx;
    InitChainEntry sInitChain[1] = { BgHakaGate_sInitChainRom };
    CollisionHeader* colHeader = NULL;
    PlayState* play2 = play;

    Actor_ProcessInitChain(thisx, sInitChain);
    self->switchFlag = (thisx->params >> 8) & 0xFF;
    thisx->params &= 0xFF;
    DynaPolyActor_Init(&self->dyna, 0);
    sStatueRotY = 0;
    FUN_00372f38(thisx, play, &self->skelAnimModel[0], 0x1C, &self->skelAnimModel[1], 0x1A,
                                             &self->skelAnimModel[2], 0x14, &self->skelAnimModel[3], 0x18,
                                             &self->skelAnimModel[4], 0x1B, 0);
    self->unk_1CC = 0;
    switch (thisx->params) {
        case 0:
            self->unk_1CC = (s32)self->skelAnimModel[0];
            break;
        case 2:
            self->unk_1CC = (s32)self->skelAnimModel[2];
            break;
        case 3:
            self->unk_1CC = (s32)self->skelAnimModel[3];
            break;
        default:
            self->unk_1CC = (s32)self->skelAnimModel[1];
            break;
    }
    FUN_0034f94c(&self->skelAnimModel[5], play, thisx, 11);
    if (thisx->params == BGHAKAGATE_SKULL) {
        if (sSkullOfTruthRotY != 0x100) {
            self->actionFunc = FUN_0021dd58;
        } else if (ABS(thisx->shape.rot.y) < 0x4000) {
            if ((Rand_ZeroOne() * 3.0f) < sPuzzleState) {
                self->actionVar4 = 1;
                sSkullOfTruthRotY = thisx->shape.rot.y + 0x8000;
                if (Flags_GetSwitch(play2, self->switchFlag)) {
                    self->actionFunc = FUN_003e1f20;
                } else {
                    self->actionFunc = FUN_002070e8;
                }
            } else {
                sPuzzleState++;
                self->actionFunc = FUN_0021dd58;
            }
        } else {
            self->actionFunc = FUN_0021dd58;
        }
        self->actionVar5 = Rand_ZeroOne() * 20.0f;
        thisx->flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
        if (Flags_GetSwitch(play, self->switchFlag)) {
            self->actionVar3 = 350;
        }
    } else {
        if (thisx->params == BGHAKAGATE_STATUE) {
            ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 0x14), &colHeader);
            self->actionVar1 = 0;
            sStatueDistToPlayer = 0.0f;
            if (Flags_GetSwitch(play, self->switchFlag)) {
                self->actionFunc = FUN_00181c4c;
            } else {
                self->actionFunc = FUN_00181e38;
            }
        } else if (thisx->params == 1) {
            ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 0x13), &colHeader);
            if (Flags_GetSwitch(play, self->switchFlag)) {
                self->actionFunc = FUN_003e1f20;
            } else {
                self->actionFunc = FUN_0019ae14;
            }
        } else {
            ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 0xE), &colHeader);
            if (Flags_GetSwitch(play, self->switchFlag)) {
                thisx->world.pos.y += 80.0f;
                self->actionFunc = FUN_003e1f20;
            } else {
                thisx->flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
                Actor_SetFocus(thisx, 30.0f);
                self->actionFunc = FUN_00176ac0;
            }
        }
        self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
    }
}

void BgHakaGate_Destroy(Actor* thisx, PlayState* play) {
    BgHakaGate* self = (BgHakaGate*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    if (thisx->params == BGHAKAGATE_STATUE) {
        sSkullOfTruthRotY = 0x100;
        sPuzzleState = 1;
    }
    FUN_00350f34(thisx, &self->skelAnimModel[0], &self->skelAnimModel[1],
                                            &self->skelAnimModel[2], &self->skelAnimModel[3],
                                            &self->skelAnimModel[4], 0);
    FUN_0035046c(&self->skelAnimModel[5]);
    self->unk_1CC = 0;
}

// BgHakaGate_Draw: 58 words off (structure matches; flame-matrix 9-element scale block needs ROM's per-element
// `vmov s1,s0; vmul s1,m,s1` copies for x/y, ~20 helper shapes tried; prologue s16/s17 pool order differs too).
#if 0
void FUN_003713fc(MTX34* m, s32 mode, f32 x, f32 y, f32 z);
void Matrix_RotateX(MTX34* mtx, s32 mode, f32 angle);
s16 Camera_GetCamDirYaw(Camera* camera);
void FUN_00373bec(void* obj);

static inline void BgHakaGate_Scale(MTX34* m, f32 x, f32 y, f32 z) {
    m->data[0][0] *= x;
    m->data[1][0] *= x;
    m->data[2][0] *= x;
    m->data[0][1] *= y;
    m->data[1][1] *= y;
    m->data[2][1] *= y;
    m->data[0][2] *= z;
    m->data[1][2] *= z;
    m->data[2][2] *= z;
}

void BgHakaGate_Draw(Actor* thisx, PlayState* play) {
    BgHakaGate* self = (BgHakaGate*)thisx;
    MTX34 mtx;

    MTX34CopyAsm(&mtx, &thisx->modelMtx);
    if (thisx->flags & ACTOR_FLAG_REACT_TO_LENS) {
        if (self->skelAnimModel[3] != NULL) {
            self->skelAnimModel[3]->SetUnkAC(1);
            self->skelAnimModel[3]->FUN_003721e0(&mtx);
            self->skelAnimModel[3]->FUN_00372170(0);
        }
    } else if (thisx->params == 1) {
        FUN_003713fc(&mtx, 1, 0.0f, 0.0f, -2000.0f);
        Matrix_RotateX(&mtx, 1, self->actionVar2 * (2.0f * 3.14159274f / 65536.0f));
        FUN_003713fc(&mtx, 1, 0.0f, 0.0f, 2000.0f);
        if (self->skelAnimModel[1] != NULL) {
            self->skelAnimModel[1]->SetUnkAC(1);
            self->skelAnimModel[1]->FUN_003721e0(&mtx);
            self->skelAnimModel[1]->FUN_00372170(0);
        }
        mtx = thisx->modelMtx;
        FUN_003713fc(&mtx, 1, 0.0f, 0.0f, 2000.0f);
        Matrix_RotateX(&mtx, 1, -self->actionVar2 * (2.0f * 3.14159274f / 65536.0f));
        FUN_003713fc(&mtx, 1, 0.0f, 0.0f, -2000.0f);
        if (self->skelAnimModel[4] != NULL) {
            self->skelAnimModel[4]->SetUnkAC(1);
            self->skelAnimModel[4]->FUN_003721e0(&mtx);
            self->skelAnimModel[4]->FUN_00372170(0);
        }
    } else {
        if ((SkeletonAnimationModel*)self->unk_1CC != NULL) {
            ((SkeletonAnimationModel*)self->unk_1CC)->SetUnkAC(1);
            ((SkeletonAnimationModel*)self->unk_1CC)->FUN_003721e0(&mtx);
            ((SkeletonAnimationModel*)self->unk_1CC)->FUN_00372170(0);
        }
    }
    if (thisx->params == BGHAKAGATE_SKULL && self->actionVar3 > 0) {
        MTX34 flameMtx;
        f32 angle;

        MTX34CopyAsm(&flameMtx, &thisx->modelMtx);
        FUN_003713fc(&flameMtx, 0, thisx->world.pos.x, thisx->world.pos.y + 15.0f, thisx->world.pos.z);
        angle = Camera_GetCamDirYaw(play->cameraPtrs[play->activeCamera]) * (2.0f * 3.14159274f / 65536.0f);
        if (angle != 0.0f) {
            f32 s = sinf(angle);
            f32 c = cosf(angle);
            f32 a, b;

            a = flameMtx.data[0][0];
            b = flameMtx.data[0][2];
            flameMtx.data[0][0] = a * c - b * s;
            flameMtx.data[0][2] = a * s + b * c;
            a = flameMtx.data[1][0];
            b = flameMtx.data[1][2];
            flameMtx.data[1][0] = a * c - b * s;
            flameMtx.data[1][2] = a * s + b * c;
            a = flameMtx.data[2][0];
            b = flameMtx.data[2][2];
            flameMtx.data[2][0] = a * c - b * s;
            flameMtx.data[2][2] = a * s + b * c;
        }
        {
            f32 scale = self->actionVar3 * 0.00001f;
            BgHakaGate_Scale(&flameMtx, scale, scale, scale);
        }
        FUN_00373bec(self->unk_1EC);
        ((SkeletonAnimationModel*)self->skelAnimModel[5])->SetUnkAC(1);
        ((SkeletonAnimationModel*)self->skelAnimModel[5])->FUN_003721e0(&flameMtx);
        ((SkeletonAnimationModel*)self->skelAnimModel[5])->FUN_00372170(0);
    }
}
#endif

GLOBAL_ASM("asm/BgHakaGate_Draw.s")

void BgHakaGate_Update(Actor* thisx, PlayState* play) {
    BgHakaGate* self = (BgHakaGate*)thisx;

    self->actionFunc(self, play);
    if (thisx->params == BGHAKAGATE_SKULL) {
        self->actionVar5++;
    }
}

void FUN_003e1f20(BgHakaGate* self, PlayState* play) {
}

void FUN_00181c4c(BgHakaGate* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (self->dyna.unk_1A8 != 0.0f) {
        player->stateFlags2 &= ~0x10;
        self->dyna.unk_1A8 = 0.0f;
    }
}

void FUN_00176ac0(BgHakaGate* self, PlayState* play) {
    if (Flags_GetSwitch(play, self->switchFlag)) {
        OnePointCutscene_Attention(play, &self->dyna.actor, 0, 0);
        self->actionFunc = FUN_00176a54;
    }
}

void FUN_002070e8(BgHakaGate* self, PlayState* play) {
    if (Flags_GetSwitch(play, self->switchFlag)) {
        if (Math_StepToS(&self->actionVar3, 350, 20)) {
            self->actionFunc = FUN_003e1f20;
        }
    }
}

void FUN_0021dd58(BgHakaGate* self, PlayState* play) {
    if (Flags_GetSwitch(play, self->switchFlag)) {
        Math_StepToS(&self->actionVar3, 350, 20);
    }
    if (play->actorCtx.unk_03) {
        self->dyna.actor.flags |= ACTOR_FLAG_REACT_TO_LENS;
    } else {
        self->dyna.actor.flags &= ~ACTOR_FLAG_REACT_TO_LENS;
    }
}

void FUN_001cd7ec(BgHakaGate* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 turnFinished;
    s16 turnAngle;

    self->actionVar2++;
    self->actionVar2 = CLAMP_MAX(self->actionVar2, 5);
    turnFinished = Math_StepToS(&self->actionVar3, 600, self->actionVar2);
    turnAngle = self->actionVar3 * self->actionVar1;
    self->dyna.actor.shape.rot.y = DEG_TO_BINANG((self->actionVar4 + turnAngle) * 0.1f);
    if ((player->stateFlags2 & 0x10) && (sStatueDistToPlayer > 0.0f)) {
        player->actor.world.pos.x =
            self->dyna.actor.home.pos.x +
            (sStatueDistToPlayer * Math_SinS(self->dyna.actor.shape.rot.y - self->actionVar5));
        player->actor.world.pos.z =
            self->dyna.actor.home.pos.z +
            (sStatueDistToPlayer * Math_CosS(self->dyna.actor.shape.rot.y - self->actionVar5));
    } else {
        sStatueDistToPlayer = 0.0f;
    }
    sStatueRotY = self->dyna.actor.shape.rot.y;
    if (turnFinished) {
        player->stateFlags2 &= ~0x10;
        self->dyna.unk_1A8 = 0.0f;
        self->actionVar4 = (self->actionVar4 + turnAngle) % 3600;
        self->actionVar2 = 0;
        self->actionVar3 = 0;
        self->actionVar1 = 8;
        self->actionFunc = FUN_00181e38;
    }
    FUN_00373264(&self->dyna.actor, 0x01000161);
}

#if 0 // near-miss drafts, asm kept (see STATUS.md). FUN_00181e38: 6 words (ROM converts playerDirection to float in s2
      // before the forceDirection select; ours s1 after). R11: explicit `f32 fpd` local (24 decl-order x placement forms),
      // operand order, greedy+pair mutations: all 6.
void FUN_00181e38(BgHakaGate* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 playerDirection;
    f32 forceDirection;
    if (self->dyna.unk_1A8 != 0.0f) {
        if (self->actionVar1 == 0) {
            self->actionVar5 = self->dyna.actor.shape.rot.y - self->dyna.actor.yawTowardsPlayer;
            sStatueDistToPlayer = self->dyna.actor.xzDistToPlayer;
            playerDirection = ((s16)(self->dyna.actor.yawTowardsPlayer - player->actor.shape.rot.y) > 0) ? -1 : 1;
            forceDirection = (self->dyna.unk_1A8 >= 0.0f) ? 1.0f : -1.0f;
            self->actionVar1 = playerDirection * forceDirection;
            self->actionFunc = FUN_001cd7ec;
        } else {
            player->stateFlags2 &= ~0x10;
            self->dyna.unk_1A8 = 0.0f;
            if (self->actionVar1 != 0) {
                self->actionVar1--;
            }
        }
    } else {
        if (sPuzzleState == 100) {
            self->actionFunc = FUN_00181c4c;
        } else {
            self->actionVar1 = 0;
        }
    }
}

void FUN_0019ae14(BgHakaGate* self, PlayState* play) {
    if ((sStatueDistToPlayer > 1.0f) && (sStatueRotY != 0)) {
        Player* player = GET_PLAYER(play);
        f32 radialDist;
        f32 angDist;
        f32 cos = Math_CosS(sStatueRotY);
        f32 sin = Math_SinS(sStatueRotY);
        f32 dx = player->actor.world.pos.x - self->dyna.actor.world.pos.x;
        f32 dz = player->actor.world.pos.z - self->dyna.actor.world.pos.z;

        radialDist = dx * cos - dz * sin;
        angDist = dx * sin + dz * cos;

        if ((radialDist > 110.0f) || (_fabsf(angDist) > 40.0f)) {
            s16 yawDiff = sSkullOfTruthRotY - sStatueRotY;

            sStatueDistToPlayer = 0.0f;
            if (ABS(yawDiff) < 0x80) {
                Flags_SetSwitch(play, self->switchFlag);
                sPuzzleState = SKULL_OF_TRUTH_FOUND;
                self->actionFunc = FUN_003e1f20;
            } else {
                Audio_PlaySoundGeneral(0x0100048B, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
                FUN_0036b940(play, &play->colCtx.dyna, self->dyna.bgId);
                Audio_PlayActorSound2(&self->dyna.actor, 0x010001A8);
                self->actionVar1 = 90;
                self->actionFunc = FUN_0019a798;
            }
        }
    }
}
#endif

void FUN_0019a798(BgHakaGate* self, PlayState* play) {
    s16 timer = self->actionVar1;

    if (self->actionVar1 != 0) {
        self->actionVar1--;
    }
    if (timer > 0) {
        if (self->actionVar1 == 0) {
            Audio_PlayActorSound2(&self->dyna.actor, 0x0100016B);
        }
    }
    if (self->actionVar1 == 0) {
        if (Math_ScaledStepToS(&self->actionVar2, 0, 0x800)) {
            Player* player = GET_PLAYER(play);

            Audio_PlayActorSound2(&self->dyna.actor, 0x010001EA);
            if (gSaveContext.respawnFlag == 0 && (player->stateFlags1 & 0x2000) && (self->dyna.actor.xzDistToPlayer < 220.0f)) {
                Gameplay_TriggerVoidOut(play);
                play->fadeOutTransition = 4;
                Audio_PlaySoundGeneral(0x010004E1, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
                play->sceneLoadFlag = 0x14;
            }
            FUN_0036d15c(play, &play->colCtx.dyna, self->dyna.bgId);
            self->actionFunc = FUN_0019ae14;
        }
    } else {
        Math_ScaledStepToS(&self->actionVar2, 0x3000, 0x800);
    }
}

void FUN_00176a54(BgHakaGate* self, PlayState* play) {
    if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y + 80.0f, 1.0f)) {
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_METALDOOR_STOP);
        self->dyna.actor.flags &= ~ACTOR_FLAG_UPDATE_CULLING_DISABLED;
        self->actionFunc = FUN_003e1f20;
    } else {
        FUN_00373264(&self->dyna.actor, NA_SE_EV_METALDOOR_SLIDE_FLAGGED);
    }
}

}
