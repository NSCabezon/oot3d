extern "C" {
#include "z_en_vb_ball.hpp"

#define FLAGS 0x80000030

void EnVbBall_Init(Actor* thisx, PlayState* play);
void EnVbBall_Destroy(Actor* thisx, PlayState* play);
void EnVbBall_Update(Actor* thisx, PlayState* play2);
void EnVbBall_Draw(Actor* thisx, PlayState* play);

f32 __hardfp_sinf(f32 x);
f32 __hardfp_cosf(f32 x);
SkeletonAnimationModel* FUN_0036a924(Actor* actor, PlayState* play, s16 objectSlot, s32 arg3);
void FUN_00357248(SkeletonAnimationModel** model);

GLOBAL_ASM("data/z_En_Vb_Ball.data.s")
GLOBAL_ASM("../src/game/actors/En_Vb_Ball/data_syms.s")

ActorInit En_Vb_Ball_InitVars = {
    ACTOR_PLAYER,
    ACTORCAT_BOSS,
    FLAGS,
    OBJECT_FD,
    sizeof(EnVbBall),
    (ActorFunc)EnVbBall_Init,
    (ActorFunc)EnVbBall_Destroy,
    (ActorFunc)EnVbBall_Update,
    (ActorFunc)EnVbBall_Draw,
};

extern ColliderCylinderInit EnVbBall_sCylinderInit;

void EnVbBall_Init(Actor* thisx, PlayState* play) {
    EnVbBall* self = (EnVbBall*)thisx;
    f32 angle;

    self->model2 = NULL;
    FUN_00372f38(thisx, play, &self->model0, 8, &self->model1, 7, 0);
    if (self->actor.params == 100) {
        self->model2 = FUN_0036a924(thisx, play, 1, 0x53);
    }
    self->unk_218 = 0.1f;
    if (self->actor.params >= 200) { // Volvagia's bones
        self->yRotVel = Rand_CenteredFloat(768.0f);
        self->xRotVel = Rand_CenteredFloat(768.0f);
        f32 px = self->actor.world.pos.x;
        f32 pz = self->actor.world.pos.z;
        angle = Math_FAtan2F(px, pz);
        self->actor.velocity.y = Rand_ZeroFloat(3.0f);
        self->actor.velocity.x = 2.0f * __hardfp_sinf(angle);
        self->actor.velocity.z = 2.0f * __hardfp_cosf(angle);
        self->actor.gravity = -0.8f;
    } else { // Volvagia's rocks
        Collider_InitCylinder(play, &self->collider);
        Collider_SetCylinder(play, &self->collider, thisx, &EnVbBall_sCylinderInit);
        Actor_SetScale(thisx, self->actor.world.rot.z / 10000.0f);
        self->collider.dim.radius = (s16)(self->actor.scale.y * 3000.0f);
        self->collider.dim.height = (s16)(self->actor.scale.y * 5000.0f);
        self->collider.dim.yShift = (s16)(self->actor.scale.y * -2500.0f);
        self->xRotVel = Rand_CenteredFloat(8192.0f);
        self->yRotVel = Rand_CenteredFloat(8192.0f);
        self->shadowSize = self->actor.scale.y * 68.0f;
    }
}

void EnVbBall_Destroy(Actor* thisx, PlayState* play) {
    EnVbBall* self = (EnVbBall*)thisx;

    if (self->actor.params < 200) {
        Collider_DestroyCylinder(play, &self->collider);
    }
    FUN_00350f34(thisx, &self->model0, &self->model1, 0);
    FUN_00357248(&self->model2);
}

#if 0 // draft EnVbBall_Update: same size as ROM, 14 words off (rot-add reg r0/r1 swap x2 (6w), gravity store s28 vs s0, bones UpdateBgCheckInfo arg order mov r0,play last (3w), 2 pool words); ~600 variants incl. hill-climbs failed
void FUN_0036442c(void* bossFd, Vec3f* pos, Vec3f* vel, Vec3f* acc, f32 scale); // SpawnDust
void FUN_00335814(void* effects, Vec3f* pos, Vec3f* vel, Vec3f* acc, f32 scale); // SpawnDebris
void FUN_0036fca8(Actor* actor, PlayState* play, s32 arg2, s32 arg3);

#define EFFECTS(boss) ((void*)((u8*)(boss) + 0x2924))

static void EnVbBall_UpdateBones(EnVbBall* self, PlayState* play2) {
    PlayState* play = play2;
    void* bossFd = self->actor.parent;
    f32 angle;
    s16 i;

    Actor_UpdateBgCheckInfo(play, &self->actor, 50.0f, 50.0f, 100.0f, 4);
    if ((self->actor.bgCheckFlags & 1) && (self->actor.velocity.y <= 0.0f)) {
        self->xRotVel = Rand_CenteredFloat(16384.0f);
        self->yRotVel = Rand_CenteredFloat(16384.0f);
        f32 px = self->actor.world.pos.x;
        f32 pz = self->actor.world.pos.z;
        angle = Math_FAtan2F(px, pz);
        self->actor.velocity.x = __hardfp_sinf(angle) * 10.0f;
        self->actor.velocity.z = __hardfp_cosf(angle) * 10.0f;
        self->actor.velocity.y *= -0.5f;
        if (self->actor.params & 1) {
            Audio_PlaySoundGeneral(0x01000397, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        }
        for (i = 0; i < 4; i++) {
            Vector3f dustVel(0.0f, 0.0f, 0.0f);
            Vector3f dustAcc(0.0f, 0.0f, 0.0f);
            Vec3f dustPos;

            dustVel.x = Rand_CenteredFloat(8.0f);
            dustVel.y = Rand_ZeroFloat(1.0f);
            dustVel.z = Rand_CenteredFloat(8.0f);

            dustAcc.y = 0.3f;

            dustPos.x = Rand_CenteredFloat(20.0f) + self->actor.world.pos.x;
            dustPos.y = self->actor.floorHeight + 10.0f;
            dustPos.z = Rand_CenteredFloat(20.0f) + self->actor.world.pos.z;

            FUN_0036442c(bossFd, &dustPos, (Vec3f*)&dustVel, (Vec3f*)&dustAcc, Rand_ZeroFloat(200.0f) + 400.0f);
        }
    }
    if (self->actor.world.pos.y < 50.0f) {
        Actor_Kill(&self->actor);
    }
}

void EnVbBall_Update(Actor* thisx, PlayState* play) {
    EnVbBall* self = (EnVbBall*)thisx;
    void* bossFd = self->actor.parent;
    f32 radius;
    s16 spawnNum;
    s16 i;

    self->unkTimer2++;
    if (self->unkTimer1 != 0) {
        self->unkTimer1--;
    }
    self->actor.shape.rot.x += (s16)self->xRotVel;
    self->actor.shape.rot.y += (s16)self->yRotVel;
    {
        f32 g = -1.0f;
        self->actor.gravity = g;
        self->actor.velocity.y += g;
    }
    FUN_0036b96c(&self->actor);
    if (self->actor.params >= 200) {
        EnVbBall_UpdateBones(self, play);
    } else {
        Math_ApproachF(&self->shadowOpacity, 255.0f, 1.0f, 40.0f);
        radius = self->actor.scale.y * 1700.0f;
        self->actor.world.pos.y -= radius;
        Actor_UpdateBgCheckInfo(play, &self->actor, 50.0f, 50.0f, 100.0f, 4);
        self->actor.world.pos.y += radius;
        if ((self->actor.bgCheckFlags & 1) && (self->actor.velocity.y <= 0.0f)) {
            if ((self->actor.params == 100) || (self->actor.params == 101)) {
                Actor_Kill(&self->actor);
                if (self->actor.params == 100) {
                    FUN_0036fca8(&self->actor, play, 5, 10);
                }
                if (self->actor.params == 100) {
                    spawnNum = 2;
                } else {
                    spawnNum = 2;
                }
                for (i = 0; i < spawnNum; i++) {
                    Vec3f spawnOffset;
                    EnVbBall* newActor;
                    f32 xRotVel;

                    if (self->actor.params == 100) {
                        spawnOffset.x = Rand_CenteredFloat(13.0f);
                        spawnOffset.y = Rand_ZeroFloat(5.0f) + 6.0f;
                        spawnOffset.z = Rand_CenteredFloat(13.0f);
                    } else {
                        spawnOffset.x = Rand_CenteredFloat(10.0f);
                        spawnOffset.y = Rand_ZeroFloat(3.0f) + 4.0f;
                        spawnOffset.z = Rand_CenteredFloat(10.0f);
                    }
                    newActor = (EnVbBall*)Actor_SpawnAsChild(
                        &play->actorCtx, &self->actor, play, 0xAD, self->actor.world.pos.x + spawnOffset.x,
                        self->actor.world.pos.y + spawnOffset.y, self->actor.world.pos.z + spawnOffset.z, 0, 0,
                        self->actor.world.rot.z * 0.5f, self->actor.params + 1);
                    if (newActor != NULL) {
                        if ((i == 0) && (self->actor.params == 100)) {
                            Audio_PlaySoundGeneral(0x01000347, &newActor->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
                        }
                        newActor->actor.parent = self->actor.parent;
                        newActor->actor.velocity = spawnOffset;
                        newActor->yRotVel = 0.0f;
                        xRotVel = __sqrtf(SQ(spawnOffset.x) + SQ(spawnOffset.z));
                        newActor->xRotVel = 409.6f * xRotVel;
                        newActor->actor.shape.rot.y = Math_FAtan2F(spawnOffset.x, spawnOffset.z) * 10430.378f;
                        newActor->shadowOpacity = 255.0f;
                    }
                }
                for (i = 0; i < 6; i++) {
                    Vector3f debrisVel1(0.0f, 0.0f, 0.0f);
                    Vector3f debrisAcc1(0.0f, -1.0f, 0.0f);
                    Vec3f debrisPos1;

                    debrisVel1.x = Rand_CenteredFloat(25.0f);
                    debrisVel1.y = Rand_ZeroFloat(5.0f) + 8;
                    debrisVel1.z = Rand_CenteredFloat(25.0f);

                    debrisPos1.x = Rand_CenteredFloat(10.0f) + self->actor.world.pos.x;
                    debrisPos1.y = Rand_CenteredFloat(10.0f) + self->actor.world.pos.y;
                    debrisPos1.z = Rand_CenteredFloat(10.0f) + self->actor.world.pos.z;

                    FUN_00335814(EFFECTS(bossFd), &debrisPos1, (Vec3f*)&debrisVel1, (Vec3f*)&debrisAcc1,
                                 (s16)Rand_ZeroFloat(12.0f) + 15);
                }
                for (i = 0; i < 4; i++) {
                    Vector3f dustVel(0.0f, 0.0f, 0.0f);
                    Vector3f dustAcc(0.0f, 0.0f, 0.0f);
                    Vec3f dustPos;

                    dustVel.x = Rand_CenteredFloat(8.0f);
                    dustVel.y = Rand_ZeroFloat(1.0f);
                    dustVel.z = Rand_CenteredFloat(8.0f);

                    dustAcc.y = 1.0f / 2;

                    dustPos.x = Rand_CenteredFloat(30.0f) + self->actor.world.pos.x;
                    dustPos.y = Rand_CenteredFloat(30.0f) + self->actor.world.pos.y;
                    dustPos.z = Rand_CenteredFloat(30.0f) + self->actor.world.pos.z;

                    FUN_0036442c(bossFd, &dustPos, (Vec3f*)&dustVel, (Vec3f*)&dustAcc, Rand_ZeroFloat(200.0f) + 600.0f);
                }
            } else {
                for (i = 0; i < 2; i++) {
                    Vector3f debrisVel2(0.0f, 0.0f, 0.0f);
                    Vector3f debrisAcc2(0.0f, -1.0f, 0.0f);
                    Vec3f debrisPos2;

                    debrisVel2.x = Rand_CenteredFloat(10.0f);
                    debrisVel2.y = Rand_ZeroFloat(3.0f) + 3.0f;
                    debrisVel2.z = Rand_CenteredFloat(10.0f);

                    debrisPos2.x = Rand_CenteredFloat(5.0f) + self->actor.world.pos.x;
                    debrisPos2.y = Rand_CenteredFloat(5.0f) + self->actor.world.pos.y;
                    debrisPos2.z = Rand_CenteredFloat(5.0f) + self->actor.world.pos.z;

                    FUN_00335814(EFFECTS(bossFd), &debrisPos2, (Vec3f*)&debrisVel2, (Vec3f*)&debrisAcc2,
                                 (s16)Rand_ZeroFloat(12.0f) + 15);
                }
                Actor_Kill(&self->actor);
            }
        }
        if (self->collider.base.atFlags & AT_HIT) {
            Player* player = GET_PLAYER(play);

            self->collider.base.atFlags &= ~AT_HIT;
            Audio_PlayActorSound2(&player->actor, 0x0100003F);
        }
        Collider_UpdateCylinder(&self->actor, &self->collider);
        CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
    }
}
#endif

GLOBAL_ASM("asm/EnVbBall_Update.s")

#if 0 // draft EnVbBall_Draw: 3 insns short, 63 words off (ROM scales matrix with per-multiply vmov s1,s0 copies = MTX34_MakeScale/SetUnkAC class; needs mtx34_trans.hpp + model_manager.hpp includes)
#define VBM(s) ((SkeletonAnimationModel*)(s))

void Matrix_UpdatePosition(MTX34* dst, MTX34* src, Vec3f* translate);
void FUN_003688a8(void* a, s32 b, s32 c, void* d);

static inline void EnVbBall_Scale(MTX34* m, f32 x, f32 y, f32 z) {
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

void EnVbBall_Draw(Actor* thisx, PlayState* play) {
    EnVbBall* self = (EnVbBall*)thisx;
    MTX34 mtx;

    MTX34CopyAsm(&mtx, &thisx->modelMtx);
    if (self->actor.params >= 200) {
        if (self->model0 != NULL) {
            VBM(self->model0)->SetUnkAC(1);
            VBM(self->model0)->FUN_003721e0(&mtx);
            VBM(self->model0)->FUN_00372170(0);
        }
        return;
    }
    if (self->model1 != NULL) {
        VBM(self->model1)->SetUnkAC(1);
        VBM(self->model1)->FUN_003721e0(&mtx);
        VBM(self->model1)->FUN_00372170(0);
    }
    if (self->model2 != NULL) {
        AnimatedMaterial* mat;

        self->unk_218 += 0.1f;
        if (self->unk_218 > 1.0f) {
            self->unk_218 = 1.0f;
        }
        mat = VBM(self->model2)->unk_10;
        {
            f32 color[4] = { 0 };
            color[3] = 1.0f - self->shadowOpacity * (1.0f / 255.0f);
            FUN_003688a8(mat, 0, 4, color);
        }
        ((u8*)*(void**)((u8*)mat + 4))[0xe] = 1;
        {
            Vec3f t;
            t.x = self->actor.world.pos.x;
            t.y = 100.0f;
            t.z = self->actor.world.pos.z;
            MTX34_MakeTrans(&mtx, &t);
        }
        {
            f32 s = self->shadowSize * self->unk_218;
            EnVbBall_Scale(&mtx, s, 1.0f, s);
        }
        VBM(self->model2)->SetUnkAC(1);
        VBM(self->model2)->FUN_003721e0(&mtx);
        VBM(self->model2)->FUN_00372170(0);
    }
}
#endif

GLOBAL_ASM("asm/EnVbBall_Draw.s")
}
