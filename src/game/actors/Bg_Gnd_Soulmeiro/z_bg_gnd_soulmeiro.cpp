extern "C" {
#include "z_bg_gnd_soulmeiro.hpp"

#define FLAGS 0x80400000

#define PARAMS_GET_U(params, shift, count) ((((u16)(params)) >> (shift)) & ((1 << (count)) - 1))

void BgGndSoulmeiro_Init(Actor* actor, GameState* state);
void BgGndSoulmeiro_Destroy(Actor* actor, GameState* state);
void BgGndSoulmeiro_Update(Actor* actor, GameState* state);
void BgGndSoulmeiro_Draw(Actor* actor, GameState* state);

void FUN_003f5500(BgGndSoulmeiro* self, PlayState* play);
void FUN_0010fb78(BgGndSoulmeiro* self, PlayState* play);
void FUN_003f84d4(BgGndSoulmeiro* self, PlayState* play);

GLOBAL_ASM("data/z_Bg_Gnd_Soulmeiro.data.s")

ActorInit Bg_Gnd_Soulmeiro_InitVars = {
    ACTOR_BG_GND_SOULMEIRO,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_DEMO_KEKKAI,
    sizeof(BgGndSoulmeiro),
    (ActorFunc)BgGndSoulmeiro_Init,
    (ActorFunc)BgGndSoulmeiro_Destroy,
    (ActorFunc)BgGndSoulmeiro_Update,
    (ActorFunc)BgGndSoulmeiro_Draw,
};

void BgGndSoulmeiro_Init(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    BgGndSoulmeiro* self = (BgGndSoulmeiro*)actor;

    // sInitChain (0x00511080) and sCylinderInit (0x00511048) live in the data dump above
    Actor_ProcessInitChain(actor, (InitChainEntry*)0x00511080);
    self->actionFunc = NULL;

    switch (PARAMS_GET_U(actor->params, 0, 8)) {
        case 0:
            FUN_00372f38(actor, play, &self->skelAnimModel, 9, 0);
            Collider_InitCylinder(play, &self->collider);
            Collider_SetCylinder(play, &self->collider, actor, (ColliderCylinderInit*)0x00511048);
            self->actionFunc = FUN_003f5500;
            if (Flags_GetSwitch(play, PARAMS_GET_U(actor->params, 8, 6))) {
                Actor_Spawn(&play->actorCtx, play, ACTOR_MIR_RAY, actor->world.pos.x, actor->world.pos.y,
                            actor->world.pos.z, 0, 0, 0, 9, 1);
                actor->draw = NULL;
                Actor_Kill(actor);
                return;
            }
            actor->draw = BgGndSoulmeiro_Draw;
            break;
        case 1:
            FUN_00372f38(actor, play, &self->skelAnimModel, 1, 0);
            if (Flags_GetSwitch(play, PARAMS_GET_U(actor->params, 8, 6))) {
                actor->draw = BgGndSoulmeiro_Draw;
            } else {
                actor->draw = NULL;
            }
            self->actionFunc = FUN_0010fb78;
            break;
        case 2:
            FUN_00372f38(actor, play, &self->skelAnimModel, 2, 0);
            if (Flags_GetSwitch(play, PARAMS_GET_U(actor->params, 8, 6))) {
                actor->draw = BgGndSoulmeiro_Draw;
            } else {
                actor->draw = NULL;
            }
            self->actionFunc = FUN_0010fb78;
            break;
    }
}

void BgGndSoulmeiro_Destroy(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    BgGndSoulmeiro* self = (BgGndSoulmeiro*)actor;

    if (PARAMS_GET_U(actor->params, 0, 8) == 0) {
        Collider_DestroyCylinder(play, &self->collider);
    }
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void FUN_003f5500(BgGndSoulmeiro* self, PlayState* play) {
    Actor* actor = &self->actor;

    if (!Flags_GetSwitch(play, PARAMS_GET_U(actor->params, 8, 6))) {
        actor->draw = BgGndSoulmeiro_Draw;
        if (self->collider.base.acFlags & AC_HIT) {
            FUN_00372244(&play->unk_5FCC, 0x1E, 0x01000487); // NA_SE_SY_CORRECT_CHIME
            self->timer = 60;
            self->actionFunc = FUN_003f84d4;
        } else {
            Collider_UpdateCylinder(actor, &self->collider);
            CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
        }
    }
}

void FUN_0010fb78(BgGndSoulmeiro* self, PlayState* play) {
    Actor* thisx = &self->actor;

    thisx->draw = Flags_GetSwitch(play, PARAMS_GET_U(thisx->params, 8, 6)) ? BgGndSoulmeiro_Draw : NULL;
}

void BgGndSoulmeiro_Update(Actor* actor, GameState* state) {
    BgGndSoulmeiro* self = (BgGndSoulmeiro*)actor;
    PlayState* play = (PlayState*)state;

    if (self->actionFunc != NULL) {
        self->actionFunc(self, play);
    }
}

// FUN_003f84d4 (web break countdown, N64 func_8087AF38). Function-local static: guard 0x00511044 (TU .data, cut
// from the data chunk), object 0x00569950 pinned in data/oot3d.ld (scripts/pin_static.py).
void EffectSsDeadDb_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep,
                          s16 primR, s16 primG, s16 primB, s16 primA, s16 envR, s16 envG, s16 envB, s16 unused,
                          s32 arg14, s16 playSfx);
// Vec3f with a Set()-based ctor: reproduces the ROM's register choice in the guarded zero init.
struct BgGndSoulmeiroVec3f {
    f32 x, y, z;
    BgGndSoulmeiroVec3f(f32 b, f32 c, f32 a) {
        Set(a, b, c);
    }
    void Set(f32 c, f32 a, f32 b) {
        x = a;
        y = b;
        z = c;
    }
};
void FUN_003f84d4(BgGndSoulmeiro* self, PlayState* play) {
    static BgGndSoulmeiroVec3f zeroVec(0.0f, 0.0f, 0.0f);
    Vec3f vecA;
    Vec3f vecB;
    Actor* thisx = &self->actor;

    if (self->timer != 0) {
        self->timer--;
        if (self->timer == 30) {
            Flags_SetSwitch(play, PARAMS_GET_U(thisx->params, 8, 6));
            self->actor.draw = NULL;
        }
    }

    if (self->timer == 0) {
        Flags_SetSwitch(play, PARAMS_GET_U(thisx->params, 8, 6));
        Actor_Kill(thisx);
        Actor_Spawn(&play->actorCtx, play, ACTOR_MIR_RAY, thisx->world.pos.x, thisx->world.pos.y, thisx->world.pos.z, 0,
                    0, 0, 9, 1);
    } else if ((self->timer % 6) == 0) {
        s16 temp_2 = Rand_ZeroOne() * (10922.0f);
        s32 i;
        s16 temp_1;
        f32 temp_3;
        f32 temp_4;
        f32 distXZ;

        vecA.y = 0.0f;
        vecB.y = thisx->world.pos.y;

        for (i = 0; i < 6; i++) {
            temp_1 = Rand_CenteredFloat(0x2800) + temp_2;
            temp_3 = Math_SinS(temp_1);
            temp_4 = Math_CosS(temp_1);

            vecB.x = thisx->world.pos.x + (120.0f * temp_3);
            vecB.z = thisx->world.pos.z + (120.0f * temp_4);
            distXZ = Math_Vec3f_DistXZ(&thisx->home.pos, &vecB) * (1.0f / 120.0f);
            if (distXZ < 0.7f) {
                temp_3 = Math_SinS(temp_1 + 0x8000);
                temp_4 = Math_CosS(temp_1 + 0x8000);
                vecB.x = thisx->world.pos.x + (120.0f * temp_3);
                vecB.z = thisx->world.pos.z + (120.0f * temp_4);
                distXZ = Math_Vec3f_DistXZ(&thisx->home.pos, &vecB) * (1.0f / 120.0f);
            }

            vecA.x = 4.0f * temp_3 * distXZ;
            vecA.y = 0.0f;
            vecA.z = 4.0f * temp_4 * distXZ;
            EffectSsDeadDb_Spawn(play, &thisx->home.pos, &vecA, (Vec3f*)&zeroVec, 60, 6, 255, 255, 150, 170, 255, 0, 0, 1, 16,
                                 true);
            temp_2 += 0x2AAA;
        }
    }
}

void BgGndSoulmeiro_Draw(Actor* actor, GameState* state) {
    BgGndSoulmeiro* self = (BgGndSoulmeiro*)actor;
    MTX34 mtx;

    MTX34CopyAsm(&mtx, &actor->modelMtx);

    switch (PARAMS_GET_U(actor->params, 0, 8)) {
        case 0:
            if (self->skelAnimModel != NULL) {
                self->skelAnimModel->SetUnkAC(1);
                self->skelAnimModel->FUN_003721e0(&mtx);
                self->skelAnimModel->FUN_00372170(0);
            }
            break;
        case 1:
            if (self->skelAnimModel != NULL) {
                self->skelAnimModel->SetUnkAC(1);
                self->skelAnimModel->FUN_003721e0(&mtx);
                self->skelAnimModel->FUN_00372170(0);
            }
            break;
        case 2:
            if (self->skelAnimModel != NULL) {
                self->skelAnimModel->SetUnkAC(1);
                self->skelAnimModel->FUN_003721e0(&mtx);
                self->skelAnimModel->FUN_00372170(0);
            }
            break;
    }
}
}
