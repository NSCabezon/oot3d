extern "C" {
#include "z_obj_blockstop.hpp"

#define FLAGS 0x00000000

void ObjBlockstop_Init(Actor* actor, GameState* state);
void ObjBlockstop_Destroy(Actor* actor, GameState* state);
void ObjBlockstop_Update(Actor* actor, GameState* state);

// BgCheck_EntityLineTest2 (N64 name); 3DS wrapper calling BgCheck_LineTestImpl, defined in src/game/z_bgcheck.cpp
s32 FUN_003aa608(CollisionContext* colCtx, Vec3f* posA, Vec3f* posB, Vec3f* posResult,
                                           CollisionPoly** poly, s32 chkWall, s32 chkFloor, s32 chkCeiling,
                                           s32 chkOneFace, s32* bgId, Actor* actor);
DynaPolyActor* DynaPoly_GetActor(CollisionContext* colCtx, s32 bgId);

ActorInit Obj_Blockstop_InitVars = {
    ACTOR_OBJ_BLOCKSTOP,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(ObjBlockstop),
    (ActorFunc)ObjBlockstop_Init,
    (ActorFunc)ObjBlockstop_Destroy,
    (ActorFunc)ObjBlockstop_Update,
    NULL,
};

void ObjBlockstop_Init(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;

    if (Flags_GetSwitch(play, actor->params)) {
        Actor_Kill(actor);
    } else {
        actor->world.pos.y += 1.0f;
    }
}

void ObjBlockstop_Destroy(Actor* actor, GameState* state) {
}

void ObjBlockstop_Update(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    DynaPolyActor* dynaPolyActor;
    Vec3f sp4C;
    s32 bgId;

    if (FUN_003aa608(&play->colCtx, &actor->home.pos, &actor->world.pos, &sp4C, &actor->floorPoly, 0, 0, 1, 1,
                     &bgId, actor)) {
        dynaPolyActor = DynaPoly_GetActor(&play->colCtx, bgId);

        if (dynaPolyActor != NULL && dynaPolyActor->actor.id == ACTOR_OBJ_OSHIHIKI) {
            if ((dynaPolyActor->actor.params & 0xF) == 3 || (dynaPolyActor->actor.params & 0xF) == 7) {
                Audio_PlaySoundGeneral(0x1000487, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
            } else {
                Audio_PlaySoundGeneral(0x100048c, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
            }
            Flags_SetSwitch(play, actor->params);
            Actor_Kill(actor);
        }
    }
}


}
