extern "C" {
#include "z_obj_hsblock.hpp"

#define FLAGS 0x00000000

#define ACTOR_FLAG_UPDATE_CULLING_DISABLED 0x10


void ObjHsblock_Init(Actor* actor, GameState* state);
void ObjHsblock_Destroy(Actor* actor, GameState* state);
void ObjHsblock_Update(Actor* actor, GameState* state);
void ObjHsblock_Draw(Actor* actor, GameState* state);

void FUN_003c07a8(ObjHsblock* self, PlayState* play); // N64: func_80B93DF4
void FUN_003f59b8(ObjHsblock* self, PlayState* play); // N64: func_80B93E5C

ActorInit Obj_Hsblock_InitVars = {
    ACTOR_OBJ_HSBLOCK,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_D_HSBLOCK,
    sizeof(ObjHsblock),
    (ActorFunc)ObjHsblock_Init,
    (ActorFunc)ObjHsblock_Destroy,
    (ActorFunc)ObjHsblock_Update,
    (ActorFunc)ObjHsblock_Draw,
};

// Leading word of the original data dump (0x00535134); nothing references it, kept so the tables below land at their original addresses.
__attribute__((used)) static u32 sData_00535134 = 0x00377DA5;

// Actor_SetFocus height per params & 3 (N64 D_80B940C0)
static f32 sFocusOffset[] = { 85.0f, 85.0f, 0.0f };

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 2000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 400, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 2000, ICHAIN_STOP),
};

// Index into the DynaPoly collision list per params & 3 (N64 sCollisionHeaders: post, post, target)
static s32 sCollisionHeaderIdx[] = { 1, 1, 0 };

void ObjHsblock_Init(Actor* actor, GameState* state) {
    ObjHsblock* self = (ObjHsblock*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader;
    void* colInfo;

    FUN_00372f38(actor, play, &self->skelAnimModel, 1, &self->skelAnimModel2, 0, 0);
    colInfo = DynaPolyInfo_Alloc(&self->dyna, state, sCollisionHeaderIdx[actor->params & 3]);
    colHeader = NULL;
    DynaPolyActor_Init(&self->dyna, 0);
    ReadCollisionHeaderFromZsi(colInfo, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    Actor_ProcessInitChain(actor, sInitChain);

    if ((s32)(actor->params << 26) < 0) {
        Actor_SpawnAsChild(&play->actorCtx, actor, (PlayState*)state, ACTOR_OBJ_ICE_POLY, actor->world.pos.x, actor->world.pos.y,
                           actor->world.pos.z, actor->world.rot.x, actor->world.rot.y, actor->world.rot.z, 1);
    }

    switch (actor->params & 3) {
        case 0:
        case 2:
            self->actionFunc = NULL;
            break;
        case 1:
            if (Flags_GetSwitch(play, (actor->params >> 8) & 0x3F)) {
                self->actionFunc = NULL;
            } else {
                self->dyna.actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
                self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y - 105.0f;
                self->actionFunc = FUN_003c07a8;
            }
            break;
    }
}

void ObjHsblock_Destroy(Actor* actor, GameState* state) {
    ObjHsblock* self = (ObjHsblock*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, &self->skelAnimModel2, 0);
}

void FUN_003c07a8(ObjHsblock* self, PlayState* play) {
    if (Flags_GetSwitch(play, (self->dyna.actor.params >> 8) & 0x3F)) {
        self->actionFunc = FUN_003f59b8;
    }
}

void FUN_003f59b8(ObjHsblock* self, PlayState* play) {
    Math_SmoothStepToF(&self->dyna.actor.velocity.y, 16.0f, 0.1f, 0.8f, 0.0f);
    if (__fabsf(Math_SmoothStepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y, 0.3f,
                                 self->dyna.actor.velocity.y, 0.3f)) < 0.001f) {
        self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y;
        self->actionFunc = NULL;
        self->dyna.actor.flags &= ~ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    }
}

void ObjHsblock_Update(Actor* actor, GameState* state) {
    ObjHsblock* self = (ObjHsblock*)actor;
    PlayState* play = (PlayState*)state;

    if (self->actionFunc != NULL) {
        self->actionFunc(self, play);
    }
    Actor_SetFocus(actor, sFocusOffset[actor->params & 3]);
}

void ObjHsblock_Draw(Actor* actor, GameState* state) {
    ObjHsblock* self = (ObjHsblock*)actor;
    PlayState* play = (PlayState*)state;
    MTX34 modelMtx;
    u8 isFireTemple;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    // N64 picks sFireTempleColor here for gDPSetEnvColor; the color use is gone on 3DS but the
    // sceneNum load survives (dead ldrh in the original).
    isFireTemple = (play->sceneNum == SCENE_HIDAN);

    if ((actor->params & 3) != 0) {
        FUN_00357fd0(GET_PLAYER(play), actor->unk_178, &actor->world.pos);
    }

    switch (actor->params & 3) {
        case 0:
        case 1:
            self->skelAnimModel->SetUnkAC(1);
            self->skelAnimModel->FUN_003721e0(&modelMtx);
            self->skelAnimModel->FUN_00372170(0);
            break;
        case 2:
            self->skelAnimModel2->SetUnkAC(1);
            self->skelAnimModel2->FUN_003721e0(&modelMtx);
            self->skelAnimModel2->FUN_00372170(0);
            break;
    }
}
}
