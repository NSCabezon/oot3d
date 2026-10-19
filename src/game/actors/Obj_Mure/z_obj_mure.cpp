extern "C" {
#include "z_obj_mure.hpp"

#define FLAGS 0x00000000

void ObjMure_Init(Actor* actor, GameState* state);
void ObjMure_Destroy(Actor* actor, GameState* state);
void ObjMure_Update(Actor* actor, GameState* state);

void FUN_0015eed0(ObjMure* self, PlayState* play); // N64: ObjMure_InitialAction
void FUN_003bc5dc(ObjMure* self, PlayState* play); // N64: ObjMure_CulledState
void FUN_003cc298(ObjMure* self, PlayState* play); // N64: ObjMure_ActiveState
void FUN_001a25b4(ObjMure* self, PlayState* play); // N64: ObjMure_GroupBehavior0
void FUN_0015e750(ObjMure* self, PlayState* play); // N64: ObjMure_GroupBehavior1
void FUN_00352aa8(ObjMure* self, f32 randMax);     // N64: ObjMure_SetFollowTargets
void FUN_00343f44(ObjMure* self, s32 idx1);        // N64: ObjMure_SetChildToFollowPlayer

#define R_UPDATE_RATE SREG(30)
#define FRAMES(n) ((n) * 3.0f / R_UPDATE_RATE + 0.5f)

ActorInit Obj_Mure_InitVars = {
    ACTOR_OBJ_MURE,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(ObjMure),
    (ActorFunc)ObjMure_Init,
    (ActorFunc)ObjMure_Destroy,
    (ActorFunc)ObjMure_Update,
    NULL,
};

static f32 sZClip[] = { 1600.0f, 1600.0f, 1000.0f, 1000.0f, 1000.0f };

static s32 sMaxChildSpawns[] = { 12, 9, 8, 0 };

static s16 sSpawnActorIds[] = { ACTOR_EN_KUSA, 0, ACTOR_EN_FISH, ACTOR_EN_INSECT, ACTOR_EN_BUTTE };

static s16 sSpawnParams[] = { 0, 2, -1, 0, -1 };

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 1200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1200, ICHAIN_STOP),
};

static ObjMureActionFunc sTypeGroupBehaviorFunc[] = {
    NULL, NULL, FUN_001a25b4, FUN_001a25b4, FUN_0015e750,
};

void ObjMure_Init(Actor* actor, GameState* state) {
    ObjMure* self = (ObjMure*)actor;

    self->chNum = (actor->params >> 12) & 0xF;
    self->ptn = (actor->params >> 8) & 7;
    self->svNum = (actor->params >> 5) & 3;
    self->type = actor->params & 0x1F;

    if (self->ptn >= 4 || self->type >= 5) {
        Actor_Kill(actor);
        return;
    }
    switch (self->type) {
        case 2:
        case 3:
        case 4:
            Actor_ProcessInitChain(actor, sInitChain);
            break;
        default:
            Actor_Kill(actor);
            return;
    }
    self->actionFunc = FUN_0015eed0;
}

void ObjMure_Destroy(Actor* actor, GameState* state) {
}

void FUN_0015eed0(ObjMure* self, PlayState* play) {
    self->actionFunc = FUN_003bc5dc;
}

static inline s32 ObjMure_GetMaxChildSpawns(ObjMure* self) {
    if (self->chNum == 0) {
        return sMaxChildSpawns[self->ptn];
    }
    return self->chNum;
}

static inline void ObjMure_GetSpawnPos(Vec3f* outPos, Vec3f* inPos, s32 ptn, s32 idx) {
    *outPos = *inPos;
}

static inline void ObjMure_SpawnActors0(ObjMure* self, PlayState* play2) {
    Actor* actor = &self->actor;
    s32 i;
    Vec3f pos;
    s32 maxChildren = ObjMure_GetMaxChildSpawns(self);
    // play2/play split (as in N64 SpawnActors1) keeps `play` in r1 at entry instead of fp.
    PlayState* play = play2;

    for (i = 0; i < maxChildren; i++) {
        switch (self->childrenStates[i]) {
            case 1:
                break;
            case 2:
                ObjMure_GetSpawnPos(&pos, &actor->world.pos, self->ptn, i);
                self->children[i] =
                    Actor_Spawn(&play2->actorCtx, play, sSpawnActorIds[self->type], pos.x, pos.y, pos.z,
                                actor->world.rot.x, actor->world.rot.y, actor->world.rot.z, sSpawnParams[self->type], 1);
                if (self->children[i] != NULL) {
                    self->children[i]->flags |= 0x800;
                    self->children[i]->room = actor->room;
                }
                break;
            default:
                ObjMure_GetSpawnPos(&pos, &actor->world.pos, self->ptn, i);
                self->children[i] =
                    Actor_Spawn(&play2->actorCtx, play, sSpawnActorIds[self->type], pos.x, pos.y, pos.z,
                                actor->world.rot.x, actor->world.rot.y, actor->world.rot.z, sSpawnParams[self->type], 1);
                if (self->children[i] != NULL) {
                    self->children[i]->room = actor->room;
                }
                break;
        }
    }
}

// By-value copy helper: makes armcc keep &spawnPos (not &actor->world.pos) in a register across the loop.
static inline Vec3f ObjMure_PosV(Vec3f* p) {
    return *p;
}

static inline void ObjMure_SpawnActors1(ObjMure* self, PlayState* play2) {
    PlayState* play = play2;
    Actor* actor = &self->actor;
    Vec3f spawnPos;
    s32 maxChildren = ObjMure_GetMaxChildSpawns(self);
    s32 i;

    for (i = 0; i < maxChildren; i++) {
        spawnPos = ObjMure_PosV(&actor->world.pos);
        self->children[i] = Actor_Spawn(&play2->actorCtx, play, sSpawnActorIds[self->type], spawnPos.x, spawnPos.y,
                                        spawnPos.z, actor->world.rot.x, actor->world.rot.y, actor->world.rot.z,
                                        (self->type == 4 && i == 0) ? 1 : sSpawnParams[self->type], 1);
        if (self->children[i] != NULL) {
            self->childrenStates[i] = 0;
            self->children[i]->room = actor->room;
        } else {
            self->childrenStates[i] = 1;
        }
    }
}

static inline void ObjMure_SpawnActors(ObjMure* self, PlayState* play) {
    switch (self->svNum) {
        case 0:
            ObjMure_SpawnActors0(self, play);
            break;
        case 1:
            ObjMure_SpawnActors1(self, play);
            break;
    }
}

void FUN_003bc5dc(ObjMure* self, PlayState* play) {
    if (__fabsf(self->actor.unk_EC.z) < sZClip[self->type]) {
        self->actionFunc = FUN_003cc298;
        self->actor.flags |= 0x10;
        ObjMure_SpawnActors(self, play);
    }
}

static inline void ObjMure_KillActorsImpl(ObjMure* self, PlayState* play) {
    s32 maxChildren = ObjMure_GetMaxChildSpawns(self);
    s32 i;

    for (i = 0; i < maxChildren; i++) {
        switch (self->childrenStates[i]) {
            case 1:
                self->children[i] = NULL;
                break;
            case 2:
                if (self->children[i] != NULL) {
                    Actor_Kill(self->children[i]);
                    self->children[i] = NULL;
                }
                break;
            default:
                if (self->children[i] != NULL) {
                    if (Actor_HasParent(self->children[i], play)) {
                        self->children[i] = NULL;
                    } else {
                        Actor_Kill(self->children[i]);
                        self->children[i] = NULL;
                    }
                }
                break;
        }
    }
}

static inline void ObjMure_KillActors(ObjMure* self, PlayState* play) {
    ObjMure_KillActorsImpl(self, play);
}

static inline void ObjMure_CheckChildren(ObjMure* self, PlayState* play) {
    s32 maxChildren = ObjMure_GetMaxChildSpawns(self);
    s32 i;

    for (i = 0; i < maxChildren; i++) {
        if (self->children[i] != NULL) {
            if (self->childrenStates[i] == 0) {
                if (self->children[i]->update != NULL) {
                    if (self->children[i]->flags & 0x800) {
                        self->childrenStates[i] = 2;
                    }
                } else {
                    self->childrenStates[i] = 1;
                    self->children[i] = NULL;
                }
            } else if (self->childrenStates[i] == 2 && self->children[i]->update == NULL) {
                self->childrenStates[i] = 1;
                self->children[i] = NULL;
            }
        }
    }
}

void FUN_00352aa8(ObjMure* self, f32 randMax) {
    s32 index;
    s32 maxChildren = ObjMure_GetMaxChildSpawns(self);
    s32 i;

    for (i = 0; i < maxChildren; i++) {
        if (self->children[i] != NULL) {
            self->children[i]->child = NULL;
            if (Rand_ZeroOne() <= randMax) {
                index = Rand_ZeroOne() * (maxChildren - 0.5f);
                if (i != index) {
                    self->children[i]->child = self->children[index];
                }
            }
        }
    }
}

void FUN_00343f44(ObjMure* self, s32 idx1) {
    s32 maxChildren = ObjMure_GetMaxChildSpawns(self);
    s32 i;
    s32 i2;
    s32 j;

    for (i = 0, i2 = 0; i < maxChildren; i++) {
        if (self->children[i] != NULL) {
            if (i2 < idx1) {
                i2++;
                self->children[i]->child = self->children[i];
                for (j = 0; j < maxChildren; j++) {
                    if (i != j && self->children[j]->child == self->children[i]) {
                        self->children[j]->child = NULL;
                    }
                }
            } else if (self->children[i]->child == self->children[i]) {
                self->children[i]->child = NULL;
            }
        }
    }
}

void FUN_001a25b4(ObjMure* self, PlayState* play) {
    if (self->unk_1FC <= 0) {
        if (self->unk_1FE) {
            self->unk_1FE = false;
            FUN_00352aa8(self, (Rand_ZeroOne() * 0.5f) + 0.1f);
            if (self->actor.xzDistToPlayer < 60.0f) {
                self->unk_1FC = FRAMES((s16)(Rand_ZeroOne() * 5.5f) + 4);
            } else {
                self->unk_1FC = FRAMES((s16)(Rand_ZeroOne() * 40.5f) + 4);
            }
        } else {
            self->unk_1FE = true;
            if (self->actor.xzDistToPlayer < 60.0f) {
                self->unk_1FC = FRAMES((s16)(Rand_ZeroOne() * 10.5f) + 4);
                FUN_00352aa8(self, (Rand_ZeroOne() * 0.2f) + 0.8f);
            } else {
                self->unk_1FC = FRAMES((s16)(Rand_ZeroOne() * 10.5f) + 4);
                FUN_00352aa8(self, (Rand_ZeroOne() * 0.2f) + 0.6f);
            }
        }
    }
    if (self->actor.xzDistToPlayer < 120.0f) {
        self->unk_200++;
    } else {
        self->unk_200 = 0;
    }
    if (self->unk_200 >= (s32)FRAMES(80)) {
        FUN_00343f44(self, 1);
    } else {
        FUN_00343f44(self, 0);
    }
}

void FUN_0015e750(ObjMure* self, PlayState* play) {
    s32 maxChildren;
    s32 i;

    if (self->unk_1FC <= 0) {
        if (self->unk_1FE) {
            self->unk_1FE = false;
            FUN_00352aa8(self, Rand_ZeroOne() * 0.2f);
            if (self->actor.xzDistToPlayer < 60.0f) {
                self->unk_1FC = FRAMES((s16)(Rand_ZeroOne() * 5.5f) + 4);
            } else {
                self->unk_1FC = FRAMES((s16)(Rand_ZeroOne() * 40.5f) + 4);
            }
        } else {
            self->unk_1FE = true;
            FUN_00352aa8(self, Rand_ZeroOne() * 0.7f);
            self->unk_1FC = FRAMES((s16)(Rand_ZeroOne() * 10.5f) + 4);
        }
    }

    maxChildren = ObjMure_GetMaxChildSpawns(self);
    for (i = 0; i < maxChildren; i++) {
        Actor* child = self->children[i];
        if (child != NULL) {
            if (child->child != NULL && child->child->update == NULL) {
                child->child = NULL;
            }
        }
    }
}

void FUN_003cc298(ObjMure* self, PlayState* play) {
    ObjMure_CheckChildren(self, play);
    if (__fabsf(self->actor.unk_EC.z) >= sZClip[self->type] + 40.0f) {
        self->actionFunc = FUN_003bc5dc;
        self->actor.flags &= ~0x10;
        ObjMure_KillActors(self, play);
    } else if (sTypeGroupBehaviorFunc[self->type] != NULL) {
        sTypeGroupBehaviorFunc[self->type](self, play);
    }
}

void ObjMure_Update(Actor* actor, GameState* state) {
    ObjMure* self = (ObjMure*)actor;

    if (self->unk_1FC > 0) {
        self->unk_1FC--;
    }
    self->actionFunc(self, (PlayState*)state);
}
}
