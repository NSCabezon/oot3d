extern "C" {
#include "z_en_ex_ruppy.hpp"
#include "../En_Diving_Game/z_en_diving_game.hpp"

#define FLAGS 0x80000010

void EnExRuppy_Init(Actor* actor, GameState* state);
void EnExRuppy_Destroy(Actor* actor, GameState* state);
void EnExRuppy_Update(EnExRuppy* self, PlayState* play);
void EnExRuppy_Draw(Actor* actor, GameState* state);
void EnExRuppy_WaitInGame(EnExRuppy* self, PlayState* play);
void EffectSsGSplash_Spawn(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel, s16 type, s16 scale);

extern s16 EnExRuppy_sCollectibleTypes[5];
GLOBAL_ASM("../src/game/actors/En_Ex_Ruppy/data_syms.s")

GLOBAL_ASM("data/z_En_Ex_Ruppy.data.s")

ActorInit En_Ex_Ruppy_InitVars = {
    ACTOR_EN_EX_RUPPY,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnExRuppy),
    (ActorFunc)EnExRuppy_Init,
    (ActorFunc)EnExRuppy_Destroy,
    (ActorFunc)EnExRuppy_Update,
    (ActorFunc)EnExRuppy_Draw,
};

GLOBAL_ASM("asm/EnExRuppy_Init.s")

void EnExRuppy_Destroy(Actor* actor, GameState* state) {
    EnExRuppy* self = (EnExRuppy*)actor;

    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

// Best Update draft: 3/27 words differ, timer temporary register allocation.
#if 0
void EnExRuppy_Update(EnExRuppy* self, PlayState* play) {
    self->actor.shape.rot.y += 0x51B;
    self->actionFunc(self, play);
    if (self->timer != 0) {
        self->timer--;
    }
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(play, &self->actor, 20.0f, 20.0f, 50.0f, 0x1C);
}
#endif
GLOBAL_ASM("asm/EnExRuppy_Update.s")

GLOBAL_ASM("asm/EnExRuppy_Draw.s")

GLOBAL_ASM("asm/EnExRuppy_DropIntoWater.s")

GLOBAL_ASM("asm/EnExRuppy_EnterWater.s")

void EnExRuppy_GalleryTarget(EnExRuppy* self, PlayState* play) {
    if (self->galleryFlag) {
        Math_ApproachF(&self->actor.shape.yOffset, 700.0f, 0.5f, 200.0f);
    } else {
        Math_ApproachF(&self->actor.shape.yOffset, -700.0f, 0.5f, 200.0f);
    }
}

void EnExRuppy_Kill(EnExRuppy* self, PlayState* play) {
    self->invisible++;
    self->invisible &= 1;
    if (self->timer == 0) {
        Actor_Kill(&self->actor);
    }
}

void EnExRuppy_Sink(EnExRuppy* self, PlayState* play) {
    EnDivingGame* divingGame;
    Vec3f pos;

    if ((self->actor.bgCheckFlags & 0x20) && (self->actor.yDistToWater > 15.0f)) {
        pos = self->actor.world.pos;
        pos.y += self->actor.yDistToWater;
        self->actor.velocity.y = -1.0f;
        self->actor.gravity = -0.2f;
        EffectSsGSplash_Spawn(play, &pos, NULL, NULL, 0, 800);
        Audio_PlaySoundGeneral(0x0100016E, &self->actor.world.pos, 4,
                              &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        self->actionFunc = EnExRuppy_WaitInGame;
    }
    divingGame = (EnDivingGame*)self->actor.parent;
    if ((divingGame != NULL) && (divingGame->actor.update != NULL) && (divingGame->phase == 0)) {
        self->timer = 30;
        self->actionFunc = EnExRuppy_Kill;
    }
}

static inline void EnExRuppy_CollectInRange(EnExRuppy* self, PlayState* play, f32 collectRadius) {
    if (self->actor.xyzDistToPlayerSq < SQ(collectRadius)) {
        Audio_PlaySoundGeneral(0x01000488, NULL, 4,
                              &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        Item_DropCollectible(play, &self->actor.world.pos, EnExRuppy_sCollectibleTypes[self->colorIdx] | 0x8000);
        Actor_Kill(&self->actor);
    }
}

void EnExRuppy_WaitAsCollectible(EnExRuppy* self, PlayState* play) {
    EnExRuppy_CollectInRange(self, play, 30.0f);
}

GLOBAL_ASM("asm/EnExRuppy_WaitInGame.s")

GLOBAL_ASM("asm/EnExRuppy_WaitToBlowUp.s")

}
