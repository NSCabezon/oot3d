extern "C" {
#include "z_en_ba.hpp"

#define FLAGS 0x00000015

void EnBa_Init(Actor* thisx, PlayState* play2);
void EnBa_Destroy(Actor* actor, PlayState* play);
void EnBa_Update(EnBa* self, PlayState* play);
void EnBa_Draw(Actor* actor, PlayState* play);

// Functions in other TUs (still asm) referenced by the actor
void FUN_003e3838(EnBa* self, PlayState* play);
void FUN_0011bb70(EnBa* self, PlayState* play);
void FUN_00232dac(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg);
void FUN_00198c20(Actor* actor, PlayState* play);
void Collider_DestroyJntSph(PlayState* play, void* collider);
void EffectSsDeadSound_SpawnStationary(PlayState* play, Vec3f* pos, s32 sfxId, s32 a, s32 b, s32 c);

GLOBAL_ASM("data/z_En_Ba.data.s")
GLOBAL_ASM("../src/game/actors/En_Ba/data_syms.s")

ActorInit En_Ba_InitVars = {
    ACTOR_EN_BA,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_BXA,
    sizeof(EnBa),
    (ActorFunc)EnBa_Init,
    (ActorFunc)EnBa_Destroy,
    (ActorFunc)EnBa_Update,
    (ActorFunc)EnBa_Draw,
};

GLOBAL_ASM("asm/EnBa_Init.s")

// UNMATCHED draft (32 words of 280, same size): remaining diffs = ROM keeps `1` in r6 / `4` in r7 (ours swapped),
// `ldr r1,=0x55555556` scheduled before `strb animType` with params in r0 (ours params in r1), table ldm base,
// and the inlined FUN_0036651c store order. Needs EQU syms in data_syms.s (EnBa_sInitChain 0x520A4C,
// EnBa_sJntSphInit 0x520A3C, EnBa_sFaceTable 0x4D9260, shared gUnk_0054C8CC).
#if 0
typedef struct {
    s32 v[3];
} EnBaFaceTable;
extern InitChainEntry EnBa_sInitChain[];
extern u8 EnBa_sJntSphInit[];
extern const EnBaFaceTable EnBa_sFaceTable;
extern s32 gUnk_0054C8CC;

void Collider_InitJntSph(PlayState* play, void* collider);
void Collider_SetJntSph(PlayState* play, void* collider, Actor* actor, void* init, void* elements);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 index);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 index, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void FUN_003586ec(SkeletonAnimationModel_unk_0C* anim);
void FUN_0034e994(FaceAnimations* face, SkeletonAnimationModel* model, ZARInfo* zar, s32 a, s32 b, s32 c);

void EnBa_Init(Actor* thisx, PlayState* play2) {
    EnBa* self = (EnBa*)thisx;
    Vector3f sp38(0.01f, 0.01f, 0.01f);
    PlayState* play = play2;
    s16 i;
    ZARInfo* zar;
    s32 faceIdx;
    s32 cmabIdx;
    s16 params;
    s16 p2;
    EnBaFaceTable tbl;

    Actor_ProcessInitChain(&self->actor, EnBa_sInitChain);
    self->actor.world.pos.y = self->actor.home.pos.y + 100.0f;
    for (i = 13; i >= 0; i--) {
        self->unk_200[i] = *(Vec3f*)&sp38;
        self->unk_2A8[i].x = -0x4000;
        self->unk_158[i] = self->actor.world.pos;
        self->unk_158[i].y = self->actor.world.pos.y - (i + 1) * 32.0f;
    }

    self->actor.targetMode = 4;
    params = thisx->params;
    if (params == 999) {
        self->unk_C48 = 1;
        FUN_00372f38(&self->actor, play2, 0);
        return;
    }
    self->upperParams = (params >> 8) & 0xFF;
    thisx->params &= 0xFF;
    zar = FUN_00372f38(&self->actor, play2, &self->skelAnimModel, 1, 0);
    FUN_00353c9c(&self->actor, play2, &self->skelAnime, 0, 0, self->unk_228, self->unk_228 + 0x2D8, 14);
    Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, (f32)Animation_GetLastFrame(&self->skelAnime, 0), 2, 1.0f);
    if (self->actor.params < 3) {
        self->unk_7D8 = self->skelAnime.unk_28->unk_0C;
        cmabIdx = 3;
    } else {
        self->unk_7D8 = self->skelAnimModel->unk_0C;
        cmabIdx = 4;
    }
    self->unk_7D8->FUN_00372d94(zar->GetCMABByIndex(cmabIdx));
    self->unk_7D8->animSpeed = 1.0f;
    {
        SkeletonAnimationModel_unk_0C* anim = self->unk_7D8;
        f32 frame = 0.0f;

        if (gUnk_0054C8CC == 0) {
            *(f32*)&anim->curFrame = frame;
            FUN_003586ec(anim);
        }
    }
    self->unk_7D8->animType = 1;
    p2 = self->actor.params;
    faceIdx = p2 % 3;
    tbl = EnBa_sFaceTable;
    if (p2 < 3) {
        SkelAnime_SetFaceAnimations(&self->faceAnim, &self->skelAnime, 2, -1, -1);
    } else {
        FUN_0034e994(&self->faceAnim, self->skelAnimModel, zar, 5, -1, -1);
    }
    self->faceAnim.FUN_0035e3a4(0, tbl.v[faceIdx]);
    self->faceAnim.FUN_0035e330();

    if (self->actor.params < 3) {
        if (Flags_GetSwitch(play, self->upperParams)) {
            Actor_Kill(&self->actor);
            return;
        }
        ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 48.0f);
        Actor_SetScale(&self->actor, 0.01f);
        FUN_0036651c(self);
        self->actor.colChkInfo.health = 4;
        self->actor.colChkInfo.mass = MASS_HEAVY;
        Collider_InitJntSph(play2, &self->unk_B7C);
        Collider_SetJntSph(play2, &self->unk_B7C, &self->actor, EnBa_sJntSphInit, &self->unk_B7C[0x20]);
    } else {
        self->actor.flags &= ~5;
        Actor_ChangeType(play, &play->actorCtx, &self->actor, 6);
        Actor_SetScale(&self->actor, 0.021f);
        self->unk_9A8 = 0;
        self->actor.speedXZ = Rand_CenteredFloat(16.0f);
        self->actor.velocity.y = Rand_CenteredFloat(30.0f);
        self->actor.world.rot.y = Rand_CenteredFloat(65535.0f);
        self->actor.gravity = -2.0f;
        self->sub.unk_74 = 30;
        self->actionFunc = (EnBaActionFunc)FUN_00106ecc;
    }
}
#endif

void FUN_0036651c(EnBa* self) {
    self->unk_9A8 = 4;
    self->actor.speedXZ = 10.0f;
    self->sub.unk_78 = 1000;
    self->actionFunc = FUN_003e3838;
}

void FUN_003664d4(EnBa* self) {
    f32 speed;

    self->unk_9A8 = 5;
    self->sub.unk_74 = 30;
    self->sub.unk_76 = 0;
    speed = 20.0f;
    self->actor.speedXZ = speed;
    self->sub.unk_78 = 1000;
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    self->actionFunc = FUN_0011bb70;
}

void EnBa_Destroy(Actor* thisx, PlayState* play) {
    EnBa* self = (EnBa*)thisx;

    if (self->unk_C48 == 1) {
        FUN_00350f34(thisx, 0);
        return;
    }
    Collider_DestroyJntSph(play, &self->unk_B7C);
    self->faceAnim.Destroy();
    SkelAnime_Free2(&self->skelAnime);
    FUN_00350f34(thisx, &self->skelAnimModel, 0);
}

GLOBAL_ASM("asm/EnBa_Update.s")

// UNMATCHED draft (far, 157 words, size 776 vs 792): logic follows the ROM, but armcc folds the `play2 = play` copies
// (ROM keeps play2 in r5, &actorCtx in r8, sub base r7) and the Vec3f at/eye do not overlap the Actor_Spawn stack args.
#if 0
f32 sinf(f32 x);
f32 cosf(f32 x);
void Animation_PlayOnce(SkelAnime* skelAnime, s32 index);
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);
void FUN_003d51ec(Actor* thisx, PlayState* play);
void FUN_003c6320(EnBa* self, PlayState* play);

void EnBa_Update(Actor* thisx, PlayState* play) {
    EnBa* self = (EnBa*)thisx;

    if (self->unk_C48 != 0) {
        return;
    }
    self->actor.xyzDistToPlayerSq *= 1.1f;
    if (self->actor.params < 3 && (self->unk_B7C[0x11] & 2)) {
        s32 sfx = 0x010003CA;

        self->unk_B7C[0x11] &= ~2;
        self->actor.colChkInfo.health--;
        if (self->actor.colChkInfo.health == 0) {
            PlayState* play2 = play;

            Animation_PlayOnce(&self->skelAnime, 2);
            Audio_PlayActorSound2(&self->actor, sfx);
            Actor_SetColorFilter(&self->actor, 0x400000, 0xFF, 0, 12);
            self->actor.flags &= ~1;
            self->camId = Gameplay_CreateSubCamera(play);
            Gameplay_ChangeCameraStatus(play2, 0, 1);
            Gameplay_ChangeCameraStatus(play2, (s16)self->camId, 7);
            FUN_0036e980(play2, &self->actor, 5);
            self->unk_C44 = Actor_Spawn(&play2->actorCtx, play2, 0xDE, self->actor.world.pos.x,
                                        self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 0x3E7, 1);
            Actor_ChangeType(play2, &play2->actorCtx, &self->actor, 6);
            {
                Player* player = GET_PLAYER(play);
                f32 angle;
                Vec3f at;
                Vec3f eye;
                PlayState* play3 = play;

                angle = Math_FAtan2F(player->actor.world.pos.x - self->actor.world.pos.x,
                                     player->actor.world.pos.z - self->actor.world.pos.z) + 1.0472f;
                at.x = (self->actor.world.pos.x + player->actor.world.pos.x) * 0.5f;
                at.y = player->actor.world.pos.y + 105.0f;
                at.z = (self->actor.world.pos.z + player->actor.world.pos.z) * 0.5f;
                eye.x = at.x + sinf(angle) * 170.0f;
                eye.y = player->actor.world.pos.y - 4.0f;
                eye.z = at.z + cosf(angle) * 170.0f;
                Gameplay_CameraSetAtEye(play3, (s16)self->camId, &at, &eye);
            }
            self->unk_9A8 = 1;
            self->sub.unk_7A = 0;
            self->actionFunc = (EnBaActionFunc)FUN_003d51ec;
        } else {
            self->unk_9A8 = 3;
            self->actor.speedXZ = 10.0f;
            self->sub.unk_78 = 1000;
            self->sub.unk_74 = 30;
            self->actor.colChkInfo.mass = MASS_IMMOVABLE;
            Audio_PlayActorSound2(&self->actor, sfx);
            Actor_SetColorFilter(&self->actor, 0x400000, 0xFF, 0, 12);
            self->actionFunc = (EnBaActionFunc)FUN_003c6320;
        }
    }
    self->actionFunc(self, play);
    if (self->actor.params < 3) {
        self->actor.focus.pos = self->unk_158[7];
    }
    if (self->unk_9A8 >= 4) {
        CollisionCheck_SetAC(play, &play->colChkCtx, (Collider*)&self->unk_B7C);
    }
}
#endif

// UNMATCHED draft FUN_00232dac (SkelAnime PostLimbDraw; 54 words of 128, same size): ROM allocates
// i->r6, mtx->r7, &sBaseRot->r8, self->r5 (ours r4/sb/r8/r5); ~35 variants (alias locals, helper param orders,
// early-return form) did not move it. Typed `EnBa* self` param (decl at top + (PostLimbDrawOpa) cast at the
// SkelAnime_DrawOpa call) gives 54, `void* arg` + cast inside gives 61. Stays GLOBAL_ASM in functions_200000s.cpp.
#if 0
void FUN_003713fc(MTX34* m, s32 mode, f32 x, f32 y, f32 z);
void FUN_0036e88c(MTX34* m, s16 x, s16 y, s16 z, s32 mode);
void Matrix_Scale(MTX34* m, s32 mode, f32 x, f32 y, f32 z);
void Collider_UpdateSpheres(s32 index, void* collider, MTX34* mtx);
extern Vec3s EnBa_sBaseRot;

static inline void EnBa_SetLimbMtx(EnBa* self, MTX34* mtx, s32 i) {
    FUN_003713fc(mtx, 0, self->unk_158[i].x, self->unk_158[i].y, self->unk_158[i].z);
    FUN_0036e88c(mtx, self->unk_2A8[i].x, self->unk_2A8[i].y, self->unk_2A8[i].z, 1);
    FUN_0036e88c(mtx, EnBa_sBaseRot.x, EnBa_sBaseRot.y, EnBa_sBaseRot.z, 1);
    Matrix_Scale(mtx, 1, self->unk_200[i].x, self->unk_200[i].y, self->unk_200[i].z);
}

void FUN_00232dac(PlayState* play, s32 i, MTX34* mtx, EnBa* self) {
    Vector3f zero(0.0f, 0.0f, 0.0f);

    if (self->unk_9A8 != 1) {
        EnBa_SetLimbMtx(self, mtx, i);
        if (i == 6) {
            Matrix_Scale(mtx, 1, 1.0f, 0.5f, 0.5f);
        } else if (i == 7) {
            Collider_UpdateSpheres(9, &self->unk_B7C, mtx);
        } else if (i == 12) {
            Collider_UpdateSpheres(13, &self->unk_B7C, mtx);
        }
    } else {
        if (i == 0) {
            EnBa_SetLimbMtx(self, mtx, 0);
        }
        VEC3TransformAsm((Vector3f*)&self->unk_158[i], mtx, &zero);
        if (i == 6) {
            Matrix_Scale(mtx, 1, 1.0f, 0.5f, 0.5f);
        } else if (i == 7) {
            Matrix_Scale(mtx, 1, 1.0f, 2.0f, 2.0f);
        }
    }
}
#endif

void FUN_003d51ec(EnBa* self, PlayState* play) {
    self->sub.unk_7A++;
    if (self->sub.unk_7A == 0x22) {
        EffectSsDeadSound_SpawnStationary(play, &self->actor.world.pos, 0x01000472, 1, 1, 40);
    }
    if (FUN_003731e0(&self->skelAnime)) {
        PlayState* play2 = play;

        FUN_00198c20(&self->actor, play2);
    }
}

void EnBa_Draw(Actor* thisx, PlayState* play) {
    EnBa* self = (EnBa*)thisx;
    f32 zero = 0.0f;
    if (self->unk_C48 == 0) {
        SkeletonAnimationModel_unk_0C* p;
        if (PauseContext_GetState() != 0) {
            p = self->unk_7D8;
            p->animSpeed = zero;
        } else {
            p = self->unk_7D8;
            p->animSpeed = 1.0f;
        }
        if (self->actor.params < 3) {
            MTX34 unk;

            SkelAnime_DrawOpa(&self->skelAnime, &unk, NULL, FUN_00232dac, thisx, 0);
        } else {
            self->skelAnimModel->SetUnkAC(1);
            self->skelAnimModel->FUN_003721e0(&self->actor.modelMtx);
            self->skelAnimModel->FUN_00372170(0);
        }
    }
}

f32 FUN_003159f8(Vec3f* src, Vec3f* dst, f32 frac, f32 step, f32 nearDist, f32 farDist) {
    f32 ret = 0.0f;
    f32 dx = dst->x - src->x;
    f32 dy = dst->y - src->y;
    f32 dz = dst->z - src->z;
    f32 dist = __sqrtf(dx * dx + dy * dy + dz * dz);

    if (dist >= nearDist || dist > farDist) {
        ret = Math_SmoothStepToF(&dst->x, src->x, frac, step, 0.0f);
        ret += Math_SmoothStepToF(&dst->y, src->y, frac, step, 0.0f);
        ret += Math_SmoothStepToF(&dst->z, src->z, frac, step, 0.0f);
    }
    return ret;
}

void FUN_00106ecc(Actor* actor, PlayState* play) {
    EnBa* parent;

    if (actor->bgCheckFlags & 2) {
        parent = (EnBa*)actor->parent;
        if (parent != NULL && parent->unk_C49 < 5) {
            Audio_PlayActorSound2(actor, 0x01000471);
            parent->unk_C49++;
        }
    }
    if (actor->bgCheckFlags & 1) {
        EnBa* self = (EnBa*)actor;
        s16 t;

        actor->scale.y -= 0.001f;
        actor->scale.x += 0.0005f;
        actor->scale.z += 0.0005f;
        t = self->sub.unk_74 - 1;
        self->sub.unk_74 = t;
        if (t == 0) {
            Actor_Kill(actor);
        }
        return;
    }
    Actor_MoveForward(actor);
    Actor_UpdateBgCheckInfo(play, actor, 30.0f, 28.0f, 80.0f, 5);
}

}
