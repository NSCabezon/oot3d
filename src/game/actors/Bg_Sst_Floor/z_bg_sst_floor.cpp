extern "C" {
#include "z_bg_sst_floor.hpp"

#include <math.h>

#define FLAGS 0x80400030

#define BGCHECKFLAG_GROUND 0x0001
#define BGCHECKFLAG_GROUND_TOUCH 0x0002

#define CAM_SET_BOSS_BONGO 0x0C
#define CAM_SET_DUNGEON0 0x03

#define NA_SE_EN_SHADEST_TAIKO_HIGH 0x010003C1

// CollisionHeader in include/ has no vtxList; the ROM reads it at +0x18.
typedef struct {
    /* 0x00 */ char unk_00[0x18];
    /* 0x18 */ Vec3s* vtxList;
} BgSstFloorCollHeader;

void BgSstFloor_Init(Actor* actor, GameState* state);
void BgSstFloor_Destroy(Actor* actor, GameState* state);
void BgSstFloor_Update(Actor* actor, GameState* state);
void BgSstFloor_Draw(Actor* actor, GameState* state);

// Helpers not declared in include/ yet
void Camera_ChangeSetting(Camera* camera, s32 setting);
f32 Actor_WorldDistXZToActor(Actor* actorA, Actor* actorB);
void FUN_002a1a08(PlayState* play, DynaCollisionContext* dyna); // N64: DynaPoly_InvalidateLookup (tail call)
s32 FUN_0032d8d8(DynaPolyActor* dyna);                         // DynaPolyActor_IsPlayerAbove
s32 FUN_0036adf4(DynaPolyActor* dyna);                         // DynaPolyActor_IsPlayerOnTop

ActorInit Bg_Sst_Floor_InitVars = {
    ACTOR_BG_SST_FLOOR,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SST,
    sizeof(BgSstFloor),
    (ActorFunc)BgSstFloor_Init,
    (ActorFunc)BgSstFloor_Destroy,
    (ActorFunc)BgSstFloor_Update,
    (ActorFunc)BgSstFloor_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgSstFloor_Init(Actor* actor, GameState* state) {
    BgSstFloor* self = (BgSstFloor*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, 1);
    FUN_00372f38(actor, play, &self->skelAnimModel, 0, 0);
    self->colHeader = (CollisionHeader*)DynaPolyInfo_Alloc(&self->dyna, state, 0);
    ReadCollisionHeaderFromZsi(self->colHeader, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
}

void BgSstFloor_Destroy(Actor* actor, GameState* state) {
    BgSstFloor* self = (BgSstFloor*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}


// N64 Math_StepToS (z_lib.c), inlined in the ROM
static inline void BgSstFloor_StepToS(s16* pValue, s16 target, s16 step) {
    if (step != 0) {
        if (target < *pValue) {
            step = -step;
        }

        *pValue += step;

        if (((*pValue - target) * step) >= 0) {
            *pValue = target;
        }
    }
}

void BgSstFloor_Update(Actor* actor, GameState* state) {
    BgSstFloor* self = (BgSstFloor*)actor;
    Player* player = GET_PLAYER((PlayState*)state);
    CollisionHeader* colHeader = self->colHeader;

    if (FUN_0032d8d8(&self->dyna) && (actor->yDistToPlayer < 1000.0f)) {
        Camera_ChangeSetting(((PlayState*)state)->cameraPtrs[0], CAM_SET_BOSS_BONGO);
    } else {
        Camera_ChangeSetting(((PlayState*)state)->cameraPtrs[0], CAM_SET_DUNGEON0);
    }

    if (FUN_0036adf4(&self->dyna) && (*(s16*)((u8*)player + 0x2282) > 1000)) {
        self->dyna.actor.params = BONGOFLOOR_HIT;
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EN_SHADEST_TAIKO_HIGH);
    }

    if (self->dyna.actor.params == BONGOFLOOR_HIT) {
        Actor* item00 = ((PlayState*)state)->actorCtx.actorLists[ACTORCAT_MISC].head;
        f32 distFromRim;

        self->drumAmp = 80;
        self->dyna.actor.params = BONGOFLOOR_REST;
        self->drumPhase = 42;

        if (FUN_0036adf4(&self->dyna) && !(player->stateFlags1 & 0x6000)) {
            distFromRim = 600.0f - self->dyna.actor.xzDistToPlayer;
            if (distFromRim > 0.0f) {
                distFromRim = (distFromRim > 350.0f) ? 350.0f : distFromRim;
                player->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND;
                player->actor.velocity.y = distFromRim * 9.0f * (1.0f / 350.0f);
            }
        }

        while (item00 != NULL) {
            if ((item00->id == ACTOR_EN_ITEM00) && (item00->world.pos.y == 0.0f)) {
                distFromRim = 600.0f - Actor_WorldDistXZToActor(&self->dyna.actor, item00);
                if (distFromRim > 0.0f) {
                    distFromRim = (distFromRim > 350.0f) ? 350.0f : distFromRim;
                    item00->bgCheckFlags &= ~(BGCHECKFLAG_GROUND | BGCHECKFLAG_GROUND_TOUCH);
                    item00->velocity.y = distFromRim * 9.0f * (1.0f / 350.0f);
                }
            }
            item00 = item00->next;
        }
    }

    {
        s32 n = (s32)(self->drumPhase > 0 ? 0.5f + (f32)self->drumPhase * 2.0f / 3.0f : (f32)self->drumPhase * 2.0f / 3.0f - 0.5f);
        f32 sn = sinf((f32)n * (M_PI / 2));
        self->drumHeight = -((f32)self->drumAmp * sn);
    }
    BgSstFloor_StepToS(&self->drumAmp, 0, 5);

    ((BgSstFloorCollHeader*)colHeader)->vtxList[1].y = ((BgSstFloorCollHeader*)colHeader)->vtxList[0].y =
        ((BgSstFloorCollHeader*)colHeader)->vtxList[2].y = ((BgSstFloorCollHeader*)colHeader)->vtxList[3].y =
            ((BgSstFloorCollHeader*)colHeader)->vtxList[4].y = ((BgSstFloorCollHeader*)colHeader)->vtxList[7].y =
                ((BgSstFloorCollHeader*)colHeader)->vtxList[9].y = ((BgSstFloorCollHeader*)colHeader)->vtxList[11].y =
                    ((BgSstFloorCollHeader*)colHeader)->vtxList[13].y = self->dyna.actor.home.pos.y + self->drumHeight;

    if (self->drumPhase != 0) {
        self->drumPhase--;
    }
    FUN_002a1a08((PlayState*)state, &((PlayState*)state)->colCtx.dyna);
}

// Matrix_Scale(x, y, z, MTXMODE_APPLY) on a 3x4 matrix: scales each column
static inline void BgSstFloor_ScaleMtx(MTX34* mtx, f32 x, f32 y, f32 z) {
    f32* m = (f32*)mtx;

    m[0] *= x;
    m[4] *= x;
    m[8] *= x;
    m[1] *= y;
    m[5] *= y;
    m[9] *= y;
    m[2] *= z;
    m[6] *= z;
    m[10] *= z;
}

void BgSstFloor_Draw(Actor* actor, GameState* state) {
    BgSstFloor* self = (BgSstFloor*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    BgSstFloor_ScaleMtx(&modelMtx, 1.0f, (f32)self->drumHeight * -0.0025f, 1.0f);

    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&modelMtx);
    self->skelAnimModel->FUN_00372170(0);
}
}
