extern "C" {
#include "z_en_girla.hpp"

#define FLAGS 0x00000019

void EnGirlA_Init(EnGirlA* self, PlayState* play);
void EnGirlA_Destroy(Actor* actor, GameState* state);
void EnGirlA_Update(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_GirlA.data.s")

GLOBAL_ASM("../src/game/actors/En_GirlA/data_syms.s")

extern s16 sShieldDiscounts[];

typedef struct ShopItemEntry {
    /* 0x00 */ s16 objID;
    /* 0x02 */ char unk_02[0x0C];
    /* 0x0E */ u16 itemDescTextId;
    /* 0x10 */ char unk_10[0x20];
} ShopItemEntry; // size = 0x30

extern ShopItemEntry sShopItemEntries[];
s32 EnGirlA_TryChangeShopItem(EnGirlA* self);
void EnGirlA_InitItem(EnGirlA* self, PlayState* play);
void EnGirlA_InitializeItemAction(EnGirlA* self, PlayState* play);
void EnGirlA_Draw(Actor* thisx, GameState* state);
s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);


ActorInit En_GirlA_InitVars = {
    ACTOR_EN_GIRLA,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnGirlA),
    (ActorFunc)EnGirlA_Init,
    (ActorFunc)EnGirlA_Destroy,
    (ActorFunc)EnGirlA_Update,
    NULL,
};

void Rupees_ChangeBy(s16 rupeeChange);
void Item_Give(PlayState* play, s32 itemId);
void Health_ChangeBy(PlayState* play, s16 healthChange);
void FUN_00355830(s32 item, s16 amount);
void FUN_00357248(struct SkeletonAnimationModel** skelAnimModel);
void FUN_00369128(PlayState* play);
u8 Item_CheckObtainability(s32 itemId);
s32 Inventory_HasEmptyBottle(void);

s32 FUN_00103398(PlayState* play, EnGirlA* self) {
    if ((s16)gSaveContext.healthCapacity == gSaveContext.health) {
        return 2;
    }
    if (gSaveContext.rupees < self->basePrice) {
        return 4;
    }
    return 1;
}

static s32 EnGirlA_CanBuyItem(PlayState* play, EnGirlA* self, s32 itemId) {
    if (gSaveContext.rupees < self->basePrice) {
        return 4;
    }
    if (Item_CheckObtainability(itemId) == ITEM_NONE) {
        return 0;
    }
    return 1;
}

static s32 EnGirlA_CanBuyBottle(PlayState* play, EnGirlA* self, s32 itemId) {
    if (!Inventory_HasEmptyBottle()) {
        return 3;
    }
    if (gSaveContext.rupees < self->basePrice) {
        return 4;
    }
    if (Item_CheckObtainability(itemId) == ITEM_NONE) {
        return 0;
    }
    return 1;
}

s32 FUN_00114c78(PlayState* play, EnGirlA* self) {
    return EnGirlA_CanBuyItem(play, self, 0x1A);
}

s32 FUN_00130d80(PlayState* play, EnGirlA* self) {
    return EnGirlA_CanBuyItem(play, self, 0x23);
}

s32 FUN_001002c4(PlayState* play, EnGirlA* self) {
    return EnGirlA_CanBuyBottle(play, self, 0x17);
}

s32 FUN_0010320c(PlayState* play, EnGirlA* self) {
    return EnGirlA_CanBuyBottle(play, self, 0x16);
}

s32 FUN_00103264(PlayState* play, EnGirlA* self) {
    return EnGirlA_CanBuyBottle(play, self, 0x20);
}

s32 FUN_00104198(PlayState* play, EnGirlA* self) {
    return EnGirlA_CanBuyBottle(play, self, 0x15);
}

s32 FUN_00149340(PlayState* play, EnGirlA* self) {
    if (gSaveContext.inventory.equipment & gBitFlags[4]) {
        return 2;
    }
    if (gSaveContext.rupees < self->basePrice) {
        return 4;
    }
    if (Item_CheckObtainability(ITEM_SHIELD_DEKU) == ITEM_NONE) {
        return 0;
    }
    return 1;
}

s32 FUN_0014d9e0(PlayState* play, EnGirlA* self) {
    if (gSaveContext.inventory.equipment & gBitFlags[5]) {
        return 2;
    }
    if (gSaveContext.rupees < self->basePrice) {
        return 4;
    }
    if (Item_CheckObtainability(ITEM_SHIELD_HYLIAN) == ITEM_NONE) {
        return 0;
    }
    return 1;
}

s32 FUN_0013002c(PlayState* play, EnGirlA* self) {
    if (gSaveContext.linkAge != 0 || (gSaveContext.inventory.equipment & gBitFlags[9])) {
        return 2;
    }
    if (gSaveContext.rupees < self->basePrice) {
        return 4;
    }
    if (Item_CheckObtainability(0x42) == ITEM_NONE) {
        return 0;
    }
    return 1;
}

s32 FUN_00130098(PlayState* play, EnGirlA* self) {
    if ((gSaveContext.inventory.equipment & gBitFlags[2]) && !(gSaveContext.inventory.equipment & gBitFlags[3])) {
        return 2;
    }
    if (gSaveContext.rupees < self->basePrice) {
        return 4;
    }
    if (Item_CheckObtainability(0x3D) == ITEM_NONE) {
        return 0;
    }
    return 1;
}

s32 FUN_00131770(PlayState* play, EnGirlA* self) {
    if (gSaveContext.linkAge != 0 || (gSaveContext.inventory.equipment & gBitFlags[10])) {
        return 2;
    }
    if (gSaveContext.rupees < self->basePrice) {
        return 4;
    }
    if (Item_CheckObtainability(0x43) == ITEM_NONE) {
        return 0;
    }
    return 1;
}

s32 FUN_00112e88(PlayState* play, EnGirlA* self) {
    if (AMMO(ITEM_BOMBCHU) >= 50) {
        return 2;
    }
    if (gSaveContext.rupees < self->basePrice) {
        return 4;
    }
    if (Item_CheckObtainability(ITEM_BOMBCHU) == ITEM_NONE) {
        return 0;
    }
    return 1;
}

s32 FUN_003cfc54(PlayState* play, Actor* thisx) {
    EnGirlA* self = (EnGirlA*)thisx;

    if (CUR_CAPACITY(5) <= AMMO(ITEM_SLINGSHOT)) {
        return 2;
    }
    if (gSaveContext.rupees < self->basePrice) {
        return 4;
    }
    if (Item_CheckObtainability(0x58) == ITEM_NONE) {
        return 0;
    }
    return 1;
}

s32 FUN_00100228(PlayState* play, Actor* thisx) {
    EnGirlA* self = (EnGirlA*)thisx;

    if (Item_CheckObtainability(ITEM_BOW) == ITEM_NONE) {
        return 5;
    }
    if (CUR_CAPACITY(0) <= AMMO(ITEM_BOW)) {
        return 2;
    }
    if (gSaveContext.rupees < self->basePrice) {
        return 4;
    }
    return 1;
}

s32 FUN_003ce7e8(PlayState* play, EnGirlA* self) {
    if (!(gSaveContext.inventory.questItems & gBitFlags[19])) {
        return 2;
    }
    if (CUR_CAPACITY(1) <= AMMO(ITEM_BOMB)) {
        return 2;
    }
    return (gSaveContext.rupees < self->basePrice) ? 4 : 1;
}

s32 FUN_0011419c(PlayState* play, EnGirlA* self) {
    return EnGirlA_CanBuyBottle(play, self, 0x1D);
}

s32 FUN_003ceb34(PlayState* play, EnGirlA* self) {
    return EnGirlA_CanBuyBottle(play, self, 0x1C);
}

s32 FUN_003ceb8c(PlayState* play, EnGirlA* self) {
    return EnGirlA_CanBuyBottle(play, self, 0x19);
}

s32 FUN_003cff9c(PlayState* play, EnGirlA* self) {
    return EnGirlA_CanBuyBottle(play, self, 0x18);
}

void FUN_003c3b88(PlayState* play, EnGirlA* self) {
    switch (self->itemCount) {
        case 5:
            Item_Give(play, 0x8E);
            break;
        case 10:
            Item_Give(play, 0x8F);
            break;
        case 20:
            Item_Give(play, 0x90);
            break;
        case 30:
            Item_Give(play, 0x91);
            break;
    }
    Rupees_ChangeBy(-self->basePrice);
}

void BuyEvent_ShieldDiscount(PlayState* play, EnGirlA* self) {
    if (self->actor.params == 0xC) {
        if (gSaveContext.infTable[7] & 0x40) {
            Rupees_ChangeBy(sShieldDiscounts[(s32)Rand_ZeroFloat(7.9f)] - self->basePrice);
            return;
        }
    }
    Rupees_ChangeBy(-self->basePrice);
}

void FUN_00188d38(PlayState* play, EnGirlA* self) {
    u16 flags = gSaveContext.itemGetInf[0];

    switch (self->actor.params) {
        case 0x18: // SI_BOMBCHU_10_2 -> ITEMGETINF_06
            flags |= 0x40;
            break;
        case 0x19: // SI_BOMBCHU_10_3 -> ITEMGETINF_07
            flags |= 0x80;
            break;
        case 0x1A: // SI_BOMBCHU_20_3 -> ITEMGETINF_08
            flags |= 0x100;
            break;
        case 0x1B: // SI_BOMBCHU_20_4 -> ITEMGETINF_09
            flags |= 0x200;
            break;
        case 0x1C: // SI_BOMBCHU_10_4 -> ITEMGETINF_0A
            flags |= 0x400;
            break;
        case 0x15: // SI_BOMBCHU_10_1 -> ITEMGETINF_03
            flags |= 0x8;
            break;
        case 0x16: // SI_BOMBCHU_20_1 -> ITEMGETINF_04
            flags |= 0x10;
            break;
        case 0x17: // SI_BOMBCHU_20_2 -> ITEMGETINF_05
            flags |= 0x20;
            break;
        default:
            goto rupees;
    }
    gSaveContext.itemGetInf[0] = flags;

rupees:
    Rupees_ChangeBy(-self->basePrice);
}

void func_80A3BEE0(PlayState* play, Actor* thisx) {
    EnGirlA* self = (EnGirlA*)thisx;
    ShopItemEntry* itemEntry;

    if (EnGirlA_TryChangeShopItem(self)) {
        EnGirlA_InitItem(self, play);
        itemEntry = &sShopItemEntries[self->actor.params];
        self->actor.textId = itemEntry->itemDescTextId;
    } else {
        self->isInvisible = false;
        self->actor.draw = EnGirlA_Draw;
    }
}

s32 FUN_00112ef0(PlayState* play, Actor* thisx) {
    EnGirlA* self = (EnGirlA*)thisx;

    if (CUR_CAPACITY(6) != 0 && (CUR_CAPACITY(6) <= AMMO(ITEM_STICK))) {
        return 2;
    }
    if (gSaveContext.rupees < self->basePrice) {
        return 4;
    }
    if (Item_CheckObtainability(ITEM_STICK) == ITEM_NONE) {
        return 0;
    }
    return 1;
}

void FUN_00161554(PlayState* play, Actor* thisx) {
    EnGirlA* self = (EnGirlA*)thisx;

    switch (self->actor.params) {
        case 0x07:
            Item_Give(play, 0x19);
            break;
        case 0x08:
            Item_Give(play, 0x15);
            break;
        case 0x09:
            Item_Give(play, 0x16);
            break;
        case 0x0A:
            Item_Give(play, 0x17);
            break;
        case 0x27:
            Item_Give(play, 0x1C);
            break;
        case 0x28:
            Item_Give(play, 0x1D);
            break;
        case 0x29:
            Item_Give(play, 0x1E);
            break;
        case 0x2B:
            Item_Give(play, 0x18);
            break;
        case 0x2A:
            Item_Give(play, 0x20);
            break;
    }
    Rupees_ChangeBy(-self->basePrice);
}

void ItemGive_DekuNuts(PlayState* play, EnGirlA* self) {
    switch (self->itemCount) {
        case 5:
            Item_Give(play, 0x8C);
            break;
        case 10:
            Item_Give(play, 0x8D);
            break;
    }
    Rupees_ChangeBy(-self->basePrice);
}

void FUN_003f87a8(EnGirlA* self, PlayState* play) {
}

s32 FUN_00103778(PlayState* play, EnGirlA* self) {
    return 4;
}

s32 FUN_00114194(PlayState* play, EnGirlA* self) {
    return 4;
}

s32 FUN_001311c8(PlayState* play, EnGirlA* self) {
    return 5;
}

void FUN_00103780(PlayState* play, EnGirlA* self) {
    Rupees_ChangeBy(-self->basePrice);
}

void FUN_001c27b0(PlayState* play, EnGirlA* self) {
    Rupees_ChangeBy(-self->basePrice);
}

void FUN_001c33e0(PlayState* play, EnGirlA* self) {
    Rupees_ChangeBy(-self->basePrice);
}

void FUN_003cf518(PlayState* play, EnGirlA* self) {
    Rupees_ChangeBy(-self->basePrice);
}

void FUN_001041f0(PlayState* play, EnGirlA* self) {
    Item_Give(play, 0x1A);
    Rupees_ChangeBy(-self->basePrice);
}

void FUN_0011370c(PlayState* play, EnGirlA* self) {
    Item_Give(play, 0x42);
    Rupees_ChangeBy(-self->basePrice);
}

void FUN_00114908(PlayState* play, EnGirlA* self) {
    Item_Give(play, 0x21);
    Rupees_ChangeBy(-self->basePrice);
}

void FUN_001167a4(PlayState* play, EnGirlA* self) {
    Item_Give(play, 0x43);
    Rupees_ChangeBy(-self->basePrice);
}

void FUN_0012e0c4(PlayState* play, EnGirlA* self) {
    Item_Give(play, 0x3E);
    Rupees_ChangeBy(-self->basePrice);
}

void FUN_00130c78(PlayState* play, EnGirlA* self) {
    Item_Give(play, 0x3F);
    Rupees_ChangeBy(-self->basePrice);
}

void FUN_00376a38(PlayState* play, EnGirlA* self) {
    Item_Give(play, 0);
    Rupees_ChangeBy(-self->basePrice);
}

void FUN_003c4180(PlayState* play, EnGirlA* self) {
    Item_Give(play, 0x95);
    Rupees_ChangeBy(-self->basePrice);
}

void FUN_003ce7c0(PlayState* play, EnGirlA* self) {
    FUN_00355830(ITEM_BOW, self->itemCount);
    Rupees_ChangeBy(-self->basePrice);
}

void FUN_00113734(PlayState* play, EnGirlA* self) {
    FUN_00369128(play);
    *(u16*)((char*)&gSaveContext + 0x4A) = 8;
    Rupees_ChangeBy(-self->basePrice);
}

void FUN_003cf3a0(PlayState* play, EnGirlA* self) {
    Health_ChangeBy(play, self->itemCount);
    Rupees_ChangeBy(-self->basePrice);
}

void EnGirlA_Destroy(Actor* thisx, GameState* state) {
    EnGirlA* self = (EnGirlA*)thisx;

    FUN_00357248(&self->skelAnimModel);
    FUN_00357248(&self->skelAnimModel2);
    FUN_00350f34(thisx, 0);
}

void EnGirlA_Init(EnGirlA* self, PlayState* play) {
    s16 params;

    EnGirlA_TryChangeShopItem(self);
    params = self->actor.params;
    self->requiredObjectSlot = Object_GetIndex(&play->objectCtx, sShopItemEntries[params].objID);
    if (self->requiredObjectSlot >= 0) {
        self->actor.params = params;
        self->actionFunc2 = EnGirlA_InitializeItemAction;
    } else {
        Actor_Kill(&self->actor);
    }
    self->unk_1FC = 1.0f;
}

void FUN_001032bc(EnGirlA* self, PlayState* play) {
    Actor_SetScale(&self->actor, 0.25f);
    self->actor.shape.yOffset = 24.0f;
    self->actor.shape.shadowScale = 4.0f;
    self->actionFunc(self, play);
    Actor_SetFocus(&self->actor, 5.0f);
    {
        Vec3s* r = &self->actor.shape.rot;
        r->x = 0;
    }
    if (self->actor.params != 0x26) {
        if (self->isSelected) {
            self->yRotation += 0x1F4;
        } else {
            Math_SmoothStepToS(&self->yRotation, 0, 10, 0x7D0, 0);
        }
    }
}

void EnGirlA_Update(Actor* thisx, GameState* state) {
    EnGirlA* self = (EnGirlA*)thisx;

    self->actionFunc2(self, (PlayState*)state);
}

// Draft (69 of 142 words differ: literal-pool/VFP numbering of 360/65536/DEG consts, hoisted mov r6,#1), see STATUS.md
#if 0
void Matrix_RotateY(MTX34* mtx, s32 mode, f32 angle);
void Matrix_UpdatePosition(MTX34* dst, MTX34* src, Vec3f* translate);
void EnGirlA_Draw(Actor* thisx, GameState* state) {
    EnGirlA* self = (EnGirlA*)thisx;
    MTX34 mtx;
    Vec3f ofs;
    f32 animSpeed = 1.0f;
    f32 zero = 0.0f;
    f32 x_, y_, z_;

    if (PauseContext_GetState() != 0) {
        animSpeed = zero;
    }
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->unk_0C->SetAnimSpeed(animSpeed);
    }
    if (self->skelAnimModel2 != NULL) {
        self->skelAnimModel2->unk_0C->SetAnimSpeed(animSpeed);
    }
    MTX34CopyAsm(&mtx, &thisx->modelMtx);
    x_ = self->unk_1F0 * self->unk_1FC;
    y_ = self->unk_1F4 * self->unk_1FC;
    z_ = self->unk_1F8 * self->unk_1FC;
    ofs.x = x_; ofs.y = y_; ofs.z = z_;
    Matrix_UpdatePosition(&mtx, &mtx, &ofs);
    Matrix_RotateY(&mtx, 1, self->yRotation * 360.0f / 65536.0f * 0.017453292f);
    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&mtx);
    self->skelAnimModel->FUN_00372170(0);
    if (self->skelAnimModel2 != NULL) {
        mtx = thisx->modelMtx;
        x_ = self->unk_1F0 * self->unk_1FC;
        y_ = self->unk_1F4 * self->unk_1FC;
        z_ = self->unk_1F8 * self->unk_1FC;
        ofs.x = x_; ofs.y = y_; ofs.z = z_;
        Matrix_UpdatePosition(&mtx, &mtx, &ofs);
        if (thisx->params == 0x27) {
            Matrix_RotateY(&mtx, 1, self->yRotation * 360.0f / 65536.0f * 0.017453292f);
            ofs.x = -7.5f; ofs.y = zero; ofs.z = zero;
            Matrix_UpdatePosition(&mtx, &mtx, &ofs);
            Matrix_RotateY(&mtx, 1, self->yRotation * -360.0f / 65536.0f * 0.017453292f);
            ofs.x = 7.5f; ofs.y = zero; ofs.z = zero;
            Matrix_UpdatePosition(&mtx, &mtx, &ofs);
        }
        self->skelAnimModel2->SetUnkAC(1);
        self->skelAnimModel2->FUN_003721e0(&mtx);
        self->skelAnimModel2->FUN_00372170(0);
    }
}

#endif

GLOBAL_ASM("asm/EnGirlA_Draw.s")


// Draft (functionally decoded, 181 of 215 words differ; raw offsets = debt), see STATUS.md
#if 0
typedef struct ShopItemEntry2 {
    s16 objID; s16 gi0; s16 gi1; s16 anim0; s16 anim1; s16 price; s16 count; u16 textId; u16 promptTextId; s16 pad12;
    u32 f14; u32 f18; u32 f1c; u32 f20; f32 offX; f32 offY; f32 offZ;
} ShopItemEntry2;
extern ShopItemEntry2 sShopItemEntries2[] __asm("sShopItemEntries");
extern s16 sMaskItems[8];
s32 Object_IsLoaded(ObjectContext* objectCtx, s32 slot);
void FUN_00357298(PlayState* play, Actor* actor);
void FUN_00357248(struct SkeletonAnimationModel** m);
struct SkeletonAnimationModel* FUN_0036a924(Actor* actor, PlayState* play, s16 obj, s16 gi);
void FUN_003f87a8(EnGirlA* self, PlayState* play);
void FUN_001032bc(EnGirlA* self, PlayState* play);
void func_80A3BEE0(PlayState* play, Actor* thisx);
void EnGirlA_InitializeItemAction(EnGirlA* self, PlayState* play) {
    ShopItemEntry2* itemEntry = &sShopItemEntries2[self->actor.params];
    EnGirlA* s2 = self;
    ZARInfo* zar;
    s32 one = 1;
    s32 zero = 0;

    if (Object_IsLoaded(&play->objectCtx, self->requiredObjectSlot)) {
        self->actor.flags &= ~0x10;
        self->actor.objBankIndex = self->requiredObjectSlot;
        switch (self->actor.params) {
            case 0x1E:
                if (gSaveContext.itemGetInf[3] & 0x100) { self->actor.textId = 0x70B6; } else { self->actor.textId = itemEntry->textId; }
                *(u16*)((char*)self + 0x1B4) = itemEntry->promptTextId;
                break;
            case 0x1F:
                if (gSaveContext.itemGetInf[3] & 0x400) { self->actor.textId = 0x70B5; } else { self->actor.textId = itemEntry->textId; }
                *(u16*)((char*)self + 0x1B4) = itemEntry->promptTextId;
                break;
            case 0x20:
                if (gSaveContext.itemGetInf[3] & 0x200) { self->actor.textId = 0x70B4; } else { self->actor.textId = itemEntry->textId; }
                *(u16*)((char*)self + 0x1B4) = itemEntry->promptTextId;
                break;
            case 0x21:
                if (gSaveContext.itemGetInf[3] & 0x800) { self->actor.textId = 0x70B7; } else { self->actor.textId = itemEntry->textId; }
                *(u16*)((char*)self + 0x1B4) = itemEntry->promptTextId;
                break;
            case 0x22:
                if (gSaveContext.itemGetInf[3] & 0x800) {
                    self->actor.textId = 0x70BB;
                    *(u16*)((char*)self + 0x1B4) = itemEntry->promptTextId;
                } else {
                    self->actor.textId = itemEntry->textId;
                    *(u16*)((char*)self + 0x1B4) = 0xEB;
                }
                break;
            default:
                self->actor.textId = itemEntry->textId;
                *(u16*)((char*)self + 0x1B4) = itemEntry->promptTextId;
                break;
        }
        if ((u32)(self->actor.params - 0x1E) <= 7 &&
            gSaveContext.inventory.items[SLOT(0x21)] == sMaskItems[self->actor.params - 0x1E]) {
            self->isInvisible = 1;
            self->actor.draw = NULL;
            if ((u32)(self->actor.params - 0x1E) <= 7) {
                self->actor.textId = 0xBD;
            }
        } else {
            FUN_00357298(play, &self->actor);
        }
        *(u32*)((char*)self + 0x1C0) = 0x00190990;
        *(void**)((char*)self + 0x1C4) = (void*)func_80A3BEE0;
        *(u32*)((char*)self + 0x1B8) = itemEntry->f14;
        *(u32*)((char*)self + 0x1D0) = itemEntry->f18;
        *(u32*)((char*)self + 0x1D4) = itemEntry->f1c;
        *(u32*)((char*)self + 0x1D8) = itemEntry->f20;
        self->basePrice = itemEntry->price;
        self->itemCount = itemEntry->count;
        *(u16*)((char*)self + 0x1E0) = itemEntry->gi0;
        zar = FUN_00372f38(&self->actor, play, 0);
        if (self->skelAnimModel != NULL) FUN_00357248(&self->skelAnimModel);
        if (self->skelAnimModel2 != NULL) FUN_00357248(&self->skelAnimModel2);
        self->skelAnimModel = FUN_0036a924(&self->actor, play, itemEntry->objID, itemEntry->gi0);
        if (itemEntry->gi1 < 0) {
            self->skelAnimModel2 = NULL;
        } else {
            self->skelAnimModel2 = FUN_0036a924(&self->actor, play, itemEntry->objID, itemEntry->gi1);
        }
        if (itemEntry->anim0 != -1 && self->skelAnimModel != NULL) {
            SkeletonAnimationModel_unk_0C* m = self->skelAnimModel->unk_0C;
            m->FUN_00372d94(zar->GetCMABByIndex(itemEntry->anim0));
            m->animType = 1;
        }
        if (itemEntry->anim1 != -1 && self->skelAnimModel2 != NULL) {
            SkeletonAnimationModel_unk_0C* m = self->skelAnimModel2->unk_0C;
            m->FUN_00372d94(zar->GetCMABByIndex(itemEntry->anim1));
            m->animType = 1;
        }
        self->unk_1F0 = itemEntry->offX;
        self->unk_1F4 = itemEntry->offY;
        self->unk_1F8 = itemEntry->offZ;
        self->actor.flags &= ~1;
        Actor_SetScale(&self->actor, 0.25f);
        self->actor.shape.yOffset = 24.0f;
        self->actor.shape.shadowScale = 4.0f;
        self->actor.world.pos.y = self->actor.home.pos.y;
        self->actor.speedXZ = 0.0f;
        self->actionFunc = FUN_003f87a8;
        self->actionFunc2 = (EnGirlAActionFunc)FUN_001032bc;
        *(s32*)((char*)self + 0x1B0) = 1;
        self->isSelected = 0;
        self->yRotation = 0;
        *(u16*)((char*)self + 0x1CA) = self->actor.shape.rot.y;
    }
}

#endif

GLOBAL_ASM("asm/EnGirlA_InitializeItemAction.s")



}
