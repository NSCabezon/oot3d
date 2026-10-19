extern "C" {
#include "z_obj_switch.hpp"

s32 FUN_0036ae04(void);
void ObjSwitch_SetOn(ObjSwitch* thisx, PlayState* play);
void ObjSwitch_FloorUp(ObjSwitch* thisx, PlayState* play);
void ObjSwitch_EyeClosing(ObjSwitch* thisx, PlayState* play);
void ObjSwitch_FloorDown(ObjSwitch* thisx, PlayState* play);
void caseD_2_0016ea9c(ObjSwitch* thisx, PlayState* play);
void caseD_3(ObjSwitch* thisx, PlayState* play);

void FUN_001eb278(ObjSwitch* thisx, PlayState* play) {
    thisx->crystalR = 0;
    thisx->crystalG = 0;
    thisx->crystalB = 0;
    thisx->actionFunc = ObjSwitch_CrystalOff;
}

void ObjSwitch_EyeInit(ObjSwitch* thisx, PlayState* play) {
    if (Flags_GetSwitch(play, ((u16)thisx->actor.params >> 8) & 0x3F)) {
        thisx->actionFunc = ObjSwitch_EyeClosed;
        thisx->eyeTexIndex = 3;
    } else {
        thisx->actionFunc = ObjSwitch_EyeOpen;
        thisx->eyeTexIndex = 0;
    }
}

void ObjSwitch_EyeClosing(ObjSwitch* thisx, PlayState* play) {
    if (!thisx->cooldownOn || FUN_0036ae04() == thisx->actor.type || thisx->cooldownTimer <= 0) {
        if (play->state.frames & 1) {
            thisx->eyeTexIndex++;
        }
        if (thisx->eyeTexIndex >= 3) {
            thisx->actionFunc = ObjSwitch_EyeClosed;
            thisx->eyeTexIndex = 3;
            Audio_PlayActorSound2(&thisx->actor, 0x0100016C);
        }
    }
}

void caseD_1(ObjSwitch* thisx, PlayState* play) {
    if (thisx->skelAnimModel[0] != NULL) {
        thisx->skelAnimModel[0]->FUN_003721e0(&thisx->actor.modelMtx);
        thisx->skelAnimModel[0]->SetUnkAC(1);
        thisx->skelAnimModel[0]->FUN_00372170(0);
    }
}

void caseD_0(Actor* actor, GameState* state) {
    ObjSwitch* thisx = (ObjSwitch*)actor;
    s32 type = ((u32)(u16)actor->params << 25) >> 29;

    FUN_00357fd0(GET_PLAYER((PlayState*)state), actor->unk_178, &actor->world.pos);
    if (thisx->skelAnimModel[type] != NULL) {
        thisx->skelAnimModel[type]->FUN_003721e0(&thisx->actor.modelMtx);
        thisx->skelAnimModel[type]->SetUnkAC(1);
        thisx->skelAnimModel[type]->FUN_00372170(0);
    }
}

void ObjSwitch_EyeOpen(ObjSwitch* thisx, PlayState* play) {
    if ((thisx->eyeHitFlags & 2) && !(thisx->eyeColFlags & 2) && thisx->eyeHitActor != NULL) {
        s32 yawDiff = (s16)(thisx->eyeHitActor->world.rot.y - thisx->actor.shape.rot.y);
        if ((u32)(yawDiff + 0x5000) > 0xA000) {
            goto eyeOpenClose;
        }
    }
    if ((s32)((u32)(u16)thisx->actor.params << 24) >= 0) {
        return;
    }
eyeOpenClose:
    {
        thisx->actionFunc = ObjSwitch_EyeClosing;
        thisx->cooldownTimer = 150;
        ObjSwitch_SetOn(thisx, play);
        thisx->actor.params &= ~0x80;
    }
}

void ObjSwitch_EyeOpening(ObjSwitch* thisx, PlayState* play) {
    if ((((u16)thisx->actor.params >> 4) & 7) != 1 || !thisx->cooldownOn ||
        FUN_0036ae04() == thisx->actor.type || thisx->cooldownTimer <= 0) {
        if (play->state.frames & 1) {
            thisx->eyeTexIndex--;
        }
        if (thisx->eyeTexIndex <= 0) {
            FUN_001cf100(thisx);
            Audio_PlayActorSound2(&thisx->actor, 0x0100016C);
        }
    }
}

void ObjSwitch_FloorRelease(ObjSwitch* thisx) {
    s32 type = ((u16)thisx->actor.params >> 4) & 7;

    if ((type != 1 && type != 3) || !thisx->cooldownOn || FUN_0036ae04() == thisx->actor.type ||
        thisx->cooldownTimer <= 0) {
        thisx->actor.scale.y += 0.0495f;
        if (thisx->actor.scale.y >= 0.165f) {
            FUN_001cf0e4(thisx);
            Audio_PlayActorSound2(&thisx->actor, 0x0100016C);
        }
    }
}

static inline void ObjSwitch_SetFloorDown(ObjSwitch* thisx) {
    thisx->actor.scale.y = 0.0165f;
    thisx->releaseTimer = 9;
    thisx->actionFunc = ObjSwitch_FloorDown;
}

void ObjSwitch_FloorPress(ObjSwitch* thisx, PlayState* play) {
    if (3 == (((u32)(u16)thisx->actor.params << 25) >> 29) || !thisx->cooldownOn || FUN_0036ae04() == thisx->actor.type ||
        thisx->cooldownTimer <= 0) {
        thisx->actor.scale.y -= 0.0495f;
        if (thisx->actor.scale.y <= 0.0165f) {
            ObjSwitch_SetFloorDown(thisx);
            Audio_PlayActorSound2(&thisx->actor, 0x0100016C);
            if (thisx->actor.params & 0x4000) {
                Player* player = GET_PLAYER(play);
                player->actor.freezeTimer = thisx->cooldownTimer;
            }
        }
    }
}

#define FLAGS 0x00000010

void ObjSwitch_Init(Actor* actor, GameState* state);
void ObjSwitch_Destroy(Actor* actor, GameState* state);
void ObjSwitch_Update(Actor* actor, GameState* state);
void ObjSwitch_Draw(Actor* actor, GameState* state);
typedef void (*ObjSwitchDrawFunc)(Actor*, PlayState*);
extern ObjSwitchDrawFunc ObjSwitch_sDrawFuncs[8];
GLOBAL_ASM("../src/game/actors/Obj_Switch/data_syms.s")
void ObjSwitch_Draw(Actor* actor, GameState* state) {
    ObjSwitch_sDrawFuncs[(u16)actor->params & 7](actor, (PlayState*)state);
}

GLOBAL_ASM("data/z_Obj_Switch.data.s")

ActorInit Obj_Switch_InitVars = {
    ACTOR_OBJ_SWITCH,
    ACTORCAT_SWITCH,
    FLAGS,
    OBJECT_GAMEPLAY_DANGEON_KEEP,
    sizeof(ObjSwitch),
    (ActorFunc)ObjSwitch_Init,
    (ActorFunc)ObjSwitch_Destroy,
    (ActorFunc)ObjSwitch_Update,
    (ActorFunc)ObjSwitch_Draw,
};

GLOBAL_ASM("asm/ObjSwitch_Init.s")

GLOBAL_ASM("asm/ObjSwitch_Destroy.s")

GLOBAL_ASM("asm/ObjSwitch_Update.s")

GLOBAL_ASM("asm/ObjSwitch_CrystalOff.s")

GLOBAL_ASM("asm/ObjSwitch_CrystalOn.s")

GLOBAL_ASM("asm/ObjSwitch_CrystalTurnOff.s")

GLOBAL_ASM("asm/ObjSwitch_CrystalTurnOn.s")

GLOBAL_ASM("asm/ObjSwitch_EyeClosed.s")

GLOBAL_ASM("../split/asm/FUN_001442ec.s")

GLOBAL_ASM("asm/ObjSwitch_FloorDown.s")

GLOBAL_ASM("asm/ObjSwitch_FloorUp.s")

GLOBAL_ASM("asm/ObjSwitch_InitTrisCollider.s")

GLOBAL_ASM("asm/ObjSwitch_SetOff.s")

GLOBAL_ASM("asm/ObjSwitch_SetOn.s")

}
