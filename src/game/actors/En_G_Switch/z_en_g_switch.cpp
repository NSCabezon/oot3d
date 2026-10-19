extern "C" {
#include "z_en_g_switch.hpp"

#define FLAGS 0x80000030

void EnGSwitch_Init(Actor* thisx, PlayState* play);
void EnGSwitch_Destroy(Actor* thisx, PlayState* play);
void EnGSwitch_Update(Actor* thisx, PlayState* play);
void FUN_00238cb4(EnGSwitch* self, PlayState* play);
void FUN_00198288(EnGSwitch* self, PlayState* play);
void FUN_00352ee0(Actor* actor, PlayState* play, s32 count, SkeletonAnimationModel** models, s32* modelIds);
void FUN_003df554(EnGSwitch* self, PlayState* play);
void FUN_002692ac(Actor* thisx, PlayState* play);
void FUN_002593d0(EnGSwitch* self, PlayState* play);
void FUN_001e9c1c(EnGSwitch* self, PlayState* play);
s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);

GLOBAL_ASM("data/z_En_G_Switch.data.s")

ActorInit En_G_Switch_InitVars = {
    ACTOR_EN_G_SWITCH,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnGSwitch),
    (ActorFunc)EnGSwitch_Init,
    (ActorFunc)EnGSwitch_Destroy,
    (ActorFunc)EnGSwitch_Update,
    NULL,
};

// Actor data at fixed ROM addresses (EQU symbols in data_syms.s).
GLOBAL_ASM("../src/game/actors/En_G_Switch/data_syms.s")
extern s16 EnGSwitch_sCollectedCount;
typedef struct { s16 collectedCount; s16 pad; s32 shardCount; } EnGSwitchStatics;
extern EnGSwitchStatics EnGSwitch_sStatics;
extern ColliderCylinderInit EnGSwitch_sCylinderInit;
extern s32 EnGSwitch_sMajorScale[];

void EnGSwitch_Init(Actor* thisx, PlayState* play) {
    EnGSwitch* self = (EnGSwitch*)thisx;
    EnGSwitch* s2 = self;
    s32 i;

    s2->type = (self->actor.params >> 12) & 0xF;
    self->switchFlag = self->actor.params & 0x3F;
    self->numEffects = 40;
    for (i = 0; i < 42; i++) {
        self->modelIds[i] = -1;
        self->models[i] = NULL;
    }
    switch (self->type) {
        case 0:
            EnGSwitch_sCollectedCount = 0;
            self->silverCount = ((u16)self->actor.params >> 6) & 0x3F;
            FUN_00352ee0(&self->actor, play, 42, self->models, self->modelIds);
            if (Flags_GetSwitch(play, self->switchFlag)) {
                Actor_Kill(&self->actor);
            } else {
                self->actionFunc = FUN_003df554;
            }
            break;
        case 1:
            self->colorIdx = 5;
            self->numEffects = 20;
            Collider_InitCylinder(play, &self->collider);
            Collider_SetCylinder(play, &self->collider, &self->actor, &EnGSwitch_sCylinderInit);
            self->actor.shape.yOffset = 700.0f;
            self->actor.draw = (ActorFunc)FUN_002692ac;
            self->modelIds[1] = 0x41;
            FUN_00352ee0(&self->actor, play, 42, self->models, self->modelIds);
            if (Flags_GetSwitch(play, self->switchFlag)) {
                Actor_Kill(&self->actor);
            } else {
                Actor_SetScale(&self->actor, 0.03f);
                self->actionFunc = FUN_00198288;
            }
            break;
        case 2:
            self->modelIds[0] = 1;
            self->actor.gravity = -3.0f;
            self->colorIdx = Rand_ZeroFloat(2.99f);
            Collider_InitCylinder(play, &self->collider);
            Collider_SetCylinder(play, &self->collider, thisx, &EnGSwitch_sCylinderInit);
            self->actor.scale.x = 0.25f;
            self->actor.scale.y = 0.45f;
            self->actor.scale.z = 0.25f;
            self->collider.info.bumper.dmgFlags = 0x1F820;
            self->objectId = 300;
            self->requiredObjectSlot = Object_GetIndex(&play->objectCtx, self->objectId);
            if (self->requiredObjectSlot < 0) {
                Actor_Kill(&self->actor);
            }
            self->collider.dim.radius = 24.0f;
            self->collider.dim.height = 74.0f;
            self->collider.dim.yShift = 0.0f;
            self->actionFunc = FUN_002593d0;
            break;
        case 3:
            self->modelIds[1] = 0x41;
            for (i = 0; i < self->numEffects; i++) {
                self->modelIds[i + 2] = 0x41;
            }
            FUN_00352ee0(&self->actor, play, 42, self->models, self->modelIds);
            thisx->shape.yOffset = 700.0f;
            Actor_SetScale(&self->actor, 0.05f);
            Collider_InitCylinder(play, &self->collider);
            Collider_SetCylinder(play, &self->collider, &self->actor, &EnGSwitch_sCylinderInit);
            self->actor.draw = (ActorFunc)FUN_002692ac;
            self->collider.dim.radius = 20.0f;
            self->collider.dim.height = 60.0f;
            self->collider.dim.yShift = 5.0f;
            self->actionFunc = FUN_001e9c1c;
            break;
    }
}

void FUN_0034fda8(Actor* actor, s32 count, SkeletonAnimationModel** models);
void FUN_00352ee0(Actor* actor, PlayState* play, s32 count, SkeletonAnimationModel** models, s32* modelIds);
s32 Object_IsLoaded(ObjectContext* objectCtx, s32 slot);
void FUN_002073c0(EnGSwitch* self, PlayState* play);
void FUN_003df554(EnGSwitch* self, PlayState* play);
void FUN_002692ac(Actor* thisx, PlayState* play);
void FUN_002593d0(EnGSwitch* self, PlayState* play);
s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
void FUN_001e9c1c(EnGSwitch* self, PlayState* play);
void FUN_001e5048(EnGSwitch* self, PlayState* play);
void FUN_00369178(SkeletonAnimationModel* model, s32 idx);
void FUN_0036932c(SkeletonAnimationModel* model, s32 idx);
void FUN_0037266c(SkeletonAnimationModel* model, s32 idx);
void FUN_0035e5b4(Vec3f* pos, u32 sfxId, s8 pitch);
s32 FUN_00360084(ActorContext* actorCtx, s32 actorId, s32 arg2, Actor** out, s32 max);
void Rupees_ChangeBy(s16 rupeeChange);
void FUN_0033ba54(EnGSwitch* self, PlayState* play);

void EnGSwitch_Destroy(Actor* thisx, PlayState* play) {
    EnGSwitch* self = (EnGSwitch*)thisx;

    Collider_DestroyCylinder(play, &self->collider);
    FUN_0034fda8(thisx, 42, self->models);
}

void EnGSwitch_Update(Actor* thisx, PlayState* play) {
    EnGSwitch* self = (EnGSwitch*)thisx;
    EnGSwitch* s2 = self;

    self->actionFunc(self, play);
    if (self->killTimer != 0) {
        self->killTimer--;
    }
    if (self->waitTimer != 0) {
        self->waitTimer--;
    }
    if (self->delayTimer != 0) {
        self->delayTimer--;
    }
    if ((self->type != 0) && (self->type != 1) && (self->type != 3)) {
        Actor_MoveForward(&self->actor);
        Actor_UpdateBgCheckInfo(play, &self->actor, 50.0f, 50.0f, 100.0f, 0x1C);
    }
    if (self->actor.draw != NULL) {
        if (self->type == 3) {
            Vec3f temp;
            s16 i;
            EnGSwitchEffect* effect = self->effects;

            for (i = 0; i < self->numEffects; i++, effect++) {
                if (effect->flag) {
                    effect->rot.x += Rand_ZeroOne() * 10.0f + 15.0f;
                    effect->rot.y += Rand_ZeroOne() * 10.0f + 15.0f;
                    effect->rot.z += Rand_ZeroOne() * 10.0f + 15.0f;
                    temp.x = effect->pos.x + effect->velocity.x;
                    temp.y = effect->pos.y + effect->velocity.y;
                    temp.z = effect->pos.z + effect->velocity.z;
                    Math_ApproachF(&effect->pos.x, temp.x, 0.3f, 30.0f);
                    Math_ApproachF(&effect->pos.y, temp.y, 0.8f, 250.0f);
                    Math_ApproachF(&effect->pos.z, temp.z, 0.3f, 30.0f);
                    Math_ApproachF(&effect->velocity.y, -20.0f, 0.9f, 1.0f);
                    if (effect->timer != 0) {
                        effect->timer--;
                    } else if (effect->scale < 10) {
                        effect->flag = 0;
                    } else {
                        effect->scale -= 2;
                    }
                }
            }
        }
        if ((self->actionFunc != FUN_00238cb4) && (self->actionFunc != FUN_00198288)) {
            Collider_UpdateCylinder(thisx, &self->collider);
            CollisionCheck_SetAC(play, &play->colChkCtx, &s2->collider.base);
        }
    }
}

// Draft of FUN_0033ba54 = EnGSwitch_Break with SpawnEffects inlined (NOT compiled): 86/198 words differ, same size
// (788 vs 792 incl. pool). Matrix must be a Break-scope local (shares the outgoing-arg slot at sp+0) but then the
// helper reads it through a pointer (r5=sp base); randPos only stays in memory if passed by pointer to a helper.
#if 0
void FUN_0034ddc8(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel, s16 scale, s16 type);
void EffectSsHitMark_SpawnCustomScale(PlayState* play, s32 type, s32 arg2, s32 scale, Vec3f* pos);
f32 __hardfp_sinf(f32);
f32 __hardfp_cosf(f32);

static inline void EnGSwitch_SpawnEffects(EnGSwitch* self, Vec3f* pos, s16 scale, s16 colorIdx, MTX34* pm) {
    MTX34& mtx = *pm;
    Vec3f baseVel;
    s16 j;
    EnGSwitchEffect* effect = self->effects;

    MTX34CopyAsm(&mtx, &self->actor.modelMtx);
    for (j = 0; j < self->numEffects; j++, effect++) {
        if (!effect->flag) {
            f32 pitch;
            f32 yaw;
            f32 sn;
            f32 cs;
            f32 a;
            f32 b;

            effect->pos = *pos;
            effect->scale = scale;
            effect->colorIdx = colorIdx;
            effect->timer = 45;
            effect->rot.x = effect->rot.y = effect->rot.z = 0.0f;
            pitch = Rand_CenteredFloat(1000.0f) - 13000.0f;
            yaw = Rand_CenteredFloat(65535.0f);
            if (yaw != 0.0f) {
                sn = __hardfp_sinf(yaw);
                cs = __hardfp_cosf(yaw);
            } else {
                sn = 0.0f;
                cs = 1.0f;
            }
            mtx.data[0][0] = cs;
            mtx.data[0][1] = 0.0f;
            mtx.data[0][2] = sn;
            mtx.data[0][3] = 0.0f;
            mtx.data[1][0] = 0.0f;
            mtx.data[1][1] = 1.0f;
            mtx.data[1][2] = 0.0f;
            mtx.data[1][3] = 0.0f;
            mtx.data[2][0] = -sn;
            mtx.data[2][1] = 0.0f;
            mtx.data[2][2] = cs;
            mtx.data[2][3] = 0.0f;
            if (pitch != 0.0f) {
                sn = __hardfp_sinf(pitch);
                cs = __hardfp_cosf(pitch);
                a = mtx.data[0][1];
                b = mtx.data[0][2];
                mtx.data[0][1] = a * cs + b * sn;
                mtx.data[0][2] = b * cs - a * sn;
                a = mtx.data[1][1];
                b = mtx.data[1][2];
                mtx.data[1][1] = a * cs + b * sn;
                mtx.data[1][2] = b * cs - a * sn;
                a = mtx.data[2][1];
                b = mtx.data[2][2];
                mtx.data[2][1] = a * cs + b * sn;
                mtx.data[2][2] = b * cs - a * sn;
            }
            baseVel.x = baseVel.y = 0.0f;
            baseVel.z = 20.0f;
            VEC3TransformAsm((Vector3f*)&effect->velocity, &mtx, (Vector3f*)&baseVel);
            effect->flag = 1;
            return;
        }
    }
}

// EnGSwitch_Break
void FUN_0033ba54(EnGSwitch* self, PlayState* play) {
    Vec3f randPos;
    Vec3f hitPos;
    Vector3f accel(0.0f, 0.0f, 0.0f);
    Vector3f velocity(0.0f, 0.0f, 0.0f);
    s32 i;

    randPos.x = self->actor.world.pos.x + Rand_CenteredFloat(40.0f);
    randPos.y = self->actor.world.pos.y + 30.0f + Rand_CenteredFloat(35.0f);
    randPos.z = self->actor.world.pos.z + Rand_CenteredFloat(40.0f);
    hitPos.x = self->collider.info.bumper.hitPos.x;
    hitPos.y = self->collider.info.bumper.hitPos.y;
    hitPos.z = self->collider.info.bumper.hitPos.z;
    EffectSsHitMark_SpawnCustomScale(play, 0, 0, 700, &hitPos);
    if (self->type == 2) {
        velocity.y = 15.0f;
        FUN_0034ddc8(play, &hitPos, (Vec3f*)&velocity, (Vec3f*)&accel, 5, 2);
    }
    if (self->type == 3) {
        for (i = 0; i < self->numEffects; i++) {
            MTX34 mtx;
            EnGSwitch_SpawnEffects(self, &randPos, 100, self->colorIdx, &mtx);
        }
    }
}

#endif

typedef struct EnGSwitchGallery {
    /* 0x000 */ Actor actor;
    /* 0x1A4 */ u8 pad_1A4[0x1AE - 0x1A4];
    /* 0x1AE */ s16 hitCount;
    /* 0x1B0 */ u8 pad_1B0[0x1BE - 0x1B0];
    /* 0x1BE */ s16 targetState[2];
} EnGSwitchGallery;

void FUN_0036b96c(Actor* actor);
static __inline __pure float EnGSwitch_Fabsf(float x) {
    return __fabsf(x);
}
#define fabsf EnGSwitch_Fabsf

void FUN_001e9c1c(EnGSwitch* self, PlayState* play) {
    EnGSwitchGallery* gallery;

    self->actor.shape.rot.y += 0x280;
    if (self->delayTimer == 0) {
        switch (self->moveMode) {
            case 2:
                Actor_MoveForward(&self->actor);
                f32 vy = self->actor.velocity.y;
                if ((vy < 0.0f) && (self->actor.home.pos.y - 50.0f > self->actor.world.pos.y)) {
                    self->actor.velocity.y = 0.0f;
                    self->actor.gravity = 0.0f;
                    gallery = (EnGSwitchGallery*)self->actor.parent;
                    if (gallery->actor.update != NULL) {
                        gallery->targetState[self->index] = 1;
                    }
                    Actor_Kill(&self->actor);
                }
                break;
            case 4:
                FUN_0036b96c(&self->actor);
                if ((self->actor.velocity.x < 0.0f) && (self->actor.world.pos.x < self->targetPos.x)) {
                    gallery = (EnGSwitchGallery*)self->actor.parent;
                    if (gallery->actor.update != NULL) {
                        gallery->targetState[self->index] = 1;
                    }
                    Actor_Kill(&self->actor);
                }
                break;
            case 5:
                FUN_0036b96c(&self->actor);
                if (self->actor.world.pos.x > self->targetPos.x) {
                    gallery = (EnGSwitchGallery*)self->actor.parent;
                    if (gallery->actor.update != NULL) {
                        gallery->targetState[self->index] = 1;
                    }
                    Actor_Kill(&self->actor);
                }
                break;
            default:
                switch (self->moveState) {
                    case 0:
                        if ((fabsf(self->actor.world.pos.x - self->targetPos.x) > 5.0f) ||
                            (fabsf(self->actor.world.pos.y - self->targetPos.y) > 5.0f)) {
                            Math_ApproachF(&self->actor.world.pos.x, self->targetPos.x, 0.3f, 30.0f);
                            Math_ApproachF(&self->actor.world.pos.y, self->targetPos.y, 0.3f, 30.0f);
                        } else {
                            self->moveState = 1;
                            self->waitTimer = 90;
                        }
                        break;
                    case 1:
                        if (self->waitTimer == 0) {
                            if ((fabsf(self->actor.world.pos.x - self->actor.home.pos.x) > 5.0f) ||
                                (fabsf(self->actor.world.pos.y - self->actor.home.pos.y) > 5.0f)) {
                                Math_ApproachF(&self->actor.world.pos.x, self->actor.home.pos.x, 0.3f, 30.0f);
                                Math_ApproachF(&self->actor.world.pos.y, self->actor.home.pos.y, 0.3f, 30.0f);
                            } else {
                                gallery = (EnGSwitchGallery*)self->actor.parent;
                                if (gallery->actor.update != NULL) {
                                    gallery->targetState[self->index] = 1;
                                }
                                Actor_Kill(&self->actor);
                            }
                        }
                        break;
                }
                break;
        }
        if ((self->collider.base.acFlags & AC_HIT) || BREG(8)) {
            self->collider.base.acFlags &= ~AC_HIT;
            gallery = (EnGSwitchGallery*)self->actor.parent;
            if (gallery->actor.update != NULL) {
                gallery->hitCount++;
                gallery->targetState[self->index] = 2;
                Audio_PlaySoundGeneral(0x0100022A, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                                       &DAT_0054ac24.unk_0);
                Audio_PlaySoundGeneral(0x01000488, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                                       &DAT_0054ac24.unk_0);
                FUN_0033ba54(self, play);
                self->killTimer = 0x4B;
                self->broken = 1;
                self->actionFunc = FUN_00238cb4;
            }
        }
    }
}

// Draft of FUN_001e5048 (EnGSwitch_DrawEffects, NOT compiled): 240/330 words off (built 1204 vs rom 1320 B). ROM inlines a
// Matrix class (identity+translate via Vector3f temp copy, per-column scale with vmov copies, rotXYZ) on a 3x4 at
// sp+0xC, SetUnkAC 1 kept in r8; same unsolved class as Bg_Menkuri_Eye Draw. Switch cases = FUN_0037266c on idx==colorIdx.
#if 0
f32 __hardfp_sinf(f32);
f32 __hardfp_cosf(f32);
#define ROTX(r) a = mtx.data[r][1]; b = mtx.data[r][2]; mtx.data[r][1] = a * cs + b * sn; mtx.data[r][2] = b * cs - a * sn;
#define ROTY(r) a = mtx.data[r][0]; b = mtx.data[r][2]; mtx.data[r][0] = a * cs - b * sn; mtx.data[r][2] = a * sn + b * cs;
#define ROTZ(r) a = mtx.data[r][0]; b = mtx.data[r][1]; mtx.data[r][0] = a * cs + b * sn; mtx.data[r][1] = b * cs - a * sn;

void FUN_001e5048(EnGSwitch* self, PlayState* play) {
    EnGSwitchEffect* effect = self->effects;
    MTX34 mtx;
    s16 i;

    MTX34CopyAsm(&mtx, &self->actor.modelMtx);
    for (i = 0; i < self->numEffects; i++, effect++) {
        SkeletonAnimationModel* model;

        if (effect->flag && (model = self->models[i + 2]) != NULL) {
            f32 scale = effect->scale * 0.0001f;
            f32 sn, cs, a, b, ang;
            Vec3f pos = effect->pos;

            mtx.data[0][0] = 1.0f; mtx.data[0][1] = 0.0f; mtx.data[0][2] = 0.0f; mtx.data[0][3] = pos.x;
            mtx.data[1][0] = 0.0f; mtx.data[1][1] = 1.0f; mtx.data[1][2] = 0.0f; mtx.data[1][3] = pos.y;
            mtx.data[2][0] = 0.0f; mtx.data[2][1] = 0.0f; mtx.data[2][2] = 1.0f; mtx.data[2][3] = pos.z;
            mtx.data[0][0] *= scale; mtx.data[1][0] *= scale; mtx.data[2][0] *= scale;
            mtx.data[0][1] *= scale; mtx.data[1][1] *= scale; mtx.data[2][1] *= scale;
            mtx.data[0][2] *= scale; mtx.data[1][2] *= scale; mtx.data[2][2] *= scale;
            ang = effect->rot.x;
            if (ang != 0.0f) {
                sn = __hardfp_sinf(ang);
                cs = __hardfp_cosf(ang);
                ROTX(0) ROTX(1) ROTX(2)
            }
            ang = effect->rot.y;
            if (ang != 0.0f) {
                sn = __hardfp_sinf(ang);
                cs = __hardfp_cosf(ang);
                ROTY(0) ROTY(1) ROTY(2)
            }
            ang = effect->rot.z;
            if (ang != 0.0f) {
                sn = __hardfp_sinf(ang);
                cs = __hardfp_cosf(ang);
                ROTZ(0) ROTZ(1) ROTZ(2)
            }
            switch (effect->colorIdx) {
                case 0:
                    FUN_0037266c(model, 0);
                    FUN_0036932c(model, 1);
                    FUN_0036932c(model, 2);
                    FUN_0036932c(model, 3);
                    FUN_0036932c(model, 4);
                    FUN_0036932c(model, 5);
                    break;
                case 1:
                    FUN_0036932c(model, 0);
                    FUN_0037266c(model, 1);
                    FUN_0036932c(model, 2);
                    FUN_0036932c(model, 3);
                    FUN_0036932c(model, 4);
                    FUN_0036932c(model, 5);
                    break;
                case 2:
                    FUN_0036932c(model, 0);
                    FUN_0036932c(model, 1);
                    FUN_0037266c(model, 2);
                    FUN_0036932c(model, 3);
                    FUN_0036932c(model, 4);
                    FUN_0036932c(model, 5);
                    break;
                case 3:
                    FUN_0036932c(model, 0);
                    FUN_0036932c(model, 1);
                    FUN_0036932c(model, 2);
                    FUN_0037266c(model, 3);
                    FUN_0036932c(model, 4);
                    FUN_0036932c(model, 5);
                    break;
                case 4:
                    FUN_0036932c(model, 0);
                    FUN_0036932c(model, 1);
                    FUN_0036932c(model, 2);
                    FUN_0036932c(model, 3);
                    FUN_0037266c(model, 4);
                    FUN_0036932c(model, 5);
                    break;
                case 5:
                    FUN_0036932c(model, 0);
                    FUN_0036932c(model, 1);
                    FUN_0036932c(model, 2);
                    FUN_0036932c(model, 3);
                    FUN_0036932c(model, 4);
                    FUN_0037266c(model, 5);
                    break;
            }
            model->SetUnkAC(1);
            model->FUN_003721e0(&mtx);
            model->FUN_00372170(0);
        }
    }
}
#endif

void FUN_00238cb4(EnGSwitch* self, PlayState* play) {
    if (self->killTimer == 0) {
        Actor_Kill(&self->actor);
    }
}

void FUN_0037e450(Actor* thisx, PlayState* play) {
    EnGSwitch* self = (EnGSwitch*)thisx;
    MTX34 mtx;

    if (!self->broken) {
        MTX34CopyAsm(&mtx, &thisx->modelMtx);
        SkeletonAnimationModel* model = self->models[0];
        if (model != NULL) {
            model->SetUnkAC(1);
            model->FUN_003721e0(&mtx);
            model->FUN_00372170(0);
        }
    }
}

void FUN_002593d0(EnGSwitch* self, PlayState* play) {
    if (Object_IsLoaded(&play->objectCtx, self->requiredObjectSlot)) {
        self->actor.objBankIndex = self->requiredObjectSlot;
        self->actor.draw = (ActorFunc)FUN_0037e450;
        FUN_00352ee0(&self->actor, play, 42, self->models, self->modelIds);
        self->actionFunc = FUN_002073c0;
    }
}

void FUN_001745ac(EnGSwitch* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    self->actor.shape.rot.y += 0x280;
    if (self->killTimer == 0) {
        Actor_Kill(&self->actor);
        return;
    }
    self->actor.world.pos = player->actor.world.pos;
    self->actor.world.pos.y =
        player->actor.world.pos.y + 40.0f + Math_SinS(self->killTimer * 0x3A98) * (self->killTimer * 0.3f);
    if (LINK_IS_ADULT) {
        self->actor.world.pos.y += 20.0f;
    }
}

void FUN_002692ac(Actor* thisx, PlayState* play) {
    EnGSwitch* self = (EnGSwitch*)thisx;

    if (!self->broken) {
        MTX34 mtx;

        MTX34CopyAsm(&mtx, &thisx->modelMtx);
        SkeletonAnimationModel* model = self->models[1];
        if (model != NULL) {
            switch (self->colorIdx) {
                case 0:
                    FUN_00369178(model, 0x6C);
                    break;
                case 1:
                    FUN_00369178(model, 0x6D);
                    break;
                case 2:
                    FUN_00369178(model, 0x6E);
                    break;
                case 3:
                    FUN_00369178(model, 0x71);
                    break;
                case 4:
                    FUN_00369178(model, 0x70);
                    break;
                case 5:
                    FUN_0036932c(model, 0);
                    FUN_0036932c(model, 1);
                    FUN_0036932c(model, 2);
                    FUN_0036932c(model, 3);
                    FUN_0036932c(model, 4);
                    FUN_0037266c(model, 5);
                    break;
                default:
                    break;
            }
            model->SetUnkAC(1);
            model->FUN_003721e0(&mtx);
            model->FUN_00372170(0);
        }
    }
    if (self->type == 3) {
        FUN_001e5048(self, play);
    }
}

void FUN_003df554(EnGSwitch* self, PlayState* play) {
    if (self->noteIndex < EnGSwitch_sCollectedCount && EnGSwitch_sCollectedCount < 5) {
        FUN_0035e5b4(NULL, 0x010004C9, (s8)EnGSwitch_sMajorScale[self->noteIndex]);
        self->noteIndex = EnGSwitch_sCollectedCount;
    }
    if (EnGSwitch_sCollectedCount >= self->silverCount) {
        if ((play->sceneNum == 0xB) && (self->actor.room == 2)) {
            Flags_SetTempClear(play, self->actor.room);
        } else {
            if (!((gSaveContext.masterQuestFlag != 0) && (play->sceneNum == 1) && (self->actor.room == 2))) {
                FUN_00372244(&play->unk_5FCC, 0x1E, 0x1000487);
            }
            Flags_SetSwitch(play, self->switchFlag);
        }
        Audio_PlaySoundGeneral(0x01000488, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               &DAT_0054ac24.unk_0);
        Actor_Kill(&self->actor);
    }
}

void FUN_00198288(EnGSwitch* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    self->actor.shape.rot.y += 0x280;
    if (self->actor.xyzDistToPlayerSq < SQ(30.0f)) {
        if (gSaveContext.masterQuestFlag == 1) {
            if (play->sceneNum == 1) {
                if (self->actor.room == 2) {
                    s32 found = 0;
                    Actor* list[8];
                    s32 count;
                    s32 i;

                    count = FUN_00360084(&play->actorCtx, 0x1A0, 1, list, 8);
                    for (i = 0; i < count; i++) {
                        Actor* actor = list[i];

                        if (actor->xzDistToPlayer < 60.0f) {
                            found = 1;
                        }
                    }
                    if (found) {
                        return;
                    }
                }
            } else if ((play->sceneNum == 11) && (self->actor.room == 2)) {
                if (((s32)self->actor.home.pos.x == -1480) && ((s32)self->actor.home.pos.y == 200) &&
                    ((s32)self->actor.home.pos.z == -1000)) {
                    if (((u8*)player)[0x1A9] != 0x11) {
                        return;
                    }
                }
            }
        }

        Rupees_ChangeBy(5);
        s16 cnt = EnGSwitch_sCollectedCount;
        cnt++;
        EnGSwitch_sCollectedCount = cnt;
        Audio_PlaySoundGeneral(0x01000488, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               &DAT_0054ac24.unk_0);
        self->actor.world.pos = player->actor.world.pos;
        self->actor.world.pos.y += 40.0f;
        if (LINK_IS_ADULT) {
            self->actor.world.pos.y += 20.0f;
        }
        self->actor.gravity = 0.0f;
        self->killTimer = 0x17;
        self->actionFunc = FUN_001745ac;
    }
}

void FUN_002073c0(EnGSwitch* self, PlayState* play) {
    self->actor.shape.rot.y += 0x280;
    if (self->collider.base.acFlags & AC_HIT) {
        Vec3f* thisPos = &self->actor.world.pos;
        s32 i;
        s16 angle;
        PlayState* play2 = play;

        self->collider.base.acFlags &= ~AC_HIT;
        i = 0;
        angle = 0;
        if (EnGSwitch_sStatics.shardCount > 0) do {
            Vec3f pos;
            Vec3f vel;
            f32 sn = Math_SinS(angle);
            f32 cs = Math_CosS(angle);
            f32 rand;
            s16 phi_s0;

            pos.x = sn * 8.0f;
            pos.y = 10.0f + Rand_CenteredFloat(5.0f);
            pos.z = cs * 8.0f;

            vel.x = pos.x * 0.5f;
            vel.y = 10.0f + Rand_ZeroOne() * 15.0f;
            vel.z = pos.z * 0.5f;

            pos.x += thisPos->x;
            pos.y += thisPos->y;
            pos.z += thisPos->z;

            rand = Rand_ZeroOne();
            if (rand < 0.2f) {
                phi_s0 = 0x60;
            } else if (rand < 0.6f) {
                phi_s0 = 0x40;
            } else {
                phi_s0 = 0x20;
            }

            EffectSsKakera_Spawn(play2, &pos, &vel, thisPos, -240, phi_s0 | 0x200, 10, 10, 0,
                                 30.0f + Rand_ZeroOne() * 130.0f, 0, 0x20, 60, -1, 300, NULL);
            angle += 0x4E20;
        } while (++i < EnGSwitch_sStatics.shardCount);
        FUN_0037378c(play, thisPos, 30.0f, 4, 20, 50, 0);
        Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 40, 0x010001DE);
        FUN_0033ba54(self, play);
        self->killTimer = 0x4B;
        self->broken = 1;
        self->actionFunc = FUN_00238cb4;
    }
}
}
