extern "C" {
#include "z_en_daiku_kakariko.hpp"

extern s32 EnDaikuKakariko_sMaskReactionSets[];

void EnDaikuKakariko_HandleTalking(EnDaikuKakariko* self, PlayState* play) {
    s16 screenX;
    s16 screenY;

    if (self->talkState == 2) {
        s32 talkState = 2;

        if ((Message_GetState(&play->msgCtx) == 6) && Message_ShouldAdvance(play)) {
            switch (self->actor.textId) {
                case 0x6061:
                    gSaveContext.infTable[23] |= 0x40;
                    break;
                case 0x6064:
                    gSaveContext.infTable[23] |= 0x100;
                    break;
            }
            talkState = 0;
        }
        self->talkState = talkState;
    } else if (Actor_ProcessTalkRequest(&self->actor, play)) {
        self->talkState = 2;
    } else {
        Actor_GetScreenPos(play, &self->actor, &screenX, &screenY);

        if ((screenX >= 0) && (screenX <= 320) && (screenY >= 0) && (screenY <= 240) && (self->talkState == 0) &&
            (FUN_0036bb28(&self->actor, play, 100.0f) == 1)) {
            self->actor.textId = Text_GetFaceReaction(play, EnDaikuKakariko_sMaskReactionSets[self->actor.params & 3]);

            if (self->actor.textId == 0) {
                switch (self->actor.params & 3) {
                    case 0:
                        if (self->flags & 8) {
                            self->actor.textId = 0x5076;
                        } else {
                            self->actor.textId = 0x5075;
                        }
                        break;
                    case 1:
                        if (self->flags & 1) {
                            self->actor.textId = 0x502A;
                        } else {
                            self->actor.textId = 0x5074;
                        }
                        break;
                    case 2:
                        if (self->flags & 1) {
                            self->actor.textId = 0x506A;
                        } else {
                            self->actor.textId = 0x506B;
                        }
                        break;
                    case 3:
                        if (self->flags & 1) {
                            self->actor.textId = 0x5077;
                        } else {
                            self->actor.textId = 0x5078;
                        }
                        break;
                }
            }
        }
    }
}

}
