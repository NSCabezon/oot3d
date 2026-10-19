extern "C" {
#include "z_obj_syokudai.hpp"
#include "model_manager.hpp"

#define FLAGS 0x00000410

void ObjSyokudai_Init(Actor* thisx, PlayState* play);
void ObjSyokudai_Destroy(Actor* thisx, PlayState* play);
void ObjSyokudai_Update(Actor* actor, GameState* state);
void ObjSyokudai_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Obj_Syokudai.data.s")

ActorInit Obj_Syokudai_InitVars = {
    ACTOR_OBJ_SYOKUDAI,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_SYOKUDAI,
    sizeof(ObjSyokudai),
    (ActorFunc)ObjSyokudai_Init,
    (ActorFunc)ObjSyokudai_Destroy,
    (ActorFunc)ObjSyokudai_Update,
    (ActorFunc)ObjSyokudai_Draw,
};


// Models at +0x270/+0x274: real virtual calls, slot 1. _ZTI ObjSyokudaiModel is an EQU in shared_syms.s so armlink VFE
// (--vfemode=force, --no_rtti_data) knows the call class (see CLAUDE.md "VFE fix").
extern "C++" {
struct ObjSyokudaiModel {
    virtual void unk_vf0();
    virtual void Destroy();
};
}

void LightContext_RemoveLight(PlayState* play, void* lightCtx, s32 lightNode);

// Init draft: 80/205 words off (shape right; play/alloc callee-saved numbering + scratch regs of the counter %5 and the
// collider/light arg setup differ). Needs actor-dir EQUs (data_syms.s, not kept): sCounter 0x0053585C, sColMaterials
// 0x0053585D, sCylInitStand 0x00535864, sCylInitFlame 0x0053589C, sInitChain 0x005358D4, sObjectIds 0x004DBC44 (rodata u8[]),
// sLitCount 0x005A2EC8 (bss s32[64]). Best variants: block-scoped play2 for Flags_GetSwitch (+14 words), `{ u8* p = ...; }`
// counter block.
#if 0
extern u8 ObjSyokudai_sCounter[];
extern u8 ObjSyokudai_sColMaterials[];
extern u8 ObjSyokudai_sCylInitStand[];
extern u8 ObjSyokudai_sCylInitFlame[];
extern u8 ObjSyokudai_sInitChain[];
extern u8 ObjSyokudai_sObjectIds[];
extern s32 ObjSyokudai_sLitCount[];

s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
void Lights_PointGlowSetInfo(void* info, f32 x, f32 y, f32 z, u8 r, u8 g, u8 b, s16 radius, s32 extra);
s32 LightContext_InsertLight(PlayState* play, void* lightCtx, void* info);
void* ZAR_GetCMBByIndex(void* zar, s32 index);
void FUN_0047d548(SkeletonAnimationModel* model, s32 arg);

void ObjSyokudai_Init(Actor* thisx, PlayState* play) {
    ObjSyokudai* self = (ObjSyokudai*)thisx;
    u32 torchType = ((u32)(u16)self->actor.params << 18) >> 30;
    ZARInfo* zar;
    ZARInfo* zar2;
    ModelAllocator* alloc;
    void* cmb;
    PlayState* play2 = play;

    Actor_ProcessInitChain(&self->actor, (InitChainEntry*)ObjSyokudai_sInitChain);
    ActorShape_Init(&self->actor.shape, 0.0f, NULL, 0.0f);
    { u8* p = ObjSyokudai_sCounter; u8 c = *p; self->unk_280 = c % 5; *p = c + 1; }

    zar = &play->GetObjectStatus(self->actor.objBankIndex)->zarInfo;
    alloc = GetModelManager().allocator;
    alloc->owner = self->actor.unk_178;
    cmb = ZAR_GetCMBByIndex(zar, ObjSyokudai_sObjectIds[torchType]);
    self->model1 = (ObjSyokudaiModel*)alloc->Create(cmb, 1);
    zar2 = &play->GetObjectStatus(Object_GetIndex(&play->objectCtx, 1))->zarInfo;
    cmb = ZAR_GetCMBByIndex(zar2, 0x29);
    self->model2 = (ObjSyokudaiModel*)alloc->Create(cmb, 0);
    self->animModel = ((SkeletonAnimationModel*)self->model2)->unk_0C;
    self->animModel->FUN_00372d94(zar2->GetCMABByIndex(0));
    self->animModel->SetAnimSpeed(2.0f);
    self->animModel->SetAnimType(1);
    FUN_0047d548((SkeletonAnimationModel*)self->model2, 2);
    alloc->owner = NULL;

    Collider_InitCylinder(play, &self->collider1);
    Collider_SetCylinder(play, &self->collider1, &self->actor, (ColliderCylinderInit*)ObjSyokudai_sCylInitStand);
    self->collider1.base.colType = ObjSyokudai_sColMaterials[torchType];

    Collider_InitCylinder(play, &self->collider2);
    Collider_SetCylinder(play, &self->collider2, &self->actor, (ColliderCylinderInit*)ObjSyokudai_sCylInitFlame);

    self->actor.colChkInfo.mass = 0xFF;

    Lights_PointGlowSetInfo(self->unk_258, self->actor.world.pos.x, self->actor.world.pos.y + 70.0f,
                            self->actor.world.pos.z, 255, 255, 180, -1, 0);
    self->lightNode = LightContext_InsertLight(play, (u8*)play + 0xA70, self->unk_258);

    if ((self->actor.params & 0x400) || ((torchType != 2) && Flags_GetSwitch(play2, self->actor.params & 0x3F))) {
        self->litTimer = -1;
    }

    self->flameTexScroll = (s32)(Rand_ZeroOne() * 20.0f);
    self->unk_27F = 0;
    ObjSyokudai_sLitCount[self->actor.params & 0x3F] = 0;
    Actor_SetFocus(&self->actor, 60.0f);
    self->actor.unk_19B = 3;
}
#endif
GLOBAL_ASM("asm/ObjSyokudai_Init.s")

void ObjSyokudai_Destroy(Actor* thisx, PlayState* play) {
    ObjSyokudai* self = (ObjSyokudai*)thisx;

    Collider_DestroyCylinder(play, &self->collider1);
    Collider_DestroyCylinder(play, &self->collider2);
    LightContext_RemoveLight(play, (u8*)play + 0xA70, self->lightNode);

    if (self->model1 != NULL) {
        self->model1->Destroy();
    }
    self->model1 = NULL;

    if (self->model2 != NULL) {
        self->model2->Destroy();
    }
    self->model2 = NULL;
}

GLOBAL_ASM("asm/ObjSyokudai_Update.s")

#if 0
void FUN_00330b98(void* mgr, SkeletonAnimationModel* model, s32 flag);
void FUN_0036c258(f32* sinOut, f32* cosOut, f32 angle);
void FUN_0036c174(MTX34* dst, MTX34* a, MTX34* b);
void FUN_003283a0(MTX34* dst, MTX34* src, Vec3f* scale);
void Matrix_UpdatePosition(MTX34* dst, MTX34* src, Vec3f* translate);
s16 Camera_GetCamDirYaw(Camera* camera);
u32 PauseContext_GetState(void);

void ObjSyokudai_Draw(Actor* thisx, PlayState* play) {
    ObjSyokudai* self = (ObjSyokudai*)thisx;
    s32 timerMax = (((u32)(u16)self->actor.params << 22) >> 28) * 75 + 150;
    MTX34 mtx;
    Vec3f t;
    Vec3f sc;
    f32 flameScale;

    FUN_00357fd0(GET_PLAYER(play), self->actor.unk_178, &self->actor.world.pos);
    ((SkeletonAnimationModel*)self->model1)->FUN_003721e0(&self->actor.modelMtx);
    ((SkeletonAnimationModel*)self->model1)->SetUnkAC(1);
    FUN_00330b98(GetModelManager().unk_180, (SkeletonAnimationModel*)self->model1, 0);

    if (self->litTimer != 0) {
        flameScale = 1.0f;
        if (self->litTimer > timerMax) {
            flameScale = (timerMax - self->litTimer + 15) * (1.0f / 15.0f);
        } else if ((self->litTimer > 0) && (self->litTimer < 30)) {
            flameScale = self->litTimer * (1.0f / 30.0f);
        }
        t.x = 0.0f;
        t.y = 52.0f;
        t.z = 0.0f;
        Matrix_UpdatePosition(&mtx, &self->actor.modelMtx, &t);
        {
            MTX34 rot;
            f32 sinA;
            f32 cosA;
            f32 angle = (s16)(Camera_GetCamDirYaw(play->cameraPtrs[play->activeCamera]) - self->actor.shape.rot.y) * 0.0000958738f + 3.14159274f;
            FUN_0036c258(&sinA, &cosA, angle);
            (void)rot;
            (void)sinA;
            (void)cosA;
        }
        sc.x = sc.y = sc.z = flameScale * 0.0027f;
        FUN_003283a0(&mtx, &mtx, &sc);
        ((SkeletonAnimationModel*)self->model2)->FUN_003721e0(&mtx);
        ((SkeletonAnimationModel*)self->model2)->SetUnkAC(1);
        self->animModel->SetAnimSpeed(PauseContext_GetState() == 0 ? 2.0f : 1.0f);
        FUN_00330b98(GetModelManager().unk_180, (SkeletonAnimationModel*)self->model2, 0);
    }
}
#endif
GLOBAL_ASM("asm/ObjSyokudai_Draw.s")


}
