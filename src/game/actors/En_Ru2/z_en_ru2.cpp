extern "C" {
#include "z_en_ru2.hpp"

#define FLAGS 0x00000010

void EnRu2_Init(Actor* actor, GameState* state);
void EnRu2_Destroy(Actor* actor, GameState* state);
void EnRu2_Update(Actor* actor, GameState* state);
void EnRu2_Draw(Actor* actor, GameState* state);

// N64 tables and data that have no symbol in the scatter file (absolute symbols in syms.s).
extern EnRu2ActionFunc sEnRu2ActionFuncs[];
extern EnRu2DrawFunc sEnRu2DrawFuncs[];
extern ColliderCylinderInit sEnRu2CylinderInit;
extern char sEnRu2AnimIdle[];
extern char sEnRu2AnimCrossingArms[];
extern char sEnRu2AnimHandsOnHips[];

GLOBAL_ASM("data/z_En_Ru2.data.s")
GLOBAL_ASM("../src/game/actors/En_Ru2/syms.s")

ActorInit En_Ru2_InitVars = {
    ACTOR_EN_RU2,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_RU2,
    sizeof(EnRu2),
    (ActorFunc)EnRu2_Init,
    (ActorFunc)EnRu2_Destroy,
    (ActorFunc)EnRu2_Update,
    (ActorFunc)EnRu2_Draw,
};

#if 0 // Haiku draft, does not match yet: 528 B vs 552 B, register allocation differs (ROM keeps
// play in r5 and the shape ptr in r7, and merges the case-2/3 shadowAlpha tail). Tried 7 variants.
void* ZAR_GetCMBByIndex(void* zar, s32 idx);
void FUN_00353e78(void* zar, PlayState* play, SkelAnime* skelAnime, void* cmb, void* p, s32 a, void* b, void* c, s32 d);
void FUN_00347ed4(Actor* actor, void* anim, s32 mode, f32 morphFrames, s32 direction);
void Collider_SetCylinderType1(PlayState* play, ColliderCylinder* collider, Actor* actor, ColliderCylinderInit* init);


static inline void EnRu2_InitWaterTrial(EnRu2* self) {
    FUN_00347ed4(&self->actor, sEnRu2AnimCrossingArms, 2, 0.0f, 0);
    self->action = 7;
}

static inline void EnRu2_InitCredits(EnRu2* self) {
    FUN_00347ed4(&self->actor, sEnRu2AnimHandsOnHips, 0, 0.0f, 0);
    self->action = 10;
    self->drawConfig = 0;
}

static inline void EnRu2_InitWaterTempleEncounter(EnRu2* self) {
    FUN_00347ed4(&self->actor, sEnRu2AnimIdle, 0, 0.0f, 0);
    self->action = 14;
    self->drawConfig = 1;
}

static inline void EnRu2_InitChamberOfSages(EnRu2* self) {
    FUN_00347ed4(&self->actor, sEnRu2AnimIdle, 0, 0.0f, 0);
    self->actor.shape.yOffset = -9984.0f;
}

void EnRu2_Init(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnRu2* self = (EnRu2*)actor;

    u8* zarBase;
    void* zar;
    void* cmb;
    s32 type;

    ActorShape* shape = &actor->shape;

    ActorShape_Init(shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinderType1(play, &self->collider, actor, &sEnRu2CylinderInit);
    if ((u8)actor->objBankIndex < 0x13) {
        zarBase = (u8*)play + ((u8)actor->objBankIndex << 7);
        if (*(s32*)(zarBase + 0x3A64) != 0) {
            zarBase += 0x3A5C;
        }
    } else {
        zarBase = NULL;
    }
    zar = zarBase + 0x10;
    cmb = ZAR_GetCMBByIndex(zar, 0);
    FUN_00353e78(zar, play, &self->skelAnime, cmb, actor->unk_178, -1, self->unk_228, self->unk_708, 0x18);
    SkelAnime_SetFaceAnimations(&self->faceAnims, &self->skelAnime, 0, -1, -1);

    type = actor->params & 0xFF;
    if (type == 2 || type == 3) {
        if (type == 2) {
            FUN_00347ed4(actor, sEnRu2AnimCrossingArms, 2, 0.0f, 0);
            self->action = 7;
        } else {
            FUN_00347ed4(actor, sEnRu2AnimHandsOnHips, 0, 0.0f, 0);
            self->action = 10;
            self->drawConfig = 0;
        }
        shape->shadowAlpha = 0;
    } else if (type == 4) {
        if (Flags_GetSwitch(play, ((u32)actor->params << 16) >> 24)) {
            Actor_Kill(actor);
        } else {
            FUN_00347ed4(actor, sEnRu2AnimIdle, 0, 0.0f, 0);
            self->action = 14;
            self->drawConfig = 1;
        }
    } else {
        FUN_00347ed4(actor, sEnRu2AnimIdle, 0, 0.0f, 0);
        shape->yOffset = -9984.0f;
    }
    self->unk_DD2 = 0;
    self->unk_DD3 = 3;
}

#endif
GLOBAL_ASM("asm/EnRu2_Init.s")

void EnRu2_Destroy(Actor* actor, GameState* state) {
    EnRu2* self = (EnRu2*)actor;

    Collider_DestroyCylinder((PlayState*)state, &self->collider);
    SkelAnime_Free2(&self->skelAnime);
    self->faceAnims.Destroy();
}

void EnRu2_Update(Actor* actor, GameState* state) {
    EnRu2* self = (EnRu2*)actor;

    if ((u32)self->action < 0x14) {
        EnRu2ActionFunc func = sEnRu2ActionFuncs[self->action];

        if (func != NULL) {
            func(self, (PlayState*)state);
        }
    }
}

void EnRu2_Draw(Actor* actor, GameState* state) {
    EnRu2* self = (EnRu2*)actor;
    s32 drawConfig = self->drawConfig;

    if ((u32)drawConfig < 3) {
        EnRu2DrawFunc func = sEnRu2DrawFuncs[drawConfig];

        if (func != NULL) {
            func(self, (PlayState*)state);
        }
    }
}
}
