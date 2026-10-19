extern "C" {
#include "z_en_toryo.hpp"

#define FLAGS 0x00000009
#define DEG_TO_BINANG2(deg) ((f32)(s16)((deg) * 182.04445f))

void EnToryo_Init(Actor* actor, PlayState* play);
void EnToryo_Destroy(Actor* actor, PlayState* play);
void EnToryo_Update(Actor* actor, PlayState* play);
void EnToryo_Draw(Actor* actor, PlayState* play);
s32 EnToryo_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void EnToryo_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void FUN_0034c664(Actor* actor, EnToryoInteractInfo* info, s32 arg2, s32 mode);
void FUN_0034e01c(MTX34* mtx, Vec3s* rot);
void GraphicsContext_UnknownStub(GraphicsContext* gfxCtx); // N64: Gfx_SetupDL_25Opa (empty on 3DS)

GLOBAL_ASM("data/z_En_Toryo.data.s")

ActorInit En_Toryo_InitVars = {
    ACTOR_EN_TORYO,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_TORYO,
    sizeof(EnToryo),
    (ActorFunc)EnToryo_Init,
    (ActorFunc)EnToryo_Destroy,
    (ActorFunc)EnToryo_Update,
    (ActorFunc)EnToryo_Draw,
};

GLOBAL_ASM("asm/EnToryo_Init.s")

#if 0 // NON_MATCHING drafts (asm still in use: see notes)
void EnToryo_Destroy(Actor* thisx, PlayState* play) {
    EnToryo* self = (EnToryo*)thisx;

    Collider_DestroyCylinder(play, &self->collider);
    FUN_003508b8(thisx, self->skelAnimModel, self->skelAnimModel2, self->skelAnimModel3);
    SkelAnime_Free2(&self->skelAnime);
}

void EnToryo_Update(Actor* thisx, PlayState* play) {
    EnToryo* self = (EnToryo*)thisx;
    Player* player = GET_PLAYER(play);
    f32 rot;

    self->skelAnime.unk_80[1] = 1;
    Collider_UpdateCylinder(thisx, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, (Collider*)&self->collider);

    self->actionFunc(self, play);

    if (self->stateFlags & 8) {
        self->interactInfo.trackPos.x = player->actor.focus.pos.x;
        self->interactInfo.trackPos.y = player->actor.focus.pos.y;
        self->interactInfo.trackPos.z = player->actor.focus.pos.z;

        if (self->stateFlags & 0x10) {
            FUN_0034c664(thisx, &self->interactInfo, 0, 4);
            return;
        }

        s16 shapeYaw = thisx->shape.rot.y;
        s16 yawToPlayer = thisx->yawTowardsPlayer;
        rot = yawToPlayer - shapeYaw;
        if ((rot < DEG_TO_BINANG2(80.0f)) && (rot > DEG_TO_BINANG2(-80.0f))) {
            FUN_0034c664(thisx, &self->interactInfo, 0, 2);
        } else {
            FUN_0034c664(thisx, &self->interactInfo, 0, 1);
        }
    }
}

#endif

GLOBAL_ASM("asm/EnToryo_Destroy.s")

GLOBAL_ASM("asm/EnToryo_Update.s")

void EnToryo_Draw(Actor* thisx, PlayState* play) {
    EnToryo* self = (EnToryo*)thisx;

    GraphicsContext_UnknownStub(play->state.gfxCtx);
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, EnToryo_OverrideLimbDraw, EnToryo_PostLimbDraw,
                      thisx, 0);
}

s32 EnToryo_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnToryo* self = (EnToryo*)thisx;

    if (self->stateFlags & 8) {
        if (limbIndex == 9) {
            FUN_0034e01c(mtx, &self->interactInfo.torsoRot);
        } else if (limbIndex == 10) {
            FUN_0034e01c(mtx, &self->interactInfo.headRot);
        }
    }
    return 0;
}

// Function-local static: guard 0x0052FA00 (TU .data, cut from the data chunk), object 0x0059B718 pinned in
// data/oot3d.ld (scripts/pin_static.py).
// N64-order wrapper (Matrix_MultVec3f(src, dest)).
static inline void MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}

// Vec3f with a Set()-based ctor: gives the ROM's x->s0, y->s1, z->s2 literal registers in the guarded init.
struct EnToryoVec3f {
    f32 x, y, z;
    EnToryoVec3f(f32 a, f32 b, f32 c) {
        Set(a, b, c);
    }
    void Set(f32 a, f32 b, f32 c) {
        x = a;
        y = b;
        z = c;
    }
};

void EnToryo_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnToryo* self = (EnToryo*)thisx;
    static EnToryoVec3f sMultVec(800.0f, 1000.0f, 0.0f);

    if (limbIndex == 10) {
        MultVec3f((Vector3f*)&sMultVec, (Vector3f*)&self->actor.focus.pos, mtx);
    }
}

}
