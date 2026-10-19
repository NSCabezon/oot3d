extern "C" {
extern __pure float _fabsf(float);
static __inline __pure float zur_fabsf(float f) {
    return _fabsf(f);
}
#include "z_bg_jya_zurerukabe.hpp"

#define FLAGS 0x80400010

void BgJyaZurerukabe_Init(Actor* actor, GameState* state);
void BgJyaZurerukabe_Destroy(Actor* actor, GameState* state);
void BgJyaZurerukabe_Update(Actor* actor, GameState* state);
void BgJyaZurerukabe_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Bg_Jya_Zurerukabe.data.s")
GLOBAL_ASM("../src/game/actors/Bg_Jya_Zurerukabe/z_bg_jya_zurerukabe_syms.s")

// Absolute data symbols (see z_bg_jya_zurerukabe_syms.s): N64 D_8089B9F0 etc.
extern s16 D_8089B9F0[4];
extern s16 D_8089B9F8[4];
extern f32 D_8089B9C0[4];
extern s16 D_8089BA18[6][2];
extern s16 D_8089BA30[6];
extern InitChainEntry sBgJyaZurerukabeInitChain[];

ActorInit Bg_Jya_Zurerukabe_InitVars = {
    ACTOR_BG_JYA_ZURERUKABE,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_JYA_OBJ,
    sizeof(BgJyaZurerukabe),
    (ActorFunc)BgJyaZurerukabe_Init,
    (ActorFunc)BgJyaZurerukabe_Destroy,
    (ActorFunc)BgJyaZurerukabe_Update,
    (ActorFunc)BgJyaZurerukabe_Draw,
};

// N64 func_8089B7C4 (wait) lives as raw bytes inside FUN_003bf948.s at 0x003bf9b0
#define ZUR_ACTION_WAIT ((BgJyaZurerukabeActionFunc)0x003bf9b0)

static inline void BgJyaZurerukabe_SetWait(BgJyaZurerukabe* self) {
    self->actionFunc = ZUR_ACTION_WAIT;
}

void BgJyaZurerukabe_Init(Actor* actor, GameState* state) {
    BgJyaZurerukabe* self = (BgJyaZurerukabe*)actor;
    PlayState* play = (PlayState*)state;
    void* colInfo;
    s32 i;

    FUN_00372f38(actor, play, &self->skelAnimModel, 0x25, 0);
    colInfo = DynaPolyInfo_Alloc(&self->dyna, state, 7);
    {
        CollisionHeader* colHeader = NULL;
        DynaPolyActor_Init(&self->dyna, 0);
        ReadCollisionHeaderFromZsi(colInfo, &colHeader);
        self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    }
    Actor_ProcessInitChain(actor, sBgJyaZurerukabeInitChain);

    for (i = 0; i < 4; i++) {
        if (zur_fabsf(D_8089B9F0[i] - self->dyna.actor.home.pos.y) < 1.0f) {
            self->unk_1C0 = i;
            break;
        }
    }

    self->unk_1C6 = D_8089B9F8[self->unk_1C0];
    BgJyaZurerukabe_SetWait(self);
}

void BgJyaZurerukabe_Destroy(Actor* actor, GameState* state) {
    BgJyaZurerukabe* self = (BgJyaZurerukabe*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    D_8089B9C0[self->unk_1C0] = 0.0f;
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgJyaZurerukabe_Draw(Actor* actor, GameState* state) {
    BgJyaZurerukabe* self = (BgJyaZurerukabe*)actor;
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

// Update: draft (#if 0) -- the N64 func_8089B4C8 (player knockback check) is inlined in the ROM
// after the switch jump table; the knockback callee's address is still unidentified.
GLOBAL_ASM("asm/BgJyaZurerukabe_Update.s")

#if 0
void BgJyaZurerukabe_Update(Actor* actor, GameState* state) {
    BgJyaZurerukabe* self = (BgJyaZurerukabe*)actor;
    PlayState* play = (PlayState*)state;

    if (self->unk_1C2 > 0) {
        self->unk_1C2--;
    }
    self->actionFunc(self, play);
    if (self->unk_1C0 == 0) {
        Player* player = GET_PLAYER(play);
        if ((*(u32*)((u8*)player + 0x1710) == 0x200000) && (player->actor.wallPoly != NULL)) {
            s32 i;
            for (i = 0; i < 6; i++) {
                f32 posY = player->actor.world.pos.y;
                if ((posY >= D_8089BA18[i][0]) && (posY <= D_8089BA18[i][1])) {
                    break;
                }
            }
            switch (i) {
                case 0: case 2: case 3: case 5:
                    if (zur_fabsf(D_8089B9C0[D_8089BA30[i]]) > 1.0f) {
                        /* UNKNOWN_CALLEE(play, &self->dyna.actor, 1.5f, self->dyna.actor.shape.rot.y, 0.0f, 0); */
                    }
                    break;
                case 1: case 4:
                    if (zur_fabsf(D_8089B9C0[D_8089BA30[i]] - D_8089B9C0[D_8089BA30[i + 1]]) > 1.0f) {
                        /* UNKNOWN_CALLEE(...) */
                    }
                    break;
            }
        }
    }
}
#endif
}
