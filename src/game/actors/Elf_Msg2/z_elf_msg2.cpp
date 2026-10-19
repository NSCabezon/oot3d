extern "C" {
#include "z_elf_msg2.hpp"

#define FLAGS 0x00000010

#define PARAMS_GET_U(params, start, count) (((params) >> (start)) & ((1 << (count)) - 1))

s32 FUN_00369a48(Actor* actor, PlayState* play); // N64: Actor_TextboxIsClosing

void ElfMsg2_Init(Actor* thisx, PlayState* play);
void ElfMsg2_Destroy(Actor* thisx, PlayState* play);
void ElfMsg2_Update(Actor* thisx, PlayState* play);

void FUN_003ee7b0(ElfMsg2* self, PlayState* play); // WaitForTextRead
void FUN_003f2314(ElfMsg2* self, PlayState* play); // WaitUntilActivated
void FUN_003f9384(ElfMsg2* self, PlayState* play); // WaitForTextClose

GLOBAL_ASM("data/z_Elf_Msg2.data.s")

ActorInit Elf_Msg2_InitVars = {
    ACTOR_ELF_MSG2,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(ElfMsg2),
    (ActorFunc)ElfMsg2_Init,
    (ActorFunc)ElfMsg2_Destroy,
    (ActorFunc)ElfMsg2_Update,
    NULL,
};

static s32 ElfMsg2_GetMessageId(ElfMsg2* self) {
    return PARAMS_GET_U(self->actor.params, 0, 8) + 0x100;
}

static s32 ElfMsg2_KillCheck(ElfMsg2* self, PlayState* play) {
    s32 unused = 0; // not in source: needed to reproduce the original register allocation (play in r6)
    if ((self->actor.world.rot.y > 0) && (self->actor.world.rot.y <= 0x40) &&
        Flags_GetSwitch(play, self->actor.world.rot.y - 1)) {
        if (PARAMS_GET_U(self->actor.params, 8, 6) != 0x3F) {
            Flags_SetSwitch(play, PARAMS_GET_U(self->actor.params, 8, 6));
        }
        Actor_Kill(&self->actor);
        return 1;
    } else if ((self->actor.world.rot.y == -1) && Flags_GetClear(play, self->actor.room)) {
        if (PARAMS_GET_U(self->actor.params, 8, 6) != 0x3F) {
            Flags_SetSwitch(play, PARAMS_GET_U(self->actor.params, 8, 6));
        }
        Actor_Kill(&self->actor);
        return 1;
    } else if (PARAMS_GET_U(self->actor.params, 8, 6) == 0x3F) {
        return 0;
    } else if (Flags_GetSwitch(play, PARAMS_GET_U(self->actor.params, 8, 6))) {
        Actor_Kill(&self->actor);
        return 1;
    }
    return 0;
}

void ElfMsg2_Init(Actor* thisx, PlayState* play) {
    ElfMsg2* self = (ElfMsg2*)thisx;

    if (!ElfMsg2_KillCheck(self, play)) {
        if ((self->actor.world.rot.x > 0) && (self->actor.world.rot.x <= 7)) {
            self->actor.targetMode = self->actor.world.rot.x - 1;
        }
        Actor_ProcessInitChain(thisx, (InitChainEntry*)0x00520364); // TODO: sInitChain in data/z_Elf_Msg2.data.s (raw address)
        if (self->actor.world.rot.y > 0x40) {
            self->actionFunc = FUN_003f2314;
        } else {
            self->actionFunc = FUN_003ee7b0;
            self->actor.flags |= 0x40000;
            self->actor.flags |= 1;
            self->actor.textId = ElfMsg2_GetMessageId(self);
        }
        self->actor.shape.rot.x = self->actor.shape.rot.y = self->actor.shape.rot.z = 0;
    }
}

void ElfMsg2_Destroy(Actor* thisx, PlayState* play) {
}

void FUN_003ee7b0(ElfMsg2* self, PlayState* play) {
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        self->actionFunc = FUN_003f9384;
    }
}

void FUN_003f2314(ElfMsg2* self, PlayState* play) {
    if ((self->actor.world.rot.y >= 0x41) && (self->actor.world.rot.y <= 0x80) &&
        Flags_GetSwitch(play, self->actor.world.rot.y - 0x41)) {
        self->actionFunc = FUN_003ee7b0;
        self->actor.flags |= 0x40000;
        self->actor.flags |= 1;
        self->actor.textId = ElfMsg2_GetMessageId(self);
    }
}

void FUN_003f9384(ElfMsg2* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        if (self->actor.world.rot.z == 1) {
            self->actionFunc = FUN_003ee7b0;
        } else {
            Actor_Kill(&self->actor);
            if (PARAMS_GET_U(self->actor.params, 8, 6) != 0x3F) {
                Flags_SetSwitch(play, PARAMS_GET_U(self->actor.params, 8, 6));
            }
        }
    }
}

void ElfMsg2_Update(Actor* thisx, PlayState* play) {
    ElfMsg2* self = (ElfMsg2*)thisx;

    if (!ElfMsg2_KillCheck(self, play)) {
        self->actionFunc(self, play);
    }
}
}
