extern "C" {
#include "z_en_okarina_effect.hpp"

#define FLAGS 0x02000010

void EnOkarinaEffect_Init(Actor* actor, GameState* state);
void EnOkarinaEffect_Destroy(Actor* actor, GameState* state);
void EnOkarinaEffect_Update(Actor* actor, GameState* state);

ActorInit En_Okarina_Effect_InitVars = {
    ACTOR_EN_OKARINA_EFFECT,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnOkarinaEffect),
    (ActorFunc)EnOkarinaEffect_Init,
    (ActorFunc)EnOkarinaEffect_Destroy,
    (ActorFunc)EnOkarinaEffect_Update,
    NULL,
};

// PlayState envCtx fields (inside unk_2B80); offsets from the asm.
#define PLAY_ENV_U8(play, off) (*((u8*)(play) + (off)))
#define ENV_PRECIP_RAIN_CUR 0x326F // N64: envCtx.precipitation[PRECIP_RAIN_CUR]
#define ENV_PRECIP_SOS_MAX 0x3272  // N64: envCtx.precipitation[PRECIP_SOS_MAX]
#define ENV_STORM_REQUEST 0x325D   // N64: envCtx.stormRequest
#define ENV_LIGHTNING_STATE 0x325F // N64: envCtx.lightningState

// N64: gWeatherMode. No symbol exists for it in the scatter file, so it is defined as an
// absolute symbol (EQU 0x00531EB4) in src/game/shared_syms.s; a constant-address macro makes armcc
// split the address into base + offset and not match.
extern u8 gWeatherMode;

void FUN_00106198(EnOkarinaEffect* self, PlayState* play); // N64: EnOkarinaEffect_TriggerStorm
void FUN_003666a0(PlayState* play);                          // N64: Environment_StopStormNatureAmbience
// N64: EnOkarinaEffect_ManageStorm-like path; this code is embedded (unsymbolized) at the end of
// EnSyatekiMan_Start's asm, so it is referenced by absolute address.
#define FUN_00196a80 ((EnOkarinaEffectActionFunc)0x00196A80)

void EnOkarinaEffect_Init(Actor* actor, GameState* state) {
    EnOkarinaEffect* self = (EnOkarinaEffect*)actor;
    PlayState* play = (PlayState*)state;

    // N64 calls Actor_Kill when it is already raining; 3DS instead picks a different action.
    if (PLAY_ENV_U8(play, ENV_PRECIP_RAIN_CUR) != 0) {
        self->actionFunc = FUN_00196a80;
    } else {
        self->actionFunc = FUN_00106198;
    }
}

void EnOkarinaEffect_Destroy(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;

    PLAY_ENV_U8(play, ENV_PRECIP_SOS_MAX) = 0;
    if ((gWeatherMode != 4) && (gWeatherMode != 5) && (PLAY_ENV_U8(play, ENV_STORM_REQUEST) == 1)) {
        PLAY_ENV_U8(play, ENV_STORM_REQUEST) = 2;
        FUN_003666a0(play);
    }
    PLAY_ENV_U8(play, ENV_LIGHTNING_STATE) = 2;
}

void EnOkarinaEffect_Update(Actor* actor, GameState* state) {
    EnOkarinaEffect* self = (EnOkarinaEffect*)actor;

    self->actionFunc(self, (PlayState*)state);
}
}
