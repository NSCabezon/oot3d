extern "C" {
#include "z_obj_bean.hpp"
#include "mtx34_trans.hpp"

#define FLAGS 0x00400000

void ObjBean_Init(Actor* actor, GameState* state);
void ObjBean_Destroy(Actor* actor, GameState* state);
void ObjBean_Update(Actor* actor, GameState* state);
void ObjBean_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Obj_Bean.data.s")

ActorInit Obj_Bean_InitVars = {
    ACTOR_OBJ_BEAN,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_MAMENOKI,
    sizeof(ObjBean),
    (ActorFunc)ObjBean_Init,
    (ActorFunc)ObjBean_Destroy,
    (ActorFunc)ObjBean_Update,
    (ActorFunc)ObjBean_Draw,
};

GLOBAL_ASM("asm/ObjBean_Init.s")

void Matrix_RotateY(MTX34* mtx, s32 mode, f32 angle);
void FUN_0014390c(ObjBean* self, PlayState* play); // ObjBean_WaitForWater (asm)
void FUN_0015e154(ObjBean* self, PlayState* play); // ObjBean_WaitForBean (asm, draft below)
void FUN_003cc7f4(ObjBean* self);                  // ObjBean_LeavesStill (asm)
s16 FUN_0036a0c8(s16 base, s16 range);
void FUN_00350f34(Actor* actor, ...); // variadic, see include/functions.hpp
void Camera_ChangeSetting(Camera* camera, s32 setting);
s32 FUN_0036adf4(DynaPolyActor* dyna); // DynaPolyActor_IsPlayerOnTop
s32 FUN_0032d8d8(DynaPolyActor* dyna); // DynaPolyActor_IsPlayerAbove
void FUN_0036ac64(ObjBean* self);      // ObjBean_UpdatePosition (still asm, see #if 0 draft below)
void FUN_00110118(ObjBean* self, PlayState* play);
void FUN_001100a0(ObjBean* self, PlayState* play); // ObjBean_WaitForPlayer
void FUN_001a23f4(ObjBean* self);                  // ObjBean_SetupWaitForPlayer (z_obj_bean_setup.cpp)
void FUN_0036ac40(ObjBean* self, PlayState* play); // ObjBean_WaitForStepOff
void FUN_0036d15c(PlayState* play, DynaCollisionContext* dyna, s32 bgId); // DynaPoly_EnableCollision
void Math_ScaledStepToS(s16* pValue, s16 target, s16 step);
void FUN_00239e24(ObjBean* self, PlayState* play); // ObjBean_Fly / func_80B90A34
void FUN_0036bbd0(Actor* actor, PlayState* play, s32 exchangeItemId, f32 range);

GLOBAL_ASM("../src/game/actors/Obj_Bean/data_syms.s")
extern ObjBean* D_80B90E30;
extern s32 sObjBeanBreakDlists[2];
extern ColliderCylinderInit sObjBeanCylinderInit;
extern InitChainEntry sObjBeanInitChain[];
f32 BgCheck_EntityRaycastFloor4(CollisionContext* colCtx, CollisionPoly** outPoly, s32* bgId, Actor* actor, Vec3f* pos);
void FUN_0036b940(PlayState* play, DynaCollisionContext* dyna, s32 bgId); // DynaPoly_DisableCollision

// Draft (84 words off, structurally close; see STATUS.md): ObjBean_Init and the N64 helpers it inlines.
#if 0
static inline void ObjBean_Move(ObjBean* self) {
    self->dyna.actor.world.pos.x = self->pathPoints.x + self->posOffsetX;
    self->dyna.actor.world.pos.y = self->pathPoints.y;
    self->dyna.actor.world.pos.z = self->pathPoints.z + self->posOffsetZ;
}

static inline void ObjBean_FindFloor(ObjBean* self, PlayState* play) {
    Vec3f checkPos;
    s32 bgId;

    checkPos.x = self->dyna.actor.world.pos.x;
    checkPos.y = self->dyna.actor.world.pos.y + 29.999998f;
    checkPos.z = self->dyna.actor.world.pos.z;
    self->dyna.actor.floorHeight =
        BgCheck_EntityRaycastFloor4(&play->colCtx, &self->dyna.actor.floorPoly, &bgId, &self->dyna.actor, &checkPos);
}

// (ObjBean_InitCollider helper unused: Init draft inlines the collider calls)
static inline void ObjBean_InitCollider(Actor* thisx, PlayState* play) {
    ObjBean* self = (ObjBean*)thisx;

    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->dyna.actor, &sObjBeanCylinderInit);
    Collider_UpdateCylinder(&self->dyna.actor, &self->collider);
}

static inline void ObjBean_InitDynaPoly(ObjBean* self, PlayState* play, void* collision, s32 moveFlag) {
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, moveFlag);
    ReadCollisionHeaderFromZsi(collision, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);
}

static inline void ObjBean_SetupWaitForPlayer(ObjBean* self) {
    self->actionFunc = FUN_001100a0;
    ObjBean_SetDrawMode(self, 0x4);
}

static inline void ObjBean_SetupLeavesStill(ObjBean* self) {
    self->transformFunc = FUN_003cc7f4;
    self->unk_1C0 = Rand_S16Offset_003702c8(12, 40);
    self->unk_1C4 = Rand_S16Offset_003702c8(200, 400);
    self->unk_1C6 = 0xD;
    self->unk_1CA = FUN_0036a0c8(100, 800);
    self->unk_1CC = 0xD;
}

static inline void ObjBean_SetupWaitForWater(ObjBean* self) {
    self->actionFunc = FUN_0014390c;
    ObjBean_SetDrawMode(self, 0x3);
    Actor_SetScale(&self->dyna.actor, 0.1f);
    ObjBean_SetupLeavesStill(self);
}

static inline void ObjBean_SetupWaitForBean(ObjBean* self) {
    self->actionFunc = FUN_0015e154;
    ObjBean_SetDrawMode(self, 0x1);
    self->dyna.actor.textId = 0x2F;
}

void ObjBean_Init(Actor* thisx, GameState* state) {
    ObjBean* self = (ObjBean*)thisx;

    thisx->unk_19A = 1;
    FUN_00372f38(&self->dyna.actor, ((PlayState*)state), &self->unk_218, 0, &self->unk_21C, 2, &self->unk_220, 3, &self->unk_224, 1, 0);
    Actor_ProcessInitChain(&self->dyna.actor, sObjBeanInitChain);
    if (LINK_AGE_IN_YEARS == YEARS_ADULT) {
        if (Flags_GetSwitch(((PlayState*)state), self->dyna.actor.params & 0x3F)) {
            if (((s16)self->dyna.actor.params >> 8 & 0x1F) == 0x1F) {
                Actor_Kill(&self->dyna.actor);
                return;
            }
            if (((PlayState*)state)->setupPathList[((s16)self->dyna.actor.params >> 8 & 0x1F)].count < 3) {
                Actor_Kill(&self->dyna.actor);
                return;
            }
            self->pathCount = ((PlayState*)state)->setupPathList[((s16)self->dyna.actor.params >> 8 & 0x1F)].count - 1;
            self->currentPointIndex = 0;
            self->nextPointIndex = 1;
            {
                Path* p = &((PlayState*)state)->setupPathList[((s16)self->dyna.actor.params >> 8 & 0x1F)];
                Math_Vec3s_ToVec3f(&self->pathPoints, p->points);
            }
            ObjBean_Move(self);
            ObjBean_SetupWaitForPlayer(self);

            ObjBean_InitDynaPoly(self, ((PlayState*)state), ((PlayState*)state)->GetObjectStatus(self->dyna.actor.objBankIndex)->zarInfo.FUN_003532c0(0), 3);
            self->stateFlags |= 0x20;
            Collider_InitCylinder((PlayState*)state, &self->collider);
            Collider_SetCylinder((PlayState*)state, &self->collider, &self->dyna.actor, &sObjBeanCylinderInit);
            Collider_UpdateCylinder(&self->dyna.actor, &((ObjBean*)thisx)->collider);
            self->stateFlags |= 0x10;

            ActorShape_Init(&self->dyna.actor.shape, 0.0f, ActorShadow_DrawCircle, 8.8f);
            ObjBean_FindFloor(self, ((PlayState*)state));
            self->unk_1F6 = self->dyna.actor.home.rot.z & 3;
        } else {
            Actor_Kill(&self->dyna.actor);
            return;
        }
    } else if (Flags_GetSwitch(((PlayState*)state), self->dyna.actor.params & 0x3F) != 0) {
        ObjBean_SetupWaitForWater(self);
    } else {
        ObjBean_SetupWaitForBean(self);
    }
    self->dyna.actor.world.rot.z = self->dyna.actor.home.rot.z = self->dyna.actor.shape.rot.z = 0;
}
#endif

void ObjBean_Destroy(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    ObjBean* self = (ObjBean*)thisx;

    if (self->stateFlags & 0x20) {
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    }
    if (self->stateFlags & 0x10) {
        Collider_DestroyCylinder(play, &self->collider);
    }
    if (D_80B90E30 == self) {
        D_80B90E30 = NULL;
    }
    FUN_00350f34(thisx, &self->unk_218, &self->unk_21C, &self->unk_220, &self->unk_224, 0);
}

void FUN_001100a0(ObjBean* self, PlayState* play) {
    if (FUN_0036adf4(&self->dyna)) {
        self->actionFunc = FUN_00110118;
        ObjBean_SetDrawMode(self, 0x4);
        self->dyna.actor.flags |= 0x10;
        self->dyna.actor.speedXZ = 0.0f;
        Camera_ChangeSetting(play->cameraPtrs[0], (play->sceneNum != 0x5B) ? 0x2C : 0x2D);
    }
    FUN_0036ac64(self);
}

void FUN_0036ac40(ObjBean* self, PlayState* play) {
    if (!FUN_0032d8d8(&self->dyna)) {
        FUN_001a23f4(self);
    }
    FUN_0036ac64(self);
}

void FUN_001eb198(ObjBean* self, PlayState* play) {
    Actor* currentActor = play->actorCtx.actorLists[ACTORCAT_BG].head;

    while (currentActor != NULL) {
        if (currentActor->id == ACTOR_EN_HORSE) {
            f32 dx = currentActor->world.pos.x - self->dyna.actor.world.pos.x;
            f32 dy = currentActor->world.pos.y - self->dyna.actor.world.pos.y;
            f32 dz = currentActor->world.pos.z - self->dyna.actor.world.pos.z;
            f32 distSq = dx * dx + dy * dy + dz * dz;
            if (distSq < 10000.0f) {
                self->timer = 150;
                return;
            }
        }
        currentActor = currentActor->next;
    }
    if (self->timer <= 0) {
        self->actionFunc = FUN_00239e24;
        ObjBean_SetDrawMode(self, 0x4);
        self->timer = 45;
    }
}

static inline void ObjBean_GrowthOsc(ObjBean* self) {
    self->unk_1B6.x += 0x384;
    if (self->unk_1B6.x > 0x5FFF) {
        self->unk_1B6.x = 0x5FFF;
    }
    self->unk_1B6.y += 0x258;
    if (self->unk_1B6.y > 0x4000) {
        self->unk_1B6.y = 0x4000;
    }
    self->dyna.actor.scale.y = Math_SinS(self->unk_1B6.x) * 0.16970563f;
    self->dyna.actor.scale.x = self->dyna.actor.scale.z = Math_SinS(self->unk_1B6.y) * 0.10700001f;
    Math_StepToF(&self->posOffsetX, 0.0f, 0.1f);
    Math_StepToF(&self->posOffsetZ, 0.0f, 0.1f);
    Math_ScaledStepToS(&self->dyna.actor.shape.rot.y, self->dyna.actor.home.rot.y, 0x64);
}

static inline s32 ObjBean_HorseNear(ObjBean* self, PlayState* play) {
    Actor* currentActor = play->actorCtx.actorLists[ACTORCAT_BG].head;

    while (currentActor != NULL) {
        if (currentActor->id == ACTOR_EN_HORSE) {
            f32 dx = currentActor->world.pos.x - self->dyna.actor.world.pos.x;
            f32 dy = currentActor->world.pos.y - self->dyna.actor.world.pos.y;
            f32 dz = currentActor->world.pos.z - self->dyna.actor.world.pos.z;
            if (dx * dx + dy * dy + dz * dz < 10000.0f) {
                return true;
            }
        }
        currentActor = currentActor->next;
    }
    return false;
}

void FUN_00239e24(ObjBean* self, PlayState* play) {
    s32 trampled = ObjBean_HorseNear(self, play);

    ObjBean_GrowthOsc(self);
    FUN_0036d15c(play, &play->colCtx.dyna, self->dyna.bgId);
    if (self->timer <= 0 && !trampled) {
        ObjBeanActionFunc setup = FUN_001100a0;
        self->unk_1B6.z = 0;
        self->unk_1B6.y = 0;
        self->unk_1B6.x = 0;
        self->unk_1E4 = 0.0f;
        self->actionFunc = setup;
        ObjBean_SetDrawMode(self, 0x4);
    }
}

// cfg tail split out of ObjLightswitch_Disappear (ws split). N64 func_80B8FE3C
void FUN_001436c4(ObjBean* self, PlayState* play); // N64 func_80B8FE6C setup target (asm, not yet split)

void FUN_00143684(ObjBean* self, PlayState* play) {
    if (self->timer <= 0) {
        self->actionFunc = FUN_001436c4;
        ObjBean_SetDrawMode(self, 0x3);
        Actor_SetScale(&self->dyna.actor, 0.01f);
    }
}

#if 0
// NEAR MISS (6 words): ROM materialises 0x228/0x5a after the draw-mode RMW (r0/r1 alternate); we hoist mov r0,#0x228.
void FUN_0015e154(ObjBean* self, PlayState* play) {
    if (Actor_ProcessTalkRequest(&self->dyna.actor, play)) {
        if (FUN_0036bc84(play) == 4) {
            s16* t = &self->timer;
            self->actionFunc = FUN_00143684;
            ObjBean_SetDrawMode(self, 0x1);
            *t = 0x5A;
            Flags_SetSwitch(play, self->dyna.actor.params & 0x3F);
        }
    } else {
        FUN_0036bbd0(&self->dyna.actor, play, 4, 40.0f);
    }
}
#endif

void FUN_00289334(ObjBean* self, PlayState* play) {
    if (FUN_0036adf4(&self->dyna)) {
        return;
    }
    self->pathCount = play->setupPathList[((s16)self->dyna.actor.params >> 8 & 0x1F)].count - 1;
    self->currentPointIndex = 0;
    self->nextPointIndex = 1;
    {
        Path* path = &play->setupPathList[((s16)self->dyna.actor.params >> 8 & 0x1F)];
        Math_Vec3s_ToVec3f(&self->pathPoints, path->points);
    }
    self->dyna.actor.world.pos.x = self->pathPoints.x + self->posOffsetX;
    self->dyna.actor.world.pos.y = self->pathPoints.y;
    self->dyna.actor.world.pos.z = self->pathPoints.z + self->posOffsetZ;
    self->actionFunc = FUN_001eb198;
    ObjBean_SetDrawMode(self, 0);
    self->timer = 150;
    self->unk_1B6.z = 0;
    self->unk_1B6.y = 0;
    self->unk_1B6.x = 0;
    Actor_SetScale(&self->dyna.actor, 0.0f);
}

void FUN_0036ac64(ObjBean* self) {
    f32 k;

    self->unk_1B6.x += 0xB6;
    self->unk_1B6.y += 0xFB;
    self->unk_1B6.z += 0x64;

    k = 0.1f;
    Math_StepToF(&self->unk_1E4, 2.0f, k);
    self->posOffsetX = (Math_SinS(self->unk_1B6.x * 3) + Math_SinS(self->unk_1B6.y * 3)) * self->unk_1E4;
    self->posOffsetZ = (Math_CosS(self->unk_1B6.x * 4) + Math_CosS(self->unk_1B6.y * 4)) * self->unk_1E4;
    self->dyna.actor.scale.x = self->dyna.actor.scale.z =
        ((Math_SinS(self->unk_1B6.z * 5) * 0.06f) + (Math_SinS(self->unk_1B6.y * 8) * 0.01f) + 1.07f) * k;
    self->dyna.actor.scale.y = (1.0f + (Math_CosS(self->unk_1B6.z * 10) * 0.2f)) * k;
    self->dyna.actor.shape.rot.y = (f32)self->dyna.actor.home.rot.y + (Math_SinS(self->unk_1B6.x * 3) * 1000.0f) +
                                   (Math_SinS((s16)(self->unk_1B6.z * 2)) * 2100.0f);
}

GLOBAL_ASM("asm/ObjBean_Update.s")

// Draft (2 words off: `and r1,r5,#1` lands in r0): ObjBean_Break (inlined) + ObjBean_Update.
#if 0
static inline void ObjBean_Break(ObjBean* self, PlayState* play) {
    Vec3f pos;
    Vec3f velocity;
    f32 temp_f20;
    s16 angle;
    s32 scale;
    s32 i;
    s16 gravity;
    s32 forty = 40;
    s16 arg5;

    angle = 0;
    for (i = 0; i < 36; i++) {
        angle += 0x4E20;
        temp_f20 = Rand_ZeroOne() * 60.0f;

        pos.x = (temp_f20 * Math_SinS(angle)) + self->dyna.actor.world.pos.x;
        pos.y = self->dyna.actor.world.pos.y;
        pos.z = (temp_f20 * Math_CosS(angle)) + self->dyna.actor.world.pos.z;

        velocity.x = Math_SinS(angle) * 3.5f;
        velocity.y = Rand_ZeroOne() * 13.0f;
        velocity.z = Math_CosS(angle) * 3.5f;

        velocity.x += self->dyna.actor.world.pos.x - self->dyna.actor.prevPos.x;
        velocity.y += self->dyna.actor.world.pos.y - self->dyna.actor.prevPos.y;
        velocity.z += self->dyna.actor.world.pos.z - self->dyna.actor.prevPos.z;

        scale = (s32)(Rand_ZeroOne() * 180.0f) + 30;
        if (scale < 90) {
            if (Rand_ZeroOne() < 0.1f) {
                arg5 = 0x60;
            } else {
                arg5 = 0x40;
            }
            gravity = -80;
        } else {
            arg5 = 0x40;
            gravity = -100;
        }
        void* dl = (void*)sObjBeanBreakDlists[i & 1];
        EffectSsKakera_Spawn(play, &pos, &velocity, &pos, gravity, arg5 | 0x200, 40, 3, 0, scale, 0, 0,
                             (s16)(forty + (scale >> 3)), -1, 1, dl);
    }
}

void ObjBean_Update(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    ObjBean* self = (ObjBean*)thisx;

    if (self->timer > 0) {
        self->timer--;
    }

    self->actionFunc(self, play);

    if (self->stateFlags & 0x4) {
        ObjBean_Move((ObjBean*)thisx);
        if (self->dyna.actor.xzDistToPlayer < 150.0f) {
            self->collider.dim.radius = (s16)(self->dyna.actor.scale.x * 640.0f + 0.5f);
            Collider_UpdateCylinder(&self->dyna.actor, &self->collider);
            CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
        }

        ObjBean_FindFloor(self, play);

        self->dyna.actor.shape.shadowDraw = ActorShadow_DrawCircle;
        self->dyna.actor.shape.shadowScale = self->dyna.actor.scale.x * 88.0f;

        if (ObjBean_HorseNear(self, play)) {
            ObjBean_Break(self, play);
            FUN_0036b940(play, &play->colCtx.dyna, self->dyna.bgId);
            self->actionFunc = FUN_00289334;
            ObjBean_SetDrawMode(self, 0);
        }
    } else {
        thisx->shape.shadowDraw = NULL;
    }
    Actor_SetFocus(thisx, 6.0f);
    if (self->stateFlags & 0x20) {
        if (FUN_0036adf4(&self->dyna)) {
            self->stateFlags |= 0x80;
        } else {
            self->stateFlags &= ~0x80;
        }
    }
}
#endif

GLOBAL_ASM("asm/ObjBean_Draw.s")

// Draft (123 words off: zero/1.0f hoisted into r6/r7 and no param-copy `mov r5,r4`): ObjBean_Draw.
#if 0
struct ObjBeanVec {
    f32& operator[](s32 i) {
        return (&x)[i];
    }
    f32 x, y, z;
};

static inline void ObjBean_TranslateNew(MTX34* m, f32 x, f32 y, f32 z) {
    ObjBeanVec p;

    p[0] = x;
    p[1] = y;
    p[2] = z;
    MTX34_MakeTrans(m, (Vec3f*)&p);
}

static inline void ObjBean_Scale(MTX34* m, f32 x, f32 y, f32 z) {
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

static void ObjBean_DrawSoftSoilSpot(ObjBean* self, PlayState* play) {
    MTX34 mtx;

    ObjBean_TranslateNew(&mtx, self->dyna.actor.home.pos.x, self->dyna.actor.home.pos.y, self->dyna.actor.home.pos.z);
    Matrix_RotateY(&mtx, 1, self->dyna.actor.home.rot.y * 0.000095873802f);
    ObjBean_Scale(&mtx, 0.1f, 0.1f, 0.1f);
    self->unk_220->FUN_003721e0(&mtx);
    self->unk_220->SetUnkAC(1);
    self->unk_220->FUN_00372170(0);
}

static void ObjBean_DrawBeanstalk(ObjBean* self, PlayState* play) {
    MTX34 mtx;

    ObjBean_TranslateNew(&mtx, self->dyna.actor.world.pos.x, self->dyna.actor.world.pos.y, self->dyna.actor.world.pos.z);
    Matrix_RotateY(&mtx, 1, self->dyna.actor.shape.rot.y * 0.000095873802f);
    ObjBean_Scale(&mtx, 0.1f, self->stalkSizeMultiplier, 0.1f);
    self->unk_224->FUN_003721e0(&mtx);
    self->unk_224->SetUnkAC(1);
    self->unk_224->FUN_00372170(0);
}

void ObjBean_Draw(Actor* thisx, GameState* state) {
    ObjBean* self = (ObjBean*)thisx;
    PlayState* play = (PlayState*)state;

    if (self->stateFlags & 0x2) {
        self->unk_218->FUN_003721e0(&self->dyna.actor.modelMtx);
        self->unk_218->SetUnkAC(1);
        self->unk_218->FUN_00372170(0);
    }
    if (self->stateFlags & 0x4) {
        self->unk_21C->FUN_003721e0(&self->dyna.actor.modelMtx);
        self->unk_21C->SetUnkAC(1);
        self->unk_21C->FUN_00372170(0);
    }
    if (self->stateFlags & 0x1) {
        ObjBean_DrawSoftSoilSpot(self, play);
    }
    if (self->stateFlags & 0x8) {
        ObjBean_DrawBeanstalk(self, play);
    }
}
#endif
}
