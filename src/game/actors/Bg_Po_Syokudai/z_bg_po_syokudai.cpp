extern "C" {
#include "z_bg_po_syokudai.hpp"
#include <math.h>

#define FLAGS 0x80000000

void BgPoSyokudai_Init(Actor* actor, GameState* state);
void BgPoSyokudai_Destroy(Actor* actor, GameState* state);
void BgPoSyokudai_Update(Actor* actor, GameState* state);
void BgPoSyokudai_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Bg_Po_Syokudai.data.s")

GLOBAL_ASM("../src/game/actors/Bg_Po_Syokudai/data_syms.s")
extern InitChainEntry BgPoSyokudai_sInitChain[];
extern ColliderCylinderInit BgPoSyokudai_sCylinderInit;
extern u32 BgPoSyokudai_sFlameModelIdx[];
extern u8 BgPoSyokudai_sPrimColors[];

ActorInit Bg_Po_Syokudai_InitVars = {
    ACTOR_BG_PO_SYOKUDAI,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_SYOKUDAI,
    sizeof(BgPoSyokudai),
    (ActorFunc)BgPoSyokudai_Init,
    (ActorFunc)BgPoSyokudai_Destroy,
    (ActorFunc)BgPoSyokudai_Update,
    (ActorFunc)BgPoSyokudai_Draw,
};

void* LightContext_InsertLight(PlayState* play, void* lightCtx, void* info);
void Lights_PointGlowSetInfo(void* light, f32 x, f32 y, f32 z, u8 r, u8 g, u8 b, s16 radius, s32 pad);
void FUN_00350508(void* obj, PlayState* play, s32 arg2, s32 arg3);

void BgPoSyokudai_Init(Actor* actor, GameState* state) {
    BgPoSyokudai* self = (BgPoSyokudai*)actor;
    PlayState* play = (PlayState*)state;

    Actor_ProcessInitChain(actor, BgPoSyokudai_sInitChain);
    self->flameColor = actor->params >> 8;
    actor->params &= 0x3F;
    actor->colChkInfo.mass = 0xFF;
    FUN_00372f38(actor, play, (SkeletonAnimationModel**)((u8*)self + 0x21C), 2, 0);
    FUN_00350508((u8*)self + 0x220, play, 0, BgPoSyokudai_sFlameModelIdx[self->flameColor]);
    self->lightNode = LightContext_InsertLight(play, (u8*)play + 0xA70, (u8*)self + 0x1AC);
    Lights_PointGlowSetInfo((u8*)self + 0x1AC, actor->world.pos.x, actor->world.pos.y + 65.0f, actor->world.pos.z, 0, 0,
                            0, 0, 0);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, actor, &BgPoSyokudai_sCylinderInit);
    self->collider.dim.pos = actor->world.pos;

    if (self->flameColor == 0 && Flags_GetSwitch(play, 0x1F) && Flags_GetSwitch(play, 0x1E) &&
        Flags_GetSwitch(play, 0x1D) && !Flags_GetSwitch(play, actor->params)) {
        Actor_Spawn(&play->actorCtx, play, 0x91, 119.0f, 225.0f, -1566.0f, 0, 0, 0, actor->params, 1);
        ((u8*)play + 0x3000)[0x237] = 4;
    } else if (!Flags_GetSwitch(play, 0x1C) && !Flags_GetSwitch(play, 0x1B)) {
        Actor_Spawn(&play->actorCtx, play, 0x91, actor->world.pos.x, actor->world.pos.y + 52.0f, actor->world.pos.z, 0, 0,
                    0, (self->flameColor << 8) + actor->params + 0x1000, 1);
    } else if (!Flags_GetSwitch(play, actor->params)) {
        if (((u8*)play + 0x3000)[0x237] == 0xFF) {
            ((u8*)play + 0x3000)[0x237] = 4;
        }
    }
    self->flameTextureScroll = (s32)(Rand_ZeroOne() * 30.0f);
    actor->unk_19B = 3;
}

void LightContext_RemoveLight(PlayState* play, void* lightCtx, void* light);
void FUN_0035046c(void* model);

void BgPoSyokudai_Destroy(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    BgPoSyokudai* self = (BgPoSyokudai*)thisx;
    u8* env;

    LightContext_RemoveLight(play, (u8*)play + 0xA70, *(void**)((u8*)self + 0x1A8));
    Collider_DestroyCylinder(play, (ColliderCylinder*)((u8*)self + 0x1C4));
    if (((u8*)play + 0x3000)[0x237] != 0xFF) {
        ((u8*)play + 0x3000)[0x237] = 0xFF;
    }
    FUN_00350f34(thisx, (SkeletonAnimationModel**)((u8*)self + 0x21C), 0);
    FUN_0035046c((u8*)self + 0x220);
}

void FUN_00373264(Actor* actor, s32 arg1);

void BgPoSyokudai_Update(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    BgPoSyokudai* self = (BgPoSyokudai*)thisx;

    CollisionCheck_SetAC(play, &play->colChkCtx, (Collider*)((u8*)self + 0x1C4));
    CollisionCheck_SetOC(play, &play->colChkCtx, (Collider*)((u8*)self + 0x1C4));
    if (Flags_GetSwitch(play, self->actor.params)) {
        FUN_00373264(thisx, 0x01000188);
    }
    if (*(s8*)((u8*)play + 0x4C30) != thisx->room) {
        *((u8*)play + 0x3237) = 0xFF;
    }
    self->flameTextureScroll++;
}

// BgPoSyokudai_Draw: NOT matched (35 words off, ~60 variants). Structure/rotation/scale rows match. Left: GPR numbering (ROM play=r7,
// SetUnkAC 1=r6, primColor=r5; ours r5/r7/r6), translate vec VFP regs (ROM x=s0 copy of s2, y=s1 literal; helper param order perms
// all tried), pause tail (ROM vldreq 1.0 / vstrne s17(0)/vstreq). Raw `(PlayState*)state` into FUN_00357fd0 arg gave 47->35.
#if 0
void Matrix_UpdatePosition(MTX34* dst, MTX34* src, Vec3f* translate);
void Lights_PointSetColorAndRadius(void* light, u8 r, u8 g, u8 b, s16 radius, s32 pad);
s16 Camera_GetCamDirYaw(Camera* camera);
void FUN_00357fd0(Player* player, void* arg1, Vec3f* pos);
void FUN_00373bec(void* obj);

static inline void BgPoSyokudai_Translate(MTX34* mtx, f32 y, f32 x, f32 z) {
    Vec3f translate;

    translate.x = x;
    translate.y = y;
    translate.z = z;
    Matrix_UpdatePosition(mtx, mtx, &translate);
}
static inline void BgPoSyokudai_Scale(MTX34* m, f32 x, f32 y, f32 z) {
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

void BgPoSyokudai_Draw(Actor* actor, GameState* state) {
    BgPoSyokudai* self = (BgPoSyokudai*)actor;
    PlayState* play = (PlayState*)state;
    MTX34 mtx;

    MTX34CopyAsm(&mtx, &actor->modelMtx);
    FUN_00357fd0(GET_PLAYER((PlayState*)state), actor->unk_178, &actor->world.pos);
    if (self->skelAnim != NULL) {
        ((SkeletonAnimationModel*)self->skelAnim)->SetUnkAC(1);
        ((SkeletonAnimationModel*)self->skelAnim)->FUN_003721e0(&mtx);
        ((SkeletonAnimationModel*)self->skelAnim)->FUN_00372170(0);
    }
    if (Flags_GetSwitch(play, actor->params)) {
        u8* primColor = BgPoSyokudai_sPrimColors + self->flameColor * 4;
        f32 brightness = 0.3f * Rand_ZeroOne() + 0.7f;
        f32 angle;

        Lights_PointSetColorAndRadius((u8*)self + 0x1AC, (u8)(primColor[0] * brightness), (u8)(primColor[1] * brightness),
                                      (u8)(primColor[2] * brightness), 200, 0);
        BgPoSyokudai_Translate(&mtx, 52.0f, 0.0f, 0.0f);
        angle = (s16)(Camera_GetCamDirYaw(play->cameraPtrs[play->activeCamera]) - actor->shape.rot.y + 0x8000) *
                (3.14159274f / 32768.0f);
        if (angle != 0.0f) {
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
        BgPoSyokudai_Scale(&mtx, 0.0027f, 0.0027f, 0.0027f);
        *(f32*)((u8*)*(void**)((u8*)self + 0x228) + 0xC) = (PauseContext_GetState() != 0) ? 0.0f : 1.0f;
        FUN_00373bec(*(void**)((u8*)self + 0x228));
        ((SkeletonAnimationModel*)*(void**)((u8*)self + 0x220))->SetUnkAC(1);
        ((SkeletonAnimationModel*)*(void**)((u8*)self + 0x220))->FUN_003721e0(&mtx);
        ((SkeletonAnimationModel*)*(void**)((u8*)self + 0x220))->FUN_00372170(0);
    }
}
#endif

GLOBAL_ASM("asm/BgPoSyokudai_Draw.s")
}
