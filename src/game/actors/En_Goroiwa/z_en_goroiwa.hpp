#ifndef _Z_EN_GOROIWA_H_
#define _Z_EN_GOROIWA_H_

#include "global.hpp"

struct EnGoroiwa;

typedef void (*EnGoroiwaActionFunc)(struct EnGoroiwa*, PlayState*);

typedef struct {
    /* 0x00 */ Vec3f center;
    /* 0x0C */ f32 radius;
} EnGoroiwaSphere; // size = 0x10

typedef struct {
    /* 0x00 */ u8 unk_00[0x38];
    /* 0x38 */ EnGoroiwaSphere worldSphere;
    /* 0x48 */ u8 unk_48[0x08];
} EnGoroiwaColliderElement; // size = 0x50

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ EnGoroiwaColliderElement* elements;
} EnGoroiwaColliderJntSph; // size = 0x20

typedef struct EnGoroiwa {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01A8 */ EnGoroiwaActionFunc actionFunc;
    /* 0x01AC */ EnGoroiwaColliderJntSph collider;
    /* 0x01CC */ EnGoroiwaColliderElement colliderElements[1];
    /* 0x021C */ char unk_21C[0x10];
    /* 0x022C */ f32 rollRotSpeed;
    /* 0x0230 */ s16 waitTimer;
    /* 0x0232 */ char unk_232[0x2];
    /* 0x0234 */ s16 collisionDisabledTimer;
    /* 0x0236 */ s16 endWaypoint;
    /* 0x0238 */ s16 currentWaypoint;
    /* 0x023A */ s16 nextWaypoint;
    /* 0x023C */ s16 pathDirection;
    /* 0x023E */ u8 isInKokiri;
    /* 0x023F */ u8 stateFlags;
} EnGoroiwa; // size = 0x240

extern ActorInit En_Goroiwa_InitVars;

#endif
