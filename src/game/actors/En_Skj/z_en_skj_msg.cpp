extern "C" {
#include "z_en_skj.hpp"

void FUN_00340bdc(PlayState* play, s32 arg);
void FUN_003523dc(s32 arg);
u8* FUN_003371d8(void);
void FUN_0033704c(s32 arg);
u8* FUN_0033f400(void);
void FUN_0033f248(s32 arg0, s32 arg1);
void FUN_0033f2d8(void);
extern u16 EnSkj_sMsgVars[];

void FUN_00334fa0(PlayState* play) {
    u8* msg = (u8*)&play->msgCtx;
    FUN_00340bdc(play, msg[0x1F0] + 1);
    if (msg[0x1F0] == 0x2E) {
        u8* flags;
        FUN_003523dc(1);
        flags = FUN_003371d8();
        *(u8**)(msg + 0x1E0) = flags;
        EnSkj_sMsgVars[1] = 0;
        flags[2] = 0;
        FUN_0033704c(0xA000);
        *(u16*)(msg + 0x2BE) = *(u16*)(msg + 0x2C0);
    } else if (msg[0x1F0] == 0x2C) {
        u8* p;
        FUN_003523dc(6);
        p = FUN_0033f400();
        *(u8**)(msg + 0x1E0) = p;
        EnSkj_sMsgVars[1] = 0;
        p[2] = 0;
        FUN_0033f248(0xE, 1);
        msg[0x2D3] = 3;
    }
    FUN_0033f2d8();
}

}
