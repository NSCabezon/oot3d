extern "C" {
#include "z_bg_spot07_taki.hpp"

#define FLAGS 0x80400030

void BgSpot07Taki_Init(Actor* actor, GameState* state);
void BgSpot07Taki_Destroy(Actor* actor, GameState* state);
void BgSpot07Taki_Update(Actor* actor, GameState* state);
void BgSpot07Taki_Draw(Actor* actor, GameState* state);
void BgSpot07Taki_DoNothing(BgSpot07Taki* self, PlayState* play);

void FUN_003586ec(void* arg);

ActorInit Bg_Spot07_Taki_InitVars = {
    ACTOR_BG_SPOT07_TAKI,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SPOT07_OBJECT,
    sizeof(BgSpot07Taki),
    (ActorFunc)BgSpot07Taki_Init,
    (ActorFunc)BgSpot07Taki_Destroy,
    (ActorFunc)BgSpot07Taki_Update,
    (ActorFunc)BgSpot07Taki_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

// Per-play float cache at PlayState+0x7C00+0x380 (3 floats). Raw offset until PlayState is filled in.
#define SPOT07_FRAME(play, i) (*(f32*)((u8*)(play) + 0x7C00 + 0x380 + (i) * 4))
// Unknown global at 0x0054C8CC (read by many functions).
extern s32 gUnk_0054C8CC;
#define SPOT07_G_54C8CC gUnk_0054C8CC

void BgSpot07Taki_Destroy(Actor* actor, GameState* state) {
    BgSpot07Taki* self = (BgSpot07Taki*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->bgId);
    FUN_00350f34(actor, &self->skelAnimModel[0], &self->skelAnimModel[1], &self->skelAnimModel[2],
                 &self->skelAnimModel[3], &self->skelAnimModel[4], &self->skelAnimModel[5], 0);
}

void BgSpot07Taki_Update(Actor* actor, GameState* state) {
    BgSpot07Taki* self = (BgSpot07Taki*)actor;

    self->actionFunc(self, (PlayState*)state);
}

#define T7_DRAW(m) \
    self->skelAnimModel[m]->SetUnkAC(1);          \
    self->skelAnimModel[m]->FUN_003721e0(&mtx);   \
    self->skelAnimModel[m]->FUN_00372170(0)

void BgSpot07Taki_Draw(Actor* actor, GameState* state) {
    BgSpot07Taki* self = (BgSpot07Taki*)actor;
    PlayState* play = (PlayState*)state;
    f32 speed;
    MTX34 mtx;

    MTX34CopyAsm(&mtx, &actor->modelMtx);
    speed = 1.0f;
    if (PauseContext_GetState() != 0) {
        speed = 0.0f;
    }
    {
        SkeletonAnimationModel_unk_0C* anim0 = self->skelAnimModel[0]->unk_0C;
        SPOT07_FRAME(play, 0) = *(f32*)&anim0->curFrame;
    }
    SPOT07_FRAME(play, 1) = *(f32*)&self->skelAnimModel[1]->unk_0C->curFrame;

    if (LINK_IS_ADULT) {
        if (actor->params == 0) {
            T7_DRAW(2);
        } else {
            T7_DRAW(4);
        }
    }
    if (!LINK_IS_ADULT) {
        if (actor->params == 0) {
            self->skelAnimModel[0]->unk_0C->SetAnimSpeed(speed);
            T7_DRAW(0);
        } else {
            self->skelAnimModel[1]->unk_0C->SetAnimSpeed(speed);
            T7_DRAW(1);
        }
        return;
    }
    if (actor->params == 0) {
        T7_DRAW(3);
    } else {
        self->skelAnimModel[5]->unk_0C->SetAnimSpeed(speed);
        T7_DRAW(5);
    }
}

void BgSpot07Taki_DoNothing(BgSpot07Taki* self, PlayState* play) {
}

// Mixed GetCMABByIndex shapes: calls 0 and 2 take the plain r2-hoisted shape via the helper, call 1 needs the scoped
// cmab/m0 locals (SetupAnimB). With this split the frame floats also get s16/s17/s18 like the ROM.
static inline void Spot07_SetupAnim(SkeletonAnimationModel** pm, ZARInfo* zar, s32 i, f32 frame) {
    (*pm)->unk_0C->FUN_00372d94(zar->GetCMABByIndex(i));
    (*pm)->unk_0C->SetAnimType(1);
    {
        SkeletonAnimationModel_unk_0C* p = (*pm)->unk_0C;
        if (SPOT07_G_54C8CC == 0) {
            *(f32*)&p->curFrame = frame;
            FUN_003586ec(p);
        }
    }
}
static inline void Spot07_SetupAnimB(SkeletonAnimationModel** pm, ZARInfo* zar, s32 i, f32 frame) {
    {
        void* cmab = zar->GetCMABByIndex(i);
        SkeletonAnimationModel* m0 = *pm;
        m0->unk_0C->FUN_00372d94(cmab);
    }
    (*pm)->unk_0C->SetAnimType(1);
    {
        SkeletonAnimationModel_unk_0C* p = (*pm)->unk_0C;
        if (SPOT07_G_54C8CC == 0) {
            *(f32*)&p->curFrame = frame;
            FUN_003586ec(p);
        }
    }
}
void BgSpot07Taki_Init(Actor* thisx, GameState* state) {
    BgSpot07Taki* self = (BgSpot07Taki*)thisx;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;
    ZARInfo* zarInfo;
    f32 frame0;
    f32 frame1;
    f32 frame2;

    DynaPolyActor_Init((DynaPolyActor*)self, 1);
    Actor_ProcessInitChain(&self->actor, sInitChain);
    zarInfo = FUN_00372f38(&self->actor, play, &self->skelAnimModel[0], 0, &self->skelAnimModel[1], 1,
                           &self->skelAnimModel[2], 2, &self->skelAnimModel[3], 3,
                           &self->skelAnimModel[4], 4, &self->skelAnimModel[5], 5, 0);
    frame0 = SPOT07_FRAME(play, 0);
    frame1 = SPOT07_FRAME(play, 1);
    frame2 = SPOT07_FRAME(play, 2);
    Spot07_SetupAnim(&self->skelAnimModel[0], zarInfo, 0, frame1);
    Spot07_SetupAnimB(&self->skelAnimModel[1], zarInfo, 1, frame0);
    self->skelAnimModel[5]->unk_0C->FUN_00372d94(zarInfo->GetCMABByIndex(2));
    self->skelAnimModel[5]->unk_0C->SetAnimType(1);
    {
        SkeletonAnimationModel_unk_0C* p = self->skelAnimModel[5]->unk_0C;
        if (SPOT07_G_54C8CC == 0) {
            *(f32*)&p->curFrame = frame2;
            FUN_003586ec(p);
        }
    }

    if (LINK_IS_ADULT) {
        if (self->actor.params == 0) {
            ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc((DynaPolyActor*)self, state, 0), &colHeader);
        } else {
            ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc((DynaPolyActor*)self, state, 1), &colHeader);
        }
        self->bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->actor, colHeader);
    }
    self->actionFunc = BgSpot07Taki_DoNothing;
}

}
