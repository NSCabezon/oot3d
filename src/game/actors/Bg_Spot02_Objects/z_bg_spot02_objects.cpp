extern "C" {
#include "z_bg_spot02_objects.hpp"

#define FLAGS 0x00000030

#define R_UPDATE_RATE SREG(30)

// EVENTCHKINF_1D (gSaveContext.eventChkInf[1], bit 0x2000)
#define EVT_1D_SET (gSaveContext.eventChkInf[1] & 0x2000)

extern InitChainEntry BgSpot02Objects_sInitChain[];

typedef struct BgSpot02Quad {
    Vec3f v[4];
} BgSpot02Quad;
extern const BgSpot02Quad BgSpot02Objects_sQuad;

typedef struct BgSpot02ObjSub {
    char unk_000[0x178];
    u32 unk_178;
} BgSpot02ObjSub;

typedef struct BgSpot02Obj {
    char unk_00[0x8];
    BgSpot02ObjSub* unk_08;
} BgSpot02Obj;

void BgSpot02Objects_Init(Actor* actor, GameState* state);
void BgSpot02Objects_Destroy(Actor* actor, GameState* state);
void BgSpot02Objects_Update(Actor* actor, GameState* state);
void BgSpot02Objects_Draw(Actor* actor, GameState* state);

// Action / draw functions still in functions_*.cpp (referenced by address).
void FUN_003e8b60(BgSpot02Objects* self, PlayState* play);
void FUN_0019bb28(BgSpot02Objects* self, PlayState* play);
void FUN_00175404(BgSpot02Objects* self, PlayState* play);
void FUN_001e8c0c(BgSpot02Objects* self, PlayState* play);
void FUN_0021f548(Actor* actor, GameState* state);
void FUN_00207130(BgSpot02Objects* self, PlayState* play);
void FUN_0023c210(Actor* actor, GameState* state);
void FUN_00138500(BgSpot02Objects* self, PlayState* play);
void FUN_00205edc(BgSpot02Objects* self, PlayState* play);

void Matrix_RotateX(MTX34* mtx, s32 mode, f32 angle);
void Matrix_RotateY(MTX34* mtx, s32 mode, f32 angle);
f32 FUN_00361490(u16 a, u16 b, u16 c);
void EffectSsBomb2_SpawnLayered(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep);
int __cxa_guard_acquire(unsigned int* guard_object);
void __cxa_guard_release(unsigned int* guard_object);

// Function-local `static Vec3f zeroVec` of N64 func_808AC908: guard word lives inside this actor's data dump
// (0x005136D0, right after sInitChain), the vector in .bss at 0x00569A88 (EQU in syms.s).
extern unsigned int BgSpot02Objects_sZeroVecGuard;
extern Vec3f BgSpot02Objects_sZeroVec;

void OnePointCutscene_Attention(PlayState* play, Actor* actor, s32 a2, s32 a3);
void Audio_PlaySoundGeneral(u32 sfxId, Vec3f* pos, u8 a2, f32* a3, f32* a4, s8* a5);

void* SystemArena_Malloc_0035010c(s32 size);
void* FUN_003500c4(void* p);
void FUN_0034ff2c(void* obj, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void FUN_0034fea8(void* obj, s32 bank, GameState* state, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7);
void FUN_0034ea6c(BgSpot02ObjSub* sub, Vec3f* quad);
void FUN_0034fc7c(void* obj);
void SystemArena_Free(void* p);

GLOBAL_ASM("data/z_Bg_Spot02_Objects.data.s")
GLOBAL_ASM("../src/game/actors/Bg_Spot02_Objects/syms.s")

ActorInit Bg_Spot02_Objects_InitVars = {
    ACTOR_BG_SPOT02_OBJECTS,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SPOT02_OBJECTS,
    sizeof(BgSpot02Objects),
    (ActorFunc)BgSpot02Objects_Init,
    (ActorFunc)BgSpot02Objects_Destroy,
    (ActorFunc)BgSpot02Objects_Update,
    (ActorFunc)BgSpot02Objects_Draw,
};

void BgSpot02Objects_Init(Actor* actor, GameState* state) {
    BgSpot02Objects* self = (BgSpot02Objects*)actor;
    PlayState* play = (PlayState*)state;
    ZARInfo* zar;
    CollisionHeader* colHeader = NULL;
    BgSpot02Quad quad;
    s32 n;
    void* obj;
    s32 idx;

    DynaPolyActor_Init(&self->dyna, 0);
    idx = 6;
    if (play->sceneNum == 0x5C) {
        idx = 7;
    }
    zar = FUN_00372f38(actor, play, &self->skelAnimModel[0], 0,
                       &self->skelAnimModel[1], 4, &self->skelAnimModel[2], 3,
                       &self->skelAnimModel[3], idx, 0);
    self->skelAnimModel[0]->unk_0C->FUN_00372d94(zar->GetCMABByIndex(1));
    self->skelAnimModel[0]->unk_0C->SetAnimType(0);
    self->skelAnimModel[0]->unk_0C->SetAnimSpeed(2.0f);

    obj = SystemArena_Malloc_0035010c(0x28);
    if (obj != NULL) {
        obj = FUN_003500c4(obj);
    }
    self->unk_1CC = obj;
    FUN_0034ff2c(self->unk_1CC, 8, 2, 1, 2, 0);
    FUN_0034fea8(self->unk_1CC, actor->objBankIndex, state, 1, 0x2601, 0x2601, 0x812F, 0x812F);
    quad = BgSpot02Objects_sQuad;
    FUN_0034ea6c(((BgSpot02Obj*)self->unk_1CC)->unk_08, quad.v);
    ((BgSpot02Obj*)self->unk_1CC)->unk_08->unk_178 |= 1;

    self->unk_1C3 = (u8)(actor->params >> 8);
    actor->params = (u16)(actor->params & 0xFF);

    switch (actor->params) {
        case 0:
        case 1:
        case 2:
            Actor_ProcessInitChain(actor, BgSpot02Objects_sInitChain);

            if (actor->params == 0) {
                if (Flags_GetSwitch(play, self->unk_1C3)) {
                    actor->world.pos.y += 255.0f;
                    self->actionFunc = FUN_003e8b60;
                } else {
                    self->actionFunc = FUN_00175404;
                }
                n = 1;
            } else if (actor->params == 1) {
                self->actionFunc = FUN_003e8b60;
                ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 0), &colHeader);
                actor->flags |= 0x400000;
                n = -1;
            } else {
                n = 2;
                if (play->sceneNum == 0x53) {
                    self->actionFunc = FUN_0019bb28;
                } else {
                    self->actionFunc = FUN_003e8b60;
                }
                if (play->sceneNum == 0x5C) {
                    n = 3;
                    actor->flags |= 0x400000;
                }
            }
            if (n >= 0) {
                ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, n), &colHeader);
            }
            self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);

            if ((EVT_1D_SET && (play->sceneNum == 0x53) && (actor->params == 2)) ||
                (LINK_IS_ADULT && (actor->params == 1))) {
                Actor_Kill(actor);
            }
            break;

        case 3:
            self->unk_1C2 = 0;
            Actor_ChangeType(play, &play->actorCtx, actor, 7);
            self->actionFunc = FUN_001e8c0c;
            actor->draw = FUN_0021f548;

            if (EVT_1D_SET) {
                Actor_Kill(actor);
            }
            break;

        case 4:
            self->timer = (s16)((-12 * 3.0f) / R_UPDATE_RATE + 0.5f);
            self->unk_1C8 = 0xFFFF;
            Actor_ChangeType(play, &play->actorCtx, actor, 7);
            self->actionFunc = FUN_00207130;
            actor->draw = FUN_0023c210;
            break;
    }
}


static inline void BgSpot02_Vec3fCtor(Vec3f* v, f32 x, f32 y, f32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

// N64 func_808AC908
void FUN_0019bb28(BgSpot02Objects* self, PlayState* play) {
    Vec3f pos;

    if (!(((unsigned int*)BgSpot02Objects_sInitChain)[1] & 1) && __cxa_guard_acquire(&BgSpot02Objects_sZeroVecGuard)) {
        BgSpot02_Vec3fCtor(&BgSpot02Objects_sZeroVec, 0.0f, 0.0f, 0.0f);
        __cxa_guard_release(&BgSpot02Objects_sZeroVecGuard);
    }

    if (play->GetCutsceneState() != 0 && play->csCtx.npcActions[3] != NULL && *(u16*)play->csCtx.npcActions[3] == 2) {
        Audio_PlayActorSound2(&self->dyna.actor, 0x01000219);
        Audio_PlayActorSound2(&self->dyna.actor, 0x01000318);
        gSaveContext.eventChkInf[1] |= 0x2000;
        self->timer = 38;
        pos.x = Math_SinS(self->dyna.actor.shape.rot.y) * 50.0f + self->dyna.actor.world.pos.x;
        pos.y = self->dyna.actor.world.pos.y + 30.0f;
        pos.z = Math_CosS(self->dyna.actor.shape.rot.y) * 50.0f + self->dyna.actor.world.pos.z;
        EffectSsBomb2_SpawnLayered(play, &pos, &BgSpot02Objects_sZeroVec, &BgSpot02Objects_sZeroVec, 70, 30);
        self->actionFunc = FUN_00205edc;
    }
}

// N64 func_808AC8FC (empty)
void FUN_003e8b60(BgSpot02Objects* self, PlayState* play) {
}

// N64 func_808ACAFC
void FUN_00175404(BgSpot02Objects* self, PlayState* play) {
    if (Flags_GetSwitch(play, self->unk_1C3)) {
        Actor_SetFocus(&self->dyna.actor, 60.0f);
        OnePointCutscene_Attention(play, &self->dyna.actor, 0, 0);
        self->actionFunc = FUN_00138500;
    }
}

// N64 func_808ACC34
void FUN_001e8c0c(BgSpot02Objects* self, PlayState* play) {
    if (play->GetCutsceneState() != 0 && play->csCtx.npcActions[0] != NULL && *(u16*)play->csCtx.npcActions[0] == 2) {
        self->unk_1C2++;

        if (self->unk_1C2 >= 16) {
            Actor_Kill(&self->dyna.actor);
        }
    }

    if (play->csCtx.frames == (s32)(245 * 3.0f / R_UPDATE_RATE + 0.5f) ||
        play->csCtx.frames == (s32)(351 * 3.0f / R_UPDATE_RATE + 0.5f)) {
        Audio_PlaySoundGeneral(0x01000185, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
    }
}

// N64 func_808AD3D4
void FUN_00207130(BgSpot02Objects* self, PlayState* play) {
    if (play->GetCutsceneState() != 0 && play->csCtx.npcActions[2] != NULL) {
        if (play->sceneNum == 0x53 && play->csCtx.frames == 0x226) {
            Audio_PlayActorSound2(&self->dyna.actor, 0x0100012D);
        }

        if (*(u16*)play->csCtx.npcActions[2] == 2) {
            if (self->timer < (s32)(32 * 3.0f / R_UPDATE_RATE + 0.5f)) {
                self->timer++;
            } else {
                Actor_Kill(&self->dyna.actor);
            }
        }
    }
}

typedef struct BgSpot02CueU {
    u16 action, startFrame, endFrame, rotX, rotY, rotZ;
    Vec3i startPos;
} BgSpot02CueU;

// FUN_0023c210 (N64 func_808AD450 draw): NOT matched, best 38/180 words differ (Sonnet stage 2). The u32-word
// matrix build (see Bg_Haka_Water notes) reproduces the identity/translation shape; remaining diff is VFP
// register/schedule choice (pos vcvt order s2/s0/s1 vs ours s0/s1/s2, 0.1f hoisting). `if (animSpeed == 0.0f) {}`
// is needed to keep the 0.0 constant in a VFP reg.
#if 0
static inline void BgSpot02_TransMtx(MTX34* m, Vector3f* v) {
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

static inline void BgSpot02_ScaleMtx(MTX34* mtx, f32 x, f32 y, f32 z) {
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

#define CUE2 ((BgSpot02CueU*)play->csCtx.npcActions[2])

// N64 func_808AD450 (draw)
void FUN_0023c210(Actor* actor, GameState* state) {
    BgSpot02Objects* self = (BgSpot02Objects*)actor;
    PlayState* play = (PlayState*)state;
    MTX34 modelMtx;
    f32 lerp;
    f32 animSpeed;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    if (play->GetCutsceneState() != 0 && CUE2 != NULL) {
        u16 temp = (u16)(CUE2->rotZ * (360.0f / 65536.0f));

        if (self->unk_1C8 != temp) {
            if (self->unk_1C8 == 0xFFFF) {
                self->unk_1C8 = temp;
                self->unk_1CA = self->unk_1C8;
            } else {
                self->unk_1CA = self->unk_1C8;
                self->unk_1C8 = temp;
            }
        }
        lerp = FUN_00361490(CUE2->endFrame, CUE2->startFrame, play->csCtx.frames);
        if (CUE2->action == 2) {
            f32 sx, sy, sz;
            animSpeed = 1.0f;
            if (PauseContext_GetState() != 0) {
                animSpeed = 0.0f;
            }
            if (animSpeed == 0.0f) {}
            {
                Vector3f pos((f32)CUE2->startPos.x, (f32)CUE2->startPos.y, (f32)CUE2->startPos.z);
                BgSpot02_TransMtx(&modelMtx, &pos);
            }
            Matrix_RotateX(&modelMtx, 1, CUE2->rotX * (2.0f * 3.14159265f / 65536.0f));
            Matrix_RotateY(&modelMtx, 1, CUE2->rotY * (2.0f * 3.14159265f / 65536.0f));
            if (play->sceneNum == 0x53 && actor->room == 1) {
                sx = 0.27f; sy = 0.27f;
                sz = (self->unk_1CA + (s32)(self->unk_1C8 - self->unk_1CA) * lerp) * 0.1f * 0.3f;
            } else {
                sx = 0.9f; sy = 0.9f;
                sz = (self->unk_1CA + (s32)(self->unk_1C8 - self->unk_1CA) * lerp) * 0.1f;
            }
            BgSpot02_ScaleMtx(&modelMtx, sx, sy, sz);
            self->skelAnimModel[0]->unk_0C->SetAnimSpeed(animSpeed * 2.0f);
            self->skelAnimModel[0]->SetUnkAC(1);
            self->skelAnimModel[0]->FUN_003721e0(&modelMtx);
            self->skelAnimModel[0]->FUN_00372170(0);
        }
    }
}

#endif

void BgSpot02Objects_Destroy(Actor* actor, GameState* state) {
    BgSpot02Objects* self = (BgSpot02Objects*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel[0], &self->skelAnimModel[1],
                 &self->skelAnimModel[2], &self->skelAnimModel[3], 0);
    if (self->unk_1CC != NULL) {
        FUN_0034fc7c(self->unk_1CC);
        SystemArena_Free(self->unk_1CC);
    }
    self->unk_1CC = NULL;
}

void BgSpot02Objects_Update(Actor* actor, GameState* state) {
    BgSpot02Objects* self = (BgSpot02Objects*)actor;

    self->actionFunc(self, (PlayState*)state);
}

void BgSpot02Objects_Draw(Actor* actor, GameState* state) {
    BgSpot02Objects* self = (BgSpot02Objects*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    self = (BgSpot02Objects*)((u8*)self + actor->params * 4);
    self->skelAnimModel[1]->SetUnkAC(1);
    self->skelAnimModel[1]->FUN_003721e0(&modelMtx);
    self->skelAnimModel[1]->FUN_00372170(0);
}
}
