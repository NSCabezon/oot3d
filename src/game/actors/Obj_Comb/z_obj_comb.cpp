extern "C" {
#include "z_obj_comb.hpp"

#define FLAGS 0x80000000

void ObjComb_Init(Actor* thisx, PlayState* play);
void ObjComb_Destroy(Actor* thisx, GameState* state);
void ObjComb_Update(Actor* thisx, PlayState* play);
void ObjComb_Draw(Actor* actor, GameState* state);
void ObjComb_Wait(ObjComb* self, PlayState* play);

void Collider_InitJntSph(PlayState* play, ObjCombColliderJntSph* collider);
void Collider_SetJntSph(PlayState* play, ObjCombColliderJntSph* collider, Actor* actor, void* init,
                        ObjCombColliderElement* elements);
void Collider_DestroyJntSph(PlayState* play, ObjCombColliderJntSph* collider);

GLOBAL_ASM("data/z_Obj_Comb.data.s")

// Data lives in the original .data at 0x00534F14 (data/z_Obj_Comb.data.s); named by EQU symbols in data_syms.s.
extern InitChainEntry sObjCombInitChain[];
extern u8 sObjCombJntSphInit[];

GLOBAL_ASM("../src/game/actors/Obj_Comb/data_syms.s")

ActorInit Obj_Comb_InitVars = {
    ACTOR_OBJ_COMB,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_FIELD_KEEP,
    sizeof(ObjComb),
    (ActorFunc)ObjComb_Init,
    (ActorFunc)ObjComb_Destroy,
    (ActorFunc)ObjComb_Update,
    (ActorFunc)ObjComb_Draw,
};

void ObjComb_Init(Actor* thisx, PlayState* play) {
    ObjComb* self = (ObjComb*)thisx;

    Actor_ProcessInitChain(&self->actor, sObjCombInitChain);
    FUN_00372f38(&self->actor, play, &self->skelAnimModel, 9, 0);
    thisx->unk_19B = 2;
    Collider_InitJntSph(play, &self->collider);
    Collider_SetJntSph(play, &self->collider, &self->actor, sObjCombJntSphInit, self->colliderElements);
    self->actionFunc = ObjComb_Wait;
}

void ObjComb_Destroy(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    ObjComb* self = (ObjComb*)thisx;

    Collider_DestroyJntSph(play, &self->collider);
    FUN_00350f34(thisx, &self->skelAnimModel, 0);
}

void ObjComb_Update(Actor* thisx, PlayState* play) {
    ObjComb* self = (ObjComb*)thisx;

    self->unk_21A += 0x2EE0;
    self->actionFunc(self, play);
    thisx->shape.rot.x = Math_SinS(self->unk_21A) * self->unk_218 + thisx->home.rot.x;
}

GLOBAL_ASM("asm/ObjComb_Draw.s")

GLOBAL_ASM("asm/ObjComb_Wait.s")

}
