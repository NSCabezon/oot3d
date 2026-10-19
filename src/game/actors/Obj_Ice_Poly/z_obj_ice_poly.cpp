extern "C" {
#include "z_obj_ice_poly.hpp"

#define FLAGS 0x80400010

void ObjIcePoly_Init(Actor* actor, PlayState* play);
void ObjIcePoly_Destroy(Actor* actor, PlayState* play);
void ObjIcePoly_Update(Actor* actor, PlayState* play);
void ObjIcePoly_Draw(Actor* actor, PlayState* play);

GLOBAL_ASM("data/z_Obj_Ice_Poly.data.s")

GLOBAL_ASM("../src/game/actors/Obj_Ice_Poly/data_syms.s")

// Static data lives in the data dump above (0x00535180..); EQU symbols in data_syms.s.
extern s16 ObjIcePoly_sOffsetY[];
extern ColliderCylinderInit ObjIcePoly_sCylinderInitIce;
extern ColliderCylinderInit ObjIcePoly_sCylinderInitHard;
extern f32 ObjIcePoly_sScale[];
extern Color_RGBA8 ObjIcePoly_sColorWhite;
extern Color_RGBA8 ObjIcePoly_sColorGray;
#define sOffsetY ObjIcePoly_sOffsetY
#define sCylinderInitIce ObjIcePoly_sCylinderInitIce
#define sCylinderInitHard ObjIcePoly_sCylinderInitHard
#define sScale ObjIcePoly_sScale
#define sColorWhite ObjIcePoly_sColorWhite
#define sColorGray ObjIcePoly_sColorGray

void ObjIcePoly_Idle(ObjIcePoly* self, PlayState* play);

ActorInit Obj_Ice_Poly_InitVars = {
    ACTOR_OBJ_ICE_POLY,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(ObjIcePoly),
    (ActorFunc)ObjIcePoly_Init,
    (ActorFunc)ObjIcePoly_Destroy,
    (ActorFunc)ObjIcePoly_Update,
    (ActorFunc)ObjIcePoly_Draw,
};

void ObjIcePoly_Init(Actor* thisx, PlayState* play) {
    ObjIcePoly* self = (ObjIcePoly*)thisx;

    self->unk_1A9 = (s16)((u16)thisx->params) >> 8;
    thisx->params &= 0xFF;
    if ((u16)thisx->params >= 3) {
        Actor_Kill(thisx);
        return;
    }
    Actor_SetScale(thisx, sScale[thisx->params]);
    thisx->world.pos.y = thisx->home.pos.y + sOffsetY[thisx->params];
    Collider_InitCylinder(play, &self->iceCollider);
    Collider_SetCylinder(play, &self->iceCollider, thisx, &sCylinderInitIce);
    Collider_InitCylinder(play, &self->hardCollider);
    Collider_SetCylinder(play, &self->hardCollider, thisx, &sCylinderInitHard);
    Collider_UpdateCylinder(thisx, &self->iceCollider);
    Collider_UpdateCylinder(thisx, &self->hardCollider);
    self->iceCollider.dim.radius *= thisx->scale.x;
    self->iceCollider.dim.height *= thisx->scale.y;
    self->hardCollider.dim.radius *= thisx->scale.x;
    self->hardCollider.dim.height *= thisx->scale.y;
    thisx->colChkInfo.mass = MASS_IMMOVABLE;
    self->alpha = 255;
    Actor_SetFocus(thisx, thisx->scale.y * 30.0f);
    self->actionFunc = ObjIcePoly_Idle;
    FUN_00372f38(thisx, play, &self->skelAnimModel, 0x10, 0);
}

void ObjIcePoly_Destroy(Actor* thisx, PlayState* play) {
    ObjIcePoly* self = (ObjIcePoly*)thisx;

    if (self->actor.params >= 0 && self->actor.params < 3) {
        Collider_DestroyCylinder(play, &self->iceCollider);
        Collider_DestroyCylinder(play, &self->hardCollider);
        FUN_00350f34(thisx, &self->skelAnimModel, 0);
    }
}

void ObjIcePoly_Update(Actor* thisx, PlayState* play) {
    ObjIcePoly* self = (ObjIcePoly*)thisx;

    self->actionFunc(self, play);
}


float sinf(float x);
float cosf(float x);

// UNMATCHED draft ObjIcePoly_Draw (16 words of 111, same size): sinf/cosf Z (-0.1227) then X (+0.1227) rotation of the
// copied model matrix, as one shared loop helper. Remaining: ROM hoists `mov r5,#1` (SetUnkAC arg) before the first
// sinf, and the last-row s1/s2 load order of the second rotation is swapped.
GLOBAL_ASM("asm/ObjIcePoly_Draw.s")
#if 0
void FUN_003695cc(SkeletonAnimationModel* skelAnimModel, s32 arg1, s32 arg2, s32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7);

static inline void ObjIcePoly_RotCols(MTX34* m, s32 c0, s32 c1, f32 angle) {
    f32 sn = sinf(angle);
    f32 cs = cosf(angle);
    s32 r;

    for (r = 0; r < 3; r++) {
        f32 a = m->data[r][c0];
        f32 b = m->data[r][c1];

        m->data[r][c0] = a * cs + b * sn;
        m->data[r][c1] = b * cs - a * sn;
    }
}

void ObjIcePoly_Draw(Actor* thisx, PlayState* play) {
    ObjIcePoly* self = (ObjIcePoly*)thisx;
    MTX34 mtx;

    MTX34CopyAsm(&mtx, &thisx->modelMtx);
    ObjIcePoly_RotCols(&mtx, 0, 1, -0.12271846f);
    ObjIcePoly_RotCols(&mtx, 1, 2, 0.12271846f);
    if (self->skelAnimModel != NULL) {
        FUN_003695cc(self->skelAnimModel, 0, 4, 0, 0.0f, 0.0f, 0.0f, (f32)self->alpha * (1.0f / 255.0f));
        FUN_003695cc(self->skelAnimModel, 1, 4, 0, 0.0f, 0.0f, 0.0f, (f32)self->alpha * (1.0f / 255.0f));
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&mtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}
#endif

// UNMATCHED draft ObjIcePoly_Idle (far, 144 words): needs the function-local static Vector3f (guard @0x535188 shares
// the .data base 0x535180 with the two colors, object @0x5a2ea4 .bss => pin_static) and Player raw offsets
// (+0x229C f32, +0x22A0 s16); tag: static.
GLOBAL_ASM("asm/ObjIcePoly_Idle.s")
#if 0
s16 Rand_S16Offset(s16 base, s16 range);
void OnePointCutscene_Init(PlayState* play, s16 csId, s16 arg2, Actor* actor, s16 camId);
void EffectSsKiraKira_SpawnDispersed(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel,
                                     Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s32 life);

void ObjIcePoly_Melt(ObjIcePoly* self, PlayState* play);

void ObjIcePoly_Idle(ObjIcePoly* self, PlayState* play) {
    static Vector3f zeroVec(0.0f, 0.0f, 0.0f);
    Vec3f pos;
    Player* player;
    u32 dmgFlags;

    if (self->iceCollider.base.acFlags & AC_HIT) {
        dmgFlags = self->iceCollider.info.acHitInfo->toucher.dmgFlags;
        if (dmgFlags == 0x800) {
            self->meltTimer = -0x13;
        } else if (dmgFlags == 0x20000) {
            self->meltTimer = -0x4B;
        }
        self->actor.focus.rot.y = self->actor.yawTowardsPlayer;
        OnePointCutscene_Init(play, 0x1400, 40, &self->actor, 0);
        self->actionFunc = ObjIcePoly_Melt;
    } else if (self->actor.parent != NULL) {
        self->actor.parent->freezeTimer = 40;
        CollisionCheck_SetAT(play, &play->colChkCtx, &self->iceCollider.base);
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->iceCollider.base);
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->iceCollider.base);
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->hardCollider.base);
    } else {
        Actor_Kill(&self->actor);
    }
    player = GET_PLAYER(play);
    if (FUN_00374be8(play, 0x12)) {
        if (*(s32*)&self->actor.xzDistToPlayer < 0x42140000) {
            *(s16*)((u8*)player + 0x22A0) = self->actor.yawTowardsPlayer;
            *(f32*)((u8*)player + 0x229C) = 1.0f;
        }
    }
    pos.x = self->actor.world.pos.x +
            (Rand_S16Offset(15, 15) * (Rand_ZeroOne() < 0.5f ? -1 : 1)) * self->actor.scale.x;
    pos.y = self->actor.world.pos.y + Rand_S16Offset(10, 90) * self->actor.scale.y;
    pos.z = self->actor.world.pos.z +
            (Rand_S16Offset(15, 15) * (Rand_ZeroOne() < 0.5f ? -1 : 1)) * self->actor.scale.z;
    if (*(u32*)((u8*)play + 0x5BF4) % 7 == 0) {
        EffectSsKiraKira_SpawnDispersed(play, &pos, (Vec3f*)&zeroVec, (Vec3f*)&zeroVec, &sColorWhite, &sColorGray,
                                        2000, 5);
    }
}
#endif

// UNMATCHED draft ObjIcePoly_Melt (22 words of 155; size +4): all but the final scale/rounding block matches. ROM loads
// the 0.5f constant (+copy) before the `meltTimer > 0` test and numbers the VFP regs differently (s2=3.0, s4=w, s3=base);
// the sColor pair is derived from one literal (`sub r0,r1,#4`) => needs sColorWhite/Gray as real statics at the start
// of the data dump (trimmed data copy), not EQUs. Any named const local hoists the pool to the function entry.
GLOBAL_ASM("asm/ObjIcePoly_Melt.s")
#if 0
s16 Rand_S16Offset(s16 base, s16 range);
void FUN_00366150(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel, Color_RGBA8* prim, Color_RGBA8* env,
                  s16 scale, s16 scaleStep);

void ObjIcePoly_Melt(ObjIcePoly* self, PlayState* play) {
    Vec3f accel;
    Vec3f vel;
    Vec3f pos;
    s32 i;

    accel.x = 0.0f;
    accel.y = self->actor.scale.y;
    accel.z = 0.0f;
    vel.x = 0.0f;
    vel.y = self->actor.scale.y;
    vel.z = 0.0f;

    for (i = 0; i < 2; i++) {
        pos.x = self->actor.world.pos.x +
                (Rand_S16Offset(20, 20) * (Rand_ZeroOne() < 0.5f ? -1 : 1)) * self->actor.scale.x;
        pos.y = self->actor.world.pos.y + self->actor.scale.y * (Rand_ZeroOne() * 50.0f);
        pos.z = self->actor.world.pos.z +
                (Rand_S16Offset(20, 20) * (Rand_ZeroOne() < 0.5f ? -1 : 1)) * self->actor.scale.x;
        FUN_00366150(play, &pos, &vel, &accel, &sColorWhite, &sColorGray,
                     Rand_S16Offset(0x15E, 0x64) * self->actor.scale.x, self->actor.scale.x * 20.0f);
    }
    if (self->meltTimer < 0) {
        if (self->actor.parent != NULL) {
            self->actor.parent->freezeTimer = 40;
        }
        self->meltTimer++;
        if (self->meltTimer == 0) {
            self->meltTimer = 40;
            Audio_PlayActorSound2(&self->actor, 0x010001F9);
        }
    } else {
        s32 n;
        f32 w;
        f32 base;

        if (self->meltTimer != 0) {
            self->meltTimer--;
        }
        base = sScale[self->actor.params];
        w = self->meltTimer * 3.0f;
        if (self->meltTimer > 0) {
            n = (s32)(0.5f + w * 0.5f);
        } else {
            n = (s32)(w * 0.5f - 0.5f);
        }
        self->actor.scale.y = base * (0.5f + n * 0.0125f);
        self->alpha -= 6;
        if (self->meltTimer == 0) {
            Actor_Kill(&self->actor);
        }
    }
}
#endif

}
