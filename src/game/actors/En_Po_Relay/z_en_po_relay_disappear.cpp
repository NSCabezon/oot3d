extern "C" {
#include "z_en_po_relay.hpp"

void Audio_PlayActorSound2(Actor* actor, s32 arg1);

// Own TU: the two guarded function-local statics below need their guards/objects pinned at the ROM addresses
// (see ws/En_Po_Relay/STATUS.md for the pin_static commands).
#define R_UPDATE_RATE SREG(30)

void EffectSsDeadDb_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep,
                          s16 primR, s16 primG, s16 primB, s16 primA, s16 envR, s16 envG, s16 envB, s32 unk, s32 life,
                          s32 playSfx);
s32 Camera_GetCamDirYaw(void* cam);
void Item_DropCollectible2(PlayState* play, Vec3f* pos, s16 params);
void Flags_SetTempClear(PlayState* play, s32 flag);
s32 Flags_GetCollectible(PlayState* play, s32 flag);

static inline void* EnPoRelay_GetActiveCam(PlayState* play) {
    return *(void**)((u8*)play + 0xA54 + *(s16*)((u8*)play + 0xA64) * 4);
}

// Guarded function-local statics (ROM guards 0x52DED8 accel / 0x52DEDC velocity; objects 0x59B5BC / 0x59B5B0).
// The velocity ctor takes (x, z, y) only to get the ROM's VFP register numbering.
struct EnPoRelayParticleVec {
    f32 x, y, z;
    EnPoRelayParticleVec(f32 x_, f32 z_, f32 y_) {
        x = x_;
        y = y_;
        z = z_;
    }
};

void EnPoRelay_DisappearAndReward(EnPoRelay* self, PlayState* play) {
    Vec3f vec;
    f32 multiplier;
    PlayState* play2 = play;
    static EnPoRelayParticleVec sDisappearParticlesVelocity(0.0f, 0.0f, 1.5f);
    static Vector3f sDisappearParticlesAccel(0.0f, 0.0f, 0.0f);

    self->actionTimer++;
    if (self->actionTimer < (s32)(8 * 3.0f / R_UPDATE_RATE + 0.5f)) {
        if (self->actionTimer < (s32)(5 * 3.0f / R_UPDATE_RATE + 0.5f)) {
            vec.y = Math_SinS((self->actionTimer * 0x1000) - 0x4000) * 23.0f + (self->actor.world.pos.y + 40.0f);
            multiplier = Math_CosS((self->actionTimer * 0x1000) - 0x4000) * 23.0f;
            vec.x = (multiplier * Math_SinS(Camera_GetCamDirYaw(EnPoRelay_GetActiveCam(play)) + 0x4800)) +
                    self->actor.world.pos.x;
            vec.z = (multiplier * Math_CosS(Camera_GetCamDirYaw(EnPoRelay_GetActiveCam(play)) + 0x4800)) +
                    self->actor.world.pos.z;
        } else {
            vec.y = self->actor.world.pos.y + 40.0f + 15.0f * (self->actionTimer - 5);
            vec.x = (Math_SinS(Camera_GetCamDirYaw(EnPoRelay_GetActiveCam(play)) + 0x4800) * 23.0f) +
                    self->actor.world.pos.x;
            vec.z = (Math_CosS(Camera_GetCamDirYaw(EnPoRelay_GetActiveCam(play)) + 0x4800) * 23.0f) +
                    self->actor.world.pos.z;
        }
        if (self->actionTimer % 3 != 0) {
            EffectSsDeadDb_Spawn(play, &vec, (Vec3f*)&sDisappearParticlesVelocity, (Vec3f*)&sDisappearParticlesAccel,
                                 self->actionTimer * 10 + 80, 0, 255, 255, 255, 255, 0, 0, 255, 1, 11, 1);
            vec.x = self->actor.world.pos.x * 2.0f - vec.x;
            vec.z = self->actor.world.pos.z * 2.0f - vec.z;
            EffectSsDeadDb_Spawn(play, &vec, (Vec3f*)&sDisappearParticlesVelocity, (Vec3f*)&sDisappearParticlesAccel,
                                 self->actionTimer * 10 + 80, 0, 255, 255, 255, 255, 0, 0, 255, 1, 11, 1);
            vec.x = self->actor.world.pos.x;
            vec.z = self->actor.world.pos.z;
            EffectSsDeadDb_Spawn(play, &vec, (Vec3f*)&sDisappearParticlesVelocity, (Vec3f*)&sDisappearParticlesAccel,
                                 self->actionTimer * 10 + 80, 0, 255, 255, 255, 255, 0, 0, 255, 1, 11, 1);
        }
        if (self->actionTimer == 1) {
            Audio_PlayActorSound2(&self->actor, 0x010002E8);
        }
    }
    if (Math_StepToF(&self->actor.scale.x, 0.0f, 0.001f) != 0) {
        if (self->hookshotSlotFull) {
            Vec3f posAtGround;

            posAtGround.x = self->actor.world.pos.x;
            posAtGround.y = self->actor.floorHeight;
            posAtGround.z = self->actor.world.pos.z;
            {
                s32 hs = *(s32*)((u8*)&gSaveContext + 0xEE8); // HIGH_SCORE(HS_DAMPE_RACE)
                s32 ts = gSaveContext.timer1Value;
                if (hs > ts) {
                    hs = ts;
                }
                *(s32*)((u8*)&gSaveContext + 0xEE8) = hs;
            }
            if (!Flags_GetCollectible(play2, self->actor.params) && (gSaveContext.timer1Value <= 60)) {
                Item_DropCollectible2(play, &posAtGround, ((u16)self->actor.params << 8) + 0x4006);
            } else {
                Actor_Spawn(&play->actorCtx, play, 0x15, posAtGround.x, posAtGround.y, posAtGround.z, 0, 0, 0, 2, 1);
            }
        } else {
            Flags_SetTempClear(play, 4);
            *(s32*)((u8*)&gSaveContext + 0xEE8) = gSaveContext.timer1Value;
        }
        Actor_Kill(&self->actor);
    }
    self->actor.world.pos.y += 10.0f;
    self->actor.scale.y = self->actor.scale.x;
    self->actor.scale.z = self->actor.scale.x;
}

// FUN_00235d68 (PostLimbDraw): draft, 2 words off (x/z stores s2/s0 swapped in the guarded static init; ~100 ctor/param
// variants tried). Must stay in THIS TU: the ROM reaches its guard as TU .data +8 (after the two guards above).
// If it ever matches: pin_static split 0x52DED8 --size 12 (instead of 8), bss 0x59B5B0 36 (instead of 24), drop its
// GLOBAL_ASM line from src/functions/functions_200000s.cpp.
#if 0
void Lights_PointNoGlowSetInfo(void* info, f32 x, f32 y, f32 z, s32 r, s32 g, s32 b, s32 a, s32 e);

struct EnPoRelayLanternVec {
    f32 x, y, z;
    EnPoRelayLanternVec(f32 x_, f32 y_, f32 z_) {
        z = z_;
        y = y_;
        x = x_;
    }
};

void FUN_00235d68(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnPoRelay* self = (EnPoRelay*)thisx;
    static EnPoRelayLanternVec sLanternLightOffset(0.0f, 1200.0f, 0.0f);

    if (limbIndex == 18) {
        Vec3f vec;
        f32 rnd = Rand_ZeroOne();

        self->lightColorR = (s16)(rnd * 30.0f) + 225;
        self->lightColorG = (s16)(rnd * 100.0f) + 155;
        self->lightColorB = (s16)(rnd * 160.0f) + 95;
        VEC3TransformAsm((Vector3f*)&vec, mtx, (Vector3f*)&sLanternLightOffset);
        Lights_PointNoGlowSetInfo(&self->unk_B30, vec.x, vec.y, vec.z, self->lightColorR, self->lightColorG,
                                  self->lightColorB, 200, 0);
    }
}
#endif

}
