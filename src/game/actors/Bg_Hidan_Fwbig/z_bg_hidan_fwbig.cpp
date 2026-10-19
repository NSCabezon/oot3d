extern "C" {
#include "z_bg_hidan_fwbig.hpp"

#define FLAGS 0x80400010
#define CAM_ID_MAIN 0

void OnePointCutscene_Init(PlayState* play, s32 csId, s32 duration, Actor* actor, s32 camId);
s32 Player_InCsMode(PlayState* play);
s32 Math_ScaledStepToS(s16* value, s16 target, s16 step);
void FUN_0036d940(Actor* actor, s32 timer); // Actor_PlaySfx_FlaggedTimer

void BgHidanFwbig_Init(Actor* actor, GameState* state);
void BgHidanFwbig_Update(Actor* actor, GameState* state);
void BgHidanFwbig_Destroy(Actor* actor, GameState* state);
void BgHidanFwbig_Draw(Actor* actor, GameState* state);

void BgHidanFwbig_WaitForSwitch(BgHidanFwbig* self, PlayState* play);
void BgHidanFwbig_WaitForCs(BgHidanFwbig* self, PlayState* play);
void BgHidanFwbig_Lower(BgHidanFwbig* self, PlayState* play);
void BgHidanFwbig_Rise(BgHidanFwbig* self, PlayState* play);
void BgHidanFwbig_WaitForPlayer(BgHidanFwbig* self, PlayState* play);
void BgHidanFwbig_Move(BgHidanFwbig* self, PlayState* play);

ActorInit Bg_Hidan_Fwbig_InitVars = {
    ACTOR_BG_HIDAN_FWBIG,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_HIDAN_OBJECTS,
    sizeof(BgHidanFwbig),
    (ActorFunc)BgHidanFwbig_Init,
    (ActorFunc)BgHidanFwbig_Destroy,
    (ActorFunc)BgHidanFwbig_Update,
    (ActorFunc)BgHidanFwbig_Draw,
};

GLOBAL_ASM("data/z_Bg_Hidan_Fwbig.data.s")

GLOBAL_ASM("asm/BgHidanFwbig_Init.s")

void BgHidanFwbig_Destroy(Actor* actor, GameState* state) {
    BgHidanFwbig* self = (BgHidanFwbig*)actor;
    PlayState* play = (PlayState*)state;

    Collider_DestroyCylinder(play, &self->collider);
    FUN_00350f34(&self->actor, &self->skelAnimModel, 0);
}

GLOBAL_ASM("asm/BgHidanFwbig_Update.s")

void BgHidanFwbig_Rise(BgHidanFwbig* self, PlayState* play) {
    if (Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y, 10.0f)) {
        if (self->direction == 0) {
            Flags_UnsetSwitch(play, self->actor.params);
            self->actionFunc = BgHidanFwbig_WaitForSwitch;
        } else {
            self->actionFunc = BgHidanFwbig_Move;
        }
    }
}

void BgHidanFwbig_WaitForSwitch(BgHidanFwbig* self, PlayState* play) {
    BgHidanFwbig* s2 = self;

    if (Flags_GetSwitch(play, self->actor.params)) {
        s2->actionFunc = BgHidanFwbig_WaitForCs;
        OnePointCutscene_Init(play, 3340, -99, &self->actor, CAM_ID_MAIN);
        self->timer = (s16)(35 * 3.0f / 2.0f + 0.5f);
    }
}

void BgHidanFwbig_WaitForCs(BgHidanFwbig* self, PlayState* play) {
    if (self->timer-- == 0) {
        self->actionFunc = BgHidanFwbig_Lower;
    }
}

// Code at 0x0010AB74 (ROM name BgHidanFwbig_WaitForPlayer): the N64 WaitForTimer body.
void BgHidanFwbig_WaitForPlayer(BgHidanFwbig* self, PlayState* play) {
    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer == 0) {
        self->actionFunc = BgHidanFwbig_Rise;
    }
    FUN_0036d940(&self->actor, self->timer);
}

// cfg tail: Bg_Hidan_Curtain's BgHidanCurtain_WaitForTimer code (0x0010ABAC), split off by cfg_split.py.
GLOBAL_ASM("../split/asm/FUN_0010abac.s")

static void BgHidanFwbig_UpdatePosition(BgHidanFwbig* self) {
    s16 startAngle = self->actor.shape.rot.y + self->direction * -0x4000;

    self->actor.world.pos.x = (Math_SinS(startAngle) * 885.4f) + self->actor.home.pos.x;
    self->actor.world.pos.z = (Math_CosS(startAngle) * 885.4f) + self->actor.home.pos.z;
}

void BgHidanFwbig_Lower(BgHidanFwbig* self, PlayState* play) {
    if (Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y - (2400.0f * self->actor.scale.y), 10.0f)) {
        if (self->direction == 0) {
            self->actionFunc = BgHidanFwbig_WaitForPlayer;
            self->timer = (s16)(150 * 3.0f / 2.0f + 0.5f);
        } else if (self->moveState == 2) {
            Actor_Kill(&self->actor);
        } else {
            if (self->moveState == 0) {
                self->actor.shape.rot.y -= (self->direction * 0x1800);
            } else {
                self->actor.shape.rot.y = self->actor.home.rot.y;
                self->moveState = 0;
            }
            BgHidanFwbig_UpdatePosition(self);
            self->actionFunc = BgHidanFwbig_Rise;
        }
    }
}

void BgHidanFwbig_Move(BgHidanFwbig* self, PlayState* play) {
    if (!Player_InCsMode(play)) {
        if (Math_ScaledStepToS(&self->actor.shape.rot.y, self->actor.home.rot.y + (self->direction * 0x6390), 0x20)) {
            self->moveState = 1;
            self->actionFunc = BgHidanFwbig_Lower;
        } else {
            BgHidanFwbig_UpdatePosition(self);
        }
    }
}

void BgHidanFwbig_Draw(Actor* actor, GameState* state) {
    BgHidanFwbig* self = (BgHidanFwbig*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->actor.modelMtx);

    f32 animSpeed = 2.0f;
    if (PauseContext_GetState() != 0) {
        animSpeed = 0.0f;
    }

    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->unk_0C->SetAnimSpeed(animSpeed);
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}
}
