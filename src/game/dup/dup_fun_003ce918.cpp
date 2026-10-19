extern "C" {
#include "global.hpp"

typedef struct DupPriceEntry {
    /* 0x00 */ s16 itemPrice;
} DupPriceEntry;

typedef struct DupPriceActor {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x64];
    /* 0x0208 */ DupPriceEntry* entry;
} DupPriceActor;

void Rupees_ChangeBy(s16 value);

void FUN_003ce918(DupPriceActor* self) {
    Rupees_ChangeBy(-self->entry->itemPrice);
}

}
