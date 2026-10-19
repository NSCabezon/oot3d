extern "C" {
#include "z_en_ossan.hpp"

#define FLAGS 0x00000019
#define OSSAN_SHELF_AT(s, i) (*(void**)((u8*)(s) + 0x2A4 + (i) * 4))
#define OSSAN_SHELF_IDX(s) (*(u8*)((u8*)(s) + 0x2FA))

void EnOssan_Init(Actor* actor, GameState* state);
void EnOssan_Destroy(Actor* actor, GameState* state);
void EnOssan_Update(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_Ossan.data.s")
GLOBAL_ASM("../src/game/actors/En_Ossan/data_syms.s")

extern s32 EnOssan_sTable[];
extern u32 EnOssan_sBtnB;
extern s16 EnOssan_sMaskPaymentPrice[];
extern void (*EnOssan_sTalkOwner[])(PlayState*);
extern s16 EnOssan_sMaskStore[];
typedef s16 (*OssanItemReplaceFunc)(s16);
extern s16 EnOssan_sStores[];
extern OssanItemReplaceFunc EnOssan_sReplaceFuncs[];
extern f32 EnOssan_sPosOffsets[];
typedef void (*OssanInitFunc)(EnOssan*, PlayState*);
extern OssanInitFunc EnOssan_sInitFuncs[];
s32 Object_IsLoaded(ObjectContext* objectCtx, s32 slot);
void* ZAR_GetCTXBByIndex(ZARInfo* zar, s32 index);
void FUN_00114bd4(EnOssan* self);
void EnOssan_MainActionFunc(EnOssan* self, PlayState* play);
typedef void (*OssanStateFunc)(EnOssan*, PlayState*, Actor*);
extern OssanStateFunc EnOssan_sStateFunc[];
extern f32 EnOssan_sShopkeeperScale[];
s32 Math_StepToS(s16* pValue, s16 target, s16 step);
void Actor_SetFocus(Actor* actor, f32 yOffset);
extern s16 EnOssan_sObjectIds[];
extern InitChainEntry EnOssan_sInitChain[];
s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
void EnOssan_InitActionFunc(EnOssan* self, PlayState* play);

ActorInit En_Ossan_InitVars = {
    ACTOR_EN_OSSAN,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnOssan),
    (ActorFunc)EnOssan_Init,
    (ActorFunc)EnOssan_Destroy,
    (ActorFunc)EnOssan_Update,
    NULL,
};

static inline s32 EnOssan_TryGetObjBankIndices(EnOssan* self, PlayState* play, s16* objectIds) {
    if (objectIds[1] != 0x1A1) {
        *(s8*)((u8*)self + 0x28D) = Object_GetIndex(&play->objectCtx, objectIds[1]);
        if (*(s8*)((u8*)self + 0x28D) < 0) {
            return 0;
        }
    } else {
        *(s8*)((u8*)self + 0x28D) = -1;
    }
    if (objectIds[2] != 0x1A1) {
        *(s8*)((u8*)self + 0x28E) = Object_GetIndex(&play->objectCtx, objectIds[2]);
        if (*(s8*)((u8*)self + 0x28E) < 0) {
            return 0;
        }
    } else {
        *(s8*)((u8*)self + 0x28E) = -1;
    }
    return 1;
}

void EnOssan_Init(Actor* actor, GameState* state) {
    EnOssan* self = (EnOssan*)actor;
    PlayState* play = (PlayState*)state;
    EnOssan* s2 = self;
    s16* objectIds;

    if (self->actor.params == 6 && gSaveContext.linkAge == 0) {
        self->actor.params = 9;
    }
    *(s32*)((u8*)self + 0x384) = 0;
    *(s32*)((u8*)self + 0x388) = 0;
    *(s32*)((u8*)self + 0x38C) = 0;
    *(s32*)((u8*)self + 0x390) = 0;
    *(s32*)((u8*)self + 0x394) = 0;
    *(s32*)((u8*)self + 0x398) = 0;
    *(s32*)((u8*)self + 0x39C) = 0;
    *(s32*)((u8*)self + 0x3A0) = 0;
    *(s32*)((u8*)self + 0x3A4) = 0;

    if (self->actor.params == 0xA && !(gSaveContext.infTable[7] & 0x40)) {
        Actor_Kill(&self->actor);
        return;
    }
    if (self->actor.params == 1 && gSaveContext.linkAge != 0) {
        Actor_Kill(&self->actor);
        return;
    }
    if (self->actor.params == 2 && !(gSaveContext.eventChkInf[2] & 0x20)) {
        Actor_Kill(&self->actor);
        return;
    }

    objectIds = &EnOssan_sObjectIds[self->actor.params * 3];
    *(s8*)((u8*)self + 0x28C) = Object_GetIndex(&play->objectCtx, objectIds[0]);
    if (*(s8*)((u8*)self + 0x28C) < 0) {
        Actor_Kill(&self->actor);
        return;
    }
    if (!EnOssan_TryGetObjBankIndices(s2, play, objectIds)) {
        Actor_Kill(&self->actor);
        return;
    }
    Actor_ProcessInitChain(&self->actor, EnOssan_sInitChain);
    self->actionFunc = (void (*)(struct EnOssan*, GameState*))EnOssan_InitActionFunc;
}

void EnOssan_Destroy(Actor* actor, GameState* state) {
    EnOssan* self = (EnOssan*)actor;
    PlayState* play = (PlayState*)state;

    ((FaceAnimations*)((u8*)self + 0x3B8))->Destroy();
    SkelAnime_Free((SkelAnime*)((u8*)self + 0x1A4), play);
    FUN_00350f34(actor, (u8*)self + 0x3A8, (u8*)self + 0x3AC, (u8*)self + 0x3B0,
                 (u8*)self + 0x3B4, 0);
    Collider_DestroyCylinder(play, (ColliderCylinder*)((u8*)self + 0x230));
}

void EnOssan_Update(Actor* actor, GameState* state) {
    EnOssan* self = (EnOssan*)actor;

    self->timer++;
    self->actionFunc(self, state);
}

void FUN_00355eb0(PlayState* play, EnOssan* self);
void FUN_0034e37c(PlayState* play, EnOssan* self);
extern u16 gSaveContext_F00[];

void FUN_0014ef94(EnOssan* self, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == 5) && Message_ShouldAdvance(play)) {
        FUN_00355eb0(play, self);
    }
}

void FUN_0039dc38(PlayState* play);

void FUN_002561a4(EnOssan* self, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == 5) && Message_ShouldAdvance(play)) {
        FUN_0034e37c(play, self);
    }
}

void FUN_002a8e5c(EnOssan* self, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == 5) && Message_ShouldAdvance(play)) {
        FUN_00355eb0(play, self);
    }
}

void FUN_00232564(EnOssan* self, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == 5) && Message_ShouldAdvance(play)) {
        gSaveContext_F00[0x17] |= 0x1000;
        FUN_00355eb0(play, self);
    }
}

void FUN_00232500(EnOssan* self, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == 1) && Message_ShouldAdvance(play)) {
        *(s16*)((u8*)self + 0x2A0) = 0x16;
        Message_ContinueTextbox(play, 0x3012);
        gSaveContext_F00[0x17] |= 0x1000;
    }
}


void Matrix_RotateX(MTX34* mtx, s32 mode, f32 angle);

s32 FUN_002a9d9c(Actor* thisx, s32 limbIndex, MTX34* mtx, EnOssan* self) {
    if (limbIndex == 7) {
        Matrix_RotateX(mtx, 1, *(s16*)((u8*)self + 0x292) * 0.000095873802f);
    }
    return 0;
}

s32 FUN_00378fa0(Actor* thisx, s32 limbIndex, MTX34* mtx, EnOssan* self) {
    if (limbIndex == 9) {
        Matrix_RotateX(mtx, 1, *(s16*)((u8*)self + 0x292) * 0.000095873802f);
    }
    return 0;
}



GLOBAL_ASM("asm/EnOssan_InitKokiriShopkeeper.s")

void FUN_002311dc(EnOssan* self) {}


// Blink state (3DS offsets): eyeTextureIdx 0x296, blinkTimer 0x298, blinkFunc 0x29C.
#define OSSAN_EYE_IDX(s) (*(s16*)((u8*)(s) + 0x296))
#define OSSAN_BLINK_TIMER(s) (*(s16*)((u8*)(s) + 0x298))
#define OSSAN_BLINK_FUNC(s) (*(void (**)(EnOssan*))((u8*)(s) + 0x29C))
// Shop shelf (EnTana*) table at 0x2A4, indexed by the byte at 0x2FA; shelf fn ptrs at +0x1C0/+0x1C4.
#define OSSAN_SHELF_PTR(s) (*(void**)((u8*)(s) + 0x2A4 + OSSAN_SHELF_IDX(s) * 4))
typedef void (*OssanShelfFunc)(PlayState*, void*);
#define OSSAN_SHELF_FUNC(sh, off) (*(OssanShelfFunc*)((u8*)(sh) + (off)))

#define OSSAN_CAM(s) (*(f32*)((u8*)(s) + 0x37C))
#define OSSAN_TWEEN(s) (*(f32*)((u8*)(s) + 0x378))
#define OSSAN_STATE(s) (*(u16*)((u8*)(s) + 0x2A0))
#define OSSAN_STATE_PREV(s) (*(u16*)((u8*)(s) + 0x2A2))
#define OSSAN_STICKX(s) (*(s32*)((u8*)(s) + 0x2C8))
#define OSSAN_SHELF_TEXT(s) (*(u16*)((u8*)OSSAN_SHELF_PTR(s) + 0x116))

void FUN_0034e418(EnOssan* self);
void FUN_0034e27c(PlayState* play, EnOssan* self);
void FUN_0034e32c(EnOssan* self, PlayState* play, f32 angle);
void FUN_00104118(EnOssan* self);
void FUN_00114bd4(EnOssan* self);

void FUN_00104118(EnOssan* self) {
    s16 decr = OSSAN_BLINK_TIMER(self) - 1;
    s16 eyeTextureIdxTemp;

    if (decr != 0) {
        OSSAN_BLINK_TIMER(self) = decr;
        return;
    }
    eyeTextureIdxTemp = OSSAN_EYE_IDX(self) + 1;
    if (eyeTextureIdxTemp < 3) {
        OSSAN_EYE_IDX(self) = eyeTextureIdxTemp;
        OSSAN_BLINK_TIMER(self) = 1;
    } else {
        OSSAN_EYE_IDX(self) = 0;
        OSSAN_BLINK_TIMER(self) = (s32)(Rand_ZeroOne() * 60.0f) + 20;
        OSSAN_BLINK_FUNC(self) = FUN_00114bd4;
    }
}

void FUN_00114bd4(EnOssan* self) {
    s16 decr = OSSAN_BLINK_TIMER(self) - 1;

    if (decr != 0) {
        OSSAN_BLINK_TIMER(self) = decr;
    } else {
        OSSAN_BLINK_FUNC(self) = FUN_00104118;
    }
}

void FUN_00271c9c(EnOssan* self, PlayState* play) {
    void* shelf;
    if ((Message_GetState(&play->msgCtx) == 6) && Message_ShouldAdvance(play)) {
        shelf = OSSAN_SHELF_PTR(self);
        FUN_0034e37c(play, self);
        *(u8*)((u8*)self + 0x2F9) = 0;
        *(f32*)((u8*)self + 0x378) = 0.0f;
        OSSAN_SHELF_FUNC(shelf, 0x1C0)(play, shelf);
    }
}

void FUN_001c2e6c(EnOssan* self, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == 5) && Message_ShouldAdvance(play)) {
        *(u16*)((u8*)self + 0x2A0) = *(u16*)((u8*)self + 0x2A2);
        Message_ContinueTextbox(play, *(u16*)((u8*)OSSAN_SHELF_PTR(self) + 0x116));
    }
}

void FUN_0016d1f4(EnOssan* self, PlayState* play) {
    void* shelf;
    if ((Message_GetState(&play->msgCtx) == 5) && Message_ShouldAdvance(play)) {
        *(f32*)((u8*)self + 0x378) = 0.0f;
        FUN_0034e418(self);
        shelf = OSSAN_SHELF_PTR(self);
        OSSAN_SHELF_FUNC(shelf, 0x1C4)(play, shelf);
        *(u16*)((u8*)self + 0x2A0) = *(u16*)((u8*)self + 0x2A2);
        Message_ContinueTextbox(play, *(u16*)((u8*)OSSAN_SHELF_PTR(self) + 0x116));
    }
}

s32 FUN_00287938(Actor* thisx, s32 limbIndex, MTX34* mtx, EnOssan* self) {
    if (limbIndex == 0) {
        Matrix_RotateX(mtx, 1, *(s16*)((u8*)self + 0x292) * 0.000095873802f);
    }
    return 0;
}

void FUN_001c2ecc(EnOssan* self, PlayState* play) {
    Math_ApproachF(&OSSAN_CAM(self), 0.0f, 0.5f, 10.0f);
    if (OSSAN_CAM(self) < 0.5f && OSSAN_CAM(self) > -0.5f) {
        FUN_0034e32c(self, play, 0.0f);
    }
    FUN_0034e32c(self, play, OSSAN_CAM(self));
    if (OSSAN_CAM(self) == 0.0f) {
        FUN_00355eb0(play, self);
    }
}

void FUN_0016cf58(EnOssan* self, PlayState* play) {
    Math_ApproachF(&OSSAN_CAM(self), 30.0f, 0.5f, 10.0f);
    if (OSSAN_CAM(self) > 29.5f) {
        FUN_0034e32c(self, play, 30.0f);
    }
    FUN_0034e32c(self, play, OSSAN_CAM(self));
    if (OSSAN_CAM(self) < 30.0f) {
        OSSAN_STICKX(self) = 0;
        return;
    }
    FUN_0034e32c(self, play, 30.0f);
    FUN_0034e27c(play, self);
    OSSAN_STATE(self) = 6;
    Message_ContinueTextbox(play, OSSAN_SHELF_TEXT(self));
}

void FUN_0019149c(EnOssan* self, PlayState* play) {
    Math_ApproachF(&OSSAN_CAM(self), -30.0f, 0.5f, 10.0f);
    if (OSSAN_CAM(self) < -29.5f) {
        FUN_0034e32c(self, play, -30.0f);
    }
    FUN_0034e32c(self, play, OSSAN_CAM(self));
    if (OSSAN_CAM(self) > -30.0f) {
        OSSAN_STICKX(self) = 0;
        return;
    }
    FUN_0034e32c(self, play, -30.0f);
    FUN_0034e27c(play, self);
    OSSAN_STATE(self) = 7;
    Message_ContinueTextbox(play, OSSAN_SHELF_TEXT(self));
}

void FUN_001910e0(EnOssan* self, PlayState* play) {
    Math_ApproachF(&OSSAN_TWEEN(self), 1.0f, 1.0f, 0.15f);
    if (OSSAN_TWEEN(self) >= 0.85f) {
        OSSAN_TWEEN(self) = 1.0f;
    }
    FUN_0034e418(self);
    if (OSSAN_TWEEN(self) == 1.0f) {
        if (Message_GetState(&play->msgCtx) == 5 && Message_ShouldAdvance(play)) {
            OSSAN_STATE(self) = *(u16*)((u8*)self + 0x2A2);
            Message_ContinueTextbox(play, OSSAN_SHELF_TEXT(self));
        }
    }
}

void FUN_002325bc(EnOssan* self, PlayState* play) {
    Math_ApproachF(&OSSAN_TWEEN(self), 0.0f, 1.0f, 0.15f);
    if (OSSAN_TWEEN(self) <= 0.15f) {
        OSSAN_TWEEN(self) = 0.0f;
    }
    FUN_0034e418(self);
    if (OSSAN_TWEEN(self) == 0.0f) {
        Math_ApproachF(&OSSAN_CAM(self), 0.0f, 0.5f, 10.0f);
        if (OSSAN_CAM(self) < 0.5f && OSSAN_CAM(self) > -0.5f) {
            FUN_0034e32c(self, play, 0.0f);
        }
        FUN_0034e32c(self, play, OSSAN_CAM(self));
        if (OSSAN_CAM(self) == 0.0f) {
            Message_ContinueTextbox(play, 0x3010);
            OSSAN_STATE(self) = 0x14;
        }
    }
}

// Mask shop hello dialog (N64 EnOssan_SetupHelloDialog).
u16 FUN_00372b50(EnOssan* self) {
    *(u8*)((u8*)self + 0x28F) = 8;
    if (self->actor.params == 0xA) {
        if (gSaveContext.inventory.items[SLOT(0x21)] == 0x2C) {
            if (gSaveContext.itemGetInf[3] & 0x800) {
                if (!(gSaveContext.eventChkInf[8] & 0x8000)) {
                    *(u8*)((u8*)self + 0x28F) = 3;
                    return 0x70C6;
                }
                return 0x70AC;
            }
            if (gSaveContext.itemGetInf[3] & 0x400) {
                if (!(gSaveContext.eventChkInf[8] & 0x4000)) {
                    *(u8*)((u8*)self + 0x28F) = 1;
                    return 0x70C5;
                }
                return 0x70AC;
            }
            if (gSaveContext.itemGetInf[3] & 0x200) {
                if (!(gSaveContext.eventChkInf[8] & 0x2000)) {
                    *(u8*)((u8*)self + 0x28F) = 2;
                    return 0x70C4;
                }
                return 0x70AC;
            }
            if (gSaveContext.itemGetInf[3] & 0x100) {
                if (!(gSaveContext.eventChkInf[8] & 0x1000)) {
                    *(u8*)((u8*)self + 0x28F) = 0;
                    return 0x70A5;
                }
                return 0x70AC;
            }
        } else {
            if (gSaveContext.itemGetInf[3] & 0x800) {
                return 0x70AC;
            } else if (!(gSaveContext.itemGetInf[3] & 0x400) && !(gSaveContext.itemGetInf[2] & 0x10) &&
                       !(gSaveContext.itemGetInf[3] & 0x100)) {
                if (!(gSaveContext.itemGetInf[2] & 8)) {
                    return 0x70A1;
                } else {
                    *(u8*)((u8*)self + 0x28F) = 4;
                    return 0x70A6;
                }
            } else {
                return 0x70C7;
            }
        }
    }
    return 0x9E;
}

// Shelf column search (N64 EnOssan_CursorLeftRight-like): find the next shelf slot with an item.
void FUN_00338fa0(EnOssan* self) {
    s32 stick = *(s32*)((u8*)self + 0x2CC);
    u8 idx = OSSAN_SHELF_IDX(self);
    u8 base;
    u8 i;

    if (stick < 0) {
        base = idx & 0xFE;
        if (OSSAN_SHELF_AT(self, base) != NULL) {
            OSSAN_SHELF_IDX(self) = base;
        } else {
        i = base + 2;
        if (base < 4) {
            if (i >= 4) {
                i = 0;
            }
            while (i != base) {
                if (OSSAN_SHELF_AT(self, i) != NULL) {
                    OSSAN_SHELF_IDX(self) = i;
                    return;
                }
                i += 2;
                if (i >= 4) {
                    i = 0;
                }
            }
        } else {
            if (i >= 8) {
                i = 4;
            }
            while (i != base) {
                if (OSSAN_SHELF_AT(self, i) != NULL) {
                    OSSAN_SHELF_IDX(self) = i;
                    return;
                }
                i += 2;
                if (i >= 8) {
                    i = 4;
                }
            }
        }
        }
    } else if (stick > 0) {
        base = idx | 1;
        if (OSSAN_SHELF_AT(self, base) != NULL) {
            OSSAN_SHELF_IDX(self) = base;
        } else {
        i = base + 2;
        if (base < 4) {
            if (i >= 4) {
                i = 1;
            }
            while (i != base) {
                if (OSSAN_SHELF_AT(self, i) != NULL) {
                    OSSAN_SHELF_IDX(self) = i;
                    return;
                }
                i += 2;
                if (i >= 4) {
                    i = 1;
                }
            }
        } else {
            if (i >= 8) {
                i = 5;
            }
            while (i != base) {
                if (OSSAN_SHELF_AT(self, i) != NULL) {
                    OSSAN_SHELF_IDX(self) = i;
                    return;
                }
                i += 2;
                if (i >= 8) {
                    i = 5;
                }
            }
        }
        }
    }
}

void FUN_00340a1c(PlayState* play, s32 mode);
void FUN_0034c294(PlayState* play, s32 action);
s32 FUN_00369f3c(PlayState* play);
void FUN_00367c7c(PlayState* play, u16 textId, Actor* actor);
void FUN_0036bbd0(Actor* actor, PlayState* play, s32 exchangeItem, f32 range);
void FUN_0034e1d0(PlayState* play, Actor* actor);

// N64 EnOssan_State_ContinueShoppingPrompt.
static inline void EnOssan_ContinueShopping(EnOssan* self, PlayState* play, Actor* player) {
    u16 one = 1;
    *(s16*)((u8*)player + 0xBE) += 0x8000;
    *(u32*)((u8*)player + 0x1714) |= 0x20000000;
    FUN_00340a1c(play, 2);
    FUN_00367c7c(play, *(u16*)((u8*)self + 0x116), &self->actor);
    *(u16*)((u8*)gGameInfo + 0x400 + 0xD2) = one;
    *(s16*)((u8*)self + 0x294) = 0;
    *(s16*)((u8*)self + 0x292) = 0;
    FUN_0034c294(play, 0x10);
    FUN_0034e32c(self, play, 0.0f);
    FUN_00355eb0(play, self);
    FUN_0036bbd0(&self->actor, play, -1, 100.0f);
}

void FUN_002561e8(EnOssan* self, PlayState* play, Actor* player) {
    u8 talkState = Message_GetState(&play->msgCtx);

    if (talkState == 4) {
        if (Message_ShouldAdvance(play)) {
            void* item;
            OSSAN_TWEEN(self) = 0.0f;
            FUN_0034e418(self);
            item = OSSAN_SHELF_PTR(self);
            OSSAN_SHELF_FUNC(item, 0x1C4)(play, item);
            if (!(*(u32*)((u8*)play + 0x18) & EnOssan_sBtnB) && !FUN_00369f3c(play)) {
                EnOssan_ContinueShopping(self, play, player);
            } else {
                FUN_0034e1d0(play, &self->actor);
            }
        }
    } else if (talkState == 5 && Message_ShouldAdvance(play)) {
        void* item;
        OSSAN_TWEEN(self) = 0.0f;
        FUN_0034e418(self);
        item = OSSAN_SHELF_PTR(self);
        OSSAN_SHELF_FUNC(item, 0x1C4)(play, item);
        EnOssan_ContinueShopping(self, play, player);
    }
}

// N64 EnOssan_State_SelectMaskItem.
void FUN_0034e37c(PlayState* play, EnOssan* self);

static inline void EnOssan_BackToBrowse(EnOssan* self, PlayState* play) {
    OSSAN_STATE(self) = *(u16*)((u8*)self + 0x2A2);
    Message_ContinueTextbox(play, OSSAN_SHELF_TEXT(self));
}

static inline s32 EnOssan_TestCancelOption(EnOssan* self, PlayState* play, u32* input) {
    if (input[1] & EnOssan_sBtnB) {
        EnOssan_BackToBrowse(self, play);
        return 1;
    }
    return 0;
}

void FUN_0016d038(EnOssan* self, PlayState* play) {
    u8 talkState = Message_GetState(&play->msgCtx);
    u32* input = (u32*)((u8*)play + 0x14);
    void* item = OSSAN_SHELF_PTR(self);

    Math_ApproachF(&OSSAN_TWEEN(self), 1.0f, 1.0f, 0.15f);
    if (OSSAN_TWEEN(self) >= 0.85f) {
        OSSAN_TWEEN(self) = 1.0f;
    }
    FUN_0034e418(self);
    if (OSSAN_TWEEN(self) == 1.0f) {
        if (talkState == 5) {
            if (Message_ShouldAdvance(play)) {
                EnOssan_BackToBrowse(self, play);
            }
        } else if (talkState == 4 && !EnOssan_TestCancelOption(self, play, input) && Message_ShouldAdvance(play)) {
            switch (FUN_00369f3c(play)) {
                case 0:
                    switch (*(s16*)((u8*)item + 0x1C)) {
                        case 0x1E:
                            gSaveContext.itemGetInf[2] |= 8;
                            break;
                        case 0x1F:
                            gSaveContext.itemGetInf[2] |= 0x20;
                            break;
                        case 0x20:
                            gSaveContext.itemGetInf[2] |= 0x10;
                            break;
                        case 0x21:
                            gSaveContext.itemGetInf[2] |= 0x40;
                            break;
                        case 0x22:
                        case 0x23:
                        case 0x24:
                        case 0x25:
                            break;
                    }
                    FUN_0034e37c(play, self);
                    *(u8*)((u8*)self + 0x2F9) = 0;
                    OSSAN_TWEEN(self) = 0.0f;
                    OSSAN_SHELF_FUNC(item, 0x1C0)(play, item);
                    break;
                case 1:
                    EnOssan_BackToBrowse(self, play);
                    break;
            }
        }
    }
}

// N64 EnOssan_HandleCanBuyLonLonMilk + EnOssan_State_SelectMilkBottle.
static inline void EnOssan_SetStateCantGetItem(PlayState* play, EnOssan* self, u16 textId) {
    Message_ContinueTextbox(play, textId);
    OSSAN_STATE(self) = 0xE;
}

static inline void EnOssan_SetStateQuickBuyDialog(PlayState* play, EnOssan* self, u16 textId) {
    Message_ContinueTextbox(play, textId);
    OSSAN_STATE(self) = 0x17;
}

typedef s32 (*OssanCanBuyFunc)(PlayState*, void*);

static inline void EnOssan_HandleCanBuyLonLonMilk(PlayState* play, EnOssan* self) {
    void* item = OSSAN_SHELF_PTR(self);

    switch ((*(OssanCanBuyFunc*)((u8*)item + 0x1D0))(play, item)) {
        case 0:
            Message_ContinueTextbox(play, 0x9C);
            OSSAN_STATE(self) = 0x12;
            *(u8*)((u8*)self + 0x2F9) = 0;
            break;
        case 1:
            OSSAN_SHELF_FUNC(item, 0x1D4)(play, item);
            EnOssan_SetStateQuickBuyDialog(play, self, 0x98);
            *(u8*)((u8*)self + 0x2F9) = 0;
            OSSAN_TWEEN(self) = 0.0f;
            OSSAN_SHELF_FUNC(item, 0x1C0)(play, item);
            break;
        case 3:
            EnOssan_SetStateCantGetItem(play, self, 0x96);
            break;
        case 4:
            EnOssan_SetStateCantGetItem(play, self, 0x85);
            break;
    }
}

void FUN_00190cc4(EnOssan* self, PlayState* play) {
    u32* input = (u32*)((u8*)play + 0x14);

    Math_ApproachF(&OSSAN_TWEEN(self), 1.0f, 1.0f, 0.15f);
    if (OSSAN_TWEEN(self) >= 0.85f) {
        OSSAN_TWEEN(self) = 1.0f;
    }
    FUN_0034e418(self);
    if (OSSAN_TWEEN(self) == 1.0f && Message_GetState(&play->msgCtx) == 4 &&
        !EnOssan_TestCancelOption(self, play, input) && Message_ShouldAdvance(play)) {
        switch (FUN_00369f3c(play)) {
            case 0:
                EnOssan_HandleCanBuyLonLonMilk(play, self);
                break;
            case 1:
                EnOssan_BackToBrowse(self, play);
                break;
        }
    }
}

// N64 EnOssan_HandleCanBuyWeirdEgg + EnOssan_State_SelectWeirdEgg.
static inline void EnOssan_HandleCanBuyWeirdEgg(PlayState* play, EnOssan* self) {
    void* item = OSSAN_SHELF_PTR(self);

    switch ((*(OssanCanBuyFunc*)((u8*)item + 0x1D0))(play, item)) {
        case 0:
            FUN_0034e37c(play, self);
            *(u8*)((u8*)self + 0x2F9) = 0;
            OSSAN_TWEEN(self) = 0.0f;
            OSSAN_SHELF_FUNC(item, 0x1C0)(play, item);
            break;
        case 1:
            OSSAN_SHELF_FUNC(item, 0x1D4)(play, item);
            EnOssan_SetStateQuickBuyDialog(play, self, 0x9A);
            *(u8*)((u8*)self + 0x2F9) = 0;
            OSSAN_TWEEN(self) = 0.0f;
            OSSAN_SHELF_FUNC(item, 0x1C0)(play, item);
            break;
        case 2:
            Audio_PlaySoundGeneral(0x0100048B, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
            EnOssan_SetStateCantGetItem(play, self, 0x9D);
            break;
        case 4:
            Audio_PlaySoundGeneral(0x0100048B, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
            EnOssan_SetStateCantGetItem(play, self, 0x85);
            break;
    }
}

void FUN_00190e98(EnOssan* self, PlayState* play) {
    u32* input = (u32*)((u8*)play + 0x14);

    Math_ApproachF(&OSSAN_TWEEN(self), 1.0f, 1.0f, 0.15f);
    if (OSSAN_TWEEN(self) >= 0.85f) {
        OSSAN_TWEEN(self) = 1.0f;
    }
    FUN_0034e418(self);
    if (OSSAN_TWEEN(self) == 1.0f && Message_GetState(&play->msgCtx) == 4 &&
        !EnOssan_TestCancelOption(self, play, input) && Message_ShouldAdvance(play)) {
        switch (FUN_00369f3c(play)) {
            case 0:
                EnOssan_HandleCanBuyWeirdEgg(play, self);
                break;
            case 1:
                EnOssan_BackToBrowse(self, play);
                break;
        }
    }
}

// N64 EnOssan_State_StartConversation (with EnOssan_TryPaybackMask inlined).
void FUN_00376a60(s16 amount);

static inline s32 EnOssan_TestEndInteraction(EnOssan* self, PlayState* play, u32* input) {
    if (input[1] & EnOssan_sBtnB) {
        FUN_0034e1d0(play, &self->actor);
        return 1;
    }
    return 0;
}

static inline void EnOssan_TryPaybackMask(EnOssan* self, PlayState* play) {
    s16 price = EnOssan_sMaskPaymentPrice[*(u8*)((u8*)self + 0x28F)];

    if (gSaveContext.rupees < price) {
        Message_ContinueTextbox(play, 0x70A8);
        *(u8*)((u8*)self + 0x290) = 1;
        *(u8*)((u8*)self + 0x28F) = 5;
    } else {
        FUN_00376a60(-price);
        if (*(u8*)((u8*)self + 0x28F) == 3) {
            gSaveContext.eventChkInf[8] |= 0x8000;
            Message_ContinueTextbox(play, 0x70A9);
            *(u8*)((u8*)self + 0x28F) = 6;
            return;
        }
        if (*(u8*)((u8*)self + 0x28F) == 0) {
            gSaveContext.eventChkInf[8] |= 0x1000;
        } else if (*(u8*)((u8*)self + 0x28F) == 1) {
            gSaveContext.eventChkInf[8] |= 0x4000;
        } else if (*(u8*)((u8*)self + 0x28F) == 2) {
            gSaveContext.eventChkInf[8] |= 0x2000;
        }
        Message_ContinueTextbox(play, 0x70A7);
        *(u8*)((u8*)self + 0x28F) = 8;
    }
    OSSAN_STATE(self) = 1;
}

// FUN_0016d280: 4 words off (ROM loads rupees before the price table address).
#if 0
void FUN_0016d280(EnOssan* self, PlayState* play) {
    u8 dialogState = Message_GetState(&play->msgCtx);
    u32* input = (u32*)((u8*)play + 0x14);

    if (self->actor.params == 0xA && dialogState == 4) {
        if (!EnOssan_TestEndInteraction(self, play, input) && Message_ShouldAdvance(play)) {
            switch (FUN_00369f3c(play)) {
                case 0:
                    FUN_00355eb0(play, self);
                    break;
                case 1:
                    FUN_0034e1d0(play, &self->actor);
                    break;
            }
        }
    } else if (dialogState == 5 && Message_ShouldAdvance(play)) {
        Audio_PlaySoundGeneral(0x0100049D, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        switch (*(u8*)((u8*)self + 0x28F)) {
            case 6:
                Message_ContinueTextbox(play, 0x70AA);
                OSSAN_STATE(self) = 0x19;
                return;
            case 4:
                FUN_0034e1d0(play, &self->actor);
                return;
            case 0:
            case 1:
            case 2:
            case 3:
                EnOssan_TryPaybackMask(self, play);
                return;
            case 5:
                FUN_003716f0(play, 0x1D1, 0x14, 0x2E);
                return;
        }
        if (!EnOssan_TestEndInteraction(self, play, input)) {
            FUN_00355eb0(play, self);
        }
    }
}
#endif

// N64 EnOssan_HasPlayerSelectedItem.
#define OSSAN_SFX(id) Audio_PlaySoundGeneral((id), NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0)

s32 FUN_00339100(PlayState* play, EnOssan* self, u32* input) {
    void* item = OSSAN_SHELF_PTR(self);

    if (EnOssan_TestEndInteraction(self, play, input)) {
        return 1;
    }
    if (Message_ShouldAdvance(play)) {
        if (*(u16*)((u8*)item + 0x1C) != 0x26 && *(u16*)((u8*)item + 0x1BC) == 0) {
            OSSAN_STATE_PREV(self) = OSSAN_STATE(self);
            Message_ContinueTextbox(play, *(u16*)((u8*)OSSAN_SHELF_PTR(self) + 0x1B4));
            *(s32*)((u8*)self + 0x330) = 0;
            *(s32*)((u8*)self + 0x368) = 0;
            switch (*(s16*)((u8*)item + 0x1C)) {
                case 0x1E:
                case 0x1F:
                case 0x20:
                case 0x21:
                case 0x22:
                case 0x23:
                case 0x24:
                case 0x25:
                    OSSAN_SFX(0x0100048D);
                    *(u8*)((u8*)self + 0x2F9) = 0;
                    OSSAN_STATE(self) = 0x18;
                    break;
                case 0x11:
                    OSSAN_SFX(0x0100048D);
                    *(u8*)((u8*)self + 0x2F9) = 0;
                    OSSAN_STATE(self) = 0xA;
                    break;
                case 0x12:
                    OSSAN_SFX(0x0100048D);
                    *(u8*)((u8*)self + 0x2F9) = 0;
                    OSSAN_STATE(self) = 0xB;
                    break;
                case 0x13:
                case 0x14:
                    OSSAN_SFX(0x0100048B);
                    *(u8*)((u8*)self + 0x2F9) = 0;
                    OSSAN_STATE(self) = 0xC;
                    break;
                case 3:
                case 6:
                case 0x2D:
                case 0x2E:
                case 0x2F:
                    OSSAN_SFX(0x0100048D);
                    *(u8*)((u8*)self + 0x2F9) = 0;
                    OSSAN_STATE(self) = 0xD;
                    break;
                default:
                    OSSAN_SFX(0x0100048D);
                    *(u8*)((u8*)self + 0x2F9) = 0;
                    OSSAN_STATE(self) = 9;
                    break;
            }
        } else {
            OSSAN_SFX(0x0100048B);
        }
        return 1;
    }
    return 0;
}

// N64 EnOssan_HandleCanBuyItem + EnOssan_State_ItemSelected.
static inline void EnOssan_HandleCanBuyItem(PlayState* play, EnOssan* self) {
    void* item = OSSAN_SHELF_PTR(self);

    switch ((*(OssanCanBuyFunc*)((u8*)item + 0x1D0))(play, item)) {
        case 0:
            if (*(u16*)((u8*)item + 0x1C) == 0xC && (gSaveContext.infTable[7] & 0x40)) {
                Message_ContinueTextbox(play, 0x71B2);
                OSSAN_STATE(self) = 0x1A;
            } else {
                FUN_0034e37c(play, self);
                *(u8*)((u8*)self + 0x2F9) = 0;
                OSSAN_TWEEN(self) = 0.0f;
                OSSAN_SHELF_FUNC(item, 0x1C0)(play, item);
            }
            break;
        case 1:
            OSSAN_SHELF_FUNC(item, 0x1D4)(play, item);
            EnOssan_SetStateQuickBuyDialog(play, self, 0x84);
            *(u8*)((u8*)self + 0x2F9) = 0;
            OSSAN_TWEEN(self) = 0.0f;
            OSSAN_SHELF_FUNC(item, 0x1C0)(play, item);
            break;
        case 2:
            OSSAN_SFX(0x0100048B);
            EnOssan_SetStateCantGetItem(play, self, 0x86);
            break;
        case 3:
            OSSAN_SFX(0x0100048B);
            EnOssan_SetStateCantGetItem(play, self, 0x96);
            break;
        case 4:
            OSSAN_SFX(0x0100048B);
            EnOssan_SetStateCantGetItem(play, self, 0x85);
            break;
        case 5:
            OSSAN_SFX(0x0100048B);
            EnOssan_SetStateCantGetItem(play, self, 0x86);
            break;
    }
}

void FUN_0016cc8c(EnOssan* self, PlayState* play, Actor* player) {
    u32* input = (u32*)((u8*)play + 0x14);

    Math_ApproachF(&OSSAN_TWEEN(self), 1.0f, 1.0f, 0.15f);
    if (OSSAN_TWEEN(self) >= 0.85f) {
        OSSAN_TWEEN(self) = 1.0f;
    }
    FUN_0034e418(self);
    if (OSSAN_TWEEN(self) == 1.0f && Message_GetState(&play->msgCtx) == 4 &&
        !EnOssan_TestCancelOption(self, play, input) && Message_ShouldAdvance(play)) {
        switch (FUN_00369f3c(play)) {
            case 0:
                EnOssan_HandleCanBuyItem(play, self);
                break;
            case 1:
                EnOssan_BackToBrowse(self, play);
                break;
        }
    }
}

// N64 EnOssan_State_FacingShopkeeper (EnOssan_SetCursorIndexFromNeutral inlined).
static inline u8 EnOssan_SetCursorIndexFromNeutral(EnOssan* self, u8 shelfOffset) {
    u8 i;

    if (OSSAN_SHELF_IDX(self) & 1) {
        for (i = shelfOffset + 1; i < shelfOffset + 4; i += 2) {
            if (OSSAN_SHELF_AT(self, i) != NULL) {
                return i;
            }
        }
        for (i = shelfOffset; i < shelfOffset + 4; i += 2) {
            if (OSSAN_SHELF_AT(self, i) != NULL) {
                return i;
            }
        }
    } else {
        for (i = shelfOffset; i < shelfOffset + 4; i += 2) {
            if (OSSAN_SHELF_AT(self, i) != NULL) {
                return i;
            }
        }
        for (i = shelfOffset + 1; i < shelfOffset + 4; i += 2) {
            if (OSSAN_SHELF_AT(self, i) != NULL) {
                return i;
            }
        }
    }
    return 0xFF;
}

static inline s32 EnOssan_FacingShopkeeperDialogResult(EnOssan* self, PlayState* play) {
    switch (FUN_00369f3c(play)) {
        case 0:
            OSSAN_STATE(self) = 3;
            EnOssan_sTalkOwner[self->actor.params](play);
            *(s32*)((u8*)self + 0x330) = 0;
            *(s32*)((u8*)self + 0x368) = 0;
            return 1;
        case 1:
            FUN_0034e1d0(play, &self->actor);
            return 1;
        default:
            return 0;
    }
}

void FUN_0019157c(EnOssan* self, PlayState* play) {
    u32* input = (u32*)((u8*)play + 0x14);
    u8 nextIndex;

    if (Message_GetState(&play->msgCtx) == 4 && !EnOssan_TestEndInteraction(self, play, input)) {
        if (Message_ShouldAdvance(play) && EnOssan_FacingShopkeeperDialogResult(self, play)) {
            OSSAN_SFX(0x0100048D);
            return;
        }
        if (*(s32*)((u8*)self + 0x2C8) < 0) {
            nextIndex = EnOssan_SetCursorIndexFromNeutral(self, 4);
            if (nextIndex != 0xFF) {
                OSSAN_SHELF_IDX(self) = nextIndex;
                OSSAN_STATE(self) = 4;
                *(s32*)((u8*)self + 0x330) = 0;
                OSSAN_SFX(0x0100048E);
            }
        } else if (*(s32*)((u8*)self + 0x2C8) > 0) {
            nextIndex = EnOssan_SetCursorIndexFromNeutral(self, 0);
            if (nextIndex != 0xFF) {
                OSSAN_SHELF_IDX(self) = nextIndex;
                OSSAN_STATE(self) = 5;
                *(s32*)((u8*)self + 0x368) = 0;
                OSSAN_SFX(0x0100048E);
            }
        }
    }
}

// N64 EnOssan_State_BrowseRightShelf.
void FUN_00338fa0(EnOssan* self);
void FUN_0034e27c(PlayState* play, EnOssan* self);

static inline u8 EnOssan_CursorRight(EnOssan* self, u8 cursorIndex, u8 shelfSlotMin) {
    u8 c = shelfSlotMin + 4;

    while (cursorIndex >= shelfSlotMin && cursorIndex < c) {
        cursorIndex -= 2;
        if (cursorIndex >= shelfSlotMin && cursorIndex < c) {
            if (OSSAN_SHELF_AT(self, cursorIndex) != NULL) {
                return cursorIndex;
            }
        }
    }
    return 0xFF;
}

static inline u8 EnOssan_CursorLeft(EnOssan* self, u8 cursorIndex, u8 shelfSlotMax) {
    while (cursorIndex < shelfSlotMax) {
        cursorIndex += 2;
        if ((cursorIndex < shelfSlotMax) && OSSAN_SHELF_AT(self, cursorIndex) != NULL) {
            return cursorIndex;
        }
    }
    return 0xFF;
}

static inline s32 EnOssan_ReturnItemToShelf(EnOssan* self) {
    Math_ApproachF(&OSSAN_TWEEN(self), 0.0f, 1.0f, 0.15f);
    if (OSSAN_TWEEN(self) <= 0.15f) {
        OSSAN_TWEEN(self) = 0.0f;
    }
    FUN_0034e418(self);
    if (OSSAN_TWEEN(self) == 0.0f) {
        return 1;
    } else {
        return 0;
    }
}

static inline void EnOssan_SetLookToShopkeeperFromShelf(PlayState* play, EnOssan* self) {
    OSSAN_SFX(0x0100048E);
    *(u8*)((u8*)self + 0x2F9) = 0;
    OSSAN_STATE(self) = 8;
}

void FUN_002165ec(EnOssan* self, PlayState* play) {
    u32* input = (u32*)((u8*)play + 0x14);
    u8 prevIndex = OSSAN_SHELF_IDX(self);
    u8 nextIndex;

    if (!EnOssan_ReturnItemToShelf(self)) {
        *(s16*)((u8*)self + 0x28A) = 3;
        return;
    }
    if (*(s16*)((u8*)self + 0x28A) != 0) {
        (*(s16*)((u8*)self + 0x28A))--;
        return;
    }
    *(u8*)((u8*)self + 0x2F9) = 0xFF;
    *(s32*)((u8*)self + 0x330) = 1;
    FUN_0034e27c(play, self);
    if (Message_GetState(&play->msgCtx) == 5 && !FUN_00339100(play, self, input)) {
        if (*(u8*)((u8*)self + 0x2D0) != 0) {
            if (*(s32*)((u8*)self + 0x2C8) < 0) {
                nextIndex = EnOssan_CursorRight(self, OSSAN_SHELF_IDX(self), 0);
                if (nextIndex != 0xFF) {
                    OSSAN_SHELF_IDX(self) = nextIndex;
                } else {
                    EnOssan_SetLookToShopkeeperFromShelf(play, self);
                    return;
                }
            } else if (*(s32*)((u8*)self + 0x2C8) > 0) {
                nextIndex = EnOssan_CursorLeft(self, OSSAN_SHELF_IDX(self), 4);
                if (nextIndex != 0xFF) {
                    OSSAN_SHELF_IDX(self) = nextIndex;
                }
            }
        } else {
            if (*(s32*)((u8*)self + 0x2C8) < 0 && *(s32*)((u8*)self + 0x2C8) < -500) {
                nextIndex = EnOssan_CursorRight(self, OSSAN_SHELF_IDX(self), 0);
                if (nextIndex != 0xFF) {
                    OSSAN_SHELF_IDX(self) = nextIndex;
                } else {
                    EnOssan_SetLookToShopkeeperFromShelf(play, self);
                    return;
                }
            } else if (*(s32*)((u8*)self + 0x2C8) > 0 && *(s32*)((u8*)self + 0x2C8) > 500) {
                nextIndex = EnOssan_CursorLeft(self, OSSAN_SHELF_IDX(self), 4);
                if (nextIndex != 0xFF) {
                    OSSAN_SHELF_IDX(self) = nextIndex;
                }
            }
        }
        FUN_00338fa0(self);
        if (OSSAN_SHELF_IDX(self) != prevIndex) {
            Message_ContinueTextbox(play, OSSAN_SHELF_TEXT(self));
            OSSAN_SFX(0x0100048E);
        }
    }
}

// N64 EnOssan_HandleCanBuyBombs + EnOssan_BuyGoronCityBombs + EnOssan_State_SelectBombs.
void FUN_0016cc8c(EnOssan* self, PlayState* play, Actor* player);

static inline void EnOssan_HandleCanBuyBombs(PlayState* play, EnOssan* self) {
    void* item = OSSAN_SHELF_PTR(self);

    switch ((*(OssanCanBuyFunc*)((u8*)item + 0x1D0))(play, item)) {
        case 0:
        case 1:
            OSSAN_SHELF_FUNC(item, 0x1D4)(play, item);
            EnOssan_SetStateQuickBuyDialog(play, self, 0x84);
            *(u8*)((u8*)self + 0x2F9) = 0;
            OSSAN_TWEEN(self) = 0.0f;
            OSSAN_SHELF_FUNC(item, 0x1C0)(play, item);
            break;
        case 2:
            OSSAN_SFX(0x0100048B);
            EnOssan_SetStateCantGetItem(play, self, 0x86);
            break;
        case 4:
            OSSAN_SFX(0x0100048B);
            EnOssan_SetStateCantGetItem(play, self, 0x85);
            break;
    }
}

static inline void EnOssan_BuyGoronCityBombs(PlayState* play, EnOssan* self) {
    if (gSaveContext.linkAge != 0) {
        if (!(gSaveContext.eventChkInf[2] & 0x20)) {
            if (gSaveContext.infTable[15] & 0x1000) {
                EnOssan_SetStateCantGetItem(play, self, 0x302E);
            } else {
                *(s32*)((u8*)self + 0x330) = 0;
                *(s32*)((u8*)self + 0x368) = 0;
                *(u8*)((u8*)self + 0x2F9) = 0;
                OSSAN_STATE(self) = 0x13;
            }
        } else {
            EnOssan_HandleCanBuyBombs(play, self);
        }
    } else {
        EnOssan_HandleCanBuyBombs(play, self);
    }
}

void FUN_001911a4(EnOssan* self, PlayState* play, Actor* player) {
    u32* input = (u32*)((u8*)play + 0x14);

    Math_ApproachF(&OSSAN_TWEEN(self), 1.0f, 1.0f, 0.15f);
    if (OSSAN_TWEEN(self) >= 0.85f) {
        OSSAN_TWEEN(self) = 1.0f;
    }
    FUN_0034e418(self);
    if (OSSAN_TWEEN(self) == 1.0f) {
        if (self->actor.params != 8) {
            FUN_0016cc8c(self, play, player);
            return;
        }
        if (Message_GetState(&play->msgCtx) == 4 && !EnOssan_TestCancelOption(self, play, input) &&
            Message_ShouldAdvance(play)) {
            switch (FUN_00369f3c(play)) {
                case 0:
                    EnOssan_BuyGoronCityBombs(play, self);
                    break;
                case 1:
                    EnOssan_BackToBrowse(self, play);
                    break;
            }
        }
    }
}

// N64 EnOssan_State_BrowseLeftShelf.
void FUN_001ff2dc(EnOssan* self, PlayState* play) {
    u32* input = (u32*)((u8*)play + 0x14);
    u8 prevIndex = OSSAN_SHELF_IDX(self);
    u8 nextIndex;

    if (!EnOssan_ReturnItemToShelf(self)) {
        *(s16*)((u8*)self + 0x28A) = 3;
        return;
    }
    if (*(s16*)((u8*)self + 0x28A) != 0) {
        (*(s16*)((u8*)self + 0x28A))--;
        return;
    }
    *(u8*)((u8*)self + 0x2F9) = 0xFF;
    *(s32*)((u8*)self + 0x368) = 1;
    FUN_0034e27c(play, self);
    if (Message_GetState(&play->msgCtx) == 5 && !FUN_00339100(play, self, input)) {
        if (*(u8*)((u8*)self + 0x2D0) != 0) {
            if (*(s32*)((u8*)self + 0x2C8) > 0) {
                nextIndex = EnOssan_CursorRight(self, OSSAN_SHELF_IDX(self), 4);
                if (nextIndex != 0xFF) {
                    OSSAN_SHELF_IDX(self) = nextIndex;
                } else {
                    EnOssan_SetLookToShopkeeperFromShelf(play, self);
                    return;
                }
            } else if (*(s32*)((u8*)self + 0x2C8) < 0) {
                nextIndex = EnOssan_CursorLeft(self, OSSAN_SHELF_IDX(self), 8);
                if (nextIndex != 0xFF) {
                    OSSAN_SHELF_IDX(self) = nextIndex;
                }
            }
        } else {
            if (*(s32*)((u8*)self + 0x2C8) > 0 && *(s32*)((u8*)self + 0x2C8) > 500) {
                nextIndex = EnOssan_CursorRight(self, OSSAN_SHELF_IDX(self), 4);
                if (nextIndex != 0xFF) {
                    OSSAN_SHELF_IDX(self) = nextIndex;
                } else {
                    EnOssan_SetLookToShopkeeperFromShelf(play, self);
                    return;
                }
            } else if (*(s32*)((u8*)self + 0x2C8) < 0 && *(s32*)((u8*)self + 0x2C8) < -500) {
                nextIndex = EnOssan_CursorLeft(self, OSSAN_SHELF_IDX(self), 8);
                if (nextIndex != 0xFF) {
                    OSSAN_SHELF_IDX(self) = nextIndex;
                }
            }
        }
        FUN_00338fa0(self);
        if (OSSAN_SHELF_IDX(self) != prevIndex) {
            Message_ContinueTextbox(play, OSSAN_SHELF_TEXT(self));
            OSSAN_SFX(0x0100048E);
        }
    }
}

// N64 EnOssan_State_ItemPurchased (EnOssan_UpdateShopOfferings inlined).
s32 FUN_0025355c(Actor* item, PlayState* play);

// func_80AC5DAC: 5 words off (ROM keeps &gSaveContext+0xF00 in r0 and the flags in r1 for the 0x8000 set).
void func_80AC5DAC(EnOssan* self, PlayState* play) {
    void* item;
    void* itemTemp;
    s32 i;
    PlayState* play2 = play;
    if (Message_GetState(&play2->msgCtx) == 6 && Message_ShouldAdvance(play)) {
        EnOssan* s2 = self;
        if (s2->actor.params == 0xA) {
            itemTemp = OSSAN_SHELF_PTR(self);
            OSSAN_TWEEN(self) = 0.0f;
            FUN_0034e418(self);
            item = OSSAN_SHELF_PTR(self);
            OSSAN_SHELF_FUNC(item, 0x1C4)(play, item);
            if (*(u16*)((u8*)itemTemp + 0x1C) == 0x22 && !(gSaveContext.itemGetInf[3] & 0x8000)) {
                gSaveContext.itemGetInf[3] |= 0x8000;
                Message_ContinueTextbox(play, 0x70AB);
                *(u8*)((u8*)self + 0x28F) = 4;
                if (self->actor.params == 0xA) {
                    s16* shopItem = EnOssan_sMaskStore;
                    OssanItemReplaceFunc* replaceFuncs = (OssanItemReplaceFunc*)((u8*)EnOssan_sMaskStore + 0x40);
                    s16* shelfRot;

                    i = 0;
                    shelfRot = (s16*)((u8*)replaceFuncs - 0x36C);
                    for (; i < 8; i++, shopItem += 4) {
                        if (shopItem[0] >= 0 && OSSAN_SHELF_AT(self, i) == NULL) {
                            s16 params = replaceFuncs[shopItem[0]](shopItem[0]);

                            if (params >= 0) {
                                Actor* shelves = *(Actor**)((u8*)self + 0x2C4);
                                OSSAN_SHELF_AT(self, i) = Actor_Spawn(
                                    &play->actorCtx, play, ACTOR_EN_GIRLA, shelves->world.pos.x + shopItem[1],
                                    shelves->world.pos.y + shopItem[2], shelves->world.pos.z + shopItem[3],
                                    *(s16*)((u8*)shelves + 0xBC), *(s16*)((u8*)shelves + 0xBE) + shelfRot[i],
                                    *(s16*)((u8*)shelves + 0xC0), params, 1);
                            }
                        }
                    }
                }
                OSSAN_STATE(self) = 1;
            } else {
                FUN_0034e1d0(play, &self->actor);
            }
            for (i = 0; i < 8; i++) {
                if (OSSAN_SHELF_AT(self, i) != NULL) {
                    FUN_0025355c((Actor*)OSSAN_SHELF_AT(self, i), play);
                }
            }
        } else {
            item = OSSAN_SHELF_PTR(self);
            OSSAN_SHELF_FUNC(item, 0x1D8)(play, item);
            OSSAN_STATE(self) = 0x11;
            Message_ContinueTextbox(play, 0x6B);
        }
    }
}

// N64 EnOssan_MainActionFunc (UpdateJoystickInputState / UpdateItemSelectedProperty /
// UpdateStickDirectionPromptAnim / UpdateCursorAnim inlined).
#define OSS_F32(s, o) (*(f32*)((u8*)(s) + (o)))
#define OSS_S32(s, o) (*(s32*)((u8*)(s) + (o)))
#define OSS_U8(s, o) (*(u8*)((u8*)(s) + (o)))

// EnOssan_MainActionFunc: ~227 words off (size 1020 vs 1032), register allocation of the hoisted constants
// (ROM: 0 in r0, 1 in r1, stick in r3/r2) and fused vmla for the stick prompt offsets.
#if 0
static inline void EnOssan_UpdateJoystickInputState(PlayState* play, EnOssan* self) {
    s32 stickX = (s32)OSS_F32(play, 0x3C);
    s32 stickY = (s32)OSS_F32(play, 0x40);

    OSS_U8(self, 0x2D1) = OSS_U8(self, 0x2D0) = 0;
    if (OSS_S32(self, 0x2C8) == 0) {
        if (stickX > 30 || stickX < -30) {
            OSS_S32(self, 0x2C8) = stickX;
            OSS_U8(self, 0x2D0) = 1;
        }
    } else if (stickX <= 30 && stickX >= -30) {
        OSS_S32(self, 0x2C8) = 0;
    } else if (OSS_S32(self, 0x2C8) * stickX < 0) {
        OSS_S32(self, 0x2C8) = stickX;
        OSS_U8(self, 0x2D0) = 1;
    } else {
        OSS_S32(self, 0x2C8) += stickX;
        if (OSS_S32(self, 0x2C8) > 2000) {
            OSS_S32(self, 0x2C8) = 2000;
        } else if (OSS_S32(self, 0x2C8) < -2000) {
            OSS_S32(self, 0x2C8) = -2000;
        }
    }
    if (OSS_S32(self, 0x2CC) == 0) {
        if (stickY > 30 || stickY < -30) {
            OSS_S32(self, 0x2CC) = stickY;
            OSS_U8(self, 0x2D1) = 1;
        }
    } else if (stickY <= 30 && stickY >= -30) {
        OSS_S32(self, 0x2CC) = 0;
    } else if (OSS_S32(self, 0x2CC) * stickY < 0) {
        OSS_S32(self, 0x2CC) = stickY;
        OSS_U8(self, 0x2D1) = 1;
    } else {
        OSS_S32(self, 0x2CC) += stickY;
        if (OSS_S32(self, 0x2CC) > 2000) {
            OSS_S32(self, 0x2CC) = 2000;
        } else if (OSS_S32(self, 0x2CC) < -2000) {
            OSS_S32(self, 0x2CC) = -2000;
        }
    }
}

static inline void EnOssan_UpdateItemSelectedProperty(EnOssan* self) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (OSSAN_SHELF_AT(self, i) != NULL) {
            if (OSSAN_STATE(self) != 9 && OSSAN_STATE(self) != 0xA && OSSAN_STATE(self) != 0xB &&
                OSSAN_STATE(self) != 0xC && OSSAN_STATE(self) != 0xD && OSSAN_STATE(self) != 0x18 &&
                OSSAN_STATE(self) != 0xE && OSS_U8(self, 0x2F9) == 0) {
                *(s16*)((u8*)OSSAN_SHELF_AT(self, i) + 0x1C8) = 0;
            } else {
                if (OSSAN_SHELF_IDX(self) == i) {
                    *(s16*)((u8*)OSSAN_SHELF_AT(self, i) + 0x1C8) = 1;
                } else {
                    *(s16*)((u8*)OSSAN_SHELF_AT(self, i) + 0x1C8) = 0;
                }
            }
        }
    }
}

static inline void EnOssan_UpdateStickDirectionPromptAnim(EnOssan* self) {
    f32 t = OSS_F32(self, 0x36C);
    f32 stickT = OSS_F32(self, 0x370);
    s32 r;
    s32 g;

    if (OSS_U8(self, 0x374) == 0) {
        t += 0.05f;
        if (t > 1.0f) {
            t = 1.0f;
            OSS_U8(self, 0x374) = 1;
        }
    } else {
        t -= 0.05f;
        if (t < 0.0f) {
            t = 0.0f;
            OSS_U8(self, 0x374) = 0;
        }
    }
    OSS_F32(self, 0x36C) = t;
    if (OSS_U8(self, 0x375) == 0) {
        stickT += 0.05f;
        if (stickT > 1.0f) {
            stickT = 1.0f;
            OSS_U8(self, 0x375) = 1;
        }
    } else {
        stickT = 0.0f;
        OSS_U8(self, 0x375) = 0;
    }
    OSS_F32(self, 0x370) = stickT;
    r = (s32)(t * -191.0f);
    g = (s32)(t * -163.0f);
    OSS_S32(self, 0x318) = (u8)(0 - r);
    OSS_S32(self, 0x31C) = (u8)(0 - g);
    OSS_S32(self, 0x320) = 180;
    OSS_S32(self, 0x314) = 255;
    OSS_S32(self, 0x350) = (u8)(0 - r);
    OSS_S32(self, 0x354) = (u8)(0 - g);
    OSS_S32(self, 0x358) = 180;
    OSS_S32(self, 0x34C) = 255;
    OSS_F32(self, 0x35C) = 388.0f;
    OSS_F32(self, 0x324) = 12.0f;
    OSS_F32(self, 0x328) = OSS_F32(self, 0x360) = 96.0f;
    OSS_F32(self, 0x344) = 368.0f + 8.0f * stickT;
    OSS_F32(self, 0x30C) = 32.0f - 8.0f * stickT;
    OSS_F32(self, 0x310) = OSS_F32(self, 0x348) = 96.0f;
}

static inline void EnOssan_UpdateCursorAnim(EnOssan* self) {
    f32 t = OSS_F32(self, 0x2F4);

    if (OSS_U8(self, 0x2F8) == 0) {
        t += 0.05f;
        if (t >= 1.0f) {
            t = 1.0f;
            OSS_U8(self, 0x2F8) = 1;
        }
    } else {
        t -= 0.05f;
        if (t <= 0.0f) {
            t = 0.0f;
            OSS_U8(self, 0x2F8) = 0;
        }
    }
    OSS_F32(self, 0x2F4) = t;
    OSS_S32(self, 0x2E4) = 0;
    OSS_S32(self, 0x2E8) = (u8)(80 - (s32)(t * 80.0f));
    OSS_S32(self, 0x2EC) = 255;
    OSS_S32(self, 0x2F0) = 255;
}

void EnOssan_MainActionFunc(EnOssan* self, PlayState* play) {
    Actor* player = *(Actor**)((u8*)play + 0x20AC);

    (*(void (**)(EnOssan*))((u8*)self + 0x29C))(self);
    EnOssan_UpdateJoystickInputState(play, self);
    EnOssan_UpdateItemSelectedProperty(self);
    EnOssan_UpdateStickDirectionPromptAnim(self);
    EnOssan_UpdateCursorAnim(self);
    Math_StepToS((s16*)((u8*)self + 0x292), *(s16*)((u8*)self + 0x294), 0x320);

    if (player != NULL) {
        EnOssan_sStateFunc[OSSAN_STATE(self)](self, play, player);
    }
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(play, &self->actor, 26.0f, 10.0f, 0.0f, 5);
    Actor_SetFocus(&self->actor, 90.0f);
    Actor_SetScale(&self->actor, EnOssan_sShopkeeperScale[self->actor.params]);
    if (*(void**)((u8*)self + 0x22C) != NULL) {
        (*(void (**)(EnOssan*, PlayState*))((u8*)self + 0x22C))(self, play);
    }
    FUN_003731e0((SkelAnime*)((u8*)self + 0x1A4));
}
#endif
GLOBAL_ASM("asm/EnOssan_MainActionFunc.s")

// N64 EnOssan_InitActionFunc (EnOssan_SpawnItemsOnShelves inlined).
// EnOssan_InitActionFunc: 55 words off (structure matches; register numbering: ROM play=sb, self+0x200=r7, zero=r8).
#if 0
void EnOssan_InitActionFunc(EnOssan* self, PlayState* play) {
    s16* items;
    ZARInfo* zar;
    ZARInfo* zar2;
    void* cmab;
    s32 i;
    s32 stickAnim;
    ObjectStatus* status;

    if (!Object_IsLoaded(&play->objectCtx, *(s8*)((u8*)self + 0x28C))) {
        return;
    }
    if (*(s8*)((u8*)self + 0x28D) >= 0 && !Object_IsLoaded(&play->objectCtx, *(s8*)((u8*)self + 0x28D))) {
        return;
    }
    if (*(s8*)((u8*)self + 0x28E) >= 0 && !Object_IsLoaded(&play->objectCtx, *(s8*)((u8*)self + 0x28E))) {
        return;
    }
    self->actor.flags &= ~0x10;
    status = play->GetObjectStatus(self->actor.objBankIndex);
    zar = &status->zarInfo;
    FUN_00372f38(&self->actor, play, (u8*)self + 0x3A8, 0x54, (u8*)self + 0x3AC, 0x54, (u8*)self + 0x3B0, 0x54,
                 (u8*)self + 0x3B4, 0x54, 0);
    cmab = zar->GetCMABByIndex(0x2B);
    for (i = 0; i < 4; i++) {
        self->skelModels[i]->unk_0C->FUN_00372d94(cmab);
        self->skelModels[i]->unk_0C->animType = 1;
        self->skelModels[i]->unk_0C->animSpeed = 2.0f;
    }
    self->actor.objBankIndex = *(u8*)((u8*)self + 0x28C);
    status = play->GetObjectStatus(self->actor.objBankIndex);
    *(ZARInfo**)((u8*)self + 0x380) = &status->zarInfo;
    *(Actor**)((u8*)self + 0x2C4) = Actor_Find(&play->actorCtx, 0xC2, 6);
    if (*(Actor**)((u8*)self + 0x2C4) == NULL) {
        return;
    }
    self->actor.world.pos.x += EnOssan_sPosOffsets[self->actor.params * 3];
    self->actor.world.pos.y += EnOssan_sPosOffsets[self->actor.params * 3 + 1];
    self->actor.world.pos.z += EnOssan_sPosOffsets[self->actor.params * 3 + 2];
    items = &EnOssan_sStores[self->actor.params * 32];
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 20.0f);
    EnOssan_sInitFuncs[self->actor.params](self, play);
    *(void**)((u8*)self + 0x3A0) = ZAR_GetCTXBByIndex(zar, 0x1D);
    *(void**)((u8*)self + 0x3A4) = ZAR_GetCTXBByIndex(zar, 0x1E);
    self->actor.textId = FUN_00372b50(self);
    self->actor.colChkInfo.mass = 0xFF;
    self->actor.colChkInfo.cylRadius = 0x32;
    OSSAN_STATE(self) = 0;
    OSS_S32(self, 0x2C8) = OSS_S32(self, 0x2CC) = 0;
    OSSAN_SHELF_IDX(self) = 0;
    OSS_F32(self, 0x2E0) = 0.1f;
    OSS_S32(self, 0x2EC) = 0xFF;
    OSS_S32(self, 0x2E8) = 0x50;
    OSS_S32(self, 0x2E4) = 0;
    OSS_S32(self, 0x2F0) = 0xFF;
    OSS_F32(self, 0x2F4) = 0.0f;
    OSS_U8(self, 0x2F8) = 0;
    OSS_U8(self, 0x2F9) = 0;
    OSS_U8(self, 0x290) = 0;
    OSS_S32(self, 0x2FC) = 0xFF;
    OSS_S32(self, 0x300) = 0xFF;
    OSS_S32(self, 0x304) = 0xFF;
    OSS_S32(self, 0x308) = 0xB4;
    OSS_F32(self, 0x30C) = 32.0f;
    OSS_F32(self, 0x310) = 96.0f;
    OSS_S32(self, 0x314) = 0xFF;
    OSS_S32(self, 0x318) = 0;
    OSS_S32(self, 0x320) = 0xB4;
    OSS_S32(self, 0x31C) = 0;
    OSS_F32(self, 0x324) = 12.0f;
    OSS_F32(self, 0x328) = 96.0f;
    OSS_F32(self, 0x32C) = 1.0f;
    OSS_S32(self, 0x334) = 0xFF;
    OSS_S32(self, 0x330) = 0;
    OSS_S32(self, 0x338) = 0xFF;
    OSS_S32(self, 0x33C) = 0xFF;
    OSS_S32(self, 0x340) = 0xB4;
    OSS_F32(self, 0x344) = 368.0f;
    OSS_F32(self, 0x348) = 96.0f;
    OSS_S32(self, 0x34C) = 0xFF;
    OSS_S32(self, 0x350) = 0;
    OSS_S32(self, 0x358) = 0xB4;
    OSS_S32(self, 0x354) = 0;
    OSS_F32(self, 0x35C) = 388.0f;
    OSS_F32(self, 0x360) = 96.0f;
    OSS_F32(self, 0x364) = 1.0f;
    OSS_S32(self, 0x368) = 0;
    OSS_F32(self, 0x36C) = 0.0f;
    OSS_F32(self, 0x370) = 0.0f;
    OSS_U8(self, 0x374) = 0;
    OSS_U8(self, 0x375) = 0;
    OSS_F32(self, 0x378) = 0.0f;
    Actor_SetScale(&self->actor, EnOssan_sShopkeeperScale[self->actor.params]);
    {
        OssanItemReplaceFunc* replaceFuncs = EnOssan_sReplaceFuncs;
        s16* shelfRot = (s16*)((u8*)replaceFuncs - 0x36C);

        for (i = 0; i < 8; i++, items += 4) {
            if (items[0] < 0) {
                OSSAN_SHELF_AT(self, i) = NULL;
            } else {
                s16 params = replaceFuncs[items[0]](items[0]);

                if (params < 0) {
                    OSSAN_SHELF_AT(self, i) = NULL;
                } else {
                    Actor* shelves = *(Actor**)((u8*)self + 0x2C4);
                    OSSAN_SHELF_AT(self, i) = Actor_Spawn(
                        &play->actorCtx, play, ACTOR_EN_GIRLA, shelves->world.pos.x + items[1],
                        shelves->world.pos.y + items[2], shelves->world.pos.z + items[3],
                        *(s16*)((u8*)shelves + 0xBC), *(s16*)((u8*)shelves + 0xBE) + shelfRot[i],
                        *(s16*)((u8*)shelves + 0xC0), params, 1);
                }
            }
        }
    }
    *(s16*)((u8*)self + 0x294) = 0;
    *(s16*)((u8*)self + 0x292) = 0;
    *(s16*)((u8*)self + 0x298) = 0x14;
    *(s16*)((u8*)self + 0x296) = 0;
    *(void**)((u8*)self + 0x29C) = (void*)FUN_00114bd4;
    self->actor.flags &= ~1;
    self->actionFunc = (void (*)(struct EnOssan*, GameState*))EnOssan_MainActionFunc;
}
#endif
GLOBAL_ASM("asm/EnOssan_InitActionFunc.s")


// Shopkeeper Draw functions (N64 EnOssan_Draw*): face anims, skeleton, shadow/cursor, then the per-shop tail.
void FUN_0035ddb4(PlayState* play, EnOssan* self);
void FUN_0035e04c(PlayState* play, EnOssan* self, void* pos, u8 drawCursor, f32 z);

#define OSSAN_DRAW(NAME, EYE, OVERRIDE, TAIL)                                                                  \
    void NAME(Actor* thisx, PlayState* play) {                                                                 \
        EnOssan* self = (EnOssan*)thisx;                                                                       \
        FaceAnimations* fa = (FaceAnimations*)((u8*)thisx + 0x3B8);                                            \
        fa->FUN_0035e3a4(0, EYE);                                                                              \
        ((FaceAnimations*)((u8*)self + 0x3B8))->FUN_0035e330();                                                \
        SkelAnime_DrawOpa((SkelAnime*)((u8*)self + 0x1A4), (MTX34*)((u8*)self + 0x148), OVERRIDE, NULL, thisx, \
                          0);                                                                                  \
        FUN_0035e04c(play, self, (u8*)self + 0x2D4, *(u8*)((u8*)self + 0x2F9), *(f32*)((u8*)self + 0x2E0));   \
        TAIL;                                                                                                  \
    }

OSSAN_DRAW(EnOssan_DrawKokiriShopkeeper, OSSAN_EYE_IDX(self), NULL, FUN_0035ddb4(play, self))
OSSAN_DRAW(FUN_00131028, OSSAN_EYE_IDX(self), NULL, FUN_0035ddb4(play, self))
OSSAN_DRAW(FUN_001c27c4, OSSAN_EYE_IDX(self), NULL, FUN_0035ddb4(play, self))
OSSAN_DRAW(FUN_003b400c, OSSAN_EYE_IDX(self), NULL, FUN_0035ddb4(play, self))
OSSAN_DRAW(FUN_001c283c, OSSAN_EYE_IDX(self), (OverrideLimbDrawOpa)FUN_00378fa0, FUN_0035ddb4(play, self))
OSSAN_DRAW(FUN_002121fc, OSSAN_EYE_IDX(self), (OverrideLimbDrawOpa)FUN_00287938, FUN_0035ddb4(play, self))
OSSAN_DRAW(FUN_00293210, OSSAN_EYE_IDX(self), (OverrideLimbDrawOpa)FUN_002a9d9c, FUN_0035ddb4(play, self))
OSSAN_DRAW(FUN_0035dd44, *(u8*)((u8*)self + 0x290), NULL, FUN_0035ddb4(play, self))

// Per-shopkeeper skeleton init (N64 EnOssan_InitXxxShopkeeper). Shape that gives the ROM's push {r4-r8} + 0x18
// frame: a named `SkelAnime* skel` local assigned before the FUN_00353e78 call and used ONLY there (armcc first
// gives it a callee-saved reg, then rematerialises `add r2,r4,#0x1a4` but keeps the reg in the push list; the
// 2nd SkelAnime use must be the plain expression). Order: cmb, store, zar, skel, unk_178, table entry.
void* ZAR_GetCMBByIndex(void* zar, s32 index);
void FUN_00353e78(void* zar, PlayState* play, SkelAnime* skel, void* cmb, void* p, s32 anim, void* jointTable,
                  void* morphTable, s32 limbCount);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 index);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 index, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void FUN_0036932c(void* p, s32 n);

#define OSSAN_CMB(s, i) (*(void**)((u8*)(s) + 0x384 + (i) * 4))
#define OSSAN_ZAR(s) (*(void**)((u8*)(s) + 0x380))
#define OSSAN_SKEL(s) ((SkelAnime*)((u8*)(s) + 0x1A4))
#define OSSAN_FACE(s) ((FaceAnimations*)((u8*)(s) + 0x3B8))

// Index passed as a parameter would merge the literal 0 (index 0 callers) with the zero args, so the shopkeepers
// that pass a table animation spell the body out; the ones passing anim 0 share this helper.
static inline void EnOssan_InitSkel(EnOssan* self, PlayState* play, s32 i) {
    void* cmb = ZAR_GetCMBByIndex(OSSAN_ZAR(self), 0);
    void* zar;
    SkelAnime* skel;
    void* p;

    OSSAN_CMB(self, i) = cmb;
    zar = OSSAN_ZAR(self);
    skel = OSSAN_SKEL(self);
    p = self->actor.unk_178;
    FUN_00353e78(zar, play, skel, cmb, p, 0, NULL, NULL, 0);
}

#define OSSAN_INIT_SKEL_ANIM(self, play, i)                           \
    void* cmb = ZAR_GetCMBByIndex(OSSAN_ZAR(self), 0);                \
    void* zar;                                                        \
    SkelAnime* skel;                                                  \
    void* p;                                                          \
    s32 anim;                                                         \
                                                                      \
    OSSAN_CMB(self, i) = cmb;                                         \
    zar = OSSAN_ZAR(self);                                            \
    skel = OSSAN_SKEL(self);                                          \
    p = self->actor.unk_178;                                          \
    anim = EnOssan_sTable[i];                                         \
    FUN_00353e78(zar, play, skel, cmb, p, anim, NULL, NULL, 0)

void FUN_001e1ac0(EnOssan* self, PlayState* play) {
    OSSAN_INIT_SKEL_ANIM(self, play, 0);
    SkelAnime_SetFaceAnimations(OSSAN_FACE(self), OSSAN_SKEL(self), 1, -1, -1);
    self->actor.draw = (ActorFunc)FUN_002121fc;
    *(void**)((u8*)self + 0x22C) = NULL;
}

void FUN_001c3314(EnOssan* self, PlayState* play) {
    OSSAN_INIT_SKEL_ANIM(self, play, 0);
    SkelAnime_SetFaceAnimations(OSSAN_FACE(self), OSSAN_SKEL(self), 1, -1, -1);
    FUN_0036932c(*(void**)((u8*)self + 0x1CC), 1);
    self->actor.draw = (ActorFunc)FUN_00293210;
    *(void**)((u8*)self + 0x22C) = NULL;
}

void FUN_00285d84(EnOssan* self, PlayState* play) {
    f32 end;

    EnOssan_InitSkel(self, play, 2);
    end = Animation_GetLastFrame(OSSAN_SKEL(self), EnOssan_sTable[2]);
    SkelAnime_SetFaceAnimations(OSSAN_FACE(self), OSSAN_SKEL(self), 0, -1, -1);
    Animation_Change_00375c08(OSSAN_SKEL(self), EnOssan_sTable[2], 1.0f, 0.0f, end, 0, 0.0f);
    self->actor.draw = (ActorFunc)FUN_001c27c4;
    *(void**)((u8*)self + 0x22C) = (void*)FUN_002311dc;
}

void FUN_00285e5c(EnOssan* self, PlayState* play) {
    f32 end;

    EnOssan_InitSkel(self, play, 3);
    end = Animation_GetLastFrame(OSSAN_SKEL(self), EnOssan_sTable[3]);
    SkelAnime_SetFaceAnimations(OSSAN_FACE(self), OSSAN_SKEL(self), 0, -1, -1);
    Animation_Change_00375c08(OSSAN_SKEL(self), EnOssan_sTable[3], 1.0f, 0.0f, end, 0, 0.0f);
    self->actor.draw = (ActorFunc)FUN_001c283c;
    *(void**)((u8*)self + 0x22C) = (void*)FUN_002311dc;
}

void FUN_002148a4(EnOssan* self, PlayState* play) {
    OSSAN_INIT_SKEL_ANIM(self, play, 4);
    SkelAnime_SetFaceAnimations(OSSAN_FACE(self), OSSAN_SKEL(self), 0, -1, -1);
    self->actor.draw = (ActorFunc)FUN_00131028;
    *(void**)((u8*)self + 0x22C) = NULL;
}

void FUN_0016cbf8(EnOssan* self, PlayState* play) {
    OSSAN_INIT_SKEL_ANIM(self, play, 5);
    SkelAnime_SetFaceAnimations(OSSAN_FACE(self), OSSAN_SKEL(self), 0, -1, -1);
    self->actor.draw = (ActorFunc)FUN_0035dd44;
    *(void**)((u8*)self + 0x22C) = NULL;
}

void FUN_0016d4d0(EnOssan* self, PlayState* play) {
    OSSAN_INIT_SKEL_ANIM(self, play, 6);
    SkelAnime_SetFaceAnimations(OSSAN_FACE(self), OSSAN_SKEL(self), 0, -1, -1);
    self->actor.draw = (ActorFunc)FUN_003b400c;
    *(void**)((u8*)self + 0x22C) = NULL;
}

void FUN_00190c68(EnOssan* self, PlayState* play) {
    if (Actor_HasParent(&self->actor, play)) {
        self->actor.parent = NULL;
        OSSAN_STATE(self) = 0x10;
        return;
    }
    GiveItem(&self->actor, play, *(s32*)((u8*)OSSAN_SHELF_PTR(self) + 0x1B8), 120.0f, 120.0f);
}

void FUN_00271d10(EnOssan* self, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == 5) && Message_ShouldAdvance(play)) {
        gSaveContext.itemGetInf[2] |= 0x400;
        OSSAN_SHELF_IDX(self) = 2;
        FUN_0034e37c(play, self);
    }
}

// N64 EnOssan_EndInteraction.
void FUN_00371680(PlayState* play, s32 a, s32 b);
void FUN_00340a1c(PlayState* play, s32 mode);
void Interface_ChangeAlpha(s32 alpha);

void FUN_0034e1d0(PlayState* play, Actor* actor) {
    EnOssan* self = (EnOssan*)actor;
    Actor* player = *(Actor**)((u8*)play + 0x20AC);

    *(u16*)((u8*)gGameInfo + 0x400 + 0xD2) = 0;
    Actor_ProcessTalkRequest(&self->actor, play);
    FUN_00371680(play, 4, 0);
    *(u32*)((u8*)player + 0x1714) &= ~0x20000000;
    FUN_00340a1c(play, 1);
    Interface_ChangeAlpha(0x32);
    *(u8*)((u8*)self + 0x2F9) = 0;
    *(s32*)((u8*)self + 0x330) = 0;
    *(s32*)((u8*)self + 0x368) = 0;
    FUN_0034e32c(self, play, 0.0f);
    self->actor.textId = FUN_00372b50(self);
    OSSAN_STATE(self) = 0;
}

// N64 EnOssan_State_Idle (EnOssan_SetStateStartShopping inlined).
void FUN_0034c294(PlayState* play, s32 action);

void FUN_002168b4(EnOssan* self, PlayState* play, Actor* player) {
    *(s16*)((u8*)self + 0x294) = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        *(u32*)((u8*)player + 0x1714) |= 0x20000000;
        FUN_0039dc38(play);
        *(u16*)((u8*)gGameInfo + 0x400 + 0xD2) = 1;
        *(s16*)((u8*)self + 0x294) = 0;
        *(s16*)((u8*)self + 0x292) = 0;
        FUN_0034c294(play, 0x10);
        FUN_0034e32c(self, play, 0.0f);
        OSSAN_STATE(self) = 1;
    } else if (self->actor.xzDistToPlayer < 100.0f) {
        FUN_0036bb28(&self->actor, play, 100.0f);
    }
}

#if 0
// NON-MATCHING drafts (near misses, see ws/En_Ossan/STATUS.md). GLOBAL_ASM stays in the functions files.

// EnOssan_InitKokiriShopkeeper (2 words, R12): with the skel-local recipe the frame matches; residual is the load
// order of zar (r8) / unk_178 (r7). Loading p first gives the right regs but the wrong order; zar first swaps the
// regs. ~430 decl/statement/alias (s2, play2, thisx) combinations tried. Real decls are above FUN_001e1ac0.
#if 0
void EnOssan_InitKokiriShopkeeper(EnOssan* self, PlayState* play) {
    void* cmb = ZAR_GetCMBByIndex(OSSAN_ZAR(self), 0);
    SkelAnime* skel;
    void* zar;
    void* p;
    f32 end;

    OSSAN_CMB(self, 1) = cmb;
    p = self->actor.unk_178;
    skel = OSSAN_SKEL(self);
    zar = OSSAN_ZAR(self);
    FUN_00353e78(zar, play, skel, cmb, p, 0, NULL, NULL, 0);
    FUN_0036932c(*(void**)((u8*)self + 0x1CC), 3);
    end = Animation_GetLastFrame(OSSAN_SKEL(self), EnOssan_sTable[1]);
    SkelAnime_SetFaceAnimations(OSSAN_FACE(self), OSSAN_SKEL(self), 0, -1, -1);
    Animation_Change_00375c08(OSSAN_SKEL(self), EnOssan_sTable[1], 1.0f, 0.0f, end, 0, 0.0f);
    self->actor.draw = (ActorFunc)EnOssan_DrawKokiriShopkeeper;
    *(void**)((u8*)self + 0x22C) = (void*)FUN_002311dc;
    Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, 0x18, self->actor.world.pos.x, self->actor.world.pos.y,
                       self->actor.world.pos.z, 0, 0, 0, 3);
}
#endif

// FUN_0034e27c (34 words, prologue/copy order), FUN_0034e418 (52 words, table lerp). Not in the G55 Sonnet list.
#if 0
void FUN_0034e27c(PlayState* play, EnOssan* self) {
    Vec3f* pos = (Vec3f*)((u8*)self + 0x2D4);
    f32* tbl = (f32*)0x0052D4B4;
    f32 dx, dy, dz, inv, k;

    *pos = *(Vec3f*)((u8*)OSSAN_SHELF_PTR(self) + 0x3C);
    dx = *(f32*)((u8*)play + 0x1B8) - pos->x;
    dy = *(f32*)((u8*)play + 0x1BC) - *(f32*)((u8*)self + 0x2D8);
    dz = *(f32*)((u8*)play + 0x1C0) - *(f32*)((u8*)self + 0x2DC);
    inv = 1.0f / sqrtf(dx * dx + dy * dy + dz * dz);
    k = tbl[7];
    pos->x += dx * inv * k;
    *(f32*)((u8*)self + 0x2D8) += dy * inv * k;
    *(f32*)((u8*)self + 0x2DC) += dz * inv * k;
}

void FUN_0034e418(EnOssan* self) {
    u8 idx = OSSAN_SHELF_IDX(self);
    s16 params = self->actor.params;
    u8* table = (u8*)0x0052D590;
    s16* e = (s16*)(table + params * 64 + idx * 8);
    f32* v = (f32*)(table + 0x388 + (idx >> 2) * 12);
    f32 t = *(f32*)((u8*)self + 0x378);
    f32 x = (f32)e[1] + (v[0] - (f32)e[1]) * t;
    f32 y = (f32)e[2] + (v[1] - (f32)e[2]) * t;
    f32 z = (f32)e[3] + (v[2] - (f32)e[3]) * t;
    f32* base = (f32*)*(void**)((u8*)self + 0x2C4);
    f32* sh = (f32*)OSSAN_SHELF_PTR(self);
    sh[0x28 / 4] = base[0x28 / 4] + x;
    sh[0x2C / 4] = base[0x2C / 4] + y;
    sh[0x30 / 4] = base[0x30 / 4] + z;
    sh[0x1FC / 4] = 1.0f - t;
}
#endif

// FUN_0025355c: 5 words off (ROM keeps play in r4 for the whole function; ours uses ip). EQU EnOssan_sMaskItems = 0x00524F34.
typedef struct { Actor actor; char pad[0x18]; s16 isInvisible; } GirlAView;
extern s16 EnOssan_sMaskItems[];
s32 FUN_0025355c(Actor* item, PlayState* play) {
    GirlAView* g = (GirlAView*)item;
    if ((u32)(item->params - 0x1E) <= 7) {
        s32 idx = item->params - 0x1E;
        if (gSaveContext.inventory.items[SLOT(0x21)] == EnOssan_sMaskItems[idx]) {
            g->isInvisible = 1;
            item->draw = NULL;
            if ((u32)(item->params - 0x1E) <= 7) {
                item->textId = 0xBD;
            }
        } else {
            FUN_00357298(play, item);
        }
        return 1;
    }
    return 0;
}
#endif

}
