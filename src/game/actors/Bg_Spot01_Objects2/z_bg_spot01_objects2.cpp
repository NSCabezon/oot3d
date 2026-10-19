extern "C" {
#include "z_bg_spot01_objects2.hpp"

#define FLAGS 0x00400010
#define PARAMS_GET_U(params, shift, count) (((params) >> (shift)) & ((1 << (count)) - 1))

s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
s32 Object_IsLoaded(ObjectContext* objectCtx, s32 bankIndex);

// FUN_00372f38 and FUN_00350f34 are variadic: model pointers are followed by their indices, ended by 0

void BgSpot01Objects2_Init(BgSpot01Objects2* self, PlayState* play);
void BgSpot01Objects2_Destroy(BgSpot01Objects2* self, PlayState* play);
void BgSpot01Objects2_Update(BgSpot01Objects2* self, PlayState* play);

void FUN_00258cd4(BgSpot01Objects2* self, GameState* state); // N64: func_808AC2BC
void FUN_003e8c74(BgSpot01Objects2* self, PlayState* play); // N64: func_808AC474 (empty)

ActorInit Bg_Spot01_Objects2_InitVars = {
    ACTOR_BG_SPOT01_OBJECTS2,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(BgSpot01Objects2),
    (ActorFunc)BgSpot01Objects2_Init,
    (ActorFunc)BgSpot01Objects2_Destroy,
    (ActorFunc)BgSpot01Objects2_Update,
    NULL,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 12800, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 2000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1500, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgSpot01Objects2_Init(BgSpot01Objects2* self, PlayState* play) {
    switch (self->dyna.actor.params & 7) {
        case 0:
        case 1:
        case 2:
            self->objectId = OBJECT_SPOT01_MATOYA;
            break;
        case 3:
            self->objectId = OBJECT_SPOT01_MATOYAB;
            break;
        case 4:
            self->objectId = OBJECT_SPOT01_MATOYA;
    }

    if (self->objectId >= 0) {
        self->requiredObjectSlot = Object_GetIndex(&play->objectCtx, (s16)self->objectId);
        if (self->requiredObjectSlot < 0) {
            Actor_Kill(&self->dyna.actor);
            return;
        }
    } else {
        Actor_Kill(&self->dyna.actor);
    }
    self->skelAnimModel[0] = NULL;
    self->skelAnimModel[1] = NULL;
    self->skelAnimModel[2] = NULL;
    self->skelAnimModel[3] = NULL;
    self->skelAnimModel[4] = NULL;
    self->actionFunc = (BgSpot01Objects2ActionFunc)FUN_00258cd4;
    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
}

void BgSpot01Objects2_Destroy(BgSpot01Objects2* self, PlayState* play) {
    FUN_00350f34(&self->dyna.actor, &self->skelAnimModel[0], &self->skelAnimModel[1],
                 &self->skelAnimModel[2], &self->skelAnimModel[3], &self->skelAnimModel[4], 0);
}

void BgSpot01Objects2_Update(BgSpot01Objects2* self, PlayState* play) {
    self->actionFunc(self, play);
}

void FUN_0038b8d4(BgSpot01Objects2* self, PlayState* play) {
    MTX34 modelMtx;
    s32 idx;

    MTX34CopyAsm(&modelMtx, &self->dyna.actor.modelMtx);
    idx = PARAMS_GET_U(self->dyna.actor.params, 0, 3);
    self->skelAnimModel[idx]->SetUnkAC(1);
    self->skelAnimModel[idx]->FUN_003721e0(&modelMtx);
    self->skelAnimModel[idx]->FUN_00372170(0);
}

static s32 BgSpot01Objects2_GetPathPoint(Path* pathList, Vec3f* pos, s32 path, s32 waypoint) {
    Vec3s* pointPos = &(pathList + path)->points[waypoint];

    pos->x = pointPos->x;
    pos->y = pointPos->y;
    pos->z = pointPos->z;
    return 0;
}

// GameState* param + local PlayState* cast is needed for the switch index to land in r1
void FUN_00258cd4(BgSpot01Objects2* self, GameState* state) {
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;
    Actor* thisx = &self->dyna.actor;
    Vec3f position;

    if (Object_IsLoaded(&play->objectCtx, self->requiredObjectSlot)) {
        self->dyna.actor.objBankIndex = self->requiredObjectSlot;
        DynaPolyActor_Init(&self->dyna, 1);

        switch (self->dyna.actor.params & 7) {
            case 0:
            case 1:
            case 2:
                FUN_00372f38(thisx, play, &self->skelAnimModel[0], 3, &self->skelAnimModel[1],
                             1, &self->skelAnimModel[2], 4, 0);
                break;
            case 4:
                FUN_00372f38(thisx, play, &self->skelAnimModel[4], 0, 0);
                ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 0), &colHeader);
                self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
                break;
            case 3:
                FUN_00372f38(thisx, play, &self->skelAnimModel[3], 0, 0);
                ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 0), &colHeader);
                self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
                if (IS_DAY) {
                    BgSpot01Objects2_GetPathPoint(play->setupPathList, &position,
                                                  PARAMS_GET_U((s32)thisx->params, 8, 8), 0);
                    // passing `state` (not `play`) is what orders the r2/r1 argument moves as in the ROM
                    Actor_SpawnAsChild(&play->actorCtx, thisx, (PlayState*)state, ACTOR_EN_DAIKU_KAKARIKO,
                                       position.x, position.y, position.z, thisx->world.rot.x, thisx->world.rot.y, thisx->world.rot.z,
                                       (PARAMS_GET_U((s32)thisx->params, 8, 8) << 8) + 1);
                }
                break;
        }

        self->dyna.actor.draw = (ActorFunc)FUN_0038b8d4;
        self->actionFunc = FUN_003e8c74;
    }
}

void FUN_003e8c74(BgSpot01Objects2* self, PlayState* play) {
}
}
