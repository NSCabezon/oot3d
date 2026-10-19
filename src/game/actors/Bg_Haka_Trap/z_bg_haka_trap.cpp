extern "C" {
#include "z_bg_haka_trap.hpp"

#define FLAGS 0x00400030

#define HAKA_TRAP_GUILLOTINE 0
#define HAKA_TRAP_SPIKED_CRUSHER 1
#define HAKA_TRAP_SPIKED_WALL_EAST 2
#define HAKA_TRAP_SPIKED_WALL_WEST 3
#define HAKA_TRAP_FAN_BLADE 4
#define HAKA_TRAP_GUILLOTINE_FAST 5

void FUN_001ca810(BgHakaTrap* self, PlayState* play); // SpikedWall_CloseIn
void FUN_001cb334(BgHakaTrap* self, PlayState* play); // SpikedCrusher_Fall
void FUN_001e8684(BgHakaTrap* self, PlayState* play); // Guillotine_Fall
void FUN_00345fe0(BgHakaTrap* self, PlayState* play); // UpdateBodyColliderPos
s32 Player_InCsMode(PlayState* play);
void FUN_00373264(Actor* actor, s32 sfxId); // Actor_PlaySfx_Flagged
void FUN_0019ba38(BgHakaTrap* self, PlayState* play); // Guillotine_Lift
s32 Quake_Add(Camera* camera, s32 type);
void Quake_SetSpeed(s32 quakeIndex, s16 speed);
void Quake_SetQuakeValues(s32 quakeIndex, s16 a, s16 b, s16 c, s16 d);
void Quake_SetCountdown(s32 quakeIndex, s16 countdown);
#define SPIKEDWALL_BURNING(play) (*(s32*)((u8*)(play) + 0x7FA8)) // sIsSpikeWallBurning (lives in PlayState)
#define SPIKEDWALL_FLAGS(play) (*(s32*)((u8*)(play) + 0x7FAC))  // sSpikedWallFlags (lives in PlayState)

void FUN_0049fa58(Vec3f* pos); // Audio_StopSfxByPos
s32 Collider_DestroyTris(PlayState* play, ColliderTris* collider);
void FUN_0032d184(PlayState* play, Actor* actor, void* color, s32 a, s32 b); // func_80026230 (fog)
void Collider_SetTrisVertices(ColliderTris* collider, s32 index, Vec3f* a, Vec3f* b, Vec3f* c);
void GraphicsContext_UnknownStub(GraphicsContext* gfxCtx);
f32 BgCheck_EntityRaycastFloor4(CollisionContext* colCtx, CollisionPoly** outPoly, s32* outBgId, Actor* actor, Vec3f* pos);

void BgHakaTrap_Init(Actor* actor, GameState* state);
void BgHakaTrap_FanBlade_Idle(BgHakaTrap* self, PlayState* play);
void Collider_InitTris(PlayState* play, ColliderTris* collider);
s32 Collider_SetTris(PlayState* play, ColliderTris* collider, Actor* actor, void* src, void* elements);
void BgHakaTrap_Destroy(Actor* actor, GameState* state);
void BgHakaTrap_Update(BgHakaTrap* self, PlayState* play);
void BgHakaTrap_Draw(Actor* actor, GameState* state);
void FUN_001ca65c(BgHakaTrap* self, PlayState* play); // SpikedWall_Burn
void FUN_001777d8(BgHakaTrap* self, PlayState* play); // SpikedCrusher_Lift

GLOBAL_ASM("data/z_Bg_Haka_Trap.data.s")

GLOBAL_ASM("../src/game/actors/Bg_Haka_Trap/data_syms.s")

// These tables live in the data dump above; no scatter symbols, so absolute EQU symbols (data_syms.s).
extern u8 BgHakaTrap_sFogBlack[];
extern u8 BgHakaTrap_sColChkInfoInit[];
extern u8 BgHakaTrap_sCylinderInit[];
extern u8 BgHakaTrap_sTrisElementsInit[];
extern u8 BgHakaTrap_sTrisInit[];

ActorInit Bg_Haka_Trap_InitVars = {
    ACTOR_BG_HAKA_TRAP,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HAKA_OBJECTS,
    sizeof(BgHakaTrap),
    (ActorFunc)BgHakaTrap_Init,
    (ActorFunc)BgHakaTrap_Destroy,
    (ActorFunc)BgHakaTrap_Update,
    (ActorFunc)BgHakaTrap_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

#define SPIKED_CRUSHER_FLIP(play) (*(s32*)((u8*)(play) + 0x7F74)) // sSpikedCrusherFlip (lives in PlayState)

void BgHakaTrap_Init(Actor* actor, GameState* state) {
    BgHakaTrap* self = (BgHakaTrap*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(actor, sInitChain);
    self->unk_314 = 0;
    *(s32*)&self->unk_318 = actor->params & 0xFF00;
    actor->params &= 0xFF;

    FUN_00372f38(actor, play, &self->skelAnimModel[0], 14, &self->skelAnimModel[1], 18,
                                             &self->skelAnimModel[2], 15, &self->skelAnimModel[3], 16,
                                             &self->skelAnimModel[4], 13, 0);
    actor->unk_19B = 3;

    if (actor->params != HAKA_TRAP_FAN_BLADE) {
        Collider_InitCylinder(play, &self->colliderCylinder);
        Collider_SetCylinder(play, &self->colliderCylinder, actor, (ColliderCylinderInit*)BgHakaTrap_sCylinderInit);

        if ((actor->params == HAKA_TRAP_GUILLOTINE) || (actor->params == HAKA_TRAP_GUILLOTINE_FAST)) {
            self->timer = 30;
            self->colliderCylinder.dim.yShift = 10.0f;
            actor->velocity.y = 0.1f;

            if (actor->params == HAKA_TRAP_GUILLOTINE_FAST) {
                actor->params = HAKA_TRAP_GUILLOTINE;
                self->isGuillotineFast = true;
            }

            self->actionFunc = FUN_001e8684;
        } else {
            DynaPolyActor_Init(&self->dyna, 1);
            actor->flags |= 0x10;

            if (actor->params == HAKA_TRAP_SPIKED_CRUSHER) {
                ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 12), &colHeader);
                self->timer = 45;

                if (SPIKED_CRUSHER_FLIP(play)) {
                    self->actionFunc = FUN_001777d8;
                    SPIKED_CRUSHER_FLIP(play) = false;
                } else {
                    SPIKED_CRUSHER_FLIP(play) = true;
                    actor->velocity.y = 0.5f;
                    self->actionFunc = FUN_001cb334;
                }

                actor->floorHeight = actor->home.pos.y - 225.0f;
                self->spikedCrusherStepY = (actor->floorHeight + 50.0f) - 25.0f;

                self->colliderCylinder.dim.radius = 10.0f;
                self->colliderCylinder.dim.height = 40.0f;
            } else {
                if (actor->params == HAKA_TRAP_SPIKED_WALL_EAST) {
                    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 9), &colHeader);
                    actor->home.pos.x -= 200.0f;
                } else {
                    actor->home.pos.x += 200.0f;
                    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 10), &colHeader);
                }

                actor->unk_19B = 2;
                Collider_InitTris(play, &self->spikesCollider);
                Collider_SetTris(play, &self->spikesCollider, actor, BgHakaTrap_sTrisInit, self->spikesColliderElements);

                self->colliderCylinder.dim.radius = 18.0f;
                self->colliderCylinder.dim.height = 115.0f;
                self->colliderCylinder.info.toucherFlags |= 0x10;

                self->actionFunc = FUN_001ca810;
            }

            self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
        }
    } else {
        self->timer = 60;
        actor->uncullZoneScale = 500.0f;
        self->actionFunc = BgHakaTrap_FanBlade_Idle;
    }

    CollisionCheck_SetInfo(&actor->colChkInfo, NULL, (CollisionCheckInfoInit*)BgHakaTrap_sColChkInfoInit);
}

void BgHakaTrap_Destroy(Actor* actor, GameState* state) {
    BgHakaTrap* self = (BgHakaTrap*)actor;
    PlayState* play = (PlayState*)state;

    if (self->dyna.actor.params != HAKA_TRAP_FAN_BLADE) {
        if (self->dyna.actor.params != HAKA_TRAP_GUILLOTINE) {
            DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
            if ((self->dyna.actor.params == HAKA_TRAP_SPIKED_WALL_EAST) ||
                (self->dyna.actor.params == HAKA_TRAP_SPIKED_WALL_WEST)) {
                Collider_DestroyTris(play, &self->spikesCollider);
            }
        }

        Collider_DestroyCylinder(play, &self->colliderCylinder);
    }

    FUN_0049fa58(&self->chainLiftSfxPos);
    FUN_00350f34(actor, &self->skelAnimModel[0], &self->skelAnimModel[1], &self->skelAnimModel[2], &self->skelAnimModel[3], &self->skelAnimModel[4], 0);
}

#if 0 // draft: 15 words off (play in r1 for the actionFunc call + params reload after the cylinder y store). R5 sweep: greedy
      // single-site/pair mutations, N64 literal shape, GameState* state signatures, helpers for timer/colliders,
      // casted/aliased cylinder stores, DECR: none below 15.
void BgHakaTrap_Update(BgHakaTrap* self, PlayState* play2) {
    BgHakaTrap* s2 = self; // dead alias (greedy mutation): 17 -> 15 words
    if (self->unk_314 != 0) { s2->unk_314--; }
    self->actionFunc(self, play2);
    Actor* thisx = &self->dyna.actor;
    PlayState* play = play2;
    if ((thisx->params != HAKA_TRAP_FAN_BLADE) && (thisx->params != HAKA_TRAP_SPIKED_CRUSHER)) {
        self->colliderCylinder.dim.pos.y = self->dyna.actor.world.pos.y;

        if ((thisx->params == HAKA_TRAP_GUILLOTINE) || (thisx->params == HAKA_TRAP_GUILLOTINE_FAST)) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &self->colliderCylinder.base);
            CollisionCheck_SetOC(play, &play->colChkCtx, &self->colliderCylinder.base);
        } else {
            if (self->actionFunc == FUN_001ca810) {
                CollisionCheck_SetAC(play, &play->colChkCtx, &self->spikesCollider.base);
            }

            if ((self->actionFunc == FUN_001ca810) && (self->unk_314 > 0)) {
                return;
            }

            CollisionCheck_SetAT(play, &play->colChkCtx, &self->colliderCylinder.base);
        }
    }
}
#endif

GLOBAL_ASM("asm/BgHakaTrap_Update.s")


static inline void BgHakaTrap_MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}

void BgHakaTrap_Draw(Actor* actor, GameState* state) {
    BgHakaTrap* self = (BgHakaTrap*)actor;
    PlayState* play = (PlayState*)state;
    MTX34 modelMtx;

    if (self->actionFunc == FUN_001ca65c) {
        FUN_0032d184(play, actor, BgHakaTrap_sFogBlack, self->timer + 30, 60);
    }

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    {
        s32 idx = actor->params;
        if (self->skelAnimModel[idx] != NULL) {
            self->skelAnimModel[idx]->SetUnkAC(1);
            self->skelAnimModel[idx]->FUN_003721e0(&modelMtx);
            self->skelAnimModel[idx]->FUN_00372170(0);
        }
    }

    if (self->actionFunc == FUN_001ca810) {
        Vec3f vtx[3];
        MTX34 mtx;

        MTX34CopyAsm(&mtx, &actor->modelMtx);
        BgHakaTrap_MultVec3f((Vector3f*)(BgHakaTrap_sTrisElementsInit + 0x18), (Vector3f*)&vtx[0], &mtx);
        BgHakaTrap_MultVec3f((Vector3f*)(BgHakaTrap_sTrisElementsInit + 0x24), (Vector3f*)&vtx[1], &mtx);
        BgHakaTrap_MultVec3f((Vector3f*)(BgHakaTrap_sTrisElementsInit + 0x30), (Vector3f*)&vtx[2], &mtx);
        Collider_SetTrisVertices(&self->spikesCollider, 0, (Vec3f*)&vtx[0], (Vec3f*)&vtx[1], (Vec3f*)&vtx[2]);
        BgHakaTrap_MultVec3f((Vector3f*)(BgHakaTrap_sTrisElementsInit + 0x6c), (Vector3f*)&vtx[1], &mtx);
        Collider_SetTrisVertices(&self->spikesCollider, 1, (Vec3f*)&vtx[0], (Vec3f*)&vtx[2], (Vec3f*)&vtx[1]);
    }

    if (self->actionFunc == FUN_001ca65c) {
        ((void (*)(PlayState*, Actor*))GraphicsContext_UnknownStub)(play, actor);
    }

    if ((self->actionFunc == FUN_001777d8) && (self->isSpikedCrusherStationary == 0)) {
        f32 x_ = self->dyna.actor.world.pos.x;
        f32 z_ = self->dyna.actor.world.pos.z;
        f32 y_ = self->dyna.actor.world.pos.y + 110.0f;
        self->chainLiftSfxPos.z = z_;
        self->chainLiftSfxPos.y = y_;
        self->chainLiftSfxPos.x = x_;
        Audio_PlaySoundGeneral(0x01000264, &self->chainLiftSfxPos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
    }
}

void FUN_001777d8(BgHakaTrap* self, PlayState* play) {
    if (self->timer != 0) {
        self->timer--;
        if (self->timer > 30) {
            self->isSpikedCrusherStationary = Math_StepToF(&self->dyna.actor.world.pos.y, self->spikedCrusherStepY, 15.0f);
            goto done;
        }
    }
    self->isSpikedCrusherStationary = Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y, 20.0f);
done:
    if (self->timer == 0) {
        self->timer = 45;
        self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y;
        self->dyna.actor.velocity.y = 0.5f;
        self->actionFunc = FUN_001cb334;
    }
}

#if 0 // draft: quake-block constants hoisted into callee-saved regs in ROM (see STATUS.md)
void FUN_001cb334(BgHakaTrap* self, PlayState* play) {
    Vec3f vector;
    f32 floorHeight;
    f32 yIntersect;
    s32 i;
    s32 bgId;
    Player* player = GET_PLAYER(play);

    if (self->dyna.actor.xyzDistToPlayerSq < 9728.0f) {
        if (player->actor.world.pos.y < self->dyna.actor.world.pos.y + 25.0f) {
            player->actor.bgCheckFlags |= 0x100;
        }
    }

    self->dyna.actor.velocity.y *= 1.6f;

    if (self->timer != 0) {
        self->timer--;
    }

    vector.x = self->dyna.actor.world.pos.x + 90.0f;
    vector.y = (self->dyna.actor.world.pos.y + 1.0f) + 25.0f;
    vector.z = self->dyna.actor.world.pos.z;

    floorHeight = self->dyna.actor.floorHeight;

    for (i = 0; i < 3; i++) {
        yIntersect = BgCheck_EntityRaycastFloor4(&play->colCtx, &self->dyna.actor.floorPoly, &bgId, &self->dyna.actor, &vector) - 25.0f;
        if (floorHeight < yIntersect) {
            floorHeight = yIntersect;
        }

        vector.x -= 90.0f;
    }

    if (*(s32*)&self->unk_318 != 0) {
        vector.x = self->dyna.actor.world.pos.x;
        vector.y = (self->dyna.actor.world.pos.y + 1.0f) + 25.0f;
        vector.z = self->dyna.actor.world.pos.z - 90.0f;
        yIntersect = BgCheck_EntityRaycastFloor4(&play->colCtx, &self->dyna.actor.floorPoly, &bgId, &self->dyna.actor, &vector) - 25.0f;
        if (floorHeight < yIntersect) {
            floorHeight = yIntersect;
        }
    }

    if (Math_StepToF(&self->dyna.actor.world.pos.y, floorHeight, self->dyna.actor.velocity.y)) {
        if (self->dyna.actor.velocity.y > 0.01f) {
            Audio_PlayActorSound2(&self->dyna.actor, 0x010001A8);
            if (self->dyna.actor.xzDistToPlayer < 300.0f) {
                s32 q = Quake_Add(play->cameraPtrs[play->activeCamera], 3);
                Quake_SetSpeed(q, 30000);
                Quake_SetQuakeValues(q, 8, 0, 0, 0);
                Quake_SetCountdown(q, 4);

            }
        }
        self->dyna.actor.velocity.y = 0.0f;
    }

    if (self->dyna.actor.velocity.y >= 0.01f) {
        FUN_00373264(&self->dyna.actor, 0x010001A4);
    }

    if (self->timer == 0) {
        self->dyna.actor.velocity.y = 0.0f;
        self->timer = 45;
        self->spikedCrusherStepY = (s16)self->dyna.actor.world.pos.y + 50.0f;
        self->spikedCrusherStepY = CLAMP_MAX(self->spikedCrusherStepY, self->dyna.actor.home.pos.y);
        self->actionFunc = FUN_001777d8;
    }
}
#endif




void FUN_0019ba38(BgHakaTrap* self, PlayState* play) {
    if (self->timer != 0) {
        self->timer--;
    }

    if (self->isGuillotineFast != 0) {
        Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y, 27.0f);
    } else {
        if (self->timer > 30) {
            Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y - 90.0f, 9.0f);
        } else {
            Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y, 4.5f);
        }

        if (self->timer == 30) {
            Audio_PlayActorSound2(&self->dyna.actor, 0x010001A1); // NA_SE_EV_GUILLOTINE_UP
        }
    }

    if (self->timer == 0) {
        self->timer = 30;
        self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y;
        self->dyna.actor.velocity.y = 0.1f;
        self->actionFunc = FUN_001e8684;
    }

    FUN_00345fe0(self, play);
}

void EffectSsDeadDb_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep,
                          s16 primR, s16 primG, s16 primB, s16 primA, s16 envR, s16 envG, s16 envB, s16 unused,
                          s32 arg14, s16 playSfx);

// SpikedWall_Burn. Function-local static: guard 0x005113D8 (TU .data, cut from the data chunk), object
// 0x0056998C pinned in data/oot3d.ld (scripts/pin_static.py).
void FUN_001ca65c(BgHakaTrap* self, PlayState* play) {
    static Vector3f zeroVec(0.0f, 0.0f, 0.0f);
    Vec3f vector;
    s32 i;

    if (self->timer != 0) {
        self->timer--;
    }

    FUN_00373264(&self->dyna.actor, 0x010001B2);

    if ((self->timer % 3) != 0) {
        for (i = 0; i < 2; i++) {
            vector.x = self->dyna.actor.world.pos.x +
                       Rand_ZeroOne() * ((self->dyna.actor.params == HAKA_TRAP_SPIKED_WALL_EAST) ? -30.0f : 30.0f);
            vector.y = Rand_ZeroOne() * 10.0f + 30.0f + self->dyna.actor.world.pos.y;
            vector.z = Rand_CenteredFloat(320.0f) + self->dyna.actor.world.pos.z;

            EffectSsDeadDb_Spawn(play, &vector, (Vec3f*)&zeroVec, (Vec3f*)&zeroVec, 130, 20, 255, 255, 150, 170, 255,
                                 0, 0, 1, 11, false);
        }
    }

    if (self->timer == 0) {
        SPIKEDWALL_BURNING(play) = false;
        Actor_Kill(&self->dyna.actor);
    }
}

void FUN_001ca810(BgHakaTrap* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (!SPIKEDWALL_BURNING(play) && !Player_InCsMode(play)) {
        if (!Math_StepToF(&self->dyna.actor.world.pos.x, self->dyna.actor.home.pos.x, 0.5f)) {
            FUN_00373264(&self->dyna.actor, 0x010001AF);
        } else if (self->dyna.actor.params == HAKA_TRAP_SPIKED_WALL_EAST) {
            SPIKEDWALL_FLAGS(play) |= 1;
        } else if (self->dyna.actor.params == HAKA_TRAP_SPIKED_WALL_WEST) {
            SPIKEDWALL_FLAGS(play) |= 2;
        }
    }

    FUN_00345fe0(self, play);

    if (self->spikesCollider.base.acFlags & AC_HIT) {
        self->timer = 30;
        SPIKEDWALL_BURNING(play) = true;
        self->actionFunc = FUN_001ca65c;
    } else if (SPIKEDWALL_FLAGS(play) == 3) {
        SPIKEDWALL_FLAGS(play) = 4;
        player->actor.bgCheckFlags |= 0x100; // BGCHECKFLAG_CRUSHED
    }

    if (self->colliderCylinder.base.atFlags & 2) {
        if (self->unk_314 == 0) {
            self->unk_314 = 30;
            self->colliderCylinder.base.atFlags &= ~2;
        }
    }
}

void FUN_00345fe0(BgHakaTrap* self, PlayState* play) {
    f32 cos;
    Vec3f colOff;
    f32 sin;
    f32 zSign;
    Player* player = GET_PLAYER(play);

    FUN_0036c5d8(&self->dyna.actor, &colOff, &player->actor.world.pos);

    sin = Math_SinS(self->dyna.actor.shape.rot.y);
    cos = Math_CosS(self->dyna.actor.shape.rot.y);

    if (self->dyna.actor.params == HAKA_TRAP_GUILLOTINE) {
        colOff.x = CLAMP(colOff.x, -50.0f, 50.0f);
        zSign = (colOff.z >= 0.0f) ? 1.0f : -1.0f;
        colOff.z = zSign * -15.0f;
    } else {
        colOff.x = -CLAMP(colOff.x, -162.0f, 162.0f);
        zSign = (colOff.z >= 0.0f) ? 1.0f : -1.0f;
        colOff.z = zSign * 15.0f;
    }

    self->colliderCylinder.dim.pos.x = self->dyna.actor.world.pos.x + colOff.x * cos + colOff.z * sin;
    self->colliderCylinder.dim.pos.z = self->dyna.actor.world.pos.z + colOff.x * sin + colOff.z * cos;
}
#if 0 // draft: quake-block constants hoisted into callee-saved regs in ROM (see STATUS.md)
void FUN_001e8684(BgHakaTrap* self, PlayState* play) {
    s32 isDoneFalling;
    s32 timer;

    if (self->isGuillotineFast) {
        self->dyna.actor.velocity.y *= 3.0f;
    } else {
        self->dyna.actor.velocity.y *= 2.0f;
    }

    if (self->timer != 0) {
        self->timer -= 1;
    }

    isDoneFalling = Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y - 185.0f, self->dyna.actor.velocity.y);
    timer = self->timer;

    if ((timer == 9 && !self->isGuillotineFast) || (timer == 15 && self->isGuillotineFast)) {
        Audio_PlayActorSound2(&self->dyna.actor, 0x010001A2);

        if (self->dyna.actor.xzDistToPlayer < 300.0f) {
            s32 q;
            s32 three = 3;
            s32 z = 0;
            s32 five = 5;
            q = Quake_Add(play->cameraPtrs[*(s16*)((u8*)play + 0xA64)], three);
            Quake_SetSpeed(q, 30000);
            Quake_SetQuakeValues(q, five, z, z, z);
            Quake_SetCountdown(q, three);
            self->timer += z;

        }
    }

    if (self->timer == 0) {
        self->dyna.actor.velocity.y = 0.0f;
        self->timer = (self->isGuillotineFast) ? 15 : 60;
        Audio_PlayActorSound2(&self->dyna.actor, 0x010001A1);
        self->actionFunc = FUN_0019ba38;
    }

    FUN_00345fe0(self, play);

    if (!isDoneFalling) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &self->colliderCylinder.base);
    }
}
#endif



}