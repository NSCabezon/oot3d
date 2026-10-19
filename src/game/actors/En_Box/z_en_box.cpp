extern "C" {
#include "z_en_box.hpp"

#define FLAGS 0x00000000
#define R_UPDATE_RATE SREG(30)

void EnBox_Init(Actor* thisx, PlayState* play);
void EnBox_Destroy(Actor* thisx, PlayState* play);
void EnBox_Update(EnBox* self, PlayState* play);
f32* EnBox_CurFramePtr(SkelAnime* skelAnime);
f32 BgCheck_EntityRaycastFloor4(CollisionContext* colCtx, CollisionPoly** outPoly, s32* bgId, Actor* actor, Vec3f* pos);
void FUN_00290ce0(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 life);
void EnBox_Draw(Actor* thisx, PlayState* play);

void EnBox_Fall(EnBox* self, PlayState* play);
void EnBox_AppearInit(EnBox* self, PlayState* play);
void EnBox_AppearAnimation(EnBox* self, PlayState* play);
void EnBox_WaitOpen(EnBox* self, PlayState* play);
void EnBox_Open(EnBox* self, PlayState* play);
void FUN_00353484(Actor* actor, PlayState* play);
void OnePointCutscene_EndCutscene(PlayState* play, s16 camId);
void FUN_0027f34c(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel, s32 a, s32 b, s32 c);
s32 Player_InCsMode(PlayState* play);
Actor** FUN_0033de14(s32 size);
void FUN_0033ddd4(void* ptr);
s32 FUN_00360084(ActorContext* actorCtx, s32 actorId, s32 arg2, Actor** out, s32 max);
s32 FUN_003603c0(SkelAnime* skelAnime, s32 anim);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 anim, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void FUN_0035c528(u32 seqId);
s32 FUN_0036f2f8(Actor* actor, s32 angle, PlayState* play);
void FUN_00346778(Actor* actor, PlayState* play, s32 getItemId);
s32 FUN_00387fd0(Actor* actor);
s32 OnePointCutscene_Init(PlayState* play, s32 csId, s32 duration, Actor* actor, s32 camId);
s32 OnePointCutscene_Attention(PlayState* play, Actor* actor, s32 arg2, s32 arg3);
s32 FUN_0036ae04(void);
s32 OnePointCutscene_CheckForCategory(PlayState* play, s32 category);
void FUN_0037073c(PlayState* play, s32 action);
void EffectSs_Spawn(PlayState* play, s32 type, s32 priority, void* initParams);
void FUN_003356d4(PlayState* play, s32 type, Vec3f* pos, Vec3f* vel, Vec3f* accel, void* a, void* b, s32 c,
                  s32 d, s32 e, s32 f);
void* FUN_003687a8(void* arg);
void FUN_003589cc(void* arg, s32 arg2);
void FUN_00358964(void* arg, s32 arg2, void* color);


void FUN_0034f6bc(PlayState* play, DynaCollisionContext* dyna, s32 bgId);
void FUN_0036b940(PlayState* play, DynaCollisionContext* dyna, s32 bgId);
void FUN_00358ea8(ZARInfo* zar, PlayState* play, SkelAnime* skel, void* cmb, void* p, s32 b, void* jointTable,
                  void* morphTable, s32 limbCount);
void* ZAR_GetCMBByIndex(ZARInfo* zar, s32 index);
void FUN_0037266c(void* model, s32 idx);
void FUN_0036932c(void* model, s32 idx);

GLOBAL_ASM("data/z_En_Box.data.s")

ActorInit En_Box_InitVars = {
    ACTOR_EN_BOX,
    ACTORCAT_CHEST,
    FLAGS,
    OBJECT_BOX,
    sizeof(EnBox),
    (ActorFunc)EnBox_Init,
    (ActorFunc)EnBox_Destroy,
    (ActorFunc)EnBox_Update,
    (ActorFunc)EnBox_Draw,
};

GLOBAL_ASM("asm/EnBox_Init.s")

#if 0
/* Real statics for the drafts below (data chunk 0x5213C8..0x5213E8 would have to be dropped). */
static f32 sIceSmokeMinFrame = 30.0f;

static InitChainEntry sInitChain[] = {
    ICHAIN_U8(targetMode, 0, ICHAIN_STOP),
};

static s32 sIceSmokeTimerBase = 7;
static s32 sIceSmokeTimerThreshold = 30;

static s32 sAnimations[4] = { 1, 0, 2, 2 };
#endif


void EnBox_Destroy(Actor* thisx, PlayState* play) {
    EnBox* self = (EnBox*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    SkelAnime_Free2(&self->skelAnime);
}

#if 0
/* Draft, 9 words: movementFlags bic r1,r0 RMW reg; 0x7C param compare mov r1/r0 swap; timer 0x392/0x394 ldrb order; accel vstr order. Needs EnBox_CurFramePtr in its own TU (z_en_box_anim.cpp, linker-inlined) when enabled. */
void EnBox_Update(EnBox* self, PlayState* play) {

    if (self->movementFlags & 0x10) {
        CollisionPoly* poly;
        s32 bgId;
        Vec3f checkPos;
        f32 newY;

        self->movementFlags &= ~0x10;
        checkPos = self->dyna.actor.world.pos;
        checkPos.y += 1.0f;
        newY = BgCheck_EntityRaycastFloor4(&play->colCtx, &poly, &bgId, &self->dyna.actor, &checkPos);
        if (newY != -32000.0f) {
            self->dyna.actor.world.pos.y = newY;
        }
        {
            u32 fl = self->dyna.actor.flags;
            self->dyna.actor.flags = fl & ~0x10;
        }
    }

    self->actionFunc(self, play);

    if (!(self->movementFlags & 1)) {
        Actor_MoveForward(&self->dyna.actor);
        Actor_UpdateBgCheckInfo(play, &self->dyna.actor, 0.0f, 0.0f, 0.0f, 0x1C);
    }

    if (self->type == 5 || self->type == 6 || self->type == 7 || self->type == 8 || self->type == 13) {
        Actor_SetFocus(&self->dyna.actor, 20.0f);
    } else {
        Actor_SetFocus(&self->dyna.actor, 40.0f);
    }

    if ((((u16)self->dyna.actor.params >> 5) & 0x7F) == 0x7C && self->actionFunc == EnBox_Open &&
        *EnBox_CurFramePtr(&self->skelAnime) > sIceSmokeMinFrame &&
        (f32)self->iceSmokeTimer < 50.0f - sIceSmokeMinFrame + 45.0f) {
        Vec3f pos;
        Vector3f vel(0.0f, 1.0f, 0.0f);
        Vector3f accel(0.0f, 0.0f, 0.0f);
        f32 f0;

        FUN_00373264(&self->dyna.actor, 0x01000461);
        if (self->iceSmokeTimer >= self->unk_394[0]) {
            s32 base;
            s32 n;

            f0 = 2.0f * Rand_ZeroOne() - 1.0f;
            pos = self->dyna.actor.world.pos;
            if (self->type == 5 || self->type == 6 || self->type == 7 || self->type == 8 || self->type == 13) {
                pos.x += f0 * 10.0f * Math_SinS(self->dyna.actor.world.rot.y + 0x4000);
                pos.z += f0 * 10.0f * Math_CosS(self->dyna.actor.world.rot.y + 0x4000);
                f0 = 2.0f * Rand_ZeroOne() - 1.0f;
                vel.x = f0 * 0.8f * Math_SinS(self->dyna.actor.world.rot.y);
                vel.y = 1.8f;
                vel.z = f0 * 0.8f * Math_CosS(self->dyna.actor.world.rot.y);
            } else {
                pos.x += f0 * 20.0f * Math_SinS(self->dyna.actor.world.rot.y + 0x4000);
                pos.z += f0 * 20.0f * Math_CosS(self->dyna.actor.world.rot.y + 0x4000);
                f0 = 2.0f * Rand_ZeroOne() - 1.0f;
                vel.x = f0 * 1.6f * Math_SinS(self->dyna.actor.world.rot.y);
                vel.y = 1.8f;
                vel.z = f0 * 1.6f * Math_CosS(self->dyna.actor.world.rot.y);
            }
            FUN_00290ce0(play, &pos, (Vec3f*)&vel, (Vec3f*)&accel, 150);

            base = sIceSmokeTimerBase;
            if (self->iceSmokeTimer < sIceSmokeTimerThreshold) {
                base = base / 2;
            }
            n = (s32)(Rand_ZeroOne() * 3.0f) + base;
            if (n > 0) {
                n = (s32)(0.5f + (f32)((s32)(Rand_ZeroOne() * 3.0f) + base) * 3.0f * 0.5f);
            } else {
                n = (s32)((f32)((s32)(Rand_ZeroOne() * 3.0f) + base) * 3.0f * 0.5f - 0.5f);
            }
            self->unk_394[0] = n + self->iceSmokeTimer;
        }
        self->iceSmokeTimer++;
    }
}
#endif
GLOBAL_ASM("asm/EnBox_Update.s")

typedef struct EnBoxColor4 {
    /* 0x00 */ f32 v[4];
} EnBoxColor4;

void EnBox_Draw(Actor* thisx, PlayState* play) {
    EnBox* self = (EnBox*)thisx;

    if (self->alpha != 0) {
        EnBoxColor4 color = *(EnBoxColor4*)0x004D9284;
        void* model;

        color.v[3] = (f32)self->alpha * (1.0f / 255.0f);
        model = FUN_003687a8(self->skelAnime.unk_28);
        FUN_003589cc(model, 4);
        FUN_00358964(model, 4, &color);
        FUN_00357fd0(GET_PLAYER(play), model, &self->dyna.actor.world.pos);
        SkelAnime_DrawOpa(&self->skelAnime, (MTX34*)&self->dyna.actor.modelMtx, NULL, NULL, &self->dyna.actor, 0);
    }
}

static __inline __pure float EnBox_Fabsf(float x) {
    return __fabsf(x);
}

static inline void EnBox_SetupAction(EnBox* self, EnBoxActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void EnBox_AppearAnimation(EnBox* self, PlayState* play) {
    s32 timer;

    FUN_0036d15c(play, &play->colCtx.dyna, self->dyna.bgId);

    timer = self->appearTimer;
    if (timer < 0) {
        timer++;
    } else if (timer < 60) {
        timer++;
        self->dyna.actor.world.pos.y += 0.8333333f;
    } else if (timer < 90) {
        self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y;
        timer++;
        self->alpha += 8;
        self->appearTimer = timer;
        return;
    } else {
        EnBox_SetupAction(self, EnBox_WaitOpen);
        return;
    }
    self->appearTimer = timer;
}

void EnBox_AppearInit(EnBox* self, PlayState* play) {
    if (FUN_0036ae04() == self->dyna.actor.type || self->appearTimer != 0) {
        EnBox_SetupAction(self, EnBox_AppearAnimation);
        self->appearTimer = 0;
        Actor_Spawn(&play->actorCtx, play, ACTOR_DEMO_KANKYO, self->dyna.actor.home.pos.x,
                    self->dyna.actor.home.pos.y, self->dyna.actor.home.pos.z, 0, 0, 0, 0x11, 1);
        Audio_PlaySoundGeneral(0x010001D2, &self->dyna.actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               &DAT_0054ac24.unk_0);
    }
}

void EnBox_AppearOnRoomClear(Actor* thisx, PlayState* play) {
    EnBox* self = (EnBox*)thisx;
    s32 treasureFlag;

    treasureFlag = (u16)thisx->params & 0x1F;
    if ((u32)(treasureFlag - 20) <= 11) {
        FUN_00353484(thisx, play);
    }

    if (Flags_GetTempClear(play, self->dyna.actor.room) && !Player_InCsMode(play)) {
        u8* csBase = (u8*)0x00588958;
        if (*(s32*)(csBase + 0x4EC) > 0) {
            return;
        }
        Flags_SetClear(play, self->dyna.actor.room);
        EnBox_SetupAction(self, EnBox_AppearInit);
        OnePointCutscene_Attention(play, &self->dyna.actor, 0, 0);
        if (OnePointCutscene_CheckForCategory(play, self->dyna.actor.type)) {
            self->appearTimer = 0;
        } else {
            self->appearTimer = -45;
        }
    }
}

void EnBox_AppearOnSwitchFlag(Actor* thisx, PlayState* play) {
    EnBox* self = (EnBox*)thisx;
    s32 treasureFlag;

    treasureFlag = (u16)thisx->params & 0x1F;
    if ((u32)(treasureFlag - 20) <= 11) {
        FUN_00353484(thisx, play);
    }

    if (Flags_GetSwitch(play, self->switchFlag)) {
        OnePointCutscene_Attention(play, &self->dyna.actor, 0, 0);
        EnBox_SetupAction(self, EnBox_AppearInit);
        self->appearTimer = -45;
    }
}

#if 0
/* Draft, far (~70 words): int regs play/angle/pos-ptr permuted (ROM: r6=&world.pos r7=&pos r8=play sb=angle) and VFP const numbering (ROM s17=25 s18=65536 s19=1 s20=0). */
void EnBox_Fall(EnBox* self, PlayState* play) {
    f32 yDiff;

    self->alpha = 255;
    self->movementFlags &= ~1;
    if (self->dyna.actor.bgCheckFlags & 1) {
        s32 i;
        Vec3f pos;
        Vec3f velocity;
        Vec3f accel;

        self->movementFlags |= 2;
        if (self->movementFlags & 4) {
            self->movementFlags &= ~4;
        } else {
            self->movementFlags |= 4;
        }
        if (self->type == 3) {
            self->dyna.actor.velocity.y = -self->dyna.actor.velocity.y * 0.55f;
        } else {
            self->dyna.actor.velocity.y = -self->dyna.actor.velocity.y * 0.65f;
        }
        if (self->dyna.actor.velocity.y < 5.5f) {
            self->dyna.actor.shape.rot.z = 0;
            self->dyna.actor.world.pos.y = self->dyna.actor.floorHeight;
            EnBox_SetupAction(self, EnBox_WaitOpen);
            OnePointCutscene_EndCutscene(play, (s16)self->subCamId);
        }
        Audio_PlaySoundGeneral(0x010001AD, &self->dyna.actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               &DAT_0054ac24.unk_0);
        for (i = 0; i < 20; i++) {
            f32 randomRadius = Rand_ZeroOne() * 25.0f;
            s16 randomAngle = Rand_ZeroOne() * 65536.0f;

            pos = self->dyna.actor.world.pos;
            pos.x += Math_SinS(randomAngle) * randomRadius;
            pos.z += Math_CosS(randomAngle) * randomRadius;
            velocity.y = 1.0f;
            velocity.x = Math_SinS(randomAngle);
            velocity.z = Math_CosS(randomAngle);
            accel.x = 0.0f;
            accel.y = 0.0f;
            accel.z = 0.0f;
            FUN_0027f34c(play, &pos, &velocity, &accel, 100, 30, 15);
        }
    }
    yDiff = self->dyna.actor.world.pos.y - self->dyna.actor.floorHeight;
    if (self->movementFlags & 4) {
        self->dyna.actor.shape.rot.z = yDiff * 50.0f;
    } else {
        self->dyna.actor.shape.rot.z = -yDiff * 50.0f;
    }
}
#endif
GLOBAL_ASM("asm/EnBox_Fall.s")

void EnBox_FallOnSwitchFlag(EnBox* self, PlayState* play) {
    s32 treasureFlag;

    treasureFlag = (u16)self->dyna.actor.params & 0x1F;
    if ((u32)(treasureFlag - 20) <= 11) {
        FUN_00353484(&self->dyna.actor, play);
    }

    if (self->appearTimer >= 0) {
        EnBox_SetupAction(self, EnBox_Fall);
        self->subCamId = OnePointCutscene_Init(play, 4500, 9999, &self->dyna.actor, 0);
        FUN_0036d15c(play, &play->colCtx.dyna, self->dyna.bgId);
    } else if (self->appearTimer > -18) {
        self->appearTimer++;
    } else if (Flags_GetSwitch(play, self->switchFlag)) {
        self->appearTimer++;
    }
}
GLOBAL_ASM("../split/asm/FUN_001824b8.s")
void EnBox_Open(EnBox* self, PlayState* play) {
    self->dyna.actor.flags &= ~0x80;

    if (FUN_003731e0(&self->skelAnime)) {
        if (self->unk_38C > 0) {
            if (self->unk_38C < 120) {
                self->unk_38C++;
            } else {
                Math_StepToF(&self->unk_248, 0.0f, 0.05f);
            }
        } else {
            if (self->unk_38C > -120) {
                self->unk_38C--;
            } else {
                Math_StepToF(&self->unk_248, 0.0f, 0.05f);
            }
        }
    } else {
        u32 sfxId = 0;

        if (Animation_OnFrameImpl(&self->skelAnime, 30.0f, 1.0f)) {
            sfxId = 0x01000176;
        } else if (Animation_OnFrameImpl(&self->skelAnime, 90.0f, 1.0f)) {
            sfxId = 0x01000177;
        }

        if (sfxId != 0) {
            Audio_PlaySoundGeneral(sfxId, &self->dyna.actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                                   &DAT_0054ac24.unk_0);
        }
    }
}

#if 0
/* Draft, 16 words: ctx ptr sl/sb swap with max=4 (ROM sl=&actorCtx, sb=max) and list-loop count/i regs (ROM i=r0,count=r2). Needs FUN_00387fd0 and CheckForCategory in own TUs (done). */
void EnBox_WaitOpen(EnBox* self, PlayState* play) {
    f32 frameCount;
    s32 anim;
    s32 linkAge;

    self->alpha = 255;
    self->movementFlags |= 1;
    if (self->unk_38C != 0) {
        if (gSaveContext.masterQuestFlag == 1 && *(u16*)((u8*)play + 0x104) == 2 && self->dyna.actor.room == 0xE &&
            5 != ((self->dyna.actor.params >> 12) & 0xF)) {
            s32 count;
            s32 i;
            s32 max = 4;
            Actor** list = FUN_0033de14(max * sizeof(Actor*));

            count = FUN_00360084(&play->actorCtx, 0x15F, 1, list, max);
            for (i = 0; i < count; i++) {
                Actor* other = list[i];

                if (*(s32*)((u8*)other + 0x1FC) != 0 && *(f32*)((u8*)other + 0x1C4) >= -1250.0f) {
                    self->unk_38C = 0;
                    FUN_0033ddd4(list);
                    return;
                }
            }
            FUN_0033ddd4(list);
        }
        linkAge = gSaveContext.linkAge;
        anim = sAnimations[(self->unk_38C < 0 ? 2 : 0) + linkAge];
        frameCount = FUN_003603c0(&self->skelAnime, anim);
        Animation_Change_00375c08(&self->skelAnime, anim, 1.5f, 0.0f, frameCount, 2, 0.0f);
        EnBox_SetupAction(self, EnBox_Open);
        if (self->unk_38C > 0) {
            if (self->type != 5 && self->type != 6 && self->type != 7 && self->type != 8 && self->type != 13) {
                Actor_SpawnAsChild(&play->actorCtx, &self->dyna.actor, play, 0xAA, self->dyna.actor.world.pos.x,
                                   self->dyna.actor.world.pos.y, self->dyna.actor.world.pos.z,
                                   self->dyna.actor.shape.rot.x, self->dyna.actor.shape.rot.y,
                                   self->dyna.actor.shape.rot.z, -1);
                FUN_0035c528(0x01000599);
            }
        }
        Flags_SetTreasure(play, (u16)self->dyna.actor.params & 0x1F);
    } else {
        Vec3f sp4C;
        Player* player;
        f32 minZ;

        player = GET_PLAYER(play);
        FUN_0036c5d8(&self->dyna.actor, &sp4C, &player->actor.world.pos);
        minZ = -50.0f;
        if (gSaveContext.masterQuestFlag != 0) {
            if (*(u16*)((u8*)play + 0x104) == 4 && self->dyna.actor.room == 5) {
                minZ = -44.0f;
            }
        } else if (*(u16*)((u8*)play + 0x104) == 5 && *(u8*)((u8*)play + 0x4C30) == 4) {
            Actor* found;

            if (FUN_00360084(&play->actorCtx, 0x15F, 1, &found, 1) > 0) {
                if (FUN_00387fd0(found) == 0) {
                    minZ = sp4C.z + 10.0f;
                }
            }
        }
        if (sp4C.z > minZ && sp4C.z < 0.0f && EnBox_Fabsf(sp4C.y) < 10.0f && EnBox_Fabsf(sp4C.x) < 20.0f &&
            FUN_0036f2f8(&self->dyna.actor, 0x3000, play)) {
            FUN_00346778(&self->dyna.actor, play, -(((u16)self->dyna.actor.params >> 5) & 0x7F));
        }
        if (Flags_GetTreasure(play, (u16)self->dyna.actor.params & 0x1F)) {
            EnBox_SetupAction(self, EnBox_Open);
        }
    }
}
#endif
GLOBAL_ASM("asm/EnBox_WaitOpen.s")

GLOBAL_ASM("../split/asm/FUN_00290d3c.s")

void FUN_0021ded8(EnBox* self, PlayState* play) {
    self->appearTimer--;
    if (self->appearTimer > 0) {
        return;
    }
    EnBox_SetupAction(self, EnBox_AppearAnimation);
    self->appearTimer = 0;
    Actor_Spawn(&play->actorCtx, play, ACTOR_DEMO_KANKYO, self->dyna.actor.home.pos.x, self->dyna.actor.home.pos.y,
                self->dyna.actor.home.pos.z, 0, 0, 0, 0x11, 1);
    Audio_PlaySoundGeneral(0x010001D2, &self->dyna.actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                           &DAT_0054ac24.unk_0);
}

#if 0
/* Draft, 4 words (compare const lands in r0 instead of r1; stateFlags2 RMW). GLOBAL_ASM stays in functions_1C0000s.cpp. */
void FUN_001eae0c(EnBox* self, PlayState* play2) {
    PlayState* play = play2;
    Player* player = GET_PLAYER(play);
    s32 treasureFlag = (u16)self->dyna.actor.params & 0x1F;
    f32 dx;
    f32 dy;
    f32 dz;

    if ((u32)(treasureFlag - 20) <= 11) {
        FUN_00353484(&self->dyna.actor, play);
    }

    dx = self->dyna.actor.world.pos.x - player->actor.world.pos.x;
    dy = self->dyna.actor.world.pos.y - player->actor.world.pos.y;
    dz = self->dyna.actor.world.pos.z - player->actor.world.pos.z;
    if (dx * dx + dy * dy + dz * dz > 22500.0f) {
        self->ocarinaState = 0;
        return;
    }

    if (self->ocarinaState == 0) {
        if (!(player->stateFlags2 & 0x1000000)) {
            Player* p2 = player;
            p2->stateFlags2 |= 0x800000;
            return;
        }
        self->ocarinaState = 1;
    }

    if (self->ocarinaState == 1) {
        FUN_0037073c(play, 1);
        self->ocarinaState = 2;
    } else if (self->ocarinaState == 2 && *(u16*)((u8*)play + 0x2B7E) == 4) {
        u16 lastSong = *(u16*)((u8*)play + 0x2B7C);

        if ((lastSong == 8 && self->type == 9) || (lastSong == 9 && self->type == 10)) {
            self->dyna.actor.flags &= ~0x2000000;
            EnBox_SetupAction(self, EnBox_AppearInit);
            OnePointCutscene_Attention(play, &self->dyna.actor, 0, 0);
            self->appearTimer = 0;
            self->ocarinaState = 0;
            return;
        }
        self->ocarinaState = 0;
    } else {
        self->ocarinaState = 0;
    }
}
#endif



#if 0
/* Draft, 38 words with real statics below (play/zero r5<->r6 swap); with EQU externs + EnBox_sInitChain EQU the head matches but the ROM derives sInitChain as sAnimations-0xC (adjacent statics -> needs data tail split). */
void EnBox_Init(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnBox* self = (EnBox*)thisx;
    CollisionHeader* colHeader = NULL;
    s32 anim;
    void* cmb;
    f32 animFrameStart;
    f32 endFrame;

    thisx->unk_19A = 1;
    self->zar = &play->GetObjectStatus(thisx->objBankIndex)->zarInfo;
    anim = sAnimations[gSaveContext.linkAge];
    Actor_ProcessInitChain(thisx, sInitChain);
    DynaPolyActor_Init(&self->dyna, 0);
    ReadCollisionHeaderFromZsi(self->zar->FUN_003532c0(0), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
    FUN_0034f6bc(play, &play->colCtx.dyna, self->dyna.bgId);

    self->movementFlags = 0;
    self->type = (thisx->params >> 12) & 0xF;
    self->switchFlag = thisx->world.rot.z;
    thisx->gravity = -5.5f;
    thisx->minVelocityY = -50.0f;
    self->iceSmokeTimer = 0;
    self->unk_394[0] = 0;
    self->ocarinaState = 0;

    if (Flags_GetTreasure(play, (u16)thisx->params & 0x1F)) {
        self->alpha = 255;
        self->iceSmokeTimer = (u32)(45.0f - sIceSmokeMinFrame + 50.0f);
        EnBox_SetupAction(self, EnBox_Open);
        self->movementFlags |= 0x10;
        thisx->flags |= 0x10;
        if (gSaveContext.masterQuestFlag == 1 && *(u16*)((u8*)play2 + 0x104) == 1 && thisx->room == 0xE) {
            self->movementFlags |= 1;
        }
    } else if ((self->type == 3 || self->type == 8) && !Flags_GetSwitch(play, self->switchFlag)) {
        FUN_0036b940(play, &play->colCtx.dyna, self->dyna.bgId);
        if (Rand_ZeroOne() < 0.5f) {
            self->movementFlags |= 4;
        }
        EnBox_SetupAction(self, EnBox_FallOnSwitchFlag);
        self->appearTimer = -18;
        self->movementFlags |= 1;
        self->alpha = 0;
        thisx->flags |= 0x10;
    } else if ((self->type == 1 || self->type == 7) && !Flags_GetClear(play, thisx->room)) {
        EnBox_SetupAction(self, (EnBoxActionFunc)EnBox_AppearOnRoomClear);
        FUN_0036b940(play, &play->colCtx.dyna, self->dyna.bgId);
        thisx->world.pos.y = thisx->home.pos.y - 50.0f;
        self->movementFlags |= 1;
        self->alpha = 0;
        thisx->flags |= 0x10;
    } else if (self->type == 9 || self->type == 10) {
        EnBox_SetupAction(self, FUN_001eae0c);
        thisx->flags |= 0x2000000;
        FUN_0036b940(play, &play->colCtx.dyna, self->dyna.bgId);
        thisx->world.pos.y = thisx->home.pos.y - 50.0f;
        self->movementFlags |= 1;
        self->alpha = 0;
        thisx->flags |= 0x10;
    } else if (self->type == 11 && !Flags_GetSwitch(play, self->switchFlag)) {
        EnBox_SetupAction(self, (EnBoxActionFunc)EnBox_AppearOnSwitchFlag);
        FUN_0036b940(play, &play->colCtx.dyna, self->dyna.bgId);
        thisx->world.pos.y = thisx->home.pos.y - 50.0f;
        self->movementFlags |= 1;
        self->alpha = 0;
        thisx->flags |= 0x10;
    } else if (self->type == 12 || self->type == 13) {
        EnBox_SetupAction(self, FUN_0021ded8);
        FUN_0036b940(play, &play->colCtx.dyna, self->dyna.bgId);
        thisx->world.pos.y = thisx->home.pos.y - 50.0f;
        self->movementFlags |= 1;
        self->alpha = 0;
        self->appearTimer = 60;
    } else {
        if (self->type == 4 || self->type == 6) {
            thisx->flags |= 0x80;
        }
        EnBox_SetupAction(self, EnBox_WaitOpen);
        self->movementFlags |= 1;
        self->movementFlags |= 0x10;
        thisx->flags |= 0x10;
    }

    thisx->world.rot.y += 0x8000;
    thisx->home.rot.z = thisx->world.rot.z = thisx->shape.rot.z = 0;

    cmb = ZAR_GetCMBByIndex(self->zar, 1);
    FUN_00358ea8(self->zar, play, &self->skelAnime, cmb, thisx->unk_178, anim, self->jointTable, self->morphTable, 3);
    animFrameStart = 0.0f;
    endFrame = (f32)FUN_003603c0(&self->skelAnime, anim);
    if (Flags_GetTreasure(play, (u16)thisx->params & 0x1F)) {
        animFrameStart = endFrame;
    }
    Animation_Change_00375c08(&self->skelAnime, anim, 1.5f, animFrameStart, endFrame, 2, 0.0f);

    if (self->type == 2) {
        FUN_0037266c(self->skelAnime.unk_28, 3);
        FUN_0037266c(self->skelAnime.unk_28, 1);
        FUN_0036932c(self->skelAnime.unk_28, 2);
        FUN_0036932c(self->skelAnime.unk_28, 0);
    } else {
        FUN_0037266c(self->skelAnime.unk_28, 2);
        FUN_0037266c(self->skelAnime.unk_28, 0);
        FUN_0036932c(self->skelAnime.unk_28, 3);
        FUN_0036932c(self->skelAnime.unk_28, 1);
    }

    if (self->type == 5 || self->type == 6 || self->type == 7 || self->type == 8 || self->type == 13) {
        Actor_SetScale(thisx, 0.005f);
        Actor_SetFocus(thisx, 20.0f);
    } else {
        Actor_SetScale(thisx, 0.01f);
        Actor_SetFocus(thisx, 40.0f);
    }
}
#endif

}
