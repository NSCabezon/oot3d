extern "C" {
#include "z_bg_haka_water.hpp"

#define FLAGS 0x80400030

void BgHakaWater_Init(Actor* actor, GameState* state);
void BgHakaWater_Destroy(Actor* actor, GameState* state);
void BgHakaWater_Update(Actor* actor, GameState* state);
void BgHakaWater_Draw(Actor* actor, GameState* state);

// 7-arg form of the model loader: also loads a secondary model into skelAnimModel2

// 3DS water box entries are 0x10 bytes (repo WaterBox in z3Dbgcheck.hpp is only 4 bytes)
typedef struct {
    /* 0x00 */ char unk_00[0x2];
    /* 0x02 */ s16 ySurface;
    /* 0x04 */ char unk_04[0xC];
} BgHakaWaterBox;

// ROM copies this 1-entry chain from rodata (0x4D70F4) to the stack before use.
extern InitChainEntry BgHakaWater_sInitChainRom;
GLOBAL_ASM("../src/game/actors/Bg_Haka_Water/data_syms.s")

ActorInit Bg_Haka_Water_InitVars = {
    ACTOR_BG_HAKA_WATER,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_HAKACH_OBJECTS,
    sizeof(BgHakaWater),
    (ActorFunc)BgHakaWater_Init,
    (ActorFunc)BgHakaWater_Destroy,
    (ActorFunc)BgHakaWater_Update,
    (ActorFunc)BgHakaWater_Draw,
};

// Mixed GetCMABByIndex shapes: `actor->params` (param reused after cast) gives the r2 shape on every call;
// the scoped cmab0/m0 locals in block 1 restore the normal shape for the first call.
void FUN_003e760c(BgHakaWater* self, PlayState* play); // N64: BgHakaWater_Wait

void BgHakaWater_Init(Actor* actor, GameState* state) {
    BgHakaWater* self = (BgHakaWater*)actor;
    PlayState* play = (PlayState*)state;
    ZARInfo* zar;
    s32 i;
    InitChainEntry sInitChain[1];

    sInitChain[0] = BgHakaWater_sInitChainRom;
    Actor_ProcessInitChain(&self->actor, sInitChain);
    zar = FUN_00372f38(&self->actor, play, &self->skelAnimModel, 6,
                                                    &self->skelAnimModel2, 7, 0);
    {
        void* cmab0 = zar->GetCMABByIndex(1);
        SkeletonAnimationModel* m0 = self->skelAnimModel;
        m0->unk_0C->FUN_00372d94(cmab0);
    }
    self->skelAnimModel->unk_0C->SetAnimType(1);
    self->skelAnimModel2->unk_0C->FUN_00372d94(zar->GetCMABByIndex(0));
    self->skelAnimModel2->unk_0C->SetAnimType(1);

    if (Flags_GetSwitch(play, actor->params)) {
        self->actor.home.pos.y -= 200.0f;
        self->actor.world.pos.y = self->actor.home.pos.y;
        self->isLowered = true;
    } else {
        self->isLowered = false;
    }
    for (i = 0; i < 9; i++) {
        ((BgHakaWaterBox*)((PlayState*)state)->colCtx.stat.colHeader->waterBoxes)[i].ySurface =
            (s16)self->actor.world.pos.y - 8;
    }
    self->actionFunc = FUN_003e760c;
}

void BgHakaWater_Destroy(Actor* actor, GameState* state) {
    BgHakaWater* self = (BgHakaWater*)actor;

    FUN_00350f34(actor, &self->skelAnimModel, &self->skelAnimModel2, 0);
}

void BgHakaWater_Update(Actor* actor, GameState* state) {
    BgHakaWater* self = (BgHakaWater*)actor;
    PlayState* play = (PlayState*)state;

    self->actionFunc(self, play);
}

// BgHakaWater_Draw: not matched (best 109/162 words differ, 640 vs 648 B; VFP callee-saved reg numbering
// s18/s19/s20 differs and the Vector3f/identity-matrix build is only structurally similar). Draft under #if 0.
// Matrix build trick that reproduces the ROM int-copy shape: write the identity/translation as u32 words
// (float stores fold away otherwise) and keep trans as a `Vector3f`.
#if 0
static inline void BgHakaWater_ScaleMtx(MTX34* mtx, Vector3f* sc) {
    f32 x = sc->x, y = sc->y, z = sc->z;
    mtx->data[0][0] *= x;
    mtx->data[1][0] *= x;
    mtx->data[2][0] *= x;
    mtx->data[0][1] *= y;
    mtx->data[1][1] *= y;
    mtx->data[2][1] *= y;
    mtx->data[0][2] *= z;
    mtx->data[1][2] *= z;
    mtx->data[2][2] *= z;
}

static inline void BgHakaWater_TransMtx(MTX34* m, Vector3f* v) {
    u32* w = (u32*)m;
    w[0] = 0x3f800000;
    w[1] = 0;
    w[2] = 0;
    w[3] = *(u32*)&v->x;
    w[4] = 0;
    w[5] = 0x3f800000;
    w[6] = 0;
    w[7] = *(u32*)&v->y;
    w[8] = 0;
    w[9] = 0;
    w[10] = 0x3f800000;
    w[11] = *(u32*)&v->z;
}

void BgHakaWater_Draw(Actor* actor, GameState* state) {
    BgHakaWater* self = (BgHakaWater*)actor;
    MTX34 mtx;
    f32 animSpeed;
    f32 temp;
    u8 alpha;

    MTX34CopyAsm(&mtx, &actor->modelMtx);
    animSpeed = 1.0f;
    if (PauseContext_GetState() != 0) {
        animSpeed = 0.0f;
    }
    if (self->isLowered) {
        temp = actor->world.pos.y - actor->home.pos.y;
    } else {
        temp = actor->world.pos.y - (actor->home.pos.y - 200.0f);
    }
    alpha = (u8)(0.765f * temp);
    FUN_003695cc(self->skelAnimModel, 0, 4, 0, 1.0f, 1.0f, 1.0f, (f32)alpha * (1.0f / 255.0f));
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->unk_0C->SetAnimSpeed(animSpeed);
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&mtx);
        self->skelAnimModel->FUN_00372170(0);
    }

    {
        Vector3f trans(0.0f, 92.0f, -1680.0f);
        BgHakaWater_TransMtx(&mtx, &trans);
    }
    {
        Vector3f sc(0.1f, 0.1f, 0.1f);
        BgHakaWater_ScaleMtx(&mtx, &sc);
    }
    temp -= 170.0f;
    if (temp < 0.0f) {
        temp = 0.0f;
    }
    alpha = (u8)(5.1f * temp);
    FUN_003695cc(self->skelAnimModel2, 0, 4, 0, 1.0f, 1.0f, 1.0f, (f32)alpha * (1.0f / 255.0f));
    FUN_003695cc(self->skelAnimModel2, 1, 4, 0, 1.0f, 1.0f, 1.0f, (f32)alpha * (1.0f / 255.0f));
    if (self->skelAnimModel2 != NULL) {
        self->skelAnimModel2->unk_0C->SetAnimSpeed(animSpeed);
        self->skelAnimModel2->SetUnkAC(1);
        self->skelAnimModel2->FUN_003721e0(&mtx);
        self->skelAnimModel2->FUN_00372170(0);
    }
}
}
#endif

GLOBAL_ASM("asm/BgHakaWater_Draw.s")
}
