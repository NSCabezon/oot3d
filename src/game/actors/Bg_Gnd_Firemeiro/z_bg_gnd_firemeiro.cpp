extern "C" {
#include "z_bg_gnd_firemeiro.hpp"

#define FLAGS 0x80400030

#define BOOTS_HOVER 2

void BgGndFiremeiro_Init(Actor* actor, GameState* state);
void BgGndFiremeiro_Destroy(Actor* actor, GameState* state);
void BgGndFiremeiro_Update(Actor* actor, GameState* state);
void BgGndFiremeiro_Draw(Actor* actor, GameState* state);

void FUN_003f5454(BgGndFiremeiro* self, PlayState* play);
void FUN_003ed7e4(BgGndFiremeiro* self, PlayState* play);
void FUN_0012a2e8(BgGndFiremeiro* self, PlayState* play);

s32 FUN_0036adf4(DynaPolyActor* dyna);

ActorInit Bg_Gnd_Firemeiro_InitVars = {
    ACTOR_BG_GND_FIREMEIRO,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_DEMO_KEKKAI,
    sizeof(BgGndFiremeiro),
    (ActorFunc)BgGndFiremeiro_Init,
    (ActorFunc)BgGndFiremeiro_Destroy,
    (ActorFunc)BgGndFiremeiro_Update,
    (ActorFunc)BgGndFiremeiro_Draw,
};

void BgGndFiremeiro_Init(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    BgGndFiremeiro* self = (BgGndFiremeiro*)actor;
    CollisionHeader* colHeader = NULL;

    FUN_00372f38(actor, play, &self->skelAnimModel, 0xf, 0);
    ActorShape_Init(&actor->shape, 0.0f, NULL, 0.0f);
    Actor_SetScale(actor, 0.1f);
    self->initPos = actor->world.pos;

    if (actor->params == 0) {
        DynaPolyActor_Init(&self->dyna, 0);
        ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 3), &colHeader);
        self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
        self->actionFunc = FUN_003f5454;
    }
}

void BgGndFiremeiro_Destroy(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    BgGndFiremeiro* self = (BgGndFiremeiro*)actor;

    if (actor->params == 0) {
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    }

    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void FUN_003f5454(BgGndFiremeiro* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Actor* thisx = &self->dyna.actor;

    if ((*((u8*)player + 0x1A7) != BOOTS_HOVER) && FUN_0036adf4(&self->dyna)) {
        if (thisx->world.pos.y < self->initPos.y) {
            self->actionFunc = FUN_003ed7e4;
            self->timer = 30;
        } else {
            self->actionFunc = FUN_0012a2e8;
            self->timer = 30;
        }
    } else {
        if (thisx->world.pos.y < self->initPos.y) {
            thisx->world.pos.y += 2.0f;
            if (thisx->world.pos.y > self->initPos.y) {
                thisx->world.pos.y = self->initPos.y;
            }
        }
    }
}

void FUN_003ed7e4(BgGndFiremeiro* self, PlayState* play) {
    f32 sunkHeight = self->initPos.y - 150.0f;

    if (FUN_0036adf4(&self->dyna)) {
        self->timer = 15;
    }

    if (self->dyna.actor.world.pos.y > sunkHeight) {
        self->dyna.actor.world.pos.y -= 0.5f;
        if (self->dyna.actor.world.pos.y < sunkHeight) {
            self->dyna.actor.world.pos.y = sunkHeight;
        }
        FUN_0035ae08(&self->dyna.actor, 0x010001d9);
    }

    if (self->timer > 0) {
        self->timer--;
    } else {
        self->actionFunc = FUN_003f5454;
    }
}

void FUN_0012a2e8(BgGndFiremeiro* self, PlayState* play) {
    f32 randSign;

    if (FUN_0036adf4(&self->dyna)) {
        if (self->timer > 0) {
            self->timer--;
            randSign = ((self->timer & 1) ? 2.0f : -2.0f);

            self->dyna.actor.world.pos = self->initPos;
            self->dyna.actor.world.pos.x += randSign * Math_SinS(self->timer * 0x2FFF);
            self->dyna.actor.world.pos.z += randSign * Math_CosS(self->timer * 0x2FFF);
            self->dyna.actor.world.pos.y += Math_CosS(self->timer * 0x7FFF);

            if (!(self->timer & 3)) {
                Audio_PlayActorSound2(&self->dyna.actor, 0x0100018f);
            }
        } else {
            self->timer = 15;
            self->dyna.actor.world.pos = self->initPos;
            self->actionFunc = FUN_003ed7e4;
        }
    } else {
        self->dyna.actor.world.pos = self->initPos;
        self->actionFunc = FUN_003f5454;
    }
}

void BgGndFiremeiro_Update(Actor* actor, GameState* state) {
    BgGndFiremeiro* self = (BgGndFiremeiro*)actor;
    PlayState* play = (PlayState*)state;

    self->actionFunc(self, play);
}

void BgGndFiremeiro_Draw(Actor* actor, GameState* state) {
    BgGndFiremeiro* self = (BgGndFiremeiro*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);

    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}
}
