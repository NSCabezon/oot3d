extern "C" {
#include "z_magic_dark.hpp"
#include "model_manager.hpp"
#include "mtx34_trans.hpp"

#define FLAGS 0x02000030

typedef union {
    u32 w;
    struct {
        u16 a;
        u16 b;
    } h;
} MDPair;

void MagicDark_Init(Actor* actor, GameState* state);
void MagicDark_Destroy(Actor* actor, GameState* state);
void MagicDark_Update(Actor* actor, GameState* state);
void MagicDark_Draw(Actor* actor, GameState* state);
void MagicDark_DiamondUpdate(Actor* actor, PlayState* play);
void MagicDark_DiamondDraw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Magic_Dark.data.s")

GLOBAL_ASM("../src/game/actors/Magic_Dark/data_syms.s")

ActorInit Magic_Dark_InitVars = {
    ACTOR_MAGIC_DARK,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(MagicDark),
    (ActorFunc)MagicDark_Init,
    (ActorFunc)MagicDark_Destroy,
    (ActorFunc)MagicDark_Update,
    (ActorFunc)MagicDark_Draw,
};


// Real virtual call on the model at +0x1BC (slot 1). VFE key placeholder: _ZTI<class> in shared_syms.s.
extern "C++" {
class MagicDarkModel {
public:
    virtual void vf00();
    virtual void Destroy();
};
class EnGanonMantFreer { // same object/class as En_Ganon_Mant (shared _ZTI16EnGanonMantFreer)
public:
    virtual void vf00();
    virtual void vf04();
    virtual void vf08();
    virtual void* Alloc(u32 size, const char* file, s32 line);
    virtual void Free(void* model);
};
}
extern EnGanonMantFreer* gUnk_0055A1F8; // src/game/shared_syms.s

void FUN_0034708c(PlayState* play);
void* FUN_003488e4(void* model);
void FUN_00348904(void* mgr, void* model);

void MagicDark_Destroy(Actor* actor, GameState* state) {
    MagicDark* self = (MagicDark*)actor;
    s32 i;

    if (gSaveContext.nayrusLoveTimer == 0) {
        FUN_0034708c((PlayState*)state);
    }

    if (self->model != NULL) {
        ((MagicDarkModel*)self->model)->Destroy();
        self->model = NULL;
    }

    if (self->freerModel != NULL) {
        void* model = FUN_003488e4(self->freerModel);
        gUnk_0055A1F8->Free(model);
        self->freerModel = NULL;
    }

    for (i = 0; i < 2; i++) {
        if (self->mgrModel[i] != NULL) {
            FUN_00348904(GetModelManager().unk_247C, self->mgrModel[i]);
        }
    }
}

extern u8 MagicDark_sData[]; // data_532384 (EQU in data_syms.s), start of this actor's data
void* ZAR_GetCMBByIndex(void* zar, s32 index);
void* ZAR_GetCTXBByIndex(void* zar, s32 index);
void* FUN_00348f34(void* mem, void* data);
void FUN_00348be4(void* obj);
void FUN_00348a64(void* obj, s32 index, void* ctxb, s32 a, s32 b, s32 c, s32 d);
void* FUN_0034897c(void* mgr, void* obj, void* p, s32 arg);
void FUN_003429c8(void* obj, s32 idx, f32* vec);
void FUN_0047d548(SkeletonAnimationModel* model, s32 arg);

#define MAGIC_DARK_FILE "d:\\home\\queen\\dailyBuild\\game_us\\sources\\z_magic_dark.cpp"

void MagicDark_Init(Actor* actor, GameState* state) {
    MagicDark* self = (MagicDark*)actor;
    PlayState* play = (PlayState*)state;
    Player* player = GET_PLAYER(play);
    ZARInfo* zar;
    void* ctxb;
    f32 vec[4];
    s32 i;

    if (gSaveContext.linkAge != 0) {
        self->scale = 0.4f;
    } else {
        self->scale = 0.6f;
    }

    self->actor.world.pos = player->actor.world.pos;
    Actor_SetScale(&self->actor, 0.0f);
    self->actor.room = -1;

    if (gSaveContext.nayrusLoveTimer != 0) {
        self->actor.update = (ActorFunc)MagicDark_DiamondUpdate;
        self->actor.draw = (ActorFunc)MagicDark_DiamondDraw;
        f32 sc = self->scale;
        self->actor.scale.x = self->actor.scale.z = sc * 1.6f;
        self->actor.scale.y = sc * 0.8f;
        self->timer = 0;
        self->primAlpha = 0;
    } else {
        self->timer = 0;
        gSaveContext.nayrusLoveTimer = 0;
    }

    zar = &play->GetObjectStatus(self->actor.objBankIndex)->zarInfo;

    GetModelManager().allocator->owner = actor->unk_178;
    self->cmb = ZAR_GetCMBByIndex(zar, 0x24);
    {
        ModelManager& mm = GetModelManager();

        self->model = mm.allocator->Create(self->cmb, 1);
    }
    self->model->unk_AD[0] = 0;
    {
        ModelManager& mm = GetModelManager();

        mm.allocator->owner = NULL;
    }
    self->model->unk_0C->FUN_00372d94(zar->GetCMABByIndex(0x11));
    self->model->unk_0C->SetAnimType(1);
    self->model->unk_0C->SetAnimSpeed(2.0f);
    FUN_0047d548(self->model, 2);
    ctxb = ZAR_GetCTXBByIndex(zar, 2);
    {
        const char* file = MAGIC_DARK_FILE;
        void* mem = gUnk_0055A1F8->Alloc(0x1B8, file, 393);

        self->freerModel = (mem != NULL) ? FUN_00348f34(mem, MagicDark_sData) : NULL;
    }
    FUN_00348be4(self->freerModel);
    FUN_00348a64(self->freerModel, 0, ctxb, 0x2601, 0x2601, 0x812F, 0x812F);
    vec[0] = 0.0f;
    vec[1] = 150.0f / 255.0f;
    vec[2] = 1.0f;
    vec[3] = 1.0f;
    for (i = 0; i < 2; i++) {
        self->mgrModel[i] = FUN_0034897c(GetModelManager().unk_247C, self->freerModel, self->actor.unk_178, 0);
        FUN_003429c8(self->mgrModel[i], 1, vec);
    }
    self->actor.unk_19A = 1;
}

void MagicDark_DimLighting(PlayState* play, f32 intensity);

#define MD_R_UPDATE_RATE SREG(30)
#define MD_NORM(t) ((t) > 0 ? (s32)(0.5f + ((f32)(t) * (f32)MD_R_UPDATE_RATE) * (1.0f / 3.0f)) : (s32)(((f32)(t) * (f32)MD_R_UPDATE_RATE) * (1.0f / 3.0f) - 0.5f))

void MagicDark_Update(Actor* actor, GameState* state) {
    MagicDark* self = (MagicDark*)actor;
    PlayState* play = (PlayState*)state;
    u8* player = *(u8**)((u8*)play + 0x20AC);

    FUN_00373264(&self->actor, 0x010000C4);
    if (self->timer < (s32)(105.0f / MD_R_UPDATE_RATE + 0.5f)) {
        MagicDark_DimLighting(play, MD_NORM(self->timer) * (1.0f / 45.0f));
        Math_SmoothStepToF(&self->actor.scale.x, self->scale * (1.0f / 12.000001f), 0.05f, 0.01f, 0.0001f);
        Actor_SetScale(&self->actor, self->actor.scale.x);
    } else if (self->timer < (s32)(165.0f / MD_R_UPDATE_RATE + 0.5f)) {
        Actor_SetScale(&self->actor, self->actor.scale.x * 0.9f);
        Math_SmoothStepToF(&self->orbOffset.y, *(f32*)(player + 0x2344), 0.5f, 3.0f, 1.0f);
        if (self->timer >= (s32)(147.0f / MD_R_UPDATE_RATE + 0.5f)) {
            MagicDark_DimLighting(play, (54 - MD_NORM(self->timer)) * 0.2f);
        }
    } else {
        self->actor.update = (ActorFunc)MagicDark_DiamondUpdate;
        self->actor.draw = (ActorFunc)MagicDark_DiamondDraw;
        f32 sc = self->scale;
        self->actor.scale.x = self->actor.scale.z = sc * 1.6f;
        actor->scale.y = sc * 0.8f;
        self->timer = 0;
        self->primAlpha = 0;
    }
    self->timer++;
}

GLOBAL_ASM("asm/MagicDark_Draw.s")

#if 0
f32 __hardfp_sinf(f32);
f32 __hardfp_cosf(f32);

static inline void MagicDark_Scale(MTX34* m, f32 x, f32 y, f32 z) {
    m->data[0][0] *= x;
    m->data[1][0] *= x;
    m->data[2][0] *= x;
    m->data[0][1] *= y;
    m->data[1][1] *= y;
    m->data[2][1] *= y;
    m->data[0][2] *= z;
    m->data[1][2] *= z;
    m->data[2][2] *= z;
}

// Inlined "apply" branch of Matrix_RotateY (mode 1).
static inline void MagicDark_RotateY(MTX34* m, f32 angle) {
    if (angle != 0.0f) {
        f32 sn = __hardfp_sinf(angle);
        f32 cs = __hardfp_cosf(angle);
        s32 r;

        for (r = 0; r < 3; r++) {
            f32 a = m->data[r][0];
            f32 c = m->data[r][2];

            m->data[r][0] = a * cs - c * sn;
            m->data[r][2] = a * sn + c * cs;
        }
    }
}

void FUN_00333abc(void* mat, s32 idx, f32* out);
void FUN_00333a38(void* mat, s32 idx, f32* in);
#define MD_COUNT(p) (*(s32*)((u8*)(*(void**)(*(void**)((u8*)(p) + 8))) + 8))

void MagicDark_DiamondDraw(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    MagicDark* self = (MagicDark*)actor;
    u8* player = (u8*)GET_PLAYER(play);
    MTX34 mtx;
    s32 i;

    if (*(u32*)(player + 0x1710) & 0x100000) {
        return;
    }

    {
        Vec3f t;
        f32 wx = *(f32*)(player + 0x2340);
        f32 wz;
        f32 wy;
        f32 heightDiff;
        f32 py;

        self->actor.world.pos.x = wx;
        wz = *(f32*)(player + 0x2348);
        self->actor.world.pos.z = wz;
        wy = *(f32*)(player + 0x2344);
        heightDiff = wy - self->actor.world.pos.y;
        if (heightDiff < -2.0f) {
            py = wy + 2.0f;
            self->actor.world.pos.y = py;
        } else if (heightDiff > 2.0f) {
            py = wy - 2.0f;
            self->actor.world.pos.y = py;
        } else {
            py = self->actor.world.pos.y;
        }
        t.x = wx;
        t.y = py;
        t.z = wz;
        MTX34_MakeTrans(&mtx, &t);
    }
    MagicDark_Scale(&mtx, self->actor.scale.x, self->actor.scale.y, self->actor.scale.z);
    MagicDark_RotateY(&mtx, self->actor.shape.rot.y * 0.000095873802f);
    self->model->unk_0C->animSpeed = (PauseContext_GetState() == 0) ? 2.0f : 0.0f;

    {
        f32 buf[4];

        for (i = 0; i < MD_COUNT(self->cmb); i++) {
            void* mat = self->model->unk_10;

            FUN_00333abc(mat, i, buf);
            buf[3] = (f32)self->primAlpha * (1.0f / 255.0f);
            FUN_00333a38(mat, i, buf);
            *((u8*)(*(void**)((u8*)mat + 4)) + i * 292) = 1;
        }
    }

    self->model->SetUnkAC(1);
    self->model->FUN_003721e0(&mtx);
    {
        SkeletonAnimationModel* m = self->model;

        m->unk_24 = mtx.data[0][3];
        m->unk_28 = mtx.data[1][3];
        m->unk_2C = mtx.data[2][3];
    }
    self->model->unk_AD[0] = 1;
    self->model->FUN_00372170(1);
}
#endif
GLOBAL_ASM("asm/MagicDark_DiamondDraw.s")

#if 0
s32 GetMessageMode(PlayState* play);
s16 Camera_GetCamDirYaw(Camera* camera);

void MagicDark_DiamondUpdate(Actor* actor, PlayState* play) {
    MagicDark* self = (MagicDark*)actor;
    Player* player = GET_PLAYER(play);
    s16 nayrusLoveTimer = gSaveContext.nayrusLoveTimer;
    s32 msgMode = GetMessageMode(play);
    u8 phi_a0;

    if ((msgMode == 0xD) || (msgMode == 0x11)) {
        Actor_Kill(&self->actor);
        return;
    }

    if (nayrusLoveTimer >= (s32)(3600.0f / MD_R_UPDATE_RATE + 0.5f)) {
        ((u8*)player)[0x2488] = 0;
        gSaveContext.nayrusLoveTimer = 0;
        Actor_Kill(&self->actor);
        return;
    }

    ((u8*)player)[0x2488] = (s8)MD_NORM(-100);
    f32 sc = self->scale;
    s32 timer = self->timer;
    self->actor.scale.x = self->actor.scale.z = sc;

    if (timer < (s32)(60.0f / MD_R_UPDATE_RATE + 0.5f)) {
        self->actor.scale.x = self->actor.scale.z = (1.6f - MD_NORM(timer) * 0.03f) * sc;
        self->actor.scale.y = (MD_NORM(timer) * 0.01f + 0.8f) * sc;
    } else {
        self->actor.scale.y = sc;
    }

    self->actor.scale.x = self->actor.scale.x * MD_R_UPDATE_RATE * (1.3f / 3.0f);
    self->actor.scale.z = self->actor.scale.z * MD_R_UPDATE_RATE * (1.3f / 3.0f);

    phi_a0 = (timer < (s32)(60.0f / MD_R_UPDATE_RATE + 0.5f)) ? (u8)(MD_NORM(timer) * 12) : 255;

    if (nayrusLoveTimer >= (s32)(3540.0f / MD_R_UPDATE_RATE + 0.5f)) {
        self->primAlpha = 15595 - MD_NORM(nayrusLoveTimer) * 13;
        if (nayrusLoveTimer & 1) {
            self->primAlpha = self->primAlpha >> 1;
        }
    } else if (nayrusLoveTimer >= (s32)(3300.0f / MD_R_UPDATE_RATE + 0.5f)) {
        self->primAlpha = (u8)(MD_NORM(nayrusLoveTimer) << 7) + 127;
    } else {
        self->primAlpha = 255;
    }

    if (self->primAlpha > phi_a0) {
        self->primAlpha = phi_a0;
    }

    self->actor.world.rot.y += (s32)(0.5f + ((f32)MD_R_UPDATE_RATE * 1000.0f) * (1.0f / 3.0f));
    self->actor.shape.rot.y = self->actor.world.rot.y + Camera_GetCamDirYaw(play->cameraPtrs[play->activeCamera]);
    self->timer++;
    gSaveContext.nayrusLoveTimer = nayrusLoveTimer + 1;

    if (nayrusLoveTimer < (s32)(3300.0f / MD_R_UPDATE_RATE + 0.5f)) {
        FUN_00373264(&self->actor, 0x010004CF);
    } else {
        FUN_00373264(&self->actor, 0x010004D0);
    }
}
#endif
GLOBAL_ASM("asm/MagicDark_DiamondUpdate.s")

typedef struct MagicDarkEnvBlock {
    /* 0x0000 */ char pad_0000[0x8];
    /* 0x0008 */ s16 adjFogColor[3];
    /* 0x000E */ s16 adjFogNear;
    /* 0x0010 */ char pad_0010[0x30];
    /* 0x0040 */ u16 fogNear;
} MagicDarkEnvBlock;

// PlayState raw offsets: roomCtx.curRoom.type = +0x4C33 (5 = boss), envCtx at +0x3200, fogColor at +0x3251.
void MagicDark_DimLighting(PlayState* play, f32 intensity) {
    s32 i;
    f32 colorScale;
    f32 fogScale;
    MagicDarkEnvBlock* env;

    if (*((u8*)play + 0x4C33) != 5) {
        intensity = CLAMP_MIN(intensity, 0.0f);
        intensity = CLAMP_MAX(intensity, 1.0f);
        fogScale = intensity - 0.2f;
        fogScale = CLAMP_MIN(fogScale, 0.0f);

        env = (MagicDarkEnvBlock*)((u8*)play + 0x3200);
        env->adjFogNear = (850.0f - env->fogNear) * fogScale;

        if (intensity == 0.0f) {
            for (i = 0; i < 3; i++) {
                env->adjFogColor[i] = 0;
            }
        } else {
            colorScale = intensity * 5.0f;

            if (colorScale > 1.0f) {
                colorScale = 1.0f;
            }

            for (i = 0; i < 3; i++) {
                env->adjFogColor[i] = -(s16)(*((u8*)play + 0x3251 + i) * colorScale);
            }
        }
    }
}

// Reads two u16 fields at +0x182/+0x184 of (p + 0x100); returns the first sign-extended.
s32 FUN_00320d54(void* p, u32 v) {
    u8* base = (u8*)p + 0x100;
    MDPair r;
    r.w = v;
    r.h.a = *(u16*)(base + 0x82);
    r.h.b = *(u16*)(base + 0x84);
    return (s16)r.w;
}

}
