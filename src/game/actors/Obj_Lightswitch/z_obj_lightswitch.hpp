#ifndef _Z_OBJ_LIGHTSWITCH_H_
#define _Z_OBJ_LIGHTSWITCH_H_

#include "global.hpp"

struct ObjLightswitch;

typedef void (*ObjLightswitchActionFunc)(struct ObjLightswitch*, PlayState*);

typedef enum ObjLightswitch_Type {
    /* 0 */ OBJLIGHTSWITCH_TYPE_STAY_ON, // doesn't turn off unless the switch flag is cleared some other way
    /* 1 */ OBJLIGHTSWITCH_TYPE_1,       // turns on and off
    /* 2 */ OBJLIGHTSWITCH_TYPE_2,       // turns on and off
    /* 3 */ OBJLIGHTSWITCH_TYPE_BURN     // disappears when turned on
} ObjLightswitch_Type;

// JntSph collider types are not in the repo headers yet (3DS layout, local for now).
typedef struct {
    /* 0x00 */ Vec3f center;
    /* 0x0C */ f32 radius;
} ObjLightswitchSphere; // size = 0x10

typedef struct {
    /* 0x00 */ u8 unk_00[0x38];
    /* 0x38 */ ObjLightswitchSphere worldSphere;
    /* 0x48 */ u8 unk_48[0x08];
} ObjLightswitchColliderElement; // size = 0x50

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ ObjLightswitchColliderElement* elements;
} ObjLightswitchColliderJntSph; // size = 0x20

typedef struct ObjLightswitch {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ObjLightswitchActionFunc actionFunc;
    /* 0x01A8 */ ObjLightswitchColliderJntSph collider;
    /* 0x01C8 */ ObjLightswitchColliderElement colliderElements[1];
    /* 0x0218 */ s16 timer;       // collision-related threshold and controls animation/logic when turning on/off
    /* 0x021A */ s16 toggleDelay; // timer ticking down used for delaying turning on/off or disappearing, can be bypassed
    /* 0x021C */ s16 faceTextureIndex; // texture used by the center part of the sun
    /* 0x021E */ s16 color[3];         // rgb, (0-255 color component) << 6
    /* 0x0224 */ s16 alpha;            // (0-255 alpha) << 6
    /* 0x0226 */ s16 flameRingRot;
    /* 0x0228 */ s16 flameRingRotSpeed;
    /* 0x022A */ u8 prevFrameACflags;
    /* 0x022B */ char unk_22B[0x1];
    /* 0x022C */ SkeletonAnimationModel* skelAnimModel[3];
    /* 0x0238 */ char unk_238[0x1CC];
} ObjLightswitch; // size = 0x0404

extern ActorInit Obj_Lightswitch_InitVars;

#endif
