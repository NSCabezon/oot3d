extern "C" {
#include "z_bg_spot18_obj.hpp"

#define FLAGS 0x80400000

void BgSpot18Obj_Init(Actor* actor, GameState* state);
void BgSpot18Obj_Destroy(Actor* actor, GameState* state);
void BgSpot18Obj_Update(Actor* actor, GameState* state);
void BgSpot18Obj_Draw(Actor* actor, GameState* state);

s32 FUN_001ffc9c(BgSpot18Obj* self, PlayState* play); // N64: func_808B8910
s32 FUN_001ffd00(BgSpot18Obj* self, PlayState* play); // N64: func_808B8A5C
s32 FUN_00257bc4(BgSpot18Obj* self, PlayState* play); // N64: func_808B8A98
s32 FUN_00218d60(BgSpot18Obj* self, PlayState* play); // N64: func_808B8B08
s32 FUN_001ffd28(BgSpot18Obj* self, PlayState* play); // N64: func_808B8BB4
s32 FUN_001c41d0(BgSpot18Obj* self, PlayState* play); // N64: func_808B8C90
void FUN_003bf20c(BgSpot18Obj* self, PlayState* play); // Empty action, N64: func_808B8DD0

ActorInit Bg_Spot18_Obj_InitVars = {
    ACTOR_BG_SPOT18_OBJ,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SPOT18_OBJ,
    sizeof(BgSpot18Obj),
    (ActorFunc)BgSpot18Obj_Init,
    (ActorFunc)BgSpot18Obj_Destroy,
    (ActorFunc)BgSpot18Obj_Update,
    (ActorFunc)BgSpot18Obj_Draw,
};

static u8 D_808B90F0[2][2] = { { 0x01, 0x01 }, { 0x01, 0x00 } };

static f32 D_808B90F4[] = {
    0.1f,
    0.1f,
};

static u32 D_808B9104[] = {
    0,
    0,
};

static BgSpot18ObjInitFunc D_808B913C[] = {
    FUN_001ffd28,
    FUN_001c41d0,
};

static BgSpot18ObjInitFunc D_808B910C[] = {
    FUN_00257bc4,
    FUN_001ffc9c,
    FUN_001ffd00,
    FUN_00218d60,
};

static InitChainEntry sInitChain1[] = {
    ICHAIN_F32(minVelocityY, -10, ICHAIN_CONTINUE),           ICHAIN_F32(gravity, -4, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1400, ICHAIN_CONTINUE),     ICHAIN_F32(uncullZoneScale, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 800, ICHAIN_STOP),
};

static InitChainEntry sInitChain2[] = {
    ICHAIN_F32(uncullZoneForward, 1200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 700, ICHAIN_STOP),
};

s32 FUN_001ffc9c(BgSpot18Obj* self, PlayState* play) {
    s32 age = (LINK_AGE_IN_YEARS == YEARS_ADULT) ? 1 : 0;
    s32 val = D_808B90F0[((self->dyna.actor.params) & 0xF)][age];

    if (val == 0 || val == 1) {
        return val;
    }
    return 0;
}

s32 FUN_001ffd00(BgSpot18Obj* self, PlayState* play) {
    Actor_SetScale(&self->dyna.actor, D_808B90F4[((self->dyna.actor.params) & 0xF)]);
    return 1;
}

s32 FUN_00257bc4(BgSpot18Obj* self, PlayState* play) {
    CollisionHeader* colHeader = NULL;
    void* colData;

    DynaPolyActor_Init(&self->dyna, 0);
    if ((self->dyna.actor.params & 0xF) == 0) {
        FUN_00372f38(&self->dyna.actor, play, &self->skelAnimModel, 4, 0);
        colData = DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 4);
    } else {
        FUN_00372f38(&self->dyna.actor, play, &self->skelAnimModel, 3, 0);
        colData = DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 3);
    }
    ReadCollisionHeaderFromZsi(colData, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);
    return 1;
}

s32 FUN_00218d60(BgSpot18Obj* self, PlayState* play) {
    self->dyna.actor.flags |= D_808B9104[((self->dyna.actor.params) & 0xF)];
    return 1;
}

s32 FUN_001ffd28(BgSpot18Obj* self, PlayState* play) {
    Actor_ProcessInitChain(&self->dyna.actor, sInitChain1);

    if (LINK_AGE_IN_YEARS == YEARS_CHILD) {
        self->actionFunc = (BgSpot18ObjActionFunc)0x00207DE8;
    } else if (Flags_GetSwitch(play, (((self->dyna.actor.params) >> 8) & 0x3F))) {
        self->actionFunc = (BgSpot18ObjActionFunc)0x00207DE8;
        self->dyna.actor.world.pos.x = (Math_SinS(self->dyna.actor.world.rot.y) * 80.0f) + self->dyna.actor.home.pos.x;
        self->dyna.actor.world.pos.z = (Math_CosS(self->dyna.actor.world.rot.y) * 80.0f) + self->dyna.actor.home.pos.z;
    } else {
        self->unk_1C0 = 30;
        self->actionFunc = (BgSpot18ObjActionFunc)0x001852E8;
    }
    return 1;
}

s32 FUN_001c41d0(BgSpot18Obj* self, PlayState* play) {
    BgSpot18ObjActionFunc f;
    Actor_ProcessInitChain(&self->dyna.actor, sInitChain2);
    f = FUN_003bf20c;
    s32 r = 1;
    self->actionFunc = f;
    return r;
}

void BgSpot18Obj_Init(Actor* actor, GameState* state) {
    BgSpot18Obj* self = (BgSpot18Obj*)actor;
    PlayState* play = (PlayState*)state;
    u32 i;

    for (i = 0; i < ARRAY_COUNTU(D_808B910C); i++) {
        if (D_808B910C[i](self, play) == 0) {
            Actor_Kill(actor);
            return;
        }
    }
    if ((D_808B913C[(self->dyna.actor.params & 0xF)] != NULL) &&
        (!D_808B913C[(self->dyna.actor.params & 0xF)](self, play))) {
        Actor_Kill(actor);
    }
}

void BgSpot18Obj_Destroy(Actor* actor, GameState* state) {
    BgSpot18Obj* self = (BgSpot18Obj*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgSpot18Obj_Update(Actor* actor, GameState* state) {
    BgSpot18Obj* self = (BgSpot18Obj*)actor;

    if (self->unk_1C0 > 0) {
        self->unk_1C0--;
    }
    self->actionFunc(self, (PlayState*)state);
}

void BgSpot18Obj_Draw(Actor* actor, GameState* state) {
    BgSpot18Obj* self = (BgSpot18Obj*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&modelMtx);
    self->skelAnimModel->FUN_00372170(0);
}
}
