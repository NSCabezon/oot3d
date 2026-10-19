extern "C" {
#include "z_en_skj.hpp"

extern u8 EnSkj_sSoundState[];
extern u8 EnSkj_sSoundStateFlag[];
s32 FUN_0034bdb8(s32 arg0);

void FUN_00285fa4(void) {
    if (*(s32*)(EnSkj_sSoundState + 0x70) != -1) {
        FUN_0034bdb8(0);
        Audio_QueueSeqCmd(0, *(s32*)(EnSkj_sSoundState + 0x70), 0);
    }
    *(s32*)(EnSkj_sSoundState + 0x70) = -1;
}

u8* FUN_003371d8(void) {
    if (EnSkj_sSoundStateFlag[1] < 0xFE) {
        *(s32*)(EnSkj_sSoundState + 0x94) = 0;
    }
    return EnSkj_sSoundStateFlag;
}


s32 FUN_00366684(s32 arg0);
void FUN_0033c7a4(EnSkj* self);
void FUN_0032e1f0(EnSkj* self) {
    s32 v = FUN_00366684(0);

    if (v != -1) {
        *(s32*)(EnSkj_sSoundState + 0x70) = v;
    }
    FUN_0033c7a4(self);
}

}
