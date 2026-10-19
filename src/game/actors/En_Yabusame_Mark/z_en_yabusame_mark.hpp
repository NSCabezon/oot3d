#ifndef _Z_EN_YABUSAME_MARK_H_
#define _Z_EN_YABUSAME_MARK_H_

#include "global.hpp"

struct EnYabusameMark;

typedef void (*EnYabusameMarkActionFunc)(struct EnYabusameMark*, PlayState*);

// 3DS quad collider: Collider base (0x18), ColliderInfo elem (0x28), quad dims (0x40)
typedef struct EnYabusameMarkCollider {
    /* 0x00 */ Collider base;
    /* 0x18 */ ColliderInfo elem;
    /* 0x40 */ u8 unk_40[0x40];
} EnYabusameMarkCollider; // size = 0x80

typedef struct EnYabusameMark {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnYabusameMarkActionFunc actionFunc;
    /* 0x01A8 */ s16 typeIndex;
    /* 0x01AA */ s16 subTypeIndex;
    /* 0x01AC */ Vec3f worldPos;
    /* 0x01B8 */ Vec3f vertexA;
    /* 0x01C4 */ Vec3f vertexB;
    /* 0x01D0 */ Vec3f vertexC;
    /* 0x01DC */ Vec3f vertexD;
    /* 0x01E8 */ EnYabusameMarkCollider collider;
} EnYabusameMark; // size = 0x268

extern ActorInit En_Yabusame_Mark_InitVars;

#endif
