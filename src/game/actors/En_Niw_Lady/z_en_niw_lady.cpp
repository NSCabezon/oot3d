extern "C" {
#include "z_en_niw_lady.hpp"

#define FLAGS 0x00000019

void EnNiwLady_Init(Actor* thisx, GameState* state);
void EnNiwLady_Destroy(Actor* thisx, GameState* state);
void EnNiwLady_Update(Actor* actor, GameState* state);
void EnNiwLady_Draw(Actor* thisx, PlayState* play);
s32 FUN_0022de08(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx); // EnNiwLady_OverrideLimbDraw

s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
void FUN_0034e01c(MTX34* mtx, Vec3s* rot);
void FUN_003725e0(PlayState* play); // Message_CloseTextbox
extern s16 EnNiwLady_sMissingCuccoTextIds[];
extern s16 EnNiwLady_sTradeItemTextIds[];
void FUN_0036bbd0(Actor* actor, PlayState* play, s32 exchangeItem, f32 range); // Actor_OfferTalkExchangeEquiCylinder
void FUN_0021bce8(EnNiwLady* self, PlayState* play); // func_80ABAA9C
void FUN_00299110(EnNiwLady* self, PlayState* play); // func_80ABAC00
void FUN_00258a04(EnNiwLady* self, PlayState* play); // func_80ABA878
void FUN_0038a59c(EnNiwLady* self, PlayState* play); // func_80ABA9B8
void FUN_00288414(EnNiwLady* self, PlayState* play); // func_80ABAB08

void FUN_00154904(EnNiwLady* self, PlayState* play); // func_80AB9F24
void FUN_001792f4(EnNiwLady* self, PlayState* play); // func_80ABA244
void FUN_002389ec(EnNiwLady* self, PlayState* play); // func_80ABA778
void FUN_001cade8(EnNiwLady* self, PlayState* play); // func_80ABAC84

GLOBAL_ASM("data/z_En_Niw_Lady.data.s")
GLOBAL_ASM("../src/game/actors/En_Niw_Lady/data_syms.s")

ActorInit En_Niw_Lady_InitVars = {
    ACTOR_EN_NIW_LADY,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_ANE,
    sizeof(EnNiwLady),
    (ActorFunc)EnNiwLady_Init,
    (ActorFunc)EnNiwLady_Destroy,
    (ActorFunc)EnNiwLady_Update,
    NULL,
};

void EnNiwLady_Init(Actor* thisx, GameState* state) {
    EnNiwLady* self = (EnNiwLady*)thisx;
    PlayState* play = (PlayState*)state;

    self->aneObjectSlot = Object_GetIndex(&play->objectCtx, 0x110);
    self->osAnimeObjectSlot = Object_GetIndex(&play->objectCtx, 0xC5);
    if ((self->osAnimeObjectSlot < 0) || (self->aneObjectSlot < 0)) {
        Actor_Kill(thisx);
        return;
    }
    self->unk_278 = 0;
    if (play->sceneNum == 0x37) {
        self->unk_278 = 1;
    }
    if ((self->unk_278 != 0) && (gSaveContext.nightFlag == 0)) {
        Actor_Kill(thisx);
        return;
    }
    thisx->uncullZoneForward = 600.0f;
    self->actionFunc = FUN_00154904;
}

void EnNiwLady_Destroy(Actor* thisx, GameState* state) {
    EnNiwLady* self = (EnNiwLady*)thisx;
    PlayState* play = (PlayState*)state;

    Collider_DestroyCylinder(play, &self->collider);
    FUN_003508b8(&self->actor, self->skelAnimModel, 0);
    SkelAnime_Free2(&self->skelAnime);
    ((FaceAnimations*)self->faceAnims)->Destroy();
}

GLOBAL_ASM("asm/EnNiwLady_Update.s")

void EnNiwLady_Draw(Actor* thisx, PlayState* play2) {
    EnNiwLady* self = (EnNiwLady*)thisx;
    PlayState* play = play2;

    if (self->unk_27E != 0) {
        FaceAnimations* fa = (FaceAnimations*)self->faceAnims;

        fa->FUN_0035e3a4(0, self->faceState);
        GraphicsContext_UnknownStub(play->state.gfxCtx);
        SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, (OverrideLimbDrawOpa)FUN_0022de08, NULL, &self->actor,
                          0);
    }
    ((FaceAnimations*)self->faceAnims)->FUN_0035e330();
}


// func_80ABA21C
void FUN_00180e48(EnNiwLady* self, PlayState* play) {
    self->actor.textId = EnNiwLady_sMissingCuccoTextIds[0];
    self->unk_262 = 6;
    self->actionFunc = FUN_001792f4;
}

// EnNiwLady_OverrideLimbDraw
s32 FUN_0022de08(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnNiwLady* self = (EnNiwLady*)thisx;

    if (limbIndex == 14) {
        FUN_0034e01c(mtx, &self->headRot);
    } else if (limbIndex == 7) {
        FUN_0034e01c(mtx, &self->torsoRot);
    }
    return 0;
}

// func_80ABAA9C
void FUN_0021bce8(EnNiwLady* self, PlayState* play) {
    self->unk_26E = 11;
    if ((self->unk_262 == Message_GetState(&play->msgCtx)) && Message_ShouldAdvance(play)) {
        FUN_003725e0(play);
        self->actionFunc = FUN_002389ec;
    }
}

// func_80ABAC00
void FUN_00299110(EnNiwLady* self, PlayState* play) {
    s32 getItemId;

    if (Actor_HasParent(&self->actor, play)) {
        self->actionFunc = FUN_001cade8;
    } else {
        getItemId = self->getItemId;
        if (gSaveContext.linkAge == 0) {
            getItemId = !(gSaveContext.itemGetInf[2] & 0x1000) ? 0x1D : 0xE;
        }
        GiveItem(&self->actor, play, getItemId, 200.0f, 100.0f);
    }
}

// func_80ABAC84
void FUN_001cade8(EnNiwLady* self, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) != 6) || !Message_ShouldAdvance(play)) {
        return;
    }
    if (gSaveContext.linkAge == 0) {
        if (!(gSaveContext.itemGetInf[2] & 0x1000)) {
            gSaveContext.itemGetInf[2] |= 0x1000;
        } else {
            gSaveContext.itemGetInf[2] |= 0x4000;
        }
        self->actionFunc = FUN_002389ec;
    } else {
        gSaveContext.itemGetInf[0] |= 0x1000;
        self->unk_262 = 6;
        self->actionFunc = FUN_001792f4;
    }
}

// func_80ABA654
void FUN_001e8d10(EnNiwLady* self, PlayState* play) {
    if (self->unk_262 == Message_GetState(&play->msgCtx) && Message_ShouldAdvance(play)) {
        FUN_003725e0(play);
        self->unk_26E = 0xB;
        if (!(gSaveContext.itemGetInf[0] & 0x1000)) {
            self->actor.parent = NULL;
            GiveItem(&self->actor, play, self->getItemId = 0xF, 100.0f, 50.0f);
            self->actionFunc = FUN_00299110;
            return;
        }
        if (self->unk_26C == 1) {
            GiveItem(&self->actor, play, self->getItemId = 0x55, 100.0f, 50.0f);
            self->actionFunc = FUN_00299110;
            return;
        }
        self->actionFunc = FUN_001792f4;
    }
}

// func_80ABA778
void FUN_002389ec(EnNiwLady* self, PlayState* play) {
    self->unk_262 = 6;
    self->unk_273 = 0;
    if (!(gSaveContext.itemGetInf[2] & 0x1000)) {
        if (self->unk_274 != 0) {
            self->unk_27A = 1;
        } else {
            self->unk_27A = 0;
        }
        self->unk_273 = 1;
        self->unk_262 = 4;
    } else {
        self->unk_27A = 2;
        if (!(gSaveContext.itemGetInf[2] & 0x4000)) {
            self->unk_27A = 3;
            if (gSaveContext.eventChkInf[6] & 0x400) {
                self->unk_27A = 9;
                if (self->unk_277 != 0) {
                    self->unk_27A = 10;
                }
            } else {
                self->unk_27A = 4;
            }
        }
    }
    self->actor.textId = EnNiwLady_sTradeItemTextIds[self->unk_27A];
    self->actionFunc = FUN_00258a04;
}

// func_80ABA878
void FUN_00258a04(EnNiwLady* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if ((Message_GetState(&play->msgCtx) == 0) || (Message_GetState(&play->msgCtx) == 6)) {
        self->unk_26E = 11;
    }
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        s8 playerExchangeItemId = FUN_0036bc84(play);

        if ((playerExchangeItemId == 6) && (gSaveContext.eventChkInf[6] & 0x400)) {
            FUN_00372244(&play->unk_5FCC, 0x1E, 0x100048C);
            player->actor.textId = EnNiwLady_sTradeItemTextIds[5];
            self->unk_26E = self->unk_27A + 21;
            self->unk_262 = 4;
            self->actionFunc = FUN_00288414;
        } else if (playerExchangeItemId != 0) {
            player->actor.textId = EnNiwLady_sTradeItemTextIds[7];
            self->unk_26E = self->unk_27A + 21;
        } else {
            self->unk_274 = 1;
            self->unk_26E = self->unk_27A + 21;
            if (!self->unk_273) {
                self->actionFunc = FUN_002389ec;
            } else {
                self->actionFunc = FUN_0038a59c;
            }
        }
        return;
    }
    FUN_0036bbd0(&self->actor, play, 6, 50.0f);
}

// func_80ABA9B8
void FUN_0038a59c(EnNiwLady* self, PlayState* play) {
    if ((self->unk_262 == Message_GetState(&play->msgCtx)) && Message_ShouldAdvance(play)) {
        switch (FUN_00369f3c(play)) {
            case 0:
                FUN_003725e0(play);
                self->actor.parent = NULL;
                GiveItem(&self->actor, play, 0x1D, 200.0f, 100.0f);
                self->actionFunc = FUN_00299110;
                break;
            case 1:
                self->actor.textId = EnNiwLady_sTradeItemTextIds[3];
                self->unk_26E = self->unk_27A + 21;
                Message_ContinueTextbox(play, self->actor.textId);
                self->unk_262 = 5;
                self->actionFunc = FUN_0021bce8;
                break;
        }
    }
}

// func_80ABAB08
void FUN_00288414(EnNiwLady* self, PlayState* play) {
    if ((self->unk_262 == Message_GetState(&play->msgCtx)) && Message_ShouldAdvance(play)) {
        switch (FUN_00369f3c(play)) {
            case 0:
                FUN_003725e0(play);
                self->actor.parent = NULL;
                GiveItem(&self->actor, play, 0xE, 200.0f, 100.0f);
                self->actionFunc = FUN_00299110;
                break;
            case 1:
                FUN_003725e0(play);
                self->unk_277 = 1;
                self->actor.textId = EnNiwLady_sTradeItemTextIds[8];
                self->unk_26E = self->unk_27A + 21;
                Message_ContinueTextbox(play, self->actor.textId);
                self->unk_262 = 5;
                self->actionFunc = FUN_0021bce8;
                break;
        }
    }
}
}
