extern "C" {
#include "z_boss_sst.hpp"

#define FLAGS 0x00000435

void BossSst_Init(Actor* actor, GameState* state);
void BossSst_Destroy(Actor* actor, GameState* state);
void BossSst_Update(Actor* actor, GameState* state);
void BossSst_Draw(Actor* actor, GameState* state);

void* FUN_00352e80(void* model);
s32 FUN_003731e0(SkelAnime* skelAnime);
void FUN_003672b8(void* arg);
void Animation_MorphToLoop(SkelAnime* skelAnime, s32 idx, f32 morph);
void Animation_MorphToPlayOnce(SkelAnime* skelAnime, s32 idx, f32 morph);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void FUN_00119704(BossSst* self, PlayState* play);
void FUN_00153a50(BossSst* self, PlayState* play);
void FUN_0021bb64(BossSst* self, PlayState* play);
void FUN_001c5754(Actor* actor, PlayState* play);
void FUN_0011956c(BossSst* self, PlayState* play);
void FUN_00204184(BossSst* self, PlayState* play);
void FUN_0023877c(BossSst* self, PlayState* play);
void FUN_001966b8(BossSst* self, PlayState* play);
void FUN_0021b740(BossSst* self, PlayState* play);
void FUN_00196554(BossSst* self, PlayState* play);
void FUN_00119704(BossSst* self, PlayState* play);
void FUN_001c5754(Actor* actor, PlayState* play);
void Matrix_RotateY(MTX34* mtx, s32 mode, f32 angle);
void Matrix_UpdatePosition(MTX34* dst, MTX34* src, Vec3f* translate);
u32 Math_StepToS(s16* value, s16 target, s16 step);
u32 Math_ScaledStepToS(s16* value, s16 target, s16 scale);
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
extern s32 BossSst_AnimationTable[];
extern s32 BossSst_AnimationTable2[];
extern s32 BossSst_HandAnimationTable[];
extern s32 BossSst_AnimationTableA[];
extern s32 BossSst_AnimationTableB[];
extern s32 BossSst_AnimationTableC[];
extern void* BossSst_HandPointers[];
extern BossSst* BossSst_HeadPointer;
extern char BossSst_AssetBase[];

struct BossSstUnknown11AView {
    char prefix[0x11A];
    s16 unk_11A;
};

struct BossSstUnknownEDCView {
    char prefix[0xEDC];
    f32 unk_EDC;
};

struct BossSstModelView {
    char unk_00[0x16];
    u8 flags16;
    char unk_17[0x31F];
    u8 flags336;
};

struct BossSstElementView {
    char prefix[0x3C];
    char skeletonAnimationModel[0x98];
};

typedef char BossSstElementViewSizeCheck[(sizeof(BossSstElementView) == 0xD4) ? 1 : -1];

void* FUN_003fd784(BossSstElementView* element) {
    return (char*)FUN_00352e80(element->skeletonAnimationModel) - 0x3C;
}

#if 0
void FUN_001c5754(Actor* thisx, PlayState* play) {
    BossSst* self = (BossSst*)thisx;
    if (FUN_003731e0(&self->skelAnime)) {
        if (self->skelAnime.animationType != 0x18) {
            Animation_MorphToLoop(&self->skelAnime, 0x18, -5.0f);
        }
        self->actionFunc = FUN_00119704;
    }
}
#endif

#if 0
void FUN_0023877c(BossSst* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    char* assetBase = (char*)BossSst_AssetBase;
    BossSst* head = *(BossSst**)(assetBase + 0x30);
    f32 targetZ = self->actor.home.pos.z + Math_CosS(head->actor.shape.rot.y) * 200.0f;
    Math_ApproachF(&self->actor.home.pos.z, targetZ, 25.0f, 0.5f);
    f32 targetX = self->actor.home.pos.x + Math_SinS(head->actor.shape.rot.y) * 200.0f;
    Math_ApproachF(&self->actor.home.pos.x, targetX, 25.0f, 0.5f);
    if (self->ready == 0) {
        Math_ScaledStepToS(&self->unk_23C, 0, 0x800);
        Math_StepToS(&self->unk_23A, -3500, 0x1F4);
        Math_ScaledStepToS(&self->actor.shape.rot.x, self->actor.home.rot.x, 0x800);
        Math_ScaledStepToS(&self->actor.shape.rot.z, self->actor.home.rot.z, 0x800);
        Math_ScaledStepToS(&self->actor.shape.rot.y, self->actor.home.rot.y, 0x800);
        if (head->actionFunc == FUN_00204184) {
            self->ready = 1;
            Animation_MorphToPlayOnce(&self->skelAnime, BossSst_AnimationTable2[self->actor.params], 10.0f);
        }
    } else {
        Math_StepToF(&self->actor.world.pos.y, self->actor.floorHeight, 30.0f);
    }
}
#endif

#if 1
void FUN_00153e3c(BossSst* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if (self->timer != 0) {
        self->timer--;
    }
    s32 timer = self->timer;
    if ((timer / 4) * 4 == timer) {
        if ((self->timer & 7) != 0) {
            Animation_MorphToPlayOnce(&self->skelAnime, BossSst_AnimationTableA[self->actor.params], 4.0f);
        } else {
            Animation_MorphToPlayOnce(&self->skelAnime, BossSst_AnimationTableB[self->actor.params], 6.0f);
        }
    }
    ((BossSstUnknown11AView*)self)->unk_11A = 0x12C;
    f32 jitter = Rand_CenteredFloat(20.0f);
    f32 oldPos = self->actor.world.pos.x;
    self->actor.world.pos.x = jitter + oldPos;
    jitter = Rand_CenteredFloat(20.0f);
    oldPos = self->actor.world.pos.y;
    self->actor.world.pos.y = jitter + oldPos;
    jitter = Rand_CenteredFloat(20.0f);
    oldPos = self->actor.world.pos.z;
    self->actor.world.pos.z = jitter + oldPos;
    if (self->actor.world.pos.y < self->actor.floorHeight + 100.0f) {
        Math_StepToF(&self->actor.world.pos.y, self->actor.floorHeight + 100.0f, 20.0f);
    }
    if (self->timer == 0) {
        Animation_MorphToPlayOnce(&self->skelAnime, BossSst_AnimationTableC[self->actor.params], 8.0f);
        self->actionFunc = FUN_0021bb64;
    }
}
#endif

void FUN_0023872c(BossSst* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if (self->ready != 0) {
        Animation_MorphToPlayOnce(&self->skelAnime, 0x13, -5.0f);
        self->unk_128D &= ~1;
        self->actionFunc = (void (*)(BossSst*, PlayState*))FUN_001c5754;
    }
}

void FUN_0019677c(BossSst* self, PlayState* play) {
    Math_SmoothStepToF(&self->actor.world.pos.y, 250.0f, 0.5f, 70.0f, 5.0f);
    if (FUN_003731e0(&self->skelAnime) && !self->ready) {
        Animation_MorphToPlayOnce(&self->skelAnime, BossSst_AnimationTable[self->actor.params], 10.0f);
        self->ready = 1;
    }
    FUN_00373264(&self->actor, 0x010003D7);
}

#if 0
void FUN_0021b6c4(Actor* thisx, PlayState* play) {
    BossSst* self = (BossSst*)thisx;
    FUN_003731e0(&self->skelAnime);
    s32 reached = 1 - Math_SmoothStepToS(&self->actor.shape.rot.y, self->targetYaw, 4, 0x800, 0x100);
    if (reached < 0) {
        reached = 0;
    }
    if (reached != 0) {
        FUN_003672b8(BossSst_HandPointers[0]);
        FUN_003672b8(BossSst_HandPointers[1]);
        self->timer = 0xC5;
        self->ready = 0;
        self->actionFunc = FUN_0011956c;
    }
}
#endif

#if 0
s32 FUN_00287bd0(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    BossSst* self = (BossSst*)thisx;
    if (limbIndex == 0) {
        Matrix_RotateY(mtx, 1, self->unk_23C * 0.0000958738f);
        Vec3f translate = {(f32)self->unk_23A, 0.0f, 0.0f};
        Matrix_UpdatePosition(mtx, mtx, &translate);
    }
    return 0;
}
#endif

void FUN_002aa090(BossSst* self, PlayState* play) {
    Animation_MorphToPlayOnce(&self->skelAnime, BossSst_AnimationTable2[self->actor.params], 10.0f);
    if (self->actionFunc != FUN_001966b8) {
        self->ready = 0;
    }
    self->unk_EFC &= ~3;
    self->unk_EFD |= 1;
    self->unk_EFD &= ~2;
    self->unk_F00 = 0xC;
    self->unk_EFD |= 4;
    Actor_SetColorFilter(&self->actor, 0, 0xFF, 0, Animation_GetLastFrame(&self->skelAnime, 0x12));
    self->actionFunc = FUN_0023877c;
}

#if 0
void FUN_0021b740(BossSst* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    BossSst* left = (BossSst*)BossSst_HandPointers[0];
    BossSst* right = (BossSst*)BossSst_HandPointers[1];
    if (left->ready && left->actionFunc == FUN_00196554 && right->ready && right->actionFunc == FUN_00196554) {
        s32 endFrame = Animation_GetLastFrame(&self->skelAnime, 0x11);
        Animation_Change_00375c08(&self->skelAnime, 0x11, 0.0f, 0.0f, (f32)endFrame, 3, -5.0f);
        left->unk_EFC |= 1;
        right->unk_EFC |= 1;
        for (s32 i = 0; i < 5; i++) {
            ((u8*)left->unk_F08)[5 + i * 0xA0] = 0x20;
            ((u8*)right->unk_F08)[5 + i * 0xA0] = 0x20;
        }
        self->unk_EFC |= 1;
        self->actor.speedXZ = -5.0f;
        self->ready = 0;
        self->actionFunc = (void (*)(BossSst*, PlayState*))FUN_00153a50;
    } else {
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->targetYaw, 4, 0x800, 0x400);
    }
}
#endif

#if 0
void FUN_001966b8(BossSst* self, PlayState* play) {
    if (self->timer != 0) {
        self->timer--;
    }
    FUN_003731e0(&self->skelAnime);
    if (self->timer > 1) {
        self->actor.shape.rot.x -= 0x200;
        Math_StepToF(&self->actor.world.pos.y, self->actor.floorHeight + 200.0f, 50.0f);
    } else {
        self->actor.shape.rot.x += 0x400;
        Math_StepToF(&self->actor.world.pos.y, self->actor.floorHeight, 100.0f);
    }
    if (self->timer == 0) {
        if (self->actor.floorHeight >= 100.0f) {
            Audio_PlayActorSound2(&self->actor, 0x010003C1);
        }
        ((void (*)(BossSst*))FUN_002aa090)(self);
    }
}
#endif

#if 0
void FUN_00196404(Actor* thisx, PlayState* play) {
    BossSst* self = (BossSst*)thisx;
    if (!FUN_003731e0(&self->skelAnime)) {
        return;
    }

    void** hands = BossSst_HandPointers;
    s32* handAnimations = (s32*)((char*)hands - 0x50);
    BossSst* left = (BossSst*)hands[0];
    BossSst* right = (BossSst*)hands[1];
    s32 leftAnim = handAnimations[left->actor.params];
    if (leftAnim == 8 && handAnimations[right->actor.params] == 8) {
        Animation_MorphToLoop(&self->skelAnime, 0x19, -5.0f);
        self->unk_128D |= 1;
        self->actor.speedXZ = -5.0f;
        self->actionFunc = FUN_0021b740;
        return;
    }
    if (leftAnim == 9 || handAnimations[right->actor.params] == 9) {
        Animation_MorphToLoop(&self->skelAnime, 0x19, -5.0f);
        self->unk_128D |= 1;
        self->ready = 0;
        self->actionFunc = FUN_0023872c;
        return;
    }
    if (self->skelAnime.animationType == 0x15) {
        Animation_MorphToPlayOnce(&self->skelAnime, 0x13, -5.0f);
        self->unk_128D &= ~1;
        self->actionFunc = (void (*)(BossSst*, PlayState*))FUN_001c5754;
        return;
    } else {
        if (self->skelAnime.animationType != 0x18) {
            Animation_MorphToLoop(&self->skelAnime, 0x18, -5.0f);
        }
        self->actionFunc = FUN_00119704;
    }
}
#endif

#if 0
void FUN_001c5b78(BossSst* self, PlayState* play) {
    if (self->timer != 0) {
        self->timer--;
    }
    if (FUN_003731e0(&self->skelAnime)) {
        Animation_MorphToLoop(&self->skelAnime, 0x10, -5.0f);
        self->actor.speedXZ = 5.0f;
        self->unk_128D |= 1;
        self->unk_129C = 0x0FC00702;
        ((BossSstModelView*)self->unk_F08)->flags336 |= 5;
        ((BossSstModelView*)self->unk_F08)->flags16 &= ~1;
        if (self->actionFunc != FUN_001c5b78) {
            self->timer = 0x71;
        }
        self->actionFunc = FUN_00204184;
    }
}
#endif

GLOBAL_ASM("data/z_Boss_Sst.data_00516174.s")
GLOBAL_ASM("data/z_Boss_Sst.data_00516400.s")
GLOBAL_ASM("../src/game/actors/Boss_Sst/data_syms.s")

ActorInit Boss_Sst_InitVars = {
    ACTOR_BOSS_SST,
    ACTORCAT_BOSS,
    FLAGS,
    OBJECT_SST,
    sizeof(BossSst),
    (ActorFunc)BossSst_Init,
    (ActorFunc)BossSst_Destroy,
    (ActorFunc)BossSst_Update,
    (ActorFunc)BossSst_Draw,
};

GLOBAL_ASM("asm/BossSst_Init.s")

GLOBAL_ASM("asm/BossSst_Destroy.s")

GLOBAL_ASM("asm/BossSst_Update.s")

GLOBAL_ASM("asm/BossSst_Draw.s")

GLOBAL_ASM("asm/FUN_00354014.s")

}
