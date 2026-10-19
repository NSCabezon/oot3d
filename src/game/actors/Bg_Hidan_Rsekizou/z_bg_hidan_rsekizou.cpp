extern "C" {
#include "z_bg_hidan_rsekizou.hpp"

#define FLAGS 0x80400000

void BgHidanRsekizou_Init(Actor* actor, GameState* state);
void BgHidanRsekizou_Destroy(Actor* thisx, PlayState* play);
void BgHidanRsekizou_Update(Actor* thisx, PlayState* play);
void BgHidanRsekizou_Draw(Actor* thisx, PlayState* play);

void Collider_InitJntSph(PlayState* play, BgHidanRsekizouColliderJntSph* collider);
void Collider_SetJntSph(PlayState* play, BgHidanRsekizouColliderJntSph* collider, Actor* actor,
                        BgHidanRsekizouJntSphInit* init, BgHidanRsekizouColliderElement* elements);

extern InitChainEntry BgHidanRsekizou_sInitChain[];
extern BgHidanRsekizouJntSphInit BgHidanRsekizou_sJntSphInit;

GLOBAL_ASM("data/z_Bg_Hidan_Rsekizou.data.s")
GLOBAL_ASM("../src/game/actors/Bg_Hidan_Rsekizou/data_syms.s")

ActorInit Bg_Hidan_Rsekizou_InitVars = {
    ACTOR_BG_HIDAN_RSEKIZOU,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HIDAN_OBJECTS,
    sizeof(BgHidanRsekizou),
    (ActorFunc)BgHidanRsekizou_Init,
    (ActorFunc)BgHidanRsekizou_Destroy,
    (ActorFunc)BgHidanRsekizou_Update,
    (ActorFunc)BgHidanRsekizou_Draw,
};

void BgHidanRsekizou_Init(Actor* actor, GameState* state) {
    BgHidanRsekizou* self = (BgHidanRsekizou*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;
    ZARInfo* zarInfo;
    s32 i;

    Actor_ProcessInitChain(actor, BgHidanRsekizou_sInitChain);
    DynaPolyActor_Init(&self->dyna, 0);
    zarInfo = FUN_00372f38(actor, play, &self->skelAnimModel[0], 7, &self->skelAnimModel[1], 7,
                                         &self->skelAnimModel[2], 7, &self->skelAnimModel[3], 7,
                                         &self->skelAnimModel[4], 7, &self->skelAnimModel[5], 7,
                                         &self->skelAnimModel[6], 7, &self->skelAnimModel[7], 7,
                                         &self->skelAnimModel[8], 0x14, 0);
    {
        void* cmab = zarInfo->GetCMABByIndex(0);
        SkeletonAnimationModel* m0 = self->skelAnimModel[0];
        m0->unk_0C->FUN_00372d94(cmab);
    }
    self->skelAnimModel[0]->unk_0C->SetAnimType(1);
    {
        void* cmab = zarInfo->GetCMABByIndex(0);
        SkeletonAnimationModel* m0 = self->skelAnimModel[1];
        m0->unk_0C->FUN_00372d94(cmab);
    }
    self->skelAnimModel[1]->unk_0C->SetAnimType(1);
    {
        void* cmab = zarInfo->GetCMABByIndex(0);
        SkeletonAnimationModel* m0 = self->skelAnimModel[2];
        m0->unk_0C->FUN_00372d94(cmab);
    }
    self->skelAnimModel[2]->unk_0C->SetAnimType(1);
    {
        void* cmab = zarInfo->GetCMABByIndex(0);
        SkeletonAnimationModel* m0 = self->skelAnimModel[3];
        m0->unk_0C->FUN_00372d94(cmab);
    }
    self->skelAnimModel[3]->unk_0C->SetAnimType(1);
    {
        void* cmab = zarInfo->GetCMABByIndex(0);
        SkeletonAnimationModel* m0 = self->skelAnimModel[4];
        m0->unk_0C->FUN_00372d94(cmab);
    }
    self->skelAnimModel[4]->unk_0C->SetAnimType(1);
    {
        void* cmab = zarInfo->GetCMABByIndex(0);
        SkeletonAnimationModel* m0 = self->skelAnimModel[5];
        m0->unk_0C->FUN_00372d94(cmab);
    }
    self->skelAnimModel[5]->unk_0C->SetAnimType(1);
    {
        void* cmab = zarInfo->GetCMABByIndex(0);
        SkeletonAnimationModel* m0 = self->skelAnimModel[6];
        m0->unk_0C->FUN_00372d94(cmab);
    }
    self->skelAnimModel[6]->unk_0C->SetAnimType(1);
    self->skelAnimModel[7]->unk_0C->FUN_00372d94(zarInfo->GetCMABByIndex(0));
    self->skelAnimModel[7]->unk_0C->SetAnimType(1);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 0x12), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    Collider_InitJntSph(play, &self->collider);
    Collider_SetJntSph(play, &self->collider, actor, &BgHidanRsekizou_sJntSphInit, self->colliderElements);
    for (i = 0; i < 6; i++) {
        self->collider.elements[i].worldSphere.radius = self->collider.elements[i].modelSphere.radius;
    }
    self->burnFrame = 0;
    self->bendFrame = 0;
}

void Collider_DestroyJntSph(PlayState* play, BgHidanRsekizouColliderJntSph* collider);

void BgHidanRsekizou_Destroy(Actor* thisx, PlayState* play) {
    BgHidanRsekizou* self = (BgHidanRsekizou*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyJntSph(play, &self->collider);
    FUN_00350f34(thisx, &self->skelAnimModel[0], &self->skelAnimModel[1], &self->skelAnimModel[2],
                               &self->skelAnimModel[3], &self->skelAnimModel[4], &self->skelAnimModel[5],
                               &self->skelAnimModel[6], &self->skelAnimModel[7], &self->skelAnimModel[8], 0);
}

// BgHidanRsekizou_Update: matched; the z rotation goes through RotZ with ROM-derived param order (VFP numbering).
static inline f32 RotZ(f32 h, f32 s, f32 c, f32 z, f32 x) { return (h - s * x) + c * z; }
void BgHidanRsekizou_Update(Actor* thisx, PlayState* play) {
    BgHidanRsekizou* self = (BgHidanRsekizou*)thisx;
    s32 i;
    f32 yawSine;
    f32 yawCosine;

    if (self->bendFrame != 0) {
        self->bendFrame--;
    }
    if (self->bendFrame == 0) {
        self->bendFrame = 3;
    }

    self->dyna.actor.shape.rot.y += 0x100;
    yawSine = Math_SinS(self->dyna.actor.shape.rot.y);
    yawCosine = Math_CosS(self->dyna.actor.shape.rot.y);

    for (i = 0; i < 6; i++) {
        BgHidanRsekizouColliderElement* element = &self->collider.elements[i];

        element->worldSphere.center.x = (element->modelSphere.center.x * yawCosine +
                                         yawSine * element->modelSphere.center.z) + self->dyna.actor.home.pos.x;
        element->worldSphere.center.y = self->dyna.actor.home.pos.y + element->modelSphere.center.y;
        element->worldSphere.center.z = RotZ(self->dyna.actor.home.pos.z, yawSine, yawCosine, element->modelSphere.center.z, element->modelSphere.center.x);
    }

    CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
    FUN_00373264(thisx, 0x0100018A);
}


// BgHidanRsekizou_Draw: C matches the call structure but the identity-MTX34 guard init (VFP numbering, known blocker)
// differs: 92/125 words. Kept as draft under #if 0 + GLOBAL_ASM.
#if 0
s16 Camera_GetCamDirYaw(Camera* camera);
void FUN_0031de1c(PlayState* play, BgHidanRsekizou* self, s16 frame, MTX34* mtx, u32 a, u32 modelOffset);

void BgHidanRsekizou_Draw(Actor* thisx, PlayState* play) {
    BgHidanRsekizou* self = (BgHidanRsekizou*)thisx;
    MTX34 mtx;
    MTX34 identity;
    s32 i;

    MTX34CopyAsm(&mtx, &s2->dyna.actor.modelMtx);
    if (s2->skelAnimModel[8] != NULL) {
        s2->skelAnimModel[8]->SetUnkAC(1);
        s2->skelAnimModel[8]->FUN_003721e0(&mtx);
        s2->skelAnimModel[8]->FUN_00372170(0);
    }
    MTX34CopyAsm(&identity, (MTX34*)&MTX34_Identity());

    if ((s16)((Camera_GetCamDirYaw(play->cameraPtrs[play->activeCamera]) - s2->dyna.actor.shape.rot.y) - 0x2E6C) >= 0) {
        for (i = 3; i >= 0; i--) {
            FUN_0031de1c(play, self, i, &identity, 0, 4);
        }
        for (i = 0; i < 4; i++) {
            FUN_0031de1c(play, self, i, &identity, 1, 0);
        }
    } else {
        for (i = 3; i >= 0; i--) {
            FUN_0031de1c(play, self, i, &identity, 1, 4);
        }
        for (i = 0; i < 4; i++) {
            FUN_0031de1c(play, self, i, &identity, 0, 0);
        }
    }
}
#endif

GLOBAL_ASM("asm/BgHidanRsekizou_Draw.s")

void FUN_00371fac(MTX34* mtx, void* billboard);

void FUN_0031de1c(PlayState* play, BgHidanRsekizou* self, s16 frame, MTX34* mtx, u32 a, u32 modelOffset) {
    BgHidanRsekizou* s2 = self;
    SkeletonAnimationModel* model = self->skelAnimModel[modelOffset + frame];
    f32 fVar6;
    f32 coss;
    f32 sins;
    f32 xx;
    f32 tmpf7;
    f32 speed;
    MTX34 local;
    PlayState* play2 = play;

    speed = 1.0f;
    if (PauseContext_GetState()) {
        speed = 0.0f;
    }
    frame++;
    fVar6 = (frame != 4) ? frame + ((3 - s2->bendFrame) * (1.0f / 3.0f)) : frame;

    if (a == 0) {
        sins = -Math_SinS(s2->dyna.actor.shape.rot.y - (frame * 1500));
        coss = -Math_CosS(s2->dyna.actor.shape.rot.y - (frame * 1500));
    } else {
        sins = Math_SinS(s2->dyna.actor.shape.rot.y - (frame * 1500));
        coss = Math_CosS(s2->dyna.actor.shape.rot.y - (frame * 1500));
    }

    xx = (0.7f * fVar6) + 0.5f;
    mtx->data[0][0] = mtx->data[1][1] = mtx->data[2][2] = xx;
    tmpf7 = 20.0f + xx * (fVar6 * 10.0f);
    mtx->data[0][3] = s2->dyna.actor.world.pos.x + tmpf7 * sins;
    mtx->data[1][3] = s2->dyna.actor.world.pos.y + (30.0f + (0.7f * fVar6));
    mtx->data[2][3] = s2->dyna.actor.world.pos.z + tmpf7 * coss;

    if (model != NULL) {
        local = *mtx;
        FUN_00371fac(&local, (char*)play2 + 0x2FC);
        model->SetUnkAC(1);
        model->FUN_003721e0(&local);
        model->unk_0C->SetAnimSpeed(speed);
        model->FUN_00372170(0);
    }
}

}
