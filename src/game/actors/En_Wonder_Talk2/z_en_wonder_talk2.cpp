extern "C" {
#include "z_en_wonder_talk2.hpp"

#define FLAGS 0x08000009
#define PARAMS_GET_U(p, s, n) (((p) >> (s)) & ((1 << (n)) - 1))

void EnWonderTalk2_Init(Actor* actor, PlayState* play);
void EnWonderTalk2_Destroy(Actor* actor, PlayState* play);
void EnWonderTalk2_Update(Actor* actor, PlayState* play);

GLOBAL_ASM("data/z_En_Wonder_Talk2.data.s")

ActorInit En_Wonder_Talk2_InitVars = {
    ACTOR_EN_WONDER_TALK2,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnWonderTalk2),
    (ActorFunc)EnWonderTalk2_Init,
    (ActorFunc)EnWonderTalk2_Destroy,
    (ActorFunc)EnWonderTalk2_Update,
    NULL,
};


// Init draft (Opus closing pass): SIZE EXACT (464 B), 15/116 words differ (fastchk), all in the rot.z
// range loop: armcc's closed-form division uses r2/r1/r3 where ROM uses r3/r2/r1 (pure reg permutation;
// ~60 loop/type/decl variants tried). Everything else (gBitFlags folding, table index, params reload,
// talkMode==3 tail) matches. Keys: Actor* thisx signature, CHECK_QUEST_ITEM (gBitFlags symbol), table via
// EQU symbol in data_syms.s, `thisx->flags &=` (stops store merging), `s16 rotZmod10` declared before
// `s32 rangeIndex`. Kept #if 0: a non-identical Init would be a new differing function.
#if 0
void FUN_00157294(EnWonderTalk2* self, PlayState* play); // EnWonderTalk2_DoNothing
extern s16 EnWonderTalk2_sRangeTypes[];
// when enabling: add the GLOBAL-ASM include of ../src/game/actors/En_Wonder_Talk2/data_syms.s here

void EnWonderTalk2_Init(Actor* thisx, PlayState* play) {
    EnWonderTalk2* self = (EnWonderTalk2*)thisx;

    self->baseMsgId = PARAMS_GET_U(self->actor.params, 6, 8);
    if (self->baseMsgId == 0x3D) {
        if (!CHECK_QUEST_ITEM(11)) {
            self->baseMsgId = 0x41;
        }
    }
    if (self->actor.world.rot.z > 0) {
        s16 rotZmod10 = self->actor.world.rot.z;
        s32 rangeIndex = 0;

        while (rotZmod10 > 10) {
            rotZmod10 -= 10;
            rangeIndex++;
        }
        self->triggerRange = rotZmod10 * 40.0f;
        if (rangeIndex > 6) {
            rangeIndex = 0;
        }
        self->actor.targetMode = EnWonderTalk2_sRangeTypes[rangeIndex];
    }

    self->unk_1C8 = 1;
    if (self->actor.world.rot.x != 0) {
        self->unk_1C8 = 0;
    }
    self->unk_1C9 = 0;
    if (self->baseMsgId == 0x36) {
        self->unk_1C9 = 1;
    }

    self->initPos = self->actor.world.pos;
    self->switchFlag = PARAMS_GET_U(self->actor.params, 0, 6);
    self->talkMode = PARAMS_GET_U(self->actor.params, 14, 2);

    if (self->switchFlag == 0x3F) {
        self->switchFlag = -1;
    }
    if (self->switchFlag >= 0 && Flags_GetSwitch(play, self->switchFlag)) {
        Actor_Kill(&self->actor);
        return;
    }
    if (self->talkMode == 1 && play->sceneNum == 0xB && self->switchFlag != 0x08 && self->switchFlag != 0x16 &&
        self->switchFlag != 0x2F) {
        self->unk_15A = false;
        self->talkMode = 4;
    }
    if (self->talkMode == 3) {
        thisx->flags &= ~0x8000000;
        self->actionFunc = FUN_00157294;
    } else {
        self->actionFunc = (EnWonderTalk2ActionFunc)0x0025A340;
    }
}
#endif

GLOBAL_ASM("asm/EnWonderTalk2_Init.s")

void EnWonderTalk2_Destroy(Actor* actor, PlayState* play) {
}

void EnWonderTalk2_Update(Actor* thisx, PlayState* play) {
    EnWonderTalk2* self = (EnWonderTalk2*)thisx;
    EnWonderTalk2ActionFunc action = self->actionFunc;

    action(self, play);
    self->actor.world.pos.y = self->initPos.y;
    Actor_SetFocus(&self->actor, self->height);
}
}
