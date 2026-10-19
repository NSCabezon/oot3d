extern "C" {
#include "z_en_fire_rock.hpp"

// Moved from src/functions/functions_3A0000s.cpp (tail-called by EnFireRock_Update). Player flame-state setter.
void FUN_003a30b8(PlayState* play, Actor* actor, s16 a2, u8 a3, f32 f0, f32 f1) { // Actor_SetPlayerKnockbackSmall
    Player* player = *(Player**)((u8*)play + 0x20AC);

    *(u8*)((u8*)player + 0x2290) = a3;
    *(u8*)((u8*)player + 0x2291) = 1;
    *(f32*)((u8*)player + 0x2294) = f0;
    *(u16*)((u8*)player + 0x2292) = a2;
    *(f32*)((u8*)player + 0x2298) = f1;
}

}
