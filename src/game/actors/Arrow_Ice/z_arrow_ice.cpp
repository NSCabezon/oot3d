extern "C" {
#include "z_arrow_ice.hpp"
#include "model_manager.hpp"
#include "mtx34_trans.hpp"

#define FLAGS 0x02000010

void ArrowIce_Init(Actor* actor, GameState* state);
void ArrowIce_Destroy(Actor* thisx, PlayState* play);
void ArrowIce_Update(Actor* thisx, PlayState* play);
void ArrowIce_Draw(Actor* actor, GameState* state);
void ArrowIce_Charge(ArrowIce* self, PlayState* play);
void ArrowIce_Fly(ArrowIce* self, PlayState* play);
void ArrowIce_Hit(ArrowIce* self, PlayState* play);

void FUN_0034708c(PlayState* play); // Magic_Reset
s32 GetMessageMode(PlayState* play);
extern InitChainEntry ArrowIce_sInitChain[];
void* ZAR_GetCMBByIndex(void* zar, s32 index);
void FUN_0047d548(SkeletonAnimationModel* model, s32 arg);

GLOBAL_ASM("data/z_Arrow_Ice.data.s")
GLOBAL_ASM("../src/game/actors/Arrow_Ice/data_syms.s")

ActorInit Arrow_Ice_InitVars = {
    ACTOR_ARROW_ICE,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(ArrowIce),
    (ActorFunc)ArrowIce_Init,
    (ActorFunc)ArrowIce_Destroy,
    (ActorFunc)ArrowIce_Update,
    (ActorFunc)ArrowIce_Draw,
};


static inline void ArrowIce_SetupAction(ArrowIce* self, ArrowIceActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void ArrowIce_Init(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    ArrowIce* self = (ArrowIce*)actor;
    ZARInfo* zar;
    void* cmb;
    ModelAllocator* alloc;

    Actor_ProcessInitChain(&self->actor, ArrowIce_sInitChain);
    self->radius = 0;
    self->unk_1B8 = 1.0f;
    ArrowIce_SetupAction(self, ArrowIce_Charge);
    Actor_SetScale(&self->actor, 0.01f);
    self->alpha = 100;
    self->timer = 0;
    self->unk_1BC = 0.0f;
    zar = &play->GetObjectStatus(actor->objBankIndex)->zarInfo;
    {
        ModelManager& mm = GetModelManager();
        mm.allocator->owner = actor->unk_178;
    }
    { ModelManager& mm = GetModelManager(); alloc = mm.allocator; }
    cmb = ZAR_GetCMBByIndex(zar, 0x27);
    self->model = (ArrowIceModel*)alloc->Create(cmb, 1);
    {
        ModelManager& mm = GetModelManager();
        mm.allocator->owner = NULL;
    }
    ((SkeletonAnimationModel*)self->model)->unk_AD[0] = 0;
    self->animCtl = ((SkeletonAnimationModel*)self->model)->unk_0C;
    self->animCtl->FUN_00372d94(zar->GetCMABByIndex(0x18));
    self->animCtl->SetAnimType(1);
    self->animCtl->SetAnimSpeed(2.0f);
    FUN_0047d548((SkeletonAnimationModel*)self->model, 2);
    self->actor.unk_19A = 1;
}

void ArrowIce_Destroy(Actor* thisx, PlayState* play) {
    ArrowIce* self = (ArrowIce*)thisx;

    FUN_0034708c(play);
    ArrowIceModel* model = self->model;
    if (model != NULL) {
        model->Destroy();
        self->model = NULL;
    }
}

void ArrowIce_Update(Actor* thisx, PlayState* play) {
    ArrowIce* self = (ArrowIce*)thisx;
    s32 msgMode = GetMessageMode(play);

    if (msgMode == 0xD || msgMode == 0x11) {
        Actor_Kill(&self->actor);
        return;
    }
    self->actionFunc(self, play);
}


// ArrowIce_Draw: NOT matched (best 235/288 words off, 1164 vs 1152 B; clone of ArrowFire_Draw draft). Structure,
// stack layout (0x40 frame) and MakeTrans/Scale blocks match; ROM keeps 9.587e-5 (s16) and 0.0 (s17/s18) hoisted
// across the whole body, 3 callee-saved VFP regs (vpush {d8,d9}) and `mov r5,#1` shared by SetUnkAC/strb/unk_AD,
// and the color block computes all three u*K products before the vcvt/and chain. Draft below.
#if 0

#define AIM(s) ((SkeletonAnimationModel*)(s)->model)
void Matrix_RotateX(MTX34* mtx, s32 mode, f32 angle);
void Matrix_RotateY(MTX34* mtx, s32 mode, f32 angle);
void Matrix_RotateZ(MTX34* mtx, s32 mode, f32 angle);
void Matrix_UpdatePosition(MTX34* dst, MTX34* src, Vec3f* translate);
void FUN_00333abc(void* mat, s32 idx, f32* color);
void FUN_00333a38(void* mat, s32 idx, f32* color);
void FUN_003339e8(void* mgr, s32 a, f32* color, s32 b);

static inline void ArrowIce_TranslateNew(MTX34* mtx, f32 x, f32 y, f32 z) {
    Vec3f t;

    t.x = x;
    t.y = y;
    t.z = z;
    MTX34_MakeTrans(mtx, &t);
}

static inline void ArrowIce_Translate(MTX34* mtx, f32 x, f32 y, f32 z) {
    Vec3f t;

    t.x = x;
    t.y = y;
    t.z = z;
    Matrix_UpdatePosition(mtx, mtx, &t);
}

static inline void ArrowIce_Scale(MTX34* m, f32 x, f32 y, f32 z) {
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

void ArrowIce_Draw(Actor* actor, GameState* state) {
    ArrowIce* self = (ArrowIce*)actor;
    ArrowIceParentArrow* arrow;
    Actor* transform;
    MTX34 mtx;

    arrow = (ArrowIceParentArrow*)self->actor.parent;
    AIM(self)->unk_AD[0] = 0;
    if ((arrow == NULL) || (arrow->actor.update == NULL)) {
        return;
    }
    if (self->timer >= 255) {
        return;
    }
    transform = (arrow->hitFlags & 2) ? actor : &arrow->actor;
    ArrowIce_TranslateNew(&mtx, transform->world.pos.x, transform->world.pos.y, transform->world.pos.z);
    Matrix_RotateY(&mtx, 1, transform->shape.rot.y * 0.000095873802f);
    Matrix_RotateX(&mtx, 1, transform->shape.rot.x * 0.000095873802f);
    Matrix_RotateZ(&mtx, 1, transform->shape.rot.z * 0.000095873802f);
    ArrowIce_Scale(&mtx, 0.01f, 0.01f, 0.01f);
    Matrix_RotateZ(&mtx, 1, 0.0f);
    Matrix_RotateX(&mtx, 1, 1.5707964f);
    if (self->timer != 0) {
        ArrowIce_Translate(&mtx, 0.0f, 0.0f, 0.0f);
    } else {
        ArrowIce_Translate(&mtx, 0.0f, 1500.0f, 0.0f);
    }
    ArrowIce_Scale(&mtx, self->radius * 0.2f, self->unk_1B8 * 3.0f, self->radius * 0.2f);
    ArrowIce_Translate(&mtx, 0.0f, -700.0f, 0.0f);
    {
        f32 animSpeed = 2.0f;

        if (PauseContext_GetState() != 0) {
            animSpeed = 0.0f;
        }
        self->animCtl->animSpeed = animSpeed;
    }
    {
        void* mat = AIM(self)->unk_10;
        f32 color[4];

        FUN_00333abc(mat, 0, color);
        color[3] = self->alpha * (1.0f / 255.0f);
        FUN_00333a38(mat, 0, color);
        ((u8*)*(void**)((u8*)mat + 4))[0] = 1;
    }
    AIM(self)->SetUnkAC(1);
    AIM(self)->FUN_003721e0(&mtx);
    {
        SkeletonAnimationModel* m = AIM(self);

        m->unk_24 = mtx.data[0][3];
        m->unk_28 = mtx.data[1][3];
        m->unk_2C = mtx.data[2][3];
    }
    AIM(self)->unk_AD[0] = 1;
    AIM(self)->FUN_00372170(0);
    if (self->unk_1BC > 0.0f) {
        f32 color[4];

        f32 u = self->unk_1BC;
        s32 ci1 = (s32)(u * 10.0f);
        s32 ci2 = (s32)(u * 50.0f);
        s32 ci3 = (s32)(u * 150.0f);

        color[0] = 0.0f;
        color[1] = (f32)(u32)(ci1 & 0xFF) * (1.0f / 255.0f);
        color[2] = (f32)(u32)(ci2 & 0xFF) * (1.0f / 255.0f);
        color[3] = (f32)(u32)(ci3 & 0xFF) * (1.0f / 255.0f);
        FUN_003339e8((u8*)&GetModelManager() + 0x25F0, 2, color, 0);
    }
}
#endif

GLOBAL_ASM("asm/ArrowIce_Draw.s")

void ArrowIce_Charge(ArrowIce* self, PlayState* play) {
    ArrowIceParentArrow* arrow = (ArrowIceParentArrow*)self->actor.parent;

    if (arrow == NULL || arrow->actor.update == NULL) {
        Actor_Kill(&self->actor);
        return;
    }
    if (self->radius < 10) {
        self->radius += 1;
    }
    self->actor.world.pos = arrow->actor.world.pos;
    self->actor.shape.rot = arrow->actor.shape.rot;
    FUN_00373264(&self->actor, 0x0100007F);
    if (arrow->actor.parent == NULL) {
        self->unkPos = self->actor.world.pos;
        self->radius = 10;
        self->actionFunc = ArrowIce_Fly;
        self->alpha = 255;
    }
}

static inline void ArrowIce_MoveUnkPos(Vec3f* unkPos, Vec3f* icePos, f32 scale) {
    unkPos->x += ((icePos->x - unkPos->x) * scale);
    unkPos->y += ((icePos->y - unkPos->y) * scale);
    unkPos->z += ((icePos->z - unkPos->z) * scale);
}

// arrow2 is a dead alias used only for the timer read: it makes armcc keep a second copy of the parent
// in r1 (ROM `movs r1, r0`) and save r5/r6 for the Vec3f copy.
void ArrowIce_Fly(ArrowIce* self, PlayState* play) {
    ArrowIceParentArrow* arrow = (ArrowIceParentArrow*)self->actor.parent;
    ArrowIceParentArrow* arrow2 = arrow;
    f32 distanceScaled;

    if (arrow == NULL || arrow->actor.update == NULL) {
        Actor_Kill(&self->actor);
        return;
    }
    self->actor.world.pos = arrow->actor.world.pos;
    self->actor.shape.rot = arrow->actor.shape.rot;
    distanceScaled = Math_Vec3f_DistXYZ(&self->unkPos, &self->actor.world.pos) * (1.0f / 24.0f);
    self->unk_1B8 = distanceScaled;
    if (self->unk_1B8 < 1.0f) {
        self->unk_1B8 = 1.0f;
    }
    ArrowIce_MoveUnkPos(&self->unkPos, &self->actor.world.pos, 0.05f);
    if (arrow->hitFlags & 1) {
        Audio_PlayActorSound2(&self->actor, 0x0100012D);
        self->actionFunc = ArrowIce_Hit;
        self->timer = 32;
        self->alpha = 255;
    } else if (arrow2->timer < 34) {
        if (self->alpha < 35) {
            Actor_Kill(&self->actor);
        } else {
            self->alpha -= 0x19;
        }
    }
}

void ArrowIce_Hit(ArrowIce* self, PlayState* play) {
    f32 scale;
    f32 offset;

    if (self->actor.unk_F8 < 50.0f) {
        scale = 10.0f;
    } else {
        if (950.0f < self->actor.unk_F8) {
            scale = 310.0f;
        } else {
            scale = self->actor.unk_F8;
            scale = ((scale - 50.0f) * (1.0f / 3.0f)) + 10.0f;
        }
    }
    if (self->timer != 0) {
        self->timer--;
        if (self->timer >= 8) {
            offset = ((self->timer - 8) * (1.0f / 24.0f));
            offset = SQ(offset);
            self->radius = (((1.0f - offset) * scale) + 10.0f);
            self->unk_1B8 += ((2.0f - self->unk_1B8) * 0.1f);
            if (self->timer < 16) {
                self->alpha = (self->timer - 8) * 0x23;
            }
        }
    }
    if (self->timer > 8) {
        if (self->unk_1BC < 1.0f) {
            self->unk_1BC += 0.25f;
        }
    } else {
        if (self->unk_1BC > 0.0f) {
            self->unk_1BC -= 0.125f;
        }
    }
    if (self->timer < 8) {
        self->alpha = 0;
    }
    if (self->timer == 0) {
        self->timer = 255;
        Actor_Kill(&self->actor);
    }
}

}
