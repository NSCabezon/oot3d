#ifndef _Z_EN_HORSE_H_
#define _Z_EN_HORSE_H_

#include "global.hpp"

#define ENHORSE_BOOST                      (1 << 0)   /*         0x1 */
#define ENHORSE_BOOST_DECEL                (1 << 1)   /*         0x2 */
#define ENHORSE_JUMPING                    (1 << 2)   /*         0x4 */
#define ENHORSE_CALC_RIDER_POS             (1 << 3)   /*         0x8 */
#define ENHORSE_FORCE_REVERSING            (1 << 4)   /*        0x10 */
#define ENHORSE_FORCE_WALKING              (1 << 5)   /*        0x20 */
#define ENHORSE_FLAG_6                     (1 << 6)   /*        0x40 */
#define ENHORSE_FLAG_7                     (1 << 7)   /*        0x80 */
#define ENHORSE_FLAG_8                     (1 << 8)   /*       0x100 */
#define ENHORSE_FLAG_9                     (1 << 9)   /*       0x200 */
#define ENHORSE_STOPPING_NEIGH_SOUND       (1 << 10)  /*       0x400 */
#define ENHORSE_LAND2_SOUND                (1 << 11)  /*       0x800 */
#define ENHORSE_SANDDUST_SOUND             (1 << 12)  /*      0x1000 */
#define ENHORSE_INACTIVE                   (1 << 13)  /*      0x2000 */
#define ENHORSE_OBSTACLE                   (1 << 14)  /*      0x4000 */
#define ENHORSE_TURNING_TO_PLAYER          (1 << 15)  /*      0x8000 */
#define ENHORSE_UNRIDEABLE                 (1 << 16)  /*    0x1 0000 */
#define ENHORSE_CANT_JUMP                  (1 << 17)  /*    0x2 0000 */
#define ENHORSE_FLAG_18                    (1 << 18)  /*    0x4 0000 */
#define ENHORSE_FLAG_19                    (1 << 19)  /*    0x8 0000 */
#define ENHORSE_FLAG_20                    (1 << 20)  /*   0x10 0000 */
#define ENHORSE_FLAG_21                    (1 << 21)  /*   0x20 0000 */
#define ENHORSE_FIRST_BOOST_REGEN          (1 << 22)  /*   0x40 0000 */
#define ENHORSE_INGO_WON                   (1 << 23)  /*   0x80 0000 */
#define ENHORSE_FLAG_24                    (1 << 24)  /*  0x100 0000 */
#define ENHORSE_FLAG_25                    (1 << 25)  /*  0x200 0000 */
#define ENHORSE_FLAG_26                    (1 << 26)  /*  0x400 0000 */
#define ENHORSE_DRAW                       (1 << 27)  /*  0x800 0000 */
#define ENHORSE_FLAG_28                    (1 << 28)  /* 0x1000 0000 */
#define ENHORSE_FLAG_29                    (1 << 29)  /* 0x2000 0000 */
#define ENHORSE_FLAG_30                    (1 << 30)  /* 0x4000 0000 */
#define ENHORSE_FLAG_31                    (1 << 31)  /* 0x8000 0000 */

typedef enum EnHorseAction {
    /*  0 */ ENHORSE_ACT_FROZEN,
    /*  1 */ ENHORSE_ACT_INACTIVE,
    /*  2 */ ENHORSE_ACT_IDLE,
    /*  3 */ ENHORSE_ACT_FOLLOW_PLAYER,
    /*  4 */ ENHORSE_ACT_INGO_RACE,
    /*  5 */ ENHORSE_ACT_MOUNTED_IDLE,
    /*  6 */ ENHORSE_ACT_MOUNTED_IDLE_WHINNEYING,
    /*  7 */ ENHORSE_ACT_MOUNTED_TURN,
    /*  8 */ ENHORSE_ACT_MOUNTED_WALK,
    /*  9 */ ENHORSE_ACT_MOUNTED_TROT,
    /* 10 */ ENHORSE_ACT_MOUNTED_GALLOP,
    /* 11 */ ENHORSE_ACT_MOUNTED_REARING,
    /* 12 */ ENHORSE_ACT_STOPPING,
    /* 13 */ ENHORSE_ACT_REVERSE,
    /* 14 */ ENHORSE_ACT_LOW_JUMP,
    /* 15 */ ENHORSE_ACT_HIGH_JUMP,
    /* 16 */ ENHORSE_ACT_BRIDGE_JUMP,
    /* 17 */ ENHORSE_ACT_CS_UPDATE,
    /* 18 */ ENHORSE_ACT_HBA,
    /* 19 */ ENHORSE_ACT_FLEE_PLAYER
} EnHorseAction;

typedef enum EnHorseAnimationIndex {
    /* 0 */ ENHORSE_ANIM_IDLE,
    /* 1 */ ENHORSE_ANIM_WHINNY,
    /* 2 */ ENHORSE_ANIM_STOPPING,
    /* 3 */ ENHORSE_ANIM_REARING,
    /* 4 */ ENHORSE_ANIM_WALK,
    /* 5 */ ENHORSE_ANIM_TROT,
    /* 6 */ ENHORSE_ANIM_6,
    /* 7 */ ENHORSE_ANIM_GALLOP, // 3DS index (N64 6)
    /* 8 */ ENHORSE_ANIM_8
} EnHorseAnimationIndex;

typedef enum HorseType {
    /* 0 */ HORSE_EPONA,
    /* 1 */ HORSE_HNI
} HorseType;

struct EnHorse;

typedef void (*EnHorsePostdrawFunc)(struct EnHorse*, struct PlayState*);

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ void* elements;
} EnHorseColliderJntSph; // size = 0x20

typedef struct EnHorse {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ u8 action;
    /* 0x01A5 */ char unk_1A5[0x3];
    /* 0x01A8 */ s32 noInputTimer;
    /* 0x01AC */ s32 noInputTimerMax;
    /* 0x01B0 */ u8 type;
    /* 0x01B1 */ char unk_1B1[0x13];
    /* 0x01C4 */ SkelAnime skin;
    /* 0x0248 */ char unk_248[0xC00 - 0x248];
    /* 0x0C00 */ char unk_C00[0x74];
    /* 0x0C74 */ char unk_C74[0xE40 - 0xC74]; // FaceAnimations (accessed through a cast)
    /* 0x0E40 */ u32 unk_E40;
    /* 0x0E44 */ char unk_E44[0xE54 - 0xE44];
    /* 0x0E54 */ s32 stateFlags;
    /* 0x0E58 */ Vec3f lastPos;
    /* 0x0E64 */ s16 lastYaw;
    /* 0x0E66 */ char unk_E66[2];
    /* 0x0E68 */ s32 curRaceWaypoint;
    /* 0x0E6C */ s32 boostSpeed;
    /* 0x0E70 */ s32 playerControlled;
    /* 0x0E74 */ u8 animationIdx;
    /* 0x0E75 */ char unk_E75[0xE78 - 0xE75];
    /* 0x0E78 */ f32 curFrame;
    /* 0x0E7C */ s32 unk_E7C;
    /* 0x0E80 */ Vec3f unk_E80;
    /* 0x0E8C */ Vec3f unk_E8C;
    /* 0x0E98 */ s32 unk_E98;
    /* 0x0E9C */ u8 numBoosts;
    /* 0x0E9D */ char unk_E9D[3];
    /* 0x0EA0 */ s32 boostRegenTime;
    /* 0x0EA4 */ s32 boostTimer;
    /* 0x0EA8 */ EnHorsePostdrawFunc postDrawFunc;
    /* 0x0EAC */ char unk_EAC[0xEB4 - 0xEAC];
    /* 0x0EB4 */ s16 followTimer;
    /* 0x0EB6 */ u8 prevAction;
    /* 0x0EB7 */ char unk_EB7;
    /* 0x0EB8 */ Vec3f riderPos;
    /* 0x0EC4 */ u8 unk_EC4;
    /* 0x0EC5 */ char unk_EC5[0xEC8 - 0xEC5];
    /* 0x0EC8 */ Vec2f curStick;
    /* 0x0ED0 */ Vec2f lastStick;
    /* 0x0ED8 */ f32 jumpStartY;
    /* 0x0EDC */ ColliderCylinder colliderCylinder1;
    /* 0x0F34 */ ColliderCylinder colliderCylinder2;
    /* 0x0F8C */ EnHorseColliderJntSph colliderJntSph;
    /* 0x0FAC */ char unk_FAC[0xFFC - 0xFAC];
    /* 0x0FFC */ u8 playerDir;
    /* 0x0FFD */ char unk_FFD[3];
    /* 0x1000 */ s16 angleToPlayer;
    /* 0x1002 */ s16 followPlayerTurnSpeed;
    /* 0x1004 */ char unk_1004[0x100A - 0x1004];
    /* 0x100A */ s16 waitTimer;
    /* 0x100C */ char unk_100C[2];
    /* 0x100E */ u8 cutsceneAction;
    /* 0x100F */ char unk_100F[0x1014 - 0x100F];
    /* 0x1014 */ s32 inRace;
    /* 0x1018 */ Actor* rider;
    /* 0x101C */ char unk_101C[4];
    /* 0x1020 */ u16 ingoRaceFlags;
    /* 0x1022 */ char unk_1022[2];
    /* 0x1024 */ f32 ingoHorseMaxSpeed;
    /* 0x1028 */ char unk_1028[4];
    /* 0x102C */ s32 hbaStarted;
    /* 0x1030 */ s32 hbaFlags;
    /* 0x1034 */ s32 hbaTimer;
    /* 0x1038 */ u8 bridgeJumpIdx;
    /* 0x1039 */ char unk_1039[3];
    /* 0x103C */ Vec3f bridgeJumpStart;
    /* 0x1048 */ s32 bridgeJumpTimer;
    /* 0x104C */ f32 bridgeJumpYVel;
    /* 0x1050 */ s16 bridgeJumpRelAngle;
    /* 0x1052 */ char unk_1052[0x1088 - 0x1052];
    /* 0x1088 */ Vec3f unk_1088;
    /* 0x1094 */ u8 unk_1094;
    /* 0x1095 */ char unk_1095[0x3];
} EnHorse; // size = 0x1098

extern ActorInit En_Horse_InitVars;

#endif
