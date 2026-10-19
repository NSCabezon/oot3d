#ifndef _Z_EN_GUEST_H_
#define _Z_EN_GUEST_H_

#include "global.hpp"

struct EnGuest;

typedef void (*EnGuestActionFunc)(struct EnGuest*, PlayState*);

typedef struct EnGuestInteractInfo {
    /* 0x00 */ char unk_00[0x14];
    /* 0x14 */ f32 yOffset;
    /* 0x18 */ Vec3f trackPos;
    /* 0x24 */ char unk_24[0x4];
} EnGuestInteractInfo; // size = 0x28

typedef struct EnGuest {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ Vec3s jointTable[16];
    /* 0x0288 */ char unk_288[0x2E0];
    /* 0x0568 */ Vec3s morphTable[16];
    /* 0x05C8 */ char unk_5C8[0x2E0];
    /* 0x08A8 */ EnGuestActionFunc actionFunc;
    /* 0x08AC */ ColliderCylinder collider;
    /* 0x0904 */ EnGuestInteractInfo interactInfo;
    /* 0x092C */ s16 unk_2C8;
    /* 0x092E */ s16 unk_2CA;
    /* 0x0930 */ s16 fidgetTableY[16];
    /* 0x0950 */ s16 fidgetTableZ[16];
    /* 0x0970 */ s8 osAnimeObjectSlot;
    /* 0x0971 */ u8 unk_30D;
    /* 0x0972 */ u8 unk_30E;
    /* 0x0973 */ char unk_973[0x1];
    /* 0x0974 */ SkeletonAnimationModel* skelAnimModel;
} EnGuest; // size = 0x978

extern ActorInit En_Guest_InitVars;

#endif
