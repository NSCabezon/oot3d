extern "C" {
#include "z_en_weather_tag.hpp"

#define FLAGS 0x80000010

// N64 gWeatherMode (see En_Okarina_Effect: absolute EQU symbol 0x00531EB4)
extern u8 gWeatherMode;

void EnWeatherTag_Init(EnWeatherTag* self, PlayState* play);
void EnWeatherTag_Destroy(Actor* thisx, GameState* state);
void EnWeatherTag_Update(Actor* thisx, GameState* state);
__attribute__((noinline)) u32 FUN_00328ddc(u8* s, u32 idx);

// Action functions of this actor (defined below)
void FUN_001ea4f0(EnWeatherTag* self, PlayState* play);
void FUN_00207200(EnWeatherTag* self, PlayState* play);
void FUN_001ea5ac(EnWeatherTag* self, PlayState* play);
void FUN_001ea540(EnWeatherTag* self, PlayState* play);
void FUN_0021de8c(EnWeatherTag* self, PlayState* play);
void FUN_00299704(EnWeatherTag* self, PlayState* play);
void FUN_0020724c(EnWeatherTag* self, PlayState* play);
void FUN_00176c68(EnWeatherTag* self, PlayState* play);
void FUN_0025a7d8(EnWeatherTag* self, PlayState* play);
void FUN_0025a824(EnWeatherTag* self, PlayState* play);
void FUN_0038ac64(EnWeatherTag* self, PlayState* play);

#define WEATHER_TAG_RANGE100(x) ((x >> 8) * 100.0f)

// N64 WeatherTag_CheckEnableWeatherEffect / WeatherTag_CheckRestoreWeather (not inlined on 3DS)
u8 FUN_0033b880(EnWeatherTag* self, PlayState* play, u8 skyboxConfig, u8 changeSkyboxNextConfig, u8 lightConfig,
                u8 changeLightNextConfig, u16 changeDuration, u8 weatherMode);
u8 FUN_0032d8e8(EnWeatherTag* self, PlayState* play, u8 skyboxConfig, u8 changeSkyboxNextConfig, u8 lightConfig,
                u8 changeLightNextConfig, u16 changeDuration);
void FUN_003665b4(PlayState* play); // Environment_PlayStormNatureAmbience
void FUN_003666a0(PlayState* play); // Environment_StopStormNatureAmbience
f32 Actor_WorldDistXZToActor(Actor* actorA, Actor* actorB);
void FUN_00275218(EnWeatherTag* self, PlayState* play);
void FUN_002890a8(EnWeatherTag* self, PlayState* play);
void FUN_0025a888(EnWeatherTag* self, PlayState* play);
void FUN_00206004(EnWeatherTag* self, PlayState* play);
s16* FUN_003373b8(u32 res, u32 idx); // sandstorm intensity table from the actor ZAR (unk_1B0)

#define WT_PRECIP_RAIN_MAX(play) (*((u8*)(play) + 0x326E))
#define WT_PRECIP_SNOW_MAX(play) (*((u8*)(play) + 0x3271))
#define WT_LIGHTNING_STATE(play) (*((u8*)(play) + 0x325F))

static inline void EnWeatherTag_SetupAction(EnWeatherTag* self, EnWeatherTagActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

ActorInit En_Weather_Tag_InitVars = {
    ACTOR_EN_WEATHER_TAG,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnWeatherTag),
    (ActorFunc)EnWeatherTag_Init,
    (ActorFunc)EnWeatherTag_Destroy,
    (ActorFunc)EnWeatherTag_Update,
    NULL,
};

// Object bank lookup: NULL if the slot is out of range or not loaded (3DS ZAR loading, no N64 equivalent).
// (Self*, PlayState*) signature on Init is needed for the r0 reuse in the prologue.
static inline u8* EnWeatherTag_GetObj(PlayState* play, s32 idx) {
    if ((u32)idx < 0x13 && *(s32*)((u8*)play + (idx << 7) + 0x3A64) != 0) {
        return (u8*)play + (idx << 7) + 0x3A5C;
    }
    return NULL;
}

void EnWeatherTag_Init(EnWeatherTag* self, PlayState* play) {
    self->actor.flags &= ~1;
    self->unk_1B0 = FUN_00328ddc(EnWeatherTag_GetObj(play, (u8)self->actor.objBankIndex) + 0x10, 0);

    switch (self->actor.params & 0xF) {
        case 0:
            self->actionFunc = (EnWeatherTagActionFunc)FUN_001ea4f0;
            break;
        case 1:
            if (Flags_GetEventChkInf(0x18)) {
                Actor_Kill(&self->actor);
            }
            self->actionFunc = (EnWeatherTagActionFunc)FUN_00207200;
            break;
        case 2:
            if (gSaveContext.eventChkInf[4] & 0x400) {
                Actor_Kill(&self->actor);
            }
            self->actionFunc = (EnWeatherTagActionFunc)FUN_001ea5ac;
            break;
        case 3:
            if (gSaveContext.eventChkInf[4] & 0x400) {
                Actor_Kill(&self->actor);
            }
            self->actionFunc = (EnWeatherTagActionFunc)FUN_001ea540;
            break;
        case 4:
            if (gSaveContext.eventChkInf[4] & 0x200) {
                Actor_Kill(&self->actor);
            }
            self->actionFunc = (EnWeatherTagActionFunc)FUN_0021de8c;
            break;
        case 5:
            if (!(gSaveContext.eventChkInf[4] & 0x100) || !(gSaveContext.eventChkInf[4] & 0x200) ||
                !(gSaveContext.eventChkInf[4] & 0x400) || CHECK_QUEST_ITEM(QUEST_MEDALLION_SHADOW)) {
                Actor_Kill(&self->actor);
            }
            self->actionFunc = (EnWeatherTagActionFunc)FUN_00299704;
            break;
        case 6:
            ((u8*)play)[0x7FA4] = (u8)(((u8*)play)[0x7FA4] + 1);
            self->unk_1AC = ((u8*)play)[0x7FA4];
            self->actionFunc = (EnWeatherTagActionFunc)FUN_0020724c;
            break;
        case 7:
            self->actionFunc = (EnWeatherTagActionFunc)FUN_00176c68;
            break;
    }
}

// The `params` local (switch) plus re-reading actor.params for the range is needed for the float regs.
void EnWeatherTag_Destroy(Actor* thisx, GameState* state) {
    EnWeatherTag* self = (EnWeatherTag*)thisx;
    u8* play = (u8*)state;
    s32 params = self->actor.params;

    switch (params & 0xF) {
        case 0:
            if (self->actionFunc == (EnWeatherTagActionFunc)FUN_0025a7d8 && *(u16*)(play + 0x104) == 0x51) {
                gWeatherMode = 0;
            } else if (self->actionFunc == (EnWeatherTagActionFunc)FUN_001ea4f0 && *(u16*)(play + 0x104) == 0x1D) {
                if (self->actor.xzDistToPlayer < (WEATHER_TAG_RANGE100(self->actor.params))) {
                    gWeatherMode = 1;
                }
            }
            break;
        case 3:
            if (self->actionFunc == (EnWeatherTagActionFunc)FUN_0025a824) {
                gWeatherMode = 0;
            }
            break;
        case 5:
            if (self->actionFunc == (EnWeatherTagActionFunc)FUN_0038ac64 &&
                self->actor.xzDistToPlayer > WEATHER_TAG_RANGE100(self->actor.params) + 100.0f) {
                gWeatherMode = 0;
            }
            break;
        case 6:
            play[0x7FA4]--;
            break;
    }
}

// N64 EnWeatherTag_DisabledCloudyHyruleMarket
void FUN_001ea4f0(EnWeatherTag* self, PlayState* play) {
    if (FUN_0033b880(self, play, 0, 4, 0, 3, 60, 1)) {
        EnWeatherTag_SetupAction(self, (EnWeatherTagActionFunc)FUN_0025a7d8);
    }
}

// N64 EnWeatherTag_EnabledCloudyHyruleMarket
void FUN_0025a7d8(EnWeatherTag* self, PlayState* play) {
    if (FUN_0032d8e8(self, play, 4, 0, 3, 0, 60)) {
        EnWeatherTag_SetupAction(self, (EnWeatherTagActionFunc)FUN_001ea4f0);
    }
}

// N64 EnWeatherTag_DisabledCloudyLonLonRanch
void FUN_00207200(EnWeatherTag* self, PlayState* play) {
    if (FUN_0033b880(self, play, 0, 1, 0, 2, 100, 2)) {
        EnWeatherTag_SetupAction(self, (EnWeatherTagActionFunc)FUN_00275218);
    }
}

// N64 EnWeatherTag_DisabledCloudyDeathMountain
void FUN_0021de8c(EnWeatherTag* self, PlayState* play) {
    if (FUN_0033b880(self, play, 0, 1, 0, 2, 60, 2)) {
        EnWeatherTag_SetupAction(self, (EnWeatherTagActionFunc)FUN_002890a8);
    }
}

// N64 EnWeatherTag_DisabledCloudySnow
void FUN_001ea5ac(EnWeatherTag* self, PlayState* play) {
    if (FUN_0033b880(self, play, 0, 1, 0, 2, 60, 3)) {
        WT_PRECIP_SNOW_MAX(play) = 64;
        EnWeatherTag_SetupAction(self, (EnWeatherTagActionFunc)FUN_0025a888);
    }
}

// N64 EnWeatherTag_DisabledRainLakeHylia
void FUN_001ea540(EnWeatherTag* self, PlayState* play) {
    if (FUN_0033b880(self, play, 0, 1, 0, 2, 100, 4)) {
        FUN_003665b4(play);
        WT_PRECIP_RAIN_MAX(play) = 25;
        EnWeatherTag_SetupAction(self, (EnWeatherTagActionFunc)FUN_0025a824);
    }
}

// N64 EnWeatherTag_EnabledRainLakeHylia
void FUN_0025a824(EnWeatherTag* self, PlayState* play) {
    if (FUN_0032d8e8(self, play, 1, 0, 2, 0, 100)) {
        FUN_003666a0(play);
        WT_PRECIP_RAIN_MAX(play) = 0;
        EnWeatherTag_SetupAction(self, (EnWeatherTagActionFunc)FUN_001ea540);
    }
}

// N64 EnWeatherTag_DisabledRainThunder
void FUN_00176c68(EnWeatherTag* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (Actor_WorldDistXZToActor(&player->actor, &self->actor) < WEATHER_TAG_RANGE100(self->actor.params)) {
        FUN_003665b4(play);
        WT_LIGHTNING_STATE(play) = 1;
        WT_PRECIP_RAIN_MAX(play) = 25;
        EnWeatherTag_SetupAction(self, (EnWeatherTagActionFunc)FUN_00206004);
    }
}
// N64 EnWeatherTag_DisabledCloudyRainThunderKakariko
void FUN_00299704(EnWeatherTag* self, PlayState* play) {
    s32 lc = 2;
    s32 sky = 0;

    if (play->sceneNum == 0x53) {
        lc = 0;
        sky = 1;
    }
    if (FUN_0033b880(self, play, sky, 1, 0, lc, 100, 5)) {
        FUN_003665b4(play);
        WT_LIGHTNING_STATE(play) = 1;
        WT_PRECIP_RAIN_MAX(play) = 30;
        EnWeatherTag_SetupAction(self, (EnWeatherTagActionFunc)FUN_0038ac64);
    }
}

// N64 EnWeatherTag_EnabledCloudyRainThunderKakariko
void FUN_0038ac64(EnWeatherTag* self, PlayState* play) {
    s32 lc = 2;

    if (play->sceneNum == 0x53) {
        lc = 0;
    }
    if (FUN_0032d8e8(self, play, 1, 0, lc, 0, 100)) {
        FUN_003666a0(play);
        WT_LIGHTNING_STATE(play) = 2;
        WT_PRECIP_RAIN_MAX(play) = 0;
        EnWeatherTag_SetupAction(self, (EnWeatherTagActionFunc)FUN_00299704);
    }
}

// SetSandstormIntensity. 3DS: intensities come from a ZAR table; all sandstorm tags accumulate "in range" in
// play+0x7FA5 and only the last one spawned (counter play+0x7FA4) applies it.
void FUN_0020724c(EnWeatherTag* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16* data = FUN_003373b8(self->unk_1B0, 0);
    u8 inRange = Actor_WorldDistXZToActor(&player->actor, &self->actor) < WEATHER_TAG_RANGE100(self->actor.params);

    if (self->unk_1AC != ((u8*)play)[0x7FA4]) {
        ((u8*)play)[0x7FA5] += inRange;
        return;
    }
    if (inRange | ((u8*)play)[0x7FA5]) {
        Math_SmoothStepToS((s16*)((u8*)play + 0x320E), -data[0], 1, data[1], data[2]);
        Math_SmoothStepToF((f32*)((u8*)play + 0x3214), -data[3], 1.0f, data[4], data[5]);
    } else {
        Math_SmoothStepToS((s16*)((u8*)play + 0x320E), 0, 1, data[1] / 2, data[2]);
        Math_SmoothStepToF((f32*)((u8*)play + 0x3214), 0.0f, 1.0f, data[4] * 0.5f, data[5]);
    }
    ((u8*)play)[0x7FA5] = 0;
}

void EnWeatherTag_Update(Actor* thisx, GameState* state) {
    EnWeatherTag* self = (EnWeatherTag*)thisx;

    self->actionFunc(thisx, state);
}

u32 FUN_00328ddc(u8* s, u32 idx) {
    s32 bank = *(s32*)(s + 0x3C);
    u32 count;
    u32 ret = 0;

    if (bank == -1) {
        count = 0;
    } else {
        count = *(u32*)(*(u8**)(s + 0x0C) + bank * 16);
    }
    if (count > idx) {
        ret = (*(u32**)(s + 0x68))[idx];
    }
    return ret;
}

}
