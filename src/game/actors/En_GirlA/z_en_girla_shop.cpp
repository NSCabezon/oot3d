extern "C" {
#include "z_en_girla.hpp"

typedef struct ShopItemEntry {
    /* 0x00 */ s16 objID;
    /* 0x02 */ char unk_02[0x0C];
    /* 0x0E */ u16 itemDescTextId;
    /* 0x10 */ char unk_10[0x20];
} ShopItemEntry; // size = 0x30

extern ShopItemEntry sShopItemEntries[];
s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
void EnGirlA_InitializeItemAction(EnGirlA* self, PlayState* play);

s32 EnGirlA_TryChangeShopItem(EnGirlA* self) {
    switch (self->actor.params) {
        case 0x11: // SI_MILK_BOTTLE
            if (gSaveContext.itemGetInf[0] & 0x4) { // ITEMGETINF_TALON_BOTTLE
                self->actor.params = 0x10; // SI_RECOVERY_HEART
                return true;
            }
            break;
        case 0x18: // SI_BOMBCHU_10_2 -> ITEMGETINF_06
            if (gSaveContext.itemGetInf[0] & 0x40) {
                self->actor.params = 0x26; // SI_SOLD_OUT
                return true;
            }
            break;
        case 0x19: // SI_BOMBCHU_10_3 -> ITEMGETINF_07
            if (gSaveContext.itemGetInf[0] & 0x80) {
                self->actor.params = 0x26;
                return true;
            }
            break;
        case 0x1A: // SI_BOMBCHU_20_3 -> ITEMGETINF_08
            if (gSaveContext.itemGetInf[0] & 0x100) {
                self->actor.params = 0x26;
                return true;
            }
            break;
        case 0x1B: // SI_BOMBCHU_20_4 -> ITEMGETINF_09
            if (gSaveContext.itemGetInf[0] & 0x200) {
                self->actor.params = 0x26;
                return true;
            }
            break;
        case 0x1C: // SI_BOMBCHU_10_4 -> ITEMGETINF_0A
            if (gSaveContext.itemGetInf[0] & 0x400) {
                self->actor.params = 0x26;
                return true;
            }
            break;
        case 0x15: // SI_BOMBCHU_10_1 -> ITEMGETINF_03
            if (gSaveContext.itemGetInf[0] & 0x8) {
                self->actor.params = 0x26;
                return true;
            }
            break;
        case 0x16: // SI_BOMBCHU_20_1 -> ITEMGETINF_04
            if (gSaveContext.itemGetInf[0] & 0x10) {
                self->actor.params = 0x26;
                return true;
            }
            break;
        case 0x17: // SI_BOMBCHU_20_2 -> ITEMGETINF_05
            if (gSaveContext.itemGetInf[0] & 0x20) {
                self->actor.params = 0x26;
                return true;
            }
            break;
    }
    return false;
}

void EnGirlA_InitItem(EnGirlA* self, PlayState* play) {
    s16 params = self->actor.params;

    self->requiredObjectSlot = Object_GetIndex(&play->objectCtx, sShopItemEntries[params].objID);
    if (self->requiredObjectSlot < 0) {
        Actor_Kill(&self->actor);
        return;
    }
    self->actor.params = params;
    self->actionFunc2 = EnGirlA_InitializeItemAction;
}


}
