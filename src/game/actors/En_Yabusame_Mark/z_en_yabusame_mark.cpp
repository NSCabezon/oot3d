extern "C" {
#include "z_en_yabusame_mark.hpp"

#define FLAGS 0x00000000

// Local prototypes for helpers not declared in include/ (3DS signatures)
s32 Collider_InitQuad(PlayState* play, EnYabusameMarkCollider* quad);
s32 Collider_SetQuad(PlayState* play, EnYabusameMarkCollider* dest, Actor* actor, void* src);
void FUN_0034eaf8(PlayState* play, EnYabusameMarkCollider* quad); // Collider_DestroyQuad
void FUN_0035479c(EnYabusameMarkCollider* quad, Vec3f* a, Vec3f* b, Vec3f* c, Vec3f* d); // Collider_SetQuadVertices
void FUN_0034ddc8(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel, s16 scale, s16 type); // EffectSsExtra_Spawn
void EffectSsHitMark_SpawnCustomScale(PlayState* play, s32 type, s32 arg2, s32 scale, Vec3f* pos);
void FUN_0035c528(u32 seqId); // Audio_PlayFanfare
void FUN_0048961c(u32 sfxId); // Audio_StopSfxById
void Audio_PlaySoundGeneral(u32 sfxId, Vec3f* pos, u8 a2, f32* a3, f32* a4, s8* a5);

extern __pure float _fabsf(float);
static __inline __pure float bbg_fabsf(float f) {
    return _fabsf(f);
}

extern f32 sYabusameRingDistance[];
extern Vec3f sYabusameTargetPos[];
extern Vec3f sYabusameCollisionVertices[];
extern u8 sYabusameQuadInit[];

void EnYabusameMark_Init(EnYabusameMark* self, PlayState* play);
void EnYabusameMark_Destroy(EnYabusameMark* self, PlayState* play);
void EnYabusameMark_Update(Actor* thisx, PlayState* play);
void FUN_0020761c(EnYabusameMark* self, PlayState* play); // N64: func_80B42F74

GLOBAL_ASM("data/z_En_Yabusame_Mark.data.s")
GLOBAL_ASM("../src/game/actors/En_Yabusame_Mark/z_en_yabusame_mark_symbols.s")

ActorInit En_Yabusame_Mark_InitVars = {
    ACTOR_EN_YABUSAME_MARK,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnYabusameMark),
    (ActorFunc)EnYabusameMark_Init,
    (ActorFunc)EnYabusameMark_Destroy,
    (ActorFunc)EnYabusameMark_Update,
    NULL,
};

void EnYabusameMark_Init(EnYabusameMark* self, PlayState* play) {
    self->actor.flags &= ~1;
    // u16 store: ROM stores params with ldrh/strh and reloads typeIndex (ldrsh) for the switch
    *(u16*)&self->typeIndex = self->actor.params;
    self->actor.targetMode = 5;
    switch (self->typeIndex) {
        case 0:
            self->subTypeIndex = 0;
            if (self->actor.world.pos.z > 0.0f) {
                self->subTypeIndex = 1;
            }
            break;
        case 1:
            self->subTypeIndex = 2;
            if (self->actor.world.pos.z < -2000.0f) {
                self->subTypeIndex = 3;
            }
            break;
        case 2:
            self->subTypeIndex = 4;
            break;
    }
    Collider_InitQuad(play, &self->collider);
    Collider_SetQuad(play, &self->collider, &self->actor, sYabusameQuadInit);
    self->worldPos = self->actor.world.pos;
    self->actor.flags |= 0x10;
    if (gSaveContext.sceneSetupIndex != 4) {
        Actor_Kill(&self->actor);
        return;
    }
    self->actionFunc = FUN_0020761c;
}

void EnYabusameMark_Destroy(EnYabusameMark* self, PlayState* play) {
    FUN_0034eaf8(play, &self->collider);
}


void EnYabusameMark_Update(Actor* thisx, PlayState* play) {
    EnYabusameMark* self = (EnYabusameMark*)thisx;
    u32 arrayIndex;

    self->actionFunc(self, play);
    arrayIndex = self->typeIndex * 4;
    self->vertexA.x = self->actor.world.pos.x + sYabusameCollisionVertices[arrayIndex + 0].x;
    self->vertexA.y = self->actor.world.pos.y + sYabusameCollisionVertices[arrayIndex + 0].y;
    self->vertexA.z = self->actor.world.pos.z + sYabusameCollisionVertices[arrayIndex + 0].z;
    self->vertexB.x = self->actor.world.pos.x + sYabusameCollisionVertices[arrayIndex + 1].x;
    self->vertexB.y = self->actor.world.pos.y + sYabusameCollisionVertices[arrayIndex + 1].y;
    self->vertexB.z = self->actor.world.pos.z + sYabusameCollisionVertices[arrayIndex + 1].z;
    self->vertexC.x = self->actor.world.pos.x + sYabusameCollisionVertices[arrayIndex + 2].x;
    self->vertexC.y = self->actor.world.pos.y + sYabusameCollisionVertices[arrayIndex + 2].y;
    self->vertexC.z = self->actor.world.pos.z + sYabusameCollisionVertices[arrayIndex + 2].z;
    self->vertexD.x = self->actor.world.pos.x + sYabusameCollisionVertices[arrayIndex + 3].x;
    self->vertexD.y = self->actor.world.pos.y + sYabusameCollisionVertices[arrayIndex + 3].y;
    self->vertexD.z = self->actor.world.pos.z + sYabusameCollisionVertices[arrayIndex + 3].z;
    FUN_0035479c(&self->collider, &self->vertexA, &self->vertexB, &self->vertexC, &self->vertexD);
    CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
}


// Action function: N64 func_80B42F74; asm still in functions_200000s.cpp
// Draft is 2 words off: ROM stores effectVelocity.x/.y from s1/s2 (reusing effectAccel ctor regs), ours from s0.
#if 0 // draft: not byte-identical yet, see STATUS.md
void FUN_0020761c(EnYabusameMark* self, PlayState* play) {
    Vector3f effectAccel(0.0f, 0.0f, 0.0f);
    Vector3f effectVelocity(0.0f, 0.0f, 0.0f);
    Vec3f arrowHitPos;
    Vec3f distanceFromCenter;
    s32 scoreIndex;
    f32 scoreDistance100;
    f32 scoreDistance60;
    f32 scoreDistance30;

    if (self->collider.base.acFlags & 2) {
        self->collider.base.acFlags &= ~2;

        arrowHitPos.x = self->collider.elem.bumper.hitPos.x;
        arrowHitPos.y = self->collider.elem.bumper.hitPos.y;
        arrowHitPos.z = self->collider.elem.bumper.hitPos.z;

        effectVelocity.y = 15.0f;

        EffectSsHitMark_SpawnCustomScale(play, 0, 2, 700, &arrowHitPos);

        scoreIndex = 2;

        scoreDistance100 = sYabusameRingDistance[self->typeIndex * 4 + 0];
        scoreDistance60 = sYabusameRingDistance[self->typeIndex * 4 + 1];
        scoreDistance30 = sYabusameRingDistance[self->typeIndex * 4 + 2];

        distanceFromCenter.x = bbg_fabsf(sYabusameTargetPos[self->subTypeIndex].x - arrowHitPos.x);
        distanceFromCenter.y = bbg_fabsf(sYabusameTargetPos[self->subTypeIndex].y - arrowHitPos.y);
        distanceFromCenter.z = bbg_fabsf(sYabusameTargetPos[self->subTypeIndex].z - arrowHitPos.z);

        if (distanceFromCenter.x > scoreDistance100 || distanceFromCenter.y > scoreDistance100 ||
            distanceFromCenter.z > scoreDistance100) {
            scoreIndex = 1;
            if (distanceFromCenter.x > scoreDistance60 || distanceFromCenter.y > scoreDistance60 ||
                distanceFromCenter.z > scoreDistance60) {
                scoreIndex = 0;
            }
            if (distanceFromCenter.x > scoreDistance30 || distanceFromCenter.y > scoreDistance30 ||
                distanceFromCenter.z > scoreDistance30) {
                return;
            }
        }

        if (scoreIndex == 2) {
            FUN_0035c528(0x01000590);
        }
        if (scoreIndex == 1) {
            FUN_0048961c(0x0100048C);
            Audio_PlaySoundGeneral(0x0100048C, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        }
        if (scoreIndex == 0) {
            Audio_PlaySoundGeneral(0x0100048D, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        }
        FUN_0034ddc8(play, &arrowHitPos, (Vec3f*)&effectVelocity, (Vec3f*)&effectAccel, 5, scoreIndex);
    }
}
#endif
}
