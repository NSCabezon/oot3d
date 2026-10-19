extern "C" {
#include "z_bg_jya_1flift.hpp"

#define FLAGS 0x80400010
#define PARAMS_GET_U(params, start, count) (((params) >> (start)) & ((1 << (count)) - 1))

float fabsf(float arg);

// Declarations not in include/ yet (3DS addresses)
s32 FUN_0036adf4(DynaPolyActor* dyna); // DynaPolyActor_IsPlayerOnTop
void Camera_ChangeSetting(Camera* camera, s32 setting);

void BgJya1flift_Init(Actor* actor, GameState* state);
void BgJya1flift_Destroy(Actor* actor, GameState* state);
void BgJya1flift_Update(Actor* actor, GameState* state);
void BgJya1flift_Draw(Actor* actor, GameState* state);
void BgJya1flift_DoNothing(BgJya1flift* self, PlayState* play);
void BgJya1flift_Move(BgJya1flift* self, PlayState* play);
void BgJya1flift_DelayMove(BgJya1flift* self, PlayState* play);
void FUN_003c0a58(BgJya1flift* self, PlayState* play); // WaitForSwitch
void FUN_00185384(BgJya1flift* self); // ChangeDirection

GLOBAL_ASM("data/z_Bg_Jya_1flift.data.s")
GLOBAL_ASM("../src/game/actors/Bg_Jya_1flift/data_syms.s")

extern u8 BgJya1flift_sIsSpawned;
extern f32 BgJya1flift_sFinalPositions[];
extern ColliderCylinderInit BgJya1flift_sCylinderInit;
extern InitChainEntry BgJya1flift_sInitChain[];

ActorInit Bg_Jya_1flift_InitVars = {
    ACTOR_BG_JYA_1FLIFT,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_JYA_OBJ,
    sizeof(BgJya1flift),
    (ActorFunc)BgJya1flift_Init,
    (ActorFunc)BgJya1flift_Destroy,
    (ActorFunc)BgJya1flift_Update,
    (ActorFunc)BgJya1flift_Draw,
};

static inline void BgJya1flift_InitDynapoly(BgJya1flift* self, PlayState* play, void* collision, s32 moveFlag) {
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, moveFlag);
    ReadCollisionHeaderFromZsi(collision, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);
}

void BgJya1flift_Init(Actor* thisx, GameState* state) {
    BgJya1flift* self = (BgJya1flift*)thisx;
    PlayState* play = (PlayState*)state;

    self->hasInitialized = false;
    if (BgJya1flift_sIsSpawned) {
        Actor_Kill(thisx);
        return;
    }
    FUN_00372f38(thisx, play, &self->skelAnimModel, 0, 0);
    BgJya1flift_InitDynapoly(self, play, DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 0), 1);
    Actor_ProcessInitChain(thisx, BgJya1flift_sInitChain);
    BgJya1flift* s2 = self;
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &s2->collider, &s2->dyna.actor, &BgJya1flift_sCylinderInit);
    self->dyna.actor.colChkInfo.mass = MASS_IMMOVABLE;
    if (Flags_GetSwitch(play, PARAMS_GET_U(thisx->params, 0, 6))) {
        if (LINK_AGE_IN_YEARS == YEARS_ADULT) {
            self->actionFunc = BgJya1flift_Move;
            self->isMovingDown ^= true;
            self->dyna.actor.velocity.y = 0.0f;
        } else {
            self->actionFunc = BgJya1flift_DoNothing;
            self->dyna.actor.world.pos.y = BgJya1flift_sFinalPositions[0];
        }
    } else {
        self->actionFunc = FUN_003c0a58;
        self->dyna.actor.world.pos.y = BgJya1flift_sFinalPositions[0];
    }
    thisx->room = -1;
    BgJya1flift_sIsSpawned = true;
    self->hasInitialized = true;
}


void BgJya1flift_Destroy(Actor* actor, GameState* state) {
    BgJya1flift* self = (BgJya1flift*)actor;
    PlayState* play = (PlayState*)state;

    if (self->hasInitialized) {
        BgJya1flift_sIsSpawned = false;
        Collider_DestroyCylinder(play, &self->collider);
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    }
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgJya1flift_DoNothing(BgJya1flift* self, PlayState* play) {
}

void BgJya1flift_Move(BgJya1flift* self, PlayState* play) {
    f32 tempVelocity;

    Math_StepToF(&self->dyna.actor.velocity.y, 6.0f, 0.4f);
    if (self->dyna.actor.velocity.y < 1.0f) {
        tempVelocity = 1.0f;
    } else {
        tempVelocity = self->dyna.actor.velocity.y;
    }
    if (fabsf(Math_SmoothStepToF(&self->dyna.actor.world.pos.y, BgJya1flift_sFinalPositions[self->isMovingDown], 0.5f,
                                 tempVelocity, 1.0f)) < 0.001f) {
        self->dyna.actor.world.pos.y = BgJya1flift_sFinalPositions[self->isMovingDown];
        self->actionFunc = BgJya1flift_DelayMove;
        self->moveDelay = 0;
        Audio_PlayActorSound2(&self->dyna.actor, 0x0100018C);
    } else {
        FUN_00373264(&self->dyna.actor, 0x01000210);
    }
}

void BgJya1flift_DelayMove(BgJya1flift* self, PlayState* play) {
    self->moveDelay++;
    if (self->moveDelay > 20) {
        FUN_00185384(self);
    }
}

// WaitForSwitch. Name pinned by the link map (was GLOBAL_ASM in functions_3C0000s.cpp).
// ChangeDirection is written out inline here, as in the ROM.
void FUN_003c0a58(BgJya1flift* self, PlayState* play) {
    if (Flags_GetSwitch(play, PARAMS_GET_U(self->dyna.actor.params, 0, 6))) {
        self->actionFunc = BgJya1flift_Move;
        self->isMovingDown ^= true;
        self->dyna.actor.velocity.y = 0.0f;
    }
}

void BgJya1flift_Update(Actor* actor, GameState* state) {
    BgJya1flift* self = (BgJya1flift*)actor;
    PlayState* play = (PlayState*)state;
    s32 tempIsRiding;
    s8 roomNum = *(s8*)((u8*)play + 0x4C30);
    if (roomNum == 6 || roomNum == 0) {
        self->actionFunc(self, play);
        tempIsRiding = FUN_0036adf4(&self->dyna) ? true : false;
        if ((self->actionFunc == BgJya1flift_Move) || (self->actionFunc == BgJya1flift_DelayMove)) {
            if (tempIsRiding) {
                Camera_ChangeSetting(play->cameraPtrs[0], 0x30);
            } else if (!tempIsRiding && self->isLinkRiding) {
                Camera_ChangeSetting(play->cameraPtrs[0], 3);
            }
        }
        self->isLinkRiding = tempIsRiding;
        Collider_UpdateCylinder(actor, &self->collider);
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    } else {
        Actor_Kill(actor);
    }
}

void BgJya1flift_Draw(Actor* actor, GameState* state) {
    BgJya1flift* self = (BgJya1flift*)actor;
    PlayState* play = (PlayState*)state;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->dyna.actor.modelMtx);
    if (self->skelAnimModel != NULL) {
        FUN_00357fd0(GET_PLAYER(play), self->dyna.actor.unk_178, &self->dyna.actor.world.pos);
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}

}
