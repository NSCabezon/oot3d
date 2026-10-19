extern "C" {
#include "z_en_ex_item.hpp"
#include "model_manager.hpp"

#define FLAGS 0x00000030

typedef struct {
    /* 0x0000 */ char unk_0000[0x1AE];
    /* 0x01AE */ s16 hasRewardFinishedAppearing;
} EnBomBowlPitView;

void EnExItem_Init(Actor* thisx, PlayState* play);
void EnExItem_Destroy(Actor* thisx, PlayState* play);
void EnExItem_Update(Actor* thisx, PlayState* play);
void EnExItem_Draw(Actor* thisx, PlayState* play);

void EnExItem_WaitForObject(EnExItem* self, PlayState* play);
void EnExItem_BowlPrize(EnExItem* self, PlayState* play);
void EnExItem_SetupBowlCounter(EnExItem* self, PlayState* play);
void EnExItem_BowlCounter(EnExItem* self, PlayState* play);
void EnExItem_ExitChest(EnExItem* self, PlayState* play);
void EnExItem_FairyMagic(EnExItem* self, PlayState* play);
void EnExItem_TargetPrizeApproach(EnExItem* self, PlayState* play);
void EnExItem_TargetPrizeGive(EnExItem* self, PlayState* play);
void EnExItem_TargetPrizeFinish(EnExItem* self, PlayState* play);

s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
s32 Object_IsLoaded(ObjectContext* objectCtx, s32 bankIndex);
void Player_SetCsActionWithHaltedActors(PlayState* play, Actor* actor, s32 csAction);

ActorInit En_Ex_Item_InitVars = {
    ACTOR_EN_EX_ITEM,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnExItem),
    (ActorFunc)EnExItem_Init,
    (ActorFunc)EnExItem_Destroy,
    (ActorFunc)EnExItem_Update,
    (ActorFunc)EnExItem_Draw,
};

#define ACTOR_FLAG_ATTENTION_ENABLED 0x1

void EnExItem_Init(Actor* thisx, PlayState* play) {
    EnExItem* self = (EnExItem*)thisx;

    self->actor.unk_19A = 1;
    self->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    self->type = self->actor.params & 0xFF;
    self->unusedParam = (self->actor.params >> 8) & 0xFF;
    self->initPos = self->actor.world.pos;
    self->getItemObjectId = -1;
    switch (self->type) {
        case EXITEM_BOMB_BAG_BOWLING:
        case EXITEM_BOMB_BAG_COUNTER:
        case 20:
        case 25:
            self->getItemObjectId = OBJECT_GI_BOMBPOUCH;
            break;
        case EXITEM_HEART_PIECE_BOWLING:
        case EXITEM_HEART_PIECE_COUNTER:
            self->getItemObjectId = OBJECT_GI_HEARTS;
            break;
        case EXITEM_BOMBCHUS_BOWLING:
        case EXITEM_BOMBCHUS_COUNTER:
            self->getItemObjectId = OBJECT_GI_BOMB_2;
            break;
        case EXITEM_BOMBS_BOWLING:
        case EXITEM_BOMBS_COUNTER:
            self->getItemObjectId = OBJECT_GI_BOMB_1;
            break;
        case EXITEM_PURPLE_RUPEE_BOWLING:
        case EXITEM_PURPLE_RUPEE_COUNTER:
        case EXITEM_GREEN_RUPEE_CHEST:
        case EXITEM_BLUE_RUPEE_CHEST:
        case EXITEM_RED_RUPEE_CHEST:
        case EXITEM_13:
        case EXITEM_14:
            self->getItemObjectId = OBJECT_GI_RUPY;
            break;
        case EXITEM_SMALL_KEY_CHEST:
            self->scale = 0.05f;
            self->timer = 7 * 3.0f / 2.0f + 0.5f;
            self->actor.velocity.y = 10.0f;
            self->actionFunc = EnExItem_ExitChest;
            break;
        case EXITEM_MAGIC_FIRE:
        case EXITEM_MAGIC_WIND:
        case EXITEM_MAGIC_DARK:
            self->getItemObjectId = OBJECT_GI_GODDESS;
            break;
        case EXITEM_BULLET_BAG:
            self->getItemObjectId = OBJECT_GI_DEKUPOUCH;
    }

    if (self->getItemObjectId >= 0) {
        self->requiredObjectSlot = Object_GetIndex(&play->objectCtx, self->getItemObjectId);
        self->actor.draw = NULL;
        if (self->requiredObjectSlot < 0) {
            Actor_Kill(&self->actor);
            return;
        }
        self->actionFunc = EnExItem_WaitForObject;
    }
}

void EnExItem_Destroy(Actor* thisx, PlayState* play) {
    EnExItem* self = (EnExItem*)thisx;

    if (self->model != NULL) {
        self->model->Destroy();
    }
    self->model = NULL;
}

void EnExItem_Update(Actor* thisx, PlayState* play) {
    EnExItem* self = (EnExItem*)thisx;

    if (self->timer != 0) {
        self->timer--;
    }
    if (self->chestKillTimer != 0) {
        self->chestKillTimer--;
    }
    if (self->prizeRotateTimer != 0) {
        self->prizeRotateTimer--;
    }
    self->actionFunc(self, play);
}

void FUN_00369178(SkeletonAnimationModel* model, s32 entry);

#define AM(s) ((SkeletonAnimationModel*)(s)->model)

static void EnExItem_DrawModel(EnExItem* self) {
    AM(self)->FUN_003721e0(&self->actor.modelMtx);
    AM(self)->SetUnkAC(1);
    AM(self)->FUN_00372170(0);
}

static void EnExItem_DrawItems(EnExItem* self) {
    if (self->model == NULL) {
        return;
    }
    EnExItem_DrawModel(self);
}

static void EnExItem_DrawHeartPiece(EnExItem* self) {
    if (self->model == NULL) {
        return;
    }
    EnExItem_DrawModel(self);
}

static void EnExItem_DrawRupee(EnExItem* self) {
    if (self->model == NULL) {
        return;
    }
    FUN_00369178(AM(self), self->giDrawId);
    EnExItem_DrawModel(self);
}

static void EnExItem_DrawKey(EnExItem* self) {
    if (self->model == NULL) {
        return;
    }
    EnExItem_DrawModel(self);
}

static void EnExItem_DrawMagic(EnExItem* self) {
    if (self->model == NULL) {
        return;
    }
    EnExItem_DrawModel(self);
}

void EnExItem_Draw(Actor* thisx, PlayState* play) {
    EnExItem* self = (EnExItem*)thisx;

    Actor_SetScale(thisx, self->scale);
    switch (self->type) {
        case EXITEM_BOMB_BAG_BOWLING:
        case EXITEM_BOMBCHUS_BOWLING:
        case EXITEM_BOMBS_BOWLING:
        case EXITEM_BOMB_BAG_COUNTER:
        case EXITEM_BOMBCHUS_COUNTER:
        case EXITEM_BOMBS_COUNTER:
        case EXITEM_BULLET_BAG:
        case 20:
        case 25:
            EnExItem_DrawItems(self);
            break;
        case EXITEM_HEART_PIECE_BOWLING:
        case EXITEM_HEART_PIECE_COUNTER:
            EnExItem_DrawHeartPiece(self);
            break;
        case EXITEM_PURPLE_RUPEE_BOWLING:
        case EXITEM_PURPLE_RUPEE_COUNTER:
        case EXITEM_GREEN_RUPEE_CHEST:
        case EXITEM_BLUE_RUPEE_CHEST:
        case EXITEM_RED_RUPEE_CHEST:
        case EXITEM_13:
        case EXITEM_14:
            EnExItem_DrawRupee(self);
            break;
        case EXITEM_SMALL_KEY_CHEST:
            EnExItem_DrawKey(self);
            break;
        case EXITEM_MAGIC_FIRE:
        case EXITEM_MAGIC_WIND:
        case EXITEM_MAGIC_DARK:
            EnExItem_DrawMagic(self);
            break;
    }
}

void* ZAR_GetCMBByIndex(void* zar, s32 index);

void EnExItem_WaitForObject(EnExItem* self, PlayState* play) {
    s32 onCounter;
    ZARInfo* zar;
    void* cmb;
    void* cmab;

    if (Object_IsLoaded(&play->objectCtx, self->requiredObjectSlot)) {
        self->actor.objBankIndex = self->requiredObjectSlot;
        self->actor.draw = (ActorFunc)EnExItem_Draw;
        self->stopRotate = 0;
        zar = &play->GetObjectStatus(self->actor.objBankIndex)->zarInfo;
        cmb = NULL;
        cmab = NULL;
        onCounter = 0;
        switch (self->type) {
            case 25:
            case EXITEM_BOMB_BAG_COUNTER:
                onCounter = 1;
                // fallthrough
            case EXITEM_BOMB_BAG_BOWLING:
            case 20:
                if (self->type == 0 || self->type == 5) {
                    self->giDrawId = 0x18;
                    cmb = ZAR_GetCMBByIndex(zar, 1);
                } else {
                    self->giDrawId = 0x19;
                    cmb = ZAR_GetCMBByIndex(zar, 2);
                }
                self->scale = 0.5f;
                self->timer = 65 * 3.0f / 2.0f + 0.5f;
                self->prizeRotateTimer = 35 * 3.0f / 2.0f + 0.5f;
                if (!onCounter) {
                    self->actionFunc = EnExItem_BowlPrize;
                } else {
                    self->actor.shape.yOffset = -18.0f;
                    self->actionFunc = EnExItem_SetupBowlCounter;
                }
                break;
            case EXITEM_HEART_PIECE_COUNTER:
                onCounter = 1;
                // fallthrough
            case EXITEM_HEART_PIECE_BOWLING:
                cmb = ZAR_GetCMBByIndex(zar, 1);
                self->scale = 0.5f;
                self->timer = 65 * 3.0f / 2.0f + 0.5f;
                self->prizeRotateTimer = 35 * 3.0f / 2.0f + 0.5f;
                if (!onCounter) {
                    Audio_PlaySoundGeneral(0x010004A8, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
                    self->actionFunc = EnExItem_BowlPrize;
                } else {
                    self->actor.shape.yOffset = -10.0f;
                    self->actionFunc = EnExItem_SetupBowlCounter;
                }
                break;
            case EXITEM_BOMBCHUS_COUNTER:
                onCounter = 1;
                // fallthrough
            case EXITEM_BOMBCHUS_BOWLING:
                self->giDrawId = 0x27;
                cmb = ZAR_GetCMBByIndex(zar, 0);
                self->scale = 0.5f;
                self->timer = 65 * 3.0f / 2.0f + 0.5f;
                self->prizeRotateTimer = 35 * 3.0f / 2.0f + 0.5f;
                if (!onCounter) {
                    self->actionFunc = EnExItem_BowlPrize;
                } else {
                    self->actionFunc = EnExItem_SetupBowlCounter;
                }
                break;
            case EXITEM_BOMBS_BOWLING:
            case EXITEM_BOMBS_COUNTER:
                self->giDrawId = 0x1F;
                cmb = ZAR_GetCMBByIndex(zar, 0);
                self->scale = 0.5f;
                self->unkFloat = 0.5f;
                self->timer = 65 * 3.0f / 2.0f + 0.5f;
                self->prizeRotateTimer = 35 * 3.0f / 2.0f + 0.5f;
                if (self->type == EXITEM_BOMBS_BOWLING) {
                    self->actionFunc = EnExItem_BowlPrize;
                } else {
                    self->actor.shape.yOffset = 10.0f;
                    self->actionFunc = EnExItem_SetupBowlCounter;
                }
                break;
            case EXITEM_PURPLE_RUPEE_BOWLING:
            case EXITEM_PURPLE_RUPEE_COUNTER:
                self->giDrawId = 0x70;
                cmb = ZAR_GetCMBByIndex(zar, 0);
                self->scale = 0.5f;
                self->unkFloat = 0.5f;
                self->timer = 65 * 3.0f / 2.0f + 0.5f;
                self->prizeRotateTimer = 35 * 3.0f / 2.0f + 0.5f;
                if (self->type == EXITEM_PURPLE_RUPEE_BOWLING) {
                    self->actionFunc = EnExItem_BowlPrize;
                } else {
                    self->actor.shape.yOffset = 40.0f;
                    self->actionFunc = EnExItem_SetupBowlCounter;
                }
                break;
            case EXITEM_GREEN_RUPEE_CHEST:
            case EXITEM_BLUE_RUPEE_CHEST:
            case EXITEM_RED_RUPEE_CHEST:
            case EXITEM_13:
            case EXITEM_14:
                self->scale = 0.5f;
                self->unkFloat = 0.5f;
                self->timer = 7 * 3.0f / 2.0f + 0.5f;
                self->actor.velocity.y = 10.0f;
                switch (self->type) {
                    case EXITEM_GREEN_RUPEE_CHEST:
                        self->giDrawId = 0x6C;
                        break;
                    case EXITEM_BLUE_RUPEE_CHEST:
                        self->giDrawId = 0x6D;
                        break;
                    case EXITEM_RED_RUPEE_CHEST:
                        self->giDrawId = 0x6E;
                        break;
                    case EXITEM_14:
                        self->giDrawId = 0x70;
                        break;
                }
                cmb = ZAR_GetCMBByIndex(zar, 0);
                self->actionFunc = EnExItem_ExitChest;
                break;
            case EXITEM_MAGIC_FIRE:
                self->scale = 0.35f;
                cmb = ZAR_GetCMBByIndex(zar, 0);
                cmab = zar->GetCMABByIndex(0);
                self->actionFunc = EnExItem_FairyMagic;
                break;
            case EXITEM_MAGIC_WIND:
                self->scale = 0.35f;
                cmb = ZAR_GetCMBByIndex(zar, 1);
                cmab = zar->GetCMABByIndex(1);
                self->actionFunc = EnExItem_FairyMagic;
                break;
            case EXITEM_MAGIC_DARK:
                self->scale = 0.35f;
                cmb = ZAR_GetCMBByIndex(zar, 2);
                cmab = zar->GetCMABByIndex(2);
                self->actionFunc = EnExItem_FairyMagic;
                break;
            case EXITEM_BULLET_BAG:
                self->giDrawId = 0x6B;
                cmb = ZAR_GetCMBByIndex(zar, 0);
                self->scale = 0.1f;
                self->timer = 80 * 3.0f / 2.0f + 0.5f;
                self->prizeRotateTimer = 35 * 3.0f / 2.0f + 0.5f;
                self->actionFunc = EnExItem_TargetPrizeApproach;
                break;
            case EXITEM_SMALL_KEY_CHEST:
                break;
        }

        if (cmb != NULL) {
            ModelAllocator* alloc = GetModelManager().allocator;
            alloc->owner = self->actor.unk_178;
            self->model = (EnExItemModel*)alloc->Create(cmb, 1);
            alloc->owner = NULL;
            if (cmab != NULL) {
                SkeletonAnimationModel_unk_0C* m = ((SkeletonAnimationModel*)self->model)->unk_0C;
                m->FUN_00372d94(cmab);
                m->SetAnimType(1);
            }
        }
    }
}


#if 0 // draft: 9 words off (near): pos update order z,x,y with d1/d2 locals; remaining = x/y stores before z vmla + 0.5 literal load placement; see STATUS.md
void EnExItem_BowlPrize(EnExItem* self, PlayState* play) {
    f32 tmpf1;
    f32 tmpf2;
    f32 tmpf3;
    f32 tmpf4;
    f32 tmpf5;
    f32 tmpf6;
    f32 tmpf7;
    f32 sp3C;

    if (!self->stopRotate) {
        self->actor.shape.rot.y += (s16)(0x1000 * 2.0f / 3.0f + 0.5f);
        if ((self->prizeRotateTimer == 0) && ((self->actor.shape.rot.y & 0xFFFF) == 0x9000)) {
            self->stopRotate++;
        }
    } else {
        Math_SmoothStepToS(&self->actor.shape.rot.y, 0, 5, 0x1000, 0);
    }
    if (self->timer != 0) {
        if (self->prizeRotateTimer != 0) {
            sp3C = 250.0f;
            if (self->type == EXITEM_BOMBCHUS_BOWLING) {
                sp3C = 220.0f;
            }
            tmpf1 = play->view.lookAt.x - play->view.eye.x;
            tmpf2 = play->view.lookAt.y - play->view.eye.y;
            tmpf3 = play->view.lookAt.z + sp3C - play->view.eye.z;
            tmpf4 = __sqrtf(SQ(tmpf1) + SQ(tmpf2) + SQ(tmpf3));

            tmpf5 = (tmpf1 / tmpf4) * 5.0f;
            tmpf6 = (tmpf2 / tmpf4) * 5.0f;
            tmpf7 = (tmpf3 / tmpf4) * 5.0f;

            tmpf1 = play->view.eye.x + tmpf5 - self->actor.world.pos.x;
            tmpf2 = play->view.eye.y + tmpf6 - self->actor.world.pos.y;
            tmpf3 = play->view.eye.z + tmpf7 - self->actor.world.pos.z;

            f32 d1 = (tmpf1 / tmpf4) * 5.0f;
            f32 d2 = (tmpf2 / tmpf4) * 5.0f;
            { f32 d3 = (tmpf3 / tmpf4) * 5.0f; self->actor.world.pos.z += d3 * 0.5f; }
            self->actor.world.pos.x += d1;
            self->actor.world.pos.y += d2;
        }
    } else {
        if ((self->actor.parent != NULL) && (self->actor.parent->update != NULL)) {
            ((EnBomBowlPitView*)self->actor.parent)->hasRewardFinishedAppearing = 1;
        }
        Actor_Kill(&self->actor);
    }
}

#endif

GLOBAL_ASM("asm/EnExItem_BowlPrize.s")

void EnExItem_SetupBowlCounter(EnExItem* self, PlayState* play) {
    self->actor.world.rot.y = self->actor.shape.rot.y = 0x4268;
    self->actionFunc = EnExItem_BowlCounter;
}

void EnExItem_BowlCounter(EnExItem* self, PlayState* play) {
    self->actor.shape.rot.y += (s16)(0x800 * 2.0f / 3.0f + 0.5f);
    if (self->killItem) {
        Actor_Kill(&self->actor);
    }
}

void EnExItem_ExitChest(EnExItem* self, PlayState* play) {
    self->actor.shape.rot.y += (s16)(0x1000 * 2.0f / 3.0f + 0.5f);
    if (self->timer != 0) {
        if (self->timer == 1) {
            self->chestKillTimer = 20 * 3.0f / 2.0f;
        }
    } else {
        self->actor.velocity.y = 0.0f;
        if (self->chestKillTimer == 0) {
            Actor_Kill(&self->actor);
        }
    }
    Actor_MoveForward(&self->actor);
}

void EnExItem_FairyMagic(EnExItem* self, PlayState* play) {
    self->actor.shape.rot.y += (s16)(0x800 * 2.0f / 3.0f + 0.5f);
}

#if 0 // draft: 9 words off (near): first-batch order x,y,z, d1..d3 locals then z,x,y adds; see STATUS.md
void EnExItem_TargetPrizeApproach(EnExItem* self, PlayState* play) {
    f32 tmpf4;
    f32 tmpf6;
    f32 tmpf7;
    f32 tmpf5;
    f32 tmpf1;
    f32 tmpf2;
    f32 tmpf3;

    Math_ApproachF(&self->scale, 0.8f, 0.1f, 0.02f);
    if (!self->stopRotate) {
        self->actor.shape.rot.y += (s16)(0x1000 * 2.0f / 3.0f + 0.5f);
        if ((self->prizeRotateTimer == 0) && ((self->actor.shape.rot.y & 0xFFFF) == 0x9000)) {
            self->stopRotate++;
        }
    } else {
        Math_SmoothStepToS(&self->actor.shape.rot.y, -0x4000, 5, 0x1000, 0);
    }

    if (self->timer != 0) {
        if (self->prizeRotateTimer != 0) {
            tmpf1 = play->view.lookAt.x - play->view.eye.x;
            tmpf2 = play->view.lookAt.y - 10.0f - play->view.eye.y;
            tmpf3 = play->view.lookAt.z + 10.0f - play->view.eye.z;
            tmpf4 = __sqrtf(SQ(tmpf1) + SQ(tmpf2) + SQ(tmpf3));

            tmpf5 = (tmpf1 / tmpf4) * 5.0f;
            tmpf6 = (tmpf2 / tmpf4) * 5.0f;
            tmpf7 = (tmpf3 / tmpf4) * 5.0f;

            tmpf1 = play->view.eye.x + tmpf5 - self->actor.world.pos.x;
            tmpf2 = play->view.eye.y - 10.0f + tmpf6 - self->actor.world.pos.y;
            tmpf3 = play->view.eye.z + 10.0f + tmpf7 - self->actor.world.pos.z;

            f32 d1 = (tmpf1 / tmpf4) * 5.0f;
            f32 d2 = (tmpf2 / tmpf4) * 5.0f;
            f32 d3 = (tmpf3 / tmpf4) * 5.0f;
            self->actor.world.pos.z += d3;
            self->actor.world.pos.x += d1;
            self->actor.world.pos.y += d2;
        }
    } else {
        s32 itemId;

        self->actor.draw = NULL;
        Player_SetCsActionWithHaltedActors(play, NULL, 7);
        self->actor.parent = NULL;
        if (CUR_UPG_VALUE(UPG_BULLET_BAG) == 1) {
            itemId = GI_BULLET_BAG_40;
        } else {
            itemId = GI_BULLET_BAG_50;
        }
        GiveItem(&self->actor, play, itemId, 2000.0f, 1000.0f);
        self->actionFunc = EnExItem_TargetPrizeGive;
    }
}
#endif

GLOBAL_ASM("asm/EnExItem_TargetPrizeApproach.s")

void EnExItem_TargetPrizeGive(EnExItem* self, PlayState* play) {
    s32 itemId;

    if (Actor_HasParent(&self->actor, play)) {
        self->actionFunc = EnExItem_TargetPrizeFinish;
    } else {
        itemId = (CUR_UPG_VALUE(UPG_BULLET_BAG) == 2) ? GI_BULLET_BAG_50 : GI_BULLET_BAG_40;

        GiveItem(&self->actor, play, itemId, 2000.0f, 1000.0f);
    }
}

void EnExItem_TargetPrizeFinish(EnExItem* self, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_DONE) && Message_ShouldAdvance(play)) {
        gSaveContext.itemGetInf[1] |= 0x2000;
        Actor_Kill(&self->actor);
    }
}

}
