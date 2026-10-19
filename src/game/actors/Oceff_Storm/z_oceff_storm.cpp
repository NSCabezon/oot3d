extern "C" {
#include "z_oceff_storm.hpp"
#include "model_manager.hpp"
#include "mtx34_trans.hpp"

#define FLAGS 0x02000030

void OceffStorm_Init(Actor* thisx, PlayState* play);
void OceffStorm_Destroy(Actor* actor, GameState* state);
void OceffStorm_Update(Actor* actor, GameState* state);
void OceffStorm_Draw(Actor* thisx, PlayState* play);
s16 Camera_GetCamDirYaw(Camera* camera);

GLOBAL_ASM("data/z_Oceff_Storm.data.s")

GLOBAL_ASM("../src/game/actors/Oceff_Storm/data_syms.s")

ActorInit Oceff_Storm_InitVars = {
    ACTOR_OCEFF_STORM,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(OceffStorm),
    (ActorFunc)OceffStorm_Init,
    (ActorFunc)OceffStorm_Destroy,
    (ActorFunc)OceffStorm_Update,
    (ActorFunc)OceffStorm_Draw,
};

#define R_UPDATE_RATE SREG(30)

void FUN_0034708c(PlayState* play); // Magic_Reset
void FUN_00367c48(void* obj);
void FUN_00348904(void* mgr, void* model);
void* FUN_003488e4(void* model);

// Object behind the global pointer at 0x0055A1F8 (slot 4 frees a model). Real virtual call.
extern "C++" {
class OceffStormFreer {
public:
    virtual void vf00();
    virtual void vf04();
    virtual void vf08();
    virtual void* Alloc(u32 size, const char* file, s32 line);
    virtual void Free(void* model);
};
}
extern OceffStormFreer* gUnk_0055A1F8; // src/game/shared_syms.s

void OceffStorm_Destroy(Actor* actor, GameState* state) {
    OceffStorm* self = (OceffStorm*)actor;
    Player* player = GET_PLAYER((PlayState*)state);

    FUN_0034708c((PlayState*)state);
    if (gSaveContext.nayrusLoveTimer != 0) {
        u8* playerBase = (u8*)player + 0x1000;

        playerBase[0x72A] |= 0x40;
    }
    if (self->model != NULL) {
        ((OceffStormModel*)self->model)->Destroy();
        self->model = NULL;
    }
    if (self->unk_1C4 != NULL) {
        FUN_00348904(GetModelManager().unk_247C, self->unk_1C4);
        self->unk_1C4 = NULL;
    }
    if (self->unk_1C0 != NULL) {
        void* model = FUN_003488e4(self->unk_1C0);
        gUnk_0055A1F8->Free(model);
        self->unk_1C0 = NULL;
    }
    FUN_00367c48(self->unk_1BC);
}

void OceffStorm_Update(Actor* actor, GameState* state) {
    OceffStorm* self = (OceffStorm*)actor;
    PlayState* play = (PlayState*)state;
    Player* player = GET_PLAYER(play);

    self->actor.world.pos = player->actor.world.pos;
    self->actor.shape.rot.y = Camera_GetCamDirYaw(play->cameraPtrs[play->activeCamera]);
    self->actionFunc(self, play);
}

void OceffStorm_UnkAction(OceffStorm* self, PlayState* play) {
    if (self->primColorAlpha < 100) {
        self->primColorAlpha = (u8)((f32)self->primColorAlpha + (0.5f + ((f32)R_UPDATE_RATE * (1.0f / 3.0f)) * 5.0f));
    }
    //! @bug Actor_Kill is never called so the actor will stay alive forever
}

// OceffStorm_DefaultAction (N64 logic, 3DS frame scaling: frames = counter * R_UPDATE_RATE / 3).
void FUN_003f450c(OceffStorm* self, PlayState* play) {
    f32 frames = (f32)self->counter * ((f32)R_UPDATE_RATE * (1.0f / 3.0f));

    if (frames < 20.0f) {
        self->primColorAlpha = (s32)(frames * 5.0f);
    } else if (frames > 80.0f) {
        self->primColorAlpha = (s32)((100.0f - frames) * 5.0f);
    } else {
        self->primColorAlpha = 100;
    }

    if (frames < 10.0f || frames >= 90.0f) {
        self->vtxAlpha = 0;
    } else {
        if (frames <= 65.0f) {
            if (self->vtxAlpha <= 200) {
                self->vtxAlpha = (u8)((f32)self->vtxAlpha + (0.5f + ((f32)R_UPDATE_RATE * (1.0f / 3.0f)) * 10.0f));
            }
            self->actor.scale.x = self->actor.scale.z = 0.4f;
            self->actor.scale.y = 0.3f;
        } else if (frames > 65.0f) {
            self->vtxAlpha = (u8)((90.0f - frames) * 10.0f);
        } else {
            self->actor.scale.x = self->actor.scale.z = 0.4f;
            self->vtxAlpha = 255;
        }
    }

    if (frames > 60.0f) {
        self->actor.world.pos.y += self->posYOff * (f32)R_UPDATE_RATE * (1.0f / 3.0f) * 0.01f;
        self->posYOff += self->posYOffAdd;
        self->posYOffAdd += (f32)R_UPDATE_RATE * (1.0f / 3.0f) * 10.0f;
    }

    if (3.0f / R_UPDATE_RATE * 100.0f > self->counter) {
        self->counter++;
    } else {
        Actor_Kill(&self->actor);
    }
}

void FUN_00333abc(void* mat, s32 idx, f32* out);
void FUN_00333a38(void* mat, s32 idx, f32* in);
void OceffStorm_Draw2(Actor* thisx, PlayState* play);

// Element count reached through ((void*)+8 -> [0] -> +8): same chain FUN_00333abc uses.
#define OCEFF_STORM_COUNT(p) (*(s32*)((u8*)(*(void**)(*(void**)((u8*)(p) + 8))) + 8))

// Inline model-position setter (keeps the model pointer cached across the three stores).
static inline void OceffStorm_SetPos(SkeletonAnimationModel* m, f32 x, f32 y, f32 z) {
    m->unk_24 = x;
    m->unk_28 = y;
    m->unk_2C = z;
}

void OceffStorm_Draw(Actor* thisx, PlayState* play) {
    OceffStorm* self = (OceffStorm*)thisx;
    s32 i;
    OceffStorm* s2 = self;

    self->model->SetUnkAC(1);
    self->model->FUN_003721e0(&self->actor.modelMtx);
    OceffStorm_SetPos(self->model, self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z);
    self->model->unk_AD[0] = 1;
    self->model->unk_0C->animSpeed = (PauseContext_GetState() == 0) ? 1.0f : 0.0f;

    {
        f32 buf[4];

        for (i = 0; i < OCEFF_STORM_COUNT(self->unk_1B8); i++) {
            void* mat = self->model->unk_10;

            FUN_00333abc(mat, i, buf);
            buf[3] = (f32)s2->vtxAlpha * (1.0f / 255.0f);
            FUN_00333a38(mat, i, buf);
            *((u8*)(*(void**)((u8*)mat + 4)) + i * 292) = 1;
        }
    }

    self->model->FUN_00372170(0);
    OceffStorm_Draw2(thisx, play);
}

void* FUN_003432d4(void* obj, void* data);
void FUN_00348be4(void* obj);
void* ZAR_GetCTXBByIndex(void* zar, s32 index);
void FUN_00348a64(void* obj, s32 index, void* ctxb, s32 a, s32 b, s32 c, s32 d);
void* FUN_0034897c(void* mgr, void* obj, void* p, s32 arg);
void FUN_0047d548(SkeletonAnimationModel* model, s32 arg);
void FUN_00367c54(void);
void FUN_00367c60(Camera* camera, f32 arg);
void* ZAR_GetCMBByIndex(void* zar, s32 index);
extern u8 OceffStorm_sUnk535E9C; // data_535E9C (EQU in data_syms.s)

#define OCEFF_STORM_FILE "d:\\home\\queen\\dailyBuild\\game_us\\sources\\z_oceff_storm.cpp"

void OceffStorm_Init(Actor* thisx, PlayState* play) {
    OceffStorm* self = (OceffStorm*)thisx;
    ZARInfo* zar;
    void* ctxb;
    OceffStorm* s2 = self;

    self->actionFunc = FUN_003f450c;
    self->actor.scale.y = 0.0f;
    self->actor.scale.x = self->actor.scale.z = 0.0f;
    self->counter = 0;
    self->primColorAlpha = 0;
    self->vtxAlpha = 0;
    self->posYOffAdd = 0.0f;
    self->posYOff = self->posYOffAdd;

    zar = &play->GetObjectStatus(self->actor.objBankIndex)->zarInfo;

    {
        // The ROM embeds the __FILE__ string inside the function's literal pool; a local keeps it ahead of the ints.
        const char* file = OCEFF_STORM_FILE;
        void* mem = gUnk_0055A1F8->Alloc(0x1B8, file, 434);

        self->unk_1C0 = (mem != NULL) ? FUN_003432d4(mem, &OceffStorm_sUnk535E9C) : NULL;
    }
    FUN_00348be4(self->unk_1C0);
    ctxb = ZAR_GetCTXBByIndex(zar, 0x36);
    FUN_00348a64(s2->unk_1C0, 0, ctxb, 0x2601, 0x2601, 0x2901, 0x2901);
    FUN_00348a64(self->unk_1C0, 1, ctxb, 0x2601, 0x2601, 0x2901, 0x2901);
    {
        ModelManager& mm = GetModelManager();

        self->unk_1C4 = FUN_0034897c(mm.unk_247C, self->unk_1C0, self->actor.unk_178, 0);
    }
    self->actor.unk_19A = 1;

    if (self->actor.params == 1) {
        self->actionFunc = OceffStorm_UnkAction;
        self->actor.draw = (ActorFunc)OceffStorm_Draw2;
        return;
    }

    Actor_Spawn(&play->actorCtx, play, ACTOR_EN_OKARINA_EFFECT, self->actor.world.pos.x,
                self->actor.world.pos.y - 30.0f, self->actor.world.pos.z, 0, 0, 0, 1, 1);
    {
        ModelManager& mm = GetModelManager();

        mm.allocator->owner = self->actor.unk_178;
    }
    self->unk_1B8 = ZAR_GetCMBByIndex(zar, 0x30);
    {
        ModelManager& mm = GetModelManager();

        self->model = mm.allocator->Create(self->unk_1B8, 1);
    }
    self->model->unk_AD[0] = 0;
    {
        ModelManager& mm = GetModelManager();

        mm.allocator->owner = NULL;
    }
    self->model->unk_0C->FUN_00372d94(zar->GetCMABByIndex(0x1B));
    self->model->unk_0C->SetAnimType(1);
    FUN_0047d548(self->model, 2);
    {
        Camera* camera = Gameplay_GetCamera(play, -1);

        FUN_00367c54();
        FUN_00367c60(camera, *(f32*)((u8*)play + 0x1B0));
        self->unk_1BC = camera;
    }
}

// OceffStorm_Draw2: NOT matched (~130 words off, 133 insns). 3DS version writes a 16-byte colour into the effect
// object (+0xF0) and two scroll MTX34 (+0x110/+0x140, MTX34_MakeTrans shape) then calls FUN_00328350(mm+0x25F0, 2, obj, 0).
// Draft below has the right data flow; armcc store-forwards the colour/MTX temps instead of keeping them on the stack
// (ROM: rgba at sp+0x14 copied with ldm/stm, one shared Vec3f at sp+0x84, consts s17/s16 hoisted: 0.0f, 0.5f, 75/255).
// Same VFP-numbering / hoisting blocker class as OceffWipe*_Draw. Tried: array vs struct colour locals, by-value
// MakeColor/MakeScroll helpers, shared vs per-matrix Vec3f.
#if 0
typedef struct {
    f32 r, g, b, a;
} OceffStormColor;

typedef struct {
    /* 0x000 */ char unk_000[0xF0];
    /* 0x0F0 */ OceffStormColor color;
    /* 0x100 */ char unk_100[0x10];
    /* 0x110 */ MTX34 scrollA;
    /* 0x140 */ MTX34 scrollB;
} OceffStormEffect;

void FUN_0035bae4(void* obj, s32 index, OceffStormColor* vec);
void FUN_00328350(void* mgr, s32 a, void* obj, s32 b);
extern f32 OceffStorm_sScroll[]; // data_535E88 (EQU in data_syms.s): {0.25f, -32.0f, -8.0f, -8.0f, -4.0f}

static inline void OceffStorm_MakeScroll(MTX34* m, Vec3f* pos, f32 x, f32 y) {
    pos->x = x;
    pos->y = y;
    MTX34_MakeTrans(m, pos);
}

void OceffStorm_Draw2(Actor* thisx, PlayState* play) {
    OceffStorm* self = (OceffStorm*)thisx;
    u32 scroll = play->state.frames & 0xFFF;
    f32 zero = 0.0f;
    OceffStormColor vec;
    OceffStormColor rgba;
    MTX34 mtxA;
    MTX34 mtxB;
    Vec3f pos;

    vec.b = zero;
    vec.a = zero;
    rgba.r = 50.0f / 255.0f;
    rgba.g = 50.0f / 255.0f;
    rgba.b = 75.0f / 255.0f;
    rgba.a = ((f32)self->primColorAlpha * 0.5f - vec.a) * (1.0f / 255.0f);
    vec.r = 50.0f / 255.0f;
    vec.g = 50.0f / 255.0f;
    ((OceffStormEffect*)self->unk_1C4)->color = rgba;
    FUN_0035bae4(self->unk_1C4, 1, &vec);

    pos.z = zero;
    OceffStorm_MakeScroll(&mtxA, &pos, (f32)(s32)scroll * OceffStorm_sScroll[1] * 0.0025f,
                          (f32)(s32)scroll * OceffStorm_sScroll[2] * (1.0f / 240.0f));
    OceffStorm_MakeScroll(&mtxB, &pos, (f32)(s32)scroll * OceffStorm_sScroll[3] * 0.0025f,
                          (f32)(s32)scroll * OceffStorm_sScroll[4] * (1.0f / 240.0f));
    ((OceffStormEffect*)self->unk_1C4)->scrollA = mtxA;
    ((OceffStormEffect*)self->unk_1C4)->scrollB = mtxB;
    {
        ModelManager& mm = GetModelManager();

        FUN_00328350((u8*)&mm + 0x25F0, 2, self->unk_1C4, 0);
    }
}
#endif

GLOBAL_ASM("asm/OceffStorm_Draw2.s")

}
