extern "C" {
#include "z_en_blkobj.hpp"

#define FLAGS 0x00400030
#define PLAYER_STATE2_26 (1 << 26)
#define ACTOR_FLAG_INSIDE_CULLING_VOLUME 0x40

void FUN_00340e14(void);
typedef ZARInfo* (*EnBlkobjLoadModelsFunc)(Actor*, PlayState*, s32, SkeletonAnimationModel**, s32,
                                           SkeletonAnimationModel**, s32, s32);

void EnBlkobj_DoNothing(EnBlkobj* self, PlayState* play);
void EnBlkobj_Wait(EnBlkobj* self, PlayState* play);
void EnBlkobj_SpawnDarkLink(EnBlkobj* self, PlayState* play);
void EnBlkobj_DarkLinkFight(EnBlkobj* self, PlayState* play);

static inline void EnBlkobj_SetupAction(EnBlkobj* self, EnBlkobjActionFunc actionFunc) {
    self->actionFunc = actionFunc;
    self->timer = 0;
}

void EnBlkobj_Init(Actor* thisx, GameState* state);
void EnBlkobj_Destroy(Actor* thisx, GameState* state);
void EnBlkobj_Update(EnBlkobj* self, PlayState* play);
void EnBlkobj_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_Blkobj.data.s")

ActorInit En_Blkobj_InitVars = {
    ACTOR_EN_BLKOBJ,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_BLKOBJ,
    sizeof(EnBlkobj),
    (ActorFunc)EnBlkobj_Init,
    (ActorFunc)EnBlkobj_Destroy,
    (ActorFunc)EnBlkobj_Update,
    (ActorFunc)EnBlkobj_Draw,
};

void EnBlkobj_Init(Actor* thisx, GameState* state) {
    EnBlkobj* self = (EnBlkobj*)thisx;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;
    ZARInfo* zar;

    Actor_ProcessInitChain(thisx, (InitChainEntry*)0x00520F50);
    zar = ((EnBlkobjLoadModelsFunc)FUN_00340e14)(thisx, play, 0, &self->skel[0], 0, &self->skel[1], 1, 0);
    {
        void* cmab0 = zar->GetCMABByIndex(0);
        SkeletonAnimationModel* m0 = self->skel[0];
        m0->unk_0C->FUN_00372d94(cmab0);
    }
    self->skel[0]->unk_0C->SetAnimType(1);
    EnBlkobj* s2 = self;
    s2->skel[1]->unk_0C->FUN_00372d94(zar->GetCMABByIndex(1));
    self->skel[1]->unk_0C->SetAnimType(1);
    DynaPolyActor_Init((DynaPolyActor*)thisx, 0);
    if (Flags_GetClear(play, thisx->room)) {
        self->alpha = 255;
        EnBlkobj_SetupAction(self, EnBlkobj_DoNothing);
        *(u8*)((u8*)play + 0x3237) = 0;
    } else {
        ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc((DynaPolyActor*)thisx, state, 0), &colHeader);
        self->bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
        EnBlkobj_SetupAction(self, EnBlkobj_Wait);
    }
}

void EnBlkobj_Destroy(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnBlkobj* self = (EnBlkobj*)thisx;
    Player* player;

    DynaPoly_DeleteBgActor((PlayState*)state, &play->colCtx.dyna, self->bgId);
    FUN_00350f34(thisx, &self->skel[0], &self->skel[1], 0);
    if (*(u8*)((u8*)play + 0x3237) == 0) {
        *(u8*)((u8*)play + 0x3237) = 0xFF;
    }
    player = GET_PLAYER(play);
    if (player != NULL) {
        player->stateFlags2 &= ~PLAYER_STATE2_26;
    }
}

void EnBlkobj_Update(EnBlkobj* self, PlayState* play) {
    self->actionFunc(self, play);
}

GLOBAL_ASM("asm/EnBlkobj_Draw.s")

void EnBlkobj_DarkLinkFight(EnBlkobj* self, PlayState* play) {
    s32 alphaMod;

    if (self->timer == 0) {
        if (Actor_Find(&play->actorCtx, ACTOR_EN_TORCH2, ACTORCAT_BOSS) == NULL) {
            Flags_SetClear(play, self->actor.room);
            self->timer++;
        }
    } else if (self->timer++ > 1) {
        alphaMod = (self->timer - 1) >> 2;
        if (alphaMod > 5) {
            alphaMod = 5;
        }
        self->alpha += alphaMod;
        if (self->alpha > 255) {
            self->alpha = 255;
            EnBlkobj_SetupAction(self, EnBlkobj_DoNothing);
            DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->bgId);
            *(u8*)((u8*)play + 0x3237) = 0;
        }
    }
}

void EnBlkobj_DoNothing(EnBlkobj* self, PlayState* play) {
}

void EnBlkobj_SpawnDarkLink(EnBlkobj* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    player->stateFlags2 &= ~PLAYER_STATE2_26;
    if (!(self->actor.flags & ACTOR_FLAG_INSIDE_CULLING_VOLUME)) {
        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_TORCH2, self->actor.world.pos.x, self->actor.world.pos.y,
                    self->actor.world.pos.z, 0, self->actor.yawTowardsPlayer, 0, 0, 1);
        EnBlkobj_SetupAction(self, EnBlkobj_DarkLinkFight);
    }
}

void EnBlkobj_Wait(EnBlkobj* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (self->actor.xzDistToPlayer < 120.0f) {
        EnBlkobj_SetupAction(self, EnBlkobj_SpawnDarkLink);
    }
    player->stateFlags2 |= PLAYER_STATE2_26;
}

}
