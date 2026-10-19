extern "C" {
#include "z_efc_erupc.hpp"
#include "model_manager.hpp"

#define FLAGS 0x00000030
#define R_UPDATE_RATE SREG(30)

void EfcErupc_Init(Actor* actor, GameState* state);
void EfcErupc_Destroy(Actor* actor, GameState* state);
void EfcErupc_Update(Actor* actor, GameState* state);
void EfcErupc_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Efc_Erupc.data.s")

GLOBAL_ASM("../src/game/actors/Efc_Erupc/data_syms.s")

ActorInit Efc_Erupc_InitVars = {
    ACTOR_EFC_ERUPC,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_EFC_ERUPC,
    sizeof(EfcErupc),
    (ActorFunc)EfcErupc_Init,
    (ActorFunc)EfcErupc_Destroy,
    (ActorFunc)EfcErupc_Update,
    (ActorFunc)EfcErupc_Draw,
};

#if 0 // EfcErupc_Init: 8/108 words off (near): first-loop pointer/counter regs (ROM r0/r2, ours r2/r3), `str r1,[r2,r0]` base/index order of the actionFunc store (raw-offset form needed), and ldr sb/str order in the model loop. GLOBAL_ASM below.
void* ZAR_GetCMBByIndex(void* zar, s32 index);
void EfcErupc_UpdateAction(EfcErupc* self, PlayState* play);

static inline u8* EfcErupc_GetObjBank(PlayState* play, u32 idx) {
    if (idx < 0x13 && *(s32*)((u8*)play + (idx << 7) + 0x3A64) != 0) {
        return (u8*)play + (idx << 7) + 0x3A5C;
    }
    return NULL;
}

typedef void (*EfcErupcActionFunc)(EfcErupc* self, PlayState* play);

static inline void EfcErupc_SetupAction(EfcErupc* self, EfcErupcActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

static inline void EfcErupc_InitEffects(EfcErupcEffect* effect) {
    s16 i;

    for (i = 0; i < 100; i++, effect++) {
        effect->isActive = 0;
    }
}

void EfcErupc_Init(Actor* thisx, GameState* state) {
    EfcErupc* self = (EfcErupc*)thisx;
    PlayState* play = (PlayState*)state;
    u8* zar;
    ModelAllocator* alloc;
    s32 i;

    {
        EfcErupcActionFunc fn = EfcErupc_UpdateAction;
        s32 off = 0x1F64;
        *(EfcErupcActionFunc*)((u8*)self + off) = fn;
    }
    Actor_SetScale(&self->actor, 1.0f);
    EfcErupc_InitEffects(self->effects);
    self->unk_1A4 = self->unk_1A6 = self->unk_1A8 = 0;
    self->unk_1AA = 0;
    self->unk_1AC = 5;
    self->unk_1AE = -100;
    self->actor.unk_19A = 1;

    zar = EfcErupc_GetObjBank(play, (u8)self->actor.objBankIndex) + 0x10;
    alloc = GetModelManager().allocator;
    alloc->owner = self->actor.unk_178;
    for (i = 0; i < 2; i++) {
        SkeletonAnimationModel* model = alloc->Create(ZAR_GetCMBByIndex(zar, i), 1);
        SkeletonAnimationModel_unk_0C* anim = model->unk_0C;

        self->unk_1F68[i] = (EfcErupcFx*)model;
        anim->FUN_00372d94(((ZARInfo*)zar)->GetCMABByIndex(i));
    }
    for (i = 0; i < 100; i++) {
        self->unk_1F70[i] = (EfcErupcFx*)alloc->Create(ZAR_GetCMBByIndex(zar, 2), 1);
    }
    alloc->owner = NULL;
}
#endif
GLOBAL_ASM("asm/EfcErupc_Init.s")

struct EfcErupcFx {
    virtual void unk_vf0();
    virtual void Destroy();
};

void EfcErupc_Destroy(Actor* thisx, GameState* state) {
    EfcErupc* self = (EfcErupc*)thisx;
    s32 i;

    for (i = 0; i < 2; i++) {
        if (self->unk_1F68[i] != NULL) {
            self->unk_1F68[i]->Destroy();
            self->unk_1F68[i] = NULL;
        }
    }
    for (i = 0; i < 100; i++) {
        if (self->unk_1F70[i] != NULL) {
            self->unk_1F70[i]->Destroy();
            self->unk_1F70[i] = NULL;
        }
    }
}

typedef struct __attribute__((aligned(4))) {
    u8 c[4][3];
} EfcErupcColors;

extern const EfcErupcColors EfcErupc_sEffectColors;

void EfcErupc_Update(Actor* thisx, GameState* state) {
    EfcErupc* self = (EfcErupc*)thisx;
    PlayState* play = (PlayState*)state;
    s16 i;
    s16 index;
    s32 frames;
    EfcErupcColors effectColors;
    EfcErupcEffect* cur;

    self->actionFunc(self, play);
    cur = self->effects;
    effectColors = EfcErupc_sEffectColors;
    for (i = 0; i < 100; i++, cur++) {
        if (cur->isActive) {
            cur->animTimer++;
            cur->pos.x += cur->vel.x * (R_UPDATE_RATE / 3.0f);
            cur->pos.y += cur->vel.y * (R_UPDATE_RATE / 3.0f);
            cur->pos.z += cur->vel.z * (R_UPDATE_RATE / 3.0f);
            cur->vel.x += cur->accel.x * (R_UPDATE_RATE / 3.0f);
            cur->vel.y += cur->accel.y * (R_UPDATE_RATE / 3.0f);
            cur->vel.z += cur->accel.z * (R_UPDATE_RATE / 3.0f);
            if (cur->animTimer > 0) {
                frames = (s32)((f32)cur->animTimer * R_UPDATE_RATE / 3.0f + 0.5f);
            } else {
                frames = (s32)((f32)cur->animTimer * R_UPDATE_RATE / 3.0f - 0.5f);
            }
            index = frames % 4;
            cur->colorR = effectColors.c[index][0];
            cur->colorG = effectColors.c[index][1];
            cur->colorB = effectColors.c[index][2];
            cur->alpha -= (s32)(R_UPDATE_RATE * 20.0f / 3.0f + 0.5f);
            if (cur->alpha <= 0) {
                cur->alpha = 0;
                cur->isActive = 0;
            }
        }
    }
}

#if 0 // EfcErupc_Draw: far (209/238 words off): SetUnkAC family (ROM keeps mov sb,#1 hoisted, self r6/play r7, different reg roles); drafts of the scale/translate blocks are in shape. GLOBAL_ASM below.
#include "mtx34_trans.hpp"

typedef struct {
    char unk_00[0x54];
    f32 scaleA;
    f32 scaleB;
} EfcErupcData;

extern EfcErupcData EfcErupc_sData;
void FUN_003586ec(SkeletonAnimationModel_unk_0C* anim);
void FUN_0036c174(MTX34* dst, MTX34* a, MTX34* b);
extern s32 gUnk_0054C8CC;

void EfcErupc_Draw(Actor* thisx, GameState* state) {
    EfcErupc* self = (EfcErupc*)thisx;
    PlayState* play = (PlayState*)state;
    MTX34 mtx;
    f32 sc;
    SkeletonAnimationModel* model;

    ((SkeletonAnimationModel*)self->unk_1F68[0])->unk_0C->animSpeed = (PauseContext_GetState() != 0) ? 0.0f : 1.0f;
    mtx = self->actor.modelMtx;
    sc = EfcErupc_sData.scaleA * 0.8f;
    mtx.data[0][0] *= sc;
    mtx.data[1][0] *= sc;
    mtx.data[2][0] *= sc;
    mtx.data[0][1] *= 0.8f;
    mtx.data[1][1] *= 0.8f;
    mtx.data[2][1] *= 0.8f;
    mtx.data[0][2] *= 0.8f;
    mtx.data[1][2] *= 0.8f;
    mtx.data[2][2] *= 0.8f;
    if (play->GetCutsceneState() != 0 && *(void**)((u8*)play + 0x22E0) != NULL && **(u16**)((u8*)play + 0x22E0) == 2) {
        model = (SkeletonAnimationModel*)self->unk_1F68[1];
        model->SetUnkAC(1);
        model->FUN_003721e0(&mtx);
        model->unk_0C->animSpeed = 0.0f;
        if (gUnk_0054C8CC == 0) {
            model->unk_0C->curFrame = (f32)(self->unk_1A8 - 1);
            FUN_003586ec(model->unk_0C);
        }
        model->FUN_00372170(0);
    }
    mtx = self->actor.modelMtx;
    mtx.data[0][0] *= 3.4f;
    mtx.data[1][0] *= 3.4f;
    mtx.data[2][0] *= 3.4f;
    mtx.data[0][1] *= 3.4f;
    mtx.data[1][1] *= 3.4f;
    mtx.data[2][1] *= 3.4f;
    mtx.data[0][2] *= 3.4f;
    mtx.data[1][2] *= 3.4f;
    mtx.data[2][2] *= 3.4f;
    if (play->GetCutsceneState() != 0 && *(void**)((u8*)play + 0x22E4) != NULL &&
        (**(u16**)((u8*)play + 0x22E4) == 2 || **(u16**)((u8*)play + 0x22E4) == 3)) {
        model = (SkeletonAnimationModel*)self->unk_1F68[0];
        model->SetUnkAC(1);
        model->FUN_003721e0(&mtx);
        model->FUN_00372170(0);
    }
    {
        EfcErupcEffect* effect = self->effects;
        s16 i;

        for (i = 0; i < 100; i++, effect++) {
            if (effect->isActive) {
                Vec3f pos = effect->pos;
                MTX34 emtx;
                f32 s;

                MTX34_MakeTrans(&emtx, &pos);
                FUN_0036c174(&emtx, &emtx, (MTX34*)((u8*)play + 0x2FC));
                s = effect->scale * EfcErupc_sData.scaleB;
                emtx.data[0][0] *= s;
                emtx.data[1][0] *= s;
                emtx.data[2][0] *= s;
                emtx.data[0][1] *= s;
                emtx.data[1][1] *= s;
                emtx.data[2][1] *= s;
                model = (SkeletonAnimationModel*)self->unk_1F70[i];
                FUN_003695cc(model, 0, 4, 2, effect->colorR * (1.0f / 255.0f), effect->colorG * (1.0f / 255.0f),
                             effect->colorB * (1.0f / 255.0f), effect->alpha * (1.0f / 255.0f));
                model->SetUnkAC(1);
                model->FUN_003721e0(&emtx);
                model->FUN_00372170(0);
            }
        }
    }
}
#endif
GLOBAL_ASM("asm/EfcErupc_Draw.s")

GLOBAL_ASM("asm/EfcErupc_UpdateAction.s")

typedef struct {
    Vec3f pos;
    Vec3f velocity;
    Vec3f accel;
    s16 unk_24;
    s16 unk_26;
    u8 unk_28;
    u8 unk_29;
} EfcErupcEffectParams;

void EffectSs_Spawn(PlayState* play, s32 type, s32 priority, void* initParams);

void FUN_00314a30(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 arg4, s16 arg5, u8 arg6) {
    EfcErupcEffectParams params;

    Math_Vec3f_Copy(&params.pos, pos);
    Math_Vec3f_Copy(&params.velocity, velocity);
    Math_Vec3f_Copy(&params.accel, accel);
    params.unk_24 = arg4;
    params.unk_26 = arg5;
    params.unk_28 = 2;
    params.unk_29 = arg6;
    EffectSs_Spawn(play, 3, 10, &params);
}

}
