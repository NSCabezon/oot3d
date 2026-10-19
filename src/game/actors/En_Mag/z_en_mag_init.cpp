extern "C" {
#include "z_en_mag.hpp"
#include "model_manager.hpp"

typedef struct MagWork MagWork;
typedef struct MagHolder MagHolder;

void* ZAR_GetCMBByIndex(void* zar, s32 index);
void* ZAR_GetCSABByIndex(void* zar, s32 index);
s32 FUN_003487ec(void* p);
void FUN_003480a8(MagWork* work, s32 arg);
void FUN_00348068(MagHolder* holder, void** p);
void FUN_00348054(MagHolder* holder, f32 f);
extern char EnMag_sObj587934;
void FUN_00347fbc(MagWork* self, s32 mode);
u8 FUN_00410350(void* self);
extern MagWork* EnMag_sWork;

typedef struct MagCsabData {
    /* 0x00 */ char unk_00[0x10];
    /* 0x10 */ s32 unk_10;
} MagCsabData;

typedef struct MagCsabLink {
    /* 0x00 */ char unk_00[0x14];
    /* 0x14 */ s32 unk_14;
} MagCsabLink;

typedef struct MagCsab {
    /* 0x00 */ MagCsabLink* link;
} MagCsab;

#define MAG_SAVE_U8(off) (*(u8*)((u8*)&gSaveContext + (off)))

void EnMag_Init(Actor* thisx, GameState* state) {
    EnMag* self = (EnMag*)thisx;
    PlayState* play = (PlayState*)state;
    void* zar;
    ModelAllocator* alloc;
    s32 idx;

    self->actor.unk_19A = 1;
    zar = (u8*)play->GetObjectStatus(self->actor.objBankIndex) + 0x10;
    {
        ModelManager& mm = GetModelManager();
        alloc = mm.allocator;
    }
    alloc->owner = NULL;
    {
        u8 v = FUN_00410350((u8*)&GetModelManager() + 0x32C0);
        if (v == 0 || v == 2) {
            idx = 2;
            self->unk_1CE = 1;
        } else {
            idx = 3;
            self->unk_1CE = 2;
        }
    }
    self->unk_1A4 = (EnMagObj*)alloc->Create(ZAR_GetCMBByIndex(zar, idx), 0);
    self->unk_1A8 = (EnMagObj*)alloc->Create(ZAR_GetCMBByIndex(zar, 0), 0);
    self->unk_1AC = (EnMagObj*)alloc->Create(ZAR_GetCMBByIndex(zar, 1), 0);
    {
        s32 id = FUN_003487ec(&EnMag_sObj587934);
        SkeletonAnimationModel_unk_0C* m = ((SkeletonAnimationModel*)self->unk_1A8)->unk_0C;
        m->FUN_00372d94(((ZARInfo*)zar)->GetCMABByIndex(id));
        m->SetAnimType(1);
    }
    YREG(1) = 63;
    YREG(3) = 80;
    YREG(4) = 255;
    YREG(5) = 30;
    YREG(6) = 30;
    YREG(7) = 119;
    YREG(8) = 7;
    YREG(9) = 5;
    YREG(10) = 3;
    VREG(4) = 1;
    VREG(5) = 6;
    VREG(6) = 2;
    self->unk_1CA = 6;
    self->unk_1CC = 10;
    VREG(19) = 99;
    VREG(21) = 9;
    VREG(23) = 10;
    VREG(24) = 8;
    self->unk_1C6 = 60;
    self->unk_1D0 = self->unk_1D4 = self->unk_1D8 = 0.0f;
    self->unk_1C8 = 0;
    self->unk_1DC = 0.0f;
    self->unk_1C4 = 0;
    FUN_003480a8(EnMag_sWork, 0);
    if (MAG_SAVE_U8(0x1576) != 0) {
        self->unk_1D4 = 255.0f;
        self->unk_1D8 = 255.0f;
        self->unk_1D0 = 255.0f;
        self->unk_1DC = 255.0f;
        MAG_SAVE_U8(0x1576) = 0;
        self->unk_1C8 = 2;
        self->unk_1C0 = 30;
        MAG_SAVE_U8(0x15A6) = 1;
        MAG_SAVE_U8(0x15A7) = 255;
        {
            MagCsab* csab = (MagCsab*)ZAR_GetCSABByIndex((u8*)play->GetObjectStatus(self->actor.objBankIndex) + 0x10, self->unk_1CE);
            FUN_00348068((MagHolder*)self->unk_1A4, (void**)csab);
            MagCsabLink* link = csab->link;
            MagCsabData* data = (MagCsabData*)((u8*)link + link->unk_14);
            s32 n = data->unk_10 + 1;
            FUN_00348054((MagHolder*)self->unk_1A4, (f32)n);
        }
        FUN_00347fbc(EnMag_sWork, 1);
    }
    self->unk_1BC = 0;
    self->unk_1BE = 0;
    self->unk_1B4 = 0;
    self->unk_1B8 = 0;
    self->unk_1C2 = 0;
}


}
