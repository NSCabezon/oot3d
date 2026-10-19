extern "C" {
#include "z_item_etcetera.hpp"

#define FLAGS 0x00000010

void ItemEtcetera_Init(Actor* actor, GameState* state);
void ItemEtcetera_Destroy(Actor* actor, GameState* state);
void ItemEtcetera_Update(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Item_Etcetera.data.s")

ActorInit Item_Etcetera_InitVars = {
    ACTOR_ITEM_ETCETERA,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(ItemEtcetera),
    (ActorFunc)ItemEtcetera_Init,
    (ActorFunc)ItemEtcetera_Destroy,
    (ActorFunc)ItemEtcetera_Update,
    NULL,
};

GLOBAL_ASM("asm/ItemEtcetera_Init.s")

// Model at +0x1B0: real virtual call, slot 1. _ZTI17ItemEtceteraModel is defined as an EQU in syms.s so armlink VFE
// (--vfemode=force, --no_rtti_data) knows the call class; else the call matches every vtable and keeps Class_4EBE18 D0 -> _ZdlPv -> libheap.
extern "C++" {
struct ItemEtceteraModel {
    virtual void unk_vf0();
    virtual void Destroy();
};
}

struct ItemEtceteraLayout {
    /* 0x000 */ char pad_000[0x1B0];
    /* 0x1B0 */ ItemEtceteraModel* model;
};

void ItemEtcetera_Destroy(Actor* actor, GameState* state) {
    ItemEtceteraLayout* self = (ItemEtceteraLayout*)actor;

    if (self->model != NULL) {
        self->model->Destroy();
    }
    self->model = NULL;
}

void ItemEtcetera_Update(Actor* actor, GameState* state) {
    ItemEtcetera* self = (ItemEtcetera*)actor;
    ((void (*)(Actor*, GameState*))(*(void**)((u8*)self + 0x1AC)))(actor, state);
}

void ItemEtcetera_Draw(Actor* actor, GameState* state) {
    ItemEtcetera* self = (ItemEtcetera*)actor;

    if (*(SkeletonAnimationModel**)((u8*)self + 0x1B0) != NULL) {
        (*(SkeletonAnimationModel**)((u8*)self + 0x1B0))->FUN_003721e0((MTX34*)((u8*)self + 0x148));
        (*(SkeletonAnimationModel**)((u8*)self + 0x1B0))->SetUnkAC(1);
        (*(SkeletonAnimationModel**)((u8*)self + 0x1B0))->FUN_00372170(0);
    }
}

void ItemEtcetera_DrawThroughLens(Actor* actor, GameState* state) {
    ItemEtcetera* self = (ItemEtcetera*)actor;

    if (*(u8*)((u8*)state + 0x208F) != 0 && *(SkeletonAnimationModel**)((u8*)self + 0x1B0) != NULL) {
        (*(SkeletonAnimationModel**)((u8*)self + 0x1B0))->FUN_003721e0((MTX34*)((u8*)self + 0x148));
        (*(SkeletonAnimationModel**)((u8*)self + 0x1B0))->SetUnkAC(1);
        (*(SkeletonAnimationModel**)((u8*)self + 0x1B0))->FUN_00372170(0);
    }
}

// Guarded function-local statics: guards (0x531DD8 vel / 0x531DD4 accel) and colors (0x531DDC) emulated with EQU syms (syms.s);
// velocity/accel are real TU .bss statics (pinned at 0x5A2ABC in data/oot3d.ld) so armcc forms &velocity as &accel-0xc at the call.
GLOBAL_ASM("../src/game/actors/Item_Etcetera/syms.s")

int __cxa_guard_acquire(unsigned int* guard_object);
void __cxa_guard_release(unsigned int* guard_object);
f32 Rand_ZeroOne(void);
void EffectSsKiraKira_SpawnDispersed(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor,
                                     Color_RGBA8* envColor, s32 scale, s32 life);

extern unsigned int ItemEtcetera_sData[]; // [2] = velocity guard
extern unsigned int ItemEtcetera_sGuardAcc;
extern Color_RGBA8 ItemEtcetera_sColors[2];
extern "C++" {
struct ItemEtceteraVec3f : Vec3f {
    ItemEtceteraVec3f() {}
    ItemEtceteraVec3f(f32 x_, f32 y_, f32 z_) { x = x_; y = y_; z = z_; }
};
}
static Vec3f ItemEtcetera_sVelocity;
static Vec3f ItemEtcetera_sAccel;
inline void* operator new(unsigned int, void* p) { return p; }

static inline void ItemEtcetera_Vec3fSet(Vec3f* v, f32 x, f32 y, f32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

void ItemEtcetera_MoveFireArrowDown(Actor* thisx, PlayState* play) {
    ItemEtcetera* self = (ItemEtcetera*)thisx;

    Actor_UpdateBgCheckInfo(play, &self->actor, 10.0f, 10.0f, 0.0f, 5);
    Actor_MoveForward(&self->actor);
    if (!(self->actor.bgCheckFlags & 1)) {
        Vec3f pos;
        if (!(ItemEtcetera_sData[2] & 1) && __cxa_guard_acquire(&ItemEtcetera_sData[2])) {
            ItemEtcetera_Vec3fSet(&ItemEtcetera_sVelocity, 0.0f, 0.2f, 0.0f);
            __cxa_guard_release(&ItemEtcetera_sData[2]);
        }
        if (!(ItemEtcetera_sData[1] & 1) && __cxa_guard_acquire(&ItemEtcetera_sGuardAcc)) {
            new (&ItemEtcetera_sAccel) ItemEtceteraVec3f(0.0f, 0.05f, 0.0f);
            __cxa_guard_release(&ItemEtcetera_sGuardAcc);
        }
        ItemEtcetera_sVelocity.x = Rand_CenteredFloat(3.0f);
        ItemEtcetera_sVelocity.z = Rand_CenteredFloat(3.0f);
        ItemEtcetera_sVelocity.y = -0.05f;
        ItemEtcetera_sAccel.y = -0.025f;
        pos.x = Rand_CenteredFloat(12.0f) + self->actor.world.pos.x;
        pos.y = Rand_ZeroOne() * 6.0f + self->actor.world.pos.y;
        pos.z = Rand_CenteredFloat(12.0f) + self->actor.world.pos.z;
        EffectSsKiraKira_SpawnDispersed(play, &pos, &ItemEtcetera_sVelocity, &ItemEtcetera_sAccel,
                                        &ItemEtcetera_sColors[0], &ItemEtcetera_sColors[1], 5000, 16);
    }
    self->actor.shape.rot.y += 0x400;
    if (Actor_HasParent(&self->actor, play)) {
        if ((self->actor.params & 0xFF) == 1) {
            gSaveContext.eventChkInf[3] |= 2;
            Flags_SetSwitch(play, 0xB);
        }
        Actor_Kill(&self->actor);
    } else {
        GiveItem(&self->actor, play, *(s16*)((u8*)self + 0x1B6), 30.0f, 50.0f);
    }
}

GLOBAL_ASM("asm/ItemEtcetera_UpdateFireArrow.s")

}
