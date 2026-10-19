extern "C" {
#include "z_bg_ice_objects.hpp"

#define FLAGS 0x80400030

void BgIceObjects_Init(Actor* actor, GameState* state);
void BgIceObjects_Destroy(Actor* actor, GameState* state);
void BgIceObjects_Update(Actor* actor, GameState* state);
void BgIceObjects_Draw(Actor* actor, GameState* state);

// Action functions: the 3DS build keeps these at fixed ld-pinned addresses.
void FUN_001979d0(BgIceObjects* self, PlayState* play); // N64: BgIceObjects_Idle
void FUN_00196f54(BgIceObjects* self, PlayState* play); // N64: BgIceObjects_Slide (still asm)
void FUN_0015441c(BgIceObjects* self, PlayState* play); // N64: BgIceObjects_Reset
// N64: BgIceObjects_Stuck. Lives as DCB bytes inside BgPoEvent_BlockWait.s, so it is referenced by address.
#define BgIceObjects_Stuck ((BgIceObjectsActionFunc)0x001976e4)

s32 Player_InCsMode(PlayState* play);
f32 Actor_WorldDistXZToPoint(Actor* actor, Vec3f* point);
void FUN_00366150(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor,
                  Color_RGBA8* envColor, s32 life, s32 scale);

/*
 * Tables live in the raw data dump of Bg_Hidan_Syoku (0x511F80..0x511FE8). They are exposed as absolute
 * symbols in BgIceObjects_data.s so armcc loads the base address like the original code does.
 */
extern InitChainEntry BgIceObjects_sInitChain[];
extern s16 BgIceObjects_sXStarts[];
extern s16 BgIceObjects_sZStarts[];
extern s16 BgIceObjects_sZStops[7][2];
extern s16 BgIceObjects_sXStops[7][2];
extern f32 BgIceObjects_sBlock[]; // 0x511F84: sWhite, sGray, (initchain), speed step at +0xC
extern Color_RGBA8 BgIceObjects_sColors[];
s16 Rand_S16Offset(s16 base, s16 range);
extern Vec3f BgIceObjects_sZeroVec;
GLOBAL_ASM("../src/game/actors/Bg_Ice_Objects/BgIceObjects_data.s")
#define sInitChain BgIceObjects_sInitChain
#define sXStarts BgIceObjects_sXStarts
#define sZStarts BgIceObjects_sZStarts
#define sZStops BgIceObjects_sZStops
#define sXStops BgIceObjects_sXStops
#define sWhite (*(Color_RGBA8*)((u32)&BgIceObjects_sColors[0] - 4))
#define sGray BgIceObjects_sColors[0]
#define sSpeedStep BgIceObjects_sBlock[3]
#define sZeroVec BgIceObjects_sZeroVec

ActorInit Bg_Ice_Objects_InitVars = {
    ACTOR_BG_ICE_OBJECTS,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_ICE_OBJECTS,
    sizeof(BgIceObjects),
    (ActorFunc)BgIceObjects_Init,
    (ActorFunc)BgIceObjects_Destroy,
    (ActorFunc)BgIceObjects_Update,
    (ActorFunc)BgIceObjects_Draw,
};

void BgIceObjects_Init(Actor* actor, GameState* state) {
    BgIceObjects* self = (BgIceObjects*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;
    void* colInfo;

    Actor_ProcessInitChain(actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, 0);
    ZARInfo* zarInfo = FUN_00372f38(actor, play, &self->skelAnimModel, 0, 0);
    self->skelAnimModel->unk_0C->FUN_00372d94(zarInfo->GetCMABByIndex(2));
    self->skelAnimModel->unk_0C->SetAnimType(1);
    colInfo = DynaPolyInfo_Alloc(&self->dyna, state, 0);
    ReadCollisionHeaderFromZsi(colInfo, &colHeader);
    Math_Vec3f_Copy(&self->targetPos, &actor->home.pos);
    self->actionFunc = FUN_001979d0;
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    actor->params = 0;
}

void BgIceObjects_Destroy(Actor* actor, GameState* state) {
    BgIceObjects* self = (BgIceObjects*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

/* Slide (FUN_00196f54) stays GLOBAL_ASM (asm/FUN_00196f54.s, in functions_180000s.cpp). The C below matches
 * except for sWhite/sGray: the original takes `ldr r1,=&sGray; sub r0,r1,#4` (one literal, no CSE) because the
 * colors are same-TU statics; with extern/EQU symbols armcc either emits two literals (+4 B) or CSEs the pointer
 * into r7 (extra push + 4 B stack slot). */
#define TRY_SLIDE 0
/*
 * Checks which of the eight possible x and z positions the block is at,
 * defaulting to the maximum x wall or minimum z wall. Each x and z position
 * has only one possible wall or pit on each side of it.
 */
static __forceinline void BgIceObjects_SetNextTarget(BgIceObjects* self, PlayState* play) {
    s16 x16 = 0;
    s32 i;

    if ((self->dyna.unk_1B0 == 0) || (self->dyna.unk_1B0 == -0x8000)) {
        x16 = self->dyna.actor.world.pos.x;
        for (i = 0; i < 7; i++) {
            if (x16 == sXStarts[i]) {
                self->targetPos.z = (self->dyna.unk_1B0 == 0) ? sZStops[i][0] : sZStops[i][1];
                return;
            }
        }
        self->targetPos.z = (self->dyna.unk_1B0 == 0) ? -340.0f : -1260.0f;
    } else {
        x16 = self->dyna.actor.world.pos.z;
        for (i = 0; i < 7; i++) {
            if (x16 == sZStarts[i]) {
                self->targetPos.x = (self->dyna.unk_1B0 == 0x4000) ? sXStops[i][0] : sXStops[i][1];
                return;
            }
        }
        self->targetPos.x = (self->dyna.unk_1B0 == 0x4000) ? -860.0f : -1780.0f;
    }
}

/*
 * Falling block: moves down until it reaches the bottom of the pit, then resets.
 */
static void BgIceObjects_FallInPit(BgIceObjects* self, PlayState* play) {

    self->dyna.actor.velocity.y += 1.0f;
    if (Math_StepToF(&self->dyna.actor.world.pos.y, -300.0f, self->dyna.actor.velocity.y)) {
        self->dyna.actor.velocity.y = 0.0f;
        self->dyna.actor.world.pos.x = self->dyna.actor.home.pos.x;
        self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y - 60.0f;
        self->dyna.actor.world.pos.z = self->dyna.actor.home.pos.z;
        if (self->dyna.actor.params != 0) {
            FUN_0036e980(play, &self->dyna.actor, 7);
        }
        self->actionFunc = FUN_0015441c;
    }
}

/*
 * Checks if the block has fallen into any of the pits.
 */
static void BgIceObjects_CheckPits(BgIceObjects* self, PlayState* play) {

    if ((self->dyna.actor.velocity.y > 0.0f) || ((self->dyna.actor.world.pos.x <= -1660.0f) && (self->dyna.actor.world.pos.z <= -1060.0f)) ||
        ((self->dyna.actor.world.pos.x <= -1580.0f) && (self->dyna.actor.world.pos.z >= -420.0f)) ||
        ((self->dyna.actor.world.pos.x >= -980.0f) && (self->dyna.actor.world.pos.z <= -1180.0f)) ||
        ((self->dyna.actor.world.pos.x >= -860.0f) && (self->dyna.actor.world.pos.z >= -700.0f))) {
        BgIceObjects_FallInPit(self, play);
    }
}

void FUN_001979d0(BgIceObjects* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (self->dyna.unk_1A8 != 0.0f) {
        player->stateFlags2 &= ~0x10;
        if ((self->dyna.unk_1A8 > 0.0f) && !Player_InCsMode(play)) {
            BgIceObjects_SetNextTarget(self, play);
            if (Actor_WorldDistXZToPoint(&self->dyna.actor, &self->targetPos) > 1.0f) {
                self->dyna.actor.flags |= 0x10;
                FUN_0036e980(play, &self->dyna.actor, 8);
                self->dyna.actor.params = 1;
                self->actionFunc = FUN_00196f54;
            }
        }
        self->dyna.unk_1A8 = 0.0f;
    }
    if (self->dyna.actor.velocity.y > 0.0f) {
        self->dyna.actor.velocity.y += 1.0f;
        if (Math_StepToF(&self->dyna.actor.world.pos.y, -300.0f, self->dyna.actor.velocity.y)) {
            self->dyna.actor.velocity.y = 0.0f;
            self->dyna.actor.world.pos.x = self->dyna.actor.home.pos.x;
            self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y - 60.0f;
            self->dyna.actor.world.pos.z = self->dyna.actor.home.pos.z;
            if (self->dyna.actor.params != 0) {
                FUN_0036e980(play, &self->dyna.actor, 7);
            }
            self->actionFunc = FUN_0015441c;
        }
    }
}

#if TRY_SLIDE
void FUN_00196f54(BgIceObjects* self, PlayState* play) {
    s32 atTarget;
    Vec3f pos;
    Vec3f velocity;
    f32 spread;

    Math_StepToF(&self->dyna.actor.speedXZ, 10.0f, sSpeedStep);
    atTarget = Math_StepToF(&self->dyna.actor.world.pos.x, self->targetPos.x, self->dyna.actor.speedXZ);
    atTarget &= Math_StepToF(&self->dyna.actor.world.pos.z, self->targetPos.z, self->dyna.actor.speedXZ);
    if (atTarget) {
        self->dyna.actor.speedXZ = 0.0f;
        self->targetPos.x = self->dyna.actor.world.pos.x;
        self->targetPos.z = self->dyna.actor.world.pos.z;
        self->dyna.actor.flags = (self->dyna.actor.velocity.y <= 0.0f) ? (self->dyna.actor.flags & ~0x10) : self->dyna.actor.flags;
        self->dyna.actor.params = 0;
        FUN_0036e980(play, &self->dyna.actor, 7);
        Audio_PlayActorSound2(&self->dyna.actor, 0x0100018c);
        if ((__fabsf(self->dyna.actor.world.pos.x + 1387.0f) < 1.0f) && (__fabsf(self->dyna.actor.world.pos.z + 260.0f) < 1.0f)) {
            self->actionFunc = BgIceObjects_Stuck;
        } else {
            self->actionFunc = FUN_001979d0;
        }
    } else if ((self->dyna.actor.speedXZ > 6.0f) && (self->dyna.actor.world.pos.y >= 0.0f)) {
        spread = Rand_CenteredFloat(120.0f);
        velocity.x = -(1.5f + Rand_ZeroOne()) * Math_SinS(self->dyna.unk_1B0);
        velocity.y = Rand_ZeroOne() + 1.0f;
        velocity.z = -(1.5f + Rand_ZeroOne()) * Math_CosS(self->dyna.unk_1B0);
        pos.x = self->dyna.actor.world.pos.x - (60.0f * Math_SinS(self->dyna.unk_1B0)) - (Math_CosS(self->dyna.unk_1B0) * spread);
        pos.z = self->dyna.actor.world.pos.z - (60.0f * Math_CosS(self->dyna.unk_1B0)) + (Math_SinS(self->dyna.unk_1B0) * spread);
        pos.y = self->dyna.actor.world.pos.y;
        FUN_00366150(play, &pos, &velocity, &sZeroVec, &sWhite, &sGray, 250, Rand_S16Offset(40, 15));
        spread = Rand_CenteredFloat(120.0f);
        pos.x = self->dyna.actor.world.pos.x - (60.0f * Math_SinS(self->dyna.unk_1B0)) + (Math_CosS(self->dyna.unk_1B0) * spread);
        pos.z = self->dyna.actor.world.pos.z - (60.0f * Math_CosS(self->dyna.unk_1B0)) - (Math_SinS(self->dyna.unk_1B0) * spread);
        FUN_00366150(play, &pos, &velocity, &sZeroVec, &sWhite, &sGray, 250, Rand_S16Offset(40, 15));
        FUN_00373264(&self->dyna.actor, 0x010000e0);
    }
    BgIceObjects_CheckPits(self, play);
}

#endif
void FUN_0015441c(BgIceObjects* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (self->dyna.unk_1A8 != 0.0f) {
        player->stateFlags2 &= ~0x10;
        self->dyna.unk_1A8 = 0.0f;
    }
    if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y, 1.0f)) {
        self->dyna.actor.flags &= ~0x10;
        Math_Vec3f_Copy(&self->targetPos, &self->dyna.actor.home.pos);
        self->dyna.actor.speedXZ = 0.0f;
        self->actionFunc = FUN_001979d0;
    }
}


void BgIceObjects_Update(Actor* actor, GameState* state) {
    BgIceObjects* self = (BgIceObjects*)actor;
    PlayState* play = (PlayState*)state;

    self->actionFunc(self, play);
}

void BgIceObjects_Draw(Actor* actor, GameState* state) {
    BgIceObjects* self = (BgIceObjects*)actor;
    MTX34 modelMtx;
    f32 animSpeed;

    MTX34CopyAsm(&modelMtx, &self->dyna.actor.modelMtx);
    animSpeed = 1.0f;
    if (PauseContext_GetState() != 0) {
        animSpeed = 0.0f;
    }
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->unk_0C->SetAnimSpeed(animSpeed);
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}
}
