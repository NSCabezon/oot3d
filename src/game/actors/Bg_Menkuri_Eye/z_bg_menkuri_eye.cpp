extern "C" {
#include "z_bg_menkuri_eye.hpp"

#define FLAGS 0x80400020

void BgMenkuriEye_Init(Actor* actor, GameState* state);
void BgMenkuriEye_Destroy(Actor* actor, GameState* state);
void BgMenkuriEye_Update(Actor* actor, GameState* state);
void BgMenkuriEye_Draw(Actor* actor, GameState* state);

void Collider_InitJntSph(PlayState* play, BgMenkuriEyeColliderJntSph* collider);
void Collider_SetJntSph(PlayState* play, BgMenkuriEyeColliderJntSph* collider, Actor* actor, void* init,
                        BgMenkuriEyeColliderElement* elements);
void Collider_DestroyJntSph(PlayState* play, BgMenkuriEyeColliderJntSph* collider);

// Data lives in the original .data at 0x00512D58 (data/z_Bg_Menkuri_Eye.data.s); named by EQU symbols in data_syms.s.
// The init chain (0x512D88) and the shot-eye counter (0x512D8C) sit in one block, so they are one struct here.
typedef struct {
    /* 0x00 */ u32 initChainWord;
    /* 0x04 */ s32 numEyesShot;
} BgMenkuriEyeData;

extern BgMenkuriEyeData sBgMenkuriEyeData;
extern u8 sBgMenkuriEyeJntSphInit[];
#define sBgMenkuriEyeInitChain ((InitChainEntry*)&sBgMenkuriEyeData)
#define sBgMenkuriEyeNumEyesShot sBgMenkuriEyeData.numEyesShot

GLOBAL_ASM("data/z_Bg_Menkuri_Eye.data.s")

GLOBAL_ASM("../src/game/actors/Bg_Menkuri_Eye/data_syms.s")

ActorInit Bg_Menkuri_Eye_InitVars = {
    ACTOR_BG_MENKURI_EYE,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_MENKURI_OBJECTS,
    sizeof(BgMenkuriEye),
    (ActorFunc)BgMenkuriEye_Init,
    (ActorFunc)BgMenkuriEye_Destroy,
    (ActorFunc)BgMenkuriEye_Update,
    (ActorFunc)BgMenkuriEye_Draw,
};

void BgMenkuriEye_Init(Actor* actor, GameState* state) {
    BgMenkuriEye* self = (BgMenkuriEye*)actor;
    PlayState* play = (PlayState*)state;

    Actor_ProcessInitChain(actor, sBgMenkuriEyeInitChain);
    FUN_00372f38(actor, play, &self->skelAnimModel, 3, 0);
    Collider_InitJntSph(play, &self->collider);
    Collider_SetJntSph(play, &self->collider, actor, sBgMenkuriEyeJntSphInit, self->colliderElements);
    self->collider.elements->worldSphereCenter = actor->world.pos;
    self->collider.elements->worldSphereRadius = self->collider.elements->modelSphereRadius;
    if (!Flags_GetSwitch(play, actor->params)) {
        sBgMenkuriEyeNumEyesShot = 0;
    }
    self->framesUntilDisable = -1;
    actor->unk_19B = 1;
}

void BgMenkuriEye_Destroy(Actor* actor, GameState* state) {
    BgMenkuriEye* self = (BgMenkuriEye*)actor;
    PlayState* play = (PlayState*)state;

    Collider_DestroyJntSph(play, &self->collider);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgMenkuriEye_Update(Actor* actor, GameState* state) {
    BgMenkuriEye* self = (BgMenkuriEye*)actor;
    PlayState* play = (PlayState*)state;
    s16 yawDiff;

    if (!Flags_GetSwitch(play, actor->params)) {
        if (self->framesUntilDisable != -1) {
            if (self->framesUntilDisable != 0) {
                self->framesUntilDisable--;
            }
            if (self->framesUntilDisable == 0) {
                self->framesUntilDisable = -1;
                sBgMenkuriEyeNumEyesShot--;
            }
        }
    }
    if ((self->collider.base.acFlags & AC_HIT) &&
        (yawDiff = self->collider.base.ac->world.rot.y - actor->shape.rot.y, ABS(yawDiff) > 0x5000)) {
        self->collider.base.acFlags &= ~AC_HIT;
        if (self->framesUntilDisable == -1) {
            Audio_PlayActorSound2(actor, 0x010002B7); // NA_SE_EN_AMOS_DAMAGE
            sBgMenkuriEyeNumEyesShot++;
            sBgMenkuriEyeNumEyesShot = CLAMP_MAX(sBgMenkuriEyeNumEyesShot, 4);
        }
        self->framesUntilDisable = 624; // 416 at N64 rate, scaled for the 3DS update rate
        if (!Flags_GetSwitch(play, actor->params) && sBgMenkuriEyeNumEyesShot == 4) {
            Flags_SetSwitch(play, actor->params);
            FUN_00372244(&play->unk_5FCC, 0x1E, 0x01000487); // NA_SE_SY_CORRECT_CHIME
        }
    }
    if (self->framesUntilDisable == -1) {
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
    }
    Actor_SetFocus(actor, 0.0f);
}

// Draw draft (structural, ~199/218 words off): ROM inlines Matrix translate+RotateZYX+Scale with the 3x4 matrix at sp+0xc
// and a Vec3f temp at sp (address-taken), SetUnkAC 1 kept in r5; unknown header helpers, see CLAUDE.md SetUnkAC class.
#if 0
f32 __hardfp_sinf(f32);
f32 __hardfp_cosf(f32);
#define ROTZ(r) a = mtx.data[r][0]; b = mtx.data[r][1]; mtx.data[r][0] = a * cs + b * sn; mtx.data[r][1] = b * cs - a * sn;
#define ROTY(r) a = mtx.data[r][0]; b = mtx.data[r][2]; mtx.data[r][0] = a * cs - b * sn; mtx.data[r][2] = a * sn + b * cs;
#define ROTX(r) a = mtx.data[r][1]; b = mtx.data[r][2]; mtx.data[r][1] = a * cs + b * sn; mtx.data[r][2] = b * cs - a * sn;

void BgMenkuriEye_Draw(Actor* actor, GameState* state) {
    BgMenkuriEye* self = (BgMenkuriEye*)actor;
    PlayState* play = (PlayState*)state;
    MTX34 mtx;
    s16 rx, ry, rz;
    f32 ang, sn, cs, a, b;

    MTX34CopyAsm(&mtx, &actor->modelMtx);
    if (!Flags_GetSwitch(play, actor->params) && self->framesUntilDisable == -1) {
        FUN_003695cc(self->skelAnimModel, 0, 4, 0, 1.0f, 1.0f, 1.0f, 1.0f);
    } else {
        FUN_003695cc(self->skelAnimModel, 0, 4, 0, 0.78431374f, 0.0f, 0.0f, 1.0f);
    }
    Vector3f pos(actor->world.pos.x, actor->world.pos.y, actor->world.pos.z);
    mtx.data[0][3] = pos.x; mtx.data[1][3] = pos.y; mtx.data[2][3] = pos.z;
    mtx.data[0][0] = 1.0f; mtx.data[0][1] = 0.0f; mtx.data[0][2] = 0.0f;
    mtx.data[1][0] = 0.0f; mtx.data[1][1] = 1.0f; mtx.data[1][2] = 0.0f;
    mtx.data[2][0] = 0.0f; mtx.data[2][1] = 0.0f; mtx.data[2][2] = 1.0f;
    rx = actor->world.rot.x;
    ry = actor->world.rot.y;
    rz = actor->world.rot.z;
    ang = rz * 9.58738E-5f;
    if (ang != 0.0f) {
        sn = __hardfp_sinf(ang);
        cs = __hardfp_cosf(ang);
        ROTZ(0) ROTZ(1) ROTZ(2)
    }
    if (ry != 0) {
        ang = ry * 9.58738E-5f;
        if (ang != 0.0f) {
            sn = __hardfp_sinf(ang);
            cs = __hardfp_cosf(ang);
            ROTY(0) ROTY(1) ROTY(2)
        }
    }
    if (rx != 0) {
        ang = rx * 9.58738E-5f;
        if (ang != 0.0f) {
            sn = __hardfp_sinf(ang);
            cs = __hardfp_cosf(ang);
            ROTX(0) ROTX(1) ROTX(2)
        }
    }
    mtx.data[0][0] *= actor->scale.x; mtx.data[1][0] *= actor->scale.x; mtx.data[2][0] *= actor->scale.x;
    mtx.data[0][1] *= actor->scale.y; mtx.data[1][1] *= actor->scale.y; mtx.data[2][1] *= actor->scale.y;
    mtx.data[0][2] *= actor->scale.z; mtx.data[1][2] *= actor->scale.z; mtx.data[2][2] *= actor->scale.z;
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&mtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}
#endif

GLOBAL_ASM("asm/BgMenkuriEye_Draw.s")
}
