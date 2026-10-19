extern "C" {
#include "z_bg_ddan_jd.hpp"

#define FLAGS 0x80400030

#define MOVE_HEIGHT_MIDDLE 140.0f
#define MOVE_HEIGHT_TOP 700.0f

#define IDLE_FRAMES 150 // N64: 100, scaled by R_UPDATE_RATE (3DS)

// Since ySpeed is used to determine if the platform should rise to the top of the dungeon, these must be assigned
// different values in order for the shortcut to work correctly
#define DEFAULT_Y_SPEED 1
#define SHORTCUT_Y_SPEED 5

typedef enum BgDdanJdState {
    /* 0 */ STATE_GO_BOTTOM,
    /* 1 */ STATE_GO_MIDDLE_FROM_BOTTOM,
    /* 2 */ STATE_GO_MIDDLE_FROM_TOP,
    /* 3 */ STATE_GO_TOP
} BgDdanJdState;

void BgDdanJd_Init(Actor* actor, GameState* state);
void BgDdanJd_Destroy(Actor* actor, GameState* state);
void BgDdanJd_Update(Actor* actor, GameState* state);
void BgDdanJd_Draw(Actor* actor, GameState* state);

void FUN_003e4bc4(BgDdanJd* self, PlayState* play);
void FUN_003de6f8(BgDdanJd* self, PlayState* play);

void OnePointCutscene_Init(PlayState* play, s16 csId, s16 arg2, Actor* actor, s16 camId);

ActorInit Bg_Ddan_Jd_InitVars = {
    ACTOR_BG_DDAN_JD,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_DDAN_OBJECTS,
    sizeof(BgDdanJd),
    (ActorFunc)BgDdanJd_Init,
    (ActorFunc)BgDdanJd_Destroy,
    (ActorFunc)BgDdanJd_Update,
    (ActorFunc)BgDdanJd_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgDdanJd_Init(Actor* actor, GameState* state) {
    BgDdanJd* self = (BgDdanJd*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, 1);
    FUN_00372f38(actor, play, &self->skelAnimModel, 3, 0);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 1), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    self->idleTimer = IDLE_FRAMES;
    self->state = STATE_GO_BOTTOM;

    // Missing check for actor.params < 0x40. This will cause inconsistent behavior if params >= 0x40 and the bound
    // switch state is turned on while in the same room, as the shortcut behavior won't become enabled until the actor
    // is reloaded.
    if (Flags_GetSwitch(play, actor->params)) {
        self->ySpeed = SHORTCUT_Y_SPEED;
    } else {
        self->ySpeed = DEFAULT_Y_SPEED;
    }
    self->actionFunc = FUN_003e4bc4;
}

void BgDdanJd_Destroy(Actor* actor, GameState* state) {
    BgDdanJd* self = (BgDdanJd*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

// Implements the platform's movement state, including dust particles and sfx
void FUN_003de6f8(BgDdanJd* self, PlayState* play) {
    Vec3f dustPos;

    // if this is the platform that rises all the way to the top, and the switch state has just changed to on
    if (self->ySpeed == DEFAULT_Y_SPEED && self->dyna.actor.params < 0x40 &&
        Flags_GetSwitch(play, self->dyna.actor.params)) {
        self->ySpeed = SHORTCUT_Y_SPEED;
        self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y + MOVE_HEIGHT_MIDDLE;
        self->state = STATE_GO_MIDDLE_FROM_BOTTOM;
        self->idleTimer = 0;
        self->actionFunc = FUN_003e4bc4;
        OnePointCutscene_Init(play, 3060, -99, &self->dyna.actor, 0);
    } else if (Math_StepToF(&self->dyna.actor.world.pos.y, self->targetY, self->ySpeed)) {
        Audio_PlayActorSound2(&self->dyna.actor, 0x01000248); // NA_SE_EV_PILLAR_MOVE_STOP
        self->actionFunc = FUN_003e4bc4;
    }

    // Generate random dust particles at the platform's base.
    dustPos.y = self->dyna.actor.home.pos.y;
    if (*(u32*)((u8*)play + 0x5BF4) & 1) { // gameplayFrames
        dustPos.x = self->dyna.actor.world.pos.x + 65.0f;
        dustPos.z = Rand_CenteredFloat(110.0f) + self->dyna.actor.world.pos.z;
        FUN_0037378c(play, &dustPos, 5.0f, 1, 20, 60, 1);
        dustPos.x = self->dyna.actor.world.pos.x - 65.0f;
        dustPos.z = Rand_CenteredFloat(110.0f) + self->dyna.actor.world.pos.z;
        FUN_0037378c(play, &dustPos, 5.0f, 1, 20, 60, 1);
    } else {
        dustPos.x = Rand_CenteredFloat(110.0f) + self->dyna.actor.world.pos.x;
        dustPos.z = self->dyna.actor.world.pos.z + 65.0f;
        FUN_0037378c(play, &dustPos, 5.0f, 1, 20, 60, 1);
        dustPos.x = Rand_CenteredFloat(110.0f) + self->dyna.actor.world.pos.x;
        dustPos.z = self->dyna.actor.world.pos.z - 65.0f;
        FUN_0037378c(play, &dustPos, 5.0f, 1, 20, 60, 1);
    }
    if (self->ySpeed == SHORTCUT_Y_SPEED) {
        FUN_00373264(&self->dyna.actor, 0x0100017b); // NA_SE_EV_ELEVATOR_MOVE
    }
}

void FUN_003e4bc4(BgDdanJd* self, PlayState* play) {
    if (self->idleTimer != 0) {
        self->idleTimer--;
    }

    // if this is the platform that rises all the way to the top, and the switch state has just changed to on
    if (self->ySpeed == DEFAULT_Y_SPEED && self->dyna.actor.params < 0x40 &&
        Flags_GetSwitch(play, self->dyna.actor.params)) {
        self->ySpeed = SHORTCUT_Y_SPEED;
        self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y + MOVE_HEIGHT_MIDDLE;
        self->state = STATE_GO_MIDDLE_FROM_BOTTOM;
        self->idleTimer = 0;
        OnePointCutscene_Init(play, 3060, -99, &self->dyna.actor, 0);
    }
    if (self->idleTimer == 0) {
        self->idleTimer = IDLE_FRAMES;
        if (self->state == STATE_GO_BOTTOM) {
            self->targetY = self->dyna.actor.home.pos.y + MOVE_HEIGHT_MIDDLE;
            self->state = STATE_GO_MIDDLE_FROM_BOTTOM;
        } else if (self->state == STATE_GO_MIDDLE_FROM_BOTTOM) {
            if (self->ySpeed != DEFAULT_Y_SPEED) {
                self->targetY = self->dyna.actor.home.pos.y + MOVE_HEIGHT_TOP;
                self->state = STATE_GO_TOP;
            } else {
                self->targetY = self->dyna.actor.home.pos.y;
                self->state = STATE_GO_BOTTOM;
            }
        } else if (self->state == STATE_GO_MIDDLE_FROM_TOP) {
            if (self->ySpeed != DEFAULT_Y_SPEED) {
                self->targetY = self->dyna.actor.home.pos.y + MOVE_HEIGHT_TOP;
                self->state = STATE_GO_TOP;
            } else {
                self->targetY = self->dyna.actor.home.pos.y;
                self->state = STATE_GO_BOTTOM;
            }
        } else if (self->state == STATE_GO_TOP) {
            self->targetY = self->dyna.actor.home.pos.y + MOVE_HEIGHT_MIDDLE;
            self->state = STATE_GO_MIDDLE_FROM_TOP;
        }
        self->actionFunc = FUN_003de6f8;
    }
}

void BgDdanJd_Update(Actor* actor, GameState* state) {
    BgDdanJd* self = (BgDdanJd*)actor;

    self->actionFunc(self, (PlayState*)state);
}

void BgDdanJd_Draw(Actor* actor, GameState* state) {
    BgDdanJd* self = (BgDdanJd*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->dyna.actor.modelMtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}
}
