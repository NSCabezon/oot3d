#ifndef _Z_EN_DNT_DEMO_H_
#define _Z_EN_DNT_DEMO_H_

#include "global.hpp"
#include "../En_Dnt_Nomal/z_en_dnt_nomal.hpp"

struct EnDntDemo;

typedef void (*EnDntDemoActionFunc)(struct EnDntDemo*, PlayState*);

// Leader (En_Dnt_Jiji) as seen from this actor: only the fields used here.
typedef struct EnDntDemoLeader {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x10];
    /* 0x01B4 */ u8 isSolid;
    /* 0x01B5 */ char unk_1B5;
    /* 0x01B6 */ s16 action;
    /* 0x01B8 */ s16 timer;
    /* 0x01BA */ char unk_1BA[0x8];
    /* 0x01C2 */ s16 unburrow;
    /* 0x01C4 */ char unk_1C4[0x2];
    /* 0x01C6 */ s16 stageSignal;
} EnDntDemoLeader;

typedef struct EnDntDemo {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnDntDemoActionFunc actionFunc;
    /* 0x01A8 */ s16 judgeTimer;
    /* 0x01AA */ s16 unkTimer1;
    /* 0x01AC */ s16 unkTimer2;
    /* 0x01AE */ s16 debugArrowTimer;
    /* 0x01B0 */ s16 leaderSignal;
    /* 0x01B2 */ s16 action;
    /* 0x01B4 */ s16 prize;
    /* 0x01B6 */ s16 subCamId;
    /* 0x01B8 */ Vec3f scrubPos[9];
    /* 0x0224 */ Vec3f leaderPos;
    /* 0x0230 */ EnDntNomal* scrubs[9];
    /* 0x0254 */ EnDntDemoLeader* leader;
} EnDntDemo; // size = 0x0258

typedef enum EnDntSignal {
    /* 0 */ DNT_SIGNAL_NONE,
    /* 1 */ DNT_SIGNAL_LOOK,
    /* 2 */ DNT_SIGNAL_CELEBRATE,
    /* 3 */ DNT_SIGNAL_DANCE,
    /* 4 */ DNT_SIGNAL_HIDE,
    /* 5 */ DNT_SIGNAL_RETURN,
    /* 6 */ DNT_SIGNAL_UNUSED
} EnDntSignal;

typedef enum EnDntLeaderAction {
    /* 0 */ DNT_LEADER_ACTION_NONE,
    /* 1 */ DNT_LEADER_ACTION_UP,
    /* 2 */ DNT_LEADER_ACTION_UNUSED,
    /* 3 */ DNT_LEADER_ACTION_ATTACK
} EnDntLeaderAction;

typedef enum EnDntLeaderSignal {
    /* 0 */ DNT_LEADER_SIGNAL_NONE,
    /* 1 */ DNT_LEADER_SIGNAL_UP,
    /* 2 */ DNT_LEADER_SIGNAL_BURROW,
    /* 3 */ DNT_LEADER_SIGNAL_RETURN
} EnDntLeaderSignal;

typedef enum EnDntPrize {
    /* 0 */ DNT_PRIZE_NONE,
    /* 1 */ DNT_PRIZE_NUTS,
    /* 2 */ DNT_PRIZE_STICK
} EnDntPrize;

typedef enum EnDntAction {
    /* 0 */ DNT_ACTION_NONE,
    /* 1 */ DNT_ACTION_DANCE,
    /* 2 */ DNT_ACTION_ATTACK,
    /* 3 */ DNT_ACTION_LOW_RUPEES,
    /* 4 */ DNT_ACTION_HIGH_RUPEES,
    /* 5 */ DNT_ACTION_PRIZE
} EnDntAction;

extern ActorInit En_Dnt_Demo_InitVars;

#endif
