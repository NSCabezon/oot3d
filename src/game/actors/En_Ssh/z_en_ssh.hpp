#ifndef _Z_EN_SSH_H_
#define _Z_EN_SSH_H_

#include "global.hpp"
#include <stddef.h>

struct EnSsh;

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ void* elements;
} EnSshColliderJntSph; // size = 0x20

typedef struct {
    /* 0x00 */ char unk_00[0x34];
    /* 0x34 */ f32 modelSphereRadius;
    /* 0x38 */ char unk_38[0x18];
} EnSshColliderElement; // size = 0x50

typedef struct EnSsh {
    /* 0x0000, sizeof 0x01A4 */ Actor actor;
    /* 0x01A4, sizeof 0x0084 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x750];
    /* 0x0978, sizeof 0x0004 */ void (*actionFunc)(struct EnSsh*, PlayState*);
    /* 0x097C, sizeof 0x0210 (stride 0x58) */ ColliderCylinder colliderCylinders[6];
    /* 0x0B8C, sizeof 0x0020 */ EnSshColliderJntSph colliderJntSph;
    /* 0x0BAC, sizeof 0x0050 */ EnSshColliderElement colliderJntSphElements[1];
    /* 0x0BFC, sizeof 0x0004 */ char unk_BFC[0x04];
    /* 0x0C00, sizeof 0x0002 */ s16 unkTimer;
    /* 0x0C02 */ char unk_C02[0x06];
    /* 0x0C08, sizeof 0x0002 */ s16 sfxTimer;
    /* 0x0C0A */ char unk_C0A[0x02];
    /* 0x0C0C, sizeof 0x0002 */ s16 animTimer;
    /* 0x0C0E, sizeof 0x0002 */ s16 swayTimer;
    /* 0x0C10, sizeof 0x0004 */ s32 blureIdx;
    /* 0x0C14, sizeof 0x0004 */ f32 colliderScale;
    /* 0x0C18, sizeof 0x0004 */ f32 floorHeightOffset;
    /* 0x0C1C */ char unk_C1C[0x86];
    /* 0x0CA2, sizeof 0x0002 */ u16 stateFlags;
    /* 0x0CA4 */ char unk_CA4[0x08];
    /* 0x0CAC, sizeof 0x01CC (storage) */ char faceAnimations[0x1CC];
} EnSsh; // size = 0xE78

typedef char EnSshSizeCheck[(sizeof(EnSsh) == 0xE78) ? 1 : -1];
typedef char EnSshColliderJntSphSizeCheck[(sizeof(EnSshColliderJntSph) == 0x20) ? 1 : -1];
typedef char EnSshColliderElementSizeCheck[(sizeof(EnSshColliderElement) == 0x50) ? 1 : -1];
typedef char EnSshJntSphElementsPointerOffsetCheck[
    ((offsetof(EnSsh, colliderJntSph) + offsetof(EnSshColliderJntSph, elements)) == 0xBA8) ? 1 : -1];
typedef char EnSshJntSphElementsOffsetCheck[(offsetof(EnSsh, colliderJntSphElements) == 0xBAC) ? 1 : -1];
typedef char EnSshUnkTimerOffsetCheck[(offsetof(EnSsh, unkTimer) == 0xC00) ? 1 : -1];
typedef char EnSshColliderScaleOffsetCheck[(offsetof(EnSsh, colliderScale) == 0xC14) ? 1 : -1];

extern ActorInit En_Ssh_InitVars;

#endif
