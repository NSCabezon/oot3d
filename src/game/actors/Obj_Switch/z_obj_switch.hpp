#ifndef _Z_OBJ_SWITCH_H_
#define _Z_OBJ_SWITCH_H_

#include "global.hpp"

struct ObjSwitch;
typedef void (*ObjSwitchActionFunc)(ObjSwitch*, PlayState*);

struct ObjSwitch {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x18];
    /* 0x01BC */ ObjSwitchActionFunc actionFunc;
    /* 0x01C0 */ s16 releaseTimer;
    /* 0x01C2 */ char unk_1C2[2];
    /* 0x01C4 */ s16 cooldownTimer;
    /* 0x01C6 */ u8 cooldownOn;
    /* 0x01C7 */ u8 unk_1C7;
    /* 0x01C8 */ s16 eyeTexIndex;
    /* 0x01CA */ char unk_1CA[0xA];
    /* 0x01D4 */ u8 crystalR;
    /* 0x01D5 */ u8 crystalG;
    /* 0x01D6 */ u8 crystalB;
    /* 0x01D7 */ u8 eyeColFlags;
    /* 0x01D8 */ char unk_1D8[8];
    /* 0x01E0 */ Actor* eyeHitActor;
    /* 0x01E4 */ char unk_1E4[5];
    /* 0x01E9 */ u8 eyeHitFlags;
    /* 0x01EA */ char unk_1EA[0xC6];
    /* 0x02B0 */ SkeletonAnimationModel* skelAnimModel[8];
    /* 0x02D0 */ char unk_2D0[0x2F4];
};

typedef char ObjSwitch_SizeCheck[sizeof(ObjSwitch) == 0x5C4 ? 1 : -1];

extern ActorInit Obj_Switch_InitVars;

void FUN_001cf100(ObjSwitch* thisx);
void FUN_001cf0e4(ObjSwitch* thisx);
void FUN_001eb278(ObjSwitch* thisx, PlayState* play);
void ObjSwitch_EyeInit(ObjSwitch* thisx, PlayState* play);
void ObjSwitch_EyeClosing(ObjSwitch* thisx, PlayState* play);
void ObjSwitch_EyeOpen(ObjSwitch* thisx, PlayState* play);
void ObjSwitch_EyeOpening(ObjSwitch* thisx, PlayState* play);
void ObjSwitch_FloorPress(ObjSwitch* thisx, PlayState* play);
void ObjSwitch_FloorRelease(ObjSwitch* thisx);
void ObjSwitch_EyeClosed(ObjSwitch* thisx, PlayState* play);
void ObjSwitch_EyeOpen(ObjSwitch* thisx, PlayState* play);
void ObjSwitch_CrystalOff(ObjSwitch* thisx, PlayState* play);
void caseD_0(Actor* actor, GameState* state);
void caseD_1(ObjSwitch* thisx, PlayState* play);
void ObjSwitch_FloorDownInit(ObjSwitch* thisx);

#endif
