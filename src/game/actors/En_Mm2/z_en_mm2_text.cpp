extern "C" {
#include "z_en_mm2.hpp"

// N64: func_80AAEF70. Moved from functions_350000s.cpp.
void FUN_003539d8(EnMm2* self, PlayState* play) {
    if ((gSaveContext.eventChkInf[9] & 0xF) != 0xF) {
        self->actor.textId = 0x6086;
    } else if (gSaveContext.infTable[23] & 0x8000) {
        if (gSaveContext.eventInf[1] & 1) {
            self->actor.textId = 0x6082;
        } else if (gSaveContext.timer2State != 0) {
            self->actor.textId = 0x6076;
        } else if (gSaveContext.unk_EE0 == 158) {
            self->actor.textId = 0x607E;
        } else {
            self->actor.textId = 0x6081;
        }
    } else if (gSaveContext.timer2State != 0) {
        self->actor.textId = 0x6076;
    } else {
        self->actor.textId = 0x607D;
        gSaveContext.eventInf[1] &= ~1;
        gSaveContext.unk_EE0 = 158;
    }
}

}
