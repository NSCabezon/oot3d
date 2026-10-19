extern "C" {
#include "z_en_skj.hpp"

#define FLAGS 0x02000015

void EnSkj_Init(Actor* thisx, PlayState* play);
void EnSkj_Destroy(Actor* actor, PlayState* play);
void EnSkj_Update(Actor* thisx, PlayState* play);
void EnSkj_Draw(Actor* actor, PlayState* play);

GLOBAL_ASM("data/z_En_Skj.data.s")

GLOBAL_ASM("../src/game/actors/En_Skj/data_syms.s")

extern u8 EnSkj_sStumpVars[];
extern s32 EnSkj_sTalkOfferAccepted;
extern EnSkjUnkStruct EnSkj_sSmallStumpSkullKid;
extern EnSkjActionFunc EnSkj_sActionFuncs[];
extern u8 EnSkj_sSoundState[];
extern u8 EnSkj_sSoundStateFlag[];

void EnSkj_WaitForNextRound(EnSkj* self, PlayState* play);
void EnSkj_WaitToGiveReward(EnSkj* self, PlayState* play);
u32 Actor_ProcessTalkRequest(Actor* actor, PlayState* play);
s32 FUN_0034bdb8(s32 arg0);
s32 FUN_00366684(s32 arg0);
void FUN_0033c7a4(EnSkj* self);
void FUN_003729b8(EnSkj* self, s32 action);
void FUN_003945c4(EnSkj* self, PlayState* play);
void FUN_001ff780(EnSkj* self, PlayState* play);
f32* EnSkj_CurFramePtr(SkelAnime* skelAnime);
void Matrix_RotateY(MTX34* mtx, s32 mode, f32 angle);
extern u8 EnSkj_sSndBase[];
extern u8 EnSkj_sSndTbl[];
extern u8 EnSkj_sSndBufA[];
extern u8 EnSkj_sSndBufB[];
extern u8 EnSkj_sSndBufC[];
extern u8 EnSkj_sSndBufD[];
void FUN_0032b184(void* p, u32 n);
void __rt_memclr_w(void* p, u32 n);
void FUN_00285fa4(void);
void FUN_002f6944(void);
void FUN_004c0878(void);
void FUN_003525d4(void);
void FUN_002f7af4(s32 arg0, f32 arg1, f32 arg2);
f32 FUN_0035dd04(EnSkj* self);
void FUN_003686a8(EnSkj* self, s32 index);
void FUN_0039def0(EnSkj* self, PlayState* play);
s32 FUN_00369f3c(PlayState* play);
void EnSkj_SetupWaitForOcarina(EnSkj* self, PlayState* play);
void EnSkj_StartOcarinaMinigame(EnSkj* self, PlayState* play);
void EnSkj_WaitForOcarina(EnSkj* self, PlayState* play);
void FUN_0032e1f0(EnSkj* self);
void FUN_0032c540(s32 value);
void FUN_0032c550(s32 value);
void FUN_0032c560(s32 value);
void FUN_0032c570(s32 arg0);
void EnSkj_WaitForOfferResponse(EnSkj* self, PlayState* play);
void EnSkj_CleanupOcarinaGame(EnSkj* self, PlayState* play);
extern EnSkjUnkStruct EnSkj_sOcarinaSkullKids[2];
extern s32 EnSkj_sOcarinaRewards[];
extern u8 EnSkj_sSeqVars[];

ActorInit En_Skj_InitVars = {
    ACTOR_EN_SKJ,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_SKJ,
    sizeof(EnSkj),
    (ActorFunc)EnSkj_Init,
    (ActorFunc)EnSkj_Destroy,
    (ActorFunc)EnSkj_Update,
    (ActorFunc)EnSkj_Draw,
};


// JOIN: the ROM body falls through into the (separate tsv entry) FUN_0018f08c, so the full function is 56 B.
// FUN_0018f08c is removed from data/oot3d.ld + keep_symbols.txt (orchestrator applies the same on main).
void EnSkj_Destroy(Actor* thisx, PlayState* play) {
    EnSkj* self = (EnSkj*)thisx;
    Collider_DestroyCylinder(play, (ColliderCylinder*)self->unk_9B0);
    SkelAnime_Free2((SkelAnime*)self->unk_1A4);
    FUN_00350f34(&self->actor, &self->model0, 0);
}

void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 colorIntensityMax, s32 bufFlag, s32 duration);
void Actor_ApplyDamage(Actor* actor);
void EnSkj_SpawnBlood(PlayState* play, Vec3f* pos);
void FUN_0033cefc(PlayState* play, s32 a, s32 b, Vec3f* pos);

// Draft (far, ~200 words: ROM inlines EnSkj_SpawnBlood = 32-element effect array built with __aeabi_vec_ctor_nocookie_nodtor, 0x580 frame; not modelled). GLOBAL_ASM kept.
#if 0
static inline void EnSkj_SetupDie(EnSkj* self) {
    FUN_003686a8(self, 3);
    FUN_003729b8(self, 3);
}

void EnSkj_Update(Actor* thisx, PlayState* play) {
    Vec3f dropPos;
    EnSkj* self = (EnSkj*)thisx;

    EnSkj_sTalkOfferAccepted = Actor_ProcessTalkRequest(&self->actor, play);
    self->timer++;
    if (self->unk_A0E != 0) {
        self->unk_A0E--;
    }
    if (self->unk_A10 != 0) {
        self->unk_A10--;
    }
    if (self->unk_A12 != 0) {
        self->unk_A12--;
    }
    if (self->unk_A12 == 0) {
        self->unk_A19[0] = 3;
    }

    if ((self->unk_A16 != 0) && (self->unk_A20 == 0)) {
        if (self->action == 9) {
            dropPos.x = self->actor.world.pos.x;
            dropPos.y = self->actor.world.pos.y;
            dropPos.z = self->actor.world.pos.z;
            Item_DropCollectible(play, &dropPos, 0x13);
        }
        Actor_Kill(&self->actor);
        return;
    }

    Actor_SetFocus(&self->actor, 30.0f);
    Actor_SetScale(&self->actor, 0.01f);
    self->actionFunc(self, play);
    self->actor.textId = self->textId;
    if ((self->unk_A17 != 0) && (EnSkj_sTalkOfferAccepted == 0) && (*((u8*)self + 0x9C1) & 2)) {
        *((u8*)self + 0x9C1) &= ~2;
        switch (*((u8*)self + 0xB9)) {
            case 0xF: {
                Vec3f effectPos;
                s16 yawDiff;

                effectPos.x = (f32)*(s16*)((u8*)self + 0x9D6);
                effectPos.y = (f32)*(s16*)((u8*)self + 0x9D8);
                effectPos.z = (f32)*(s16*)((u8*)self + 0x9DA);
                EnSkj_SpawnBlood(play, &effectPos);
                FUN_0033cefc(play, 0, 0, &effectPos);
                yawDiff = self->actor.yawTowardsPlayer - self->actor.world.rot.y;
                if ((self->action == 2) || (self->action == 6)) {
                    if ((yawDiff > 0x6000) || (yawDiff < -0x6000)) {
                        Actor_SetColorFilter(&self->actor, 0x400000, 255, 0, 8);
                        EnSkj_SetupDie(self);
                        break;
                    }
                }
                Actor_ApplyDamage(&self->actor);
                Audio_PlayActorSound2(&self->actor, 0x0100046F);
                Actor_SetColorFilter(&self->actor, 0x400000, 255, 0, 8);
                if (self->actor.colChkInfo.health != 0) {
                    if (self->unk_A19[0] != 0) {
                        self->unk_A19[0]--;
                    }
                    if (self->unk_A12 == 0) {
                        self->unk_A12 = 60;
                    }
                    FUN_003686a8(self, 4);
                    FUN_003729b8(self, 4);
                    break;
                }
                EnSkj_SetupDie(self);
                break;
            }
            case 0:
                self->unk_A16 = 1;
                self->actor.velocity.y = 8.0f;
                self->actor.speedXZ = -8.0f;
                FUN_003686a8(self, 0);
                FUN_003729b8(self, 0);
                break;
            default:
                break;
        }
    }
    Collider_UpdateCylinder(&self->actor, (ColliderCylinder*)self->unk_9B0);

    if ((self->unk_A17 != 0) && (EnSkj_sTalkOfferAccepted == 0)) {
        CollisionCheck_SetAT(play, &play->colChkCtx, (Collider*)self->unk_9B0);
        if (*(u16*)((u8*)self + 0x11A) == 0) {
            CollisionCheck_SetAC(play, &play->colChkCtx, (Collider*)self->unk_9B0);
        }
    }

    CollisionCheck_SetOC(play, &play->colChkCtx, (Collider*)self->unk_9B0);
    FUN_003731e0((SkelAnime*)self->unk_1A4);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(play, &self->actor, 20.0f, 20.0f, 20.0f, 7);
    *((u8*)self + 0xD0) = self->unk_A20;
}
#endif
GLOBAL_ASM("asm/EnSkj_Update.s")

void FUN_0037266c(void* model, s32 idx);
void FUN_0036932c(void* model, s32 idx);
typedef struct EnSkjColorF {
    f32 r, g, b, a;
} EnSkjColorF;
void FUN_00357388(Actor* actor, EnSkjColorF* color, s32 arg2, s32 arg3);
extern EnSkjColorF EnSkj_sColorWhite;

void EnSkj_Draw(Actor* thisx, PlayState* play) {
    EnSkj* self = (EnSkj*)thisx;
    EnSkjColorF color;
    f32 alpha;

    if ((*(u16*)((u8*)&gSaveContext + 0xF0E) & 0x200) && *((u8*)self + 0xA44) != 0) {
        FUN_0037266c(*(void**)((u8*)self + 0x1CC), 3);
    } else {
        FUN_0036932c(*(void**)((u8*)self + 0x1CC), 3);
    }
    color = EnSkj_sColorWhite;
    alpha = (f32)(u32)self->unk_A20 * (1.0f / 255.0f);
    color.a = alpha;
    if (alpha > 0.0f) {
        FUN_00357388(thisx, &color, 4, 2);
        SkelAnime_DrawOpa((SkelAnime*)self->unk_1A4, &thisx->modelMtx, NULL, NULL, thisx, 0);
    }
}

void EnSkj_CleanupOcarinaGame(EnSkj* self, PlayState* play) {
    if (EnSkj_sOcarinaSkullKids[0].skullkid != NULL) {
        EnSkj_sOcarinaSkullKids[0].unk_0 = 2;
    }

    if (EnSkj_sOcarinaSkullKids[1].skullkid != NULL) {
        EnSkj_sOcarinaSkullKids[1].unk_0 = 2;
    }

    if ((EnSkj_sOcarinaSkullKids[0].unk_0 == 2) && (EnSkj_sOcarinaSkullKids[1].unk_0 == 2)) {
        FUN_00285fa4();
        Actor_Kill(&self->actor);
    }
}

void EnSkj_FailedMiniGame(EnSkj* self, PlayState* play) {
    if (EnSkj_sTalkOfferAccepted != 0) {
        self->actionFunc = EnSkj_WaitForNextRound;
    } else {
        FUN_0036bb28(&self->actor, play, 26.0f);
    }
}

void EnSkj_OcarinaMinigameShortStumpUpdate(EnSkj* self, PlayState* play) {
    EnSkj_sTalkOfferAccepted = Actor_ProcessTalkRequest(&self->actor, play);
    self->timer++;

    self->actor.focus.pos.x = 1230.0f;
    self->actor.focus.pos.y = -90.0f;
    self->actor.focus.pos.z = 450.0f;

    self->actionFunc(self, play);

    u16* p = &self->actor.textId;
    *(u8*)((u8*)self + 0xD0) = (u8)self->unk_A20;
    *p = self->textId;
    self->actor.xzDistToPlayer = 50.0f;
}

void EnSkj_SariasSongShortStumpUpdate(Actor* thisx, PlayState* play) {
    EnSkj* self = (EnSkj*)thisx;

    EnSkj_sTalkOfferAccepted = Actor_ProcessTalkRequest(&self->actor, play);
    *(u8*)((u8*)self + 0xD0) = (u8)self->unk_A20;
}

void EnSkj_SetupWaitForOcarina(EnSkj* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 dx = player->actor.world.pos.x - self->actor.world.pos.x;
    f32 dz = player->actor.world.pos.z - self->actor.world.pos.z;
    f32 selfY = self->actor.world.pos.y;
    f32 dy;
    f32 dist2 = dx * dx + dz * dz;

    dy = player->actor.world.pos.y - selfY;
    if (dist2 > 676.0f) {
        return;
    }
    if (dy >= 0.0f) {
        EnSkj_sOcarinaSkullKids[0].skullkid->unk_A1B = 1;
        EnSkj_sOcarinaSkullKids[1].skullkid->unk_A1B = 1;
        {
            EnSkjActionFunc next;

            if (!(*(u32*)((u8*)player + 0x1714) & 0x1000000)) {
                next = EnSkj_WaitForOcarina;
            } else {
                *(u32*)((u8*)player + 0x1714) |= 0x2000000;
                FUN_0032e1f0((EnSkj*)4);
                Math_SmoothStepToS(&player->actor.shape.rot.y, self->actor.world.rot.y, 5, 0x7D0, 0);
                {
                    u16 rot = player->actor.shape.rot.y;

                    player->actor.world.rot.y = rot;
                    *(u16*)((u8*)player + 0x2220) = rot;
                }
                *(Actor**)((u8*)player + 0x1740) = &self->actor;
                FUN_00367c7c(play, 0x10BE, &self->actor);
                next = EnSkj_StartOcarinaMinigame;
            }
            self->actionFunc = next;
        }
    }
}


void EnSkj_WaitForNextRound(EnSkj* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 6 && Message_ShouldAdvance(play)) {
        Message_ContinueTextbox(play, 0x102E);
        self->actionFunc = EnSkj_WaitForOfferResponse;
    }
}

void EnSkj_WaitForOcarina(EnSkj* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (*(u32*)((u8*)player + 0x1714) & 0x1000000) {
        *(u32*)((u8*)player + 0x1714) |= 0x2000000;
        FUN_0032e1f0((EnSkj*)4);
        Math_SmoothStepToS(&player->actor.shape.rot.y, self->actor.world.rot.y, 5, 0x7D0, 0);
        {
            u16 rot = player->actor.shape.rot.y;

            player->actor.world.rot.y = rot;
            *(u16*)((u8*)player + 0x2220) = rot;
        }
        FUN_0032c570(1);
        FUN_0032c560(0);
        FUN_0032c550(1);
        FUN_0032c540(0);
        *(Actor**)((u8*)player + 0x1740) = &self->actor;
        FUN_00367c7c(play, 0x10BE, &self->actor);
        self->actionFunc = EnSkj_StartOcarinaMinigame;
    } else {
        f32 dx = player->actor.world.pos.x - self->actor.world.pos.x;
        f32 dz = player->actor.world.pos.z - self->actor.world.pos.z;
        f32 selfY = self->actor.world.pos.y;
        f32 dy;
        f32 dist2 = dx * dx + dz * dz;

        dy = player->actor.world.pos.y - selfY;
        if (dist2 > 676.0f) {
            return;
        }
        if (dy >= 0.0f) {
            *(u32*)((u8*)player + 0x1714) |= 0x800000;
        }
    }
}


void EnSkj_WaitForOfferResponse(EnSkj* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 4 && Message_ShouldAdvance(play)) {
        switch (FUN_00369f3c(play)) {
            case 0: {
                u8* p = (u8*)GET_PLAYER(play);

                p[0x172A] |= 0x20;
                self->actionFunc = EnSkj_SetupWaitForOcarina;
                break;
            }
            case 1:
                self->actionFunc = EnSkj_CleanupOcarinaGame;
                break;
        }
    }
}


void EnSkj_WaitToGiveReward(EnSkj* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 6 && Message_ShouldAdvance(play)) {
        s32 item = EnSkj_sOcarinaRewards[((u8*)&gSaveContext)[0x53]];

        GiveItem(&self->actor, play, item, 26.0f, 26.0f);
        self->actionFunc = FUN_003945c4;
    }
}

void EnSkj_WonOcarinaMiniGame(EnSkj* self, PlayState* play) {
    if (EnSkj_sTalkOfferAccepted != 0) {
        self->actionFunc = EnSkj_WaitToGiveReward;
    } else {
        FUN_0036bb28(&self->actor, play, 26.0f);
    }
}


void FUN_0032e1f0(EnSkj* self);


// Draft (not matching, 86 words far: register roles of arg/sound-state base, see STATUS.md)
void FUN_0033704c(s32 arg) {
    u8 flag = EnSkj_sSndBase[0x78C];

    if (flag != 0xFF) {
        if ((arg << 20) == (s32)0xFFF00000) {
            arg |= 0x1000;
        }
    }
    if (arg == 0xCFFF) {
        if (flag != 0xFF) {
            arg = 0xDFFF;
            goto L;
        }
    }
    if (arg == 0x0FFF) {
        if (flag != 0xFF) {
            arg = 0x1FFF;
            goto L;
        }
    }
    if (arg == 0xFFFF) {
        *(s32*)(EnSkj_sSoundState + 0x94) = 0;
        EnSkj_sSoundState[0x14] = 0;
        return;
    }
L:
    {
        s32 v = arg + (s32)0x80000000;
        *(s32*)(EnSkj_sSoundState + 0x94) = v;
        EnSkj_sSoundState[0x3C] = 0;
        EnSkj_sSoundState[0x3D] = 0x0E;
        if (arg != 0xA000) {
            EnSkj_sSoundState[0x3D] = 0x0D;
        }
        *(u16*)(EnSkj_sSoundState + 0x50) = arg & ~0xC000;
        EnSkj_sSoundState[0x2C] = 8;
        EnSkj_sSoundState[0x3B] = 0;
        EnSkj_sSoundState[0x2B] = 0;
        u8 fl = (v != 0) ? 0xFE : 0xFF;
        EnSkj_sSoundState[0x3E] = 0;
        EnSkj_sSoundStateFlag[1] = fl;
    }
    EnSkj_sSoundState[0x14] = 1;
    *(u16*)(EnSkj_sSoundState + 0x46) = 0;
    FUN_0032b184(EnSkj_sSndBufA, 0x1C);
    FUN_0032b184(EnSkj_sSndBufB, 0x1C);
    FUN_0032b184(EnSkj_sSndBufC, 0x1C);
    __rt_memclr_w(EnSkj_sSndBufD, 0x0E);
    if (arg & 0x8000) {
        EnSkj_sSoundState[0x2C] = 0;
    }
    if (arg & 0x4000) {
        EnSkj_sSoundState[0x3A] = 0;
    }
    if (arg & 0xD000) {
        u8 i = 0;
        u8 j = 0;
        u8* t2 = EnSkj_sSndTbl + 0x4A0; u8* t3 = EnSkj_sSndTbl + 0x4B0;
        do {
            u8 e = EnSkj_sSndTbl[j << 3];
            j++;
            if (e != 0xFF) {
                t3[0x6D + i] = t2[e];
                i++;
            }
        } while (i < 8 && j < 0x10);
    }
}

u8* FUN_003371d8(void);


// Draft (not matching, 5 words: switch block layout, see STATUS.md). FUN_003729b8 stays asm in functions_370000s.cpp.
void FUN_003729b8(EnSkj* self, s32 action) {
    self->action = action;
    self->actionFunc = EnSkj_sActionFuncs[action];
    switch (action) {
        case 0:
        case 3:
        case 4:
        case 9:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
            self->unk_A17 = 0;
            break;
        default:
            self->unk_A17 = 1;
            break;
    }
}

void FUN_0014fe9c(EnSkj* self) {
    if (self->unk_A1B != 0) {
        self->actor.flags |= 1;
        FUN_003729b8(self, 0x1A);
    }
}


void FUN_00287ca0(EnSkj* self, PlayState* play) {
    s32 paramsDecr = self->actor.params - 1;

    EnSkj_sOcarinaSkullKids[paramsDecr].unk_0 = 0;
    EnSkj_sOcarinaSkullKids[paramsDecr].skullkid = NULL;
    self->unk_A16 = 1;
    self->actor.velocity.y = 8.0f;
    self->actor.speedXZ = -8.0f;
    FUN_003686a8(self, 0);
    FUN_003729b8(self, 0);
}

void FUN_00217c6c(EnSkj* self, PlayState* play) {
    if (self->actor.velocity.y <= 0.0f) {
        if (self->actor.bgCheckFlags & 2) {
            self->actor.bgCheckFlags &= ~2;
            self->actor.speedXZ = 0.0f;
            FUN_003686a8(self, 5);
            FUN_003729b8(self, 0x12);
        }
    }
}

void FUN_00287c5c(EnSkj* self, PlayState* play) {
    if (EnSkj_sTalkOfferAccepted != 0) {
        FUN_003729b8(self, 0x18);
        return;
    }
    FUN_0036bb28(&self->actor, play, FUN_0035dd04(self));
}

void FUN_00131184(EnSkj* self, PlayState* play) {
    if (EnSkj_sTalkOfferAccepted != 0) {
        FUN_003729b8(self, 0xD);
        return;
    }
    FUN_0036bb28(&self->actor, play, FUN_0035dd04(self));
}

void FUN_003945c4(EnSkj* self, PlayState* play) {
    if (Actor_HasParent(&self->actor, play)) {
        self->actor.parent = NULL;
        self->actionFunc = FUN_0039def0;
    } else {
        GiveItem(&self->actor, play, EnSkj_sOcarinaRewards[((u8*)&gSaveContext)[0x53]], 26.0f, 26.0f);
    }
}

void FUN_001ff780(EnSkj* self, PlayState* play) {
    if (*EnSkj_CurFramePtr((SkelAnime*)self->unk_1A4) == 0.0f) {
        self->unk_A34 = 0.0f;
        self->unk_A10 = 0x5A;
        self->unk_A30 = 600.0f;
        self->unk_A0C = 0x2000;
        FUN_003686a8(self, 7);
        FUN_003729b8(self, 7);
    }
}

// ---- batch 3 (Haiku) candidates ----
s32 Animation_GetLastFrame(SkelAnime* skelAnime, void* anim);
void FUN_0037073c(PlayState* play, s32 arg);
void EnSkj_WaitForPlayback(EnSkj* self, PlayState* play);
void FUN_00372a60(u8* arg);
void FUN_00340bdc(PlayState* play, s32 arg);
void FUN_003523dc(s32 arg);
void FUN_0033704c(s32 arg);
u8* FUN_0033f400(void);
void FUN_0033f248(s32 arg0, s32 arg1);
void FUN_0033f2d8(void);
extern u16 EnSkj_sMsgVars[];
extern __pure float _fabsf(float);
static __inline __pure float EnSkj_Fabsf(float f) {
    return _fabsf(f);
}


void FUN_0027313c(EnSkj* self) {
    f32 last = (f32)Animation_GetLastFrame((SkelAnime*)&self->unk_1A4, (void*)6);

    if (*EnSkj_CurFramePtr((SkelAnime*)&self->unk_1A4) == last) {
        self->unk_A34 = 0.0f;
        self->actor.speedXZ = 2.0f;
        FUN_003686a8(self, 8);
        FUN_003729b8(self, 0x13);
    }
}

void FUN_00114ee8(EnSkj* self) {
    f32 last = (f32)Animation_GetLastFrame((SkelAnime*)&self->unk_1A4, (void*)6);

    if (*EnSkj_CurFramePtr((SkelAnime*)&self->unk_1A4) == last) {
        self->unk_A34 = 0.0f;
        self->unk_A10 = 0x5A;
        self->unk_A30 = 600.0f;
        self->unk_A0C = 0x2000;
        FUN_003686a8(self, 7);
        FUN_003729b8(self, 7);
    }
}

void FUN_003c429c(EnSkj* self) {
    f32 last = (f32)Animation_GetLastFrame((SkelAnime*)&self->unk_1A4, (void*)8);

    if (*EnSkj_CurFramePtr((SkelAnime*)&self->unk_1A4) >= 20.0f) {
        if (*EnSkj_CurFramePtr((SkelAnime*)&self->unk_1A4) < 20.7f) {
            Audio_PlayActorSound2(&self->actor, 0x01000470);
        }
    }
    if (*EnSkj_CurFramePtr((SkelAnime*)&self->unk_1A4) == last) {
        self->unk_A16 = 1;
        FUN_003729b8(self, 9);
    }
}

void FUN_00256d50(EnSkj* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 6 && Message_ShouldAdvance(play)) {
        FUN_0036e980(play, &self->actor, 7);
        self->unk_A16 = 1;
        self->actor.velocity.y = 8.0f;
        self->actor.speedXZ = -8.0f;
        FUN_003686a8(self, 0);
        FUN_003729b8(self, 0);
    }
}

void FUN_001ff7f0(EnSkj* self, PlayState* play) {
    if (Actor_HasParent(&self->actor, play)) {
        self->actor.parent = NULL;
        FUN_003729b8(self, 0xF);
    } else {
        f32 yDist = EnSkj_sSmallStumpSkullKid.skullkid->actor.world.pos.y - self->actor.world.pos.y;
        f32 yRange = EnSkj_Fabsf(yDist) + 10.0f;
        f32 xzRange = FUN_0035dd04(self);

        GiveItem(&self->actor, play, 0x3E, xzRange, yRange);
    }
}

void FUN_00217ed8(EnSkj* self, PlayState* play) {
    s32 msgState = Message_GetState(&play->msgCtx);
    Player* player = GET_PLAYER(play);

    if (msgState == 6 && Message_ShouldAdvance(play)) {
        self->textId = 0x10BC;
        FUN_003686a8(self, 9);
        FUN_003729b8(self, 0xA);
        *(u32*)((u8*)player + 0x1714) |= 0x800000;
        *(Actor**)((u8*)player + 0x1740) = (Actor*)EnSkj_sSmallStumpSkullKid.skullkid;
    }
}

void FUN_00192878(EnSkj* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 4 && Message_ShouldAdvance(play)) {
        s32 choice = FUN_00369f3c(play);

        if (choice != 0) {
            if (choice == 1) {
                Message_ContinueTextbox(play, 0x101D);
                FUN_003729b8(self, 0x16);
            }
        } else {
            Message_ContinueTextbox(play, 0x101E);
            FUN_003729b8(self, 0x15);
        }
    }
}

void FUN_00217cc4(EnSkj* self, PlayState* play) {
    s32 msgState = Message_GetState(&play->msgCtx);

    FUN_0036e980(play, &self->actor, 1);
    if (msgState == 6 && Message_ShouldAdvance(play)) {
        self->actor.velocity.y = 8.0f;
        self->actor.speedXZ = 2.0f;
        FUN_003686a8(self, 0);
        FUN_00372a60((u8*)&self->unk_1A4);
        {
            SkelAnime* skel = (SkelAnime*)&self->unk_1A4;

            skel->curFrame = skel->startFrame;
        }
        FUN_003729b8(self, 0x11);
    }
}

void FUN_00217d50(EnSkj* self, PlayState* play) {
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 10, (s16)self->unk_A34, 0);
    Math_ApproachF(&self->unk_A34, 2000.0f, 1.0f, 100.0f);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    if (self->actor.xzDistToPlayer < 120.0f) {
        self->actor.speedXZ = 0.0f;
        FUN_00367c7c(play, 0x101C, &self->actor);
        FUN_003686a8(self, 9);
        FUN_003729b8(self, 0x14);
    }
}

void FUN_0039def0(EnSkj* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 6 && Message_ShouldAdvance(play)) {
        u8 idx = ((u8*)&gSaveContext)[0x53];

        if (idx < 3) {
            ((u8*)&gSaveContext)[0x53] = idx + 1;
        }
        if (idx == 2) {
            *(u16*)((u8*)&gSaveContext + 0xF0A) |= 0x80;
            self->actionFunc = EnSkj_CleanupOcarinaGame;
        } else {
            Message_ContinueTextbox(play, 0x102E);
            self->actionFunc = EnSkj_WaitForOfferResponse;
        }
    }
}

void FUN_00256fb0(EnSkj* self) {
    if (self->unk_A20 != 0xFF) {
        s32 v = self->unk_A20 + 0x14;
        self->unk_A20 = v;
        self->unk_A20 = ((u32)v > 0xFF) ? 0xFF : v;
    }
    {
        s32 paramsDecr = self->actor.params - 1;
        if (EnSkj_sOcarinaSkullKids[paramsDecr].unk_0 == 2) {
            self->actor.velocity.y = 8.0f;
            self->actor.speedXZ = -8.0f;
            FUN_003686a8(self, 0);
            FUN_003729b8(self, 0x1C);
            return;
        }
    }
    if (self->unk_A1C[0] != 0) {
        FUN_003686a8(self, 2);
        FUN_003729b8(self, 0x1B);
    }
}

void FUN_002732c4(EnSkj* self) {
    if (self->unk_A20 != 0xFF) {
        s32 v = self->unk_A20 + 0x14;
        self->unk_A20 = v;
        self->unk_A20 = ((u32)v > 0xFF) ? 0xFF : v;
    }
    {
        s32 paramsDecr = self->actor.params - 1;
        if (EnSkj_sOcarinaSkullKids[paramsDecr].unk_0 == 2) {
            self->actor.velocity.y = 8.0f;
            self->actor.speedXZ = -8.0f;
            FUN_003686a8(self, 0);
            FUN_003729b8(self, 0x1C);
            return;
        }
    }
    if (self->unk_A1C[0] == 0) {
        FUN_003686a8(self, 9);
        FUN_003729b8(self, 0x1A);
    }
}

void EnSkj_StartOcarinaMinigame(EnSkj* self, PlayState* play) {
    s32 msgState = Message_GetState(&play->msgCtx);
    Player* player = GET_PLAYER(play);

    Math_SmoothStepToS(&player->actor.shape.rot.y, self->actor.world.rot.y, 5, 0x7D0, 0);
    {
        u16 rot = player->actor.shape.rot.y;

        player->actor.world.rot.y = rot;
        *(u16*)((u8*)player + 0x2220) = rot;
    }
    if (msgState == 2) {
        FUN_0037073c(play, 0x2E);
        if (EnSkj_sOcarinaSkullKids[0].skullkid != NULL) {
            EnSkj_sOcarinaSkullKids[0].skullkid->unk_A1C[0] = 1;
        }
        self->unk_A0E = 0xF0;
        self->actionFunc = EnSkj_WaitForPlayback;
    }
}


void FUN_00192904(EnSkj* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 6) {
        if (Message_ShouldAdvance(play)) {
            if (*(u16*)((u8*)&gSaveContext + 0xF0A) & 0x40) {
                self->textId = 0x10BC;
                FUN_003686a8(self, 9);
                FUN_003729b8(self, 0xA);
            } else {
                f32 yRange;

                FUN_003729b8(self, 0xE);
                yRange = EnSkj_Fabsf(EnSkj_sSmallStumpSkullKid.skullkid->actor.world.pos.y - self->actor.world.pos.y) + 10.0f;
                GiveItem(&self->actor, play, 0x3E, FUN_0035dd04(self), yRange);
            }
        }
    }
}

void FUN_003b422c(EnSkj* self) {
    f32 last = (f32)Animation_GetLastFrame((SkelAnime*)&self->unk_1A4, (void*)7);

    if (*EnSkj_CurFramePtr((SkelAnime*)&self->unk_1A4) == last) {
        if (*((u8*)self + 0xA19) == 0) {
            *((u8*)self + 0xA19) = 3;
            self->actor.velocity.y = 8.0f;
            self->actor.speedXZ = -8.0f;
            FUN_003686a8(self, 0);
            FUN_003729b8(self, 0);
        } else {
            self->unk_A34 = 0.0f;
            self->unk_A10 = 0x5A;
            self->unk_A30 = 600.0f;
            self->unk_A0C = 0x2000;
            FUN_003686a8(self, 7);
            FUN_003729b8(self, 7);
        }
    }
}

static inline void EnSkj_SetNaviId(EnSkj* self) {
    switch (self->actor.params) {
        case 0:
            if (gSaveContext.itemGetInf[3] & 0x200) {
                *((u8*)self + 0x123) = 0x41;
            } else if (gSaveContext.itemGetInf[1] & 0x40) {
                *((u8*)self + 0x123) = 0x40;
            } else {
                *((u8*)self + 0x123) = 0x3F;
            }
            break;

        case 1:
        case 2:
            *((u8*)self + 0x123) = 0x3F;
            break;

        default:
            *((u8*)self + 0x123) = 0x36;
            break;
    }
}

static inline void EnSkj_SetupWaitInRange(EnSkj* self) {
    self->textId = 0x10BC;
    FUN_003686a8(self, 9);
    FUN_003729b8(self, 0xA);
}

void FUN_00217e0c(EnSkj* self, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == 6) && Message_ShouldAdvance(play)) {
        gSaveContext.itemGetInf[1] |= 0x40;
        EnSkj_SetNaviId(self);
        EnSkj_SetupWaitInRange(self);
    }
}

void Rupees_ChangeBy(s16 value);
void FUN_00345fcc(PlayState* play);
void Item_Give(PlayState* play, s32 itemId);
void FUN_00256dd0(EnSkj* self, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == 6) && Message_ShouldAdvance(play)) {
        Rupees_ChangeBy(10);
        gSaveContext.itemGetInf[3] |= 0x200;
        EnSkj_SetNaviId(self);
        FUN_00345fcc(play);
        Item_Give(play, 0x2C);
        Message_ContinueTextbox(play, 0x101F);
        FUN_003729b8(self, 0x16);
    }
}

void Collider_SetCylinderType1(PlayState* play, ColliderCylinder* collider, Actor* actor, ColliderCylinderInit* init);
extern InitChainEntry EnSkj_sInitChain[];
extern ColliderCylinderInit EnSkj_sColliderInit;
extern DamageTable EnSkj_sDamageTable;
void EnSkj_SariasSongShortStumpUpdate(Actor* thisx, PlayState* play);
void EnSkj_OcarinaMinigameShortStumpUpdate(EnSkj* self, PlayState* play);
void EnSkj_SetupWaitForOcarina(EnSkj* self, PlayState* play);
void FUN_0034fe20(Actor* actor, PlayState* play, SkelAnime* skelAnime, s32 cmbIndex, s32 csabIndex, void* jointTable,
                  void* morphTable, s32 limbCount);

// Draft (near miss, 1 word: `strh r5` vs `strh r0` for actor.params = type, see STATUS.md). GLOBAL_ASM kept.
#if 0
static inline void EnSkj_SetupResetFight(EnSkj* self) {
    self->unk_A30 = 200.0f;
    self->unk_A0C = 0xAAA;
    FUN_003686a8(self, 2);
    FUN_003729b8(self, 2);
}

void EnSkj_Init(Actor* thisx, PlayState* play) {
    u16 type = (thisx->params >> 10) & 0x3F;
    EnSkj* self = (EnSkj*)thisx;

    *((u8*)self + 0xA44) = ~thisx->params & 1;
    thisx->params |= 1;
    FUN_00372f38(thisx, play, self->model0, 1, 0);
    Actor_ProcessInitChain(thisx, EnSkj_sInitChain);
    switch (type) {
        case 5:
            EnSkj_sSmallStumpSkullKid.unk_0 = 1;
            EnSkj_sSmallStumpSkullKid.skullkid = self;
            self->actor.destroy = NULL;
            self->actor.draw = NULL;
            self->actor.update = (ActorFunc)EnSkj_SariasSongShortStumpUpdate;
            self->actor.flags &= ~5;
            Actor_ChangeType(play, &play->actorCtx, thisx, 6);
            break;

        case 6:
            EnSkj_sSmallStumpSkullKid.unk_0 = 1;
            EnSkj_sSmallStumpSkullKid.skullkid = self;
            self->actor.destroy = NULL;
            self->actor.draw = NULL;
            self->actor.update = (ActorFunc)EnSkj_OcarinaMinigameShortStumpUpdate;
            self->actor.flags &= ~5;
            Actor_ChangeType(play, &play->actorCtx, thisx, 6);
            self->actor.focus.pos.x = 1230.0f;
            self->actor.focus.pos.y = -90.0f;
            self->actor.focus.pos.z = 450.0f;
            self->actionFunc = EnSkj_SetupWaitForOcarina;
            break;

        default:
            self->actor.params = type;
            if (((self->actor.params != 0) && (self->actor.params != 1)) && (self->actor.params != 2)) {
                if (gSaveContext.inventory.items[SLOT(0x2D)] < 0x32) {
                    Actor_Kill(&self->actor);
                    return;
                }
            }

            EnSkj_SetNaviId(self);
            FUN_0034fe20(&self->actor, play, (SkelAnime*)self->unk_1A4, 0, 3, (u8*)self + 0x228, (u8*)self + 0x5D0, 0x12);
            if (type <= 2) {
                self->actor.flags &= ~5;
                self->actor.flags |= 9;
                Actor_ChangeType(play, &play->actorCtx, &self->actor, 4);
            }

            if ((type < 0) || (type >= 7)) {
                self->actor.flags &= ~0x2000000;
            }

            if ((type >= 1) && (type <= 2)) {
                s32 idx = type - 1;
                self->actor.targetMode = 7;
                self->unk_A38 = self->actor.world.pos;
                EnSkj_sOcarinaSkullKids[idx].unk_0 = 1;
                EnSkj_sOcarinaSkullKids[idx].skullkid = self;
                self->unk_A1C[0] = 0;
                self->unk_A20 = 0;
                *((u8*)self + 0xD0) = 0;
                self->actor.flags &= ~1;
                FUN_003686a8(self, 9);
                FUN_003729b8(self, 0x19);
            } else {
                self->unk_A20 = 0xFF;
                *((u8*)self + 0xD0) = 0xFF;
                EnSkj_SetupResetFight(self);
            }

            self->actor.colChkInfo.damageTable = &EnSkj_sDamageTable;
            self->actor.colChkInfo.health = 10;
            Collider_InitCylinder(play, (ColliderCylinder*)self->unk_9B0);
            Collider_SetCylinderType1(play, (ColliderCylinder*)self->unk_9B0, &self->actor, &EnSkj_sColliderInit);
            ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 40.0f);
            Actor_SetScale(thisx, 0.01f);
            self->actor.speedXZ = 0.0f;
            self->actor.gravity = -1.0f;
            self->actor.velocity.y = 0.0f;
            self->actor.textId = self->textId = 0;
            self->unk_A0E = 0;
            self->unk_A16 = 0;
            self->unk_A18 = 3;
            self->unk_A19[0] = 3;
            {
                Vec3f mult;
                MTX34 mtx;

                mult.x = 0.0f;
                mult.y = 0.0f;
                mult.z = 120.0f;

                Matrix_RotateY(&mtx, 0, self->actor.shape.rot.y * (1.0f / 32768.0f) * 3.1415927f);
                Vec3f* d = &self->unk_A24;
                VEC3TransformAsm((Vector3f*)d, &mtx, (Vector3f*)&mult);

                self->unk_A24.x += self->actor.world.pos.x;
                self->unk_A24.z += self->actor.world.pos.z;
            }
            break;
    }
}
#endif
GLOBAL_ASM("asm/EnSkj_Init.s")

s32 FUN_0036cd8c(PlayState* play);
void FUN_003cfeac(EnSkj* self, PlayState* play) {
    if (self->unk_A0E == 0) {
        self->unk_A30 = 200.0f;
        self->unk_A0C = 0xAAA;
        FUN_003686a8(self, 2);
        FUN_003729b8(self, 2);
    } else {
        s32 diff;
        s32 reset = 0;

        if (*(f32*)((u8*)self + 0x98) < self->unk_A30) {
            reset = FUN_0036cd8c(play);
        }
        if (reset == 0) {
            diff = (s16)(*(u16*)((u8*)self + 0x92) - *(u16*)((u8*)self + 0x36));
            if ((s16)self->unk_A0C > diff && -(s16)self->unk_A0C < diff) {
                reset = 1;
            }
        }
        if (reset != 0) {
            self->unk_A0E = 0x384;
            self->unk_A34 = 0.0f;
            self->unk_A10 = 0x5A;
            self->unk_A30 = 600.0f;
            self->unk_A0C = 0x2000;
            FUN_003686a8(self, 7);
            FUN_003729b8(self, 7);
        }
    }
}

void EffectSsDeadDb_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep,
                          s16 primR, s16 primG, s16 primB, s16 primA, s16 envR, s16 envG, s16 envB, s16 unused,
                          s32 arg14, s16 playSfx);
void FUN_003c41a8(EnSkj* self, PlayState* play) {
    Vec3f pos;
    Vec3f vel;
    Vec3f accel;
    s32 v = self->unk_A20 - 4;

    if ((u32)v > 0xFF) {
        v = 0;
    }
    self->unk_A20 = v;
    *((u8*)self + 0xD0) = (u8)self->unk_A20;
    pos.x = Rand_CenteredFloat(30.0f) + self->actor.world.pos.x;
    pos.y = Rand_CenteredFloat(30.0f) + self->actor.world.pos.y;
    pos.z = Rand_CenteredFloat(30.0f) + self->actor.world.pos.z;
    accel.z = 0.0f;
    accel.y = 0.0f;
    accel.x = 0.0f;
    vel.z = 0.0f;
    vel.y = 0.0f;
    vel.x = 0.0f;
    EffectSsDeadDb_Spawn(play, &pos, &vel, &accel, 100, 10, 255, 255, 255, 255, 0, 0, 255, 1, 11, 1);
}

void FUN_0014f84c(EnSkj* self, PlayState* play) {
    s32 v = self->unk_A20;

    if (*((u8*)self + 0xA1A) == 2) {
        *(u16*)((u8*)play + 0x2B7E) = 0;
        *((u8*)self + 0xA1A) = 0;
    }
    if (*((u8*)self + 0xA16) != 0) {
        v = v - 0x14;
        if ((u32)v > 0xFF) {
            v = 0;
        }
        self->unk_A20 = v;
        *((u8*)self + 0xD0) = (u8)self->unk_A20;
    }
    if (self->actor.velocity.y <= 0.0f) {
        if (self->actor.bgCheckFlags & 2) {
            self->actor.bgCheckFlags &= ~2;
            {
                f32 angle;
                Vec3f a;
                MTX34 mtx;

                a.x = 0.0f;
                a.y = 0.0f;
                a.z = 120.0f;
                angle = (f32)self->actor.shape.rot.y * 3.0517578125e-05f * 3.1415927f;
                Matrix_RotateY(&mtx, 0, angle);
                Vec3f* d = &self->unk_A24;
                VEC3TransformAsm((Vector3f*)d, &mtx, (Vector3f*)&a);
                self->unk_A24.x += self->actor.world.pos.x;
                self->unk_A24.z += self->actor.world.pos.z;
            }
            self->actor.speedXZ = 0.0f;
            FUN_003686a8(self, 5);
            FUN_003729b8(self, 5);
        }
    }
}

void SkelAnime_SetPlaySpeed(SkelAnime* skelAnime, f32 speed);

static inline void EnSkj_Backflip(EnSkj* self) {
    self->actor.velocity.y = 8.0f;
    self->actor.speedXZ = -8.0f;
    FUN_003686a8(self, 0);
    FUN_003729b8(self, 0);
}

static inline s32 EnSkj_func_80AFEDF8(EnSkj* self, PlayState* play) {
    s16 yawDiff;

    if (self->actor.xzDistToPlayer < self->unk_A30) {
        if (FUN_0036cd8c(play) != 0) {
            return 1;
        }
    }

    yawDiff = self->actor.yawTowardsPlayer - self->actor.world.rot.y;

    if ((yawDiff < (s16)self->unk_A0C) && (-(s16)self->unk_A0C < yawDiff)) {
        return 1;
    }

    return 0;
}

void FUN_0014f978(EnSkj* self, PlayState* play) {
    Vec3f pos1;
    Vec3f pos2;
    MTX34 mtx;
    f32 phi_f14;
    s16 yawDistToPlayer;

    if (self->unk_A10 == 0) {
        *((u8*)self + 0xA18) = 3;
        FUN_003686a8(self, 1);
        FUN_003729b8(self, 1);
    } else if (self->unk_A0E != 0) {
        yawDistToPlayer = Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 5, self->unk_A34, 0);
        self->actor.world.rot.y = self->actor.shape.rot.y;
        Math_ApproachF(&self->unk_A34, 4000.0f / 3.0f, 1.0f, 400.0f / 3.0f);

        pos1.x = 0.0f;
        pos1.y = 0.0f;
        pos1.z = -120.0f;

        Matrix_RotateY(&mtx, 0, self->actor.shape.rot.y * (1.0f / 32768.0f) * 3.1415927f);
        VEC3TransformAsm((Vector3f*)&pos2, &mtx, (Vector3f*)&pos1);
        f32 prevPosX = self->actor.world.pos.x;
        f32 prevPosZ = self->actor.world.pos.z;
        self->actor.world.pos.x = self->unk_A24.x + pos2.x;
        self->actor.world.pos.z = self->unk_A24.z + pos2.z;
        { f32 dx = self->actor.world.pos.x - prevPosX; f32 dz = self->actor.world.pos.z - prevPosZ; phi_f14 = sqrtf(dx * dx + dz * dz); }

        phi_f14 = (phi_f14 > 10.0f) ? 10.0f : phi_f14;
        phi_f14 *= 0.1f;

        if (yawDistToPlayer < 0) { SkelAnime_SetPlaySpeed((SkelAnime*)&self->unk_1A4, -1.0f - phi_f14); } else { SkelAnime_SetPlaySpeed((SkelAnime*)&self->unk_1A4, phi_f14 + 1.0f); }
    } else if (EnSkj_func_80AFEDF8(self, play) != 0) {
        self->unk_A16 = 1;
        EnSkj_Backflip(self);
    } else {
        self->unk_A30 = 200.0f;
        self->unk_A0C = 0xAAA;
        FUN_003686a8(self, 2);
        FUN_003729b8(self, 2);
    }
}

static inline s32 EnSkj_RangeCheck(Player* player, EnSkj* self) {
    f32 xDiff = player->actor.world.pos.x - self->actor.world.pos.x;
    f32 zDiff = player->actor.world.pos.z - self->actor.world.pos.z;
    f32 d2 = SQ(xDiff) + SQ(zDiff);
    f32 yDiff = player->actor.world.pos.y - self->actor.world.pos.y;

    return (d2 <= 676.0f) && (yDiff >= 0.0f);
}


s32 Player_GetMask(PlayState* play);
u16 Text_GetFaceReaction(PlayState* play, u32 reactionSet);
void FUN_00343868(PlayState* play, s32 arg);
f32 FUN_0035dd04(EnSkj* self);

static inline void EnSkj_TurnPlayer(EnSkj* self, Player* player) {
    Math_SmoothStepToS(&player->actor.shape.rot.y, self->actor.world.rot.y, 5, 0x7D0, 0);
    {
        u16 rot = player->actor.shape.rot.y;

        player->actor.world.rot.y = rot;
        *(u16*)((u8*)player + 0x2220) = rot;
    }
}

void FUN_0014fbfc(EnSkj* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (*(u32*)((u8*)player + 0x1714) & 0x1000000) {
        *(u32*)((u8*)player + 0x1714) |= 0x2000000;
        *(Actor**)((u8*)player + 0x1740) = (Actor*)EnSkj_sSmallStumpSkullKid.skullkid;
        player->actor.world.pos.x = EnSkj_sSmallStumpSkullKid.skullkid->actor.world.pos.x;
        player->actor.world.pos.y = EnSkj_sSmallStumpSkullKid.skullkid->actor.world.pos.y;
        player->actor.world.pos.z = EnSkj_sSmallStumpSkullKid.skullkid->actor.world.pos.z;
        EnSkj_TurnPlayer(EnSkj_sSmallStumpSkullKid.skullkid, player);
        FUN_00343868(play, 0x22);
        self->unk_A1A = 0;
        FUN_003729b8(self, 0xB);
    } else if (EnSkj_sTalkOfferAccepted != 0) {
        player->actor.world.pos.x = EnSkj_sSmallStumpSkullKid.skullkid->actor.world.pos.x;
        player->actor.world.pos.y = EnSkj_sSmallStumpSkullKid.skullkid->actor.world.pos.y;
        player->actor.world.pos.z = EnSkj_sSmallStumpSkullKid.skullkid->actor.world.pos.z;
        if ((Player_GetMask(play) == 2) && !(gSaveContext.itemGetInf[3] & 0x200)) {
            Audio_PlaySoundGeneral(0x0100048C, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
            FUN_003729b8(self, 0x10);
        } else {
            FUN_003729b8(self, 0xD);
        }
    } else if (!EnSkj_RangeCheck(player, EnSkj_sSmallStumpSkullKid.skullkid)) {
        self->unk_A30 = 200.0f;
        self->unk_A0C = 0xAAA;
        FUN_003686a8(self, 2);
        FUN_003729b8(self, 2);
    } else {
        *(u32*)((u8*)player + 0x1714) |= 0x800000;
        if (gSaveContext.itemGetInf[1] & 0x40) {
            if (gSaveContext.itemGetInf[3] & 0x200) {
                self->textId = Text_GetFaceReaction(play, 0x15);
                if (self->textId == 0) {
                    self->textId = 0x1020;
                }
            } else if (Player_GetMask(play) == 0) {
                self->textId = 0x10BC;
            } else if (Player_GetMask(play) == 2) {
                self->textId = 0x101B;
            } else {
                self->textId = Text_GetFaceReaction(play, 0x15);
            }
            FUN_0036bb28(&self->actor, play, FUN_0035dd04(self));
        }
    }
}

s32 GetMessageMode(PlayState* play);
s32 FUN_004896d4(s32 sfxId);
void FUN_003725e0(PlayState* play);
void FUN_00334fa0(PlayState* play);
void FUN_00340bdc(PlayState* play, s32 arg);
void FUN_003523dc(s32 arg);
void FUN_0033f248(s32 arg0, s32 arg1);
void EnSkj_FailedMiniGame(EnSkj* self, PlayState* play);
void EnSkj_WonOcarinaMiniGame(EnSkj* self, PlayState* play);

void EnSkj_WaitForPlayback(EnSkj* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    EnSkj_TurnPlayer(self, player);

    if (*(u16*)((u8*)play + 0x2B7E) == 3) {
        FUN_003725e0(play);
        *(u16*)((u8*)play + 0x2B7E) = 4;
        *(Actor**)((u8*)player + 0x1740) = &self->actor;
        FUN_0036bb28(&self->actor, play, 26.0f);
        self->textId = 0x102D;
        self->actionFunc = EnSkj_FailedMiniGame;
        return;
    } else if (*(u16*)((u8*)play + 0x2B7E) == 0xF) {
        Audio_PlaySoundGeneral(0x01000487, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        FUN_003725e0(play);
        *(u16*)((u8*)play + 0x2B7E) = 4;
        *(Actor**)((u8*)player + 0x1740) = &self->actor;
        FUN_0036bb28(&self->actor, play, 26.0f);
        self->textId = 0x10BF;
        self->actionFunc = EnSkj_WonOcarinaMiniGame;
    } else {
        switch (GetMessageMode(play)) {
            case 0x2B:
                if (EnSkj_sOcarinaSkullKids[0].skullkid != NULL) {
                    EnSkj_sOcarinaSkullKids[0].skullkid->unk_A1C[0] = 0;
                }
                if (!FUN_004896d4(0x010004BB)) {
                    if (EnSkj_sOcarinaSkullKids[1].skullkid != NULL) {
                        EnSkj_sOcarinaSkullKids[1].skullkid->unk_A1C[0] = 1;
                    }
                    FUN_00334fa0(play);
                }
                break;
            case 0x2D:
                if (EnSkj_sOcarinaSkullKids[1].skullkid != NULL) {
                    EnSkj_sOcarinaSkullKids[1].skullkid->unk_A1C[0] = 0;
                }
                if (!FUN_004896d4(0x010004BB)) {
                    FUN_00334fa0(play);
                    self->unk_A0E = 0xF0;
                }
                break;
            case 0x2E:
                if (self->unk_A0E != 0) {
                    self->unk_A0E--;
                } else {
                    Audio_PlaySoundGeneral(0x010004AC, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
                    FUN_003725e0(play);
                    *(u16*)((u8*)play + 0x2B7E) = 4;
                    *(Actor**)((u8*)player + 0x1740) = &self->actor;
                    FUN_0036bb28(&self->actor, play, 26.0f);
                    self->textId = 0x102D;
                    self->actionFunc = EnSkj_FailedMiniGame;
                }
                break;
            case 0x30:
                if (!FUN_004896d4(0x010004BB)) {
                    if (EnSkj_sOcarinaSkullKids[0].skullkid != NULL) {
                        EnSkj_sOcarinaSkullKids[0].skullkid->unk_A1C[0] = 1;
                    }
                    self->unk_A0E = 0xF0;
                    FUN_003523dc(6);
                    FUN_0033f248(0xE, 1);
                    FUN_00340bdc(play, 0x2A);
                    *((u8*)play + 0x2B73) = 2;
                }
                break;
        }
    }
}

static inline void EnSkj_SetupWrongSong(EnSkj* self) {
    self->textId = 0x1041;
    FUN_003686a8(self, 9);
    FUN_003729b8(self, 0x17);
}

static inline void EnSkj_SetupAfterSong(EnSkj* self) {
    self->unk_A1A = 0;
    FUN_003686a8(self, 9);
    FUN_003729b8(self, 0xC);
}

void FUN_001929e0(EnSkj* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (!(gSaveContext.itemGetInf[1] & 0x40) && ((GetMessageMode(play) == 0xE) || (GetMessageMode(play) == 0xF))) {
        *(u16*)((u8*)play + 0x2B7E) = 4;
        FUN_003725e0(play);
        *(Actor**)((u8*)player + 0x1740) = &self->actor;
        FUN_0036bb28(&self->actor, play, FUN_0035dd04(self));
        EnSkj_SetupWrongSong(self);
    } else {
        if ((GetMessageMode(play) == 0xD) && (self->unk_A1A == 0)) {
            self->unk_A1A = 1;
            FUN_003686a8(self, 2);
        } else if ((self->unk_A1A != 0) && (GetMessageMode(play) == 0x1A)) {
            self->unk_A1A = 0;
            FUN_003686a8(self, 9);
        }
        if (*(u16*)((u8*)play + 0x2B7E) == 4) {
            *(u16*)((u8*)play + 0x2B7E) = 0;
            self->unk_A1A = 0;
            FUN_003686a8(self, 9);
            FUN_003729b8(self, 0xA);
        } else if (*(u16*)((u8*)play + 0x2B7E) == 3) {
            if (!(gSaveContext.itemGetInf[1] & 0x40)) {
                *(u16*)((u8*)play + 0x2B7E) = 4;
                Audio_PlaySoundGeneral(0x01000487, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
                *(Actor**)((u8*)player + 0x1740) = &self->actor;
                FUN_0036bb28(&self->actor, play, FUN_0035dd04(self));
                self->textId = 0x10BB;
                EnSkj_SetupAfterSong(self);
            } else {
                *(u16*)((u8*)play + 0x2B7E) = 5;
            }
        } else if (*(u16*)((u8*)play + 0x2B7E) == 2) {
            *(u32*)((u8*)player + 0x1714) &= ~0x1000000;
        } else if (*(u16*)((u8*)play + 0x2B7E) == 1) {
            *(u32*)((u8*)player + 0x1714) |= 0x800000;
        } else {
            if (*(u16*)((u8*)play + 0x2B7E) >= 5) {
                *(u16*)((u8*)&gSaveContext + 0x15B0) = 0;
                if (gSaveContext.itemGetInf[1] & 0x40) {
                    *(u16*)((u8*)play + 0x2B7E) = 4;
                    *(Actor**)((u8*)player + 0x1740) = &self->actor;
                    FUN_0036bb28(&self->actor, play, FUN_0035dd04(self));
                    self->textId = 0x10BD;
                    EnSkj_SetupAfterSong(self);
                } else {
                    *(u16*)((u8*)play + 0x2B7E) = 4;
                    *(Actor**)((u8*)player + 0x1740) = &self->actor;
                    FUN_0036bb28(&self->actor, play, FUN_0035dd04(self));
                    EnSkj_SetupWrongSong(self);
                }
            }
        }
    }
}

static inline s32 EnSkj_ShootNeedle(EnSkj* self, PlayState* play) {
    Vec3f pos;
    Vec3f pos2;
    Actor* needle;
    MTX34 mtx;

    pos.x = 1.5f;
    pos.y = 0.0f;
    pos.z = 40.0f;

    Matrix_RotateY(&mtx, 0, self->actor.shape.rot.y * (1.0f / 32768.0f) * 3.1415927f);
    VEC3TransformAsm((Vector3f*)&pos2, &mtx, (Vector3f*)&pos);

    pos2.x += self->actor.world.pos.x;
    pos2.z += self->actor.world.pos.z;
    pos2.y = self->actor.world.pos.y + 27.0f;

    needle = Actor_Spawn(&play->actorCtx, play, 0x116, pos2.x, pos2.y, pos2.z, self->actor.shape.rot.x,
                         self->actor.shape.rot.y, self->actor.shape.rot.z, 0, 1);
    if (needle != NULL) {
        *(u16*)((u8*)needle + 0x206) = 100;
        needle->speedXZ = 24.0f;
        return 1;
    }
    return 0;
}

void FUN_00104218(EnSkj* self, PlayState* play) {
    u8 val;
    f32 lastFrame = (f32)Animation_GetLastFrame((SkelAnime*)&self->unk_1A4, (void*)9);

    if (*EnSkj_CurFramePtr((SkelAnime*)&self->unk_1A4) == lastFrame && self->unk_A10 == 0) {
        val = *((u8*)self + 0xA18);
        if (*((u8*)self + 0xA18) != 0) {
            EnSkj_ShootNeedle(self, play);
            val--;
            self->unk_A10 = 6;
            *((u8*)self + 0xA18) = val;
        } else {
            FUN_00372a60((u8*)&self->unk_1A4);
            FUN_003729b8(self, 8);
        }
    }
}

void FUN_0032b184(void* p, u32 n);
void __rt_memclr_w(void* p, u32 n);

void FUN_003cfcec(EnSkj* self, PlayState* play) {
    if (self->actor.params == 0) {
        if (!(gSaveContext.itemGetInf[1] & 0x40) && self->actor.xzDistToPlayer < 200.0f) {
            self->unk_A16 = 1;
            EnSkj_Backflip(self);
        } else if (EnSkj_sSmallStumpSkullKid.unk_0 != 0) {
            Player* player = GET_PLAYER(play);

            if (EnSkj_RangeCheck(player, EnSkj_sSmallStumpSkullKid.skullkid)) {
                self->textId = 0x10BC;
                FUN_003686a8(self, 9);
                FUN_003729b8(self, 0xA);
                *(u32*)((u8*)player + 0x1714) |= 0x800000;
                *(Actor**)((u8*)player + 0x1740) = (Actor*)EnSkj_sSmallStumpSkullKid.skullkid;
            }
        }
    } else {
        if (EnSkj_func_80AFEDF8(self, play) != 0) {
            self->unk_A30 = 600.0f;
            self->unk_A0C = 0x2000;
            self->unk_A0E = 0x258;
            FUN_003686a8(self, 6);
            FUN_003729b8(self, 6);
        }
    }
}
}
