extern "C" {
#include "z_en_attack_niw.hpp"
#include "regs.hpp"

#define FLAGS 0x00000010

#define R_UPDATE_RATE SREG(30)
#define FRAMES(n) ((n) * 3.0f / R_UPDATE_RATE + 0.5f)
#define ANGLE_UNIT (M_PI / 32768.0f)

void EnAttackNiw_Init(Actor* actor, GameState* state);
void EnAttackNiw_Destroy(Actor* actor, GameState* state);
void EnAttackNiw_Update(Actor* actor, PlayState* play);
void EnAttackNiw_Draw(Actor* actor, GameState* state);

void* ZAR_GetCMBByIndex(ZARInfo* zar, s32 index);
void FUN_00353e78(ZARInfo* zar, PlayState* play, SkelAnime* skel, void* cmb, void* p, s32 b, void* jointTable,
                  void* morphTable, s32 limbCount);
void FUN_00205a40(EnAttackNiw* self, PlayState* play);
extern __pure float _fabsf(float);
#define fabsf _fabsf
void FUN_00363604(EnAttackNiw* self, PlayState* play, s16 arg);
void FUN_00206b30(EnAttackNiw* self, PlayState* play);
void FUN_0033bd9c(Actor* actor);
void FUN_00368fc0(PlayState* play, Actor* actor, f32 speed, s16 yaw, f32 unk, s32 unk2);
void EffectSsGSplash_Spawn(PlayState* play, Vec3f* pos, Vec3f* a, Vec3f* b, s16 c, s16 d);
void FUN_001255e4(EnAttackNiw* self, PlayState* play);
void FUN_003625f8(MTX34* out, Vec3f* rot, f32 scale);
void FUN_0032e438(MTX34* out, Vec3f* rot);
void FUN_0036c174(MTX34* dst, MTX34* a, MTX34* b);
s32 FUN_00389c20(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg);

GLOBAL_ASM("data/z_En_Attack_Niw.data.s")

ActorInit En_Attack_Niw_InitVars = {
    ACTOR_EN_ATTACK_NIW,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_NIW,
    sizeof(EnAttackNiw),
    (ActorFunc)EnAttackNiw_Init,
    (ActorFunc)EnAttackNiw_Destroy,
    (ActorFunc)EnAttackNiw_Update,
    (ActorFunc)EnAttackNiw_Draw,
};

void EnAttackNiw_Init(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnAttackNiw* self = (EnAttackNiw*)actor;
    ZARInfo* zar;
    void* cmb;

    Actor_ProcessInitChain(&self->actor, (InitChainEntry*)0x005209a8);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 25.0f);
    zar = &play->GetObjectStatus(self->actor.objBankIndex)->zarInfo;
    cmb = ZAR_GetCMBByIndex(zar, 0);
    FUN_00353e78(zar, play, (SkelAnime*)((u8*)self + 0x1A4), cmb, self->actor.unk_178, 0, self->jointTable,
                 self->morphTable, 9);
    if (self->actor.params < 0) {
        self->actor.params = 0;
    }
    Actor_SetScale(&self->actor, 0.01f);
    self->actor.gravity = 0.0f;
    self->unk_298.x = Rand_CenteredFloat(100.0f);
    self->unk_298.y = Rand_CenteredFloat(10.0f);
    self->unk_298.z = Rand_CenteredFloat(100.0f);
    Actor_SetScale(&self->actor, 0.01f);
    self->actor.flags &= ~1;
    self->actor.shape.rot.y = self->actor.world.rot.y = (Rand_ZeroOne() - 0.5f) * 60000.0f;
    self->actionFunc = FUN_00205a40;
}

void EnAttackNiw_Destroy(Actor* actor, GameState* state) {
    EnAttackNiw* self = (EnAttackNiw*)actor;
    Actor* parent = self->actor.parent;

    if (parent != NULL) {
        if (parent->update != NULL) {
            s16 timer;
            u8* base = (u8*)parent + 0x600;
            timer = *(s16*)(base + 0x16);
            if (timer > 0) {
                *(s16*)(base + 0x16) = timer - 1;
            }
        }
    }
    SkelAnime_Free2((SkelAnime*)((u8*)self + 0x1A4));
}

// FUN_00363604: bytes match except VFP constant registers (ROM: 0.0=s16, 7000=s17, -10000=s18, 0.5=s19, 9=s20; ours 7000=s16, -10000=s17, 0.5=s18, 0.0=s19). See STATUS.md.
#if 0
void FUN_00363604(EnAttackNiw* self, PlayState* play, s16 arg2) {
    if (self->unk_254 == 0) {
        if (arg2 == 0) {
            self->unk_264 = 0.0f;
        } else {
            self->unk_264 = -10000.0f;
        }
        self->unk_28E++;
        self->unk_254 = FRAMES(3);
        if ((self->unk_28E & 1) == 0) {
            self->unk_264 = 0.0f;
            if (arg2 == 0) {
                self->unk_254 = FRAMES((s16)Rand_ZeroFloat(30.0f));
            }
        }
    }

    if (self->unk_258_ == 0) {
        self->unk_292++;
        self->unk_292 &= 1;
        switch (arg2) {
            case 0:
                self->unk_26C = 0.0f;
                self->unk_268 = 0.0f;
                break;
            case 1:
                self->unk_258_ = FRAMES(3);
                self->unk_268 = self->unk_26C = 7000.0f;
                if (self->unk_292 == 0) {
                    self->unk_26C = 0.0f;
                    self->unk_268 = 0.0f;
                }
                break;
            case 2:
                self->unk_258_ = FRAMES(2);
                self->unk_268 = self->unk_26C = -10000.0f;
                self->unk_280 = self->unk_278 = 25000.0f;
                self->unk_284 = self->unk_27C = 6000.0f;
                if (self->unk_292 == 0) {
                    self->unk_278 = 8000.0f;
                    self->unk_280 = 8000.0f;
                }
                break;
            case 3:
                self->unk_258_ = FRAMES(2);
                self->unk_278 = 10000.0f;
                self->unk_280 = 10000.0f;
                if (self->unk_292 == 0) {
                    self->unk_278 = 3000.0f;
                    self->unk_280 = 3000.0f;
                }
                break;
            case 4:
                self->unk_254 = self->unk_256 = FRAMES(5);
                break;
            case 5:
                self->unk_258_ = FRAMES(5);
                self->unk_278 = 14000.0f;
                self->unk_280 = 14000.0f;
                if (self->unk_292 == 0) {
                    self->unk_278 = 10000.0f;
                    self->unk_280 = 10000.0f;
                }
                break;
        }
    }

    if (self->unk_2C0 != self->unk_288) {
        Math_ApproachF(&self->unk_2C0, self->unk_288, 0.5f, 4000.0f);
    }
    if (self->unk_2BC != self->unk_264) {
        Math_ApproachF(&self->unk_2BC, self->unk_264, 0.5f, 4000.0f);
    }
    if (self->unk_2A4.x != self->unk_26C) {
        Math_ApproachF(&self->unk_2A4.x, self->unk_26C, 0.8f, 7000.0f);
    }
    if (self->unk_2A4.y != self->unk_280) {
        Math_ApproachF(&self->unk_2A4.y, self->unk_280, 0.8f, 7000.0f);
    }
    if (self->unk_2A4.z != self->unk_284) {
        Math_ApproachF(&self->unk_2A4.z, self->unk_284, 0.8f, 7000.0f);
    }
    if (self->unk_2B0.x != self->unk_268) {
        Math_ApproachF(&self->unk_2B0.x, self->unk_268, 0.8f, 7000.0f);
    }
    if (self->unk_2B0.y != self->unk_278) {
        Math_ApproachF(&self->unk_2B0.y, self->unk_278, 0.8f, 7000.0f);
    }
    if (self->unk_2B0.z != self->unk_27C) {
        Math_ApproachF(&self->unk_2B0.z, self->unk_27C, 0.8f, 7000.0f);
    }
}
#endif

// FUN_00205a40 draft (view helper + body): not matched, see STATUS.md.
#if 0
static void AttackNiw_ViewDir(View* view, Vec3f* out) {
    f32 dx = view->lookAt.x - view->eye.x;
    f32 dy = view->lookAt.y - view->eye.y;
    f32 dz = view->lookAt.z - view->eye.z;
    f32 inv = 1.0f / sqrtf(dx * dx + dy * dy + dz * dz);
    out->x = dx * inv;
    out->y = dy * inv;
    out->z = dz * inv;
}
#endif

#if 0
void FUN_00205a40(EnAttackNiw* self, PlayState* play) {
    s16 sp4E;
    s16 sp4C;
    f32 tmpf1;
    f32 tmpf2;
    f32 tmpf3;
    Vec3f sp34;
    f32 dx;
    f32 dy;
    f32 dz;
    Vec3f dir;
    f32 atX;
    f32 atY;
    f32 atZ;

    self->actor.speedXZ = 10.0f;

    AttackNiw_ViewDir(&play->view, &dir);
    dx = dir.x * 160.0f;
    dy = dir.y * 160.0f;
    dz = dir.z * 160.0f;
    atX = play->view.eye.x + dx;
    atY = play->view.eye.y + dy;
    atZ = play->view.eye.z + dz;

    tmpf1 = (self->unk_298.x + atX) - play->view.eye.x;
    tmpf2 = (self->unk_298.y + atY) - play->view.eye.y;
    tmpf3 = (self->unk_298.z + atZ) - play->view.eye.z;

    sp34.x = atX + tmpf1;
    sp34.y = atY + tmpf2;
    sp34.z = atZ + tmpf3;

    self->unk_2D4 = Math_Atan2S(sp34.z - self->actor.world.pos.z, sp34.x - self->actor.world.pos.x);
    {
        f32 ddx = sp34.x - self->actor.world.pos.x;
        f32 ddz = sp34.z - self->actor.world.pos.z;
        self->unk_2D0 = Math_Atan2S(sqrtf(ddx * ddx + ddz * ddz), self->actor.world.pos.y - sp34.y) * -1.0f;
    }

    Math_SmoothStepToS(&self->actor.world.rot.y, self->unk_2D4, 5, self->unk_2DC, 0);
    Math_SmoothStepToS(&self->actor.world.rot.x, self->unk_2D0, 5, self->unk_2DC, 0);
    Math_ApproachF(&self->unk_2DC, 5000.0f, 1.0f, 100.0f);

    Actor_SetFocus(&self->actor, self->unk_2E4);
    Actor_GetScreenPos(play, &self->actor, &sp4E, &sp4C);

    if (self->actor.bgCheckFlags & 8) {
        self->unk_2D4 = self->actor.yawTowardsPlayer;
        self->unk_2D0 = self->actor.world.rot.x - 3000.0f;
        self->unk_2DC = 0.0f;
        self->unk_284 = 0.0f;
        self->unk_27C = 0.0f;
        self->unk_254 = self->unk_256 = self->unk_258_ = self->unk_25A = 0;
        self->unk_25C = FRAMES(100);
        self->actor.gravity = -0.2f;
        self->unk_2E0 = 5.0f;
        self->unk_288 = 0.0f;
        self->actionFunc = FUN_00206b30;
    } else if (((self->actor.unk_EC.z > 0.0f) && (fabsf(sp34.x - self->actor.world.pos.x) < 50.0f) &&
                (fabsf(sp34.y - self->actor.world.pos.y) < 50.0f) &&
                (fabsf(sp34.z - self->actor.world.pos.z) < 50.0f)) ||
               (self->actor.bgCheckFlags & 1)) {
        self->unk_2D4 = self->actor.yawTowardsPlayer;
        self->unk_2D0 = self->actor.world.rot.x - 2000.0f;
        self->unk_2DC = 0.0f;
        self->unk_27C = 0.0f;
        self->unk_284 = 0.0f;
        self->unk_254 = self->unk_256 = self->unk_258_ = self->unk_25A = 0;
        self->actor.gravity = -0.2f;
        self->unk_2E0 = 5.0f;
        self->unk_288 = 0.0f;
        self->actionFunc = FUN_00206b30;
    } else {
        self->unk_254 = FRAMES(10);
        self->unk_264 = -10000.0f;
        self->unk_288 = -3000.0f;
        FUN_00363604(self, play, 2);
    }
}
#endif

static s32 EnAttackNiw_IsOnScreen(EnAttackNiw* self, PlayState* play) {
    s16 screenX;
    s16 screenY;

    Actor_SetFocus(&self->actor, self->unk_2E4);
    Actor_GetScreenPos(play, &self->actor, &screenX, &screenY);
    if ((*(f32*)((u8*)&self->actor + 0xF4) < -1020.0f) || ((u32)((s32)screenX + 1000) > 2320) ||
        (screenY < -1000) || (screenY > 1240)) {
        return 0;
    }
    return 1;
}

void FUN_00206b30(EnAttackNiw* self, PlayState* play) {
    if (!EnAttackNiw_IsOnScreen(self, play)) {
        Actor_Kill(&self->actor);
        return;
    }

    if (self->actor.bgCheckFlags & 1) {
        if (self->unk_25A == 0) {
            self->unk_25A = FRAMES(3);
            self->actor.velocity.y = 3.5f;
        }
        if (self->actor.gravity != -2.0f) {
            self->unk_26C = self->unk_268 = self->unk_284 = self->unk_27C = 0.0f;
            self->unk_280 = self->unk_278 = 14000.0f;
            self->unk_2D0 = 0.0f;
            self->unk_2D4 = self->actor.yawTowardsPlayer;
            self->actor.gravity = -2.0f;
            self->unk_262 = FRAMES(50);
            self->unk_25C = FRAMES(100);
        }
    }
    if (self->unk_25C == (s32)FRAMES(50)) {
        self->unk_2D4 = Rand_CenteredFloat(200.0f) + self->actor.yawTowardsPlayer;
    }
    Math_SmoothStepToS(&self->actor.world.rot.y, self->unk_2D4, 2, self->unk_2DC, 0);
    Math_SmoothStepToS(&self->actor.world.rot.x, self->unk_2D0, 2, self->unk_2DC, 0);
    Math_ApproachF(&self->unk_2DC, 10000.0f, 1.0f, 1000.0f);
    Math_ApproachF(&self->actor.speedXZ, self->unk_2E0, 0.9f, 1.0f);
    if ((self->actor.gravity == -2.0f) && (self->unk_262 == 0) &&
        ((self->actor.bgCheckFlags & 8) || (self->unk_25C == 0))) {
        self->unk_2E0 = 0.0f;
        self->actor.gravity = 0.0f;
        self->unk_2DC = 0.0f;
        self->unk_2D0 = self->actor.world.rot.x - 5000.0f;
        self->actionFunc = FUN_001255e4;
    } else if (self->actor.bgCheckFlags & 1) {
        FUN_00363604(self, play, 5);
    } else {
        FUN_00363604(self, play, 2);
    }
}

void FUN_001255e4(EnAttackNiw* self, PlayState* play) {
    if (!EnAttackNiw_IsOnScreen(self, play)) {
        Actor_Kill(&self->actor);
        return;
    }
    Math_SmoothStepToS(&self->actor.world.rot.x, self->unk_2D0, 5, self->unk_2DC, 0);
    Math_ApproachF(&self->unk_2DC, 5000.0f, 1.0f, 100.0f);
    Math_ApproachF(&self->actor.velocity.y, 5.0f, 0.3f, 1.0f);
    FUN_00363604(self, play, 2);
}

void EnAttackNiw_Update(Actor* thisx, PlayState* play) {
    f32 tmpf1;
    EnAttackNiw* self = (EnAttackNiw*)thisx;
    Player* player = GET_PLAYER(play);

    self->unk_28C++;
    if (self->unk_254 != 0) {
        self->unk_254--;
    }
    if (self->unk_258_ != 0) {
        self->unk_258_--;
    }
    if (self->unk_25A != 0) {
        self->unk_25A--;
    }
    if (self->unk_25E != 0) {
        self->unk_25E--;
    }
    if (self->unk_260 != 0) {
        self->unk_260--;
    }
    if (self->unk_25C != 0) {
        self->unk_25C--;
    }
    if (self->unk_262 != 0) {
        self->unk_262--;
    }

    self->actor.shape.shadowScale = 15.0f;
    self->actor.shape.rot = self->actor.world.rot;

    { PlayState* play2 = play; self->actionFunc(self, play2); }

    Actor_UpdateBgCheckInfo(play, &self->actor, 20.0f, 20.0f, 60.0f, 0x1D);

    if (self->actionFunc == FUN_00205a40) {
        FUN_0033bd9c(thisx);
    } else {
        Actor_MoveForward(thisx);
    }

    if (self->actor.floorHeight <= -32000.0f) {
        Actor_Kill(&self->actor);
        return;
    }

    if ((self->actor.bgCheckFlags & 0x20) && (self->actionFunc != FUN_001255e4)) {
        Vec3f sp30;

        Math_Vec3f_Copy(&sp30, &self->actor.world.pos);
        sp30.y += self->actor.yDistToWater;
        EffectSsGSplash_Spawn(play, &sp30, NULL, NULL, 0, 0x190);
        self->unk_2DC = self->actor.gravity = self->unk_2E0 = 0.0f;
        self->unk_2D0 = self->actor.world.rot.x - 5000.0f;
        self->actionFunc = FUN_001255e4;
        return;
    }

    tmpf1 = 20.0f;
    if (self->actor.xyzDistToPlayerSq < SQ(tmpf1)) {
        EnAttackNiw* cucco = (EnAttackNiw*)self->actor.parent;
        if ((self->actor.parent->update != NULL) && (self->actor.parent != NULL) && (cucco != NULL) &&
            (*(u16*)((u8*)cucco + 0x5EA) == 0) && (*((u8*)player + 0x2488) == 0)) {
            FUN_00368fc0(play, &self->actor, 2.0f, self->actor.world.rot.y, 0.0f, 0x10);
            *(u16*)((u8*)cucco + 0x5EA) = 0x46;
        }
    }
    if (self->unk_25E == 0) {
        self->unk_25E = FRAMES(30);
        Audio_PlayActorSound2(&self->actor, 0x01000169);
    }
    if (self->unk_260 == 0) {
        self->unk_260 = FRAMES(7);
        Audio_PlayActorSound2(&self->actor, 0x0100036E);
    }
}


static inline void EnAttackNiw_SetAxis(Vec3f* v, f32 x, f32 y, f32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

s32 FUN_00389c20(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg) {
    EnAttackNiw* self = (EnAttackNiw*)arg;
    MTX34 rotMtx;
    Vec3f rot;

    if (limbIndex == 7) {
        EnAttackNiw_SetAxis(&rot, 0.0f, 0.0f, -1.0f);
        FUN_003625f8(&rotMtx, &rot, self->unk_2BC * ANGLE_UNIT);
        FUN_0036c174(mtx, mtx, &rotMtx);
    } else if (limbIndex == 8) {
        {
            f32 one = 1.0f;
            EnAttackNiw_SetAxis(&rot, 0.0f, one, 0.0f);
        }
        FUN_003625f8(&rotMtx, &rot, self->unk_2C0 * ANGLE_UNIT);
        FUN_0036c174(mtx, mtx, &rotMtx);
    } else if (limbIndex == 4) {
        EnAttackNiw_SetAxis(&rot, self->unk_2B0.z * -ANGLE_UNIT, self->unk_2B0.y * -ANGLE_UNIT,
                            self->unk_2B0.x * ANGLE_UNIT);
        FUN_0032e438(&rotMtx, &rot);
        FUN_0036c174(mtx, mtx, &rotMtx);
    } else if (limbIndex == 6) {
        EnAttackNiw_SetAxis(&rot, self->unk_2A4.z * ANGLE_UNIT, self->unk_2A4.y * ANGLE_UNIT,
                            self->unk_2A4.x * ANGLE_UNIT);
        FUN_0032e438(&rotMtx, &rot);
        FUN_0036c174(mtx, mtx, &rotMtx);
    }
    return 0;
}

void EnAttackNiw_Draw(Actor* actor, GameState* state) {
    EnAttackNiw* self = (EnAttackNiw*)actor;

    SkelAnime_DrawOpa((SkelAnime*)((u8*)self + 0x1A4), &self->actor.modelMtx, (OverrideLimbDrawOpa)FUN_00389c20,
                      NULL, actor, 0);
}
}
