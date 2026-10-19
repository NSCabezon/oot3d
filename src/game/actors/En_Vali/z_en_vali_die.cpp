extern "C" {
#include "z_en_vali.hpp"

#define ACTOR_FLAG_ATTENTION_ENABLED (1 << 0)

void Animation_PlayOnce(SkelAnime* skelAnime, s32 idx);
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);
s16 Camera_GetCamDirYaw(Camera* camera);
void EffectSsLightning_Spawn(PlayState* play, Vec3f* pos, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale,
                             s16 yaw, s32 life, s32 numSegments);
void FUN_0011ca08(EnVali* self, PlayState* play);
void FUN_00120ad0(EnVali* self, PlayState* play);

// EnVali_Electrify (N64 EnVali_DoElectricEffectsAndSfx inlined). Its two function-local statics are the first
// objects of this TU's .data (0x0052FC48), in front of the guards of Die/PostLimbDraw below.
void FUN_001094c0(EnVali* self, PlayState* play) {
    static Color_RGBA8 sEffPrimColor = { 255, 255, 255, 255 };
    static Color_RGBA8 sEffEnvColor = { 200, 255, 255, 255 };
    Vec3f effPos;
    f32 cosCamYaw;
    f32 sinCamYaw;
    s32 i;
    s16 angle;

    if (self->electrifyTimer != 0) {
        self->electrifyTimer--;
    }
    for (i = 0; i < 4; i++) {
        cosCamYaw = -Math_CosS(Camera_GetCamDirYaw(play->cameraPtrs[play->activeCamera]));
        sinCamYaw = Math_SinS(Camera_GetCamDirYaw(play->cameraPtrs[play->activeCamera]));
        if (!((self->electrifyTimer + (i << 1)) % 4)) {
            angle = (s16)Rand_CenteredFloat(12288.0f) + (i * 0x4000) + 0x2000;
            effPos.x = self->actor.world.pos.x + (Math_SinS(angle) * 12.0f * cosCamYaw);
            effPos.y = self->actor.world.pos.y - (Math_CosS(angle) * 12.0f) + 10.0f;
            effPos.z = self->actor.world.pos.z + (Math_SinS(angle) * 12.0f * sinCamYaw);
            EffectSsLightning_Spawn(play, &effPos, &sEffPrimColor, &sEffEnvColor, 17, angle, 6, 2);
        }
    }
    FUN_00373264(&self->actor, 0x0100030A);
    if (self->electrifyTimer == 0) {
        self->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
        self->collider.base.acFlags |= AC_ON;
        if (self->actor.params == 1) {
            Animation_PlayOnce(&self->skelAnime, 2);
            Actor_SetColorFilter(&self->actor, 0x400000, 150, 0x200000, 30);
            self->actor.params = 0;
            self->unk_8B8 = 10;
            self->collider.base.acFlags &= ~AC_ON;
            self->actionFunc = FUN_00120ad0;
        } else {
            self->actionFunc = FUN_0011ca08;
        }
    } else {
        if (self->electrifyTimer & 1) {
            self->actor.world.pos.y += 1.0f;
        } else {
            self->actor.world.pos.y -= 1.0f;
        }
    }
}

void Item_DropCollectibleRandom(PlayState* play, Actor* actor, Vec3f* pos, s32 params);
s16 Rand_S16Offset_003702c8(s16 base, s16 range);
s16 Rand_S16Offset(s16 base, s16 range);
void FUN_003d53a0(EnVali* self, PlayState* play);
void FUN_00364538(EnVali* self, PlayState* play);

void FUN_00364538(EnVali* self, PlayState* play) {
    s32 i;

    for (i = 0; i < 3; i++) {
        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_BILI, self->actor.world.pos.x, self->actor.world.pos.y,
                    self->actor.world.pos.z, 0, self->actor.world.rot.y, 0, 0, 1);
        self->actor.world.rot.y += 0x5555;
    }
    Item_DropCollectibleRandom(play, &self->actor, &self->actor.world.pos, 0x50);
    self->timer = Rand_S16Offset_003702c8(10, 10);
    self->collider.base.acFlags &= ~AC_ON;
    Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 0x28, 0x010002F7);
    self->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    self->actor.draw = NULL;
    self->actionFunc = FUN_003d53a0;
}

// EnVali_Die
f32 Rand_CenteredFloat(f32 v);
void EffectSsDtBubble_SpawnColorProfile(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel, s16 scale, s32 a,
                                        s32 b, s32 c);
void FUN_003d53a0(EnVali* self, PlayState* play) {
    static Vector3f sEffVel(0.0f, 0.0f, 0.0f);
    static Vector3f sEffAccel(0.0f, 0.0f, 0.0f);
    s16 effScale;
    Vec3f effPos;
    s32 i;

    if (self->timer != 0) {
        self->timer--;
    }
    for (i = 0; i < 2; i++) {
        effPos.x = Rand_CenteredFloat(20.0f) + self->actor.world.pos.x;
        effPos.y = Rand_CenteredFloat(8.0f) + self->actor.world.pos.y;
        effPos.z = Rand_CenteredFloat(20.0f) + self->actor.world.pos.z;
        sEffVel.y = Rand_ZeroOne() + 1.0f;
        effScale = Rand_S16Offset(40, 40);
        if (Rand_ZeroOne() < 0.7f) {
            EffectSsDtBubble_SpawnColorProfile(play, &effPos, (Vec3f*)&sEffVel, (Vec3f*)&sEffAccel, effScale, 25, 2, 1);
        } else {
            EffectSsDtBubble_SpawnColorProfile(play, &effPos, (Vec3f*)&sEffVel, (Vec3f*)&sEffAccel, effScale, 25, 0, 1);
        }
    }
    if (self->timer == 0) {
        Actor_Kill(&self->actor);
    }
}

// EnVali_PostLimbDraw (OverrideLimbDraw lives in z_en_vali_draw.cpp)
void FUN_0011ca08(EnVali* self, PlayState* play);
void FUN_0035479c(ColliderQuad* collider, Vec3f* base, Vec3f* tip, Vec3f* quad0, Vec3f* quad1);
void VEC3TransformAsm(Vector3f* dest, MTX34* mtx, Vector3f* src);

static inline void EnVali_MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}

void FUN_001c4c88(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    static Vector3f D_80B28970(3000.0f, 0.0f, 0.0f);
    static Vector3f D_80B2897C(-1000.0f, 0.0f, 0.0f);
    Vec3f tentacleTipPos;
    Vec3f tentacleBasePos;
    EnVali* self = (EnVali*)thisx;

    if (self->actionFunc == FUN_0011ca08) {
        if (limbIndex == 2 || limbIndex == 7) {
            EnVali_MultVec3f(&D_80B28970, (Vector3f*)&tentacleTipPos, &self->actor.modelMtx);
            EnVali_MultVec3f(&D_80B2897C, (Vector3f*)&tentacleBasePos, &self->actor.modelMtx);
            if (limbIndex == 2) {
                FUN_0035479c(&self->leftTentacleCollider, &tentacleBasePos, &tentacleTipPos,
                             &self->leftTentacleCollider.dim.quad[0], &self->leftTentacleCollider.dim.quad[1]);
            } else {
                FUN_0035479c(&self->rightTentacleCollider, &tentacleBasePos, &tentacleTipPos,
                             &self->rightTentacleCollider.dim.quad[0], &self->rightTentacleCollider.dim.quad[1]);
            }
        }
    }
}

}
