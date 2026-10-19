extern "C" {
#include "mtx34_trans.hpp"
#include "z_bg_toki_hikari.hpp"

#define FLAGS 0x80400020

void BgTokiHikari_Init(Actor* actor, GameState* state);
void BgTokiHikari_Destroy(Actor* actor, GameState* state);
void BgTokiHikari_Update(Actor* actor, GameState* state);
void BgTokiHikari_Draw(Actor* actor, GameState* state);

void FUN_003e82e8(BgTokiHikari* self, PlayState* play); // N64: BgTokiHikari_DoNothing
void FUN_0019a4cc(BgTokiHikari* self, PlayState* play); // N64: func_808BA204
void FUN_0019a490(BgTokiHikari* self, PlayState* play); // N64: func_808BA22C
void FUN_001c84b8(BgTokiHikari* self, PlayState* play); // N64: func_808BA274

void FUN_003586ec(SkeletonAnimationModel_unk_0C* anim);
extern s32 gUnk_0054C8CC;    // src/game/shared_syms.s
extern s32 gSaveContext_004; // src/game/shared_syms.s

#define TOKI_FRAME(play) (*(f32*)((u8*)(play) + 0x7C00 + 0x37C))

ActorInit Bg_Toki_Hikari_InitVars = {
    ACTOR_BG_TOKI_HIKARI,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_TOKI_OBJECTS,
    sizeof(BgTokiHikari),
    (ActorFunc)BgTokiHikari_Init,
    (ActorFunc)BgTokiHikari_Destroy,
    (ActorFunc)BgTokiHikari_Update,
    (ActorFunc)BgTokiHikari_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_STOP),
};

void BgTokiHikari_Init(Actor* actor, GameState* state) {
    BgTokiHikari* self = (BgTokiHikari*)actor;
    PlayState* play = (PlayState*)state;
    ZARInfo* zarInfo;

    zarInfo = FUN_00372f38(actor, play, &self->skelAnimModel[0], 7, &self->skelAnimModel[1], 9,
                           &self->skelAnimModel[2], 10, &self->skelAnimModel[3], 2,
                           &self->skelAnimModel[4], 0, &self->skelAnimModel[5], 1, 0);
    self->skelAnimModel[2]->unk_0C->FUN_00372d94(zarInfo->GetCMABByIndex(0));
    self->skelAnimModel[2]->unk_0C->SetAnimType(1);
    {
        f32 frame = TOKI_FRAME(play);
        SkeletonAnimationModel_unk_0C* anim = self->skelAnimModel[2]->unk_0C;
        if (gUnk_0054C8CC == 0) {
            *(f32*)&anim->curFrame = frame;
            FUN_003586ec(anim);
        }
    }

    switch (actor->params) {
        case 0:
            Actor_ProcessInitChain(actor, sInitChain);
            self->actionFunc = FUN_003e82e8;
            break;
        case 1:
            if (!(gSaveContext.eventChkInf[4] & 0x800)) {
                self->unk_14C = 0.0f;
                self->actionFunc = FUN_0019a4cc;
            } else {
                Actor_Kill(actor);
            }
            break;
    }
}

void BgTokiHikari_Destroy(Actor* actor, GameState* state) {
    BgTokiHikari* self = (BgTokiHikari*)actor;

    FUN_00350f34(actor, &self->skelAnimModel[0], &self->skelAnimModel[1], &self->skelAnimModel[2],
                 &self->skelAnimModel[3], &self->skelAnimModel[4], &self->skelAnimModel[5], 0);
}

void FUN_003e82e8(BgTokiHikari* self, PlayState* play) {
}

void BgTokiHikari_Update(Actor* actor, GameState* state) {
    BgTokiHikari* self = (BgTokiHikari*)actor;

    self->actionFunc(self, (PlayState*)state);
}

void FUN_0019a4cc(BgTokiHikari* self, PlayState* play) {
    if (*(u16*)((u8*)play + 0x53F2) != 0) { // roomCtx.drawParams[1]
        self->actionFunc = FUN_0019a490;
    }
}

void FUN_0019a490(BgTokiHikari* self, PlayState* play) {
    if (self->unk_14C < 1.0f) {
        self->unk_14C += 0.05f;
    } else {
        self->unk_14C = 1.0f;
        self->actionFunc = FUN_001c84b8;
    }
}

void FUN_001c84b8(BgTokiHikari* self, PlayState* play) {
    if (self->unk_14C > 0.2f) {
        self->unk_14C -= 0.025f;
    } else {
        self->unk_14C = 0.0f;
        Actor_Kill(&self->actor);
    }
}

// BgTokiHikari_Draw draft (recipe R8: identity+translate now via MTX34_MakeTrans, int-store block matches in shape).
// Still far (~170 insns): ROM's 2nd modelMtx copy in case 1 is an inline ldm/stm struct copy (not MTX34CopyAsm), scale/translate
// constants held in s17-s20 differently, frame 0x40 with push {r4-r6}. Needs a full rewrite pass.
#if 0
void Matrix_RotateZ(MTX34* mtx, s32 mode, f32 rad);
struct TokiVec {
    f32& operator[](s32 i) {
        return (&x)[i];
    }
    f32 x, y, z;
};
static inline void TokiTranslateNew(MTX34* mtx, f32 x, f32 y, f32 z) {
    TokiVec pos;
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    MTX34_MakeTrans(mtx, (Vec3f*)&pos);
}
static inline void TokiScale(MTX34* mtx, f32 sx, f32 sy, f32 sz) {
    mtx->data[0][0] *= sx;
    mtx->data[1][0] *= sx;
    mtx->data[2][0] *= sx;
    mtx->data[0][1] *= sy;
    mtx->data[1][1] *= sy;
    mtx->data[2][1] *= sy;
    mtx->data[0][2] *= sz;
    mtx->data[1][2] *= sz;
    mtx->data[2][2] *= sz;
}

void BgTokiHikari_Draw(Actor* actor, GameState* state) {
    BgTokiHikari* self = (BgTokiHikari*)actor;
    PlayState* play = (PlayState*)state;
    MTX34 mtx;
    f32 speed = 1.0f;

    switch (actor->params) {
        case 0:
            MTX34CopyAsm(&mtx, &actor->modelMtx);
            if (PauseContext_GetState() != 0) {
                speed = 0.0f;
            }
            TOKI_FRAME(play) = *(f32*)&self->skelAnimModel[2]->unk_0C->curFrame;
            if (gSaveContext_004 == 0) {
                self->skelAnimModel[0]->SetUnkAC(1);
                self->skelAnimModel[0]->FUN_003721e0(&mtx);
                self->skelAnimModel[0]->FUN_00372170(0);
            } else {
                self->skelAnimModel[1]->SetUnkAC(1);
                self->skelAnimModel[1]->FUN_003721e0(&mtx);
                self->skelAnimModel[1]->FUN_00372170(0);
                self->skelAnimModel[2]->unk_0C->SetAnimSpeed(speed);
                self->skelAnimModel[2]->SetUnkAC(1);
                self->skelAnimModel[2]->FUN_003721e0(&mtx);
                self->skelAnimModel[2]->FUN_00372170(0);
            }
            break;
        case 1:
            MTX34CopyAsm(&mtx, &actor->modelMtx);
            TokiTranslateNew(&mtx, 0.0f, 276.0f, 1122.0f);
            TokiScale(&mtx, 0.32f, 0.32f, self->unk_14C * 7.0f);
            Matrix_RotateZ(&mtx, 1, 3.14159274f);
            self->skelAnimModel[3]->SetUnkAC(1);
            self->skelAnimModel[3]->FUN_003721e0(&mtx);
            self->skelAnimModel[3]->FUN_00372170(0);

            MTX34CopyAsm(&mtx, &actor->modelMtx);
            TokiTranslateNew(&mtx, 0.0f, 276.0f, 1122.0f);
            TokiScale(&mtx, 0.32f, 0.32f, self->unk_14C * 7.0f);
            Matrix_RotateZ(&mtx, 1, 3.14159274f);
            {
                f32 k = self->unk_14C;
                f32 a = (k * 255.0f) * 0.003921569f;
                f32 b = (k * 155.0f) * 0.003921569f;
                FUN_003695cc(self->skelAnimModel[4], 0, 4, 0, a, a, a, b);
            }
            self->skelAnimModel[4]->SetUnkAC(1);
            self->skelAnimModel[4]->FUN_003721e0(&mtx);
            self->skelAnimModel[4]->FUN_00372170(0);
            break;
    }
}
#endif

GLOBAL_ASM("asm/BgTokiHikari_Draw.s")
}
