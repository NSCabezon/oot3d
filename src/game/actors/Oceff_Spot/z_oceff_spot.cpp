extern "C" {
#include "z_oceff_spot.hpp"
#include "model_manager.hpp"

#define FLAGS 0x02000010
#define R_UPDATE_RATE SREG(30)

void OceffSpot_Init(Actor* thisx, PlayState* play);
void OceffSpot_Destroy(Actor* actor, PlayState* play);
void OceffSpot_Update(Actor* thisx, PlayState* play);
void OceffSpot_Draw(Actor* actor, PlayState* play);

// Model at +0x1E8: real virtual call, slot 1. _ZTI14OceffSpotModel is an EQU in data_syms.s so armlink VFE
// (--vfemode=force, --no_rtti_data) knows the call class (see Item_Ocarina).
extern "C++" {
struct OceffSpotModel {
    virtual void unk_vf0();
    virtual void Destroy();
};
}

void LightContext_RemoveLight(PlayState* play, void* lightCtx, void* node);
void FUN_0034708c(PlayState* play); // Magic_Reset
void FUN_00367c48(void* arg);

GLOBAL_ASM("data/z_Oceff_Spot.data.s")

GLOBAL_ASM("../src/game/actors/Oceff_Spot/data_syms.s")

extern u16 gTimeSpeed;

ActorInit Oceff_Spot_InitVars = {
    ACTOR_OCEFF_SPOT,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(OceffSpot),
    (ActorFunc)OceffSpot_Init,
    (ActorFunc)OceffSpot_Destroy,
    (ActorFunc)OceffSpot_Update,
    (ActorFunc)OceffSpot_Draw,
};

static inline void OceffSpot_SetupAction(OceffSpot* self, OceffSpotActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void OceffSpot_End(OceffSpot* self, PlayState* play);
void OceffSpot_Wait(OceffSpot* self, PlayState* play);
void OceffSpot_GrowCylinder(OceffSpot* self, PlayState* play);

// Init: 8/201 words off (zar/cmb callee-saved regs r7/r8 swapped; everything else matches). Draft kept as asm.
// Needs OceffSpot_sInitChain EQU (data_syms.s).
#if 0
extern InitChainEntry OceffSpot_sInitChain[]; // 0x00535E30 (EQU in data_syms.s)
void* ZAR_GetCMBByIndex(ZARInfo* zar, s32 index);
void* LightContext_InsertLight(PlayState* play, void* lightCtx, void* info);
void Lights_PointNoGlowSetInfo(void* light, f32 x, f32 y, f32 z, u8 r, u8 g, u8 b, s16 radius, s32 pad);
void FUN_0047d548(SkeletonAnimationModel* model, s32 arg);
void FUN_00367c54(void* camera);
void FUN_00367c60(void* camera, f32 arg);

void OceffSpot_Init(Actor* thisx, PlayState* play) {
    OceffSpot* self = (OceffSpot*)thisx;
    ZARInfo* zar;
    Camera* camera;
    void* cmb;

    Actor_ProcessInitChain(&self->actor, OceffSpot_sInitChain);
    OceffSpot_SetupAction(self, OceffSpot_GrowCylinder);

    Lights_PointNoGlowSetInfo(&self->unk_1A8, (s16)self->actor.world.pos.x, (s16)self->actor.world.pos.y,
                              (s16)self->actor.world.pos.z, 0, 0, 0, 0, 0);
    self->lightNode1 = LightContext_InsertLight(play, (u8*)play + 0xA70, &self->unk_1A8);

    Lights_PointNoGlowSetInfo(&self->unk_1C4, (s16)self->actor.world.pos.x, (s16)self->actor.world.pos.y,
                              (s16)self->actor.world.pos.z, 0, 0, 0, 0, 0);
    self->lightNode2 = LightContext_InsertLight(play, (u8*)play + 0xA70, &self->unk_1C4);
    if (YREG(15) != 0) { // R_SCENE_CAM_TYPE
        thisx->scale.y = 2.4f;
    } else {
        thisx->scale.y = 0.3f;
    }
    self->unk_174 = 0.0f;

    zar = &play->GetObjectStatus(self->actor.objBankIndex)->zarInfo;
    {
        ModelManager& mm = GetModelManager();
        mm.allocator->owner = self->actor.unk_178;
    }
    cmb = ZAR_GetCMBByIndex(zar, 0x2F);
    {
        ModelManager& mm = GetModelManager();
        self->model = mm.allocator->Create(cmb, 1);
    }
    self->model->unk_AD[0] = 0;
    {
        ModelManager& mm = GetModelManager();
        mm.allocator->owner = NULL;
    }
    self->model->unk_0C->FUN_00372d94(zar->GetCMABByIndex(0x1A));
    self->model->unk_0C->SetAnimType(1);
    FUN_0047d548(self->model, 2);
    self->actor.unk_19A = 1;
    camera = Gameplay_GetCamera(play, -1);
    FUN_00367c54(camera);
    FUN_00367c60(camera, *(f32*)((u8*)play + 0x1B0));
    self->unk_1EC = camera;
}
#endif
GLOBAL_ASM("asm/OceffSpot_Init.s")

void OceffSpot_Destroy(Actor* thisx, PlayState* play) {
    OceffSpot* self = (OceffSpot*)thisx;
    Player* player = *(Player**)((u8*)play + 0x20AC); // GET_PLAYER

    LightContext_RemoveLight(play, (u8*)play + 0xA70, self->lightNode1);
    LightContext_RemoveLight(play, (u8*)play + 0xA70, self->lightNode2);
    FUN_0034708c(play);
    if (gSaveContext.nayrusLoveTimer != 0 && *(s32*)((u8*)play + 0x20A8) != 0) {
        *(u8*)((u8*)player + 0x172A) |= 0x40; // stateFlags3 |= PLAYER_STATE3_RESTORE_NAYRUS_LOVE
    }
    if (self->model != NULL) {
        ((OceffSpotModel*)self->model)->Destroy();
        self->model = NULL;
    }
    FUN_00367c48(self->unk_1EC);
}

void Lights_PointNoGlowSetInfo(void* light, f32 x, f32 y, f32 z, u8 r, u8 g, u8 b, s16 radius, s32 pad);
void FUN_0036bee0(PlayState* play, f32 intensity, f32 a, f32 b, f32 c); // Environment_AdjustLights
f32 __hardfp_sinf(f32 x);
f32 __hardfp_cosf(f32 x);

void OceffSpot_Update(Actor* thisx, PlayState* play) {
    OceffSpot* self = (OceffSpot*)thisx;
    PlayState* play2 = play;
    Player* player = *(Player**)((u8*)play2 + 0x20AC); // GET_PLAYER
    f32 temp;

    temp = (1.0f - __hardfp_cosf(self->unk_174 * M_PI)) * 0.5f;
    self->actionFunc(self, play);

    self->actor.scale.z = 0.42f * temp;
    self->actor.scale.x = 0.42f * temp;

    self->actor.world.pos = player->actor.world.pos;
    self->actor.world.pos.y += 5.0f;

    temp = (2.0f - self->unk_174) * self->unk_174;
    FUN_0036bee0(play, temp * 0.5f, 800.0f, 0.0f, 0.9f);

    Lights_PointNoGlowSetInfo(&self->unk_1A8, (s16)self->actor.world.pos.x, (s16)self->actor.world.pos.y + 55.0f,
                              (s16)self->actor.world.pos.z, (s32)(255.0f * temp), (s32)(255.0f * temp),
                              (s32)(200.0f * temp), (s16)(100.0f * temp), 0);

    Lights_PointNoGlowSetInfo(&self->unk_1C4,
                              (s16)self->actor.world.pos.x + Math_SinS(player->actor.shape.rot.y) * 20.0f,
                              (s16)self->actor.world.pos.y + 20.0f,
                              (s16)self->actor.world.pos.z + Math_CosS(player->actor.shape.rot.y) * 20.0f,
                              (s32)(255.0f * temp), (s32)(255.0f * temp), (s32)(200.0f * temp),
                              (s16)(100.0f * temp), 0);
}

// Draw: 3DS sun model (rotated matrix + SkeletonAnimationModel calls).
s16 Camera_GetCamDirYaw(void* camera);

// Row view of the MTX34 with an operator[]: keeps the constant row/column index 1 live in a callee-saved register
// (shared with the SetUnkAC(1) store), which is what gives the ROM's push {r4-r7,lr} frame (see CLAUDE.md SetUnkAC recipe).
extern "C++" {
struct MRow { f32& operator[](s32 i) { return (&a)[i]; } f32 a, b, c, d; };
static inline MRow& R(MTX34* m, s32 r) { return ((MRow*)m)[r]; }
}

void OceffSpot_Draw(Actor* thisx, PlayState* play) {
    OceffSpot* self = (OceffSpot*)thisx;
    MTX34 mtx;
    f32 angle;
    f32 sinVal;
    f32 cosVal;
    f32 x;
    f32 y;
    f32 z;
    void* camera;

    MTX34CopyAsm(&mtx, &self->actor.modelMtx);
    camera = *(void**)((u8*)play + 0xA54 + *(s16*)((u8*)play + 0xA64) * 4);
    angle = (f32)(s16)(Camera_GetCamDirYaw(camera) - thisx->shape.rot.y + 0x8000) * (M_PI / 32768.0f);
    if (angle != 0.0f) {
        sinVal = __hardfp_sinf(angle);
        cosVal = __hardfp_cosf(angle);
        x = R(&mtx,0)[0];
        z = R(&mtx,0)[2];
        R(&mtx,0)[0] = x * cosVal - z * sinVal;
        R(&mtx,0)[2] = x * sinVal + z * cosVal;
        x = R(&mtx,1)[0];
        z = R(&mtx,1)[2];
        R(&mtx,1)[0] = x * cosVal - z * sinVal;
        R(&mtx,1)[2] = x * sinVal + z * cosVal;
        x = R(&mtx,2)[0];
        z = R(&mtx,2)[2];
        R(&mtx,2)[0] = x * cosVal - z * sinVal;
        R(&mtx,2)[2] = x * sinVal + z * cosVal;
    }

    camera = *(void**)((u8*)play + 0xA54 + *(s16*)((u8*)play + 0xA66) * 4);
    if (camera != NULL && *(u16*)((u8*)camera + 0x18A) == 0x19) {
        angle = -M_PI / 12.0f;
        sinVal = __hardfp_sinf(angle);
        cosVal = __hardfp_cosf(angle);
        y = R(&mtx,0)[1];
        z = R(&mtx,0)[2];
        R(&mtx,0)[1] = y * cosVal + z * sinVal;
        R(&mtx,0)[2] = z * cosVal - y * sinVal;
        y = R(&mtx,1)[1];
        z = R(&mtx,1)[2];
        R(&mtx,1)[1] = y * cosVal + z * sinVal;
        R(&mtx,1)[2] = z * cosVal - y * sinVal;
        y = R(&mtx,2)[1];
        z = R(&mtx,2)[2];
        R(&mtx,2)[1] = y * cosVal + z * sinVal;
        R(&mtx,2)[2] = z * cosVal - y * sinVal;
    }

    self->model->SetUnkAC(1);
    self->model->FUN_003721e0(&mtx);
    self->model->unk_AD[0] = 1;
    if (PauseContext_GetState() == 0) {
        self->model->unk_0C->SetAnimSpeed(1.0f);
    } else {
        self->model->unk_0C->SetAnimSpeed(0.0f);
    }
    self->model->FUN_00372170(0);
}

void OceffSpot_End(OceffSpot* self, PlayState* play) {
    if (self->unk_174 > 0.0f) {
        self->unk_174 -= (f32)R_UPDATE_RATE * (1.0f / 3.0f) * 0.05f;
    } else {
        self->unk_174 = 0.0f;
        Actor_Kill(&self->actor);
        if (gTimeSpeed != 400 && *(u16*)((u8*)play + 0x2B9E) == 0 && (gSaveContext.eventInf[0] & 0xF) != 1) {
            if (*(u16*)((u8*)play + 0x2B80) != 0x31 || *(u16*)((u8*)play + 0x2B7E) != 8) {
                *(u16*)((u8*)&gSaveContext + 0x15B0) = 1; // sunsSongState = SUNSSONG_START
            }
        } else {
            *(u16*)((u8*)play + 0x2B7E) = 4; // ocarinaMode = OCARINA_MODE_04
        }
    }
}

void OceffSpot_GrowCylinder(OceffSpot* self, PlayState* play) {
    if (self->unk_174 < 1.0f) {
        self->unk_174 += (f32)R_UPDATE_RATE * (1.0f / 3.0f) * 0.05f;
    } else {
        OceffSpot_SetupAction(self, OceffSpot_Wait);
        self->timer = (u16)(3.0f / R_UPDATE_RATE * 60.0f + 0.5f);
        self->unk_174 = 1.0f;
    }
}

void OceffSpot_Wait(OceffSpot* self, PlayState* play) {
    if (self->timer > 0) {
        self->timer--;
    } else {
        OceffSpot_SetupAction(self, OceffSpot_End);
    }
}

}
