// FUN_0031a3dc = EnRu1_TurnUpperBodyForward. Kept in its own TU so armcc does not inline it into
// FUN_0039d788 (the ROM keeps the bl).
extern "C" {
#include "z_en_ru1.hpp"

void* FUN_003478bc(void* a, s32 b);
void EffectSsGRipple_Spawn(PlayState* play, Vec3f* pos, s32 a, s32 radiusMax, s32 life);

// FUN_0031cddc (= EnRu1 ripple timer). Own TU: armcc inlined it into FUN_00392200 when it lived in z_en_ru1.cpp.
void FUN_0031cddc(EnRu1* self, PlayState* play) {
    f32* timer = &self->unk_BC8;
    s16 radiusMax;
    u8 idx;
    Vec3f pos;

    *timer += 1.0f;
    if (!(*timer < (f32)kREG(3) + 15.0f)) {
        radiusMax = kREG(1) + 500;
    idx = self->unk_219;
    {
        f32 posX = *(f32*)((u8*)FUN_003478bc(self->unk_1CC, idx) + 0xC);
        f32 posZ = *(f32*)((u8*)FUN_003478bc(self->unk_1CC, idx) + 0x2C);

        pos.x = posX;
        pos.y = self->actor.world.pos.y + self->actor.yDistToWater;
        pos.z = posZ;
    }
    EffectSsGRipple_Spawn(play, &pos, 100, radiusMax, 0);
        *timer = 0.0f;
    }
}

void FUN_0031a3dc(EnRu1* self) {
    EnRu1InteractInfo* info = &self->interactInfo;
    Vec3s* torsoRot = &info->torsoRot;
    EnRu1InteractInfo* s2 = info;
    Vec3s* headRot = &s2->headRot;

    Math_SmoothStepToS(&headRot->x, 0, 0x14, 0x1838, 0x64);
    Math_SmoothStepToS(&headRot->y, 0, 0x14, 0x1838, 0x64);
    Math_SmoothStepToS(&torsoRot->x, 0, 0x14, 0x1838, 0x64);
    Math_SmoothStepToS(&torsoRot->y, 0, 0x14, 0x1838, 0x64);
}

// FUN_00319f20 = EnRu1_CheckRoomAndSink (func_80AED624). Actor_Kill is not a call in the ROM: the room-change
// branch is inlined and returns 0 early. `!(flag & 1)` arm first + dead s2 alias on one read give the ROM shape.
s32 FUN_00319f20(EnRu1* self, PlayState* play) {
    EnRu1* s2 = self;
    s8 curRoomNum = *(s8*)((u8*)play + 0x4C30);

    if (self->roomNum2 != curRoomNum) {
        if (gSaveContext.infTable[20] & 8) {
            self->action = 0x2E;
            if (!(gSaveContext.infTable[20] & 1)) {
                self->unk_BB0 = 0x1AE;
                self->unk_BB2 = 0x3C;
            } else {
                self->unk_BB2 = -1;
            }
            return 0;
        }
    }
    if (((self->roomNum1 != curRoomNum) || (self->roomNum2 != curRoomNum)) &&
        (self->actor.yDistToWater > (f32)kREG(16) + 50.0f) && (s2->action != 0x21) && (self->action != 0x2E)) {
        self->action = 0x21;
        self->drawConfig = 2;
        self->sinkTimer = 0.0f;
        self->alpha = 0xFF;
    }
    return 1;
}
}
