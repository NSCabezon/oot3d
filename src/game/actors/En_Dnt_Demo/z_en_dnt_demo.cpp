extern "C" {
#include "z_en_dnt_demo.hpp"

#define FLAGS 0x00000000
#define ENDNTNOMAL_STAGE 1

void EnDntDemo_Init(Actor* thisx, PlayState* play2);
void EnDntDemo_Destroy(Actor* thisx, PlayState* play);
void EnDntDemo_Update(Actor* thisx, PlayState* play);

void EnDntDemo_Judge(EnDntDemo* self, PlayState* play);
void EnDntDemo_Results(EnDntDemo* self, PlayState* play);
void EnDntDemo_Prize(EnDntDemo* self, PlayState* play);


s32 Player_GetMask(PlayState* play);
s32 OnePointCutscene_Init(PlayState* play, s32 csId, s32 arg2, Actor* actor, s32 camId);
void FUN_0036ef10(PosRot* pos, s32 sfxId, f32 volume);

ActorInit En_Dnt_Demo_InitVars = {
    ACTOR_EN_DNT_DEMO,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnDntDemo),
    (ActorFunc)EnDntDemo_Init,
    (ActorFunc)EnDntDemo_Destroy,
    (ActorFunc)EnDntDemo_Update,
    NULL,
};

static s16 EnDntDemo_sResultTable[8][7] = {
    { 0, 1, 0, 1, 2, 0, 1 }, { 1, 0, 1, 0, 1, 1, 2 }, { 2, 1, 1, 1, 0, 0, 0 }, { 1, 2, 1, 1, 1, 1, 0 },
    { 0, 0, 2, 0, 0, 0, 1 }, { 0, 0, 0, 2, 1, 2, 0 }, { 1, 1, 1, 1, 1, 1, 1 }, { 2, 2, 2, 2, 2, 2, 2 },
};

static s16 EnDntDemo_sResultValues[3][2] = {
    { 4, 3 },
    { 4, 2 },
    { 3, 1 },
};

static Vec3f EnDntDemo_sScrubPos[] = {
    { 3810.0f, -20.0f, 1010.0f }, { 3890.0f, -20.0f, 990.0f }, { 3730.0f, -20.0f, 950.0f },
    { 3840.0f, -20.0f, 930.0f },  { 3910.0f, -20.0f, 870.0f }, { 3780.0f, -20.0f, 860.0f },
    { 3710.0f, -20.0f, 840.0f },  { 3860.0f, -20.0f, 790.0f }, { 3750.0f, -20.0f, 750.0f },
};

// Vec3f with a user copy constructor: the temporary copy is built member-wise on the stack, then block-copied.
struct V3 {
    f32 x, y, z;
    V3() {}
    V3(const V3& o) : x(o.x), y(o.y), z(o.z) {}
};
void EnDntDemo_Init(Actor* thisx, PlayState* play2) {
    EnDntDemo* self = (EnDntDemo*)thisx;
    PlayState* play = play2;
    s32 i;

    for (i = 0; i < 9; i++) {
        *(V3*)&self->scrubPos[i] = V3(*(V3*)&EnDntDemo_sScrubPos[i]);
        self->scrubs[i] = (EnDntNomal*)Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, ACTOR_EN_DNT_NOMAL,
                                                          self->scrubPos[i].x, self->scrubPos[i].y, self->scrubPos[i].z,
                                                          0, 0, 0, i + ENDNTNOMAL_STAGE);
    }

    self->leaderPos.x = 4050.0f;
    self->leaderPos.y = -20.0f;
    self->leaderPos.z = 1000.0f;
    self->leader = (EnDntDemoLeader*)Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, ACTOR_EN_DNT_JIJI,
                                                        self->leaderPos.x, self->leaderPos.y, self->leaderPos.z, 0, 0,
                                                        0, 0);
    if (self->leader != NULL) {
        self->subCamId = 0;
    }
    self->actor.flags &= ~1;
    self->actionFunc = EnDntDemo_Judge;
}

void EnDntDemo_Destroy(Actor* thisx, PlayState* play) {
}

void EnDntDemo_Update(Actor* thisx, PlayState* play) {
    EnDntDemo* self = (EnDntDemo*)thisx;

    if (self->unkTimer2 != 0) {
        self->unkTimer2--;
    }
    if (self->unkTimer1 != 0) {
        self->unkTimer1--;
    }
    self->actionFunc(self, play);
}

void EnDntDemo_Judge(EnDntDemo* self, PlayState* play) {
    s16 delay;
    s16 reaction;
    s16 rand9;
    s16 maskIdx;
    s16 resultIdx;
    u8 ignore;
    s32 i;

    if (self->leaderSignal != DNT_SIGNAL_NONE) {
        for (i = 0; i < 9; i++) {
            self->scrubs[i]->stageSignal = self->leaderSignal;
            self->scrubs[i]->action = self->action;
            self->scrubs[i]->stagePrize = DNT_PRIZE_NONE;
        }
        if (self->leader->isSolid) {
            self->leader->stageSignal = DNT_LEADER_SIGNAL_BURROW;
        }
        self->leaderSignal = DNT_SIGNAL_NONE;
        self->actionFunc = EnDntDemo_Results;
    } else if ((self->actor.xzDistToPlayer > 30.0f) || (Player_GetMask(play) == 0)) {
        self->debugArrowTimer++;
        if (self->subCamId != 0) {
            self->subCamId = 0;
        }
        if (self->judgeTimer != 0) {
            for (i = 0; i < 9; i++) {
                self->scrubs[i]->stageSignal = DNT_SIGNAL_HIDE;
            }
            self->judgeTimer = 0;
        }
    } else {
        if ((Player_GetMask(play) != 0) && (self->subCamId == 0)) {
            self->subCamId = OnePointCutscene_Init(play, 2220, -99, &self->scrubs[3]->actor, 0);
        }
        self->debugArrowTimer = 0;
        if (self->judgeTimer == 40) {
            for (i = 0; i < 9; i++) {
                self->scrubs[i]->stageSignal = DNT_SIGNAL_LOOK;
            }
        }
        if (self->judgeTimer > 40) {
            FUN_0036ef10(&self->actor.world, 0x010001B4, 2.0f);
        }
        if (self->judgeTimer < 120) {
            self->judgeTimer++;
        } else {
            ignore = false;
            reaction = DNT_SIGNAL_NONE;
            delay = 0;
            switch (Player_GetMask(play)) {
                case 2:
                    if (!(gSaveContext.itemGetInf[1] & 0x4000)) {
                        reaction = DNT_SIGNAL_CELEBRATE;
                        self->prize = DNT_PRIZE_STICK;
                        Audio_QueueSeqCmd(0, 0x010005AB, 0);
                        break;
                    }
                    // fallthrough
                case 8:
                    if (!(gSaveContext.itemGetInf[1] & 0x8000) && (Player_GetMask(play) != 2)) {
                        FUN_00372244(&play->unk_5FCC, 0x1E, 0x0100048C);
                        self->prize = DNT_PRIZE_NUTS;
                        self->leader->stageSignal = DNT_LEADER_SIGNAL_UP;
                        reaction = DNT_SIGNAL_LOOK;
                        if (self->subCamId != 0) {
                            self->subCamId = 0;
                            reaction = DNT_SIGNAL_LOOK;
                            OnePointCutscene_Init(play, 2340, -99, &self->leader->actor, 0);
                        }
                        break;
                    }
                    // fallthrough
                case 1:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                    rand9 = Rand_ZeroFloat(8.99f);
                    maskIdx = (s16)Player_GetMask(play) - 1;
                    if (rand9 == 8) {
                        ignore = true;
                        delay = 8;
                        reaction = DNT_SIGNAL_HIDE;
                    } else {
                        if (maskIdx > 7) {
                            maskIdx = Rand_ZeroFloat(7.99f);
                        }

                        resultIdx = EnDntDemo_sResultTable[rand9][maskIdx];
                        reaction = EnDntDemo_sResultValues[resultIdx][0];
                        self->action = EnDntDemo_sResultValues[resultIdx][1];
                        switch (self->action) {
                            case DNT_ACTION_LOW_RUPEES:
                                Audio_QueueSeqCmd(0, 0x0100059B, 0);
                                break;
                            case DNT_ACTION_ATTACK:
                                if (self->subCamId != 0) {
                                    self->subCamId = 0;
                                    OnePointCutscene_Init(play, 2350, -99, &self->scrubs[3]->actor, 0);
                                }
                                Audio_QueueSeqCmd(0, 0x01000588, 0);
                                break;
                            case DNT_ACTION_DANCE:
                                Audio_QueueSeqCmd(0, 0x010005C2, 0);
                                break;
                        }
                        break;
                    }
            }
            if (reaction != DNT_SIGNAL_NONE) {
                for (i = 0; i < 9; i++) {
                    if (delay != 0) {
                        self->scrubs[i]->timer3 = delay * i;
                    }
                    self->scrubs[i]->action = self->action;
                    self->scrubs[i]->stageSignal = reaction;
                    self->scrubs[i]->ignore = ignore;
                    if (self->prize != DNT_PRIZE_NONE) {
                        self->scrubs[i]->timer1 = 300;
                        self->scrubs[i]->stagePrize = self->prize;
                        self->scrubs[i]->targetPos = self->leader->actor.world.pos;
                        if (self->prize == DNT_PRIZE_NUTS) {
                            self->leader->stageSignal = DNT_LEADER_SIGNAL_UP;
                        }
                        if (self->prize == DNT_PRIZE_STICK) {
                            self->leader->timer = 300;
                        }
                    }
                }
                self->actionFunc = EnDntDemo_Results;
            }
        }
    }
}

void EnDntDemo_Prize(EnDntDemo* self, PlayState* play) {
    s32 i;

    if (self->leaderSignal != DNT_SIGNAL_NONE) {
        for (i = 0; i < 9; i++) {
            self->scrubs[i]->action = self->action;
            self->scrubs[i]->stageSignal = self->leaderSignal;
            self->scrubs[i]->stagePrize = DNT_PRIZE_NONE;
        }
        self->leaderSignal = self->action = DNT_SIGNAL_NONE;
    }
}

void EnDntDemo_Results(EnDntDemo* self, PlayState* play) {
    s32 i;
    s16 offsetAngle;
    Vec3f leaderPos;
    f32 offsetDist;

    if (self->leaderSignal != DNT_SIGNAL_NONE) {
        for (i = 0; i < 9; i++) {
            self->scrubs[i]->action = self->action;
            self->scrubs[i]->stageSignal = self->leaderSignal;
            self->scrubs[i]->stagePrize = DNT_PRIZE_NONE;
        }
        if (self->leader->action == DNT_LEADER_ACTION_UP) {
            self->leader->stageSignal = DNT_LEADER_SIGNAL_BURROW;
        } else if (self->leader->unburrow) {
            self->leader->stageSignal = DNT_LEADER_SIGNAL_RETURN;
        } else {
            self->leader->action = DNT_LEADER_ACTION_ATTACK;
        }
        self->leader->timer = 0;
        self->leaderSignal = self->action = DNT_SIGNAL_NONE;
        self->actionFunc = EnDntDemo_Prize;
    } else if (self->prize == DNT_PRIZE_STICK) {
        for (i = 0; i < 9; i++) {
            offsetAngle = -self->leader->actor.shape.rot.y;
            leaderPos = self->leader->actor.world.pos;

            if (!(i & 1)) {
                offsetAngle -= 0x59D8;
            }
            offsetDist = ((i + 1) * 20.0f) + 20.0f;
            self->scrubs[i]->timer2 = 10;
            self->scrubs[i]->targetPos.x = leaderPos.x + Math_SinS(offsetAngle) * offsetDist;
            self->scrubs[i]->targetPos.y = leaderPos.y;
            self->scrubs[i]->targetPos.z = leaderPos.z + Math_CosS(offsetAngle) * offsetDist;
        }
    }
}

}
