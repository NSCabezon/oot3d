extern "C" {
#include "z_en_owl.hpp"

void FUN_00231364(s32 arg0, s32 arg1);
void EnOwl_WaitDefault(EnOwl* self, PlayState* play);
void FUN_00188650(EnOwl* self);
s32 Gameplay_InCsMode(PlayState* play);
extern u16 gTimeSpeed;

#if 0 // Sol: opacity fixes inline231364, but456/460B34words remains incomplete.
static inline void EnOwl_SetCurFrame(SkelAnime* skelAnime, f32 frame) {
    skelAnime->curFrame = frame;
}

void FUN_00188484(EnOwl* self, PlayState* play) {
    ObjectStatus* objectStatus = play->GetObjectStatus(self->actor.objBankIndex);
    ZARInfo* zarInfo = (ZARInfo*)((u8*)objectStatus + 0x10);

    Math_SmoothStepToS(&self->actor.world.rot.y, *(s16*)((u8*)self + 0x1360), 2, 900, 600);
    self->actor.shape.rot.y = self->actor.world.rot.y;
    if (self->actor.xzDistToPlayer < 50.0f && !Gameplay_InCsMode(play)) {
        s32 choice = (self->actor.params & 0xFC0) >> 6;
        switch (choice) {
        case 7:
            play->FUN_0037573c(zarInfo->GetQDBByIndex(0));
            *(s32*)((u8*)self + 0x140) = 0;
            break;
        case 8:
        case 9:
            play->FUN_0037573c(zarInfo->GetQDBByIndex(1));
            *(s32*)((u8*)self + 0x140) = 0;
            break;
        }
        Audio_PlaySoundGeneral(0x0100048C, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               (s8*)&DAT_0054ac24.unk_0);
        *(u8*)((u8*)&gSaveContext + 0x15A2) = 1;
        FUN_00231364(0x14, 0xA);
        self->actionFunc = (void*)EnOwl_WaitDefault;
        *(u8*)((u8*)self + 0x136C) = 0;
        self->actionFlags |= 0x80;
        gTimeSpeed = 0;
    }
    if (self->unk_238 >= 37.0f) {
        if (self->unk_135E == 0) {
            self->actionFunc = (void*)FUN_00188650;
        } else {
            EnOwl_SetCurFrame((SkelAnime*)((u8*)self + 0x1FC), 21.0f);
            self->unk_135E--;
        }
    }
    self->actionFlags |= 8;
}
#endif
}
