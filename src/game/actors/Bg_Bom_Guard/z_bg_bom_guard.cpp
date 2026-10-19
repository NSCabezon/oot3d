extern "C" {
#include "z_bg_bom_guard.hpp"

#define FLAGS 0x80400010

void BgBomGuard_Init(Actor* actor, GameState* state);
void BgBomGuard_Destroy(Actor* actor, GameState* state);
void BgBomGuard_Update(Actor* actor, GameState* state);

void FUN_003e68c4(BgBomGuard* self, PlayState* play); // N64: BgBomGuard_UpdateImpl

GLOBAL_ASM("data/z_Bg_Bom_Guard.data.s")

ActorInit Bg_Bom_Guard_InitVars = {
    ACTOR_BG_BOM_GUARD,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_BOWL,
    sizeof(BgBomGuard),
    (ActorFunc)BgBomGuard_Init,
    (ActorFunc)BgBomGuard_Destroy,
    (ActorFunc)BgBomGuard_Update,
    NULL,
};

void BgBomGuard_Init(Actor* actor, GameState* state) {
    BgBomGuard* self = (BgBomGuard*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, 0);
    actor->unk_19A = 1;
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 2), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    self->dyna.actor.scale.x = 1.0f;
    self->dyna.actor.scale.y = 1.0f;
    self->dyna.actor.scale.z = 1.0f;
    self->homePos = self->dyna.actor.world.pos;
    self->actionFunc = FUN_003e68c4;
}

void BgBomGuard_Destroy(Actor* actor, GameState* state) {
    DynaPoly_DeleteBgActor((PlayState*)state, &((PlayState*)state)->colCtx.dyna, ((BgBomGuard*)actor)->dyna.bgId);
}

#if 0
// FUN_003e68c4 (N64: BgBomGuard_UpdateImpl) lives in src/functions/functions_3E0000s.cpp as GLOBAL_ASM.
// NON_MATCHING: this C is identical except for one register (movgt/strbgt r1 vs r2). Also, the original
// symbol region (244 B) contains an unreferenced 0x20-byte function at 0x3e699c after the literal pool
// (mov r1,#0x1d8; ldrh r1,[r1,r0]; cmp r1,#0; ldrne r1,=0x001070ac; strne r1,[r0,#0x1bc]; bx lr),
// which C cannot emit into the same section.
typedef struct {
    /* 0x000 */ Actor actor;
    /* 0x1A4 */ char unk_1A4[0x4D8];
    /* 0x67C */ u8 gameStartStatus;
} EnBomBowlManStub;

extern __pure float _fabsf(float);
static __inline __pure float bbg_fabsf(float f) {
    return _fabsf(f); // __fabsf() gives a worse register allocation
}

void FUN_003e68c4(BgBomGuard* self, PlayState* play) {
    Actor* it = play->actorCtx.actorLists[ACTORCAT_NPC].head;

    self->isActive = false;

    while (it != NULL) {
        if (it->id == ACTOR_EN_BOM_BOWL_MAN) {
            if ((((EnBomBowlManStub*)it)->gameStartStatus != 0) && (bbg_fabsf(play->view.eye.x) > -20.0f) &&
                (bbg_fabsf(play->view.eye.y) > 110.0f)) {
                self->isActive = true;
            }
            break;
        }
        it = it->next;
    }

    if (!self->isActive) {
        // Move under ground
        self->dyna.actor.world.pos.y = sREG(64) + -200.0f;
    } else {
        self->dyna.actor.world.pos.y = 0.0f;
    }
}
#endif

void BgBomGuard_Update(Actor* actor, GameState* state) {
    BgBomGuard* self = (BgBomGuard*)actor;

    self->actionFunc(self, (PlayState*)state);
}
}
