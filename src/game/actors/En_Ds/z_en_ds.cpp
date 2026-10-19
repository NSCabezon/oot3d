extern "C" {
#include "z_en_ds.hpp"

#define FLAGS 0x00000009

void EnDs_Init(Actor* thisx, PlayState* play);
void EnDs_Destroy(Actor* thisx, PlayState* play);
void EnDs_Update(Actor* thisx, PlayState* play);
void EnDs_Draw(Actor* thisx, PlayState* play);
void EnDs_Wait(EnDs* self, PlayState* play);
void EnDs_Talk(EnDs* self, PlayState* play);
void EnDs_TalkAfterGiveOddPotion(EnDs* self, PlayState* play);
void EnDs_DisplayOddPotionText(EnDs* self, PlayState* play);
void EnDs_GiveOddPotion(EnDs* self, PlayState* play);
void EnDs_BrewOddPotion1(EnDs* self, PlayState* play);
void EnDs_BrewOddPotion2(EnDs* self, PlayState* play);
void EnDs_BrewOddPotion3(EnDs* self, PlayState* play);
void EnDs_TalkAfterBrewOddPotion(EnDs* self, PlayState* play);
void EnDs_OfferOddPotion(EnDs* self, PlayState* play);
void EnDs_OfferBluePotion(EnDs* self, PlayState* play);
void EnDs_TalkNoEmptyBottle(EnDs* self, PlayState* play);
void EnDs_GiveBluePotion(EnDs* self, PlayState* play);
s32 EnDs_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void EnDs_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);

// Helpers outside this actor (ROM names; prototypes inferred from the call sites)
s32 FUN_00369a48(Actor* actor, PlayState* play);           // Actor_TextboxIsClosing
s32 FUN_00369f3c(PlayState* play);                         // exchange choice index
s32 FUN_0036bb28(Actor* actor, PlayState* play, f32 arg2); // Actor_OfferTalk
s8 FUN_0036bc84(PlayState* play);                          // Actor_GetPlayerExchangeItemId
void FUN_0036bbd0(Actor* actor, PlayState* play, s32 exchangeItemId, f32 range);
void FUN_0036bcc8(PlayState* play, Actor* actor, void* a, void* b, f32 x, f32 y, f32 z, s32 arg7);
void FUN_0036bee0(PlayState* play, f32 a, f32 b, f32 c, f32 d);
void FUN_003725e0(PlayState* play);
void FUN_00370778(PlayState* play);
void FUN_0034e01c(MTX34* mtx, Vec3s* rot);
void SkelAnime_SetCurrentFrame2(SkelAnime* skelAnime, f32 frame);
s32 Inventory_HasEmptyBottle(void);
void Rupees_ChangeBy(s16 amount);
void FUN_00372244(void* arg0, s16 arg1, u32 sfxId);
void GraphicsContext_UnknownStub(GraphicsContext* gfxCtx); // N64 Gfx_SetupDL_37Opa (empty on 3DS)
void* ZAR_GetCMBByIndex(void* zar, s32 index);
void FUN_00353e78(void* zar, PlayState* play, SkelAnime* skelAnime, void* cmb, void* p, s32 a, void* b, void* c, s32 d);
void Animation_PlayOnce(SkelAnime* skelAnime, void* anim);


ActorInit En_Ds_InitVars = {
    ACTOR_EN_DS,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_DS,
    sizeof(EnDs),
    (ActorFunc)EnDs_Init,
    (ActorFunc)EnDs_Destroy,
    (ActorFunc)EnDs_Update,
    (ActorFunc)EnDs_Draw,
};


static inline u8* EnDs_GetObjStatus(PlayState* play, s32 idx) {
    if ((u32)idx < 0x13 && *(s32*)((u8*)play + (idx << 7) + 0x3A64) != 0) {
        return (u8*)play + (idx << 7) + 0x3A5C;
    }
    return NULL;
}

void EnDs_Init(Actor* thisx, PlayState* play) {
    EnDs* self = (EnDs*)thisx;
    void* zar;
    void* cmb;
    Actor* actor = &self->actor;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 36.0f);
    zar = (void*)(EnDs_GetObjStatus(play, (u8)self->actor.objBankIndex) + 0x10);
    cmb = ZAR_GetCMBByIndex(zar, 0);

    self->actor.unk_19A = 1;
    FUN_00372f38(thisx, play, &self->model, 0, 0);
    FUN_00353e78(zar, play, &self->skelAnime, cmb, self->actor.unk_178, 0, self->unk_228, self->unk_32C, 5);
    Animation_PlayOnce(&self->skelAnime, NULL);
    actor->colChkInfo.mass = MASS_IMMOVABLE;
    Actor_SetScale(actor, 0.013f);
    self->actionFunc = EnDs_Wait;
    thisx->targetMode = 1;
    self->unk_1E8 = 0;
    self->unk_1E4 = 0.0f;
    self->actor.flags &= ~1;
}

void EnDs_Destroy(Actor* thisx, PlayState* play) {
    EnDs* self = (EnDs*)thisx;

    FUN_003508b8(thisx, self->model, 0);
    SkelAnime_Free2(&self->skelAnime);
}

#if 0 // draft: Update 7 words off (play kept in r5 vs ROM r6; hoisted 0x1838 tried)
void EnDs_Update(Actor* thisx, PlayState* play) {
    EnDs* self = (EnDs*)thisx;

    if (FUN_00370734(&self->skelAnime)) {
        SkelAnime_SetCurrentFrame2(&self->skelAnime, 0.0f);
    }
    self->actionFunc(self, play);
    if (self->unk_1E8 & 1) {
        FUN_0036bcc8(play, &self->actor, &self->unk_1D8, &self->unk_1DE, self->actor.focus.pos.x, self->actor.focus.pos.y,
                     self->actor.focus.pos.z, 0x4300);
    } else {
        Math_SmoothStepToS(&self->unk_1D8.x, 0, 6, 0x1838, 100);
        Math_SmoothStepToS(&self->unk_1D8.y, 0, 6, 0x1838, 100);
        Math_SmoothStepToS(&self->unk_1DE.x, 0, 6, 0x1838, 100);
        Math_SmoothStepToS(&self->unk_1DE.y, 0, 6, 0x1838, 100);
    }
}
#endif

GLOBAL_ASM("asm/EnDs_Update.s")

// PostLimbDraw: guarded function-local static pinned by pin_static.py (guard 0x522B4C = the whole former z_En_Ds.data chunk, object 0x5990A4)
// N64 Matrix_MultVec3f shape: (src, dest) order drives the arg scheduling of the tail call
static inline void EnDs_MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}

void EnDs_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnDs* self = (EnDs*)thisx;
    static Vector3f sMultVec(1100.0f, 500.0f, 0.0f);

    if (limbIndex == 3) {
        EnDs_MultVec3f(&sMultVec, (Vector3f*)&self->actor.focus.pos, mtx);
    }
}

s32 EnDs_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnDs* self = (EnDs*)thisx;

    if (limbIndex == 3) {
        FUN_0034e01c(mtx, &self->unk_1D8);
    }
    return false;
}

void EnDs_Draw(Actor* thisx, PlayState* play) {
    EnDs* self = (EnDs*)thisx;

    GraphicsContext_UnknownStub(play->state.gfxCtx);
    SkelAnime_DrawOpa(&self->skelAnime, (MTX34*)((u8*)self + 0x148), EnDs_OverrideLimbDraw, EnDs_PostLimbDraw, &self->actor, 0);
}

void EnDs_Talk(EnDs* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        self->actionFunc = EnDs_Wait;
        self->actor.flags &= ~0x10000;
    }
    self->unk_1E8 |= 1;
}

void EnDs_TalkNoEmptyBottle(EnDs* self, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_EVENT) && Message_ShouldAdvance(play)) {
        FUN_003725e0(play);
        self->actionFunc = EnDs_Wait;
    }
    self->unk_1E8 |= 1;
}

void EnDs_TalkAfterGiveOddPotion(EnDs* self, PlayState* play) {
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        self->actionFunc = EnDs_Talk;
    } else {
        self->actor.flags |= 0x10000;
        FUN_0036bb28(&self->actor, play, 1000.0f);
    }
}

void EnDs_DisplayOddPotionText(EnDs* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        self->actor.textId = 0x504F;
        self->actionFunc = EnDs_TalkAfterGiveOddPotion;
        self->actor.flags &= ~0x100;
        gSaveContext.itemGetInf[3] |= 1;
    }
}

void EnDs_GiveOddPotion(EnDs* self, PlayState* play) {
    if (Actor_HasParent(&self->actor, play)) {
        self->actor.parent = NULL;
        self->actionFunc = EnDs_DisplayOddPotionText;
        *(u16*)((u8*)&gSaveContext + 0x1562) = 0;
    } else {
        GiveItem(&self->actor, play, 0x20, 10000.0f, 50.0f);
    }
}

void EnDs_TalkAfterBrewOddPotion(EnDs* self, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_EVENT) && Message_ShouldAdvance(play)) {
        FUN_003725e0(play);
        self->actionFunc = EnDs_GiveOddPotion;
        GiveItem(&self->actor, play, 0x20, 10000.0f, 50.0f);
    }
}

void EnDs_BrewOddPotion3(EnDs* self, PlayState* play) {
    if (self->brewTimer > 0) {
        self->brewTimer--;
    } else {
        self->actionFunc = EnDs_TalkAfterBrewOddPotion;
        Message_ContinueTextbox(play, 0x504D);
    }
    Math_StepToF(&self->unk_1E4, 0.0f, 0.03f);
    FUN_0036bee0(play, self->unk_1E4 * (2.0f - self->unk_1E4), 0.0f, 0.1f, 1.0f);
}

void EnDs_BrewOddPotion2(EnDs* self, PlayState* play) {
    if (self->brewTimer > 0) {
        self->brewTimer--;
    } else {
        self->actionFunc = EnDs_BrewOddPotion3;
        self->brewTimer = 90;
        Flags_UnsetSwitch(play, 0x3F);
    }
}

void EnDs_BrewOddPotion1(EnDs* self, PlayState* play) {
    if (self->brewTimer > 0) {
        self->brewTimer--;
    } else {
        self->actionFunc = EnDs_BrewOddPotion2;
        self->brewTimer = 30;
    }
    Math_StepToF(&self->unk_1E4, 1.0f, 0.01f);
    FUN_0036bee0(play, self->unk_1E4 * (2.0f - self->unk_1E4), 0.0f, 0.1f, 1.0f);
}

void EnDs_OfferOddPotion(EnDs* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_CHOICE) && Message_ShouldAdvance(play)) {
        switch (FUN_00369f3c(play)) {
            case 0:
                self->actionFunc = EnDs_BrewOddPotion1;
                self->brewTimer = 90;
                Flags_SetSwitch(play, 0x3F);
                FUN_00370778(play);
                *(u8*)((u8*)player + 0x172B) = EXCH_ITEM_NONE;
                break;
            case 1:
                Message_ContinueTextbox(play, 0x504C);
                self->actionFunc = EnDs_Talk;
                break;
        }
    }
}

s32 EnDs_CheckRupeesAndBottle(void) {
    if (gSaveContext.rupees < 100) {
        return 0;
    } else if (Inventory_HasEmptyBottle() == 0) {
        return 1;
    } else {
        return 2;
    }
}

void EnDs_GiveBluePotion(EnDs* self, PlayState* play) {
    if (Actor_HasParent(&self->actor, play)) {
        self->actor.parent = NULL;
        self->actionFunc = EnDs_Talk;
    } else {
        GiveItem(&self->actor, play, 0x12, 10000.0f, 50.0f);
    }
}

void EnDs_OfferBluePotion(EnDs* self, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_CHOICE) && Message_ShouldAdvance(play)) {
        switch (FUN_00369f3c(play)) {
            case 0:
                switch (EnDs_CheckRupeesAndBottle()) {
                    case 0:
                        Message_ContinueTextbox(play, 0x500E);
                        break;
                    case 1:
                        Message_ContinueTextbox(play, 0x96);
                        self->actionFunc = EnDs_TalkNoEmptyBottle;
                        return;
                    case 2:
                        Rupees_ChangeBy(-100);
                        self->actor.flags &= ~0x10000;
                        GiveItem(&self->actor, play, 0x12, 10000.0f, 50.0f);
                        self->actionFunc = EnDs_GiveBluePotion;
                        return;
                }
                break;
            case 1:
                Message_ContinueTextbox(play, 0x500D);
                break;
        }
        self->actionFunc = EnDs_Talk;
    }
}

void EnDs_Wait(EnDs* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Actor* thisx = &self->actor;
    s16 yawDiff;

    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        if (FUN_0036bc84(play) == EXCH_ITEM_ODD_MUSHROOM) {
            FUN_00372244((u8*)play + 0x5FCC, 0x1E, 0x0100048C);
            player->actor.textId = 0x504A;
            self->actionFunc = EnDs_OfferOddPotion;
            return;
        }
        if (gSaveContext.itemGetInf[3] & 1) {
            player->actor.textId = 0x500C;
            self->actionFunc = EnDs_OfferBluePotion;
        } else {
            if (INV_CONTENT(ITEM_ODD_MUSHROOM) == ITEM_ODD_MUSHROOM) {
                player->actor.textId = 0x5049;
            } else {
                player->actor.textId = 0x5048;
            }
            self->actionFunc = EnDs_Talk;
        }
    } else {
        yawDiff = thisx->yawTowardsPlayer - thisx->shape.rot.y;
        thisx->textId = 0x5048;
        if ((ABS(yawDiff) < 0x2151) && (self->actor.xzDistToPlayer < 200.0f)) {
            FUN_0036bbd0(&self->actor, play, EXCH_ITEM_ODD_MUSHROOM, 100.0f);
            self->unk_1E8 |= 1;
        }
    }
}

}
