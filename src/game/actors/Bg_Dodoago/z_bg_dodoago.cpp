extern "C" {
#include "z_bg_dodoago.hpp"
#include "../En_Bom/z_en_bom.hpp"

#define FLAGS 0x00400030

// Static data lives in the GLOBAL_ASM dump below (0x00510DE0..0x00510E70). Raw addresses until it gets symbols.
#define sDisableBombCatcher (*(u8*)0x00510DE0)
#define sFirstExplosiveFlag (*(s16*)0x00510DE2)
#define sTimer (*(s32*)0x00510DE4)
#define sMainColliderCylinderInit ((ColliderCylinderInit*)0x00510DF0)
#define sLeftRightColliderCylinderInit ((ColliderCylinderInit*)0x00510E28)
#define sInitChain ((InitChainEntry*)0x00510E60)

GLOBAL_ASM("data/z_Bg_Dodoago.data.s")

// roomCtx.unk_7C0 is an s16 array on 3DS (N64 roomCtx.drawParams)
#define DRAW_PARAMS(play) ((s16*)&(play)->roomCtx.unk_7C0)
#define PARAMS_GET_U(params, shift, count) ((((u16)(params)) >> (shift)) & ((1 << (count)) - 1))
#define EVENTCHKINF_B0 0xB0
#define R_UPDATE_RATE SREG(30)
#define FRAMES(n) ((n) * 3.0f / R_UPDATE_RATE + 0.5f)

void BgDodoago_Init(Actor* thisx, GameState* state);
void BgDodoago_Destroy(Actor* thisx, GameState* state);
void BgDodoago_Update(Actor* thisx, GameState* state);
void BgDodoago_Draw(Actor* thisx, GameState* state);

void FUN_003f2fc4(BgDodoago* self, PlayState* play);

ActorInit Bg_Dodoago_InitVars = {
    ACTOR_BG_DODOAGO,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_DDAN_OBJECTS,
    sizeof(BgDodoago),
    (ActorFunc)BgDodoago_Init,
    (ActorFunc)BgDodoago_Destroy,
    (ActorFunc)BgDodoago_Update,
    (ActorFunc)BgDodoago_Draw,
};


// N64 BgDodoago_DoNothing: empty action (3DS `bx lr`)
void FUN_001832a8(BgDodoago* self, PlayState* play) {
}

#if 0 // NON_MATCHING draft: BgDodoago_Init (differs only in stack frame/literal reuse: 3DS has no STACK_PAD and raw data refs change regalloc)
void BgDodoago_Init(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    BgDodoago* self = (BgDodoago*)thisx;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, 0);
    FUN_00372f38(thisx, play, &self->skelAnimModel, 2, NULL);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 0), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);
    ActorShape_Init(&self->dyna.actor.shape, 0.0f, NULL, 0.0f);

    if (Flags_GetSwitch(play, PARAMS_GET_U(self->dyna.actor.params, 0, 6))) {
        self->actionFunc = FUN_001832a8;
        self->dyna.actor.shape.rot.x = 0x1333;
        DRAW_PARAMS(play)[BGDODOAGO_EYE_LEFT] = DRAW_PARAMS(play)[BGDODOAGO_EYE_RIGHT] = 255;
        return;
    }

    Collider_InitCylinder(play, &self->mainCollider);
    Collider_InitCylinder(play, &self->leftCollider);
    Collider_InitCylinder(play, &self->rightCollider);
    Collider_SetCylinder(play, &self->mainCollider, &self->dyna.actor, sMainColliderCylinderInit);
    Collider_SetCylinder(play, &self->leftCollider, &self->dyna.actor, sLeftRightColliderCylinderInit);
    Collider_SetCylinder(play, &self->rightCollider, &self->dyna.actor, sLeftRightColliderCylinderInit);

    self->actionFunc = FUN_003f2fc4;
    sDisableBombCatcher = false;
}

#endif
GLOBAL_ASM("asm/BgDodoago_Init.s")

void BgDodoago_Destroy(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    BgDodoago* self = (BgDodoago*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyCylinder(play, &self->mainCollider);
    Collider_DestroyCylinder(play, &self->leftCollider);
    Collider_DestroyCylinder(play, &self->rightCollider);
    FUN_00350f34(thisx, &self->skelAnimModel, 0);
}

#if 0 // NON_MATCHING draft: BgDodoago_Update (3DS build differs in register allocation / bomb timer store)
void BgDodoago_Update(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    BgDodoago* self = (BgDodoago*)thisx;
    Actor* actor;
    EnBom* bomb;

    if (self->dyna.actor.parent == NULL) {
        // this is a "bomb catcher", it kills the XZ speed and sets the timer for bombs that are dropped through the
        // holes in the bridge above the skull
        if ((self->leftCollider.base.ocFlags1 & OC1_HIT) || (self->rightCollider.base.ocFlags1 & OC1_HIT)) {

            if (self->leftCollider.base.ocFlags1 & OC1_HIT) {
                actor = self->leftCollider.base.oc;
            } else {
                actor = self->rightCollider.base.oc;
            }
            self->leftCollider.base.ocFlags1 &= ~OC1_HIT;
            self->rightCollider.base.ocFlags1 &= ~OC1_HIT;

            if (actor->type == ACTORCAT_EXPLOSIVES && actor->id == ACTOR_EN_BOM && actor->params == 0) {
                bomb = (EnBom*)actor;
                // disable the bomb catcher for a few seconds
                self->dyna.actor.parent = &bomb->actor;
                *(s16*)((u8*)bomb + 0x26C) = FRAMES(50);
                bomb->actor.speedXZ = 0.0f;
                sTimer = 0;
            }
        }
    } else {
        sTimer++;
        Flags_GetSwitch(play, PARAMS_GET_U(self->dyna.actor.params, 0, 6));
        if (!sDisableBombCatcher && sTimer > 140) {
            if (Flags_GetSwitch(play, PARAMS_GET_U(self->dyna.actor.params, 0, 6))) {
                // this prevents clearing the actor's parent pointer, effectively disabling the bomb catcher
                sDisableBombCatcher++;
            } else {
                self->dyna.actor.parent = NULL;
            }
        }
    }
    self->actionFunc(self, play);
}

#endif
GLOBAL_ASM("asm/BgDodoago_Update.s")

void BgDodoago_Draw(Actor* thisx, GameState* state) {
    BgDodoago* self = (BgDodoago*)thisx;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &thisx->modelMtx);
    if (Flags_GetEventChkInf(EVENTCHKINF_B0) && self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}

}
