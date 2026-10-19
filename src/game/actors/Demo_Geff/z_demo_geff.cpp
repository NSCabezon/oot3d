extern "C" {
#include "z_demo_geff.hpp"

#define FLAGS 0x00000030

void DemoGeff_Init(Actor* actor, GameState* state);
void DemoGeff_Destroy(Actor* actor, GameState* state);
void DemoGeff_Update(Actor* actor, GameState* state);
void DemoGeff_Draw(Actor* actor, GameState* state);

s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
s32 Object_IsLoaded(ObjectContext* objectCtx, s32 bankIndex);

void caseD_0_0020c520(DemoGeff* self, PlayState* play); // N64: func_809783D4 (+ func_80978370 inlined)
void caseD_1_00224b24(Actor* actor, GameState* state); // N64: func_80978344
void FUN_002a0c50(Actor* actor, GameState* state);     // N64: func_80978030

// 0x0020C51C (N64 func_809784D4, empty) and 0x002A0B64 (N64 func_80978308) have no symbol of their own:
// their bytes are embedded at the end of FUN_0020c1fc / FUN_002a0808 in the disassembly.
static DemoGeffFunc sActionFuncs[] = {
    caseD_0_0020c520,
    (DemoGeffFunc)0x002A0B64,
};

static DemoGeffFunc sDrawFuncs[] = {
    (DemoGeffFunc)0x0020C51C,
    (DemoGeffFunc)caseD_1_00224b24,
};

static s16 sObjectIds[] = {
    OBJECT_GEFF, OBJECT_GEFF, OBJECT_GEFF, OBJECT_GEFF, OBJECT_GEFF, OBJECT_GEFF, OBJECT_GEFF, OBJECT_GEFF, OBJECT_GEFF,
};

static DemoGeffFunc sInitFuncs[] = {
    (DemoGeffFunc)FUN_002a0c50, (DemoGeffFunc)FUN_002a0c50, (DemoGeffFunc)FUN_002a0c50,
    (DemoGeffFunc)FUN_002a0c50, (DemoGeffFunc)FUN_002a0c50, (DemoGeffFunc)FUN_002a0c50,
    (DemoGeffFunc)FUN_002a0c50, (DemoGeffFunc)FUN_002a0c50, (DemoGeffFunc)FUN_002a0c50,
};

ActorInit Demo_Geff_InitVars = {
    ACTOR_DEMO_GEFF,
    ACTORCAT_BOSS,
    FLAGS,
    OBJECT_GEFF,
    sizeof(DemoGeff),
    (ActorFunc)DemoGeff_Init,
    (ActorFunc)DemoGeff_Destroy,
    (ActorFunc)DemoGeff_Update,
    (ActorFunc)DemoGeff_Draw,
};

void DemoGeff_Init(Actor* actor, GameState* state) {
    DemoGeff* self = (DemoGeff*)actor;
    PlayState* play = (PlayState*)state;

    if (self->actor.params < 0 || self->actor.params >= 9) {
        Actor_Kill(&self->actor);
        return;
    }
    self->action = 0;
    self->drawConfig = 0;
    FUN_00372f38(&self->actor, play, &self->skelAnimModel, 0, 0);
}

void DemoGeff_Destroy(Actor* actor, GameState* state) {
    DemoGeff* self = (DemoGeff*)actor;

    if (self->skelAnimModel != NULL) {
        FUN_00350f34(&self->actor, &self->skelAnimModel, 0);
    }
}

void FUN_002a0c50(Actor* actor, GameState* state) {
    DemoGeff* self = (DemoGeff*)actor;
    Vec3f* scale = &self->actor.scale;

    self->action = 1;
    self->drawConfig = 1;

    switch (actor->params) {
        case 0:
        case 3:
        case 6:
            scale->x = (kREG(7) * 0.01f) + 0.3f;
            scale->y = (kREG(8) * 0.01f) + 0.3f;
            scale->z = (kREG(9) * 0.01f) + 0.3f;
            break;
        case 1:
        case 4:
        case 7:
            scale->x = (kREG(10) * 0.01f) + 0.15f;
            scale->y = (kREG(11) * 0.01f) + 0.29f;
            scale->z = (kREG(12) * 0.01f) + 0.12f;
            break;
        default:
            scale->x = (kREG(13) * 0.01f) + 0.1f;
            scale->y = (kREG(14) * 0.01f) + 0.15f;
            scale->z = (kREG(15) * 0.01f) + 0.2f;
            break;
    }
}

void caseD_1_00224b24(Actor* actor, GameState* state) {
    DemoGeff* self = (DemoGeff*)actor;

    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&actor->modelMtx);
    self->skelAnimModel->FUN_00372170(0);
}

void caseD_0_0020c520(DemoGeff* self, PlayState* play) {
    ObjectContext* objCtx = &play->objectCtx;
    s32 objectSlot = Object_GetIndex(objCtx, sObjectIds[self->actor.params]);
    DemoGeffFunc initFunc;

    if (objectSlot < 0) {
        Actor_Kill(&self->actor);
        return;
    }
    if (Object_IsLoaded(objCtx, objectSlot)) {
        self->objectSlot = objectSlot;
        initFunc = sInitFuncs[self->actor.params];
        if (initFunc == NULL) {
            Actor_Kill(&self->actor);
            return;
        }
        initFunc(self, play);
    }
}

void DemoGeff_Update(Actor* actor, GameState* state) {
    DemoGeff* self = (DemoGeff*)actor;
    PlayState* play = (PlayState*)state;

    if (self->action < 0 || self->action >= 2 || sActionFuncs[self->action] == NULL) {
        return;
    }
    sActionFuncs[self->action](self, play);
}

void DemoGeff_Draw(Actor* actor, GameState* state) {
    DemoGeff* self = (DemoGeff*)actor;
    PlayState* play = (PlayState*)state;
    s32 drawConfig = self->drawConfig;

    if (drawConfig < 0 || drawConfig >= 2 || sDrawFuncs[drawConfig] == NULL) {
        return;
    }
    sDrawFuncs[drawConfig](self, play);
}
}
