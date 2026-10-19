extern "C" {
#include "z_bg_mori_kaitenkabe.hpp"

#define FLAGS 0x80400020

// Helpers not declared in include/ yet
s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
s32 Object_IsLoaded(ObjectContext* objectCtx, s32 slot);
s32 Player_InCsMode(PlayState* play);
extern __pure float _fabsf(float);
#define DEG_TO_BINANG(deg) ((s16)((deg) * 182.04445f))

// Moved from functions_100000s.cpp / functions_3E0000s.cpp (ld-pinned names kept)
__attribute__((noinline)) void FUN_0010feb0(BgMoriKaitenkabe* self);                                   // N64: BgMoriKaitenkabe_SetupWait
void BgMoriKaitenkaby_WaitForMoriTex(BgMoriKaitenkabe* self, PlayState* play); // N64: BgMoriKaitenkabe_WaitForMoriTex
void FUN_003f61dc(BgMoriKaitenkabe* self, PlayState* play);                    // N64: BgMoriKaitenkabe_Rotate

void BgMoriKaitenkabe_Init(Actor* actor, GameState* state);
void BgMoriKaitenkabe_Destroy(Actor* actor, GameState* state);
void BgMoriKaitenkabe_Update(Actor* actor, GameState* state);
void BgMoriKaitenkabe_Draw(Actor* actor, GameState* state);
void BgMoriKaitenkabe_Wait(BgMoriKaitenkabe* self, PlayState* play);

// Init chain lives in the data dump at 0x00513394 (no symbol in asm)
#define sInitChain ((InitChainEntry*)0x00513394)

GLOBAL_ASM("data/z_Bg_Mori_Kaitenkabe.data.s")

ActorInit Bg_Mori_Kaitenkabe_InitVars = {
    ACTOR_BG_MORI_KAITENKABE,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_MORI_OBJECTS,
    sizeof(BgMoriKaitenkabe),
    (ActorFunc)BgMoriKaitenkabe_Init,
    (ActorFunc)BgMoriKaitenkabe_Destroy,
    (ActorFunc)BgMoriKaitenkabe_Update,
    NULL,
};

void BgMoriKaitenkabe_Init(Actor* actor, GameState* state) {
    BgMoriKaitenkabe* self = (BgMoriKaitenkabe*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, 0);
    FUN_00372f38(actor, play, &self->skelAnimModel, 6, 0);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 4), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    self->moriTexObjectSlot = Object_GetIndex(&play->objectCtx, OBJECT_MORI_TEX);
    if (self->moriTexObjectSlot >= 0) {
        self->actionFunc = BgMoriKaitenkaby_WaitForMoriTex;
    } else {
        Actor_Kill(actor);
    }
}

void BgMoriKaitenkabe_Destroy(Actor* actor, GameState* state) {
    BgMoriKaitenkabe* self = (BgMoriKaitenkabe*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgMoriKaitenkabe_Update(Actor* actor, GameState* state) {
    BgMoriKaitenkabe* self = (BgMoriKaitenkabe*)actor;
    PlayState* play = (PlayState*)state;

    self->actionFunc(self, play);
}

void BgMoriKaitenkabe_Draw(Actor* actor, GameState* state) {
    BgMoriKaitenkabe* self = (BgMoriKaitenkabe*)actor;
    MTX34 modelMtx;
    Vec3f scale;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    scale.x = 1.002f;
    scale.y = 1.002f;
    scale.z = 1.002f;
    modelMtx.data[0][0] *= scale.x;
    modelMtx.data[1][0] *= scale.x;
    modelMtx.data[2][0] *= scale.x;
    modelMtx.data[0][1] *= scale.y;
    modelMtx.data[1][1] *= scale.y;
    modelMtx.data[2][1] *= scale.y;
    modelMtx.data[0][2] *= scale.z;
    modelMtx.data[1][2] *= scale.z;
    modelMtx.data[2][2] *= scale.z;
    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&modelMtx);
    self->skelAnimModel->FUN_00372170(0);
}

void BgMoriKaitenkaby_WaitForMoriTex(BgMoriKaitenkabe* self, PlayState* play) {
    if (Object_IsLoaded(&play->objectCtx, self->moriTexObjectSlot)) {
        self->actionFunc = BgMoriKaitenkabe_Wait;
        self->timer = 0;
        self->dyna.actor.draw = (ActorFunc)BgMoriKaitenkabe_Draw;
    }
}

void FUN_0010feb0(BgMoriKaitenkabe* self) {
    self->actionFunc = BgMoriKaitenkabe_Wait;
    self->timer = 0;
}

static inline void BgMoriKaitenkabe_CrossProduct(Vec3f* dest, Vec3f* v1, Vec3f* v2) {
    dest->x = (v1->y * v2->z) - (v1->z * v2->y);
    dest->y = (v1->z * v2->x) - (v1->x * v2->z);
    dest->z = (v1->x * v2->y) - (v1->y * v2->x);
}

void BgMoriKaitenkabe_Wait(BgMoriKaitenkabe* self, PlayState* play) {
    Vec3f push;
    Vec3f leverArm;
    Vec3f torque;
    Player* player = GET_PLAYER(play);

    if (self->dyna.unk_1A8 > 0.001f) {
        self->timer++;
        if ((self->timer > 28) && !Player_InCsMode(play)) {
            // SetupRotate (inlined)
            self->actionFunc = FUN_003f61dc;
            self->rotSpeed = 0.0f;
            self->rotYdeg = 0.0f;
            FUN_0036e980(play, &self->dyna.actor, 8);
            Math_Vec3f_Copy(&self->lockedPlayerPos, &player->actor.world.pos);
            push.x = Math_SinS(self->dyna.unk_1B0);
            push.y = 0.0f;
            push.z = Math_CosS(self->dyna.unk_1B0);
            leverArm.x = self->dyna.actor.world.pos.x - player->actor.world.pos.x;
            leverArm.y = 0.0f;
            leverArm.z = self->dyna.actor.world.pos.z - player->actor.world.pos.z;
            BgMoriKaitenkabe_CrossProduct(&torque, &push, &leverArm);
            self->rotDirection = (torque.y > 0.0f) ? 1.0f : -1.0f;
        }
    } else {
        self->timer = 0;
    }
    if (__fabsf(self->dyna.unk_1A8) > 0.001f) {
        self->dyna.unk_1A8 = 0.0f;
        player->stateFlags2 &= ~0x10;
    }
}

// _fabsf through a static inline wrapper: __fabsf directly swaps r0/r1 in the threshold compare
static __inline __pure float BgMoriKaitenkabe_Fabsf(float f) {
    return _fabsf(f);
}

void FUN_003f61dc(BgMoriKaitenkabe* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Actor* thisx = &self->dyna.actor;
    s16 rotY;

    Math_StepToF(&self->rotSpeed, 0.6f, 0.02f);
    if (Math_StepToF(&self->rotYdeg, self->rotDirection * 45.0f, self->rotSpeed)) {
        FUN_0010feb0(self);
        FUN_0036e980(play, thisx, 7);
        if (self->rotDirection > 0.0f) {
            thisx->home.rot.y += 0x2000;
        } else {
            thisx->home.rot.y -= 0x2000;
        }
        thisx->world.rot.y = thisx->shape.rot.y = thisx->home.rot.y;
        Audio_PlaySoundGeneral(0x010001EA, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
    } else {
        rotY = DEG_TO_BINANG(self->rotYdeg);
        thisx->world.rot.y = thisx->shape.rot.y = thisx->home.rot.y + rotY;
        Audio_PlaySoundGeneral(0x010001DA, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
    }
    if (BgMoriKaitenkabe_Fabsf(self->dyna.unk_1A8) > 0.001f) {
        self->dyna.unk_1A8 = 0.0f;
        player->stateFlags2 &= ~0x10;
    }
    Math_Vec3f_Copy(&player->actor.world.pos, &self->lockedPlayerPos);
}

}
