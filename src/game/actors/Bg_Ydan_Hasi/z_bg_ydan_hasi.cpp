extern "C" {
#include "z_bg_ydan_hasi.hpp"

#define FLAGS 0x80400030

#include <math.h>

void BgYdanHasi_Init(Actor* actor, GameState* state);
void BgYdanHasi_Destroy(Actor* thisx, PlayState* play);
void BgYdanHasi_InitWater(BgYdanHasi* self, PlayState* play);
void BgYdanHasi_MoveWater(BgYdanHasi* self, PlayState* play);
void BgYdanHasi_DecWaterTimer(BgYdanHasi* self, PlayState* play);
void BgYdanHasi_SetupThreeBlocks(BgYdanHasi* self, PlayState* play);
void BgYdanHasi_UpdateThreeBlocks(BgYdanHasi* self, PlayState* play);
void BgYdanHasi_UpdateFloatingBlock(BgYdanHasi* self, PlayState* play);
void BgYdanHasi_Update(Actor* thisx, PlayState* play);
void BgYdanHasi_Draw(Actor* thisx, PlayState* play);

ActorInit Bg_Ydan_Hasi_InitVars = {
    ACTOR_BG_YDAN_HASI,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_YDAN_OBJECTS,
    sizeof(BgYdanHasi),
    (ActorFunc)BgYdanHasi_Init,
    (ActorFunc)BgYdanHasi_Destroy,
    (ActorFunc)BgYdanHasi_Update,
    (ActorFunc)BgYdanHasi_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgYdanHasi_Init(Actor* actor, GameState* state) {
    BgYdanHasi* self = (BgYdanHasi*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;
    ZARInfo* zar;
    BgYdanHasiWaterBox* waterBox;

    Actor_ProcessInitChain(actor, sInitChain);
    self->type = (actor->params >> 8) & 0x3F;
    actor->params = actor->params & 0xFF;
    actor->unk_19A = 1;
    {
        u8 modelIdx[4] = { 0x0A, 0x04, 0x01, 0x00 };

        FUN_00372f38(actor, play, &self->model, modelIdx[actor->params], 0);
    }
    zar = &play->GetObjectStatus(actor->objBankIndex)->zarInfo;
    waterBox = &((BgYdanHasiWaterBox*)play->colCtx.stat.colHeader->waterBoxes)[1];
    DynaPolyActor_Init(&self->dyna, 1);
    if (actor->params == 1) {
        // Water the moving platform floats on in B1
        waterBox->ySurface = actor->world.pos.y = actor->home.pos.y += -5.0f;
        SkeletonAnimationModel_unk_0C* anim = self->model->unk_0C;
        anim->FUN_00372d94(zar->GetCMABByIndex(0));
        anim->SetAnimType(1);
        self->actionFunc = BgYdanHasi_InitWater;
    } else {
        if (actor->params == 0) {
            // Moving platform on the water in B1
            ReadCollisionHeaderFromZsi(zar->FUN_003532c0(4), &colHeader);
            actor->scale.z = 0.15f;
            actor->scale.x = 0.15f;
            actor->world.pos.y = waterBox->ySurface + 20.0f;
            self->actionFunc = BgYdanHasi_UpdateFloatingBlock;
        } else {
            // 3 platforms on 2F
            ReadCollisionHeaderFromZsi(zar->FUN_003532c0(0), &colHeader);
            self->actionFunc = BgYdanHasi_SetupThreeBlocks;
            actor->draw = NULL;
            Actor_SetFocus(actor, 40.0f);
        }
        self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    }
    self->timer = 0;
}

void BgYdanHasi_Destroy(Actor* thisx, PlayState* play) {
    BgYdanHasi* self = (BgYdanHasi*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(thisx, &self->model, 0);
}

void BgYdanHasi_Update(Actor* thisx, PlayState* play) {
    BgYdanHasi* self = (BgYdanHasi*)thisx;

    self->actionFunc(self, play);
    self->unk_1C6++;
}

// The early model test and the params/unk_0C reads go through the (BgYdanHasi*)thisx cast while the
// later calls use `self`: that split keeps thisx in r0 early and the model in r1.
void BgYdanHasi_Draw(Actor* thisx, PlayState* play) {
    BgYdanHasi* self = (BgYdanHasi*)thisx;

    if (((BgYdanHasi*)thisx)->model != NULL) {
        if (thisx->params == 1) {
            SkeletonAnimationModel_unk_0C* anim = ((BgYdanHasi*)thisx)->model->unk_0C;

            anim->animSpeed = (PauseContext_GetState() != 0) ? 0.0f : 1.0f;
        }
        self->model->FUN_003721e0(&self->dyna.actor.modelMtx);
        self->model->SetUnkAC(1);
        self->model->FUN_00372170(0);
    }
}

void FUN_0036d940(Actor* actor, s32 timer);

void BgYdanHasi_DecWaterTimer(BgYdanHasi* self, PlayState* play) {
    if (self->timer != 0) {
        self->timer--;
    }
    FUN_0036d940(&self->dyna.actor, self->timer);
    if (self->timer == 0) {
        self->actionFunc = BgYdanHasi_MoveWater;
    }
}

void BgYdanHasi_InitWater(BgYdanHasi* self, PlayState* play) {
    if (Flags_GetSwitch(play, self->type)) {
        self->timer = 900;
        self->actionFunc = BgYdanHasi_MoveWater;
    }
}

void BgYdanHasi_MoveWater(BgYdanHasi* self, PlayState* play) {
    BgYdanHasiWaterBox* waterBox;

    if (self->timer == 0) {
        if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y, 1.0f) != 0) {
            Flags_UnsetSwitch(play, self->type);
            self->actionFunc = BgYdanHasi_InitWater;
        }
        FUN_0035ae08(&self->dyna.actor, 0x010001B5);
    } else {
        if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y - 47.0f, 0.5f)) {
            self->actionFunc = BgYdanHasi_DecWaterTimer;
        }
        FUN_0035ae08(&self->dyna.actor, 0x010001B5);
    }
    waterBox = (BgYdanHasiWaterBox*)play->colCtx.stat.colHeader->waterBoxes;
    waterBox[1].ySurface = self->dyna.actor.world.pos.y;
}

s32 OnePointCutscene_Init(PlayState* play, s32 csId, s32 arg2, Actor* actor, s32 camId);

void BgYdanHasi_SetupThreeBlocks(BgYdanHasi* self, PlayState* play) {
    if (Flags_GetSwitch(play, self->type)) {
        self->timer = 390;
        self->dyna.actor.draw = (ActorFunc)BgYdanHasi_Draw;
        self->actionFunc = BgYdanHasi_UpdateThreeBlocks;
        OnePointCutscene_Init(play, 3040, 30, &self->dyna.actor, 0);
    }
}

void BgYdanHasi_UpdateFloatingBlock(BgYdanHasi* self, PlayState* play) {
    BgYdanHasiWaterBox* waterBox;
    f32 framesAfterMath;

    framesAfterMath = sinf((self->unk_1C6 % 383) * (M_PI / 192)) * 165.0f;
    self->dyna.actor.world.pos.x =
        (framesAfterMath * Math_SinS(self->dyna.actor.world.rot.y)) + self->dyna.actor.home.pos.x;
    self->dyna.actor.world.pos.z =
        (framesAfterMath * Math_CosS(self->dyna.actor.world.rot.y)) + self->dyna.actor.home.pos.z;
    waterBox = &((BgYdanHasiWaterBox*)play->colCtx.stat.colHeader->waterBoxes)[1];
    self->dyna.actor.world.pos.y = waterBox->ySurface + 20.0f;
    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer == 0) {
        self->timer = 75;
    }
    self->dyna.actor.world.pos.y += 2.0f * sinf(self->timer * (M_PI / 37.5));
}

void BgYdanHasi_UpdateThreeBlocks(BgYdanHasi* self, PlayState* play) {
    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer == 0) {
        if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y, 3.0f) != 0) {
            Flags_UnsetSwitch(play, self->type);
            self->dyna.actor.draw = NULL;
            self->actionFunc = BgYdanHasi_SetupThreeBlocks;
        } else {
            FUN_0035ae08(&self->dyna.actor, 0x0100017B);
        }
    } else if (!Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y + 120.0f, 3.0f)) {
        FUN_0035ae08(&self->dyna.actor, 0x0100017B);
    } else {
        FUN_0036d940(&self->dyna.actor, self->timer);
    }
}

}
