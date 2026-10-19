extern "C" {
#include "z_en_okarina_tag.hpp"

#define FLAGS 0x02000010

extern __pure float _fabsf(float);

#define PARAMS_GET_U(params, shift, count) ((((u32)(u16)(params)) << (32 - (shift) - (count))) >> (32 - (count)))

// Raw 3DS layout helpers (fields not yet in the repo headers)
#define OCARINA_MODE(play) (*(u16*)((u8*)(play) + 0x2B7E))
#define SET_OCARINA_MODE(play, v) (*(u16*)((u8*)(play) + 0x2B7E) = (v))
#define SAVE_BYTE(off) (*((u8*)&gSaveContext + 0x1000 + (off)))
#define SONG_FLAGS_F2 (*(u16*)((u8*)&gSaveContext + 0xE00 + 0xF2))
#define SONG_FLAGS_EE (*(u16*)((u8*)&gSaveContext + 0xE00 + 0xEE))
#define SONG_FLAGS_F4 (*(u16*)((u8*)&gSaveContext + 0xE00 + 0xF4))

void EnOkarinaTag_Init(Actor* actor, GameState* state);
void EnOkarinaTag_Destroy(Actor* actor, GameState* state);
void EnOkarinaTag_Update(Actor* actor, GameState* state);

// Action functions (3DS names; N64 name in comment)
void FUN_00288d0c(EnOkarinaTag* self, PlayState* play); // func_80ABEF2C
void FUN_00288e3c(EnOkarinaTag* self, PlayState* play); // func_80ABF0CC
void FUN_00274900(EnOkarinaTag* self, PlayState* play); // func_80ABF28C
void FUN_00274acc(EnOkarinaTag* self, PlayState* play); // func_80ABF4C8
void FUN_0027458c(EnOkarinaTag* self, PlayState* play); // func_80ABF708
void FUN_0028887c(EnOkarinaTag* self, PlayState* play); // func_80ABF7CC

// Unnamed 3DS helpers
s32 FUN_00366748(PlayState* play);
s32 FUN_0033feac(PlayState* play);
void FUN_003725e0(PlayState* play); // Message_CloseTextbox
void FUN_0037073c(PlayState* play, u16 action); // Message_StartOcarina
void FUN_00370778(PlayState* play);
void FUN_0034e4fc(f32 tempo, u32 bgmId); // Audio_SetMainBgmTempoFreqAfterFanfare (3DS arg order)

ActorInit En_Okarina_Tag_InitVars = {
    ACTOR_EN_OKARINA_TAG,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnOkarinaTag),
    (ActorFunc)EnOkarinaTag_Init,
    (ActorFunc)EnOkarinaTag_Destroy,
    (ActorFunc)EnOkarinaTag_Update,
    NULL,
};

void EnOkarinaTag_Init(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnOkarinaTag* self = (EnOkarinaTag*)thisx;

    self->actor.flags &= ~0x1;
    self->type = (self->actor.params >> 10) & 0x3F;
    self->ocarinaSong = PARAMS_GET_U(self->actor.params, 6, 4);
    self->switchFlag = PARAMS_GET_U(self->actor.params, 0, 6);
    if (self->switchFlag == 0x3F) {
        self->switchFlag = -1;
    }
    if (self->ocarinaSong == 0xF) {
        self->ocarinaSong = 0;
        self->unk_1B0 = 1;
    }
    self->actor.targetMode = 1;
    if (self->actor.world.rot.z > 0) {
        self->interactRange = self->actor.world.rot.z * 40.0f;
    }

    if ((self->switchFlag >= 0) && Flags_GetSwitch(play, self->switchFlag)) {
        Actor_Kill(&self->actor);
        return;
    }
    switch (self->type) {
        case 7:
            self->actionFunc = FUN_00288d0c;
            break;
        case 2:
            if (LINK_IS_ADULT) {
                Actor_Kill(&self->actor);
                break;
            }
        case 1:
        case 4:
        case 6:
            self->actionFunc = FUN_00274900;
            break;
        case 5:
            self->actor.textId = 0x5021;
            self->actionFunc = FUN_0027458c;
            break;
        default:
            Actor_Kill(&self->actor);
            break;
    }
}

void EnOkarinaTag_Destroy(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;

    if (FUN_00366748(play) != 0) {
        if (FUN_0033feac(play) == 0x89B) {
            FUN_003725e0(play);
        }
    }
}

void EnOkarinaTag_Update(Actor* actor, GameState* state) {
    EnOkarinaTag* self = (EnOkarinaTag*)actor;
    PlayState* play = (PlayState*)state;

    self->actionFunc(self, play);
}

void FUN_0028887c(EnOkarinaTag* self, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_EVENT) && Message_ShouldAdvance(play)) {
        FUN_003725e0(play);
        if (!CHECK_QUEST_ITEM(QUEST_SONG_SUN)) {
            play->FUN_0037573c(play->sceneZARInfo.GetQDBByIndex(0));
            *((u8*)&gSaveContext + 0x1000 + 0x5A2) = 1;
        }
        self->actionFunc = FUN_0027458c;
    }
}

void FUN_0027458c(EnOkarinaTag* self, PlayState* play) {
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        self->actionFunc = FUN_0028887c;
        return;
    }
    s16 yawDiff = self->actor.yawTowardsPlayer - self->actor.world.rot.y;
    self->unk_1B2++;
    if (self->actor.xzDistToPlayer > 120.0f) {
        return;
    }
    if (CHECK_QUEST_ITEM(QUEST_SONG_SUN)) {
        self->actor.textId = 0x5021;
    }
    s16 absYaw = ABS(yawDiff);
    if (absYaw < 0x4300) {
        self->unk_1B2 = 0;
        FUN_0036bb28(&self->actor, play, 70.0f);
    }
}

void FUN_00288d0c(EnOkarinaTag* self, PlayState* play) {
    Actor* actor = &self->actor;
    Player* player = GET_PLAYER(play);

    self->unk_1B2++;
    if ((self->switchFlag >= 0) && Flags_GetSwitch(play, self->switchFlag)) {
        actor->flags &= ~0x1;
        return;
    }
    if ((self->ocarinaSong != 6) || (SAVE_BYTE(0x2DD) != 0)) {
        if ((actor->xzDistToPlayer < (90.0f + self->interactRange)) &&
            (_fabsf(player->actor.world.pos.y - actor->world.pos.y) < 80.0f)) {
            if (player->stateFlags2 & 0x2000000) {
                u16 song = *(volatile u16*)&self->ocarinaSong;

                player->stateFlags2 |= 0x800000;
                if (song == 6) {
                    song = 0xA;
                }
                FUN_0037073c(play, song + 0x22);
                self->actionFunc = FUN_00288e3c;
            } else if ((actor->xzDistToPlayer < (50.0f + self->interactRange)) &&
                       (_fabsf(player->actor.world.pos.y - actor->world.pos.y) < 40.0f)) {
                self->unk_1B2 = 0;
                *(Actor**)((u8*)player + 0x1740) = actor;
            }
        }
    }
}

#define PLAY_CHIME() \
    Audio_PlaySoundGeneral(0x01000487, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0)
#define GS_BYTE(off) (*((u8*)&gSaveContext + (off)))

void FUN_00288e3c(EnOkarinaTag* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    u16 mode = OCARINA_MODE(play);

    if (mode == 4) {
        self->actionFunc = FUN_00288d0c;
        return;
    }
    if (mode == 3) {
        if (self->switchFlag >= 0) {
            Flags_SetSwitch(play, self->switchFlag);
        }
        if (play->sceneNum == 5) {
            FUN_00370778(play);
            if (self->switchFlag == 0x1C) {
                *(u32*)((u8*)&gSaveContext + 0x1000 + 0x3D0) |= 0x8;
            } else if (self->switchFlag == 0x1D) {
                *(u32*)((u8*)&gSaveContext + 0x1000 + 0x3D0) |= 0x10;
            }
        }
        s16 scene = play->sceneNum;
        if ((scene != 0x3B) && (scene != 0x3D)) {
            SET_OCARINA_MODE(play, 4);
        }
        switch (scene) {
            case 0x3B:
                if (((u8*)play)[0x5C02] == 0) {
                    goto sound;
                }
                if (GS_BYTE(0x4E) == 0) {
                    goto action;
                }
                goto sound;
            default:
                if (scene == 0x3D) {
                    if (GS_BYTE(0x4E) == 0) {
                        goto action;
                    }
                    goto sound;
                } else if (GS_BYTE(0xE) != 0) {
                    if (scene != 6) {
                        goto sound;
                    }
                    if (self->actor.room == 0xE) {
                        goto action;
                    }
                } else {
                    if (scene != 6) {
                        goto sound;
                    }
                }
                break;
        }
        if (self->actor.room == 0x18) {
            goto action;
        }
    sound:
        PLAY_CHIME();
    action:
        self->actionFunc = FUN_00288d0c;
        return;
    }
    if (self->unk_1B0 != 0) {
        if ((mode == 5) || (mode == 6) || (mode == 7) || (mode == 8) || (mode == 9) || (mode == 0xA) ||
            (mode == 0xD)) {
            if (self->switchFlag >= 0) {
                Flags_SetSwitch(play, self->switchFlag);
            }
            SET_OCARINA_MODE(play, 4);
            PLAY_CHIME();
            self->actionFunc = FUN_00288d0c;
            return;
        }
    }
    if ((u32)(mode - 5) <= 8) {
        SET_OCARINA_MODE(play, 4);
        self->actionFunc = FUN_00288d0c;
    } else if (mode == 1) {
        player->stateFlags2 |= 0x800000;
    }
}

static __inline __pure float EnOkarinaTag_Fabsf(float x) {
    return _fabsf(x);
}
void FUN_00274900(EnOkarinaTag* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    self->unk_1B2++;
    if ((self->ocarinaSong == 6) && (SAVE_BYTE(0x2DD) == 0)) {
        return;
    }
    if ((self->switchFlag >= 0) && Flags_GetSwitch(play, self->switchFlag)) {
        self->actor.flags &= ~0x1;
        return;
    }
    if (self->type == 4) {
        if (SONG_FLAGS_F4 & 0x800) {
            return;
        }
    } else if (self->type == 6) {
        if (SONG_FLAGS_EE & 0x2000) {
            return;
        }
    }
    if (self->actor.xzDistToPlayer >= (90.0f + self->interactRange)) {
        return;
    }
    if (EnOkarinaTag_Fabsf(player->actor.world.pos.y - self->actor.world.pos.y) >= 80.0f) {
        return;
    }
    if (player->stateFlags2 & 0x1000000) {
        switch (self->type) {
            case 1:
            case 6:
                FUN_0037073c(play, 0x24);
                break;
            case 2:
                FUN_0037073c(play, 0x27);
                break;
            case 4:
                FUN_0037073c(play, 0x26);
                break;
            default:
                Actor_Kill(&self->actor);
                break;
        }
        player->stateFlags2 |= 0x800000;
        self->actionFunc = FUN_00274acc;
    } else if ((self->actor.xzDistToPlayer < (50.0f + self->interactRange)) &&
               (EnOkarinaTag_Fabsf(player->actor.world.pos.y - self->actor.world.pos.y) < 40.0f)) {
        self->unk_1B2 = 0;
        player->stateFlags2 |= 0x800000;
    }
}

#define GAMEPLAY_KEEP_SLOT(play) (*((u8*)(play) + 0x3A5A))
#define GAMEPLAY_ZAR(play, idx) \
    (((play)->objectCtx.status[idx].unk_08 == 0) ? (ZARInfo*)0 : &(play)->objectCtx.status[idx].zarInfo)
#define SAVE_CS_TRIGGER (*((u8*)&gSaveContext + 0x1000 + 0x5A2))

void FUN_00274acc(EnOkarinaTag* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (OCARINA_MODE(play) == 4) {
        self->actionFunc = FUN_00274900;
        return;
    }
    if (OCARINA_MODE(play) == 3) {
        PLAY_CHIME();
        if (self->switchFlag >= 0) {
            Flags_SetSwitch(play, self->switchFlag);
        }
        switch (self->type) {
            case 1:
                Flags_SetSwitch(play, self->switchFlag);
                SONG_FLAGS_F2 |= 0x200;
                break;
            case 2:
                play->FUN_0037573c(GAMEPLAY_ZAR(play, GAMEPLAY_KEEP_SLOT(play))->GetQDBByIndex(0xA));
                SAVE_CS_TRIGGER = 1;
                FUN_0034e4fc(1.18921f, 0x87);
                break;
            case 4:
                play->FUN_0037573c(GAMEPLAY_ZAR(play, GAMEPLAY_KEEP_SLOT(play))->GetQDBByIndex(0xB));
                SAVE_CS_TRIGGER = 1;
                break;
            case 6:
                if (LINK_IS_ADULT) {
                    play->FUN_0037573c(play->sceneZARInfo.GetQDBByIndex(1));
                } else {
                        play->FUN_0037573c(GAMEPLAY_ZAR(play, GAMEPLAY_KEEP_SLOT(play))->GetQDBByIndex(0));
                }
                SAVE_CS_TRIGGER = 1;
                SONG_FLAGS_EE |= 0x2000;
                PLAY_CHIME();
                break;
            default:
                break;
        }
        SET_OCARINA_MODE(play, 4);
        self->actionFunc = FUN_00274900;
        return;
    }
    if ((u32)(OCARINA_MODE(play) - 5) <= 8) {
        SET_OCARINA_MODE(play, 4);
        self->actionFunc = FUN_00274900;
    } else if (OCARINA_MODE(play) == 1) {
        player->stateFlags2 |= 0x800000;
    }
}
}
