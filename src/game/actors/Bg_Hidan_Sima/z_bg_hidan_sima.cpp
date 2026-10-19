extern "C" {
#include "z_bg_hidan_sima.hpp"

#define FLAGS 0x80400010

void BgHidanSima_Init(Actor* actor, GameState* state);
void BgHidanSima_Destroy(Actor* thisx, PlayState* play);
void BgHidanSima_Update(Actor* thisx, PlayState* play);
void FUN_003dfd44(BgHidanSima* self, PlayState* play);
void BgHidanSima_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Bg_Hidan_Sima.data.s")
GLOBAL_ASM("../src/game/actors/Bg_Hidan_Sima/data_syms.s")

ActorInit Bg_Hidan_Sima_InitVars = {
    ACTOR_BG_HIDAN_SIMA,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HIDAN_OBJECTS,
    sizeof(BgHidanSima),
    (ActorFunc)BgHidanSima_Init,
    (ActorFunc)BgHidanSima_Destroy,
    (ActorFunc)BgHidanSima_Update,
    (ActorFunc)BgHidanSima_Draw,
};

void FUN_0017a578(BgHidanSima* self, PlayState* play);
void FUN_003586ec(SkeletonAnimationModel_unk_0C* anim);
extern s32 gUnk_0054C8CC; // src/game/shared_syms.s
s32 FUN_0036adf4(DynaPolyActor* dyna); // DynaPolyActor_IsPlayerOnTop
void Collider_InitJntSph(PlayState* play, BgHidanSimaColliderJntSph* collider);
void Collider_SetJntSph(PlayState* play, BgHidanSimaColliderJntSph* collider, Actor* actor, void* init,
                        BgHidanSimaColliderElement* elements);
extern InitChainEntry BgHidanSima_sInitChain[];
extern u8 BgHidanSima_sJntSphInit[];

void BgHidanSima_Init(Actor* actor, GameState* state) {
    BgHidanSima* self = (BgHidanSima*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;
    ZARInfo* zarInfo;
    s32 i;

    Actor_ProcessInitChain(actor, BgHidanSima_sInitChain);
    DynaPolyActor_Init(&self->dyna, 1);
    actor->uncullZoneForward = 1400.0f;
    zarInfo = FUN_00372f38(actor, (PlayState*)state, &self->skelAnimModel[0], 7, &self->skelAnimModel[1], 7,
                                     &self->skelAnimModel[2], 7, &self->skelAnimModel[3], 7,
                                     &self->skelAnimModel[4], 0x17, &self->skelAnimModel[5], 0x13, 0);
    for (i = 0; i < 4; i++) {
        {
            SkeletonAnimationModel_unk_0C* anim0 = self->skelAnimModel[i]->unk_0C;
            anim0->FUN_00372d94(zarInfo->GetCMABByIndex(0));
        }
        self->skelAnimModel[i]->unk_0C->animType = 1;
        self->skelAnimModel[i]->unk_0C->animSpeed = 2.0f;
        {
            SkeletonAnimationModel_unk_0C* anim = self->skelAnimModel[i]->unk_0C;
            f32 frame = (i * 60) / 4;
            if (gUnk_0054C8CC == 0) {
                *(f32*)&anim->curFrame = frame;
                FUN_003586ec(anim);
            }
        }
    }
    if (actor->params == 0) {
        ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 0x13), &colHeader);
    } else {
        ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 0x11), &colHeader);
    }
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    Collider_InitJntSph(play, &self->collider);
    Collider_SetJntSph(play, &self->collider, actor, BgHidanSima_sJntSphInit,
                       (BgHidanSimaColliderElement*)self->unk_1E4);
    self->collider.elements[0].worldSphere.radius = self->collider.elements[0].modelSphere.radius;
    self->collider.elements[1].worldSphere.radius = self->collider.elements[1].modelSphere.radius;
    if (actor->params == 0) {
        self->actionFunc = FUN_0017a578;
    } else {
        self->actionFunc = (BgHidanSimaActionFunc)0x00159670;
    }
}

void Collider_DestroyJntSph(PlayState* play, void* collider);

void BgHidanSima_Destroy(Actor* thisx, PlayState* play) {
    BgHidanSima* self = (BgHidanSima*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyJntSph(play, &self->collider);
    FUN_00350f34(thisx, &self->skelAnimModel[0], &self->skelAnimModel[1], &self->skelAnimModel[2],
                           &self->skelAnimModel[3], &self->skelAnimModel[4], &self->skelAnimModel[5], 0);
}

s32 Player_InCsMode(PlayState* play);
void FUN_0017a578(BgHidanSima* self, PlayState* play);

void FUN_003eb728(BgHidanSima* self, PlayState* play);
void FUN_00128734(BgHidanSima* self, PlayState* play);
s16 Camera_GetCamDirYaw(Camera* camera);

// SinkingPlatform_Idle (ROM FUN_0017a578). Early-return form matches (fpmode=fast keeps the NaN-aware lt/ge).
void FUN_0017a578(BgHidanSima* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y, 3.4f);
    if (self->unk_29C != 0) {
        self->unk_29C--;
    }
    if (FUN_0036adf4(&self->dyna) && !(player->stateFlags1 & 0x6000) && self->unk_29C <= 0) {
        self->timer = 30;
        self->dyna.actor.world.rot.y = Camera_GetCamDirYaw(play->cameraPtrs[play->activeCamera]) + 0x4000;
        if (self->dyna.actor.world.pos.y >= self->dyna.actor.home.pos.y) {
            self->actionFunc = FUN_003eb728;
            return;
        }
        self->actionFunc = FUN_00128734;
    }
}

// SinkingPlatform_WarningShake (ROM FUN_003eb728). 3DS scales the shake angle by 2/3 with float rounding.
static s32 BgHidanSima_ScaleAngle(s32 angle) {
    s32 s;
    if (angle > 0) {
        s = (s32)(angle * 2.0f / 3.0f + 0.5f);
    } else {
        s = (s32)(angle * 2.0f / 3.0f - 0.5f);
    }
    return s;
}

void FUN_003eb728(BgHidanSima* self, PlayState* play) {
    if (!Player_InCsMode(play)) {
        if (self->timer != 0) {
            self->timer--;
        }
        if (self->timer != 0) {
            self->dyna.actor.world.pos.x =
                Math_SinS(BgHidanSima_ScaleAngle(self->dyna.actor.world.rot.y + (self->timer * 0x4000))) * 5.0f +
                self->dyna.actor.home.pos.x;
            self->dyna.actor.world.pos.z =
                Math_CosS(BgHidanSima_ScaleAngle(self->dyna.actor.world.rot.y + (self->timer * 0x4000))) * 5.0f +
                self->dyna.actor.home.pos.z;
        } else {
            self->dyna.actor.world.pos.x = self->dyna.actor.home.pos.x;
            self->dyna.actor.world.pos.z = self->dyna.actor.home.pos.z;
            self->actionFunc = FUN_00128734;
        }
        if (self->timer % 6 == 0) {
            Audio_PlayActorSound2(&self->dyna.actor, 0x0100018F);
        }
    }
}

// MovingPlatform_Move (ROM FUN_003dfd44). MovingPlatform_Idle is at raw 0x00159670 (inside an unlabeled cfg tail):
// until it is split into a real symbol this is a raw-address cast (debt).
f32 __hardfp_sinf(f32 x);

void FUN_003dfd44(BgHidanSima* self, PlayState* play) {
    f32 distanceFromHome;

    if (self->timer != 0) {
        self->timer--;
    }
    if (self->dyna.actor.world.rot.y != self->dyna.actor.home.rot.y) {
        distanceFromHome = (__hardfp_sinf((((90 - self->timer) * 0.011111111f) - 0.5f) * 3.1415927f * 2.0f * 0.33333334f) + 1.0f) * 200.0f;
    } else {
        distanceFromHome = (__hardfp_sinf(((self->timer * 0.011111111f) - 0.5f) * 3.1415927f * 2.0f * 0.33333334f) + 1.0f) * -200.0f;
    }
    self->dyna.actor.world.pos.x = distanceFromHome * Math_SinS(self->dyna.actor.world.rot.y) + self->dyna.actor.home.pos.x;
    self->dyna.actor.world.pos.z = distanceFromHome * Math_CosS(self->dyna.actor.world.rot.y) + self->dyna.actor.home.pos.z;
    if (self->timer == 0) {
        self->timer = 30;
        self->actionFunc = (BgHidanSimaActionFunc)0x00159670;
    }
    FUN_00373264(&self->dyna.actor, 0x0100018A);
}


// SinkingPlatform_Sink (ROM FUN_00128734): sinks 100 units below home, then goes back to Idle.
void FUN_00128734(BgHidanSima* self, PlayState* play) {
    if (!Player_InCsMode(play)) {
        if (FUN_0036adf4(&self->dyna)) {
            self->timer = 30;
        } else if (self->timer != 0) {
            self->timer--;
        }
        Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y - 105.0f, 1.7f);
        if (self->timer == 0) {
            self->unk_29C = 12;
            self->actionFunc = FUN_0017a578;
        }
    }
}

f32 __hardfp_cosf(f32 x);

// Inlined in ROM (N64 BgHidanSima_UpdateFireCollider).
static void BgHidanSima_UpdateFireCollider(BgHidanSima* self) {
    f32 cos = Math_CosS(self->dyna.actor.world.rot.y + 0x8000);
    f32 sin = Math_SinS(self->dyna.actor.world.rot.y + 0x8000);
    BgHidanSimaColliderElement* elem;

    elem = &self->collider.elements[0];
    elem->worldSphere.center.x = self->dyna.actor.world.pos.x + sin * elem->modelSphere.center.z;
    elem->worldSphere.center.y = self->dyna.actor.world.pos.y + elem->modelSphere.center.y;
    elem->worldSphere.center.z = self->dyna.actor.world.pos.z + cos * elem->modelSphere.center.z;

    elem = &self->collider.elements[1];
    elem->worldSphere.center.x = self->dyna.actor.world.pos.x + sin * elem->modelSphere.center.z;
    elem->worldSphere.center.y = self->dyna.actor.world.pos.y + elem->modelSphere.center.y;
    elem->worldSphere.center.z = self->dyna.actor.world.pos.z + cos * elem->modelSphere.center.z;
}

void BgHidanSima_Update(Actor* thisx, PlayState* play) {
    BgHidanSima* self = (BgHidanSima*)thisx;
    s32 temp;

    self->actionFunc(self, play);
    if (self->dyna.actor.params != 0) {
        temp = (self->dyna.actor.world.rot.y == self->dyna.actor.shape.rot.y) ? self->timer : (self->timer + 120);
        if (self->actionFunc == FUN_003dfd44) {
            temp += 30;
        }
        self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y - ((1.0f - __hardfp_cosf(temp * (M_PI / 20.0f))) * 5.0f);
        if (self->actionFunc == FUN_003dfd44) {
            BgHidanSima_UpdateFireCollider(self);
            CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
        }
    }
}

GLOBAL_ASM("asm/BgHidanSima_Draw.s")
}
