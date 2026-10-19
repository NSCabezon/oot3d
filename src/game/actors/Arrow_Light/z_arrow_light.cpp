extern "C" {
#include "z_arrow_light.hpp"
#include "model_manager.hpp"

#define FLAGS 0x02000010

void ArrowLight_Init(Actor* actor, GameState* state);
void ArrowLight_Destroy(Actor* actor, GameState* state);
void ArrowLight_Update(Actor* actor, GameState* state);
void ArrowLight_Draw(Actor* actor, GameState* state);

void ArrowLight_Charge(ArrowLight* self, PlayState* play);
void ArrowLight_Fly(ArrowLight* self, PlayState* play);
void ArrowLight_Hit(ArrowLight* self, PlayState* play);

void FUN_0034708c(PlayState* play);
s32 GetMessageMode(GameState* state);
void FUN_00373264(Actor* actor, s32 arg1);
void* ZAR_GetCMBByIndex(void* zar, s32 index);
void FUN_0047d548(SkeletonAnimationModel* model, s32 arg);

ActorInit Arrow_Light_InitVars = {
    ACTOR_ARROW_LIGHT,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(ArrowLight),
    (ActorFunc)ArrowLight_Init,
    (ActorFunc)ArrowLight_Destroy,
    (ActorFunc)ArrowLight_Update,
    (ActorFunc)ArrowLight_Draw,
};

__attribute__((used)) static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 2000, ICHAIN_STOP),
};

void ArrowLight_Init(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    ArrowLight* self = (ArrowLight*)actor;
    ZARInfo* zar;
    void* cmb;
    ModelAllocator* alloc;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    self->radius = 0;
    self->unk_160 = 1.0f;
    self->actionFunc = ArrowLight_Charge;
    Actor_SetScale(&self->actor, 0.01f);
    self->alpha = 130;
    self->timer = 0;
    self->unk_164 = 0.0f;
    zar = &play->GetObjectStatus(actor->objBankIndex)->zarInfo;
    {
        ModelManager& mm = GetModelManager();
        mm.allocator->owner = actor->unk_178;
    }
    { ModelManager& mm = GetModelManager(); alloc = mm.allocator; }
    cmb = ZAR_GetCMBByIndex(zar, 0x28);
    self->model = (ArrowLightModel*)alloc->Create(cmb, 1);
    {
        ModelManager& mm = GetModelManager();
        mm.allocator->owner = NULL;
    }
    ((SkeletonAnimationModel*)self->model)->unk_AD[0] = 0;
    self->anim = ((SkeletonAnimationModel*)self->model)->unk_0C;
    self->anim->FUN_00372d94(zar->GetCMABByIndex(0x19));
    self->anim->SetAnimType(1);
    self->anim->SetAnimSpeed(2.0f);
    FUN_0047d548((SkeletonAnimationModel*)self->model, 2);
    self->actor.unk_19A = 1;
}

// EnArrow fields used here (only the offsets matter)
struct EnArrowLayout {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x160];
    /* 0x0304 */ u8 timer;
    /* 0x0305 */ u8 hitFlags;
};

void ArrowLight_Destroy(Actor* actor, GameState* state) {
    ArrowLight* self = (ArrowLight*)actor;
    ArrowLightModel* model;

    FUN_0034708c((PlayState*)state);
    model = self->model;
    if (model != NULL) {
        model->Destroy();
        self->model = NULL;
    }
}

void ArrowLight_Update(Actor* actor, GameState* state) {
    ArrowLight* self = (ArrowLight*)actor;

    s32 msgMode = GetMessageMode(state);
    if ((msgMode == 0xD) || (msgMode == 0x11)) {
        Actor_Kill(actor);
        return;
    }

    self->actionFunc(self, (PlayState*)state);
}

void ArrowLight_Charge(ArrowLight* self, PlayState* play) {
    EnArrowLayout* arrow = (EnArrowLayout*)self->actor.parent;

    if ((arrow == NULL) || (arrow->actor.update == NULL)) {
        Actor_Kill(&self->actor);
        return;
    }

    if (self->radius < 10) {
        self->radius += 1;
    }
    // copy position and rotation from arrow
    self->actor.world.pos = arrow->actor.world.pos;
    self->actor.shape.rot = arrow->actor.shape.rot;

    FUN_00373264(&self->actor, 0x01000080);

    // if arrow has no parent, player has fired the arrow
    if (arrow->actor.parent == NULL) {
        self->unkPos = self->actor.world.pos;
        self->radius = 10;
        self->actionFunc = ArrowLight_Fly;
        self->alpha = 255;
    }
}

static inline void ArrowLight_MoveTowards(Vec3f* unkPos, Vec3f* lightPos, f32 scale) {
    unkPos->x += ((lightPos->x - unkPos->x) * scale);
    unkPos->y += ((lightPos->y - unkPos->y) * scale);
    unkPos->z += ((lightPos->z - unkPos->z) * scale);
}

void ArrowLight_Hit(ArrowLight* self, PlayState* play) {
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
            self->unk_160 += ((2.0f - self->unk_160) * 0.1f);
            if (self->timer < 16) {
                self->alpha = (self->timer - 8) * 0x23;
            }
        }
    }

    if (self->timer > 8) {
        if (self->unk_164 < 1.0f) {
            self->unk_164 += 0.25f;
        }
    } else {
        if (self->unk_164 > 0.0f) {
            self->unk_164 -= 0.125f;
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

void ArrowLight_Fly(ArrowLight* self, PlayState* play) {
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
    self->unk_160 = distanceScaled;
    distanceScaled = (distanceScaled <= 1.0f) ? 1.0f : distanceScaled;
    self->unk_160 = distanceScaled;
    ArrowLight_MoveTowards(&self->unkPos, &self->actor.world.pos, 0.05f);

    if (arrow->hitFlags & 1) {
        Audio_PlayActorSound2(&self->actor, 0x0100012E);
        self->actionFunc = ArrowLight_Hit;
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

#if 0 // draft: 259/288 words off, VFP/GPR numbering family (MakeTrans + SetUnkAC, see Arrow_Fire / CLAUDE.md); size 1176 vs 1152
#define ALM(s) ((SkeletonAnimationModel*)(s)->model)
void Matrix_RotateX(MTX34* mtx, s32 mode, f32 angle);
void Matrix_RotateY(MTX34* mtx, s32 mode, f32 angle);
void Matrix_RotateZ(MTX34* mtx, s32 mode, f32 angle);
void Matrix_UpdatePosition(MTX34* dst, MTX34* src, Vec3f* translate);
void FUN_00333abc(void* mat, s32 idx, f32* color);
void FUN_00333a38(void* mat, s32 idx, f32* color);
void FUN_003339e8(void* mgr, s32 a, f32* color, s32 b);

static inline void ArrowLight_TranslateNew(MTX34* mtx, f32 x, f32 y, f32 z) {
    Vec3f t;

    t.x = x;
    t.y = y;
    t.z = z;
    MTX34_MakeTrans(mtx, &t);
}

static inline void ArrowLight_Translate(MTX34* mtx, f32 x, f32 y, f32 z) {
    Vec3f t;

    t.x = x;
    t.y = y;
    t.z = z;
    Matrix_UpdatePosition(mtx, mtx, &t);
}

static inline void ArrowLight_Scale(MTX34* m, f32 x, f32 y, f32 z) {
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

void ArrowLight_Draw(Actor* actor, GameState* state) {
    ArrowLight* self = (ArrowLight*)actor;
    EnArrowLayout* arrow;
    Actor* transform;
    MTX34 mtx;

    ALM(self)->unk_AD[0] = 0;
    arrow = (EnArrowLayout*)self->actor.parent;
    if ((arrow == NULL) || (arrow->actor.update == NULL)) {
        return;
    }
    if (self->timer >= 255) {
        return;
    }
    transform = (arrow->hitFlags & 2) ? &self->actor : &arrow->actor;
    ArrowLight_TranslateNew(&mtx, transform->world.pos.x, transform->world.pos.y, transform->world.pos.z);
    Matrix_RotateY(&mtx, 1, transform->shape.rot.y * 0.000095873802f);
    Matrix_RotateX(&mtx, 1, transform->shape.rot.x * 0.000095873802f);
    Matrix_RotateZ(&mtx, 1, transform->shape.rot.z * 0.000095873802f);
    ArrowLight_Scale(&mtx, 0.01f, 0.01f, 0.01f);
    Matrix_RotateZ(&mtx, 1, 0.0f);
    Matrix_RotateX(&mtx, 1, 1.5707964f);
    if (self->timer != 0) {
        ArrowLight_Translate(&mtx, 0.0f, 0.0f, 0.0f);
    } else {
        ArrowLight_Translate(&mtx, 0.0f, 1500.0f, 0.0f);
    }
    ArrowLight_Scale(&mtx, self->radius * 0.2f, self->unk_160 * 4.0f, self->radius * 0.2f);
    ArrowLight_Translate(&mtx, 0.0f, -700.0f, 0.0f);
    self->anim->animSpeed = (PauseContext_GetState() != 0) ? 0.0f : 2.0f;
    {
        void* mat = ALM(self)->unk_10;
        f32 color[4];

        FUN_00333abc(mat, 0, color);
        color[3] = self->alpha * (1.0f / 255.0f);
        FUN_00333a38(mat, 0, color);
        ((u8*)*(void**)((u8*)mat + 4))[0] = 1;
    }
    ALM(self)->SetUnkAC(1);
    ALM(self)->FUN_003721e0(&mtx);
    {
        SkeletonAnimationModel* m = ALM(self);

        m->unk_24 = mtx.data[0][3];
        m->unk_28 = mtx.data[1][3];
        m->unk_2C = mtx.data[2][3];
    }
    ALM(self)->unk_AD[0] = 1;
    ALM(self)->FUN_00372170(0);
    if (self->unk_164 > 0.0f) {
        f32 color[4];

        color[0] = (f32)((s32)(self->unk_164 * 30.0f) & 0xFF) * (1.0f / 255.0f);
        color[1] = (f32)((s32)(self->unk_164 * 40.0f) & 0xFF) * (1.0f / 255.0f);
        color[2] = 0.0f;
        color[3] = (f32)((s32)(self->unk_164 * 150.0f) & 0xFF) * (1.0f / 255.0f);
        FUN_003339e8((u8*)&GetModelManager() + 0x25F0, 2, color, 0);
    }
}
#endif
GLOBAL_ASM("asm/ArrowLight_Draw.s")

}
