extern "C" {
#include "z_en_ms.hpp"

#define FLAGS 0x00000009

void EnMs_Init(Actor* actor, GameState* state);
void EnMs_Destroy(Actor* actor, GameState* state);
void EnMs_Update(Actor* actor, GameState* state);
void EnMs_Draw(Actor* actor, GameState* state);
void EnMs_Wait(EnMs* self, PlayState* play);
void EnMs_Talk(EnMs* self, PlayState* play);
void EnMs_Sell(EnMs* self, PlayState* play);
void EnMs_TalkAfterPurchase(EnMs* self, PlayState* play);

GLOBAL_ASM("data/z_En_Ms.data.s")

GLOBAL_ASM("../src/game/actors/En_Ms/data_syms.s")

extern s16 EnMs_sPrices[];
extern u16 EnMs_sOfferTextIDs[];
void Rupees_ChangeBy(s16 rupeeChange);
void Collider_SetCylinderType1(PlayState* play, ColliderCylinder* collider, Actor* actor, ColliderCylinderInit* init);

ActorInit En_Ms_InitVars = {
    ACTOR_EN_MS,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_MS,
    sizeof(EnMs),
    (ActorFunc)EnMs_Init,
    (ActorFunc)EnMs_Destroy,
    (ActorFunc)EnMs_Update,
    (ActorFunc)EnMs_Draw,
};

extern InitChainEntry EnMs_sInitChain[];
extern ColliderCylinderInit EnMs_sCylinderInit;

static inline void EnMs_SetOfferText(EnMs* self, PlayState* play) {
    self->actor.textId = Text_GetFaceReaction(play, 0x1B);
    if (self->actor.textId == 0) {
        if (BEANS_BOUGHT >= 10) {
            self->actor.textId = 0x406B;
        } else {
            self->actor.textId = EnMs_sOfferTextIDs[BEANS_BOUGHT];
        }
    }
}

void EnMs_Init(Actor* actor, GameState* state) {
    EnMs* self = (EnMs*)actor;
    PlayState* play = (PlayState*)state;

    if (LINK_AGE_IN_YEARS != YEARS_CHILD) {
        Actor_Kill(actor);
        return;
    }
    Actor_ProcessInitChain(actor, EnMs_sInitChain);
    FUN_00372f38(actor, play, &self->skelAnimModel, 0, 0);
    FUN_00353c9c(actor, play, &self->skelAnime, 0, 0, self->jointTable, self->morphTable, 12);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinderType1(play, &self->collider, actor, &EnMs_sCylinderInit);
    ActorShape_Init(&actor->shape, 0.0f, ActorShadow_DrawCircle, 35.0f);
    Actor_SetScale(actor, 0.015f);
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    self->actor.speedXZ = 0.0f;
    self->actor.gravity = -1.0f;
    self->actor.velocity.y = 0.0f;
    EnMs_SetOfferText(self, play);
    self->actionFunc = EnMs_Wait;
}

void EnMs_Destroy(Actor* actor, GameState* state) {
    EnMs* self = (EnMs*)actor;
    PlayState* play = (PlayState*)state;

    Collider_DestroyCylinder(play, &self->collider);
    FUN_003508b8(actor, self->skelAnimModel, 0);
    SkelAnime_Free2(&self->skelAnime);
}

extern f32 EnMs_sFocusHeight;

void EnMs_Update(Actor* actor, GameState* state) {
    EnMs* self = (EnMs*)actor;
    PlayState* play = (PlayState*)state;

    self->activeTimer = self->activeTimer + 1;
    Actor_SetFocus(&self->actor, EnMs_sFocusHeight);
    self->actor.targetArrowOffset = 500.0f;
    Actor_SetScale(&self->actor, 0.015f);
    FUN_003731e0(&self->skelAnime);
    self->actionFunc(self, play);

    if (gSaveContext.entranceIndex == 0x157 && gSaveContext.sceneSetupIndex == 8) {
        Actor_MoveForward(&self->actor);
        Actor_UpdateBgCheckInfo((PlayState*)state, &self->actor, 0.0f, 0.0f, 0.0f, 4);
    }
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
}

void EnMs_Draw(Actor* actor, GameState* state) {
    EnMs* self = (EnMs*)actor;
    PlayState* play = (PlayState*)state;

    GraphicsContext_UnknownStub(play->state.gfxCtx);
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, NULL, NULL, actor, 0);
}

void EnMs_TalkAfterPurchase(EnMs* self, PlayState* play);

void EnMs_Sell(EnMs* self, PlayState* play) {
    if (Actor_HasParent(&self->actor, play)) {
        Rupees_ChangeBy(-EnMs_sPrices[BEANS_BOUGHT]);
        self->actor.parent = NULL;
        self->actionFunc = EnMs_TalkAfterPurchase;
    } else {
        GiveItem(&self->actor, play, 0x16, 90.0f, 10.0f);
    }
}

void EnMs_Talk(EnMs* self, PlayState* play) {
    u8 dialogState;
    Actor* thisx = &self->actor;

    dialogState = Message_GetState(&play->msgCtx);
    if (dialogState != 4) {
        if ((dialogState == 6) && Message_ShouldAdvance(play)) {
            self->actionFunc = EnMs_Wait;
        }
    } else if (Message_ShouldAdvance(play)) {
        switch (FUN_00369f3c(play)) {
            case 0:
                if (gSaveContext.rupees < EnMs_sPrices[BEANS_BOUGHT]) {
                    Message_ContinueTextbox(play, 0x4069);
                    return;
                }
                GiveItem(thisx, play, 0x16, 90.0f, 10.0f);
                self->actionFunc = EnMs_Sell;
                return;
            case 1:
                Message_ContinueTextbox(play, 0x4068);
            default:
                return;
        }
    }
}

void EnMs_TalkAfterPurchase(EnMs* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 6 && Message_ShouldAdvance(play)) {
        Message_ContinueTextbox(play, 0x406C);
        self->actionFunc = EnMs_Talk;
    }
}

void EnMs_Wait(EnMs* self, PlayState* play) {
    s16 yawDiff;

    yawDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
    EnMs_SetOfferText(self, play);

    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        self->actionFunc = EnMs_Talk;
    } else if ((self->actor.xzDistToPlayer < 90.0f) && (ABS(yawDiff) < 0x2000)) {
        FUN_0036bb28(&self->actor, play, 90.0f);
    }
}

}
