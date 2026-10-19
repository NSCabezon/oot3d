extern "C" {
#include "z_en_mag.hpp"

typedef struct MagObj {
    /* 0x00 */ char unk_00[4];
    /* 0x04 */ void* unk_04;
    /* 0x08 */ f32 unk_08;
    /* 0x0C */ char unk_0C[4];
    /* 0x10 */ u8 unk_10;
} MagObj;

typedef struct MagLink {
    /* 0x00 */ char unk_00[0x14];
    /* 0x14 */ s32 unk_14;
} MagLink;

typedef struct MagData {
    /* 0x00 */ char unk_00[0x10];
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ u8 unk_14;
} MagData;

void FUN_003ff284(MagObj* obj, void** p) {
    obj->unk_04 = p;
    obj->unk_08 = 0.0f;
    if (obj->unk_10 == 0) {
        MagLink* link = (MagLink*)*p;
        MagData* data = (MagData*)((u8*)link + link->unk_14);
        if (data->unk_14 == 0) {
            return;
        }
    }
    obj->unk_10 = 1;
}

void FUN_003ff2c4(MagObj* obj, f32 f) {
    obj->unk_08 = f;
    MagLink* link = (MagLink*)*(void**)obj->unk_04;
    MagData* data = (MagData*)((u8*)link + link->unk_14);
    s32 count = data->unk_10;
    if (f > (f32)count) {
        if (obj->unk_10 != 0) { obj->unk_08 = f - (f32)(count + 1); } else { obj->unk_08 = (f32)count; }
    } else if (f < -1.0f) {
        if (obj->unk_10 != 0) { obj->unk_08 = f + (f32)(count + 1); } else { obj->unk_08 = (f32)count; }
    }
}

}
