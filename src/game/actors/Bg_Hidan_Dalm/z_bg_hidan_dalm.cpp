extern "C" {
#include "z_bg_hidan_dalm.hpp"

#define FLAGS 0x00400000

#define UPDBGCHECKINFO_FLAG_0 (1 << 0)
#define UPDBGCHECKINFO_FLAG_2 (1 << 2)
#define NA_SE_IT_HAMMER_HIT 0x010000FC
#define NA_SE_EV_DARUMA_VANISH 0x0100020E
#define PARAMS_GET_U(params, shift, count) ((((u16)(params)) >> (shift)) & ((1 << (count)) - 1))

void BgHidanDalm_Wait(BgHidanDalm* self, PlayState* play);
void BgHidanDalm_Shrink(BgHidanDalm* self, PlayState* play);

void BgHidanDalm_Init(Actor* actor, GameState* state);
void BgHidanDalm_Destroy(Actor* actor, GameState* state);
void BgHidanDalm_Update(Actor* actor, GameState* state);
void BgHidanDalm_Draw(Actor* actor, GameState* state);

void EffectSsKiraKira_SpawnSmallYellow(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel);
s32 Collider_DestroyTris(PlayState* play, ColliderTris* collider);
void Collider_InitTris(PlayState* play, ColliderTris* collider);
void Collider_SetTris(PlayState* play, ColliderTris* collider, Actor* actor, void* src, ColliderTrisElement* elements);

GLOBAL_ASM("data/z_Bg_Hidan_Dalm.data.s")

GLOBAL_ASM("../src/game/actors/Bg_Hidan_Dalm/data_syms.s")

extern InitChainEntry BgHidanDalm_sInitChain[];
extern ColliderTrisInit BgHidanDalm_sTrisInit;
extern ColliderTrisElementInit BgHidanDalm_sTrisElementInit[];
#define sInitChain BgHidanDalm_sInitChain
#define sTrisInit BgHidanDalm_sTrisInit
#define sTrisElementInit BgHidanDalm_sTrisElementInit

#define ACELEM_HIT (1 << 1)
#define BGCHECKFLAG_GROUND_TOUCH (1 << 1)
#define BGCHECKFLAG_WALL (1 << 3)
#define ACTOR_FLAG_UPDATE_CULLING_DISABLED 0x10
#define PLAYER_MWA_HAMMER_FORWARD 0x16
#define PLAYER_MWA_HAMMER_SIDE 0x17
#define PLAYER_CSACTION_7 7
#define PLAYER_CSACTION_8 8

s32 Player_InCsMode(PlayState* play);
void FUN_0036f59c(Player* player, u32 sfxId);
void Collider_SetTrisVertices(ColliderTris* collider, s32 index, Vec3f* a, Vec3f* b, Vec3f* c);

ActorInit Bg_Hidan_Dalm_InitVars = {
    ACTOR_BG_HIDAN_DALM,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HIDAN_OBJECTS,
    sizeof(BgHidanDalm),
    (ActorFunc)BgHidanDalm_Init,
    (ActorFunc)BgHidanDalm_Destroy,
    (ActorFunc)BgHidanDalm_Update,
    (ActorFunc)BgHidanDalm_Draw,
};


void BgHidanDalm_Init(Actor* actor, GameState* state) {
    BgHidanDalm* self = (BgHidanDalm*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, 0);
    FUN_00372f38(actor, play, &self->skelAnimModel[0], 3, &self->skelAnimModel[1], 4, 0);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 3), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    Collider_InitTris(play, &self->collider);
    Collider_SetTris(play, &self->collider, actor, &sTrisInit, self->colliderElements);

    self->switchFlag = (actor->params >> 8) & 0xFF;
    actor->params &= 0xFF;
    actor->unk_19B = 2;
    if (Flags_GetSwitch(play, self->switchFlag) == 0) {
        self->actionFunc = BgHidanDalm_Wait;
    } else {
        Actor_Kill(actor);
    }
}

void BgHidanDalm_Destroy(Actor* actor, GameState* state) {
    BgHidanDalm* self = (BgHidanDalm*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyTris(play, &self->collider);
    FUN_00350f34(actor, &self->skelAnimModel[0], &self->skelAnimModel[1], 0);
}

void BgHidanDalm_Update(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    BgHidanDalm* self = (BgHidanDalm*)actor;

    self->actionFunc(self, play);
    Actor_MoveForward(&self->dyna.actor);
    Actor_UpdateBgCheckInfo(play, &self->dyna.actor, 10.0f, 15.0f, 32.0f,
                            UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2);
}

void BgHidanDalm_Wait(BgHidanDalm* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if ((self->collider.base.acFlags & AC_HIT) && !Player_InCsMode(play) &&
        (*((s8*)player + 0x2226) == PLAYER_MWA_HAMMER_FORWARD ||
         *((s8*)player + 0x2226) == PLAYER_MWA_HAMMER_SIDE)) {
        self->collider.base.acFlags &= ~AC_HIT;
        if ((self->collider.elements[0].base.bumperFlags & ACELEM_HIT) ||
            (self->collider.elements[1].base.bumperFlags & ACELEM_HIT)) {
            self->dyna.actor.world.rot.y -= 0x4000;
        } else {
            self->dyna.actor.world.rot.y += 0x4000;
        }
        self->dyna.actor.world.pos.x += 32.5f * Math_SinS(self->dyna.actor.world.rot.y);
        self->dyna.actor.world.pos.z += 32.5f * Math_CosS(self->dyna.actor.world.rot.y);

        FUN_0036e980(play, &self->dyna.actor, PLAYER_CSACTION_8);
        self->dyna.actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
        self->actionFunc = BgHidanDalm_Shrink;
        self->dyna.actor.speedXZ = 10.0f;
        self->dyna.actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND_TOUCH;
        self->dyna.actor.bgCheckFlags &= ~BGCHECKFLAG_WALL;
        Flags_SetSwitch(play, self->switchFlag);
        FUN_0036f59c(GET_PLAYER(play), 0x010000FC);
        Audio_PlayActorSound2(&self->dyna.actor, 0x0100020E);
    } else {
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
    }
}

// Function-local static: guard 0x005116B8 (first word after the TU .data, cut from the data chunk),
// object 0x005699C8 pinned in data/oot3d.ld (scripts/pin_static.py).
void BgHidanDalm_Shrink(BgHidanDalm* self, PlayState* play) {
    static Vector3f accel(0.0f, 0.0f, 0.0f);
    s32 i;
    Vec3f velocity;
    Vec3f pos;

    if (Math_StepToF(&self->dyna.actor.scale.x, 0.0f, 0.004f)) {
        FUN_0036e980(play, &self->dyna.actor, PLAYER_CSACTION_7);
        Actor_Kill(&self->dyna.actor);
    }

    self->dyna.actor.scale.y = self->dyna.actor.scale.z = self->dyna.actor.scale.x;

    pos.x = self->dyna.actor.world.pos.x;
    pos.y = self->dyna.actor.world.pos.y + self->dyna.actor.scale.x * 160.0f;
    pos.z = self->dyna.actor.world.pos.z;

    for (i = 0; i < 4; i++) {
        velocity.x = 5.0f * Math_SinS(self->dyna.actor.world.rot.y + 0x8000) + (Rand_ZeroOne() - 0.5f) * 5.0f;
        velocity.z = 5.0f * Math_CosS(self->dyna.actor.world.rot.y + 0x8000) + (Rand_ZeroOne() - 0.5f) * 5.0f;
        velocity.y = (Rand_ZeroOne() - 0.5f) * 1.5f;
        EffectSsKiraKira_SpawnSmallYellow(play, &pos, &velocity, (Vec3f*)&accel);
    }
}


// N64-order wrapper (arg scheduling of the ROM follows the N64 call order)
static inline void MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}

void BgHidanDalm_Draw(Actor* actor, GameState* state) {
    BgHidanDalm* self = (BgHidanDalm*)actor;
    MTX34 modelMtx;
    Vec3f pos[3];
    MTX34 mtx;

    MTX34CopyAsm(&modelMtx, &self->dyna.actor.modelMtx);
    if (self->dyna.actor.params == 0) {
        if (self->skelAnimModel[0] != NULL) {
            self->skelAnimModel[0]->SetUnkAC(1);
            self->skelAnimModel[0]->FUN_003721e0(&modelMtx);
            self->skelAnimModel[0]->FUN_00372170(0);
        }
    } else {
        if (self->skelAnimModel[1] != NULL) {
            self->skelAnimModel[1]->SetUnkAC(1);
            self->skelAnimModel[1]->FUN_003721e0(&modelMtx);
            self->skelAnimModel[1]->FUN_00372170(0);
        }
    }

    if (self->actionFunc == BgHidanDalm_Wait) {
        MTX34CopyAsm(&mtx, &self->dyna.actor.modelMtx);

        MultVec3f((Vector3f*)&sTrisElementInit[0].dim.vtx[0], (Vector3f*)&pos[0], &mtx);
        MultVec3f((Vector3f*)&sTrisElementInit[0].dim.vtx[1], (Vector3f*)&pos[1], &mtx);
        MultVec3f((Vector3f*)&sTrisElementInit[0].dim.vtx[2], (Vector3f*)&pos[2], &mtx);
        Collider_SetTrisVertices(&self->collider, 0, &pos[0], &pos[1], &pos[2]);
        MultVec3f((Vector3f*)&sTrisElementInit[1].dim.vtx[2], (Vector3f*)&pos[1], &mtx);
        Collider_SetTrisVertices(&self->collider, 1, &pos[0], &pos[2], &pos[1]);

        MultVec3f((Vector3f*)&sTrisElementInit[2].dim.vtx[0], (Vector3f*)&pos[0], &mtx);
        MultVec3f((Vector3f*)&sTrisElementInit[2].dim.vtx[1], (Vector3f*)&pos[1], &mtx);
        MultVec3f((Vector3f*)&sTrisElementInit[2].dim.vtx[2], (Vector3f*)&pos[2], &mtx);
        Collider_SetTrisVertices(&self->collider, 2, &pos[0], &pos[1], &pos[2]);
        MultVec3f((Vector3f*)&sTrisElementInit[3].dim.vtx[1], (Vector3f*)&pos[2], &mtx);
        Collider_SetTrisVertices(&self->collider, 3, &pos[0], &pos[2], &pos[1]);
    }
}

}
