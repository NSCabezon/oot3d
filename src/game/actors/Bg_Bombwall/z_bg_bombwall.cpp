extern "C" {
#include "z_bg_bombwall.hpp"

#define FLAGS 0x80400000

#define PARAMS_GET_U(params, shift, count) ((((u16)(params)) >> (shift)) & ((1 << (count)) - 1))
// play->sceneId is not in the PlayState header yet (raw offset +0x104, u16)
#define PLAY_SCENE_ID(play) (*(u16*)((u8*)(play) + 0x104))

// Helpers not declared in include/ (3DS signatures; include/ prototypes are truncated)
void Collider_InitTris(PlayState* play, ColliderTris* collider);
void Collider_SetTris(PlayState* play, ColliderTris* collider, Actor* actor, void* src, ColliderTrisElement* elements);
void Collider_SetTrisVertices(ColliderTris* collider, s32 index, Vec3f* a, Vec3f* b, Vec3f* c);
s32 Collider_DestroyTris(PlayState* play, ColliderTris* collider);

void BgBombwall_Init(Actor* actor, GameState* state);
void BgBombwall_Destroy(Actor* actor, GameState* state);
void BgBombwall_Update(Actor* actor, GameState* state);
void BgBombwall_Draw(Actor* actor, GameState* state);

void FUN_003f5a3c(BgBombwall* self, PlayState* play); // N64: func_8086ED70 (wait for bomb hit)
void FUN_003cc660(BgBombwall* self, PlayState* play);

ActorInit Bg_Bombwall_InitVars = {
    ACTOR_BG_BOMBWALL,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_GAMEPLAY_FIELD_KEEP,
    sizeof(BgBombwall),
    (ActorFunc)BgBombwall_Init,
    (ActorFunc)BgBombwall_Destroy,
    (ActorFunc)BgBombwall_Update,
    (ActorFunc)BgBombwall_Draw,
};

// 3DS data lives in data/z_Bg_Bom_Guard.data.s (ROM 0x00510AD8..0x00510BD4); do not redefine it here.
GLOBAL_ASM("../src/game/actors/Bg_Bombwall/bb_symbols.s")
extern ColliderTrisElementInit bbTrisElementsInit[];
extern ColliderTrisInit bbTrisInit;
extern InitChainEntry bbInitChain[];
extern Vec3s bbDustOffsets[];

static void BgBombwall_InitDynapoly(Actor* thisx, PlayState* play) {
    BgBombwall* self = (BgBombwall*)thisx;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init((DynaPolyActor*)thisx, 0);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc((DynaPolyActor*)thisx, (GameState*)play, 0), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
}

static void BgBombwall_RotateVec(Vec3f* arg0, Vec3f* arg1, f32 arg2, f32 arg3) {
    arg0->x = (arg1->z * arg2) + (arg1->x * arg3);
    arg0->y = arg1->y;
    arg0->z = (arg1->z * arg3) - (arg1->x * arg2);
}

static void BgBombwall_DestroyCollision(Actor* thisx, PlayState* play) {
    BgBombwall* self = (BgBombwall*)thisx;
    if (self->colFlags & 2) {
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
        self->colFlags &= ~2;
    }
    if (self->colFlags & 1) {
        Collider_DestroyTris(play, &self->collider);
        self->colFlags &= ~1;
    }
}

static void BgBombwall_SetBroken(BgBombwall* self, PlayState* play) {
    self->skelAnimModel = self->skelAnimModelBroken;
    BgBombwall_DestroyCollision(&self->dyna.actor, play);
    self->actionFunc = NULL;
}

static void BgBombwall_SetupWait(BgBombwall* self, PlayState* play) {
    self->skelAnimModel = self->skelAnimModelIntact;
    self->actionFunc = FUN_003f5a3c;
}

void BgBombwall_Init(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    BgBombwall* self = (BgBombwall*)actor;
    Vec3f vecs[3];
    Vec3f sp80;
    s32 i;
    s32 j;
    f32 sin = Math_SinS(self->dyna.actor.shape.rot.y);
    f32 cos = Math_CosS(self->dyna.actor.shape.rot.y);

    Actor_ProcessInitChain(&self->dyna.actor, bbInitChain);
    Actor_SetScale(&self->dyna.actor, 0.1f);
    self->skelAnimModel = NULL;

    if (PLAY_SCENE_ID(play) != 0x60) {
        FUN_00372f38(&self->dyna.actor, play, &self->skelAnimModelIntact, 7,
                                                &self->skelAnimModelBroken, 6, 0);
    } else if ((self->dyna.actor.params & 0xF) == 3) {
        FUN_00372f38(&self->dyna.actor, play, &self->skelAnimModelIntact, 0x11,
                                                &self->skelAnimModelBroken, 0x10, 0);
    } else {
        FUN_00372f38(&self->dyna.actor, play, &self->skelAnimModelIntact, 0xF,
                                                &self->skelAnimModelBroken, 0xE, 0);
    }

    if (Flags_GetSwitch(play, PARAMS_GET_U(self->dyna.actor.params, 0, 6))) {
        BgBombwall_SetBroken(self, play);
    } else {
        BgBombwall_InitDynapoly(actor, play);
        ((BgBombwall*)actor)->colFlags |= 2;
        Collider_InitTris(play, &self->collider);
        Collider_SetTris(play, &self->collider, &self->dyna.actor, &bbTrisInit, self->colliderElements);

        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                sp80.x = bbTrisInit.elements[i].dim.vtx[j].x;
                sp80.y = bbTrisInit.elements[i].dim.vtx[j].y;
                sp80.z = bbTrisInit.elements[i].dim.vtx[j].z + 2.0f;

                BgBombwall_RotateVec(&vecs[j], &sp80, sin, cos);

                vecs[j].x += actor->world.pos.x;
                vecs[j].y += actor->world.pos.y;
                vecs[j].z += actor->world.pos.z;
            }
            Collider_SetTrisVertices(&self->collider, i, &vecs[0], &vecs[1], &vecs[2]);
        }

        self->colFlags |= 1;
        BgBombwall_SetupWait(self, play);
    }
}

void BgBombwall_Destroy(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    BgBombwall* self = (BgBombwall*)actor;

    if (self->colFlags & 2) {
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
        self->colFlags &= ~2;
    }
    if (self->colFlags & 1) {
        Collider_DestroyTris(play, &self->collider);
        self->colFlags &= ~1;
    }
    FUN_00350f34(actor, &self->skelAnimModelIntact, &self->skelAnimModelBroken, 0);
    self->skelAnimModel = NULL;
}

void BgBombwall_Update(Actor* actor, GameState* state) {
    BgBombwall* self = (BgBombwall*)actor;

    if (self->actionFunc != NULL) {
        self->actionFunc(self, (PlayState*)state);
    }
}

void BgBombwall_Draw(Actor* actor, GameState* state) {
    BgBombwall* self = (BgBombwall*)actor;
    MTX34 mtx;

    MTX34CopyAsm(&mtx, &self->dyna.actor.modelMtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&mtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}


void FUN_003cc660(BgBombwall* self, PlayState* play) {
    if (self->timer > 0) {
        self->timer--;
    } else {
        BgBombwall_SetBroken(self, play);
        if ((self->dyna.actor.params << 16) < 0) {
            FUN_00372244((u8*)play + 0x5FCC, 0x1E, 0x01000487);
        }
    }
}

static void BgBombwall_SpawnDust(BgBombwall* self, PlayState* play) {
    s16 rand;
    s16 rand2;
    Vec3f sp88;
    s32 i;
    Vec3f* pos = &self->dyna.actor.world.pos;
    f32 sin = Math_SinS(self->dyna.actor.shape.rot.y);
    f32 cos = Math_CosS(self->dyna.actor.shape.rot.y);
    f32 fx;
    f32 fz;

    for (i = 0; i < 7; i++) {
        fx = bbDustOffsets[i].x;
        fz = bbDustOffsets[i].z;
        sp88.x = ((fz * sin) + (fx * cos)) + pos->x;
        sp88.y = bbDustOffsets[i].y + pos->y;
        sp88.z = ((fz * cos) - (fx * sin)) + pos->z;
        rand = ((s16)(Rand_ZeroOne() * 120.0f)) + 0x14;
        rand2 = ((s16)(Rand_ZeroOne() * 240.0f)) + 0x14;
        FUN_0037378c(play, &sp88, 50.0f, 2, rand, rand2, 1);
    }

    sp88.x = pos->x;
    sp88.y = pos->y + 90.0f;
    sp88.z = pos->z + 15.0f;
    FUN_0037378c(play, &sp88, 40.0f, 4, 0xA, 0x32, 1);
}

static void BgBombwall_SetupBreak(BgBombwall* self, PlayState* play) {
    self->skelAnimModel = self->skelAnimModelIntact;
    self->timer = 1;
    BgBombwall_SpawnDust(self, play);
    self->actionFunc = FUN_003cc660;
}

// N64 func_8086ED70. Matching needs the N64 `play2` param pattern with play2 passed straight to CollisionCheck_SetAC
// (otherwise armcc picks r0 for the acFlags byte and r0/r1 for the 600.0f bit-compare instead of r1 / r1,r2).
void FUN_003f5a3c(BgBombwall* self, PlayState* play2) {
    PlayState* play = play2;

    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        BgBombwall_SetupBreak(self, play);
        Flags_SetSwitch(play, PARAMS_GET_U(self->dyna.actor.params, 0, 6));
    } else if (self->dyna.actor.xzDistToPlayer < 600.0f) {
        CollisionCheck_SetAC(play2, &play->colChkCtx, &self->collider.base);
    }
}
}
