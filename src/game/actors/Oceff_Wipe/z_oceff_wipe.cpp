extern "C" {
#include "z_oceff_wipe.hpp"
#include "model_manager.hpp"
#include "mtx34_trans.hpp"

#define FLAGS 0x02000010

void OceffWipe_Init(Actor* actor, GameState* state);
void OceffWipe_Destroy(Actor* actor, GameState* state);
void OceffWipe_Update(Actor* actor, GameState* state);
void OceffWipe_Draw(Actor* actor, GameState* state);

ActorInit Oceff_Wipe_InitVars = {
    ACTOR_OCEFF_WIPE,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(OceffWipe),
    (ActorFunc)OceffWipe_Init,
    (ActorFunc)OceffWipe_Destroy,
    (ActorFunc)OceffWipe_Update,
    (ActorFunc)OceffWipe_Draw,
};

void* ZAR_GetCMBByIndex(void* zar, s32 index);
void FUN_0047d548(SkeletonAnimationModel* model, s32 arg);
void FUN_00367c54(void* obj);
void FUN_00367c60(void* obj, f32 arg);

void OceffWipe_Init(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    OceffWipe* self = (OceffWipe*)actor;
    s32 cmbIdx;
    s32 cmabIdx;
    Camera* cam;
    ZARInfo* zar;
    OceffWipe* s2 = self;

    Actor_SetScale(&self->actor, 0.1f);
    self->timer = 0;
    self->actor.world.pos = play->cameraPtrs[play->activeCamera]->eye;
    if (self->actor.params != 0) {
        cmbIdx = 0x31;
        cmabIdx = 0x1C;
    } else {
        cmbIdx = 0x32;
        cmabIdx = 0x1D;
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
    self->model->unk_0C->SetAnimType(1);
    FUN_0047d548(self->model, 2);
    self->actor.unk_19A = 1;
    cam = Gameplay_GetCamera(play, -1);
    FUN_00367c54(cam);
    FUN_00367c60(cam, *(f32*)((u8*)play + 0x1B0));
    s2->obj = cam;
}

void FUN_0034708c(PlayState* play); // Magic_Reset
void FUN_00367c48(void* obj);

void OceffWipe_Destroy(Actor* thisx, GameState* state) {
    Player* player = GET_PLAYER((PlayState*)state);

    FUN_0034708c((PlayState*)state);
    if (gSaveContext.nayrusLoveTimer != 0) {
        u8* playerBase = (u8*)player + 0x1000;

        playerBase[0x72A] |= 0x40;
    }
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

void OceffWipe_Update(Actor* thisx, GameState* state) {
    OceffWipe* self = (OceffWipe*)thisx;
    PlayState* play = (PlayState*)state;

    self->actor.world.pos = play->cameraPtrs[play->activeCamera]->eye;
    if (self->timer < 240) {
        self->timer++;
    } else {
        Actor_Kill(&self->actor);
    }
}

// OceffWipe_Draw: NOT matched, ~36 insns off (difflib) after recipe R8 (MTX34_MakeTrans): the integer-store identity+translate
// block and the unfolded 0.1 scale now match. Left: VFP numbering (quake s17-s19, z s22, 0.0f hoisted to s20 early), predicated
// vstrne/vstreq animSpeed, early mov r7,#1. Same residue in OceffWipe2/4 (OceffWipe4 ~17 insns: zero/one locals hoist the consts).
#if 0
Vec3f FUN_0032e648(Camera* cam); // quake offset (returned in s0-s2)
void FUN_00371fac(MTX34* mtx, void* billboard);
void Matrix_UpdatePosition(MTX34* dst, MTX34* src, Vec3f* translate);
void FUN_00333abc(void* mat, s32 idx, f32* color);
void FUN_00333a38(void* mat, s32 idx, f32* color);

static inline void OceffWipe_TranslateNew(MTX34* mtx, f32 x, f32 y, f32 z) {
    Vec3f pos;

    pos.x = x;
    pos.y = y;
    pos.z = z;
    MTX34_MakeTrans(mtx, &pos);
}

static inline void OceffWipe_Scale(MTX34* m, f32 x, f32 y, f32 z) {
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

static inline void OceffWipe_Translate(MTX34* mtx, f32 y, f32 x, f32 z) {
    Vec3f translate;

    translate.x = x;
    translate.y = y;
    translate.z = z;
    Matrix_UpdatePosition(mtx, mtx, &translate);
}

static inline f32 OceffWipe_FovLerp(PlayState* play, f32 a, f32 b) {
    f32 fov = *(f32*)((u8*)play + 0x198);

    if (fov < 45.0f) {
        fov = 45.0f;
    } else if (fov > 60.0f) {
        fov = 60.0f;
    }
    return a + (b - a) * ((fov - 45.0f) * (1.0f / 15.0f));
}

static inline s32 OceffWipe_MaterialCount(void* cmb) {
    void** p = *(void***)((u8*)cmb + 8);

    return *(s32*)((u8*)*p + 8);
}

void OceffWipe_Draw(Actor* actor, GameState* state) {
    OceffWipe* self = (OceffWipe*)actor;
    PlayState* play = (PlayState*)state;
    Vec3f eye = play->cameraPtrs[play->activeCamera]->eye;
    MTX34 mtx;
    Vec3f quake = FUN_0032e648(play->cameraPtrs[play->activeCamera]);
    f32 x = *(f32*)((u8*)play + 0x198);
    f32 z;
    f32 frames;
    f32 alpha;
    s32 i;

    if (x < 45.0f) {
        x = 45.0f;
    } else if (x > 60.0f) {
        x = 60.0f;
    }
    z = 1330.0f + -90.0f * ((x - 45.0f) * (1.0f / 15.0f));
    frames = (f32)self->timer * ((f32)SREG(30) * (1.0f / 3.0f));
    if (frames < 32.0f) {
        z = Math_SinS((s16)(frames * 512.0f)) * z;
    }
    if (self->timer < 210) {
        alpha = 1.0f;
    } else {
        alpha = (f32)(240 - self->timer) * (1.0f / 30.0f);
    }
    OceffWipe_TranslateNew(&mtx, eye.x + quake.x, eye.y + quake.y, eye.z + quake.z);
    OceffWipe_Scale(&mtx, 0.1f, 0.1f, 0.1f);
    FUN_00371fac(&mtx, (u8*)play + 0x2FC);
    OceffWipe_Translate(&mtx, 0.0f, 0.0f, -z);
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
    self->model->unk_0C->animSpeed = (PauseContext_GetState() != 0) ? 0.0f : 1.0f;
    for (i = 0; i < OceffWipe_MaterialCount(self->cmb); i++) {
        void* mat = self->model->unk_10;
        f32 color[4];

        FUN_00333abc(mat, i, color);
        color[3] = alpha;
        FUN_00333a38(mat, i, color);
        ((u8*)*(void**)((u8*)mat + 4))[i * 292] = 1;
    }
    self->model->FUN_00372170(0);
}
#endif

GLOBAL_ASM("asm/OceffWipe_Draw.s")
}
