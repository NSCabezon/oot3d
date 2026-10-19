#ifndef _Z_EN_SSH_H_
#define _Z_EN_SSH_H_

#include "global.hpp"

struct EnSsh;

typedef struct EnSsh {
    /* 0x0000, sizeof 0x01A4 */ Actor actor;
    /* 0x01A4, sizeof 0x0084 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x750];
    /* 0x0978, sizeof 0x0004 */ void (*actionFunc)(struct EnSsh*, PlayState*);
    /* 0x097C, sizeof 0x0210 (stride 0x58) */ ColliderCylinder colliderCylinders[6];
    /* 0x0B8C, sizeof 0x0020 */ char colliderJntSph[0x20];
    /* 0x0BAC */ char unk_BAC[0x54];
    /* 0x0C00, sizeof 0x0002 */ s16 unkTimer;
    /* 0x0C02 */ char unk_C02[0x06];
    /* 0x0C08, sizeof 0x0002 */ s16 sfxTimer;
    /* 0x0C0A */ char unk_C0A[0x02];
    /* 0x0C0C, sizeof 0x0002 */ s16 animTimer;
    /* 0x0C0E, sizeof 0x0002 */ s16 swayTimer;
    /* 0x0C10, sizeof 0x0004 */ s32 blureIdx;
    /* 0x0C14 */ char unk_C14[0x04];
    /* 0x0C18, sizeof 0x0004 */ f32 floorHeightOffset;
    /* 0x0C1C */ char unk_C1C[0x86];
    /* 0x0CA2, sizeof 0x0002 */ u16 stateFlags;
    /* 0x0CA4 */ char unk_CA4[0x08];
    /* 0x0CAC, sizeof 0x01CC (storage) */ char faceAnimations[0x1CC];
} EnSsh; // size = 0xE78

typedef char EnSshSizeCheck[(sizeof(EnSsh) == 0xE78) ? 1 : -1];

extern ActorInit En_Ssh_InitVars;

#endif
