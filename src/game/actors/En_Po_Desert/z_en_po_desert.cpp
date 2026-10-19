extern "C" {
#include "z_en_po_desert.hpp"

#define FLAGS 0x00001090

void EnPoDesert_Init(Actor* thisx, PlayState* play);
void EnPoDesert_Destroy(Actor* thisx, PlayState* play);
void EnPoDesert_Update(Actor* thisx, PlayState* play);
void EnPoDesert_Draw(Actor* actor, GameState* state);
void EnPoDesert_MoveToNextPoint(EnPoDesert* self, PlayState* play);
void EnPoDesert_Disappear(EnPoDesert* self, PlayState* play);
void EnPoDesert_WaitForPlayer(EnPoDesert* self, PlayState* play);
void LightContext_RemoveLight(PlayState* play, void* lightCtx, void* lightNode);
s32 Gameplay_InCsMode(PlayState* play);
void Animation_MorphToLoop(SkelAnime* skelAnime, s32 idx, f32 morph);
void Animation_MorphToPlayOnce(SkelAnime* skelAnime, s32 idx, f32 morph);
f32 Actor_WorldDistXZToPoint(Actor* actor, Vec3f* point);
s16 Actor_WorldYawTowardPoint(Actor* actor, Vec3f* point);
void Actor_MoveXZGravity(Actor* actor);
void SkelAnime_Update(SkelAnime* skelAnime);
void FUN_00180fd4(EnPoDesert* self, PlayState* play);
f32 __hardfp_sinf(f32 x);

GLOBAL_ASM("data/z_En_Po_Desert.data.s")

GLOBAL_ASM("../src/game/actors/En_Po_Desert/data_syms.s")

extern ColliderCylinderInit EnPoDesert_sCylinderInit;
extern InitChainEntry EnPoDesert_sInitChain[];

static inline s32 EnPoDesert_Frames(s16 t) {
    if (t > 0) {
        return (s32)(((f32)t * 2.0f) / 3.0f + 0.5f);
    }
    return (s32)(((f32)t * 2.0f) / 3.0f - 0.5f);
}

ActorInit En_Po_Desert_InitVars = {
    ACTOR_EN_PO_DESERT,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_PO_FIELD,
    sizeof(EnPoDesert),
    (ActorFunc)EnPoDesert_Init,
    (ActorFunc)EnPoDesert_Destroy,
    (ActorFunc)EnPoDesert_Update,
    (ActorFunc)EnPoDesert_Draw,
};

void* LightContext_InsertLight(PlayState* play, void* lightCtx, void* lightInfo);
void Lights_PointNoGlowSetInfo(void* light, f32 x, f32 y, f32 z, u8 r, u8 g, u8 b, s16 radius, s32 pad);

static inline void EnPoDesert_SetNextPathPointInl(EnPoDesert* self, PlayState* play) {
    Path* path = &((Path*)(*(u8**)((u8*)play + 0x5C20)))[self->actor.params];
    Vec3s* pathPoint;

    Animation_MorphToLoop(&self->skelAnime, 0, -6.0f);
    pathPoint = &path->points[self->currentPathPoint];
    self->actor.home.pos.x = pathPoint->x;
    self->actor.home.pos.y = pathPoint->y;
    self->actor.home.pos.z = pathPoint->z;
    {
        f32 dist = Actor_WorldDistXZToPoint(&self->actor, &self->actor.home.pos);

        self->initDistToNextPoint = dist;
        if (!(dist >= 1.0f)) {
            dist = 1.0f;
        }
        self->initDistToNextPoint = dist;
    }
    self->yDiff = self->actor.home.pos.y - self->actor.world.pos.y;
    *(f32*)((u8*)self + 0x6C) = 0.0f;
    self->currentPathPoint++;
    if (path->count == self->currentPathPoint) {
        self->currentPathPoint = 0;
    }
    self->actionFunc = EnPoDesert_WaitForPlayer;
}

void EnPoDesert_Init(Actor* thisx, PlayState* play) {
    EnPoDesert* self = (EnPoDesert*)thisx;

    FUN_00372f38(thisx, play, &self->skelModel, 1, 0);
    Actor_ProcessInitChain(&self->actor, EnPoDesert_sInitChain);
    FUN_00353c9c(thisx, play, &self->skelAnime, 0, 2, self->jointTable, self->morphTable, 16);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, &EnPoDesert_sCylinderInit);
    self->lightColor.r = 255;
    self->lightColor.g = 255;
    self->lightColor.b = 210;
    self->lightColor.a = 255;
    self->lightNode = LightContext_InsertLight(play, (u8*)play + 0xA70, &self->lightInfo);
    Lights_PointNoGlowSetInfo(&self->lightInfo, (s16)self->actor.home.pos.x, (s16)self->actor.home.pos.y,
                              (s16)self->actor.home.pos.z, 255, 255, 255, 200, 0);
    ActorShape_Init(&self->actor.shape, 0.0f, (ActorShadowFunc)ActorShadow_DrawCircle, 37.0f);
    self->currentPathPoint = 1;
    self->actor.params = (self->actor.params >> 8) & 0xFF;
    self->targetY = self->actor.world.pos.y;
    EnPoDesert_SetNextPathPointInl(self, play);
}

void EnPoDesert_Destroy(Actor* thisx, PlayState* play) {
    EnPoDesert* self = (EnPoDesert*)thisx;

    SkelAnime_Free2(&self->skelAnime);
    FUN_003508b8(thisx, self->skelModel, 0);
    LightContext_RemoveLight(play, (u8*)play + 0xA70, self->lightNode);
    Collider_DestroyCylinder(play, &self->collider);
}

// Unmatched: Update (5/84 words off: flags/shadowDraw regs r1/r2 vs r0/r1 in the lens branch).


static inline void EnPoDesert_UpdateSpeedModifierInl(EnPoDesert* self) {
    f32 third;
    f32 half;
    f32 two;
    s32 rounded;
    s16 timer;

    if (self->speedModifier == 0) {
        self->speedModifier = 48;
    }
    self->speedModifier--;
    timer = self->speedModifier;
    two = 2.0f;
    third = 1.0f / 3.0f;
    half = 0.5f;
    if (timer > 0) {
        rounded = (s32)(half + ((f32)timer * two) * third);
    } else {
        rounded = (s32)(((f32)timer * two) * third - half);
    }
    self->actor.world.pos.y = Math_SinS(rounded * 0x800) * 13.0f + self->targetY;
}

void EnPoDesert_Update(Actor* thisx, PlayState* play) {
    EnPoDesert* self = (EnPoDesert*)thisx;
    EnPoDesert* s2 = self;
    FUN_003731e0(&self->skelAnime);
    self->actionFunc(self, play);
    Actor_MoveForward(thisx);
    EnPoDesert_UpdateSpeedModifierInl(self);
    Actor_UpdateBgCheckInfo(play, thisx, 0.0f, 27.0f, 60.0f, 4);
    Actor_SetFocus(thisx, 42.0f);
    Collider_UpdateCylinder(thisx, &s2->collider);
    CollisionCheck_SetOC(play, (CollisionCheckContext*)((u8*)play + 0x5C78), &self->collider.base);
    if (*(u8*)((u8*)play + 0x208F) != 0) {
        thisx->flags |= 0x1 | 0x80;
        thisx->shape.shadowDraw = (ActorShadowFunc)ActorShadow_DrawCircle;
    } else {
        thisx->flags &= ~(0x1 | 0x80);
        thisx->shape.shadowDraw = NULL;
    }
}

typedef struct {
    f32 r;
    f32 g;
    f32 b;
    f32 a;
} EnPoDesertColor;

void FUN_0037266c(SkeletonAnimationModel* model, s32 idx);
void FUN_0036932c(SkeletonAnimationModel* model, s32 idx);
void FUN_00357a50(SkelAnime* skelAnime, s32 idx, s32 count, void* colors, s32 arg4);
void FUN_00357388(Actor* actor, EnPoDesertColor* color, s32 arg2, s32 arg3);
void FUN_00358778(SkeletonAnimationModel* model, s32 arg1, s32 arg2, EnPoDesertColor* color, s32 arg4);
s32 EnPoDesert_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void EnPoDesert_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);

void EnPoDesert_Draw(Actor* actor, GameState* state) {
    EnPoDesert* self = (EnPoDesert*)actor;
    EnPoDesertColor color;

    FUN_0037266c(self->skelAnime.unk_28, 7);
    FUN_0037266c(self->skelAnime.unk_28, 0);
    FUN_0037266c(self->skelAnime.unk_28, 3);
    FUN_0036932c(self->skelAnime.unk_28, 1);
    FUN_0036932c(self->skelAnime.unk_28, 4);
    FUN_0036932c(self->skelAnime.unk_28, 5);
    FUN_0036932c(self->skelAnime.unk_28, 2);
    if (self->actor.flags & 0x80) {
        color.r = self->lightColor.r * (1.0f / 255.0f);
        color.g = self->lightColor.g * (1.0f / 255.0f);
        color.b = self->lightColor.b * (1.0f / 255.0f);
        color.a = self->lightColor.a * (1.0f / 255.0f);
        FUN_00357a50(&self->skelAnime, 5, 3, &color, 1);
        FUN_00357388(&self->actor, &color, 4, 2);
        SkelAnime_DrawOpa(&self->skelAnime, (MTX34*)((u8*)self + 0x148), EnPoDesert_OverrideLimbDraw,
                          EnPoDesert_PostLimbDraw, &self->actor, 0);
        Rand_ZeroOne();
        FUN_00358778(self->skelModel, 1, 4, &color, 0);
        self->skelModel->SetUnkAC(1);
        self->skelModel->FUN_003721e0(&self->skelModelMtx);
        self->skelModel->FUN_00372170(0);
    }
}



void EnPoDesert_Disappear(EnPoDesert* self, PlayState* play) {
    s16 timer;
    s32 rounded;

    if (self->actionTimer != 0) {
        self->actionTimer--;
    }
    self->actor.shape.rot.y += 0x1555;
    timer = self->actionTimer;
    rounded = EnPoDesert_Frames(timer);
    self->lightColor.a = (u8)((f32)rounded * 15.9375f);
    self->actor.shape.shadowAlpha = self->lightColor.a;
    if (timer == 0) {
        Actor_Kill(&self->actor);
    }
}


void EnPoDesert_MoveToNextPoint(EnPoDesert* self, PlayState* play) {
    f32 temp_f20;
    s32 rounded;
    s16 timer;

    if (self->actionTimer != 0) {
        self->actionTimer--;
    }
    timer = self->actionTimer;
    rounded = EnPoDesert_Frames(timer);
    temp_f20 = __hardfp_sinf((f32)rounded * (M_PI / 20.0f)) * 5.0f;
    self->actor.world.pos.x += temp_f20 * Math_CosS(self->actor.shape.rot.y);
    self->actor.world.pos.z += temp_f20 * Math_SinS(self->actor.shape.rot.y);
    if (self->actionTimer == 0) {
        self->actionTimer = 60;
    }
    temp_f20 = Actor_WorldDistXZToPoint(&self->actor, &self->actor.home.pos);
    self->actor.world.rot.y = Actor_WorldYawTowardPoint(&self->actor, &self->actor.home.pos);
    FUN_00370084(&self->actor.shape.rot.y, self->actor.world.rot.y + 0x8000, 5, 0x400);
    timer = self->speedModifier;
    rounded = EnPoDesert_Frames(timer);
    *(f32*)((u8*)self + 0x6C) = __hardfp_sinf((f32)rounded * (M_PI / 32.0f)) * 2.5f + 5.5f;
    FUN_00373264(&self->actor, 0x010002E1);
    self->targetY = self->actor.home.pos.y - ((temp_f20 * self->yDiff) / self->initDistToNextPoint);
    if (temp_f20 < 40.0f) {
        if (self->currentPathPoint != 0) {
            FUN_00180fd4(self, play);
        } else {
            Animation_MorphToPlayOnce(&self->skelAnime, 0, -6.0f);
            *(f32*)((u8*)self + 0x6C) = 0.0f;
            self->actionTimer = 24;
            Audio_PlayActorSound2(&self->actor, 0x010002E4);
            self->actionFunc = EnPoDesert_Disappear;
        }
    }
}



// Unmatched: OverrideLimbDraw (44/62 words off: ROM copies the scale into a fresh VFP reg for the first six multiplies).
GLOBAL_ASM("asm/EnPoDesert_OverrideLimbDraw.s")

#if 0 // OverrideLimbDraw draft, 44 words off
static inline void EnPoDesert_ScaleMtx(MTX34* m, f32 x, f32 y, f32 z) {
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

s32 EnPoDesert_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnPoDesert* self = (EnPoDesert*)thisx;
    f32 half;
    f32 third;
    f32 two;

    if (self->actionFunc == EnPoDesert_Disappear && limbIndex == 13) {
        s16 timer = self->actionTimer;
        s32 rounded;
        f32 mtxScale;

        two = 2.0f;
        third = 1.0f / 3.0f;
        half = 0.5f;
        if (timer > 0) {
            rounded = (s32)(half + ((f32)timer * two) * third);
        } else {
            rounded = (s32)(((f32)timer * two) * third - half);
        }
        mtxScale = (f32)rounded * (1.0f / 16.0f);
        EnPoDesert_ScaleMtx(mtx, mtxScale, mtxScale, mtxScale);
    }
    return 0;
}
#endif

GLOBAL_ASM("../split/asm/FUN_00381cf4.s")

// PostLimbDraw: guarded function-local static pinned by pin_static.py (guard 0x0052DDA8, object 0x0059B52C, 12 B)
void EnPoDesert_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    static Vector3f sBaseLightPos(0.0f, 1400.0f, 0.0f);
    EnPoDesert* self = (EnPoDesert*)thisx;
    f32 rand;
    Color_RGBA8 color;
    Vec3f lightPos;

    if (limbIndex == 13) {
        self->skelModelMtx = *mtx;
        VEC3TransformAsm((Vector3f*)&lightPos, mtx, &sBaseLightPos);
        rand = Rand_ZeroOne();
        color.r = (s16)(rand * 30.0f) + 225;
        color.g = (s16)(rand * 100.0f) + 155;
        color.b = (s16)(rand * 160.0f) + 95;
        color.a = 200;
        Lights_PointNoGlowSetInfo(&self->lightInfo, lightPos.x, lightPos.y, lightPos.z, color.r, color.g, color.b, color.a,
                                  0);
    }
}

void EnPoDesert_WaitForPlayer(EnPoDesert* self, PlayState* play) {
    FUN_00373264(&self->actor, 0x010002E1);
    if (self->actor.xzDistToPlayer < 200.0f && (self->currentPathPoint != 2 || *(u8*)((u8*)play + 0x208F) != 0)) {
        if (self->currentPathPoint == 2) {
            if (Gameplay_InCsMode(play)) {
                self->actor.shape.rot.y += 0x800;
                return;
            }
            FUN_00367c7c(play, 0x600B, NULL);
        }
        Animation_MorphToLoop(&self->skelAnime, 2, -5.0f);
        self->actionFunc = EnPoDesert_MoveToNextPoint;
    } else {
        self->actor.shape.rot.y += 0x800;
    }
}

}
