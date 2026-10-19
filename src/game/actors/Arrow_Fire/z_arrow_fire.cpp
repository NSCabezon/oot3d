extern "C" {
#include "z_arrow_fire.hpp"
#include "model_manager.hpp"
#include "mtx34_trans.hpp"

#define FLAGS 0x02000010

// Layout of the parent (EnArrow) fields read here.
struct EnArrowLayout {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x160];
    /* 0x0304 */ u8 timer;
    /* 0x0305 */ u8 hitFlags;
};

void ArrowFire_Init(Actor* actor, GameState* state);
void ArrowFire_Destroy(Actor* thisx, PlayState* play);
void ArrowFire_Update(Actor* thisx, PlayState* play);
void ArrowFire_Draw(Actor* actor, GameState* state);
void ArrowFire_Charge(ArrowFire* self, PlayState* play);
void ArrowFire_Fly(ArrowFire* self, PlayState* play);
void ArrowFire_Hit(ArrowFire* self, PlayState* play);

void FUN_0034708c(PlayState* play); // Magic_Reset
s32 GetMessageMode(PlayState* play);
void* ZAR_GetCMBByIndex(void* zar, s32 index);
void FUN_0047d548(SkeletonAnimationModel* model, s32 arg);

ActorInit Arrow_Fire_InitVars = {
    ACTOR_ARROW_FIRE,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(ArrowFire),
    (ActorFunc)ArrowFire_Init,
    (ActorFunc)ArrowFire_Destroy,
    (ActorFunc)ArrowFire_Update,
    (ActorFunc)ArrowFire_Draw,
};

__attribute__((used)) static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 2000, ICHAIN_STOP),
};

static inline void ArrowFire_SetupAction(ArrowFire* self, ArrowFireActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void ArrowFire_Init(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    ArrowFire* self = (ArrowFire*)actor;
    ZARInfo* zar;
    void* cmb;
    ModelAllocator* alloc;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    self->radius = 0;
    self->unk_158 = 1.0f;
    ArrowFire_SetupAction(self, ArrowFire_Charge);
    Actor_SetScale(&self->actor, 0.01f);
    self->alpha = 160;
    self->timer = 0;
    self->unk_15C = 0.0f;
    zar = &play->GetObjectStatus(actor->objBankIndex)->zarInfo;
    {
        ModelManager& mm = GetModelManager();
        mm.allocator->owner = actor->unk_178;
    }
    { ModelManager& mm = GetModelManager(); alloc = mm.allocator; }
    cmb = ZAR_GetCMBByIndex(zar, 0x26);
    self->model = (ArrowFireModel*)alloc->Create(cmb, 1);
    {
        ModelManager& mm = GetModelManager();
        mm.allocator->owner = NULL;
    }
    ((SkeletonAnimationModel*)self->model)->unk_AD[0] = 0;
    ((SkeletonAnimationModel*)self->model)->unk_0C->FUN_00372d94(zar->GetCMABByIndex(0x17));
    ((SkeletonAnimationModel*)self->model)->unk_0C->SetAnimType(1);
    ((SkeletonAnimationModel*)self->model)->unk_0C->SetAnimSpeed(2.0f);
    FUN_0047d548((SkeletonAnimationModel*)self->model, 2);
    self->actor.unk_19A = 1;
}

void ArrowFire_Destroy(Actor* thisx, PlayState* play) {
    ArrowFire* self = (ArrowFire*)thisx;

    FUN_0034708c(play);
    if (self->model != NULL) {
        self->model->Destroy();
        ((ArrowFire*)thisx)->model = NULL;
    }
}

#if 0 // draft: Draw ~237/276 words off (stack layout/0x40 frame matches; VFP/GPR numbering, hoisted zero s17 + mov r5,#1, if/else vstreq shape differ; MakeTrans + SetUnkAC family)
#define AFM(s) ((SkeletonAnimationModel*)(s)->model)
void Matrix_RotateX(MTX34* mtx, s32 mode, f32 angle);
void Matrix_RotateY(MTX34* mtx, s32 mode, f32 angle);
void Matrix_RotateZ(MTX34* mtx, s32 mode, f32 angle);
void Matrix_UpdatePosition(MTX34* dst, MTX34* src, Vec3f* translate);
void FUN_00333abc(void* mat, s32 idx, f32* color);
void FUN_00333a38(void* mat, s32 idx, f32* color);
void FUN_003339e8(void* mgr, s32 a, f32* color, s32 b);

static inline void ArrowFire_TranslateNew(MTX34* mtx, f32 x, f32 y, f32 z) {
    Vec3f t;

    t.x = x;
    t.y = y;
    t.z = z;
    MTX34_MakeTrans(mtx, &t);
}

static inline void ArrowFire_Translate(MTX34* mtx, f32 x, f32 y, f32 z) {
    Vec3f t;

    t.x = x;
    t.y = y;
    t.z = z;
    Matrix_UpdatePosition(mtx, mtx, &t);
}

static inline void ArrowFire_Scale(MTX34* m, f32 x, f32 y, f32 z) {
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

void ArrowFire_Draw(Actor* actor, GameState* state) {
    ArrowFire* self = (ArrowFire*)actor;
    EnArrowLayout* arrow;
    Actor* transform;
    MTX34 mtx;

    AFM(self)->unk_AD[0] = 0;
    arrow = (EnArrowLayout*)self->actor.parent;
    if ((arrow == NULL) || (arrow->actor.update == NULL)) {
        return;
    }
    if (self->timer >= 255) {
        return;
    }
    transform = (arrow->hitFlags & 2) ? &self->actor : &arrow->actor;
    ArrowFire_TranslateNew(&mtx, transform->world.pos.y, transform->world.pos.x, transform->world.pos.z);
    Matrix_RotateY(&mtx, 1, transform->shape.rot.y * 0.000095873802f);
    Matrix_RotateX(&mtx, 1, transform->shape.rot.x * 0.000095873802f);
    Matrix_RotateZ(&mtx, 1, transform->shape.rot.z * 0.000095873802f);
    ArrowFire_Scale(&mtx, 0.01f, 0.01f, 0.01f);
    Matrix_RotateZ(&mtx, 1, 0.0f);
    Matrix_RotateX(&mtx, 1, 1.5707964f);
    if (self->timer != 0) {
        ArrowFire_Translate(&mtx, 0.0f, 0.0f, 0.0f);
    } else {
        ArrowFire_Translate(&mtx, 0.0f, 1500.0f, 0.0f);
    }
    ArrowFire_Scale(&mtx, self->radius * 0.2f, self->unk_158 * 4.0f, self->radius * 0.2f);
    ArrowFire_Translate(&mtx, 0.0f, -700.0f, 0.0f);
    AFM(self)->unk_0C->animSpeed = (PauseContext_GetState() != 0) ? 0.0f : 2.0f;
    {
        void* mat = AFM(self)->unk_10;
        f32 color[4];

        FUN_00333abc(mat, 0, color);
        color[3] = self->alpha * (1.0f / 255.0f);
        FUN_00333a38(mat, 0, color);
        ((u8*)*(void**)((u8*)mat + 4))[0] = 1;
    }
    AFM(self)->SetUnkAC(1);
    AFM(self)->FUN_003721e0(&mtx);
    {
        SkeletonAnimationModel* m = AFM(self);

        m->unk_24 = mtx.data[0][3];
        m->unk_28 = mtx.data[1][3];
        m->unk_2C = mtx.data[2][3];
    }
    AFM(self)->unk_AD[0] = 1;
    AFM(self)->FUN_00372170(0);
    if (self->unk_15C > 0.0f) {
        f32 color[4];

        color[0] = (self->unk_15C * 40.0f) * (1.0f / 255.0f);
        color[1] = 0.0f;
        color[2] = 0.0f;
        color[3] = (f32)((s32)(self->unk_15C * 150.0f) & 0xFF) * (1.0f / 255.0f);
        FUN_003339e8((u8*)&GetModelManager() + 0x25F0, 2, color, 0);
    }
}
#endif

GLOBAL_ASM("asm/ArrowFire_Draw.s")

void ArrowFire_Charge(ArrowFire* self, PlayState* play) {
    EnArrowLayout* arrow = (EnArrowLayout*)self->actor.parent;

    if ((arrow == NULL) || (arrow->actor.update == NULL)) {
        Actor_Kill(&self->actor);
        return;
    }

    if (self->radius < 10) {
        self->radius += 1;
    }
    self->actor.world.pos = arrow->actor.world.pos;
    self->actor.shape.rot = arrow->actor.shape.rot;

    FUN_00373264(&self->actor, 0x0100007e);

    if (arrow->actor.parent == NULL) {
        self->unkPos = self->actor.world.pos;
        self->radius = 10;
        ArrowFire_SetupAction(self, ArrowFire_Fly);
        self->alpha = 255;
    }
}

static inline void ArrowFire_Step(Vec3f* unkPos, Vec3f* firePos, f32 scale) {
    unkPos->x += ((firePos->x - unkPos->x) * scale);
    unkPos->y += ((firePos->y - unkPos->y) * scale);
    unkPos->z += ((firePos->z - unkPos->z) * scale);
}

void ArrowFire_Fly(ArrowFire* self, PlayState* play) {
    EnArrowLayout* arrow;
    f32 distanceScaled;
    EnArrowLayout* a2;

    arrow = (EnArrowLayout*)self->actor.parent;
    a2 = arrow;
    if ((arrow == NULL) || (arrow->actor.update == NULL)) {
        Actor_Kill(&self->actor);
        return;
    }
    self->actor.world.pos = arrow->actor.world.pos;
    self->actor.shape.rot = arrow->actor.shape.rot;
    distanceScaled = Math_Vec3f_DistXYZ(&self->unkPos, &self->actor.world.pos) * (1.0f / 24.0f);
    self->unk_158 = distanceScaled;
    distanceScaled = (distanceScaled <= 1.0f) ? 1.0f : distanceScaled;
    self->unk_158 = distanceScaled;
    ArrowFire_Step(&self->unkPos, &self->actor.world.pos, 0.05f);

    if (arrow->hitFlags & 1) {
        Audio_PlayActorSound2(&self->actor, 0x0100012c);
        ArrowFire_SetupAction(self, ArrowFire_Hit);
        self->timer = 32;
        self->alpha = 255;
    } else if (a2->timer < 34) {
        if (self->alpha < 35) {
            Actor_Kill(&self->actor);
        } else {
            self->alpha -= 0x19;
        }
    }
}

void ArrowFire_Hit(ArrowFire* self, PlayState* play) {
    f32 scale;
    f32 offset;
    u16 timer;

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

    timer = self->timer;
    if (timer != 0) {
        self->timer--;

        if (self->timer >= 8) {
            offset = ((self->timer - 8) * (1.0f / 24.0f));
            offset = SQ(offset);
            self->radius = (((1.0f - offset) * scale) + 10.0f);
            self->unk_158 += ((2.0f - self->unk_158) * 0.1f);
            if (self->timer < 16) {
                self->alpha = (self->timer - 8) * 0x23;
            }
        }
    }

    if (self->timer > 8) {
        if (self->unk_15C < 1.0f) {
            self->unk_15C += 0.25f;
        }
    } else {
        if (self->unk_15C > 0.0f) {
            self->unk_15C -= 0.125f;
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

void ArrowFire_Update(Actor* thisx, PlayState* play) {
    ArrowFire* self = (ArrowFire*)thisx;

    s32 msgMode = GetMessageMode(play);

    if ((msgMode == 0xD) || (msgMode == 0x11)) {
        Actor_Kill(&self->actor);
        return;
    }

    self->actionFunc(self, play);
}

}
