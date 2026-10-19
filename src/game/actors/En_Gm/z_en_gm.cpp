extern "C" {
#include "z_en_gm.hpp"

#define FLAGS 0x00000019

void EnGm_Init(Actor* actor, GameState* state);
void EnGm_Destroy(Actor* actor, GameState* state);
void EnGm_Update(Actor* actor, GameState* state);

void FUN_003b3c2c(EnGm* self, PlayState* play); // N64: func_80A3D838 (shared code outside this file)

ActorInit En_Gm_InitVars = {
    ACTOR_EN_GM,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_GM,
    sizeof(EnGm),
    (ActorFunc)EnGm_Init,
    (ActorFunc)EnGm_Destroy,
    (ActorFunc)EnGm_Update,
    NULL,
};

// 0x00525938 (previously GLOBAL_ASM("data/z_En_Gm.data.s")). On 3DS the init chain precedes the cylinder init.
static InitChainEntry sInitChain[] = {
    ICHAIN_U8(targetMode, 5, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 30, ICHAIN_STOP),
};

ColliderCylinderInit En_Gm_sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        COLSHAPE_CYLINDER, // N64 ColliderCylinderInitType1 (no oc2 byte): shape lands in the oc2 slot
        0,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 100, 120, 0, { 0, 0, 0 } },
};

void EnGm_Init(Actor* actor, GameState* state) {
    EnGm* self = (EnGm*)actor;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    self->actionFunc = FUN_003b3c2c;
}

void EnGm_Destroy(Actor* actor, GameState* state) {
    EnGm* self = (EnGm*)actor;

    Collider_DestroyCylinder((PlayState*)state, &self->collider);
    SkelAnime_Free2(&self->skelAnime);
    self->faceAnims.Destroy();
}

void EnGm_Update(Actor* actor, GameState* state) {
    EnGm* self = (EnGm*)actor;
    PlayState* play = (PlayState*)state;

    self->actionFunc(self, play);
}
}
