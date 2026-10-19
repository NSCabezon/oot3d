extern "C" {
extern __pure float _fabsf(float);
static __inline __pure float baj_fabsf(float f) {
    return _fabsf(f);
}
#include "z_bg_jya_amishutter.hpp"

#define FLAGS 0x80000000

void BgJyaAmishutter_Init(Actor* actor, GameState* state);
void BgJyaAmishutter_Destroy(Actor* actor, GameState* state);
void BgJyaAmishutter_Update(Actor* actor, GameState* state);
void BgJyaAmishutter_Draw(Actor* actor, GameState* state);

void FUN_003cc1c0(BgJyaAmishutter* self);
void FUN_003af520(BgJyaAmishutter* self);
void FUN_003bce70(BgJyaAmishutter* self);
void FUN_003bafe8(BgJyaAmishutter* self);
__attribute__((noinline)) void FUN_0015ddd4(BgJyaAmishutter* self);

extern InitChainEntry sBgJyaAmishutterInitChain[];
void BgJyaAmishutter_Init(Actor* actor, GameState* state) {
    BgJyaAmishutter* self = (BgJyaAmishutter*)actor;
    PlayState* play = (PlayState*)state;
    void* colInfo;

    FUN_00372f38(actor, play, &self->skelAnimModel, 0x22, 0);
    colInfo = DynaPolyInfo_Alloc(&self->dyna, state, 5);
    {
        CollisionHeader* colHeader = NULL;
        DynaPolyActor_Init(&self->dyna, 0);
        ReadCollisionHeaderFromZsi(colInfo, &colHeader);
        self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    }
    Actor_ProcessInitChain(actor, sBgJyaAmishutterInitChain);
    self->actionFunc = FUN_003cc1c0;
}



ActorInit Bg_Jya_Amishutter_InitVars = {
    ACTOR_BG_JYA_AMISHUTTER,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_JYA_OBJ,
    sizeof(BgJyaAmishutter),
    (ActorFunc)BgJyaAmishutter_Init,
    (ActorFunc)BgJyaAmishutter_Destroy,
    (ActorFunc)BgJyaAmishutter_Update,
    (ActorFunc)BgJyaAmishutter_Draw,
};

InitChainEntry sBgJyaAmishutterInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_STOP),
};



void BgJyaAmishutter_Destroy(Actor* actor, GameState* state) {
    BgJyaAmishutter* self = (BgJyaAmishutter*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void FUN_003cc1c0(BgJyaAmishutter* self) {
    if ((self->dyna.actor.xzDistToPlayer < 60.0f) && (baj_fabsf(self->dyna.actor.yDistToPlayer) < 30.0f)) {
        self->actionFunc = FUN_003af520;
    }
}

void FUN_003af520(BgJyaAmishutter* self) {
    if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y + 100.0f, 3.0f)) {
        self->actionFunc = FUN_003bce70;
        Audio_PlayActorSound2(&self->dyna.actor, 0x0100018E);
    } else {
        FUN_00373264(&self->dyna.actor, 0x0100018D);
    }
}

void FUN_003bce70(BgJyaAmishutter* self) {
    if (self->dyna.actor.xzDistToPlayer > 300.0f) {
        self->actionFunc = FUN_003bafe8;
    }
}

void FUN_003bafe8(BgJyaAmishutter* self) {
    if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y, 3.0f)) {
        FUN_0015ddd4(self);
        Audio_PlayActorSound2(&self->dyna.actor, 0x0100018E);
    } else {
        FUN_00373264(&self->dyna.actor, 0x0100018D);
    }
}

void FUN_0015ddd4(BgJyaAmishutter* self) {
    self->actionFunc = FUN_003cc1c0;
}


void BgJyaAmishutter_Update(Actor* actor, GameState* state) {
    BgJyaAmishutter* self = (BgJyaAmishutter*)actor;

    self->actionFunc(self);
}

void BgJyaAmishutter_Draw(Actor* actor, GameState* state) {
    BgJyaAmishutter* self = (BgJyaAmishutter*)actor;
    PlayState* play = (PlayState*)state;
    MTX34 mtx;

    MTX34CopyAsm(&mtx, &actor->modelMtx);
    if (self->skelAnimModel != NULL) {
        FUN_00357fd0(GET_PLAYER(play), actor->unk_178, &actor->world.pos);
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&mtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}
}
