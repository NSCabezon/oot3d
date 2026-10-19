extern "C" {
#include "z_en_skj.hpp"

extern u8 EnSkj_sStumpVars[];
extern u8 EnSkj_sSeqVars[];
void FUN_002f6944(void);
void FUN_003525d4(void);
void FUN_002f7af4(s32 arg0, f32 arg1, f32 arg2);

void FUN_004c0878(void) {
    if (*(s32*)(EnSkj_sStumpVars + 0x10) != 0) {
        FUN_002f6944();
        FUN_003525d4();
        *(s32*)(EnSkj_sStumpVars + 0x10) = 0;
    }
    FUN_002f7af4(*(s32*)(EnSkj_sStumpVars + 0xC), 400.0f, 240.0f);
    *(s32*)(EnSkj_sStumpVars + 0x14) = 0;
}


void FUN_0032c540(s32 value) {
    *(s32*)(EnSkj_sStumpVars + 0x40) = value;
}

void FUN_0032c550(s32 value) {
    *(s32*)(EnSkj_sStumpVars + 0x3C) = value;
}

void FUN_0032c560(s32 value) {
    *(s32*)(EnSkj_sStumpVars + 0x44) = value;
}


}
