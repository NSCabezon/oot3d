extern "C" {
#include "z_bg_haka_sgami.hpp"

#define FLAGS 0x80400011

void BgHakaSgami_Init(Actor* thisx, GameState* state);
void BgHakaSgami_Destroy(Actor* thisx, GameState* state);
void BgHakaSgami_Update(Actor* thisx, GameState* state);
void FUN_00154c10(BgHakaSgami* self, PlayState* play); // SetupSpin
void FUN_00127c04(BgHakaSgami* self, PlayState* play); // Spin (asm, see #if 0 draft)
void FUN_0028a8f4(Actor* thisx, GameState* state);     // Draw

ActorInit Bg_Haka_Sgami_InitVars = {
    ACTOR_BG_HAKA_SGAMI,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(BgHakaSgami),
    (ActorFunc)BgHakaSgami_Init,
    (ActorFunc)BgHakaSgami_Destroy,
    (ActorFunc)BgHakaSgami_Update,
    NULL,
};

// Static data (3DS ROM 0x005111FC..0x00511390). FUN_00127c04 (asm) reads sTrisInit and
// blureEffectVertices1 by absolute address.
static ColliderTrisElementInit sTrisElementsInit[] = {
    {
        { 2, { 0x20000000, 0, 4 }, { 0x00000000, 0, 0 }, 1, 0, 0 },
        { { { 365.0f, 45.0f, 27.0f }, { 130.0f, 45.0f, 150.0f }, { 290.0f, 45.0f, 145.0f } } },
    },
    {
        { 2, { 0x20000000, 0, 4 }, { 0x00000000, 0, 0 }, 1, 0, 0 },
        { { { 250.0f, 45.0f, 90.0f }, { 50.0f, 45.0f, 80.0f }, { 160.0f, 45.0f, 160.0f } } },
    },
    {
        { 2, { 0x20000000, 0, 4 }, { 0x00000000, 0, 0 }, 1, 0, 0 },
        { { { -305.0f, 33.0f, -7.0f }, { -220.0f, 33.0f, 40.0f }, { -130.0f, 33.0f, -5.0f } } },
    },
    {
        { 2, { 0x20000000, 0, 4 }, { 0x00000000, 0, 0 }, 1, 0, 0 },
        { { { -190.0f, 33.0f, 40.0f }, { -30.0f, 33.0f, 15.0f }, { -70.0f, 33.0f, -30.0f } } },
    },
};



// Unreferenced in C (0x005112FC).


static u8 sP1StartColor[] = { 250, 250, 250, 200 };
static u8 sP2StartColor[] = { 200, 200, 200, 130 };
static u8 sP1EndColor[] = { 200, 200, 200, 60 };
static u8 sP2EndColor[] = { 150, 150, 150, 20 };

// 0x005112FC: { 5.0f, 3.0f } is unreferenced and precedes sColChkInfoInit in ROM. As separate
// statics armcc always emits the colliders' info right after the colours, so both are kept in
// one object to pin the layout (debt: real origin of the two floats unknown).
static struct {
    f32 unk_00[2];
    CollisionCheckInfoInit info;
} sColChkInfoInit = { { 5.0f, 3.0f }, { 0, 80, 130, MASS_IMMOVABLE } };

static ColliderTrisInit sTrisInit = {
    { 0x0A, 0x11, 0x00, 0x00, 0x20, 0x02 },
    4,
    sTrisElementsInit,
};

static ColliderCylinderInit sCylinderInit = {
    { 0x0C, 0x00, 0x39, 0x39, 0x20, 0x01 },
    { 2, { 0, 0, 0 }, { 0x0001F8A4, 0, 0 }, 0, 1, 1 },
    { 80.0f, 130.0f, 0.0f, { 0.0f, 0.0f, 0.0f } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneScale, 1000, ICHAIN_CONTINUE),
    ICHAIN_U8(targetMode, 4, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

__attribute__((used)) static Vec3f blureEffectVertices2[] = {
    { -20.0f, 50.0f, 130.0f },
    { -50.0f, 62.0f, 20.0f },
};

__attribute__((used)) static Vec3f blureEffectVertices1[] = {
    { 380.0f, 50.0f, 50.0f },
    { 310.0f, 62.0f, 0.0f },
};


// 3DS EffectBlure init params (N64 EffectBlureInit1). The vertex array element has an
// out-of-line empty constructor in ROM (FUN_003fd780, aliased in data_syms.s).
struct BlureInitVertex {
    BlureInitVertex();
    u8 data[0x24];
};

typedef struct {
    /* 0x000 */ s32 unk_00;
    /* 0x004 */ BlureInitVertex vtx[16];
    /* 0x244 */ u8 p1StartColor[4];
    /* 0x248 */ u8 p2StartColor[4];
    /* 0x24C */ u8 p1EndColor[4];
    /* 0x250 */ u8 p2EndColor[4];
    /* 0x254 */ s32 elemDuration;
    /* 0x258 */ s32 unkFlag;
    /* 0x25C */ u8 calcMode;
    /* 0x260 */ s32 unk_260;
    /* 0x264 */ u8 unk_264;
} BgHakaSgamiBlureInit; // size = 0x268


void Collider_InitTris(PlayState* play, ColliderTris* collider);
void Collider_SetTris(PlayState* play, ColliderTris* collider, Actor* actor, void* src, ColliderTrisElement* elements);
s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
s32 Effect_Add(PlayState* play, s32* pIndex, s32 type, u8 arg3, u8 arg4, void* initParams);

void BgHakaSgami_Init(Actor* thisx, GameState* state) {
    BgHakaSgami* self = (BgHakaSgami*)thisx;
    BgHakaSgamiBlureInit blureInit;
    s32 i;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    self->unk_151 = thisx->params;
    s8 lens = self->unk_151;
    thisx->params = (thisx->params >> 8) & 0xFF;
    self->actor.unk_19A = 1;
    self->actor.unk_19B = 3;
    if (lens != 0) {
        thisx->flags |= 0x80;
    }

    Collider_InitTris((PlayState*)state, &self->scytheCollider);
    Collider_SetTris((PlayState*)state, &self->scytheCollider, &self->actor, &sTrisInit, self->scytheColliderElements);
    Collider_InitCylinder((PlayState*)state, &self->scytheCenterCollider);
    Collider_SetCylinder((PlayState*)state, &self->scytheCenterCollider, &self->actor, &sCylinderInit);

    self->scytheCenterCollider.dim.pos = self->actor.world.pos;
    CollisionCheck_SetInfo(&self->actor.colChkInfo, NULL, &sColChkInfoInit.info);

    for (i = 0; i < 4; i++) {
        blureInit.p1StartColor[i] = sP1StartColor[i];
        blureInit.p2StartColor[i] = sP2StartColor[i];
        blureInit.p1EndColor[i] = sP1EndColor[i];
        blureInit.p2EndColor[i] = sP2EndColor[i];
    }

    blureInit.elemDuration = 15;
    if (self->unk_151 != 0) {
        blureInit.unkFlag = 1;
    } else {
        blureInit.unkFlag = 0;
    }
    blureInit.calcMode = 2;
    blureInit.unk_264 = 15;

    Effect_Add((PlayState*)state, &self->blureEffectIndex[0], 1, 0, 0, &blureInit);
    Effect_Add((PlayState*)state, &self->blureEffectIndex[1], 1, 0, 0, &blureInit);

    if (self->actor.params == 0) {
        self->requiredObjectSlot = Object_GetIndex(&((PlayState*)state)->objectCtx, 0x69); // OBJECT_HAKA_OBJECTS
        thisx->flags &= ~1;
    } else {
        self->requiredObjectSlot = Object_GetIndex(&((PlayState*)state)->objectCtx, 0x6B); // OBJECT_HAKACH_OBJECTS
        self->scytheCenterCollider.dim.radius = 30.0f;
        self->scytheCenterCollider.dim.height = 70.0f;
        Actor_SetFocus(&self->actor, 40.0f);
    }

    if (self->requiredObjectSlot < 0) {
        Actor_Kill(&self->actor);
    } else {
        self->actionFunc = FUN_00154c10;
    }
}

// SetupSpin (was FUN_00154c10 in functions_140000s.cpp)
#define SCYTHE_SPIN_TIME 48 // N64: 32

s32 Object_IsLoaded(ObjectContext* objectCtx, s32 bankIndex);
s32 Collider_DestroyTris(PlayState* play, ColliderTris* collider);
void Effect_Delete(PlayState* play, s32 index);
void MTX34CopyAsm(MTX34* dst, MTX34* src);

void BgHakaSgami_Destroy(Actor* thisx, GameState* state) {
    BgHakaSgami* self = (BgHakaSgami*)thisx;
    PlayState* play = (PlayState*)state;

    Effect_Delete(play, self->blureEffectIndex[0]);
    Effect_Delete(play, self->blureEffectIndex[1]);
    Collider_DestroyTris(play, &self->scytheCollider);
    Collider_DestroyCylinder(play, &self->scytheCenterCollider);
    FUN_00350f34(thisx, &self->skelAnimModel, 0);
}

void BgHakaSgami_Update(Actor* thisx, GameState* state) {
    BgHakaSgami* self = (BgHakaSgami*)thisx;
    PlayState* play = (PlayState*)state;
    Player* player = GET_PLAYER(play);

    if (!(player->stateFlags1 & 0x300000C0) || (self->actionFunc == FUN_00154c10)) {
        self->actionFunc(self, play);
    }
}

void FUN_00154c10(BgHakaSgami* self, PlayState* play) {
    if (Object_IsLoaded(&play->objectCtx, self->requiredObjectSlot)) {
        self->actor.objBankIndex = self->requiredObjectSlot;
        self->actor.draw = FUN_0028a8f4;
        if (self->actor.params == 0) {
            FUN_00372f38(&self->actor, play, &self->skelAnimModel, 0x1E, 0);
        } else {
            FUN_00372f38(&self->actor, play, &self->skelAnimModel, 4, 0);
        }
        self->timer = SCYTHE_SPIN_TIME;
        self->actor.flags &= ~0x10;
        self->actionFunc = FUN_00127c04;
    }
}

// Draw (was FUN_0028a8f4 in functions_280000s.cpp)
void FUN_0028a8f4(Actor* thisx, GameState* state) {
    BgHakaSgami* self = (BgHakaSgami*)thisx;
    MTX34 mtx;

    MTX34CopyAsm(&mtx, &thisx->modelMtx);
    if (self->unk_151 != 0) {
        if (self->skelAnimModel != NULL) {
            self->skelAnimModel->SetUnkAC(1);
            self->skelAnimModel->FUN_003721e0(&mtx);
            self->skelAnimModel->FUN_00372170(0);
        }
    } else {
        if (self->skelAnimModel != NULL) {
            self->skelAnimModel->SetUnkAC(1);
            self->skelAnimModel->FUN_003721e0(&mtx);
            self->skelAnimModel->FUN_00372170(0);
        }
    }
}

#if 0
// Spin draft (Haiku stage 1, unmatched). Real code still GLOBAL_ASM in functions_120000s.cpp.



void FUN_00127c04(BgHakaSgami* self, PlayState* play) {
    s32 i;
    s32 j;
    Vec3f scytheVertices[3];
    f32 actorRotYSin;
    f32 actorRotYCos;
    s32 iterateCount;
    ColliderTrisElementInit* elementInit;

    if (self->timer != 0) {
        self->timer--;
    }

    self->actor.shape.rot.y += ((s16)(512.0f * __hardfp_sinf(self->timer * (M_PI / 16.0f))) + 0x400) >> 1;

    if (self->timer == 0) {
        self->timer = SCYTHE_SPIN_TIME;
    }

    actorRotYSin = Math_SinS(self->actor.shape.rot.y);
    actorRotYCos = Math_CosS(self->actor.shape.rot.y);

    iterateCount = (self->actor.params != 0) ? 4 : 2;

    for (i = iterateCount - 2; i < iterateCount; i++) {
        elementInit = &sTrisInit.elements[i];

        for (j = 0; j < 3; j++) {
            scytheVertices[j].x = self->actor.world.pos.x + elementInit->dim.vtx[j].z * actorRotYSin +
                                  elementInit->dim.vtx[j].x * actorRotYCos;
            scytheVertices[j].y = self->actor.world.pos.y + elementInit->dim.vtx[j].y;
            scytheVertices[j].z = self->actor.world.pos.z + elementInit->dim.vtx[j].z * actorRotYCos -
                                  elementInit->dim.vtx[j].x * actorRotYSin;
        }

        Collider_SetTrisVertices(&self->scytheCollider, i, &scytheVertices[0], &scytheVertices[1], &scytheVertices[2]);

        for (j = 0; j < 3; j++) {
            scytheVertices[j].x = (2 * self->actor.world.pos.x) - scytheVertices[j].x;
            scytheVertices[j].z = (2 * self->actor.world.pos.z) - scytheVertices[j].z;
        }

        Collider_SetTrisVertices(&self->scytheCollider, (i + 2) % 4, &scytheVertices[0], &scytheVertices[1],
                                 &scytheVertices[2]);
    }

    if ((self->unk_151 == 0) || play->actorCtx.lensActive) {
        scytheVertices[0].x = self->actor.world.pos.x + blureEffectVertices1[self->actor.params].z * actorRotYSin +
                              blureEffectVertices1[self->actor.params].x * actorRotYCos;
        scytheVertices[0].y = self->actor.world.pos.y + blureEffectVertices1[self->actor.params].y;
        scytheVertices[0].z = self->actor.world.pos.z + blureEffectVertices1[self->actor.params].z * actorRotYCos -
                              blureEffectVertices1[self->actor.params].x * actorRotYSin;
        scytheVertices[1].x = self->actor.world.pos.x + blureEffectVertices2[self->actor.params].z * actorRotYSin +
                              blureEffectVertices2[self->actor.params].x * actorRotYCos;
        scytheVertices[1].y = self->actor.world.pos.y + blureEffectVertices2[self->actor.params].y;
        scytheVertices[1].z = self->actor.world.pos.z + blureEffectVertices2[self->actor.params].z * actorRotYCos -
                              blureEffectVertices2[self->actor.params].x * actorRotYSin;
        EffectBlure_AddVertex(Effect_GetByIndex(self->blureEffectIndex[0]), &scytheVertices[0], &scytheVertices[1]);

        for (j = 0; j < 2; j++) {
            scytheVertices[j].x = (2 * self->actor.world.pos.x) - scytheVertices[j].x;
            scytheVertices[j].z = (2 * self->actor.world.pos.z) - scytheVertices[j].z;
        }

        EffectBlure_AddVertex(Effect_GetByIndex(self->blureEffectIndex[1]), &scytheVertices[0], &scytheVertices[1]);
    } else {
        Effect_GetByIndex(self->blureEffectIndex[0])->unk_25E = 0;
        Effect_GetByIndex(self->blureEffectIndex[1])->unk_25E = 0;
    }

    CollisionCheck_SetAT(play, &play->colChkCtx, &self->scytheCollider.base);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->scytheCenterCollider.base);
    CollisionCheck_SetAC(play, &play->colChkCtx, &self->scytheCenterCollider.base);
    FUN_00373264(&self->actor, 0x010001a3);
}

#endif
}
