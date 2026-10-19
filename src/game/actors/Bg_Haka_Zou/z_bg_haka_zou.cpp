extern "C" {
#include "z_bg_haka_zou.hpp"

#define FLAGS 0x00400010

void BgHakaZou_Init(Actor* actor, GameState* state);
void BgHakaZou_Destroy(BgHakaZou* self, PlayState* play);
void BgHakaZou_Update(BgHakaZou* self, PlayState* play);

void FUN_001546ec(BgHakaZou* self, PlayState* play); // N64: BgHakaZou_WaitForObject (defined below)
void FUN_0025e314(Actor* actor, PlayState* play);    // N64: BgHakaZou_Draw (defined below)
void FUN_00109fc4(BgHakaZou* self, PlayState* play);    // N64: BgHakaZou_CrumbleSkullWall (still asm)
void FUN_003e5ad8(BgHakaZou* self, PlayState* play);    // N64: BgHakaZou_WaitForHit (still asm)

s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
s32 Object_IsLoaded(ObjectContext* objectCtx, s32 bankIndex);
s32 Flags_GetSwitch(PlayState* play, s32 flag);
s16 Rand_S16Offset(s16 base, s16 range);
void GraphicsContext_UnknownStub(GraphicsContext* gfxCtx);

GLOBAL_ASM("data/z_Bg_Haka_Zou.data.s")

ActorInit Bg_Haka_Zou_InitVars = {
    ACTOR_BG_HAKA_ZOU,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(BgHakaZou),
    (ActorFunc)BgHakaZou_Init,
    (ActorFunc)BgHakaZou_Destroy,
    (ActorFunc)BgHakaZou_Update,
    NULL,
};

void BgHakaZou_Init(Actor* actor, GameState* state) {
    BgHakaZou* self = (BgHakaZou*)actor;
    PlayState* play = (PlayState*)state;

    // sInitChain (ICHAIN gravity/scale) is in the data dump at 0x00511570
    Actor_ProcessInitChain(actor, (InitChainEntry*)0x00511570);

    self->switchFlag = (actor->params >> 8) & 0xFF;
    actor->params &= 0xFF;

    if (actor->params == BGHAKAZOU_TYPE_SKULL_WALL_RUBBLE) {
        Actor_SetScale(actor, (Rand_ZeroOne() * 0.005f) + 0.025f);

        actor->speedXZ = Rand_ZeroOne();
        actor->world.rot.y = actor->shape.rot.y * ((Rand_ZeroOne() < 0.5f) ? -1 : 1) + Rand_CenteredFloat(0x1000);
        self->timer = 10;
        actor->world.rot.x = Rand_S16Offset(0x100, 0x300) * ((Rand_ZeroOne() < 0.5f) ? -1 : 1);
        actor->world.rot.z = Rand_S16Offset(0x400, 0x800) * ((Rand_ZeroOne() < 0.5f) ? -1 : 1);
    } else {
        Collider_InitCylinder(play, &self->collider);
        // sCylinderInit is in the data dump at 0x00511578
        Collider_SetCylinder(play, &self->collider, actor, (ColliderCylinderInit*)0x00511578);
        Collider_UpdateCylinder(actor, &self->collider);

        DynaPolyActor_Init(&self->dyna, 0);

        if (actor->params == BGHAKAZOU_TYPE_GIANT_BIRD_STATUE) {
            actor->uncullZoneScale = 3000.0f;
            actor->uncullZoneDownward = 3000.0f;
            actor->uncullZoneForward = 2000.0f;
        }
    }

    if (actor->params == BGHAKAZOU_TYPE_BOMBABLE_RUBBLE) {
        self->requiredObjectSlot = Object_GetIndex(&play->objectCtx, 0x8D);
    } else {
        self->requiredObjectSlot = Object_GetIndex(&play->objectCtx, 0x69);
    }

    if (self->requiredObjectSlot < 0) {
        Actor_Kill(actor);
    } else if (actor->params != BGHAKAZOU_TYPE_SKULL_WALL_RUBBLE && Flags_GetSwitch(play, self->switchFlag)) {
        if (actor->params != BGHAKAZOU_TYPE_GIANT_BIRD_STATUE) {
            Actor_Kill(actor);
        } else {
            actor->world.pos.z -= 80.0f;
            actor->world.pos.y -= 40 + 14;
            actor->shape.rot.x = -0x4000;
        }
    }

    if (actor->params == BGHAKAZOU_TYPE_BOMBABLE_RUBBLE) {
        actor->unk_19B = 2;
    }

    self->actionFunc = (BgHakaZouActionFunc)FUN_001546ec;
}

// N64: BgHakaZou_Draw (3DS: draws the skeleton model). Name pinned to the ld symbol FUN_0025e314.
void FUN_0025e314(Actor* actor, PlayState* play) {
    BgHakaZou* self = (BgHakaZou*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}

// N64: BgHakaZou_WaitForObject. Name pinned to the ld symbol FUN_001546ec.
void FUN_001546ec(BgHakaZou* self, PlayState* play) {
    CollisionHeader* colHeader;

    if (Object_IsLoaded(&play->objectCtx, self->requiredObjectSlot)) {
        self->dyna.actor.objBankIndex = self->requiredObjectSlot;
        self->dyna.actor.draw = (ActorFunc)FUN_0025e314;

        if (self->dyna.actor.params == BGHAKAZOU_TYPE_SKULL_WALL_RUBBLE) {
            FUN_00372f38(&self->dyna.actor, play, &self->skelAnimModel, 0xC, NULL);
            self->actionFunc = FUN_00109fc4;
        } else {
            // N64: Actor_SetObjectDependency(play, actor). Empty on 3DS (see Bg_Haka_Megane).
            ((void (*)(PlayState*, Actor*))GraphicsContext_UnknownStub)(play, &self->dyna.actor);
            colHeader = NULL;

            if (self->dyna.actor.params == BGHAKAZOU_TYPE_GIANT_BIRD_STATUE) {
                FUN_00372f38(&self->dyna.actor, play, &self->skelAnimModel, 0x1F, NULL);
                ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 8), &colHeader);
                self->collider.dim.radius = 80;
                self->collider.dim.height = 100;
                self->collider.dim.yShift = -30;
                self->collider.dim.pos.x -= 56;
                self->collider.dim.pos.z += 56;
                self->dyna.actor.uncullZoneScale = 1500.0f;
            } else if (self->dyna.actor.params == BGHAKAZOU_TYPE_BOMBABLE_SKULL_WALL) {
                FUN_00372f38(&self->dyna.actor, play, &self->skelAnimModel, 0xC, NULL);
                ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 7), &colHeader);
                self->collider.dim.yShift = -50;
            } else {
                FUN_00372f38(&self->dyna.actor, play, &self->skelAnimModel, 2, NULL);
                ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 1), &colHeader);
                self->collider.dim.radius = 55;
                self->collider.dim.height = 20;
            }

            self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);

            self->actionFunc =
                (self->dyna.actor.params == BGHAKAZOU_TYPE_GIANT_BIRD_STATUE && Flags_GetSwitch(play, self->switchFlag))
                    ? (BgHakaZouActionFunc)0x003E1A70 // N64: BgHakaZou_DoNothing (unlabeled)
                    : FUN_003e5ad8;
        }
    }
}

// N64: BgHakaZou_CrumbleSkullWall. Name pinned to the ld symbol FUN_00109fc4.
void FUN_00109fc4(BgHakaZou* self, PlayState* play) {
    if (self->timer != 0) {
        self->timer--;
    }

    self->dyna.actor.shape.rot.x += self->dyna.actor.world.rot.x;
    self->dyna.actor.shape.rot.z += self->dyna.actor.world.rot.z;

    if (self->dyna.actor.bgCheckFlags & 2) { // BGCHECKFLAG_GROUND_TOUCH
        if (self->dyna.actor.velocity.y < -8.0f) {
            u16 flags = self->dyna.actor.bgCheckFlags;

            self->dyna.actor.velocity.y *= -0.6f;
            self->dyna.actor.velocity.y = CLAMP_MAX(self->dyna.actor.velocity.y, 10.0f);
            flags &= ~3; // BGCHECKFLAG_GROUND | BGCHECKFLAG_GROUND_TOUCH
            self->dyna.actor.speedXZ = 2.0f;
            self->dyna.actor.bgCheckFlags = flags;
        } else {
            Actor_Kill(&self->dyna.actor);
        }
    }

    if (self->timer == 0) {
        Actor_Kill(&self->dyna.actor);
    }
}

void BgHakaZou_Destroy(BgHakaZou* self, PlayState* play) {
    if (self->dyna.actor.params != BGHAKAZOU_TYPE_SKULL_WALL_RUBBLE) {
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
        Collider_DestroyCylinder(play, &self->collider);
    }
    FUN_00350f34(&self->dyna.actor, &self->skelAnimModel, 0);
}

void BgHakaZou_Update(BgHakaZou* self, PlayState* play) {
    self->actionFunc(self, play);

    if (self->dyna.actor.params == BGHAKAZOU_TYPE_SKULL_WALL_RUBBLE) {
        Actor_MoveForward(&self->dyna.actor);
    }
}
// NOT MATCHED: FUN_003e5ad8 (BgHakaZou_WaitForHit, 752 B) stays GLOBAL_ASM in functions_*.cpp.
// Draft below (N64 logic, unverified). Helpers it calls (SpawnSkullWallRubble, SpawnRubbleParticles,
// BirdStatueAnim_Explode) are not drafted; the 3DS build inlines/renames them, so the draft is a sketch.
#if 0
#define AC_HIT 0x02
#define GIANT_BIRD_STATUE_TIMER 20

void BgHakaZou_IdleKill(BgHakaZou* self, PlayState* play) {
    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer == 0) {
        Actor_Kill(&self->dyna.actor);
    }
}

void BgHakaZou_WaitForHit(BgHakaZou* self, PlayState* play) {
    if (self->collider.base.acFlags & AC_HIT) {
        Flags_SetSwitch(play, self->switchFlag);

        if (self->dyna.actor.params == BGHAKAZOU_TYPE_GIANT_BIRD_STATUE) {
            self->timer = 20;
            self->actionFunc = BgHakaZou_BirdStatueAnim_Explode; // unlabeled on 3DS (FUN_*)
            OnePointCutscene_Init(play, 3400, 999, &self->dyna.actor, 0 /* CAM_ID_MAIN */);
        } else if (self->dyna.actor.params == BGHAKAZOU_TYPE_BOMBABLE_RUBBLE) {
            BgHakaZou_SpawnRubbleParticles(self, play);
            self->dyna.actor.draw = NULL;
            self->timer = 1;
            Actor_PlaySfx(&self->dyna.actor, NA_SE_EV_EXPLOSION);
            self->actionFunc = BgHakaZou_IdleKill;
        } else {
            BgHakaZou_SpawnSkullWallRubble(self, play);
            self->timer = 1;
            Actor_PlaySfx(&self->dyna.actor, NA_SE_EV_WALL_BROKEN);
            self->actionFunc = BgHakaZou_IdleKill;
        }
    } else {
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
    }
}
#endif
}
