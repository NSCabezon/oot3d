extern "C" {
#include "z_bg_bowl_wall.hpp"

#define FLAGS 0x80400030
#define BG_BOWL_WALL_TYPE_FIRST_WALL 0

void BgBowlWall_Init(Actor* actor, GameState* state);
void BgBowlWall_Destroy(Actor* actor, GameState* state);
void BgBowlWall_Update(Actor* actor, GameState* state);
void BgBowlWall_Draw(Actor* actor, GameState* state);
void FUN_00180e80(BgBowlWall* self, PlayState* play);

GLOBAL_ASM("data/z_Bg_Bowl_Wall.data.s")

ActorInit Bg_Bowl_Wall_InitVars = {
    ACTOR_BG_BOWL_WALL,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_BOWL,
    sizeof(BgBowlWall),
    (ActorFunc)BgBowlWall_Init,
    (ActorFunc)BgBowlWall_Destroy,
    (ActorFunc)BgBowlWall_Update,
    (ActorFunc)BgBowlWall_Draw,
};

void BgBowlWall_Init(Actor* actor, GameState* state) {
    BgBowlWall* self = (BgBowlWall*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;
    ZARInfo* zarInfo;
    s32 pad = 0;

    DynaPolyActor_Init((DynaPolyActor*)actor, 0);
    if (actor->params == 0) {
        zarInfo = FUN_00372f38(actor, play, &self->skelAnimModel, 0, 0);
        self->skelAnimModel->unk_0C->FUN_00372d94(zarInfo->GetCMABByIndex(0));
        self->skelAnimModel->unk_0C->SetAnimType(1);
        ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc((DynaPolyActor*)actor, state, 0), &colHeader);
    } else {
        zarInfo = FUN_00372f38(actor, play, &self->skelAnimModel, 1, 0);
        self->skelAnimModel->unk_0C->FUN_00372d94(zarInfo->GetCMABByIndex(1));
        self->skelAnimModel->unk_0C->SetAnimType(1);
        ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc((DynaPolyActor*)actor, state, 1), &colHeader);
    }
    self->bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    self->homePos = actor->world.pos;
    actor->scale.x = actor->scale.y = actor->scale.z = 1.0f;
    self->actionFunc = FUN_00180e80;
}

void BgBowlWall_Destroy(Actor* actor, GameState* state) {
    BgBowlWall* self = (BgBowlWall*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgBowlWall_Update(Actor* actor, GameState* state) {
    BgBowlWall* self = (BgBowlWall*)actor;

    if (self->timer != 0) {
        self->timer--;
    }
    self->actionFunc(self, (PlayState*)state);
}

void BgBowlWall_Draw(Actor* actor, GameState* state) {
    BgBowlWall* self = (BgBowlWall*)actor;
    MTX34 mtx;
    f32 speed;

    MTX34CopyAsm(&mtx, &actor->modelMtx);
    speed = 1.0f;
    if (PauseContext_GetState() != 0) {
        speed = 0.0f;
    }
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->unk_0C->SetAnimSpeed(speed);
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&mtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}

// Matched: InitImpl (3DS FUN_00180E80, was in functions_180000s.cpp).
// En_Wall_Tubo view: effCenterPos sits at +0x1AC in the 3DS build.
typedef struct {
    Actor actor;
    char unk_1A4[0x8];
    Vec3f effCenterPos;
} EnWallTuboTargetView;

void FUN_00180e80(BgBowlWall* self, PlayState* play) {
    s16 type = self->actor.params;
    Vec3f* targetPos;
    EnWallTuboTargetView* target;
    Actor* child;

    if (type != 0) {
        type += (s16)Rand_ZeroFloat(2.99f);
        {
            s16* sWallRots = (s16*)0x00510C14;

            self->actor.shape.rot.z = self->actor.world.rot.z = sWallRots[type];
        }
    }
    targetPos = &((Vec3f*)0x00510C1C)[type];
    self->effCenterPos.x = self->actor.world.pos.x + targetPos->x;
    self->effCenterPos.y = self->actor.world.pos.y + targetPos->y;
    self->effCenterPos.z = self->actor.world.pos.z + targetPos->z;

    target = (EnWallTuboTargetView*)Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, ACTOR_EN_WALL_TUBO,
                                                       self->effCenterPos.x, self->effCenterPos.y,
                                                       self->effCenterPos.z, 0, 0, 0, self->actor.params);
    if (target != NULL) {
        target->effCenterPos = self->effCenterPos;
        if (type != 0) {
            target->effCenterPos = self->effCenterPos = self->actor.world.pos;
        }
        if (self->bowlingGirl == NULL) {
            child = play->actorCtx.actorLists[ACTORCAT_NPC].head;
            while (child != NULL) {
                if (child->id != ACTOR_EN_BOM_BOWL_MAN) {
                    child = child->next;
                    continue;
                }
                self->bowlingGirl = child;
                break;
            }
        }
        self->actionFunc = (BgBowlWallActionFunc)0x003E699C; // BgBowlWall_WaitTargetHit (raw address, see debt)
    }
}

// ---- Unmatched action functions (drafts, NOT compiled) ----
// WaitTargetHit: 3DS code at 0x003E699C is the tail of FUN_003E68C4 (Bg_Bom_Guard's GLOBAL_ASM), not a standalone symbol.
// Explode / Disappear / WaitReappear: no clean function boundary in functions.tsv (Explode is raw DCB inside FUN_00107054).
#if 0
void BgBowlWall_WaitTargetHit(BgBowlWall* self, PlayState* play) {
    if (self->targetHit) {
        self->actionFunc = BgBowlWall_Explode;
    }
}

void BgBowlWall_Explode(BgBowlWall* self, PlayState* play) {
    s32 explode = false;
    Vec3f effAccel = { 0.0f, 0.1f, 0.0f };
    Vec3f effVel = { 0.0f, 0.0f, 0.0f };
    Vec3f effPos;
    s16 quakeIndex;
    s32 i;

    if (self->actor.params == BG_BOWL_WALL_TYPE_FIRST_WALL) {
        Math_SmoothStepToS(&self->actor.shape.rot.x, -0x3E80, 3, 0x1F4, 0);
        self->actor.world.rot.x = self->actor.shape.rot.x;
        if (self->actor.shape.rot.x < -0x3C1E) {
            explode = true;
        }
    } else {
        Math_ApproachF(&self->actor.world.pos.y, self->homePos.y - 450.0f, 0.3f, 10.0f);
        if (self->actor.world.pos.y < (self->homePos.y - 400.0f)) {
            explode = true;
        }
    }
    if (explode) {
        for (i = 0; i < 15; i++) {
            effPos.x = Rand_CenteredFloat(300.0f) + self->effCenterPos.x;
            effPos.y = -100.0f;
            effPos.z = Rand_CenteredFloat(400.0f) + self->effCenterPos.z;
            EffectSsBomb2_SpawnLayered(play, &effPos, &effVel, &effAccel, 100, 30);
            effPos.y = -50.0f;
            EffectSsHahen_SpawnBurst(play, &effPos, 10.0f, 0, 50, 15, 3, -1, 10, NULL);
            Actor_PlaySfx(&self->actor, NA_SE_IT_BOMB_EXPLOSION);
        }
        quakeIndex = Quake_Request(GET_ACTIVE_CAM(play), QUAKE_TYPE_1);
        Quake_SetSpeed(quakeIndex, 0x7FFF);
        Quake_SetPerturbations(quakeIndex, 300, 0, 0, 0);
        Quake_SetDuration(quakeIndex, 30);
        self->timer = 20;
        self->actionFunc = BgBowlWall_Disappear;
    }
}

void BgBowlWall_Disappear(BgBowlWall* self, PlayState* play) {
    if (self->timer >= 2) {
        if (self->actor.params == BG_BOWL_WALL_TYPE_FIRST_WALL) {
            Math_SmoothStepToS(&self->actor.shape.rot.x, -0x3E80, 1, 0xC8, 0);
        } else {
            Math_ApproachF(&self->actor.world.pos.y, self->homePos.y - 450.0f, 0.3f, 10.0f);
        }
    } else if (self->timer == 1) {
        self->actor.world.pos.y = self->homePos.y - 450.0f;
        self->actor.world.rot.x = self->actor.shape.rot.x = 0;
        self->bowlingGirl->wallsState[self->actor.params] = EN_BOM_BOWL_MAN_WALL_STATE_DISAPPEARED;
        self->actionFunc = BgBowlWall_WaitReappear;
    }
}

void BgBowlWall_WaitReappear(BgBowlWall* self, PlayState* play) {
    if (self->bowlingGirl->wallsState[self->actor.params] != EN_BOM_BOWL_MAN_WALL_STATE_DISAPPEARED) {
        Math_ApproachF(&self->actor.world.pos.y, self->homePos.y, 0.3f, 50.0f);
        if (fabsf(self->actor.world.pos.y - self->homePos.y) <= 10.0f) {
            self->actor.world.pos.y = self->homePos.y;
            self->targetHit = false;
            self->actionFunc = FUN_00180e80;
        }
    }
}
#endif

}
