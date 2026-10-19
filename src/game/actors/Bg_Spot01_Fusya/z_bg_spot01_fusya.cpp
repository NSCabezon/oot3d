extern "C" {
#include "z_bg_spot01_fusya.hpp"

#define FLAGS 0x80400010

void BgSpot01Fusya_Init(BgSpot01Fusya* self, PlayState* play);
void BgSpot01Fusya_Destroy(BgSpot01Fusya* self, PlayState* play);
void BgSpot01Fusya_Update(BgSpot01Fusya* self, PlayState* play);
void BgSpot01Fusya_Draw(BgSpot01Fusya* self, PlayState* play);

void FUN_003e8674(BgSpot01Fusya* self, PlayState* play); // N64: func_808AAA50
void FUN_0036ef10(Vec3f* pos, u32 sfxId, f32 freq); // N64: func_800F436C

ActorInit Bg_Spot01_Fusya_InitVars = {
    ACTOR_BG_SPOT01_FUSYA,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SPOT01_OBJECTS,
    sizeof(BgSpot01Fusya),
    (ActorFunc)BgSpot01Fusya_Init,
    (ActorFunc)BgSpot01Fusya_Destroy,
    (ActorFunc)BgSpot01Fusya_Update,
    (ActorFunc)BgSpot01Fusya_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 12800, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 1300, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1300, ICHAIN_STOP),
};

void BgSpot01Fusya_Init(BgSpot01Fusya* self, PlayState* play) {
    Actor_ProcessInitChain(&self->actor, sInitChain);
    FUN_00372f38(&self->actor, play, &self->skelAnimModel, 0, 0);
    self->unk_1AC = 100.0f;
    self->unk_1B0 = 100.0f;
    self->unk_1B4 = 0.5f;
    if (gSaveContext.sceneSetupIndex < 4) {
        gSaveContext.eventChkInf[6] &= ~0x20;
    }
    self->actionFunc = FUN_003e8674;
}

void BgSpot01Fusya_Destroy(BgSpot01Fusya* self, PlayState* play) {
    FUN_00350f34(&self->actor, &self->skelAnimModel, 0);
}

void FUN_003e8674(BgSpot01Fusya* self, PlayState* play) {
    if (gSaveContext.eventChkInf[6] & 0x20) {
        self->unk_1B0 = 1800.0f;
    }
    f32 temp;
    Actor* actor = &self->actor;

    actor->shape.rot.z += self->unk_1AC;
    temp = ((self->unk_1AC - 100.0f) / 1700.0f) + 1.0f;
    FUN_0036ef10(&actor->world.pos, 0x10001DC, temp);
    Math_ApproachF(&self->unk_1AC, self->unk_1B0, self->unk_1B4, 100.0f);
}

void BgSpot01Fusya_Update(BgSpot01Fusya* self, PlayState* play) {
    self->actionFunc(self, play);
}

void BgSpot01Fusya_Draw(BgSpot01Fusya* self, PlayState* play) {
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->actor.modelMtx);
    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&modelMtx);
    self->skelAnimModel->FUN_00372170(0);
}
}
