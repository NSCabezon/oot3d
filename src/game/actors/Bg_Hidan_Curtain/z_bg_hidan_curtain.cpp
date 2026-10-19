extern "C" {
#include "z_bg_hidan_curtain.hpp"

#define FLAGS 0x80400010
#define CAM_ID_MAIN 0

void OnePointCutscene_Init(PlayState* play, s32 csId, s32 duration, Actor* actor, s32 camId);

typedef struct BgHidanCurtainParams {
    /* 0x00 */ s16 radius;
    /* 0x02 */ s16 height;
    /* 0x04 */ f32 scale;
    /* 0x08 */ f32 riseDist;
    /* 0x0C */ f32 riseSpeed;
} BgHidanCurtainParams; // size = 0x10

void BgHidanCurtain_Init(Actor* actor, PlayState* play);
void BgHidanCurtain_Destroy(Actor* actor, PlayState* play);
void BgHidanCurtain_Update(Actor* actor, PlayState* play);
void BgHidanCurtain_Draw(Actor* actor, PlayState* play);

void FUN_0010b9d0(BgHidanCurtain* self, PlayState* play); // N64: BgHidanCurtain_WaitForClear
void FUN_0010bdc8(BgHidanCurtain* self, PlayState* play); // N64: BgHidanCurtain_WaitForSwitchOff
void FUN_003ba250(BgHidanCurtain* self, PlayState* play); // N64: BgHidanCurtain_TurnOn
void FUN_003d5fd4(BgHidanCurtain* self, PlayState* play); // N64: BgHidanCurtain_WaitForCutscene
void FUN_003d9f6c(BgHidanCurtain* self, PlayState* play); // N64: BgHidanCurtain_TurnOff
void FUN_003e8738(BgHidanCurtain* self, PlayState* play); // N64: BgHidanCurtain_WaitForSwitchOn
void BgHidanCurtain_WaitForTimer(BgHidanCurtain* self, PlayState* play); // N64 name; code at 0x0010abac (see data file)

extern BgHidanCurtainParams BgHidanCurtain_sHCParams[];

GLOBAL_ASM("data/z_Bg_Hidan_Curtain.data.s")
GLOBAL_ASM("../src/game/actors/Bg_Hidan_Curtain/BgHidanCurtain_data.s")

ActorInit Bg_Hidan_Curtain_InitVars = {
    ACTOR_BG_HIDAN_CURTAIN,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(BgHidanCurtain),
    (ActorFunc)BgHidanCurtain_Init,
    (ActorFunc)BgHidanCurtain_Destroy,
    (ActorFunc)BgHidanCurtain_Update,
    (ActorFunc)BgHidanCurtain_Draw,
};

extern CollisionCheckInfoInit BgHidanCurtain_sCcInfoInit; // 0x00511608 (see data file)

void BgHidanCurtain_Init(Actor* thisx, PlayState* play) {
    BgHidanCurtain* self = (BgHidanCurtain*)thisx;
    BgHidanCurtainParams* hcParams;

    Actor_SetFocus(&self->actor, 20.0f);
    self->type = ((thisx->params >> 12) & 0xF);
    if (self->type >= 7) {
        Actor_Kill(&self->actor);
        return;
    }

    if ((self->type == 2) || (self->type == 4)) {
        self->size = 1;
    } else {
        self->size = 0;
    }
    hcParams = &BgHidanCurtain_sHCParams[self->size];
    self->treasureFlag = ((thisx->params >> 6) & 0x3F);
    thisx->params &= 0x3F;

    ZARInfo* zarInfo = FUN_00372f38(&self->actor, play, &self->model, 0x37, 0);
    self->model->unk_0C->FUN_00372d94(zarInfo->GetCMABByIndex(0x22));
    self->model->unk_0C->SetAnimType(1);
    self->model->unk_0C->SetAnimSpeed(2.0f);

    Actor_SetScale(&self->actor, hcParams->scale);
    Collider_InitCylinder(play, &self->collider);
    // sCylinderInit lives 0x38 bytes before sHCParams (0x00511610); ROM derives it from the sHCParams literal
    Collider_SetCylinder(play, &self->collider, &self->actor,
                         (ColliderCylinderInit*)((u8*)BgHidanCurtain_sHCParams - 0x38));
    self->collider.dim.pos = self->actor.world.pos;
    self->collider.dim.radius = hcParams->radius;
    self->collider.dim.height = hcParams->height;
    // 3DS-only: Master Quest Fire Temple room 0x1A uses the 3rd params entry
    if (gSaveContext.masterQuestFlag == 1 && play->sceneNum == 6 && self->actor.room == 0x1A) {
        Actor_SetScale(&self->actor, BgHidanCurtain_sHCParams[2].scale);
        self->collider.dim.radius = self->collider.dim.radius * 1.5f;
    }
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetInfo(&thisx->colChkInfo, NULL, &BgHidanCurtain_sCcInfoInit);
    if (self->type == 0) {
        ((BgHidanCurtain*)thisx)->actionFunc = FUN_0010b9d0;
    } else {
        self->actionFunc = FUN_003e8738;
        if ((self->type == 4) || (self->type == 5)) {
            thisx->world.pos.y = thisx->home.pos.y - hcParams->riseDist;
        }
    }
    if (((self->type == 1) && Flags_GetTreasure(play, self->treasureFlag)) ||
        (((self->type == 0) || (self->type == 6)) && Flags_GetClear(play, self->actor.room))) {
        Actor_Kill(&self->actor);
    }
#define FR(n) ((n) > 0 ? (s32)(0.5f + ((n) * 3.0f) * 0.5f) : (s32)(((n) * 3.0f) * 0.5f - 0.5f))
    self->texScroll = FR(Rand_ZeroOne() * 15.0f);
#undef FR
}

void FUN_003d5fd4(BgHidanCurtain* self, PlayState* play) {
    if (self->timer-- == 0) {
        self->actionFunc = FUN_003d9f6c;
    }
}

void FUN_0010b9d0(BgHidanCurtain* self, PlayState* play) {
    if (Flags_GetClear(play, self->actor.room)) {
        self->actionFunc = FUN_003d9f6c;
    }
}

void FUN_0010bdc8(BgHidanCurtain* self, PlayState* play) {
    if (!Flags_GetSwitch(play, self->actor.params)) {
        self->actionFunc = FUN_003ba250;
    }
}

void FUN_003ba250(BgHidanCurtain* self, PlayState* play) {
    f32 riseSpeed = BgHidanCurtain_sHCParams[self->size].riseSpeed;

    if (Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y, riseSpeed)) {
        Flags_UnsetSwitch(play, self->actor.params);
        self->actionFunc = FUN_003e8738;
    }
}

void FUN_003e8738(BgHidanCurtain* self, PlayState* play) {
    if (Flags_GetSwitch(play, self->actor.params)) {
        if (self->type == 1) {
            self->actionFunc = FUN_003d5fd4;
            OnePointCutscene_Init(play, 3350, -99, &self->actor, CAM_ID_MAIN);
            self->timer = 75;
        } else if (self->type == 3) {
            self->actionFunc = FUN_003d5fd4;
            OnePointCutscene_Init(play, 3360, 60, &self->actor, CAM_ID_MAIN);
            self->timer = 45;
        } else {
            self->actionFunc = FUN_003d9f6c;
        }
    }
}

void FUN_003d9f6c(BgHidanCurtain* self, PlayState* play) {
    BgHidanCurtainParams* hcParams = &BgHidanCurtain_sHCParams[self->size];

    if (Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y - hcParams->riseDist, hcParams->riseSpeed)) {
        if ((self->type == 0) || (self->type == 6)) {
            Actor_Kill(&self->actor);
        } else if (self->type == 5) {
            self->actionFunc = FUN_0010bdc8;
        } else {
            if (self->type == 2) {
                self->timer = 600;
            } else if (self->type == 4) {
                self->timer = 300;
            } else if (self->type == 3) {
                self->timer = 240;
            } else { // self->type == 1
                self->timer = 450;
            }
            self->actionFunc = BgHidanCurtain_WaitForTimer;
        }
    }
}

void BgHidanCurtain_Destroy(Actor* thisx, PlayState* play) {
    BgHidanCurtain* self = (BgHidanCurtain*)thisx;

    Collider_DestroyCylinder(play, &self->collider);
    FUN_00350f34(thisx, &self->model, 0);
}

void BgHidanCurtain_Update(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    BgHidanCurtain* self = (BgHidanCurtain*)thisx;
    BgHidanCurtainParams* hcParams = &BgHidanCurtain_sHCParams[self->size];
    f32 riseProgress;

    u8* cam = (u8*)play->cameraPtrs[CAM_ID_MAIN] + 0x100;
    s16 setting = *(s16*)(cam + 0x8A);
    BgHidanCurtain* s2 = self;

    if ((setting == 0x28) || (setting == 0x38)) {
        self->collider.base.atFlags &= ~AT_HIT;
    } else {
        if ((self->collider.base.atFlags & AT_HIT) != 0) {
            self->collider.base.atFlags &= ~AT_HIT;
            FUN_00374bb8(play, &self->actor, 5.0f, self->actor.yawTowardsPlayer, 1.0f);
        }
        if ((s2->type == 4) || (self->type == 5)) {
            self->actor.world.pos.y = (2.0f * self->actor.home.pos.y) - hcParams->riseDist - self->actor.world.pos.y;
        }

        self->actionFunc(self, play);

        if ((self->type == 4) || (self->type == 5)) {
            self->actor.world.pos.y = (2.0f * self->actor.home.pos.y) - hcParams->riseDist - self->actor.world.pos.y;
        }
        riseProgress = (hcParams->riseDist - (self->actor.home.pos.y - self->actor.world.pos.y)) / hcParams->riseDist;
        self->alpha = 255.0f * riseProgress;
        if (self->alpha > 50) {
            self->collider.dim.height = hcParams->height * riseProgress;
            CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
            CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
            if (gSaveContext.sceneSetupIndex < 4) {
                FUN_00373264(&self->actor, 0x010001FA);
            }
        } else if ((self->type == 1) && Flags_GetTreasure(play, self->treasureFlag)) {
            Actor_Kill(&self->actor);
        }
        self->texScroll++;
    }
}

void BgHidanCurtain_Draw(Actor* thisx, PlayState* play) {
    BgHidanCurtain* self = (BgHidanCurtain*)thisx;
    MTX34 mtx;
    f32 speed;

    MTX34CopyAsm(&mtx, &self->actor.modelMtx);
    speed = 2.0f;
    if (PauseContext_GetState() != 0) {
        speed = 0.0f;
    }
    FUN_003695cc(self->model, 0, 4, 0, 1.0f, 0.5f, 0.2f, (f32)self->alpha * (1.0f / 255.0f));
    if (self->model != NULL) {
        self->model->unk_0C->animSpeed = speed;
        self->model->SetUnkAC(1);
        self->model->FUN_003721e0(&mtx);
        self->model->FUN_00372170(0);
    }
}
}
