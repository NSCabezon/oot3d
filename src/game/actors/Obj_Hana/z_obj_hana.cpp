extern "C" {
#include "z_obj_hana.hpp"

#define FLAGS 0x80000000

// EVENTCHKINF_OBTAINED_ZELDAS_LETTER: gSaveContext.eventChkInf[4], bit 0x1
#define EVENTCHKINF_OBTAINED_ZELDAS_LETTER_SET (gSaveContext.eventChkInf[4] & 0x1)

void ObjHana_Init(Actor* actor, GameState* state);
void ObjHana_Destroy(Actor* actor, GameState* state);
void ObjHana_Update(Actor* actor, GameState* state);
void ObjHana_Draw(Actor* actor, GameState* state);

ActorInit Obj_Hana_InitVars = {
    ACTOR_OBJ_HANA,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_FIELD_KEEP,
    sizeof(ObjHana),
    (ActorFunc)ObjHana_Init,
    (ActorFunc)ObjHana_Destroy,
    (ActorFunc)ObjHana_Update,
    (ActorFunc)ObjHana_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 8, 10, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit sColChkInfoInit = { 0, 12, 60, MASS_IMMOVABLE };

typedef struct HanaParams {
    /* 0x00 */ s32 modelIdx;
    /* 0x04 */ f32 scale;
    /* 0x08 */ f32 yOffset;
    /* 0x0C */ s16 radius;
    /* 0x0E */ s16 height;
} HanaParams; // size = 0x10

static HanaParams sHanaParams[] = {
    { 13, 0.01f, 0.0f, -1, 0 },
    { 5, 0.1f, 58.0f, 10, 18 },
    { 10, 0.4f, 0.0f, 12, 44 },
};

static u32 sInitChain[] = {
    0x000A0A93, // ICHAIN_VEC3F_DIV1000(scale, 10, ICHAIN_CONTINUE)
    0x03841F8D, // ICHAIN_F32(cullingVolumeDistance, 900, ICHAIN_CONTINUE)
    0x003C200D, // ICHAIN_F32(cullingVolumeScale, 60, ICHAIN_CONTINUE)
    0x0320208C, // ICHAIN_F32(cullingVolumeDownward, 800, ICHAIN_STOP)
};

void ObjHana_Init(Actor* actor, GameState* state) {
    ObjHana* self = (ObjHana*)actor;
    PlayState* play = (PlayState*)state;
    s16 type = actor->params & 3;
    HanaParams* params = &sHanaParams[type];

    FUN_00372f38(actor, play, &self->skelAnimModel, params->modelIdx, 0);
    Actor_ProcessInitChain(actor, (InitChainEntry*)sInitChain);
    Actor_SetScale(actor, params->scale);
    actor->shape.yOffset = params->yOffset;
    if (params->radius >= 0) {
        Collider_InitCylinder(play, &self->collider);
        Collider_SetCylinder(play, &self->collider, actor, &sCylinderInit);
        Collider_UpdateCylinder(actor, &self->collider);
        self->collider.dim.radius = params->radius;
        self->collider.dim.height = params->height;
        CollisionCheck_SetInfo(&actor->colChkInfo, NULL, &sColChkInfoInit);
    }

    if (type == 2 && EVENTCHKINF_OBTAINED_ZELDAS_LETTER_SET) {
        Actor_Kill(actor);
    }
}

void ObjHana_Destroy(Actor* actor, GameState* state) {
    ObjHana* self = (ObjHana*)actor;
    PlayState* play = (PlayState*)state;

    if (sHanaParams[actor->params & 3].radius >= 0) {
        Collider_DestroyCylinder(play, &self->collider);
    }
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void ObjHana_Update(Actor* actor, GameState* state) {
    ObjHana* self = (ObjHana*)actor;
    PlayState* play = (PlayState*)state;

    if (sHanaParams[actor->params & 3].radius >= 0 && actor->xzDistToPlayer < 400.0f) {
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    }
}

void ObjHana_Draw(Actor* actor, GameState* state) {
    ObjHana* self = (ObjHana*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}
}
