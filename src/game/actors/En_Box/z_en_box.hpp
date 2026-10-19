#ifndef _Z_EN_BOX_H_
#define _Z_EN_BOX_H_

#include "global.hpp"

struct EnBox;

typedef void (*EnBoxActionFunc)(struct EnBox*, PlayState*);

typedef struct EnBox {
    /* 0x0000 */ DynaPolyActor dyna; // dyna.bgId at 0x1A4
    /* 0x01BC */ SkelAnime skelAnime;
    /* 0x0240 */ s32 appearTimer; // pre-delay (negative) and animation progress for appear/fall
    /* 0x0244 */ s32 subCamId;
    /* 0x0248 */ f32 unk_248;
    /* 0x024C */ EnBoxActionFunc actionFunc;
    /* 0x0250 */ Vec3s jointTable[26];
    /* 0x02EC */ Vec3s morphTable[26];
    /* 0x0388 */ ZARInfo* zar;
    /* 0x038C */ s16 unk_38C; // N64 unk_1F4: frame count set by player code
    /* 0x038E */ u8 movementFlags;
    /* 0x038F */ u8 alpha;
    /* 0x0390 */ u8 switchFlag;
    /* 0x0391 */ u8 type;
    /* 0x0392 */ u8 iceSmokeTimer;
    /* 0x0393 */ u8 ocarinaState;
    /* 0x0394 */ u8 unk_394[0x4];
} EnBox; // size = 0x0398

extern ActorInit En_Box_InitVars;

#endif
