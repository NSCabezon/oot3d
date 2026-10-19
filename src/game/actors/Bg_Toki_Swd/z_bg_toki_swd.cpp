extern "C" {
#include "z_bg_toki_swd.hpp"

#define FLAGS 0x80000010

void BgTokiSwd_Init(Actor* actor, GameState* state);
void BgTokiSwd_Destroy(Actor* actor, GameState* state);
void BgTokiSwd_Update(Actor* actor, GameState* state);
void BgTokiSwd_Draw(Actor* actor, GameState* state);
void FUN_003e5dc8(BgTokiSwd* self, PlayState* play);
void FUN_00174d90(BgTokiSwd* self, PlayState* play);
void FUN_0013f400(BgTokiSwd* self, PlayState* play);

s32 Gameplay_InCsMode(PlayState* play);
s32 Item_Give(PlayState* play, s32 item);
s32 Flags_GetEnv(PlayState* play, s32 flag);
void FUN_0034df30(Actor* actor, PlayState* play); // Actor_OfferCarry

// roomCtx.unk_7C0 is an s16 on 3DS (N64 roomCtx.drawParams[0])
#define DRAW_PARAM0(play) (*(s16*)&(play)->roomCtx.unk_7C0)

ActorInit Bg_Toki_Swd_InitVars = {
    ACTOR_BG_TOKI_SWD,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_TOKI_OBJECTS,
    sizeof(BgTokiSwd),
    (ActorFunc)BgTokiSwd_Init,
    (ActorFunc)BgTokiSwd_Destroy,
    (ActorFunc)BgTokiSwd_Update,
    (ActorFunc)BgTokiSwd_Draw,
};

static u32 sInitChain[] = {
    0x00190A92, // ICHAIN_VEC3F_DIV1000(scale, 25, ICHAIN_STOP)
};

static CollisionCheckInfoInit sColChkInfoInit = { 10, 35, 100, MASS_IMMOVABLE };

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1 | OC2_UNK1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 10, 70, 0, { 0, 0, 0 } },
};

void BgTokiSwd_Init(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    BgTokiSwd* self = (BgTokiSwd*)actor;

    Actor_ProcessInitChain(actor, (InitChainEntry*)sInitChain);
    FUN_00372f38(actor, play, &self->skelAnimModel, 4, 0);
    self->actor.shape.yOffset = 800.0f;
    self->actionFunc = FUN_003e5dc8;
    if (LINK_IS_ADULT) {
        self->actor.draw = NULL;
    }
    if (gSaveContext.sceneSetupIndex == 5) {
        // writing through the raw `state` param (not the `play` local) keeps the bne (no if-conversion)
        DRAW_PARAM0((PlayState*)state) = 0xFF;
    }
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, actor, &sCylinderInit);
    Collider_UpdateCylinder(actor, &self->collider);
    CollisionCheck_SetInfo(&actor->colChkInfo, NULL, &sColChkInfoInit);
}

void BgTokiSwd_Destroy(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    BgTokiSwd* self = (BgTokiSwd*)actor;

    Collider_DestroyCylinder(play, &self->collider);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgTokiSwd_Update(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    BgTokiSwd* self = (BgTokiSwd*)actor;

    self->actionFunc(self, play);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
}

void BgTokiSwd_Draw(Actor* actor, GameState* state) {
    BgTokiSwd* self = (BgTokiSwd*)actor;
    MTX34 mtx;

    MTX34CopyAsm(&mtx, (MTX34*)((u8*)actor + 0x148));
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&mtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}

void FUN_003e5dc8(BgTokiSwd* self, PlayState* play) {
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        return;
    }

    if (!(gSaveContext.eventChkInf[4] & 0x8000) && gSaveContext.sceneSetupIndex < 4 &&
        Actor_IsFacingAndNearPlayer(&self->actor, 800.0f, 0x7530) && !Gameplay_InCsMode(play)) {
        gSaveContext.eventChkInf[4] |= 0x8000;
        play->FUN_0037573c(play->GetObjectStatus(self->actor.objBankIndex)->zarInfo.GetQDBByIndex(0));
        gSaveContext.cutsceneTrigger = 1;
    }

    if (!LINK_IS_ADULT || (gSaveContext.eventChkInf[5] & 0x20)) {
        if (Actor_HasParent(&self->actor, play)) {
            if (!LINK_IS_ADULT) {
                Item_Give(play, 0x3C);
                play->FUN_0037573c(play->GetObjectStatus(self->actor.objBankIndex)->zarInfo.GetQDBByIndex(1));
            } else {
                play->FUN_0037573c(play->GetObjectStatus(self->actor.objBankIndex)->zarInfo.GetQDBByIndex(2));
            }
            Audio_SetBGM(0, 0);
            Audio_QueueSeqCmd(0, 0x010005C0, 0);
            gSaveContext.cutsceneTrigger = 1;
            self->actor.parent = NULL;
            self->actionFunc = FUN_00174d90;
        } else if (Actor_IsFacingPlayer(&self->actor, 0x2000)) {
            FUN_0034df30(&self->actor, play);
        }
    }

    if (gSaveContext.sceneSetupIndex == 5) {
        if (DRAW_PARAM0(play) >= 1) {
            DRAW_PARAM0(play)--;
        } else {
            DRAW_PARAM0(play) = 0;
        }
    }
}

void FUN_00174d90(BgTokiSwd* self, PlayState* play) {
    if (Actor_HasParent(&self->actor, play)) {
        if (!LINK_IS_ADULT) {
            Audio_PlayActorSound2(&self->actor, 0x0100011F);
            self->actor.draw = NULL;
        } else {
            self->actor.draw = (ActorFunc)BgTokiSwd_Draw;
        }
        self->actionFunc = FUN_0013f400;
    } else {
        GET_PLAYER(play)->interactRangeActor = &self->actor;
    }
}

void FUN_0013f400(BgTokiSwd* self, PlayState* play) {
    if (Flags_GetEnv(play, 1) && (DRAW_PARAM0(play) < 0xFF)) {
        DRAW_PARAM0(play) += 5;
    }
}
}
