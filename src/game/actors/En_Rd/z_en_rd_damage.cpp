extern "C" {
#include "z_en_rd.hpp"

void FUN_002a57b0(EnRd* self, PlayState* play);
void Audio_PlayActorSound2(Actor* actor, s32 arg1);
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);

void FUN_00374ab0(PlayState* play, EnRd* self) {
    self->action = 1;
    self->actor.speedXZ = 0.0f;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    if (self->isMourning != 0) {
        self->timer = 900;
        u8* data = (u8*)0x0052E230;
        if (*(s32*)((u8*)play + 0x5BF4) != *(s32*)(data + 0x10)) {
            *(s32*)(data + 0x10) = *(s32*)((u8*)play + 0x5BF4);
            Audio_PlayActorSound2(&self->actor, 0x010002A6);
        }
        Actor_SetColorFilter(&self->actor, 0x800000, 0xFFFF80C8, 0, 0xFF);
    } else if (self->unk_965 == 1) {
        Actor_SetColorFilter(&self->actor, 0, 0xC8, 0, 0x50);
    } else {
        Audio_PlayActorSound2(&self->actor, 0x010002A6);
        Actor_SetColorFilter(&self->actor, 0x800000, 0xC8, 0, 0x50);
    }
    self->unk_9C8 = 0;
    self->actionFunc = FUN_002a57b0;
}
}
