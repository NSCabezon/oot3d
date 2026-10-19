extern "C" {
#include "z_bg_po_event.hpp"

#define FLAGS 0x00400000
#define CAM_ID_MAIN 0
#define PLAYER_STATE2_4 0x10
#define TIMER_STATE_STOP 10

extern u8 sPuzzleState;
extern InitChainEntry sBgPoEventInitChain[];
extern s16 sBgPoEventPaintingPosX[];
extern s16 sBgPoEventPaintingPosY[];
extern s16 sBgPoEventPaintingPosZ[];
extern s16 sBgPoEventBlockPosX[];
extern s16 sBgPoEventBlockPosZ[];
typedef struct {
    /* 0x00 */ u8 pad[0x18];
    /* 0x18 */ Vec3f vtx[3];
} BgPoEventTrisElemInit; // size = 0x3C
typedef struct {
    /* 0x00 */ u8 pad[8];
    /* 0x08 */ s32 count;
    /* 0x0C */ BgPoEventTrisElemInit* elements;
} BgPoEventTrisInit;
extern BgPoEventTrisInit sBgPoEventTrisInit;
f32 BgCheck_EntityRaycastFloor4(CollisionContext* colCtx, CollisionPoly** outPoly, s32* outBgId, Actor* actor, Vec3f* pos);
void Collider_InitTris(PlayState* play, ColliderTris* collider);
void Collider_SetTris(PlayState* play, ColliderTris* collider, Actor* actor, void* src, ColliderTrisElement* elements);
void Collider_SetTrisVertices(ColliderTris* collider, s32 index, Vec3f* a, Vec3f* b, Vec3f* c);
extern Vec3f sBgPoEventZeroVec;
extern u32 sBgPoEventDisplayLists[];
void FUN_00357878(Actor* actor, Vec3f* pos, Vec3f* scale, s32 alpha, PlayState* play);
void EffectSsDeadDb_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep, s16 primR,
                          s16 primG, s16 primB, s16 primA, s16 envR, s16 envG, s16 envB, s32 unk, s32 life, s32 playSfx);

void BgPoEvent_Init(Actor* thisx, PlayState* play);
void BgPoEvent_Destroy(Actor* thisx, PlayState* play);
void BgPoEvent_Update(Actor* thisx, PlayState* play);
void BgPoEvent_Draw(Actor* thisx, PlayState* play);

void BgPoEvent_AmyWait(BgPoEvent* self, PlayState* play);
void BgPoEvent_AmyPuzzle(BgPoEvent* self, PlayState* play);
void BgPoEvent_BlockShake(BgPoEvent* self, PlayState* play);
void BgPoEvent_BlockSolved(BgPoEvent* self, PlayState* play);
void BgPoEvent_PaintingAppear(BgPoEvent* self, PlayState* play);
void BgPoEvent_PaintingEmpty(BgPoEvent* self, PlayState* play);
void BgPoEvent_PaintingPresent(BgPoEvent* self, PlayState* play);
static s32 BgPoEvent_NextPainting(BgPoEvent* self);
void BgPoEvent_PaintingBurn(BgPoEvent* self, PlayState* play);
void BgPoEvent_PaintingVanish(BgPoEvent* self, PlayState* play);
void BgPoEvent_BlockWait(BgPoEvent* self, PlayState* play);
void BgPoEvent_BlockFall(BgPoEvent* self, PlayState* play);
void BgPoEvent_BlockIdle(BgPoEvent* self, PlayState* play);
void BgPoEvent_BlockReset(BgPoEvent* self, PlayState* play);
void BgPoEvent_BlockPush(BgPoEvent* self, PlayState* play);
s32 Player_InCsMode(PlayState* play);
void FUN_00298de4(BgPoEvent* self, PlayState* play);
void FUN_00346850(BgPoEvent* self);
void FUN_0036fca8(Actor* actor, PlayState* play, s32 quakeIndex, s32 rumbleIndex);
s32 FUN_0036d288(PlayState* play, DynaPolyActor* dyna, s16 arg2, s16 arg3, s32 arg4);

void OnePointCutscene_Init(PlayState* play, s32 csId, s32 duration, Actor* actor, s32 camId);
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);
void Collider_DestroyTris(PlayState* play, ColliderTris* tris);
s32 Math_ScaledStepToS(s16* value, s16 target, s16 step);
s32 FUN_0033bd6c(Player* player);
s32 FUN_0036f2f8(Actor* actor, s32 angle, PlayState* play);

GLOBAL_ASM("../src/game/actors/Bg_Po_Event/data_syms.s")
GLOBAL_ASM("data/z_Bg_Po_Event.data.s")

ActorInit Bg_Po_Event_InitVars = {
    ACTOR_BG_PO_EVENT,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_PO_SISTERS,
    sizeof(BgPoEvent),
    (ActorFunc)BgPoEvent_Init,
    (ActorFunc)BgPoEvent_Destroy,
    (ActorFunc)BgPoEvent_Update,
    (ActorFunc)BgPoEvent_Draw,
};

// NON_MATCHING draft: 19 words off (regs: ROM keeps play in r8 / &sp9C in r7, ours swapped; Spawn mov r2/r1 order;
// sTrisInit passed as (u8*)&sPuzzleState + 0x24 and sBlocksAtRest via raw play+0x7FC7 are debt shapes).
#if 0
static void BgPoEvent_InitPaintings(BgPoEvent* self, PlayState* play) {
    BgPoEventTrisElemInit* elementInit;
    s32 i1;
    s32 i2;
    Vec3f sp9C[3];
    f32 coss;
    f32 sins;
    f32 scaleY;
    s32 phi_t2;
    Actor* newPainting;
    PlayState* play2 = play;

    sins = Math_SinS(self->dyna.actor.shape.rot.y);
    coss = Math_CosS(self->dyna.actor.shape.rot.y);
    if (self->type == 4) {
        sins *= 2.4f;
        scaleY = 1.818f;
        coss *= 2.4f;
    } else {
        scaleY = 1.0f;
    }
    for (i1 = 0; i1 < sBgPoEventTrisInit.count; i1++) {
        elementInit = &sBgPoEventTrisInit.elements[i1];
        for (i2 = 0; i2 < 3; i2++) {
            sp9C[i2].x = (elementInit->vtx[i2].z * sins + coss * elementInit->vtx[i2].x) + self->dyna.actor.home.pos.x;
            sp9C[i2].y = self->dyna.actor.home.pos.y + scaleY * elementInit->vtx[i2].y;
            sp9C[i2].z = self->dyna.actor.home.pos.z + coss * elementInit->vtx[i2].z - sins * elementInit->vtx[i2].x;
        }
        Collider_SetTrisVertices(&self->collider, i1, &sp9C[0], &sp9C[1], &sp9C[2]);
    }
    if ((self->type != 4) && (self->index != 2)) {
        phi_t2 = (self->type == 2) ? self->index : self->index + 2;
        newPainting = Actor_SpawnAsChild(&play2->actorCtx, &self->dyna.actor, play2, ACTOR_BG_PO_EVENT,
                                         sBgPoEventPaintingPosX[phi_t2], sBgPoEventPaintingPosY[self->index],
                                         sBgPoEventPaintingPosZ[self->index], 0, self->dyna.actor.shape.rot.y + 0x8000,
                                         0, ((self->index + 1) << 0xC) + (self->type << 8) + self->dyna.actor.params);
        if (newPainting == NULL) {
            Actor_Kill(&self->dyna.actor);
            return;
        }
        if (self->index == 0) {
            if (self->dyna.actor.child->child == NULL) {
                Actor_Kill(&self->dyna.actor);
                return;
            }
            self->dyna.actor.parent = self->dyna.actor.child->child;
            self->dyna.actor.child->child->child = &self->dyna.actor;
        }
    }
    self->timer = 0;
    *((u8*)play + 0x7FC7) = 0; // sBlocksAtRest (raw PlayState offset)
    if (self->type == 4) {
        sPuzzleState = 0;
        self->actionFunc = BgPoEvent_AmyWait;
    } else {
        sPuzzleState = (s32)(Rand_ZeroOne() * 3.0f) % 3;
        self->actionFunc = BgPoEvent_PaintingEmpty;
    }
}

static void BgPoEvent_InitBlocks(BgPoEvent* self, PlayState* play) {
    Actor* newBlock;
    CollisionHeader* colHeader = NULL;
    s32 bgId;

    self->dyna.actor.flags |= 0x30;
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 0), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);
    if ((self->type == 0) && (self->index != 3)) {
        PlayState* play2 = play;
        newBlock = Actor_SpawnAsChild(&play2->actorCtx, &self->dyna.actor, play2, ACTOR_BG_PO_EVENT,
                                      sBgPoEventBlockPosX[self->index], self->dyna.actor.world.pos.y,
                                      sBgPoEventBlockPosZ[self->index], 0, self->dyna.actor.shape.rot.y,
                                      self->dyna.actor.shape.rot.z - 0x4000,
                                      ((self->index + 1) << 0xC) + (self->type << 8) + self->dyna.actor.params);
        if (newBlock == NULL) {
            Actor_Kill(&self->dyna.actor);
            return;
        }
        if (self->index == 0) {
            if (self->dyna.actor.child->child == NULL) {
                Actor_Kill(&self->dyna.actor);
                return;
            }
            if (self->dyna.actor.child->child->child == NULL) {
                Actor_Kill(&self->dyna.actor);
                Actor_Kill(self->dyna.actor.child);
                return;
            }
            self->dyna.actor.parent = self->dyna.actor.child->child->child;
            self->dyna.actor.child->child->child->child = &self->dyna.actor;
        }
    }
    self->dyna.actor.world.pos.y = 833.0f;
    self->dyna.actor.floorHeight = BgCheck_EntityRaycastFloor4(&play->colCtx, &self->dyna.actor.floorPoly, &bgId,
                                                               &self->dyna.actor, &self->dyna.actor.world.pos);
    self->actionFunc = BgPoEvent_BlockWait;
}

void BgPoEvent_Init(Actor* thisx, PlayState* play) {
    BgPoEvent* self = (BgPoEvent*)thisx;

    Actor_ProcessInitChain(thisx, sBgPoEventInitChain);
    self->type = (((u16)thisx->params) >> 8) & 0xF;
    self->index = (thisx->params >> 12) & 0xF;
    thisx->params &= 0x3F;
    sPuzzleState = 0;
    FUN_00372f38(thisx, play, &self->unk_2A0[0], 4, &self->unk_2A0[1], 5, &self->unk_2A0[2], 1, &self->unk_2A0[3], 2,
                 &self->unk_2A0[4], 3, 0);

    if (self->type > 1) {
        Collider_InitTris(play, &self->collider);
        Collider_SetTris(play, &self->collider, thisx, (u8*)&sPuzzleState + 0x24, (ColliderTrisElement*)self->colliderElements);
        if (Flags_GetSwitch(play, thisx->params)) {
            Actor_Kill(thisx);
        } else {
            BgPoEvent_InitPaintings(self, play);
        }
    } else {
        DynaPolyActor_Init(&self->dyna, 0);
        if (Flags_GetSwitch(play, thisx->params)) {
            Actor_Kill(thisx);
        } else {
            BgPoEvent_InitBlocks(self, play);
        }
    }
}
#endif
GLOBAL_ASM("asm/BgPoEvent_Init.s")

void BgPoEvent_Destroy(Actor* thisx, PlayState* play) {
    BgPoEvent* self = (BgPoEvent*)thisx;

    if (self->type > 1) {
        Collider_DestroyTris(play, &self->collider);
    } else {
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
        if ((self->type == 1) && (gSaveContext.timer1Value > 0)) {
            gSaveContext.timer1State = TIMER_STATE_STOP;
        }
    }
    FUN_00350f34(thisx, &self->unk_2A0[0], &self->unk_2A0[1], &self->unk_2A0[2], &self->unk_2A0[3],
                 &self->unk_2A0[4], 0);
}

void BgPoEvent_Update(Actor* thisx, PlayState* play) {
    BgPoEvent* self = (BgPoEvent*)thisx;

    self->actionFunc(self, play);
    if ((self->actionFunc == BgPoEvent_AmyWait) || (self->actionFunc == BgPoEvent_PaintingPresent)) {
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
    }
}

void BgPoEvent_Draw(Actor* thisx, PlayState* play) {
    BgPoEvent* self = (BgPoEvent*)thisx;
    Vec3f sp58;
    Vec3f sp4C;
    MTX34 mtx;
    u8 type;
    u8 alpha;
    f32 sp48;

    MTX34CopyAsm(&mtx, &thisx->modelMtx);
    type = self->type;
    if ((type == 3) || (type == 2)) {
        if (self->actionFunc == BgPoEvent_PaintingEmpty) {
            alpha = 255;
        } else if (self->actionFunc == BgPoEvent_PaintingPresent) {
            alpha = 0;
        } else {
            alpha = self->timer;
        }
        FUN_003695cc((SkeletonAnimationModel*)self->unk_2A0[type], (s8)sBgPoEventDisplayLists[type], 4, 0, 1.0f, 1.0f,
                     1.0f, (f32)(255 - alpha) * (1.0f / 255.0f));
    }
    if (self->unk_2A0[type] != NULL) {
        ((SkeletonAnimationModel*)self->unk_2A0[type])->SetUnkAC(1);
        ((SkeletonAnimationModel*)self->unk_2A0[type])->FUN_003721e0(&mtx);
        ((SkeletonAnimationModel*)self->unk_2A0[type])->FUN_00372170(0);
    }
    if ((type == 0) || (type == 1)) {
        sp48 = (833.0f - thisx->world.pos.y) * 0.0025f;
        if (!(sp48 > 1.0f)) {
            sp58.x = thisx->world.pos.x;
            sp58.y = thisx->world.pos.y - 30.0f;
            sp58.z = thisx->world.pos.z;
            sp4C.x = sp4C.z = (sp48 * 0.3f) + 0.4f;
            sp4C.y = 1.0f;
            FUN_00357878(thisx, &sp58, &sp4C, (u8)(155.0f + sp48 * 100.0f), play);
        }
    }
}

void BgPoEvent_AmyWait(BgPoEvent* self, PlayState* play) {
    if (self->collider.base.acFlags & AC_HIT) {
        sPuzzleState |= 0x20;
        self->timer = 8;
        Actor_SetColorFilter(&self->dyna.actor, 0x400000, 255, 0, 5);
        Audio_PlayActorSound2(&self->dyna.actor, 0x0100035F);
        self->actionFunc = FUN_00298de4;
    }
}

void FUN_00298de4(BgPoEvent* self, PlayState* play) {
    Vec3f pos;

    if (sPuzzleState == 0xF) {
        pos.x = self->dyna.actor.world.pos.x - 5.0f;
        pos.y = Rand_CenteredFloat(120.0f) + self->dyna.actor.world.pos.y;
        pos.z = Rand_CenteredFloat(120.0f) + self->dyna.actor.world.pos.z;
        EffectSsDeadDb_Spawn(play, &pos, &sBgPoEventZeroVec, &sBgPoEventZeroVec, 170, 0, 200, 255, 100, 170, 0, 255,
                             0, 1, 11, 1);
    } else if (sPuzzleState == 0x20) {
        Actor_Kill(&self->dyna.actor);
    } else {
        DECR(self->timer);
    }
}

void BgPoEvent_BlockFall(BgPoEvent* self, PlayState* play) {
    self->dyna.actor.velocity.y += 2.0f / 3.0f;
    if (Math_StepToF(&self->dyna.actor.world.pos.y, 433.0f, self->dyna.actor.velocity.y)) {
        self->dyna.actor.velocity.y = 0.0f;
        self->dyna.actor.flags &= ~0x20;
        *((u8*)play + 0x7FC7) += 1; // sBlocksAtRest (raw PlayState offset)
        if (self->type != 1) {
            FUN_00346850(self);
        } else {
            Audio_PlayActorSound2(&self->dyna.actor, 0x01000174);
            FUN_0036fca8(&self->dyna.actor, play, 5, 5);
            FUN_0035239c(self->timer);
            if (*(s32*)((u8*)play + 0x7FCC) == 0) { // firstFall (raw PlayState offset)
                *(s32*)((u8*)play + 0x7FCC) = 1;
            } else {
                FUN_0036e980(play, &GET_PLAYER(play)->actor, 7);
            }
        }
        self->direction = 0;
        self->actionFunc = BgPoEvent_BlockIdle;
    }
}

void BgPoEvent_BlockIdle(BgPoEvent* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Actor* amy;

    if (sPuzzleState == 0xF) {
        self->actionFunc = BgPoEvent_BlockSolved;
        if ((self->type == 0) && (self->index == 0)) {
            amy = Actor_Spawn(&play->actorCtx, play, ACTOR_EN_PO_SISTERS, self->dyna.actor.world.pos.x + 30.0f,
                              self->dyna.actor.world.pos.y - 30.0f, self->dyna.actor.world.pos.z + 30.0f, 0,
                              self->dyna.actor.shape.rot.y, 0, self->dyna.actor.params + 0x300, 1);
            if (amy != NULL) {
                OnePointCutscene_Init(play, 3170, 30, amy, CAM_ID_MAIN);
            }
            FUN_00372244(&play->unk_5FCC, 0x1E, 0x01000487);
            gSaveContext.timer1State = TIMER_STATE_STOP;
        }
    } else {
        if ((gSaveContext.timer1Value == 0) && (*((u8*)play + 0x7FC7) == 5)) {
            player->stateFlags2 &= ~PLAYER_STATE2_4;
            sPuzzleState = 0x10;
            *((u8*)play + 0x7FC7) = 0;
        }
        if ((sPuzzleState == 0x40) || ((sPuzzleState == 0x10) && !Player_InCsMode(play))) {
            self->dyna.actor.world.rot.z = self->dyna.actor.shape.rot.z;
            self->actionFunc = BgPoEvent_BlockReset;
            if (sPuzzleState == 0x10) {
                sPuzzleState = 0x40;
                Audio_PlayActorSound2(&self->dyna.actor, 0x010001FB);
                FUN_0036e980(play, &player->actor, 8);
            }
        } else if (self->dyna.unk_1A8 != 0.0f) {
            if (self->direction == 0) {
                if (FUN_0036d288(play, &self->dyna, 0x1E, 0x32, -0x14) != 0) {
                    *((u8*)play + 0x7FC7) -= 1;
                    self->direction = (self->dyna.unk_1A8 >= 0.0f) ? 1.0f : -1.0f;
                    self->actionFunc = BgPoEvent_BlockPush;
                } else {
                    player->stateFlags2 &= ~PLAYER_STATE2_4;
                    self->dyna.unk_1A8 = 0.0f;
                }
            } else {
                player->stateFlags2 &= ~PLAYER_STATE2_4;
                self->dyna.unk_1A8 = 0.0f;
                DECR(self->direction);
            }
        } else {
            self->direction = 0;
        }
    }
}

void BgPoEvent_BlockPush(BgPoEvent* self, PlayState* play) {
    f32 displacement;
    s32 blockStop;
    Player* player = GET_PLAYER(play);

    self->dyna.actor.speedXZ += 0.1f;
    self->dyna.actor.speedXZ = CLAMP_MAX(self->dyna.actor.speedXZ, 2.0f);
    blockStop = Math_StepToF((f32*)((u8*)play + 0x7FC8), 20.0f, self->dyna.actor.speedXZ); // static blockPushDist
    displacement = *(f32*)((u8*)play + 0x7FC8) * self->direction;
    self->dyna.actor.world.pos.x = (displacement * Math_SinS(self->dyna.unk_1B0)) + self->dyna.actor.home.pos.x;
    self->dyna.actor.world.pos.z = (displacement * Math_CosS(self->dyna.unk_1B0)) + self->dyna.actor.home.pos.z;
    if (blockStop) {
        player->stateFlags2 &= ~PLAYER_STATE2_4;
        if ((self->dyna.unk_1A8 > 0.0f) && (FUN_0036d288(play, &self->dyna, 0x1E, 0x32, -0x14) == 0)) {
            Audio_PlayActorSound2(&self->dyna.actor, 0x0100018C);
        }
        self->dyna.actor.home.pos.x = self->dyna.actor.world.pos.x;
        self->dyna.actor.home.pos.z = self->dyna.actor.world.pos.z;
        self->dyna.unk_1A8 = 0.0f;
        *(f32*)((u8*)play + 0x7FC8) = 0.0f;
        self->dyna.actor.speedXZ = 0.0f;
        self->direction = 5;
        *((u8*)play + 0x7FC7) += 1; // sBlocksAtRest (raw PlayState offset)
        self->actionFunc = BgPoEvent_BlockIdle;
        if (self->type == 1) {
            return;
        }
        FUN_00346850(self);
        FUN_00346850((BgPoEvent*)self->dyna.actor.parent);
    }
    FUN_00373264(&self->dyna.actor, 0x01000161);
}

void BgPoEvent_BlockReset(BgPoEvent* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (self->dyna.unk_1A8 != 0.0f) {
        player->stateFlags2 &= ~PLAYER_STATE2_4;
        self->dyna.unk_1A8 = 0.0f;
    }
    if (Math_StepToF(&self->dyna.actor.world.pos.y, 493.0f, 1.0f) &&
        Math_ScaledStepToS(&self->dyna.actor.shape.rot.z, self->dyna.actor.world.rot.z - 0x4000, 0x400)) {

        self->index = (self->index + 1) % 4;
        self->actionFunc = BgPoEvent_BlockFall;
        sPuzzleState = 0;
        if (self->type == 1) {
            self->timer += 10;
            self->timer = CLAMP_MAX(self->timer, 120);
        }
    }
}

void BgPoEvent_BlockShake(BgPoEvent* self, PlayState* play) {
    DECR(self->timer);
    if (self->timer < 23) {
        self->dyna.actor.world.pos.x = self->dyna.actor.home.pos.x + 2.0f * ((self->timer % 3) - 1);
        if (!(self->timer % 6)) {
            Audio_PlayActorSound2(&self->dyna.actor, 0x0100018F);
        }
    }
    if (self->timer == 0) {
        self->dyna.actor.world.pos.x = self->dyna.actor.home.pos.x;
        sPuzzleState = 0;
        self->timer = 60;
        self->actionFunc = BgPoEvent_BlockFall;
    }
}

void BgPoEvent_BlockSolved(BgPoEvent* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (self->dyna.unk_1A8 != 0.0f) {
        player->stateFlags2 &= ~PLAYER_STATE2_4;
    }
    if (Math_StepToF(&self->dyna.actor.world.pos.y, 369.0f, 2.0f)) {
        sPuzzleState = 0x20;
        Actor_Kill(&self->dyna.actor);
    }
}

void BgPoEvent_BlockWait(BgPoEvent* self, PlayState* play) {
    self->dyna.actor.world.pos.y = 833.0f;
    if (sPuzzleState == 0x3F) {
        if (self->type == 1) {
            OnePointCutscene_Init(play, 3150, 65, NULL, CAM_ID_MAIN);
        }
        self->timer = 68;
        self->actionFunc = BgPoEvent_BlockShake;
    } else if (self->dyna.actor.xzDistToPlayer > 50.0f) {
        if (self->type != 1) {
            sPuzzleState |= (1 << self->index);
        } else {
            sPuzzleState |= 0x10;
        }
    } else if (self->type != 1) {
        sPuzzleState &= ~(1 << self->index);
    } else {
        sPuzzleState &= ~0x10;
    }
}
GLOBAL_ASM("../split/asm/FUN_001976E4.s")

void BgPoEvent_PaintingAppear(BgPoEvent* self, PlayState* play) {
    self->timer -= 30;
    if (self->timer <= 0) {
        self->timer = 1500;
        self->actionFunc = BgPoEvent_PaintingPresent;
    }
}

void BgPoEvent_PaintingBurn(BgPoEvent* self, PlayState* play) {
    Vec3f sp54;

    self->timer--;
    sp54.x = (Math_SinS(self->dyna.actor.shape.rot.y) * 5.0f) + self->dyna.actor.world.pos.x;
    sp54.y = Rand_CenteredFloat(66.0f) + self->dyna.actor.world.pos.y;
    sp54.z = Rand_CenteredFloat(50.0f) + self->dyna.actor.world.pos.z;
    if (self->timer >= 0) {
        if (self->timer % 3 != 0) {
            if (self->type == 2) {
                EffectSsDeadDb_Spawn(play, &sp54, &sBgPoEventZeroVec, &sBgPoEventZeroVec, 100, 0, 255, 255, 150, 170,
                                     255, 0, 0, 1, 11, 1);
            } else {
                EffectSsDeadDb_Spawn(play, &sp54, &sBgPoEventZeroVec, &sBgPoEventZeroVec, 100, 0, 200, 255, 255, 170,
                                     50, 100, 255, 1, 11, 1);
            }
        }
        if (self->timer == 0) {
            self->dyna.actor.draw = NULL;
            return;
        }
    }
    if (self->timer < -90) {
        Actor_Kill(&self->dyna.actor);
    }
}

void BgPoEvent_PaintingEmpty(BgPoEvent* self, PlayState* play) {
    if (sPuzzleState == self->index) {
        self->timer = 255;
        self->actionFunc = BgPoEvent_PaintingAppear;
    }
}

void BgPoEvent_PaintingPresent(BgPoEvent* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    DECR(self->timer);

    if (((self->timer == 0) || ((self->dyna.actor.xzDistToPlayer < 130.0f) && (self->dyna.actor.yDistToPlayer < 50.0f)) ||
         (FUN_0033bd6c(player) && (self->dyna.actor.xzDistToPlayer < 310.0f) &&
          ((self->index != 2) ? (self->dyna.actor.yDistToPlayer < 100.0f) : (self->dyna.actor.yDistToPlayer < 0.0f)) &&
          FUN_0036f2f8((&self->dyna.actor), 0x2000, play))) &&
        ((self->dyna.actor.parent != NULL) || (self->dyna.actor.child != NULL))) {
        self->timer = 0;
        Audio_PlayActorSound2((&self->dyna.actor), 0x0100035C);
        self->actionFunc = BgPoEvent_PaintingVanish;
    } else if (self->collider.base.acFlags & AC_HIT) {
        if (!BgPoEvent_NextPainting(self)) {
            Actor_Spawn(&play->actorCtx, play, ACTOR_EN_PO_SISTERS, self->dyna.actor.world.pos.x, self->dyna.actor.world.pos.y - 40.0f,
                        self->dyna.actor.world.pos.z, 0, self->dyna.actor.shape.rot.y, 0, self->dyna.actor.params + ((self->type - 1) << 8), 1);
            OnePointCutscene_Init(play, 3160, 80, (&self->dyna.actor), CAM_ID_MAIN);
            FUN_00372244(&play->unk_5FCC, 0x1E, 0x01000487);
        } else {
            Audio_PlayActorSound2((&self->dyna.actor), 0x0100035F);
            OnePointCutscene_Init(play, 3160, 35, (&self->dyna.actor), CAM_ID_MAIN);
        }
        if (self->dyna.actor.parent != NULL) {
            self->dyna.actor.parent->child = NULL;
            self->dyna.actor.parent = NULL;
        }
        if (self->dyna.actor.child != NULL) {
            self->dyna.actor.child->parent = NULL;
            self->dyna.actor.child = NULL;
        }
        self->timer = 30;
        self->actionFunc = BgPoEvent_PaintingBurn;
    }
}

static s32 BgPoEvent_NextPainting(BgPoEvent* self) {
    if ((self->dyna.actor.parent != NULL) && (self->dyna.actor.child != NULL)) {
        s32 idx;
        if (Rand_ZeroOne() < 0.5f) {
            idx = ((BgPoEvent*)self->dyna.actor.parent)->index;
        } else {
            idx = ((BgPoEvent*)self->dyna.actor.child)->index;
        }
        sPuzzleState = idx;
    } else if (self->dyna.actor.parent != NULL) {
        sPuzzleState = ((BgPoEvent*)self->dyna.actor.parent)->index;
    } else if (self->dyna.actor.child != NULL) {
        sPuzzleState = ((BgPoEvent*)self->dyna.actor.child)->index;
    } else {
        return 0;
    }
    return 1;
}

void BgPoEvent_PaintingVanish(BgPoEvent* self, PlayState* play) {
    self->timer += 30;
    if (self->timer >= 255) {
        BgPoEvent_NextPainting(self);
        self->actionFunc = BgPoEvent_PaintingEmpty;
    }
}

void FUN_00346850(BgPoEvent* self) {
    s32 phi_v1;
    s32 phi_a1;
    s32 phi_a3;
    s32 phi_t0;

    if ((self->index == 3) || (self->index == 1)) {
        phi_v1 = self->dyna.actor.world.pos.z;
        phi_a1 = self->dyna.actor.child->world.pos.z;
        if (self->index == 3) {
            phi_a3 = self->dyna.actor.world.pos.x;
            phi_t0 = self->dyna.actor.child->world.pos.x;
        } else {
            phi_a3 = self->dyna.actor.child->world.pos.x;
            phi_t0 = self->dyna.actor.world.pos.x;
        }
    } else {
        phi_v1 = self->dyna.actor.world.pos.x;
        phi_a1 = self->dyna.actor.child->world.pos.x;
        if (self->index == 0) {
            phi_a3 = self->dyna.actor.world.pos.z;
            phi_t0 = self->dyna.actor.child->world.pos.z;
        } else {
            phi_a3 = self->dyna.actor.child->world.pos.z;
            phi_t0 = self->dyna.actor.world.pos.z;
        }
    }
    if ((phi_v1 == phi_a1) && ((phi_t0 - phi_a3) == 60)) {
        sPuzzleState |= (1 << self->index);
    } else {
        sPuzzleState &= ~(1 << self->index);
    }
}

}
