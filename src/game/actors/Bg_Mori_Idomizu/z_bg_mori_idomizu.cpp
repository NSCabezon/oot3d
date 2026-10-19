extern "C" {
#include "z_bg_mori_idomizu.hpp"

#define FLAGS 0x80400030

void BgMoriIdomizu_Init(Actor* actor, GameState* state);
void BgMoriIdomizu_Destroy(Actor* actor, GameState* state);
void BgMoriIdomizu_Update(Actor* actor, GameState* state);

void FUN_0011076c(BgMoriIdomizu* self, PlayState* play);
void FUN_003f7224(BgMoriIdomizu* self, PlayState* play);
void FUN_002ac5a0(Actor* actor, GameState* state);

s32 Object_IsLoaded(ObjectContext* objectCtx, s32 bankIndex);
s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
s32 OnePointCutscene_Init(PlayState* play, s32 csId, s32 arg2, Actor* actor, s32 camId);

static struct {
    s16 isSpawned;
    s16 unk_02;
} sData = { 0, -1 };

ActorInit Bg_Mori_Idomizu_InitVars = {
    ACTOR_BG_MORI_IDOMIZU,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_MORI_OBJECTS,
    sizeof(BgMoriIdomizu),
    (ActorFunc)BgMoriIdomizu_Init,
    (ActorFunc)BgMoriIdomizu_Destroy,
    (ActorFunc)BgMoriIdomizu_Update,
    NULL,
};

static inline void BgMoriIdomizu_SetWaterLevel(PlayState* play, s16 waterLevel) {
    // 3DS WaterBox stride is 0x10 (repo type is 4 bytes): entries 2..4 are indices 8, 12, 16 of the 4-byte type
    WaterBox* waterBox = play->colCtx.stat.colHeader->waterBoxes;

    waterBox[2 * 4].ySurface = waterLevel;
    waterBox[3 * 4].ySurface = waterLevel;
    waterBox[4 * 4].ySurface = waterLevel;
}

void BgMoriIdomizu_Init(Actor* actor, GameState* state) {
    BgMoriIdomizu* self = (BgMoriIdomizu*)actor;
    PlayState* play = (PlayState*)state;
    ZARInfo* zarInfo;

    if (sData.isSpawned) {
        Actor_Kill(actor);
        return;
    }
    self->actor.scale.x = 1.1f;
    self->actor.scale.z = 1.0f;
    self->actor.scale.y = 1.0f;
    self->actor.world.pos.x = 119.0f;
    self->actor.world.pos.z = -1820.0f;
    self->prevSwitchFlagSet = Flags_GetSwitch(play, self->actor.params & 0x3F);
    if (self->prevSwitchFlagSet != 0) {
        self->actor.world.pos.y = -282.0f;
        BgMoriIdomizu_SetWaterLevel(play, -282);
    } else {
        self->actor.world.pos.y = 184.0f;
        BgMoriIdomizu_SetWaterLevel(play, 184);
    }
    self->moriTexObjectSlot = Object_GetIndex(&play->objectCtx, 0x73);
    if (self->moriTexObjectSlot < 0) {
        Actor_Kill(actor);
        return;
    }
    zarInfo = FUN_00372f38(actor, play, &self->skelAnimModel, 5, 0);
    self->skelAnimModel->unk_0C->FUN_00372d94(zarInfo->GetCMABByIndex(0));
    self->skelAnimModel->unk_0C->SetAnimType(1);
    self->actionFunc = FUN_0011076c;
    sData.isSpawned = true;
    self->isLoaded = true;
    self->actor.room = -1;
}

void BgMoriIdomizu_Destroy(Actor* actor, GameState* state) {
    BgMoriIdomizu* self = (BgMoriIdomizu*)actor;

    if (self->isLoaded) {
        sData.isSpawned = false;
    }
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgMoriIdomizu_Update(Actor* actor, GameState* state) {
    BgMoriIdomizu* self = (BgMoriIdomizu*)actor;
    PlayState* play = (PlayState*)state;

    if (self->actionFunc != NULL) {
        self->actionFunc(self, play);
    }
}

void FUN_0011076c(BgMoriIdomizu* self, PlayState* play) {
    if (Object_IsLoaded(&play->objectCtx, self->moriTexObjectSlot)) {
        self->actionFunc = FUN_003f7224;
        self->actor.draw = (ActorFunc)FUN_002ac5a0;
    }
}

void FUN_003f7224(BgMoriIdomizu* self, PlayState* play) {
    Actor* thisx = &self->actor;
    s8 roomNum;
    s32 switchFlagSet;

    roomNum = play->roomCtx.unk_00[0]; // curRoom.num (raw, RoomContext not typed)
    switchFlagSet = Flags_GetSwitch(play, thisx->params & 0x3F);
    if (switchFlagSet) {
        self->targetWaterLevel = -282.0f;
    } else {
        self->targetWaterLevel = 184.0f;
    }
    if (switchFlagSet && !self->prevSwitchFlagSet) {
        OnePointCutscene_Init(play, 3240, 70, thisx, 0);
        self->drainTimer = 135;
    } else if (!switchFlagSet && self->prevSwitchFlagSet) {
        OnePointCutscene_Init(play, 3240, 70, thisx, 0);
        self->drainTimer = 135;
        thisx->world.pos.y = 0.0f;
    }
    self->drainTimer--;
    if ((roomNum == 7) || (roomNum == 8) || (roomNum == 9)) {
        if (self->drainTimer < 105) {
            Math_StepToF(&thisx->world.pos.y, self->targetWaterLevel, 3.5f);
            BgMoriIdomizu_SetWaterLevel(play, thisx->world.pos.y);
            if (self->drainTimer > 0) {
                if (switchFlagSet) {
                    Audio_PlaySoundGeneral(0x10001B5, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
                } else {
                    Audio_PlaySoundGeneral(0x10001B5, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
                }
            }
        }
    } else {
        thisx->world.pos.y = self->targetWaterLevel;
        BgMoriIdomizu_SetWaterLevel(play, thisx->world.pos.y);
        Actor_Kill(thisx);
        return;
    }
    self->prevSwitchFlagSet = switchFlagSet;
}

void FUN_002ac5a0(Actor* actor, GameState* state) {
    BgMoriIdomizu* self = (BgMoriIdomizu*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->actor.modelMtx);

    f32 animSpeed = 1.0f;
    if (PauseContext_GetState() != 0) {
        animSpeed = 0.0f;
    }

    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->unk_0C->SetAnimSpeed(animSpeed);
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}
}
