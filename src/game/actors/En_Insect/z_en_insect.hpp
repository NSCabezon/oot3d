#ifndef _Z_EN_INSECT_H_
#define _Z_EN_INSECT_H_

#include "global.hpp"

struct EnInsect;

#define INSECT_FLAG_0 (1 << 0)
#define INSECT_FLAG_1 (1 << 1)
#define INSECT_FLAG_IS_SHORT_LIVED (1 << 2)
#define INSECT_FLAG_UNCATCHABLE (1 << 3)
#define INSECT_FLAG_FOUND_SOIL (1 << 4)
#define INSECT_FLAG_SOIL_CLOSE (1 << 5)
#define INSECT_FLAG_DROPPED_HAS_LANDED (1 << 6)
#define INSECT_FLAG_7 (1 << 7)
#define INSECT_FLAG_CRAWLING (1 << 8)

typedef void (*EnInsectActionFunc)(struct EnInsect*, PlayState*);

typedef struct EnInsect {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char collider[0x70]; // ColliderJntSph (3DS layout), kept opaque
    /* 0x0214 */ SkelAnime skelAnime;
    /* 0x0298 */ char unk_298[0x368];
    /* 0x0600 */ char unk_600[0x40];
    /* 0x0640 */ EnInsectActionFunc actionFunc;
    /* 0x0644 */ u16 insectFlags;
    /* 0x0646 */ s16 unk_316;
    /* 0x0648 */ s16 unk_318;
    /* 0x064A */ s16 actionTimer;
    /* 0x064C */ s16 lifeTimer;
    /* 0x064E */ s16 crawlSoundDelay;
    /* 0x0650 */ struct ObjMakekinsuta* soilActor;
    /* 0x0654 */ f32 unk_324;
    /* 0x0658 */ s16 unk_328;
    /* 0x065A */ u8 unk_32A;
} EnInsect; // size = 0x065C

extern ActorInit En_Insect_InitVars;

#define R_UPDATE_RATE SREG(30)
#define EN_INSECT_FRAMES(n) ((s16)(0.5f + (3.0f / R_UPDATE_RATE) * (n)))

#endif
