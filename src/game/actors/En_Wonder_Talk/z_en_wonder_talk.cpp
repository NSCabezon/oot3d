extern "C" {
#include "z_en_wonder_talk.hpp"

#define FLAGS 0x88000009

void EnWonderTalk_Init(Actor* actor, PlayState* play);
void EnWonderTalk_Destroy(Actor* actor, PlayState* play);
void EnWonderTalk_Update(Actor* actor, PlayState* play);

void FUN_0025a1d0(EnWonderTalk* self, PlayState* play); // N64: func_80B391CC
void FUN_001cbb90(EnWonderTalk* self, PlayState* play); // N64: func_80B3943C
void FUN_002065e0(EnWonderTalk* self, PlayState* play); // N64: func_80B395F0
void FUN_003725e0(PlayState* play);                      // Message_CloseTextbox

ActorInit En_Wonder_Talk_InitVars = {
    ACTOR_EN_WONDER_TALK,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnWonderTalk),
    (ActorFunc)EnWonderTalk_Init,
    (ActorFunc)EnWonderTalk_Destroy,
    (ActorFunc)EnWonderTalk_Update,
    NULL,
};

void EnWonderTalk_Destroy(Actor* actor, PlayState* play) {
}

// Every access must go through self->actor (incl. Actor_Kill(&self->actor)); any use of thisx makes armcc hoist `mov r3,#1`.
void EnWonderTalk_Init(Actor* thisx, PlayState* play) {
    EnWonderTalk* self = (EnWonderTalk*)thisx;

    self->unk_150 = ((u32)self->actor.params << 16) >> 27;
    self->unk_152 = ((u32)self->actor.params << 21) >> 27;
    self->switchFlag = self->actor.params & 0x3F;
    if (self->switchFlag == 0x3F) {
        self->switchFlag = -1;
    }
    self->actor.targetMode = 1;
    if (self->switchFlag >= 0) {
        if (Flags_GetSwitch(play, self->switchFlag)) {
            Actor_Kill(&self->actor);
            return;
        }
    }
    self->unk_15C = 40.0f;
    self->actionFunc = FUN_0025a1d0;
}

#if 0 // FUN_0025a1d0: C matches bytes 0x25a1d0..0x25a340, but the asm region also holds the tail 0x25a340..0x25a384
// (an En_Wonder_Talk2 action fn merged by functions.tsv). Stays GLOBAL_ASM until the cfg is split.
void FUN_0025a1d0(EnWonderTalk* self, PlayState* play) {
    if (self->switchFlag < 0 || !Flags_GetSwitch(play, self->switchFlag)) {
        switch (self->unk_150) {
            case 1:
                self->height = 0.0f;
                self->unk_15C = 80.0f;
                if (!LINK_IS_ADULT) {
                    self->actor.textId = 0x7040;
                } else {
                    self->actor.textId = 0x7088;
                }

                self->unk_156 = TEXT_STATE_EVENT;
                break;
            case 2:
                self->unk_15C = 40.0f;
                self->actor.textId = 0x5002;
                self->unk_156 = TEXT_STATE_CHOICE;
                self->height = 30.0f;
                break;
            case 3:
                self->height = 0.0f;
                self->actor.textId = 0x501E;
                self->unk_156 = TEXT_STATE_EVENT;
                self->unk_15C = 110.0f;
                break;
            case 4:
                self->height = 0.0f;
                self->actor.textId = 0x5020;
                self->unk_156 = TEXT_STATE_DONE;
                self->unk_15C = 120.0f;
                if (gSaveContext.eventChkInf[1] & 0x2000) {
                    Actor_Kill(&self->actor);
                }
                break;
            case 5:
                self->height = 0.0f;
                self->actor.textId = 0x501F;
                self->unk_156 = TEXT_STATE_EVENT;
                self->unk_15C = 110.0f;
                break;
            default:
                self->actor.textId = 0x7072;
                self->unk_156 = TEXT_STATE_EVENT;
                break;
        }

        self->actionFunc = FUN_001cbb90;
    }
}

#endif

void FUN_001cbb90(EnWonderTalk* self, PlayState* play) {
    s16 yawDiff;
    s16 yawDiffTemp;

    self->unk_15A++;
    if (self->unk_150 == 4 && (gSaveContext.eventChkInf[1] & 0x2000)) {
        Actor_Kill(&self->actor);
        return;
    }
    if (self->switchFlag < 0 || !Flags_GetSwitch(play, self->switchFlag)) {
        if (Actor_ProcessTalkRequest(&self->actor, play)) {
            if (self->unk_156 != TEXT_STATE_DONE) {
                self->actionFunc = FUN_002065e0;
            } else {
                if (self->switchFlag >= 0) {
                    self->actor.flags &= ~1;
                    Flags_SetSwitch(play, self->switchFlag);
                }
                self->actionFunc = FUN_0025a1d0;
            }
        } else if (self->actor.xzDistToPlayer > self->unk_15C) {
        } else {
            yawDiffTemp = (self->actor.yawTowardsPlayer - self->actor.world.rot.y);
            yawDiff = ABS(yawDiffTemp);

            if (yawDiff < 0x4000) {
                self->unk_15A = 0;
                FUN_0036bb28(&self->actor, play, self->unk_15C);
            }
        }
    }
}

void FUN_002065e0(EnWonderTalk* self, PlayState* play) {
    if (self->unk_156 == Message_GetState(&play->msgCtx) && Message_ShouldAdvance(play)) {
        if (self->switchFlag >= 0) {
            self->actor.flags &= ~1;
            Flags_SetSwitch(play, self->switchFlag);
        }
        switch (self->unk_150) {
            case 1:
                FUN_003725e0(play);
                self->actionFunc = FUN_0025a1d0;
                break;
            case 2:
                switch (FUN_00369f3c(play)) {
                    case 0:
                        if (!LINK_IS_ADULT) {
                            self->actor.textId = 0x5001;
                        } else {
                            self->actor.textId = 0x5003;
                        }
                        break;
                    case 1:
                        self->actor.textId = 0x5004;
                        break;
                }

                self->unk_156 = TEXT_STATE_DONE;
                Message_ContinueTextbox(play, self->actor.textId);
                self->actionFunc = FUN_0025a1d0;
                break;
            case 3:
                FUN_003725e0(play);
                if (self->unk_164 == 0) {
                    Actor_Spawn(&play->actorCtx, play, ACTOR_EN_POH, self->actor.world.pos.x, self->actor.world.pos.y,
                                self->actor.world.pos.z, 0, 0, 0, 2, 1);
                    self->unk_164 = 1;
                }

                self->actionFunc = FUN_0025a1d0;
                break;
            case 5:
                FUN_003725e0(play);
                if (self->unk_164 == 0) {
                    Actor_Spawn(&play->actorCtx, play, ACTOR_EN_POH, self->actor.world.pos.x, self->actor.world.pos.y,
                                self->actor.world.pos.z, 0, 0, 0, 3, 1);
                    self->unk_164 = 1;
                }
                self->actionFunc = FUN_0025a1d0;
                break;
        }
    }
}

void EnWonderTalk_Update(Actor* thisx, PlayState* play) {
    EnWonderTalk* self = (EnWonderTalk*)thisx;

    if (self->unk_158 != 0) {
        self->unk_158--;
    }
    self->actionFunc(self, play);
    Actor_SetFocus(&self->actor, self->height);
}
}
