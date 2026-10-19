extern "C" {
#include "z_oceff_wipe4.hpp"
#include "model_manager.hpp"
#include "mtx34_trans.hpp"

#define FLAGS 0x02000010

void OceffWipe4_Init(Actor* actor, GameState* state);
void OceffWipe4_Destroy(Actor* actor, GameState* state);
void OceffWipe4_Update(Actor* actor, GameState* state);
void OceffWipe4_Draw(Actor* actor, GameState* state);

ActorInit Oceff_Wipe4_InitVars = {
    ACTOR_OCEFF_WIPE4,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(OceffWipe4),
    (ActorFunc)OceffWipe4_Init,
    (ActorFunc)OceffWipe4_Destroy,
    (ActorFunc)OceffWipe4_Update,
    (ActorFunc)OceffWipe4_Draw,
};

void* ZAR_GetCMBByIndex(void* zar, s32 index);
void FUN_0047d548(SkeletonAnimationModel* model, s32 arg);
void FUN_00367c54(void* obj);
void FUN_00367c60(void* obj, f32 arg);

void OceffWipe4_Init(Actor* actor, GameState* state) {
    OceffWipe4* self = (OceffWipe4*)actor;
    PlayState* play = (PlayState*)state;
    ZARInfo* zar;
    Camera* cam;
    s32 cmbIdx;
    s32 cmabIdx;
    OceffWipe4* s2 = self;

    Actor_SetScale(&self->actor, 0.1f);
    self->timer = 0;
    self->actor.world.pos = play->cameraPtrs[play->activeCamera]->eye;
    cmabIdx = 0x1E;
    cmbIdx = 0x33;
    if (self->actor.params == 1) {
        cmbIdx = 0x34;
        cmabIdx = 0x1F;
    }
    zar = &play->GetObjectStatus(actor->objBankIndex)->zarInfo;
    {
        ModelManager& mm = GetModelManager();
        mm.allocator->owner = self->actor.unk_178;
    }
    self->cmb = ZAR_GetCMBByIndex(zar, cmbIdx);
    {
        ModelManager& mm = GetModelManager();
        self->model = mm.allocator->Create(self->cmb, 1);
    }
    self->model->unk_AD[0] = 0;
    {
        ModelManager& mm = GetModelManager();
        mm.allocator->owner = NULL;
    }
    self->model->unk_0C->FUN_00372d94(zar->GetCMABByIndex(cmabIdx));
    self->model->unk_0C->SetAnimType(0);
    FUN_0047d548(self->model, 2);
    self->actor.unk_19A = 1;
    cam = Gameplay_GetCamera(play, -1);
    FUN_00367c54(cam);
    FUN_00367c60(cam, *(f32*)((u8*)play + 0x1B0));
    s2->obj = cam;
}

void FUN_0034708c(PlayState* play); // Magic_Reset
void FUN_00367c48(void* obj);

void OceffWipe4_Destroy(Actor* thisx, GameState* state) {
    FUN_0034708c((PlayState*)state);
    {
        void* obj = *(void**)((u8*)thisx + 0x1A8);
        if (obj != NULL) {
            void** vtbl = *(void***)obj;

            ((void (*)(void*))vtbl[1])(obj);
            *(void**)((u8*)thisx + 0x1A8) = NULL;
        }
    }
    FUN_00367c48(*(void**)((u8*)thisx + 0x1B0));
}

#define R_UPDATE_RATE SREG(30)

void OceffWipe4_Update(Actor* thisx, GameState* state) {
    OceffWipe4* self = (OceffWipe4*)thisx;
    PlayState* play = (PlayState*)state;

    self->actor.world.pos = play->cameraPtrs[play->activeCamera]->eye;
    if (3.0f / R_UPDATE_RATE * 50.0f > self->timer) {
        self->timer++;
    } else {
        Actor_Kill(&self->actor);
    }
}

// OceffWipe4_Draw: NOT matched, ~17 insns off (difflib) after recipe R8 (MTX34_MakeTrans). Matrix block + 0.1 scale match; zero/one
// locals reproduce ROM's early 0.0f/1.0f hoist. Left: ROM keeps z (s17) and the post-sine z (s21, vmovge) in separate regs and
// numbers one=s17/zero=s16, hoists mov r8,#1 before the matrix stores (3 insns short).
#if 0
Vec3f FUN_0032e648(Camera* cam); // quake offset (returned in s0-s2)
void FUN_00371fac(MTX34* mtx, void* billboard);
void Matrix_UpdatePosition(MTX34* dst, MTX34* src, Vec3f* translate);
void FUN_00333abc(void* mat, s32 idx, f32* color);
void FUN_00333a38(void* mat, s32 idx, f32* color);

static inline void OW_TranslateNew(MTX34* mtx, f32 x, f32 y, f32 z) {
    Vec3f pos;

    pos.x = x;
    pos.y = y;
    pos.z = z;
    MTX34_MakeTrans(mtx, &pos);
}

static inline void OW_Scale(MTX34* m, f32 x, f32 y, f32 z) {
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

static inline void OW_Translate(MTX34* mtx, f32 y, f32 x, f32 z) {
    Vec3f translate;

    translate.x = x;
    translate.y = y;
    translate.z = z;
    Matrix_UpdatePosition(mtx, mtx, &translate);
}

static inline s32 OW_MaterialCount(void* cmb) {
    void** p = *(void***)((u8*)cmb + 8);

    return *(s32*)((u8*)*p + 8);
}

f32 FUN_0032c7a8(PlayState* play, f32 a, f32 b);

void OceffWipe4_Draw(Actor* actor, GameState* state) {
    OceffWipe4* self = (OceffWipe4*)actor;
    PlayState* play = (PlayState*)state;
    Vec3f eye = play->cameraPtrs[play->activeCamera]->eye;
    MTX34 mtx;
    Vec3f quake = FUN_0032e648(play->cameraPtrs[play->activeCamera]);
    f32 z;
    f32 frames;
    u8 alpha;
    s32 i;

    if (actor->params == 1) {
        z = FUN_0032c7a8(play, 1250.0f, 1180.0f);
    } else {
        z = FUN_0032c7a8(play, 1330.0f, 1240.0f);
    }
    frames = (f32)self->timer * ((f32)SREG(30) * (1.0f / 3.0f));
    z = (frames < 16.0f) ? Math_SinS((s16)(frames * 1024.0f)) * z : z;
    if (frames < 30.0f) {
        alpha = 255;
    } else {
        alpha = (u8)((50.0f - frames) * 12.0f);
    }
    {
        f32 one = 1.0f;
        f32 zero = 0.0f;
    OW_TranslateNew(&mtx, eye.x + quake.x, eye.y + quake.y, eye.z + quake.z);
        OW_Scale(&mtx, 0.1f, 0.1f, 0.1f);
        FUN_00371fac(&mtx, (u8*)play + 0x2FC);
        OW_Translate(&mtx, zero, zero, -z);
        self->model->SetUnkAC(1);
        self->model->FUN_003721e0(&mtx);
        {
            f32 px = eye.x + quake.x;
            f32 py = eye.y + quake.y;
            f32 pz = eye.z + quake.z;
            SkeletonAnimationModel* model = self->model;

            model->unk_24 = px;
            model->unk_28 = py;
            model->unk_2C = pz;
        }
        self->model->unk_AD[0] = 1;
        if (PauseContext_GetState() != 0) {
            self->model->unk_0C->animSpeed = zero;
        } else {
            self->model->unk_0C->animSpeed = one;
        }
    }
    for (i = 0; i < OW_MaterialCount(self->cmb); i++) {
        void* mat = self->model->unk_10;
        f32 color[4];

        FUN_00333abc(mat, i, color);
        color[3] = alpha * (1.0f / 255.0f);
        FUN_00333a38(mat, i, color);
        ((u8*)*(void**)((u8*)mat + 4))[i * 292] = 1;
    }
    self->model->FUN_00372170(0);
}
#endif

GLOBAL_ASM("asm/OceffWipe4_Draw.s")
}
