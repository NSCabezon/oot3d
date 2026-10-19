extern "C" {
#include "z_actor_1d7.hpp"

#define FLAGS 0x80000000

void Actor1D7_Init(Actor* actor, GameState* state);
void Actor1D7_Destroy(Actor* actor, GameState* state);
static __inline __pure float Actor1D7_FabsfZ(float f, float z) {
    return (f < z) ? -f : f;
}

void Actor1D7_Update(Actor* actor, GameState* state);
void Actor1D7_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Actor_1D7.data.s")

GLOBAL_ASM("../src/game/actors/Actor_1D7/data_syms.s")

// Tables live in the data dump above (no scatter symbols) -> absolute EQU symbols in data_syms.s.
extern f32 Actor1D7_sConsts[];
extern f32 Actor1D7_sAngles[];
extern InitChainEntry Actor1D7_sInitChain[];

ActorInit Actor_1D7_InitVars = {
    ACTOR_1D7,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_YDAN_OBJECTS,
    sizeof(Actor1D7),
    (ActorFunc)Actor1D7_Init,
    (ActorFunc)Actor1D7_Destroy,
    (ActorFunc)Actor1D7_Update,
    (ActorFunc)Actor1D7_Draw,
};

void Actor1D7_Init(Actor* actor, GameState* state) {
    Actor1D7* self = (Actor1D7*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    self->unk_1C2 = 0;
    self->hasBgActor = 0;
    Actor_ProcessInitChain(actor, Actor1D7_sInitChain);
    s16 params = actor->params;
    s32 swch = (*(u32*)((u8*)&gSaveContext + 0x1000 + 0x360) & (1 << params)) != 0;
    if (swch != 0) {
        Actor_Kill(actor);
        return;
    }
    FUN_00372f38(actor, play, &self->skelAnimModel, 0xB, 0);
    DynaPolyActor_Init(&self->dyna, 0);
    self->hasBgActor = 1;
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 5), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    self->timer = 0;
}

void Actor1D7_Destroy(Actor* actor, GameState* state) {
    Actor1D7* self = (Actor1D7*)actor;
    PlayState* play = (PlayState*)state;

    if (self->hasBgActor) {
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    }
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

GLOBAL_ASM("asm/Actor1D7_Update.s")

#if 0 // Update draft, 33/161 words off (stage 3: finish-block order fixed; left: float reg numbering + loop radius mul reassociated to (scale*c3)*325 vs ROM scale*(c3*325))
void FUN_0037bb4c(Actor* actor, GameState* state);

static __inline __pure float Actor1D7_Fabsf(float f) {
    return (f < 0.0f) ? -f : f;
}

#define ACTOR1D7_ANGLE_TBL Actor1D7_sAngles

void Actor1D7_Update(Actor* actor, GameState* state) {
    Actor1D7* self = (Actor1D7*)actor;
    PlayState* play = (PlayState*)state;
    Vec3f pos = actor->world.pos;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 distSq;
    f32 angle;
    f32 radius;
    f32 baseRadius;
    Actor* other;
    u8* player;

    if (self->timer != 0) {
        return;
    }
    player = (u8*)GET_PLAYER(play);
    f32 maxDy = 400.0f;
    f32 twoPi = 6.2831854f;
    f32 ringWidth = 40.0f;
    f32 zero = 0.0f;
    if (*(player + 0x1A9) == 6 && *(u16*)(player + 0x2248) != 0) {
        Vec3f* pp = (Vec3f*)(player + 0x22A8);

        dx = pp->x - pos.x;
        dy = pp->y - pos.y;
        dz = pp->z - pos.z;
        distSq = dx * dx + dz * dz;
        angle = Math_FAtan2F(dz, dx);
        if (angle < zero) {
            angle += twoPi;
        }
        radius = 288.0f;
        if (!(radius * radius < distSq)) {
            if (!((radius - ringWidth) * (radius - ringWidth) > distSq)) {
                if (!(dy < zero)) {
                    if (!(dy > maxDy)) {
                        if (Actor1D7_FabsfZ(angle - ACTOR1D7_ANGLE_TBL[actor->params], zero) < 0.75f) {
                            self->unk_1C4 = *(f32*)(player + 0x22AC);
                            self->unk_1C8 = angle;
                            goto done;
                        }
                    }
                }
            }
        }
    }

    baseRadius = Actor1D7_sConsts[0] * ringWidth;
    maxDy = Actor1D7_sConsts[1] * ringWidth;
    for (other = play->actorCtx.actorLists[ACTORCAT_ITEMACTION].head; other != NULL; other = other->next) {
        if (other->id != 0x9F) {
            continue;
        }
        dx = other->world.pos.x - pos.x;
        dy = other->world.pos.y - pos.y;
        dz = other->world.pos.z - pos.z;
        radius = baseRadius + other->scale.x * (Actor1D7_sConsts[3] * 325.0f);
        distSq = dx * dx + dz * dz;
        angle = Math_FAtan2F(dz, dx);
        if (angle < zero) {
            angle += twoPi;
        }
        if (distSq <= radius * radius && distSq >= (radius - ringWidth) * (radius - ringWidth) && dy >= zero &&
            dy <= maxDy && Actor1D7_FabsfZ(angle - ACTOR1D7_ANGLE_TBL[actor->params], zero) < 1.2f) {
            self->unk_1C4 = other->world.pos.y + Actor1D7_sConsts[4];
            self->unk_1C8 = angle;
            goto done;
        }
    }
    return;
done:
    self->timer = 60;
    self->unk_1CC = zero;
    actor->update = (ActorFunc)FUN_0037bb4c;
}
#endif

void Actor1D7_Draw(Actor* actor, GameState* state) {
    Actor1D7* self = (Actor1D7*)actor;
    MTX34 modelMtx;
    SkeletonAnimationModel* model;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    model = (self->unk_1C2 == 0) ? self->skelAnimModel : NULL;
    if (model != NULL) {
        model->SetUnkAC(1);
        model->FUN_003721e0(&modelMtx);
        model->FUN_00372170(0);
    }
}
}
