extern "C" {
#include "z_en_skj.hpp"

extern u8 EnSkj_sSeqVars[];
void FUN_004c0878(void);

void FUN_0032c570(s32 arg0) {
    FUN_004c0878();
    if (arg0 == 1) {
        Audio_PlaySoundGeneral(0x01000485, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
    }
    *(s32*)(EnSkj_sSeqVars + 0x5C) = 0;
    *(s32*)(EnSkj_sSeqVars + 0x34) = 0x12;
    *(s32*)(EnSkj_sSeqVars + 0x74) = 2;
}

}
