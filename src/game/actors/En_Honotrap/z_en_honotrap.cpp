extern "C" {
#include "z_en_honotrap.hpp"
#include <math.h>

#define FLAGS 0x80000010

void EnHonotrap_Init(Actor* actor, GameState* state);
void EnHonotrap_Destroy(Actor* thisx, GameState* state);
void EnHonotrap_Update(Actor* thisx, PlayState* play);
void EnHonotrap_Draw(Actor* actor, GameState* state);

void EnHonotrap_SetupEyeIdle(EnHonotrap* self);
void EnHonotrap_EyeIdle(EnHonotrap* self, PlayState* play);
void EnHonotrap_SetupEyeClose(EnHonotrap* self);
void EnHonotrap_EyeClose(EnHonotrap* self, PlayState* play);
void EnHonotrap_EyeAttack(EnHonotrap* self, PlayState* play);
void FUN_0035046c(void* obj);
void EffectSsBomb2_SpawnLayered(PlayState* play, Vector3f* pos, Vector3f* velocity, Vector3f* accel, s16 scale, s16 duration);
void FUN_0035fb14(Actor* actor);
void FUN_0036b96c(Actor* actor);
void FUN_00359450(void* player, MTX34* out);
void FUN_003624c8(MTX34* mtx, Vec3s* rot, s32 flag);
s32 Math_ScaledStepToS(s16* value, s16 target, s16 step);
f32 Actor_WorldDistXYZToPoint(Actor* actor, Vec3f* point);
void EnHonotrap_FlameMove(EnHonotrap* self, PlayState* play);
void EnHonotrap_FlameDrop(EnHonotrap* self, PlayState* play);
void EnHonotrap_FlameVanish(EnHonotrap* self, PlayState* play);
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);
void EnHonotrap_EyeOpen(EnHonotrap* self, PlayState* play);
s32 Collider_DestroyTris(PlayState* play, void* collider);

GLOBAL_ASM("data/z_En_Honotrap.data.s")
GLOBAL_ASM("data/z_En_Honotrap.data_00526550.s")
GLOBAL_ASM("../src/game/actors/En_Honotrap/data_syms.s")

ActorInit En_Honotrap_InitVars = {
    ACTOR_EN_HONOTRAP,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_DANGEON_KEEP,
    sizeof(EnHonotrap),
    (ActorFunc)EnHonotrap_Init,
    (ActorFunc)EnHonotrap_Destroy,
    (ActorFunc)EnHonotrap_Update,
    (ActorFunc)EnHonotrap_Draw,
};

// EnHonotrap_Init: 14 words off. Left: inner-loop regs (ROM j=r1 / elem ptr=r3 + `mov r3,r0` dest copy; ours j=r3 / ptr=r1),
// flame branch `mov r1,#0xff` for room store. Data via data_syms.s EQUs; `vtx++` at loop end gave 122->16 words.
#if 0
void Collider_InitTris(PlayState* play, ColliderTris* collider);
void Collider_SetTris(PlayState* play, ColliderTris* collider, Actor* actor, void* src, ColliderTrisElement* elements);
void Collider_SetTrisVertices(ColliderTris* collider, s32 index, Vec3f* a, Vec3f* b, Vec3f* c);
void FUN_0034f94c(void* obj, PlayState* play, Actor* actor, s32 arg3);
void EnHonotrap_Flame(EnHonotrap* self, PlayState* play);
extern InitChainEntry EnHonotrap_sInitChain[];
extern ColliderTrisInit EnHonotrap_sTrisInit;
extern ColliderCylinderInit EnHonotrap_sCylinderInit;
extern CollisionCheckInfoInit EnHonotrap_sColChkInfoInit;

static inline void EnHonotrap_Vec3fSum(Vec3f* l, Vec3f* r, Vec3f* dest) {
    dest->x = l->x + r->x;
    dest->y = l->y + r->y;
    dest->z = l->z + r->z;
}

static inline void EnHonotrap_InitEye(Actor* thisx, PlayState* play) {
    EnHonotrap* self = (EnHonotrap*)thisx;
    f32 cos;
    f32 sin;

    Actor_SetScale(thisx, 0.1f);
    sin = Math_SinS(thisx->home.rot.y);
    cos = Math_CosS(thisx->home.rot.y);
    Collider_InitTris(play, (ColliderTris*)&self->collider.tris);
    Collider_SetTris(play, (ColliderTris*)&self->collider.tris, thisx, &EnHonotrap_sTrisInit, self->collider.tris.elems);

    {
    s32 i;
    s32 j;
    Vec3f* vtx;
    Vec3f triangle[3];

    for (i = 0; i < 2; i++) {
        vtx = triangle;
        for (j = 0; j < 3; j++) {
            Vec3f* baseVtx = &EnHonotrap_sTrisInit.elements[i].dim.vtx[j];

            vtx->x = baseVtx->z * sin + baseVtx->x * cos;
            vtx->y = baseVtx->y;
            vtx->z = baseVtx->z * cos - baseVtx->x * sin;
            EnHonotrap_Vec3fSum(vtx, &thisx->world.pos, vtx);
            vtx++;
        }
        Collider_SetTrisVertices((ColliderTris*)&self->collider.tris, i, &triangle[0], &triangle[1], &triangle[2]);
    }
    }
    self->actionFunc = EnHonotrap_EyeIdle;
    self->eyeState = 3;
    Actor_SetFocus(thisx, 0.0f);
}

static inline void EnHonotrap_InitFlame(Actor* thisx, PlayState* play) {
    EnHonotrap* self = (EnHonotrap*)thisx;

    Actor_SetScale(&self->actor, 0.0001f);
    Collider_InitCylinder(play, &self->collider.cyl);
    Collider_SetCylinder(play, &self->collider.cyl, &self->actor, &EnHonotrap_sCylinderInit);
    Collider_UpdateCylinder(&self->actor, &self->collider.cyl);
    self->actor.minVelocityY = -1.0f;
    CollisionCheck_SetInfo(&self->actor.colChkInfo, NULL, &EnHonotrap_sColChkInfoInit);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    self->actor.shape.shadowAlpha = 128;
    self->targetPos = GET_PLAYER(play)->actor.world.pos;
    self->targetPos.y += 10.0f;
    self->flameScroll = Rand_ZeroOne() * 511.0f;
    self->actionFunc = EnHonotrap_Flame;
    Audio_PlayActorSound2(&self->actor, 0x01000179);
    if (self->actor.params == 2) {
        thisx->room = -1;
        self->collider.cyl.dim.radius = 12.0f;
        self->collider.cyl.dim.height = 30.0f;
        self->actor.shape.yOffset = -1000.0f;
    }
}

void EnHonotrap_Init(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnHonotrap* self = (EnHonotrap*)thisx;

    Actor_ProcessInitChain(thisx, EnHonotrap_sInitChain);
    {
        ZARInfo* zar = FUN_00372f38(thisx, play, &self->eyeModel, 6, 0);

        self->eyeAnim = self->eyeModel->unk_0C;
        self->eyeAnim->FUN_00372d94(zar->GetCMABByIndex(2));
    }
    self->eyeAnim->SetAnimSpeed(0.0f);
    FUN_0034f94c(&self->flameModel, play, thisx, 0);
    if (thisx->params == 0) {
        EnHonotrap_InitEye(thisx, play);
    } else {
        EnHonotrap_InitFlame(thisx, play);
    }
}
#endif
GLOBAL_ASM("asm/EnHonotrap_Init.s")

void EnHonotrap_Destroy(Actor* thisx, GameState* state) {
    EnHonotrap* self = (EnHonotrap*)thisx;
    PlayState* play = (PlayState*)state;

    if (self->actor.params == 0) {
        Collider_DestroyTris(play, &self->collider);
    } else {
        Collider_DestroyCylinder(play, &self->collider.cyl);
    }
    FUN_00350f34(&self->actor, &self->eyeModel, 0);
    FUN_0035046c(&self->flameModel);
}


// EnHonotrap_Draw: ~23 words off. Left: zero const reg numbering (ROM zero=s18, one=s17, angle=s19), pause tail
// (ROM vstrne s18/vstreq s17), entry `mov r6,#1` before `add r7`, eye color store schedule.
#if 0
typedef struct EnHonotrapDrawColor {
    f32 r, g, b, a;
} EnHonotrapDrawColor;

void FUN_00358778(SkeletonAnimationModel* skelAnimModel, s32 arg1, s32 arg2, EnHonotrapDrawColor* color, s32 arg4);
void FUN_003586ec(SkeletonAnimationModel_unk_0C* anim);
void FUN_00373bec(void* obj);
s16 Camera_GetCamDirYaw(Camera* camera);
extern s32 gUnk_0054C8CC;
extern f32 EnHonotrap_sEyeScale[];

static inline void EnHonotrap_DrawEye(Actor* thisx, PlayState* play) {
    EnHonotrap* self = (EnHonotrap*)thisx;
    MTX34 mtx;
    EnHonotrapDrawColor color;
    SkeletonAnimationModel_unk_0C* anim;
    f32 frame;

    MTX34CopyAsm(&mtx, &self->actor.modelMtx);
    anim = self->eyeAnim;
    frame = EnHonotrap_sEyeScale[self->eyeState];
    if (gUnk_0054C8CC == 0) {
        *(f32*)&anim->curFrame = frame;
        FUN_003586ec(anim);
    }
    FUN_00373bec(self->eyeAnim);
    color.r = 1.0f;
    if (self->eyeState != 3) {
        color.g = 0.5f;
        color.b = 0.5f;
    } else {
        color.g = 1.0f;
        color.b = 1.0f;
    }
    color.a = 1.0f;
    FUN_00358778(self->eyeModel, 0, 4, &color, 0);
    if (self->eyeModel != NULL) {
        self->eyeModel->SetUnkAC(1);
        self->eyeModel->FUN_003721e0(&mtx);
        self->eyeModel->FUN_00372170(0);
    }
}

static inline void EnHonotrap_DrawFlame(Actor* thisx, GameState* state) {
    EnHonotrap* self = (EnHonotrap*)thisx;
    PlayState* play = (PlayState*)state;
    MTX34 mtx;
    f32 zero = 0.0f;
    f32 angle;

    self->flameScroll -= 20;
    self->flameScroll %= 512U;
    MTX34CopyAsm(&mtx, &self->actor.modelMtx);
    angle = (s16)(Camera_GetCamDirYaw(play->cameraPtrs[play->activeCamera]) - self->actor.shape.rot.y + 0x8000) *
            (3.14159274f / 32768.0f);
    if (angle != zero) {
        f32 s = sinf(angle);
        f32 c = cosf(angle);
        f32 a, b;

        a = mtx.data[0][0];
        b = mtx.data[0][2];
        mtx.data[0][0] = a * c - b * s;
        mtx.data[0][2] = a * s + b * c;
        a = mtx.data[1][0];
        b = mtx.data[1][2];
        mtx.data[1][0] = a * c - b * s;
        mtx.data[1][2] = a * s + b * c;
        a = mtx.data[2][0];
        b = mtx.data[2][2];
        mtx.data[2][0] = a * c - b * s;
        mtx.data[2][2] = a * s + b * c;
    }
    if (PauseContext_GetState() == 0) {
        self->flameAnim->animSpeed = 1.0f;
    } else {
        self->flameAnim->animSpeed = zero;
    }
    FUN_00373bec(self->flameAnim);
    self->flameModel->SetUnkAC(1);
    self->flameModel->FUN_003721e0(&mtx);
    self->flameModel->FUN_00372170(0);
}

void EnHonotrap_Draw(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;

    switch (thisx->params) {
        case 0:
            EnHonotrap_DrawEye(thisx, play);
            break;
        case 1:
        case 2:
            EnHonotrap_DrawFlame(thisx, state);
            break;
    }
}
#endif
GLOBAL_ASM("asm/EnHonotrap_Draw.s")

void EnHonotrap_EyeAttack(EnHonotrap* self, PlayState* play) {
    if (self->timer <= 0) {
        self->actionFunc = EnHonotrap_EyeClose;
    }
}

void EnHonotrap_EyeClose(EnHonotrap* self, PlayState* play) {
    self->eyeState++;
    if (self->eyeState >= 3) {
        EnHonotrap_SetupEyeIdle(self);
        self->timer = 300;
    }
}

void EnHonotrap_EyeIdle(EnHonotrap* self, PlayState* play) {
    if (self->actor.child != NULL) {
        self->timer = 300;
    } else if ((self->timer <= 0) && (self->actor.xzDistToPlayer < 750.0f)) {
        if ((self->actor.yDistToPlayer < 0.0f) && (self->actor.yDistToPlayer > -700.0f)) {
            s32 yaw = self->actor.yawTowardsPlayer;
            s32 rotY = self->actor.shape.rot.y;
            s32 angle = yaw - rotY;

            if (angle > -0x4000) {
                if (angle < 0x4000) {
                    self->actionFunc = EnHonotrap_EyeOpen;
                    Actor_SetColorFilter(&self->actor, 0x400000, 255, 0, 40);
                    self->timer = 45;
                    Audio_PlayActorSound2(&self->actor, 0x010001D8);
                }
            }
        }
    }
}

void EnHonotrap_EyeOpen(EnHonotrap* self, PlayState* play) {
    self->eyeState--;
    if (self->eyeState <= 0) {
        self->actionFunc = EnHonotrap_EyeAttack;
        self->eyeState = 0;
        Actor_SpawnAsChild(
            &play->actorCtx, &self->actor, play, 0x11C,
            (Math_SinS(self->actor.shape.rot.y) * 12.0f) + self->actor.home.pos.x, self->actor.home.pos.y - 10.0f,
            (Math_CosS(self->actor.shape.rot.y) * 12.0f) + self->actor.home.pos.z, self->actor.home.rot.x,
            self->actor.home.rot.y, self->actor.home.rot.z, 1);
    }
}

void EnHonotrap_Flame(EnHonotrap* self, PlayState* play) {
    f32 targetScale = (self->actor.params == 1) ? 0.004f : 0.0048f;
    s32 targetReached = Math_StepToF(&self->actor.scale.x, targetScale, 0.0006f);

    self->actor.scale.z = self->actor.scale.y = self->actor.scale.x;
    if (targetReached) {
        if (self->actor.params == 1) {
            f32 distInverse;

            self->actionFunc = EnHonotrap_FlameMove;
            distInverse = 1.0f / (Actor_WorldDistXYZToPoint(&self->actor, &self->targetPos) + 1.0f);
            self->actor.velocity.x = (self->targetPos.x - self->actor.world.pos.x) * distInverse;
            self->actor.velocity.y = (self->targetPos.y - self->actor.world.pos.y) * distInverse;
            self->actor.velocity.z = (self->targetPos.z - self->actor.world.pos.z) * distInverse;
            self->speedMod = 0.0f;
            self->timer = 240;
        } else {
            self->timer = 60;
            self->actor.velocity.y = 1.0f;
            self->actor.velocity.x = 2.0f * Math_SinS(self->actor.world.rot.y);
            self->actor.velocity.z = 2.0f * Math_CosS(self->actor.world.rot.y);
            self->actionFunc = EnHonotrap_FlameDrop;
        }
    }
}

void EnHonotrap_FlameChase(EnHonotrap* self, PlayState* play) {
    Actor* actor = &self->actor;
    EnHonotrap* s2 = self;

    Math_ScaledStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 0x300);
    Math_StepToF(&s2->actor.speedXZ, 3.0f, 0.1f);
    if (-self->actor.yDistToPlayer < 10.0f) {
        self->actor.gravity = 0.08f;
    } else {
        self->actor.gravity = -0.08f;
    }
    FUN_0035fb14(&self->actor);
    if (self->actor.velocity.y > 1.0f) {
        self->actor.velocity.y = 1.0f;
    }
    self->actor.velocity.y *= 0.95f;
    FUN_0036b96c(&self->actor);
    Actor_UpdateBgCheckInfo(play, actor, 7.0f, 10.0f, 0.0f, 0x1D);
    if (self->collider.cyl.base.atFlags & AT_BOUNCED) {
        Vec3s shieldRot;
        MTX34 mtx;

        FUN_00359450(GET_PLAYER(play), &mtx);
        FUN_003624c8(&mtx, &shieldRot, 0);
        self->actor.world.rot.y = ((shieldRot.y * 2) - self->actor.world.rot.y) + 0x8000;
        self->actionFunc = EnHonotrap_FlameVanish;
    } else if (self->collider.cyl.base.atFlags & AT_HIT) {
        actor->speedXZ *= 0.1f;
        actor->velocity.y *= 0.1f;
        self->actionFunc = EnHonotrap_FlameVanish;
    } else if ((self->actor.bgCheckFlags & 0x0008) || (self->timer <= 0)) {
        self->actionFunc = EnHonotrap_FlameVanish;
    } else {
        Collider* colBase = &self->collider.cyl.base;
        Actor* a2 = actor;

        Collider_UpdateCylinder(a2, &self->collider.cyl);
        CollisionCheck_SetAT(play, &play->colChkCtx, colBase);
        CollisionCheck_SetAC(play, &play->colChkCtx, colBase);
        CollisionCheck_SetOC(play, &play->colChkCtx, colBase);
        self->colChkFlags |= 1;
        self->colChkFlags |= 2;
        self->colChkFlags |= 4;
    }
}

void EnHonotrap_FlameDrop(EnHonotrap* self, PlayState* play) {
    if ((self->collider.cyl.base.atFlags & AT_HIT) || (self->timer <= 0)) {
        if ((self->collider.cyl.base.atFlags & AT_HIT) && !(self->collider.cyl.base.atFlags & AT_BOUNCED)) {
            FUN_00374bb8(play, &self->actor, 5.0f, self->actor.yawTowardsPlayer, 0.0f);
        }
        self->actor.velocity.x = self->actor.velocity.y = self->actor.velocity.z = 0.0f;
        self->actionFunc = EnHonotrap_FlameVanish;
        return;
    }
    if (self->actor.velocity.y > 0.0f) {
        self->actor.world.pos.x += self->actor.velocity.x;
        self->actor.world.pos.z += self->actor.velocity.z;
        Actor_UpdateBgCheckInfo(play, &self->actor, 7.0f, 12.0f, 0.0f, 0x05);
    }
    if (!Math_StepToF(&self->actor.world.pos.y, self->actor.floorHeight + 1.0f, self->actor.velocity.y)) {
        self->actor.velocity.y += 1.0f;
    } else {
        self->actor.velocity.y = 0.0f;
    }
    Collider* colBase = &self->collider.cyl.base;
    Actor* actor = &self->actor;

    Collider_UpdateCylinder(actor, &self->collider.cyl);
    CollisionCheck_SetAT(play, &play->colChkCtx, colBase);
    CollisionCheck_SetAC(play, &play->colChkCtx, colBase);
    CollisionCheck_SetOC(play, &play->colChkCtx, colBase);
    self->colChkFlags |= 1;
    self->colChkFlags |= 2;
    self->colChkFlags |= 4;
}

// FlameMove: 4 words off (VFP numbering: ROM speed.y->s16, speed.z->s18; ours swapped)
#if 0
void Math3D_Vec3fNormalizedReflect(Vec3f* vec, Vec3f* normal, Vec3f* result);

static inline void EnHonotrap_GetNormal(Vec3f* normal, Vec3f* vec) {
    f32 magnitude = __sqrtf(SQ(vec->x) + SQ(vec->y) + SQ(vec->z));

    if (magnitude < 0.001f) {
        normal->x = normal->y = 0.0f;
        normal->z = 1.0f;
    } else {
        f32 inv = 1.0f / magnitude;

        normal->x = vec->x * inv;
        normal->y = vec->y * inv;
        normal->z = vec->z * inv;
    }
}

static inline void EnHonotrap_SetupFlameVanish(EnHonotrap* self) {
    self->actionFunc = EnHonotrap_FlameVanish;
}

static inline void EnHonotrap_FlameCollisionCheck(EnHonotrap* self, PlayState* play) {
    Collider* colBase = &self->collider.cyl.base;
    Actor* actor = &self->actor;

    Collider_UpdateCylinder(actor, &self->collider.cyl);
    CollisionCheck_SetAT(play, &play->colChkCtx, colBase);
    CollisionCheck_SetAC(play, &play->colChkCtx, colBase);
    CollisionCheck_SetOC(play, &play->colChkCtx, colBase);
    self->colChkFlags |= 1;
    self->colChkFlags |= 2;
    self->colChkFlags |= 4;
}

static inline void EnHonotrap_SetupFlameChase(EnHonotrap* self) {
    self->actionFunc = EnHonotrap_FlameChase;
    self->actor.speedXZ = 0.0f;
    self->actor.velocity.x = self->actor.velocity.y = self->actor.velocity.z = 0.0f;
    self->actor.world.rot.x = self->actor.world.rot.y = self->actor.world.rot.z = 0;
    self->timer = 150;
}

void EnHonotrap_FlameMove(EnHonotrap* self, PlayState* play) {
    Actor* thisx = &self->actor;
    Vec3f speed;
    s32 targetReached;
    EnHonotrap* s2 = self;
    PlayState* play2 = play;

    Math_StepToF(&self->speedMod, 13.0f, 0.5f);
    speed.x = fabsf(self->speedMod * thisx->velocity.x);
    speed.y = fabsf(self->speedMod * thisx->velocity.y);
    speed.z = fabsf(self->speedMod * thisx->velocity.z);
    targetReached = 1;
    targetReached &= Math_StepToF(&thisx->world.pos.x, s2->targetPos.x, speed.x);
    targetReached &= Math_StepToF(&thisx->world.pos.y, self->targetPos.y, speed.y);
    targetReached &= Math_StepToF(&thisx->world.pos.z, self->targetPos.z, speed.z);
    Actor_UpdateBgCheckInfo(play2, &self->actor, 7.0f, 10.0f, 0.0f, 0x1D);

    if (self->collider.cyl.base.atFlags & AT_BOUNCED) {
        Vec3f shieldNorm;
        Vec3f tempVel;
        Vec3f shieldVec;
        MTX34 mtx;

        FUN_00359450(GET_PLAYER(play), &mtx);
        shieldVec.x = -mtx.data[0][2];
        shieldVec.y = -mtx.data[1][2];
        shieldVec.z = -mtx.data[2][2];
        EnHonotrap_GetNormal(&shieldNorm, &shieldVec);

        tempVel = thisx->velocity;
        Math3D_Vec3fNormalizedReflect(&tempVel, &shieldNorm, &thisx->velocity);
        thisx->speedXZ = self->speedMod * 0.5f;
        thisx->world.rot.y = Math_Atan2S(thisx->velocity.z, thisx->velocity.x);
        EnHonotrap_SetupFlameVanish(self);
    } else if (self->collider.cyl.base.atFlags & AT_HIT) {
        thisx->speedXZ = 0.0f;
        thisx->velocity.y = 0.0f;
        EnHonotrap_SetupFlameVanish(self);
    } else if (self->timer <= 0) {
        EnHonotrap_SetupFlameVanish(self);
    } else {
        EnHonotrap_FlameCollisionCheck(self, play);
        if (targetReached) {
            EnHonotrap_SetupFlameChase(self);
        }
    }
}
#endif
GLOBAL_ASM("asm/EnHonotrap_FlameMove.s")

void EnHonotrap_FlameVanish(EnHonotrap* self, PlayState* play) {
    Actor* thisx = &self->actor;
    s32 targetReached = Math_StepToF(&thisx->scale.x, 0.0001f, 0.00015f);

    self->actor.scale.z = self->actor.scale.y = self->actor.scale.x;
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(play, &self->actor, 7.0f, 10.0f, 0.0f, 0x1D);
    if (targetReached) {
        Actor_Kill(&self->actor);
    }
}


}
